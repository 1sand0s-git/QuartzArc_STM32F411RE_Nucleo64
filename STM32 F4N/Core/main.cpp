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
//ADC_HandleTypeDef ADC_Handle = {0};
//uint16_t ADC_TempData = 0;



//Task Timing
//
//These constants are used to determine the update rate (in milliseconds) of each of the
//tasks that are run in the processing loop within the main() function.
//
const uint32_t QA_FT_ADCUpdateTickThreshold = 1000;       //

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



  //---------
  //---------
  //Setup ADC

/*  //Setup ADC GPIO
  GPIO_InitTypeDef GPIO_Init = {0};
  GPIO_Init.Pin    = GPIO_PIN_0;
  GPIO_Init.Mode   = GPIO_MODE_ANALOG;
  GPIO_Init.Pull   = GPIO_NOPULL;
  GPIO_Init.Speed  = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_Init);


  //Setup ADC Trigger Timer
  __HAL_RCC_TIM2_CLK_ENABLE();

  //
  TIM_HandleTypeDef TIM_Handle = {0};
  TIM_Handle.Instance               = TIM2;
  TIM_Handle.Init.Prescaler         = 5000;
  TIM_Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TIM_Handle.Init.Period            = 500;
  TIM_Handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  TIM_Handle.Init.RepetitionCounter = 0x0;
  TIM_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&TIM_Handle) != HAL_OK) {
  	GPIO_UserLED->on();
  	UART_STLink->txStringCR("TIM: Initialization Failed");
  	while(1) {}
  }
	UART_STLink->txStringCR("TIM: Initialized");

  //
  TIM_MasterConfigTypeDef TIM_MC_Init = {0};
  TIM_MC_Init.MasterOutputTrigger = TIM_TRGO_UPDATE;
  HAL_TIMEx_MasterConfigSynchronization(&TIM_Handle, &TIM_MC_Init);

  //Setup ADC Peripheral

  //
  __HAL_RCC_ADC1_CLK_ENABLE();

  //
  ADC_Handle.Instance                   = ADC1;
  ADC_Handle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
  ADC_Handle.Init.Resolution            = ADC_RESOLUTION_12B;
  ADC_Handle.Init.ScanConvMode          = ENABLE;
  ADC_Handle.Init.ContinuousConvMode    = DISABLE;
  ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
  ADC_Handle.Init.NbrOfDiscConversion   = 0;
  ADC_Handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
  ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T2_TRGO;
  ADC_Handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  ADC_Handle.Init.NbrOfConversion       = 1;
  ADC_Handle.Init.DMAContinuousRequests = ENABLE;
  ADC_Handle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&ADC_Handle) != HAL_OK) {
  	GPIO_UserLED->on();
  	UART_STLink->txStringCR("ADC: Initialization Failed");
  	while(1) {}
  }
  UART_STLink->txStringCR("ADC: Initialized");

  //Setup ADC Channel
  ADC_ChannelConfTypeDef ADC_Channel_Init = {0};
  ADC_Channel_Init.Channel      = ADC_CHANNEL_0;
  ADC_Channel_Init.Rank         = 1;
  ADC_Channel_Init.Offset       = 0;
  ADC_Channel_Init.SamplingTime = ADC_SAMPLETIME_84CYCLES;
  if (HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Channel_Init) != HAL_OK) {
  	GPIO_UserLED->on();
  	UART_STLink->txStringCR("ADC: Channel Setup Failed");
  	while(1) {}
  }
  UART_STLink->txStringCR("ADC: Channel Setup Successful");


  //Setup ADC Interrupt
  HAL_NVIC_SetPriority(ADC_IRQn, 0x8, 0x0);
  HAL_NVIC_EnableIRQ(ADC_IRQn);


  //Start ADC Conversion
  HAL_ADC_Start_IT(&ADC_Handle);
  __HAL_TIM_ENABLE(&TIM_Handle);*/


  //----------
  //ADC Driver

    //Setup ADC Driver
  QAD_ADC_InitStruct sADCInit;
  sADCInit.eTimer = QAD_Timer2;
  sADCInit.uTimer_Prescaler = 5000;
  sADCInit.uTimer_Period    = 500;

  if (QAD_ADC::init(sADCInit)) {
  	UART_STLink->txStringCR("ADC: Initialization Failed");
  	GPIO_UserLED->on();
  	while(1) {}
  }
  UART_STLink->txStringCR("ADC: Initialized");

    //Setup Channels
  QAD_ADC_ChannelData sADCChannel = {0};

  sADCChannel.pGPIO           = GPIOA;
  sADCChannel.uPin            = GPIO_PIN_0;
  sADCChannel.eChannel        = QAD_ADC_PeriphChannel0;
  sADCChannel.eSamplingTime   = QAD_ADC_PeriphSamplingTime_56Cycles;
  QAD_ADC::addChannel(sADCChannel);

  sADCChannel.pGPIO           = GPIOA;
  sADCChannel.uPin            = GPIO_PIN_1;
  sADCChannel.eChannel        = QAD_ADC_PeriphChannel1;
  sADCChannel.eSamplingTime   = QAD_ADC_PeriphSamplingTime_56Cycles;
  QAD_ADC::addChannel(sADCChannel);

  sADCChannel.pGPIO           = GPIOA;
  sADCChannel.uPin            = GPIO_PIN_4;
  sADCChannel.eChannel        = QAD_ADC_PeriphChannel4;
  sADCChannel.eSamplingTime   = QAD_ADC_PeriphSamplingTime_56Cycles;
  QAD_ADC::addChannel(sADCChannel);

    //Start ADC Driver
  if (QAD_ADC::start()) {
  	UART_STLink->txStringCR("ADC: Unable to start driver");
  	GPIO_UserLED->on();
  	while(1) {}
  }
  UART_STLink->txStringCR("ADC: Driver started");


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
	uint32_t uADCUpdateTicks = 0;
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


    //ADC Update
    uADCUpdateTicks += uTicks;
    if (uADCUpdateTicks >= QA_FT_ADCUpdateTickThreshold) {

    	char strData[256];
    	for (uint8_t i=0; i<QAD_ADC::getChannelCount(); i++) {
    	  sprintf(strData, "ADC Channel %u: %u", i, QAD_ADC::getData(i));
    	  UART_STLink->txStringCR(strData);
    	}
    	UART_STLink->txCR();

    	uADCUpdateTicks -= QA_FT_ADCUpdateTickThreshold;
    }


  	//----------------------------------
    //Update Heartbeat LED
    //The heartbeat LED uses the User LED to flash at a regular rate to visually show whether the microcontroller has locked up or
    //become stuck in an exception or interrupt handler
    uHeartbeatTicks += uTicks;
    if (uHeartbeatTicks >= QA_FT_HeartbeatTickThreshold) { //If heartbeat ticks has exceeded threshold then update heartbeat LED
    	GPIO_UserLED->toggle();
    	uHeartbeatTicks -= QA_FT_HeartbeatTickThreshold;     //Reset hearbeat ticks
    }

	}

	//This return value is unused, but is included in the source code to prevent compiler warning that main() doesn't return a value
	return 0;
}

