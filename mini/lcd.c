#include"defines.h"
#include"types.h"
#include<LPC21xx.H>
#include"lcd_defines.h"
#include"delay.h"
void writeLCD(u8 data)
{
	SCLRBIT(IOCLR0,RW);				  //For write operation RW should be '0', for read operation RW should be '1'.
	WRITEBYTE(IOPIN0,LCD_DATA,data);  //The received data will be written from the position LCD_DATA(8).
	SSETBIT(IOSET0,EN);
	delay_us(1);
	SCLRBIT(IOCLR0,EN);
	delay_ms(1);
}
void cmdLCD(u8 cmd)
{
	SCLRBIT(IOCLR0,RS);				  //For passing commands the RS  bit should be '0'.
	writeLCD(cmd);
}
void charLCD(u8 ascii)
{
	SSETBIT(IOSET0,RS);				  //For passing data the RS bit should be '1'.
	writeLCD(ascii);				  //Calling the writeLCD() by passing the ascii as argument. 
}
void InitLCD(void)
{
	WRITEBYTE(IODIR0,LCD_DATA,0xff);  //Setting the directions as output for the all LCD pins that is from p0.8 to p0.15 and RS,RW,EN also.
	SETBIT(IODIR0,RS);
	SETBIT(IODIR0,RW);
	SETBIT(IODIR0,EN);
	delay_ms(15);
	cmdLCD(MODE_8BIT_1LINE);
	delay_ms(5);
	cmdLCD(MODE_8BIT_1LINE);
	delay_us(100);
	cmdLCD(MODE_8BIT_1LINE);

	cmdLCD(MODE_8BIT_2LINE);
	cmdLCD(DISP_ON_CUR_OFF);
	cmdLCD(CLEAR_LCD);
	cmdLCD(SHIFT_CUR_RIGHT);
}
void strLCD(u8 *str)
{
	while(*str)						//Untill the given string position is not NULL
	{
		charLCD(*str++);			//Transmitting the each character and incrementing the position
	}
}
void u32LCD(u32 n)
{
	u8 a[10];
	s32 i=0;
	if(n==0)
	{
		charLCD('0');				//If n==0, transmitting '0' character to LCD
	}
	else
	{
		while(n)
		{
			a[i++]=(n%10)+'0';		//Last digit is extracted and converted to ascii and stored in the arrray
			n/=10;					//the last digit is removed from the actual number
		}
		for(--i;i>=0;i--)
		{
			charLCD(a[i]);			//Printing all the characters shored in the array on LCD.
		}
	}
}
void f32LCD(f32 fnum,u8 ndp)
{
	u32 inum;
	if(fnum<0.0)					//If number is negative just a '-' symbol is printed before the number
	{
		charLCD('-');
		fnum=-fnum;					//The negative number is converted to positive
	}
	inum=fnum;						//Float number is stored into integer format, just to print the number before the decimal point
	u32LCD(inum);					//Integer number is printed
	charLCD('.');					//Decimal point is printed
	while(ndp)						//ndp represents number of digits after decimal
	{
		fnum=(fnum-inum)*10;		//(fnum-inum)gives the value after the decimal point, and it is multiplies by 10 every time in the loop
		inum=fnum;					// now that number is converted to integer for the value before the decimal
		charLCD(inum+48);			//That new digit is printed
		ndp--;						//number od digits after decimal will be decremented
	}
}                            