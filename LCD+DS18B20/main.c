/*
 * main.c
 *
 *  Created on: 07.11.2016
 *      Author: coba31rus
 */
#include "lcd.h"
#include "onewire.h"
#include <stdio.h>
//#include <avr/iom8.h>

#define F_CPU 8000000UL

int t;
char high,low;
char hstr[15], lstr[15];

void sysinit(void){
	lcd_init();
	ONEWIRE_PORT &= ~_BV(ONEWIRE_PIN_NUM);

}

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

void printMainMenu(void){
	lcd_clear();
	lcd_str_out(hstr);
	lcd_gotoxy(0,1);
	lcd_str_out(lstr);
}

int main(void){
	sysinit();
	while(1){
		calcTemp();
		sprintf(hstr,"%X",t);
		sprintf(lstr,"%d.%d",high,low);
		printMainMenu();
		delay_ms(1000);
	}
}
