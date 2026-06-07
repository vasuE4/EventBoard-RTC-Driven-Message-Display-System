#include <LPC214x.H>
#include"lcd.h"
#include"lcd_defines.h"
#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)

//RTC  requires 32.768 KHZ clock for 1sec increment
//PREINT and PREFRAC registers divide PCLK to generate 32.768 KHZ

#define PREINT_VAL (int) ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL + 1)*32768))

//RTC control register (CCR) bit defnitions
//Bit 0 -- clock enable--> 1 = Enable RTC counter, 0 = Disable RTC counters
#define RTC_ENABLE (1<<0)

//bit 1 -- clock reset --> 1 = Reset RTC counters, 0 = Normal operation
#define RTC_RESET (1<<1)

//only for LPC2148
//bit 4 --> clock source select
// 1 = use external 32.768 KHZ oscillator
// 0 = use internal PCLK as RTC clock source
#define RTC_CLKSRC (1<<4)

#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

void RTC_Init(void);
void GetRTCTimeInfo(s32 *,s32 *,s32 *);
void DisplayRTCTime(u32,u32,u32);
void GetRTCDateInfo(s32 *,s32 *,s32 *);
void DisplayRTCDate(u32,u32,u32);

void SetRTCTimeInfo(u32,u32,u32);
void SetRTCDateInfo(u32,u32,u32);

void GetRTCDay(s32 *);
void DisplayRTCDay(u32);
void SetRTCDay(u32);

s32 hour,min,sec,date,month,year,day;
char week[][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

#define _LPC2148

void RTC_Init(void)
{
	//display and reset the rtc
	CCR = RTC_RESET;

	#ifndef _LPC2148

	//set prescalar integer and fractional parts
	PREINT = PREINT_VAL;
	PREFRAC = PREFRAC_VAL;

	//enable the RTC
	CCR = RTC_ENABLE;
	#else
	//enable the RTC with external clock source
	CCR = RTC_ENABLE | RTC_CLKSRC;
	#endif
}

void GetRTCTimeInfo(s32 *hour,s32 *min,s32 *sec)  //This function used to read the Regiter values that is time.
{
	*hour=HOUR;
	*min=MIN;
	*sec=SEC;
}

void DisplayRTCTime(u32 hour,u32 min,u32 sec)	  //This function used to diaplat the time
{
	//cmdLCD(GOTO_LINE1_POS0);
	charLCD(hour/10 + 48);
	charLCD(hour%10 + 48);
	charLCD(':');
	charLCD(min/10 + 48);
	charLCD(min%10 + 48);
	charLCD(':');
	charLCD(sec/10 + 48);
	charLCD(sec%10 + 48);
}

void GetRTCDateInfo(s32 *date,s32 *month,s32 *year)	//This function is used to read the Date
{
	*date=DOM;
	*month=MONTH;
	*year=YEAR;
}

void DisplayRTCDate(u32 date,u32 month,u32 year)	//This function is used to display date
{
	//cmdLCD(GOTO_LINE1_POS0);
	charLCD(date/10 + 48);
	charLCD(date%10 + 48);
	charLCD('/');
	charLCD(month/10 + 48);
	charLCD(month%10 + 48);
	charLCD('/');
	u32LCD(year);
}

void SetRTCTimeInfo(u32 hour,u32 min,u32 sec)		//This function is used to set the time manually
{
	HOUR = hour;
	MIN = min;
	SEC = sec;
}

void SetRTCDateInfo(u32 date,u32 month,u32 year)	//This function is used to set the date manually
{
	DOM = date;
	MONTH = month;
	YEAR = year;
}

void GetRTCDay(s32 *dow)							//This function is used to read the Day
{
	*dow = DOW;
}

void DisplayRTCDay(u32 dow)							//This function is used to display the day
{
	//cmdLCD(GOTO_LINE1_POS0 + 10);
	strLCD((u8 *)week[dow]);
}

void SetRTCDay(u32 dow)								//This function is used to set the day manually
{
	DOW = dow;
}
 
