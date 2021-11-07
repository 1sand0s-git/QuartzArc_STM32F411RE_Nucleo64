/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: Encoder Driver                                                  */
/*   Filename: QAD_Encoder.cpp                                             */
/*   Date: 28th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_Encoder.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


  //----------------------------------
  //QAD_Encoder Initialization Methods

//QAD_Encoder::init
//QAD_Encoder Initialization Method
QA_Result QAD_Encoder::init(void) {
  if (!QAD_TimerMgr::getEncoder(m_eTimer)) {
  	return QA_Error_PeriphNotSupported;
  }

  if (QAD_TimerMgr::getState(m_eTimer))
  	return QA_Error_PeriphBusy;

  QAD_TimerMgr::registerTimer(m_eTimer, QAD_Timer_InUse_Encoder);
  QA_Result eRes = periphInit();

  if (eRes)
  	QAD_TimerMgr::deregisterTimer(m_eTimer);
  return eRes;
}


//QAD_Encoder::deinit
//QAD_Encoder Initialization Method
void QAD_Encoder::deinit(void) {
  if (!m_eInitState)
  	return;

  periphDeinit(DeinitFull);
  QAD_TimerMgr::deregisterTimer(m_eTimer);
}


  //---------------------------
  //QAD_Encoder Control Methods

//QAD_Encoder::start
//QAD_Encoder Control Method
void QAD_Encoder::start(void) {
  if ((m_eInitState) && (!m_eState)) {
  	clearData();
  	HAL_TIM_Encoder_Start(&m_sHandle, TIM_CHANNEL_ALL);
  	m_eState = QA_Active;
  }
}


//QAD_Encoder::stop
//QAD_Encoder Control Method
void QAD_Encoder::stop(void) {
  if ((m_eInitState) && (m_eState)) {
  	HAL_TIM_Encoder_Stop(&m_sHandle, TIM_CHANNEL_ALL);
  	m_eState = QA_Inactive;
  }
}


//QAD_Encoder::update
//QAD_Encoder Control Method
void QAD_Encoder::update(uint32_t uTicks) {

	if (m_eState) {
		m_uValueOld = m_uValueNew;
		m_uValueNew = __HAL_TIM_GET_COUNTER(&m_sHandle);

		uint16_t uDiffA;
		uint16_t uDiffB;
		bool bValComp = (m_uValueNew < m_uValueOld);
		if (bValComp) {
			uDiffA = m_uValueOld - m_uValueNew;
			uDiffB = m_uValueNew + (m_uMaxVal - m_uValueOld);
		} else {
			uDiffA = m_uValueNew - m_uValueOld;
			uDiffB = m_uValueOld + (m_uMaxVal - m_uValueNew);
		}

		uint16_t uDiff = (uDiffA < uDiffB) ? uDiffA : uDiffB;
		m_iValue += (bValComp ? 0-uDiff : uDiff);
		m_uAccel = uDiff * 1000 / uTicks;
	}
}


//QAD_Encoder::getState
//QAD_Encoder Control Method
QA_ActiveState QAD_Encoder::getState(void) {
  return m_eState;
}


//QAD_Encoder::getValue
//QAD_Encoder Control Method
int16_t QAD_Encoder::getValue(void) {
  if (m_eInitState) {
  	int16_t iOutVal = m_iValue / 4;
  	m_iValue = m_iValue % 4;
  	return (m_eMode) ? (iOutVal * iOutVal * iOutVal) : iOutVal;
  }
  return 0;
}


//QAD_Encoder::getAccel
//QAD_Encoder Control Method
uint16_t QAD_Encoder::getAccel(void) {
  return (m_eInitState) ? m_uAccel : 0;
}


//QAD_Encoder::setMode
//QAD_Encoder Control Method
void QAD_Encoder::setMode(QAD_EncoderMode eMode) {
  m_eMode = eMode;
}


//QAD_Encoder::getMode
//QAD_Encoder Control Method
QAD_EncoderMode QAD_Encoder::getMode(void) {
  return m_eMode;
}


  //------------------------------------------
  //QAD_Encoder Private Initialization Methods

//QAD_Encoder::periphInit
//QAD_Encoder Private Initialization Method
QA_Result QAD_Encoder::periphInit(void) {
  GPIO_InitTypeDef GPIO_Init = {0};

  //Ch1 GPIO
  GPIO_Init.Pin       = m_uCh1_Pin;
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init.Alternate = m_uCh1_AF;
  HAL_GPIO_Init(m_pCh1_GPIO, &GPIO_Init);

  //Ch2 GPIO
  GPIO_Init.Pin       = m_uCh2_Pin;
  GPIO_Init.Alternate = m_uCh2_AF;
  HAL_GPIO_Init(m_pCh2_GPIO, &GPIO_Init);

  //Enable Timer Clock
  QAD_TimerMgr::enableClock(m_eTimer);

  //Initialize Timer
  m_sHandle.Instance               = QAD_TimerMgr::getInstance(m_eTimer);
  m_sHandle.Init.Prescaler         = 0;
  m_sHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  m_sHandle.Init.Period            = m_uMaxVal;
  m_sHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  m_sHandle.Init.RepetitionCounter = 0xFF;
  m_sHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  TIM_Encoder_InitTypeDef ENC_Init = {0};
  ENC_Init.EncoderMode   = TIM_ENCODERMODE_TI12;
  ENC_Init.IC1Polarity   = TIM_ICPOLARITY_RISING;
  ENC_Init.IC1Selection  = TIM_ICSELECTION_DIRECTTI;
  ENC_Init.IC1Prescaler  = TIM_ICPSC_DIV1;
  ENC_Init.IC1Filter     = 0x0;
  ENC_Init.IC2Polarity   = TIM_ICPOLARITY_RISING;
  ENC_Init.IC2Selection  = TIM_ICSELECTION_DIRECTTI;
  ENC_Init.IC2Prescaler  = TIM_ICPSC_DIV1;
  ENC_Init.IC2Filter     = 0x0;
  if (HAL_TIM_Encoder_Init(&m_sHandle, &ENC_Init) != HAL_OK) {
  	periphDeinit(DeinitPartial);
  	return QA_Fail;
  }

  //Set States
  m_eState = QA_Inactive;
  m_eInitState = QA_Initialized;

  //Return
  return QA_OK;
}


//QAD_Encoder::periphDeinit
//QAD_Encoder Private Initialization Method
void QAD_Encoder::periphDeinit(QAD_Encoder::DeinitMode eMode) {

	//Full Deinit
	if (eMode) {

		HAL_TIM_Encoder_DeInit(&m_sHandle);
	}

	//Disable Timer Clock
	QAD_TimerMgr::disableClock(m_eTimer);

	//Deinit GPIOs
	HAL_GPIO_DeInit(m_pCh1_GPIO, m_uCh1_Pin);
	HAL_GPIO_DeInit(m_pCh2_GPIO, m_uCh2_Pin);

	//Set States
	m_eState     = QA_Inactive;
	m_eInitState = QA_NotInitialized;
}


//QAD_Encoder::clearData
//QAD_Encoder Private Tool Method
void QAD_Encoder::clearData(void) {
  m_uValueOld = 0;
  m_uValueNew = 0;
  m_iValue    = 0;
  m_uAccel    = 0;
  __HAL_TIM_SET_COUNTER(&m_sHandle, 0);
}




