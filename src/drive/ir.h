#ifndef _IR_H_
#define _IR_H_

#include<reg51.h>
//#include"uart.h"

/*ȫ�ֱ������ĸ��ļ������þ����ĸ��ļ���c�ļ��ﶨ�壬��Ҫ������ͷ�ļ������Է����ظ����壬
������c��Ҫ����ͬһ����������ô����extern�ؼ���*/
sbit IRIN = P3^2;

extern unsigned char IrValue[6];//ȫ�ֱ����������ж�ȥ���ϸ��� ���±�0-3�Ƿŵ� ���յ��ĺ��������ݣ���ַ����ַ���롢�������룩���жϵ�����Լ����õĻ��������ֵ����IrValue[4]
//extern unsigned char Time;


void IrInit(void);


#endif