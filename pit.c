/**
*	@file pit.c
*	@author Kod z zajęć laboratoryjnych
*	@brief Plik naglowkowy 
*/
#include "MKL05Z4.h"
#include	"pit.h"

void PIT_Init(void)
{
	uint32_t tsv1;
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;																		// Włączenie sygnału zegara do modułu PIT
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;																			// Włączenie modułu PIT
	tsv1=BUS_CLOCK/10/2;																								// Przerwanie co ~50ms	
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(tsv1);												// Załadowanie wartości startowej
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;		// Odblokowanie przerwania i wystartowanie licznika
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn, 64);
	NVIC_EnableIRQ(PIT_IRQn);	
}
