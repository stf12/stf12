/**
 * @file NOR_flash.c
 * @ingroup LOW_LEVEL_API
 *
 * @version V4.3.1
 * @date July 7, 2011
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



/* Includes ------------------------------------------------------------------*/
#include "syserror.h"
#include "NOR_flash.h"

/* Delay definition */
#define NOR_BLOCK_ERASE_TIMEOUT				((uint32_t)0x00A00000)
#define NOR_CHIP_ERASE_TIMEOUT				((uint32_t)0x30000000)
#define NOR_PROGRAM_TIMEOUT					((uint32_t)0x00001400)
#define NOR_BUFFERED_PROGRAM_TIMEOUT		((uint32_t)0x00001400)

#define ADDR_SHIFT(B,A) (B + (2 * (A)))
#define NOR_WRITE(nAddress, xData)  (*(volatile uint16_t *)(nAddress) = (xData))
#define NOR_IS_READY() (GPIO_ReadInputDataBit(FSMC_NWAIT_GPIO_PORT, FSMC_NWAIT_GPIO_PIN) != RESET)
#define NOR_IS_BUSY() (GPIO_ReadInputDataBit(FSMC_NWAIT_GPIO_PORT, FSMC_NWAIT_GPIO_PIN) == RESET)



//	static uint16_t nTemp2,nTemp2a,nTemp2b;
//	static uint32_t s_nTimeout = NOR_BUFFERED_PROGRAM_TIMEOUT;





