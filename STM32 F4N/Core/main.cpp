/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Core                                                          */
/*   Role: Application Entry Point                                         */
/*   Filename: main.cpp                                                    */
/*   Date: 22th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "main.hpp"
#include "boot.hpp"

#include "QAD_GPIO.hpp"
#include "QAD_EXTI.hpp"
#include "QAD_Timer.hpp"
#include "QAD_Encoder.hpp"
#include "QAD_PWM.hpp"
#include "QAD_ADC.hpp"
#include "QAD_I2C.hpp"

#include "QAS_Serial_Dev_UART.hpp"

#include <string.h>
#include <stdio.h>


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//User LED driver class (driver defined in QAD_GPIO.hpp)
QAD_GPIO_Output* GPIO_UserLED;


//STLink UART system class (system defined in QAS_Serial_Dev_UART.hpp)
QAS_Serial_Dev_UART* UART_STLink;


//I2C Test Driver
QAD_I2C* I2C_ADXL345;



//Task Timing
//
//These constants are used to determine the update rate (in milliseconds) of each of the
//tasks that are run in the processing loop within the main() function.
//
const uint32_t QA_FT_HeartbeatTickThreshold = 500;  //Time in milliseconds between heartbeat LED updates
                                                    //The rate of flashing of the heartbeat LED will be double the value defined here


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//main
//Application entry Point
//
//This is the first C++ function to be called when the microcontroller starts up
//This function is called by the assembly language in startup_stm32f411retx.s
int main(void) {

	//---------------------
	//---------------------
  //System Initialization


	//Calls the SystemInitialize method located within boot.hpp, which initializes the
	//CPU caches, as well as the PLLs, system, bus and some peripheral clocks.
	//All GPIOs are also enabled by this method.
	if (SystemInitialize()) {
		while (1) {}
	}


	//----------------------------------
	//Initialize the User LED using the QAD_GPIO_Output driver class.
	//QAD_USERLED_GPIO_PORT and QAD_USERLED_GPIO_PIN are defined in setup.hpp
	GPIO_UserLED = new QAD_GPIO_Output(QAD_USERLED_GPIO_PORT, QAD_USERLED_GPIO_PIN);


	//----------------------------------
	//Initialize UART2, which is used to communicate over serial via the STLINK connection
	//Baudrates, IRQ Priorities, GPIO details and FIFO sizes are defined in setup.hpp

	//Fill out initialization structure required to create UART class (defined in QAS_Serial_Dev_UART.hpp)
  QAS_Serial_Dev_UART_InitStruct sSerialInit;
  sSerialInit.sUART_Init.uart        = QAD_UART2;                //Define the UART peripheral to be used (enum defined in QAD_UART.hpp)
  sSerialInit.sUART_Init.baudrate    = QAD_UART2_BAUDRATE;       //Define the baudrate to be used by the UART peripheral
  sSerialInit.sUART_Init.irqpriority = QAD_IRQPRIORITY_UART2;    //Defined the IRQ priority to be used by the TX and RX interrupts
  sSerialInit.sUART_Init.txgpio      = QAD_UART2_TX_PORT;        //Define the GPIO port for the TX pin
  sSerialInit.sUART_Init.txpin       = QAD_UART2_TX_PIN;         //Define the pin number for the TX pin
  sSerialInit.sUART_Init.txaf        = QAD_UART2_TX_AF;          //Define the alternate function for the TX pin
  sSerialInit.sUART_Init.rxgpio      = QAD_UART2_RX_PORT;        //Define the GPIO port for the RX pin
  sSerialInit.sUART_Init.rxpin       = QAD_UART2_RX_PIN;         //Define the pin number for the RX pin
  sSerialInit.sUART_Init.rxaf        = QAD_UART2_RX_AF;          //Define the alternate function for the TX pin
  sSerialInit.uTXFIFO_Size           = QAD_UART2_TX_FIFOSIZE;    //Define the size (in bytes) for the transmit FIFO
  sSerialInit.uRXFIFO_Size           = QAD_UART2_RX_FIFOSIZE;    //Define the size (in bytes) for the receive FIFO

  //Create the UART class, passing to it a reference to the initialization structure
  UART_STLink = new QAS_Serial_Dev_UART(sSerialInit);

  //If initialization failed then turn on user LED and jump into infinite loop
  if (UART_STLink->init(NULL) == QA_Fail) {
  	GPIO_UserLED->on();
    while (1) {}
  }

  //If initialization succeeded then output a message via serial
  UART_STLink->txCR();
  UART_STLink->txStringCR("STM32F411 Nucleo64 Booting...");



  //--------------
  //--------------
  //Initialize I2C

  /*GPIO_InitTypeDef GPIO_Init = {0};

    //SCL
  GPIO_Init.Pin           = GPIO_PIN_10;
  GPIO_Init.Mode          = GPIO_MODE_AF_OD;
  GPIO_Init.Pull          = GPIO_PULLUP;
  GPIO_Init.Speed         = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init.Alternate     = GPIO_AF4_I2C2;
  HAL_GPIO_Init(GPIOB, &GPIO_Init);

    //SDA
  GPIO_Init.Pin           = GPIO_PIN_3;
  GPIO_Init.Mode          = GPIO_MODE_AF_OD;
  GPIO_Init.Pull          = GPIO_PULLUP;
  GPIO_Init.Speed         = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init.Alternate     = GPIO_AF9_I2C2;
  HAL_GPIO_Init(GPIOB, &GPIO_Init);


    //Enable I2C Clock
	__HAL_RCC_I2C2_CLK_ENABLE();
  __HAL_RCC_I2C2_FORCE_RESET();
  __HAL_RCC_I2C2_RELEASE_RESET();


    //Init I2C Peripheral
  I2C_HandleTypeDef I2C_Handle = {0};
  I2C_Handle.Instance                = I2C2;
  I2C_Handle.Init.ClockSpeed         = 100000;
  I2C_Handle.Init.DutyCycle          = I2C_DUTYCYCLE_2;
  I2C_Handle.Init.AddressingMode     = I2C_ADDRESSINGMODE_7BIT;
  I2C_Handle.Init.DualAddressMode    = I2C_DUALADDRESS_DISABLE;
  I2C_Handle.Init.GeneralCallMode    = I2C_GENERALCALL_DISABLE;
  I2C_Handle.Init.NoStretchMode      = I2C_NOSTRETCH_DISABLE;
  I2C_Handle.Init.OwnAddress1        = 0x00;
  I2C_Handle.Init.OwnAddress2        = 0x00;
  if (HAL_I2C_Init(&I2C_Handle) != HAL_OK) {
  	GPIO_UserLED->on();
  	UART_STLink->txStringCR("I2C: Initialization Failed");
  	while(1) {}
  }
  UART_STLink->txStringCR("I2C: Initialized");

    //Enable I2C Peripheral
  __HAL_I2C_ENABLE(&I2C_Handle);


    //
  uint8_t uData = 0;
  if (HAL_I2C_Mem_Read(&I2C_Handle, 0x53 << 1, 0x00, 1, &uData, 1, 1000) != HAL_OK) {
  	UART_STLink->txStringCR("I2C Read: Failed");
  	while(1) {}
  }

  char strOut[256];
  sprintf(strOut, "I2C Read: ID - %02X", uData);
  UART_STLink->txStringCR(strOut);*/


  QAD_I2C_InitStruct I2C_Init;
  I2C_Init.eI2C                = QAD_I2C2;
  I2C_Init.uIRQPriority_Event  = 0xE;
  I2C_Init.uIRQPriority_Error  = 0xE;
  I2C_Init.uClockSpeed         = 100000;
  I2C_Init.eDutyCycle          = QAD_I2C_DutyCycle_2;
  I2C_Init.eAddressingMode     = QAD_I2C_AddressingMode_7Bit;
  I2C_Init.eDualAddressingMode = QAD_I2C_DualAddressingMode_Disable;
  I2C_Init.eGeneralCallMode    = QAD_I2C_GeneralCallMode_Disable;
  I2C_Init.eNoStretchMode      = QAD_I2C_NoStretchMode_Disable;
  I2C_Init.uOwnAddress1        = 0;
  I2C_Init.uOwnAddress2        = 0;

  I2C_Init.pSCL_GPIO           = GPIOB;
  I2C_Init.uSCL_Pin            = GPIO_PIN_10;
  I2C_Init.uSCL_AF             = GPIO_AF4_I2C2;

  I2C_Init.pSDA_GPIO           = GPIOB;
  I2C_Init.uSDA_Pin            = GPIO_PIN_3;
  I2C_Init.uSDA_AF             = GPIO_AF9_I2C2;
  I2C_ADXL345 = new QAD_I2C(I2C_Init);

  if (I2C_ADXL345->init()) {
  	UART_STLink->txStringCR("I2C1: Initialization Failed");
  	GPIO_UserLED->on();
  	while(1) {}
  }
  I2C_ADXL345->start();
  UART_STLink->txStringCR("I2C1: Initialized and Started");

  //Test ADXL345
  uint8_t testVal = 0;
  char strOut[256];

  I2C_ADXL345->read8Bit(0x53 << 1, 0x00, &testVal);
  sprintf(strOut, "ADXL345 ID: %02X", testVal);
  UART_STLink->txStringCR(strOut);

  testVal = 0;
  I2C_ADXL345->write8Bit(0x53 << 1, 0x31, 0x00);
  I2C_ADXL345->read8Bit(0x53 << 1, 0x31, &testVal);
  sprintf(strOut, "ADXL345 Data Format: %02X", testVal);
  UART_STLink->txStringCR(strOut);

  testVal = 0;
  I2C_ADXL345->write8Bit(0x53 << 1, 0x31, 0x04);
  I2C_ADXL345->read8Bit(0x53 << 1, 0x31, &testVal);
  sprintf(strOut, "ADXL345 Data Format: %02X", testVal);
  UART_STLink->txStringCR(strOut);


	//----------------------------------
	//----------------------------------
	//----------------------------------
	//Processing Loop

  //Create processing loop timing variables
	uint32_t uTicks;
	uint32_t uCurTick;
	uint32_t uNewTick = HAL_GetTick();
	uint32_t uOldTick = uNewTick;

  //Create task timing variables
	uint32_t uHeartbeatTicks = 0;


	//----------------------------------
	//Infinite loop for device processing
	while (1) {

		//----------------------------------
		//Frame Timing
		//Calculates how many ticks (in milliseconds) have passed since the previous loop, this value is placed into the uTicks variable
		//uTicks is then used to calculate task timing below
    uCurTick = HAL_GetTick();
    if (uCurTick != uNewTick) { //Handle loop faster than 1ms issue
    	uOldTick = uNewTick;
    	uNewTick = uCurTick;
    	if (uNewTick < uOldTick) { //Handle 32bit overflow issue
    		uTicks = (uNewTick + (0xFFFFFFFF - uOldTick));
    	} else {
    		uTicks = (uNewTick - uOldTick);
    	}

    } else {
    	uTicks = 0;
    }


  	//----------------------------------
    //Update Heartbeat LED
    //The heartbeat LED uses the User LED to flash at a regular rate to visually show whether the microcontroller has locked up or
    //become stuck in an exception or interrupt handler
    uHeartbeatTicks += uTicks;
    if (uHeartbeatTicks >= QA_FT_HeartbeatTickThreshold) { //If heartbeat ticks has exceeded threshold then update heartbeat LED
    	GPIO_UserLED->toggle();
    	uHeartbeatTicks -= QA_FT_HeartbeatTickThreshold;     //Reset heartbeat ticks
    }

	}

	//This return value is unused, but is included in the source code to prevent compiler warning that main() doesn't return a value
	return 0;
}

