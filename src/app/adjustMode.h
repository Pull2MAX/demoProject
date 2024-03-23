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

void KeyCursorMove();//本函数实现的功能是调整模式的时候，让光标出现并且被4*4键盘控制上下左右移动，3->上，2->左，4->右
void  keyEventHandler(unsigned char keyvalue);
#endif