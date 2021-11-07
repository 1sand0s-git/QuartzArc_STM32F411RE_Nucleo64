/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Systems - Serial                                              */
/*   Role: Serial Device UART Class                                        */
/*   Filename: QAS_Serial_Dev_UART.hpp                                     */
/*   Date: 22th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAS_SERIAL_DEV_UART_HPP_
#define __QAS_SERIAL_DEV_UART_HPP_

//Includes
#include "setup.hpp"

#include <memory>
#include <string.h>

#include "QAT_FIFO.hpp"
#include "QAS_Serial_Dev_Base.hpp"
#include "QAD_UART.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//------------------------------
//QAS_Serial_Dev_UART_InitStruct
typedef struct {

	QAD_UART_InitStruct uart;

	uint16_t            txfifo_size;
	uint16_t            rxfifo_size;

} QAS_Serial_Dev_UART_InitStruct;



	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------------------
//QAS_Serial_Dev_UART
class QAS_Serial_Dev_UART : public QAS_Serial_Dev_Base {
private:

	QAD_UART_Periph           m_ePeriph;

	std::unique_ptr<QAD_UART> m_pUART;

public:

	QAS_Serial_Dev_UART() = delete;
  QAS_Serial_Dev_UART(QAS_Serial_Dev_UART_InitStruct& sInit) :
  	QAS_Serial_Dev_Base(sInit.txfifo_size, sInit.rxfifo_size, DT_UART),
		m_ePeriph(sInit.uart.uart),
		m_pUART(std::make_unique<QAD_UART>(sInit.uart)) {}

private:

  QA_Result imp_init(void* p) override;
  void imp_deinit(void) override;

  void imp_handler(void* p) override;

  void imp_txStart(void) override;
  void imp_txStop(void) override;
  void imp_rxStart(void) override;
  void imp_rxStop(void) override;

};


//Prevent Recursive Inclusion
#endif /* __QAS_SERIAL_DEV_UART_HPP_ */
