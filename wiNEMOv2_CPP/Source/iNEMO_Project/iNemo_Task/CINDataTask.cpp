/*
 * CINDataTask.cpp
 *
 *  Created on: Nov 9, 2010
 *      Author: oliveris
 */

#include "CINDataTask.h"
#include "stm32f10x.h"
#include "iNEMO_lib.h"
#include "GUI_layer.h"

CINDataTask::CINDataTask(CBinarySemaphore &timSemaphore, iNEMO_DATA *pSharedData): m_timSemaphore(timSemaphore) {
	// TODO Auto-generated constructor stub
	m_pSharedData = pSharedData;
}

CINDataTask::~CINDataTask() {
	// TODO Auto-generated destructor stub
}

void CINDataTask::TimerConfig() {
	unsigned short a;
	unsigned short b;
	unsigned long n;
	unsigned char frequency = 50; //This value is the frequency interrupts in Hz
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Enable timer clocks
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );

	// Time base configuration for timer 2 - which generates the interrupts.
	n = configCPU_CLOCK_HZ/frequency;

	prvFindFactors( n, &a, &b );
	TIM_TimeBaseStructure.TIM_Period = b - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = a - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure );
	TIM_ARRPreloadConfig( TIM2, ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );
}

bool CINDataTask::HardwareInit() {
//	TimerConfig();
	return true;
}

portBASE_TYPE CINDataTask::OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority) {

	// Create the semaphore used to synchronize  the iNemo data acquisition task and the TIM2 interrupt service routine.
	if (!m_timSemaphore.Create()) {
		// Error in resource creation.
		while (1);
	}

	return pdTRUE;
}

void CINDataTask::Run() {
	m_timSemaphore.Take(0);

	iNEMO_DATA data;

	iNEMO_Data_Init(&data);
#ifdef AHRS_MOD
	iNEMO_AHRS_Init(&data.m_sensorData, &data.m_angle, &data.m_quat);
#endif

	for (;;) {
		if ( m_timSemaphore.Take(portMAX_DELAY) == pdTRUE ) {
			DataProcess(GetOutMode(),&data);
		}
	}

}
