/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: PWM Driver                                                      */
/*   Filename: QAD_PWM.cpp                                                 */
/*   Date: 1st November 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_PWM.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


  //------------------------------
  //QAD_PWM Initialization Methods

//QAD_PWM::init
//QAD_PWM Initialization Method
QA_Result QAD_PWM::init(void) {
  if (QAD_TimerMgr::getState(m_eTimer))
  	return QA_Error_PeriphBusy;

  QAD_TimerMgr::registerTimer(m_eTimer, QAD_Timer_InUse_PWM);
  QA_Result eRes = periphInit();

  if (eRes)
  	QAD_TimerMgr::deregisterTimer(m_eTimer);
  return eRes;
}


//QAD_PWM::deinit
//QAD_PWM Initialization Method
void QAD_PWM::deinit(void) {
  if (!m_eInitState)
  	return;

  periphDeinit(DeinitFull);
  QAD_TimerMgr::deregisterTimer(m_eTimer);
}


  //-----------------------
  //QAD_PWM Control Methods

//QAD_PWM::start
//QAD_PWM Control Method
void QAD_PWM::start(void) {

	for (uint8_t i=0; i<QAD_TimerMgr::getChannels(m_eTimer); i++) {
		if (m_sChannels[i].eActive)
			HAL_TIM_PWM_Start(&m_sHandle, m_uChannelSelect[i]);
	}

	m_eState = QA_Active;
}


//QAD_PWM::stop
//QAD_PWM Control Method
void QAD_PWM::stop(void) {

	for (uint8_t i=0; i<QAD_TimerMgr::getChannels(m_eTimer); i++) {
		if (m_sChannels[i].eActive)
			HAL_TIM_PWM_Stop(&m_sHandle, m_uChannelSelect[i]);
	}

	m_eState = QA_Inactive;
}


//QAD_PWM::setPWMVal
//QAD_PWM Control Method
void QAD_PWM::setPWMVal(QAD_PWM::PWM_Channel eChannel, uint16_t uVal) {
  if (eChannel >= QAD_TimerMgr::getChannels(m_eTimer))
  	return;

  __HAL_TIM_SET_COMPARE(&m_sHandle, m_uChannelSelect[eChannel], uVal);
}


  //--------------------------------------
  //QAD_PWM Private Initialization Methods


//QAD_PWM::periphInit
//QAD_PWM Private Initialization Method
QA_Result QAD_PWM::periphInit(void) {

	//Init GPIOs
	GPIO_InitTypeDef GPIO_Init = {0};
	GPIO_Init.Mode     = GPIO_MODE_AF_PP;
	GPIO_Init.Pull     = GPIO_NOPULL;
	GPIO_Init.Speed    = GPIO_SPEED_FREQ_HIGH;

	for (uint8_t i=0; i<QAD_TimerMgr::getChannels(m_eTimer); i++) {
		if (m_sChannels[i].eActive) {
			GPIO_Init.Pin         = m_sChannels[i].uPin;
			GPIO_Init.Alternate   = m_sChannels[i].uAF;

			HAL_GPIO_Init(m_sChannels[i].pGPIO, &GPIO_Init);
		}
	}

	//Enable Timer Clock
	QAD_TimerMgr::enableClock(m_eTimer);

	//Init Timer PWM Mode
	m_sHandle.Instance                     = QAD_TimerMgr::getInstance(m_eTimer);
	m_sHandle.Init.Prescaler               = m_uPrescaler;
	m_sHandle.Init.Period                  = m_uPeriod;
	m_sHandle.Init.CounterMode             = TIM_COUNTERMODE_UP;
	m_sHandle.Init.ClockDivision           = TIM_CLOCKDIVISION_DIV1;
	m_sHandle.Init.RepetitionCounter       = 0xFF;
	m_sHandle.Init.AutoReloadPreload       = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(&m_sHandle) != HAL_OK) {
		periphDeinit(DeinitPartial);
		return QA_Fail;
	}

	//Init Channels
	TIM_OC_InitTypeDef TIM_OC_Init;
	for (uint8_t i=0; i<QAD_TimerMgr::getChannels(m_eTimer); i++) {
		if (m_sChannels[i].eActive) {
			TIM_OC_Init = {0};
			TIM_OC_Init.OCMode        = TIM_OCMODE_PWM1;
			TIM_OC_Init.OCIdleState   = TIM_OCIDLESTATE_SET;
			TIM_OC_Init.Pulse         = 0;
			TIM_OC_Init.OCPolarity    = TIM_OCPOLARITY_HIGH;
			TIM_OC_Init.OCFastMode    = TIM_OCFAST_ENABLE;
			if (HAL_TIM_PWM_ConfigChannel(&m_sHandle, &TIM_OC_Init, m_uChannelSelect[i]) != HAL_OK) {
				periphDeinit(DeinitFull);
				return QA_Fail;
			}
		}
	}

	//Set States
	m_eInitState = QA_Initialized;
	m_eState     = QA_Inactive;

	//Return
	return QA_OK;
}


//QAD_PWM::periphDeinit
//QAD_PWM Private Initialization Method
void QAD_PWM::periphDeinit(QAD_PWM::DeinitMode eMode) {

	if (eMode) {

		//Deinit Timer
		HAL_TIM_PWM_DeInit(&m_sHandle);

		//Disable Timer Clock
		QAD_TimerMgr::disableClock(m_eTimer);

	}

	//Deinit GPIOs
	for (uint8_t i=0; i<QAD_TimerMgr::getChannels(m_eTimer); i++) {
		if (m_sChannels[i].eActive)
			HAL_GPIO_DeInit(m_sChannels[i].pGPIO, m_sChannels[i].uPin);
	}

	//Set States
	m_eInitState = QA_NotInitialized;
	m_eState     = QA_Inactive;
}












