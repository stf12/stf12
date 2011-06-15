/**
 * @file diskio2flash_a.c
 * @ingroup SERVICE_IF
 *
 * @version 1.0.0
 * @date May 9, 2011
 * @author IMS Systems Lab - ART Team
 * @note Module: FAT IF
 *
 * @brief It is part of <b>FAT IF</b> module. FatFS driver for the external NOR Flash chip.
 *
 * Define the disk I/O Interface as specified by FatFS. It maps the FatFS low level API on the
 * external NOR FLASH connected to the STM32 using the FSMC peripheral.
 * \image html FatFS.png "FatFS Architecture" width=245px
 *
 * This driver uses a block of 128KB size (equal to the size of one block of FLASH) of External SRAM, yo operate
 * the FLASH.
 * \image html FatFS_LL_implementation.png "FatFS Low Level implementation" width=277px
 */

/*
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 */

#include "diskio_ex.h"
#include "NOR_flash.h"
#include "SRAM_ISSI.h"
#include "syserror.h"
#include <assert.h>

#define FLASH_DEFAULT_TIMEOUT 0x00A00000

#define IS_HARDWARE_INITIALIZED() s_nHardwareInit

/**
 * Specifies the FAT objects.
 *
 * \sa <a href="http://elm-chan.org/fsw/ff/en/sfatfs.html">online documentation</a>.
 */
static FATFS s_xVolumes[FAT_DRIVE_COUNT];

/**
 * Specifies the status of the hardware used by the FS subsystem (FSMC, NOR FLASH, SRAM nad relate GPIO).
 * This variable is set to zero if the hardware is not ready and the FAT can not operate on it.
 * This variable is set to not zero otherwise.
 */
static uint32_t s_nHardwareInit = 0;

/**
 * Specifies the physical organization of the FLASH chip. Each chip is composed by
 * 128 block of 128KB size. The block is
 */
typedef enum FlashConfig_t {
	FLASH_BLOCK_COUNT = 128,		//!< Numbers of block.
	FLASH_BLOCK_SIZE_B = 0x20000 	//!< Size in byte of a block (128KB).
}FlashConfig;

/**
 * Specifies the logical configuration of the drive.
 */
typedef enum LogicDiskConfig_t {
	SECTOR_SIZE = 1024,		///< 1KB
	SECTOR_COUNT = 16384,	///< Total Number of Sector = FLASH_BLOCK_COUNT * BLOCK_SIZE
	BLOCK_SIZE = 128,		///< Number of Sector per block = FLASH_BLOCK_SIZE_B / SECTOR_SIZE = 128KB / 1KB
	BLOCK_SIZE_EXPONENT = 7	///< 2^BLOCK_SIZE_EXPONENT = BLOCK_SIZE
}LogicDiskConfig;


/*
 * Forward declaration.
 */
static void LowLevelEraseSector(DWORD nFromSector, DWORD nToSector);
static void CopyFlashToSram(uint32_t nFlashStartAddr, uint32_t nFlashEndAddr, uint32_t nSramStartAddr);
static void CopySramToFlash(uint32_t nFlashStartAddr, uint32_t nFlashEndAddr, uint32_t nSramStartAddr);

/**
 * The function never fails if Drive is a valid parameter.
 *
 * @param Drive Specifies the logical drive to initialize. Valid value is in the range [0, FAT_DRIVE_COUNT[.
 *
 * \sa <a href="http://elm-chan.org/fsw/ff/en/dinit.html">online documentation</a>.
 */
DSTATUS disk_initialize (BYTE Drive) {
	assert(Drive < FAT_DRIVE_COUNT); // check if Drive is valid.

	if (Drive >= FAT_DRIVE_COUNT) {
		SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_TASK_INVALID_PARAM_ERROR_CODE);
	}

	if (!IS_HARDWARE_INITIALIZED() && (Drive < FAT_DRIVE_COUNT)) {
		/* Enable the FSMC Clock */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

		/* Configure FSMC Bank1 NOR/SRAM2 */
		NorInit();

		/* Configure FSMC Bank1 NOR/SRAM3 */
		SramInit();

		// Change the internal state to 1
		s_nHardwareInit = 1;
	}

	return IS_HARDWARE_INITIALIZED() ? 0 : STA_NOINIT | STA_NODISK;
}

/**
 * \sa <a href="http://elm-chan.org/fsw/ff/en/dinit.html">online documentation</a>.
 */
DSTATUS disk_status (BYTE Drive) {
	assert(Drive < FAT_DRIVE_COUNT); // check if Drive is valid.

	if (Drive >= FAT_DRIVE_COUNT) {
		SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_TASK_INVALID_PARAM_ERROR_CODE);
	}

	return IS_HARDWARE_INITIALIZED() ? 0 : STA_NOINIT;
}

/**
 * \sa <a href="http://elm-chan.org/fsw/ff/en/dread.html">online documentation</a>.
 */
