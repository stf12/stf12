/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : tsensor.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : TSensor driver source file.
*                      Pin assignment:
*             ------------------------------------------
*             |  STM32F10x    |   STLM75     Pin       |
*             ------------------------------------------
*             | PB7/ SDA      |   SDA         1        |
*             | PB6/ SCL      |   SCL         2        |
*             | PB5/          |   OS/INT      3        |
*             | .             |   GND         4  (0V)  |
*             | .             |   GND         5  (0V)  |
*             | .             |   GND         6  (0V)  |
*             | .             |   GND         7  (0V)  |
*             | .             |   VDD         8  (5V)  |
*             ------------------------------------------
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "tsensor.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LM75_Addr      0x90 /* LM75 address */
#define LM75_SD_Set    0x01 /* Set SD bit in the configuration register */
#define LM75_SD_Reset  0xFE /* Reset SD bit in the configuration register */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t TempCelsius_Display[] = "     +abc.def C     ";
static uint8_t TempFahrenheit_Display[] = "     +abc.def F     ";
static uint32_t Temp_Decimal = 0, TempCelsius_Value = 0, Temp_Value_Fahrenheit = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : I2C_LM75_Init
* Description    : Initializes the I2C1.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_LM75_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  I2C_InitTypeDef   I2C_InitStructure;

  /* GPIOB Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
  /* I2C1 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

  /* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure PB.5 as Input pull-up, used as TemperatureSensor_INT */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  I2C_DeInit(I2C1);

  /* I2C1 Init */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; 
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 200000;
  I2C_Init(I2C1, &I2C_InitStructure);

  /* I2C1 Init */
  I2C_Cmd(I2C1, ENABLE);
}

/*******************************************************************************
* Function Name  : I2C_LM75_Status
* Description    : Checks the LM75 status.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
ErrorStatus I2C_LM75_Status(void)
{
  uint32_t I2C_TimeOut = 0x3FFFF;

  /* Clear the I2C1 AF flag */
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);

  /* Enable I2C1 acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  /*----- Transmission Phase -----*/
  
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on I2C1 EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }
  
  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Transmitter);
  
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && I2C_TimeOut)/* EV6 */
  {
    I2C_TimeOut--;
  }

  if(I2C_GetFlagStatus(I2C1, I2C_FLAG_AF) != 0x0)
  {
    return ERROR;
  }
  else
  {
    return SUCCESS;
  }
}

/*******************************************************************************
* Function Name  : I2C_LM75_Reg_Read
* Description    : Read the specified register from the LM75.
* Input          : RegValue: this member specifies the register to read:
*                    - LM75_TEMP_Reg: temperature register
*                    - LM75_TOS_Reg: Over-limit temperature register
*                    - LM75_THYS_Reg: Hysteresis temperature register
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t I2C_LM75_Reg_Read(uint8_t RegName)
{
  uint32_t RegValue=0;

  /* Enable I2C1 acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  /*----- Transmission Phase -----*/
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on I2C1 EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }
  
  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Transmitter);
  
  /* Test on I2C1 EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the specified register data pointer */
  I2C_SendData(I2C1, RegName);
  
  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /*----- Reception Phase -----*/
  /* Send Re-STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }
  
  /* Send STLM75 slave address for read */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /* EV6 */
  {
  }
  

  /* Test on EV7 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  /* EV7 */
  {
  }
  
  /* Store I2C1 received data */
  RegValue = (uint16_t)(I2C_ReceiveData(I2C1) << 8);
  
  /* Disable I2C1 acknowledgement */
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  
  /* Send I2C1 STOP Condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
  
  /* Test on EV7 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  /* EV7 */
  {
  }
  
  /* Store I2C1 received data */
  RegValue |= I2C_ReceiveData(I2C1);

  /* Return register value */
  return (RegValue >> 7);
}

/*******************************************************************************
* Function Name  : I2C_LM75_Reg_Write
* Description    : Write to the specified register of the LM75.
* Input          : RegValue: this member specifies the register to read:
*                    - LM75_TEMP_Reg: temperature register
*                    - LM75_TOS_Reg: Over-limit temperature register
*                    - LM75_THYS_Reg: Hysteresis temperature register
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_LM75_Reg_Write(uint8_t RegName, uint16_t RegValue)
{
  /* Shift left register value */
  RegValue = RegValue << 7;

  /*----- Transmission Phase -----*/
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on I2C1 EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }
  
  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Transmitter);
  
  /* Test on I2C1 EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the specified register data pointer */
  I2C_SendData(I2C1, RegName);
  
  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send I2C1 data */
  I2C_SendData(I2C1, (uint8_t)(RegValue >> 8));
  
  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }
  
  /* Send I2C1 data */
  I2C_SendData(I2C1, (uint8_t)RegValue);
  
  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send I2C1 STOP Condition */
  I2C_GenerateSTOP(I2C1, ENABLE);    
}

