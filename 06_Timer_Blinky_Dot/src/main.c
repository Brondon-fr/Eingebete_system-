/**
  ******************************************************************************
  * @file    	main.c
  * @author		Omar Alhammoud, Franck Brondon Mezatio Felefack
  * @version 	V1.0
  * @date		22.11.2021
  * @brief
  ******************************************************************************
*/


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include<Timer/timer.h>
#include <Potentiometer/potis.h>

int main(void)
{
	HAL_Init();

	potis_gpio_init();
	potis_init();

	timer_start();

	while(1) {
		uint32_t poti_value = potis_getVal(1);


//------------------ Aufgabe 3---------------

	 float map_value_pre = map(poti_value,0,4095,1,10);
	  tim_prescealer(map_value_pre);
//---------------------------------------------

	}
}
