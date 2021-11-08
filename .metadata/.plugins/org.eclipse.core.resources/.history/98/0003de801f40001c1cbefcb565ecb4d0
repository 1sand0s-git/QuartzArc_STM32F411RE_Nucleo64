/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: UART Management Driver                                          */
/*   Filename: QAD_UARTMgr.hpp                                             */
/*   Date: 2nd November 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_UARTMGR_HPP_
#define __QAD_UARTMGR_HPP_

//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//---------------
//QAD_UART_Periph
//
//Used to select which UART peripheral is to be used, and index into peripheral array in UART manager
enum QAD_UART_Periph : uint8_t {
	QAD_UART1 = 0,
	QAD_UART2,
	QAD_UART6,
	QAD_UARTNone
};

//UART Peripheral Count
const uint8_t QAD_UART_PeriphCount = QAD_UARTNone;


//--------------
//QAD_UART_State
//
//Used to store whether a particular UART peripheral is in use, or not currently being used
enum QAD_UART_State : uint8_t {
	QAD_UART_Unused = 0,
	QAD_UART_InUse,
	QAD_UART_InvalidDevice
};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//-------------
//QAD_UART_Data
typedef struct {

	QAD_UART_Periph   eUART;      //Used to store which UART peripheral is represented by the structure, used for manager methods that find usused UARTs

	QAD_UART_State    eState;     //Stores whether the UART peripheral is currently being used or not

	USART_TypeDef*    pInstance;  //Stores the USART_TypeDef for the UART peripheral (defined in stm32f411xe.h)

	IRQn_Type         eIRQ;       //Stores the IRQ Handler enum for the UART peripheral (defined in stm32f411xe.h)

} QAD_UART_Data;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//-----------
//QAD_UARTMgr
//
//Singleton class
//Used to allow management of UART peripherals in order to make sure that a driver is prevented from accessing any UART devices
//that are already being used by another driver
class QAD_UARTMgr {
private:

	//UART Peripheral Data
	QAD_UART_Data m_sUARTs[QAD_UART_PeriphCount];

	//------------
	//Constructors
	QAD_UARTMgr();

public:

	//-----------------------------------------------
	//Delete copy constructor and assignment operator
	QAD_UARTMgr(const QAD_UARTMgr& other) = delete;
	QAD_UARTMgr& operator=(const QAD_UARTMgr& other) = delete;


	//-----------------
	//Singleton Methods
	//
	//Used to retrieve a reference to the singleton class
	static QAD_UARTMgr& get(void) {
		static QAD_UARTMgr instance;
		return instance;
	}


	//------------
	//Data Methods

	//Used to retrieve the current state (QAD_UART_InUse, QAD_UART_Unused) of a UART peripheral
	//eUART - The UART peripheral to retrieve the state for. Member of QAD_UART_Periph
	//Returns member of QAD_UART_State enum (QAD_UART_InUse, QAD_UART_Unused)
	static QAD_UART_State getState(QAD_UART_Periph eUART) {
		if (eUART >= QAD_UARTNone)
			return QAD_UART_InvalidDevice;

		return get().m_sUARTs[eUART].eState;
	}

	//Used to retrieve an instance for a UART peripheral
	//eUART - The UART peripheral to retrieve the instance for. Member of QAD_UART_Periph
	//Returns USART_TypeDef, as defined in stm32f411xe.h
	static USART_TypeDef* getInstance(QAD_UART_Periph eUART) {
		if (eUART >= QAD_UARTNone)
			return NULL;

		return get().m_sUARTs[eUART].pInstance;
	}

	//Used to retrieve an IRQ enum for a UART peripheral
	//eUART - The UART peripheral to retrieve the IRQ enum for. Member of QAD_UART_Periph
	//Returns member of IRQn_Type enum, as defined in stm32f411xe.h
	static IRQn_Type getIRQ(QAD_UART_Periph eUART) {
		if (eUART >= QAD_UARTNone)
			return UsageFault_IRQn;

		return get().m_sUARTs[eUART].eIRQ;
	}


	//-------------------
	//Managemenet Methods

	//Used to register a UART peripheral as being used by a driver
	//eUART - the UART peripheral to be registered
	//Returns QA_OK if registration is successful, or returns QA_Error_PeriphBusy if the selected UART is already in use
	static QA_Result registerUART(QAD_UART_Periph eUART) {
		return get().imp_registerUART(eUART);
	}

	//Used to deregister a UART peripheral to mark it as no longer being used by a driver
	//eUART - the UART peripheral to be deregistered
	static void deregisterUART(QAD_UART_Periph eUART) {
		get().imp_deregisterUART(eUART);
	}


	//-------------
  //Clock Methods

	//Used to enable the clock for a specific UART peripheral
	//eUART - the UART peripheral to enable the clock for
	static void enableClock(QAD_UART_Periph eUART) {
		get().imp_enableClock(eUART);
	}

	//Used to disable the clock for a specific UART peripheral
	//eUART - the UART peripheral to disable the clock for
	static void disableClock(QAD_UART_Periph eUART) {
		get().imp_disableClock(eUART);
	}


	//--------------
	//Status Methods

	//Returns the number of UART peripherals that are currently in-use (registered/active)
	static uint8_t getUARTsActive(void) {
		return get().imp_getUARTsActive();
	}

	//Returns the number of UART peripherals that are currently not being used (deregistered/inactive)
	static uint8_t getUARTsInactive(void) {
	  return get().imp_getUARTsInactive();
	}


private:

	//NOTE: See QAD_UARTMgr.cpp for details of the following methods

	//------------------
	//Management Methods
	QA_Result imp_registerUART(QAD_UART_Periph eUART);
	void imp_deregisterUART(QAD_UART_Periph eUART);


	//-------------
	//Clock Methods
	void imp_enableClock(QAD_UART_Periph eUART);
	void imp_disableClock(QAD_UART_Periph eUART);


	//--------------
	//Status Methods
	uint8_t imp_getUARTsActive(void);
	uint8_t imp_getUARTsInactive(void);

};



//Prevent Recursive Inclusion
#endif /* __QAD_UARTMGR_HPP */














