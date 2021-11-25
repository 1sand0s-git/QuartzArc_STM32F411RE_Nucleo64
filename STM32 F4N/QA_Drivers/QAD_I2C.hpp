/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: I2C Driver                                                      */
/*   Filename: QAD_I2C.hpp                                                 */
/*   Date: 25th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_I2C_HPP_
#define __QAD_I2C_HPP_

//Includes
#include "setup.hpp"

#include "QAD_I2CMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//-----------------
//QAD_I2C_DutyCycle
enum QAD_I2C_DutyCycle : uint32_t {
	QAD_I2C_DutyCycle_2     = I2C_DUTYCYCLE_2,
	QAD_I2C_DutyCycle_16_9  = I2C_DUTYCYCLE_16_9
};


//---------------------
//QAD_I2C_AddrssingMode
enum QAD_I2C_AddressingMode : uint32_t {
	QAD_I2C_AddressingMode_7Bit  = I2C_ADDRESSINGMODE_7BIT,
	QAD_I2C_AddressingMode_10Bit = I2C_ADDRESSINGMODE_10BIT
};


//--------------------------
//QAD_I2C_DualAddressingMode
enum QAD_I2C_DualAddressingMode : uint32_t {
	QAD_I2C_DualAddressingMode_Disable  = I2C_DUALADDRESS_DISABLE,
	QAD_I2C_DualAddressingMode_Enable   = I2C_DUALADDRESS_ENABLE
};


//---------------------------------
//QAD_I2C_GeneralCallMode
enum QAD_I2C_GeneralCallMode : uint32_t {
  QAD_I2C_GeneralCallMode_Disable  = I2C_GENERALCALL_DISABLE,
  QAD_I2C_GeneralCallMode_Enable   = I2C_GENERALCALL_ENABLE
};


//QAD_I2C_NoStretchMode
enum QAD_I2C_NoStretchMode : uint32_t {
	QAD_I2C_NoStretchMode_Disable  = I2C_NOSTRETCH_DISABLE,
	QAD_I2C_NoStretchMode_Enable   = I2C_NOSTRETCH_ENABLE
};


//------------------------------------------
//------------------------------------------
//------------------------------------------

//------------------
//QAD_I2C_InitStruct
typedef struct {

	QAD_I2C_Periph              eI2C;
	uint8_t                     uIRQPriority_Event;
	uint8_t                     uIRQPriority_Error;

	uint32_t                    uClockSpeed;       // < 400kHz

	QAD_I2C_DutyCycle           eDutyCycle;
	QAD_I2C_AddressingMode      eAddressingMode;
	QAD_I2C_DualAddressingMode  eDualAddressingMode;
	QAD_I2C_GeneralCallMode     eGeneralCallMode;
	QAD_I2C_NoStretchMode       eNoStretchMode;

	uint16_t                    uOwnAddress1;
	uint16_t                    uOwnAddress2;

	GPIO_TypeDef*               pSCL_GPIO;
	uint16_t                    uSCL_Pin;
	uint8_t                     uSCL_AF;

	GPIO_TypeDef*               pSDA_GPIO;
	uint16_t                    uSDA_Pin;
	uint8_t                     uSDA_AF;

} QAD_I2C_InitStruct;


//------------------------------------------
//------------------------------------------
//------------------------------------------

//-------
//QAD_I2C
class QAD_I2C {
private:
	enum DeinitMode : uint8_t {DeinitPartial = 0, DeinitFull};

	const uint16_t              m_uTimeout = 1000;

	const uint8_t               m_uMemAddrSize8Bit  = 1;
	const uint8_t               m_uMemAddrSize16Bit = 2;

	QA_InitState                m_eInitState;
	QA_ActiveState              m_eState;

	QAD_I2C_Periph              m_eI2C;
	uint8_t                     m_uIRQPriority_Event;
	uint8_t                     m_uIRQPriority_Error;

	uint32_t                    m_uClockSpeed;

	QAD_I2C_DutyCycle           m_eDutyCycle;
	QAD_I2C_AddressingMode      m_eAddressingMode;
	QAD_I2C_DualAddressingMode  m_eDualAddressingMode;
	QAD_I2C_GeneralCallMode     m_eGeneralCallMode;
	QAD_I2C_NoStretchMode       m_eNoStretchMode;

	uint16_t                    m_uOwnAddress1;
	uint16_t                    m_uOwnAddress2;

	GPIO_TypeDef*               m_pSCL_GPIO;
	uint16_t                    m_uSCL_Pin;
	uint8_t                     m_uSCL_AF;

	GPIO_TypeDef*               m_pSDA_GPIO;
	uint16_t                    m_uSDA_Pin;
	uint8_t                     m_uSDA_AF;

	IRQn_Type                   m_eIRQ_Event;
	IRQn_Type                   m_eIRQ_Error;
	I2C_HandleTypeDef           m_sHandle;

public:

		//--------------------------
		//Constructors / Destructors

	QAD_I2C() = delete;

	QAD_I2C(QAD_I2C_InitStruct& sInit) :
		m_eInitState(QA_NotInitialized),
		m_eState(QA_Inactive),
		m_eI2C(sInit.eI2C),
		m_uIRQPriority_Event(sInit.uIRQPriority_Event),
		m_uIRQPriority_Error(sInit.uIRQPriority_Error),
		m_uClockSpeed(sInit.uClockSpeed),
		m_eDutyCycle(sInit.eDutyCycle),
		m_eAddressingMode(sInit.eAddressingMode),
		m_eDualAddressingMode(sInit.eDualAddressingMode),
		m_eGeneralCallMode(sInit.eGeneralCallMode),
		m_eNoStretchMode(sInit.eNoStretchMode),
		m_uOwnAddress1(sInit.uOwnAddress1),
		m_uOwnAddress2(sInit.uOwnAddress2),
		m_pSCL_GPIO(sInit.pSCL_GPIO),
		m_uSCL_Pin(sInit.uSCL_Pin),
		m_uSCL_AF(sInit.uSCL_AF),
		m_pSDA_GPIO(sInit.pSDA_GPIO),
		m_uSDA_Pin(sInit.uSDA_Pin),
		m_uSDA_AF(sInit.uSDA_AF),
		m_eIRQ_Event(I2C1_EV_IRQn),
		m_eIRQ_Error(I2C1_ER_IRQn),
		m_sHandle({0}) {}


	~QAD_I2C() {

		//Stop Driver if currently active
		if (m_eState)
			stop();

		if (m_eInitState)
			deinit();
	}


		//----------------------
		//Initialization Methods

	QA_Result init(void);
	void deinit(void);

	QA_InitState getInitState(void);
	I2C_HandleTypeDef& getHandle(void);


		//---------------
		//Control Methods

	void start(void);
	void stop(void);
	QA_ActiveState getState(void);


		//------------------
		//Transceive Methods

	QA_Result write8Bit(uint16_t uAddr, uint8_t uReg, uint8_t uValue);
	QA_Result write16Bit(uint16_t uAddr, uint16_t uReg, uint16_t uValue);
	QA_Result writeMultiple8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pData, uint16_t uLength);

	QA_Result read8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pValue);
	QA_Result read16Bit(uint16_t uAddr, uint16_t uReg, uint16_t* pValue);
	QA_Result readMultiple8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pData, uint16_t uLength);


private:


		//----------------------
		//Initialization Methods

	QA_Result periphInit(void);
	void periphDeinit(DeinitMode eDeinitMode);


		//------------------
		//Transceive Methods

	QA_Result write(uint16_t uAddr, uint16_t uReg, uint16_t uMemAddress, uint8_t* pData, uint16_t uLength);
	QA_Result read(uint16_t uAddr, uint16_t uReg, uint16_t uMemAddress, uint8_t* pData, uint16_t uLength);

};


//Prevent Recursive Inclusion
#endif /* __QAD_I2C_HPP_ */
