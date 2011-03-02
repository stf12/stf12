/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : tsensor.h
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : This file contains all the functions prototypes for the
*                      Temperature Sensor firmware driver.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TSENSOR_H
#define __TSENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define LM75_TEMP_Reg       0x00  /* Temperature Register of LM75 */
#define LM75_CONF_Reg       0x01  /* Configuration Register of LM75 */
#define LM75_THYS_Reg       0x02  /* Temperature Register of LM75 */
#define LM75_TOS_Reg        0x03  /* Over-temp Shutdown threshold Register of LM75 */

/* Exported types ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C_LM75_Init(void);
ErrorStatus I2C_LM75_Status(void);
uint16_t I2C_LM75_Temp_Read(void);
uint16_t I2C_LM75_Reg_Read(uint8_t RegName);
void I2C_LM75_Reg_Write(uint8_t RegName, uint16_t RegValue);
uint8_t I2C_LM75_ConfReg_Read(void);
void I2C_LM75_ConfReg_Write(uint8_t RegValue);
void I2C_LM75_ShutDown(FunctionalState NewState);
void Thermometer_Temperature(void);

#endif /* __TSENSOR_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
