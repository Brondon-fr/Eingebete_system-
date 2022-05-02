/**
  ******************************************************************************
  * @file    	main.c
  * @author		Omar Alhammoud, Franck Brondon Mezatio Felefack
  * @version 	V1.0
  * @date		11.11.2021
  * @brief  	Template using the LCD
  ******************************************************************************
*/


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <Potentiometer_DMA/potis_DMA.h>

int main(void)
{

	HAL_Init();

	uint32_t pData1[100];
	uint32_t pData2[100];
	uint16_t mittelwert1 = 0;
	uint16_t mittelwert2 = 0;
	char buf1[32];
	char buf2[32];


	potis_DMA_init();
	/* Initialization of the LCD */
	lcd_init();

	while(1) {
		start_adc_dma(1, pData1, 100);
		mittelwert1 = getMittelWert(pData1);
		sprintf(buf1, "adc1 %4ld", mittelwert1);
		lcd_draw_text_at_line(buf1, 1, BLACK, 3, WHITE);

		start_adc_dma(2, pData2, 100);
		mittelwert2 = getMittelWert(pData2);
		sprintf(buf2, "adc2 %4ld", mittelwert2);
		lcd_draw_text_at_line(buf2, 2, BLACK, 3, WHITE);

	}
}
