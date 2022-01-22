/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: Servo Driver                                                    */
/*   Filename: QAD_Servo.hpp                                               */
/*   Date: 22nd January 2022                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_SERVO_HPP_
#define __QAD_SERVO_HPP_

//Includes
#include "setup.hpp"

#include "QAD_PWM.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//--------------------
//QAD_Servo_InitStruct
typedef struct {

	QAD_PWM*           cPWM;
	QAD_PWM_Channel    eChannel;

	uint32_t           uCenter;
	uint32_t           uMin;
	uint32_t           uMax;
	uint32_t           uCurrent;

} QAD_Servo_InitStruct;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//---------
//QAD_Servo
class QAD_Servo {
private:

	QA_InitState    m_eInitState;

	QAD_PWM*        m_cPWM;
	QAD_PWM_Channel m_eChannel;

	uint32_t        m_uCenter;
	uint32_t        m_uMin;
	uint32_t        m_uMax;
	uint32_t        m_uCurrent;

public:

	//------------
	//Constructors
	QAD_Servo() = delete;

	QAD_Servo(QAD_Servo_InitStruct& sInit) :
		m_eInitState(QA_NotInitialized),
		m_cPWM(sInit.cPWM),
		m_eChannel(sInit.eChannel),
		m_uCenter(sInit.uCenter),
		m_uMin(sInit.uMin),
		m_uMax(sInit.uMax),
	  m_uCurrent(sInit.uCurrent) {};


	//----------------------
	//Initialization Methods

	QA_Result init(void);
	void deinit(void);


	//---------------
	//Control Methods

	void setCenter(uint32_t uVal);
	void setMin(uint32_t uVal);
	void setMax(uint32_t uMax);
	void setCurrent(uint32_t uVal);

	void setPosition(float fPos);

	uint32_t getCenter(void);
	uint32_t getMin(void);
	uint32_t getMax(void);
	uint32_t getCurrent(void);

	void center(void);
	void min(void);
	void max(void);

private:

	//-------------
	//Update Method

	void update(void);

};


//Prevent Recursive Inclusion
#endif /* __QAD_SERVO_HPP_ */








