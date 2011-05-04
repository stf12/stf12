/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : memory.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : Memory management layer
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "msd.h"
#include "memory.h"
#include "usb_scsi.h"
#include "usb_bot.h"
#include "usb_regs.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t Memory_Offset;                   /* Memory Offset */
uint32_t Transfer_Length;                  /* Transfer Length */
__IO uint32_t Block_Read_count = 0;
__IO uint32_t Block_offset;
__IO uint32_t Counter = 0;
uint32_t i;
uint8_t Data_Buffer[BULK_MAX_PACKET_SIZE * 8]; /* 512 bytes*/

/* Extern variables ----------------------------------------------------------*/
extern uint8_t Bulk_Data_Buff[BULK_MAX_PACKET_SIZE];  /* data buffer*/
extern uint16_t Data_Len;
extern uint8_t Bot_State;
extern Bulk_Only_CBW CBW;
extern Bulk_Only_CSW CSW;
extern uint32_t Mass_Memory_Size;
extern uint32_t Mass_Block_Size;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Read_Memory
* Description    : Handle the Read operation from the microSD card. 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Read_Memory(void)
{
  if (!Block_Read_count)
  {
    MSD_ReadBlock(Data_Buffer, Memory_Offset, 512);
    UserToPMABufferCopy(Data_Buffer, ENDP1_TXADDR, BULK_MAX_PACKET_SIZE);
    Block_Read_count = 512 - BULK_MAX_PACKET_SIZE;
    Block_offset = BULK_MAX_PACKET_SIZE;
  }
  else
  {
    UserToPMABufferCopy(Data_Buffer + Block_offset, ENDP1_TXADDR, BULK_MAX_PACKET_SIZE);
    Block_Read_count -= BULK_MAX_PACKET_SIZE;
    Block_offset += BULK_MAX_PACKET_SIZE;
  }

  SetEPTxCount(ENDP1, BULK_MAX_PACKET_SIZE);
  SetEPTxStatus(ENDP1, EP_TX_VALID);


  Memory_Offset += BULK_MAX_PACKET_SIZE;
  Transfer_Length -= BULK_MAX_PACKET_SIZE;

  CSW.dDataResidue -= BULK_MAX_PACKET_SIZE;

  if (Transfer_Length == 0)
  {
    Block_Read_count = 0;
    Block_offset = 0;
    Memory_Offset = 0;
    Bot_State = BOT_DATA_IN_LAST;
  }
}

/*******************************************************************************
* Function Name  : Write_Memory
* Description    : Handle the Write operation to the microSD card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Write_Memory(void)
{
  uint32_t temp =  Counter + 64;


  i = 0;
  for (; Counter < temp; Counter++)
  {
    Data_Buffer[Counter] = Bulk_Data_Buff[i];
    i++;
  }

  Memory_Offset += Data_Len;
  Transfer_Length -= Data_Len;

  if (!(Transfer_Length % 512))
  {
    Counter = 0;
    MSD_WriteBlock(Data_Buffer, Memory_Offset - 512, 512);
  }

  CSW.dDataResidue -= Data_Len;
  SetEPRxStatus(ENDP2, EP_RX_VALID); /* enable the next transaction*/


  if ((Transfer_Length == 0) || (Bot_State == BOT_CSW_Send))
  {
    Counter = 0;
    Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
  }
}

/*******************************************************************************
* Function Name  : Address_Management_Test
* Description    : Test the received address. 
* Input          : uint8_t Cmd : the command can be SCSI_READ10 or SCSI_WRITE10.
* Output         : None.
* Return         : Read\Write status (bool).
*******************************************************************************/
bool Address_Management_Test(uint8_t Cmd)
{
  __IO uint32_t temp1;
  __IO uint32_t temp2;

  /* Logical Block Address of First Block */
  temp1 = (CBW.CB[2] << 24) |
          (CBW.CB[3] << 16) |
          (CBW.CB[4] <<  8) |
          (CBW.CB[5] <<  0);
  /* Number of Blocks to transfer */
  temp2 = (CBW.CB[7] <<  8) |
          (CBW.CB[8] <<  0);

  Memory_Offset = temp1 * Mass_Block_Size;
  Transfer_Length = temp2 * Mass_Block_Size;

  if ((Memory_Offset + Transfer_Length) > Mass_Memory_Size)
  {
    if (Cmd == SCSI_WRITE10)
    {
      Bot_Abort(BOTH_DIR);
    }
    Bot_Abort(DIR_IN);
    Set_Scsi_Sense_Data(ILLEGAL_REQUEST, ADDRESS_OUT_OF_RANGE);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
    return (FALSE);
  }


  if (CBW.dDataLength != Transfer_Length)
  {
    if (Cmd == SCSI_WRITE10)
    {
      Bot_Abort(BOTH_DIR);
    }
    else
    {
      Bot_Abort(DIR_IN);
    }
    Set_Scsi_Sense_Data(ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
    return (FALSE);
  }
  return (TRUE);
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/




