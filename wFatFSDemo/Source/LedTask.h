/**
 * @file LedTask.h
 * @ingroup APPLICATION_TASK
 *
 * @version 1.0.1
 * @date Jun 14, 2011
 * @author Stefano Oliveri
 * @note Module: Application Task
 *
 * @brief It is part of <b>Application Task</b> module. Blinking LED task.
 *
 * This module create two tasks in order to blink two LEDs. One task manage LD1 LED. This task runs at high priority
 * so that it is not blocked by the time consuming FAT Test task. I use this task to ensure that the system is up and
 * running. The other task manage the LD2 LED and runs at lower priority so it stop blinking while the FAT Test task is
 * running, and resume when the FAT Test task ends.
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

#ifndef LEDTASK_H_
#define LEDTASK_H_

#include "FreeRTOS.h"

#ifdef __cplusplus
 extern "C" {
#endif

void LedHardwareInit();
void LedStartTask(portBASE_TYPE nPriority, portBASE_TYPE nStackDepth);

#ifdef __cplusplus
 }
#endif

#endif /* LEDTASK_H_ */
