/*
 * main.c
 *
 *  Created on: 07.11.2016
 *      Author: coba31rus
 */
#include "main.h"

int real_temperature, need_temperature = 600;
volatile unsigned char buffer_key_ovf = 0, buffer_tem_ovf = 0, tmp_buttons, pushed_buttons, menu = 0, selected = 0;
unsigned char kp, ki, kd, cou = 0;
char h_string[15], l_string[15];
volatile unsigned int show_time = 0;

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
	kp = EEPROM_read_char(0x0);
	ki = EEPROM_read_char(0x1);
	kd = EEPROM_read_char(0x2);
	need_temperature = EEPROM_read_int(0x3);
	pid_init(kp,ki,kd);
}

void key_plus(void){
	switch (menu){
		case 0:
			menu++;
			break;
		case 1:
			if(selected)
				need_temperature++;
			else
				menu++;
			break;
		case 2:
			if(selected)
				kp++;
			else
				menu++;
			break;
		case 3:
			if(selected)
				ki++;
			else
				menu++;
			break;
		case 4:
			if(selected)
				kd++;
			else
				menu++;
			break;
	}
	if(menu > MENUCOUNT)
		if(!selected)
			menu = 0;
}

void key_minus(void){
	switch (menu){
		case 0:
			menu--;
			break;
		case 1:
			if(selected)
				need_temperature--;
			else
				menu--;
			break;
		case 2:
			if(selected)
				kp--;
			else
				menu--;
			break;
		case 3:
			if(selected)
				ki--;
			else
				menu--;
			break;
		case 4:
			if(selected)
				kd--;
			else
				menu--;
			break;
	}
	if(menu > MENUCOUNT)
		if(!selected)
			menu = MENUCOUNT;
}

ISR(TIMER0_OVF_vect){
	if (buffer_key_ovf >= OVF_BUF_COUNT){ //Срабатывание 0.03264 * OVF_BUF_COUNT ms
		tmp_buttons = pushedButton(PINB>>5);
		if (tmp_buttons != KEY_UNP){
			pushed_buttons = tmp_buttons;
			if(pushed_buttons == KEY_UP){
				//while((PINB>>5)!=KEY_UNP);
				key_plus();
				pushed_buttons = KEY_UNP;
			}
			if(pushed_buttons == KEY_DOW){
				//while((PINB>>5)!=KEY_UNP);
				key_minus();
				pushed_buttons = KEY_UNP;
			}
			if(pushed_buttons == KEY_CEN){
				//while((PINB>>5)!=KEY_UNP);
				if(menu != 0){
					if(selected){
						EEPROM_write(0x0, kp);
						EEPROM_write(0x1, ki);
						EEPROM_write(0x2, kd);
						EEPROM_write_int(0x3, need_temperature);
					}
					selected = !selected;
				}
				pushed_buttons = KEY_UNP;
			}
			pid_init(kp,ki,kd);
		}
		else
			pushed_buttons = KEY_UNP;
		buffer_key_ovf = 0;
	}

	buffer_tem_ovf++;
	buffer_key_ovf++;
	show_time++;
}

int main(void){
	sysinit();
	while(1){
		if(buffer_tem_ovf >= TEMP_UPDATE){
			//real_temperature = 0x8000;
			real_temperature = readtt_2();
			if(real_temperature == 0x8000 || real_temperature == 0){
				cou = 0;
			}
			else{
				cou = calc_pwm(need_temperature, real_temperature);
			}
			HEATER = cou;
			buffer_tem_ovf = 0;
		}
		 if(show_time >= SCREEN_UPDATE){
			switch (menu){
				case 0:
					h_string[0] = *"t";
					h_string[1] = *"=";
					h_string[2] = (real_temperature>>4)/10 + 0x30;
					h_string[3] = (real_temperature>>4)%10 + 0x30;
					h_string[4] = *",";
					h_string[5] = (real_temperature%16)*625/1000 + 0x30;
					h_string[6] = ((real_temperature%16)*625/100)%10 + 0x30;
					h_string[7]	= *"\0";
					
					l_string[0] = *"H";
					l_string[1] = *"=";
					l_string[2] = cou/100 + 0x30;
					l_string[3] = (cou/10)%10 + 0x30;
					l_string[4] = cou%10 + 0x30;
					l_string[5] = *"\0";
					break;
				case 1:
					h_string[0] = *"s";
					h_string[1] = *"e";
					h_string[2] = *"t";
					h_string[3] = *" ";
					h_string[4] = *"t";
					h_string[5] = *"e";
					h_string[6] = *"m";
					h_string[7] = *"p";
					h_string[8] = *":";
					h_string[9]	= *"\0";
					
					l_string[0] = *" ";
					l_string[1] = (need_temperature>>4)/10 + 0x30;
					l_string[2] = (need_temperature>>4)%10 + 0x30;
					l_string[3] = *",";
					l_string[4] = (need_temperature%16)*625/1000 + 0x30;
					l_string[5] = ((need_temperature%16)*625/100)%10 + 0x30;
					l_string[6] = *"\0";
					break;
				case 2:
					h_string[0] = *"s";
					h_string[1] = *"e";
					h_string[2] = *"t";
					h_string[3] = *" ";
					h_string[4] = *"k";
					h_string[5] = *"p";
					h_string[6]	= *"\0";
					
					l_string[0] = *" ";
					l_string[1] = kp/100 + 0x30;
					l_string[2] = (kp/10)%10 + 0x30;
					l_string[3] = kp%10 + 0x30;
					l_string[4] = *"\0";
					break;
				case 3:
					h_string[0] = *"s";
					h_string[1] = *"e";
					h_string[2] = *"t";
					h_string[3] = *" ";
					h_string[4] = *"k";
					h_string[5] = *"i";
					h_string[6]	= *"\0";
					
					l_string[0] = *" ";
					l_string[1] = ki/100 + 0x30;
					l_string[2] = (ki/10)%10 + 0x30;
					l_string[3] = ki%10 + 0x30;
					l_string[4] = *"\0";
					break;
				case 4:
					h_string[0] = *"s";
					h_string[1] = *"e";
					h_string[2] = *"t";
					h_string[3] = *" ";
					h_string[4] = *"k";
					h_string[5] = *"d";
					h_string[6]	= *"\0";
					
					l_string[0] = *" ";
					l_string[1] = kd/100 + 0x30;
					l_string[2] = (kd/10)%10 + 0x30;
					l_string[3] = kd%10 + 0x30;
					l_string[4] = *"\0";
					break;
			}
		show_time = 0;
		if(selected)
			if(menu)
				l_string[0] = 0x3E;
		lcd_clear();
		lcd_str_out(h_string);
		lcd_gotoxy(0,1);
		lcd_str_out(l_string);
		}
		delay_ms(1);
	}
	return 0;
}
