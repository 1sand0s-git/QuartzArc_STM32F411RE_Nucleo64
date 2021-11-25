/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: I2C Management Driver                                           */
/*   Filename: QAD_I2CMgr.hpp                                              */
/*   Date: 25th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_I2CMGR_HPP_
#define __QAD_I2CMGR_HPP_

//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//--------------
//QAD_I2C_Periph
enum QAD_I2C_Periph : uint8_t {
	QAD_I2C1 = 0,
	QAD_I2C2,
	QAD_I2C3,
	QAD_I2CNone
};


//I2C Peripheral Count
const uint8_t QAD_I2C_PeriphCount = QAD_I2CNone;


//-------------
//QAD_I2C_State
enum QAD_I2C_State : uint8_t {
	QAD_I2C_Unused = 0,
	QAD_I2C_InUse_Master,
	QAD_I2C_InUse_Slave,
	QAD_I2C_InUse_Multi,
	QAD_I2C_InvalidDevice
};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//------------
//QAD_I2C_Data
typedef struct {

	QAD_I2C_Periph    eI2C;

	QAD_I2C_State     eState;

	I2C_TypeDef*      pInstance;

	IRQn_Type         eIRQ_Event;
	IRQn_Type         eIRQ_Error;

} QAD_I2C_Data;


//------------------------------------------
//------------------------------------------
//------------------------------------------


//----------
//QAD_I2CMgr
class QAD_I2CMgr {
private:

	//I2C Peripheral Data
	QAD_I2C_Data m_sI2Cs[QAD_I2C_PeriphCount];


	//------------
	//Constructors

	QAD_I2CMgr();

public:

	//------------------------------------------------------------------------------
	//Delete copy constructor and assignment operator due to being a singleton class
	QAD_I2CMgr(const QAD_I2CMgr& other) = delete;
	QAD_I2CMgr& operator=(const QAD_I2CMgr& other) = delete;


	//-----------------
	//Singleton Methods
	static QAD_I2CMgr& get(void) {
		static QAD_I2CMgr instance;
		return instance;
	}


	//------------
	//Data Methods

	static QAD_I2C_State getState(QAD_I2C_Periph eI2C) {
		if (eI2C >= QAD_I2CNone)
			return QAD_I2C_InvalidDevice;

		return get().m_sI2Cs[eI2C].eState;
	}

	static I2C_TypeDef* getInstance(QAD_I2C_Periph eI2C) {
		if (eI2C >= QAD_I2CNone)
			return NULL;

		return get().m_sI2Cs[eI2C].pInstance;
	}


	static IRQn_Type getIRQEvent(QAD_I2C_Periph eI2C) {
		if (eI2C >= QAD_I2CNone)
			return UsageFault_IRQn;

		return get().m_sI2Cs[eI2C].eIRQ_Event;
	}

	static IRQn_Type getIRQError(QAD_I2C_Periph eI2C) {
		if (eI2C >= QAD_I2CNone)
			return UsageFault_IRQn;

		return get().m_sI2Cs[eI2C].eIRQ_Error;
	}


	//------------------
	//Management Methods

	static QA_Result registerI2C(QAD_I2C_Periph eI2C, QAD_I2C_State eMode) {
		return get().imp_registerI2C(eI2C, eMode);
	}

	static void deregisterI2C(QAD_I2C_Periph eI2C) {
		get().imp_deregisterI2C(eI2C);
	}


	//-------------
	//Clock Methods

	static void enableClock(QAD_I2C_Periph eI2C) {
		get().imp_enableClock(eI2C);
	}

	static void disableClock(QAD_I2C_Periph eI2C) {
		get().imp_disableClock(eI2C);
	}


	//--------------
	//Status Methods

	static uint8_t getI2CsActive(void) {
		return get().imp_getI2CsActive();
	}

	static uint8_t getI2CsInactive(void) {
		return get().imp_getI2CsInactive();
	}

private:

	//------------------
	//Management Methods
	QA_Result imp_registerI2C(QAD_I2C_Periph eI2C, QAD_I2C_State eMode);
	void imp_deregisterI2C(QAD_I2C_Periph eI2C);


	//-------------
	//Clock Methods
	void imp_enableClock(QAD_I2C_Periph eI2C);
	void imp_disableClock(QAD_I2C_Periph eI2C);


	//--------------
	//Status Methods
	uint8_t imp_getI2CsActive(void);
	uint8_t imp_getI2CsInactive(void);

};


//Prevent Recursive Inclusion
#endif /* __QAD_I2CMGR_HPP_ */
