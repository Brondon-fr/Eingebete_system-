/**
  ******************************************************************************
  * @file    	main.c
  * @author		Florian von Zabiensky
  * @version 	V1.0
  * @date		14.03.2019
  * @brief  	Template using the LCD
  ******************************************************************************
*/


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include<BME280_Module/bme280_communication.h>
#include<WS2812B_LED/WS2812B_led.h>
int main(void)
{
	HAL_Init();


	led_init();
	bme280_dev_init();
	float i =temp;
	 Set_All_LED(255,0,0);

	 WS2812b_led_Send();

	while(1) {

	}
}
