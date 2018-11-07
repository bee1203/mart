#ifndef MYFUNC_H_
#define MYFUNC_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcd.h"

#define NORMAL_MODE  0
#define MENU_MODE    1
#define CALC_MODE    2
#define CHANGES_MODE 3
#define TOTAL_MODE  4

#define KEY_1 238
#define KEY_2 222
#define KEY_3 190
#define KEY_4 237
#define KEY_5 221
#define KEY_6 189
#define KEY_7 235
#define KEY_8 219
#define KEY_9 187
#define KEY_0 231
#define KEY_PLUS  126
#define KEY_Enter 125
#define KEY_Menu  123
#define KEY_MONEY 183
#define KEY_LOBY  119

void init()
{
	DDRA = 0xff;  // 0 1 : Moter  
				  // 5 6 7 : LED
	DDRD = 0x0f;  // 0 1 2 3 : Row
				  // 4 5 6 7 : Col
				  
	TIMSK = 0x01;
	TCCR0 = 0x07;
	_delay_ms(2);
	
	lcd_init();
	lcd_clear();
	
	for (int i = 0; i < 2; i++)
	{
		
		lcd_putsf(0,0,(unsigned char*)"     MARKET     ");
		lcd_putsf(0,1,(unsigned char*)"     SYSTEM     ");
		PORTA = 0x00;
		_delay_ms(100);
		lcd_clear();
		PORTA = 0xff;
		_delay_ms(100);
	}
	sei(); // Global Interrupt Enable
}

void Display_Clock(unsigned int hour, unsigned int min, unsigned int sec)
{
	char str[16];
	sprintf(str,"    %02d:%02d:%02d    ",hour, min, sec);
	lcd_putsf(0,1,(unsigned char *)str);
}

unsigned int getkey()
{	
	unsigned int key = 0;
	for(int i = 0; i < 4; i++)
	{
		PORTD = ~(0x00 | 1 << i); 
		_delay_ms(10);
		key = (~PIND & 0xf0); // key가 눌리지 않았을 때 0값을 출력하도록 함
		
		if (key) return PIND; // key가 눌렸다면 0이 아니므로 고유값 PIND를 반환
	}
	return 0;
}
#endif