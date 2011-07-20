/**
  * @file    LPS001DL_I2C.c
  * @author  ART Team IMS-Systems Lab
  * @version V2.2
  * @date    01/11/2011
  * @brief   This file provides a set of functions needed to manage the
  *          communication between STM32 I2C master and LPS001DL I2C slave.
  * @details
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
  * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
  * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */



/* Includes ------------------------------------------------------------------*/
#include "LPS001DL_I2C.h"
#include "HAL_LPS001DL.h"
#include "math.h"
#include "iNEMO_conf.h"


/**
 * @addtogroup LPS001DL
 * @{
 */


/**
* @defgroup LPS001DL_function
* @{
*/

/**
* \brief Initializes the I2C peripheral used to drive the LPS001DL
* \param  None
* \retval  None
*/
void LPS001DL_I2C_Init(void)
{
  I2C_InitTypeDef  I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable I2C and GPIO clocks */
  RCC_APB1PeriphClockCmd(PRESS_RCC_Periph_I2C, ENABLE);
  RCC_APB2PeriphClockCmd(PRESS_RCC_Port_I2C, ENABLE);

  /* Configure I2C pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  PRESS_I2C_SCL_Pin | PRESS_I2C_SDA_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(PRESS_I2C_Port, &GPIO_InitStructure);

  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = LPS001DL_ADDRESS;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = PRESS_I2C_Speed;

    /* Apply I2C configuration after enabling it */
  I2C_Init(PRESS_I2C, &I2C_InitStructure);

  /* I2C Peripheral Enable */
  I2C_Cmd(PRESS_I2C, ENABLE);

}


#ifdef INT1_PAD_ENABLE
/**
* \brief  Initializes the INT1 pin interrupt
* \param None
* \retval None
*/
void LPS001DL_INT1_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable INT1 pad GPIO clocks */
  RCC_APB2PeriphClockCmd(LPS001DL_INT1_RCC, ENABLE);

  /* Configure INT1 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = LPS001DL_INT1_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(LPS001DL_INT1_Port, &GPIO_InitStructure);

  /* Connect LPS001DL_INT1_EXTI_Line to LPS001DL_INT1_Pin */
  GPIO_EXTILineConfig(LPS001DL_INT1_Port_Source, LPS001DL_INT1_Pin_Source);

  /* Configure LPS001DL_INT1_EXTI_Line to generate an interrupt on LPS001DL_INT1_Edge edge */
  EXTI_InitStructure.EXTI_Line = LPS001DL_INT1_EXTI_Line;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = LPS001DL_INT1_Edge;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable the LPS001DL_INT1_EXTI_IRQCHANNEL Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = LPS001DL_INT1_EXTI_IRQCHANNEL;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LPS001DL_INT1_Preemption_Priority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = LPS001DL_INT1_Sub_Priority;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
#endif