/*******************************************************************************
* Function Name  : I2C_LM75_Temp_Read
* Description    : Read Temperature register of LM75: double temperature value.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t I2C_LM75_Temp_Read(void)
{
  uint32_t RegValue = 0;

  /* Enable I2C1 acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  /*----- Transmission Phase -----*/
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on I2C1 EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }
  
  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Transmitter);
  
  /* Test on I2C1 EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the temperature register data pointer */
  I2C_SendData(I2C1, LM75_TEMP_Reg);
  
  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /*----- Reception Phase -----*/
  /* Send Re-STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }
  
  /* Send STLM75 slave address for read */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /* EV6 */
  {
  }

  /* Test on EV7 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  /* EV7 */
  {
  }
  
  /* Store I2C1 received data */
  RegValue = I2C_ReceiveData(I2C1) << 8;
  
  /* Disable I2C1 acknowledgement */
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  
  /* Send I2C1 STOP Condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
  
  /* Test on EV7 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  /* EV7 */
  {
  }
  
  /* Store I2C1 received data */
  RegValue |= I2C_ReceiveData(I2C1);

  /* Return Temperature value */
  return (RegValue >> 5);
}

/*******************************************************************************
* Function Name  : I2C_LM75_ConfReg_Read
* Description    : Read the configuration register from the LM75.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t I2C_LM75_ConfReg_Read(void)
{
  uint32_t RegValue = 0;

  /*----- Transmission Phase -----*/
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on I2C1 EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Transmitter);
  
  /* Test on I2C1 EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the configuration register data pointer */
  I2C_SendData(I2C1, LM75_CONF_Reg);
  
  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /*----- Reception Phase -----*/
  /* Send Re-STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }
  
  /* Send STLM75 slave address for read */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /* EV6 */
  {
  }

  /* Disable I2C1 acknowledgement */
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  
  /* Send I2C1 STOP Condition */
  I2C_GenerateSTOP(I2C1, ENABLE);

  /* Test on EV7 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  /* EV7 */
  {
  }
  
  /* Store I2C1 received data */
  RegValue = I2C_ReceiveData(I2C1);

  /* Return configuration register value */
  return (RegValue);    
}

/*******************************************************************************
* Function Name  : I2C_LM75_ConfReg_Write
* Description    : Write to the configuration register of the LM75.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_LM75_ConfReg_Write(uint8_t RegValue)
{
  /*----- Transmission Phase -----*/
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on I2C1 EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }
  
  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Transmitter);
  
  /* Test on I2C1 EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the configuration register data pointer */
  I2C_SendData(I2C1, LM75_CONF_Reg);
  
  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send I2C1 data */
  I2C_SendData(I2C1, RegValue);
  
  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send I2C1 STOP Condition */
  I2C_GenerateSTOP(I2C1, ENABLE);     
}

/*******************************************************************************
* Function Name  : I2C_LM75_ShutDown
* Description    : Enables or disables the LM75.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_LM75_ShutDown(FunctionalState NewState)
{
  uint32_t RegValue = 0;

  /*----- Transmission Phase -----*/
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on I2C1 EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Transmitter);
  
  /* Test on I2C1 EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the configuration register data pointer */
  I2C_SendData(I2C1, LM75_CONF_Reg);
  
  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  /* EV8 */
  {
  }

  /*----- Reception Phase -----*/
  /* Send Re-STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }
  
  /* Send STLM75 slave address for read */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /* EV6 */
  {
  }
  
  /* Disable I2C1 acknowledgement */
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  
  /* Send I2C1 STOP Condition */
  I2C_GenerateSTOP(I2C1, ENABLE);

  /* Test on EV7 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  /* EV7 */
  {
  }
  
  /* Store I2C1 received data */
  RegValue = I2C_ReceiveData(I2C1);

  /*----- Transmission Phase -----*/
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on I2C1 EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }
  
  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(I2C1, LM75_Addr, I2C_Direction_Transmitter);
  
  /* Test on I2C1 EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }
  
  /* Send the configuration register data pointer */
  I2C_SendData(I2C1, LM75_CONF_Reg);
  
  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Enable or disable SD bit */
  if (NewState != DISABLE)
  {
    /* Disable LM75 */
    I2C_SendData(I2C1, RegValue | LM75_SD_Set);
  }
  else
  {
    /* Enable LM75 */
    I2C_SendData(I2C1, RegValue & LM75_SD_Reset);
  }

  /* Test on I2C1 EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send I2C1 STOP Condition */
  I2C_GenerateSTOP(I2C1, ENABLE);     
}

