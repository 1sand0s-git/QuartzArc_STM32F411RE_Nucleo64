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


//Driver class for testing EXTI driver functionality (driver defined in QAD_EXTI.hpp)
QAD_EXTI* EXTI_Test;

//Callback function for testing EXTI driver functionality
//This is the method that will be called when the EXTI interrupt is triggered
void EXTI_Test_Handler(void* pData) {
	UART_STLink->txStringCR("Interrupt Triggered");
}


//Driver class for testing Timer interrupt driver functionality (driver defined in QAD_Timer.hpp)
QAD_Timer* Timer_Test;

//Callback function for testing Timer interrupt driver functionality
//This is the method that will be called when the Timer Update interrupt is triggered
void Timer_Test_Handler(void* pData) {
	UART_STLink->txStringCR("Timer Triggered");
}


//Driver class for testing Rotary Encoder driver functionality (driver defined in QAD_Encoder.hpp)
QAD_Encoder* Encoder_Test;


//Driver class for testing Pulse Width Modulation functionality (driver defined in QAD_PWM.hpp)
QAD_PWM* PWM_Test;



//Task Timing
//
//These constants are used to determine the update rate (in milliseconds) of each of the
//tasks that are run in the processing loop within the main() function.
//
const uint32_t QA_FT_EncoderTickThreshold = 50;     //Time in milliseconds between encoder task updates

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
  sSerialInit.uart.uart        = QAD_UART2;                //Define the UART peripheral to be used (enum defined in QAD_UART.hpp)
  sSerialInit.uart.baudrate    = QAD_UART2_BAUDRATE;       //Define the baudrate to be used by the UART peripheral
  sSerialInit.uart.irqpriority = QAD_IRQPRIORITY_UART2;    //Defined the IRQ priority to be used by the TX and RX interrupts
  sSerialInit.uart.txgpio      = QAD_UART2_TX_PORT;        //Define the GPIO port for the TX pin
  sSerialInit.uart.txpin       = QAD_UART2_TX_PIN;         //Define the pin number for the TX pin
  sSerialInit.uart.txaf        = QAD_UART2_TX_AF;          //Define the alternate function for the TX pin
  sSerialInit.uart.rxgpio      = QAD_UART2_RX_PORT;        //Define the GPIO port for the RX pin
  sSerialInit.uart.rxpin       = QAD_UART2_RX_PIN;         //Define the pin number for the RX pin
  sSerialInit.uart.rxaf        = QAD_UART2_RX_AF;          //Define the alternate function for the TX pin
  sSerialInit.txfifo_size      = QAD_UART2_TX_FIFOSIZE;    //Define the size (in bytes) for the transmit FIFO
  sSerialInit.rxfifo_size      = QAD_UART2_RX_FIFOSIZE;    //Define the size (in bytes) for the receive FIFO

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
  //Initialize a QAD_EXTI driver class to test EXTI functionality (defined in QAD_EXTI.hpp)
  //QAD_USERBUTTON_GPIO_PORT and QAD_USERBUTTON_GPIO_PIN are defined in setup.hpp

  //Create the driver class, passing to it the GPIO port and pin that are to be used
  EXTI_Test = new QAD_EXTI(QAD_USERBUTTON_GPIO_PORT, QAD_USERBUTTON_GPIO_PIN);

  //Set the interrupt handler function that is to be called by the driver class when the
  //external interrupt is triggered
  EXTI_Test->setHandlerFunction(EXTI_Test_Handler);

  //Enable the external interrupt, with triggering on both rising and falling edges
  //EXTI_Test->enable(QAD_EXTI::EdgeBoth);


	//----------------------------------
  //Initialize a QAD_Timer driver class to test functionality (defined in QAD_Timer.hpp)
  //QAD_Timer driver is used to trigger interrupts at regular intervals

  //Fill out the initialization structure required to create the driver class (defined in QAD_Timer.hpp)
  QAD_Timer_InitStruct Timer_Test_Init;
  Timer_Test_Init.eTimer         = QAD_Timer2;      //Define the timer peripheral is to be used (enum defined in QAD_TimerMgr.hpp)
  Timer_Test_Init.eMode          = TimerMultiple;   //Define which mode to use (Continuous, Multiple or Single)
  Timer_Test_Init.uPrescaler     = 20000;           //Define the timer prescaler
  Timer_Test_Init.uPeriod        = 5000;            //Define the timer period
  Timer_Test_Init.uIRQPriority   = 0x9;             //Set the interrupt priority
  Timer_Test_Init.uCounterTarget = 6;               //Set the counter target (used when driver is in Multiple mode)

  //Create the timer class, passing it a reference to the initialization structure
  //Driver class is created in an uninitialized state
  Timer_Test = new QAD_Timer(Timer_Test_Init);

  //Initialize the timer class, and if initialization fails (returns true) the turn on
  //User LED, output failed message via serial and enter infinite loop
  if (Timer_Test->init()) {
  	GPIO_UserLED->on();
  	UART_STLink->txStringCR("Timer: Initialization Failed");
  	while (1) {}
  }

  //If timer initialization passed then output message via serial
  UART_STLink->txStringCR("Timer: Initialized");

  //Set the interrupt handler function to be called when interrupt is triggered
  Timer_Test->setHandlerFunction(Timer_Test_Handler);

  //Start timer class
  //Timer_Test->start();


	//----------------------------------
  //Initialize a QAD_Encoder Driver class to test driver functionality

  //Fill out initialization structure required to create the encoder driver class (defined in QAD_Encoder.hpp)
  QAD_Encoder_InitStruct sEncoderInit;
  sEncoderInit.pCh1_GPIO = GPIOB;                   //GPIO used for channel 1 of the timer
  sEncoderInit.uCh1_Pin  = GPIO_PIN_4;              //GPIO pin used for channel 1
  sEncoderInit.uCh1_AF   = GPIO_AF2_TIM3;           //Alternate function used to connect GPIO pin to timer peripheral
  sEncoderInit.pCh2_GPIO = GPIOB;                   //GPIO used for channel 2 of the timer
  sEncoderInit.uCh2_Pin  = GPIO_PIN_5;              //GPIO pin used for channel 2
  sEncoderInit.uCh2_AF   = GPIO_AF2_TIM3;           //Alternate function used to connect GPIO pin to timer peripheral
  sEncoderInit.eTimer    = QAD_Timer3;              //Timer peripheral to be used for rotary encoder (enum defined in QAD_TimerMgr.hpp)
  sEncoderInit.eMode     = QAD_EncoderMode_Linear;  //Encode mode, can be either Linear or Exponential (enum defined in QAD_Encoder.hpp)

  //Create encoder driver class
  //Is created in an uninitialized state
  Encoder_Test = new QAD_Encoder(sEncoderInit);

  //Initialize the encoder driver class
  //If initialization fails the turn on User LED, output failed message via UART and enter infinite loop
  if (Encoder_Test->init()) {
  	GPIO_UserLED->on();
  	UART_STLink->txStringCR("Encoder: Initialization Failed");
  	while (1) {}
  }

  //Start encoder driver class and output message via UART that encoder has successfully initialized and has been started
  Encoder_Test->start();
  UART_STLink->txStringCR("Encoder: Initialized & Started");


	//----------------------------------
  //Timer PWM mode testing as used in STM32 Basics - TIM - PWM video
  //Has been replaced by QAD_PWM driver class below

  //Initialize GPIO Pin
