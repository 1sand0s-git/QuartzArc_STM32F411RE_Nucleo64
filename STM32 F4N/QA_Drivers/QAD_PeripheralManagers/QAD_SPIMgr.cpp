/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: SPI Management Driver                                           */
/*   Filename: QAD_SPIMgr.cpp                                              */
/*   Date: 13th December 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_SPIMgr.hpp"


	//------------------------------------------
	//------------------------------------------
  //------------------------------------------


	//-----------------------
	//-----------------------
	//QAD_SPIMgr Constructors

//QAD_SPIMgr::QAD_SPIMgr
//QAD_SPIMgr Constructor
QAD_SPIMgr::QAD_SPIMgr() {

	for (uint8_t i=0; i<QAD_SPI_PeriphCount; i++) {
		m_sSPIs[i].eState = QAD_SPI_Unused;
		m_sSPIs[i].bI2S   = true;
	}

	//Set SPI Periph ID
	m_sSPIs[QAD_SPI1].eSPI = QAD_SPI1;
	m_sSPIs[QAD_SPI2].eSPI = QAD_SPI2;
	m_sSPIs[QAD_SPI3].eSPI = QAD_SPI3;
	m_sSPIs[QAD_SPI4].eSPI = QAD_SPI4;
	m_sSPIs[QAD_SPI5].eSPI = QAD_SPI5;

	//Set Instances
  m_sSPIs[QAD_SPI1].pInstance = SPI1;
  m_sSPIs[QAD_SPI2].pInstance = SPI2;
  m_sSPIs[QAD_SPI3].pInstance = SPI3;
  m_sSPIs[QAD_SPI4].pInstance = SPI4;
  m_sSPIs[QAD_SPI5].pInstance = SPI5;

	//Set IRQs
  m_sSPIs[QAD_SPI1].eIRQ = SPI1_IRQn;
  m_sSPIs[QAD_SPI2].eIRQ = SPI2_IRQn;
  m_sSPIs[QAD_SPI3].eIRQ = SPI3_IRQn;
  m_sSPIs[QAD_SPI4].eIRQ = SPI4_IRQn;
  m_sSPIs[QAD_SPI5].eIRQ = SPI5_IRQn;

}


//-----------------------------
//-----------------------------
//QAD_SPIMgr Management Methods

//QAD_SPIMgr::imp_registerSPI
//QAD_SPIMgr Management Method
QA_Result QAD_SPIMgr::imp_registerSPI(QAD_SPI_Periph eSPI, QAD_SPI_State eState) {
	if (eSPI >= QAD_SPINone)
		return QA_Fail;

	if (m_sSPIs[eSPI].eState)
		return QA_Error_PeriphBusy;

	if ((eState == QAD_SPI_InUse_I2S) && (!m_sSPIs[eSPI].bI2S))
		return QA_Error_PeriphNotSupported;

	m_sSPIs[eSPI].eState = eState;
	return QA_OK;
}


//QAD_SPIMgr::imp_deregisterSPI
//QAD_SPIMgr Management Method
void QAD_SPIMgr::imp_deregisterSPI(QAD_SPI_Periph eSPI) {
	if (eSPI >= QAD_SPINone)
		return;

	m_sSPIs[eSPI].eState = QAD_SPI_Unused;
}


//------------------------
//------------------------
//QAD_SPIMgr Clock Methods

//QAD_SPIMgr::imp_enableClock
//QAD_SPIMgr Clock Method
void QAD_SPIMgr::imp_enableClock(QAD_SPI_Periph eSPI) {
	switch (eSPI) {
		case (QAD_SPI1):
			__HAL_RCC_SPI1_CLK_ENABLE();
			__HAL_RCC_SPI1_FORCE_RESET();
			__HAL_RCC_SPI1_RELEASE_RESET();
			break;
		case (QAD_SPI2):
			__HAL_RCC_SPI2_CLK_ENABLE();
			__HAL_RCC_SPI2_FORCE_RESET();
			__HAL_RCC_SPI2_RELEASE_RESET();
			break;
		case (QAD_SPI3):
			__HAL_RCC_SPI3_CLK_ENABLE();
			__HAL_RCC_SPI3_FORCE_RESET();
			__HAL_RCC_SPI3_RELEASE_RESET();
			break;
		case (QAD_SPI4):
			__HAL_RCC_SPI4_CLK_ENABLE();
			__HAL_RCC_SPI4_FORCE_RESET();
			__HAL_RCC_SPI4_RELEASE_RESET();
			break;
		case (QAD_SPI5):
			__HAL_RCC_SPI5_CLK_ENABLE();
			__HAL_RCC_SPI5_FORCE_RESET();
			__HAL_RCC_SPI5_RELEASE_RESET();
			break;
		case (QAD_SPINone):
			break;
	}
}


//QAD_SPIMgr::imp_disableClock
//QAD_SPIMgr Clock Method
void QAD_SPIMgr::imp_disableClock(QAD_SPI_Periph eSPI) {
	switch (eSPI) {
		case (QAD_SPI1):
			__HAL_RCC_SPI1_CLK_DISABLE();
			break;
		case (QAD_SPI2):
			__HAL_RCC_SPI2_CLK_DISABLE();
			break;
		case (QAD_SPI3):
			__HAL_RCC_SPI3_CLK_DISABLE();
			break;
		case (QAD_SPI4):
			__HAL_RCC_SPI4_CLK_DISABLE();
			break;
		case (QAD_SPI5):
			__HAL_RCC_SPI5_CLK_DISABLE();
			break;
		case (QAD_SPINone):
			break;
	}
}


//-------------------------
//-------------------------
//QAD_SPIMgr Status Methods

//QAD_SPIMgr::imp_getSPIsActive
//QAD_SPIMgr Status Method
uint8_t QAD_SPIMgr::imp_getSPIsActive(void) {
	uint8_t uCount = 0;
	for (uint8_t i=0; i<QAD_SPI_PeriphCount; i++) {
		if (m_sSPIs[i].eState)
			uCount++;
	}
	return uCount;
}


//QAD_SPIMgr::imp_getSPIsInactive
//QAD_SPIMgr Status Method
uint8_t QAD_SPIMgr::imp_getSPIsInactive(void) {
	uint8_t uCount = 0;
	for (uint8_t i=0; i<QAD_SPI_PeriphCount; i++) {
		if (!m_sSPIs[i].eState)
			uCount++;
	}
	return uCount;
}
