#include"buzzer.h"

static unsigned int count = 0;
static unsigned int flag = 0; //������û�������� ,1������������0����û������
static unsigned char mykhz;
static unsigned char th0;
static unsigned char tl0;


void timer0_isr(void) interrupt 1{ //�жϴ������ ÿ���256us�ͻ��ȥһ��
//	TL0 = (65535-(500/mykhz))%256;
//	TH0 = (65535-(500/mykhz))/256;	���ε�������ԭ����Ҫ�����жϺ�Ƭ���Լ�����ģ����ǻᵼ��Ƶ�ʲ��ԣ����������������֮��ֱ�Ӹ�ֵ����Ƭ�����Կ��ټ���
	TH0 = th0;
	TL0 = tl0;
	
 
	if(flag == 1){
		buzzer = !buzzer;
	}
}

void buzzer_init(){		
	mykhz = 1;//����һ��Ĭ�ϳ�ʼֵ
	TMOD = 0x01;
	tl0 = (65535-(500/mykhz))%256;  //���������ǵ������ȼ���ã�������жϵ�ʱ��ֱ�Ӹ�ֵ����Ƭ��
	th0 = (65535-(500/mykhz))/256;

//	TH0 = th0;
//	TL0 = tl0;
	
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	
	buzzer = 1;
	flag = 0;//1��ʾ�������е�״̬ 0��ʾ��Ĭ״̬
	

}


void buzzer_start(){//����buzzer��ʼ��
	flag = 1;
}

void buzzer_stop(){
	flag = 0;
}

void buzzer_freq_set(unsigned char howmanykhz){
	tl0 = (65535-(500/howmanykhz))%256;
	th0 = (65535-(500/howmanykhz))/256;

}