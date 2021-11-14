/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: GPIO Driver                                                     */
/*   Filename: QAD_GPIO.hpp                                                */
/*   Date: 12th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_GPIO_HPP_
#define __QAD_GPIO_HPP_

//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
  //------------------------------------------


//-----------------
//QAD_GPIO_PinState
//
//Used to select/determin the current state of a GPIO pin
enum QAD_GPIO_PinState : uint8_t {
	QAD_GPIO_PinState_Off = 0,
	QAD_GPIO_PinState_On
};


//-------------------
//QAD_GPIO_OutputMode
//
//Used with the QAD_GPIO_Output driver class to select if a pin is to be setup in Push Pull or Open Drain mode
enum QAD_GPIO_OutputMode : uint8_t {
	QAD_GPIO_OutputMode_PushPull = 0,   //Output pin to be used in Push/Pull mode
	QAD_GPIO_OutputMode_OpenDrain       //Output pin to be used in Open Drain mode
};


//-----------------
//QAD_GPIO_PullMode
//
//Used with both QAD_GPIO_Output and QAD_GPIO_Input driver classes to select if a pin is to use the inbuilt Pull Up, Pull Down resistors.
enum QAD_GPIO_PullMode : uint32_t {
	QAD_GPIO_PullMode_NoPull = GPIO_NOPULL,  //Neither pull-up or pull-down resistor to be used
	QAD_GPIO_PullMode_Up     = GPIO_PULLUP,  //Pull-up resistor to be used
	QAD_GPIO_PullMode_Down   = GPIO_PULLDOWN //Pull-down resistor to be used
};


//QAD_GPIO_Speed
//
//Used with both QAD_GPIO_Output and QAD_GPIO_Input driver classes to select the speed/frequency of the GPIO pin
enum QAD_GPIO_Speed : uint32_t {
	QAD_GPIO_Speed_Low      = GPIO_SPEED_FREQ_LOW,       //GPIO pin to be used in low speed mode       (2MHz)
	QAD_GPIO_Speed_Medium   = GPIO_SPEED_FREQ_MEDIUM,    //GPIO pin to be used in medium speed mode    (12.5MHz to 50MHz)
	QAD_GPIO_Speed_High     = GPIO_SPEED_FREQ_HIGH,      //GPIO pin to be used in high speed mode      (25MHz to 100MHz)
	QAD_GPIO_Speed_VeryHigh = GPIO_SPEED_FREQ_VERY_HIGH  //GPIO pin to be used in very high speed mode (50MHz to 200MHz)
};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//---------------
//QAD_GPIO_Output
//
//Driver to allow use of a GPIO pin in output mode (when pin is not connected to a specific internal peripheral)
class QAD_GPIO_Output {
private:

	GPIO_TypeDef*        m_pGPIO;       //Stores the GPIO port of the current output pin. A member of GPIO_TypeDef as defined in stm32f411xe.h
	uint16_t             m_uPin;        //Stores the pin number of the current output pin. A member of GPIO_pins_define as defined in stm32f4xx_hal_gpio.h

	QAD_GPIO_OutputMode  m_eOutputMode; //Stores if the pin is in Push Pull or Open Drain mode. A member of the QAD_GPIO_OutputMode enum defined above
  QAD_GPIO_PullMode    m_ePullMode;   //Stores if either Pull Up or Pull Down resistor is to be used. A member of the QAD_GPIO_PullMode enum defined above
  QAD_GPIO_Speed       m_eSpeed;      //Stores the speed of the GPIO pin. A member of the QAD_GPIO_Speed enum defined above

	QAD_GPIO_PinState    m_eState;    //Stores whether the output pin is currently turned on or turned off. A member of the QAD_GPIO_PinState enum defined above

public:

	//--------------------------
	//Constructors / Destructors

	//Default constructor is deleted, as initialization details need to be provided for the driver class to initialize
	QAD_GPIO_Output() = delete;

	//NOTE: See QAD_GPIO.cpp for details of the following functions

	QAD_GPIO_Output(GPIO_TypeDef* pGPIO, uint16_t uPin);
	QAD_GPIO_Output(GPIO_TypeDef* pGPIO, uint16_t uPin, QAD_GPIO_OutputMode eMode, QAD_GPIO_PullMode ePull, QAD_GPIO_Speed eSpeed);
	~QAD_GPIO_Output();


	//---------------
	//Control Methods

	void on(void);
	void off(void);
	void toggle(void);
	QAD_GPIO_PinState getState(void);

	void setOutputMode(QAD_GPIO_OutputMode eMode);
	QAD_GPIO_OutputMode getOutputMode(void);

	void setPullMode(QAD_GPIO_PullMode eMode);
	QAD_GPIO_PullMode getPullMode(void);

	void setSpeed(QAD_GPIO_Speed eSpeed);
	QAD_GPIO_Speed getSpeed(void);


private:

	//----------------------
	//Initialization Methods

	void periphInit(void);
	void periphDeinit(void);

};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//--------------
//QAD_GPIO_Input
//
//Driver to allow use of a GPIO pin in input mode (when pin is not connected to a specific internal peripheral)
class QAD_GPIO_Input {
protected:

	GPIO_TypeDef*     m_pGPIO;      //Stores the GPIO port of the current input pin. A member of GPIO_TypeDef as defined in stm32f411xe.h
	uint16_t          m_uPin;       //Stores the pin number of the current input pin. A member of GPIO_pins_define as defined in stm32f4xx_hal_gpio.h

	QAD_GPIO_PullMode m_ePullMode;  //Stores if either Pull Up or Pull Down resistor is to be used. A member of the QAD_GPIO_PullMode enum defined above

public:

	//-------------------------
	//Contructors / Destructors

	//Default constructor is deleted, as initialization details need to be provided for the driver class to initialize
	QAD_GPIO_Input() = delete;

	//NOTE: See QAD_GPIO.cpp for details of the following functions

	QAD_GPIO_Input(GPIO_TypeDef* pGPIO, uint16_t uPin);
	QAD_GPIO_Input(GPIO_TypeDef* pGPIO, uint16_t uPin, QAD_GPIO_PullMode ePull);
	~QAD_GPIO_Input();

	//---------------
	//Control Methods

	QAD_GPIO_PinState getState(void);

	virtual void setPullMode(QAD_GPIO_PullMode ePull);
	QAD_GPIO_PullMode getPullMode(void);

protected:

	//----------------------
	//Initialization Methods

	void periphInit(void);
	void periphDeinit(void);

};


//Prevent Recursive Inclusion
#endif /* __QAD_GPIO_HPP */
