/**
  * @file    virtual_com.c 
  * @author  MCD Application Team & SystemsLab
  * @version V3.2.1
  * @date    09/20/2010
  * @brief   iNEMO Virtual Com Setup & API.
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
#include "stm32f10x_it.h"
#include "virtual_com.h"
#include <stdarg.h>

/** @defgroup iNEMO_Virtual_Com
  * @{
  */

/** @defgroup iNEMO_Virtual_Com_API
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile FLASH_Status FLASHStatus;
u8 buffer_in[VIRTUAL_COM_PORT_DATA_SIZE];

/* buffer used for writing inside the hyperterminal */
u8 serial_usb_buffer_in[SERIAL_USB_BUFFER_IN_SIZE];
u8 count_char;

/* Extern variables ----------------------------------------------------------*/
extern u32 count_in;
extern LINE_CODING linecoding;

/* Private function prototypes -----------------------------------------------*/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure USB_DISCONNECT Pin
  * @param  None
  * @retval None
  */
void iNEMO_USB_Disconnet_Pin_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable "DISCONNECT" GPIO clock */
  RCC_APB2PeriphClockCmd(iNEMO_RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

  /* Configure USB pull-up */
  GPIO_InitStructure.GPIO_Pin = iNEMO_USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(iNEMO_USB_DISCONNECT, &GPIO_InitStructure);

  USB_Cable_Config(DISABLE);
}

/**
  * @brief  Software Connection/Disconnection of USB Cable
  * @param  NewState: new state 
  *   This parameter can be one of following parameters:
  *     @arg ENABLE
  *     @arg DISABLE
  * @retval None
  */
void USB_Cable_Config (FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    GPIO_SetBits(iNEMO_USB_DISCONNECT, iNEMO_USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_ResetBits(iNEMO_USB_DISCONNECT, iNEMO_USB_DISCONNECT_PIN);
  }

}

/**
  * @brief  Reads the Option Byte to enter DFU in Software Mode
  * @param  None
  * @retval Option Byte Value
  */
uint8_t Option_Byte_Read (void)
{

  return (uint8_t) (*(__IO uint32_t*) Option_address);
}


/**
  * @brief  Reset the device
  * @param  None
  * @retval None
  */
void Reset_Device(void)
{
  USB_Cable_Config(DISABLE);
  NVIC_SystemReset();
}


/**
  * @brief  Program the Option Byte to Enter DFU in Software Mode
  * @param  none.
  * @retval None
  */
void Enter_DFU_SW_Mode(void)
{
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  FLASHStatus=FLASH_EraseOptionBytes();  
  FLASHStatus=FLASH_ProgramOptionByteData(Option_address, 0x01);
  FLASH_Lock();
}


/**
  * @brief  Configures USB Clock input (48MHz).
  * @param  None.
  * @retval None
  */

void Set_USBClock(void)
{
  /* USBCLK = PLLCLK / 1.5 */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}


/**
  * @brief  Power-off system clocks and power while entering suspend mode.
  * @param  None.
  * @retval None
  */
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}


/**
  * @brief  Restores system clocks and power while exiting suspend mode.
  * @param  None.
  * @retval None
  */

void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}


/**
  * @brief  Configures the USB interrupts.
  * @param  None.
  * @retval None
  */

void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;


  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  Create the serial number string descriptor.
  * @param  None.
  * @retval None
  */

void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToUnicode (Device_Serial0, &Virtual_Com_Port_StringSerial[2] , 8);
    IntToUnicode (Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
  }
}


/**
  * @brief  Write 1 character on the the serial peripheral.
  * @param  c: character to be written.
  * @retval None
  */

void Virtual_Com_Write_Char(char c)
{
  /* Store the data into the buffer to be sent to the virtual com */
  serial_usb_buffer_in[count_char++] = c;
  
}/* end SERIAL_Write_Char() */



/**
  * @brief  Write n characters on the serial peripheral.
  * @param  char *s: string to be written.
  * @retval None
  */

void Virtual_Com_Write_String(char *s)
{
   while (*s != '\0')
   {
     Virtual_Com_Write_Char(*s);
     s++;
   }
}/* end SERIAL_Write_String() */


/**
  * @brief  Write a decimal value (local function).
  * @param  i: data to be written.
  * @retval None
  */

static void putint(unsigned int i)
{
	unsigned char mod=i%10;
	Virtual_Com_Write_Char(0x30+mod);
}/* end putint() */



/**
  * @brief  Write n characters on the serial peripheral.
  * @param  char *str: data to be written.
  * @retval None
  */

