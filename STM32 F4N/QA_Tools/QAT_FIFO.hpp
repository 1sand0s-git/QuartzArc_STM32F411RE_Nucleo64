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

//--------------
//QAT_FIFOBuffer
class QAT_FIFOBuffer {
public:
	enum FIFOState : uint8_t {FS_NotEmpty = 0, FS_Empty};

private:

  std::unique_ptr<uint8_t[]> m_pBuffer;
	uint16_t                   m_uSize;

	uint16_t                   m_uReadIdx;
	uint16_t                   m_uWriteIdx;

public:

	  //Constructors / Destructors

	QAT_FIFOBuffer() = delete;
	QAT_FIFOBuffer(uint16_t uSize) :
		m_pBuffer(std::make_unique<uint8_t[]>(uSize)),
		m_uSize(uSize),
		m_uReadIdx(0),
		m_uWriteIdx(0) {}


	  //Data Methods

	void clear(void);
	FIFOState empty(void);
  uint16_t pending(void);

  void push(uint8_t uData);
  uint8_t pop(void);

};


//Prevent Recursive Inclusion
#endif /* __QAT_FIFO_HPP_ */
