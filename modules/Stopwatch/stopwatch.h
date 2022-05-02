/*
 * stopwatch.h
 *
 *  Created on: 07.12.2021
 *      Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
 */

#ifndef STOPWATCH_STOPWATCH_H_
#define STOPWATCH_STOPWATCH_H_

volatile uint16_t tastendruck;

TIM_HandleTypeDef tim2_handle_struct;

 typedef struct{
 	uint16_t microS;
 	uint8_t s;
 	uint8_t m;
 	uint8_t h;
 } stopwatch_time_t;
 volatile stopwatch_time_t aktulZeit;
 void stopwatch_init(void);
 stopwatch_time_t get_zwischenzeit(uint16_t tastendruck);
#endif /* STOPWATCH_STOPWATCH_H_ */