DRESULT disk_read (BYTE Drive, BYTE *Buffer, DWORD SectorNumber, BYTE SectorCount) {
	assert(Drive < FAT_DRIVE_COUNT); // check if Drive is valid.
	assert(SectorNumber < SECTOR_COUNT); // check if the first sector is valid.
	assert(SectorNumber + SectorCount - 1 < SECTOR_COUNT); // check if the last sector is valid.

	if ( (Drive >= FAT_DRIVE_COUNT) ||
		 (SectorNumber >= SECTOR_COUNT) ||
		 (SectorNumber + SectorCount > SECTOR_COUNT))
	{
		SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_TASK_INVALID_PARAM_ERROR_CODE);
		return RES_PARERR;
	}

	NorReadBuffer(BANK1_NOR2_ADDR, (uint16_t*)Buffer, SectorNumber * SECTOR_SIZE, SectorCount * SECTOR_SIZE / 2);

	return RES_OK;
}

#if	_READONLY == 0
/**
 * \sa <a href="http://elm-chan.org/fsw/ff/en/dwrite.html">online documentation</a>.
 */
DRESULT disk_write (BYTE Drive, const BYTE* Buffer, DWORD SectorNumber, BYTE SectorCount) {
	assert(Drive < FAT_DRIVE_COUNT); // check if Drive is valid.
	assert(SectorNumber < SECTOR_COUNT); // check if the first sector is valid.
	assert(SectorNumber + SectorCount - 1 < SECTOR_COUNT); // check if the last sector is valid.

	if ( (Drive >= FAT_DRIVE_COUNT) ||
		 (SectorNumber >= SECTOR_COUNT) ||
		 (SectorNumber + SectorCount > SECTOR_COUNT))
	{
		SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_TASK_INVALID_PARAM_ERROR_CODE);
		return RES_PARERR;
	}

	uint32_t nFromSector = SectorNumber;
	uint32_t nToSector = nFromSector + SectorCount;

	uint32_t nBlock = nFromSector >> BLOCK_SIZE_EXPONENT; //nStartSector / BLOCK_SIZE;
	uint32_t nLastBlock = nToSector >> BLOCK_SIZE_EXPONENT;

	uint16_t *pData = (uint16_t*)Buffer;

	uint32_t nSDAddr = nFromSector * SECTOR_SIZE;
	uint32_t nEDAddr = nSDAddr + (SectorCount * SECTOR_SIZE);

	uint32_t nSBAddr, nEBAddr, nSize;

	for (; nBlock<=nLastBlock; ++nBlock) {
		uint32_t nSramCurrentAddr = 0;
		nSBAddr = nBlock * FLASH_BLOCK_SIZE_B;
		nEBAddr = nSBAddr + FLASH_BLOCK_SIZE_B;

		// Copy data to SRAM
		if (nSBAddr < nSDAddr) {
			CopyFlashToSram(nSBAddr, nSDAddr, nSramCurrentAddr);
			nSramCurrentAddr += nSDAddr - nSBAddr;
		}

		if (nEBAddr <= nEDAddr) {
			nSize = nEBAddr - nSDAddr;
			SramWriteBuffer(pData, nSramCurrentAddr, nSize >> 1);
			nSramCurrentAddr += nSize;
			pData += nSize >> 1;
			nSDAddr += nSize;
		}
		else {
			nSize = nEDAddr - nSDAddr;
			SramWriteBuffer(pData, nSramCurrentAddr, nSize >> 1);
			nSramCurrentAddr += nSize;
			pData += nSize >> 1;
			CopyFlashToSram(nEDAddr, nEBAddr, nSramCurrentAddr);
		}

		// Erase block
		NorEraseBlock(BANK1_NOR2_ADDR, nBlock * FLASH_BLOCK_SIZE_B);

		// Copy SRAM back to flash
		CopySramToFlash(nSBAddr, nEBAddr, 0);
	}
	return RES_OK;
}
#endif

/**
 * \sa <a href="http://elm-chan.org/fsw/ff/en/dioctl.html">online documentation</a>.
 */
DRESULT disk_ioctl (BYTE Drive, BYTE Command, void *Buffer) {
	assert(Drive < FAT_DRIVE_COUNT); // check if Drive is valid.

	if (Drive >= FAT_DRIVE_COUNT) {
		SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_TASK_INVALID_PARAM_ERROR_CODE);
		return RES_PARERR;
	}

	DRESULT res = RES_OK;

	switch (Command) {
			case CTRL_SYNC:
				if ( SYS_IS_ERROR(NorGetStatus(BANK1_NOR2_ADDR, FLASH_DEFAULT_TIMEOUT)) )
					res = RES_ERROR;
				break;

			case GET_SECTOR_SIZE:
				*((WORD*)Buffer) = (WORD)SECTOR_SIZE;
				break;

			case GET_SECTOR_COUNT:
				*((DWORD*)Buffer) = (DWORD)SECTOR_COUNT;
				break;

			case GET_BLOCK_SIZE:
				*((DWORD*)Buffer) = BLOCK_SIZE;
				break;

			case CTRL_ERASE_SECTOR:
				LowLevelEraseSector(((DWORD*)Buffer)[0], ((DWORD*)Buffer)[1]);
				break;

			case CTRL_ERASE_CHIP:
				disk_initialize(Drive);
				NorEraseChip(BANK1_NOR2_ADDR);
				break;

			default:
				res = RES_PARERR;
	}

	return res;
}

