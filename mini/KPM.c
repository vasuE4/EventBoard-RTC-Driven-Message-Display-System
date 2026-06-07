#include <LPC21xx.H>
#include"defines.h"
#include"delay.h"
#include"lcd.h"
#include"lcd_defines.h"
#include"types.h"
#include"KPM_defines.h"
u8 KPMLUT[4][4]={{'1','2','3','+'},{'4','5','6','-'},{'7','8','9','*'},{'0','=','/','C'}};
void InitKPM(void)
{
	// KPM will bw ported to port 1.16 to 1.19
	WRITEBYTE(IODIR1,ROW0,15);	  //Setting the direction as output for ROW0,ROW1,ROW2,ROW3.
}
u8 colscan(void)
{
	if(READNIBBLE(IOPIN1,COL0)<15)	 //If any of the key is pressed then the value of the NIBBLE will be lessthan 15.
		return 0;					 //if pressed return 0,else return 1.
	else
		return 1;
}
u8 rowcheck(void)
{
	u8 rno;
	for(rno=0;rno<=3;rno++)					 //Iterating the loop from row0 to row3
	{
		WRITENIBBLE(IOPIN1,ROW0,~(1<<rno));	 //While checking the particular row, make that row as '0' and other as '1'.
		if(colscan()==0)					 //Checking the colscan() whether the key is pressed on not
			break;							 //If pressed loop will be breaked
	}
	WRITENIBBLE(IOPIN1,ROW0,0x0);			 //Making the all rows '0' again.
	return rno;								 //returning the rno, if key is pressed the row no will be less that 4, if not pressed the row no will be 4.
}
u8 colcheck(void)
{
	u8 cno;									 //Iterating the loop from '0' to '3'
	for(cno=0;cno<=3;cno++)
	{
		if(STATUSBIT(IOPIN1,COL0+cno)==0)	 //Checking the status of each column seperately
			break;							 //If '0' that is if pressed, it will break and return the cno.
	}
	return cno;
}
u8 keyscan(void)
{		 
	u8 keyv,rno,cno;
	while(colscan());						//This loop will be executing untill some key is pressed.
	rno=rowcheck();							//The value returned from rowcheck() will be stored in rno.
	cno=colcheck();							//The value returned from colcheck() will be stored in cno.
	keyv=KPMLUT[rno][cno];					//Based on rno and cno, keyv value is decided from the 2D array written aboove.
	while(!colscan());						//This loop will be executing untill we release the key.
	return keyv;							//Returning the value from the 2D matrix.
}
u32 readnum(void)
{
	u32 sum=0;								 //Initially taken sum as '0'.
	u8 key;
	while(1)
	{
		key=keyscan();						 //Reads a key value from keypad
		while(!colscan());					 //Loop will execute untill the key is released.
		cmdLCD(0x01);
		delay_ms(2);
		cmdLCD(0x80);
		charLCD(key);						 //Displaying the entered key
		if((key>='0') && (key<='9'))		 //If entered key is digit
		{
			sum=(sum*10) + (key-48);		 //Will be converting from ascii to integer and stored in the sum
		}
		else if(key=='C'||key=='=')			 //This indicates as an enter key, that means entering number is finished.
		{
			break;
		}
	}
	return sum;								 //Will be returning the final integer value.
}
