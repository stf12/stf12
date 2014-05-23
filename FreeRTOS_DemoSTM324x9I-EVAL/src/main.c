/******************************************************************************
 * >>>>>> NOTE 0: <<<<<<
 *
 * STM32F4 sample (trace via DEBUG).
 * In debug configurations, demonstrate how to print a greeting message
 * on the trace device. In release configurations the message is
 * simply discarded.
 * To demonstrate POSIX retargetting, reroute the STDOUT and STDERR to the
 * trace device and display messages on both of them.
 *
 * Trace support is enabled by adding the TRACE macro definition.
 * By default the trace messages are forwarded to the DEBUG output,
 * but can be rerouted to any device or completely suppressed, by
 * changing the definitions required in system/src/diag/trace_impl.c
 * (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
 *
 * >>>>>> NOTE 1: <<<<<<
 *
 * main() can be configured to create either a very simple LED flasher demo, or
 * a more comprehensive test/demo application.
 *
 * To create a very simple LED flasher example, set the
 * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY constant (defined below) to 1.  When
 * this is done, only the standard demo flash tasks are created.  The standard
 * demo flash example creates three tasks, each of which toggle an LED at a
 * fixed but different frequency.
 *
 * To create a more comprehensive test and demo application, set
 * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY to 0.
 *
 * >>>>>> NOTE 2: <<<<<<
 *
 * In addition to the normal set of standard demo tasks, the comprehensive test
 * makes heavy use of the floating point unit, and forces floating point
 * instructions to be used from interrupts that nest three deep.  The nesting
 * starts from the tick hook function, resulting is an abnormally long context
 * switch time.  This is done purely to stress test the FPU context switching
 * implementation, and that part of the test can be removed by setting
 * configUSE_TICK_HOOK to 0 in FreeRTOSConfig.h.
 ******************************************************************************
 *
 * main() creates all the demo application tasks and software timers, then starts
 * the scheduler.  The web documentation provides more details of the standard
 * demo application tasks, which provide no particular functionality, but do
 * provide a good example of how to use the FreeRTOS API.
 *
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Reg test" tasks - These fill both the core and floating point registers with
 * known values, then check that each register maintains its expected value for
 * the lifetime of the task.  Each task uses a different set of values.  The reg
 * test tasks execute with a very low priority, so get preempted very
 * frequently.  A register containing an unexpected value is indicative of an
 * error in the context switching mechanism.
 *
 * "Check" timer - The check software timer period is initially set to three
 * seconds.  The callback function associated with the check software timer
 * checks that all the standard demo tasks, and the register check tasks, are
 * not only still executing, but are executing without reporting any errors.  If
 * the check software timer discovers that a task has either stalled, or
 * reported an error, then it changes its own execution period from the initial
 * three seconds, to just 200ms.  The check software timer callback function
 * also toggles an LED each time it is called.  This provides a visual
 * indication of the system status:  If the LED toggles every three seconds,
 * then no issues have been discovered.  If the LED toggles every 200ms, then
 * an issue has been discovered with at least one task.
 *
 * Tick hook - The application tick hook is called from the schedulers tick
 * interrupt service routine when configUSE_TICK_HOOK is set to 1 in
 * FreeRTOSConfig.h.  In this example, the tick hook is used to test the kernels
 * handling of the floating point units (FPU) context, both at the task level
 * and when nesting interrupts access the floating point unit registers.  The
 * tick hook function first fills the FPU registers with a known value, it
 * then triggers a medium priority interrupt.  The medium priority interrupt
 * fills the FPU registers with a different value, and triggers a high priority
 * interrupt.  The high priority interrupt once again fills the the FPU
 * registers with a known value before returning to the medium priority
 * interrupt.  The medium priority interrupt checks that the FPU registers
 * contain the values that it wrote to them, then returns to the tick hook
 * function.  Finally, the tick hook function checks that the FPU registers
 * contain the values that it wrote to them, before it too returns.
 *
 * Button interrupt - The button marked "USER" on the starter kit is used to
 * demonstrate how to write an interrupt service routine, and how to synchronise
 * a task with an interrupt.  A task is created that blocks on a test semaphore.
 * When the USER button is pressed, the button interrupt handler gives the
 * semaphore, causing the task to unblock.  When the task unblocks, it simply
 * increments an execution count variable, then returns to block on the
 * semaphore again.
 */

