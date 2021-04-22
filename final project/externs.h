#ifndef EXTERNS_H
#define EXTERNS_H


#include <reg51.h>		/* Include x51 header file */
#define port P0
#define dataport P2							 


/* Global Variables Declaration */

extern char gsm_flag;

sbit rs = port^2;
sbit rw = port^1;
sbit en = port^0;

sbit IR = P1^4;
sbit buzzer = P3^7;
sbit motor = P3^3;


extern unsigned char flag; // flags to check if medicines are taken or not for slot 1 and slot 2
extern unsigned char rec_char, slot1, slot2, slot3;
extern unsigned char data_frame[10]; 
// dataframe : 1,2: first slot time 
// dataframe : 3,4: second slot time
// dataframe : 5->8 : present time
extern unsigned int m_sec, sec, hour, min, tim_flag, index;

/******************************************************************************************************/

/****** Function Declarations *******/


/* Declarations of UART funcitons */
void UART_Init();
void Transmit_char(char tx_data);
void Transmit_String(char *str);


/* Declaration of function of LCD */

void lcd_cmd(unsigned char item);			// Function to send command on LCD
void lcd_init();
void lcd_data(unsigned char item);			// Function to display character on LCD
void lcd_clrset();							// clear LCD display and reset cursor position
void lcd_write_string(unsigned char *str);


/* Declaration of Timer Functions */

void timer_init();
void delay(unsigned int msec);		 //Function for delay


/* Declarations of GSM functions */
void GSM_missed_call();

/* Miscellenious */
void Ext_int_Init();  	/* Call Ext. interrupt initialize */
void buzz();
void init_variables();
unsigned char swap(unsigned char dat);


#endif