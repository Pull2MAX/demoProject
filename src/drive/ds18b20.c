#include"ds18b20.h"

/*---------------------------------延时函数----------------------------------------------------*/
//下面写static是因为只要这个函数在本文件中可见就行了
static void delay700us(void)   //延时700us，专门为init里面的延时设计的
{
    unsigned char a,b;
    for(b=139;b>0;b--)
        for(a=1;a>0;a--);
}

static void delay15us(void)   //延时15us，专门是为了初始化之后，给dsport将高电平拉低为低电平设计的
{
    unsigned char a;
    for(a=15;a>0;a--);
}

static void delay45us(void)   //误差45us,专门为了readbyte函数设计的，采样之后必须要持续45us
{
    unsigned char a;
    for(a=20;a>0;a--);
}

static void delay70us(void)   //误差 70us，专门是为了writebyte函数设计的，至少持续68us
{
    unsigned char a,b;
    for(b=13;b>0;b--)
        for(a=1;a>0;a--);
}

static void delay1ms(void)   //误差1ms,为了在ds18b20ADC里面初始化后转换温度用
{
    unsigned char a,b;
    for(b=199;b>0;b--)
        for(a=1;a>0;a--);
}

static void delay750ms(void)   //误差 750ms
{
    unsigned char a,b,c;
    for(c=61;c>0;c--)
        for(b=28;b>0;b--)
            for(a=218;a>0;a--);
}


/*---------------------------------底层函数----------------------------------------------------*/
/*---------初始化--------------*/

static uchar Ds18b20Init(){//初始化就是探测这个芯片在不在，总线先会从0到1的变化，如果芯片在的话会主动总线的芯片的拉低的
					//返回结果是1表示成功，0代表失败
	uchar i = 0;
	DSPORT = 0;	//先将这个总线拉低，低电平480us-960us总线所有的器件就会复位
	delay700us();//延时700us，所有器件复位了
	DSPORT = 1;//单片机将电平拉高，如果ds18b20存在的话，等15--60us这个时间范围之内，必定会将DSPORT拉低。错过15us到60us这个窗口期就没办法获取到拉低没有。
	i = 0;
	while(DSPORT){ //如果有ds18b20拉低了那么就是有 ds18b20存在，双方可以通信
		i++;
		if(i > 5){ //等待超时了，五次检查都没有发现DSPORT为0就直接退出
			return 0;//初始化失败
		}
		delay15us();//和上面i>5联动，等待的时间是15us--60us		
	}
	return 1;//初始化成功		
}

/*-----------写函数(依据时序版)--------------*/
static void Ds18b20WriteByte(uchar dat){//写的时候权力都在mcu手中，mcu控制占据总线	
	uint i = 0,j = 0;
	for(j = 0;j < 8;++j){
		DSPORT = 0;//先从高电平拉低到低电平至少1us代表开始
		i++;//起延时1us作用
		DSPORT= dat & 0x01;	//【注意是从最低位开始读取的】
		delay70us();//单片机从高到低的时间 + dsport持续发送信号的时间总共耗时至少是60us，这个循环耗时是68us，只能多不能少。
		DSPORT = 1;//读出结果了，释放总线
		dat = dat >> 1;//既达到了延时1us的要求，还达到了位移dat方便下次循环		
	}
}
/*-----------读函数(依据时序版)--------------*/
static uchar Ds18b20ReadByte(){   //mcu只是起手发了一个开始的控制序列，后面总线的权力就交给了ds18b20芯片
	uchar byte = 0,bi = 0;
	uint i = 0,j = 0;
	for(j = 0;j < 8;++j){
		DSPORT = 0;//先拉低一会代表开始写了
		i++;
		DSPORT = 1;//拉高表示权力交出去了，ds18b20可以拉高拉低
		i++;
		i++; 
		//i++;      //两个浪费时间的举措，因为原来是0，然后总线释放到了1需要时间。采样的时间很短+ds18b20将数据放到总线也是需要时间的。太早抓不到数据，太晚了数据就没有了。
		bi = DSPORT;//单片机收到ds18b20的发送信息后就传给bi	,采样时间很短
		byte = byte | (bi << j);
		delay45us();//依据时序图，需要做延时45us左右	
	}	
	return byte;		
}


/*---------------------------------高层函数----------------------------------------------------*/

/*-----------转换温度--------------*/
static void DS18b20ADCTemperature(void){	 //这个函数的功能是让ds18b20开始转换温度了 ，然后将测好的结果放告诉暂存器里面
	Ds18b20Init();
	delay1ms();
	Ds18b20WriteByte(0xcc);//跳过ROM操作
	Ds18b20WriteByte(0x44);//温度转换的指令
	delay750ms();//给一定的时间去转温度，这个750us的时间是数据手册说的	
}
/*-----------读取暂存器里面的温度数据--------------*/
static void DS18b20ReadTemperature(void){
	Ds18b20Init();
	delay1ms();
	Ds18b20WriteByte(0xcc);	 //跳过rom操作
	Ds18b20WriteByte(0xbe); //发送读取温度命令
}

/*-----------ds18b20里面的具体数值被传送出来（集成了上面的两个函数）--------------*/
/*uint Ds18b20Readtemperature(void){
	uint temperature = 0;
	uint th,tl;
	DS18b20ADCTemperature();  //告诉芯片可以开始转换了
	DS18b20ReadTemperature(); // 告诉芯片我要开始读你了
	tl = Ds18b20ReadByte();	  //精度是12位的，但是是两个字节接收的
	th = Ds18b20ReadByte();
	temperature = tl | (th<<8);	 //移动八位是为了要空出16位的位置，然后方便低位去拼接
	
	return temperature;
}*/

double Ds18b20ReadDecimalTemperature(void){	//这个函数是为了通过串口测试下高八位和低八位数据到底有 没有读取到
	uint temperature = 0;
	uchar th,tl;
	double doubleTemperature = 0;
	DS18b20ADCTemperature();  //告诉芯片可以开始转换了
	DS18b20ReadTemperature(); // 告诉芯片我要开始读你了
	tl = Ds18b20ReadByte();	  //精度是12位的，但是是两个字节接收的
	th = Ds18b20ReadByte();
	temperature = tl | (th<<8);	 //移动八位是为了要空出16位的位置，然后方便低位去拼接
	//temperature = temperature*625/10000;//正确的写法，但是这样会溢出	

	//temperature = temperature*62/1000;	  //损失下精度，先简单测试下能不能发

	doubleTemperature = temperature*0.0625;
	//uart_send_byte(0);
	//uart_send_byte((unsigned char)doubleTemperature);  //经测试，是可以发送到的，最后的结果就是十六进制
	//uart_send_byte(th);

	return doubleTemperature;

	/* 读取结果：
	00  6A  01  00  6A  01  00  6A  01  00  6A  01  00  6A  01  00  6A  01  
	00  6B  01  00  6A  01  00  6D  01  00  7C  01  00 
	 8A  01  00  96  01  00  9E  01  00  A4  01  00  AA  01  00  AF  01  00  00  00  00  
	*/
}








