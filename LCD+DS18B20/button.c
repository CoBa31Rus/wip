/*
 * button.c
 *
 *  Created on: 14 янв. 2017 г.
 *      Author: coba31rus
 */

#include "button.h"

uint8_t button_tmp = KEY_UNP;

uint8_t pushedButton(char data){
	if (data==KEY_UNP)
		button_tmp = KEY_UNP;
		return KEY_UNP;
	if (data == button_tmp){
		return data;
		button_tmp = KEY_UNP;
	}
	if (data != KEY_UNP){
		button_tmp = data;
		return KEY_UNP;
	}
	return KEY_UNP;
}
