#include "myFunc.h"


unsigned int time_count = 0, sec = 0, min = 0, hour = 0;
int mode = NORMAL_MODE;

ISR(TIMER0_OVF_vect)
{
	time_count++;
	if (time_count >= 60)
	{
		sec++;
		if (sec > 59) {min++; sec = 0;}
		if (min > 59) {hour++; min = 0;}
		if (hour > 24) {hour = min = sec = 0;}
		time_count = 0;
	}
}

int main(void)
{
	init();
	unsigned int input_key = 0;
    while (1) 
    {
		input_key = getkey();
		if (mode == NORMAL_MODE)
		{
			Display_Clock(hour, min, sec);
			if (input_key == KEY_Munu) mode = MENU_MODE;
		}
		if (mode == MENU_MODE)
		{
			lcd_putsf(0,0,(unsigned char *)"1: Calculation  ");
			lcd_putsf(0,1,(unsigned char *)"2: Total Sales  ");
			
			if (input_key == KEY_1) mode = CALC_MODE;
			if (input_key == KEY_2) mode = TOTAL_MODE;
			input_key = 0; 
			_delay_ms(50);
		}		
    }
}

