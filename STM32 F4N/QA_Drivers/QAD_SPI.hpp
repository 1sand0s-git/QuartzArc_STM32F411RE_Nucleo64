/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: SPI Management Driver                                           */
/*   Filename: QAD_SPIMgr.hpp                                              */
/*   Date: 13th December 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_SPI_HPP_
#define __QAD_SPI_HPP_

//Includes
#include "setup.hpp"

#include "QAD_SPIMgr.hpp"


	//------------------------------------------
	//------------------------------------------
  //------------------------------------------


//------------
//QAD_SPI_Mode
enum QAD_SPI_Mode : uint32_t {
  QAD_SPI_Mode_Slave  = SPI_MODE_SLAVE,
  QAD_SPI_Mode_Master = SPI_MODE_MASTER
};


//-------------
//QAD_SPI_BiDir
enum QAD_SPI_BiDir : uint8_t {
	QAD_SPI_BiDir_TXOnly = 0,
	QAD_SPI_BiDir_RXOnly,
	QAD_SPI_BiDir_Enabled
};


//-------------
//QAD_SPI_Lines
enum QAD_SPI_Lines : uint8_t {
	QAD_SPI_Lines_1Line = 0,
	QAD_SPI_Lines_2Lines
};


//----------------
//QAD_SPI_DataSize
enum QAD_SPI_DataSize : uint32_t {
	QAD_SPI_DataSize_8bit  = SPI_DATASIZE_8BIT,
	QAD_SPI_DataSize_16bit = SPI_DATASIZE_16BIT
};


//-------------------
//QAD_SPI_ClkPolarity
enum QAD_SPI_ClkPolarity : uint32_t {
	QAD_SPI_ClkPolarity_Low  = SPI_POLARITY_LOW,
	QAD_SPI_ClkPolarity_High = SPI_POLARITY_HIGH
};


//----------------
//QAD_SPI_ClkPhase
enum QAD_SPI_ClkPhase : uint32_t {
	QAD_SPI_ClkPhase_1Edge = SPI_PHASE_1EDGE,
	QAD_SPI_ClkPhase_2Edge = SPI_PHASE_2EDGE
};


//----------
//QAD_SPI_CS
enum QAD_SPI_CS : uint32_t {
	QAD_SPI_CS_Soft        = SPI_NSS_SOFT,
	QAD_SPI_CS_Hard_Input  = SPI_NSS_HARD_INPUT,
	QAD_SPI_CS_Hard_Output = SPI_NSS_HARD_OUTPUT
};


//---------------------
//QAD_SPI_BaudPrescaler
enum QAD_SPI_BaudPrescaler : uint32_t {
	QAD_SPI_BaudPrescaler_2   = SPI_BAUDRATEPRESCALER_2,
	QAD_SPI_BaudPrescaler_4   = SPI_BAUDRATEPRESCALER_4,
	QAD_SPI_BaudPrescaler_8   = SPI_BAUDRATEPRESCALER_8,
	QAD_SPI_BaudPrescaler_16  = SPI_BAUDRATEPRESCALER_16,
	QAD_SPI_BaudPrescaler_32  = SPI_BAUDRATEPRESCALER_32,
	QAD_SPI_BaudPrescaler_64  = SPI_BAUDRATEPRESCALER_64,
	QAD_SPI_BaudPrescaler_128 = SPI_BAUDRATEPRESCALER_128,
	QAD_SPI_BaudPrescaler_256 = SPI_BAUDRATEPRESCALER_256
};


//----------------
//QAD_SPI_FirstBit
enum QAD_SPI_FirstBit : uint32_t {
	QAD_SPI_FirstBit_MSB = SPI_FIRSTBIT_MSB,
	QAD_SPI_FirstBit_LSB = SPI_FIRSTBIT_LSB
};


//--------------
//QAD_SPI_TIMode
enum QAD_SPI_TIMode : uint32_t {
	QAD_SPI_TIMode_Disable = SPI_TIMODE_DISABLE,
	QAD_SPI_TIMode_Enable  = SPI_TIMODE_ENABLE
};


//-----------
//QAD_SPI_CRC
enum QAD_SPI_CRC : uint32_t {
	QAD_SPI_CRC_Disable = SPI_CRCCALCULATION_DISABLE,
	QAD_SPI_CRC_Enable  = SPI_CRCCALCULATION_ENABLE
};


//------------------------------------------
//------------------------------------------
//------------------------------------------

//------------------
//QAD_SPI_InitStruct
typedef struct {

	QAD_SPI_Periph        eSPI;
	uint8_t               uIRQPriority;

	QAD_SPI_Mode          eSPIMode;
	QAD_SPI_BiDir         eSPIBiDir;
	QAD_SPI_Lines         eSPILines;
	QAD_SPI_DataSize      eSPIDataSize;
	QAD_SPI_ClkPolarity   eSPIClkPolarity;
	QAD_SPI_ClkPhase      eSPIClkPhase;
	QAD_SPI_CS            eSPICS;
	QAD_SPI_BaudPrescaler eSPIPrescaler;
	QAD_SPI_FirstBit      eSPIFirstBit;
	QAD_SPI_TIMode        eSPITIMode;
	QAD_SPI_CRC           eSPICRC;
	uint32_t              uSPICRCPolynomial;

	GPIO_TypeDef*         pClk_GPIO;
	uint16_t              uClk_Pin;
	uint8_t               uClk_AF;

	GPIO_TypeDef*         pMOSI_GPIO;
	uint16_t              uMOSI_Pin;
	uint8_t               uMOSI_AF;

	GPIO_TypeDef*         pMISO_GPIO;
	uint16_t              uMISO_Pin;
	uint8_t               uMISO_AF;

	GPIO_TypeDef*         pCS_GPIO;
	uint16_t              uCS_Pin;
	uint8_t               uCS_AF;

} QAD_SPI_InitStruct;


//------------------------------------------
//------------------------------------------
//------------------------------------------

//-------
//QAD_SPI
class QAD_SPI {
private:
	enum DeinitMode : uint8_t {DeinitPartial = 0, DeinitFull};

	const uint16_t        m_uTimeout = 0xFFFF;

	QA_InitState          m_eInitState;
	QA_ActiveState        m_eState;

	QAD_SPI_Periph        m_eSPI;
	uint8_t               m_uIRQPriority;

	QAD_SPI_Mode          m_eSPIMode;
	QAD_SPI_BiDir         m_eSPIBiDir;
	QAD_SPI_Lines         m_eSPILines;
	QAD_SPI_DataSize      m_eSPIDataSize;
	QAD_SPI_ClkPolarity   m_eSPIClkPolarity;
	QAD_SPI_ClkPhase      m_eSPIClkPhase;
	QAD_SPI_CS            m_eSPICS;
	QAD_SPI_BaudPrescaler m_eSPIPrescaler;
	QAD_SPI_FirstBit      m_eSPIFirstBit;
	QAD_SPI_TIMode        m_eSPITIMode;
	QAD_SPI_CRC           m_eSPICRC;
	uint32_t              m_uSPICRCPolynomial;

	GPIO_TypeDef*         m_pClk_GPIO;
	uint16_t              m_uClk_Pin;
	uint8_t               m_uClk_AF;

	GPIO_TypeDef*         m_pMOSI_GPIO;
	uint16_t              m_uMOSI_Pin;
	uint8_t               m_uMOSI_AF;

	GPIO_TypeDef*         m_pMISO_GPIO;
	uint16_t              m_uMISO_Pin;
	uint8_t               m_uMISO_AF;

	GPIO_TypeDef*         m_pCS_GPIO;
	uint16_t              m_uCS_Pin;
	uint8_t               m_uCS_AF;
	bool                  m_bCS_Soft;

	IRQn_Type             m_eIRQ;
	SPI_HandleTypeDef     m_sHandle;

public:

		//-------------------------
		//Contructors / Desutrctors

	QAD_SPI() = delete;

	QAD_SPI(QAD_SPI_InitStruct& sInit) :
		m_eInitState(QA_NotInitialized),
		m_eState(QA_Inactive),
		m_eSPI(sInit.eSPI),
		m_uIRQPriority(sInit.uIRQPriority),
		m_eSPIMode(sInit.eSPIMode),
		m_eSPIBiDir(sInit.eSPIBiDir),
		m_eSPILines(sInit.eSPILines),
		m_eSPIDataSize(sInit.eSPIDataSize),
		m_eSPIClkPolarity(sInit.eSPIClkPolarity),
		m_eSPIClkPhase(sInit.eSPIClkPhase),
		m_eSPICS(sInit.eSPICS),
		m_eSPIPrescaler(sInit.eSPIPrescaler),
		m_eSPIFirstBit(sInit.eSPIFirstBit),
		m_eSPITIMode(sInit.eSPITIMode),
		m_eSPICRC(sInit.eSPICRC),
		m_uSPICRCPolynomial(sInit.uSPICRCPolynomial),
		m_pClk_GPIO(sInit.pClk_GPIO),
		m_uClk_Pin(sInit.uClk_Pin),
		m_uClk_AF(sInit.uClk_AF),
		m_pMOSI_GPIO(sInit.pMOSI_GPIO),
		m_uMOSI_Pin(sInit.uMOSI_Pin),
		m_uMOSI_AF(sInit.uMOSI_AF),
		m_pMISO_GPIO(sInit.pMISO_GPIO),
		m_uMISO_Pin(sInit.uMISO_Pin),
		m_uMISO_AF(sInit.uMISO_AF),
		m_pCS_GPIO(sInit.pCS_GPIO),
		m_uCS_Pin(sInit.uCS_Pin),
		m_uCS_AF(sInit.uCS_AF),
		m_bCS_Soft(false),
		m_eIRQ(SPI1_IRQn),
		m_sHandle({0}) {}


	~QAD_SPI() {

		//Stop Driver if currently active
		if (m_eState)
			stop();

		//Deinitialize Driver if currently initialized
		if (m_eInitState)
			deinit();
	}


		//----------------------
		//Initialization Methods

	QA_Result init(void);
	void deinit(void);

	QA_InitState getInitState(void);
	SPI_HandleTypeDef& getHandle(void);


		//---------------
		//Control Methods

	void start(void);
	void stop(void);
	QA_ActiveState getState(void);


		//------------------
		//Transceive Methods

	QA_Result transmit(uint8_t* pTXData, uint16_t uSize);
	QA_Result receive(uint8_t* pRXData, uint16_t uSize);
	QA_Result transceive(uint8_t* pTXData, uint8_t* pRXData, uint16_t uSize);


private:


	//----------------------
	//Initialization Methods
	QA_Result periphInit(void);
	void periphDeinit(DeinitMode eDeinitMode);


};


//Prevent Recursive Inclusion
#endif /* __QAD_SPI_HPP_ */
