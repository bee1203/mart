#ifndef MYFUNC_H_
#define MYFUNC_H_

// 도레미 주파수 정의
#define DO 262
#define DOS 278
#define RE 294
#define RES 311
#define MI 330
#define FA 349
#define FAS 369
#define SO 392
#define SOS 415
#define LA 440
#define LAS 466
#define SI 494

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
int tenmilsec = 0;
int record_count = 0;

// 예제 정의
#define EXAMPLE_SELECT 0
#define EXAMPLE1 1
#define EXAMPLE2 2
#define EXAMPLE3 3

//EXAMPLE 변수
int example = EXAMPLE_SELECT;

// 음악 변수
int music_freq[11] = {2*RE, 2*RE, 2*RE, 0,   LAS, 2*RE, 0,   2*FA, 0,   FA,  0};
int music_delay[11] = {150, 150,  150,  150, 150, 150,  150, 150,  450, 150, 450};
int music_count = 0;
int music_index = 0;
int music_flag = 0;

ISR(TIMER0_OVF_vect) // if(1ms 되었을 때)
{
	TCNT0 = 256 - 250;
	time_count++;
	record_count++;
	
	if(music_flag != 1)
	{
		if (time_count >= 1000)
		{
			sec++;
			time_count = 0;
		}
	}
	if (record_count >= 10)
	{
		tenmilsec ++;
		record_count = 0;
	}
	
	if(music_flag == 1)
	{
		music_count++;
		//music_index = 0;
		if(music_freq[music_index] == 0)	OCR1A = 0;
		else								OCR1A = 8000000 / music_freq[music_index];
		
		char str[16];

		if(music_count > music_delay[music_index])
		{
			music_index++;
			music_count = 0;
		}
		if(music_index > 10)
		{
			music_flag = 0;
			time_count = 0;
			sec = 0;
		}
	}
}

unsigned int getkey() // 한 개의 포트로 9개의 push button 제어하는 함수
{
	unsigned int key = 0, result = 0;
	for (int i = 0; i < 3; i++)
	{
		if (i == 0) PORTD = 0b11111110;
		if (i == 1) PORTD = 0b11111101;
		if (i == 2) PORTD = 0b11111011;

		_delay_ms(5);
		
		key = (~PIND & 0xf0);
		if (key)
		{
			result = key | (PORTD & 0x0f);
			return result;
		}
	}
	return 0;
}

void freqset (int freq) // 주파수로 소리내는 함수
{
	OCR1A = 8000000 / freq;
}

void pianoshape (char piano[16])
{
	// 건반모양
	piano[0] = 0x7c;
	piano[1] = 0xff;
	piano[2] = 0x7c;
	piano[3] = 0xff;
	piano[4] = 0x7c;
	piano[5] = 0xff;
	piano[6] = 0x7c;
	piano[7] = 0xff;
	piano[8] = 0x7c;
	piano[9] = 0xff;
	piano[10] = 0x7c;
	piano[11] = 0xff;
	piano[12] = 0x7c;
	piano[13] = 0xff;
	piano[14] = 0x7c;
	piano[15] = 0;
}

void playpiano(int key, char piano[16])
{
	if (key == 0) 
	{
		OCR1A = 0;
		PORTA = 0b01111111;
		pianoshape(piano);	
	}
	if (key == SW1)
	{
		PORTA = 0b11111110;
		piano[1] = ' ';
		lcd_putsf(0,1,(unsigned char*)piano);
		freqset(DO);
	}
	else if (key== SW2)
	{
		PORTA = 0b11111101;
		piano[3] = ' ';
		lcd_putsf(0,1,(unsigned char*)piano);
		freqset(RE);
	}
	else if (key == SW3)
	{
		PORTA = 0b11111011;
		piano[5] = ' ';
		lcd_putsf(0,1,(unsigned char*)piano);
		freqset(MI);
	}
	else if (key == SW4)
	{
		PORTA = 0b11110111;
		piano[7] = ' ';
		lcd_putsf(0,1,(unsigned char*)piano);
		freqset(FA);
	}
	else if (key == SW5)
	{
		PORTA = 0b11101111;
		piano[9] = ' ';
		lcd_putsf(0,1,(unsigned char*)piano);
		freqset(SO);
	}
	else if (key == SW6)
	{
		PORTA = 0b11011111;
		piano[11] = ' ';
		lcd_putsf(0,1,(unsigned char*)piano);
		freqset(LA);
	}
	else if (key == SW7)
	{
		PORTA = 0b10111111;
		piano[13] = ' ';
		lcd_putsf(0,1,(unsigned char*)piano);
		freqset(SI);
	}
}

int music ()
{
	if (music_flag == 1)
	{
		if (music_freq[music_index] == 0) OCR1A = 0;
		else
		{
			if (music_count <= music_delay[music_index]) 8000000 / music_freq[music_index];
			else	                                     OCR1A = 0;
			if (music_index == 11) music_flag =0;
		}
	}
	music_index = 0;
}
#endif