#include "externs.h"

char gsm_flag = 0;
unsigned char flag = 0; // flags to check if medicines are taken or not for slot 1 and slot 2
unsigned char rec_char, slot1 = 45, slot2 =76, slot3=99;
unsigned char data_frame[10]; 
// dataframe : 1,2: first slot time 
// dataframe : 3,4: second slot time
// dataframe : 5,6: second slot time
// dataframe : 7->10 : present time
unsigned int m_sec=0;
unsigned int sec=0;
unsigned int hour=0;
unsigned int min=0;
unsigned int tim_flag=0;
unsigned int index=0;



unsigned char swap(unsigned char dat)
{
	
	unsigned char a,b,c,d;
	a=b=c=dat;
	 a=    (a & 0x80)>> 7 | (a & 0x01)<< 7 ;
	 b=    (b & 0x40)>> 5 | (b & 0x02)<< 5  ;
	c=     (c & 0x20)>> 3 | (c & 0x04)<< 3 ;
	d= 	   (dat & 0x10)>> 1 | (dat & 0x08)<< 1   |a |b |c  ;

	return d;
}


void delay(unsigned int msec)		 //Function for delay
{
	int i,j;
	for(i=0;i<msec;i++)
	    for(j=0; j<1275; j++);
}

void lcd_cmd(unsigned char item)		// Function to send command on LCD
{
	dataport = item;
	rs= 0;
	rw=0;
	en=1;
	delay(1);
	en=0;
	return;
} 



void LCD_Init (void)		/* LCD Initialize function */
{	
	delay(20);		/* LCD Power ON Initialization time >15ms */
	lcd_cmd(0x1c);	/* Initialization of 16X2 LCD in 8bit mode */
	lcd_cmd(0xf0);	/* Display ON Cursor blonk */
	lcd_cmd(0x60);	/* Auto Increment cursor */
	lcd_cmd(0x80);	/* clear display */
	lcd_cmd(0x01);	/* cursor at home position */
}


void lcd_data(unsigned char item)		// Function to display character on LCD
{
	dataport = swap(item);
	rs= 1;
	rw=0;
	en=1;
	delay(1);
	en=0;
	return;
}

void lcd_clrset()
{
	lcd_cmd(0x80);
	lcd_cmd(0x01);
}


/* Function to enable interrupt s*/
void Ext_int_Init()				
{
	EA  = 1;		/* Enable global interrupt */
	ES = 1;  		/* Enable serial interrupt */		
	ET0 = 1;
}

void lcd_write_string(unsigned char *str)
{
	int pos = 0;
	while(*str != '\0')
	{
		if(pos == 16)
			lcd_cmd(0x03);
		lcd_data(*str++);
		pos +=1;
	}
}


void UART_Init()
{
	TMOD = 0x21;		/* Timer 1, 8-bit auto reload mode */
	TH1 = 0xFD;		/* Load value for 9600 baud rate */
	SCON = 0x50;		/* Mode 1, reception enable */
	TR1 = 1;		/* Start timer 1 */
}

void Serial_ISR() interrupt 4    
{	
	if(TI)
	{
		TI = 0;
	}

	if(RI)
	{
		REN=0;
		rec_char = SBUF;		/* Give received data on port 1 */
		data_frame[index++]=rec_char-0x30;
		if(index==1 || index ==2)
		{   
			if(index == 1)
			{   //buzzer=0;
				lcd_clrset();
				lcd_write_string("enter slot 1: ");
				lcd_cmd(0x03);
				lcd_data(rec_char);
			}
			
			if(index == 2)
			{
				lcd_data(rec_char);
			}
		}


		else if(index==3 || index ==4)
		{   
			if(index == 3)
			{
				lcd_clrset();
				lcd_write_string("enter slot 2: ");
				lcd_cmd(0x03);
				lcd_data(rec_char);
			}
					
			if(index == 4)
			{
				lcd_data(rec_char);
			}
		}			

		else if(index==5 || index ==6)
		{   
			if(index == 5)
			{
				lcd_clrset();
				lcd_write_string("enter slot 3: ");
				lcd_cmd(0x03);
				lcd_data(rec_char);
			}
					
			if(index == 6)
			{
				lcd_data(rec_char);
			}
		}			

		else
		{   
			if(index == 7)
			{
				lcd_clrset();
				lcd_write_string("enter HH MM ");
				lcd_cmd(0x03);
				lcd_data(rec_char);
			}
			
			else if(index == 8)
				{
					lcd_data(rec_char);
					lcd_data(':');
				}
			else
			{
				lcd_data(rec_char);
			}
		}

		if(rec_char=='z')
			{
				lcd_data(rec_char);
				// GSM_missed_call("9624279368");
				slot1 = data_frame[0]*10 + data_frame[1];
				slot2 = data_frame[2]*10 + data_frame[3];
				slot3 =	data_frame[4]*10 + data_frame[5];
				hour  =	data_frame[6]*10 + data_frame[7];
				min  =	data_frame[8]*10 + data_frame[9];
				TR0=1;	
				ES = 0;
			}
		REN=1;
		RI=0;
					/* Clear RI flag */
	}
} 

void timer_init()
{
	TH0 = 0xF8;
	TL0 = 0xcc;
	TR0=0;
}

void TIMER_ISR() interrupt 2
{	
	TH0 = 0xF8;
	TL0 = 0xcc;
	TF0=0;
	m_sec++;
	if(m_sec == 498 )		//msec=498
	{	m_sec=0;
		sec++;
		tim_flag=1;
	}

	// generation of clock for checking the medicine slot timings
	if(sec>=60)
	{
		sec = 0;
		min++;
		if(min >= 60)
		{
			min = 0;
			hour++;
			if(hour >= 24)
			{
				hour = 0;
				flag = 0;
			}
		}
	}
}

/* Function to buzz the buzzer */
void buzz()
{	
	buzzer = 1;
	delay(600);
	buzzer=0;
	delay(300);
}

/* Function to transmit a string using UART */
void Transmit_char(char tx_data)
{
	SBUF = tx_data;		/* Load char in SBUF register */
	while (TI==0);		/* Wait until stop bit transmit */
	TI = 0;			/* Clear TI flag */
}

void Transmit_String(char *str)
{
	unsigned char i;
	for(i=0;str[i]!='z';i++)	/* Send each char of string till the NULL */
	{
		Transmit_char(str[i]);	/* Call transmit data function */
	}
}	

/* Function to give a missed call */
void GSM_missed_call()
{
	Transmit_String("ATD9624279368;\rz");
	delay(2000);
	Transmit_String("ATH\rz");
	
	gsm_flag=1;
}
