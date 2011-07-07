/**
 * @file FatTestTask.h
 * @ingroup APPLICATION_TASK
 *
 * @version 1.0.1
 * @date Jun 14, 2011
 * @author Stefano Oliveri
 * @note Module: Application Task
 *
 * @brief It is part of <b>Application Task</b> module. FatFS test task.
 *
 * This file define the API of the FAT Test task. The task try to perform some operation
 * on the FAT file system in the external flash. It uses the LCD on the board to notify
 * the user about the ongoing operation.
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

#ifndef FATTESTTASK_H_
#define FATTESTTASK_H_

#include "FreeRTOS.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define FAT_MAKEFS_TEST		///< comment/uncomment this macro to disable/enable the format test
#define FAT_WRITE_TEST		///< comment/uncomment this macro to disable/enable the file write test
#define FAT_READ_TEST		///< comment/uncomment this macro to disable/enable the file read test

/**
 * Task specific hardware initialization function. Initialize the LCD.
 */
void FatHardwareInit();

/**
 * Mount the FAT volume and create the FAT Test task
 *
 * @param nPriority task priority.
 * @param nStackDepth task stack size.
 */
void FatStartTask(portBASE_TYPE nPriority, portBASE_TYPE nStackDepth);

#ifdef __cplusplus
 }
#endif

#endif /* FATTESTTASK_H_ */
