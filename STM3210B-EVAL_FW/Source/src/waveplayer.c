/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : waveplayer.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : This file includes the wave player driver for the
*                      STM3210B-EVAL demo.
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
typedef enum
{
  LittleEndian,
  BigEndian
}Endianness;

/* Private define ------------------------------------------------------------*/
#define  ChunkID             0x52494646  /* correspond to the letters 'RIFF' */
#define  FileFormat          0x57415645  /* correspond to the letters 'WAVE' */
#define  FormatID            0x666D7420  /* correspond to the letters 'fmt ' */
#define  DataID              0x64617461  /* correspond to the letters 'data' */
#define  FactID              0x66616374  /* correspond to the letters 'fact' */

#define  WAVE_FORMAT_PCM     0x01
#define  FormatChunkSize     0x10
#define  Channel_Mono        0x01

#define  SampleRate_8000     8000
#define  SampleRate_11025    11025
#define  SampleRate_22050    22050
#define  SampleRate_44100    44100
#define  Bits_Per_Sample_8   8

#define Dummy_Byte 0xA5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static WAVE_FormatTypeDef WAVE_Format;
static __IO ErrorCode WaveFileStatus = Unvalid_RIFF_ID;
static __IO uint16_t TIM2ARRValue = 0x00;
static __IO uint32_t WaveDataLength;
static __IO uint32_t SpeechDataOffset = 0x00;
static uint32_t ReadAddr = 0x00, PauseReadAddr = 0x00;

/* Private function prototypes -----------------------------------------------*/
static uint32_t ReadUnit(uint8_t NbrOfBytes, Endianness BytesFormat);
static ErrorCode WavePlayer_WaveParsing(uint32_t ReadAddress);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : WavePlay_Init
* Description    : Wave player Initialization
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WavePlayer_Init(void)
{
  /* Peripherals InitStructure define */
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
  GPIO_InitTypeDef GPIOB_InitStructure;

  TIM_OCStructInit(&TIM_OCInitStructure);
  
  /* Read the Speech wave file status */
  WaveFileStatus = WavePlayer_WaveParsing(SpeechReadAddr);

  if(WaveFileStatus == Valid_WAVE_File) /* the .WAV file is valid */
  {
    /* Set WaveDataLenght to the Speech wave length */
    WaveDataLength = WAVE_Format.DataSize;
  }

  /* TIM4 and TIM2 configuration is used also for voice recorder application */

  /* Configure PB.08 as alternate function (TIM4_OC3) */
  GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIOB_InitStructure);

  /* TIM4 used for PWM genration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00; /* TIM4CLK = 72 MHz */
  TIM_TimeBaseStructure.TIM_Period = 0xFF;   /* PWM frequency : 281.250KHz*/
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  /* TIM4's Channel3 in PWM1 mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0x7F;  /* Duty cycle: 50%*/  
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* TIM2 used for timing, the timing period depends on wav file sample rate */
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00;    /* TIM2CLK = 72 MHz */
  TIM_TimeBaseStructure.TIM_Period = TIM2ARRValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  /* Output Compare Inactive Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0x0;
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
}

/*******************************************************************************
* Function Name  : Get_WaveFileStatus
* Description    : Returns the Wave file status.
* Input          : None
* Output         : None
* Return         : Wave file status.
*******************************************************************************/
ErrorCode Get_WaveFileStatus(void)
{
  return (WaveFileStatus);
}

/*******************************************************************************
* Function Name  : WavePlayer_Start
* Description    : Start wave playing
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WavePlayer_Start(void)
{
  /* Set WaveDataLenght to the Speech wave length */
  WaveDataLength = WAVE_Format.DataSize;
  
  TIM_SetAutoreload(TIM2, TIM2ARRValue);

  /* Start TIM4 */
  TIM_Cmd(TIM4, ENABLE);

  /* Start TIM2 */
  TIM_Cmd(TIM2, ENABLE);

  /* Set SPI Flash start read address to the Speech wave data */
  ReadAddr = SpeechReadAddr + SpeechDataOffset;
}

