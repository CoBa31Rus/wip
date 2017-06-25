/*
 * eeprom.h
 *
 *  Created on: 13 џэт. 2017 у.
 *      Author: coba31rus
 */

#ifndef EEPROM_H_
	#define EEPROM_H_
	#include <avr/eeprom.h>
	#define hibyte(a) ((a>>8) & 0xFF)
	#define lobyte(a) ((a) & 0xFF)

	void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
	unsigned char EEPROM_read_char(unsigned int uiAddress);
	int EEPROM_read_int(unsigned int uiAddress);
	void EEPROM_write_int(unsigned int uiAddress, int data);
#endif /* EEPROM_H_ */
