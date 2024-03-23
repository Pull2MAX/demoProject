#ifndef _LCD1602_H_
#define _LCD1602_H_

#include<reg51.h>
#include"ds1302.h"
#include<stdio.h>
//#include <stdlib.h>
//#include<string.h>

/*------接口定义--------*/

// I/O接口的定义
#define LCD1602_DB P0
// 控制总线的定义
sbit LCD1602_RW = P2^5;
sbit LCD1602_RS = P2^6;
sbit LCD1602_EN = P2^7;	  //取1的时候就读取下信息，不用的时候就变为0关掉

/*-------只写高层时序函数的原因是只有这三个函数是被外界调用的，而底层时序函数是不会被调用的-------*/ 
void InitLCD1602(void);
void LCDSetCursor(unsigned char x,unsigned char y);
void LCDshowStr(unsigned char x,unsigned char y,unsigned char *str);   //x代表的是列， y代表的是行，而且y要么是0要么是1
void lcdshowDouble2Str(unsigned char x,unsigned char y,double value);//本函数直接接收double类型的变量，然后转为str类型并且打印在屏幕上
void lcdshowInt2Str(unsigned char x,unsigned char y,unsigned int value);

//void lcdshowStruct(unsigned char x,unsigned char y,struct allTime value);//将结构体类型变量里面的内容拼成一个字符串然后显示在1602指定地方

void lcdshowStruct2(unsigned char x,unsigned char y,struct allTime mytime);//参考老师的思路实现的

//lcd1602的正常模式，去除光标
void LCDNormalMode();

//LCD1602的调整模式，显示光标
void LCDAdjustMode();

#endif