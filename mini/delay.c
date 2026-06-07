void delay_us(unsigned int tdly)   //This function is used for the micro seconds delay
{
	tdly*=12;
	while(tdly--);
}
void delay_ms(unsigned int tdly)   //This function is used for milli seconds delay
{
	tdly*=12000;
	while(tdly--);
}
void delay_s(unsigned int tdly)	   //This function is used for seconds delay.
{
	tdly*=12000000;
	while(tdly--);
}

