/**
*	@file utils.h
*	@author Mikolaj Telec
*	@brief Zawiera makra, definicje
*/
#ifndef UTILS_H
#define UTILS_H

// LEDs
#define RED_LED 	8
#define GREEN_LED	9
#define BLUE_LED	10
// KEYPAD
#define C4 9	// PTA9		
#define C3 10 // PTA10	
#define C2 11 // PTA11
// #define C1 12 // PTA12 - nie uzywam
#define R4 5  // PTA5
#define R3 6  // PTA6
#define R2 7  // PTA7
#define R1 8  // PTA8

// Global macros
#define mask(x) (uint32_t)(1UL << (x))
#define delay_ms(x) for(volatile int i = 0; i < 3500*x; i++);

#endif 


