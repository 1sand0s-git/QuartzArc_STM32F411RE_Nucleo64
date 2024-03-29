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


//NOTE: To determine how many PWM channels are supported by a specific timer peripheral, check the QAD_TimerMgr() constructor in QAD_TimerMgr.cpp, or
//alternatively check the reference documentation for the STM32F411RE device.


//---------------------
//QAD_PWM_CHANNEL_COUNT
//
//Used to define the maximum number of channels that can be supported by the QAD_PWM driver
#define QAD_PWM_CHANNEL_COUNT   4


//---------------
//QAD_PWM_Channel
//
//Enum used to select a specific PWM channel
//Note that not all timers support all four PWM channels
enum QAD_PWM_Channel : uint8_t {
	QAD_PWM_Channel_1 = 0,
	QAD_PWM_Channel_2,
	QAD_PWM_Channel_3,
	QAD_PWM_Channel_4
};


//--------------------------
//QAD_PWM_Channel_InitStruct
//
//This structure is used to store data specific to individual PWM channels
//This is used in both QAD_PWM_Init struct and within the QAD_PWM driver itself
typedef struct QAD_PWM_Channel_InitStruct {

	QA_ActiveState eActive;  //Stores whether this particular PWM channel is active. Member of QA_Active state defined in setup.hpp

	GPIO_TypeDef*  pGPIO;    //GPIO port to be used by this PWM channel
	uint16_t       uPin;     //Pin number to be used by this PWM channel
	uint8_t        uAF;      //Alternate function used to connect the GPIO pin to the respective timer peripheral


	//Assignment operator definition to allow easy copying of channel data from QAD_PWM_InitStruct to
	//members of m_sChannels array in QAD_PWM driver class
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
//
//This structure is used to be able to create the QAD_PWM driver class
typedef struct {

	QAD_Timer_Periph  eTimer;       //Timer peripheral to be used. Member of QAD_Timer_Periph as defined in QAD_TimerMgr.hpp

	uint32_t          uPrescaler;   //Prescaler to be used for the selected timer
	uint32_t          uPeriod;      //Counter period to be used for the selected timer

	QAD_PWM_Channel_InitStruct sChannels[QAD_PWM_CHANNEL_COUNT];  //Data for individual PWM channels
	                                                              //Note that although four channels worth of init data can be supplied, the selected
	                                                              //timer peripheral may support less than four channels

} QAD_PWM_InitStruct;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------
//QAD_PWM
//
//Driver class used for generating PWM signals on between one and four channels
//Note that the number of available channels is determined by the number of channels supported by the selected timer peripheral
class QAD_PWM {
private:

	//Deinitialization mode to be used by periphDeinit() method
  enum DeinitMode : uint8_t {
  	DeinitPartial = 0,    //Only to be used for partial deinitialization upon initialization failure in periphInit() method
  	DeinitFull            //Used for full driver deinitialization when driver is in a fully initialized state
  };

  QA_InitState       m_eInitState;  //Stores whether the driver is currently initialized. Member of QA_InitState enum defined in setup.hpp
  QA_ActiveState     m_eState;      //Stores whether the driver is currently active. Member of QA_ActiveState enum defined in setup.hpp

  QAD_Timer_Periph   m_eTimer;      //Stores the particular timer peripheral to be used by the driver
                                    //Member of QAD_Timer_Periph as defined in QAD_TimerMgr.hpp

  TIM_HandleTypeDef  m_sHandle;     //Handle used by HAL functions to access Timer peripheral (defined in stm32f4xx_hal_tim.h)

  uint32_t           m_uPrescaler;  //Prescaler to be used for selected timer
  uint32_t           m_uPeriod;     //Counter period to be used for selected timer

  QAD_PWM_Channel_InitStruct m_sChannels[QAD_PWM_CHANNEL_COUNT];  //Array of channel specific data
                                                                  //See QAD_PWM_Channel_InitStruct for more details

  uint32_t           m_uChannelSelect[QAD_PWM_CHANNEL_COUNT];     //Array used to select TIM_Channel defines as defined in stm32f4xx_hal_tim.h

public:

  //--------------------------
	//Constructors / Destructors

  QAD_PWM() = delete;                   //Delete the default class constructor, as we need an initialization structure to be provided on class creation

  QAD_PWM(QAD_PWM_InitStruct& sInit) :  //The class constructor to be used, which has a reference to an initialization structure passed to it
  	m_eInitState(QA_NotInitialized),
		m_eState(QA_Inactive),
		m_eTimer(sInit.eTimer),
		m_sHandle({0}),
		m_uPrescaler(sInit.uPrescaler),
		m_uPeriod(sInit.uPeriod) {

  	//Copy channel specific data from initialization structure to m_sChannels array in QAD_PWM class
  	for (uint8_t i=0; i<QAD_PWM_CHANNEL_COUNT; i++) {
  		m_sChannels[i] = sInit.sChannels[i];
  	}

  	//Fill out m_uChannelSelect array with TIM_Channel defines
  	m_uChannelSelect[QAD_PWM_Channel_1] = TIM_CHANNEL_1;
  	m_uChannelSelect[QAD_PWM_Channel_2] = TIM_CHANNEL_2;
  	m_uChannelSelect[QAD_PWM_Channel_3] = TIM_CHANNEL_3;
  	m_uChannelSelect[QAD_PWM_Channel_4] = TIM_CHANNEL_4;
  }

  ~QAD_PWM() {        //Destructor to make sure peripheral is made inactive and deinitialized upon class destruction

  	//Stop PWM driver if currently active
  	if (m_eState)
  		stop();

  	//Deinitialize PWM driver if currently initialized
  	if (m_eInitState)
  		deinit();
  }


  //NOTE: See QAD_PWM.cpp for details of the following functions

  //----------------------
  //Initialization Methods

  QA_Result init(void);
  void deinit(void);


  //---------------
  //Control Methods

  void start(void);
  void stop(void);

  void setPWMVal(QAD_PWM_Channel eChannel, uint16_t uVal);

  uint16_t getPeriod(void);

private:

  //----------------------
  //Initialization Methods

  QA_Result periphInit(void);
  void periphDeinit(DeinitMode eDeinitMode);

};


//Prevent Recursive Inclusion
#endif /* __QAD_PWM_HPP_ */









