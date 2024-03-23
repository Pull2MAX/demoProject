#ifndef _MOTOR_H_
#define _MOTOR_H_
/*
注意本开发板的步进电机的项目需要的芯片是开发板上的四相双极性步进电机，
先把gpio接到这个芯片的四个in口，再由这个芯片的四个out口和我们的步进电机相连
*/
#include<reg51.h>
sbit Apo = P2^1;
sbit Ane = P2^2;
sbit Bpo = P2^3;
sbit Bne = P2^4;

#define num1 300
#define num2 300

void delay_motor(unsigned char count1,unsigned char count2);

void motor_run_alert();

#endif