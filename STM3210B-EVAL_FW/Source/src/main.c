/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : Main program body
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
static __IO uint32_t TimingDelay = 0;
static __IO uint32_t LedShowStatus = 0;
static __IO ErrorStatus HSEStartUpStatus = SUCCESS;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
  /* Initialize the Demo */
  Demo_Init();

  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : Demo_Init
* Description    : Initializes the demonstration application.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Demo_Init(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
 
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
 
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
 
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);
 
    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }
 
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }

  /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
  
  /* TIM Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  /* SPI2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  
  /* TIM2  and TIM4 clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4, ENABLE);

  /* USBCLK = PLLCLK/1.5 */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

/*------------------- Resources Initialization -----------------------------*/
  /* GPIO Configuration */
  GPIO_Config();

  /* Interrupt Configuration */
  InterruptConfig();

  /* Configure the systick */    
  SysTick_Configuration();

/*------------------- Drivers Initialization -------------------------------*/
  /* Initialize the LEDs toogling */
  LedShow_Init();

  /* Initialize the Low Power application */
  LowPower_Init();

  /* Initialize the LCD */
  STM3210B_LCD_Init();

  /* Clear the LCD */ 
  LCD_Clear(White);
  
  /* If HSE is not detected at program startup */
  if(HSEStartUpStatus == ERROR)
  {
    /* Generate NMI exception */
    SCB->ICSR |= SCB_ICSR_NMIPENDSET;
  }  
  
  /* Initialize the SPI FLASH driver */
  SPI_FLASH_Init();

  /* Initialize Wave player application */
  WavePlayer_Init();

  /* Initialize the SPI FLASH driver */
  SPI_FLASH_Init();
  
  /* Checks the availability of the bitmap files */
  CheckBitmapFilesStatus();
  
  /* Display the STM32 introduction */
  STM32Intro();

  /* Clear the LCD */ 
  LCD_Clear(White);

  /* Initialize the Calendar */
  Calendar_Init();
  
  /* Initialize the Thermometer application */
  I2C_LM75_Init();

  /* Initialize the USB Mass Storage application */
  Mass_Storage_Init();

  /* Enable Leds toggling */
  LedShow(ENABLE);

  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);

  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
  
  /* Initialize the Menu */
  Menu_Init();

  /* Display the main menu icons */
  ShowMenuIcons();
}

/*******************************************************************************
* Function Name  : InterruptConfig
* Description    : Configures the used IRQ Channels and sets their priority.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void InterruptConfig(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  
  /* Set the Vector Table base address at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
  
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the EXTI15_10 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the TIM2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the USB_LP_CAN_RX0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the USB_HP_CAN_TX Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the TIM1 UP Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the EXTI Line17 Interrupt */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Configure a SysTick Base time to 10 ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 10 msec interrupts  */
  if (SysTick_Config(SystemFrequency / 100))
  { 
    /* Capture error */ 
    while (1);
  }
  
 /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/*******************************************************************************
* Function Name  : IntExtOnOffConfig
* Description    : Enables or disables EXTI for the menu navigation keys :
*                  EXTI lines 8, 12 and 14 which correpond respectively
*                  to "UP", "SEL" and "DOWN".
* Input          : NewState: New state of the navigation keys. This parameter
*                  can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void IntExtOnOffConfig(FunctionalState NewState)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Initializes the EXTI_InitStructure */
  EXTI_StructInit(&EXTI_InitStructure);

  /* Disable the EXTI line 8, 12 and 14 on falling edge */
  if(NewState == DISABLE)
  {
    EXTI_InitStructure.EXTI_Line = EXTI_Line8 | EXTI_Line12 | EXTI_Line14;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
  /* Enable the EXTI line 8, 12 and 14 on falling edge */
  else
  {
    /* Clear the the EXTI line 8, 12 and 14 interrupt pending bit */
    EXTI_ClearITPendingBit(EXTI_Line8 | EXTI_Line12 | EXTI_Line14);

    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line8 | EXTI_Line12 | EXTI_Line14;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
}

/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : Configures the different GPIO ports pins.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure PD.08, PD.12 and PD.14 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure PE.00 and PE.01 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Configure PB.09 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* PD.09 used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* RIGHT Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);

  /* LEFT Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource1);

  /* DOWN Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource14);

  /* UP Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);

  /* SEL Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource12);

  /* KEY Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
  TimingDelay = nCount;

  /* Enable the SysTick Counter */
  SysTick->CTRL |= SysTick_CTRL_ENABLE;

  while(TimingDelay != 0);

  /* Disable the SysTick Counter */
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE;

   /* Clear the SysTick Counter */
   SysTick->VAL = (uint32_t)0x0;
}

