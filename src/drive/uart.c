#include "uart.h"



void uart_init(void){	//4800�Ĳ�����,------ǧ��ע������12mhz�ľ���Ƶ����------
 	
	PCON = 0x80;//		  
	SCON = 0x50;//0 1 0 1  0 0 0 0

	//
	TMOD = 0x20;//0010 0000  
	TL1 = 243;
	TH1 = 243;
	
	TR1 = 1;//
	ES = 1;
	EA = 1;

 }
void uart_send_byte(unsigned char content){
	SBUF = content;
	while(TI==0);//�ڷ��͵�ʱ�����λһֱ����0����Ϊ1�ʹ����ڷ���
	TI = 0;//�����λ	
}

void uart_send_string(unsigned char *str){	   
	while(*str!='\0'){
		uart_send_byte(*str);
		str++;	
	}	
}


/*--------------�Լ�ʵ�ֽ�doubleתΪstringȻ���ͳ�ȥ-------------------------*/
//��֪��Ϊʲô��̬�����ڴ�ľͱ����ǵþ�̬�����ڴ�
static unsigned char str[25];
void uart_send_double2string(double value){
	sprintf(str, "%.2f", value);
    uart_send_string(str);			
} 