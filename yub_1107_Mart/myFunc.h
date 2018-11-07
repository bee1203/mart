#ifndef MYFUNC_H_
#define MYFUNC_H_

// 스위치 정의
#define SW1 30
#define SW2 46
#define SW3 78
#define SW4 29
#define SW5 45
#define SW6 77
#define SW7 27

// 타이머 변수
int sec = 0;
int time_count = 0;

void init()
{
	DDRA = 0xff;  // 0 1 : Moter  
				  // 5 6 7 : LED
	DDRD = 0x0f;  // 0 1 2 3 : Row
				  // 4 5 6 7 : Col
	lcd_init();
	lcd_clear();
	
	for (int i = 0; i < 2; i++)
	{
		
		lcd_putsf(0,0,(unsigned char*)"     MARKET     ");
		lcd_putsf(0,1,(unsigned char*)"     SYSTEM     ");
		PORTA = 0b00000000;
		_delay_ms(100);
		lcd_clear();
		PORTA = 0b01110000;
		_delay_ms(100);
	}
}

#endif