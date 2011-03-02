/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : Hardware Configuration & Setup.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "msd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Mass_Memory_Size;
uint32_t Mass_Block_Size;
uint32_t Mass_Block_Count;
sMSD_CSD MSD_csd;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    GPIO_ResetBits(GPIOD, GPIO_Pin_9);
  }
  else
  {
    GPIO_SetBits(GPIOD, GPIO_Pin_9);
  }   
}

/*******************************************************************************
* Function Name  : Get_Medium_Characteristics.
* Description    : Get the microSD card size.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_Medium_Characteristics(void)
{
  uint32_t temp1 = 0;
  uint32_t temp2 = 0;

  MSD_GetCSDRegister(&MSD_csd);

  temp1 = MSD_csd.DeviceSize + 1;
  temp2 = 1 << (MSD_csd.DeviceSizeMul + 2);

  Mass_Block_Count = temp1 * temp2;

  Mass_Block_Size =  1 << MSD_csd.RdBlockLen;

  Mass_Memory_Size = (Mass_Block_Count * Mass_Block_Size);

}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
