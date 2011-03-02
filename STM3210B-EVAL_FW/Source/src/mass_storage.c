/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : mass_storage.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : This file provides a set of functions needed to manage the
*                      communication between the STM32F10x USB and the MSD.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Mass_Storage_Init
* Description    : Initializes the peripherals used by the mass storage driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Mass_Storage_Init(void)
{
  /* Disable the Pull-Up*/
  USB_Cable_Config(DISABLE);

}

/*******************************************************************************
* Function Name  : Mass_Storage_Start
* Description    : Starts the mass storage demo.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Mass_Storage_Start (void)
{
  /* Disble the JoyStick interrupts */
  IntExtOnOffConfig(DISABLE);

  /* Clear the LCD screen */
  LCD_Clear(White);

  LCD_SetDisplayWindow(160, 223, 128, 128);
 
  LCD_DrawBMP(0x00647C00);

  /* Disable LCD Window mode */
  LCD_WindowModeDisable();
  
  /* Set the Back Color */
  LCD_SetBackColor(Blue);
  /* Set the Text Color */
  LCD_SetTextColor(White); 

  if(MSD_Init() != 0x00)
  {
    LCD_DisplayStringLine(Line8, " No MSD Card Present");
    LCD_DisplayStringLine(Line9, "  To exit Press SEL ");

    /* Loop until SEL key pressed */
    while(ReadKey() != SEL)
    {
    }
  }
  else
  {
    Get_Medium_Characteristics();

    /* Display the "  Plug the USB   " message */
    LCD_DisplayStringLine(Line8, " Plug the USB Cable ");
    LCD_DisplayStringLine(Line9, "Exit:  Push JoyStick");
    
    /* Intialize the USB cell */
    USB_Init();
  
    LCD_ClearLine(Line9);
    /* Display the "To stop Press SEL" message */
    LCD_DisplayStringLine(Line8, "  To stop Press SEL ");

    /* Loop until SEL key pressed */
    while(ReadKey() != SEL)
    {
    }

    PowerOff();
  }
  LCD_Clear(White);
  DisplayMenu();
  IntExtOnOffConfig(ENABLE);
  /* Flush SPI1 Data Register */
  SPI_I2S_ReceiveData(SPI1);
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
