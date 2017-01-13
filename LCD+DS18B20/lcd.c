/*
 * lcd.c
 *
 *  Created on: 07.11.2016
 *      Author: coba31rus
 */

#include "lcd.h"

void lcd_out(BYTE data){
	if(data & 0x01)
		{
			LCD_PIN |= (1 << 2);
		}
		else
		{
			LCD_PIN &= ~(1 << 2);
		}
		if(data & 0x02)
		{
			LCD_PIN |= (1 << 3);
		}
		else
		{
			LCD_PIN &= ~(1 << 3);
		}
		if(data & 0x04)
		{
			LCD_PIN |= (1 << 4);
		}
		else
		{
			LCD_PIN &= ~(1 << 4);
		}
		if(data & 0x08)
		{
			LCD_PIN |= (1 << 5);
		}
		else
		{
			LCD_PIN &= ~(1 << 5);
		}
}

void lcd_com(BYTE com){
	lcd_clear_rs();
	lcd_out(com);
	lcd_set_e();
	lcd_clear_e();
	delay_ms(4);
}

void lcd_init(void){
	LCD_DDR |= LCD_CONN;
	delay_ms(20);
	lcd_com(0x03);
	delay_us(40);
	lcd_com(0x03);
	delay_us(40);
	lcd_com(0x03);
	delay_us(40);
	lcd_com(0x02);
	delay_us(40);
	lcd_com(0x02);
	lcd_com(0x08);
	lcd_com(0x00);
	lcd_com(0x08);
	lcd_com(0x00);
	lcd_com(0x01);
	lcd_com(0x00);
	lcd_com(0x06);
	lcd_com(0x00);
	lcd_com(0x0C);

}

void lcd_char_out(char data){
	lcd_set_rs();
	lcd_out(data >> 4);
	lcd_set_e();
	lcd_clear_e();
	lcd_out(data & 0x0F);
	lcd_set_e();
	lcd_clear_e();
	delay_ms(4);
}

void lcd_clear(){
	lcd_com(0x00);
	lcd_com(0x01);
}

void lcd_str_out(char *str){
	while((*str) != '\0'){
		lcd_char_out(*str);
		str++;
	}
}

void lcd_gotoxy(BYTE x, BYTE y)
{
	if(x > 39) x = 39;
	if(x < 0) x = 0;
	if(y > 3) y = 3;
	if(y < 0) y = 0;

	BYTE temp = 0x00;

// Работа с дисплеем на прямую
	lcd_clear_rs();

	switch (y)
	{
		case 0:
		{
			temp |= (0x80 + x);
			lcd_com(temp >> 4);			// Передача старших 4 бит
			lcd_com(temp & 0x0F);		// Передача младших 4 бит
			break;
		}
		case 1:
		{
			temp |= (0xC0 + x);
			lcd_com(temp >> 4);			// Передача старших 4 бит
			lcd_com(temp & 0x0F);		// Передача младших 4 бит
			break;
		}
		case 2:
		{
			temp |= (0x94 + x);
			lcd_com(temp >> 4);			// Передача старших 4 бит
			lcd_com(temp & 0x0F);		// Передача младших 4 бит
			break;
		}
		case 3:
		{
			temp |= (0xD4 + x);
			lcd_com(temp >> 4);			// Передача старших 4 бит
			lcd_com(temp & 0x0F);		// Передача младших 4 бит
			break;
		}
	}
}
