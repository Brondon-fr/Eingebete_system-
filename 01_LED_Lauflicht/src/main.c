/**
  ******************************************************************************
  * @file    	main.c
  * @author		Omar Alhammoud, Franck Brondon Mezatio Felefack
  * @version 	V1.0
  * @date		14.03.2019
  * @brief  	arbeiten mit LED
  ******************************************************************************
*/


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <stdbool.h>
#define VCC1 GPIO_PIN_14
#define VCC2 GPIO_PIN_15
#define VCC3 GPIO_PIN_0
#define VCC4 GPIO_PIN_1
#define LED_A GPIO_PIN_7
#define LED_B GPIO_PIN_4
#define LED_C GPIO_PIN_5
#define LED_D GPIO_PIN_6
#define LED_E GPIO_PIN_12
#define LED_F GPIO_PIN_11
#define LED_G GPIO_PIN_12
#define LED_DOT GPIO_PIN_11
#define LED_POINT GPIO_PIN_7
void utils_gpioport_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t utils_gpioport_read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void utils_delay_ms(uint32_t t);
HAL_TickFreqTypeDef uwTickFreq2 = HAL_TICK_FREQ_DEFAULT;

int main(void)
{
	HAL_Init();
// RCC von den Port D und E aktivieren
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	// GPIO-Ports D und E Konfiguration
	GPIO_InitTypeDef GPIO_init_structure;
	GPIO_init_structure.Pin = VCC1 | VCC2 | VCC3 |VCC4 | LED_A| LED_B| LED_C| LED_D| LED_E| LED_F;
	GPIO_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init_structure.Pull = GPIO_NOPULL;
	GPIO_init_structure.Speed = GPIO_SPEED_MEDIUM;
	GPIO_InitTypeDef GPIO_init_structure2;
	GPIO_init_structure2.Pin = LED_G|LED_DOT|LED_POINT;
	GPIO_init_structure2.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init_structure2.Pull = GPIO_NOPULL;
	GPIO_init_structure2.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOD, &GPIO_init_structure);
	HAL_GPIO_Init(GPIOE, &GPIO_init_structure2);
    // CTRL2 Aktivieren
	HAL_GPIO_WritePin(GPIOD, VCC1 | VCC2 | VCC3 |VCC4   , GPIO_PIN_SET);

	//-----------------(5)-- Deaktivieren von B, D und F---------
	 HAL_GPIO_WritePin(GPIOD,LED_D |LED_B |LED_F , GPIO_PIN_SET);
	//----------------------------------------------------
	//HAL_GPIO_WritePin(GPIOD, LED_A |LED_C |LED_D |LED_E |LED_F, GPIO_PIN_SET);
	//utils_gpioport_write(GPIOD, LED_A, GPIO_PIN_SET);

	//HAL_GPIO_WritePin(GPIOE,LED_G |LED_DOT |LED_POINT , GPIO_PIN_SET);

	while(1) {

    //-------------(6) Toggeln alle LEDs mehrmals
	//	HAL_GPIO_TogglePin(GPIOD,  LED_A |LED_B |LED_C |LED_D |LED_E |LED_F);
	//	HAL_GPIO_TogglePin(GPIOE, LED_G |LED_DOT |LED_POINT);

//----------------7.1.1--(3) Lauflicht mit beiden Richtungen ----------------
		/**
		 * Lauflicht mit 8 Segment von Links nach Rechts (A->F) und wieder züruck von Recht nach Links (F->A)
		 */
			/**	HAL_GPIO_TogglePin(GPIOD,  LED_A );
				utils_delay_ms(1000);
				HAL_GPIO_TogglePin(GPIOD,  LED_B );
				utils_delay_ms(1000);
				HAL_GPIO_TogglePin(GPIOD,  LED_C );
		    	utils_delay_ms(1000);
		    	HAL_GPIO_TogglePin(GPIOD,  LED_D );
		    	utils_delay_ms(1000);
		    	HAL_GPIO_TogglePin(GPIOD,  LED_E );
				utils_delay_ms(1000);
		    	HAL_GPIO_TogglePin(GPIOD,  LED_F );
		    	utils_delay_ms(1000);
		    	HAL_GPIO_TogglePin(GPIOD,  LED_F );
		    	utils_delay_ms(1000);
		    	HAL_GPIO_TogglePin(GPIOD,  LED_E );
		    	utils_delay_ms(1000);
		    	HAL_GPIO_TogglePin(GPIOD,  LED_D );
		    	utils_delay_ms(1000);
		    	HAL_GPIO_TogglePin(GPIOD,  LED_C );
		    	utils_delay_ms(1000);
		        HAL_GPIO_TogglePin(GPIOD,  LED_B );
		    	utils_delay_ms(1000);
		    	HAL_GPIO_TogglePin(GPIOD,  LED_A );
		    	utils_delay_ms(1000);
		    	*/
//-----------------------------------------
	}
}
// ----------------(10)-------------
/**
 * @brief Mit diese Funktion schreibt man die Einzelnen Pin in der Output Data Register
 * @param GPIOx ist der Port an den die Daten geschrieben wenden
 * @param GPIO_Pin ist ein pin von dem Port
 */
void utils_gpioport_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	GPIOx->ODR = GPIO_Pin;

}
//----------------------------------------------(11)-------------

/**
 * @brief Mit diese Funktion liest man die Einzelnen Pin von dem input Data Register
 * @retval IDR (Input Data Register)
 * @param GPIOx ist der Port von dem die Daten gelesen wenden
 * @param GPIO_Pin ist ein pin von dem Port
 */
uint16_t utils_gpioport_read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	return GPIOx->IDR;
	  }
/**
 * @note diese Funktion erwartet parmeter in ms.
 *  um eine Sekunde zu warten, sollten Sie 1000 als Parameter eingeben.
 */
void utils_delay_ms(uint32_t t){
	 uint32_t tickstart = HAL_GetTick();
	  uint32_t wait = t;

	  /* Add a freq to guarantee minimum wait */
	  if (wait < HAL_MAX_DELAY)
	  {
	    wait += (uint32_t)(uwTickFreq2);
	  }

	  while((HAL_GetTick() - tickstart) < wait)
	  {
	  }
}
