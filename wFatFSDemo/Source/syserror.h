/**
 * @file syserror.h
 * @ingroup OTHERS
 *
 * @version 0.1.0
 * @date Jan 27, 2011
 * @author IMS Systems Lab - ART Team
 * @note Module: Others
 *
 * @brief It is part of <b>Others</b>module. Define the global error management API.
 *
 * The system uses a single 32 bits global variable to track the last runtime error.
 * This variable stores in the last significant 16 bits (bit [0,15]) the last error occurred in the
 * Low Level API layer. The last error occurred in the Service Layer level is stored
 * in the most significant 16 bits (bit [16, 31]).
 * The application uses the SYS_GET_LAST_ERROR() macro to retrieve the last error.
 *
 * @todo Code review: to change all functions returning a sys_error_t value in order to
 * return a sys_error_code_t value.
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
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 */


#ifndef SYSERROR_H_
#define SYSERROR_H_

#include <assert.h>

typedef unsigned short sys_error_code_t;

typedef union {
	unsigned long error_code;
	struct {
		unsigned int low_level_e:		16;
		unsigned int service_level_e:	16;
	} type;
}sys_error_t;

extern sys_error_t g_nSysError;

#define SYS_CLEAR_ERROR() {g_nSysError.error_code = 0;}
#define SYS_IS_ERROR(e) (e.error_code != 0)
#define SYS_IS_LOW_LEVEL_ERROR(e) ( (e).type.low_level_e )
#define SYS_IS_SERVICE_LEVEL_ERROR(e) ( (e).type.service_level_e )
#define SYS_SET_LOW_LEVEL_ERROR_CODE(e) {g_nSysError.type.low_level_e = e;}
#define SYS_SET_SERVICE_LEVEL_ERROR_CODE(e) {g_nSysError.type.service_level_e = e;}
#define SYS_GET_LAST_ERROR() g_nSysError
#define SYS_GET_LAST_LOW_LEVEL_ERROR_CODE() (g_nSysError.type.low_level_e)
#define SYS_GET_LAST_SERVICE_LEVEL_ERROR_CODE() (g_nSysError.type.service_level_e)
#define SYS_GET_LOW_LEVEL_ERROR_CODE(e) (e.type.low_level_e)
#define SYS_GET_SERVICE_LEVEL_ERROR_CODE(e)	(e.type.service_level_e)

#define SYS_NO_ERROR 									((sys_error_t){0x0})
#define SYS_NO_ERROR_CODE								(0x0)
#define SYS_GROUP_ERROR_COUNT							(200)

// Low Level API error constants
#define SYS_BASE_LOW_LEVEL_ERROR						((sys_error_t){0x1})
#define SYS_BASE_LOW_LEVEL_ERROR_CODE 					(0x1)

// Task Level Service error constants
#define SYS_BASE_SERVICE_LEVEL_ERROR					((sys_error_t){0x10000})
#define SYS_BASE_SERVICE_LEVEL_ERROR_CODE 				(0x1)

// Error Code definition
// *********************

// Low Level API error code
// ************************

// Memory management error
#define SYS_BASE_NOR_FLASH_ERROR_CODE					SYS_BASE_LOW_LEVEL_ERROR_CODE + SYS_GROUP_ERROR_COUNT
#define SYS_NOR_FLASH_ONGOING_ERROR_CODE				SYS_BASE_NOR_FLASH_ERROR_CODE + 1
#define SYS_NOR_FLASH_ERROR_ERROR_CODE					SYS_BASE_NOR_FLASH_ERROR_CODE + 2
#define SYS_NOR_FLASH_TIMEOUT_ERROR_CODE				SYS_BASE_NOR_FLASH_ERROR_CODE + 3
#define SYS_NOR_FLASH_CANNOT_USE_WRITE_TO_BUFFER		SYS_BASE_NOR_FLASH_ERROR_CODE + 4
#define SYS_NOR_FLASH_WRITE_TO_BUFFER_FAILED			SYS_BASE_NOR_FLASH_ERROR_CODE + 5


// Task Level Service error code
// *****************************

#define SYS_BASE_TASK_ERROR_CODE						SYS_BASE_SERVICE_LEVEL_ERROR_CODE
#define SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE			SYS_BASE_TASK_ERROR_CODE + 1
#define SYS_TASK_INVALID_CALL_ERROR_CODE				SYS_BASE_TASK_ERROR_CODE + 2
#define SYS_TASK_INVALID_PARAM_ERROR_CODE				SYS_BASE_TASK_ERROR_CODE + 3

#define SYS_BASE_FLASH_IF_ERROR_CODE					SYS_BASE_CTI_TASK_ERROR_CODE + SYS_GROUP_ERROR_COUNT
#define SYS_FLASH_IF_FAT_ERROR_CODE						SYS_BASE_FLASH_IF_ERROR_CODE + 1


#endif /* SYSERROR_H_ */
