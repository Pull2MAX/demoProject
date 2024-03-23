#include "lcd1602.h"


/***********底层时序***************/
static void isBusy(){	//忙检测函数，写数据、读数据、写指令的时候都要看看这个芯片在不在忙
	unsigned char sta = 0;//当DB的最高位是1时，就表示一直在忙
	LCD1602_DB = 0xff;
	LCD1602_RS = 0;
	LCD1602_RW = 1;

	do{
		LCD1602_EN = 1;
		sta = LCD1602_DB&0x80;
		LCD1602_EN = 0;
	}while(sta);
}

static void LCD1602_Write_Command(unsigned char cmd){  //写指令的底层函数
	isBusy();
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_EN = 1;
	LCD1602_EN = 0;	
}

static void LCD1602_Write_Data(unsigned char dat){ //写数据的函数，可以指定ascii范围内的内容
	isBusy();
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_EN = 1;
	LCD1602_EN = 0;	
}

/***********高层时序***************/
/*这个函数的功能是 传给write_command函数的指令让正在编辑的位置可以显示在屏幕上
x的取值范围是0x00到0x0f,y只可以取0x00(第一行)或者0x40（第二行）		 */
void LCDSetCursor(unsigned char x,unsigned char y){	//具体显示字符的时候需要指定显示在哪一行，哪一列。第一行是0x00到0x0f。第二行是0x40到0x4f。
	unsigned char addr;
	if(y==0)
		addr = 0x00 + x;
	else
		addr = 0x40 + x;
	LCD1602_Write_Command(addr|0x80);		
}

/*这个函数的作用是获取光标的位置然后在光标开始的地方显示字符串*/
void LCDshowStr(unsigned char x,unsigned char y,unsigned char *str){
	LCDSetCursor(x,y);
	while(*str!='\0'){
		LCD1602_Write_Data(*str++);	
	}		
}

//这个函数的作用是初始化LCD1602
void InitLCD1602(){
	LCD1602_Write_Command(0x38); //数据总线是八位，两行显示，5*7点阵
	//LCD1602_Write_Command(0x0f); //光标闪，显示功能关
	LCD1602_Write_Command(0x0c); //光标不闪，显示开
	LCD1602_Write_Command(0x06); //写入新数据之后光标右移
	LCD1602_Write_Command(0x01);  //清屏操作
}




//本函数的目的是将double类型的数值直接转换为str类型并且展示在1602上面 
static unsigned char double2strArr[25];
void lcdshowDouble2Str(unsigned char x,unsigned char y,double value){
	sprintf(double2strArr, "%4.2f", value);
	LCDshowStr(x,y,double2strArr);		
}

//static unsigned char int2strArr[10];
void lcdshowInt2Str(unsigned char x,unsigned char y,unsigned int value){ 
	unsigned char int2strArr[10];
	sprintf(int2strArr, "%d", value);
	LCDshowStr(x,y,int2strArr);
	
}

/*
//这两个函数的目的是接收自定义结构体类型的变量 ,然后将这变量里面的内容放在准备好的数组里面随后打印在屏幕上，频繁调用的是sprintf

static unsigned char struct2strArr[25];										    
void lcdshowStruct(unsigned char x,unsigned char y,struct allTime value){
	unsigned char out_index = 0;
	unsigned char collect_locate = 0;
	unsigned char struct_collect_array[17] = {0};
	sprintf(struct2strArr, "%d", value.year);
	for(; out_index < 4;++collect_locate,++out_index){
		struct_collect_array[collect_locate] = struct2strArr[out_index];		
	} //年份已经放到了收集数组中


	out_index = 0;
	sprintf(struct2strArr, "%d", value.month);
	for(;out_index<2;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//月份放到了数组中

	
	out_index = 0;
	sprintf(struct2strArr, "%d", value.day);
	for(;out_index<2;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//日子放到了数组中
	struct_collect_array[collect_locate++] = '-';  //放一个‘-’

	out_index = 0;
	sprintf(struct2strArr, "%2d", value.hour);
	for(;out_index<2;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//小时放到了数组中
	
	out_index = 0;
	sprintf(struct2strArr, "%2d", value.minute);
	for(;out_index<2;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//分钟放到了数组中

	out_index = 0;
	sprintf(struct2strArr, "%2d", value.second);
	for(;out_index<2;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//秒放到了数组中

	out_index = 0;
	sprintf(struct2strArr, "%d", value.week);
	for(;out_index<1;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//周几放到了数组中
	struct_collect_array[17] = '\0';
	LCDshowStr(x,y,struct_collect_array);	//直接显示这个特长的数组		
}
  */
//参考老师的思路，下面的函数接收的是具体的显示地址还有结构体变量

//实现的第一个函数是将四位的时间转为str然后放到准备好的str里面	
void int2str4(unsigned int dat,unsigned char str[],unsigned char bias){
	unsigned char c = 0;
	c = dat/1000;
	str[bias+0] = c + '0';//最高位拿到了	，dat是2239
	
	dat = dat % 1000;
	c = dat / 100;
	str[bias+1] = c + '0';  //第二高的位拿到了	  ，dat是239

	dat = dat % 100;
	c = dat / 10;
	str[bias+2] = c + '0'; //第三高的位置拿到了 ，dat是39

	c = dat % 10;		//最后一位拿到
	str[bias+3] = c + '0'; 		
}														 
//实现一个功能，让两位数转为str然后放到数组里 
void int2str2(unsigned int dat,unsigned char str[],unsigned char bias){
	unsigned char c = 0;
	c = dat/10;
	str[bias+0] = c + '0';

	c = dat % 10;
	str[bias+1] = c + '0';
}



void lcdshowStruct2(unsigned char x,unsigned char y,struct allTime mytime){
	unsigned char str[17] = {0};
	
	int2str4(mytime.year,str,0);  //实现的第一个函数是将四位的时间转为str然后放到准备好的str里面	
	
	int2str2(mytime.month,str,4);

	int2str2(mytime.day,str,6);

	str[8] = '-';

	int2str2(mytime.hour,str,9);

	int2str2(mytime.minute,str,11);

	int2str2(mytime.second,str,13);

	str[15] = mytime.week + '0'	; //周几不要调用函数了，因为再调用int2str2会导致周几占据2个位置，没有2个位置给他
	
	str[16] = '\0';
	LCDshowStr(x,y,str);		
}


//lcd1602的正常模式，去除光标
void LCDNormalMode(){
 
	LCD1602_Write_Command(0x0c); //光标不闪，显示开
}							    
//LCD1602的调整模式，显示光标
void LCDAdjustMode(){
	LCD1602_Write_Command(0x0f); //光标闪，显示功能关
}
 




























