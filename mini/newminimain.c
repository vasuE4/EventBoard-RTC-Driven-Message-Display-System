#include <LPC21xx.H>
#include<stdlib.h>
#include<string.h>
#include"lcd_defines.h"
#include"lcd.h"
#include"KPM.h"
#include"minimain_rtc.h"
#include"delay.h"
#include"types.h"
#include"ADC.h"
#define totalmsgs 10
#define EINT0_CHNO 14
extern s32 hour,min,sec;
typedef struct
{
	u8 hour;                        //Requires structure
	u8 min;
	u8 sec;
	char *text;
	u8 enabled;
}message;
message msglist[totalmsgs]={{7,45,00,"Good morning class starts soon ",1},                    //1st value is hours,2nd is minutes,3rd is seconds,4th is the event msg,5th is enable/disable value
								{13,45,00,"C programming session in classroom number 2 ",1},
								{10,15,00,"C module theory exam in 4th floor lab1 ",1},
								{10,20,00,"C module lab exam in 4th floor lab2 and middle lab at 10:30 AM ",1},
								{12,45,00,"Lunch break from 1PM-2PM ",1},
								{9,45,00,"ARM workshop on external interrupts in lab1 at 10 AM ",1},
								{9,25,00,"ARM kit issue time from 10 AM-10:30 AM in middle lab ",1},
								{15,15,00,"Only 15 mins break time for next arm session",1},
								{17,00,00,"Revise today's class programs at home! ",1},
								{17,45,00,"End of the day - see you tomorrow! ",1}};
void scrolllcd(char *str,int ind)
{
	s32 i,j,k,l,p,h;
	char window[18];
	int len=strlen(str); 											//Calculates the length of the str that is event msg
	if(len<=24)												//This  condition is for the string of length less than or equal to 24
	{													//This is because the LCD is having a Hidden memory concept,after the buffer of the 1st line filled,it will jump  to the second line, so to avoid this seperate logic is written for string which has more than 24 characters.
		for(i=15;i>=0;i--)										//This block of code is completely optional, you can directly use the else block directly any any length of strings.
		{												//This loop will be running for 16 times as we are using 16x2 LCD
			GetRTCTimeInfo(&hour,&min,&sec);							
			cmdLCD(0xc0);
			if(min<=msglist[ind].min+14 && hour==msglist[ind].hour)					//This statement checks whether the present time is below the 15 minutes after the event time
			{
			DisplayRTCTime(hour-hour,14-(min-msglist[ind].min),59-(sec-msglist[ind].sec));		//This logic will show the countdown, that is still how much time the msg should be scrolled
			}
			else
			{
			DisplayRTCTime(00,00,00);								//If the Scrolling time is already completed it wil show 00:00:00.
			}
			cmdLCD(0x80);
			if(i==0)						//When you are scrolling the text and the curser reaches the starting of the lcd then the first character should be removed
			{
				k=0;
				for(j=len-1;j>=0;j--)				//This loop will run for the length of the string times
				{
			 	cmdLCD(GOTO_LINE1_POS0);			//Always pointing to the 1st position 
			 	strLCD((u8 *)str+k);				//Print the string from the kth address in the string, everytime the k will be incremented 
			 	k++;
			 	delay_ms(500);
				}
			}
			else
			{
				cmdLCD(GOTO_LINE1_POS0 + i);			//This block will work in the initial stage of scrolling that it should scroll from right to left,the lcd position will be decremented from right to left to print the string
				strLCD((u8 *)str);
				delay_ms(500);
			}
		}
	}
	else									//This logic is for the string having morethan 24 characters, you can use this block directly instead of the above block if required
	{
			for(p=15;p>=0;p--)					//This loop runs 16 times as we use 16x2 LCD
			{
			  GetRTCTimeInfo(&hour,&min,&sec);			//Reads the time from the RTC registers
			  cmdLCD(0xc0);
			  if(min<=msglist[ind].min+14 && hour==msglist[ind].hour)	//This statement will check whether the time is within the range	
				{
				DisplayRTCTime(hour-hour,14-(min-msglist[ind].min),59-(sec-msglist[ind].sec));	//This is display the countdown time that still how much time should the  msg should scroll
				}
			  else
				{
				DisplayRTCTime(00,00,00);			//If the time is completed then it will print 00:00:00.
				}
			  if(p==0)						//Here p is nothing but the position, if the string reached the 1st position while scrolling from right to left
			  {
				for(i=0;i<len-16;i++)				//This loop will run len-16 times because, it already scrolled 16 characters, now it need to scroll len-16 characters only
				{
					GetRTCTimeInfo(&hour,&min,&sec);	//Reading the time from registers repeatedly because to avoid the time mismatch delays
					cmdLCD(0xc0);
		        	if(min<=msglist[ind].min+14 && hour==msglist[ind].hour)		//This block will print the  countdown time.
					{
					DisplayRTCTime(hour-hour,14-(min-msglist[ind].min),59-(sec-msglist[ind].sec));
					}
					else
					{
					DisplayRTCTime(00,00,00);
					}
					for(j=0;j<16;j++)			//Now before scrolling the 16 characters from the required position is copied tothe dummy string, to avoid buffer issues
					{
							window[j]=str[i+j];
					}
					window[17]='\0';
					cmdLCD(0x80);
					strLCD((u8 *)window);			//That particular copied dummy string is displayed
					delay_ms(500);				//This loop will run for len-16 times and scroll upto len-16 characters
				}
				cmdLCD(0x80);
				for(h=len-16;h<=len;h++)			//Now it is scrolled upto len-16 characters, but the last 16characters should also be scrolled off, that is should be deleted from strting
				{
					GetRTCTimeInfo(&hour,&min,&sec);		//Repeatedly reading the RTC information to avoid tie mismatch issues
					cmdLCD(0xc0);
			        if(min<=msglist[ind].min+14 && hour==msglist[ind].hour)		//This particular block will print the countdown time that still how much time should the msg should scroll
					{
					DisplayRTCTime(hour-hour,14-(min-msglist[ind].min),59-(sec-msglist[ind].sec));
					}
					else
					{
					DisplayRTCTime(00,00,00);
					}
					cmdLCD(GOTO_LINE1_POS0);		//Always pointing to the position 1
			 		strLCD((u8 *)str+h);			//Printing the string from the hth  address, h will be incrementing everytime.
			 		delay_ms(500);
		     	 }
			}							//This block is for scrolling when the text reaches the LCD position 1
			  else							//This block of code will work when the text is at position 16 
			  {
			    GetRTCTimeInfo(&hour,&min,&sec);			//Repeatedly reads RTC information
				cmdLCD(0xc0);
	     		if(min<=msglist[ind].min+14 && hour==msglist[ind].hour)	//This block prints the countdown time
				{
				DisplayRTCTime(hour-hour,14-(min-msglist[ind].min),59-(sec-msglist[ind].sec));
				}
				else
				{
				DisplayRTCTime(00,00,00);
				}
				for(l=0;l<16;l++)	
				{				
					window[l]=str[l];			//Copies the same string every time, instead of loop you can directly use strncpy.
				}
				window[17]='\0';
				cmdLCD(GOTO_LINE1_POS0 +p);			//Printing the text from the pth position in the LCD.Position will be decremented everytime
				strLCD((u8 *)window);
				delay_ms(500);
			   }
			}
	}
}

