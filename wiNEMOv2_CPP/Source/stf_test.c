/*
 * stf_test.c
 *
 *  Created on: Nov 4, 2010
 *      Author: oliveris
 */

/**
 * Scheduler includes.
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/**
 * Library includes.
 */
#include "stm32f10x.h"
#include "stm32f10x_it.h"

/**
 * iNemo demo files
 */
#include "iNemo.h"
#include "iNEMO_lib.h"
#include "iNEMO_conf.h"
#include "iNEMO_Led.h"

void stf_test() {
	/* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
			| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );

	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0);

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );

	iNEMO_Led_Init(LED1);

	// Virtual COM
	iNEMO_USB_Disconnet_Pin_Config();
	USB_Cable_Config(ENABLE);
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();

	vTaskStartScheduler();

	while (1);
}
