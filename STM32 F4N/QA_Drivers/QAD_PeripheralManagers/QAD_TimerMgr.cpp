/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: Timer Management Driver                                         */
/*   Filename: QAD_TimerMgr.cpp                                            */
/*   Date: 18th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_TimerMgr.hpp"



  //-------------------------
	//QAD_TimerMgr Constructors

//QAD_TimerMgr::QAD_TimerMgr
//QAD_TimerMgr Constructor
QAD_TimerMgr::QAD_TimerMgr() {

  for (uint8_t i=0; i < QAD_Timer_PeriphCount; i++) {
  	m_sTimers[i].eState   = QAD_Timer_Unused;
  	m_sTimers[i].bEncoder = (i < QAD_Timer9);
  }

  //Set Timer Periph ID
  m_sTimers[QAD_Timer1].eTimer  = QAD_Timer1;
  m_sTimers[QAD_Timer2].eTimer  = QAD_Timer2;
  m_sTimers[QAD_Timer3].eTimer  = QAD_Timer3;
  m_sTimers[QAD_Timer4].eTimer  = QAD_Timer4;
  m_sTimers[QAD_Timer5].eTimer  = QAD_Timer5;
  m_sTimers[QAD_Timer9].eTimer  = QAD_Timer9;
  m_sTimers[QAD_Timer10].eTimer = QAD_Timer10;
  m_sTimers[QAD_Timer11].eTimer = QAD_Timer11;

	//Set clock speeds
	m_sTimers[QAD_Timer1].uClockSpeed  = HAL_RCC_GetPCLK2Freq();
	m_sTimers[QAD_Timer2].uClockSpeed  = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer3].uClockSpeed  = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer4].uClockSpeed  = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer5].uClockSpeed  = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer9].uClockSpeed  = HAL_RCC_GetPCLK2Freq();
	m_sTimers[QAD_Timer10].uClockSpeed = HAL_RCC_GetPCLK2Freq();
	m_sTimers[QAD_Timer11].uClockSpeed = HAL_RCC_GetPCLK2Freq();

	//Set types
	m_sTimers[QAD_Timer1].eType  = QAD_Timer_16bit;
	m_sTimers[QAD_Timer2].eType  = QAD_Timer_32bit;
	m_sTimers[QAD_Timer3].eType  = QAD_Timer_16bit;
	m_sTimers[QAD_Timer4].eType  = QAD_Timer_16bit;
	m_sTimers[QAD_Timer5].eType  = QAD_Timer_32bit;
	m_sTimers[QAD_Timer9].eType  = QAD_Timer_16bit;
	m_sTimers[QAD_Timer10].eType = QAD_Timer_16bit;
	m_sTimers[QAD_Timer11].eType = QAD_Timer_16bit;

	//Set channel counts
	m_sTimers[QAD_Timer1].uChannels  = 4;
	m_sTimers[QAD_Timer2].uChannels  = 4;
	m_sTimers[QAD_Timer3].uChannels  = 4;
	m_sTimers[QAD_Timer4].uChannels  = 4;
	m_sTimers[QAD_Timer5].uChannels  = 4;
	m_sTimers[QAD_Timer9].uChannels  = 2;
	m_sTimers[QAD_Timer10].uChannels = 1;
	m_sTimers[QAD_Timer11].uChannels = 1;

	//Set Instances
	m_sTimers[QAD_Timer1].pInstance  = TIM1;
	m_sTimers[QAD_Timer2].pInstance  = TIM2;
	m_sTimers[QAD_Timer3].pInstance  = TIM3;
	m_sTimers[QAD_Timer4].pInstance  = TIM4;
	m_sTimers[QAD_Timer5].pInstance  = TIM5;
	m_sTimers[QAD_Timer9].pInstance  = TIM9;
	m_sTimers[QAD_Timer10].pInstance = TIM10;
	m_sTimers[QAD_Timer11].pInstance = TIM11;

	//Set Update IRQs
	m_sTimers[QAD_Timer1].eIRQ_Update  = TIM1_UP_TIM10_IRQn;
	m_sTimers[QAD_Timer2].eIRQ_Update  = TIM2_IRQn;
	m_sTimers[QAD_Timer3].eIRQ_Update  = TIM3_IRQn;
	m_sTimers[QAD_Timer4].eIRQ_Update  = TIM4_IRQn;
	m_sTimers[QAD_Timer5].eIRQ_Update  = TIM5_IRQn;
	m_sTimers[QAD_Timer9].eIRQ_Update  = TIM1_BRK_TIM9_IRQn;
	m_sTimers[QAD_Timer10].eIRQ_Update = TIM1_UP_TIM10_IRQn;
	m_sTimers[QAD_Timer11].eIRQ_Update = TIM1_TRG_COM_TIM11_IRQn;

}


  //-------------------------------
  //QAD_TimerMgr Management Methods

//QAD_TimerMgr::imp_registerTimer
//QAD_TimerMgr Management Method
QA_Result QAD_TimerMgr::imp_registerTimer(QAD_Timer_Periph eTimer, QAD_Timer_State eState) {
  if (m_sTimers[eTimer].eState)
  	return QA_Error_PeriphBusy;

  m_sTimers[eTimer].eState = eState;
  return QA_OK;
}


//QAD_TimerMgr::imp_deregisterTimer
//QAD_TimerMgr Management Method
void QAD_TimerMgr::imp_deregisterTimer(QAD_Timer_Periph eTimer) {
  m_sTimers[eTimer].eState = QAD_Timer_Unused;
}


//QAD_TimerMgr::imp_findTimer
//QAD_TimerMgr Management Method
QAD_Timer_Periph QAD_TimerMgr::imp_findTimer(QAD_Timer_Type eType, bool bEncoder) {

	for (uint8_t i=0; i<QAD_Timer_PeriphCount; i++) {

		if (eType <= m_sTimers[i].eType) {
			if ((bEncoder && m_sTimers[i].bEncoder) || (!bEncoder)) {
				return m_sTimers[i].eTimer;
			}
		}
	}

	return QAD_TimerNone;
}


  //--------------------------
  //QAD_TimerMgr Clock Methods


//QAD_TimerMgr::imp_enableClock
//QAD_TimerMgr Clock Method
void QAD_TimerMgr::imp_enableClock(QAD_Timer_Periph eTimer) {
  switch (eTimer) {
    case (QAD_Timer1):
    	__HAL_RCC_TIM1_CLK_ENABLE();
      __HAL_RCC_TIM1_FORCE_RESET();
      __HAL_RCC_TIM1_RELEASE_RESET();
      break;
    case (QAD_Timer2):
    	__HAL_RCC_TIM2_CLK_ENABLE();
			__HAL_RCC_TIM2_FORCE_RESET();
			__HAL_RCC_TIM2_RELEASE_RESET();
      break;
    case (QAD_Timer3):
    	__HAL_RCC_TIM3_CLK_ENABLE();
			__HAL_RCC_TIM3_FORCE_RESET();
			__HAL_RCC_TIM3_RELEASE_RESET();
      break;
    case (QAD_Timer4):
    	__HAL_RCC_TIM4_CLK_ENABLE();
			__HAL_RCC_TIM4_FORCE_RESET();
			__HAL_RCC_TIM4_RELEASE_RESET();
      break;
    case (QAD_Timer5):
    	__HAL_RCC_TIM5_CLK_ENABLE();
			__HAL_RCC_TIM5_FORCE_RESET();
			__HAL_RCC_TIM5_RELEASE_RESET();
      break;
    case (QAD_Timer9):
    	__HAL_RCC_TIM9_CLK_ENABLE();
			__HAL_RCC_TIM9_FORCE_RESET();
			__HAL_RCC_TIM9_RELEASE_RESET();
      break;
    case (QAD_Timer10):
    	__HAL_RCC_TIM10_CLK_ENABLE();
			__HAL_RCC_TIM10_FORCE_RESET();
			__HAL_RCC_TIM10_RELEASE_RESET();
      break;
    case (QAD_Timer11):
    	__HAL_RCC_TIM11_CLK_ENABLE();
			__HAL_RCC_TIM11_FORCE_RESET();
			__HAL_RCC_TIM11_RELEASE_RESET();
      break;
    case (QAD_TimerNone):
    	break;
  }
}


//QAD_TimerMgr::imp_disableClock
//QAD_TimerMgr Clock Method
void QAD_TimerMgr::imp_disableClock(QAD_Timer_Periph eTimer) {
  switch (eTimer) {
    case (QAD_Timer1):
    	__HAL_RCC_TIM1_CLK_DISABLE();
      break;
    case (QAD_Timer2):
    	__HAL_RCC_TIM2_CLK_DISABLE();
      break;
    case (QAD_Timer3):
    	__HAL_RCC_TIM3_CLK_DISABLE();
      break;
    case (QAD_Timer4):
    	__HAL_RCC_TIM4_CLK_DISABLE();
      break;
    case (QAD_Timer5):
    	__HAL_RCC_TIM5_CLK_DISABLE();
      break;
    case (QAD_Timer9):
    	__HAL_RCC_TIM9_CLK_DISABLE();
      break;
    case (QAD_Timer10):
    	__HAL_RCC_TIM10_CLK_DISABLE();
      break;
    case (QAD_Timer11):
    	__HAL_RCC_TIM11_CLK_DISABLE();
      break;
    case (QAD_TimerNone):
    	break;
  }
}


  //---------------------------
  //QAD_TimerMgr Status Methods


//QAD_TimerMgr::imp_getTimersActive
//QAD_TimerMgr Status Method
uint8_t QAD_TimerMgr::imp_getTimersActive(void) {
  uint8_t uCount = 0;
  for (uint8_t i=0; i<QAD_Timer_PeriphCount; i++) {
  	if (m_sTimers[i].eState)
  		uCount++;
  }
  return uCount;
}


//QAD_TimerMgr::getTimersInactive
//QAD_TimerMgr Status Method
uint8_t QAD_TimerMgr::imp_getTimersInactive(void) {
  uint8_t uCount = 0;
  for (uint8_t i=0; i<QAD_Timer_PeriphCount; i++) {
  	if (!m_sTimers[i].eState)
  		uCount++;
  }
  return uCount;
}











