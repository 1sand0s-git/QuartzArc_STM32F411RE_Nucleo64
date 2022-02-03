/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Drivers                                                       */
/*   Role: Flash Driver                                                    */
/*   Filename: QAD_Flash.hpp                                               */
/*   Date: 3rd February 2022                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_FLASH_HPP_
#define __QAD_FLASH_HPP_

//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
  //------------------------------------------

#define QAD_FLASH_SECTORCOUNT  8


//--------------------
//QAD_Flash_SectorData
typedef struct {

	uint32_t uSize;
	uint32_t uBase;

	void*    pData;

	bool     bLocked;

} QAD_Flash_SectorData;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//---------
//---------
//QAD_Flash
class QAD_Flash {
private:

	//-----------
	//Sector Data
	QAD_Flash_SectorData m_sSectors[QAD_FLASH_SECTORCOUNT];


	//------------
	//Constructors
	QAD_Flash();

public:

	//-----------------------------------------------
	//Delete copy constructor and assignment operator
	QAD_Flash(const QAD_Flash& other) = delete;
	QAD_Flash& operator=(const QAD_Flash& other) = delete;


	//----------------
	//Singleton Method

	static QAD_Flash& get(void) {
		static QAD_Flash instance;
		return instance;
	}


	//--------------
	//Sector Methods

	static uint32_t getSectorSize(uint8_t uSector) {
		if (uSector >= QAD_FLASH_SECTORCOUNT)
			return 0;

		return get().m_sSectors[uSector].uSize;
	}


	static uint32_t getSectorBaseAddr(uint8_t uSector) {
		if (uSector >= QAD_FLASH_SECTORCOUNT)
			return 0;

		return get().m_sSectors[uSector].uBase;
	}


	static void* getSectorPointer(uint8_t uSector) {
		if (uSector >= QAD_FLASH_SECTORCOUNT)
			return NULL;

		return get().m_sSectors[uSector].pData;
	}


	static bool getSectorLocked(uint8_t uSector) {
		if (uSector >= QAD_FLASH_SECTORCOUNT)
			return true;

		return get().m_sSectors[uSector].bLocked;
	}


	//------------
	//Data Methods

	static QA_Result eraseSector(uint8_t uSector) {
		return get().imp_eraseSector(uSector);
	}

	static QA_Result eraseAndWriteSector(uint8_t uSector, uint8_t* pData, uint32_t uSize) {
		return get().imp_eraseAndWriteSector(uSector, pData, uSize);
	}

	static QA_Result writeSector(uint8_t uSector, uint32_t uOffset, uint8_t* pData, uint32_t uSize) {
		return get().imp_writeSector(uSector, uOffset, pData, uSize);
	}


private:

	//------------
	//Data Methods

	QA_Result imp_eraseSector(uint8_t uSector);

	QA_Result imp_eraseAndWriteSector(uint8_t uSector, uint8_t* pData, uint32_t uSize);

	QA_Result imp_writeSector(uint8_t uSector, uint32_t uOffset, uint8_t* pData, uint32_t uSize);

};


//Prevent Recursive Inclusion
#endif /* __QAD_FLASH_HPP_ */













