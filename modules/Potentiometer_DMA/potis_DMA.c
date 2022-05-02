/*
 * potis_DMA.c
 *
 *  Created on: 11.11.2021
 *      Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
 *
 *  @brief Module für die Nutzung der zwei Potentiometer mithilfe von Dma
 @verbatim

 =============================================================================================
 	 	 	 	 	 	 #### Ressources used ####

	(#) GPIOA, GPIO_PIN_6, GPIO_PIN_7, ADC1 und ADC2-Peripheriegerät sowie DMA2
 =============================================================================================
						#### Usage: ####
	(#) call 'potis_DMA_init' 		Funktion für die initialisierung des Moduls
	(#) call 'getMittelWert(uint16_t* pData)'	Funktion, die den Mittelwert für die 100
													von den ADCs eingelesenen Werten berechnet
	(#) call 'start_adc_dma(uint8_t poti_num, uint32_t* pData,uint32_t Length)' 	Funktion um die ADCs
																		mit dma zu starten

 =============================================================================================
 @endverbatim
 *********************************************************************************************
 */
#include "stm32f4xx.h"
#include "potis_DMA.h"

static void dma_init(uint8_t adc_num);
static void  adc_init(uint8_t adc_num);
static void channel_Config(uint8_t channel_num);

/****ADC HANDLE_STRUCTURE  ***/
ADC_HandleTypeDef ADC_handle_structure1;
ADC_HandleTypeDef ADC_handle_structure2;

/****ADC CHANNEL_STRUCTURE  ***/
ADC_ChannelConfTypeDef ADC_channel_structure1;
ADC_ChannelConfTypeDef ADC_channel_structure2;


/*****  DMA_structure *****/
DMA_HandleTypeDef DMA_handle_structure;
DMA_HandleTypeDef DMA_handle_structure2;
/**
 * @brief : Funktion für die initialisierung von dma, adcs und channels
 * @param : none
 * @retval : none
 */
void potis_DMA_init(void) {

	potis_gpio_Init();
	dma_init(1);
	dma_init(2);
	adc_init(1);
	channel_Config(7);
	adc_init(2);
	channel_Config(6);

}

/**
 * @brief : Funktion, um adc mit dma zu starten
 * @param : poti_num ist die Potentiometernummer, pData ist der Speicherort wo die Daten gespeichert werden,
 *  Length ist die Länge der vom ADC-Peripheriegerät an den Speicher zu übertragenden Daten
 * @retval : none
 */
void start_adc_dma(uint8_t poti_num, uint32_t *pData, uint32_t Length) {
	if (poti_num == 1) {
		HAL_ADC_Start_DMA(&ADC_handle_structure1, pData, Length);

	} else if (poti_num == 2) {
		HAL_ADC_Start_DMA(&ADC_handle_structure2, pData, Length);

	}
}
/**
 * @brief : Funktion, die den Mittelwert für die 100 von den ADCs eingelesenen Werten berechnet
 * @param : pData ist der Speicherort, wo die Daten gespeichert sind
 * @retval : uint32_t gibt den Mittelwert zurueck
 */
uint32_t getMittelWert(uint16_t *pData) {
	uint32_t erg = 0;
	for (int i = 0; i < 100; i++) {
		erg += pData[i];
	}
	//ungenau
	erg /= 100;

	//genauer
	//erg /= 25;

	return erg;
}

/**
 * @brief : Funktion für die initialisierung von dma
 * @param : adc_num ist adc-Nummer
 * @retval : none
 */
