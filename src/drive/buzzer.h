#ifndef _BUZZER_H_
#define _BUZZER_H_

#include<reg51.h>

//���Ŷ���
sbit buzzer = P2^0;

/*#define xkhz 4	//����buzzer��Ƶ��
#define us (500/xkhz)
#define time (65535-us)
#define stateSwitchTime 600 //������������л�ʱ��	*/


void buzzer_init();//�������ֻ����ʼ�����ã�������һ��ʼ�ǲ����

void buzzer_start();//����buzzer��ʼ��

void buzzer_stop();//����buzzer��Ҫ��

void buzzer_freq_set(unsigned char howmanykhz);


#endif