#include"ds18b20.h"

/*---------------------------------��ʱ����----------------------------------------------------*/
//����дstatic����ΪֻҪ��������ڱ��ļ��пɼ�������
static void delay700us(void)   //��ʱ700us��ר��Ϊinit�������ʱ��Ƶ�
{
    unsigned char a,b;
    for(b=139;b>0;b--)
        for(a=1;a>0;a--);
}

static void delay15us(void)   //��ʱ15us��ר����Ϊ�˳�ʼ��֮�󣬸�dsport���ߵ�ƽ����Ϊ�͵�ƽ��Ƶ�
{
    unsigned char a;
    for(a=15;a>0;a--);
}

static void delay45us(void)   //���45us,ר��Ϊ��readbyte������Ƶģ�����֮�����Ҫ����45us
{
    unsigned char a;
    for(a=20;a>0;a--);
}

static void delay70us(void)   //��� 70us��ר����Ϊ��writebyte������Ƶģ����ٳ���68us
{
    unsigned char a,b;
    for(b=13;b>0;b--)
        for(a=1;a>0;a--);
}

static void delay1ms(void)   //���1ms,Ϊ����ds18b20ADC�����ʼ����ת���¶���
{
    unsigned char a,b;
    for(b=199;b>0;b--)
        for(a=1;a>0;a--);
}

static void delay750ms(void)   //��� 750ms
{
    unsigned char a,b,c;
    for(c=61;c>0;c--)
        for(b=28;b>0;b--)
            for(a=218;a>0;a--);
}


/*---------------------------------�ײ㺯��----------------------------------------------------*/
/*---------��ʼ��--------------*/

static uchar Ds18b20Init(){//��ʼ������̽�����оƬ�ڲ��ڣ������Ȼ��0��1�ı仯�����оƬ�ڵĻ����������ߵ�оƬ�����͵�
					//���ؽ����1��ʾ�ɹ���0����ʧ��
	uchar i = 0;
	DSPORT = 0;	//�Ƚ�����������ͣ��͵�ƽ480us-960us�������е������ͻḴλ
	delay700us();//��ʱ700us������������λ��
	DSPORT = 1;//��Ƭ������ƽ���ߣ����ds18b20���ڵĻ�����15--60us���ʱ�䷶Χ֮�ڣ��ض��ὫDSPORT���͡����15us��60us��������ھ�û�취��ȡ������û�С�
	i = 0;
	while(DSPORT){ //�����ds18b20��������ô������ ds18b20���ڣ�˫������ͨ��
		i++;
		if(i > 5){ //�ȴ���ʱ�ˣ���μ�鶼û�з���DSPORTΪ0��ֱ���˳�
			return 0;//��ʼ��ʧ��
		}
		delay15us();//������i>5�������ȴ���ʱ����15us--60us		
	}
	return 1;//��ʼ���ɹ�		
}

/*-----------д����(����ʱ���)--------------*/
static void Ds18b20WriteByte(uchar dat){//д��ʱ��Ȩ������mcu���У�mcu����ռ������	
	uint i = 0,j = 0;
	for(j = 0;j < 8;++j){
		DSPORT = 0;//�ȴӸߵ�ƽ���͵��͵�ƽ����1us����ʼ
		i++;//����ʱ1us����
		DSPORT= dat & 0x01;	//��ע���Ǵ����λ��ʼ��ȡ�ġ�
		delay70us();//��Ƭ���Ӹߵ��͵�ʱ�� + dsport���������źŵ�ʱ���ܹ���ʱ������60us�����ѭ����ʱ��68us��ֻ�ܶ಻���١�
		DSPORT = 1;//��������ˣ��ͷ�����
		dat = dat >> 1;//�ȴﵽ����ʱ1us��Ҫ�󣬻��ﵽ��λ��dat�����´�ѭ��		
	}
}
/*-----------������(����ʱ���)--------------*/
static uchar Ds18b20ReadByte(){   //mcuֻ�����ַ���һ����ʼ�Ŀ������У��������ߵ�Ȩ���ͽ�����ds18b20оƬ
	uchar byte = 0,bi = 0;
	uint i = 0,j = 0;
	for(j = 0;j < 8;++j){
		DSPORT = 0;//������һ�����ʼд��
		i++;
		DSPORT = 1;//���߱�ʾȨ������ȥ�ˣ�ds18b20������������
		i++;
		i++; 
		//i++;      //�����˷�ʱ��ľٴ룬��Ϊԭ����0��Ȼ�������ͷŵ���1��Ҫʱ�䡣������ʱ��ܶ�+ds18b20�����ݷŵ�����Ҳ����Ҫʱ��ġ�̫��ץ�������ݣ�̫�������ݾ�û���ˡ�
		bi = DSPORT;//��Ƭ���յ�ds18b20�ķ�����Ϣ��ʹ���bi	,����ʱ��ܶ�
		byte = byte | (bi << j);
		delay45us();//����ʱ��ͼ����Ҫ����ʱ45us����	
	}	
	return byte;		
}


