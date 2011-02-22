/**
 * @file       : Main.cpp
 * @author     : Stefano Oliveri
 * @version    : v1.0.0
 * @date       : 20/10/2010
 * Copyright   : Stefano Olivderi
 * Description : C++ template project.
 */


/******* STM Library Include **********************************/
// Specifies the MCU peripheral to use
#include "stm32f10x_conf.h"
// MCU specific settings
#include "stm32f10x.h"

#include "CFreeRTOS.h"


/**
 * Configure the hardware for the demo.
 */
static void prvSetupHardware( void );


/**
 * @brief: Main programma.
 */
int main() {

	prvSetupHardware();

	CFreeRTOS::InitHardwareForManagedTasks();
	CFreeRTOS::StartScheduler();

    while(1);

    return 0;
}

static void prvSetupHardware( void )
{
	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
}

extern "C"
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName )
	{
		/* This function will get called if a task overflows its stack.   If the
		parameters are corrupt then inspect pxCurrentTCB to find which was the
		offending task. */

		( void ) pxTask;
		( void ) pcTaskName;

		for( ;; );
	}


