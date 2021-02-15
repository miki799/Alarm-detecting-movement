/**
*	@file main.cpp
*	@author Mikolaj Telec
*	@brief Obsluga alarmu
*/
#include "MKL05Z4.h" 
#include "utils.h"
#include "LEDs.h"
#include "keypad.h"
#include "lcd1602.h"
#include "ADC.h"
#include "pit.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// !!!!!!!!!!!!!!!!!!!! 
float LIGHT_VALUE = 1.0;	// wartosc referencyjna [V]
// nalezy dobrac ja odpowiednio do oswietlenia w otoczeniu
// Przykladowe wartosci ktore uzywalem to: 0.5, 0.75, 1.0, 1.5, 2.0

///////////////////////////////////
// Zmienne globalne do kontroli ///
///////////////////////////////////
uint8_t ROW = R4;		// w kazdym przerwaniu sprawdzany tylko jeden wiersz
uint8_t ind = 1;		// blokada w czytaniu klawiatury
uint8_t but = 0;		// 1 - wcisniecie przycisku, 0 - brak wcisniecia
uint8_t ALARM_ON = 0;	// tryby 		
uint8_t ARMED = 0;
uint8_t ADMIN_MODE = 0;	
uint8_t ATTEMPTS = 0; // liczba prob wylaczenia alarmu (max 3 razy)
uint8_t BLOCKADE = 0;	// 1 - blokada wylaczenia alarmu, gdy przekroczymy trzy proby
//////////////////////////
// bufory ////////////////
//////////////////////////
char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
char pressed_key = ' ';	
char PIN[4];												// PIN 4 znakowy	
const char ADMIN_PIN[] = "8880";		// PIN administratora
const char ALARM_ARMED[] = "1234";	// uzbrajanie/rozbrajanie alarmu
char	ALARM_OFF_PIN[] = "4090";			// wylaczenie alarmu. Mozna go zmienic w trybie administratora
int amount = 0;	  									// ilosc wpisanych znakow
///////////////////
// ADC ////////////
///////////////////
float volt_coeff = ((float)(((float)2.91) / 4095) ); // korekcja wyniku 2.91/(2^(rozdzielczosc)-1)
uint8_t wynik_ok=0;
uint16_t temp;
float	wynik;
uint8_t	kal_error;	 
////////////////////////////
// Deklaracje funkcji //////
////////////////////////////
void add(void);								void admin_mode_on(void);
void del(void);								void alarm_on(void);
void pin_check_menu(void);		void alarm_off(void);
void pin_change(void);				void same_pin(void);
void pin_check_alarm(void);		void wrong_pin(void);
void alarm_armed(void);				void blockade_on(void);
void alarm_disarmed(void);		void blockade_off(void);
void user_mode_ret(void);
////////////////////////
// Obslugi przerwan ////
////////////////////////
void PIT_IRQHandler() {
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;	// czysczenie flagi
		if (ind == 1)	// gdy nie ma blokady
		{
			if (ROW > R1)	// gdy sprawdzi wiersz R1 wraca do wiersza R4
				ROW = R4;
		
			PTA -> PDOR &= ~mask(ROW);
		
			__nop(); __nop(); // bez tego pomijany jest pierwszy if :/
		
			if (( PTA -> PDIR & mask(C4) ) == 0) {
				ind = 0;	// blokada	
				pressed_key = KEY(ROW, C4);	// funkcja KEY znajduje sie w bibliotece keypad
				but = 1; ind = 1;	
			} else if (( PTA -> PDIR & mask(C3) ) == 0) {
				ind = 0;
				pressed_key = KEY(ROW, C3);	
				but = 1; ind = 1;	
		  } else if (( PTA -> PDIR & mask(C2) ) == 0) {	
				ind = 0;
				pressed_key = KEY(ROW, C2);
				but = 1; ind = 1;	
			}
			
			PTA -> PDOR |= mask(ROW);
			ROW++;		// wiersz jest zmieniany co przerwanie
		}
	NVIC_ClearPendingIRQ(PIT_IRQn);
	}
}
void ADC0_IRQHandler() 
{	
	temp = ADC0->R[0];	// Odczyt danej
	if(!wynik_ok)	{											
		wynik = temp;			// Przeslanie wyniku											
		wynik_ok=1;
	}
	NVIC_EnableIRQ(ADC0_IRQn);
}

