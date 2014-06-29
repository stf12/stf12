//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <FreeRTOS_EC.h>

/* Hardware includes. */
#include "stm32f4xx.h"
#include "stm32f4xx_hal_conf.h"

#include <stdio.h>
#include "diag/Trace.h"
#include "CMyTask.h"

// ----------------------------------------------------------------------------
//
// STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

using namespace freertosec::wrapper;
using namespace freertosec::managed;

/**
 * Set up the hardware ready to run this demo.
 */
static void prvSetupHardware( void );


// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main(int argc, char* argv[])
{
	// By customising __initialize_args() it is possible to pass arguments,
	// for example when running tests with semihosting you can pass various
	// options to the test.
	// trace_dump_args(argc, argv);

	/* Configure the hardware ready to run the test. */
	prvSetupHardware();

	// Send a greeting to the trace device (skipped on Release).
	trace_puts("Hello FreeRTOS C++ ARM World!");

	// The standard output and the standard error should be forwarded to
	// the trace device. For this to work, a redirection in _write.c is
	// required.
	puts("Standard output message.");
	fprintf(stderr, "Standard error message.\n");

	// At this stage the system clock should have already been configured
	// at high speed.
	trace_printf("System clock: %uHz\n", SystemCoreClock);

	// FreeRTOS_EC Demo

	// Create the context for the managed task
	CMTContext *pxContext = new CMTContext();
	// Activate the context
	pxContext->Activate();

	// Create a simple task.
	CMyTask task1(pxContext);
	task1.Create("task1", configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY + 1);

	// Start the managed application
	CManagedFreeRTOSApp::StartManagedApplication(pxContext);

	// Infinite loop
	while (1)
	{
		// Add your code here.
	}
}

static void prvSetupHardware( void )
{
	// The SystemInit() is called by the __initialize_hardware() in the _initialize_hardware,
	// so it is not needed to be called here.

	/* Ensure all priority bits are assigned as preemption priority bits. */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
