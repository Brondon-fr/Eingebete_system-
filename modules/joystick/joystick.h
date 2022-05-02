/*
 * joystick.h
 *
 *  Created on: Oct 24, 2021
 *      Author: brondonmezatio, Omar Alhammoud
 *
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include "stm32f4xx.h"
#define A GPIO_PIN_6
#define B GPIO_PIN_9
#define C GPIO_PIN_10
#define D GPIO_PIN_11
#define CTR GPIO_PIN_12


void joystick_init(void);

uint16_t joystick_read(void);


#endif /* JOYSTICK_H_ */


