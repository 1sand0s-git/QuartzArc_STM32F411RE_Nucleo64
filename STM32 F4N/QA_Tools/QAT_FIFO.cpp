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
  //QAT_FIFOBuffer Data Methods

//QAT_FIFOBuffer::clear
//QAT_FIFOBuffer Data Method
void QAT_FIFOBuffer::clear(void) {
	m_uReadIdx = 0;
	m_uWriteIdx = 0;
}


//QAT_FIFOBuffer::empty
//QAT_FIFOBuffer Data Method
QAT_FIFOBuffer::FIFOState QAT_FIFOBuffer::empty(void) {
	return (m_uReadIdx == m_uWriteIdx) ? FS_Empty : FS_NotEmpty;
}


//QAT_FIFOBuffer::pending
//QAT_FIFOBuffer Data Method
uint16_t QAT_FIFOBuffer::pending(void) {
	uint16_t uWriteIdx = m_uWriteIdx;
	if (uWriteIdx > m_uReadIdx) {
		return uWriteIdx-m_uReadIdx;
	}
	return m_uWriteIdx + (m_uSize - m_uReadIdx);
}


//QAT_FIFOBuffer::push
//QAT_FIFOBuffer Data Method
void QAT_FIFOBuffer::push(uint8_t uData) {
	m_pBuffer[m_uWriteIdx] = uData;
	if (m_uWriteIdx <= (m_uSize-1))
		m_uWriteIdx++; else
		m_uWriteIdx = 0;
}


//QAT_FIFOBuffer::pop
//QAT_FIFOBuffer Data Method
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
