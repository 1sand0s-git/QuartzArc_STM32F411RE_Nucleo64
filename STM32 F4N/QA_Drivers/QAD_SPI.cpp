/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: SPI Driver                                                      */
/*   Filename: QAD_SPI.cpp                                                 */
/*   Date: 13th December 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_SPI.hpp"


	//------------------------------------------
	//------------------------------------------
  //------------------------------------------


	//------------------------------
	//------------------------------
	//QAD_SPI Initialization Methods

//QAD_SPI::init
//QAD_SPI Initialization Method
QA_Result QAD_SPI::init(void) {
  if (QAD_SPIMgr::getState(m_eSPI))
  	return QA_Error_PeriphBusy;

  QAD_SPIMgr::registerSPI(m_eSPI, QAD_SPI_InUse_SPI);
  QA_Result eRes = periphInit();

  if (eRes)
  	QAD_SPIMgr::deregisterSPI(m_eSPI);
  return eRes;
}


//QAD_SPI::deinit
//QAD_SPI Initialization Method
void QAD_SPI::deinit(void) {
  if (!m_eInitState)
  	return;

  periphDeinit(DeinitFull);
  QAD_SPIMgr::deregisterSPI(m_eSPI);
}


//QAD_SPI::getInitState
//QAD_SPI Initialization Method
QA_InitState QAD_SPI::getInitState(void) {
  return m_eInitState;
}


//QAD_SPI::getHandle
//QAD_SPI Initialization Method
SPI_HandleTypeDef& QAD_SPI::getHandle(void) {
	return m_sHandle;
}


	//-----------------------
	//-----------------------
	//QAD_SPI Control Methods

//QAD_SPI::start
//QAD_SPI Control Method
void QAD_SPI::start(void) {
	__HAL_SPI_ENABLE(&m_sHandle);
	m_eState = QA_Active;
}


//QAD_SPI::stop
//QAD_SPI Control Method
void QAD_SPI::stop(void) {
	__HAL_SPI_DISABLE(&m_sHandle);
	m_eState = QA_Inactive;
}


//QAD_SPI::getState
//QAD_SPI Control Method
QA_ActiveState QAD_SPI::getState(void) {
	return m_eState;
}


	//--------------------------
	//--------------------------
	//QAD_SPI Transceive Methods

//QAD_SPI::transmit
//QAD_SPI Transceive Method
QA_Result QAD_SPI::transmit(uint8_t* pTXData, uint16_t uSize) {
	if (m_bCS_Soft)
		HAL_GPIO_WritePin(m_pCS_GPIO, m_uCS_Pin, GPIO_PIN_RESET);

	QA_Result eRes = QA_OK;
	if (HAL_SPI_Transmit(&m_sHandle, pTXData, uSize, m_uTimeout) != HAL_OK) {
		eRes = QA_Fail;
	}

	if (m_bCS_Soft)
		HAL_GPIO_WritePin(m_pCS_GPIO, m_uCS_Pin, GPIO_PIN_SET);

	return eRes;
}


//QAD_SPI::receive
//QAD_SPI Transceive Method
QA_Result QAD_SPI::receive(uint8_t* pRXData, uint16_t uSize) {
	if (m_bCS_Soft)
		HAL_GPIO_WritePin(m_pCS_GPIO, m_uCS_Pin, GPIO_PIN_RESET);

	QA_Result eRes = QA_OK;
	if (HAL_SPI_Receive(&m_sHandle, pRXData, uSize, m_uTimeout) != HAL_OK) {
		eRes = QA_Fail;
	}

	if (m_bCS_Soft)
		HAL_GPIO_WritePin(m_pCS_GPIO, m_uCS_Pin, GPIO_PIN_SET);

	return eRes;
}


//QAD_SPI::transceive
//QAD_SPI Transceive Method
QA_Result QAD_SPI::transceive(uint8_t* pTXData, uint8_t* pRXData, uint16_t uSize) {
	if (m_bCS_Soft)
		HAL_GPIO_WritePin(m_pCS_GPIO, m_uCS_Pin, GPIO_PIN_RESET);

	QA_Result eRes = QA_OK;
	if (HAL_SPI_TransmitReceive(&m_sHandle, pTXData, pRXData, uSize, m_uTimeout) != HAL_OK) {
		return QA_Fail;
	}

	if (m_bCS_Soft)
		HAL_GPIO_WritePin(m_pCS_GPIO, m_uCS_Pin, GPIO_PIN_SET);

	return eRes;
}


	//--------------------------------------
	//--------------------------------------
	//QAD_SPI Private Initialization Methods

//QAD_SPI::periphInit
//QAD_SPI Private Initialization Method
QA_Result QAD_SPI::periphInit(void) {
	GPIO_InitTypeDef GPIO_Init = {0};

	//Init Clock GPIO Pin
	GPIO_Init.Pin       = m_uClk_Pin;
	GPIO_Init.Mode      = GPIO_MODE_AF_PP;
	GPIO_Init.Pull      = GPIO_NOPULL;
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init.Alternate = m_uClk_AF;
	HAL_GPIO_Init(m_pClk_GPIO, &GPIO_Init);

	//Init MOSI GPIO Pin
	if (((m_eSPIMode == QAD_SPI_Mode_Master) && (m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_TXOnly)) ||
			((m_eSPIMode == QAD_SPI_Mode_Slave) && (m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_RXOnly)) ||
			((m_eSPIMode == QAD_SPI_Mode_Master) && (m_eSPILines == QAD_SPI_Lines_1Line) && (m_eSPIBiDir == QAD_SPI_BiDir_Enabled)) ||
			((m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_Enabled))) {
		GPIO_Init.Pin       = m_uMOSI_Pin;
		GPIO_Init.Mode      = GPIO_MODE_AF_PP;
		GPIO_Init.Pull      = GPIO_NOPULL;
		GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_Init.Alternate = m_uMOSI_AF;
		HAL_GPIO_Init(m_pMOSI_GPIO, &GPIO_Init);
	}

	//Init MISO GPIO Pin
	if (((m_eSPIMode == QAD_SPI_Mode_Master) && (m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_RXOnly)) ||
			((m_eSPIMode == QAD_SPI_Mode_Slave) && (m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_TXOnly)) ||
			((m_eSPIMode == QAD_SPI_Mode_Slave) && (m_eSPILines == QAD_SPI_Lines_1Line) && (m_eSPIBiDir == QAD_SPI_BiDir_Enabled)) ||
			((m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_Enabled))) {
		GPIO_Init.Pin       = m_uMISO_Pin;
		GPIO_Init.Mode      = GPIO_MODE_AF_PP;
		GPIO_Init.Pull      = GPIO_NOPULL;
		GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_Init.Alternate = m_uMISO_AF;
		HAL_GPIO_Init(m_pMISO_GPIO, &GPIO_Init);
	}

	//Init CS GPIO Pin
	if (m_eSPICS != QAD_SPI_CS_Soft) {
		GPIO_Init.Pin       = m_uCS_Pin;
		GPIO_Init.Mode      = GPIO_MODE_AF_PP;
		GPIO_Init.Pull      = GPIO_PULLUP;
		GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_Init.Alternate = m_uCS_AF;
		HAL_GPIO_Init(m_pCS_GPIO, &GPIO_Init);

	} else if (m_pCS_GPIO != NULL){
		GPIO_Init = {0};
		GPIO_Init.Pin       = m_uCS_Pin;
		GPIO_Init.Mode      = GPIO_MODE_OUTPUT_PP;
		GPIO_Init.Pull      = GPIO_PULLUP;
		GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		HAL_GPIO_Init(m_pCS_GPIO, &GPIO_Init);

		m_bCS_Soft = true;
		HAL_GPIO_WritePin(m_pCS_GPIO, m_uCS_Pin, GPIO_PIN_SET);
	}


	//Enable SPI Clock
	QAD_SPIMgr::enableClock(m_eSPI);


	//Initialize SPI Peripheral
	m_sHandle.Instance               = QAD_SPIMgr::getInstance(m_eSPI);
	m_sHandle.Init.Mode              = m_eSPIMode;

	if ((m_eSPILines == QAD_SPI_Lines_2Lines) ||
			((m_eSPILines == QAD_SPI_Lines_1Line) && (m_eSPIBiDir == QAD_SPI_BiDir_RXOnly)))
		m_sHandle.Init.Direction       = SPI_DIRECTION_2LINES; else
	if ((m_eSPILines == QAD_SPI_Lines_1Line) && (m_eSPIBiDir == QAD_SPI_BiDir_RXOnly))
		m_sHandle.Init.Direction       = SPI_DIRECTION_2LINES_RXONLY; else
		m_sHandle.Init.Direction       = SPI_DIRECTION_1LINE;

	m_sHandle.Init.DataSize          = m_eSPIDataSize;
	m_sHandle.Init.CLKPolarity       = m_eSPIClkPolarity;
	m_sHandle.Init.CLKPhase          = m_eSPIClkPhase;
	m_sHandle.Init.NSS               = m_eSPICS;
	m_sHandle.Init.BaudRatePrescaler = m_eSPIPrescaler;
	m_sHandle.Init.FirstBit          = m_eSPIFirstBit;
	m_sHandle.Init.TIMode            = m_eSPITIMode;
	m_sHandle.Init.CRCCalculation    = m_eSPICRC;
	m_sHandle.Init.CRCPolynomial     = m_uSPICRCPolynomial;

	if (HAL_SPI_Init(&m_sHandle) != HAL_OK) {
		periphDeinit(DeinitPartial);
		return QA_Fail;
	}


	//Setup SPI IRQ priority and enable IRQ
	HAL_NVIC_SetPriority(QAD_SPIMgr::getIRQ(m_eSPI), m_uIRQPriority, 0);
	HAL_NVIC_EnableIRQ(QAD_SPIMgr::getIRQ(m_eSPI));


	//Set Driver States
	m_eInitState = QA_Initialized;
	m_eState     = QA_Inactive;


	//Return
	return QA_OK;
}


//QAD_SPI::periphDeinit
//QAD_SPI Private Initialization Method
void QAD_SPI::periphDeinit(DeinitMode eDeinitMode) {

	if (eDeinitMode) {
		stop();
		HAL_NVIC_DisableIRQ(QAD_SPIMgr::getIRQ(m_eSPI));

		//DeInit Peripheral
		HAL_SPI_DeInit(&m_sHandle);
	}

	//Disable SPI Clock
	QAD_SPIMgr::disableClock(m_eSPI);

	//Init Clock GPIO Pin
	HAL_GPIO_DeInit(m_pClk_GPIO, m_uClk_Pin);

	//Init MOSI GPIO Pin
	if (((m_eSPIMode == QAD_SPI_Mode_Master) && (m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_TXOnly)) ||
			((m_eSPIMode == QAD_SPI_Mode_Slave) && (m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_RXOnly)) ||
			((m_eSPIMode == QAD_SPI_Mode_Master) && (m_eSPILines == QAD_SPI_Lines_1Line) && (m_eSPIBiDir == QAD_SPI_BiDir_Enabled)) ||
			((m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_Enabled))) {
		HAL_GPIO_DeInit(m_pMOSI_GPIO, m_uMOSI_Pin);
	}

	//Init MISO GPIO Pin
	if (((m_eSPIMode == QAD_SPI_Mode_Master) && (m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_RXOnly)) ||
			((m_eSPIMode == QAD_SPI_Mode_Slave) && (m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_TXOnly)) ||
			((m_eSPIMode == QAD_SPI_Mode_Slave) && (m_eSPILines == QAD_SPI_Lines_1Line) && (m_eSPIBiDir == QAD_SPI_BiDir_Enabled)) ||
			((m_eSPILines == QAD_SPI_Lines_2Lines) && (m_eSPIBiDir == QAD_SPI_BiDir_Enabled))) {
		HAL_GPIO_DeInit(m_pMISO_GPIO, m_uMISO_Pin);
	}

	//Init CS GPIO Pin
	if (m_pCS_GPIO != NULL)
		HAL_GPIO_DeInit(m_pCS_GPIO, m_uCS_Pin);

	//Set Driver States
	m_eInitState = QA_NotInitialized;
	m_eState     = QA_Inactive;

}

