/**
 * @file LedTask.c
 * @ingroup APPLICATION_TASK
 *
 * @version 1.0.0
 * @date Jun 14, 2011
 * @author Stefano Oliveri
 * @note Module: Application Task.
 *
 * @brief It is part of <b>Application Task</b> module. Blinking LED task.
 *
 */

/*
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
 * <h2><center>&copy; COPYRIGHT 2011 Stefano Oliveri</center></h2>
 */

#include "LedTask.h"
#include "task.h"
#include "stm32_eval.h"

#define LED_TASK_COUNT	2

typedef struct LedTaskParams_t {
	portTickType nFlashRate;
	Led_TypeDef xLed;
}LedTaskParams;

static LedTaskParams s_xTaskParamsVect[LED_TASK_COUNT] = {
		{1000 / portTICK_RATE_MS, LED1},
		{500 / portTICK_RATE_MS, LED2}
};

static void LedTaskFunc(void *pvParams);

void LedHardwareInit() {
	for (int i=0; i<LED_TASK_COUNT; ++i)
		STM_EVAL_LEDInit(s_xTaskParamsVect[i].xLed);
}

void LedStartTask(portBASE_TYPE nPriority, portBASE_TYPE nStackDepth) {
	// Create the high priority LED task.
	xTaskCreate(LedTaskFunc, (signed char*)"LED", nStackDepth, &s_xTaskParamsVect[0], configMAX_PRIORITIES-1, NULL);

	// Create all others LED tasks.
	for (int i=1; i< LED_TASK_COUNT; ++i)
		xTaskCreate(LedTaskFunc, (signed char*)"LED", nStackDepth, &s_xTaskParamsVect[i], nPriority, NULL);
}

/**
 * Task control function.
 *
 * @param pvParams pointer to a LedTaskParams variable.
 */
static void LedTaskFunc(void *pvParams) {
	LedTaskParams *pxParams = (LedTaskParams*) pvParams;
	portTickType nCurrentTick;

	for (;;) {
		nCurrentTick = xTaskGetTickCount();
		vTaskDelayUntil(&nCurrentTick, pxParams->nFlashRate);
		STM_EVAL_LEDToggle(pxParams->xLed);
	}

}
