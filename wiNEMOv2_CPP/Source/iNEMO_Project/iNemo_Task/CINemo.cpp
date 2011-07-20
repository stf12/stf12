/*
 * CINemo.cpp
 *
 *  Created on: Nov 9, 2010
 *      Author: oliveris
 */

#include "CINemo.h"
#include "CINCommandTask.h"
#include "CINDataTask.h"
#include "iNEMO_lib.h"

CQueue CINemo::s_usbQueue;
CBinarySemaphore CINemo::s_timSemaphore;
iNEMO_DATA CINemo::s_xSharedData;

extern u32 count_out;

/* Task priorities. */
#define IN_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )

/* The check task uses the sprintf function so requires a little more stack. */
#define IN_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 200)


void CINemo::StartTasks() {

	static CINCommandTask inCommandTask(s_usbQueue, &s_xSharedData);
	if ( inCommandTask.Create("iNemoCmd", IN_TASK_STACK_SIZE, IN_TASK_PRIORITY+1) != pdPASS ) {
		// Error in task creation
		while (1);

	}

	static CINDataTask inDataTask(s_timSemaphore, &s_xSharedData);
	if ( inDataTask.Create("iNemoData", IN_TASK_STACK_SIZE, IN_TASK_PRIORITY) != pdPASS ) {
		// Error in task creation
		while (1);
	}
}

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function handles USB Low Priority or CAN RX0 interrupts requests.
 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
#ifdef _VCOM
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	USB_Istr();

	if (count_out) {
		xQueueSendFromISR(CINemo::s_usbQueue, &count_out, &xHigherPriorityTaskWoken);
		count_out=0;
	}

	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
#endif
}

/**
 * @brief This function handles TIM2 global interrupt request by resuming the
 * iNemoData task.
 */
void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update))
  {

    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    if (CINemo::s_timSemaphore.IsValid())
    	CINemo::s_timSemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
  }
}
#ifdef __cplusplus
}
#endif


