#ifndef _LCD1602_H_
#define _LCD1602_H_

#include<reg51.h>
#include"ds1302.h"
#include<stdio.h>
//#include <stdlib.h>
//#include<string.h>

/*------�ӿڶ���--------*/

// I/O�ӿڵĶ���
#define LCD1602_DB P0
// �������ߵĶ���
sbit LCD1602_RW = P2^5;
sbit LCD1602_RS = P2^6;
sbit LCD1602_EN = P2^7;	  //ȡ1��ʱ��Ͷ�ȡ����Ϣ�����õ�ʱ��ͱ�Ϊ0�ص�

/*-------ֻд�߲�ʱ������ԭ����ֻ�������������Ǳ������õģ����ײ�ʱ�����ǲ��ᱻ���õ�-------*/ 
void InitLCD1602(void);
void LCDSetCursor(unsigned char x,unsigned char y);
void LCDshowStr(unsigned char x,unsigned char y,unsigned char *str);   //x��������У� y��������У�����yҪô��0Ҫô��1
void lcdshowDouble2Str(unsigned char x,unsigned char y,double value);//������ֱ�ӽ���double���͵ı�����Ȼ��תΪstr���Ͳ��Ҵ�ӡ����Ļ��
void lcdshowInt2Str(unsigned char x,unsigned char y,unsigned int value);

//void lcdshowStruct(unsigned char x,unsigned char y,struct allTime value);//���ṹ�����ͱ������������ƴ��һ���ַ���Ȼ����ʾ��1602ָ���ط�

void lcdshowStruct2(unsigned char x,unsigned char y,struct allTime mytime);//�ο���ʦ��˼·ʵ�ֵ�

//lcd1602������ģʽ��ȥ�����
void LCDNormalMode();

//LCD1602�ĵ���ģʽ����ʾ���
void LCDAdjustMode();

#endif