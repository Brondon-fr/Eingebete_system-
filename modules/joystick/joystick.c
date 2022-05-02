/*
 * joystick.c
 *
 *  Created on: Oct 24, 2021
 *      Author: brondonmezatio, Omar Alhammoud
 */


#include "stm32f4xx.h"
#include <stdint.h>;
#include "joystick.h"
#include <lcd/lcd.h>

/**
 * @brief diese funktion wird benutzt, um rcc zu aktivieren und die Ports, die an joystick
 *  angeschlossen sind zu konfigurieren.
 */
void joystick_init(void){
	joystick_rcc_init();
	joystick_gpio_init();
}

/**
 * @brief RCC (Reset and Clock Control) wird aktiviert
  */
void joystick_rcc_init(){
	__HAL_RCC_GPIOG_CLK_ENABLE();
}
/**
 * @brief Port konfiguration
 */
 void joystick_gpio_init(){
	GPIO_InitTypeDef GPIO_init_structure;
	GPIO_init_structure.Pin =  A |B |C |D |CTR ;
	GPIO_init_structure.Mode = GPIO_MODE_INPUT;
	GPIO_init_structure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOG, &GPIO_init_structure);
}
 /**
  * @brief diese funktion wird benutzt, um von den Ports, die an joystick
  *  angeschlossen sind zu lesen.
  *  @retval IDR (Input Data Register)
  */
uint16_t joystick_read(void){
/*gibt IDR von Port G zurück*/
	return GPIOG->IDR;
}


