/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: RGB LED Driver                                                  */
/*   Filename: QAD_RGB.cpp                                                 */
/*   Date: 14th January 2022                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2022 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_RGB.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


  //------------------------------
  //------------------------------
  //QAD_RGB Initialization Methods

//QAD_RGB::init
//QAD_RGB Initialization Method
QA_Result QAD_RGB::init(void) {

	//Set Color
	setColor();

	//Set Driver States
	m_eInitState = QA_Initialized;

	//Return
	return QA_OK;
}


//QAD_RGB::deinit
//QAD_RGB Initialization Method
void QAD_RGB::deinit(void) {

	//

	//Set Driver States
	m_eInitState = QA_NotInitialized;
}


	//-----------------------
	//-----------------------
	//QAD_RGB Control Methods

//QAD_RGB::getRed
//QAD_RGB Control Method
uint8_t QAD_RGB::getRed(void) {
  return m_uRed;
}


//QAD_RGB::getGreen
//QAD_RGB Control Method
uint8_t QAD_RGB::getGreen(void) {
  return m_uGreen;
}


//QAD_RGB::getBlue
//QAD_RGB Control Method
uint8_t QAD_RGB::getBlue(void) {
  return m_uBlue;
}



//QAD_RGB::getBrightness
//QAD_RGB Control Method
uint8_t QAD_RGB::getBrightness(void) {
	return m_uBrightness;
}


//QAD_RGB::getInvert
//QAD_RGB Control Method
bool QAD_RGB::getInvert(void) {
	return m_bInvert;
}


//QAD_RGB::setRed
//QAD_RGB Control Method
void QAD_RGB::setRed(uint8_t uRed) {

	m_uRed = uRed;
	setColor();
}


//QAD_RGB::setGreen
//QAD_RGB Control Method
void QAD_RGB::setGreen(uint8_t uGreen) {

	m_uGreen = uGreen;
	setColor();
}


//QAD_RGB::setBlue
//QAD_RGB Control Method
void QAD_RGB::setBlue(uint8_t uBlue) {

	m_uBlue = uBlue;
	setColor();
}


//QAD_RGB::setRGB
//QAD_RGB Control Method
void QAD_RGB::setRGB(uint8_t uRed, uint8_t uGreen, uint8_t uBlue) {

	m_uRed   = uRed;
	m_uGreen = uGreen;
	m_uBlue  = uBlue;
	setColor();
}


//QAD_RGB::setRGB
//QAD_RGB Control Method
void QAD_RGB::setRGB(uint32_t uRGB) {

	m_uRed   = (uRGB & 0x00FF0000) >> 16;
	m_uGreen = (uRGB & 0x0000FF00) >> 8;
	m_uBlue  = (uRGB & 0x000000FF);
	setColor();
}


//QAD_RGB::setBrightness
//QAD_RGB Control Method
void QAD_RGB::setBrightness(uint8_t uBrightness) {

	m_uBrightness = uBrightness;
	setColor();
}


//QAD_RGB::setInvert
//QAD_RGB Control Method
void QAD_RGB::setInvert(bool bInvert) {

	m_bInvert = bInvert;
	setColor();
}


	//------------------------------
	//------------------------------
	//QAD_RGB Private Control Method

//QAD_RGB::setColor
//QAD_RGB Private Control Method
void QAD_RGB::setColor(void) {

	uint16_t uPeriod = m_cPWM->getPeriod();

	float fInc = uPeriod / 255.0f;

	float fBrightness = (m_uBrightness / 255.0f) * fInc;

	float fRed   = m_uRed * fBrightness;
	float fGreen = m_uGreen * fBrightness;
	float fBlue  = m_uBlue * fBrightness;

	m_cPWM->setPWMVal(m_eRedChannel, m_bInvert ? uPeriod - fRed : fRed);
	m_cPWM->setPWMVal(m_eGreenChannel, m_bInvert ? uPeriod - fGreen : fGreen);
	m_cPWM->setPWMVal(m_eBlueChannel, m_bInvert ? uPeriod - fBlue : fBlue);

}

