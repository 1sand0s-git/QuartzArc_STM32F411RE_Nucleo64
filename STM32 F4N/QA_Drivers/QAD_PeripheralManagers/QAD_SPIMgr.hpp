/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: SPI Management Driver                                           */
/*   Filename: QAD_SPIMgr.hpp                                              */
/*   Date: 13th December 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_SPIMGR_HPP_
#define __QAD_SPIMGR_HPP_

//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
  //------------------------------------------


//--------------
//QAD_SPI_Periph
enum QAD_SPI_Periph : uint8_t {
	QAD_SPI1 = 0,
	QAD_SPI2,
	QAD_SPI3,
	QAD_SPI4,
	QAD_SPI5,
	QAD_SPINone
};

//SPI Peripheral Count
const uint8_t QAD_SPI_PeriphCount = QAD_SPINone;


//------------
//QAD_SPIState
enum QAD_SPI_State : uint8_t {
	QAD_SPI_Unused = 0,
	QAD_SPI_InUse_SPI,
	QAD_SPI_InUse_I2S,
	QAD_SPI_InvalidDevice
};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//------------
//QAD_SPI_Data
typedef struct {

	QAD_SPI_Periph     eSPI;

	QAD_SPI_State      eState;

	SPI_TypeDef*       pInstance;

	IRQn_Type          eIRQ;

	bool               bI2S;

} QAD_SPI_Data;


//------------------------------------------
//------------------------------------------
//------------------------------------------


//----------
//QAD_SPIMgr
class QAD_SPIMgr {
private:

	//SPI Peripheral Data
	QAD_SPI_Data m_sSPIs[QAD_SPI_PeriphCount];


	//------------
	//Constructors

	QAD_SPIMgr();

public:

	//------------------------------------------------------------------------------
	//Delete copy constructor and assignment operator due to being a singleton class
	QAD_SPIMgr(const QAD_SPIMgr& other) = delete;
	QAD_SPIMgr& operator=(const QAD_SPIMgr& other) = delete;


	//-----------------
	//Singleton Methods
	//
	//Used to retrieve a reference to the singleton class
	static QAD_SPIMgr& get(void) {
		static QAD_SPIMgr instance;
		return instance;
	}


	//------------
	//Data Methods

	static QAD_SPI_State getState(QAD_SPI_Periph eSPI) {
		if (eSPI >= QAD_SPINone)
			return QAD_SPI_InvalidDevice;

		return get().m_sSPIs[eSPI].eState;
	}


	static SPI_TypeDef* getInstance(QAD_SPI_Periph eSPI) {
		if (eSPI >= QAD_SPINone)
			return NULL;

		return get().m_sSPIs[eSPI].pInstance;
	}


	static IRQn_Type getIRQ(QAD_SPI_Periph eSPI) {
		if (eSPI >= QAD_SPINone)
			return UsageFault_IRQn;

		return get().m_sSPIs[eSPI].eIRQ;
	}


	static bool getI2S(QAD_SPI_Periph eSPI) {
		if (eSPI >= QAD_SPINone)
			return false;

		return get().m_sSPIs[eSPI].bI2S;
	}


	//------------------
	//Management Methods

	static QA_Result registerSPI(QAD_SPI_Periph eSPI, QAD_SPI_State eState) {
		return get().imp_registerSPI(eSPI, eState);
	}

	static void deregisterSPI(QAD_SPI_Periph eSPI) {
		get().imp_deregisterSPI(eSPI);
	}


	//-------------
	//Clock Methods

	static void enableClock(QAD_SPI_Periph eSPI) {
		get().imp_enableClock(eSPI);
	}

	static void disableClock(QAD_SPI_Periph eSPI) {
		get().imp_disableClock(eSPI);
	}


	//--------------
	//Status Methods
	static uint8_t getSPIsActive(void) {
		return get().imp_getSPIsActive();
	}

	static uint8_t getSPIsInactive(void) {
		return get().imp_getSPIsInactive();
	}


private:

	//------------------
	//Management Methods
	QA_Result imp_registerSPI(QAD_SPI_Periph eSPI, QAD_SPI_State eState);
	void imp_deregisterSPI(QAD_SPI_Periph eSPI);


	//-------------
	//Clock Methods
	void imp_enableClock(QAD_SPI_Periph eSPI);
	void imp_disableClock(QAD_SPI_Periph eSPI);


	//--------------
	//Status Methods
	uint8_t imp_getSPIsActive(void);
	uint8_t imp_getSPIsInactive(void);

};


//Prevent Recursive Inclusion
#endif /* __QAD_SPIMGR_HPP_ */
