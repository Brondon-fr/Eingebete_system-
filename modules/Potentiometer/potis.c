/*
 * potis.c
 *
 *  Created on: 03.11.2021
 *      Author:  Omar Alhammoud, Franck Brondon Mezatio Felefack

@verbatim

 =============================================================================================
 	 	 	 	 	 	 #### Ressources used ####

	(#) GPIOA, GPIO_PIN_6, GPIO_PIN_7, ADC1 und ADC2-Peripheriegerät sowie DMA2
 =============================================================================================
						#### Usage: ####
	(#) call 'potis_init' 		Funktion für die initialisierung des Moduls
	(#) call 'potis_getVal(uint8_t poti_num)'	Funktion, um die Data vom DR zu lesen und zurueckgeben.
	(#) call 'Draw_Baragraph(uint16_t X0, uint16_t Y0, uint16_t Width, uint16_t High, uint16_t Value, uint16_t Color, uint16_t bgcolor)'
	      diese Funktion zeichnet den Wert als in Schwarz gemalte Anteil im Rechteck

 =============================================================================================
 @endverbatim

*/
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "potis.h"
/****ADC HANDLE_STRUCTURE  ***/
ADC_HandleTypeDef ADC_handle_structure1;
ADC_HandleTypeDef ADC_handle_structure2;

/****ADC CHANNEL_STRUCTURE  ***/
ADC_ChannelConfTypeDef ADC_channel_structure1;
ADC_ChannelConfTypeDef ADC_channel_structure2;
/**
*@brief : in dieser Funktion wird ADC1 und kanäle 6 und 7 konfiguriert und intialisiert.
* @param : none
* @retval none
 */
void potis_init(void){
	         /*  ADC1 Konfiguration  */
	        __HAL_RCC_ADC1_CLK_ENABLE();

	        ADC_handle_structure1.Instance = ADC1;
			ADC_handle_structure1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
			ADC_handle_structure1.Init.Resolution = ADC_RESOLUTION_12B;
			ADC_handle_structure1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
			ADC_handle_structure1.Init.ScanConvMode = ENABLE;
			ADC_handle_structure1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
			ADC_handle_structure1.Init.ContinuousConvMode = ENABLE;
			ADC_handle_structure1.Init.NbrOfConversion = 2;
			ADC_handle_structure1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	         /*  Konfiguration von kanal 6  */
			ADC_channel_structure1.Channel = ADC_CHANNEL_6;
			ADC_channel_structure1.Rank = 1;
			ADC_channel_structure1.SamplingTime = ADC_SAMPLETIME_84CYCLES;
			HAL_ADC_ConfigChannel(&ADC_handle_structure1, &ADC_channel_structure1);
			/*  Konfiguration von kanal 7  */
			ADC_channel_structure2.Channel = ADC_CHANNEL_7;
			ADC_channel_structure2.Rank = 2;
			ADC_channel_structure2.SamplingTime = ADC_SAMPLETIME_84CYCLES;

			HAL_ADC_ConfigChannel(&ADC_handle_structure1, &ADC_channel_structure2);
			/*  ADC1 Initialisieren */
			HAL_ADC_Init(&ADC_handle_structure1);
}
/**
*@brief : Funktion für die Data vom DR zu lesen und zurückzugeben
* @param : poti_num
* @retval uint32_t gibt die vom DR gelesene Daten zurueck
*/
uint32_t potis_getVal(uint8_t poti_num){
	HAL_StatusTypeDef ret;
	uint32_t ADC_val = 0;
	HAL_ADC_Start(&ADC_handle_structure1);
	ret = HAL_ADC_PollForConversion(&ADC_handle_structure1, 1000);
	if (ret != HAL_TIMEOUT) {
		ADC_val = HAL_ADC_GetValue(&ADC_handle_structure1);
	//	HAL_ADC_Stop(&ADC_handle_st00ructure1);

	}
	return ADC_val;
}

/**
*@brief : diese Funktion zeichnet den Wert als in Schwarz gemalte Anteil im Rechteck
* @param : X0 koordinaten x, Y0 koordinaten y, Width ist fuer die Breite, High fuer Hoehe, Value ist eint Wert
* zwischen 1 und 1024 bestimmt den Anteil, der in Schwarz gemalt werden soll, Color ( front-Farbe),
*  bgcolor ist die Hintergrundfarbe
* @retval none
*/
void Draw_Baragraph(uint16_t X0, uint16_t Y0, uint16_t Width, uint16_t High, uint16_t Value, uint16_t Color, uint16_t bgcolor){
	/*  Value-Wert in prozent */
	uint16_t i = Value * 100 / 1024;
	if (Value < 1 || Value > 1024) {
			lcd_draw_text_at_line("Error: Value ungultig ", 4, BLACK, 1, WHITE);
		} else {
		uint16_t FrontWidth = (Width * i) / 100;
		/* zeichnet den schwarzen Anteil */
		ILI9341_Draw_Rectangle(X0, Y0, FrontWidth, High, Color);
		/* zeichnet den Rest in Blau */
		ILI9341_Draw_Rectangle(X0 + FrontWidth, Y0, Width - FrontWidth, High,
				bgcolor);
		}

}
void potis_gpio_init(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_init_structure;
		GPIO_init_structure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_init_structure.Mode = GPIO_MODE_ANALOG;
		HAL_GPIO_Init(GPIOA, &GPIO_init_structure);
}
