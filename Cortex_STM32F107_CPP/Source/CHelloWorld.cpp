/**
 * CHelloWorld.cpp
 *
 *  Created on: Jul 26, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include <stdio.h>
#include "CHelloWorld.h"
#include "CLcdTask.h"

extern CLcdTask g_lcdOwnerTask;

CHelloWorld::CHelloWorld(GPIO_TypeDef *pPort, uint16_t pin, portTickType nFlashRate) {
	m_pPort = pPort;
	m_pin = pin;
	m_bState = false;
	SetFlashRate(nFlashRate);
}

CHelloWorld::~CHelloWorld() {
	// TODO Auto-generated destructor stub
}

bool CHelloWorld::HardwareInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =  m_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( m_pPort, &GPIO_InitStructure );

	return true;
}

void CHelloWorld::Run() {
	portTickType xLastFlashTime;
	xLastFlashTime = GetTickCount();

	for(;;)
	{
		/* Delay for half the flash period then turn the LED on. */
		DelayUntil(&xLastFlashTime, m_nFlashRate);
		GPIO_WriteBit(m_pPort, m_pin, m_bState ? Bit_SET : Bit_RESET);
		m_bState = !m_bState;

		/* Delay for half the flash period then turn the LED off. */
		DelayUntil(&xLastFlashTime, m_nFlashRate);
		GPIO_WriteBit(m_pPort, m_pin, m_bState ? Bit_SET : Bit_RESET);
		m_bState = !m_bState;
	}

}

void CHelloWorld::SetFlashRate(portTickType nFlashRate) {
	// Compute the tick count.
	nFlashRate /= portTICK_RATE_MS;
	/* We will turn the LED on and off again in the delay period, so each
	delay is only half the total period. */
	nFlashRate /= ( portTickType ) 2;

	m_nFlashRate = nFlashRate;
}
