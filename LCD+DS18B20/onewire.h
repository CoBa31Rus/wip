/*
 * 1wire.h
 *
 *  Created on: 08.11.2016
 *      Author: coba31rus
 */

#ifndef ONEWIRE_H_
	#define ONEWIRE_H_

	#include <avr/io.h>
	#include <util/delay.h>

	#define ONEWIRE_PORT PORTC
	#define ONEWIRE_DDR DDRC
	#define ONEWIRE_PIN PINC
	#define ONEWIRE_PIN_NUM PC0
	#define ONEWIRE_LOW ONEWIRE_DDR |= (1<<ONEWIRE_PIN_NUM)
	#define ONEWIRE_HIGH ONEWIRE_DDR &= ~(1<<ONEWIRE_PIN_NUM)
	#define ONEWIRE_LEVEL ONEWIRE_PIN & (1 << ONEWIRE_PIN_NUM)

	#define SEI asm("sei")
	#define CLI asm("cli")

	#define delay_us(_data)	_delay_us(_data)
	#define delay_ms(_data)	_delay_ms(_data)

	int readtt_2(void);

#endif /* 1WIRE_H_ */
