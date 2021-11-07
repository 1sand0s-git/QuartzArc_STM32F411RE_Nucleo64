/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: Encoder Driver                                                  */
/*   Filename: QAD_Encoder.hpp                                             */
/*   Date: 28th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_ENCODER_HPP_
#define __QAD_ENCODER_HPP_

//Includes
#include "setup.hpp"

#include "QAD_TimerMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

enum QAD_EncoderMode : uint8_t {QAD_EncoderMode_Linear = 0, QAD_EncoderMode_Exp};

//QAD_Encoder_InitStruct
typedef struct {

	GPIO_TypeDef*     pCh1_GPIO;
	uint16_t          uCh1_Pin;
	uint8_t           uCh1_AF;

	GPIO_TypeDef*     pCh2_GPIO;
	uint16_t          uCh2_Pin;
	uint8_t           uCh2_AF;

	QAD_Timer_Periph  eTimer;

	QAD_EncoderMode   eMode;

} QAD_Encoder_InitStruct;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//QAD_Encoder
class QAD_Encoder {
private:

	const uint16_t     m_uMaxVal = 1024;

	GPIO_TypeDef*      m_pCh1_GPIO;
	uint16_t           m_uCh1_Pin;
	uint8_t            m_uCh1_AF;

	GPIO_TypeDef*      m_pCh2_GPIO;
	uint16_t           m_uCh2_Pin;
	uint8_t            m_uCh2_AF;

	QAD_Timer_Periph   m_eTimer;
	TIM_HandleTypeDef  m_sHandle;

	QA_InitState       m_eInitState;

	QA_ActiveState     m_eState;

	uint16_t           m_uValueOld;
	uint16_t           m_uValueNew;
	int16_t            m_iValue;
	uint16_t           m_uAccel;

	QAD_EncoderMode    m_eMode;

public:
	enum DeinitMode : uint8_t {DeinitPartial = 0, DeinitFull};

  QAD_Encoder() = delete;
  QAD_Encoder(QAD_Encoder_InitStruct& sInit) :
  	m_pCh1_GPIO(sInit.pCh1_GPIO),
		m_uCh1_Pin(sInit.uCh1_Pin),
		m_uCh1_AF(sInit.uCh1_AF),
		m_pCh2_GPIO(sInit.pCh2_GPIO),
		m_uCh2_Pin(sInit.uCh2_Pin),
		m_uCh2_AF(sInit.uCh2_AF),
		m_eTimer(sInit.eTimer),
		m_sHandle({0}),
		m_eInitState(QA_NotInitialized),
		m_eState(QA_Inactive),
		m_uValueOld(0),
		m_uValueNew(0),
		m_iValue(0),
		m_uAccel(0),
		m_eMode(sInit.eMode) {};

  ~QAD_Encoder() {
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

  void update(uint32_t uTicks);

  QA_ActiveState getState(void);
  int16_t getValue(void);
  uint16_t getAccel(void);

  void setMode(QAD_EncoderMode eMode);
  QAD_EncoderMode getMode(void);

private:

  //Initialization Methods
  QA_Result periphInit(void);
  void periphDeinit(DeinitMode eMode);

  //Tool Methods
  void clearData(void);

};

//Prevent Recursive Inclusion
#endif /* __QAD_ENCODER_HPP_ */