/*******************************************************************************
* Function Name  : WavePlayer_Stop
* Description    : Stop wave playing
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WavePlayer_Stop(void)
{
  /* Disable TIM2 update interrupt */
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
  /* Disable TIM4 */
  TIM_Cmd(TIM4, DISABLE);
  /* Disable TIM2 */
  TIM_Cmd(TIM2, DISABLE);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
* Function Name  : WavePlayer_RePlay
* Description    : Pause wave playing
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WavePlayer_RePlay(void)
{
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

  /* Enable TIM2 update interrupt */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  /* set Speech SPI flash wave address */
  SPI_FLASH_StartReadSequence(PauseReadAddr);
}

/*******************************************************************************
* Function Name  : WavePlayer_Pause
* Description    : Pause wave playing
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WavePlayer_Pause(void)
{
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

  /* Disable TIM2 update interrupt */
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  PauseReadAddr = WAVE_Format.DataSize - WaveDataLength;
  PauseReadAddr += ReadAddr;
}

/*******************************************************************************
* Function Name  : Decrement_WaveDataLength
* Description    : Decrements the played wave data length.
* Input          : None
* Output         : WaveDataLength: decrement the played wave data length.
* Return         : Current value of  WaveDataLength variable.
*******************************************************************************/
uint32_t Decrement_WaveDataLength(void)
{
  if (WaveDataLength != 0x00)
  {
    WaveDataLength--;
  }

  return (WaveDataLength);
}

/*******************************************************************************
* Function Name  : Set_WaveDataLength
* Description    : Decrements the played wave data length.
* Input          : None
* Output         : WaveDataLength: decrement the played wave data length.
* Return         : Current value of  WaveDataLength variable.
*******************************************************************************/
void Set_WaveDataLength(uint32_t value)
{
  WaveDataLength = value;
}

