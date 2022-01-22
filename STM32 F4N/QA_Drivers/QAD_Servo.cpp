/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: Servo Driver                                                    */
/*   Filename: QAD_Servo.cpp                                               */
/*   Date: 22nd January 2022                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_Servo.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

	//--------------------------------
	//--------------------------------
	//QAD_Servo Initialization Methods

//QAD_Servo::init
//QAD_Servo Initialization Method
QA_Result QAD_Servo::init(void) {

	//Set Driver States
	m_eInitState = QA_Initialized;

	//
	update();

	//Return
	return QA_OK;
}


//QAD_Servo::deinit
//QAD_Servo Initialization Method
void QAD_Servo::deinit(void) {
  if (!m_eInitState)
  	return;

  //Set Driver States
  m_eInitState = QA_NotInitialized;
}


//-------------------------
//-------------------------
//QAD_Servo Control Methods

//QAD_Servo::setCenter
//QAD_Servo Control Method
void QAD_Servo::setCenter(uint32_t uVal) {
	uint32_t uTempVal = uVal;

	if (uTempVal < m_uMin)
		uTempVal = m_uMin;
	if (uTempVal > m_uMax)
		uTempVal = m_uMax;

	m_uCenter = uTempVal;
}


//QAD_Servo::setMin
//QAD_Servo Control Method
void QAD_Servo::setMin(uint32_t uVal) {
	uint32_t uTempVal = uVal;

	if (uTempVal > m_uMax)
		uTempVal = m_uMax;

	m_uMin = uTempVal;

	if (m_uCenter < m_uMin)
		m_uCenter = m_uMin;

	update();
}


//QAD_Servo::setMax
//QAD_Servo Control Method
void QAD_Servo::setMax(uint32_t uVal) {
	uint32_t uTempVal = uVal;

	if (uTempVal < m_uMin)
		uTempVal = m_uMin;

	m_uMax = uTempVal;

	if (m_uCenter > m_uMax)
		m_uCenter = m_uMax;

	update();
}


//QAD_Servo::setCurrent
//QAD_Servo Control Method
void QAD_Servo::setCurrent(uint32_t uVal) {
	uint32_t uTempVal = uVal;

	if (uTempVal < m_uMin)
		uTempVal = m_uMin;
	if (uTempVal > m_uMax)
		uTempVal = m_uMax;

	m_uCurrent = uTempVal;

	update();
}


//QAD_Servo::setPosition
//QAD_Servo Control Method
void QAD_Servo::setPosition(float fPos) {

	if (fPos == 0.0f) {
		m_uCurrent = m_uCenter;

	} else if (fPos > 0.0f) {

		uint32_t uRange = m_uMax - m_uCenter;
		float fStep = uRange / 100.0f;
		float fVal  = fPos * fStep;
		fVal  = fVal + 0.5f;

		m_uCurrent = m_uCenter + fVal;

	} else {

		uint32_t uRange = m_uCenter - m_uMin;
		float fStep = uRange / 100.0f;
		float fVal  = (0.0f - fPos) * fStep;
		fVal  = fVal - 0.5f;

		m_uCurrent = m_uCenter - fVal;

	}

	update();

}


//QAD_Servo::getCenter
//QAD_Servo Control Method
uint32_t QAD_Servo::getCenter(void) {
  return m_uCenter;
}


//QAD_Servo::getMin
//QAD_Servo Control Method
uint32_t QAD_Servo::getMin(void) {
  return m_uMin;
}


//QAD_Servo::getMax
//QAD_Servo Control Method
uint32_t QAD_Servo::getMax(void) {
  return m_uMax;
}


//QAD_Servo::getCurrent
//QAD_Servo Control Method
uint32_t QAD_Servo::getCurrent(void) {
  return m_uCurrent;
}


//QAD_Servo::center
//QAD_Servo Control Method
void QAD_Servo::center(void) {
  m_uCurrent = m_uCenter;

  update();
}


//QAD_Servo::min
//QAD_Servo Control Method
void QAD_Servo::min(void) {
  m_uCurrent = m_uMin;

  update();
}


//QAD_Servo::max
//QAD_Servo Control Method
void QAD_Servo::max(void) {
  m_uCurrent = m_uMax;

  update();
}


	//--------------------------------
	//--------------------------------
	//QAD_Servo Private Update Methods

//QAD_Servo::update
//QAD_Servo Private Update Method
void QAD_Servo::update(void) {
	m_cPWM->setPWMVal(m_eChannel, m_uCurrent);
}





