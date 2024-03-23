#ifndef _DS1302_H
#define _DS1302_H

#include<reg51.h>
#include<intrins.h>
#include"ds1302delay.h"

//unsigned char code READ_RTC_ADDR[7] = {0x81,0x83,0x85,0x87,0x89,0x8b,0x8d}; //����ʱ��Ӧ��д�ļĴ�����ַ ,��ַ˳�����������ʱ��������
//unsigned char code WRITE_RTC_ADDR[7] = {0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};//д��ʱ��ļĴ�����ַ		��  ��ַ˳�����������ʱ��������

//�ú궨��ķ�ʽȥ����Ĵ����ľ����ַ�Է���ṹ��ȥ��λ��ַ������ֵ 

#define REG_ADDR_WRITE_YEAR 0x8c
#define REG_ADDR_WRITE_MONTH 0x88
#define REG_ADDR_WRITE_DAY 0x86
#define REG_ADDR_WRITE_HOUR 0x84
#define REG_ADDR_WRITE_MINUTE 0x82
#define REG_ADDR_WRITE_SECOND 0x80
#define REG_ADDR_WRITE_WEEK 0x8a

#define REG_ADDR_READ_YEAR 0x8d
#define REG_ADDR_READ_MONTH	0x89
#define REG_ADDR_READ_DAY 0x87
#define REG_ADDR_READ_HOUR 0x85
#define REG_ADDR_READ_MINUTE 0x83
#define REG_ADDR_READ_SECOND 0x81
#define REG_ADDR_READ_WEEK 0x8b	 





//������������
sbit DSIO = P3^5;
sbit RST =  P3^6;
sbit SCLK = P3^4;

//����ṹ������
struct allTime{
	unsigned int year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char week;
};

extern struct allTime mytime;


void ds1302_read_time_struct(void);
void ds1302_write_time_struct(struct allTime t1);


/*void ds1302_write_reg(unsigned char addr,unsigned char value);
unsigned char ds1302_read_reg(unsigned char addr);
void ds1302_write_time(void);
void ds1302_read_time(void); */
//void debug_print_time(void);
#endif