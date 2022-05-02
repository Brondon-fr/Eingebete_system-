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
	/* damit die LEDs besser dimmen, lassen wir den Timer schneller laufen, indem wir
	den prescealer-wert aendern. man kann auch die periode von timer.c aendern
	 */
	tim_prescealer(80000);

	while(1) {
		uint32_t poti_value = potis_getVal(1);

//------------- Aufgabe 4-----------

	float map_value = map(poti_value,0,4095,1,10000);
	 set_compare(map_value);

//---------------------------------------------

	}
}
