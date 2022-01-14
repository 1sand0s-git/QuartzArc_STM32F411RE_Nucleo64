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
#include "QAD_RGB.hpp"

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


//
QAD_PWM* PWM_LED;
QAD_RGB* RGB_LED;


//Task Timing
//
//These constants are used to determine the update rate (in milliseconds) of each of the
//tasks that are run in the processing loop within the main() function.
//
const uint32_t QA_FT_LEDTickThreshold       = 10;

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

	//----------------------------------
	//----------------------------------
  //Initialize RGB

  //PWM
  QAD_PWM_InitStruct PWM_Init;
  PWM_Init.eTimer = QAD_Timer1;
  PWM_Init.uPrescaler = 1000;
  PWM_Init.uPeriod    = 255;

  PWM_Init.sChannels[0].eActive = QA_Active;
  PWM_Init.sChannels[0].pGPIO   = GPIOA;
  PWM_Init.sChannels[0].uPin    = GPIO_PIN_8;
  PWM_Init.sChannels[0].uAF     = GPIO_AF1_TIM1;
  PWM_Init.sChannels[1].eActive = QA_Active;
  PWM_Init.sChannels[1].pGPIO   = GPIOA;
  PWM_Init.sChannels[1].uPin    = GPIO_PIN_9;
  PWM_Init.sChannels[1].uAF     = GPIO_AF1_TIM1;
  PWM_Init.sChannels[2].eActive = QA_Active;
  PWM_Init.sChannels[2].pGPIO   = GPIOA;
  PWM_Init.sChannels[2].uPin    = GPIO_PIN_10;
  PWM_Init.sChannels[2].uAF     = GPIO_AF1_TIM1;
  PWM_Init.sChannels[3].eActive = QA_Inactive;
  PWM_LED = new QAD_PWM(PWM_Init);

  if (PWM_LED->init()) {
  	UART_STLink->txStringCR("PWM: Initialization Failed");
  	GPIO_UserLED->on();
  	while(1) {}
  }
  PWM_LED->start();
  UART_STLink->txStringCR("PWM: Initialized and started");


  //RGB
  QAD_RGB_InitStruct RGB_Init;
  RGB_Init.cPWM          = PWM_LED;
  RGB_Init.eRedChannel   = QAD_PWM_Channel_1;
  RGB_Init.eGreenChannel = QAD_PWM_Channel_2;
  RGB_Init.eBlueChannel  = QAD_PWM_Channel_3;
  RGB_Init.uRed          = 0x07;
  RGB_Init.uGreen        = 0x7F;
  RGB_Init.uBlue         = 0x07;
  RGB_Init.uBrightness   = 0xBF;
  RGB_Init.bInvert       = true;
  RGB_LED = new QAD_RGB(RGB_Init);

  if (RGB_LED->init()) {
  	UART_STLink->txStringCR("RGB: Initialization Failed");
  	GPIO_UserLED->on();
  	while(1) {}
  }
  UART_STLink->txStringCR("RGB: Initialized");


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
	uint32_t uLEDTicks = 0;
	uint32_t uHeartbeatTicks = 0;

	//LED Data
	uint8_t uBrightness = 0;
	bool    bDirection = true;


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
    //Update RGB LED
    uLEDTicks += uTicks;
    if (uLEDTicks >= QA_FT_LEDTickThreshold) {

    	if (bDirection) {

    		if (uBrightness == 255) {
    			bDirection = false;
    		} else {
    			uBrightness++;
    		}

    	} else {

    		if (uBrightness == 0) {
    			bDirection = true;
    		} else {
    			uBrightness--;
    		}

    	}
    	RGB_LED->setBrightness(uBrightness);

    	uLEDTicks -= QA_FT_LEDTickThreshold;
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

