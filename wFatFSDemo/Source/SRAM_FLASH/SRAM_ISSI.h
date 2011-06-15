/**
 * @file SRAM_ISSI.h
 * @ingroup LOW_LEVEL_API
 *
 * @version V4.3.0
 * @date October 15, 2010
 * @author MCD Application Team
 * @note Module: Ext Mem Lib
 *
 * @brief It is part of <b>Ext Mem Lib</b> module.
 * This file provides a set of functions needed to drive the
 * IS61WV51216BLL SRAM memory mounted on STM3210E-EVAL board.
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

#ifndef __SRAM_ISSI_H
#define __SRAM_ISSI_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"


/**
  * FSMC Bank 1 NOR/SRAM3
  */
#define BANK1_SRAM3_ADDR    ((uint32_t)0x68000000)

 /**
   * Configures the FSMC and GPIOs to interface with the SRAM memory.
   * This function must be called before any write/read operation on the SRAM.
   */
void SramInit(void);

/**
  * Checks whether or not the SRAM FSMC bank is enabled.
  *
  * @param nBank [IN] specifies... Valid value are... TODO: STF
  *
  * @return TODO: STF
  */
uint8_t SramIsFsmcEnabled(uint32_t nBank);

/**
  * Writes a Half-word buffer to the FSMC SRAM memory.
  *
  * @param pnBuffer [IN] pointer to buffer.
  * @param nWriteAddr [IN] SRAM memory internal address from which the data will be written.
  * @param nNumHalfwordToWrite [IN] number of half-words to write.
  */
void SramWriteBuffer(uint16_t* pnBuffer, uint32_t nWriteAddr, uint32_t nNumHalfwordToWrite);

/**
  * Reads a block of data from the FSMC SRAM memory.
  *
  * @param pnBuffer [IN] pointer to the buffer that receives the data read from the SRAM memory.
  * @param nReadAddr [IN] SRAM memory internal address to read from.
  * @param nNumHalfwordToRead [IN] number of half-words to read.
  */
void SramReadBuffer(uint16_t* pnBuffer, uint32_t nReadAddr, uint32_t nNumHalfwordToRead);

/**
  * Reads a half-word from the FSMC SRAM memory.
  *
  * @param nReadAddr [IN] SRAM memory internal address to read from.
  *
  * @retval Half-word read from the SRAM memory
  */
uint16_t SramReadHalfWord(uint32_t nReadAddr);

/**
  * Writes a Half-word to the FSMC SRAM memory.
  *
  * @param nData [IN] the Half-word to be written.
  * @param nWriteAddr [IN] SRAM memory internal address where the data will be written.
  */
void SramWriteHalfWord(uint16_t nData, uint32_t nWriteAddr);

#ifdef __cplusplus
}
#endif

#endif /* SRAM_ISSI_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
