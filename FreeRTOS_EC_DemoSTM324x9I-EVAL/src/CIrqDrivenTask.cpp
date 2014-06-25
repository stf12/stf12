/*
 * CIrqDrivenTask.cpp
 *
 *  Created on: May 30, 2014
 *      Author: oliveris
 */

#include <FreeRTOS_EC.h>
#include "CIrqDrivenTask.h"
#include "CLcdTask2.h"
#include "stm324x9i_eval.h"

#include <string.h>
#include <stdio.h>

// Allocate the static object
CBinarySemaphore CIrqDrivenTask::s_semButton;

CIrqDrivenTask::CIrqDrivenTask(CMTContext *pContext): AManagedTask(pContext) {
	m_nButtonPressedCount = 0;
}

CIrqDrivenTask::~CIrqDrivenTask() {
	// TODO Auto-generated destructor stub
}

bool CIrqDrivenTask::HardwareInit() {

	/* Configure the button input.  This configures the interrupt to use the
	lowest interrupt priority, so it is ok to use the ISR safe FreeRTOS API
	from the button interrupt handler. */
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

	return true;
}

BaseType_t  CIrqDrivenTask::OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, UBaseType_t  uxPriority) {
	(void)pcName;
	(void)usStackDepth;
	(void)uxPriority;

	if ( s_semButton.Create() )
		return pdPASS;
	else
		return pdFAIL;
}

void CIrqDrivenTask::Run() {

	char msg[25];
	memset(msg, '\0', sizeof(msg));
	// Take the semaphore before started to ensure it is in the correct state.
	// A block time of zero simply means "don't block".
	s_semButton.Take(0);

	for( ;; ) {
		s_semButton.Take(portMAX_DELAY);
		sprintf(msg, "Button Pressed: %i", m_nButtonPressedCount);
		CLcdTask2::GetSharedInstance().Println(msg);
		m_nButtonPressedCount++;
	}
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  *
  */
extern "C"
void EXTI15_10_IRQHandler(void) {
	 HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  */
extern "C"
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == KEY_BUTTON_PIN)
	{
		long lHigherPriorityTaskWoken = pdFALSE;

		/* This interrupt does nothing more than demonstrate how to synchronise a
		 * task with an interrupt.  First the handler releases a semaphore.
		 * lHigherPriorityTaskWoken has been initialised to zero.
		 */
		xSemaphoreGiveFromISR( CIrqDrivenTask::s_semButton, &lHigherPriorityTaskWoken );

		/* If there was a task that was blocked on the semaphore, and giving the
		 * semaphore caused the task to unblock, and the unblocked task has a priority
		 * higher than the currently executing task (the task that this interrupt
		 * interrupted), then lHigherPriorityTaskWoken will have been set to pdTRUE.
		 * Passing pdTRUE into the following macro call will cause this interrupt to
		 * return directly to the unblocked, higher priority, task.
		 */
		portEND_SWITCHING_ISR( lHigherPriorityTaskWoken );
	}
}
