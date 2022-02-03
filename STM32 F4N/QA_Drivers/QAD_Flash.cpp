/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: Flash Driver                                                    */
/*   Filename: QAD_Flash.cpp                                               */
/*   Date: 3rd February 2022                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_Flash.hpp"


	//------------------------------------------
	//------------------------------------------
  //------------------------------------------

  //----------------------
	//----------------------
	//QAD_Flash Constructors

//QAD_Flash::QAD_Flash
//QAD_Flash Constructor
QAD_Flash::QAD_Flash() {

	//---------------------------
	//Setup Sector Base Addresses
	m_sSectors[0].uBase = 0x08000000;
	m_sSectors[1].uBase = 0x08004000;
	m_sSectors[2].uBase = 0x08008000;
	m_sSectors[3].uBase = 0x0800C000;
	m_sSectors[4].uBase = 0x08010000;
	m_sSectors[5].uBase = 0x08020000;
	m_sSectors[6].uBase = 0x08040000;
	m_sSectors[7].uBase = 0x08060000;

	//------------------
	//Setup Sector Sizes
	m_sSectors[0].uSize = 16384;
	m_sSectors[1].uSize = 16384;
	m_sSectors[2].uSize = 16384;
	m_sSectors[3].uSize = 16384;
	m_sSectors[4].uSize = 65536;
	m_sSectors[5].uSize = 131072;
	m_sSectors[6].uSize = 131072;
	m_sSectors[7].uSize = 131072;

	//---------------------
	//Setup Sector Pointers
	for (uint8_t i=0; i<QAD_FLASH_SECTORCOUNT; i++)
	  m_sSectors[i].pData = (void*)m_sSectors[i].uBase;

	//------------------------
	//Setup Sector Lock Values
	m_sSectors[0].bLocked = true;
	m_sSectors[1].bLocked = true;
	m_sSectors[2].bLocked = true;
	m_sSectors[3].bLocked = true;
	m_sSectors[4].bLocked = false;
	m_sSectors[5].bLocked = false;
	m_sSectors[6].bLocked = false;
	m_sSectors[7].bLocked = false;
}


	//----------------------
	//----------------------
	//QAD_Flash Data Methods

//QAD_Flash::imp_eraseSector
//QAD_Flash Data Method
QA_Result QAD_Flash::imp_eraseSector(uint8_t uSector) {

	//Check sector index
	if (uSector >= QAD_FLASH_SECTORCOUNT)
		return QA_Fail;

	//Check if sector is locked
	if (m_sSectors[uSector].bLocked)
		return QA_Fail;

	//Erase Sector
	if (HAL_FLASH_Unlock() != HAL_OK)
		return QA_Fail;
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);
	FLASH_Erase_Sector(uSector, FLASH_VOLTAGE_RANGE_3);
	HAL_FLASH_Lock();

	//Return
	return QA_OK;
}


//QAD_Flash::imp_eraseAndWriteSector
//QAD_Flash Data Method
QA_Result QAD_Flash::imp_eraseAndWriteSector(uint8_t uSector, uint8_t* pData, uint32_t uSize) {

	//Check sector index
	if (uSector >= QAD_FLASH_SECTORCOUNT)
		return QA_Fail;

	//Check if sector is locked
	if (m_sSectors[uSector].bLocked)
		return QA_Fail;

	//Erase Sector
	if (HAL_FLASH_Unlock() != HAL_OK)
		return QA_Fail;
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);
	FLASH_Erase_Sector(uSector, FLASH_VOLTAGE_RANGE_3);

	for (uint32_t i=0; i<uSize; i++)
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, m_sSectors[uSector].uBase + i, (uint8_t)pData[i]);

	HAL_FLASH_Lock();

	//Return
	return QA_OK;
}


//QAD_Flash::imp_writeSector
//QAD_Flash Data Method
QA_Result QAD_Flash::imp_writeSector(uint8_t uSector, uint32_t uOffset, uint8_t* pData, uint32_t uSize) {

	//Check sector index
	if (uSector >= QAD_FLASH_SECTORCOUNT)
		return QA_Fail;

	//Check if sector is locked
	if (m_sSectors[uSector].bLocked)
		return QA_Fail;

	//Check write will not exceed sector size
	if ((uOffset + uSize) >= m_sSectors[uSector].uSize)
		return QA_Fail;

	//Erase Sector
	if (HAL_FLASH_Unlock() != HAL_OK)
		return QA_Fail;
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);

	uint32_t uBase = m_sSectors[uSector].uBase + uOffset;
	for (uint32_t i=0; i<uSize; i++)
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, uBase + i, (uint8_t)pData[i]);

	HAL_FLASH_Lock();

	//Return
	return QA_OK;
}











