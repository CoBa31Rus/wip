/*
 * main.c
 *
 *  Created on: 07.11.2016
 *      Author: coba31rus
 */
#include "lcd.h"
#include "onewire.h"
#include <stdio.h>
#include "eeprom.h"

#define F_CPU 8000000UL

int t;
char high,low, hstr[15], lstr[15];

void sysinit(void){
	lcd_init();
	ONEWIRE_PORT &= ~_BV(ONEWIRE_PIN_NUM);
}

/*
void calcTemp(void){
	t = readt();
	unsigned char a=1;
	if(t & 0xF000){
		a = -1;
		t ^= 0xF000;
	}
	high = t/16*a;
	low = t%16;
}
*/

void printMainMenu(void){
	lcd_clear();
	lcd_str_out(hstr);
	lcd_gotoxy(0,1);
	lcd_str_out(lstr);
}

int main(void){
	sysinit();
	while(1){
		//calcTemp();
		t = readt();
		//sprintf(hstr,"%d.%04d",high,low*625);
		sprintf(hstr,"%d.%04d",(t>>4),(t%16)*625);
		hstr[5] = 0;
		hstr[6] = 0;
		printMainMenu();
		delay_ms(1000);
	}
}
