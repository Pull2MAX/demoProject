//本测试使用的是晶振是12MHZ
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
double Ds18b20ReadDecimalTemperature(void);  //最高层的调用，直接返回double类型的数据

#endif
