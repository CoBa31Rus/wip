/*
 * main.c
 *
 *  Created on: 07.11.2016
 *      Author: coba31rus
 */
#include "main.h"


int real_temperature; //Переменная хранящая теипературу
int need_temperature = 600; //Установленная температура
char hstr[15], lstr[15], menu = 0, menu_sel = 0; //Строки выводимые на дисплей
volatile unsigned char buffer_overflow = 0, buffer_pid_ovf = 0, tmp_buttons, pushed_buttons;
unsigned char kp = 2, ki = 1, kd = 3;
int cou = 0; //Тестовая переменная

void sysinit(void){
	//Таймер обработчик клавиш
		//Настройка таймера
	TIMSK |= (1<<TOIE0);
	TCCR0 |= (1<<CS00)|(0<<CS01)|(1<<CS02); //0,03264 ms
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
	//ПИД регулятор
	pid_init(kp,ki,kd);
}

void key_plus(void){
	switch (menu){
		case 0:
			menu++;
			break;
		case 1:
			if(menu_sel)
				need_temperature++;
			else
				menu++;
			break;
		case 2:
			if(menu_sel)
				kp++;
			else
				menu++;
			break;
		case 3:
			if(menu_sel)
				ki++;
			else
				menu++;
			break;
		case 4:
			if(menu_sel)
				kd++;
			else
				menu++;
			break;
		case 5:
			if(menu_sel)
				buffer_pid_ovf++;
			else
				menu++;
			break;
	}
	if(menu > MENUCOUNT)
		if(!menu_sel)
			menu = 0;
}

void key_minus(void){
	switch (menu){
		case 0:
			menu--;
			break;
		case 1:
			if(menu_sel)
				need_temperature--;
			else
				menu--;
			break;
		case 2:
			if(menu_sel)
				kp--;
			else
				menu--;
			break;
		case 3:
			if(menu_sel)
				ki--;
			else
				menu--;
			break;
		case 4:
			if(menu_sel)
				kd--;
			else
				menu--;
			break;
		case 5:
			if(menu_sel)
				buffer_pid_ovf--;
			else
				menu--;
			break;
	}
	if(menu > MENUCOUNT)
		if(!menu_sel)
			menu = MENUCOUNT;
}

ISR(TIMER0_OVF_vect){

	if (buffer_overflow >= OVF_BUF_COUNT){ //Срабатывание 0.03264 * OVF_BUF_COUNT ms
		real_temperature = readt();
		tmp_buttons = pushedButton(PINB>>5);
		if (tmp_buttons != KEY_UNP){
			pushed_buttons = tmp_buttons;
			if(pushed_buttons == KEY_UP){
				while((PINB>>5)!=KEY_UNP);
				key_plus();
				pushed_buttons = KEY_UNP;
			}
			if(pushed_buttons == KEY_DOW){
				while((PINB>>5)!=KEY_UNP);
				key_minus();
				pushed_buttons = KEY_UNP;
			}
			if(pushed_buttons == KEY_CEN){
				while((PINB>>5)!=KEY_UNP);
				if(menu != 0)
					menu_sel = !menu_sel;
				pushed_buttons = KEY_UNP;
			}
			pid_init(kp,ki,kd);
		}
		else
			pushed_buttons = KEY_UNP;
		buffer_overflow = 0;
	}
	else{
		buffer_overflow++;
	}



	//
	if(buffer_pid_ovf == PID_OVF){	// Срабатывание 0.03264 * PID_OVF ms
		cou = calc_pwm(need_temperature, real_temperature);
		buffer_pid_ovf = 0;
	}
	buffer_pid_ovf++;
}

void printMainMenu(void){
	lcd_clear();
	lcd_str_out(hstr);
	lcd_gotoxy(0,1);
	if(menu_sel)
		if(menu)
			lcd_char_out(0x3E);
	lcd_str_out(lstr);
}

int main(void){
	sysinit();
	while(1){
		//real_temperature = readt();
		HEATER = cou;

		switch (menu){
			case 0:
				sprintf(hstr,"%d.%04d",(real_temperature>>4),(real_temperature%16)*625);
				hstr[5] = 0;
				hstr[6] = 0;
				sprintf(lstr, "%d", cou);
				break;
			case 1:
				sprintf(hstr, "%s","Set temp:");
				sprintf(lstr,"%d.%04d",(need_temperature>>4),(need_temperature%16)*625);
				lstr[5] = 0;
				lstr[6] = 0;
				break;
			case 2:
				sprintf(hstr, "%s","Set kp:");
				sprintf(lstr,"%d",kp);
				break;
			case 3:
				sprintf(hstr, "%s","Set ki:");
				sprintf(lstr,"%d",ki);
				break;
			case 4:
				sprintf(hstr, "%s","Set kd:");
				sprintf(lstr,"%d",kd);
				break;
			case 5:
				sprintf(hstr, "%s","Set time:");
				sprintf(lstr,"%d",buffer_pid_ovf);
				break;
		}

		printMainMenu();
		delay_ms(700);
	}
}
