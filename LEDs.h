/**
*	@file LEDs.h
*	@author Mikolaj Telec
*	@brief LEDs initialization, functions, definitions etc.
*/
#include "MKL05Z4.h"
#include "utils.h"

#ifndef LEDS_H
#define LEDS_H

// LEDs macros
#define LED_ON  1
#define LED_OFF 0
#define LED_TOGGLE 2
/**
* @brief Inicjalizacja diod LED
*/
void LED_Init(void);
/**
*	@brief Mruganie diody
*/
void LED_blink(int led, int delay);
/**
* @brief Kontrola diody
*/
void LED_Ctrl(int led, int state);
/**
* @brief Zapalenie diody zielonej informujacej o uzbrojeniu alarmu
*/
void LED_ALARM_OFF(void);
/**
* @brief Zapalenie diody czerwonej informujacej o uruchomieniu alarmu
*/
void LED_ALARM_ON(void);

#endif
