/*
 * timer.h
 *
 *  Created on: 22.11.2021
 *      Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
 */

#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

void tim_prescealer(uint32_t pre_value);
float map(float value, float istart, float istop, float ostart, float ostop);
void set_compare(float pulseWert);
void timer_start (void);

#endif /* TIMER_TIMER_H_ */
