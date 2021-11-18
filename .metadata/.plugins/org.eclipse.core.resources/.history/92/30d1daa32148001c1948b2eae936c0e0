/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: ADC Driver                                                      */
/*   Filename: QAD_ADC.cpp                                                 */
/*   Date: 16th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_ADC.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


  //------------------------------
  //------------------------------
  //QAD_ADC Initialization Methods

//QAD_ADC::imp_init
//QAD_ADC Initialization Method
QA_Result QAD_ADC::imp_init(QAD_ADC_InitStruct& sInit) {
	m_eTimer           = sInit.eTimer;
	m_uTimer_Prescaler = sInit.uTimer_Prescaler;
	m_uTimer_Period    = sInit.uTimer_Period;

	if (!QAD_TimerMgr::getADC(m_eTimer)) {
		return QA_Error_PeriphNotSupported;
	}

  if (QAD_TimerMgr::getState(m_eTimer))
  	return QA_Error_PeriphBusy;

  QAD_TimerMgr::registerTimer(m_eTimer, QAD_Timer_InUse_ADC);
  QA_Result eRes = imp_periphInit(sInit);

  if (eRes)
  	QAD_TimerMgr::deregisterTimer(m_eTimer);
  return eRes;
}


//QAD_ADC::imp_deinit
//QAD_ADC Initialization Method
void QAD_ADC::imp_deinit(void) {
	if (!m_eInitState)
		return;

	imp_periphDeinit(DeinitFull);
	QAD_TimerMgr::deregisterTimer(m_eTimer);
}


  //-----------------------------------------
	//-----------------------------------------
	//QAD_ADC Peripheral Initialization Methods

//QAD_ADC::imp_periphInit
//QAD_ADC Peripheral Initialization Method
QA_Result QAD_ADC::imp_periphInit(QAD_ADC_InitStruct& sInit) {

	//Enable Timer Clock
	QAD_TimerMgr::enableClock(m_eTimer);

	//Initialize Timer
	m_sTIMHandle.Instance               = QAD_TimerMgr::getInstance(m_eTimer);
	m_sTIMHandle.Init.Prescaler         = m_uTimer_Prescaler;
	m_sTIMHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	m_sTIMHandle.Init.Period            = m_uTimer_Period;
	m_sTIMHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	m_sTIMHandle.Init.RepetitionCounter = 0x0;
	m_sTIMHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&m_sTIMHandle) != HAL_OK) {
		imp_periphDeinit(DeinitPartial);
		return QA_Fail;
	}

	//Setup Timer Trigger
	TIM_MasterConfigTypeDef MC_Init = {0};
	MC_Init.MasterOutputTrigger = TIM_TRGO_UPDATE;
	HAL_TIMEx_MasterConfigSynchronization(&m_sTIMHandle, &MC_Init);

	//Enable ADC Clock
	__HAL_RCC_ADC1_CLK_ENABLE();

	//Enable ADC IRQ
	HAL_NVIC_SetPriority(ADC_IRQn, QAD_IRQPRIORITY_ADC, 0x00);
	HAL_NVIC_EnableIRQ(ADC_IRQn);

	//Set States
	m_eInitState = QA_Initialized;
	m_eState     = QA_Inactive;

	//Return
	return QA_OK;
}


//QAD_ADC::imp_periphDeinit
//QAD_ADC Peripheral Initialization Method
void QAD_ADC::imp_periphDeinit(QAD_ADC::DeinitMode eMode) {

	if (m_eState) {
		imp_stop();
	}

	//Full Deinitialization
	if (eMode) {

		//Disable ADC IRQ
		HAL_NVIC_EnableIRQ(ADC_IRQn);

		//Disable ADC Clock
		__HAL_RCC_ADC1_CLK_DISABLE();

		//Deinit Timer
		HAL_TIM_Base_DeInit(&m_sTIMHandle);

	}

	//Disable Timer Clock
	QAD_TimerMgr::disableClock(m_eTimer);

	//Set States
	m_eInitState = QA_NotInitialized;
	m_eState     = QA_Inactive;

}


	//-----------------------
	//-----------------------
	//QAD_ADC Handler Methods

//QAD_ADC::imp_handler
//QAD_ADC Handler Method
void QAD_ADC::imp_handler(void) {
	if (__HAL_ADC_GET_FLAG(&m_sADCHandle, ADC_FLAG_EOC)) {

		m_uData[m_uDataIdx] = m_sADCHandle.Instance->DR;
		if (m_uDataIdx >= (m_uChannelCount-1))
			m_uDataIdx = 0; else
			m_uDataIdx++;

		__HAL_ADC_CLEAR_FLAG(&m_sADCHandle, ADC_FLAG_EOC);
	}
}


	//-----------------------
	//-----------------------
	//QAD_ADC Control Methods

//QAD_ADC::imp_start
//QAD_ADC Control Method
QA_Result QAD_ADC::imp_start(void) {
	if ((m_eState) || (!m_uChannelCount))
		return QA_Fail;

	//Initialize ADC
	m_sADCHandle.Instance                   = ADC1;
	m_sADCHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
	m_sADCHandle.Init.Resolution            = ADC_RESOLUTION_12B;
	m_sADCHandle.Init.ScanConvMode          = ENABLE;
	m_sADCHandle.Init.ContinuousConvMode    = DISABLE;
	m_sADCHandle.Init.DiscontinuousConvMode = DISABLE;
	m_sADCHandle.Init.NbrOfDiscConversion   = 0;
	m_sADCHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
	m_sADCHandle.Init.ExternalTrigConv      = imp_getTrigger();
	m_sADCHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	m_sADCHandle.Init.NbrOfConversion       = m_uChannelCount;
	m_sADCHandle.Init.DMAContinuousRequests = ENABLE;
	m_sADCHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&m_sADCHandle) != HAL_OK) {
		imp_stop();
		return QA_Fail;
	}

	//Channel GPIO Configuration
	GPIO_InitTypeDef GPIO_Init = {0};
	GPIO_Init.Mode  = GPIO_MODE_ANALOG;
	GPIO_Init.Pull  = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

	for (uint8_t i=0; i<m_uChannelCount; i++) {
		if ((m_sChannels[i].eChannel != QAD_ADC_PeriphChannelVRefInt) ||
				(m_sChannels[i].eChannel != QAD_ADC_PeriphChannelVBat)) {
			GPIO_Init.Pin       = m_sChannels[i].uPin;
			HAL_GPIO_Init(m_sChannels[i].pGPIO, &GPIO_Init);
		}
	}


	//Channel Configuration
	ADC_ChannelConfTypeDef  ADCChannel_Init = {0};
	ADCChannel_Init.Offset        = 0;

	for (uint8_t i=0; i<m_uChannelCount; i++) {
		ADCChannel_Init.Channel       = m_sChannels[i].eChannel;
		ADCChannel_Init.Rank          = i+1;
		ADCChannel_Init.SamplingTime = m_sChannels[i].eSamplingTime;

		if (HAL_ADC_ConfigChannel(&m_sADCHandle, &ADCChannel_Init) != HAL_OK) {
			imp_stop();
			return QA_Fail;
		}

	}

	//Clear Data
	for (uint8_t i=0; i<QAD_ADC_MAXCHANNELS; i++)
		m_uData[i] = 0;
	m_uDataIdx = 0;

	//Enable ADC IRQ
	HAL_ADC_Start_IT(&m_sADCHandle);
	__HAL_TIM_ENABLE(&m_sTIMHandle);

	//Set States
	m_eState = QA_Active;

	//Return
	return QA_OK;
}


//QAD_ADC::imp_stop
//QAD_ADC Control Method
void QAD_ADC::imp_stop(void) {

	//Disable ADC IRQ
	__HAL_TIM_DISABLE(&m_sTIMHandle);
	HAL_ADC_Stop_IT(&m_sADCHandle);

	//GPIO Deinitialization
	for (uint8_t i=0; i<m_uChannelCount; i++) {
		HAL_GPIO_DeInit(m_sChannels[i].pGPIO, m_sChannels[i].uPin);
	}

	//Deinitialize ADC
	HAL_ADC_DeInit(&m_sADCHandle);

	//Set States
	m_eState = QA_Inactive;
}


//QAD_ADC::imp_addChannel
//QAD_ADC Control Method
void QAD_ADC::imp_addChannel(QAD_ADC_ChannelData& sChannel) {

	if (m_uChannelCount >= 16)
		return;

	//Check if ADC is running
  bool bStarted = false;
  if (m_eState) {
  	bStarted = true;
  	imp_stop();
  }

  //Update Channel Data
  m_sChannels[m_uChannelCount] = sChannel;
  m_uChannelCount++;

  //Restart ADC if required
  if (bStarted) {
  	imp_start();
  }
}


//QAD_ADC::imp_removeChannel
//QAD_ADC Control Method
void QAD_ADC::imp_removeChannel(QAD_ADC_Channel eChannel) {
	if ((!m_uChannelCount) && (eChannel < m_uChannelCount))
		return;

	//Check if ADC is running
  bool bStarted = false;
  if (m_eState) {
  	bStarted = true;
  	imp_stop();
  }

  if (eChannel != (m_uChannelCount - 1)) {
  	for (uint8_t i=eChannel; i<(m_uChannelCount-1); i++)
  		m_sChannels[i] = m_sChannels[i+1];
  }
  m_uChannelCount--;

  //Restart ADC if required
  if (bStarted) {
  	imp_start();
  }
}


//QAD_ADC::imp_removeChannelPeriph
//QAD_ADC Control Method
void QAD_ADC::imp_removeChannelPeriph(QAD_ADC_PeriphChannel eChannel) {
	int8_t iChannelIdx = imp_findChannelPeriph(eChannel);
	if ((!m_uChannelCount) && (iChannelIdx < 0))
		return;

	//Check if ADC is running
  bool bStarted = false;
  if (m_eState) {
  	bStarted = true;
  	imp_stop();
  }

  if (iChannelIdx != (m_uChannelCount - 1)) {
  	for (uint8_t i=iChannelIdx; i<(m_uChannelCount-1); i++)
  		m_sChannels[i] = m_sChannels[i+1];
  }
  m_uChannelCount--;

  //Restart ADC if required
  if (bStarted) {
  	imp_start();
  }
}


//--------------------
//QAD_ADC Data Methods

//QAD_ADC::imp_getDataPeriph
//QAD_ADC Data Method
uint16_t QAD_ADC::imp_getDataPeriph(QAD_ADC_PeriphChannel eChannel) {
	int8_t iChannelIdx = imp_findChannelPeriph(eChannel);
	if ((!m_uChannelCount) || (iChannelIdx < 0))
		return 0;

	return m_uData[iChannelIdx];
}


//QAD_ADC::imp_getDataIdx
//QAD_ADC Data Method
uint16_t QAD_ADC::imp_getDataIdx(uint8_t uChannel) {
	if (!m_uChannelCount)
		return 0;

	if (uChannel <= m_uChannelCount)
		return m_uData[uChannel];

	return 0;
}


	//--------------------
	//--------------------
	//QAD_ADC Tool Methods

//QAD_ADC::imp_getTrigger
//QAD_ADC Tool Method
uint32_t QAD_ADC::imp_getTrigger(void) {
	uint32_t uTrigger;
	switch (m_eTimer) {
		case QAD_Timer2:
			uTrigger = ADC_EXTERNALTRIGCONV_T2_TRGO;
			break;
		case QAD_Timer3:
			uTrigger = ADC_EXTERNALTRIGCONV_T3_TRGO;
			break;
		default:
			uTrigger = 0xFFFFFFFF;
	}
	return uTrigger;
}


//QAD_ADC::imp_findChannel
//QAD_ADC Tool Method
int8_t QAD_ADC::imp_findChannel(QAD_ADC_Channel eChannel) {
	if (eChannel < m_uChannelCount)
		return eChannel;

  return -1;
}


//QAD_ADC::imp_findChannelPeriph
//QAD_ADC Tool Method
int8_t QAD_ADC::imp_findChannelPeriph(QAD_ADC_PeriphChannel eChannel) {
	for (uint8_t i=0; i<m_uChannelCount; i++) {
		if (m_sChannels[i].eChannel == eChannel)
			return i;
	}
	return -1;
}





















