/*
 * main.c
 *
 *  Created on: 07.11.2016
 *      Author: coba31rus
 */
#include "main.h"


int real_temperature, need_temperature = 600;
char hstr[LCD_LINE_SIZE], lstr[LCD_LINE_SIZE], menu = 0, menu_sel = 0; //Строки выводимые на дисплей
volatile unsigned char buffer_key_ovf = 0, buffer_pid_ovf = 0, buffer_tem_ovf = 0, tmp_buttons, pushed_buttons, pid_time = 200;
unsigned char kp = 4, ki = 1, kd = 3, cou = 0;


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
				pid_time++;
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
				pid_time--;
			else
				menu--;
			break;
	}
	if(menu > MENUCOUNT)
		if(!menu_sel)
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
					if(menu_sel){
						EEPROM_write(0x0, kp);
						EEPROM_write(0x1, ki);
						EEPROM_write(0x2, kd);
						EEPROM_write_int(0x3, need_temperature);
					}
					menu_sel = !menu_sel;
				}
				pushed_buttons = KEY_UNP;
			}
			pid_init(kp,ki,kd);
		}
		else
			pushed_buttons = KEY_UNP;
		buffer_key_ovf = 0;
	}

	if(buffer_tem_ovf == TEMP_UPDATE){
		real_temperature = 0x8000;
		real_temperature = readt();
		buffer_tem_ovf = 0;
	}
	//
	if(buffer_pid_ovf == pid_time){	// Срабатывание 0.03264 * PID_OVF ms
		cou = calc_pwm(need_temperature, real_temperature);
		buffer_pid_ovf = 0;
	}
	buffer_tem_ovf++;
	buffer_pid_ovf++;
	buffer_key_ovf++;
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
		if(real_temperature == 0x8000 || real_temperature == 0){
			HEATER = 0;
			cou = 0;
		}
		else
			HEATER = cou;
		switch (menu){
			case 0:
				sprintf(hstr,"t=%d.%02d",(real_temperature>>4),(real_temperature%16)*625/100);
				sprintf(lstr, "H=%d%%", cou*100/255);
				break;
			case 1:
				sprintf(hstr, " %s","Set temp:");
				sprintf(lstr," %d.%02d",(need_temperature>>4),(need_temperature%16)*625/100);
				break;
			case 2:
				sprintf(hstr, " %s","Set kp:");
				sprintf(lstr," %d",kp);
				break;
			case 3:
				sprintf(hstr, " %s","Set ki:");
				sprintf(lstr," %d",ki);
				break;
			case 4:
				sprintf(hstr, " %s","Set kd:");
				sprintf(lstr," %d",kd);
				break;
			case 5:
				sprintf(hstr, " %s","Set PID time:");
				sprintf(lstr," %d",pid_time);
				break;
		}
		if(menu_sel)
			if(menu)
				lstr[0] = 0x3E;

		printMainMenu();
		delay_ms(400);
	}
}
