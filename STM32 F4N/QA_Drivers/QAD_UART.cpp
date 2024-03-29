/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: UART Driver                                                     */
/*   Filename: QAD_UART.cpp                                                */
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


//Includes
#include "QAD_UART.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //-------------------------------
  //-------------------------------
	//QAD_UART Initialization Methods

//QAD_UART::init
//QAD_UART Initialization Method
//
//Used to initialize the UART driver
//Returns QA_OK if initialization successful, or an error if not successful (a member of QA_Result as defined in setup.hpp)
QA_Result QAD_UART::init(void) {

	//Check if selected UART peripheral is currently available
	if (QAD_UARTMgr::getState(m_eUART))
		return QA_Error_PeriphBusy;

	//Register UART peripheral as now being in use
  QAD_UARTMgr::registerUART(m_eUART);

  //Initialize UART peripheral
  QA_Result eRes = periphInit();

  //If initialization failed then deregister UART peripheral
  if (eRes)
  	QAD_UARTMgr::deregisterUART(m_eUART);

  //Return initialization result
  return eRes;
}


//QAD_UART::deinit
//QAD_UART Initialization Method
//
//Used to deinitialize the UART driver
void QAD_UART::deinit(void) {

	//Return if UART driver is not currently initialized
	if (!m_eInitState)
  	return;

	//Deinitialize UART driver
  periphDeinit(DeinitFull);

  //Deregister UART peripheral
  QAD_UARTMgr::deregisterUART(m_eUART);
}


//QAD_UART::getState
//QAD_UART Initialization Method
//
//Returns whether the peripheral is currently initialized (QA_Initialized, or QA_NotInitialized, as defined in QA_InitState enum in setup.hpp)
QA_InitState QAD_UART::getState(void) {
  return m_eInitState;
}


//QAD_UART::getHandle
//QAD_UART Initialization Method
//
//Retrieves the HAL UART peripheral handle
UART_HandleTypeDef& QAD_UART::getHandle(void) {
  return m_sHandle;
}


  //------------------------
	//QAD_UART Control Methods

//QAD_UART::startTX
//QAD_UART Control Method
//
//Used to start transmission of the UART peripheral
void QAD_UART::startTX(void) {

	//Enable Transmit Register Empty (TXE) interrupt
  __HAL_UART_ENABLE_IT(&m_sHandle, UART_IT_TXE);

  //Set TX State to active
  m_eTXState = QA_Active;
}


//QAD_UART::stopTX
//QAD_UART Control Method
//
//Used to stop transmission of the UART peripheral
void QAD_UART::stopTX(void) {

	//Disable Transmit Register Empty (TXE) interrupt
  __HAL_UART_DISABLE_IT(&m_sHandle, UART_IT_TXE);

  //Set TX State to inactive
  m_eTXState = QA_Inactive;
}


//QAD_UART::getTXState
//QAD_UART Control Method
//
//Used to retrieve whether the UART peripheral transmit is active
//Returns QA_Active if transmission currently active, or QA_Inactive if not currently active
QA_ActiveState QAD_UART::getTXState(void) {

	//Return current TX state
  return m_eTXState;
}


//QAD_UART::startRX
//QAD_UART Control Method
//
//Used to start receive of the UART peripheral
void QAD_UART::startRX(void) {

	//Enable RX Register Not-Empty (RXNE) interrupt
  __HAL_UART_ENABLE_IT(&m_sHandle, UART_IT_RXNE);

  //Set RX State to active
  m_eRXState = QA_Active;
}


//QAD_UART::stopRX
//QAD_UART Control Method
//
//Used to stop receive of the UART peripheral
void QAD_UART::stopRX(void) {

	//Disable RX Register Not-Empty (RXNE) interrupt
  __HAL_UART_DISABLE_IT(&m_sHandle, UART_IT_RXNE);

  //Set TX State to inactive
  m_eRXState = QA_Inactive;
}


//QAD_UART::getRXState
//QAD_UART Control Method
//
//Used to retrieve whether the UART peripheral receive is active
//Returns QA_Active if receive currently active, or QA_Inactive if not currently active
QA_ActiveState QAD_UART::getRXState(void) {

	//Return current RX State
  return m_eRXState;
}


  //--------------------------
  //--------------------------
  //QAD_UART Transceive Method

//QAD_UART::dataTX
//QAD_UART Transceive Method
//
//Used to transmit a single byte of data
//uData - the byte to be transmitted, which is placed into the UART data register (DR)
void QAD_UART::dataTX(uint8_t uData) {

	//Place uData into UART Data Register
  m_sHandle.Instance->DR = uData;
}


//QAD_UART::dataRX
//QAD_UART Transceive Method
//
//Used to receive a single byte of data
//Returns the data retrieved from the UART data register (DR)
uint8_t QAD_UART::dataRX(void) {

	//Read data from UART Data Register and return value
  return m_sHandle.Instance->DR;
}


  //---------------------------------------
  //---------------------------------------
  //QAD_UART Private Initialization Methods