void eint0_isr(void)__irq
{
	u8 op,ans;
	u32 h,m,s,d,mo,y,da,ind;
	cmdLCD(0x01);
	cmdLCD(0x80);
	strLCD("1.RTC2.msg3.ext");
	op=keyscan();							//Takes the input for selecting the option
	switch(op)
	{
		case '1': cmdLCD(0x01);					//If case1 is selected RTC information will be edited
				  delay_s(1);
				  cmdLCD(0x80);
				  strLCD("Enter the time:");
				  h=readnum();				//Takes the input in hours and display for confirmation
				  cmdLCD(0x80);
				  u32LCD(h);
				  delay_s(1);
				  cmdLCD(0x01);

				  cmdLCD(0x80);
				  m=readnum();				//Takes the input in minutes and display for confirmation
				  u32LCD(m);
				  delay_s(1);
				  s=readnum();				//Takes the input in seconds and display for confirmation
				  cmdLCD(0x80);
				  u32LCD(s);
				  delay_s(1);
				  SetRTCTimeInfo(h,m,s);		//These taken input values will be setted to the RTC registers
				  cmdLCD(0x01);
				  delay_ms(2);
				  cmdLCD(0x80);
				  strLCD("Enter the date: ");
				  d=readnum();				//Takes the input in date and display for confirmation
				  cmdLCD(0x80);
				  u32LCD(d);
				  delay_s(1);
				  cmdLCD(0x80);
				  mo=readnum();				//Takes the input in month and display for confirmation
				  u32LCD(mo);
				  delay_s(1);
				  y=readnum();				//Takes the input in year and display for confirmation
				  cmdLCD(0x80);
				  u32LCD(y);
				  delay_s(1);
				  SetRTCDateInfo(d,mo,y);		//These taken input values will be setted to the RTC registers
				  cmdLCD(0x01);
				  delay_ms(2);
				  cmdLCD(0x80);
				  strLCD("Enter the day: ");
				  da=readnum();				//Takes the input in day and display for confirmation
				  cmdLCD(0x80);
				  u32LCD(da);
				  delay_s(1);
				  cmdLCD(0x01);
				  SetRTCDay(da);			//These taken input values will be setted to the RTC registers
				  break;
		case '2': cmdLCD(0x01);					//If case2 is selected, we can enable or disable the event msg
				  cmdLCD(0x80);
				  strLCD("Enter the index:");
				  delay_s(1);
				  ind=readnum();			//Takes the input as index of the event msg which we want to Enable or disable and display for confirmation
				  cmdLCD(0x80);
				  u32LCD(ind);
				  delay_s(1);
				  cmdLCD(0x01);
				  delay_ms(2);
				  cmdLCD(0x80);
				  strLCD("enable or disable: ");
				  delay_s(1);
				  ans=keyscan();			//Will take the input as enable or disable,if want to enable enter '1',if want to disable enter '0'.
				  cmdLCD(0x01);
				  delay_ms(2);
				  cmdLCD(0x80);
				  charLCD(ans);
				  delay_s(1);
				  cmdLCD(0x01);
				  msglist[ind].enabled=ans;		//Will update the taken input into the structure member enabled of particular index 
				  break;
		case '3': exit(0);					//If case3 is selected,exit.
	}
	VICVectAddr=0;							//Registers are cleared, so that we can raise the interrupt again in future if needed.
	EXTINT=1<<0;

}
void eint0_enable(void)
{
	PINSEL0 |= 0x0000000c;						//Pin p0.1 is selected for EINT0.
	VICIntSelect=0<<EINT0_CHNO;					//Selected as IRQ interrupt
	VICIntEnable=1<<EINT0_CHNO;					//Interrupt is enabled
	VICVectCntl0=(1<<5)|EINT0_CHNO;					//Selected for slot0
	VICVectAddr0=(unsigned int)(eint0_isr);				//ISR block is called
	EXTMODE=1<<0;							//Selected for EDGE triggering
	EXTPOLAR=0<<0;							//Selected for falling edge as the LPC2148 does'nt support the raising edge. 
}
int main()
{
	int i;
	s32 date,month,year,dow;
	int flag=0;							 //This flag is used to check whether the event is matched or not
	u32 adcval;
	f32 eAR;
	char copy[16];						 //Just a dummy string to copy the matched event text
	//char time_str[16];
	InitLCD();							 //Initializing all the requirements
	RTC_Init();
	Init_ADC();
	InitKPM();
	eint0_enable();						 //Enabling the external interrupt
	SetRTCTimeInfo(17,14,00);			 //Setting the Time manually
	SetRTCDateInfo(23,8,2003);			 //Setting the date manually
	Read_adc(1,&adcval,&eAR);			 //This is the function call used to read the adc values and store in adcval and eAR
	while(1)
	{
		
   		cmdLCD(0x80);
		flag=0;
		for(i=totalmsgs-1;i>=0;i--)								//Will be iterated for requires number of times
		{	  	
			if((msglist[i].hour==hour)&& (min>=msglist[i].min && min<=msglist[i].min+14))	//This is the condition that it will accept the present time is greater than or equal to event time and less than orequal to event time plus 15 minutes, because the msg should be scrolling for 15 minutes
			{
				if(msglist[i].enabled==1)						//If msg is enabled then only it will enter this block
					strcpy(copy,msglist[i].text);					//If enabled the event msg will be copied to the dummy variable 
				else
				 	strcpy(copy,"                 ");				//If not enabled then we should not print the msg, so all spaces are copied to that dummy variable
				flag=1;									//This flag is useful for knowing whether the event is matched or not
				break;
			}
		}
		if(flag==1)										//If  event is matched then will enter this block
		{
			cmdLCD(0x01);
			cmdLCD(0x80);
			scrolllcd(copy,i);								//That copied text present in the dummy variable will be sent to the scrolling function
		}
		else
		{
			GetRTCTimeInfo(&hour,&min,&sec);						//If event not matched then we read the date,time,day and room temperature and display them
			GetRTCDateInfo(&date,&month,&year);
			GetRTCDay(&dow);
			cmdLCD(0x80);
			DisplayRTCTime(hour,min,sec);
			strLCD("     ");
			DisplayRTCDay(dow);
			cmdLCD(0xC0);
			DisplayRTCDate(date,month,year);
			strLCD("  ");
			f32LCD(eAR,2);
		}
    }
}
