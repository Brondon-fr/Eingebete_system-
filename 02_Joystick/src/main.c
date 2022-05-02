/**
  ******************************************************************************
  * @file    	main.c
  * @author		Florian von Zabiensky
  * @version 	V1.0
  * @date		14.03.2019
  * @brief  	Template using the LCD
  ******************************************************************************
*/

#include <joystick/joystick.h>
#include <lcd/lcd.h>
#include "stm32f4xx.h"

int main(void) {
	HAL_Init();
	lcd_init();

	joystick_init();
/**
 * hier wird joystick_read() aufgerufen, die uns IDR zurückgibt
 * IDR wird negiert weil PULLUP-Mode(beim Drücken bekommt man ein 0 ) eingesetzt wurde und danach mit den getesteten Pin verglichen.
 * wenn das Ergebnis 0 ist, ist die Taste nicht gedruckt ansonnsten ist die Taste gedruckt und eine Nachricht wird auf den LCD gezeigt
 */
	while (1) {
		uint16_t IDR = joystick_read();
		if ((~IDR & A) != 0) {
			lcd_draw_text_at_line("Unten", 2, BLACK, 2, WHITE);
		}
		if ((~IDR & B) != 0) {
			lcd_draw_text_at_line("Links", 4, BLACK, 2, WHITE);
		}
		if ((~IDR & C) != 0) {
			lcd_draw_text_at_line("Rechts", 6, BLACK, 2, WHITE);
		}
		if ((~IDR & D) != 0) {
			lcd_draw_text_at_line("Oben", 8, BLACK, 2, WHITE);
		}

		HAL_Delay(100);

	}
}

