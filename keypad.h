/**
*	@file keypad.h
*	@author Mikolaj Telec
*	@brief 4x4 Keypad handler
*		 // C4 C3 C2 C1
*	// R4	  tak
*	// R3  wyglada				C1 nie jest przeze mnie uzywana
*	// R2  klawiatura
*	// R1 
*/
#include "MKL05Z4.h"
#include "utils.h"
#ifndef KEYPAD_H
#define KEYPAD_H

/**
*	@brief Inicjalizacja klawiatury
*/
void KEYPAD_Init(void);

/**
*	@brief @brief Zwraca wartosc odpowiadajaca przyciskowi
*/
char KEY(uint8_t r, uint8_t c);
#endif
