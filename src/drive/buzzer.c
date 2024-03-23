#include"buzzer.h"

static unsigned int count = 0;
static unsigned int flag = 0; //控制有没有声音的 ,1代表有声音，0代表没有声音
static unsigned char mykhz;
static unsigned char th0;
static unsigned char tl0;


void timer0_isr(void) interrupt 1{ //中断处理程序 每间隔256us就会进去一次
//	TL0 = (65535-(500/mykhz))%256;
//	TH0 = (65535-(500/mykhz))/256;	屏蔽的这两句原来是要进入中断后单片机自己计算的，但是会导致频率不对，因此在主机计算完之后直接赋值给单片机可以快速计算
	TH0 = th0;
	TL0 = tl0;
	
 
	if(flag == 1){
		buzzer = !buzzer;
	}
}

void buzzer_init(){		
	mykhz = 1;//设置一个默认初始值
	TMOD = 0x01;
	tl0 = (65535-(500/mykhz))%256;  //这里是我们的主机先计算好，随后在中断的时候直接赋值给单片机
	th0 = (65535-(500/mykhz))/256;

//	TH0 = th0;
//	TL0 = tl0;
	
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	
	buzzer = 1;
	flag = 0;//1表示处于鸣叫的状态 0表示静默状态
	

}


void buzzer_start(){//控制buzzer开始响
	flag = 1;
}

void buzzer_stop(){
	flag = 0;
}

void buzzer_freq_set(unsigned char howmanykhz){
	tl0 = (65535-(500/howmanykhz))%256;
	th0 = (65535-(500/howmanykhz))/256;

}