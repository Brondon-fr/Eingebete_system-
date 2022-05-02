 /**
  * @file   median.c
  * @brief  Median-Filter zur Beseitigung von Datenausrei�ern
  *	@author	Michael Kreutzer
  * @version 1.0
  * @date	3.12.2012
  */

#include "median.h"
#include "stm32f4xx.h"
/**
  * @brief  Medianfilter zur Beseitigung von Datenausrei�ern.
  *
  * Die Funktion baut einen internen Ring-Puffer mit 9 Elementen auf,
  * dessen Inhalt in eine nach Elementgr��e sortierte Liste	�bertragen wird.
  * Das mittlere Element dieser Liste nennt man Median - dieses wird von der Funktion
  * als Filterergebnis zur�ck gegeben.
  * Da Datenausrei�er immer am oberen bzw. unteren Ende der sort. Liste stehen werden
  * diese heraus gefiltert.
  * Bsp: 3 - Element Mittelwert vs. Median
  *		- Eingabe:	[125 123 666]
  *		- Mittelwert: (125+123+666)/3 = 305 -> Ausrei�er 666 "verzieht" Mittelwert
  *		- Median:	123 - >>125<< - 666 = 125 -> Ausrei�er 666 eleminiert!
  *
  *	Anwendungsbeispiel:
  *		- gefilterteDrehzahl = MED_Median( Drehzahl );
  *		-> Datenausrei�er in "Drehzahl" werden beseitigt!
  *
  * @param  newElement:	Neuer Datenwert
  * @retval Median-gefilterter Wert.
  */
uint32_t MED_Median( uint32_t newElement )
{
	const uint16_t length = 9;				// Puffergr��e festlegen
	static uint32_t ringBuffer[9] = { 0 };	// mit 0,0,0,... initialisieren
	static uint16_t pos = 0;
	static uint32_t lastMedian = 0;
	uint32_t		median;				// Positionszeiger deklarieren
	uint32_t list[length];					// leere Datenstruktur f�r den sortierten Ring-Puffer

	// 1. neues Element in Ring-Puffer einf�gen
	ringBuffer[pos] = newElement;
	pos = (pos+1) % length;	// neuge Zeigerposition berechnen

	// 2. Ring-puffer sortieren
	MED_Sort( list, ringBuffer, length );

	// 3. Mittleres Element der sortierten Liste (=Median) zur�ck geben
	median = list[length/2];

	// 4. zus�tzlich, leichte Gl�ttung via Mittelwert-Filter
	median = (4*lastMedian + 1*median) / 5;
	lastMedian = median;

	return median;
}

/**
  * @brief  Sortieralgorithmus zur Sortierung einer ungeordneten Liste.
  * @param  list:	 Zeiger auf Ziel-Liste.
  * @param  srcList: Zeiger auf Quell-Liste.
  * @param	length	 L�nge der Liste(n).
  * @retval None
  */
void MED_Sort(uint32_t *list, uint32_t *srcList, uint16_t length)
{
	uint16_t i,j;
	uint32_t tmp;

	// 1. Quell-Liste kopieren
	for(j=0; j<length; j++)
	{
		list[j] = srcList[j];
	}

	// 2. Sortierung durchf�hren
	for(j=0; j<length; j++)
	{	// Bouble-Sort muss length mal aufgerufen werden,
		// um sicher zu stellen, dass die Liste auch im Worst-Case sortiert ist.

		for(i=0; i<(length-1); i++)
		{
			if( list[i] > list[i+1] )
			{	// Elemente tauschen, falls list[i] > list[i+1]
				tmp = list[i];
				list[i] = list[i+1];
				list[i+1] = tmp;
			}
		}	// Ende for i
	}	// Ende for j
}
