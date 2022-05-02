/*
 * median.h
 *
 *  Created on: 11.01.2022
 *      Author: omara
 */

#ifndef LUEFTER_CONTROLLER_MEDIAN_H_
#define LUEFTER_CONTROLLER_MEDIAN_H_
#include "stm32f4xx.h"

uint32_t MED_Median( uint32_t newElement );
void MED_Sort(uint32_t *list, uint32_t *srcList, uint16_t length);
#endif /* LUEFTER_CONTROLLER_MEDIAN_H_ */