//QAD_UART::periphInit
//QAD_UART Private Initialization Method
//
//Used to initialize the GPIOs, peripheral clock, and the peripheral itself, as well as setting the interrupt priority and enabling the interrupt
//In the case of a failed initialization, a partial deinitialization will be performed to make sure the peripheral, clock and GPIOs are all in the
//uninitialized state
//Returns QA_OK if successful, or QA_Fail if initialization fails
QA_Result QAD_UART::periphInit(void) {
	GPIO_InitTypeDef GPIO_Init = {0};

	//Init TX GPIO pin
	GPIO_Init.Pin       = m_uTXPin;                   //Set pin number
	GPIO_Init.Mode      = GPIO_MODE_AF_PP;            //Set TX Pin as alternate function in push/pull mode
	GPIO_Init.Pull      = GPIO_NOPULL;                //Disable pull-up and pull-down resistors
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;  //Set GPIO pin speed
	GPIO_Init.Alternate = m_uTXAF;                    //Set alternate function to suit required UART peripheral
	HAL_GPIO_Init(m_pTXGPIO, &GPIO_Init);

	//Init RX GPIO pin
	GPIO_Init.Pin       = m_uRXPin;                   //Set pin number
	GPIO_Init.Mode      = GPIO_MODE_AF_PP;            //Set RX Pin as alternate function in push/pull mode
	GPIO_Init.Pull      = GPIO_PULLUP;                //Enable pull-up resistor to prevent spurious receive triggering in cases where RX pin is not connected.
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;  //Set GPIO pin speed
	GPIO_Init.Alternate = m_uRXAF;                    //Set alternate function to suit required UART peripheral
	HAL_GPIO_Init(m_pRXGPIO, &GPIO_Init);


	//Enable UART Clock
	QAD_UARTMgr::enableClock(m_eUART);

	//Initialize UART Peripheral
	m_sHandle.Instance             = QAD_UARTMgr::getInstance(m_eUART); //Set instance for required UART peripheral
	m_sHandle.Init.BaudRate        = m_uBaudrate;                       //Set selected baudrate
	m_sHandle.Init.WordLength      = UART_WORDLENGTH_8B;                //Set world length to 8bits (1byte)
	m_sHandle.Init.StopBits        = UART_STOPBITS_1;                   //Set 1 stop bit
	m_sHandle.Init.Parity          = UART_PARITY_NONE;                  //Disable parity
	m_sHandle.Init.Mode            = UART_MODE_TX_RX;                   //Enable both transmit (TX) and receive (RX)
	m_sHandle.Init.HwFlowCtl       = UART_HWCONTROL_NONE;               //Disable hardware flow control (CTS/RTS)
	m_sHandle.Init.OverSampling    = UART_OVERSAMPLING_16;              //Enable 16x oversampling to provide high communication reliability
	if (HAL_UART_Init(&m_sHandle) != HAL_OK) {
		periphDeinit(DeinitPartial);
		return QA_Fail;
	}

	//Enable UART Peripheral
	__HAL_UART_ENABLE(&m_sHandle);

	//Set UART IRQ priority and enable IRQ
	HAL_NVIC_SetPriority(QAD_UARTMgr::getIRQ(m_eUART), m_uIRQPriority, 0x00);
	HAL_NVIC_EnableIRQ(QAD_UARTMgr::getIRQ(m_eUART));

	//Set driver states
	m_eInitState = QA_Initialized; //Set driver state as initialized
	m_eTXState   = QA_Inactive;    //Set TX state as inactive
	m_eRXState   = QA_Inactive;    //Set RX state as inactive

  //Return
  return QA_OK;
}


//QAD_UART::periphDeinit
//QAD_UART Private Initialization Method
//
//Used to deinitialize the GPIOs, peripheral clock, and the peripheral itself, as well as disabling the interrupt
//eDeinitMode - Set to DeinitPartial to perform a partial deinitialization (only to be used by periphInit() method
//              in a case where peripheral initialization has failed)
//            - Set to DeinitFull to perform a full deinitialization in a case where the driver is fully initialized
void QAD_UART::periphDeinit(QAD_UART::DeinitMode eDeinitMode) {

	//Check if full deinitialization is required
	if (eDeinitMode) {

		//Disable IRQs
		stopTX();                                          //Disable TX IRQ
		stopRX();                                          //Disable RX IRQ
		HAL_NVIC_DisableIRQ(QAD_UARTMgr::getIRQ(m_eUART)); //Disable overall UART IRQ

		//Disable UART Peripheral
		__HAL_UART_DISABLE(&m_sHandle);

		//Deinitialize UART Peripheral
		HAL_UART_DeInit(&m_sHandle);

	}

	//Disable UART Clock
	QAD_UARTMgr::disableClock(m_eUART);

	//Deinit TX & RX GPIO Pins
	HAL_GPIO_DeInit(m_pRXGPIO, m_uRXPin);
	HAL_GPIO_DeInit(m_pTXGPIO, m_uTXPin);

	//Set States
	m_eTXState   = QA_Inactive;       //Set transmit state as inactive
	m_eRXState   = QA_Inactive;       //Set receive state as inactive
	m_eInitState = QA_NotInitialized; //Set driver state as not initialized
}
