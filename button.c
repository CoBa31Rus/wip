/*
 * button.c
 *
 *  Created on: 14 џэт. 2017 у.
 *      Author: coba31rus
 */

#include "button.h"

unsigned char button_tmp = KEY_UNP;

unsigned char pushedButton(unsigned char data){
	if (data==KEY_UNP){
		button_tmp = KEY_UNP;
		return KEY_UNP;
	}
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

