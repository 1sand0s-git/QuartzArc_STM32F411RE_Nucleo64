/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Tools                                                         */
/*   Role: FIFO Buffer                                                     */
/*   Filename: QAT_FIFO.hpp                                                */
/*   Date: 22th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAT_FIFO_HPP_
#define __QAT_FIFO_HPP_

//Includes
#include "setup.hpp"

#include <memory>


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------------
//QAT_FIFOState
//
//Used to indicate if FIFO is empty or not
enum QAT_FIFOState : uint8_t {
	QAT_FIFOState_NotEmpty = 0, //FIFO is not empty and currently has data pending
	QAT_FIFOState_Empty         //FIFO is empty and no data is pending
};

	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//--------------
//QAT_FIFOBuffer
//
//Circular FIFO Buffer class used for temporary data storage for streaming data, such as
//within QAS_Serial_Dev_Base system class.
class QAT_FIFOBuffer {
private:

  std::unique_ptr<uint8_t[]> m_pBuffer;    //Pointer to dynamically allocated buffer. Buffer is allocated upon class creation
	uint16_t                   m_uSize;      //Size in bytes of the buffer

	uint16_t                   m_uReadIdx;   //Data read index
	uint16_t                   m_uWriteIdx;  //Data write index

public:

	//--------------------------
  //Constructors / Destructors

	QAT_FIFOBuffer() = delete;         //Delete default class constructor, as the buffer size needs to be supplied upon class creation

	QAT_FIFOBuffer(uint16_t uSize) :   //Constructor to be used, which has the buffer size (in bytes) passed to it
		m_pBuffer(std::make_unique<uint8_t[]>(uSize)),
		m_uSize(uSize),
		m_uReadIdx(0),
		m_uWriteIdx(0) {}


	//NOTE: See QAT_FIFO.cpp for details of the following methods

	//------------
	//Data Methods

	void clear(void);
	QAT_FIFOState empty(void);
  uint16_t pending(void);

  void push(uint8_t uData);
  uint8_t pop(void);

};


//Prevent Recursive Inclusion
#endif /* __QAT_FIFO_HPP_ */
