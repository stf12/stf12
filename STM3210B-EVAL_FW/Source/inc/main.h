/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : main.h
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : Header for main.c module
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm3210b_lcd.h"
#include "spi_flash.h"
#include "stm32f10x_it.h"
#include "waveplayer.h"
#include "menu.h"
#include "calendar.h"
#include "tsensor.h"
#include "mass_storage.h"
#include "lowpower.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Demo_Init(void);
void InterruptConfig(void);
void SysTick_Configuration(void);
void IntExtOnOffConfig(FunctionalState NewState);
void GPIO_Config(void);
void Delay(__IO uint32_t nCount);
uint32_t DelayJoyStick(__IO uint32_t nTime);
void Decrement_TimingDelay(void);
void LedShow_Init(void);
void LedShow(FunctionalState NewState);
uint32_t Get_LedShowStatus(void);
void CheckBitmapFilesStatus(void);
ErrorStatus Get_HSEStartUpStatus(void);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
