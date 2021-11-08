/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Systems - Serial                                              */
/*   Role: Serial Device Base Class                                        */
/*   Filename: QAS_Serial_Dev_Base.hpp                                     */
/*   Date: 22th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAS_SERIAL_DEV_BASE_HPP_
#define __QAS_SERIAL_DEV_BASE_HPP_

//Includes
#include "setup.hpp"

#include <memory>
#include <string.h>

#include "QAT_FIFO.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------------------
//QAS_Serial_Dev_Base
//
//This base class is intended to be inherited by classes that implement serial functionality from particular serial peripherals,
//or serial protocols, such as the QAS_Serial_Dev_UART class, or serial streaming to/from files and is not intended to be used standalone.
class QAS_Serial_Dev_Base {
public:
	enum DataState : uint8_t {NoData = 0, HasData};
	enum DeviceType : uint8_t {DT_UART = 0, DT_File = 1, DT_Unknown};

public:

	std::unique_ptr<QAT_FIFOBuffer> m_pTXFIFO;  //Circular FIFO buffer class to store data to be transmitted (implemented in QAT_FIFO.hpp)
	std::unique_ptr<QAT_FIFOBuffer> m_pRXFIFO;  //Circular FIFO buffer class to store data that has been received (implemented in QAT_FIFO.hpp)

	QA_InitState m_eInitState;  //Stores whether the class is currently initialized or not.

	QA_ActiveState m_eTXState;  //Stores whether the transmit component is currently active. Member of QA_ActiveState enum defined in setup.hpp
	QA_ActiveState m_eRXState;  //Stores whether the receive component is currently active. Member of QA_ActiveState enum defined in setup.hpp

	DeviceType  m_eDeviceType;  //Stores the current type of serial device. Member of DeviceType enum defined above.

public:

	//--------------------------
	//Constructors / Destructors

	QAS_Serial_Dev_Base() = delete;  //Delete the default class constructor, as we need initialization settings to be provided on class creation


	//Main class contructor
	//uTXFIFOSize - the size in bytes for the TX FIFO buffer
	//uRXFIFOSize - the size in bytes for the RX FIFO buffer
	//eDeviceType - A member of the DeviceType enum to define what type of serial device is being used
	QAS_Serial_Dev_Base(uint16_t uTXFIFOSize, uint16_t uRXFIFOSize, DeviceType eDeviceType) : //The class constructor to be used,
		                                                                                        //which is provided with FIFO sizes and device type details
		m_pTXFIFO(std::make_unique<QAT_FIFOBuffer>(uTXFIFOSize)),   //Create TX FIFO class with size in bytes provided in uTXFIFOSize
		m_pRXFIFO(std::make_unique<QAT_FIFOBuffer>(uRXFIFOSize)),   //Create RX FIFO class with size in bytes provided in uRXFIFOSize
		m_eInitState(QA_NotInitialized),                            //Set Init State to not initialized
		m_eTXState(QA_Inactive),                                    //Set TX State to inactive
		m_eRXState(QA_Inactive),                                    //Set RX State to inactive
		m_eDeviceType(eDeviceType) {}                               //Set



	//NOTE: See QAS_Serial_Dev_Base.cpp for details on the following methods

	//----------------------
	//Initialization Methods

	QA_Result init(void* p);
	void deinit(void);


	//-------------------------
	//Interrupt Handler Methods

	void handler(void* p);


	//---------------
	//Control Methods

	DeviceType getType(void);

	void rxStart(void);
	void rxStop(void);


	//----------------
	//Transmit Methods

	void txString(const char* str);
	void txStringCR(const char* str);
	void txCR(void);
	void txData(const uint8_t* pData, uint16_t uSize);


	//---------------
	//Receive Methods

	DataState rxHasData(uint16_t* uSize);

	uint8_t rxPop(void);
	QA_Result rxData(uint8_t* pData, uint16_t* uSize);

private:

	//----------------------
	//Initialization Methods

	virtual QA_Result imp_init(void* p) = 0;  //Pure virtual function to be implemented by inheriting class
	virtual void imp_deinit(void) = 0;        //Pure virtual function to be implemented by inheriting class


	//---------------------------------
	//Interrupt Request Handler Methods

	virtual void imp_handler(void* p) = 0;    //Pure virtual function to be implemented by inheriting class


	//---------------
	//Control Methods

	virtual void imp_txStart(void) = 0;       //Pure virtual function to be implemented by inheriting class
	virtual void imp_txStop(void) = 0;        //Pure virtual function to be implemented by inheriting class
	virtual void imp_rxStart(void) = 0;       //Pure virtual function to be implemented by inheriting class
	virtual void imp_rxStop(void) = 0;        //Pure virtual function to be implemented by inheriting class

};


//Prevent Recursive Inclusion
#endif /* __QAS_SERIAL_DEV_BASE_HPP_ */
