/*
 * SplashScreen.h
 *
 *  Created on: May 20, 2014
 *      Author: oliveris
 */

#ifndef SPLASHSCREEN_H_
#define SPLASHSCREEN_H_

/* Includes */
#include "stm324x9i_eval.h"
#include "stm324x9i_eval_lcd.h"

/* Private macro */
#define MESSAGE1   "          STM32F4X9X          "
#define MESSAGE2   "      Device running on       "
#define MESSAGE3   "        STM324x9I-EVAL        "
#define MESSAGE4   "                              "
#define MESSAGE5   "      program built with      "
#define MESSAGE6   "   Open Source Technologies   "


void SCDisplayMessage();

#endif /* SPLASHSCREEN_H_ */
