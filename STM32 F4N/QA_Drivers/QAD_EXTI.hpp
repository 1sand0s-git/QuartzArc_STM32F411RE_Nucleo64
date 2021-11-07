/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: EXTI Driver                                                     */
/*   Filename: QAD_EXTI.hpp                                                */
/*   Date: 17th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_EXTI_HPP_
#define __QAD_EXTI_HPP_

//Includes
#include "setup.hpp"

#include "QAD_GPIO.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-----------------
//QAD_EXTI_EdgeType
//
//Ised with the QAD_EXTI driver class to deternmine if interrupt is to be triggered on rising edge, falling edge, or both edges.
enum QAD_EXTI_EdgeType : uint8_t {
	QAD_EXTI_EdgeType_Rising = 0,     //Interrupt to trigger on rising edge
	QAD_EXTI_EdgeType_Falling,        //Interrupt to trigger on falling edge
	QAD_EXTI_EdgeType_Both            //Interrupt to trigger on both rising and falling edges
};


//--------
//QAD_EXTI
//
//Driver to allow use of a GPIO pin to trigger external interrupts.
//Inherits from QAD_GPIO_Input driver class to allow driver to dynamically switch between being used as a standard GPIO input pin,
//or to be used to trigger external interrupt.
class QAD_EXTI : public QAD_GPIO_Input {
private:

	QA_ActiveState    m_eEXTIState;  //Stores whether the GPIO pin is in standard input mode (QA_Inactive),
	                                 //or as an external interrupt (QA_Active). Uses QA_ActiveState enum defined in setup.hpp

	QAD_EXTI_EdgeType m_eEdgeType;   //Stores whether the external interrupt triggeres on rising edge, falling edge, or both edges.
	                                 //A member of the QAD_EXTI_EdgeType enum

	IRQn_Type         m_eIRQ;        //Stores the IRQ handler to be triggered. A member of IRQn_Type as defined in stm32f411xe.h

  QAD_IRQHandler_CallbackFunction m_pHandlerFunction;  //Pointer to the interrupt handler function to be called when interrupt is triggered
                                                       //Callback function pointer as defined in setup.hpp

  QAD_IRQHandler_CallbackClass*   m_pHandlerClass;     //Pointer to the interrupt handler class to be called when interrupt is triggered
                                                       //Callback class as defined in setup.hpp

public:

  //--------------------------
  //Constructors / Destructors

  //Default constructor is deleted, as initialization details need to be provided for the driver class to initialize
  QAD_EXTI() = delete;

  //NOTE: See QAD_EXTI.cpp for details of the following functions

  QAD_EXTI(GPIO_TypeDef* pGPIO, uint16_t uPin);
  QAD_EXTI(GPIO_TypeDef* pGPIO, uint16_t uPin, QAD_GPIO_PullMode ePull, QAD_EXTI_EdgeType eEdgeType);
  ~QAD_EXTI();


  //--------------
  //Handler Method

  void handler(void);


  //---------------
  //Control Methods

  void setHandlerFunction(QAD_IRQHandler_CallbackFunction pHandler);
  void setHandlerClass(QAD_IRQHandler_CallbackClass* pHandler);

  void enable(void);
  void disable(void);

  void setPullMode(QAD_GPIO_PullMode ePull) override;

};


//Prevent Recursive Inclusion
#endif /* __QAD_EXTI_HPP_ */