/*******************************************************************************
* Function Name  : Thermometer_Temperature
* Description    : Displays the temperature in Celsius and fahrenheit degree.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Thermometer_Temperature(void)
{
  uint32_t i = 0;
  
  while(ReadKey() != NOKEY)
  {
  }

  if(I2C_LM75_Status() == SUCCESS)
  {
    /* Disable the JoyStick Interrupts */
    IntExtOnOffConfig(DISABLE);

    /* Clear the LCD */
    LCD_Clear(White);
  
    /* Set the Icon display window */ 
    LCD_SetDisplayWindow(150, 210, 64, 84);
 
    /* Display the Thermometer icon */
    LCD_DrawBMP(0x0062A300);

    /* Disable LCD Window mode */
    LCD_WindowModeDisable();

    /* Set the Back Color */
    LCD_SetBackColor(Red);
    /* Set the Text Color */
    LCD_SetTextColor(White);

    LCD_DisplayStringLine(Line2, "     Temperature    ");

    /* Set the Back Color */
    LCD_SetBackColor(Blue);
    /* Set the Text Color */
    LCD_SetTextColor(Green);

    /* Wait until no key is pressed */
    while(ReadKey() != NOKEY)
    {
    }
    /* Wait until a key is beiing pressed */
    while(ReadKey() == NOKEY)
    {
      /* Get double of Temperature value */
      TempCelsius_Value = I2C_LM75_Temp_Read();

      if(TempCelsius_Value <= 1023)
      {
        /* Positive temperature measured */
        TempCelsius_Display[5] = '+';
        TempFahrenheit_Display[5] = '+'; 
      }
      else
      {
        /* Negative temperature measured */
        TempCelsius_Display[5] = '-'; 
        TempFahrenheit_Display[5] = '-'; 
        /* Remove temperature value sign */
        TempCelsius_Value = 0x800 - TempCelsius_Value;
      }

      /* Calculate temperature digits in °C */
      Temp_Decimal = ((TempCelsius_Value & 7) * 1000 / 8);
      TempCelsius_Display[10] = (Temp_Decimal / 100) + 0x30;
      TempCelsius_Display[11] = ((Temp_Decimal % 100) / 10) + 0x30;
      TempCelsius_Display[12] = ((Temp_Decimal % 100) % 10) + 0x30;
      TempCelsius_Value >>= 3;
      TempCelsius_Display[6] = (TempCelsius_Value / 100) + 0x30;
      TempCelsius_Display[7] = ((TempCelsius_Value % 100) / 10) + 0x30;
      TempCelsius_Display[8] = ((TempCelsius_Value % 100) % 10) + 0x30;

      /* Convert temperature °C to Fahrenheit */
      Temp_Value_Fahrenheit = ((9 * ((TempCelsius_Value * 1000) + Temp_Decimal)) / 5) + 32000;

      /* Calculate temperature digits in °F */
      TempFahrenheit_Display[6] = (Temp_Value_Fahrenheit / 100000) + 0x30;
      TempFahrenheit_Display[7] = ((Temp_Value_Fahrenheit % 100000) /10000) + 0x30;
      TempFahrenheit_Display[8] = ((Temp_Value_Fahrenheit % 100000) %10000/1000) + 0x30;
      TempFahrenheit_Display[10] = ((((Temp_Value_Fahrenheit % 100000) %10000) %1000) /100) + 0x30;
      TempFahrenheit_Display[11] = (((((Temp_Value_Fahrenheit % 100000) %10000) %1000) %100) /10) + 0x30;
      TempFahrenheit_Display[12] = (((((Temp_Value_Fahrenheit % 100000) %10000) %1000) %100) %10) + 0x30;

      /* Display Fahrenheit value on LCD */
      for(i = 0; i < 20; i++)
      {
        LCD_DisplayChar(Line7, (319 - (16 * i)), TempCelsius_Display[i]);
        LCD_DisplayChar(Line8, (319 - (16 * i)), TempFahrenheit_Display[i]);
      }
    }
  }
  else
  {
    LCD_Clear(White);
    LCD_DisplayStringLine(Line2, " No TSENSOR Present ");
    LCD_DisplayStringLine(Line3, "Exit:  Push JoyStick");
    while(ReadKey() == NOKEY)
    {
    }
  }
  
  LCD_Clear(White);
  /* Display menu */
  DisplayMenu();
  /* Enable the JoyStick interrupt */
  IntExtOnOffConfig(ENABLE);   
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
