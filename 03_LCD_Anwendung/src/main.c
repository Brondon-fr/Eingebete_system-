/**
  ******************************************************************************
  * @file    	main.c
  * @author		Omar Alhammoud, Franck Brondon Mezatio Felefack
  * @version 	V1.0
  * @date		14.03.2019
  * @brief  	Template using the LCD
  ******************************************************************************
*/

#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <stdio.h>
void MY_Draw_Filled_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1,
		uint16_t Y1, uint16_t Value, uint16_t Colour, uint16_t bgcolour);
void kreuz(void);

int main(void) {
	HAL_Init();
	//---------------------(2)------------
	char buf[64];
	/* Initialization of the LCD */
	lcd_init();

	//sprintf(buf, "Zahl = %d", 123);
	//lcd_draw_text_at_line(buf, 4, BLACK, 2, WHITE);
	//---------------------------(3)------------------------------
	/**
	 int i;
	 for(i=10;i>0;i--){
	 if(i>9){
	 sprintf(buf, "  %d", i);
	 lcd_draw_text_at_line(buf, 4, BLACK, 3, WHITE);
	 //		lcd_draw_text_at_coord(buf,4,4,BLACK,3,WHITE);
	 }else{
	 sprintf(buf, "  0%d", i);
	 lcd_draw_text_at_line(buf, 4, BLACK, 3, WHITE);
	 //	lcd_draw_text_at_coord(buf,4,4,BLACK,3,WHITE);
	 }
	 HAL_Delay(1000);
	 }
	 */
	//-----------------------
	// ILI9341_Draw_Filled_Rectangle_Coord(10,5,50,40,BLACK);
	//-----------------------------(4)-&-(5)--Rechteck als Countdown------------------

	 int i;
	 for(i=10;i>0;i--){
	 int j = i;
	 j = i*1024/10;
	 // sprintf(buf, "  %d", i);
	 // lcd_draw_text_at_coord(buf,4,4,BLACK,2,WHITE);
	 if(i>9){

	 // lcd_draw_text_at_line(buf, 4, BLACK, 2, WHITE);
	 //		lcd_draw_text_at_coord(buf,4,4,BLACK,3,WHITE);
	 MY_Draw_Filled_Rectangle_Coord(10,5,50,40,j,BLACK,BLUE);

	 }else{
	 MY_Draw_Filled_Rectangle_Coord(10,5,50,40,j,BLACK,BLUE);
	 //	 c
	 //	lcd_draw_text_at_coord(buf,4,4,BLACK,3,WHITE);
	 }
	 HAL_Delay(1000);
	 MY_Draw_Filled_Rectangle_Coord(10,5,50,40,j,WHITE,WHITE);

	 }

	//---------------(6) Kreuz -----------------

	 //kreuz();
//-------------------------------------------------

	while (1) {

	}
}
/**
 * @brief Die funktion zeichnet einen Rechteck als  Countdown mit zwei Farben,
 * @param X0 coordinate X und Y0 coordinate Y
 * @param X1 horizontale länge unserer Rechteck und Y1 ist die verticale Länge  unsere Rechteck
 * @param value soll ein Wert zwischen 1 und 1024 sein und gibt an, wie viel Anteil des Bargraphs proportional zum Wert width mit der Farbe color gefüllt werden soll
 * @param Colour ist die Farbe der normale Recteck
 * @param bgcolour ist die Farbe im Hintergrund
 *
 */
void MY_Draw_Filled_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Value, uint16_t Colour, uint16_t bgcolour) {
	uint16_t X_length = 0;
	uint16_t Y_length = 0;
	uint8_t Negative_X = 0;
	uint8_t Negative_Y = 0;
	int32_t Calc_Negative = 0;

	uint16_t X0_true = 0;
	uint16_t Y0_true = 0;
	uint16_t i = Value * 100 / 1024;
	Calc_Negative = ((X1 - X0) * i) / 100;
	if (Value < 1 || Value > 1024) {
		lcd_draw_text_at_line("Error: Value ungultig ", 4, BLACK, 1, WHITE);
	} else {

		if (Calc_Negative < 0)
			Negative_X = 1;
		Calc_Negative = 0;

		Calc_Negative = Y1 - Y0;
		if (Calc_Negative < 0)
			Negative_Y = 1;

		//DRAW HORIZONTAL!
		if (!Negative_X) {
			X_length = ((X1 - X0) * i) / 100;
			X0_true = X0;
		} else {
			X_length = (X0 - X1 * i) / 100;
			X0_true = X1;
		}

		//DRAW VERTICAL!
		if (!Negative_Y) {
			Y_length = Y1 - Y0;
			Y0_true = Y0;
		} else {
			Y_length = Y0 - Y1;
			Y0_true = Y1;
		}

		ILI9341_Draw_Rectangle(X0_true, Y0_true, X_length, Y_length, Colour);
		ILI9341_Draw_Rectangle(X0_true + X_length, Y0_true,X1- X_length, Y_length, bgcolour);
	}
}

/**
 * @brief die Funktion zeichnet  einen Kreuz mit den Maßen 100x100 Pixeln.
 *
 */

void kreuz(void){
	int i;
		for (i = 0; i < 100; i++) {
			lcd_draw_pixel(i, 50, BLACK);
			lcd_draw_pixel(50, i, BLACK);
			//  HAL_Delay(300);

		}
}

