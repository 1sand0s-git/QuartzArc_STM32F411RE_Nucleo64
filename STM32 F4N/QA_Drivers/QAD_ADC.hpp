/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: ADC Driver                                                      */
/*   Filename: QAD_ADC.hpp                                                 */
/*   Date: 16th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_ADC_HPP_
#define __QAD_ADC_HPP_

//Includes
#include "setup.hpp"

#include "QAD_TimerMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//QAD_ADC_Channel
enum QAD_ADC_Channel : uint8_t {
	QAD_ADC_Channel1 = 0,
	QAD_ADC_Channel2,
	QAD_ADC_Channel3,
	QAD_ADC_Channel4,
	QAD_ADC_Channel5,
	QAD_ADC_Channel6,
	QAD_ADC_Channel7,
	QAD_ADC_Channel8,
	QAD_ADC_Channel9,
	QAD_ADC_Channel10,
	QAD_ADC_Channel11,
	QAD_ADC_Channel12,
	QAD_ADC_Channel13,
	QAD_ADC_Channel14,
	QAD_ADC_Channel15,
	QAD_ADC_Channel16
};


//QAD_ADC_MAXCHANNELS
#define QAD_ADC_MAXCHANNELS    16


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//QAD_ADC_PeriphChannel
enum QAD_ADC_PeriphChannel : uint32_t {
	QAD_ADC_PeriphChannel0       = ADC_CHANNEL_0,
	QAD_ADC_PeriphChannel1       = ADC_CHANNEL_1,
	QAD_ADC_PeriphChannel2       = ADC_CHANNEL_2,
	QAD_ADC_PeriphChannel3       = ADC_CHANNEL_3,
	QAD_ADC_PeriphChannel4       = ADC_CHANNEL_4,
	QAD_ADC_PeriphChannel5       = ADC_CHANNEL_5,
	QAD_ADC_PeriphChannel6       = ADC_CHANNEL_6,
	QAD_ADC_PeriphChannel7       = ADC_CHANNEL_7,
	QAD_ADC_PeriphChannel8       = ADC_CHANNEL_8,
	QAD_ADC_PeriphChannel9       = ADC_CHANNEL_9,
	QAD_ADC_PeriphChannel10      = ADC_CHANNEL_10,
	QAD_ADC_PeriphChannel11      = ADC_CHANNEL_11,
	QAD_ADC_PeriphChannel12      = ADC_CHANNEL_12,
	QAD_ADC_PeriphChannel13      = ADC_CHANNEL_13,
	QAD_ADC_PeriphChannel14      = ADC_CHANNEL_14,
	QAD_ADC_PeriphChannel15      = ADC_CHANNEL_15,
	QAD_ADC_PeriphChannel16      = ADC_CHANNEL_16,
	QAD_ADC_PeriphChannel17      = ADC_CHANNEL_17,
	QAD_ADC_PeriphChannel18      = ADC_CHANNEL_18,
	QAD_ADC_PeriphChannelVRefInt = ADC_CHANNEL_VREFINT,
	QAD_ADC_PeriphChannelVBat    = ADC_CHANNEL_VBAT
};


//QAD_ADC_SamplingTime
enum QAD_ADC_SamplingTime : uint32_t {
	QAD_ADC_PeriphSamplingTime_3Cycles   = ADC_SAMPLETIME_3CYCLES,
	QAD_ADC_PeriphSamplingTime_15Cycles  = ADC_SAMPLETIME_15CYCLES,
	QAD_ADC_PeriphSamplingTime_28Cycles  = ADC_SAMPLETIME_28CYCLES,
	QAD_ADC_PeriphSamplingTime_56Cycles  = ADC_SAMPLETIME_56CYCLES,
	QAD_ADC_PeriphSamplingTime_84Cycles  = ADC_SAMPLETIME_84CYCLES,
	QAD_ADC_PeriphSamplingTime_112Cycles = ADC_SAMPLETIME_112CYCLES,
	QAD_ADC_PeriphSamplingTime_144Cycles = ADC_SAMPLETIME_144CYCLES,
	QAD_ADC_PeriphSamplingTime_480Cycles = ADC_SAMPLETIME_480CYCLES
};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//------------------
//QAD_ADC_InitStruct
typedef struct {

	QAD_Timer_Periph  eTimer;
	uint32_t          uTimer_Prescaler;
	uint32_t          uTimer_Period;

} QAD_ADC_InitStruct;



	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//---------------
//QAD_ADC_Channel
typedef struct QAD_ADC_ChannelData {

	GPIO_TypeDef*    pGPIO;
	uint16_t         uPin;

	QAD_ADC_PeriphChannel eChannel;
	QAD_ADC_SamplingTime  eSamplingTime;


	//---------
	//Operators

	//Assignment operator
	QAD_ADC_ChannelData& operator=(const QAD_ADC_ChannelData& other) {
		pGPIO         = other.pGPIO;
		uPin          = other.uPin;
		eChannel      = other.eChannel;
		eSamplingTime = other.eSamplingTime;
		return *this;
	}

} QAD_ADC_ChannelData;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//-------
//QAD_ADC
class QAD_ADC {
private:
	enum DeinitMode : uint8_t {DeinitPartial = 0, DeinitFull};

	QA_InitState            m_eInitState;
	QA_ActiveState          m_eState;

	QAD_Timer_Periph        m_eTimer;
	uint32_t                m_uTimer_Prescaler;
	uint32_t                m_uTimer_Period;

	TIM_HandleTypeDef       m_sTIMHandle;
	ADC_HandleTypeDef       m_sADCHandle;

	QAD_ADC_ChannelData     m_sChannels[QAD_ADC_MAXCHANNELS];
	uint8_t                 m_uChannelCount;

	uint16_t                m_uData[QAD_ADC_MAXCHANNELS];
	uint8_t                 m_uDataIdx;


	//-----------
	//Constructor
	QAD_ADC() :
		m_eInitState(QA_NotInitialized),
		m_eState(QA_Inactive),
		m_sTIMHandle({0}),
		m_sADCHandle({0}),
		m_uChannelCount(0) {}

public:

	//---------------------------------------------
	//Delete copy constructor & assignment operator
	QAD_ADC(const QAD_ADC& other) = delete;
	QAD_ADC& operator=(const QAD_ADC& other) = delete;


	  //-----------------
	  //Singleton Methods

	static QAD_ADC& get(void) {
		static QAD_ADC instance;
		return instance;
	}


	  //----------------------
	  //Initialization Methods

	static QA_Result init(QAD_ADC_InitStruct& sInit) {
		return get().imp_init(sInit);
	}

	static void deinit(void) {
		get().imp_deinit();
	}


	  //--------------
	  //Handler Method

	static void handler(void) {
		get().imp_handler();
	}


	  //---------------
	  //Control Methods

  static QA_Result start(void) {
  	return get().imp_start();
  }

  static void stop(void) {
  	get().imp_stop();
  }

  static void addChannel(QAD_ADC_ChannelData& sChannel) {
    get().imp_addChannel(sChannel);
  }

  static void removeChannel(QAD_ADC_Channel eChannel) {
    get().imp_removeChannel(eChannel);
  }

  static void removeChannel(QAD_ADC_PeriphChannel eChannel) {
  	get().imp_removeChannelPeriph(eChannel);
  }

  static uint8_t getChannelCount(void) {
  	return get().m_uChannelCount;
  }

		//------------
		//Data Methods

	static uint16_t getData(QAD_ADC_Channel eChannel) {
		return get().m_uData[eChannel];
	}

	static uint16_t getData(QAD_ADC_PeriphChannel eChannel) {
		return get().imp_getDataPeriph(eChannel);
	}

	static uint16_t getData(uint8_t uChannel) {
		return get().imp_getDataIdx(uChannel);
	}

private:

		//----------------------
		//Initialization Methods

	QA_Result imp_init(QAD_ADC_InitStruct& sInit);
	void imp_deinit(void);


		//---------------------------------
		//Peripheral Initialization Methods

	QA_Result imp_periphInit(QAD_ADC_InitStruct& sInit);
	void imp_periphDeinit(DeinitMode eMode);


		//---------------
		//Handler Methods

	void imp_handler(void);


		//---------------
		//Control Methods

	QA_Result imp_start(void);
	void imp_stop(void);

	void imp_addChannel(QAD_ADC_ChannelData& sChannel);
	void imp_removeChannel(QAD_ADC_Channel eChannel);
	void imp_removeChannelPeriph(QAD_ADC_PeriphChannel eChannel);


		//------------
		//Data Methods

	uint16_t imp_getDataPeriph(QAD_ADC_PeriphChannel eChannel);
	uint16_t imp_getDataIdx(uint8_t uChannel);


		//------------
		//Tool Methods

	uint32_t imp_getTrigger(void);
	int8_t imp_findChannel(QAD_ADC_Channel eChannel);
	int8_t imp_findChannelPeriph(QAD_ADC_PeriphChannel eChannel);


};


//Prevent Recursive Inclusion
#endif /* __QAD_ADC_HPP_ */
















