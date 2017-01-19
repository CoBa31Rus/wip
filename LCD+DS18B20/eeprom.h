/*
 * eeprom.h
 *
 *  Created on: 13 янв. 2017 г.
 *      Author: coba31rus
 */

#ifndef EEPROM_H_
	#define EEPROM_H_
	#include <avr/eeprom.h>


	void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
	unsigned char EEPROM_read_char(unsigned int uiAddress);
	//int EEPROM_read_int(unsigned int uiAddress);

#endif /* EEPROM_H_ */
