/*--------ע�Ȿ��Ŀ�Ĳ����ʻ�����ʱʱ�䶼�ǽ����ھ�����12MHZ������¼��������*/

#include"ds1302.h"

/*unsigned char code READ_RTC_ADDR[7] = {0x81,0x83,0x85,0x87,0x89,0x8b,0x8d}; //����ʱ��Ӧ��д�ļĴ�����ַ
unsigned char code WRITE_RTC_ADDR[7] = {0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};//д��ʱ��ļĴ�����ַ */

//unsigned char TIME[7];//�洢��������������ʱ��	  ���ʱ��������

struct allTime mytime;

 /*-------------------------------------------------------------------�ײ�ʱ��----------------------------------------------------------------------*/
//������д�ĵ�ַ�Ŀ�ͷ�ǲ�һ���ģ�ds1302����ݿ�ͷ�Ķ�д����Ӧ��׼����
//��ds1302�ļĴ���addrд��һ��ֵvalue,���дָ���ǵ�Ƭ����д����ds1302������ds1302�Լ���Ҫ�������ء���ȡ��
static void ds1302_write_reg(unsigned char addr,unsigned char value){ 
	
	unsigned char i = 0;
	//unsigned char dat = 0;
	//1.ʱ��ʼ(��ʼ״̬)
	SCLK = 0;
	_nop_();
	RST = 0;
	_nop_();
	//2.д���һ�ֽ�  addr������
	RST = 1;
	_nop_();
	for(i = 0;i < 8;++i){
		DSIO = addr&0x01; //��ȡ�������λ����d0�����λ������d7����ԭ��ͼ��
		addr = addr >> 1;
		//DSIO = dat;//��������׼�������ݣ�������������غ���ȡ���ŵ�����
		SCLK = 1;	//�����ص�ʱ��ʼ��ȡ���ŵ���ֵ
		_nop_();
		SCLK= 0;//���� Ϊ��һ�ζ�ȡ׼��
		_nop_();
	}	 /*��ԭ��ͼ��֪��R/W��ȡ��R/Cһ���ǰ�λ�����Ĵ������߸���
	���ֻ���м��A0-A4Ψһ��ʾ�Ĵ����������ľ��ǿ���λ��
	ds1302�����β���ʶ����ȷ�ļĴ�����ַ��Ȼ����һ��forѭ�����d0---d7����	 
	 */

	//3.д��ڶ��ֽ�  value

	for(i = 0;i < 8;++i){
		DSIO = value&0x01;
		value = value>>1;
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();
	}
	//4.ʱ�����
	RST = 0;
	_nop_();
	SCLK = 0;
	_nop_();
}


//��ds1302�ļĴ�����ȡһ���ֽ�ָ���ǵ�Ƭ���ӡ���ȡ��i/o�ڵ����ݣ����ds1302��Ҫ����ַ�����½��ء�д����i/o��

static unsigned char ds1302_read_reg(unsigned char addr){	 //���ݵ�ַ�ĵ�һλȷ���Ƕ�����д����1�ͱ�ʾ�������Դ���������8��ͷ�ģ�
	unsigned char i = 0;
	unsigned char tmp = 0;
	unsigned char dat = 0;
	
	//1.ʱ��ʼ
	RST = 0;
	_nop_();
	SCLK = 0;
	_nop_();

	//2.ȷ��mcuҪ��ȡ��һ���Ĵ����ĵ�ַ	���Ĵ����ĵ�ַ��Ҫ��mcuд��ds1302
	RST = 1;
	_nop_();
	for(i = 0;i < 8;++i){
		DSIO = addr&0x01;//�ӵ�λ��ʼ
		//DSIO = dat;//��ԭ����addr�����š��ŵ���dsio����	�������ͺ��������Ϊʲô��д����8��ͷ����Ϊdsio���һλ������1��
		addr>>=1;
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();
		}
	//3.ds1302���������ṩ�ĵ�ַ�ҵ������ַ�������Ȼ��ͨ��i/o���ظ�mcu

	//һ����ַ�����ͣ�DS1302 �ͻὫ�õ�ַ��Ӧ�ļĴ��������ݷ��� DS1302 �� I/O �����ϣ��� MCU ����λ�ش�������Ŷ�ȡ���ݡ�
	//dat = 0;
	for(i = 0;i < 8;++i){
		tmp = DSIO;		 //��һ������ǰ���ԭ������һ��forѭ��������ʱ����1��0���½��أ������ʱ����Ѿ����Կ�ʼ��ȡd0�������� ,���� дtemp��ԭ���Ƿ����������⣬keil��֧��sbit����Ķ����������ƶ� ,������ DS1302 ������Ĵ�����ַ��DS1302 �ڲ��߼���׼����Ӧ�Ĵ��������ݣ������������ I/O �����ϡ�  ������׶Σ�DS1302 �Ѿ�֪����Ҫִ�е��Ƕ�ȡ����������֪����Ҫ��ȡ�ļĴ�����ַ��
		dat |= (tmp << i);	//dsio��洢�ĵ�һ���ǵ�λ�����һ�����Ǹ�λ
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();				
	}
	//4.ʱ�����
	SCLK = 0;
	_nop_();
	RST = 0;
	_nop_();
	
	return dat;	  					  
}

