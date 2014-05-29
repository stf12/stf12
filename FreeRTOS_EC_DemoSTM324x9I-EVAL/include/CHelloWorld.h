/**
 * CHelloWorld.h
 *
 * This class implement a blinking led task.
 *
 *  Created on: Jul 26, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#ifndef CHELLOWORLD_H_
#define CHELLOWORLD_H_

#include "AManagedTask.h"
/* Library includes. */
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"


class CHelloWorld: public AManagedTask {
	/**
	 * Specifies the GPIO port to which is connected the LED.
	 */
	GPIO_TypeDef *m_pPort;

	/**
	 * Specifies the pin number to witch is connected the LED.
	 */
	uint16_t m_pin;

	/**
	 * Specifies the current LED state. The LED is on when this member is true.
	 */
	bool m_bState;

	/**
	 * Specifies the LED blinking rate.
	 */
	TickType_t m_nFlashRate;

public:
	/**
	 * Default constructor.
	 *
	 * @param pPort specifies the GPIO port to which is connected the LED.
	 * @param pin specifies the pin number to witch is connected the LED. Use the bitwise OR operator
	 *            to specify more then one pin.
	 * @param nFlashRate specifies the LED blinking rate.
	 */
	CHelloWorld(GPIO_TypeDef *pPort, uint16_t pin, TickType_t nFlashRate);

	virtual ~CHelloWorld();

	/**
	 * Task control function.
	 */
	void Run();

	/**
	 * Configure the LED pin as Output PushPull 50MHz.
	 *
	 * @return true if success, false otherwise.
	 */
	bool HardwareInit();

	/**
	 * Change the LED blinking rate.
	 *
	 * @param nFlashRate specifies the new blinking rate.
	 */
	void SetFlashRate(TickType_t nFlashRate);

	/**
	 * Get the current LED state.
	 *
	 * @return true if the LED is on, false otherwise.
	 */
	bool IsOn() const;
};


// Inline member definition.

inline
bool CHelloWorld::IsOn() const {
	return m_bState;
}

#endif /* CHELLOWORLD_H_ */
