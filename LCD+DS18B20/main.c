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
#include "button.h"
#include <avr/interrupt.h>
#include "pid.h"

#define F_CPU 8000000UL
#define OVF_BUF_COUNT 3 //кол-во переполнений таймера для проверки клавиш
#define HEATER OCR1A

int real_temperature; //Переменная хранящая теипературу
int need_temperature = 500; //Установленная температура
char hstr[15], lstr[15]; //Строки выводимые на дисплей
volatile unsigned char buffer_overflow = 0, tmp_buttons, pushed_buttons;

unsigned char cou = 0; //Тестовая переменная

void sysinit(void){
	//Таймер обработчик клавиш
		//Настройка таймера
	TIMSK |= (1<<TOIE0);
	TCCR0 |= (1<<CS00)|(0<<CS01)|(1<<CS02);
		//Настройка порта на ввод
	DDRB &= ~1<<7;
	DDRB &= ~1<<6;
	DDRB &= ~1<<5;
	sei();
	//ШИМ
	DDRB|=0x02;
	TCCR1A|=(1<<COM1A1)|(1<<WGM10);
	TCCR1B|=(1<<WGM12)|(1<<CS12)|(0<<CS11)|(0<<CS10);
	HEATER=0;
	//Дисплей
	lcd_init();
	//Порт 1Wire
	ONEWIRE_PORT &= ~_BV(ONEWIRE_PIN_NUM);
}

ISR(TIMER0_OVF_vect){
	if (buffer_overflow >= OVF_BUF_COUNT){
		tmp_buttons = pushedButton(PINB>>5);
		if (tmp_buttons != KEY_UNP){
			pushed_buttons = tmp_buttons;
			if(pushed_buttons == KEY_UP){
				cou++;
				pushed_buttons = KEY_UNP;
			}
			if(pushed_buttons == KEY_DOW){
				cou--;
				pushed_buttons = KEY_UNP;
			}
			if(pushed_buttons == KEY_CEN){
				cou = 0;
				pushed_buttons = KEY_UNP;
			}
		}
		else
			pushed_buttons = KEY_UNP;
		buffer_overflow = 0;
	}
	else{
		buffer_overflow++;
	}
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
		real_temperature = readt();
		cou = update_pid(need_temperature, real_temperature);
		HEATER = cou;
		sprintf(hstr,"%d.%04d",(real_temperature>>4),(real_temperature%16)*625);
		hstr[5] = 0;
		hstr[6] = 0;
		sprintf(lstr, "%d", cou);
		printMainMenu();
		delay_ms(700);
	}
}
