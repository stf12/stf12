/**
 * @file led_task.h
 *
 * @date   Mar 9, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-mail: software@stf12.net
 */

#ifndef LED_TASK_H_
#define LED_TASK_H_

#include "FreeRTOS.h"


void LedHardwareInit(void *pParam);
void LedStartTask(unsigned short nStackDepth, unsigned portBASE_TYPE nPriority, void *pParams);

#endif /* LED_TASK_H_ */
