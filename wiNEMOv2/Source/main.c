/**
  * @file    main.c 
  * @author  ART Team IMS-Systems Lab
  * @version V2.2
  * @date    01/11/2011
  * @brief   Main
  * @details
  *
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
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */



/**
 * Standard includes.
 */
#include <stdio.h>

/** @defgroup iNEMO_Application
  * @{
  */

/** @defgroup Main_Program
  * @{
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
#include "test.h"
#include "virtual_com.h"

extern volatile FLASH_Status FLASHStatus;

/**
 * Uncomment to enable the test functions for RTC and SD_Card
 * Comment to disable the test functions for RTC and SD_Card
 */
//#define TEST_ENABLE


/**
 * The time between cycles of the 'check' functionality (defined within the
 * tick hook.
 */
#define mainCHECK_DELAY					( ( portTickType ) 5000 / portTICK_RATE_MS )

/**
 * Task priorities.
 */
#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )

/**
 * The check task uses the sprintf function so requires a little more stack.
 */
#define mainCHECK_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 350 )


/*-----------------------------------------------------------*/

/**
 * Configure the hardware for the demo.
 */
static void prvSetupHardware( void );



/**
 * External dependence needed by printf implementation. Write a character to standard out.
 *
 * @param c Specifies the character to be written.
 * @return Returns the character written. No error conditions are managed.
 */


/**
  * @brief  Program the Option Byte to default mode.
  * @param  none.
  * @retval None
  */
void Program_Default_OB(void);


/*-----------------------------------------------------------*/

int main( void )
{
  prvSetupHardware();
  if(Option_Byte_Read()==0x01)
  {
    Program_Default_OB();
  }
  
  // Start all iNemo Tasks.
  inStartTask(NULL);
  
#ifdef TEST_ENABLE
  testStartTask(NULL);
#endif
  /* Start the scheduler. */
  vTaskStartScheduler();
  
  /* Will only get here if there was insufficient memory to create the idle
  task. */
  return 0;
}



/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );

	/* Set the Vector Table base address at 0x08003000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0000);

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );


	// iNemo Tasks initialization.

	//iNemo Task
	inInitTask(NULL);

	//test Task
	testInitTask(NULL);
}

/*-----------------------------------------------------------*/



void Program_Default_OB(void)
{
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  FLASHStatus=FLASH_EraseOptionBytes();  
  FLASHStatus=FLASH_ProgramOptionByteData(Option_address, 0x00);
  FLASH_Lock();
}




void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName )
{
	for( ;; );
}

/**
  * @}
  */ /* end of group Main_Program */

/**
  * @}
  */ /* end of group iNEMO_Application */


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
