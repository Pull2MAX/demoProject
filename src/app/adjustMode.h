#ifndef _ADJUSTMODE_H_
#define _ADJUSTMODE_H_

#include"key4_4.h"
#include"lcd1602.h"
#include"ds18b20.h"
#include"ds1302.h"
#include"buzzer.h"
#include"motor.h"
extern double alert_temperature_threshold ;
extern double Temperature;
void display_normal_temperature_time();
void WorkModeAdjust();
void KeyWorkModeAdjust();

void KeyCursorMove();//������ʵ�ֵĹ����ǵ���ģʽ��ʱ���ù����ֲ��ұ�4*4���̿������������ƶ���3->�ϣ�2->��4->��
void  keyEventHandler(unsigned char keyvalue);
#endif