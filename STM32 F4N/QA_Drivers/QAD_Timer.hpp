/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: Timer Driver                                                    */
/*   Filename: QAD_Timer.hpp                                               */
/*   Date: 19th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_TIMER_HPP_
#define __QAD_TIMER_HPP_

//Includes
#include "setup.hpp"

#include "QAD_TimerMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------------
//QAD_TimerMode
enum TimerMode : uint8_t {TimerContinuous = 0, TimerMultiple, TimerSingle};


//--------------------
//QAD_Timer_InitStruct
typedef struct {

	QAD_Timer_Periph eTimer;

	TimerMode        eMode;

	uint32_t         uPrescaler;
	uint32_t         uPeriod;

	uint8_t          uIRQPriority;

	uint16_t         uCounterTarget;

} QAD_Timer_InitStruct;


//---------
//QAD_Timer
class QAD_Timer {
private:
	enum DeinitMode : uint8_t {DeinitPartial = 0, DeinitFull};

	QAD_Timer_Periph  m_eTimer;
	TIM_HandleTypeDef m_sHandle;

	TimerMode         m_eMode;

	uint32_t          m_uPrescaler;
	uint32_t          m_uPeriod;

	uint8_t           m_uIRQPriority;

	QA_InitState      m_eInitState;

	QA_ActiveState    m_eState;

	IRQn_Type         m_eIRQ;

	QAD_IRQHandler_CallbackFunction m_pHandlerFunction;
	QAD_IRQHandler_CallbackClass*   m_pHandlerClass;

	uint16_t          m_uIRQCounterTarget;
	uint16_t          m_uIRQCounterValue;

public:

	QAD_Timer() = delete;
	QAD_Timer(QAD_Timer_InitStruct& sInit) :
		m_eTimer(sInit.eTimer),
		m_sHandle({0}),
		m_eMode(sInit.eMode),
		m_uPrescaler(sInit.uPrescaler),
		m_uPeriod(sInit.uPeriod),
		m_uIRQPriority(sInit.uIRQPriority),
		m_eInitState(QA_NotInitialized),
		m_eState(QA_Inactive),
		m_pHandlerFunction(NULL),
		m_pHandlerClass(NULL),
		m_uIRQCounterTarget(sInit.uCounterTarget),
		m_uIRQCounterValue(0) {}

	~QAD_Timer() {
		if (m_eState)
			stop();
		if (m_eInitState)
			deinit();
	}

	//Initialization Methods
	QA_Result init(void);
	void deinit(void);

	//IRQ Handler Methods
	void handler(void);

	//Control Methods
  void setHandlerFunction(QAD_IRQHandler_CallbackFunction pHandler);
  void setHandlerClass(QAD_IRQHandler_CallbackClass* pHandler);

  void setTimerMode(TimerMode eMode);
  TimerMode getTimerMode(void);

  void setIRQCounterTarget(uint16_t uTarget);
  uint16_t getIRQCounterTarget(void);

  uint16_t getIRQCounterValue(void);
  void clearIRQCounterValue(void);

  void start(void);
  void stop(void);

private:

  //Private Initialization Methods
  QA_Result periphInit(void);
  void periphDeinit(DeinitMode eMode);

};


//Prevent Recursive Inclusion
#endif /* __QAD_TIMER_HPP */









