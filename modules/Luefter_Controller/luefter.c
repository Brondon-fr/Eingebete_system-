/*
 * luefter.c
 *
 *  Created on: 18.12.2021
 *  Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
 *
 *  @verbatim

 =============================================================================================
 	 	 	 	 	 	 #### Ressources used ####

  (#) GPIOC => GPIO_PIN_11, GPIOE => GPIO_PIN_11, Timer 1, Timer 2, EXTI 11
 =============================================================================================
						#### Usage: ####
	(#) call 'luefter_init' 		Funktion für die initialisierung des Luefters
	(#) call 'luefter_start'	Funktion, um den Luefter zu starten.
	(#) call 'luefter_PIRegler(uint16_t sollWert)' 	Funktion, um die
	 Geschwindigkeit des Luefters anzupassen.
	(#) call 'luefter_set_compare(uint32_t pulseWert)'	Funktion, um die
	 Geschwindigkeit des Luefters zu aendern (100 > pulseWert > 0).
	(#) call 'float luefter_map(float value, float istart, float istop, float ostart, float ostop)'
	Funktion, um die Wertebereich zu aendern.



 =============================================================================================
 @endverbatim
 *********************************************************************************************
 */

#include "stm32f4xx.h"
#include "luefter.h"

TIM_HandleTypeDef tim_handle_struct;
TIM_HandleTypeDef tim2_handle_struct;




static void timer2_init(void);
static void timer1_init(void);
static void start_timer1(void);
static void oc_init(void);
static void oc_start(void);
static void pin_tim1_init(void);
static void timer2_start(void);
static void Exti_gpio_init(void);
static void timer1_start (void);

/* @brief Funktion, um den Luefter_Controller zu initialisieren
 * @param : none
 * @retval : none
 */
void luefter_init(){
	pin_tim1_init();
	timer1_init();
	oc_init();
	Exti_gpio_init();
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	timer2_init();
}

/* @brief Funktion, um den Luefter_Controller zu starten.
 * @param : none
 * @retval : none
 */
void luefter_start(){
	timer1_start();
	timer2_start();
}

/* @brief Funktion, um den Timer 1 zu initialisieren
 * @param : none
 * @retval : none
 */
static void timer1_init(void){
	__HAL_RCC_TIM1_CLK_ENABLE();

	tim_handle_struct.Instance = TIM1;
	tim_handle_struct.Init.Prescaler = (SystemCoreClock / 2500000) - 1;
	tim_handle_struct.Init.Period = 100 - 1;
	tim_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_handle_struct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	tim_handle_struct.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&tim_handle_struct);

}

/* @brief Funktion, um den Timer 1 zu starten
 * @param : none
 * @retval : none
 */
static void start_timer1(void){
	HAL_TIM_Base_Start(&tim_handle_struct);
}

/* @brief Funktion, um die Output_Compare Channel von timer 1 zu initialisieren
 * @param : none
 * @retval : none
 */
static void oc_init(void){
	TIM_OC_InitTypeDef tim_oc_handle_struct;
	tim_oc_handle_struct.OCMode = TIM_OCMODE_PWM1;
	tim_oc_handle_struct.Pulse = 50;
	tim_oc_handle_struct.OCIdleState = TIM_OCIDLESTATE_SET;
	tim_oc_handle_struct.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim_oc_handle_struct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	tim_oc_handle_struct.OCNPolarity = TIM_OCNPOLARITY_LOW;
	tim_oc_handle_struct.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_OC_ConfigChannel(&tim_handle_struct, &tim_oc_handle_struct,
	TIM_CHANNEL_2);
}

/* @brief Funktion, um den output-Compare von timer 1 zu starten
 * @param : none
 * @retval : none
 */
static void oc_start(void){
	HAL_TIM_OC_Start(&tim_handle_struct, TIM_CHANNEL_2);
}

/* @brief Funktion, um den pin 11 auf Port E fuer Timer 1 zu initialisieren.
 * @param : none
 * @retval : none
 */
