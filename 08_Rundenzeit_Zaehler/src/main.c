/**
  ******************************************************************************
  * @file    	main.c
  * @author		Omar Alhammoud, Franck Brondon Mezatio Felefack
  * @version 	V1.0
  * @date		07.12.2021
  * @brief
  ******************************************************************************
*/


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <Stopwatch/stopwatch.h>

int main(void)
{
	HAL_Init();

	char buf[64];
	char buffer[64];


	/* Initialization of the LCD */
	lcd_init();

	stopwatch_init();

	 stopwatch_time_t aktZeit = aktulZeit;
	stopwatch_time_t sw_time = get_zwischenzeit(tastendruck);
	while(1) {
		sprintf(buf, "%2d:%2d:%2d:%4d",aktZeit.h, aktZeit.m, aktZeit.s,  aktZeit.microS);
		lcd_draw_text_at_line(buf, 4, BLACK, 2, WHITE);
		sprintf(buffer, "%2d:%2d:%2d:%4d",sw_time.h, sw_time.m, sw_time.s,  sw_time.microS);
		lcd_draw_text_at_line(buffer, 8, BLACK, 2, WHITE);
		aktZeit = aktulZeit;
		sw_time = get_zwischenzeit(tastendruck);
	}
}
