/*
 * CINCommandTask.cpp
 *
 *  Created on: Nov 9, 2010
 *      Author: oliveris
 */

#include "CINCommandTask.h"
#include "stm32f10x.h"
#include "GUI_layer.h"
#include "utils.h"
#include "iNEMO_lib.h"


//extern u32 count_out;
//extern u8 buffer_rx[VIRTUAL_COM_PORT_DATA_SIZE];


CINCommandTask::CINCommandTask(CQueue &usbQueue, iNEMO_DATA *pSharedData): m_usbQueue(usbQueue) {
	// TODO Auto-generated constructor stub
	m_pSharedData = pSharedData;
}

CINCommandTask::~CINCommandTask() {
	// TODO Auto-generated destructor stub
}

bool CINCommandTask::HardwareInit() {

//	iNEMO_HW_Config();
//
//	while(bDeviceState != CONFIGURED);
//
//	iNEMO_Config();
//
//	// Timer Configuration
//
//	unsigned short a;
//	unsigned short b;
//	unsigned long n;
//	unsigned char frequency = 50; //This value is the frequency interrupts in Hz
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	// Enable timer clocks
//	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
//
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//
//	TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );
//
//	// Time base configuration for timer 2 - which generates the interrupts.
//	n = configCPU_CLOCK_HZ/frequency;
//
//	prvFindFactors( n, &a, &b );
//	TIM_TimeBaseStructure.TIM_Period = b - 1;
//	TIM_TimeBaseStructure.TIM_Prescaler = a - 1;
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure );
//	TIM_ARRPreloadConfig( TIM2, ENABLE );
//
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init( &NVIC_InitStructure );

	return true;
}

portBASE_TYPE CINCommandTask::OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority) {

	return m_usbQueue.Create(1, sizeof(u32)).IsValid();
}

void CINCommandTask::Run() {
	u32 _count_out = 0;

	for (;;) {
		if ( m_usbQueue.Receive(&_count_out, portMAX_DELAY) == pdTRUE ) {
			ParseGuiFrame(_count_out, m_pSharedData);
		}
	}
}