/**
 * \sa <a href="http://elm-chan.org/fsw/ff/en/fattime.html">online documentation</a>.
 */
DWORD get_fattime(void) {
	return 0x304C0000; //February 12 2004 00:00:00
}

FATFS *FatGetDriveHandler(BYTE nDrive) {
	assert(nDrive < FAT_DRIVE_COUNT); // check if Drive is valid.

	if (nDrive < FAT_DRIVE_COUNT) {
		return &s_xVolumes[nDrive];
	}
	else {
		SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_TASK_INVALID_PARAM_ERROR_CODE);
		return NULL;
	}
}

/**
 * Copy a data block from External FLASH to External RAM. This is a private method. The input parameters are not
 * validated because the check is made by the caller.
 *
 * @param nFlashStartAddr [IN] specifies the physical start address of the FLASH memory to read.
 * @param nFlashEndAddr [IN] specifies the physical end address of the FLASH memory to read.
 * @param nSramStartAddr [IN] specifies the physical start address of SRAM to write.
 */
static void CopyFlashToSram(uint32_t nFlashStartAddr, uint32_t nFlashEndAddr, uint32_t nSramStartAddr) {
	for (uint32_t i=nFlashStartAddr, j=nSramStartAddr; i<nFlashEndAddr; i=i+2, j=j+2)
		SramWriteHalfWord(NorReadHalfWord(BANK1_NOR2_ADDR, i), j); // Copy data 16 bits at time.
}

/**
 * Copy a data block from External RAM to External FLASH. This is a private method. The input parameters are not
 * validated because the check is made by the caller.
 *
 * @param nFlashStartAddr [IN] specifies the physical start address of the FLASH memory to write.
 * @param nFlashEndAddr [IN] specifies the physical end address of the FLASH memory to write.
 * @param nSramStartAddr [IN] specifies the physical start address of SRAM to read.
 */
static void CopySramToFlash(uint32_t nFlashStartAddr, uint32_t nFlashEndAddr, uint32_t nSramStartAddr) {
	for (uint32_t i=nFlashStartAddr, j=nSramStartAddr; i<nFlashEndAddr; i=i+2, j=j+2)
		NorWriteHalfWord(BANK1_NOR2_ADDR, i, SramReadHalfWord(j)); // Copy data 16 bits at time.
}

/**
 * Erase contiguous sector of the External FLASH from nFromSector to nToSector. The sector are specified in logical
 * TODO: STF - add algorithm documentation.
 *
 * @param nFromSector [IN] specifies the first sector number in LBA.
 * @param nToSector [IN] specifies the last sector number in LBA.
 *
 * @todo: Optimization hint: when all sectors in a block must be erased, I can erase the block using only
 * one NorEraseBlock call.
 */
static void LowLevelEraseSector(DWORD nFromSector, DWORD nToSector) {
	uint32_t nBlock = nFromSector >> BLOCK_SIZE_EXPONENT; //nStartSector / BLOCK_SIZE;
	uint32_t nLastBlock = nToSector >> BLOCK_SIZE_EXPONENT;

	uint32_t nS1Addr, nE1Addr, nS2Addr, nE2Addr;

	for (; nBlock<=nLastBlock; ++nBlock) {
		nS1Addr = nBlock * FLASH_BLOCK_SIZE_B;
		nE1Addr = nFromSector * SECTOR_SIZE;
		CopyFlashToSram(nS1Addr, nE1Addr, 0);

		if (nBlock == nLastBlock) {
			nS2Addr = (nToSector + 1) * SECTOR_SIZE;
			nE2Addr = (nLastBlock + 1)* FLASH_BLOCK_SIZE_B;
			CopyFlashToSram(nS2Addr, nE2Addr, nE1Addr-nS1Addr);
		}

		NorEraseBlock(BANK1_NOR2_ADDR, nBlock * FLASH_BLOCK_SIZE_B);
		CopySramToFlash(nS1Addr, nE1Addr, 0);
		if (nBlock == nLastBlock) {
			CopySramToFlash(nS2Addr, nE2Addr, nE1Addr-nS1Addr);
		}

		// Update the start sector for the next block.
		nFromSector = (nBlock + 1) * BLOCK_SIZE;
	}
}