/*******************************************************************************
* Function Name  : DelayJoyStick
* Description    : Inserts a delay time while no joystick RIGHT, LEFT and SEL 
                   pushbuttons are pressed.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : Pressed Key.  This value can be: NOKEY, RIGHT, LEFT or SEL.
*******************************************************************************/
uint32_t DelayJoyStick(__IO uint32_t nTime)
{
  __IO uint32_t keystate = 0;

  TimingDelay = nTime;
 
  while((TimingDelay != 0) && (keystate != RIGHT) && (keystate != LEFT) && (keystate != SEL))
  {
    keystate = ReadKey();
  } 
  
  return keystate;
}

/*******************************************************************************
* Function Name  : Decrement_TimingDelay
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void Decrement_TimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/*******************************************************************************
* Function Name  : LedShow_Init
* Description    : Configure the leds pins as output pushpull: LED1, LED2, LED3
*                  and LED4
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LedShow_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);

  /* PC.06, PC.07, PC.08 and PC.09 as output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 719;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0x270F;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1, 2, 3 and 4 Configuration in Timing mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 0x0;
  
  TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
}

/*******************************************************************************
* Function Name  : LedShow
* Description    : Enables or disables LED1, LED2, LED3 and LED4 toggling.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LedShow(FunctionalState NewState)
{
  /* Enable LEDs toggling */
  if(NewState == ENABLE)
  {
    LedShowStatus = 1;
    /* Enable the TIM Update Interrupt */
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    /* TIM counter enable */
    TIM_Cmd(TIM1, ENABLE);
  }
  /* Disable LEDs toggling */
  else
  {
    LedShowStatus = 0;
    /* Disable the TIM Update Interrupt */
    TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
    /* TIM counter disable */
    TIM_Cmd(TIM1, DISABLE);
  }
}

/*******************************************************************************
* Function Name  : Get_LedShowStatus
* Description    : Get the LedShowStatus value.
* Input          : None
* Output         : None
* Return         : LedShowStatus Value.
*******************************************************************************/
uint32_t Get_LedShowStatus(void)
{
  return LedShowStatus;
}

/*******************************************************************************
* Function Name  : CheckBitmapFilesStatus
* Description    : Checks the bitmap files availability and display a warning 
*                  message if these files doesn't exit.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CheckBitmapFilesStatus(void)
{
  /* Checks if the Bitmap files are loaded */
  if(CheckBitmapFiles() != 0)
  {
    /* Set the LCD Back Color */
    LCD_SetBackColor(Blue);

    /* Set the LCD Text Color */
    LCD_SetTextColor(White);    
    LCD_DisplayStringLine(Line0, "      Warning       ");
    LCD_DisplayStringLine(Line1, "No loaded Bitmap    ");
    LCD_DisplayStringLine(Line2, "files. Demo can't be");
    LCD_DisplayStringLine(Line3, "executed.           ");
    LCD_DisplayStringLine(Line4, "Please be sure that ");
    LCD_DisplayStringLine(Line5, "all files are       ");
    LCD_DisplayStringLine(Line6, "correctly programmed");
    LCD_DisplayStringLine(Line7, "in the SPI FLASH and");
    LCD_DisplayStringLine(Line8, "restart the Demo.   ");
    LCD_DisplayStringLine(Line9, "                    ");    
 
    /* Deinitializes the RCC */
    RCC_DeInit();
    
    /* Demo Can't Start */
    while(1)
    {
    }
  }
}

/*******************************************************************************
* Function Name  : Get_HSEStartUpStatus
* Description    : Returns the HSE StartUp Status.
* Input          : None
* Output         : None
* Return         : HSE StartUp Status.
*******************************************************************************/
ErrorStatus Get_HSEStartUpStatus(void)
{
  return (HSEStartUpStatus);
}

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  LCD_Clear(White);
  LCD_DisplayStringLine(Line0, "   Assert Function  ");

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