/*  GPIO_InitTypeDef GPIO_Init = {0};
  GPIO_Init.Pin       = GPIO_PIN_6;
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOB, &GPIO_Init);

  //Enable Timer Clock
  __HAL_RCC_TIM4_CLK_ENABLE();

  //Initialize Timer
  TIM_HandleTypeDef sTimerHandle = {0};
  sTimerHandle.Instance               = TIM4;
  sTimerHandle.Init.Prescaler         = 100;
  sTimerHandle.Init.Period            = 256;
  sTimerHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  sTimerHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  sTimerHandle.Init.RepetitionCounter = 0xFF;
  sTimerHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&sTimerHandle) != HAL_OK) {
  	UART_STLink->txStringCR("PWM: Initialization Failed");
  	GPIO_UserLED->on();
  	while(1) {}
  }
  UART_STLink->txStringCR("PWM: Initialized");

  //Initialize PWM Channel
  TIM_OC_InitTypeDef sChannelInit = {0};
  sChannelInit.OCMode         = TIM_OCMODE_PWM1;
  sChannelInit.OCIdleState    = TIM_OCIDLESTATE_SET;
  sChannelInit.Pulse          = 0;
  sChannelInit.OCPolarity     = TIM_OCPOLARITY_HIGH;
  sChannelInit.OCFastMode     = TIM_OCFAST_ENABLE;
  if (HAL_TIM_PWM_ConfigChannel(&sTimerHandle, &sChannelInit, TIM_CHANNEL_1) != HAL_OK) {
  	UART_STLink->txStringCR("PWM: Channel Initialization Failed");
  	GPIO_UserLED->on();
  	while(1) {}
  }
  UART_STLink->txStringCR("PWM: Channel 1 Initialized");

  //Start PWM
  HAL_TIM_PWM_Start(&sTimerHandle, TIM_CHANNEL_1);

  __HAL_TIM_SET_COMPARE(&sTimerHandle, TIM_CHANNEL_1, 192);*/


	//----------------------------------
  //Initialize PWM driver class for testing of functionality

  //Fill out initialization structure required to creat the PWM driver class (defined in QAD_PWM.hpp)
  QAD_PWM_InitStruct sPWMInit;
  sPWMInit.eTimer = QAD_Timer4;                   //Define the timer peripheral to be used (enum defined in QAD_TimerMgr.hpp)
  sPWMInit.uPrescaler = 100;                      //Define the prescaler for the timer
  sPWMInit.uPeriod    = 256;                      //Define the period for the timer
  sPWMInit.sChannels[0].eActive = QA_Active;      //Define timer channel 1 as active
  sPWMInit.sChannels[0].pGPIO   = GPIOB;          //Define the GPIO port for timer channel 1
  sPWMInit.sChannels[0].uPin    = GPIO_PIN_6;     //Define the pin number for timer channel 1
  sPWMInit.sChannels[0].uAF     = GPIO_AF2_TIM4;  //Define the alternate function for timer channel 1
  sPWMInit.sChannels[1].eActive = QA_Inactive;    //Define timer channel 2 as inactive
  sPWMInit.sChannels[2].eActive = QA_Inactive;    //Define timer channel 3 as inactive
  sPWMInit.sChannels[3].eActive = QA_Inactive;    //Define timer channel 4 as inactive

  //Create the QAD_PWM driver class, passing a reference to the initialization structure
  //Driver class is created in an uninitialized state
  PWM_Test = new QAD_PWM(sPWMInit);

  //Initialize the PWM driver class
  //If initialization fails then output failed message via UART, turn on User LED and enter infinite loop
  if (PWM_Test->init()) {
  	UART_STLink->txString("PWM: Initialization Failed");
  	GPIO_UserLED->on();
  	while(1) {}
  }

  //Start PWM driver
  PWM_Test->start();

  //Output message via UART to show PWM driver successfully initialized and has been started
  UART_STLink->txStringCR("PWM: Initialized & Started");


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
  uint32_t uEncoderTicks = 0;
	uint32_t uHeartbeatTicks = 0;

	//Temporary variable used for PWM driver testing
	int32_t iPWMVal = 128;


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
    //Process encoder task
    //Currently used to update encoder, retrieve encoder value and use it to update PWM value
    uEncoderTicks += uTicks;
    if (uEncoderTicks >= QA_FT_EncoderTickThreshold) {  //If encoder ticks has exceeded threshold then process encoder task

    	Encoder_Test->update(uEncoderTicks);              //Update encoder driver
    	iPWMVal += Encoder_Test->getValue() * 4;          //Retrieve encoder value and add to PWM value

      if (iPWMVal < 0)                                  //Limit PWM value to between 0-255, as per period defined by period set during PWM driver initialization
      	iPWMVal = 0;
      if (iPWMVal > 255)
      	iPWMVal = 255;
      PWM_Test->setPWMVal(QAD_PWM::PWM_Channel_1, iPWMVal);  //Update PWM value


    	uEncoderTicks -= QA_FT_EncoderTickThreshold;      //Reset encoder ticks
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

