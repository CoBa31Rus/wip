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
	unsigned char EEPROM_read(unsigned int uiAddress);


#endif /* EEPROM_H_ */