// ----------------------------------------------------------------------------

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Demo application includes. */
#include "partest.h"
#include "flash.h"
#include "integer.h"
#include "dynamic.h"
#include "BlockQ.h"
#include "blocktim.h"
#include "countsem.h"
#include "GenQTest.h"
#include "recmutex.h"
#include "PollQ.h"
#include "semtest.h"
#include "flop.h"
#include "death.h"

/* Hardware includes. */
#include "stm32f4xx.h"
#include "stm32f4xx_hal_conf.h"
#include "stm324x9i_eval.h"

// Others includes
#include <stdio.h>
#include "diag/Trace.h"


/* Priorities for the demo application tasks. */
#define mainFLASH_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define mainQUEUE_POLL_PRIORITY				( tskIDLE_PRIORITY + 2UL )
#define mainSEM_TEST_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 2UL )
#define mainCREATOR_TASK_PRIORITY			( tskIDLE_PRIORITY + 3UL )
#define mainFLOP_TASK_PRIORITY				( tskIDLE_PRIORITY )

/* The LED used by the check timer. */
#define mainCHECK_LED 						( LED4 )

/* A block time of zero simply means "don't block". */
#define mainDONT_BLOCK						( 0UL )

/* The period after which the check timer will expire, in ms, provided no errors
have been reported by any of the standard demo tasks.  ms are converted to the
equivalent in ticks using the portTICK_PERIOD_MS constant. */
#define mainCHECK_TIMER_PERIOD_MS			( 3000UL / portTICK_PERIOD_MS )

/* The period at which the check timer will expire, in ms, if an error has been
reported in one of the standard demo tasks.  ms are converted to the equivalent
in ticks using the portTICK_PERIOD_MS constant. */
#define mainERROR_CHECK_TIMER_PERIOD_MS 	( 200UL / portTICK_PERIOD_MS )

/* Set mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY to 1 to create a simple demo.
Set mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY to 0 to create a much more
comprehensive test application.  See the comments at the top of this file, and
the documentation page on the http://www.FreeRTOS.org web site for more
information. */
#define mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY		0

/**
 * Set up the hardware ready to run this demo.
 */
static void prvSetupHardware( void );

/**
 * The check timer callback function, as described at the top of this file.
 *
 * @param xTimer
 */
static void prvCheckTimerCallback( TimerHandle_t xTimer );

/**
 * Configure the interrupts used to test the interrupt nesting depth as
 * described at the top of this file.
 */
//static void prvSetupNestedFPUInterruptsTest( void );

/**
 * The task that is synchronised with the button interrupt.  This is done just
 * to demonstrate how to write interrupt service routines, and how to
 * synchronise a task with an interrupt.
 *
 * @param pvParameters
 */
static void prvButtonTestTask( void *pvParameters );

/**
 * This file can be used to create either a simple LED flasher example, or a
 * comprehensive test/demo application - depending on the setting of the
 * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY constant defined above.  If
 * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY is set to 1, then the following
 * function will create a lot of additional tasks and a software timer.  If
 * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY is set to 0, then the following
 * function will do nothing.
 */
static void prvOptionallyCreateComprehensveTestApplication( void );

/* The semaphore used to demonstrate a task being synchronised with an
interrupt. */
static SemaphoreHandle_t xTestSemaphore = NULL;

/* The variable that is incremented by the task synchronised with the button
interrupt. */
volatile unsigned long ulButtonPressCounts = 0UL;


// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main(int argc, char* argv[])
{
	/* Configure the hardware ready to run the test. */
	prvSetupHardware();

	// By customising __initialize_args() it is possible to pass arguments,
	// for example when running tests with semihosting you can pass various
	// options to the test.
	// trace_dump_args(argc, argv);

	// Send a greeting to the trace device (skipped on Release).
	trace_puts("Hello FreeRTOS ARM World!");

	// The standard output and the standard error should be forwarded to
	// the trace device. For this to work, a redirection in _write.c is
	// required.
	puts("Standard output message.");
	fprintf(stderr, "Standard error message.\n");

	// At this stage the system clock should have already been configured
	// at high speed.
	trace_printf("System clock: %uHz\n", SystemCoreClock);


	/* Start standard demo/test application flash tasks.  See the comments at
	the top of this file.  The LED flash tasks are always created.  The other
	tasks are only created if mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY is set to
	0 (at the top of this file).  See the comments at the top of this file for
	more information. */
	vStartLEDFlashTasks( mainFLASH_TASK_PRIORITY );

	/* The following function will only create more tasks and timers if
	mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY is set to 0 (at the top of this
	file).  See the comments at the top of this file for more information. */
	prvOptionallyCreateComprehensveTestApplication();

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );

}

