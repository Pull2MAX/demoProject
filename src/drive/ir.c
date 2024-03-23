#include"ir.h"

unsigned char IrValue[6];
static unsigned char Time;
void IrInit(void){ 
/*
1.接受处是需要中断的
2.初始化函数里面需要把中断打开
3.红外线的接收端是有一个反相(有载波和无载波颠倒 )功能的，因此解调的时候就会把无发送信号时候解析为1，把有信号解析为0 

【反相的理解】看nec协议的原理图可以知道，假设发送的时候10ms载波高电平(1)和40ms载波没有（0）表示1，10ms载波高电平(1)和20ms载波低电平（0）表示0。
那么接收的时候就会先接收到10ms载波高电平，会立马将其变成10ms没有载波。40ms的载波没有会变成40ms的载波高电平。至于说10ms高+40ms没有载波表示什么意思，是由接收端的解码算法定义的。	
*/

	IT0 = 1;//看协议图可以知道是下降沿触发，因为发送端就是上升沿触发，反相后就是下降沿触发
	EX0 = 1;//我的红外中断是接在p3.2上的，这个单片机端口是INT0。打开中断0允许
	EA = 1;//打开总中断
	
	IRIN = 1; //初始化端口	
}
static void DelayMs(unsigned int x)   //135us
{
	unsigned char i;
	while(x--)
	{
		for (i = 0; i<13; i++)
		{}
	}
}

void ReadIr() interrupt 0{	  //在init之中开启中断处理程序，红外线的内容交由单片机处理
	
	unsigned char j = 0,k = 0;
	unsigned int err = 0;
	/*
	//调试信息，判断串口是不是可以正确输出
	uart_init();
	uart_send_byte(6); 确实可以发出正确的串口信息*/	
	//IrValue[0] = 0xab;
	/*
	IrValue[1] = 0xab;
	IrValue[2] = 0xab;
	IrValue[3] = 0xab;	  //如果有中断的话，那么数组确实可以改变  */
	Time = 0;
	DelayMs(40);//接收端应该是先有9ms的低电平，然后再有一段时间的高电平，经过debug可以知道这个时间实际上是4.8ms（算上调用时间）
	//IrValue[0] = 0x11;
	if(IRIN == 0){ //过了小于9ms的时间，再看看引脚是不是0，确认是否真的接收到了信号
		IrValue[3] = 0x22;	
		err = 1000;
		while((IRIN==0)&&(err>0)){ //小于9ms时间结束后看看IRIN是不是还是0，如果还是0，那么就给予一定的时间去等待（最多等待的时间是	136us*1000），如果最后一直不会变1，那么【err超时机制会自动退出】
			DelayMs(1);//执行一次大概是136us
			err--;//【超时机制】，不傻等irin=1	
		}
		
		if(IRIN == 1){	//大约9ms的低电平后 出现了高电平
			//和上面的逻辑不太一样，这里是直接用while去消耗等待4.5ms
			err = 500;
			while((IRIN == 1)&&(err > 0)){	//每每间隔136us去检测IRIN是不是0，最多消耗500*136us	 = 68ms。根据协议图，1会持续4.5ms，所以一定时间后这个while会退出的
				DelayMs(1);
				err--;
			}  //这个while退出的时候就代表着9ms和4.5ms结束了，起始的信号没了
			
			for(k = 0; k < 4;++k){	  //一共是4组数据，地址、地址反码、命令、命令反码
				for(j = 0;j < 8;++j){ //接收单组数据,在发送每个1或者0的时候都会有560us低电平出现	
					err = 60;
					while((IRIN == 0)&&(err > 0)){	  //耗过去560us那一段，最多等待8ms
						DelayMs(1);
						err--;	
					}	
					
					err = 500;
					//下面开始区分到底是1还是0.从协议图可以知道1和0区别在高电平的持续时间不一致，1持续1.69ms，0持续0.56ms
					while((IRIN ==1)&& (err > 0)){//计算高电平的长度，通过time是几来判断到底是1还是0
						DelayMs(1);
						Time++;
						err--;
						if(Time > 30){ //超时机制
							EX0 =1;
							return;	
						}
					}

						IrValue[k]>>=1;//k表示存的是第几组,这个数据先是右移一位，从低位开始接收并且存储,然后每次向右移位一个格子
						if(Time>=8){   //8*136us = 	1ms，超过1ms就代表这个是1了
							//IrValue[k] = 0xff;
							IrValue[k] = IrValue[k]|0x80;//接收的如果是1的话，因为是lsb所以通过0x80(0b1000 0000)表示接收到的是1，IrValue需要在执行这段前先向右位移一个单位。	
						}
						Time = 0;
					}//end 每一位	
			} //end 四组

			if(IrValue[2]==~IrValue[3]){//看下键值和键值反码可对的上
				//uart_send_byte(0xff);
				IrValue[4] = IrValue[2];//将命令原数值放在IrValue[4]里面
				//return ;
			}  
		}	//end IRIN  = 1;
	}//end IRIN = 0;
}















































																												  