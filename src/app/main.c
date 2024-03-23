#include"uart.h"
#include"ds18b20.h"	 //检测温度的芯片
#include"lcd1602.h"  //lcd显示屏的型号
#include"ds1302.h"	 //实时时钟芯片
#include"buzzer.h"	 //蜂鸣器
#include"motor.h"	//步进电机
#include"key4_4.h"	//4*4的矩阵键盘
#include"ir.h"  	//红外接收器

#include"adjustMode.h"	//中间层函数，调整光标闪烁与否

void keyEventHandler_delay400ms(void)   //误差 0us
{
   unsigned char a,b,c;
    for(c=31;c>0;c--)
        for(b=176;b>0;b--)
            for(a=28;a>0;a--);
}

void mydelay900ms(void)   //误差 0us
{
    unsigned char a,b,c;
    for(c=119;c>0;c--)
        for(b=216;b>0;b--)
            for(a=16;a>0;a--);
} 

void delay500us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=80;b>0;b--)
        for(a=180;a>0;a--);
}




void main(){
unsigned char uart_receive = 0;
unsigned char DU[3] = {0xdf,'C'};
unsigned  int keyvalue = 0;
unsigned char oldIRvalue = 0;

struct allTime startTimeSet = {2024, 4, 22, 7, 40, 45, 1};

/*
【项目功能说明】
1.lcd1602显示时间还有温度
2.用户自定义温度阈值并且显示，超过阈值蜂鸣器报警
3.时间调整(增减)

-----------------------------------
说明：
1.第一行是显示温度还有报警阈值比如：T:23.6 Y:39.0
2.第二行显示时间，格式是23-04-23 16:23|5
3.常规模式下是没有光标的，但是在调整模式下是有光标的，常规模式的进入调整模式是按键是4*4键值为1的按键，再按一下就切换常规模式。一旦进入调整模式，所有显示的内容是不变的，只有切回正常模式才会应用后变化。
4.4*4键盘有四个键控制光标上下左右移动，调整模式下光标闪烁在哪个地方就是要调节该处的内容，按下加或者减会	增加或者减少这个数字
*/ 
/*
	//测试串口------>正常
	uart_init();
	uart_send_byte('X');
	uart_send_string("饕餮Shabbywindows");
*/

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
	
/*
	//测试温度显示函数 ------》正常
遇到的问题是返回的温度是double类型的，但是不好在我们的串口调试助手上显示，因为上面只能显示十六进制或者本文形式，思路就是用【文本】显示，比如uart_send_byte("25.4")
所以只要将double转成string就行 1.自己写转换函数（用的是这个）  2.调用库函数 
	
	uart_init();
	uart_send_double2string(Ds18b20ReadDecimalTemperature()); 
	uart_send_string("度.");
*/

//--------------------------------------------------------------------------------------------------------------------------------------------------------------



 /*	//测试lcd1602  ---》正常

 	InitLCD1602(); 
	LCDshowStr(0,0,"Temp:"); //这行和下一行都是固定的，所以写在循环外边
	LCDshowStr(11,0,DU);
	while(1){
	for(mycount=200;mycount>0;mycount--);
   	lcdshowDouble2Str(5,0,Ds18b20ReadDecimalTemperature());
	}
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------


/*
	//测试实时时钟ds1302 ---》正常
	一个复杂的时间变量读出来的是年+月+日+时分秒+周几，因此用结构体去包括这么多复杂变量以显示
	  
	struct allTime test = {2024, 4, 22, 7, 40, 45, 1};
	InitLCD1602(); 
	LCDshowStr(0,0,"Temp:"); //这行和下一行都是固定的，所以写在循环外边
	LCDshowStr(11,0,DU);
	
	ds1302_write_time_struct(test);//设置时间
	
	while(1){
	//第一行读取温度并显示 
	delay500us();
   	lcdshowDouble2Str(5,0,Ds18b20ReadDecimalTemperature());	
	ds1302_read_time_struct(); //读出来的数据是结构体类型存储的
	//lcdshowStruct(0,1,mytime);

	lcdshowStruct2(0,1,mytime);//第二行展示时间
	}  
*/

/*
	//测试buzzer-----》正常
    //单独加了几个函数就是可以控制buzzer的鸣叫开始和结束，还有可以有一个函数控制这个蜂鸣器的频率

		buzzer_init();	//初始化先不给这个蜂鸣器叫，初始化的频率是1khz
		buzzer_start();
 		mydelay900ms();
		buzzer_stop();

		buzzer_freq_set(10);//更改频为10
		buzzer_start();
 		mydelay900ms();
		buzzer_stop();
*/
/*
	//测试motor---->正常

	//注意本开发板的步进电机的项目需要的芯片是开发板上的四相双极性步进电机芯片，
	//先把gpio接到这个芯片的四个in口，再由这个芯片的四个out口和我们的步进电机相连
	 motor_run_alert();
*/

/*
	//测试4*4矩阵键盘   ------->正常	

	
	InitLCD1602();
	LCDshowStr(0,0,"key is:");
	
	while(1){
		keyvalue = getKeyValue(); 
		if(keyvalue!=0)
			 if(keyvalue>10){
			   lcdshowInt2Str(0,1,keyvalue); 
			 } else{
			 	InitLCD1602();
				LCDshowStr(0,0,"key is:");
				lcdshowInt2Str(0,1,keyvalue); // 写这么长是因为当一开始显示14，如果这个时候按的是7会显示，74，也就是说一开始按的是二位数，第二位会暂存，因此需要判断重新初始化
			 }
	   		
	}			
*/	

/*
	//测试IR是不是正常的 ------->正常

	//由于按键按下去交由单片机处理的时候用中断的方式接收按键的键值处理时间特别长，可能会干扰到别的中断处理处理程序导致结果不对
	解决办法&思路：
	1.中断还要不要？要，轮询太慢了，浪费单片机的资源，中断收到消息即收即用
	2.红外中断的弊端 ？延迟函数太多，没必要在单片机中断中做，【所以我们在中断启动接收开一个头，后面交给外部去做整个接收工作】
	3.红外要求delay的时间非常精准，因此不能用自己写的delay函数的方式，只能用定时器去精准控制时间

  
	LCDshowStr(0,0,"IR VALUE:");
	IrInit();
	while(1){
		if(oldIRvalue!=IrValue[4]){	//写这个for循环的额目的是消除特长键值的影响，比如第一次展示键值是224，如果第二次键值是7就会变成724。若是加了这个if就会变成正常的7。
			InitLCD1602();
			LCDshowStr(0,0,"IR VALUE:");
			lcdshowInt2Str(0,1,IrValue[4]);	//
			oldIRvalue = IrValue[4];
		}	
				
	} 
*/
/*
	//测试中间层函数的功能（完成模式切换，完成光标的移动）------->正常
	//----法1：------


	while(1){
		KeyWorkModeAdjust();//只有按1的时候才会反应,如果是调整模式，那么光标默认出生在第一行第一列,随后会出现在最后一次消失的地方	
		delay500us();
		KeyCursorMove();
	
	}

	//----法2：------ 这个方法封装的更好，对高层只有一个函数	
	InitLCD1602();
	LCDshowStr(0,0,"--new world--");
	while(1){
		keyvalue = getKeyValue();
		keyEventHandler();
	   	keyEventHandler_delay100ms();
	}
	
*/	
		
//上面的小组件测好，封装好了之后就汇聚在下面实现复杂的功能
	InitLCD1602();
	buzzer_init();
	LCDshowStr(0,0,"T:");
	LCDshowStr(7,0,DU);
	LCDshowStr(10,0,"Y:");
	lcdshowDouble2Str(12,0,alert_temperature_threshold);


	ds1302_write_time_struct(startTimeSet);//设置时间
	display_normal_temperature_time();
	while(1){
		keyvalue = getKeyValue();
		keyEventHandler(keyvalue);
		keyEventHandler_delay400ms();
	}
}