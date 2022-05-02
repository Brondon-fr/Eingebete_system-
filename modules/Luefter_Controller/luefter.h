/*
 * luefter.h
 *
 *  Created on: 18.12.2021
 *  Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
 *
*/


#ifndef LUEFTER_CONTROLLER_LUEFTER_H_
#define LUEFTER_CONTROLLER_LUEFTER_H_

// volatile uint16_t sollWert;
volatile uint32_t frequenz_rpm;
uint32_t medWert;
int32_t esum;
int32_t y;
float kp;
float ki;
float ta;
uint32_t zeit;
uint32_t systick;

void luefter_set_compare(uint32_t pulseWert);
void luefter_start(void);
void luefter_init(void);
void luefter_PIRegler(uint16_t sollWert);
void tacho_impulse_interrupt(void);
float luefter_map(float value, float istart, float istop, float ostart, float ostop);

#endif /* LUEFTER_CONTROLLER_LUEFTER_H_ */