int main(void)
{
	/////////////////////////
	// Inicjalizacje ////////
	/////////////////////////
	LED_Init();												
	KEYPAD_Init();
	LCD1602_Init();		 						
	LCD1602_Backlight(TRUE);
	kal_error=ADC_Init();			
	if(kal_error) {
		LCD1602_Print("ERROR");
		while(1);									
	}
	PIT_Init();
  ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(12); // odblokowanie przerwan oraz jednorazowe wyzwolenie przetwornika

	///////////////
	//// START
	LCD1602_start_screen(); // "ALARM", "MIKOLAJ TELEC"
	while(1) 
	{
		
		LCD1602_menu_screen(); // "PIN:"	
		
		// ALARM UZBROJONY //
		if (ARMED == 1) {			
			ADC0->SC1[0] |= ADC_SC1_ADCH(12); // wyzwolenie przetwarzania, gdy alarm jest uzbrojony
			if(wynik_ok) 
			{
				wynik = wynik*volt_coeff;	// pomiar z czujnika
				wynik_ok=0;
			}
			// sprawdzenie pomiaru
			if (ALARM_ON == 0) 
			{
				if(wynik < (float) LIGHT_VALUE) 
					alarm_on();	// "ALARM! Disable it"
			} 
			else if (ALARM_ON == 1) 
			{
				while ( ALARM_ON == 1 ) // ALARM WLACZONY	
				{ 
					LCD1602_menu_screen(); 	// "PIN:"
					// POBIERANIE PRZYCISKU GDY ALARM JEST UZBROJONY
					if(but == 1) {
						but = 0;
						if ( (amount < 4) && (pressed_key != '*') && (pressed_key != '#') )	add();	// DODAWANIE ZNAKU		
						else if ( pressed_key == '*' )	del();																		// USUWANIE WPISANYCH ZNAKOW
						else if ( (pressed_key ==  '#') && (amount == 4) )	pin_check_alarm();	// SPRAWDZANIE 		
					}
				}
			}
		}
		// POBIERANIE PRZYCISKU GDY ALARM NIE JEST UZBROJONY
		if(but == 1) {
			but = 0;
			if ( (amount < 4) && (pressed_key != '*') && (pressed_key != '#') )	add(); // DODAWANIE ZNAKU
			else if ( pressed_key == '*' )	del();																	 // USUWANIE WPISANYCH ZNAKOW
			else if ( (pressed_key ==  '#') && (amount == 4) )	pin_check_menu();  // SPRAWDZENIE 
		}
	
	} // while end

} // main end



