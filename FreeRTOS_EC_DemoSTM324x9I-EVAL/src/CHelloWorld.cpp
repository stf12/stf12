/**
 * CHelloWorld.cpp
 *
 *  Created on: Jul 26, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include <stdio.h>
#include "CHelloWorld.h"



CHelloWorld::CHelloWorld(GPIO_TypeDef *pPort, uint16_t pin, TickType_t nFlashRate) {
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

	GPIO_InitStructure.Pin =  m_pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init( m_pPort, &GPIO_InitStructure );

	return true;
}

void CHelloWorld::Run() {
	TickType_t xLastFlashTime;
	xLastFlashTime = GetTickCount();

	for(;;)
	{
		/* Delay for half the flash period then turn the LED on. */
		DelayUntil(&xLastFlashTime, m_nFlashRate);
//		HAL_GPIO_WritePin(m_pPort, m_pin, m_bState ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_TogglePin(m_pPort, m_pin);
		m_bState = !m_bState;

		/* Delay for half the flash period then turn the LED off. */
		DelayUntil(&xLastFlashTime, m_nFlashRate);
//		HAL_GPIO_WriteBit(m_pPort, m_pin, m_bState ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_TogglePin(m_pPort, m_pin);
		m_bState = !m_bState;
	}

}

void CHelloWorld::SetFlashRate(TickType_t nFlashRate) {
	// Compute the tick count.
	nFlashRate /= portTICK_PERIOD_MS;
	/* We will turn the LED on and off again in the delay period, so each
	delay is only half the total period. */
	nFlashRate /= ( TickType_t ) 2;

	m_nFlashRate = nFlashRate;
}
