/**
 * @file NOR_flash.h
 * @ingroup LOW_LEVEL_API
 *
 * @version V4.3.0
 * @date October 15, 2010
 * @author MCD Application Team
 * @note Module: Ext Mem Lib
 *
 * @brief It is part of <b>Ext Mem Lib</b> module.
 * This file provides a set of functions needed to drive the M29W128FL,
 * M29W128GL and S29GL128P NOR memories mounted on STM3210E-EVAL board.
 *
 *
 */

/*
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 */

#ifndef _NOR_FLASH_H
#define _NOR_FLASH_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "syserror.h"

typedef struct NorId_t
{
  uint16_t nManufacturerCode;
  uint16_t nDeviceCode1;
  uint16_t nDeviceCode2;
  uint16_t nDeviceCode3;
}NorId;

/**
  * FSMC Bank 1 NOR/SRAM1
  */
#define BANK1_NOR1_ADDR       ((uint32_t)0x60000000)

/**
  * FSMC Bank 1 NOR/SRAM2
  */
#define BANK1_NOR2_ADDR       ((uint32_t)0x64000000)

/**
  * M29W128GL Block size: 128 KBytes
  * (total 1024 blocks, 0x400)
  */
#define FLASH_BLOCK_SIZE				(0x20000) //NOR_FLASH

/**
  * M29W128GL Page size: 64 Bytes
  * Used in NorWriteToBufferAndProgram Function
  */
#define FLASH_PAGE_SIZE				(0x40) //TODO: FS: Esportare FLASH_PAGE_SIZE oppure no?
#define FLASH_PAGE_SIZE_MASK		(FLASH_PAGE_SIZE - 1)

/**
  * Configures the FSMC and GPIOs to interface with the NOR memory.
  * This function must be called before any write/read operation on the NOR.
  */
void NorInit(void);

/**
  * Checks whether or not the NOR FSMC bank is enabled.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  *
  * @return zero if the FSMC is not enabled, non zero otherwise.
  */
uint8_t NorIsFsmcEnabled(uint32_t nBank);

/**
  * Reads NOR memory's Manufacturer and Device Code.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  * @param pxNorId [OUT] pointer to a NorId structure which will hold the Manufacturer and Device Code.
  */
void NorReadID(uint32_t nBankAddr, NorId* pxNorId);

/**
  * Erases the specified Nor memory block (FLASH_BLOCK_SIZE bytes).
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  * @param nBlockAddr [IN] address of the block to erase.
  *
  * @retval The returned value can be one of GTS_NO_ERROR, GTS_NOR_FLASH_ONGOING_ERROR_CODE
  * or GTS_NOR_FLASH_TIMEOUT_ERROR_CODE
  */
gts_error_t NorEraseBlock(uint32_t nBankAddr, uint32_t nBlockAddr);

/**
  * Erases the entire chip.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  *
  * @retval The returned value can be one of GTS_NO_ERROR, GTS_NOR_FLASH_ONGOING_ERROR_CODE
  * or GTS_NOR_FLASH_TIMEOUT_ERROR_CODE
  */
gts_error_t NorEraseChip(uint32_t nBankAddr);


/**
  * Writes a half-word (16 bit) to the NOR memory.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  * @param nWriteAddr [IN] NOR memory internal address to write to.
  * @param nData [IN] data to write.
  *
  * @retval The returned value can be one of GTS_NO_ERROR, GTS_NOR_FLASH_ONGOING_ERROR_CODE
  * or GTS_NOR_FLASH_TIMEOUT_ERROR_CODE
  */
gts_error_t NorWriteHalfWord(uint32_t nBankAddr, uint32_t nWriteAddr, uint16_t nData);

/**
  * Writes a half-word (16bit) buffer to the FSMC NOR memory.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  * @param pnBuffer [IN] pointer to (uint16_t) buffer.
  * @param nWriteAddr [IN] NOR memory internal address from which the data will be written.
  * @param nNumHalfwordToWrite [IN] number of Half words to write.
  *
  * @retval The returned value can be: GTS_NO_ERROR, GTS_NOR_FLASH_ONGOING_ERROR_CODE
  * or GTS_NOR_FLASH_TIMEOUT_ERROR_CODE
  */
gts_error_t NorWriteBuffer(uint32_t nBankAddr, uint16_t* pnBuffer, uint32_t nWriteAddr, uint32_t nNumHalfwordToWrite);


/**
  * Writes a half-word (16bit) buffer to the FSMC NOR memory.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  * @param pnBuffer [IN] pointer to (uint16_t) buffer.
  * @param nWriteAddr [IN] NOR memory internal address from which the data will be written.
  * @param nNumHalfwordToWrite [IN] number of Half words to write. MUST BE <=FLASH_PAGE_SIZE.
  * @param pnHalfWordActuallyWritten [OUT] the actual number of halfword written.
  *
  * @retval The returned value can be one of GTS_NO_ERROR, GTS_NOR_FLASH_ONGOING_ERROR_CODE or GTS_NOR_FLASH_TIMEOUT_ERROR_CODE
  */
gts_error_t NorFastWriteBuffer(uint32_t nBankAddr, uint16_t* pnBuffer, uint32_t nWriteAddr, uint32_t nNumHalfwordToWrite, uint32_t* pnHalfWordActuallyWritten);


/**
  * Reads a half-word (16bit) from the NOR memory.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  * @param nReadAddr [IN] NOR memory internal address to read from.
  *
  * @retval Half-word read from the NOR memory.
  */
uint16_t NorReadHalfWord(uint32_t nBankAddr, uint32_t nReadAddr);

/**
  * Reads a block of data from the FSMC NOR memory.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  * @param pnBuffer [IN] pointer to the buffer that receives the data read from the NOR memory.
  * @param nReadAddr [IN] NOR memory internal address to read from.
  * @param nNumHalfwordToRead [IN] number of Half word to read.
  */
void NorReadBuffer(uint32_t nBankAddr, uint16_t* pnBuffer, uint32_t nReadAddr, uint32_t nNumHalfwordToRead);

/**
  * Returns the NOR memory to Read mode.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  *
  * @retval GTS_NO_ERROR
  */
gts_error_t NorReturnToReadMode(uint32_t nBankAddr);

/**
  * Returns the NOR memory to Read mode and resets the errors in the NOR memory Status Register.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  *
  * @retval GTS_NO_ERROR.
  */
gts_error_t NorReset(uint32_t nBankAddr);

/**
  * Returns the NOR operation status.
  *
  * @param  nBank [IN] specifies the itial address of the NOR_FLASH bank.
  * Valid vale are BANK1_NOR1_ADDR, BANK1_NOR2_ADDR.
  * @param nTimeout [IN] NOR progamming Timeout
  * @retval The returned value can be one of GTS_NO_ERROR, GTS_NOR_FLASH_ONGOING_ERROR_CODE or
  * GTS_NOR_FLASH_TIMEOUT_ERROR_CODE.
  */
gts_error_t NorGetStatus(uint32_t nBankAddr, uint32_t nTimeout);

#ifdef __cplusplus
}
#endif

#endif /* _NOR_FLASH_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
