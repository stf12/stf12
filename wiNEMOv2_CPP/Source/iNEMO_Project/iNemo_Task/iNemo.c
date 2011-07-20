/**
  * @file    iNemo.c 
  * @author  ART Team IMS-Systems Lab
  * @version V2.2
  * @date    01/11/2011
  * @brief   This file define iNEMO Task.
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


#include"iNemo.h"
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "GUI_layer.h"
#include "utils.h"
#include "iNEMO_lib.h"

extern u32 count_out;
extern u8 buffer_rx[VIRTUAL_COM_PORT_DATA_SIZE];

/**
 * Pointer to iNemoData shared beetwen DT and GT.
 * This resouce is not protected because all tasks have to modify it in a
 * mutual exclusive way.
 */
//static iNEMO_DATA *s_pSharedData = NULL;

/**
 * @ addtogroup iNEMO_task
 * @{
 */

/**
 * This resource is used to synchronize the USB IRQ and the iNemo Command Manager task.
 */
//static xQueueHandle s_usbQueue;

/**
 * This resource is used to synchronize the TIM2 IRQ and the iNemo data acquisition task.
 */
//static xSemaphoreHandle s_timSemaphore;

/* Forward declaration */
static void inCommandTaskFunction(void *pvParameters);
static void inDataTaskFunction(void *pvParameters);
static void Timer_Config(void);

/**
 * @addtogroup iNEMO_task_function
 * @{
 */

/**
 * @brief Implement a generic interface for an user defined task.
 * @details It is called by the prvSetupHardware function to setup all hardware resource used by the task.
 * @param pvParameters :  not used
 */
void inInitTask(void *pvParameters)
{
	iNEMO_HW_Config();

	while(bDeviceState != CONFIGURED);

	iNEMO_Config();
	Timer_Config();
}

/**
 * @brief Implement a generic interface for an user defined task.
 * @details It is called by the main to allocate the task's software resources.
 * @param pvParameters not used.
 */
//void inStartTask(void *pvParameters)
//{
//	// Create the queue used to synchronize iNemo Command Manager task end USB interrupt service routine.
//	s_usbQueue = xQueueCreate(1, sizeof(u32));
//	if (!s_usbQueue) {
//		// Error in resource creation.
//		while (1);
//	}
//
//	// Create the semaphore used to synchronize  the iNemo data acquisition task and the TIM2 interrupt service routine.
//	vSemaphoreCreateBinary(s_timSemaphore);
//	if (!s_timSemaphore) {
//		// Error in resource creation.
//		while (1);
//	}
//        xSemaphoreTake(s_timSemaphore, 0);
//
//	// Create the iNemo Command task
//	if ( xTaskCreate(inCommandTaskFunction, "iNemoCmd", IN_TASK_STACK_SIZE, NULL, IN_TASK_PRIORITY+1, NULL) != pdPASS ) {
//		// Error in task creation
//		while (1);
//	}
//
//	// Create the iNemo Data stream task
//	if ( xTaskCreate(inDataTaskFunction, "iNemoData", IN_TASK_STACK_SIZE, NULL, IN_TASK_PRIORITY, NULL) != pdPASS ) {
//		// Error in task creation
//		while (1);
//	}
//}

/**
 * @brief iNemo Command Manager task control function.
 * @details This task waits for a frame coming from the USB peripheral.
 * @param pvParameters not used.
 */
//void inCommandTaskFunction(void *pvParameters) {
//	u32 _count_out = 0;
//
//	for (;;) {
//		if ( xQueueReceive(s_usbQueue, &_count_out, portMAX_DELAY ) == pdTRUE ) {
//			ParseGuiFrame(_count_out, s_pSharedData);
//		}
//	}
//}

/**
 * @brief iNemo data acquisition/send task control function.
 * @details This task is synchronized with the  Timer2 interrupt. It reads all sensor data, packages the data according to the frame
 * format and sends the data to the PC over the USB connection.
 *
 * @param pvParameters not used.
 */
//void inDataTaskFunction(void *pvParameters)
//{
//
//        iNEMO_DATA data;
//
//        iNEMO_Data_Init(&data);
//
//        s_pSharedData = &data;
//#ifdef AHRS_MOD
//	iNEMO_AHRS_Init(&data.m_sensorData, &data.m_angle, &data.m_quat);
//#endif
//
//	for (;;) {
//		if ( xSemaphoreTake(s_timSemaphore, portMAX_DELAY) == pdTRUE ) {
//
//                    DataProcess(GetOutMode(),&data);
//		}
//	}
//}

/**
 * @brief Configures the timer 2
 */
void Timer_Config(void)
{
  unsigned short a;
  unsigned short b;
  unsigned long n;
  unsigned char frequency = 50; //This value is the frequency interrupts in Hz
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  // Enable timer clocks
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
  
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  
  TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );

  // Time base configuration for timer 2 - which generates the interrupts.
  n = configCPU_CLOCK_HZ/frequency;

  prvFindFactors( n, &a, &b );
  TIM_TimeBaseStructure.TIM_Period = b - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = a - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure );
  TIM_ARRPreloadConfig( TIM2, ENABLE );

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
   	
}

/**
 * @brief This function handles USB Low Priority or CAN RX0 interrupts requests.
 */
//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//#ifdef _VCOM
//	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
//
//	USB_Istr();
//	if (count_out) {
//		xQueueSendFromISR(s_usbQueue, &count_out, &xHigherPriorityTaskWoken);
//		count_out=0;
//	}
//	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
//#endif
//}

/**
 * @brief This function handles TIM2 global interrupt request by resuming the
 * iNemoData task.
 */
//void TIM2_IRQHandler(void)
//{
//  if(TIM_GetITStatus(TIM2, TIM_IT_Update))
//  {
//
//    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
//    xSemaphoreGiveFromISR(s_timSemaphore, &xHigherPriorityTaskWoken);
//    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
//  }
//}

/*
 * @}
 */ /* end of group iNEMO_task_function */


/**
 * @}
 */ /* end of group iNEMO_task */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
