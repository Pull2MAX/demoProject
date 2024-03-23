#include "lcd1602.h"


/***********�ײ�ʱ��***************/
static void isBusy(){	//æ��⺯����д���ݡ������ݡ�дָ���ʱ��Ҫ�������оƬ�ڲ���æ
	unsigned char sta = 0;//��DB�����λ��1ʱ���ͱ�ʾһֱ��æ
	LCD1602_DB = 0xff;
	LCD1602_RS = 0;
	LCD1602_RW = 1;

	do{
		LCD1602_EN = 1;
		sta = LCD1602_DB&0x80;
		LCD1602_EN = 0;
	}while(sta);
}

static void LCD1602_Write_Command(unsigned char cmd){  //дָ��ĵײ㺯��
	isBusy();
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_EN = 1;
	LCD1602_EN = 0;	
}

static void LCD1602_Write_Data(unsigned char dat){ //д���ݵĺ���������ָ��ascii��Χ�ڵ�����
	isBusy();
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_EN = 1;
	LCD1602_EN = 0;	
}

/***********�߲�ʱ��***************/
/*��������Ĺ����� ����write_command������ָ�������ڱ༭��λ�ÿ�����ʾ����Ļ��
x��ȡֵ��Χ��0x00��0x0f,yֻ����ȡ0x00(��һ��)����0x40���ڶ��У�		 */
void LCDSetCursor(unsigned char x,unsigned char y){	//������ʾ�ַ���ʱ����Ҫָ����ʾ����һ�У���һ�С���һ����0x00��0x0f���ڶ�����0x40��0x4f��
	unsigned char addr;
	if(y==0)
		addr = 0x00 + x;
	else
		addr = 0x40 + x;
	LCD1602_Write_Command(addr|0x80);		
}

/*��������������ǻ�ȡ����λ��Ȼ���ڹ�꿪ʼ�ĵط���ʾ�ַ���*/
void LCDshowStr(unsigned char x,unsigned char y,unsigned char *str){
	LCDSetCursor(x,y);
	while(*str!='\0'){
		LCD1602_Write_Data(*str++);	
	}		
}

//��������������ǳ�ʼ��LCD1602
void InitLCD1602(){
	LCD1602_Write_Command(0x38); //���������ǰ�λ��������ʾ��5*7����
	//LCD1602_Write_Command(0x0f); //���������ʾ���ܹ�
	LCD1602_Write_Command(0x0c); //��겻������ʾ��
	LCD1602_Write_Command(0x06); //д��������֮��������
	LCD1602_Write_Command(0x01);  //��������
}




//��������Ŀ���ǽ�double���͵���ֱֵ��ת��Ϊstr���Ͳ���չʾ��1602���� 
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
//������������Ŀ���ǽ����Զ���ṹ�����͵ı��� ,Ȼ���������������ݷ���׼���õ�������������ӡ����Ļ�ϣ�Ƶ�����õ���sprintf

static unsigned char struct2strArr[25];										    
void lcdshowStruct(unsigned char x,unsigned char y,struct allTime value){
	unsigned char out_index = 0;
	unsigned char collect_locate = 0;
	unsigned char struct_collect_array[17] = {0};
	sprintf(struct2strArr, "%d", value.year);
	for(; out_index < 4;++collect_locate,++out_index){
		struct_collect_array[collect_locate] = struct2strArr[out_index];		
	} //����Ѿ��ŵ����ռ�������


	out_index = 0;
	sprintf(struct2strArr, "%d", value.month);
	for(;out_index<2;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//�·ݷŵ���������

	
	out_index = 0;
	sprintf(struct2strArr, "%d", value.day);
	for(;out_index<2;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//���ӷŵ���������
	struct_collect_array[collect_locate++] = '-';  //��һ����-��

	out_index = 0;
	sprintf(struct2strArr, "%2d", value.hour);
	for(;out_index<2;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//Сʱ�ŵ���������
	
	out_index = 0;
	sprintf(struct2strArr, "%2d", value.minute);
	for(;out_index<2;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//���ӷŵ���������

	out_index = 0;
	sprintf(struct2strArr, "%2d", value.second);
	for(;out_index<2;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//��ŵ���������

	out_index = 0;
	sprintf(struct2strArr, "%d", value.week);
	for(;out_index<1;++out_index,++collect_locate){
		struct_collect_array[collect_locate] = struct2strArr[out_index];	
	}//�ܼ��ŵ���������
	struct_collect_array[17] = '\0';
	LCDshowStr(x,y,struct_collect_array);	//ֱ����ʾ����س�������		
}
  */
//�ο���ʦ��˼·������ĺ������յ��Ǿ������ʾ��ַ���нṹ�����

//ʵ�ֵĵ�һ�������ǽ���λ��ʱ��תΪstrȻ��ŵ�׼���õ�str����	
void int2str4(unsigned int dat,unsigned char str[],unsigned char bias){
	unsigned char c = 0;
	c = dat/1000;
	str[bias+0] = c + '0';//���λ�õ���	��dat��2239
	
	dat = dat % 1000;
	c = dat / 100;
	str[bias+1] = c + '0';  //�ڶ��ߵ�λ�õ���	  ��dat��239

	dat = dat % 100;
	c = dat / 10;
	str[bias+2] = c + '0'; //�����ߵ�λ���õ��� ��dat��39

	c = dat % 10;		//���һλ�õ�
	str[bias+3] = c + '0'; 		
}														 
//ʵ��һ�����ܣ�����λ��תΪstrȻ��ŵ������� 
void int2str2(unsigned int dat,unsigned char str[],unsigned char bias){
	unsigned char c = 0;
	c = dat/10;
	str[bias+0] = c + '0';

	c = dat % 10;
	str[bias+1] = c + '0';
}



void lcdshowStruct2(unsigned char x,unsigned char y,struct allTime mytime){
	unsigned char str[17] = {0};
	
	int2str4(mytime.year,str,0);  //ʵ�ֵĵ�һ�������ǽ���λ��ʱ��תΪstrȻ��ŵ�׼���õ�str����	
	
	int2str2(mytime.month,str,4);

	int2str2(mytime.day,str,6);

	str[8] = '-';

	int2str2(mytime.hour,str,9);

	int2str2(mytime.minute,str,11);

	int2str2(mytime.second,str,13);

	str[15] = mytime.week + '0'	; //�ܼ���Ҫ���ú����ˣ���Ϊ�ٵ���int2str2�ᵼ���ܼ�ռ��2��λ�ã�û��2��λ�ø���
	
	str[16] = '\0';
	LCDshowStr(x,y,str);		
}


//lcd1602������ģʽ��ȥ�����
void LCDNormalMode(){
 
	LCD1602_Write_Command(0x0c); //��겻������ʾ��
}							    
//LCD1602�ĵ���ģʽ����ʾ���
void LCDAdjustMode(){
	LCD1602_Write_Command(0x0f); //���������ʾ���ܹ�
}
 




























