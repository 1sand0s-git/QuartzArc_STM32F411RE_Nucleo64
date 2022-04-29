/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Core                                                          */
/*   Role: Exception and Interrupt Handlers                                */
/*   Filename: handlers.cpp                                                */
/*   Date: 22nd October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "handlers.hpp"

#include "QAD_GPIO.hpp"
#include "QAD_ADC.hpp"

#include "QAS_Serial_Dev_UART.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


extern QAD_GPIO_Output* GPIO_UserLED;     //User LED driver class (defined in main.cpp)

extern QAS_Serial_Dev_UART* UART_STLink;  //UART System for serial over ST-Link (defined in main.cpp)


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //---------------------------
  //---------------------------
  //Exception Handler Functions

//NMI_Handler
//Exception Handler Function
void  NMI_Handler(void) {

}


//HardFault_Handler
//Exception Handler Function
void  HardFault_Handler(void) {
	GPIO_UserLED->on();
  while (1) {}
}


//MemManage_Handler
//Exception Handler Function
void  MemManage_Handler(void) {
	GPIO_UserLED->on();
  while (1) {}
}


//BusFault_Handler
//Exception Handler Function
void  BusFault_Handler(void) {
	GPIO_UserLED->on();
  while (1) {}
}


//UsageFault_Handler
//Exception Handler Function
void  UsageFault_Handler(void) {
	GPIO_UserLED->on();
  while (1) {}
}


//SVC_Handler
//Exception Handler Function
void  SVC_Handler(void) {

}


//DebugMon_Handler
//Exception Handler Function
void  DebugMon_Handler(void) {

}


//PendSV_Handler
//Exception Handler Function
void  PendSV_Handler(void) {

}


//SysTick_Handler
//Exception Handler Function
void  SysTick_Handler(void) {
  HAL_IncTick();
}


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //---------------------------
	//---------------------------
	//Interrupt Handler Functions

//USART2_IRQHandler
//Interrupt Handler Function
//
//This is used for TX and RX interrupts for serial over ST-Link
void USART2_IRQHandler(void) {
	UART_STLink->handler(NULL); //Calls interrupt handler method in QAD_Serial_Dev_UART class
}


//ADC_IRQHandler
//Interrupt Handler Function
void ADC_IRQHandler(void) {
  QAD_ADC::handler();
}


//EXTI15_10_IRQHandler
//Interrupt Handler Function
void EXTI15_10_IRQHandler(void) {

	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
	}
}





