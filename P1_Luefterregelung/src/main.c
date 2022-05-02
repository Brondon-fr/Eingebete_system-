/**
  ******************************************************************************
  * @file    	main.c
  * @author		Omar Alhammoud, Franck Brondon Mezatio Felefack
  * @version 	V1.0
  * @date		18.12.2021
  * @brief der Luefter dreht in Geschwindigkeit von (1000-5000 rpm).
  ******************************************************************************
*/


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include<Luefter_Controller/luefter.h>
#include <Potentiometer/potis.h>
#include<Luefter_Controller/median.h>

int main(void)
{
	HAL_Init();

	potis_gpio_init();
	potis_init();

	esum = 0.0f;
	y = 0.0f;
	kp = 0.15f;
	ki = 0.01f;
	ta = 0.01;
	systick = HAL_GetTick();
	luefter_init();
	luefter_start();

	char buf[5];
	char buff[5];
	/* Initialization of the LCD */
	lcd_init();

	while(1) {
		uint16_t poti_value = potis_getVal(2);
		 sprintf(buff, "%4ld",poti_value);
		lcd_draw_text_at_line(buff, 2, BLACK, 2, WHITE);
		 uint16_t map_value = (uint16_t) luefter_map(poti_value,0,4095,1000,5000);
		medWert = MED_Median(frequenz_rpm);

		if (HAL_GetTick()-systick >= 10){
			systick = HAL_GetTick();

			luefter_PIRegler(map_value);
		}

		 sprintf(buf, "%4ld",medWert);
		 lcd_draw_text_at_line(buf, 4, BLACK, 2, WHITE);

	}
}
