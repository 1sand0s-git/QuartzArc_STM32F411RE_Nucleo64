/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Core                                                          */
/*   Role: Setup                                                           */
/*   Filename: setup.hpp                                                   */
/*   Date: 22nd October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __SETUP_HPP_
#define __SETUP_HPP_

//Includes
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //Result Enum
  //This is used as a return value for multiple functions to show if the function has succeeded or failed
  //An OK result will provide a boolean false, while any error will provide a boolean true
enum QA_Result : uint8_t {
	QA_OK = 0,                      //Function has succeeded
	QA_Fail,                        //Function has failed, with a non-specific error
	QA_Error_PeriphBusy,            //Function has not been able to initialize a particular peripheral as the peripheral is busy
	QA_Error_PeriphNotSupported     //Function has not been able to initialize a particular peripheral as the peripheral doesn't support the required functionality
};


  //Init State Enum
  //This is used to store whether a particular driver or system is initialized or not
enum QA_InitState : uint8_t {QA_NotInitialized = 0, QA_Initialized};


  //Active State Enum
  //This is used to store whether a particular driver or system is currently active or not
enum QA_ActiveState : uint8_t {QA_Inactive = 0, QA_Active};


	//----------------------------------------
	//----------------------------------------
	//----------------------------------------


	//-------------------------------
	//QAD_IRQHandler_CallbackFunction
  //This is a generic callback function pointer definition that is used within drivers / systems.
typedef void (*QAD_IRQHandler_CallbackFunction)(void* pData);


	//----------------------------
	//QAD_IRQHandler_CallbackClass
  //This is a generic class definition to be inherited by a parent class that requires the handler method to be called by drivers / systems.
class QAD_IRQHandler_CallbackClass {
public:

	virtual void handler(void* pData) = 0; //This is a pure virtual function that must be defined within the inheriting/parent class
};


	//----------------------------------------
	//----------------------------------------
	//----------------------------------------

	//----------------
	//GPIO Definitions

  //User LED
#define QAD_USERLED_GPIO_PORT           GPIOA
#define QAD_USERLED_GPIO_PIN            GPIO_PIN_5   // A5

  //User Button
#define QAD_USERBUTTON_GPIO_PORT        GPIOC
#define QAD_USERBUTTON_GPIO_PIN         GPIO_PIN_13  // C13


	//-----------------
	//UART2 Definitions
  //
  //These are used for the UART/Serial link via STLink on the STM32F411 Nucleo64 board

#define QAD_UART2_TX_PORT     GPIOA
#define QAD_UART2_TX_PIN      GPIO_PIN_2        //A2
#define QAD_UART2_TX_AF       GPIO_AF7_USART2
#define QAD_UART2_RX_PORT     GPIOA
#define QAD_UART2_RX_PIN      GPIO_PIN_3        //A3
#define QAD_UART2_RX_AF       GPIO_AF7_USART2
#define QAD_UART2_BAUDRATE    57600
#define QAD_UART2_TX_FIFOSIZE 256
#define QAD_UART2_RX_FIFOSIZE 256


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

	//----------------------------
	//Interrupt Request Priorities

#define QAD_IRQPRIORITY_UART2    ((uint8_t) 0x08) //Priority for TX/RX interrupts for UART2 handler, which is used for serial via STLink on the STM32F411 Nucleo64 board

#define QAD_IRQPRIORITY_EXTI     ((uint8_t) 0x0A) //Priority to be used by external interrupt handlers. Shared by all external interrupts

#define QAD_IRQPRIORITY_ADC      ((uint8_t) 0x0A)


//Prevent Recursive Inclusion
#endif /* __SETUP_HPP */
