/*
 * 1wire.h
 *
 *  Created on: 08.11.2016
 *      Author: coba31rus
 */

#ifndef ONEWIRE_H_
	#define ONEWIRE_H_

	#include <avr/io.h>
	//#include <avr/iom8.h>
	#include <avr/pgmspace.h>
	#include <util/delay.h>

	#define ONEWIRE_PORT PORTC
	#define ONEWIRE_DDR DDRC
	#define ONEWIRE_PIN PINC
	#define ONEWIRE_PIN_NUM PC0

	#define delay_us(_data)	_delay_us(_data)
	#define delay_ms(_data)	_delay_ms(_data)

	uint8_t onewire_reset();
	void onewire_send(uint8_t b);
	uint8_t onewire_read();
	int readt(void);

#endif /* 1WIRE_H_ */
