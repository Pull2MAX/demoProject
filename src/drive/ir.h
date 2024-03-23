#ifndef _IR_H_
#define _IR_H_

#include<reg51.h>
//#include"uart.h"

/*全局变量在哪个文件里面用就在哪个文件的c文件里定义，不要定义在头文件里面以防被重复定义，
如果多个c需要访问同一个变量，那么就用extern关键字*/
sbit IRIN = P3^2;

extern unsigned char IrValue[6];//全局变量，会由中断去不断更新 ，下标0-3是放的 接收到的红外线数据（地址、地址反码、命令、命令反码），中断的最后自己设置的会把命令数值放在IrValue[4]
//extern unsigned char Time;


void IrInit(void);


#endif