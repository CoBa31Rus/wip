/*
 * eeprom.c
 *
 *  Created on: 13 янв. 2017 г.
 *      Author: coba31rus
 */
#include "eeprom.h"

void EEPROM_write(unsigned int uiAddress, unsigned char ucData){
	while(EECR & (1<<EEWE));
	EEAR = uiAddress;
	EEDR = ucData;
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
}

unsigned char EEPROM_read_char(unsigned int uiAddress){
	while(EECR & (1<<EEWE));
	EEAR = uiAddress;
	EECR |= (1<<EERE);
	return EEDR;
}
/*
int EEPROM_read_int(unsigned int uiAddress){
	int tmp;
	tmp = (unsigned char) EEPROM_read_char(uiAddress);
	tmp << 4;
	tmp = (unsigned char) EEPROM_read_char(uiAddress + 1);

}
*/