///////////////////////
// Definicje funkcji //
///////////////////////
/**
* @brief Sprawdzenie wpisanego PINu, gdy alarm nie jest wlaczony
*/
void pin_check_menu() 
{	
	if ( (strcmp(PIN, ALARM_ARMED) == 0) && ( ADMIN_MODE == 0 )) // uzbrajanie alarmu nie jest mozliwe w trybie administratora
	{	
		if ( ARMED == 0 ) 			// uzbrojenie alarmu
			alarm_armed();
		else if ( ARMED == 1 )	// rozbrojenie alarmu
			alarm_disarmed();
	} 
	else if ( ( strcmp(PIN, ADMIN_PIN) == 0) && ( ARMED == 0 ) ) // wlaczenie trybu administratora nie jest mozliwe, gdy alarm jest uzbrojony
	{ 
		if ( ADMIN_MODE == 0 )			// wlaczenie trybu administratora
			admin_mode_on();
		else if ( ADMIN_MODE == 1 )	// wylaczenie trybu administratora
			user_mode_ret();
	}
	else if ( (ADMIN_MODE == 1) && (ARMED == 0) )	// zmiana hasla w TRYBIE ADMINISTRATORA, gdy alarm nie jest uzbrojony
	{	
		if ( ( strcmp(PIN, ALARM_OFF_PIN) == 0 ) || ( strcmp(PIN, ALARM_ARMED) == 0 ) )	// gdy zostanie podany PIN, ktory jest w uzyciu 
			same_pin();																																		// np. PIN wylaczajacy alarm lub PIN uzbrajajacy alarm
		else 																																						
			pin_change(); // zmiana PINu wylaczajacego alarm
	}
	else if ( (ADMIN_MODE == 0) 												// gdy w trybie uzytkownika (lub i gdy uzbrojony jest alarm) 
						&& (strcmp(PIN, ALARM_ARMED) != 0)				// zostanie podany inny pin niz ALARM ARMED i ADMIN_PIN
							&& (strcmp(PIN, ADMIN_PIN) != 0) ) {
		wrong_pin();
	}
	else if ( (ARMED == 1) && (strcmp(PIN, ADMIN_PIN) == 0 ) ) // gdy alarm jest uzbrojony i zostanie wpisany PIN admin'a
	{ 	
		wrong_pin();
	}
}
/**
* @brief Zmiana PINu wylaczajacego alarm
*/
void pin_change(void)
{
	ADMIN_MODE = 0;
	LED_Ctrl(RED_LED, LED_OFF);
	LED_Ctrl(GREEN_LED, LED_OFF);
	LED_Ctrl(BLUE_LED, LED_OFF);
	sprintf(ALARM_OFF_PIN, "%s", PIN);			
	LCD1602_admin_mode_off();
	del();
}
/**
* @brief Sprawdzenie wpisanego PINu, gdy alarm jest wlaczony
*/
void pin_check_alarm(void) 
{
	if ( BLOCKADE == 0 ) 
	{
		if ( ( strcmp(PIN, ALARM_OFF_PIN) == 0) )	// DOBRY PIN WYLACZAJACY ALARM
		{ 
			ATTEMPTS = 0;
		  alarm_off();
		} else 																		// ZLY PIN WYLACZAJACY ALARM
		{																		
			ATTEMPTS++;	// kolejna bledna proba
			if (ATTEMPTS == 3)	
				blockade_on();
			else 
				wrong_pin();
		}
	} 
	else if ( BLOCKADE == 1 ) 
	{
		if ( ( strcmp(PIN, ADMIN_PIN) == 0) )		// DOBRY PIN ADMINA
			blockade_off();	
		else																		// ZLY PIN ADMINA
			wrong_pin(); 	
	}
}
/**
* @brief Uzbrajanie alarmu
*/
void alarm_armed(void)
{
	ARMED = 1;
	LED_ALARM_OFF();				// GREEN LED ON, RED LED OFF
	LCD1602_alarm_armed();
	del();
}
/**
* @brief Rozbrajanie alarmu
*/
void alarm_disarmed(void)
{
	ARMED = 0;
	LED_Ctrl(GREEN_LED, LED_OFF);	// GREEN LED OFF,
	LCD1602_alarm_disarmed();
	del();
}
/**
* @brief Uruchomienie trybu administratora
*/
void admin_mode_on(void)
{
	LED_Ctrl(RED_LED, LED_ON);
	LED_Ctrl(GREEN_LED, LED_ON);
	LED_Ctrl(BLUE_LED, LED_ON);
	ADMIN_MODE = 1;
	LCD1602_admin_mode_on();
	del();
}
/**
* @brief Opuszczenie trybu administratora
*/
void user_mode_ret(void)
{
	LED_Ctrl(RED_LED, LED_OFF);
	LED_Ctrl(GREEN_LED, LED_OFF);
	LED_Ctrl(BLUE_LED, LED_OFF);
	ADMIN_MODE = 0;
	LCD1602_pin_not_set();
	del();	
}
/**
* @brief Uruchomienie alarmu
*/
void alarm_on(void)
{
	ALARM_ON = 1;
	LED_ALARM_ON();		// GREEN LED OFF, RED LED ON
	LCD1602_alarm_on();	
	del();
}
/**
* @brief Wylaczenie alarmu
*/
void alarm_off(void)
{
	ALARM_ON = 0;
	LED_ALARM_OFF();	// GREEN LED ON, RED LED OFF
	LCD1602_alarm_off();
	del();
}
/**
* @brief Uruchomienie blokady spowodowanej przekroczeniem ilosci prob
*/
void blockade_on(void)
{
	LED_Ctrl(RED_LED, LED_OFF);
	LED_Ctrl(BLUE_LED, LED_ON);
	BLOCKADE = 1;
	LCD1602_blockade_on();
	del();
}
/**
* @brief Wylaczenie blokady
*/
void blockade_off(void)
{
	LED_Ctrl(BLUE_LED, LED_OFF);
	LED_Ctrl(RED_LED, LED_ON);
	ATTEMPTS = 0;
	BLOCKADE = 0;
	LCD1602_blockade_off();
	del();
}

/**
* @brief Wyswietlenie komunikatu przy próbie ustawienia PINu, który jest aktualnie w uzyciu
*/
void same_pin(void)
{
	LCD1602_same_pin();
	del();
}
/**
* @brief Wyswietlenie komunikatu, gdy zostanie wpisany nieodpowiedni PIN
*/
void wrong_pin(void)
{
	LCD1602_wrong_pin();
	del();
}
/**
* @brief Dodawanie znakow
*/
void add()
{
	PIN[amount] = pressed_key;
	sprintf(display, "%d", atoi(PIN));
	LCD1602_SetCursor(0,1);
	LCD1602_Print(display);
	amount++;
}
/**
* @brief Czyszczenie buforow
*/
void del()
{
	amount = 0;
	sprintf(PIN,"%s", "    ");			
	sprintf(display, "%c", NULL);	  
	LCD1602_ClearAll();
}

