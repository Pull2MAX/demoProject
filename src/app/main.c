#include"uart.h"
#include"ds18b20.h"	 //����¶ȵ�оƬ
#include"lcd1602.h"  //lcd��ʾ�����ͺ�
#include"ds1302.h"	 //ʵʱʱ��оƬ
#include"buzzer.h"	 //������
#include"motor.h"	//�������
#include"key4_4.h"	//4*4�ľ������
#include"ir.h"  	//���������

#include"adjustMode.h"	//�м�㺯�������������˸���

void keyEventHandler_delay400ms(void)   //��� 0us
{
   unsigned char a,b,c;
    for(c=31;c>0;c--)
        for(b=176;b>0;b--)
            for(a=28;a>0;a--);
}

void mydelay900ms(void)   //��� 0us
{
    unsigned char a,b,c;
    for(c=119;c>0;c--)
        for(b=216;b>0;b--)
            for(a=16;a>0;a--);
} 

void delay500us(void)   //��� 0us
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
����Ŀ����˵����
1.lcd1602��ʾʱ�仹���¶�
2.�û��Զ����¶���ֵ������ʾ��������ֵ����������
3.ʱ�����(����)

-----------------------------------
˵����
1.��һ������ʾ�¶Ȼ��б�����ֵ���磺T:23.6 Y:39.0
2.�ڶ�����ʾʱ�䣬��ʽ��23-04-23 16:23|5
3.����ģʽ����û�й��ģ������ڵ���ģʽ�����й��ģ�����ģʽ�Ľ������ģʽ�ǰ�����4*4��ֵΪ1�İ������ٰ�һ�¾��л�����ģʽ��һ���������ģʽ��������ʾ�������ǲ���ģ�ֻ���л�����ģʽ�Ż�Ӧ�ú�仯��
4.4*4�������ĸ������ƹ�����������ƶ�������ģʽ�¹����˸���ĸ��ط�����Ҫ���ڸô������ݣ����¼ӻ��߼���	���ӻ��߼����������
*/ 
/*
	//���Դ���------>����
	uart_init();
	uart_send_byte('X');
	uart_send_string("����Shabbywindows");
*/

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
	
/*
	//�����¶���ʾ���� ------������
�����������Ƿ��ص��¶���double���͵ģ����ǲ��������ǵĴ��ڵ�����������ʾ����Ϊ����ֻ����ʾʮ�����ƻ��߱�����ʽ��˼·�����á��ı�����ʾ������uart_send_byte("25.4")
����ֻҪ��doubleת��string���� 1.�Լ�дת���������õ��������  2.���ÿ⺯�� 
	
	uart_init();
	uart_send_double2string(Ds18b20ReadDecimalTemperature()); 
	uart_send_string("��.");
*/

//--------------------------------------------------------------------------------------------------------------------------------------------------------------



 /*	//����lcd1602  ---������

 	InitLCD1602(); 
	LCDshowStr(0,0,"Temp:"); //���к���һ�ж��ǹ̶��ģ�����д��ѭ�����
	LCDshowStr(11,0,DU);
	while(1){
	for(mycount=200;mycount>0;mycount--);
   	lcdshowDouble2Str(5,0,Ds18b20ReadDecimalTemperature());
	}
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------


/*
	//����ʵʱʱ��ds1302 ---������
	һ�����ӵ�ʱ�����������������+��+��+ʱ����+�ܼ�������ýṹ��ȥ������ô�ิ�ӱ�������ʾ
	  
	struct allTime test = {2024, 4, 22, 7, 40, 45, 1};
	InitLCD1602(); 
	LCDshowStr(0,0,"Temp:"); //���к���һ�ж��ǹ̶��ģ�����д��ѭ�����
	LCDshowStr(11,0,DU);
	
	ds1302_write_time_struct(test);//����ʱ��
	
	while(1){
	//��һ�ж�ȡ�¶Ȳ���ʾ 
	delay500us();
   	lcdshowDouble2Str(5,0,Ds18b20ReadDecimalTemperature());	
	ds1302_read_time_struct(); //�������������ǽṹ�����ʹ洢��
	//lcdshowStruct(0,1,mytime);

	lcdshowStruct2(0,1,mytime);//�ڶ���չʾʱ��
	}  
*/

