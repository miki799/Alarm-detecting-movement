

#ifndef LCD1602_H
#define LCD1602_H

#include "frdm_bsp.h"
#include "i2c.h"


/**
 * @brief LCD1602 initialization.
 */
void LCD1602_Init(void);
/**
 * @brief Print on the display.
 *
 * @param String to display.
 */
void LCD1602_Print(char *str);
/**
 * @brief Clear whole display.
 */
void LCD1602_ClearAll(void);
/**
 * @brief Set cursor on display.
 *
 * @param Column.
 * @param Row.
 */
void LCD1602_SetCursor(uint8_t col, uint8_t row);
/**
 * @brief Backlight turn on/off.
 *
 * @param State of the backlight.
 */
void LCD1602_Backlight(uint8_t state);
void LCD1602_Blink_On(void);
void LCD1602_Blink_Off(void);
// funkcja nie byla zadeklarowana
void LCD1602_PrintNum(int number);

// MOJE FUNKCJE														// wyswietlane komunikaty
void LCD1602_start_screen(void);					// strona tytulowa
void LCD1602_menu_screen(void);						// "PIN: "
void LCD1602_alarm_off(void);							// "ALARM OFF: "
void LCD1602_alarm_on(void);							// "ALARM!", "Disable it"
void LCD1602_admin_mode_on(void);					// "ADMIN MODE ON", "Set ALARM PIN:"
void LCD1602_admin_mode_off(void);				// "ALARM PIN set", "ADMIN MODE off"
void LCD1602_same_pin(void);							// "ALREADY USED", "Try again..."
void LCD1602_pin_not_set(void);						// "Returning to", "USER MODE"	
void LCD1602_wrong_pin(void);							// "WRONG PIN", "Try again..."
void LCD1602_alarm_armed(void);						// "! ARMED !", "! ARMED !"
void LCD1602_alarm_disarmed(void);				// "! DISARMED !", "! DISARMED !"
void LCD1602_blockade_on(void);						// "BLOCKADE!!!", "Enter ADMIN PIN:"
void LCD1602_blockade_off(void);					// "BLOCKADE OFF!"
#endif  /* LCD1602_H */
