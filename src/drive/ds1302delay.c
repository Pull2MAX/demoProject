#include"ds1302delay.h"

void delay1s(void)   
{
    unsigned char a,b,c;
    for(c=167;c>0;c--)
        for(b=171;b>0;b--)
            for(a=16;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
} 

	 

void delay900ms(void)   //Îó²î 0us
{
    unsigned char a,b,c;
    for(c=119;c>0;c--)
        for(b=216;b>0;b--)
            for(a=16;a>0;a--);
} 

