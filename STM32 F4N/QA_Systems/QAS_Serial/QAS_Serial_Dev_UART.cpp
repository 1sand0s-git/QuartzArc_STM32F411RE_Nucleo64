/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Systems - Serial                                              */
/*   Role: Serial Device UART Class                                        */
/*   Filename: QAS_Serial_Dev_UART.cpp                                     */
/*   Date: 22th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAS_Serial_Dev_UART.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //------------------------------------------
  //QAS_Serial_Dev_UART Initialization Methods

//QAS_Serial_Dev_UART::imp_init
//QAS_Serial_Dev_UART Initialization Method
//
//Used too initialize the UART peripheral driver
//p - Unused in this implementation
//Returns QA_OK if driver initialization is successful, or an error if not successful (a member of QA_Result as defined in setup.hpp)
QA_Result QAS_Serial_Dev_UART::imp_init(void* p) {
	return m_pUART->init();
}


//QAS_Serial_Dev_UART::imp_deinit
//QAS_Serial_Dev_UART Initialization Method
//
//Used to deinitialize the UART peripheral driver
void QAS_Serial_Dev_UART::imp_deinit(void) {
  m_pUART->deinit();
}


	//---------------------------------------
	//QAS_Serial_Dev_UART IRQ Handler Methods

//QAS_Serial_Dev_UART::imp_handler
//QAS_Serial_Dev_UART IRQ Handler Method
//
//This method is only to be called by the interrupt request handler function from handlers.cpp
//p - Unused in this implementation
void QAS_Serial_Dev_UART::imp_handler(void* p) {
  UART_HandleTypeDef pHandle = m_pUART->getHandle();

  //RX Register Not Empty (RXNE)
  if (__HAL_UART_GET_FLAG(&pHandle, UART_FLAG_RXNE)) {
  	uint8_t uData = m_pUART->dataRX();
  	if (m_eRXState)
  		m_pRXFIFO->push(uData);
  	__HAL_UART_CLEAR_FLAG(&pHandle, UART_FLAG_RXNE);
  }

  //TX Register Empty (TXE)
  if (__HAL_UART_GET_FLAG(&pHandle, UART_FLAG_TXE)) {
  	if (!m_pTXFIFO->empty()) {
  		m_pUART->dataTX(m_pTXFIFO->pop());
  	} else {
      m_pUART->stopTX();
      m_eTXState = QA_Inactive;
  	}
  	__HAL_UART_CLEAR_FLAG(&pHandle, UART_FLAG_TXE);
  }
}


	//-----------------------------------
	//QAS_Serial_Dev_UART Control Methods

//QAS_Serial_Dev_UART::imp_txStart
//QAS_Serial_Dev_UART Control Method
//
//Used to start transmission of the UART peripheral
void QAS_Serial_Dev_UART::imp_txStart(void) {
  m_pUART->startTX();
}


//QAS_Serial_Dev_UART::imp_txStop
//QAS_Serial_Dev_UART Control Method
//
//Used to stop transmission of the UART peripheral
void QAS_Serial_Dev_UART::imp_txStop(void) {
  m_pUART->stopTX();
}


//QAS_Serial_Dev_UART::imp_rxStart
//QAS_Serial_Dev_UART Control Method
//
//Used to start receive of the UART peripheral
void QAS_Serial_Dev_UART::imp_rxStart(void) {
  m_pUART->startRX();
}


//QAS_Serial_Dev_UART::imp_rxStop
//QAS_Serial_Dev_UART Control Method
//
//Used to stop receive of the UART peripheral
void QAS_Serial_Dev_UART::imp_rxStop(void) {
  m_pUART->stopRX();
}