/*-----------------------------------------------------------�߲�ʱ��---------------------------------------------------------------*/
/*void ds1302_write_time(void){
	unsigned char i = 0;
	TIME[0] = 0x33;				// ��Ӧ 24s
	TIME[1] = 0x22;				// ��Ӧ 39m
	TIME[2] = 0x11;				// ��Ӧ 11h
	TIME[3] = 0x23;				// ��Ӧ 6��
	TIME[4] = 0x12;				// ��Ӧ 12��
	TIME[5] = 0x02;				// ��Ӧ ����2
	TIME[6] = 0x28;				// ��Ӧ 2016��

	ds1302_write_reg(0x8e,0x00);  //�ر�д����
	for(i = 0;i < 7;++i){
		ds1302_write_reg(WRITE_RTC_ADDR[i], TIME[i]);
	}
	ds1302_write_reg(0x8e,0x80); //��д����
}

void ds1302_read_time(void){
	unsigned char i = 0;
	
	for(i = 0;i < 7;++i){//ע���Ƕ��߸��Ĵ�������ֵ	Ȼ��д��TIme�������淽�����ĺ�����
	  TIME[i] = ds1302_read_reg(READ_RTC_ADDR[i]);   
	}	
}	*/
/*-----------------------------------------������Ҫ�ĺ���ʹ��ʮ���ƻ���bcd֮������໥ת��----------------------------------------------------------------*/
unsigned char bcd2decimal(unsigned char bcd){//���յ���bcd�룬����0x24����Ҫ���ʮ����24
	//��һ���Ǵ�0x24�õ�2��4 ��
	//����λ��(bcd & oxf0)>>4������λ��(bcd & 0x0f)
	
	//�ڶ�����2*10+4
	
	return ((bcd & 0xf0)>>4)*10 + (bcd & 0x0f);	
}

unsigned char decimal2bcd(unsigned char decimal){
	//������Ҫ��decimalתΪbcd
	 return ((decimal/10)<<4 | (decimal%10)) ; 
		
}

void ds1302_read_time_struct(void){
//��ע�⡿ ds1302_read_regֱ�Ӷ���������bcd�룬ֱ��ȥ��ʾ��������ģ������ҪתΪʮ����
	mytime.year =  bcd2decimal(ds1302_read_reg(REG_ADDR_READ_YEAR))+2000;	  //���Ǹ���ַ����������������λ����������Ҫ����2000����ʾ��ȷ���� 
	mytime.month = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_MONTH));
	mytime.day = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_DAY));
	mytime.hour = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_HOUR));
	mytime.minute = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_MINUTE));
	mytime.second = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_SECOND));
	mytime.week = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_WEEK));
}

void ds1302_write_time_struct(struct allTime t1){
	ds1302_write_reg(0x8e,0x00);  //�ر�д����

	ds1302_write_reg(REG_ADDR_WRITE_YEAR, decimal2bcd(t1.year-2000));
	ds1302_write_reg(REG_ADDR_WRITE_MONTH, decimal2bcd(t1.month));
	ds1302_write_reg(REG_ADDR_WRITE_DAY, decimal2bcd(t1.day));
    ds1302_write_reg(REG_ADDR_WRITE_HOUR, decimal2bcd(t1.hour));
    ds1302_write_reg(REG_ADDR_WRITE_MINUTE, decimal2bcd(t1.minute));
    ds1302_write_reg(REG_ADDR_WRITE_SECOND, decimal2bcd(t1.second));
    ds1302_write_reg(REG_ADDR_WRITE_WEEK, decimal2bcd(t1.week));

	ds1302_write_reg(0x8e,0x80); //��д����
}


