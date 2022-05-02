/*
 * potis_DMA.h
 *
 *  Created on: 11.11.2021
 *      Author:  Omar Alhammoud, Franck Brondon Mezatio Felefack
 */

#ifndef POTENTIOMETER_DMA_POTIS_DMA_H_
#define POTENTIOMETER_DMA_POTIS_DMA_H_
void potis_DMA_init(void);
void start_adc_dma(uint8_t poti_num, uint32_t* pData,uint32_t Length);
void gpio_Init(void);
uint32_t getMittelWert(uint16_t* pData);

#endif /* POTENTIOMETER_DMA_POTIS_DMA_H_ */
