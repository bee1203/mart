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
    while (1) 
    {
		char str[16];
		int key = getkey();
		sprintf(str,"%d",key);
		lcd_putsf(0,0,(unsigned char *)str);
		if (mode == NORMAL_MODE)
		{
			Display_Clock(hour, min, sec);
		}
		if (mode = MENU_MODE)
		{
			
		}
    }
}

