/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
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
static __IO uint32_t Index = 0;
static __IO uint32_t AlarmStatus = 0;
static __IO uint32_t LedCounter = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
 uint32_t tmp = 318, index = 0;

  /* Disable LCD Window mode */
  LCD_WindowModeDisable(); 

  /* If HSE is not detected at program startup or HSE clock failed during program execution */
  if((Get_HSEStartUpStatus() == ERROR) || (RCC_GetITStatus(RCC_IT_CSS) != RESET))
  { 
    /* Clear the LCD */
    LCD_Clear(White);
    /* Set the LCD Back Color */
    LCD_SetBackColor(Blue);

    /* Set the LCD Text Color */
    LCD_SetTextColor(White);

    /* Display " No Clock Detected  " message */
    LCD_DisplayStringLine(Line0, "No HSE Clock         ");
    LCD_DisplayStringLine(Line1, "Detected. STANDBY    ");
    LCD_DisplayStringLine(Line2, "mode in few seconds. ");
    
    LCD_DisplayStringLine(Line5, "If HSE Clock         ");
    LCD_DisplayStringLine(Line6, "recovers before the  ");
    LCD_DisplayStringLine(Line7, "time out, a System   ");
    LCD_DisplayStringLine(Line8, "Reset is generated.  ");
    LCD_ClearLine(Line9);
    /* Clear Clock Security System interrupt pending bit */
    RCC_ClearITPendingBit(RCC_IT_CSS);

    GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    LCD_ClearLine(Line4);
    /* Set the Back Color */
    LCD_SetBackColor(White);
    /* Set the Text Color */
    LCD_SetTextColor(Red);
    LCD_DrawRect(71, 319, 25, 320);
    LCD_SetBackColor(Green); 
    LCD_SetTextColor(White);

    /* Wait till HSE is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {
      if(index == 0x3FFFF)
      {
        LCD_DisplayChar(Line3, tmp, 0x20);
        tmp -= 16;
        index = 0;
      }
      index++;
      /* Enters the system in STANDBY mode */
      if(tmp < 16)
      {
        LCD_SetBackColor(Blue);
        LCD_ClearLine(Line3);
        LCD_ClearLine(Line4);
        LCD_ClearLine(Line5);
        LCD_ClearLine(Line6);
        LCD_DisplayStringLine(Line7, " MCU in STANDBY Mode"); 
        LCD_DisplayStringLine(Line8, "To exit press Wakeup");
        /* Request to enter STANDBY mode */
        PWR_EnterSTANDBYMode();
      }
    }
  
    /* Generate a system reset */  
    NVIC_SystemReset();
  }
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
  /* Decrement the TimingDelay variable */
  Decrement_TimingDelay();
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
  /* If counter is equal to 86339: one day was elapsed */
  if((RTC_GetCounter()/3600 == 23)&&(((RTC_GetCounter()%3600)/60) == 59)&&
     (((RTC_GetCounter()%3600)%60) == 59)) /* 23*3600 + 59*60 + 59 = 86339 */
  {
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Reset counter value */
    RTC_SetCounter(0x0);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Increment the date */
    Date_Update();
  }
  /* Clear the RTC Second Interrupt pending bit */  
  RTC_ClearITPendingBit(RTC_IT_SEC);
}

/*******************************************************************************
* Function Name  : USB_HP_CAN1_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
  CTR_HP();
}

/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
    UpFunc();
    /* Clear the EXTI Line 8 */  
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
  if(EXTI_GetITStatus(EXTI_Line9) != RESET)
  {
    /* Clear the EXTI Line 9 */  
    EXTI_ClearITPendingBit(EXTI_Line9);
  }
}

/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
  /* Clear the TIM1 Update pending bit */
  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

  if(AlarmStatus == 1)
  {
    if((LedCounter & 0x01) == 0) 
    {
      GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
    }
    else if ((LedCounter & 0x01) == 0x01)
    {
      GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
    }

    LedCounter++;

    if(LedCounter == 300)
    {
      AlarmStatus = 0;
      LedCounter = 0;
    }
  }
  else
  {
    /* If LedShowStatus is TRUE: enable leds toggling */
    if(Get_LedShowStatus() != 0)
    {
      switch(Index)
      {
        /* LD1 turned on, LD4 turned off */
        case 0:
        {
          GPIO_ResetBits(GPIOC, GPIO_Pin_9);
          GPIO_SetBits(GPIOC, GPIO_Pin_6);
          Index++;
          break;
        }
        /* LD2 turned on, LD1 turned off */
        case 1:
        {
          GPIO_ResetBits(GPIOC, GPIO_Pin_6);
          GPIO_SetBits(GPIOC, GPIO_Pin_7);
          Index++;
          break;
        }
        /* LD3 turned on, LD2 turned off */
        case 2:
        {
		  GPIO_ResetBits(GPIOC, GPIO_Pin_7);
          GPIO_SetBits(GPIOC, GPIO_Pin_8);
          Index++;
          break;
        }
        /* LD4 turned on, LD3 turned off */
        case 3:
        {
          GPIO_ResetBits(GPIOC, GPIO_Pin_8);
          GPIO_SetBits(GPIOC, GPIO_Pin_9);
          Index++;
          break;
        }

        default:
          break;
      }
      /* Reset Index to replay leds switch on sequence  */
      if(Index == 4)
      {
        Index = 0;
      }
    }
  }
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    /* Read data from SPI flash and set it as TIM4 Ch3 Pulse */
    TIM_SetCompare3(TIM4, SPI_FLASH_ReadByte());

    /* If we reach the WaveDataLength of the wave to play */
    if (Decrement_WaveDataLength() == 0)
    { 
      /* Stop wave playing */
      WavePlayer_Stop();
    }

    /* Clear TIM2 update interrupt */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}

/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line12) != RESET)
  {
    SelFunc();
    /* Clear the EXTI Line 12 */    
    EXTI_ClearITPendingBit(EXTI_Line12);
  }
  if(EXTI_GetITStatus(EXTI_Line14) != RESET)
  {
    DownFunc();  
    /* Clear the EXTI Line 14 */
    EXTI_ClearITPendingBit(EXTI_Line14);
  }
}

/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
  /* Clear the Alarm Pending Bit */
  RTC_ClearITPendingBit(RTC_IT_ALR);
  
  AlarmStatus = 1;
  Set_STOPModeStatus();

  /* Clear the EXTI Line 17/ */  
  EXTI_ClearITPendingBit(EXTI_Line17);
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void PPP_IRQHandler(void)
{
}*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
