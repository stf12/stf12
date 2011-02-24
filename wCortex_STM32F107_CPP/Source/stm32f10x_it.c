/**
  ******************************************************************************
  * @file    ADC/RegSimul_DualMode/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"


// extern void ISR_USARTxRx(int indexSerial, uint8_t recvd);
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  Questa funzione gestisce la richiesta di interrupt
  *         che arriva dal systick. Notare che nel main e'
  *         preimpostato l'intervallo di tempo con cui si
  *         succedono gli IRQ mandati dal Systick;
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void) {
//
//}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s). In weffetti e' li' che si trovano i nomi   */
/*  da assegnare alle ISR per l'handling degli IRQ...                         */
/******************************************************************************/


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

// NOTA: In tutti gli handler dobbiamo distinguere fra caso ricezione e caso trasmissione...
/**
  * @brief  This function handles USART1 interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void) {

    // Caso ricezione: se il bit RXNE e' settato, vuol dire che il buffer
    // in ricezione  della seriale contiene un carattere: memorizzo il carattere
    // nel buffer circolare della seriale coinvolta.
    /*if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        // Leggi il frame dal registro;
        // NOTA: Automaticamente il flag RXNE viene resettato non
        // appena i bit del frame vengono "flushati" dal registro
        // in cui sono contenuti;
        ISR_USARTxRicezione(1, USART_ReceiveData(USART1));
    }*/

    // Caso trasmissione: l'interrupt "TXE" e' stato attivato in
    // precedenza ed e' occorsa una richiesta di interrupt in
    // questo istante;
    /* if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
        Write one byte to the transmit data register */
        /*ISR_USARTxTrasmissione(1);
        // USART_SendData(USART1, TxBuffer1[TxCounter1++]);
    }*/
    //if(TxCounter1 == NbrOfDataToTransfer1)
    //  {
      /* Disable the USARTy Transmit interrupt */
    //    USART_ITConfig(USARTy, USART_IT_TXE, DISABLE);
    // }
    // }

}

/**
  * @brief  This function handles USART2 interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void) {

    /*if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        // Read one byte from the receive data register;
        ISR_USARTxRicezione(2, USART_ReceiveData(USART2));

    }*/

}

/**
  * @brief  This function handles USART3 interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void) {

    /*if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        // Read one byte from the receive data register;
        ISR_USARTxRicezione(3, USART_ReceiveData(USART3));
    }*/

}

/**
  * @brief  This function handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void) {

    /*if(UART_GetITStatus(UART4, USART_IT_RXNE) != RESET) {
        // Read one byte from the receive data register;
        ISR_USARTxRicezione(4, UART_ReceiveData(UART4));
    }*/

}


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
