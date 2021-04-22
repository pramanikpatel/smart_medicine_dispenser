/*
In final circuit the port 2 is to be used as data port for LCD.
Rs Rw and EN on port 1 0,1,2
Buzzer to be connected at port 3 pin 2
motor to be connected at port 3 pin 3
*/

#include "externs.h"


void main()
{			
//	buzzer = 0;
	// gsm_flag=0;	
	Ext_int_Init();  	/* Call Ext. interrupt initialize */
	UART_Init();
	timer_init();
	lcd_init();
	  IR=0;
	while(1)
	{	
		
		// To check if the patient is taking the medicine of slot 1
		if((IR == 1) && (hour == slot1) && !((flag & 0x80) == 0X80))
		{
			flag |= 0x80;
			// lcd_write_string("Medicine Taken");
		}

		// To check if the patient is taking the medicine of slot 2
		else if((IR == 1) && (hour == slot2) && !((flag & 0x40) == 0X40))
		{
			flag |= 0x40;
			// lcd_write_string("Medicine Taken");
		}

		// To check if the patient is taking the medicine of slot 3
		else if((IR == 1) && (hour == slot2) && !((flag & 0x20) == 0X20))
		{
			flag |= 0x20;
			// lcd_write_string("Medicine Taken");
		}

		// Check if slot 1 or slot 2 time has occured
		if(((hour == slot1)&& !((flag & 0x80) == 0x80)) && min <1 && (sec<59) )
		{
			gsm_flag = 0;
			buzz();
		}

		else if(((hour == slot2)&& !((flag & 0x40) == 0X40)) && min <1 && (sec<59))
			{
				gsm_flag = 0;
				buzz();
			}
		
		else if((hour == slot3)&& !((flag & 0x20) == 0X20) && min <1 && (sec<59))
			{
				gsm_flag = 0;
				buzz();
			}


		//to check if medicine has been taken or not if not taken then give missed call 
		//and buzz the buzzer again to remind the patient for slot 1
		if(!((flag & 0x80) == 0x80) && (hour==slot1) && (hour < slot1+1) && !gsm_flag && min == 2)
		{
			GSM_missed_call();
		}


		//to check if medicine has been taken or not if not taken then give missed call 
		//and buzz the buzzer again to remind the patient for slot 2
		else if(!((flag & 0x40) == 0x40) && (hour==slot2) && (hour <slot2+1) && !gsm_flag && min == 2)
		{
//			buzz();
			GSM_missed_call();
			// if want to send message using GSM and give missed calls
		}

		else if(!((flag & 0x20) == 0x20) && (hour==slot3) && (hour <slot3+1)  && !gsm_flag && min == 2)
		{
//			buzz();
			GSM_missed_call();	
			// if want to send message using GSM and give missed calls
		}




		if(tim_flag==1)
		{
		
		lcd_cmd(0x80);	
		
		lcd_data(hour / 0x0a + 0x30);
		lcd_data(hour % 0x0a + 0x30);
		lcd_data(':');

		lcd_data(min / 0x0a + 0x30);
		lcd_data(min % 0x0a + 0x30);
		lcd_data(':');

		lcd_data(sec / 0x0a + 0x30);
		lcd_data(sec % 0x0a + 0x30);	
		tim_flag=0;
		}

	}
}