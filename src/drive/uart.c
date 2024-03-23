#include "uart.h"



void uart_init(void){	//4800的波特率,------千万注意是在12mhz的晶振频率下------
 	
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
	while(TI==0);//在发送的时候这个位一直都是0，变为1就代表不在发了
	TI = 0;//软件复位	
}

void uart_send_string(unsigned char *str){	   
	while(*str!='\0'){
		uart_send_byte(*str);
		str++;	
	}	
}


/*--------------自己实现将double转为string然后发送出去-------------------------*/
//不知道为什么动态分配内存的就报错，非得静态分配内存
static unsigned char str[25];
void uart_send_double2string(double value){
	sprintf(str, "%.2f", value);
    uart_send_string(str);			
} 