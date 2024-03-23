#include"motor.h"

void delay_motor(unsigned char count1,unsigned char count2){
	unsigned char i,j;
	for(i = 0;i<count1;++i){
		for(j = 0;j<count2;++j);
	}
}



void motor_run_alert(){//这个函数是一个节拍 ，双相四拍的
	unsigned long timeflag = 1;
	while(timeflag<200){
		Apo = 0;
		Ane = 1;
		Bpo = 1;
		Bne = 0;
		delay_motor(num1,num2);
	
		Apo = 1;
		Ane = 0;
		Bpo = 1;
		Bne = 0;
		delay_motor(num1,num2);
	
		Apo = 1;
		Ane = 0;
		Bpo = 0;
		Bne = 1;
		delay_motor(num1,num2);
	
		Apo = 0;
		Ane = 1;
		Bpo = 0;
		Bne = 1;
		delay_motor(num1,num2);
		
		timeflag++;	
		}
		
}

