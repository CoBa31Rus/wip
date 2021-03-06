/*
 * lcd.c
 *
 *  Created on: 07.11.2016
 *      Author: coba31rus
 */

#include "lcd.h"

#ifdef OLD_OUT
void lcd_out(BYTE data){
	if(data & 0x01)
		LCD_PIN |= (1 << 4);
	else
		LCD_PIN &= ~(1 << 4);
	if(data & 0x02)
		LCD_PIN |= (1 << 5);
	else
		LCD_PIN &= ~(1 << 5);
	if(data & 0x04)
		LCD_PIN |= (1 << 6);
	else
		LCD_PIN &= ~(1 << 6);
	if(data & 0x08)
		LCD_PIN |= (1 << 7);
	else
		LCD_PIN &= ~(1 << 7);
}
#else
void lcd_out(BYTE data){
		data <<= LCD_SHIFT;
		LCD_DATA_CLEAR;
		LCD_PIN |= data;
}
#endif

void lcd_com(BYTE com){
	lcd_clear_rs();
	lcd_out(com);
	lcd_set_e();
	lcd_clear_e();
	delay_ms(2);
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
	delay_ms(2);
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

void lcd_gotoxy(BYTE x, BYTE y){
	lcd_clear_rs();
	BYTE temp = 0x80 + (y << 6) + x;
	lcd_com(temp >> 4);
	lcd_com(temp & 0xFF);
}
