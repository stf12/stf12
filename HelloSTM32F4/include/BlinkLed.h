//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm324x9i_eval.h"


// ----------------------------------------------------------------------------

extern
void
blink_led_init(void);

// ----------------------------------------------------------------------------

inline void
blink_led_on(void);

inline void
blink_led_off(void);

// ----------------------------------------------------------------------------

inline void
__attribute__((always_inline))
blink_led_on(void)
{
	  BSP_LED_On(LED1);
	  BSP_LED_On(LED2);
	  BSP_LED_On(LED3);
	  BSP_LED_On(LED4);
}

inline void
__attribute__((always_inline))
blink_led_off(void)
{
	  BSP_LED_Off(LED1);
	  BSP_LED_Off(LED2);
	  BSP_LED_Off(LED3);
	  BSP_LED_Off(LED4);
}

// ----------------------------------------------------------------------------

#endif // BLINKLED_H_
