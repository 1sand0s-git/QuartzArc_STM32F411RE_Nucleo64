/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: I2C Management Driver                                           */
/*   Filename: QAD_I2CMgr.cpp                                              */
/*   Date: 25th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_I2CMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


	//-----------------------
	//-----------------------
	//QAD_I2CMgr Constructors

//QAD_I2CMgr::QAD_I2CMgr
//QAD_I2CMgr Constructor
QAD_I2CMgr::QAD_I2CMgr() {

	for (uint8_t i=0; i<QAD_I2C_PeriphCount; i++) {
		m_sI2Cs[i].eState = QAD_I2C_Unused;
	}

	//Set Peripheral IDs
	m_sI2Cs[QAD_I2C1].eI2C = QAD_I2C1;
	m_sI2Cs[QAD_I2C2].eI2C = QAD_I2C2;
	m_sI2Cs[QAD_I2C3].eI2C = QAD_I2C3;

	//Set Instances
	m_sI2Cs[QAD_I2C1].pInstance = I2C1;
	m_sI2Cs[QAD_I2C2].pInstance = I2C2;
	m_sI2Cs[QAD_I2C3].pInstance = I2C3;

  //Set Event IRQs
	m_sI2Cs[QAD_I2C1].eIRQ_Event = I2C1_EV_IRQn;
	m_sI2Cs[QAD_I2C2].eIRQ_Event = I2C2_EV_IRQn;
	m_sI2Cs[QAD_I2C3].eIRQ_Event = I2C3_EV_IRQn;

	//Set Error IRQs
	m_sI2Cs[QAD_I2C1].eIRQ_Error = I2C1_ER_IRQn;
	m_sI2Cs[QAD_I2C2].eIRQ_Error = I2C2_ER_IRQn;
	m_sI2Cs[QAD_I2C3].eIRQ_Error = I2C3_ER_IRQn;

}


	//-----------------------------
	//-----------------------------
	//QAD_I2CMgr Management Methods

//QAD_I2CMgr::imp_registerI2C
//QAD_I2CMgr Management Method
QA_Result QAD_I2CMgr::imp_registerI2C(QAD_I2C_Periph eI2C, QAD_I2C_State eMode) {
  if (eI2C >= QAD_I2CNone)
  	return QA_Fail;

  if (m_sI2Cs[eI2C].eState)
  	return QA_Error_PeriphBusy;

  m_sI2Cs[eI2C].eState = eMode;
  return QA_OK;
}


//QAD_I2CMgr::imp_deregisterI2C
//QAD_I2CMgr Management Method
void QAD_I2CMgr::imp_deregisterI2C(QAD_I2C_Periph eI2C) {
  if (eI2C >= QAD_I2CNone)
  	return;

  m_sI2Cs[eI2C].eState = QAD_I2C_Unused;
}


	//------------------------
	//------------------------
	//QAD_I2CMgr Clock Methods

//QAD_I2CMgr::imp_enableClock
//QAD_I2CMgr Clock Method
void QAD_I2CMgr::imp_enableClock(QAD_I2C_Periph eI2C) {
  switch (eI2C) {
    case (QAD_I2C1):
    	__HAL_RCC_I2C1_CLK_ENABLE();
      __HAL_RCC_I2C1_FORCE_RESET();
      __HAL_RCC_I2C1_RELEASE_RESET();
    	break;
    case (QAD_I2C2):
    	__HAL_RCC_I2C2_CLK_ENABLE();
      __HAL_RCC_I2C2_FORCE_RESET();
      __HAL_RCC_I2C2_RELEASE_RESET();
    	break;
    case (QAD_I2C3):
    	__HAL_RCC_I2C3_CLK_ENABLE();
      __HAL_RCC_I2C3_FORCE_RESET();
      __HAL_RCC_I2C3_RELEASE_RESET();
    	break;
    case (QAD_I2CNone):
    	break;
  }
}


//QAD_I2CMgr::imp_disableClock
//QAD_I2CMgr Clock Method
void QAD_I2CMgr::imp_disableClock(QAD_I2C_Periph eI2C) {
  switch (eI2C) {
    case (QAD_I2C1):
    	__HAL_RCC_I2C1_CLK_DISABLE();
    	break;
    case (QAD_I2C2):
    	__HAL_RCC_I2C2_CLK_DISABLE();
    	break;
    case (QAD_I2C3):
    	__HAL_RCC_I2C3_CLK_DISABLE();
    	break;
    case (QAD_I2CNone):
    	break;
  }
}


	//-------------------------
	//-------------------------
	//QAD_I2CMgr Status Methods

//QAD_I2CMgr::imp_getI2CsActive
//QAD_I2CMgr Status Method
uint8_t QAD_I2CMgr::imp_getI2CsActive(void) {
  uint8_t uCount = 0;
  for (uint8_t i=0; i<QAD_I2C_PeriphCount; i++) {
  	if (m_sI2Cs[i].eState)
  		uCount++;
  }
  return uCount;
}


//QAD_I2CMgr::imp_getI2CsInactive
//QAD_I2CMgr Status Method
uint8_t QAD_I2CMgr::imp_getI2CsInactive(void) {
  uint8_t uCount = 0;
  for (uint8_t i=0; i<QAD_I2C_PeriphCount; i++) {
  	if (!m_sI2Cs[i].eState)
  		uCount++;
  }
  return uCount;
}