static void prvSetupHardware( void )
{
	// The SystemInit() is called by the __initialize_hardware() in the _initialize_hardware,
	// so it is not needed to be called here.

	/* Ensure all priority bits are assigned as preemption priority bits. */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* Setup the LED outputs. */
	vParTestInitialise();

	/* Configure the button input.  This configures the interrupt to use the
	lowest interrupt priority, so it is ok to use the ISR safe FreeRTOS API
	from the button interrupt handler. */
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
}

static void prvOptionallyCreateComprehensveTestApplication( void )
{
#if ( mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY == 0 )
	{
		TimerHandle_t xCheckTimer = NULL;

		/* Start all the other standard demo/test tasks. */
		vStartIntegerMathTasks( tskIDLE_PRIORITY );
		vStartDynamicPriorityTasks();
		vStartBlockingQueueTasks( mainBLOCK_Q_PRIORITY );
		vCreateBlockTimeTasks();
		vStartCountingSemaphoreTasks();
		vStartGenericQueueTasks( tskIDLE_PRIORITY );
		vStartRecursiveMutexTasks();
		vStartPolledQueueTasks( mainQUEUE_POLL_PRIORITY );
		vStartSemaphoreTasks( mainSEM_TEST_PRIORITY );

		/* Most importantly, start the tasks that use the FPU. */
		vStartMathTasks( mainFLOP_TASK_PRIORITY );

		/* Create the semaphore that is used to demonstrate a task being
		synchronised with an interrupt. */
		vSemaphoreCreateBinary( xTestSemaphore );

		/* Create the task that is unblocked by the demonstration interrupt. */
		xTaskCreate( prvButtonTestTask, "BtnTest", configMINIMAL_STACK_SIZE, ( void * ) NULL, tskIDLE_PRIORITY, NULL );

		/* Create the software timer that performs the 'check' functionality,
		as described at the top of this file. */
		xCheckTimer = xTimerCreate( "CheckTimer",					/* A text name, purely to help debugging. */
									( mainCHECK_TIMER_PERIOD_MS ),	/* The timer period, in this case 3000ms (3s). */
									pdTRUE,							/* This is an auto-reload timer, so xAutoReload is set to pdTRUE. */
									( void * ) 0,					/* The ID is not used, so can be set to anything. */
									prvCheckTimerCallback			/* The callback function that inspects the status of all the other tasks. */
								  );

		if( xCheckTimer != NULL )
		{
			xTimerStart( xCheckTimer, mainDONT_BLOCK );
		}

		/* This task has to be created last as it keeps account of the number of
		tasks it expects to see running. */
		vCreateSuicidalTasks( mainCREATOR_TASK_PRIORITY );
	}
#endif /* mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY */
}

/**
 * Task control loop.
 *
 * @param pvParameters
 */
