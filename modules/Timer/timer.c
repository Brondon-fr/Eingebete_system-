/*
 * timer.c
 *
 *  Created on: 22.11.2021
 *      Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
 *      @brief Module für die Nutzung des Timers 1
 *
 *      @verbatim

 =============================================================================================
 	 	 	 	 	 	 #### Ressources used ####

  (#) GPIOE => GPIO_PIN_11, GPIOE => GPIO_PIN_(0, 1, 14, 15), Timer 1 ,
      Timer1-channel 2, Potentiometer 1 und 2
 =============================================================================================
						#### Usage: ####

	(#) call 'tim_prescealer(uint32_t pre_value)' 	Funktion, um die prescealer zur Laufzeit zu verändern
	(#) call 'set_compare(float pulseWert)' Funktion, um die Output_Compare-Wert zur Laufzeit zu verändern
	(#) call 'float map(float value, float istart, float istop, float ostart, float ostop)'	Funktion,
	 um die wertebereich von [0-4095] zu[1-10] zu verändern
	(#) call 'timer_potis_start' 	indieser Funktion weren die Funktionen ( pin_dot_init(), gpio_init(),
	 potis_init(),, timer_init(), start_timer(), oc_init(), oc_start() ) aufgerufen um denTimer
	 mit Potentiometer zu starten.

 =============================================================================================
 @endverbatim
 *********************************************************************************************
 */

#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "timer.h"

TIM_HandleTypeDef tim_handle_struct;
static void timer_init(void);
static void start_timer(void);
static void oc_init(void);
static void oc_start(void);
static void pin_dot_init(void);

/* @brief Funktion, um den Timer zu initialisieren
 * @param : none
 * @retval : none
 */
static void timer_init(void){
	__HAL_RCC_TIM1_CLK_ENABLE();

	tim_handle_struct.Instance = TIM1;
	tim_handle_struct.Init.Prescaler = (SystemCoreClock / 10000) - 1;
	tim_handle_struct.Init.Period = 10000 - 1;
	tim_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_handle_struct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	tim_handle_struct.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&tim_handle_struct);

}

/* @brief Funktion, um den Timer zu starten
 * @param : none
 * @retval : none
 */
static void start_timer(void){
	HAL_TIM_Base_Start(&tim_handle_struct);
}

/* @brief Funktion, um die Output_Compare Channel zu initialisieren
 * @param : none
 * @retval : none
 */
void oc_init(void){
	TIM_OC_InitTypeDef tim_oc_handle_struct;
	tim_oc_handle_struct.OCMode = TIM_OCMODE_PWM1;
	tim_oc_handle_struct.Pulse = 5000;
	tim_oc_handle_struct.OCIdleState = TIM_OCIDLESTATE_SET;
	tim_oc_handle_struct.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim_oc_handle_struct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	tim_oc_handle_struct.OCNPolarity = TIM_OCNPOLARITY_LOW;
	tim_oc_handle_struct.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_OC_ConfigChannel(&tim_handle_struct, &tim_oc_handle_struct,
	TIM_CHANNEL_2);
}

/* @brief Funktion, um den output-Compare zu starten
 * @param : none
 * @retval : none
 */
static void oc_start(void){
	HAL_TIM_OC_Start(&tim_handle_struct, TIM_CHANNEL_2);
}
/* @brief Funktion, um die benötigten Pins zu konfigurieren und initialisieren ( port E : pin 11,
 *  Port D : pins (0,1,14,15)
 * @param : none
 * @retval : none
 */
static void pin_dot_init(void){
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init_structure;
	GPIO_init_structure.Pin = GPIO_PIN_11;
	GPIO_init_structure.Mode = GPIO_MODE_AF_PP;
	GPIO_init_structure.Pull = GPIO_NOPULL;
	GPIO_init_structure.Speed = GPIO_SPEED_MEDIUM;
	GPIO_init_structure.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOE, &GPIO_init_structure);
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init_structure2;
	GPIO_init_structure2.Pin = GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0
			| GPIO_PIN_1;
	GPIO_init_structure2.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init_structure2.Pull = GPIO_NOPULL;
	GPIO_init_structure2.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOD, &GPIO_init_structure2);
	HAL_GPIO_WritePin(GPIOD,
	GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_SET);

}
/* @brief Funktion, um den prescealer-Wert zur Laufzeit zu verändern
 * @param : pre_value aendert die zaehltakt dann wird den prescealer-wert geaendert.
 * @retval : none
 */
void tim_prescealer(uint32_t pre_value){
	__HAL_TIM_SET_PRESCALER(&tim_handle_struct, (SystemCoreClock/pre_value)-1);
}

/* @brief diese Methode wandelt die Value die jetzt in der Wertebereich
 *  von 0 bin 4095 ist, in einer Value der Wertebereich 1 bis 10

 * @param : value ist der potentiometer- Wert, istart ist Anfang des ausgang-Wertebereichs,
 * istop ist Ende des ausgang-Wertebereichs, ostart ist Anfang des Ziel-Wertebereichs,
 *  ostop ist Ende des Ziel-Wertebereichs
 * @retval : float rueckgabewert ist vom Typ float
 */
  float map(float value, float istart, float istop, float ostart, float ostop) {
		return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
		}
  /* @brief Funktion, um den pulse-Wert zu verändern
   * @param : pulseWert ist der neue oc-Wert
   * @retval : none
   */
  void set_compare(float pulseWert){
	  __HAL_TIM_SET_COMPARE(&tim_handle_struct, TIM_CHANNEL_2, pulseWert);
  }
  /* @brief Funktion, um den Timer zu starten
   * @param : none
   * @retval : none
   */
  void timer_start (void){

	  pin_dot_init();
	  	timer_init();
	  	start_timer();
	  	oc_init();
	  	oc_start();
  }
