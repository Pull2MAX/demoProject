/*
	���ļ����������м�㣬 ����lcd1602������ʾģʽ�ĺ���
*/
#include"adjustMode.h"
double alert_temperature_threshold = 28.0;
double  Temperature = 0;
void keyEventHandler_delay400ms(void);
//������ʵ�ֵĹ�����������ģʽ���е���ģʽ֮���л���workMode��ʾ0��������ģʽ�����ǲ���ʾ��ꡣΪ1������ʾ���ĵ���ģʽ��
static unsigned char workMode = 0;
static unsigned	char x_start = 0,y_start = 0;//

static void temperature_alert(){
	if(Temperature>alert_temperature_threshold){
		motor_run_alert();	
	}
}

//����ģʽ���¶Ⱥ�ʱ�����Ϣ��ʾ
void display_normal_temperature_time(){
	//��һ����ʾ�¶�
	Temperature = Ds18b20ReadDecimalTemperature();
	lcdshowDouble2Str(2,0,Temperature);

	//�ڶ�����ʾʱ��
	ds1302_read_time_struct();
	lcdshowStruct2(0,1,mytime);//�ڶ���չʾʱ��

	temperature_alert();
}
static void delay150ms(void)   //��� 0us
{
    unsigned char a,b,c;
    for(c=3;c>0;c--)
        for(b=116;b>0;b--)
            for(a=214;a>0;a--);
}



void WorkModeAdjust(){

	workMode = !workMode;
	if(workMode==0){  //����ģʽ����겻�� ���¶�������ʾ��ʱ����������
	 	LCDNormalMode();
		display_normal_temperature_time();		
	}
	else{ //����ģʽ�������˸,�¶Ⱥ�ʱ�䲻��ˢ�£�ֻ���ƶ����͹�괦�ļӼ�
		LCDAdjustMode();
		//�����˸��ʼλ����Ҫ��Ϊȥ�趨����λ�õ�
		LCDSetCursor(x_start,y_start);	
	}
	
}

void KeyWorkModeAdjust(){
	if(getKeyValue()==1){
		WorkModeAdjust();
	}	
}

//������ʵ�ֵĹ����ǵ���ģʽ��ʱ���ù����ֲ��ұ�4*4���̿������������ƶ���3->�ϣ�2->��4->��
void KeyCursorMove(){
	unsigned char keyvalue = 0; 
	keyvalue = getKeyValue();
	if(workMode==0) return ;
	if(keyvalue==0)return ;	
	if(!(keyvalue>1&&keyvalue<5))return ;

   	delay150ms();
	switch(keyvalue){
		case 2:	   //��
			if(x_start==0){
				x_start = 15;
			}else{
				x_start = x_start - 1;
			}
			LCDSetCursor(x_start,y_start);
			break;
		case 3:	  //��
			y_start = !y_start;
			LCDSetCursor(x_start,y_start);
			break;
		case 4:  //
			if(x_start==15){
				x_start = 0;
			}else{
				x_start = x_start + 1;
			}
			LCDSetCursor(x_start,y_start);
			break;
			
		default:
			break;	
	}
}

//��������ʵ����ֵ��λ����ֵ���Ӻͼ���,��������1�������ӣ���������0�������
static void setNumber(unsigned char numberFlag){
	if(y_start==0){
		unsigned char x = x_start;
		switch(x){
			case 12:
				if(numberFlag){
					alert_temperature_threshold+=10;	
				}else{
				   	alert_temperature_threshold-=10;
				}
				break;
			case 13:
				if(numberFlag){
					alert_temperature_threshold+=1;	
				}else{
				   	alert_temperature_threshold-=1;
				}
				break;
			case 15:
				if(numberFlag){
					alert_temperature_threshold+=0.1;	
				}else{
				   	alert_temperature_threshold-=0.1;
				}
				break;
			default:break;		
		}
	   lcdshowDouble2Str(12,0,alert_temperature_threshold);
	   LCDSetCursor(x_start,y_start);//�ѹ�껹ԭ
	}
	else{  //��һ�� ����ʱ�� ,����һ��������˼һ��
		switch(x_start){
	case 11:
		if(numberFlag){
			mytime.minute+=10;	
		}else{
			mytime.minute-=10;
		}
        break;
    case 12:
       	if(numberFlag){
			mytime.minute+=1;	
		}else{
			mytime.minute-=1;
		}
        break;
 
    default:
        // ��� x_start ��ֵ���� 0 �� 15 ֮��
        break;}	
	
	lcdshowStruct2(0,1,mytime);	//1302չʾʱ��
	ds1302_write_time_struct(mytime);
	
	
	}		
}

//�����������������������������ۺ�,���ݴ������ļ�ֵȥ����Ӧ�Ĳ���
void keyEventHandler(unsigned char keyvalue){
	switch(keyvalue){
		case 0:
			if(workMode==0)display_normal_temperature_time();break;
		case 1:
			WorkModeAdjust();
			break;
		case 2:	   //��
			if(workMode==0)break;
			if(x_start==0){
				x_start = 15;
			}else{
				x_start = x_start - 1;
			}
			LCDSetCursor(x_start,y_start);
			break;
		case 3:	  //��
			if(workMode==0)break;
			y_start = !y_start;
			LCDSetCursor(x_start,y_start);
			break;
		case 4:  // ��
			if(workMode==0)break;
			if(x_start==15){
				x_start = 0;
			}else{
				x_start = x_start + 1;
			}
			LCDSetCursor(x_start,y_start);
			break;
		case 5:	//��
			if(workMode==0)break;
			setNumber(1);
			break;
		case 6://��
			if(workMode==0)break;
			setNumber(0); 
			break;
			
		default:
			break;		
	}		
}

