/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: UART Driver                                                     */
/*   Filename: QAD_UART.hpp                                                */
/*   Date: 12th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */


//NOTE:
//The QAD_UART driver class is not intended to be used on its own, and is specifically used by the
//serial communications system QAS_Serial_Dev_UART class defined in QAS_Serial_Dev_UART.hpp


//Prevent Recursive Inclusion
#ifndef __QAD_UART_HPP_
#define __QAD_UART_HPP_

//Includes
#include "setup.hpp"

#include "QAD_UARTMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------------------
//QAD_UART_InitStruct
//
//This structure is used to be able to create the QAD_UART driver class
typedef struct {

  QAD_UART_Periph uart;         //UART peripheral to be used (member of QAD_UART_Periph, as defined in QAD_UARTMgr.hpp)
  uint32_t        baudrate;     //Baudrate to be used for UART peripheral
  uint8_t         irqpriority;  //IRQ priority to be used for TX and RX interrupts

  GPIO_TypeDef*   txgpio;       //GPIO port to be used for TX pin
  uint16_t        txpin;        //Pin number to be used for TX pin
  uint8_t         txaf;         //Alternate function to be used for TX pin

  GPIO_TypeDef*   rxgpio;       //GPIO port to be used for RX pin
  uint16_t        rxpin;        //Pin number to be used for RX pin
  uint8_t         rxaf;         //Alternate function to be used for RX pin

} QAD_UART_InitStruct;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//--------
//QAD_UART
//
//Driver to access UART peripherals
//NOTE:
//The QAD_UART driver class is not intended to be used on its own, and is specifically used by the
//serial communications system QAS_Serial_Dev_UART class defined in QAS_Serial_Dev_UART.hpp
class QAD_UART {
private:

	//Deinitialization mode to be used by periphDeinit() method
  enum DeinitMode : uint8_t {
  	DeinitPartial = 0,        //Only to be used for partial deinitialization upon initialization failure in periphInit() method
  	DeinitFull                //Used for full driver deinitialization when driver is in a fully initialized state
  };

	QA_InitState       m_eInitState;     //Stores whether the driver is currently initialized. Member of QA_InitState enum defined in setup.hpp

	QAD_UART_Periph    m_eUART;          //Stores the particular UART peripheral being used by the driver
	uint32_t           m_uBaudrate;      //Stores the baudrate being used
	uint8_t            m_uIRQPriority;   //Stores the IRQ priority for the TX and RX interrupts

	GPIO_TypeDef*      m_pTXGPIO;        //GPIO port used by TX pin
	uint16_t           m_uTXPin;         //Pin number used by TX pin
	uint8_t            m_uTXAF;          //Alternate function used by TX pin

	GPIO_TypeDef*      m_pRXGPIO;        //GPIO port used by RX pin
	uint16_t           m_uRXPin;         //Pin number used by RX pin
	uint8_t            m_uRXAF;          //Alternate function used by RX pin

	IRQn_Type          m_eIRQ;           //The IRQ used by the UART peripheral being used (a member of IRQn_Type defined in stm32f411xe.h)
	UART_HandleTypeDef m_sHandle;        //Handle used by HAL functions to access UART peripheral (defined in stm32f4xx_hal_uart.h)

	QA_ActiveState     m_eTXState;       //Stores whether the transmit component of the peripheral is currently active. Member of QA_ActiveState enum defined in setup.hpp
	QA_ActiveState     m_eRXState;       //Stores whether the receive component of the peripheral is currently active. Member of QA_ActiveState enum defined in setup.hpp

public:

	  //--------------------------
	  //Constructors / Destructors

	QAD_UART() = delete;                     //Delete the default class constructor, as we need an initialization structure to be provided on class creation


	QAD_UART(QAD_UART_InitStruct& pInit) :   //The class constructor to be used, which has a reference to an initialization structure passed to it
		m_eInitState(QA_NotInitialized),
		m_eUART(pInit.uart),
		m_uBaudrate(pInit.baudrate),
		m_uIRQPriority(pInit.irqpriority),
		m_pTXGPIO(pInit.txgpio),
		m_uTXPin(pInit.txpin),
		m_uTXAF(pInit.txaf),
		m_pRXGPIO(pInit.rxgpio),
		m_uRXPin(pInit.rxpin),
		m_uRXAF(pInit.rxaf),
		m_eIRQ(USART1_IRQn),
		m_sHandle({0}),
		m_eTXState(QA_Inactive),
		m_eRXState(QA_Inactive) {}


	~QAD_UART() {                           //Destructor to make sure peripheral is made inactive and deinitialized upon class destruction

		//Stop transmit if currently active
    if (m_eTXState)
    	stopTX();

    //Stop receive if currently active
    if (m_eRXState)
    	stopRX();

    //Deinitialize peripheral if currently initialized
    if (m_eInitState)
    	deinit();
	}


	  //NOTE: See QAD_UART.cpp for details of the following functions

	  //----------------------
	  //Initialization Methods

	QA_Result init(void);
	void deinit(void);

	QA_InitState getState(void);
	UART_HandleTypeDef& getHandle(void);


	  //---------------
	  //Control Methods

	void startTX(void);
	void stopTX(void);
	QA_ActiveState getTXState(void);

	void startRX(void);
	void stopRX(void);
	QA_ActiveState getRXState(void);

	  //------------------
	  //Transceive Methods

	void dataTX(uint8_t uData);
	uint8_t dataRX(void);

private:

	  //----------------------
	  //Initialization Methods

	QA_Result periphInit(void);
  void periphDeinit(DeinitMode eDeinitMode);

};


//Prevent Recursive Inclusion
#endif /* __QAD_UART_HPP_ */
