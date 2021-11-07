/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: Timer Driver                                                    */
/*   Filename: QAD_Timer.cpp                                               */
/*   Date: 19th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_Timer.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


  //--------------------------------
  //QAD_Timer Initialization Methods

//QAD_Timer::init
//QAD_Timer Initialization Method
QA_Result QAD_Timer::init(void) {
  if (QAD_TimerMgr::getState(m_eTimer))
  	return QA_Error_PeriphBusy;

  QAD_TimerMgr::registerTimer(m_eTimer, QAD_Timer_InUse_IRQ);
  QA_Result eRes = periphInit();

  if (eRes)
  	QAD_TimerMgr::deregisterTimer(m_eTimer);
  return eRes;
}


//QAD_Timer::deinit
//QAD_Timer Initialization Method
void QAD_Timer::deinit(void) {
  if (!m_eInitState)
  	return;

  periphDeinit(DeinitFull);
  QAD_TimerMgr::deregisterTimer(m_eTimer);
}


  //-----------------------------
  //QAD_Timer IRQ Handler Methods

//QAD_Timer::handler
//QAD_Timer IRQ Handler Method
void QAD_Timer::handler(void) {
  if (__HAL_TIM_GET_FLAG(&m_sHandle, TIM_FLAG_UPDATE)) {

  	switch (m_eMode) {
  	  case (TimerContinuous):
  	  	break;
  	  case (TimerMultiple):
  	  	m_uIRQCounterValue++;
  	    if (m_uIRQCounterValue >= m_uIRQCounterTarget) {
  	    	stop();
  	    }
  	    break;
  	  case (TimerSingle):
  	  	stop();
  	    break;
  	}

  	if (m_pHandlerFunction)
  		m_pHandlerFunction(NULL);
  	if (m_pHandlerClass)
  		m_pHandlerClass->handler(NULL);

  	__HAL_TIM_CLEAR_FLAG(&m_sHandle, TIM_FLAG_UPDATE);
  }
}


  //-------------------------
  //QAD_Timer Control Methods

//QAD_Timer::setHandlerFunction
//QAD_Timer Control Method
void QAD_Timer::setHandlerFunction(QAD_IRQHandler_CallbackFunction pHandler) {
  m_pHandlerFunction = pHandler;
}


//QAD_Timer::setHandlerClass
//QAD_Timer Control Method
void QAD_Timer::setHandlerClass(QAD_IRQHandler_CallbackClass* pHandler) {
  m_pHandlerClass = pHandler;
}


//QAD_Timer::setTimerMode
//QAD_Timer Control Method
void QAD_Timer::setTimerMode(TimerMode eMode) {
  m_eMode = eMode;
}


//QAD_Timer::getTimerMode
//QAD_Timer Control Method
TimerMode QAD_Timer::getTimerMode(void) {
  return m_eMode;
}


//QAD_Timer::setIRQCounterTarget
//QAD_Timer Control Method
void QAD_Timer::setIRQCounterTarget(uint16_t uTarget) {
  m_uIRQCounterTarget = uTarget;
}


//QAD_Timer::getIRQCounterTarget
//QAD_Timer Control Method
uint16_t QAD_Timer::getIRQCounterTarget(void) {
  return m_uIRQCounterTarget;
}


//QAD_Timer::getIRQCounterValue
//QAD_Timer Control Method
uint16_t QAD_Timer::getIRQCounterValue(void) {
  return m_uIRQCounterValue;
}


//QAD_Timer::clearIRQCounterValue
//QAD_Timer Control Method
void QAD_Timer::clearIRQCounterValue(void) {
  m_uIRQCounterValue = 0;
}


//QAD_Timer::start
//QAD_Timer Control Method
void QAD_Timer::start(void) {
  if ((m_eInitState) && (!m_eState)) {

  	m_uIRQCounterValue = 0;

  	__HAL_TIM_ENABLE_IT(&m_sHandle, TIM_IT_UPDATE);
  	__HAL_TIM_ENABLE(&m_sHandle);

  	m_eState = QA_Active;
  }
}


//QAD_Timer::stop
//QAD_Timer Control Method
void QAD_Timer::stop(void) {
  if ((m_eInitState) && (m_eState)) {

  	__HAL_TIM_DISABLE(&m_sHandle);
  	__HAL_TIM_DISABLE_IT(&m_sHandle, TIM_IT_UPDATE);

  	m_eState = QA_Inactive;
  }
}


  //----------------------------------------
  //QAD_Timer Private Initialization Methods

//QAD_Timer::periphInit
//QAD_Timer Private Initialization Method
QA_Result QAD_Timer::periphInit(void) {

	//Enable Timer Clock
	QAD_TimerMgr::enableClock(m_eTimer);

	//Initialize Timer Base
	m_sHandle.Instance               = QAD_TimerMgr::getInstance(m_eTimer);
	m_sHandle.Init.Prescaler         = m_uPrescaler;
	m_sHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	m_sHandle.Init.Period            = m_uPeriod;
	m_sHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	m_sHandle.Init.RepetitionCounter = 0x0;
	m_sHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&m_sHandle) != HAL_OK) {
		periphDeinit(DeinitPartial);
		return QA_Fail;
	}

	//Setup IRQ
	m_eIRQ = QAD_TimerMgr::getUpdateIRQ(m_eTimer);
	HAL_NVIC_SetPriority(m_eIRQ, m_uIRQPriority, 0);
	HAL_NVIC_EnableIRQ(m_eIRQ);

	//Set States
	m_eState = QA_Inactive;
	m_eInitState = QA_Initialized;

	//Return
	return QA_OK;
}


//QAD_Timer::periphDeinit
//QAD_Timer Private Initialization Method
void QAD_Timer::periphDeinit(QAD_Timer::DeinitMode eMode) {

	if (eMode) {
		HAL_NVIC_DisableIRQ(m_eIRQ);

		HAL_TIM_Base_DeInit(&m_sHandle);
	}

	//Disable Timer Clock
	QAD_TimerMgr::disableClock(m_eTimer);

	//Set States
	m_eState = QA_Inactive;
	m_eInitState = QA_NotInitialized;
}














