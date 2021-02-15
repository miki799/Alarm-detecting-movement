/**
*	@file keypad.c
*	@author Mikolaj Telec
*	@brief Inicjalizacja klawiatury oraz funkcja zwracajaca wartosc odpowiadajaca przyciskowi
*/

#include "keypad.h"

// wcisniety przycisk
char key = ' ';
/**
* @brief Inicjalizacja klawiatury
*/
void KEYPAD_Init(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		
	for (volatile int i = C4; i <= C2; i++){  								// KOLUMNY - WEJSCIE
		PORTA -> PCR[i] |= PORT_PCR_MUX(1);
		PORTA -> PCR[i] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	}
	for (volatile int j = R4; j <= R1; j++){ 									// WIERSZE - WYJSCIE
		PORTA -> PCR[j] |= PORT_PCR_MUX(1);
		PTA -> PDDR |= mask(j);
		PTA -> PDOR |= mask(j);
		
	}	
}
/**
* 	@brief Zwraca wartosc odpowiadajaca przyciskowi
*/
char KEY(uint8_t r, uint8_t c)
{	
	if ( r == R4 && c == C4 )
		key = '1';																									
	else if ( r == R4 && c == C3 )
		key = '2';		
	else if ( r == R4 && c == C2 )
		key = '3';		
	else if ( r == R3 && c == C4 )
		key = '4';	
	else if ( r == R3 && c == C3 )
		key = '5';		
	else if ( r == R3 && c == C2 )
		key = '6';		
	else if ( r == R2 && c == C4 )
		key = '7';		
	else if ( r == R2 && c == C3 )
		key = '8';		
	else if ( r == R2 && c == C2 )
		key = '9';	
	else if ( r == R1 && c == C4 )
		key = '#';	
	else if ( r == R1 && c == C3 )
		key = '0';	
	else if ( r == R1 && c == C2 )
		key = '*';  
	return key;
}