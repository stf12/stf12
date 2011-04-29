/**
  ******************************************************************************
  * @file    stm3220f_eval_fsmc_psram.c
  * @author  MCD Application Team
  * @version V4.X.0RC10
  * @date    10/15/2010
  * @brief   This file provides a set of functions needed to drive the 
  *          MT45V256KW16 PSRAM memory mounted on STM3220F-EVAL board RevA.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm3220f_eval_fsmc_psram.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM3220F_EVAL
  * @{
  */
  
/** @addtogroup STM3220F_EVAL_FSMC_PSRAM
  * @brief      This file provides a set of functions needed to drive the 
  *             MT45V256KW16 PSRAM memory mounted on STM3220F-EVAL board.
  * @{
  */ 

/** @defgroup STM3220F_EVAL_FSMC_PSRAM_Private_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM3220F_EVAL_FSMC_PSRAM_Private_Defines
  * @{
  */ 
/** 
  * @brief  FSMC Bank 1 NOR/SRAM3  
  */
#define Bank1_SRAM2_ADDR  ((uint32_t)0x64000000)    
/**
  * @}
  */ 


/** @defgroup STM3220F_EVAL_FSMC_PSRAM_Private_Macros
  * @{
  */
/**
  * @}
  */ 
  

/** @defgroup STM3220F_EVAL_FSMC_PSRAM_Private_Variables
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM3220F_EVAL_FSMC_PSRAM_Private_Function_Prototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM3220F_EVAL_FSMC_PSRAM_Private_Functions
  * @{
  */ 

/**
  * @brief  Configures the FSMC and GPIOs to interface with the PSRAM memory.
  *         This function must be called before any write/read operation
  *         on the PSRAM.
  * @param  None 
  * @retval None
  */
void PSRAM_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
  GPIO_InitTypeDef GPIO_InitStructure; 
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF, ENABLE);

  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 
  
/*-- GPIO Configuration ------------------------------------------------------*/
  /* PSRAM Data lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;

  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

  /* PSRAM Address lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource2, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource3, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource5, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource12, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource13, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource15, GPIO_AF_FSMC);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 | GPIO_Pin_5;

  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOG,GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource2, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource3, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource5, GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD,GPIO_PinSource11, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource12, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13, GPIO_AF_FSMC);

  /* NOE and NWE configuration */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;

  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5, GPIO_AF_FSMC);

  /* NE2 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 

  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_FSMC);

  /* NBL0, NBL1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
  GPIO_Init(GPIOE, &GPIO_InitStructure); 

  GPIO_PinAFConfig(GPIOE,GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource1, GPIO_AF_FSMC);

/*-- FSMC Configuration ------------------------------------------------------*/
  p.FSMC_AddressSetupTime = 0x2;
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 0xB;
  p.FSMC_BusTurnAroundDuration = 1;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_PSRAM;
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

  /*!< Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE); 
}

/**
  * @brief  Writes a Half-word buffer to the FSMC PSRAM memory. 
  * @param  pBuffer : pointer to buffer. 
  * @param  WriteAddr : PSRAM memory internal address from which the data will be 
  *         written.
  * @param  NumHalfwordToWrite : number of half-words to write. 
  * @retval None
  */
void PSRAM_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
  for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
  {
    /* Transfer data to the memory */
    *(uint16_t *) (Bank1_SRAM2_ADDR + WriteAddr) = *pBuffer++;
    
    /* Increment the address*/  
    WriteAddr += 2;
  } 
}

/**
  * @brief  Reads a block of data from the FSMC PSRAM memory.
  * @param  pBuffer : pointer to the buffer that receives the data read from the 
  *         PSRAM memory.
  * @param  ReadAddr : PSRAM memory internal address to read from.
  * @param  NumHalfwordToRead : number of half-words to read.
  * @retval None
  */
void PSRAM_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
  for(; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
  {
    /* Read a half-word from the memory */
    *pBuffer++ = *(__IO uint16_t*) (Bank1_SRAM2_ADDR + ReadAddr);

    /* Increment the address*/  
    ReadAddr += 2;
  } 
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */  

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
