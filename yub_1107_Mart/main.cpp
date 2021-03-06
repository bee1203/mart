#include "myFunc.h"

unsigned int time_count = 0, sec = 0, min = 0, hour = 0, timer = 0;
int mode = NORMAL_MODE;
int number[10] = {KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9};
unsigned int product_price = 0;
unsigned int total_money = 0;
unsigned int input_money = 0;
unsigned int total_sales = 0;
char buf1[16];
int flag = 0;
int change = 0;

ISR(TIMER0_OVF_vect)
{
	timer++;
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
	char buf1[16];
	unsigned int input_key = 0;
	while (1)
	{
		input_key = getkey();
		if (input_key == 0) flag = 0;
		if (input_key == KEY_LOBY)
		{
			lcd_clear();
			PORTA = 0xff;
			mode = NORMAL_MODE;
		}
		if (mode == NORMAL_MODE)
		{
			Display_Clock(hour, min, sec);
			if (input_key == KEY_Menu)
			{
				lcd_clear();
				mode = MENU_MODE;
			}
		}
		else if (mode == MENU_MODE)
		{
			lcd_putsf(0,0,(unsigned char *)"1: Calculation  ");
			lcd_putsf(0,1,(unsigned char *)"2: Total Sales  ");
			
			if (input_key == KEY_1)
			{
				lcd_clear();
				lcd_putsf(0,0,(unsigned char *)"Calculate Mode  ");
				PORTA = 0b11011111;
				mode = CALC_MODE;
			}
			if (input_key == KEY_2)
			{
				lcd_clear();
				lcd_putsf(0,0,(unsigned char *)"Total Sales     ");
				sprintf(buf1,"=%11d won",total_sales);
				lcd_putsf(0,1,(unsigned char *)buf1);
				PORTA = 0b01111111;
				mode = TOTAL_MODE;
			}
			_delay_ms(50);
		}
		else if (mode == CALC_MODE)
		{		
			if (change == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					if ((input_key == number[i]) && (flag == 0))
					{
						flag = 1;
						product_price = product_price * 10 + i;
						sprintf(buf1,"%d",product_price);
						lcd_putsf(0,1,(unsigned char *)buf1);
						break;
					}
				}
				if ((input_key == KEY_PLUS) && (flag == 0))
				{
					flag = 1;
					
					if (product_price > 20000)
					{
						sprintf(buf1,"     ERROR!      ");
						for (int j = 0; j < 3; j++)
						{
							lcd_putsf(0,1,(unsigned char *)buf1);
							_delay_ms(100);
							lcd_putsf(0,1,(unsigned char *)"                ");
							_delay_ms(100);
						}
						mode = NORMAL_MODE;
						total_money = 0;
					}
					else
					{
						lcd_putsf(0,1,(unsigned char *)"                ");
						total_money += product_price;

					}
					product_price = 0;
				}
				if ((input_key == KEY_Enter) && (flag == 0))
				{
					flag = 1;
					if (product_price > 20000)
					{
						sprintf(buf1,"     ERROR!     ");
						for (int j = 0; j < 3; j++)
						{
							lcd_putsf(0,1,(unsigned char *)buf1);
							_delay_ms(100);
							lcd_putsf(0,1,(unsigned char *)"                ");
							_delay_ms(100);
						}
						mode = NORMAL_MODE;
						total_money = 0;
					}
					else
					{
						total_money += product_price;
						sprintf(buf1,"=%11d won",total_money);
						lcd_putsf(0,1,(unsigned char *)buf1);
						change = 1;
					}
					product_price = 0;
				}
			}
			if ((input_key == KEY_MONEY) && (change == 1))
			{
				lcd_clear();
				lcd_putsf(0,0,(unsigned char *)"Changes Mode    ");
				PORTA = 0b10111111;
				mode = CHANGES_MODE;
			}
		}
		else if (mode == CHANGES_MODE)
		{
			change = 0;
			for (int i = 0; i < 10; i++)
			{
				if ((input_key == number[i]) && (flag == 0))
				{
					flag = 1;
					input_money = input_money * 10 + i;
					sprintf(buf1,"%d",input_money);
					lcd_putsf(0,1,(unsigned char *)buf1);
					break;
				}
			}
			if ((input_key == KEY_Enter) && (flag == 0))
			{
				flag = 1;
				if (input_money >= total_money)
				{
					int future = timer + 5000;
					total_sales += total_money;					
					sprintf(buf1,"=%11d won",input_money - total_money);
					lcd_putsf(0,1,(unsigned char *)buf1);
					total_money = 0;
					while(1)
					{
						int break_key = getkey();
						if ((timer > future) || (break_key == KEY_LOBY)) 
						{
							mode = NORMAL_MODE;
							break;
						}
					}					
				}
				else lcd_putsf(0,1,(unsigned char *)"                ");
				input_money = 0;
			}
		}
		else if (mode == TOTAL_MODE)
		{
			while(1)
			{
				int future = timer + 5000;
				int break_key = getkey();
				if ((timer > future) || (break_key == KEY_LOBY))
				{
					mode = NORMAL_MODE;
					break;
				}
			}
		}
	}
}

