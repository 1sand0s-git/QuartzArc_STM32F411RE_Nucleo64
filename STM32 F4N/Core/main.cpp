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
#include "QAD_SPI.hpp"

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


//SPI Test
QAD_SPI* SPI_Flash;


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
  //Initialize SPI

/*  GPIO_InitTypeDef GPIO_Init = {0};

    //CLK
  GPIO_Init.Pin       = GPIO_PIN_5;
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_Init);

		//MOSI
	GPIO_Init.Pin       = GPIO_PIN_7;
	GPIO_Init.Mode      = GPIO_MODE_AF_PP;
	GPIO_Init.Pull      = GPIO_NOPULL;
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_Init.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

		//MISO
	GPIO_Init.Pin       = GPIO_PIN_6;
	GPIO_Init.Mode      = GPIO_MODE_AF_PP;
	GPIO_Init.Pull      = GPIO_NOPULL;
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_Init.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

		//CS
	GPIO_Init.Pin       = GPIO_PIN_4;
	GPIO_Init.Mode      = GPIO_MODE_AF_PP;
	GPIO_Init.Pull      = GPIO_PULLUP;
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_Init.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);


	  //Enable Clock
	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_SPI1_FORCE_RESET();
	__HAL_RCC_SPI1_RELEASE_RESET();


	  //Initialize Peripheral
	SPI_HandleTypeDef SPI_Handle;
	SPI_Handle.Instance               = SPI1;
	SPI_Handle.Init.Mode              = SPI_MODE_MASTER;
	SPI_Handle.Init.Direction         = SPI_DIRECTION_2LINES;
	SPI_Handle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SPI_Handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	SPI_Handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	SPI_Handle.Init.NSS               = SPI_NSS_HARD_OUTPUT;
	SPI_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  SPI_Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SPI_Handle.Init.TIMode            = SPI_TIMODE_DISABLE;
  SPI_Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SPI_Handle.Init.CRCPolynomial     = 0;

  if (HAL_SPI_Init(&SPI_Handle) != HAL_OK) {
  	UART_STLink->txStringCR("SPI: Initialization Failed");
  	GPIO_UserLED->on();
  	while(1) {}
  }
  UART_STLink->txStringCR("SPI: Initialized successfully");

  __HAL_SPI_ENABLE(&SPI_Handle);

    //Test Communication
  uint8_t dataTX[4] = {0x9F, 0x00, 0x00, 0x00};
  uint8_t dataRX[4];

  if (HAL_SPI_TransmitReceive(&SPI_Handle, &dataTX[0], &dataRX[0], 4, 1000) != HAL_OK) {
  	UART_STLink->txStringCR("SPI: Transceive Failed");
  }

  char strOut[256];
  sprintf(strOut, "SPI: Received %02X - %02X - %02X", dataRX[1], dataRX[2], dataRX[3]);
  UART_STLink->txStringCR(strOut);*/


  //--------------
  //--------------
  //Initialize SPI
  QAD_SPI_InitStruct SPI_Init;
  SPI_Init.eSPI              = QAD_SPI1;
  SPI_Init.uIRQPriority      = 0xE;
  SPI_Init.eSPIMode          = QAD_SPI_Mode_Master;
  SPI_Init.eSPIBiDir         = QAD_SPI_BiDir_Enabled;
  SPI_Init.eSPILines         = QAD_SPI_Lines_2Lines;
  SPI_Init.eSPIDataSize      = QAD_SPI_DataSize_8bit;
  SPI_Init.eSPIClkPolarity   = QAD_SPI_ClkPolarity_Low;
  SPI_Init.eSPIClkPhase      = QAD_SPI_ClkPhase_1Edge;
  SPI_Init.eSPICS            = QAD_SPI_CS_Soft;
  SPI_Init.eSPIPrescaler     = QAD_SPI_BaudPrescaler_128	;
  SPI_Init.eSPIFirstBit      = QAD_SPI_FirstBit_MSB;
  SPI_Init.eSPITIMode        = QAD_SPI_TIMode_Disable;
  SPI_Init.eSPICRC           = QAD_SPI_CRC_Disable;
  SPI_Init.uSPICRCPolynomial = 7;

  SPI_Init.pClk_GPIO         = GPIOA;
  SPI_Init.uClk_Pin          = GPIO_PIN_5;
  SPI_Init.uClk_AF           = GPIO_AF5_SPI1;

	SPI_Init.pMOSI_GPIO        = GPIOA;
	SPI_Init.uMOSI_Pin         = GPIO_PIN_7;
	SPI_Init.uMOSI_AF          = GPIO_AF5_SPI1;

	SPI_Init.pMISO_GPIO        = GPIOA;
	SPI_Init.uMISO_Pin         = GPIO_PIN_6;
	SPI_Init.uMISO_AF          = GPIO_AF5_SPI1;

	SPI_Init.pCS_GPIO          = GPIOA;
	SPI_Init.uCS_Pin           = GPIO_PIN_4;
	SPI_Init.uCS_AF            = GPIO_AF5_SPI1;
	SPI_Flash = new QAD_SPI(SPI_Init);

	if (SPI_Flash->init()) {
		UART_STLink->txStringCR("SPI - Flash: Initialization Failed");
		GPIO_UserLED->on();
		while(1) {}
	}
	SPI_Flash->start();
	UART_STLink->txStringCR("SPI - Flash: Initialized and Started");


	//Read Flash ID
	uint8_t uDataTX[4];
	uint8_t uDataRX[4];

	uDataTX[0] = 0x9F;
	for (uint8_t i=1; i<4; i++)
		uDataTX[i] = 0x00;

	SPI_Flash->transceive(&uDataTX[0], &uDataRX[0], 4);

	char strID[256];
	sprintf(strID, "SPI - Flash: ID Register = %02X - %02X - %02X", uDataRX[1], uDataRX[2], uDataRX[3]);
	UART_STLink->txStringCR(strID);



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

