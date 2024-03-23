#ifndef _MOTOR_H_
#define _MOTOR_H_
/*
ע�Ȿ������Ĳ����������Ŀ��Ҫ��оƬ�ǿ������ϵ�����˫���Բ��������
�Ȱ�gpio�ӵ����оƬ���ĸ�in�ڣ��������оƬ���ĸ�out�ں����ǵĲ����������
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