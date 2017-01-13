/*
 * lcd.h
 *
 *  Created on: 07.11.2016
 *      Author: coba31rus
 */

#ifndef LCD_H_
	#define LCD_H_
	#include <avr/io.h>
	#include <util/delay.h>

	#define BYTE char

	#define LCD_PIN			PORTD
	#define LCD_DDR			DDRD
	#define LCD_RS			0x01
	#define LCD_E			0x02

	#define lcd_set_e()		LCD_PIN |= LCD_E
	#define lcd_set_rs()	LCD_PIN |= LCD_RS
	#define lcd_clear_e()	LCD_PIN &= ~LCD_E
	#define lcd_clear_rs()	LCD_PIN &= ~LCD_RS

	#define LCD_CLS			0x01
	#define LCD_4BIT		0x28
	#define LCD_CONN		0x3F

	#define delay_us(_data)	_delay_us(_data)
	#define delay_ms(_data)	_delay_ms(_data)

	#define CHAR_T			0xCB
	#define CHAR_F			0xFF


	void lcd_init(void);
	void lcd_char_out(char data);
	void lcd_clear();
	void lcd_str_out(char *str);
	void lcd_gotoxy(BYTE x, BYTE y);

#endif /* LCD_H_ */
