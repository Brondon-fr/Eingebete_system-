/*
 * stopwatch.c
 *
 *  Created on: 07.12.2021
 *      Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
 *
 * *      @verbatim

 =============================================================================================
 	 	 	 	 	 	 #### Ressources used ####

  (#) GPIOG => GPIO_PIN_12, Timer 2, EXTI 12
 =============================================================================================
						#### Usage: ####
	(#) call 'stopwatch_init' 		Funktion für die initialisierung des Stopwatches
	(#) call 'uint16_t get_tastendruck(void)'	Funktion, um den Wert der Tastendruck-Anzahl zu bekommen
	(#) call 'stopwatch_time_t get_zwischenzeit(uint16_t tastendruck)' 	Funktion, um
	 bestimmte gespeicherte ( bei Tastendruck ) Zwischenzeit  zu bekommen.


 =============================================================================================
 @endverbatim
 *********************************************************************************************
 */
#include "stm32f4xx.h"
#include<Timer/timer.h>
#include "stopwatch.h"
#include<Luefter_Controller/luefter.h>

static void stopwatch_gpio_init(void);
static void stopwatch_timer_init(void);
static void stopwatch_start(void);
static void stopwatch_time(void);
static void add_zwischenzeit(void);

// stopwatch_time_t aktulZeit;

/* wir benutzen array groesse 10 für den Fall,
 * dass wir die letzten 9 zwischenzeiten speichern und ausgeben wollen
 */
static  stopwatch_time_t zwischenzeit[10];

/* @brief Funktion, um den Stopwatch-Timer zu initialisieren
 * @param : none
 * @retval : none
 */
 void stopwatch_init(void){

	stopwatch_gpio_init();

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	stopwatch_timer_init();

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 6);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
 /* @brief statische Funktion, um den pin von der taste zu initialisieren ( press Button von joystick )
  * @param : none
  * @retval : none
  */
static void stopwatch_gpio_init(void){
	__HAL_RCC_GPIOG_CLK_ENABLE();

	GPIO_InitTypeDef GPIOG_init_structure;

	GPIOG_init_structure.Pin = GPIO_PIN_12;
	GPIOG_init_structure.Mode = GPIO_MODE_IT_FALLING;
	GPIOG_init_structure.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOG, &GPIOG_init_structure);
}
/* @brief statische Funktion, um den Timer 2 zu initialisieren
  * @param : none
  * @retval : none
  */
static void stopwatch_timer_init(void){
	__HAL_RCC_TIM2_CLK_ENABLE();

	tim2_handle_struct.Instance = TIM2;

	tim2_handle_struct.Init.Prescaler = (SystemCoreClock/40000)-1;
	tim2_handle_struct.Init.Period = 2 - 1;
	tim2_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim2_handle_struct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	tim2_handle_struct.Init.RepetitionCounter = 0;

	HAL_TIM_Base_Init(&tim2_handle_struct);
}
/* @brief statische Funktion, um die Zeit zu speichern.
 * @note diese funktion wird bei jedem Timer ueberlauf von der funktion
 * HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) aufgerfen.
  * @param : none
  * @retval : none
  */
static void stopwatch_time(void){
	if (aktulZeit.microS >= 9999) {
		aktulZeit.microS = 0;
		if (aktulZeit.s >= 59) {
			aktulZeit.s = 0;
			if (aktulZeit.m >= 59) {
				aktulZeit.m = 0;

				if (aktulZeit.h >= 23) {
					aktulZeit.h = 0;
				}else{
					aktulZeit.h += 1;
				}
			} else {
				aktulZeit.m += 1;
			}

		} else {
			aktulZeit.s += 1;
		}

	} else {
		aktulZeit.microS += 1;
	}

}
/* @brief  Funktion, um die Anzahl der Tastendruecke zu bekommen.
  * @param : none
  * @retval : Anzahl der Tastendruecke
  */
uint16_t get_tastendruck(void){
	return tastendruck;
}
/* @brief  Funktion, um bestimmte gespeicherte ( bei Tastendruck ) Zwischenzeit  zu bekommen.
  * @param : none
  * @retval : bestimmte gespeicherte ( bei Tastendruck ) Zwischenzeit
  */
stopwatch_time_t get_zwischenzeit(uint16_t tastendruck){
	return zwischenzeit[tastendruck];
}
/* @brief statische Funktion, um die aktuelle Zeit  zu speichern.
 * @note diese funktion wird bei jedem tastendruck von der Funktion
 *  HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) aufgerfen. Außer dem ersten Tastendruck da wird
 *   der Timer 2 gestartet.
 * @param : none
 * @retval : none
 */
 static void add_zwischenzeit(void){
	tastendruck++;
	if(tastendruck >= 10){
		tastendruck = 1;     //bei tastendruck=0 wird in Funktion HAL_GPIO_EXTI_Callback den Timer gestartet
	}
	zwischenzeit[tastendruck].microS = aktulZeit.microS;
	zwischenzeit[tastendruck].s = aktulZeit.s;
	zwischenzeit[tastendruck].m = aktulZeit.m;
	zwischenzeit[tastendruck].h = aktulZeit.h;

}
/* @brief statische Funktion, um den Timer 2 zu starten.
 * @note diese funktion wird bei dem ersten tastendruck von der Funktion
 *  HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) aufgerfen.
 * @param : none
 * @retval : none
 */
static void stopwatch_start(void) {
	HAL_TIM_Base_Start_IT(&tim2_handle_struct);
}
/* @brief  Funktion, um den Timer-Interrupt zu behandeln.
 * @note diese funktion wird von der Funktion TIM2_IRQHandler aufgerfen.
 * @param : *htim da wird gegeben, welche interrupt-Quelle diese Interrupt ausgelöst hat.
 * @retval : none
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &tim2_handle_struct) {
		// do some stuff
		stopwatch_time();


	}
}
/* @brief  Funktion, um den Tastendruck-Interrupt zu behandeln.
 * @note diese funktion wird von der Funktion EXTI15_10_IRQHandler aufgerfen.
 * @param : GPIO_Pin da wird gegeben, welche interrupt-Quelle diese Interrupt ausgelöst hat.
 * @retval : none
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case GPIO_PIN_12:
		if(tastendruck== 0){
			stopwatch_start();
			tastendruck++;
		}else{
			add_zwischenzeit();
		}
	case GPIO_PIN_11: // das ist für Luefterregelung
				tacho_impulse_interrupt();
			break;
	break;
	default:
		break;
	}
}
