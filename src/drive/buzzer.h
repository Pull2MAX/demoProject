#ifndef _BUZZER_H_
#define _BUZZER_H_

#include<reg51.h>

//引脚定义
sbit buzzer = P2^0;

/*#define xkhz 4	//控制buzzer的频率
#define us (500/xkhz)
#define time (65535-us)
#define stateSwitchTime 600 //代表蜂鸣器的切换时间	*/


void buzzer_init();//这个函数只做初始化作用，蜂鸣器一开始是不响的

void buzzer_start();//控制buzzer开始响

void buzzer_stop();//控制buzzer不要响

void buzzer_freq_set(unsigned char howmanykhz);


#endif