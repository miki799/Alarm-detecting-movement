/**
*	@file LEDs.c
*	@author Mikolaj Telec
*	@brief Obsluga diod LED
*/
#include "LEDs.h"
/**
* @brief Inicjalizacja LED
*/
void LED_Init(void)
{
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;						// dolaczenie sygnalu zegarowego
	PORTB -> PCR[RED_LED] |= PORT_PCR_MUX(1);			  // PTB8-10 - GPIO
	PORTB -> PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	PORTB -> PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	PTB -> PDDR |= mask(RED_LED) | mask(GREEN_LED) | mask(BLUE_LED);	// PTB8-10 - wyjscie
	PTB -> PSOR |= mask(RED_LED) | mask(GREEN_LED) | mask(BLUE_LED); // LED-y wylaczone
}
/**
* @brief Mruganie diody
*/
void LED_blink(int led, int delay)
{
	PTB -> PTOR |= mask(led);
	delay_ms(delay)
	PTB -> PTOR |= mask(led);
	delay_ms(delay)
}
/**
* @brief Kontrola diody
*/
void LED_Ctrl(int led, int state)
{
	switch(state)
	{
		case 0:
			PTB -> PSOR |= mask(led);
		  break;
		case 1:
			PTB -> PCOR |= mask(led);
		  break;
		case 2:
			PTB -> PTOR |= mask(led);
			break;
	}	
}
/**
* @brief Zapalenie diody zielonej informujacej o uzbrojeniu alarmu
*/
void LED_ALARM_OFF(void)
{
	PTB -> PSOR |= mask(RED_LED);			// Red led off
	PTB -> PCOR |= mask(GREEN_LED);		// Green led on
}
/**
* @brief Zapalenie diody czerwonej informujacej o uruchomieniu alarmu
*/
void LED_ALARM_ON(void)
{
	PTB -> PSOR |= mask(GREEN_LED);		// Green led off
	PTB -> PCOR |= mask(RED_LED);			// Red led on 
}