void dma_init(uint8_t adc_num){
	__HAL_RCC_DMA2_CLK_ENABLE();
	if (adc_num == 1) {
		DMA_handle_structure.Instance = DMA2_Stream0;
		DMA_handle_structure.Init.Channel = DMA_CHANNEL_0;
		DMA_handle_structure.Init.Direction = DMA_PERIPH_TO_MEMORY;
		DMA_handle_structure.Init.PeriphInc = DMA_PINC_DISABLE;
		DMA_handle_structure.Init.MemInc = DMA_MINC_ENABLE;
		DMA_handle_structure.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		DMA_handle_structure.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		DMA_handle_structure.Init.Mode = DMA_CIRCULAR;
		DMA_handle_structure.Init.Priority = DMA_PRIORITY_HIGH;
		DMA_handle_structure.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		HAL_DMA_Init(&DMA_handle_structure);
	} else if (adc_num == 2) {
		DMA_handle_structure2.Instance = DMA2_Stream2;
		DMA_handle_structure2.Init.Channel = DMA_CHANNEL_1;
		DMA_handle_structure2.Init.Direction = DMA_PERIPH_TO_MEMORY;
		DMA_handle_structure2.Init.PeriphInc = DMA_PINC_DISABLE;
		DMA_handle_structure2.Init.MemInc = DMA_MINC_ENABLE;
		DMA_handle_structure2.Init.PeriphDataAlignment =
				DMA_PDATAALIGN_HALFWORD;
		DMA_handle_structure2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		DMA_handle_structure2.Init.Mode = DMA_CIRCULAR;
		DMA_handle_structure2.Init.Priority = DMA_PRIORITY_HIGH;
		DMA_handle_structure2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		HAL_DMA_Init(&DMA_handle_structure2);
	}
}
/**
 * @brief : Funktion für die initialisierung von adc
 * @param : adc_num ist adc-Nummer
 * @retval : none
 */
void  adc_init(uint8_t adc_num){
	if (adc_num == 1) {
		__HAL_RCC_ADC1_CLK_ENABLE();
		ADC_handle_structure1.Instance = ADC1;
		ADC_handle_structure1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
		ADC_handle_structure1.Init.Resolution = ADC_RESOLUTION_12B;
		ADC_handle_structure1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		ADC_handle_structure1.Init.ScanConvMode = DISABLE;
		ADC_handle_structure1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
		ADC_handle_structure1.Init.ContinuousConvMode = ENABLE;
		ADC_handle_structure1.Init.NbrOfConversion = 1;
		ADC_handle_structure1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		ADC_handle_structure1.Init.DMAContinuousRequests = ENABLE;
		ADC_handle_structure1.DMA_Handle = &DMA_handle_structure;

		HAL_ADC_Init(&ADC_handle_structure1);
	} else if (adc_num == 2) {
		__HAL_RCC_ADC2_CLK_ENABLE();
		ADC_handle_structure2.Instance = ADC2;
		ADC_handle_structure2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
		ADC_handle_structure2.Init.Resolution = ADC_RESOLUTION_12B;
		ADC_handle_structure2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		ADC_handle_structure2.Init.ScanConvMode = DISABLE;
		ADC_handle_structure2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
		ADC_handle_structure2.Init.ContinuousConvMode = ENABLE;
		ADC_handle_structure2.Init.NbrOfConversion = 1;
		ADC_handle_structure2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		ADC_handle_structure2.Init.DMAContinuousRequests = ENABLE;
		ADC_handle_structure2.DMA_Handle = &DMA_handle_structure2;
		HAL_ADC_Init(&ADC_handle_structure2);
	}
}
/**
 * @brief : Funktion für die initialisierung von adc-channels
 * @param : channel_num ist Channel-Nummer
 * @retval : none
 */
void channel_Config(uint8_t channel_num){
	if (channel_num == 6) {
		ADC_channel_structure2.Channel = ADC_CHANNEL_6;
		ADC_channel_structure2.Rank = 1;
		ADC_channel_structure2.SamplingTime = ADC_SAMPLETIME_84CYCLES;

		HAL_ADC_ConfigChannel(&ADC_handle_structure2, &ADC_channel_structure2);
	} else if (channel_num == 7) {
		ADC_channel_structure1.Channel = ADC_CHANNEL_7;
		ADC_channel_structure1.Rank = 1;
		ADC_channel_structure1.SamplingTime = ADC_SAMPLETIME_84CYCLES;
		HAL_ADC_ConfigChannel(&ADC_handle_structure1, &ADC_channel_structure1);
	}
}
void potis_gpio_Init(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init_structure;
	GPIO_init_structure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_init_structure.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &GPIO_init_structure);
}
