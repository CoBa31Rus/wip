/*
 * main.c
 *
 *  Created on: 07.11.2016
 *      Author: coba31rus
 */
#include "main.h"

//#define DEBUG

int real_temperature, need_temperature = 600;
volatile unsigned char buffer_key_ovf = 0, buffer_tem_ovf = 0, tmp_buttons, pushed_buttons, menu = 0, selected = 0, show_time = 0;
extern unsigned char pK, iK, dK;
char h_string[15], l_string[15];
unsigned char cou = 0;
//volatile unsigned int show_time = 0;


extern int pres, ires, dres;

void sysinit(void){
	//Таймер обработчик клавиш; Timer for Key
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
	pK = EEPROM_read_char(0x0);
	iK = EEPROM_read_char(0x1);
	dK = EEPROM_read_char(0x2);
	need_temperature = EEPROM_read_int(0x3);

	//pK = 40;
	//iK = 20;
	//dK = 80;

#ifdef DEBUG
	#define BAUD 9600L
	#define UBRRL_value (F_CPU/(BAUD*16))-1
	UCSRB |= (1<<TXEN);
	UBRRL = UBRRL_value;
	UBRRH = UBRRL_value >> 8;
	UCSRC |= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
#endif

}

#ifdef DEBUG
void send_int_UART(int val){
	while(!(UCSRA & (1<<UDRE)));
	UDR = (char)val>>8;
	while(!(UCSRA & (1<<UDRE)));
	UDR = (char)val;
}
#endif

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
				pK++;
			else
				menu++;
			break;
		case 3:
			if(selected)
				iK++;
			else
				menu++;
			break;
		case 4:
			if(selected)
				dK++;
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
				pK--;
			else
				menu--;
			break;
		case 3:
			if(selected)
				iK--;
			else
				menu--;
			break;
		case 4:
			if(selected)
				dK--;
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
				if(menu != 0){
					if(selected){
						EEPROM_write(0x0, pK);
						EEPROM_write(0x1, iK);
						EEPROM_write(0x2, dK);
						EEPROM_write_int(0x3, need_temperature);
					}
					selected = !selected;
				}
				pushed_buttons = KEY_UNP;
			}
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
			real_temperature = readtt_2();
			if(real_temperature == 0){
				cou = 0;
			}
			else{
				cou = calc_pwm(need_temperature, real_temperature);
			}
			HEATER = cou;
			buffer_tem_ovf = 0;
			#ifdef DEBUG
				send_int_UART(real_temperature);
			#endif
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
					h_string[6] = *"\0";

					l_string[0] = *" ";
					l_string[1] = pK/100 + 0x30;
					l_string[2] = (pK/10)%10 + 0x30;
					l_string[3] = pK%10 + 0x30;
#ifdef DEBUG
					l_string[4] = *" ";
					l_string[5] = (pres/10000)+0x30;
					l_string[6] = (pres/1000)%10 + 0x30;
					l_string[7] = (pres/100)%10 + 0x30;
					l_string[8] = (pres/10)%10 + 0x30;
					l_string[9] = pres%10 + 0x30;
#endif
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
					l_string[1] = iK/100 + 0x30;
					l_string[2] = *".";
					l_string[3] = (iK/10)%10 + 0x30;
					l_string[4] = iK%10 + 0x30;
#ifdef DEBUG
					l_string[5] = *" ";
					l_string[6] = (ires/10000)+0x30;
					l_string[7] = (ires/1000)%10 + 0x30;
					l_string[8] = (ires/100)%10 + 0x30;
					l_string[9] = (ires/10)%10 + 0x30;
					l_string[10] = ires%10 + 0x30;
#endif
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
					l_string[1] = dK/100 + 0x30;
					l_string[2] = (dK/10)%10 + 0x30;
					l_string[3] = dK%10 + 0x30;
#ifdef DEBUG
					l_string[4] = *" ";
					l_string[5] = (dres/10000)+0x30;
					l_string[6] = (dres/1000)%10 + 0x30;
					l_string[7] = (dres/100)%10 + 0x30;
					l_string[8] = (dres/10)%10 + 0x30;
					l_string[9] = dres%10 + 0x30;
#endif
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