static void pin_tim1_init(void){
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init_structure;
	GPIO_init_structure.Pin = GPIO_PIN_11;
	GPIO_init_structure.Mode = GPIO_MODE_AF_OD;
	GPIO_init_structure.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOE, &GPIO_init_structure);
	__HAL_RCC_GPIOD_CLK_ENABLE();
}

/* @brief Funktion, um den pulse-Wert von timer 1 zu verändern
 * @param : pulseWert ist der neue oc-Wert
 * @retval : none
 */
void luefter_set_compare(uint32_t pulseWert){
	  __HAL_TIM_SET_COMPARE(&tim_handle_struct, TIM_CHANNEL_2, pulseWert);
}
/* @brief Funktion, um den Timer 1 mit Output_Compare Channel  zu starten
 * @param : none
 * @retval : none
 */
void timer1_start (void){

	  	start_timer1();
	  	oc_start();
}
/* @brief Funktion, um den pin für Exti zu initialisieren.
 * @param : none
 * @retval : none
 */
static void Exti_gpio_init(void){
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIOC_init_structure;

	GPIOC_init_structure.Pin = GPIO_PIN_11;
	GPIOC_init_structure.Mode = GPIO_MODE_IT_RISING;
	GPIOC_init_structure.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOC, &GPIOC_init_structure);
}

/**
 *  Diese Funktion wurde in stopwatch.c implementiert, da man sie
 *  nicht mehrfach implementieren darf. ich habe versucht, sie in main.c zu implementieren,
 *  haben aber immer error bekommen.
 *
 *
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case GPIO_PIN_11:
		tacho_impulse_interrupt();
	break;
	default:
		break;
	}
}
*/

/* @brief Funktion, um den Timer 2 zu initialisieren
 * @param : none
 * @retval : none
 */
static void timer2_init(){
	__HAL_RCC_TIM2_CLK_ENABLE();

		tim2_handle_struct.Instance = TIM2;

		tim2_handle_struct.Init.Prescaler = (SystemCoreClock/20000)-1;
		tim2_handle_struct.Init.Period = 0xFFFF;
		tim2_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
		tim2_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		tim2_handle_struct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		tim2_handle_struct.Init.RepetitionCounter = 0;

		HAL_TIM_Base_Init(&tim2_handle_struct);
}

/* @brief Funktion, um den Timer 2 zu starten
 * @param : none
 * @retval : none
 */
static void timer2_start(void) {
	HAL_TIM_Base_Start(&tim2_handle_struct);
}

/* @brief Funktion, um die zeit zwischen zwei tacho-Impulse zu messen
 * @param : none
 * @retval : none
 */
void tacho_impulse_interrupt(void){

	zeit=__HAL_TIM_GET_COUNTER(&tim2_handle_struct);
	__HAL_TIM_SET_COUNTER(&tim2_handle_struct,0);
   if( zeit>10){
	   frequenz_rpm= (60 *10000) /(2 * zeit);
   }
}

/* @brief Funktion, um den Timer 2 zu initialisieren
 * @param : sollWert representiert die Geschwindegkeit,auf die der Luefter laufeen
 *  soll (100 > sollWert > 0)
 * @retval : none
 */
void luefter_PIRegler(uint16_t sollWert){
	int32_t e = sollWert - medWert;
	esum += e;
	y = (kp * e) + (ki  * ta * esum);
	if (y > 100) {
		y = 100;
		esum = esum - e;
	}
	if (y < 20) {
		y = 20;
		esum = esum - e;
	}
	luefter_set_compare(y);
}
/* @brief Funktion, um den Wertebereich zu aendern.

 * @param : value ist der potentiometer- Wert, istart ist Anfang des ausgang-Wertebereichs,
 * istop ist Ende des ausgang-Wertebereichs, ostart ist Anfang des Ziel-Wertebereichs,
 *  ostop ist Ende des Ziel-Wertebereichs
 * @retval : float rueckgabewert ist vom Typ float
 */
  float luefter_map(float value, float istart, float istop, float ostart, float ostop) {
		return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
		}
