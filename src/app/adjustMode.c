/*
	本文件是驱动的中间层， 用来lcd1602调整显示模式的函数
*/
#include"adjustMode.h"
double alert_temperature_threshold = 28.0;
double  Temperature = 0;
void keyEventHandler_delay400ms(void);
//本函数实现的功能是在正常模式还有调整模式之间切换，workMode显示0代表正常模式，但是不显示光标。为1代表显示光标的调整模式。
static unsigned char workMode = 0;
static unsigned	char x_start = 0,y_start = 0;//

static void temperature_alert(){
	if(Temperature>alert_temperature_threshold){
		motor_run_alert();	
	}
}

//常规模式下温度和时间的信息显示
void display_normal_temperature_time(){
	//第一行显示温度
	Temperature = Ds18b20ReadDecimalTemperature();
	lcdshowDouble2Str(2,0,Temperature);

	//第二行显示时间
	ds1302_read_time_struct();
	lcdshowStruct2(0,1,mytime);//第二行展示时间

	temperature_alert();
}
static void delay150ms(void)   //误差 0us
{
    unsigned char a,b,c;
    for(c=3;c>0;c--)
        for(b=116;b>0;b--)
            for(a=214;a>0;a--);
}



void WorkModeAdjust(){

	workMode = !workMode;
	if(workMode==0){  //正常模式，光标不闪 ，温度正常显示，时钟正常计数
	 	LCDNormalMode();
		display_normal_temperature_time();		
	}
	else{ //调整模式，光标闪烁,温度和时间不会刷新，只会移动光标和光标处的加减
		LCDAdjustMode();
		//光标闪烁起始位置是要人为去设定光标的位置的
		LCDSetCursor(x_start,y_start);	
	}
	
}

void KeyWorkModeAdjust(){
	if(getKeyValue()==1){
		WorkModeAdjust();
	}	
}

//本函数实现的功能是调整模式的时候，让光标出现并且被4*4键盘控制上下左右移动，3->上，2->左，4->右
void KeyCursorMove(){
	unsigned char keyvalue = 0; 
	keyvalue = getKeyValue();
	if(workMode==0) return ;
	if(keyvalue==0)return ;	
	if(!(keyvalue>1&&keyvalue<5))return ;

   	delay150ms();
	switch(keyvalue){
		case 2:	   //左
			if(x_start==0){
				x_start = 15;
			}else{
				x_start = x_start - 1;
			}
			LCDSetCursor(x_start,y_start);
			break;
		case 3:	  //上
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

//本函数是实现阈值部位的数值增加和减少,传参数是1代表增加，传参数是0代表减少
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
	   LCDSetCursor(x_start,y_start);//把光标还原
	}
	else{  //第一行 设置时间 ,调节一个分钟意思一下
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
        // 如果 x_start 的值不在 0 到 15 之间
        break;}	
	
	lcdshowStruct2(0,1,mytime);	//1302展示时间
	ds1302_write_time_struct(mytime);
	
	
	}		
}

//本函数本质上是上面两个函数的综合,根据传进来的键值去做相应的操作
void keyEventHandler(unsigned char keyvalue){
	switch(keyvalue){
		case 0:
			if(workMode==0)display_normal_temperature_time();break;
		case 1:
			WorkModeAdjust();
			break;
		case 2:	   //左
			if(workMode==0)break;
			if(x_start==0){
				x_start = 15;
			}else{
				x_start = x_start - 1;
			}
			LCDSetCursor(x_start,y_start);
			break;
		case 3:	  //上
			if(workMode==0)break;
			y_start = !y_start;
			LCDSetCursor(x_start,y_start);
			break;
		case 4:  // 右
			if(workMode==0)break;
			if(x_start==15){
				x_start = 0;
			}else{
				x_start = x_start + 1;
			}
			LCDSetCursor(x_start,y_start);
			break;
		case 5:	//加
			if(workMode==0)break;
			setNumber(1);
			break;
		case 6://减
			if(workMode==0)break;
			setNumber(0); 
			break;
			
		default:
			break;		
	}		
}

