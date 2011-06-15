/**
 * @file main.c
 * @ingroup OTHERS
 *
 * @version 1.0.0
 * @date December, 2010
 * @author Stefano Oliveri
 * @note Module: Others
 *
 * @brief It is part of <b>Others</b> module. This file define the system main function.
 *
 * This file define the system main function.
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


/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Library includes. */
#include "stm32f10x.h"
#include "stm32f10x_it.h"

/* Task includes. */
#include "LedTask.h"
#include "FatTestTask.h"



/*-----------------------------------------------------------*/


#define VECTOR_TABLE_OFFSET				0x0000

/* Task priorities. */
#define LED_TASK_PRIORITY					1
#define FAT_TEST_TASK_PRIORITY				2


/* Task stacks size */
#define LED_TASK_STACK_SIZE					(configMINIMAL_STACK_SIZE)
#define FAT_TEST_TASK_PSTACK_SIZE			(4*configMINIMAL_STACK_SIZE)


/*-----------------------------------------------------------*/


/**
 * Configure the hardware for the demo.
 */
static void prvSetupHardware( void );

/**
 * Configure the GPIO used by the blinking LED task.
 */
extern void vParTestInitialise();


/**
 * External dependence needed by printf implementation. Write a character to standard out.
 *
 * @param ch Specifies the character to be written.
 * @return Returns the character written. No error conditions are managed.
 */
int putChar( int ch );

/*-----------------------------------------------------------*/

volatile int ITM_RxBuffer;



/*-----------------------------------------------------------*/

/**
 * The System main function.
 *
 * @return the main function will never return.
 */
int main( void )
{
	prvSetupHardware();

	/* Start the tasks defined within this file/specific to this demo. */
	LedStartTask(LED_TASK_PRIORITY, LED_TASK_STACK_SIZE);
	FatStartTask(FAT_TEST_TASK_PRIORITY, FAT_TEST_TASK_PSTACK_SIZE);

	/* Start the scheduler. */
	vTaskStartScheduler();

    /* Will only get here if there was insufficient memory to create the idle
    task. */
	return 0;
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, VECTOR_TABLE_OFFSET );

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );

	// Task hardware initialization.
	LedHardwareInit();
	FatHardwareInit();
}

/*-----------------------------------------------------------*/

int putChar( int ch )
{
	return ch;
}

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName )
{
	for( ;; );
}


#ifdef DEBUG

/* Keep the linker happy. */
void assert_failed( unsigned portCHAR* pcFile, unsigned portLONG ulLine )
{
	for( ;; )
	{
	}
}

#endif
