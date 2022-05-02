/**
  ******************************************************************************
  * @file    	main.c
  * @author		 Omar Alhammoud, Franck Brondon Mezatio Felefack
  * @version 	V1.0
  * @date		03.11.2021
  * @brief  	Potentiometer aktivieren und testen
  ******************************************************************************
*/


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <Potentiometer/potis.h>
uint32_t erg;
int  j;
uint32_t ergbnis;
int  f;
int main(void)
 {
	HAL_Init();

	potis_gpio_init();

	potis_init();
	lcd_init();

	char buf[64];

	while (1) {
		erg = potis_getVal(1);
		j = erg * 1023 / 4095 + 1;
		sprintf(buf, " %5d", erg);
		lcd_draw_text_at_line(buf, 1, BLACK, 3, WHITE);
		Draw_Baragraph(60, 5, 60, 30, j, BLACK, BLUE);

		ergbnis = potis_getVal(2);
		f = ergbnis * 1023 / 4095 + 1;
		sprintf(buf, " %5d", ergbnis);
		lcd_draw_text_at_line(buf, 5, BLACK, 3, WHITE);
		Draw_Baragraph(60, 100, 60, 30, f, BLACK, BLUE);

	}
}
