/*
 * button.h
 *
 *  Created on: 14 ���. 2017 �.
 *      Author: coba31rus
 */

#ifndef BUTTON_H_
	#define BUTTON_H_

	#include <avr/io.h>
	#define KEY_UP			0b110
	#define KEY_DOW			0b011
	#define KEY_CEN			0b101
	#define KEY_UNP			0b111

	unsigned char pushedButton(unsigned char data);

#endif /* BUTTON_H_ */