void NorInit()
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(FSMC_GPIO_RCC, ENABLE);

	/*-- GPIO Configuration ------------------------------------------------------*/
	/* NOR Data lines configuration */
	GPIO_InitStructure.GPIO_Pin = FSMC_DATA0_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FSMC_DATA0_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA1_GPIO_PIN;
	GPIO_Init(FSMC_DATA1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA2_GPIO_PIN;
	GPIO_Init(FSMC_DATA2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA3_GPIO_PIN;
	GPIO_Init(FSMC_DATA3_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA4_GPIO_PIN;
	GPIO_Init(FSMC_DATA4_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA5_GPIO_PIN;
	GPIO_Init(FSMC_DATA5_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA6_GPIO_PIN;
	GPIO_Init(FSMC_DATA6_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA7_GPIO_PIN;
	GPIO_Init(FSMC_DATA7_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA8_GPIO_PIN;
	GPIO_Init(FSMC_DATA8_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA9_GPIO_PIN;
	GPIO_Init(FSMC_DATA9_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA10_GPIO_PIN;
	GPIO_Init(FSMC_DATA10_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA11_GPIO_PIN;
	GPIO_Init(FSMC_DATA11_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA12_GPIO_PIN;
	GPIO_Init(FSMC_DATA12_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA13_GPIO_PIN;
	GPIO_Init(FSMC_DATA13_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA14_GPIO_PIN;
	GPIO_Init(FSMC_DATA14_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_DATA15_GPIO_PIN;
	GPIO_Init(FSMC_DATA15_GPIO_PORT, &GPIO_InitStructure);

	/* NOR Address lines configuration */
	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR0_GPIO_PIN;
	GPIO_Init(FSMC_ADDR0_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR1_GPIO_PIN;
	GPIO_Init(FSMC_ADDR1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR2_GPIO_PIN;
	GPIO_Init(FSMC_ADDR2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR3_GPIO_PIN;
	GPIO_Init(FSMC_ADDR3_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR4_GPIO_PIN;
	GPIO_Init(FSMC_ADDR4_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR5_GPIO_PIN;
	GPIO_Init(FSMC_ADDR5_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR6_GPIO_PIN;
	GPIO_Init(FSMC_ADDR6_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR7_GPIO_PIN;
	GPIO_Init(FSMC_ADDR7_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR8_GPIO_PIN;
	GPIO_Init(FSMC_ADDR8_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR9_GPIO_PIN;
	GPIO_Init(FSMC_ADDR9_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR10_GPIO_PIN;
	GPIO_Init(FSMC_ADDR10_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR11_GPIO_PIN;
	GPIO_Init(FSMC_ADDR11_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR12_GPIO_PIN;
	GPIO_Init(FSMC_ADDR12_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR13_GPIO_PIN;
	GPIO_Init(FSMC_ADDR13_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR14_GPIO_PIN;
	GPIO_Init(FSMC_ADDR14_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR15_GPIO_PIN;
	GPIO_Init(FSMC_ADDR15_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR16_GPIO_PIN;
	GPIO_Init(FSMC_ADDR16_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR17_GPIO_PIN;
	GPIO_Init(FSMC_ADDR17_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR18_GPIO_PIN;
	GPIO_Init(FSMC_ADDR18_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR19_GPIO_PIN;
	GPIO_Init(FSMC_ADDR19_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR20_GPIO_PIN;
	GPIO_Init(FSMC_ADDR20_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR21_GPIO_PIN;
	GPIO_Init(FSMC_ADDR21_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR22_GPIO_PIN;
	GPIO_Init(FSMC_ADDR22_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_ADDR23_GPIO_PIN;
	GPIO_Init(FSMC_ADDR23_GPIO_PORT, &GPIO_InitStructure);


	/* NOE configuration */
	GPIO_InitStructure.GPIO_Pin = FSMC_NOE_GPIO_PIN;
	GPIO_Init(FSMC_NOE_GPIO_PORT, &GPIO_InitStructure);

	/* NWE configuration */
	GPIO_InitStructure.GPIO_Pin = FSMC_NWE_GPIO_PIN;
	GPIO_Init(FSMC_NWE_GPIO_PORT, &GPIO_InitStructure);

	//  /* NE1 configuration */  // for future implementation with 2 flash Banks
	//  GPIO_InitStructure.GPIO_Pin = FSMC_NE1_GPIO_PIN;
	//  GPIO_Init(FSMC_NE1_GPIO_PORT, &GPIO_InitStructure);

	/* NE2 configuration */
	GPIO_InitStructure.GPIO_Pin = FSMC_NE2_GPIO_PIN;
	GPIO_Init(FSMC_NE2_GPIO_PORT, &GPIO_InitStructure);

	/* Configure PD6 for NOR memory Ready/Busy signal */
	GPIO_InitStructure.GPIO_Pin = FSMC_NWAIT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(FSMC_NWAIT_GPIO_PORT, &GPIO_InitStructure);

	/*-- FSMC Configuration ----------------------------------------------------*/
	p.FSMC_AddressSetupTime = 0x02;
	p.FSMC_AddressHoldTime = 0x00;
	p.FSMC_DataSetupTime = 0x05;
	p.FSMC_BusTurnAroundDuration = 0x00;
	p.FSMC_CLKDivision = 0x00;
	p.FSMC_DataLatency = 0x00;
	p.FSMC_AccessMode = FSMC_AccessMode_B;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/* Enable FSMC Bank1_NOR Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);
}

uint8_t NorIsFsmcEnabled(uint32_t nBank)
{
	return (FSMC_Bank1->BTCR[nBank] & 0x1);
}

void NorReadID(uint32_t nBankAddr, NorId* pxNorId)
{
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x02AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x0090);

	pxNorId->nManufacturerCode = *(volatile uint16_t *) ADDR_SHIFT(nBankAddr,0x0000);
	pxNorId->nDeviceCode1 = *(volatile uint16_t *) ADDR_SHIFT(nBankAddr,0x0001);
	pxNorId->nDeviceCode2 = *(volatile uint16_t *) ADDR_SHIFT(nBankAddr,0x000E);
	pxNorId->nDeviceCode3 = *(volatile uint16_t *) ADDR_SHIFT(nBankAddr,0x000F);
}


sys_error_t NorEraseBlock(uint32_t nBankAddr, uint32_t nBlockAddr)
{
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x02AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x0080);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x02AA), 0x0055);
	NOR_WRITE((nBankAddr + nBlockAddr), 0x30);

	return (NorGetStatus(nBankAddr,NOR_BLOCK_ERASE_TIMEOUT));
}


sys_error_t NorEraseChip(uint32_t nBankAddr)
{
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x02AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x0080);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x02AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x0010);

	return (NorGetStatus(nBankAddr,NOR_CHIP_ERASE_TIMEOUT));
}


sys_error_t NorWriteHalfWord(uint32_t nBankAddr, uint32_t nWriteAddr, uint16_t nData)
{
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x02AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x00A0);
	NOR_WRITE((nBankAddr + nWriteAddr), nData);

	return (NorGetStatus(nBankAddr,NOR_PROGRAM_TIMEOUT));
}


sys_error_t NorWriteBuffer(uint32_t nBankAddr, uint16_t* pnBuffer, uint32_t nWriteAddr, uint32_t nNumHalfwordToWrite)
{
	sys_error_code_t xStatus = SYS_NOR_FLASH_ONGOING_ERROR_CODE;

	do
	{
		/* Transfer data to the memory */
		xStatus = SYS_GET_LOW_LEVEL_ERROR_CODE(NorWriteHalfWord(nBankAddr,nWriteAddr, *pnBuffer++));
		nWriteAddr = nWriteAddr + 2;
		nNumHalfwordToWrite--;
	}
	while((xStatus == SYS_NO_ERROR_CODE) && (nNumHalfwordToWrite != 0));

	if(xStatus == SYS_NO_ERROR_CODE)
		return SYS_NO_ERROR;
	else
	{
		SYS_SET_LOW_LEVEL_ERROR_CODE(xStatus);
		return SYS_GET_LAST_ERROR();
	}
}


uint16_t NorReadHalfWord(uint32_t nBankAddr, uint32_t nReadAddr)
{
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x00555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x002AA), 0x0055);
	NOR_WRITE((nBankAddr + nReadAddr), 0x00F0 );

	return (*(volatile uint16_t *)((nBankAddr + nReadAddr)));
}


void NorReadBuffer(uint32_t nBankAddr, uint16_t* pnBuffer, uint32_t nReadAddr, uint32_t nNumHalfwordToRead)
{
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x02AA), 0x0055);
	NOR_WRITE((nBankAddr + nReadAddr), 0x00F0);

	for(; nNumHalfwordToRead != 0x00; nNumHalfwordToRead--) /* while there is data to read */
	{
		/* Read a Halfword from the NOR */
		*pnBuffer++ = *(volatile uint16_t *)((nBankAddr + nReadAddr));
		nReadAddr = nReadAddr + 2;
	}
}


sys_error_t NorReturnToReadMode(uint32_t nBankAddr)
{
	NOR_WRITE(nBankAddr, 0x00F0);

	return (SYS_NO_ERROR);
}


sys_error_t NorReset(uint32_t nBankAddr)
{
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x00555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x002AA), 0x0055);
	NOR_WRITE(nBankAddr, 0x00F0);

	return (SYS_NO_ERROR);
}


sys_error_t NorGetStatus(uint32_t nBankAddr, uint32_t nTimeout)
{
	uint16_t nVal1 = 0x00, nVal2 = 0x00;
	sys_error_code_t xStatus = SYS_NOR_FLASH_ONGOING_ERROR_CODE;
	uint32_t nTimeoutTemp = nTimeout;

	/* Poll on NOR memory Ready/Busy signal (FSMC_NWAIT) ---------------------*/
	while((GPIO_ReadInputDataBit(FSMC_NWAIT_GPIO_PORT, FSMC_NWAIT_GPIO_PIN) != RESET) && (nTimeoutTemp > 0))
	{
		nTimeoutTemp--;
	}

	nTimeoutTemp = nTimeout;

	while((GPIO_ReadInputDataBit(FSMC_NWAIT_GPIO_PORT, FSMC_NWAIT_GPIO_PIN) == RESET) && (nTimeoutTemp > 0))
	{
		nTimeoutTemp--;
	}

	/* Get the NOR memory operation status -----------------------------------*/
	/* Refer to datasheet M29W128G, page 55 */
	while((nTimeout != 0x00) && (xStatus != SYS_NO_ERROR_CODE))
	{
		nTimeout--;

		/* Read DQ6 and DQ5 */
		nVal1 = *(volatile uint16_t *)(nBankAddr);
		nVal2 = *(volatile uint16_t *)(nBankAddr);

		/* If DQ6 did not toggle between the two reads then return NOR_Success */
		if((nVal1 & 0x0040) == (nVal2 & 0x0040))
		{
			return SYS_NO_ERROR;
		}

		if((nVal1 & 0x0020) != 0x0020) //DQ5 = 0 -> Program ongoing
		{
			xStatus = SYS_NOR_FLASH_ONGOING_ERROR_CODE;
		}

		nVal1 = *(volatile uint16_t *)(nBankAddr);
		nVal2 = *(volatile uint16_t *)(nBankAddr);

		if((nVal1 & 0x0040) == (nVal2 & 0x0040))
		{
			return SYS_NO_ERROR;
		}
		else if((nVal1 & 0x0020) == 0x0020)
		{
			SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_NOR_FLASH_ERROR_ERROR_CODE);
			return SYS_GET_LAST_ERROR();
		}
	}

	if(nTimeout == 0x00)
	{
		SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_NOR_FLASH_TIMEOUT_ERROR_CODE);
		return SYS_GET_LAST_ERROR();
	}

	/* Return the operation xStatus */
	return SYS_NO_ERROR;
}

sys_error_t NorWriteToBufferAndProgramAbort(uint32_t nBankAddr)
{
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x00555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x002AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x00555), 0x00F0);

	return (SYS_NO_ERROR);
}


sys_error_t NorWriteToBufferAndProgram(uint32_t nBankAddr, uint16_t* pnBuffer, uint32_t nWriteAddr, uint32_t nNumHalfwordToWrite)
{
	uint16_t nTemp2 = 0xA5A5;
	uint32_t s_nTimeout = NOR_BUFFERED_PROGRAM_TIMEOUT;

	uint32_t nWriteBufferLocation = nWriteAddr;

	uint32_t nHalfwordToWrite = nNumHalfwordToWrite;
	if((nHalfwordToWrite > FLASH_PAGE_SIZE /2) || (nHalfwordToWrite > (FLASH_PAGE_SIZE - (nWriteAddr & FLASH_PAGE_SIZE_MASK)) /2 ))  //must have page aligned data
	{
		SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_NOR_FLASH_CANNOT_USE_WRITE_TO_BUFFER);
		return SYS_GET_LAST_ERROR();
	}

	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x0555), 0x00AA);  //unlock command issue
	NOR_WRITE(ADDR_SHIFT(nBankAddr,0x02AA), 0x0055);
	NOR_WRITE((nBankAddr + nWriteBufferLocation ), 0x0025); //write to buffer and program command issue, p.40 datasheet
	NOR_WRITE((nBankAddr + nWriteBufferLocation ), nNumHalfwordToWrite-1);

	do
	{
		/* Transfer data to the write buffer */
		NOR_WRITE((nBankAddr + nWriteAddr ), *pnBuffer++);
		nWriteAddr += 2;
		nHalfwordToWrite--;
	}
	while(nHalfwordToWrite != 0);

	NOR_WRITE((nBankAddr + nWriteBufferLocation), 0x0029);  //write to buffer and program command confirm

	pnBuffer--;
	nWriteAddr -= 2;

	while (s_nTimeout--)
	{
		if(NOR_IS_BUSY()){
			nTemp2 = *(volatile uint16_t *)(nBankAddr + nWriteAddr);  //read status register at last loaded address
		}

		if(NOR_IS_READY())
		{
			if((nTemp2 & 0xFF00)!=0x0000)
			{
				return SYS_NO_ERROR;
			}
			if((nTemp2 & 0x0080) != ((*pnBuffer) & 0x0080))  //DQ7 = !DATA -> OK
			{
				return SYS_NO_ERROR;
			}

			else if((nTemp2 & 0x0020) == 0x0020)
			{
				break;
			}
			else if((nTemp2 & 0x0002) == 0x0002)
			{
				break;
			}
			break;
		}
	}

	nTemp2 = *(volatile uint16_t *)(nBankAddr + nWriteAddr);  //read status register at last loaded address

	if(NOR_IS_READY() && (nTemp2 == (*pnBuffer) ))  //DQ7 = !DATA -> OK
		return SYS_NO_ERROR;
	else
	{
		NorWriteToBufferAndProgramAbort(nBankAddr);
		SYS_SET_LOW_LEVEL_ERROR_CODE(SYS_NOR_FLASH_WRITE_TO_BUFFER_FAILED);
		return SYS_GET_LAST_ERROR();
	}
}


sys_error_t NorFastWriteBuffer(uint32_t nBankAddr, uint16_t* pnBuffer, uint32_t nWriteAddr, uint32_t nNumHalfwordToWrite, uint32_t* nHalfWordActuallyWritten)
{
	*nHalfWordActuallyWritten = 0;
	if(((nWriteAddr & FLASH_PAGE_SIZE_MASK)!=0) && (nNumHalfwordToWrite < (FLASH_PAGE_SIZE - (nWriteAddr & FLASH_PAGE_SIZE_MASK)) /2 ))  //not aligned data
	{
		if(SYS_GET_LOW_LEVEL_ERROR_CODE(NorWriteToBufferAndProgram(nBankAddr,pnBuffer,nWriteAddr,nNumHalfwordToWrite))!=0)
			return SYS_GET_LAST_ERROR();
		else
		{
			*nHalfWordActuallyWritten += nNumHalfwordToWrite;
			return SYS_NO_ERROR;
		}
	}

	if(((nWriteAddr & FLASH_PAGE_SIZE_MASK)!=0) && (nNumHalfwordToWrite > (FLASH_PAGE_SIZE - (nWriteAddr & FLASH_PAGE_SIZE_MASK)) /2 ))  //not aligned data
	{
		if(SYS_GET_LOW_LEVEL_ERROR_CODE(NorWriteToBufferAndProgram(nBankAddr,pnBuffer, nWriteAddr, (FLASH_PAGE_SIZE - (nWriteAddr & FLASH_PAGE_SIZE_MASK))/2))!=0)
			return SYS_GET_LAST_ERROR();
		*nHalfWordActuallyWritten = (FLASH_PAGE_SIZE - (nWriteAddr & FLASH_PAGE_SIZE_MASK)) /2;
		nNumHalfwordToWrite -= *nHalfWordActuallyWritten;
		nWriteAddr += (*nHalfWordActuallyWritten) * 2;
		pnBuffer += *nHalfWordActuallyWritten;
	}

	while(nNumHalfwordToWrite != 0)  //data aligned
	{
		if(nNumHalfwordToWrite <= FLASH_PAGE_SIZE /2)
		{
			if(SYS_GET_LOW_LEVEL_ERROR_CODE(NorWriteToBufferAndProgram(nBankAddr,pnBuffer,nWriteAddr,nNumHalfwordToWrite))!=0)
				return SYS_GET_LAST_ERROR();
			else
			{
				*nHalfWordActuallyWritten += nNumHalfwordToWrite;
				break;
			}
		}
		else
		{

			for (uint8_t nTryProgram = 0;nTryProgram<3;nTryProgram++)
			{
				if(SYS_GET_LOW_LEVEL_ERROR_CODE(NorWriteToBufferAndProgram(nBankAddr,pnBuffer,nWriteAddr,FLASH_PAGE_SIZE/2))!=0)
				{
					if(nTryProgram==3)
					  return SYS_GET_LAST_ERROR();
				}
				else
				  break;
			}

			*nHalfWordActuallyWritten += FLASH_PAGE_SIZE /2;
			nNumHalfwordToWrite -= FLASH_PAGE_SIZE /2;
			nWriteAddr += FLASH_PAGE_SIZE;
			pnBuffer += FLASH_PAGE_SIZE /2;
		}
	}
	return SYS_NO_ERROR;
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
