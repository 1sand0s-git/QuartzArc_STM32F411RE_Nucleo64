/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Tools                                                         */
/*   Role: FIFO Buffer                                                     */
/*   Filename: QAT_FIFO.cpp                                                */
/*   Date: 22th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAT_FIFO.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //---------------------------
  //---------------------------
  //QAT_FIFOBuffer Data Methods

//QAT_FIFOBuffer::clear
//QAT_FIFOBuffer Data Method
//
//Used to clear pending data from the FIFO buffer
//This is done by setting both read and write indexes to zero
void QAT_FIFOBuffer::clear(void) {
	m_uReadIdx = 0;
	m_uWriteIdx = 0;
}


//QAT_FIFOBuffer::empty
//QAT_FIFOBuffer Data Method
//
//Used to check if FIFO buffer is empty, or if it has data pending
//Returns a member of QAT_FIFOState enum as defined in QAT_FIFO.hpp
QAT_FIFOState QAT_FIFOBuffer::empty(void) {
	return (m_uReadIdx == m_uWriteIdx) ? QAT_FIFOState_Empty : QAT_FIFOState_NotEmpty;
}


//QAT_FIFOBuffer::pending
//QAT_FIFOBuffer Data Method
//
//Used to return how many bytes of data are currently pending in the FIFO buffer
//Returns size in bytes of pending data
uint16_t QAT_FIFOBuffer::pending(void) {
	uint16_t uWriteIdx = m_uWriteIdx;
	if (uWriteIdx > m_uReadIdx) {
		return uWriteIdx-m_uReadIdx;
	}
	return m_uWriteIdx + (m_uSize - m_uReadIdx);
}


//QAT_FIFOBuffer::push
//QAT_FIFOBuffer Data Method
//
//Used to push a byte of data into the FIFO buffer
//uData - The byte to be pushed into the buffer
void QAT_FIFOBuffer::push(uint8_t uData) {
	m_pBuffer[m_uWriteIdx] = uData;
	if (m_uWriteIdx <= (m_uSize-1))
		m_uWriteIdx++; else
		m_uWriteIdx = 0;
}


//QAT_FIFOBuffer::pop
//QAT_FIFOBuffer Data Method
//
//Used to pull a byte of data from the FIFO buffer
//Returns the byte of data pulled from the buffer
uint8_t QAT_FIFOBuffer::pop(void) {
	if (!empty()) {
		uint8_t uData = m_pBuffer[m_uReadIdx];
		if (m_uReadIdx <= (m_uSize-1))
			m_uReadIdx++; else
			m_uReadIdx = 0;
		return uData;
	}
	return 0;
}
