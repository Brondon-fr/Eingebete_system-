/*
 * WS2812B_led.h
 *
 *  Created on: 16.02.2022
 *     Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
 */

#ifndef WS2812B_LED_WS2812B_LED_H_
#define WS2812B_LED_WS2812B_LED_H_

DMA_HandleTypeDef DMA_handle_structure;
TIM_HandleTypeDef tim_handle_struct;

void Set_LED (int LEDnum, int Red, int Green, int Blue);
void Set_All_LED (int Red, int Green, int Blue);
void WS2812b_led_Send (void);
void led_init(void);


#endif /* WS2812B_LED_WS2812B_LED_H_ */
