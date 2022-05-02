/*
 * WS2812B-led.c
 *
 *  Created on: 16.02.2022
  *      Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
  *
  *
 *@note wir haben die Funktion HAL_TIM_PWM_Start_DMA(...); verwendet,
 *@note  weil wir den macro, worüber in praktikumanleitung gesprochen wird, nicht finden konnten.
 *@note  Um das in Praktikumanleitung erwähnte Probelem zu vermeiden, haben wir versucht,
 *@note  (tim_handle_struct.hdma[TIM_DMA_ID_CC3] = &DMA_handle_structure;) bei Timer2 zu addieren.
 *@note  Das Programm funktioniert immer noch nicht, da
 *@note   (HAL_TIM_PWM_PulseFinishedCallbac(TIM_HandleTypeDef *htim)) nicht aufgerufen wird.
 * *      @verbatim

 =============================================================================================
 	 	 	 	 	 	 #### Ressources used ####

  (#) GPIOA => GPIO_PIN_2, Timer 2, DMA1 Stream1 Channel3
 =============================================================================================
						#### Usage: ####
	(#) call 'Set_LED (int LEDnum, int Red, int Green, int Blue)' Funktion, um farbe in rgb für eine bestimmte led zu setzen
	(#) call 'Set_All_LED (int Red, int Green, int Blue)'	Funktion, um farbe in rgb für alle led zu setzen.
	(#) call 'WS2812b_led_Send' 	Funktion, um Farbdaten zu Led-Streifen zu senden.
	(#) call  'led_init'            Funktion, um die noetwendigen gpio, Timer und DMA zu initialisieren.


 =============================================================================================
 @endverbatim
 *********************************************************************************************
 */
#include "stm32f4xx.h"
#include "WS2812B_led.h"
#define MAX_LED 10


uint16_t pwmData[(24*MAX_LED)+240];

uint8_t LED_Data[MAX_LED][4];
int datasentflag = 0;

void led_gpio_init(void);
void led_dma_init(void);
void led_timer2_init(void);
void led_timer2_oc_init(void);


/*
 * @brief Funktion, um farbe in rgb für eine bestimmte led zu setzen
 * @param LEdnum ist die Lednummer.
 * @param Red, Green und Blue sind sind die Farben. Sie werden in RGB eingegeben.
 */
void Set_LED (int LEDnum, int Red, int Green, int Blue)
{
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = Green;
	LED_Data[LEDnum][2] = Red;
	LED_Data[LEDnum][3] = Blue;
}

/*
 * @brief Funktion, um farbe in rgb für alle led zu setzen
 * @param Red, Green und Blue sind sind die Farben. Sie werden in RGB eingegeben.
 */
void Set_All_LED (int Red, int Green, int Blue)
 {
	for (int i = 0; i < MAX_LED; i++) {

		LED_Data[i][0] = i;
		LED_Data[i][1] = Green;
		LED_Data[i][2] = Red;
		LED_Data[i][3] = Blue;
	}

}
/*
 * @brief Funktion, um Farbdaten zu Led-Streifen zu senden.
 */
void WS2812b_led_Send (void)
{
	uint32_t indx=0;
	uint32_t color;


	for (int i= 0; i<MAX_LED; i++)
	{
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));

		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				pwmData[indx] = 60;  // 2/3 of 90
			}
			else pwmData[indx] = 30;  // 1/3 of 90

			indx++;
		}
	}
	for (int i=0; i<240; i++)
	{
		pwmData[indx] = 0;
		indx++;
	}
	HAL_TIM_PWM_Start_DMA(&tim_handle_struct, TIM_CHANNEL_3, (uint32_t *)pwmData, indx);
	while (!datasentflag){};
	datasentflag = 0;
}

/*
 * @brief Funktion, um den timer und dma zu stopen.
 * sie wird aufgerufen, wenn die dma alle Daten gesendet hat. sie wird von
 *  HAL_TIM_IRQHandler(&tim_handle_struct); aufgerufen
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(&tim_handle_struct, TIM_CHANNEL_3);
	datasentflag=1;
}

/*
 * @brief Funktion, um die für die Led-Streifen noetwendigen gpio, Timer und DMA zu initialisieren.
 */
void led_init(void){
	led_gpio_init();
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	led_dma_init();
	led_timer2_init();
	led_timer2_oc_init();
}

/*
 * @brief Funktion, um die gpio für Led-Streifen zu initialisieren.
 */
void led_gpio_init(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init_structure;
	GPIO_init_structure.Pin = GPIO_PIN_2;
	GPIO_init_structure.Mode = GPIO_MODE_AF_PP;
	GPIO_init_structure.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_init_structure);
}
/*
 * @brief Funktion, um die dma zu initialisieren.
 */
void led_dma_init(void) {
	DMA_handle_structure.Instance = DMA1_Stream1;
	DMA_handle_structure.Init.Channel = DMA_CHANNEL_3;
	DMA_handle_structure.Init.Direction = DMA_MEMORY_TO_PERIPH;
	DMA_handle_structure.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_handle_structure.Init.MemInc = DMA_MINC_ENABLE;
	DMA_handle_structure.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	DMA_handle_structure.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	DMA_handle_structure.Init.Mode = DMA_NORMAL;
	DMA_handle_structure.Init.Priority = DMA_PRIORITY_HIGH;
	DMA_handle_structure.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&DMA_handle_structure);

}
/*
 * @brief Funktion, um die Timer2 zu initialisieren.
 */
void led_timer2_init(void) {
	__HAL_RCC_TIM2_CLK_ENABLE();

	tim_handle_struct.Instance = TIM2;
	tim_handle_struct.Init.Prescaler = ((SystemCoreClock/2) / (800000*90)) - 1;
	tim_handle_struct.Init.Period = 90 - 1;
	tim_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_handle_struct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	tim_handle_struct.Init.RepetitionCounter = 0;
	tim_handle_struct.hdma[TIM_DMA_ID_CC3] = &DMA_handle_structure;
	HAL_TIM_Base_Init(&tim_handle_struct);

}
/*
 * @brief Funktion, um Funktion, um die Output-Compare für Timer2 zu initialisieren..
 */
void led_timer2_oc_init(void) {
	TIM_OC_InitTypeDef tim_oc_handle_struct;
	tim_oc_handle_struct.OCMode = TIM_OCMODE_PWM1;
	tim_oc_handle_struct.Pulse = 20;
	tim_oc_handle_struct.OCIdleState = TIM_OCIDLESTATE_SET;
	tim_oc_handle_struct.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim_oc_handle_struct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	tim_oc_handle_struct.OCNPolarity = TIM_OCNPOLARITY_LOW;
	tim_oc_handle_struct.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_OC_ConfigChannel(&tim_handle_struct, &tim_oc_handle_struct,
	TIM_CHANNEL_3);
}
