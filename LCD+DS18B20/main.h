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
	#include "eeprom.h"
	#include "button.h"
	#include <avr/interrupt.h>
	#include "pid.h"

	#define F_CPU 8000000UL
	#define OVF_BUF_COUNT 2 //кол-во переполнений таймера для проверки клавиш
	#define HEATER OCR1A
	#define PID_OVF	200
	#define MENUCOUNT 4

#endif /* MAIN_H_ */
