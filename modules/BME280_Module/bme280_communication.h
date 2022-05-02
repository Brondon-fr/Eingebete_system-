/*
 * bme280_communication.h
 *
 *  Created on: 24.02.2022
 *      Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
 */

#ifndef BME280_MODULE_BME280_COMMUNICATION_H_
#define BME280_MODULE_BME280_COMMUNICATION_H_
#include "stm32f4xx.h"

void i2c1_init(void);
void bme280_dev_init(void);
float temp, press, hum;

#endif /* BME280_MODULE_BME280_COMMUNICATION_H_ */