/*
	//����buzzer-----������
    //�������˼����������ǿ��Կ���buzzer�����п�ʼ�ͽ��������п�����һ���������������������Ƶ��

		buzzer_init();	//��ʼ���Ȳ�������������У���ʼ����Ƶ����1khz
		buzzer_start();
 		mydelay900ms();
		buzzer_stop();

		buzzer_freq_set(10);//����ƵΪ10
		buzzer_start();
 		mydelay900ms();
		buzzer_stop();
*/
/*
	//����motor---->����

	//ע�Ȿ������Ĳ����������Ŀ��Ҫ��оƬ�ǿ������ϵ�����˫���Բ������оƬ��
	//�Ȱ�gpio�ӵ����оƬ���ĸ�in�ڣ��������оƬ���ĸ�out�ں����ǵĲ����������
	 motor_run_alert();
*/

/*
	//����4*4�������   ------->����	

	
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
				lcdshowInt2Str(0,1,keyvalue); // д��ô������Ϊ��һ��ʼ��ʾ14��������ʱ�򰴵���7����ʾ��74��Ҳ����˵һ��ʼ�����Ƕ�λ�����ڶ�λ���ݴ棬�����Ҫ�ж����³�ʼ��
			 }
	   		
	}			
*/	

/*
	//����IR�ǲ��������� ------->����

	//���ڰ�������ȥ���ɵ�Ƭ�������ʱ�����жϵķ�ʽ���հ����ļ�ֵ����ʱ���ر𳤣����ܻ���ŵ�����жϴ���������½������
	����취&˼·��
	1.�жϻ�Ҫ��Ҫ��Ҫ����ѯ̫���ˣ��˷ѵ�Ƭ������Դ���ж��յ���Ϣ���ռ���
	2.�����жϵı׶� ���ӳٺ���̫�࣬û��Ҫ�ڵ�Ƭ���ж��������������������ж��������տ�һ��ͷ�����潻���ⲿȥ���������չ�����
	3.����Ҫ��delay��ʱ��ǳ���׼����˲������Լ�д��delay�����ķ�ʽ��ֻ���ö�ʱ��ȥ��׼����ʱ��

  
	LCDshowStr(0,0,"IR VALUE:");
	IrInit();
	while(1){
		if(oldIRvalue!=IrValue[4]){	//д���forѭ���Ķ�Ŀ���������س���ֵ��Ӱ�죬�����һ��չʾ��ֵ��224������ڶ��μ�ֵ��7�ͻ���724�����Ǽ������if�ͻ���������7��
			InitLCD1602();
			LCDshowStr(0,0,"IR VALUE:");
			lcdshowInt2Str(0,1,IrValue[4]);	//
			oldIRvalue = IrValue[4];
		}	
				
	} 
*/
/*
	//�����м�㺯���Ĺ��ܣ����ģʽ�л�����ɹ����ƶ���------->����
	//----��1��------


	while(1){
		KeyWorkModeAdjust();//ֻ�а�1��ʱ��ŻᷴӦ,����ǵ���ģʽ����ô���Ĭ�ϳ����ڵ�һ�е�һ��,������������һ����ʧ�ĵط�	
		delay500us();
		KeyCursorMove();
	
	}

	//----��2��------ ���������װ�ĸ��ã��Ը߲�ֻ��һ������	
	InitLCD1602();
	LCDshowStr(0,0,"--new world--");
	while(1){
		keyvalue = getKeyValue();
		keyEventHandler();
	   	keyEventHandler_delay100ms();
	}
	
*/	
		
//�����С�����ã���װ����֮��ͻ��������ʵ�ָ��ӵĹ���
	InitLCD1602();
	buzzer_init();
	LCDshowStr(0,0,"T:");
	LCDshowStr(7,0,DU);
	LCDshowStr(10,0,"Y:");
	lcdshowDouble2Str(12,0,alert_temperature_threshold);


	ds1302_write_time_struct(startTimeSet);//����ʱ��
	display_normal_temperature_time();
	while(1){
		keyvalue = getKeyValue();
		keyEventHandler(keyvalue);
		keyEventHandler_delay400ms();
	}
}