static void prvButtonTestTask( void *pvParameters )
{
	configASSERT( xTestSemaphore );

	/* This is the task used as an example of how to synchronise a task with
	an interrupt.  Each time the button interrupt gives the semaphore, this task
	will unblock, increment its execution counter, then return to block
	again. */

	/* Take the semaphore before started to ensure it is in the correct
	state. */
	xSemaphoreTake( xTestSemaphore, mainDONT_BLOCK );

	for( ;; )
	{
		xSemaphoreTake( xTestSemaphore, portMAX_DELAY );
		ulButtonPressCounts++;
	}
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  *
  */
void EXTI15_10_IRQHandler(void) {
	 HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == KEY_BUTTON_PIN)
	{
		long lHigherPriorityTaskWoken = pdFALSE;


		/* Only one line is enabled, so there is no need to test which line generated
		 * the interrupt.
		 */
		//	EXTI_ClearITPendingBit( EXTI_Line6 );
		//	HAL_NVIC_ClearPendingIRQ(KEY_BUTTON_EXTI_IRQn);


		/* This interrupt does nothing more than demonstrate how to synchronise a
		 * task with an interrupt.  First the handler releases a semaphore.
		 * lHigherPriorityTaskWoken has been initialised to zero.
		 */
		xSemaphoreGiveFromISR( xTestSemaphore, &lHigherPriorityTaskWoken );


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

static void prvCheckTimerCallback( TimerHandle_t xTimer )
{
static long lChangedTimerPeriodAlready = pdFALSE;
//static unsigned long ulLastRegTest1Value = 0, ulLastRegTest2Value = 0;
long lErrorFound = pdFALSE;

	/* Check all the demo tasks (other than the flash tasks) to ensure
	that they are all still running, and that none have detected an error. */

	if( xAreMathsTaskStillRunning() != pdTRUE )
	{
		lErrorFound = pdTRUE;
	}

	if( xAreIntegerMathsTaskStillRunning() != pdTRUE )
	{
		lErrorFound = pdTRUE;
	}

	if( xAreDynamicPriorityTasksStillRunning() != pdTRUE )
	{
		lErrorFound = pdTRUE;
	}

	if( xAreBlockingQueuesStillRunning() != pdTRUE )
	{
		lErrorFound = pdTRUE;
	}

	if ( xAreBlockTimeTestTasksStillRunning() != pdTRUE )
	{
		lErrorFound = pdTRUE;
	}

	if ( xAreGenericQueueTasksStillRunning() != pdTRUE )
	{
		lErrorFound = pdTRUE;
	}

	if ( xAreRecursiveMutexTasksStillRunning() != pdTRUE )
	{
		lErrorFound = pdTRUE;
	}

	if( xIsCreateTaskStillRunning() != pdTRUE )
	{
		lErrorFound = pdTRUE;
	}

	if( xArePollingQueuesStillRunning() != pdTRUE )
	{
		lErrorFound = pdTRUE;
	}

	if( xAreSemaphoreTasksStillRunning() != pdTRUE )
	{
		lErrorFound = pdTRUE;
	}

	/* Toggle the check LED to give an indication of the system status.  If
	the LED toggles every mainCHECK_TIMER_PERIOD_MS milliseconds then
	everything is ok.  A faster toggle indicates an error. */
	vParTestToggleLED( mainCHECK_LED );

	/* Have any errors been latch in lErrorFound?  If so, shorten the
	period of the check timer to mainERROR_CHECK_TIMER_PERIOD_MS milliseconds.
	This will result in an increase in the rate at which mainCHECK_LED
	toggles. */
	if( lErrorFound != pdFALSE )
	{
		if( lChangedTimerPeriodAlready == pdFALSE )
		{
			lChangedTimerPeriodAlready = pdTRUE;

			/* This call to xTimerChangePeriod() uses a zero block time.
			Functions called from inside of a timer callback function must
			*never* attempt	to block. */
			xTimerChangePeriod( xTimer, ( mainERROR_CHECK_TIMER_PERIOD_MS ), mainDONT_BLOCK );
		}
	}
}


/**
 *
 */
void vApplicationTickHook( void )
{

}

/**
 *	vApplicationMallocFailedHook() will only be called if
 *	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
 *	function that will get called if a call to pvPortMalloc() fails.
 *	pvPortMalloc() is called internally by the kernel whenever a task, queue,
 *	timer or semaphore is created.  It is also called by various parts of the
 *	demo application.  If heap_1.c or heap_2.c are used, then the size of the
 *	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
 *	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
 *	to query the size of free heap space that remains (although it does not
 *	provide information on how the remaining heap might be fragmented).
 */
void vApplicationMallocFailedHook( void ) {
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

/**
 * 	vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
 * 	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
 * 	task.  It is essential that code added to this hook function never attempts
 * 	to block in any way (for example, call xQueueReceive() with a block time
 * 	specified, or call vTaskDelay()).  If the application makes use of the
 * 	vTaskDelete() API function (as this demo application does) then it is also
 * 	important that vApplicationIdleHook() is permitted to return to its calling
 * 	function, because it is the responsibility of the idle task to clean up
 * 	memory allocated by the kernel to any task that has since been deleted.
 */
void vApplicationIdleHook( void ) {
}

/**
 * 	Run time stack overflow checking is performed if
 * 	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
 * 	function is called if a stack overflow is detected.
 *
 * @param pxTask
 * @param pcTaskName
 */
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;


	taskDISABLE_INTERRUPTS();
	for( ;; );
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
