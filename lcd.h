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

	//#define OLD_OUT

	#define BYTE char

	#define LCD_PIN				PORTD
	#define LCD_DDR				DDRD
	#define LCD_RS				0x04
	#define LCD_E				0x08
	#define LCD_SHIFT			0x04

	#define lcd_set_e()			LCD_PIN |= LCD_E
	#define lcd_set_rs()		LCD_PIN |= LCD_RS
	#define lcd_clear_e()		LCD_PIN &= ~LCD_E
	#define lcd_clear_rs()		LCD_PIN &= ~LCD_RS
	#define LCD_DATA_CLEAR		LCD_PIN &= (0xFF >> LCD_SHIFT)

	//#define LCD_CLS			0x01
	#define LCD_4BIT			0x28
	#define LCD_CONN			0xFC

	#define delay_us(_data)	_delay_us(_data)
	#define delay_ms(_data)	_delay_ms(_data)

	//#define CHAR_T			0xCB
	//#define CHAR_F			0xFF




	//Описание типов дефайнов для меню
	#define MAX_SCREENS			4
	#define MAIN_SCREEN			0
	#define TEMP_SCREEN			1
	#define KPROP_SCREEN		2
	#define KINTEG_SCREEN		3
	#define KDIFF_SCREEN		4
	//#define MAIN_SCREEN		0
	//#define MAIN_SCREEN		0


	void lcd_init(void);
	void lcd_char_out(char data);
	void lcd_clear(void);
	void lcd_str_out(char *str);
	void lcd_gotoxy(BYTE x, BYTE y);

#endif /* LCD_H_ */
