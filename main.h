/*
 * main.h
 *
 *  Created on: 17 янв. 2017 г.
 *      Author: coba31rus
 */

#ifndef MAIN_H_
	#define MAIN_H_
	#include "lcd.h"
	#include "onewire.h"
	#include <stdio.h>
	#include "button.h"
	#include <avr/interrupt.h>
	#include "pid.h"

	#define F_CPU 8000000UL
	#define OVF_BUF_COUNT 2 //кол-во переполнений таймера для проверки клавиш; count timer overflow for check key press
	#define LCD_LINE_SIZE 16
	#define TEMP_UPDATE 10
	#define HEATER OCR1A
	#define MENUCOUNT 4
	#define SCREEN_UPDATE 10 // SCREEN_UPDATE * 0.032 = Time (sec)

#endif /* MAIN_H_ */