/*---------------------------------�߲㺯��----------------------------------------------------*/

/*-----------ת���¶�--------------*/
static void DS18b20ADCTemperature(void){	 //��������Ĺ�������ds18b20��ʼת���¶��� ��Ȼ�󽫲�õĽ���Ÿ����ݴ�������
	Ds18b20Init();
	delay1ms();
	Ds18b20WriteByte(0xcc);//����ROM����
	Ds18b20WriteByte(0x44);//�¶�ת����ָ��
	delay750ms();//��һ����ʱ��ȥת�¶ȣ����750us��ʱ���������ֲ�˵��	
}
/*-----------��ȡ�ݴ���������¶�����--------------*/
static void DS18b20ReadTemperature(void){
	Ds18b20Init();
	delay1ms();
	Ds18b20WriteByte(0xcc);	 //����rom����
	Ds18b20WriteByte(0xbe); //���Ͷ�ȡ�¶�����
}

/*-----------ds18b20����ľ�����ֵ�����ͳ��������������������������--------------*/
/*uint Ds18b20Readtemperature(void){
	uint temperature = 0;
	uint th,tl;
	DS18b20ADCTemperature();  //����оƬ���Կ�ʼת����
	DS18b20ReadTemperature(); // ����оƬ��Ҫ��ʼ������
	tl = Ds18b20ReadByte();	  //������12λ�ģ������������ֽڽ��յ�
	th = Ds18b20ReadByte();
	temperature = tl | (th<<8);	 //�ƶ���λ��Ϊ��Ҫ�ճ�16λ��λ�ã�Ȼ�󷽱��λȥƴ��
	
	return temperature;
}*/

double Ds18b20ReadDecimalTemperature(void){	//���������Ϊ��ͨ�����ڲ����¸߰�λ�͵Ͱ�λ���ݵ����� û�ж�ȡ��
	uint temperature = 0;
	uchar th,tl;
	double doubleTemperature = 0;
	DS18b20ADCTemperature();  //����оƬ���Կ�ʼת����
	DS18b20ReadTemperature(); // ����оƬ��Ҫ��ʼ������
	tl = Ds18b20ReadByte();	  //������12λ�ģ������������ֽڽ��յ�
	th = Ds18b20ReadByte();
	temperature = tl | (th<<8);	 //�ƶ���λ��Ϊ��Ҫ�ճ�16λ��λ�ã�Ȼ�󷽱��λȥƴ��
	//temperature = temperature*625/10000;//��ȷ��д�����������������	

	//temperature = temperature*62/1000;	  //��ʧ�¾��ȣ��ȼ򵥲������ܲ��ܷ�

	doubleTemperature = temperature*0.0625;
	//uart_send_byte(0);
	//uart_send_byte((unsigned char)doubleTemperature);  //�����ԣ��ǿ��Է��͵��ģ����Ľ������ʮ������
	//uart_send_byte(th);

	return doubleTemperature;

	/* ��ȡ�����
	00  6A  01  00  6A  01  00  6A  01  00  6A  01  00  6A  01  00  6A  01  
	00  6B  01  00  6A  01  00  6D  01  00  7C  01  00 
	 8A  01  00  96  01  00  9E  01  00  A4  01  00  AA  01  00  AF  01  00  00  00  00  
	*/
}








