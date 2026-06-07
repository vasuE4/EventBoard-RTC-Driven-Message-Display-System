#include"types.h"
#include"delay.h"
#include<LPC21XX.h>
void Init_ADC(void)
{
    // cfg P0.28 as AIN1 (CH1)
    PINSEL1 |= 1 << 24;
    // activate ADC, CLK freq
    ADCR = (1 << 21) | (4 << 8);  // CLKDIV value
}
void Read_adc(u32 chno, u32 *adcval, f32 *eAR)
{
    ADCR &= ~(255 << 0);           // clear channel values
    ADCR |= (1 << chno) | (1 << 24);  // select ch, start conv
    delay_us(3);
    while (((ADDR >> 31) & 1) == 0);
    ADCR &= ~(1 << 24);           // stop conv
    *adcval = ((ADDR >> 6) & 1023);
    *eAR    = 0.32258* (*adcval);
}