/*******************************************************************************
* Function Name  : WavePlayer_WaveParsing
* Description    : Checks the format of the .WAV file and gets information about
*                  the audio format. This is done by reading the value of a
*                  number of parameters stored in the file header and comparing
*                  these to the values expected authenticates the format of a
*                  standard .WAV  file (44 bytes will be read). If  it is a valid
*                  .WAV file format, it continues reading the header to determine
*                  the  audio format such as the sample rate and the sampled data
*                  size. If the audio format is supported by this application,
*                  it retrieves the audio format in WAVE_Format structure and
*                  returns a zero value. Otherwise the function fails and the
*                  return value is nonzero.In this case, the return value specifies
*                  the cause of  the function fails. The error codes that can be
*                  returned by this function are declared in the header file.
* Input          : None
* Output         : None
* Return         : Zero value if the function succeed, otherwise it return
*                  a nonzero value which specifies the error code.
*******************************************************************************/
static ErrorCode WavePlayer_WaveParsing(uint32_t ReadAddress)
{
  uint32_t index = 0;
  uint32_t Temp = 0x00;
  uint32_t ExtraFormatBytes = 0;

  /* Start a read data byte sequence from the Flash starting from @ReadAddress */
  SPI_FLASH_StartReadSequence(ReadAddress);
    
  /* Read chunkID, must be 'RIFF'  ----------------------------------------------*/
  Temp = ReadUnit(4, BigEndian);
  if(Temp != ChunkID)
  {
    return(Unvalid_RIFF_ID);
  }
    
  /* Read the file length ----------------------------------------------------*/
  WAVE_Format.RIFFchunksize = ReadUnit(4, LittleEndian);
  
  /* Read the file format, must be 'WAVE' ------------------------------------*/
  Temp = ReadUnit(4, BigEndian);
  if(Temp != FileFormat)
  {
    return(Unvalid_WAVE_Format);
  }
  
/* Read the format chunk, must be'fmt ' --------------------------------------*/
  Temp = ReadUnit(4, BigEndian);
  if(Temp != FormatID)
  {
    return(Unvalid_FormatChunk_ID);
  }

  /* Read the length of the 'fmt' data, must be 0x10 -------------------------*/
  Temp = ReadUnit(4, LittleEndian);
  if(Temp != 0x10)
  {
    ExtraFormatBytes = 1;
  }

  /* Read the audio format, must be 0x01 (PCM) -------------------------------*/
  WAVE_Format.FormatTag = ReadUnit(2, LittleEndian);
  if(WAVE_Format.FormatTag != WAVE_FORMAT_PCM)
  {
    return(Unsupporetd_FormatTag);
  }
  
  /* Read the number of channels, must be 0x01 (Mono) ------------------------*/
  WAVE_Format.NumChannels = ReadUnit(2, LittleEndian);
  if(WAVE_Format.NumChannels != Channel_Mono)
  {
    return(Unsupporetd_Number_Of_Channel);
  }
  
  /* Read the Sample Rate ----------------------------------------------------*/
  WAVE_Format.SampleRate = ReadUnit(4, LittleEndian);

  /* Update the OCA value according to the .WAV file Sample Rate */
  switch(WAVE_Format.SampleRate)
  {
    case SampleRate_8000 : TIM2ARRValue = 9000; break; /* 8KHz = 72MHz / 9000 */
    case SampleRate_11025: TIM2ARRValue = 6531; break; /* 11.025KHz = 72MHz / 6531 */
    case SampleRate_22050: TIM2ARRValue = 3265; break; /* 22.05KHz = 72MHz / 3265 */  
    case SampleRate_44100: TIM2ARRValue = 1633; break; /* 44.1KHz = 72MHz / 1633 */    
    default: return(Unsupporetd_Sample_Rate);
  }
  
  /* Read the Byte Rate ------------------------------------------------------*/
  WAVE_Format.ByteRate = ReadUnit(4, LittleEndian);
  
  /* Read the block alignment ------------------------------------------------*/
  WAVE_Format.BlockAlign = ReadUnit(2, LittleEndian);
  
  /* Read the number of bits per sample --------------------------------------*/
  WAVE_Format.BitsPerSample = ReadUnit(2, LittleEndian);
  if(WAVE_Format.BitsPerSample != Bits_Per_Sample_8)
  {
    return(Unsupporetd_Bits_Per_Sample);  
  }

  SpeechDataOffset = 36;

/* If there is Extra format bytes, these bytes will be defined in "Fact Chunk" */
  if(ExtraFormatBytes == 1)
  {
    /* Read th Extra format bytes, must be 0x00 ------------------------------*/
    Temp = ReadUnit(2, LittleEndian);
    if(Temp != 0x00)
    {
      return(Unsupporetd_ExtraFormatBytes);
    }

    /* Read the Fact chunk, must be 'fact' -----------------------------------*/
    Temp = ReadUnit(4, BigEndian);
    if(Temp != FactID)
    {
      return(Unvalid_FactChunk_ID);
    }

    /* Read Fact chunk data Size ---------------------------------------------*/
    Temp = ReadUnit(4, LittleEndian);
    
  SpeechDataOffset += 10 + Temp;

    /* Increment SPI Flash read address by Temp bytes by reading Temp dummy bytes */
    for(index = 0; index < Temp; index++)
    {
      SPI_FLASH_SendByte(Dummy_Byte);
    }
  }

/* Read the Data chunk, must be 'data' ---------------------------------------*/
  Temp = ReadUnit(4, BigEndian);

  if(Temp != DataID)
  {
    return(Unvalid_DataChunk_ID);
  }
    
  /* Read the number of sample data ------------------------------------------*/
  WAVE_Format.DataSize = ReadUnit(4, LittleEndian);

  SpeechDataOffset += 8;

  return(Valid_WAVE_File);
}

/*******************************************************************************
* Function Name  : ReadUnit
* Description    : Reads a number of bytes from the SPI Flash and reorder them
*                  in Big or little endian.
* Input          : - NbrOfBytes : number of bytes to read.
*                    This parameter must be a number between 1 and 4.
*                  - ReadAddr : external memory address to read from.
*                  - Endians : specifies the bytes endianness.
*                    This parameter can be one of the following values:
*                          - LittleEndian
*                          - BigEndian
* Output         : None
* Return         : Bytes read from the SPI Flash.
*******************************************************************************/
static uint32_t ReadUnit(uint8_t NbrOfBytes, Endianness BytesFormat)
{
  uint32_t index = 0;
  uint32_t Temp = 0;

  for(index = 0; index < NbrOfBytes; index++)
  {
    Temp |= SPI_FLASH_ReadByte() << (index * 8);
  }

  if (BytesFormat == BigEndian)
  {
     Temp = __REV(Temp);
  }

  return Temp;
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
