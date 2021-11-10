/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Core                                                          */
/*   Role: Exception and Interrupt Handlers                                */
/*   Filename: handlers.hpp                                                */
/*   Date: 22th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __HANDLERS_HPP_
#define __HANDLERS_HPP_

//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//Define exception and interrupt handler functions as C code to make sure linker is able to correctly link the functions
extern "C" {


  //---------------------------
  //Exception Handler Functions

void  NMI_Handler(void);
void  HardFault_Handler(void);
void  MemManage_Handler(void);
void  BusFault_Handler(void);
void  UsageFault_Handler(void);
void  SVC_Handler(void);
void  DebugMon_Handler(void);
void  PendSV_Handler(void);
void  SysTick_Handler(void);


	//-------------------------
	//Interrupt Handler Methods

void USART2_IRQHandler(void);

void EXTI15_10_IRQHandler(void);

void TIM2_IRQHandler(void);

}


//Prevent Recursive Inclusion
#endif /* __HANDLERS_HPP */
