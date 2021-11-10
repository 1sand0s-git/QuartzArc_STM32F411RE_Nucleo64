/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Core                                                          */
/*   Role: Exception and Interrupt Handlers                                */
/*   Filename: handlers.cpp                                                */
/*   Date: 22th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "handlers.hpp"

#include "QAD_EXTI.hpp"
#include "QAD_Timer.hpp"

#include "QAS_Serial_Dev_UART.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

extern QAS_Serial_Dev_UART* UART_STLink;  //UART System for serial over ST-Link (defined in main.cpp)

extern QAD_EXTI* EXTI_Test;               //EXTI test driver (defined in main.cpp)
extern QAD_Timer* Timer_Test;             //Timer test driver (defined in main.cpp)



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
  while (1) {}
}


//MemManage_Handler
//Exception Handler Function
void  MemManage_Handler(void) {
  while (1) {}
}


//BusFault_Handler
//Exception Handler Function
void  BusFault_Handler(void) {
  while (1) {}
}


//UsageFault_Handler
//Exception Handler Function
void  UsageFault_Handler(void) {
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
  HAL_SYSTICK_IRQHandler();
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


//EXTI15_10_IRQHandler
//Interrupt Handler Function
//
//This is currently used for testing of the QAD_EXTI driver class
void EXTI15_10_IRQHandler(void) {
  EXTI_Test->handler();  //Calls interrupt handler method in QAD_EXTI driver class
}


//TIM2_IRQHandler
//Interrupt Handler Function
//
//This is currently used for testing of the QAD_Timer driver class
void TIM2_IRQHandler(void) {
  Timer_Test->handler();  //Calls interrupt handler method in QAD_Timer driver class
}









