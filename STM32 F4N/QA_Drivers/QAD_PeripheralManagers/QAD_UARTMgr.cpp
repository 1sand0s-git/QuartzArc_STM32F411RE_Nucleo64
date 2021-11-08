/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: UART Management Driver                                          */
/*   Filename: QAD_UARTMgr.cpp                                             */
/*   Date: 2nd November 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_UARTMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


	//------------
	//------------
	//Constructors

//QAD_UARTMgr::QAD_UARTMgr
//QAD_UARTMgr Constructor
//
//Fills out details for the system's UART peripherals
//As this is a private method in a singleton class, this method will be called the first time the class's get() method is called.
QAD_UARTMgr::QAD_UARTMgr() {

	for (uint8_t i=0; i<QAD_UART_PeriphCount; i++) {
		m_sUARTs[i].eState = QAD_UART_Unused;
	}

	//Set UART Periph ID
	m_sUARTs[QAD_UART1].eUART = QAD_UART1;
	m_sUARTs[QAD_UART2].eUART = QAD_UART2;
	m_sUARTs[QAD_UART6].eUART = QAD_UART6;

	//Set Instances
	m_sUARTs[QAD_UART1].pInstance = USART1;
	m_sUARTs[QAD_UART2].pInstance = USART2;
	m_sUARTs[QAD_UART6].pInstance = USART6;

	//Set IRQs
	m_sUARTs[QAD_UART1].eIRQ = USART1_IRQn;
	m_sUARTs[QAD_UART2].eIRQ = USART2_IRQn;
	m_sUARTs[QAD_UART6].eIRQ = USART6_IRQn;

}


  //--------------------------------------
	//--------------------------------------
	//QAD_UARTMgr Private Management Methods

//QAD_UARTMgr::imp_registerUART
//QAD_UARTMgr Private Management Method
//
//To be called from static method registerUART()
//Used to register a UART peripheral as being used by a driver
//eUART - the UART peripheral to be registered. A member of QAD_UART_Periph
//Returns QA_OK if registration is successful, or returns QA_Error_PeriphBusy if the selected UART is already in use
QA_Result QAD_UARTMgr::imp_registerUART(QAD_UART_Periph eUART) {
	if (eUART >= QAD_UARTNone)
		return QA_Fail;

  if (m_sUARTs[eUART].eState)
  	return QA_Error_PeriphBusy;

  m_sUARTs[eUART].eState = QAD_UART_InUse;
  return QA_OK;
}


//QAD_UARTMgr::imp_deregisterUART
//QAD_UARTMgr Private Management Method
//
//To be called from static method deregisterUART()
//Used to deregister a UART peripheral to mark it as no longer being used by a driver
//eUART - the UART peripheral to be deregistered. A member of QAD_UART_Periph
void QAD_UARTMgr::imp_deregisterUART(QAD_UART_Periph eUART) {
	if (eUART >= QAD_UARTNone)
		return;

  m_sUARTs[eUART].eState = QAD_UART_Unused;
}


  //---------------------------------
	//---------------------------------
	//QAD_UARTMgr Private Clock Methods

//QAD_UARTMgr::imp_enableClock
//QAD_UARTMgr Private Clock Method
//
//To be called from static method enableClock()
//Used to enable the clock for a specific UART peripheral
//eUART - the UART peripheral to enable the clock for
void QAD_UARTMgr::imp_enableClock(QAD_UART_Periph eUART) {
  switch (eUART) {
    case (QAD_UART1):
    	__HAL_RCC_USART1_CLK_ENABLE();
      __HAL_RCC_USART1_FORCE_RESET();
      __HAL_RCC_USART1_RELEASE_RESET();
      break;
    case (QAD_UART2):
    	__HAL_RCC_USART2_CLK_ENABLE();
      __HAL_RCC_USART2_FORCE_RESET();
      __HAL_RCC_USART2_RELEASE_RESET();
      break;
    case (QAD_UART6):
    	__HAL_RCC_USART6_CLK_ENABLE();
      __HAL_RCC_USART6_FORCE_RESET();
      __HAL_RCC_USART6_RELEASE_RESET();
      break;
    case (QAD_UARTNone):
    	break;
  }
}


//QAD_UARTMgr::imp_disableClock
//QAD_UARTMgr Private Clock Method
//
//To be called from static method disableClock()
//Used to disable the clock for a specific UART peripheral
//eUART - the UART peripheral to disable the clock for
void QAD_UARTMgr::imp_disableClock(QAD_UART_Periph eUART) {
  switch (eUART) {
    case (QAD_UART1):
    	__HAL_RCC_USART1_CLK_DISABLE();
      break;
    case (QAD_UART2):
    	__HAL_RCC_USART2_CLK_DISABLE();
      break;
    case (QAD_UART6):
    	__HAL_RCC_USART6_CLK_DISABLE();
      break;
    case (QAD_UARTNone):
    	break;
  }
}


  //----------------------------------
	//----------------------------------
	//QAD_UARTMgr Private Status Methods

//QAD_UARTMgr::imp_getUARTsActive
//QAD_UARTMgr Private Status Method
//
//To be called from static method getUARTsActive()
//Returns the number of UART peripherals that are currently in-use (registered/active)
uint8_t QAD_UARTMgr::imp_getUARTsActive(void) {
  uint8_t uCount = 0;
  for (uint8_t i=0; i<QAD_UART_PeriphCount; i++) {
  	if (m_sUARTs[i].eState)
  		uCount++;
  }
  return uCount;
}


//QAD_UARTMgr::imp_getUARTsInactive
//QAD_UARTMgr Private Status Method
//
//To be called from static method getUARTsInactive()
//Returns the number of UART peripherals that are currently not being used (deregistered/inactive)
uint8_t QAD_UARTMgr::imp_getUARTsInactive(void) {
  uint8_t uCount = 0;
  for (uint8_t i=0; i<QAD_UART_PeriphCount; i++) {
  	if (!m_sUARTs[i].eState)
  		uCount++;
  }
  return uCount;
}











