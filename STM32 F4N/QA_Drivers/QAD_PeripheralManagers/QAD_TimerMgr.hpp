/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: Timer Management Driver                                         */
/*   Filename: QAD_TimerMgr.hpp                                            */
/*   Date: 18th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_TIMERMGR_HPP_
#define __QAD_TIMERMGR_HPP_

//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//----------------
//QAD_Timer_Periph
enum QAD_Timer_Periph : uint8_t {
	QAD_Timer1 = 0,
	QAD_Timer2,
	QAD_Timer3,
	QAD_Timer4,
	QAD_Timer5,
	QAD_Timer9,
	QAD_Timer10,
	QAD_Timer11,
	QAD_TimerNone
};

//Timer Peripheral Count
const uint8_t QAD_Timer_PeriphCount = QAD_TimerNone;


//---------------
//QAD_Timer_State
enum QAD_Timer_State : uint8_t {
	QAD_Timer_Unused = 0,
	QAD_Timer_InUse_IRQ,
	QAD_Timer_InUse_Encoder,
	QAD_Timer_InUse_PWM,
	QAD_Timer_InUse_ADC
};


//--------------
//QAD_Timer_Type
enum QAD_Timer_Type : uint8_t {QAD_Timer_16bit = 0, QAD_Timer_32bit};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//--------------
//QAD_Timer_Data
typedef struct {

	QAD_Timer_Periph  eTimer;

	QAD_Timer_State   eState;

	uint32_t          uClockSpeed;
	QAD_Timer_Type    eType;
	uint8_t           uChannels;

	bool              bEncoder;

	TIM_TypeDef*      pInstance;

	IRQn_Type         eIRQ_Update;

} QAD_Timer_Data;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//------------
//QAD_TimerMgr
class QAD_TimerMgr {
private:

	//Timer Data
	QAD_Timer_Data  m_sTimers[QAD_Timer_PeriphCount];

	//Constructors
	QAD_TimerMgr();

public:

	//Delete copy constructor and assignment operator
	QAD_TimerMgr(const QAD_TimerMgr& other) = delete;
	QAD_TimerMgr& operator=(const QAD_TimerMgr& other) = delete;

	//Singleton Methods
	static QAD_TimerMgr& get(void) {
		static QAD_TimerMgr instance;
		return instance;
	}

  //Data Methods
	static QAD_Timer_State getState(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].eState;
	}

	static uint32_t getClockSpeed(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].uClockSpeed;
	}

	static QAD_Timer_Type getType(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].eType;
	}

	static uint8_t getChannels(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].uChannels;
	}

	static bool getEncoder(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].bEncoder;
	}

	static TIM_TypeDef* getInstance(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].pInstance;
	}

	static IRQn_Type getUpdateIRQ(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].eIRQ_Update;
	}


	//Management Methods
	static QA_Result registerTimer(QAD_Timer_Periph eTimer, QAD_Timer_State eState) {
		return get().imp_registerTimer(eTimer, eState);
	}

	static void deregisterTimer(QAD_Timer_Periph eTimer) {
		get().imp_deregisterTimer(eTimer);
	}

	static QAD_Timer_Periph findTimer(QAD_Timer_Type eType, bool bEncoder) {
		return get().imp_findTimer(eType, bEncoder);
	}


	//Clock Methods
	static void enableClock(QAD_Timer_Periph eTimer) {
		get().imp_enableClock(eTimer);
	}

	static void disableClock(QAD_Timer_Periph eTimer) {
		get().imp_disableClock(eTimer);
	}


	//Status Methods
	static uint8_t getTimersActive(void) {
		return get().imp_getTimersActive();
	}

	static uint8_t getTimersInactive(void) {
		return get().imp_getTimersInactive();
	}


private:

	//Management Methods
  QA_Result imp_registerTimer(QAD_Timer_Periph eTimer, QAD_Timer_State eState);
  void imp_deregisterTimer(QAD_Timer_Periph eTimer);
  QAD_Timer_Periph imp_findTimer(QAD_Timer_Type eType, bool bEncoder);

  //Clock Methods
  void imp_enableClock(QAD_Timer_Periph eTimer);
  void imp_disableClock(QAD_Timer_Periph eTimer);

  //Status Methods
  uint8_t imp_getTimersActive(void);
  uint8_t imp_getTimersInactive(void);

};


//Prevent Recursive Inclusion
#endif /* __QAD_TIMERMGR_HPP_ */
