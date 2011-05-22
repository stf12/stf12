/**
  ******************************************************************************
  * @file    stm3220f_eval_fsmc_psram.h
  * @author  MCD Application Team
  * @version V4.X.0RC10
  * @date    10/15/2010
  * @brief   This file contains all the functions prototypes for the 
  *          stm3220f_eval_fsmc_psram firmware driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM3220F_EVAL_FSMC_PSRAM_H
#define __STM3220F_EVAL_FSMC_PSRAM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"

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
  * @{
  */  

/** @defgroup STM3220F_EVAL_FSMC_PSRAM_Exported_Types
  * @{
  */
/**
  * @}
  */
  
/** @defgroup STM3220F_EVAL_FSMC_PSRAM_Exported_Constants
  * @{
  */ 
/**
  * @}
  */ 
  
/** @defgroup STM3220F_EVAL_FSMC_PSRAM_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM3220F_EVAL_FSMC_PSRAM_Exported_Functions
  * @{
  */      

void PSRAM_Init(void);
void PSRAM_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
void PSRAM_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);

#ifdef __cplusplus
}
#endif

#endif /* __STM3220F_EVAL_FSMC_PSRAM_H */
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