void Virtual_Com_Printf(const char *str,...)
{
   u32 arg;
   va_list ap;
  count_char =0;

   va_start(ap, str);
 
   while (*str) 
   {  
     if (*str == 92)    // backslash (\)
     {
       str++;
       if (*str == 'n')
         Virtual_Com_Write_Char('\n');
       else
       {
         Virtual_Com_Write_Char(92); // backslash (\)
         Virtual_Com_Write_Char(*str);
       }  
     } 
     else { /* else 1 */
      if (*str != '%')
         Virtual_Com_Write_Char(*str);           
      else /* else 2 */
      { 
         u32 arg1;
         int k;
         u32 pow = 1;
         str++;  
         switch (*str)
         {        
            case 'd': /* decimal */
               arg = va_arg(ap, u32);
                
               if (arg & 0x80000000) /* negative */
               {
                  Virtual_Com_Write_Char('-');
                  arg = (u32) (- ((s32) arg));
               }     
               while ((u32)(arg/(pow*10)))
               pow*=10;
               do{  
                  putint(arg/pow);
                  pow/=10;
               } while (pow >= 1);  
            break;	
          
            case 'c': /* char */
               arg = va_arg(ap, u32);				 
               Virtual_Com_Write_Char(arg);
            break;
            case 'x': /* exadecimal */
            case 'X':
               arg = va_arg(ap, u32);
               arg1 = arg;
               arg1 = ((arg >> 4) & 0x0000000F);
               if (arg1 < 10)
                  putint(arg1);
               else  
                  Virtual_Com_Write_Char( arg1 - 10 + 65 );
               arg1 = (arg & 0x0000000F);
               if (arg1 < 10)
                  putint(arg1);
               else  
                  Virtual_Com_Write_Char( arg1 - 10 + 65 );
            break;            
            case '2':
               str++;
               if ((*str == 'x') || (*str == 'X'))
               {
                  arg = va_arg(ap, u32);					 
                  arg1 = arg;
                  for (k=12; k>=0 ;k-=4)
                  {
                     arg1 = ((arg >> k) & 0x0000000F);
                     if (arg1 < 10)
                        putint(arg1);
                     else  
                        Virtual_Com_Write_Char( arg1 - 10 + 65 );
                  }
               }
             break;           
            case '4':
               str++;
               if ((*str == 'x') || (*str == 'X'))
               {
                  arg = va_arg(ap, u32); // u32 original
                  arg1 = arg;
					
                  for (k=28; k>=0 ;k-=4)
                  {
                     arg1 = ((arg >> k) & 0x0000000F);
                     if (arg1 < 10)
                        putint(arg1);
                     else  
                     Virtual_Com_Write_Char( arg1 - 10 + 65 );
                  }
               }                    
            break;                 
            default:
            break ;
         }/* end switch */
      }/* end else 2*/
     }/* end else 1 */
    str++;   
   }/* end while() */

   /* Write the USB serial buffer if USB is used */
   Virtual_Com_Write_Buffer(&serial_usb_buffer_in[0], count_char);

   va_end(ap); 
}/* end SERIAL_Printf() */




/**
  * @brief  Transmit through the virtual com tha data.
  * @param  data_buffer: the buffer that contains the bytes to transmit.
  * @param  Nb_bytes: the number of bytes to transmit.
  * @retval None
  */

void Virtual_Com_Write_Buffer(u8* data_buffer, u8 Nb_bytes)
{
  /* The USB cable is plugged into the USB-Mini connector */
  u8 b_size = Nb_bytes;
  u8 b_index = 0;
  u8 actual_input_size;
  
  while (b_index < Nb_bytes)
  {
    if (b_size < VIRTUAL_COM_PORT_DATA_SIZE)
    {
      actual_input_size = b_size;
    } 
    else
    {
      actual_input_size = VIRTUAL_COM_PORT_DATA_SIZE;
      
    }
    
    /* wait until the data transmission is finished */
    while (GetEPTxStatus(ENDP1) == EP_TX_VALID); //0x30
    /* Write data to the virtual UART through USB port */
    UserToPMABufferCopy(&(data_buffer[b_index]), ENDP1_TXADDR,actual_input_size);
    SetEPTxCount(ENDP1,actual_input_size); 
    SetEPTxValid(ENDP1);
    
    b_size = b_size - actual_input_size;
    b_index= b_index + actual_input_size; 
  }  

}

/**
  * @brief  Convert Hex 32Bits value into char
  * @param  none.
  * @retval None
  */
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}

/**
  * @}
  */ /* end of group iNEMO_Virtual_Com_API */

/**
  * @}
  */ /* end of group iNEMO_Virtual_Com */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
