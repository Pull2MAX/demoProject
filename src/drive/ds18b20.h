//������ʹ�õ��Ǿ�����12MHZ
#ifndef __DS18B20_H__
#define __DS18B20_H__

#include<reg51.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif


sbit DSPORT = P3^3;
//uchar Ds18b20Init();
//uint Ds18b20Readtemperature(void);
double Ds18b20ReadDecimalTemperature(void);  //��߲�ĵ��ã�ֱ�ӷ���double���͵�����

#endif
