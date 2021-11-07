/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: PWM Driver                                                      */
/*   Filename: QAD_PWM.hpp                                                 */
/*   Date: 1st November 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_PWM_HPP_
#define __QAD_PWM_HPP_

//Includes
#include "setup.hpp"

#include "QAD_TimerMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//QAD_PWM_CHANNEL_COUNT
#define QAD_PWM_CHANNEL_COUNT   4


//--------------------------
//QAD_PWM_Channel_InitStruct
typedef struct QAD_PWM_Channel_InitStruct {

	QA_ActiveState eActive;

	GPIO_TypeDef*  pGPIO;
	uint16_t       uPin;
	uint8_t        uAF;

	QAD_PWM_Channel_InitStruct& operator=(const QAD_PWM_Channel_InitStruct& other) {
		eActive   = other.eActive;
		pGPIO     = other.pGPIO;
		uPin      = other.uPin;
		uAF       = other.uAF;
		return *this;
	}

} QAD_PWM_Channel_InitStruct;


//------------------
//QAD_PWM_InitStruct
typedef struct {

	QAD_Timer_Periph  eTimer;

	uint32_t          uPrescaler;
	uint32_t          uPeriod;

	QAD_PWM_Channel_InitStruct sChannels[QAD_PWM_CHANNEL_COUNT];

} QAD_PWM_InitStruct;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------
//QAD_PWM
class QAD_PWM {
public:
	enum PWM_Channel : uint8_t {PWM_Channel_1 = 0, PWM_Channel_2, PWM_Channel_3, PWM_Channel_4};

private:
  enum DeinitMode : uint8_t {DeinitPartial = 0, DeinitFull};

  QA_InitState       m_eInitState;
  QA_ActiveState     m_eState;

  QAD_Timer_Periph   m_eTimer;
  TIM_HandleTypeDef  m_sHandle;

  uint32_t           m_uPrescaler;
  uint32_t           m_uPeriod;

  QAD_PWM_Channel_InitStruct m_sChannels[QAD_PWM_CHANNEL_COUNT];

  uint32_t           m_uChannelSelect[QAD_PWM_CHANNEL_COUNT];

public:

	//Constructors/Destructors
  QAD_PWM() = delete;
  QAD_PWM(QAD_PWM_InitStruct& sInit) :
  	m_eInitState(QA_NotInitialized),
		m_eState(QA_Inactive),
		m_eTimer(sInit.eTimer),
		m_sHandle({0}),
		m_uPrescaler(sInit.uPrescaler),
		m_uPeriod(sInit.uPeriod) {

  	for (uint8_t i=0; i<QAD_PWM_CHANNEL_COUNT; i++) {
  		m_sChannels[i] = sInit.sChannels[i];
  	}

  	m_uChannelSelect[PWM_Channel_1] = TIM_CHANNEL_1;
  	m_uChannelSelect[PWM_Channel_2] = TIM_CHANNEL_2;
  	m_uChannelSelect[PWM_Channel_3] = TIM_CHANNEL_3;
  	m_uChannelSelect[PWM_Channel_4] = TIM_CHANNEL_4;
  }

  ~QAD_PWM() {
  	if (m_eState)
  		stop();
  	if (m_eInitState)
  		deinit();
  }

  //Initialization Methods
  QA_Result init(void);
  void deinit(void);

  //Control Methods
  void start(void);
  void stop(void);

  void setPWMVal(PWM_Channel eChannel, uint16_t uVal);

private:

  //Initialization Methods
  QA_Result periphInit(void);
  void periphDeinit(DeinitMode eMode);

};


//Prevent Recursive Inclusion
#endif /* __QAD_PWM_HPP_ */