/**
* \brief Set configuration of LPS001DL
* \param LPS001DL_Config_Struct : pointer to a LPS001DL_ConfigTypeDef structure that contains the configuration setting for the LPS001DL.
* \retval None
* Return         : None
*/
void LPS001DL_Config(LPS001DL_ConfigTypeDef *LPS001DL_Config_Struct)
{
  u8 tmpreg = 0x00;

  tmpreg |= (u8) (LPS001DL_Config_Struct->Lowpower | LPS001DL_Config_Struct->Output_Data_Rate
                  | LPS001DL_Config_Struct->Data_Update | LPS001DL_Config_Struct->Endianess);

  LPS001DL_Write(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
}

/**
* \brief  Set interrupt configuration of LPS001DL
* \param LPS001DL_Interrupt_Struct : pointer to a LPS001DL_InterruptTypeDef structure that contains the interrupt configuration setting for the LPS001DL.
* \retval None
*/

void LPS001DL_Interrupt_Config(LPS001DL_InterruptTypeDef *LPS001DL_Interrupt_Struct)
{
  u8 tmpreg1 = 0x00;
  u8 tmpreg2 = 0x00;

  tmpreg1 |= (u8)(LPS001DL_Interrupt_Struct->Mode | LPS001DL_Interrupt_Struct->PP_OD |
                  ((LPS001DL_Interrupt_Struct->Int2_Pad_Mode)<<3) | LPS001DL_Interrupt_Struct->Int1_Pad_Mode);

  LPS001DL_Write(&tmpreg1, LPS001DL_CTRL_REG3_ADDR, 1);

  tmpreg2 |= (u8)(LPS001DL_Interrupt_Struct->Latch | LPS001DL_Interrupt_Struct->Delta_Press_Low |
                  LPS001DL_Interrupt_Struct->Delta_Press_High);


  LPS001DL_Write(&tmpreg2, LPS001DL_INTERRUPT_CONFIG_ADDR, 1);



}

/**
* \brief Enables or disables the LPS001DL interrupts
* \param NewState : new state for the LPS001DL interrupt circuit.This parameter can be: ENABLE or DISABLE.
* \retval None
*/
void LPS001DL_Interrupt_Cmd(FunctionalState NewState)
{
  u8 tmpreg = 0x00;
  LPS001DL_Read(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
  if(NewState==ENABLE)
    tmpreg |= 0x08;
  else
    tmpreg &= 0xF7;
  LPS001DL_Write(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
}


/**
* \brief  Enables or disables the lowpower mode for LPS001DL
* \param NewState: new state for the lowpower mode. This parameter can be: ENABLE or DISABLE.
* \retval None
*/
void LPS001DL_Lowpower_Cmd(FunctionalState NewState)
{
  u8 tmpreg = 0x00;
  LPS001DL_Read(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
  if(NewState==ENABLE)
    tmpreg |= 0x80;
  else
    tmpreg &= 0x7F;
  LPS001DL_Write(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
}


/**
* \brief  Enables or disables the power-down mode for LPS001DL
* \param  NewState: new state for the power-down mode.  This parameter can be: ENABLE or DISABLE.
* \retval None
*/
void LPS001DL_PowerDown_Cmd(FunctionalState NewState)
{
  u8 tmpreg = 0x00;
  LPS001DL_Read(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
  if(NewState==DISABLE)
    tmpreg |= 0x40;
  else
    tmpreg &= 0xBF;
  LPS001DL_Write(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
}


/**
* \brief Reboot memory content of LPS001DL
* \param None
* \retval None
*/
void LPS001DL_Reboot_Cmd(void)
{
  u8 tmpreg = 0x80;
  LPS001DL_Write(&tmpreg, LPS001DL_CTRL_REG2_ADDR, 1);
}



/**
* \brief Read pressure data
* \param pBuffer : pointer to the buffer that receives the pressure
*                  data read from the LPS001DL. It will give in pBuffer the
*                  PRESS_OUT_L byte and in pBuffer++ the PRESS_OUT_H byte. It is
*                  up to the user the correct interpretation of the data according
*                  to the defined endianess
* \retval  None
*/
void LPS001DL_Read_Pressure_Regs(u8* pBuffer)
{
  LPS001DL_Read(pBuffer, LPS001DL_PRESS_OUT_L_ADDR, 2);
}


/**
* \brief Read temperature data
* \param  pBuffer : pointer to the buffer that receives the temperature data read
*                  from the LPS001DL. It will give in pBuffer the TEMP_OUT_L byte and in
*                 pBuffer++ the TEMP_OUT_H byte. It is up to the user the correct interpretation
*                 of the data according to the defined endianess. Moreover temperature data are
*                 expressed as 2's complement numbers.
* \retval None
*/
void LPS001DL_Read_Temperature_Regs(u8* pBuffer)
{
  LPS001DL_Read(pBuffer, LPS001DL_TEMP_OUT_L_ADDR, 2);
}


/**
* \brief Calculate the Altitude according to the altitude-pressure
*                  conversion formula: z[m]=H_p * ln(P_0/P)
* \param  altitude: value of the altitude expressed in meter
* \retval None
*/
void LPS001DL_Get_Altitude(u16* altitude)
{
  u8 temp[2]={0,0};
  u16 press_temp = 0;
  u16 press_value = 0;
  LPS001DL_Read_Pressure_Regs(&temp[0]);
  press_temp = (((u16)temp[1])<<8) + ((u16)(temp[0]));
  press_value = (u16)(press_temp/P_Conversion);
  *altitude = (u16) (H_p)*(log(P_0/press_value));
}

/**
* \brief Read the pressure data
* \param pressure: value of the pressure expressed in LSB
* \retval None
*/
void LPS001DL_Get_Raw_Pressure(u16* pressure)
{
  u8 temp[2]={0,0};
  u16 press_temp = 0;
  u8 tmpreg = 0x00;
  LPS001DL_Read(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
  LPS001DL_Read(&temp[0], LPS001DL_PRESS_OUT_L_ADDR, 2);
  if((tmpreg&0x02)==0x00) //Little Endian Mode
  {
    press_temp = (((u16)temp[1])<<8) + ((u16)(temp[0]));
    *pressure = (u16)(press_temp);
  }
  else //Big Endian Mode
  {
    press_temp = (((u16)temp[0])<<8) + ((u16)(temp[1]));
    *pressure = (u16)(press_temp);
  }

}


/**
* \brief Read the pressure data
* \param pressure: value of the pressure expressed in dmbar
* \retval None
*/
void LPS001DL_Get_Pressure(u16* pressure)
{
  u8 temp[2]={0,0};
  u16 press_temp = 0;
  u8 tmpreg = 0x00;
  LPS001DL_Read(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
  LPS001DL_Read(&temp[0], LPS001DL_PRESS_OUT_L_ADDR, 2);
  if((tmpreg&0x02)==0x00) //Little Endian Mode
  {
    press_temp = (((u16)temp[1])<<8) + ((u16)(temp[0]));
    *pressure = (u16)(press_temp/P_Conversion);
  }
  else //Big Endian Mode
  {
    press_temp = (((u16)temp[0])<<8) + ((u16)(temp[1]));
    *pressure = (u16)(press_temp/P_Conversion);
  }

}


/**
* \brief Read the temperature data
* \param temperature: Temperature data expressed as
*                 2's complement numbers and in tenth of °C
* \retval None
*/
void LPS001DL_Get_Temperature(s16* temperature)
{
  u8 temp[2]={0,0};
  u16 temp_value = 0;
  u8 tmpreg = 0x00;
  LPS001DL_Read(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
  LPS001DL_Read_Temperature_Regs(&temp[0]);
  if((tmpreg&0x02)==0x00) //Little Endian Mode
  {
    temp_value = ((((u16)temp[1])<<8) + ((u16)(temp[0])));
    *temperature = (s16)(temp_value*T_Conversion);
  }
  else //Big Endian Mode
  {
    temp_value = ((((u16)temp[0])<<8) + ((u16)(temp[1])));
    *temperature = (s16)(temp_value*T_Conversion);
  }

}


/**
* /brief  Set pressure reference for delta pressure computing
* /param  refvalue : pressure reference value expressed in dmbar
* /retval None
*/
void LPS001DL_Set_Reference(u16 refvalue)
{
  u16 temp;
  u8 tempbuffer[2];
  u8 tmpreg = 0x00;
  LPS001DL_Read(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);
  temp = (u16) (refvalue/P_Conversion);
  if((tmpreg&0x02)==0x00)
  {
    tempbuffer[0] = (u8) (temp); //low part
    tempbuffer[1] = (u8) (temp>>8); //high part
    LPS001DL_Write(&tempbuffer[0], LPS001DL_REF_P_L_ADDR, 2);
  }
  else
  {
    tempbuffer[1] = (u8) (temp); //low part
    tempbuffer[0] = (u8) (temp>>8); //high part
    LPS001DL_Write(&tempbuffer[0], LPS001DL_REF_P_L_ADDR, 2);
  }


}


/**
* \brief Set pressure threshold for delta pressure computing
* \param refvalue : absolute pressure threshold value as delta value expressed in dmbar
* \retval None
*/
void LPS001DL_Set_Threshold(u16 refvalue)
{
  u8 tempbuffer[2];
  u16 temp;
  u8 tmpreg = 0x00;
  temp = (u16) (refvalue/P_Conversion);
  LPS001DL_Read(&tmpreg, LPS001DL_CTRL_REG1_ADDR, 1);

  if((tmpreg&0x02)==0x00)
  {
    tempbuffer[0] = (u8) (temp); //low part
    tempbuffer[1] = (u8) (temp>>8); //high part
    LPS001DL_Write(&tempbuffer[0], LPS001DL_THS_P_L_ADDR, 2);
  }
  else
  {
    tempbuffer[1] = (u8) (temp); //low part
    tempbuffer[0] = (u8) (temp>>8); //high part
    LPS001DL_Write(&tempbuffer[0], LPS001DL_THS_P_L_ADDR, 2);
  }

}


/**
* \brief Writes one or more byte to the I2C LPS001DL.
* \param pBuffer : pointer to the buffer  containing the data to be written to the LPS001DL.
* \param WriteAddr : LPS001DL's internal address to write to.
* \param NumByteToWrite: Number of bytes to write
* \retval None
*/
void LPS001DL_Write(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
  iNEMO_ENTER_CRITICAL();  
  /* Send STRAT condition */
  I2C_GenerateSTART(PRESS_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(PRESS_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send LPS001DL address for write */
  I2C_Send7bitAddress(PRESS_I2C, LPS001DL_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(PRESS_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  if(NumByteToWrite>1)
  {
    WriteAddr += 0x80; //the MSB bit of address is set to 1 to allow incremental address
  }

  /* Send the LPS001DL's internal address to write to */
  I2C_SendData(PRESS_I2C, WriteAddr);

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(PRESS_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));


  while(NumByteToWrite)
  {
    /* Send the byte to be written */
    I2C_SendData(PRESS_I2C, *pBuffer);

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(PRESS_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    pBuffer++;
    NumByteToWrite--;
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(PRESS_I2C, ENABLE);
  iNEMO_EXIT_CRITICAL();  

}



/**
* \brief Reads a block of data from the LPS001DL.
* \param pBuffer : pointer to the buffer that receives the data read from the LPS001DL.
* \param ReadAddr : LPS001DL's internal address to read from.
* \param NumByteToRead : number of bytes to read from the LPS001DL.
* retval None
*/
void LPS001DL_Read(u8* pBuffer, u8 ReadAddr, u8 NumByteToRead)
{
  iNEMO_ENTER_CRITICAL();
    /* While the bus is busy */
  while(I2C_GetFlagStatus(PRESS_I2C, I2C_FLAG_BUSY));

  /* Send START condition */
  I2C_GenerateSTART(PRESS_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(PRESS_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send LPS001DL address for write */
  I2C_Send7bitAddress(PRESS_I2C, LPS001DL_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(PRESS_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(PRESS_I2C, ENABLE);

  if(NumByteToRead>1)
  {
    ReadAddr += 0x80; //the MSB bit of address is set to 1 to allow incremental address
  }

  /* Send the LPS001DL's internal address to write to */
  I2C_SendData(PRESS_I2C, ReadAddr);

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(PRESS_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send START condition a second time */
  I2C_GenerateSTART(PRESS_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(PRESS_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send LPS001DL address for read */
  I2C_Send7bitAddress(PRESS_I2C, LPS001DL_ADDRESS, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(PRESS_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  /* While there is data to be read */
  while(NumByteToRead)
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(PRESS_I2C, DISABLE);

      /* Send STOP Condition */
      I2C_GenerateSTOP(PRESS_I2C, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(PRESS_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
      /* Read a byte from the LPS001DL */
      *pBuffer = I2C_ReceiveData(PRESS_I2C);

      /* Point to the next location where the byte read will be saved */
      pBuffer++;

      /* Decrement the read bytes counter */
      NumByteToRead--;
    }
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(PRESS_I2C, ENABLE);
  iNEMO_EXIT_CRITICAL();  
}

/**
 * @}
 */ /* end of group LPS001DL_function */ 

/**
 * @}
 */ /* end of group LPS001DL */ 

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
