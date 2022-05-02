/*
 * potis.h
 *
 *  Created on: 03.11.2021
 *      Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
 *
 */

#ifndef POTENTIOMETER_POTIS_H_
#define POTENTIOMETER_POTIS_H_
#include "stm32f4xx.h"

void potis_init(void);
uint32_t potis_getVal(uint8_t poti_num);
void Draw_Baragraph(uint16_t X0, uint16_t Y0, uint16_t Width, uint16_t High, uint16_t Value, uint16_t Color, uint16_t bgcolor);
void potis_gpio_init(void);
#endif /* POTENTIOMETER_POTIS_H_ */
