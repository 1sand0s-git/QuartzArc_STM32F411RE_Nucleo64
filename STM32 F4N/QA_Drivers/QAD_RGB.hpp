/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: RGB LED Driver                                                  */
/*   Filename: QAD_RGB.hpp                                                 */
/*   Date: 14th January 2022                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2022 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_RGB_HPP_
#define __QAD_RGB_HPP_

//Includes
#include "setup.hpp"

#include "QAD_PWM.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//------------------
//QAD_RGB_InitStruct
typedef struct {

	QAD_PWM*            cPWM;

	QAD_PWM_Channel     eRedChannel;
	QAD_PWM_Channel     eGreenChannel;
	QAD_PWM_Channel     eBlueChannel;

	uint8_t             uRed;
	uint8_t             uGreen;
	uint8_t             uBlue;

	uint8_t             uBrightness;
	bool                bInvert;      //Non-inverted = common Anode, Inverted = common Cathode

} QAD_RGB_InitStruct;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//-------
//QAD_RGB
class QAD_RGB {
private:

	QA_InitState         m_eInitState;

	QAD_PWM*             m_cPWM;

	QAD_PWM_Channel      m_eRedChannel;
	QAD_PWM_Channel      m_eGreenChannel;
	QAD_PWM_Channel      m_eBlueChannel;

	uint8_t              m_uRed;
	uint8_t              m_uGreen;
	uint8_t              m_uBlue;

	uint8_t              m_uBrightness;
	bool                 m_bInvert;

public:

	//-----------
	//Constructor

	//Delete default constructor
	QAD_RGB() = delete;

	QAD_RGB(QAD_RGB_InitStruct& sInit) :
		m_eInitState(QA_NotInitialized),
		m_cPWM(sInit.cPWM),
		m_eRedChannel(sInit.eRedChannel),
		m_eGreenChannel(sInit.eGreenChannel),
		m_eBlueChannel(sInit.eBlueChannel),
		m_uRed(sInit.uRed),
		m_uGreen(sInit.uGreen),
		m_uBlue(sInit.uBlue),
		m_uBrightness(sInit.uBrightness),
		m_bInvert(sInit.bInvert) {}


	//----------------------
	//Initialization Methods

	QA_Result init(void);
	void deinit(void);


	//---------------
	//Control Methods

	uint8_t getRed(void);
	uint8_t getGreen(void);
	uint8_t getBlue(void);

	uint8_t getBrightness(void);
	bool getInvert(void);

	void setRed(uint8_t uRed);
	void setGreen(uint8_t uGreen);
	void setBlue(uint8_t uBlue);

	void setRGB(uint8_t uRed, uint8_t uGreen, uint8_t uBlue);
	void setRGB(uint32_t uRGB);

	void setBrightness(uint8_t uBrightness);
	void setInvert(bool bInvert);

private:

	//--------------
	//Control Method

	void setColor(void);

};


//Prevent Recursive Inlusion
#endif /* __QAD_RGB_HPP_ */
