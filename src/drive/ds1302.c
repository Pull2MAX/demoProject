/*--------注意本项目的波特率还有延时时间都是建立在晶振是12MHZ的情况下计算出来的*/

#include"ds1302.h"

/*unsigned char code READ_RTC_ADDR[7] = {0x81,0x83,0x85,0x87,0x89,0x8b,0x8d}; //读的时候应该写的寄存器地址
unsigned char code WRITE_RTC_ADDR[7] = {0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};//写的时候的寄存器地址 */

//unsigned char TIME[7];//存储读出来的真正的时间	  秒分时日月周年

struct allTime mytime;

 /*-------------------------------------------------------------------底层时序----------------------------------------------------------------------*/
//【读和写的地址的开头是不一样的，ds1302会根据开头的读写做相应的准备】
//向ds1302的寄存器addr写入一个值value,这个写指的是单片机【写到】ds1302，所以ds1302自己需要在上升沿【读取】
static void ds1302_write_reg(unsigned char addr,unsigned char value){ 
	
	unsigned char i = 0;
	//unsigned char dat = 0;
	//1.时序开始(初始状态)
	SCLK = 0;
	_nop_();
	RST = 0;
	_nop_();
	//2.写入第一字节  addr的内容
	RST = 1;
	_nop_();
	for(i = 0;i < 8;++i){
		DSIO = addr&0x01; //读取到的最低位放在d0，最高位放在了d7（看原理图）
		addr = addr >> 1;
		//DSIO = dat;//在引脚上准备好数据，在下面的上升沿后会读取引脚的数据
		SCLK = 1;	//上升沿的时候开始读取引脚的数值
		_nop_();
		SCLK= 0;//拉低 为下一次读取准备
		_nop_();
	}	 /*看原理图可知：R/W读取到R/C一共是八位，而寄存器就七个，
	因此只有中间的A0-A4唯一表示寄存器，其他的就是控制位，
	ds1302会屏蔽并且识别正确的寄存器地址，然后下一个for循环会把d0---d7填满	 
	 */

	//3.写入第二字节  value

	for(i = 0;i < 8;++i){
		DSIO = value&0x01;
		value = value>>1;
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();
	}
	//4.时序结束
	RST = 0;
	_nop_();
	SCLK = 0;
	_nop_();
}


//从ds1302的寄存器读取一个字节指的是单片机从【读取】i/o口的数据，因此ds1302需要将地址经过下降沿【写到】i/o口

static unsigned char ds1302_read_reg(unsigned char addr){	 //根据地址的第一位确定是读还是写（是1就表示读，所以传递至都是8开头的）
	unsigned char i = 0;
	unsigned char tmp = 0;
	unsigned char dat = 0;
	
	//1.时序开始
	RST = 0;
	_nop_();
	SCLK = 0;
	_nop_();

	//2.确定mcu要读取哪一个寄存器的地址	，寄存器的地址需要由mcu写到ds1302
	RST = 1;
	_nop_();
	for(i = 0;i < 8;++i){
		DSIO = addr&0x01;//从低位开始
		//DSIO = dat;//将原来的addr【倒着】放到了dsio里面	，这样就合理解释了为什么读写都是8开头，因为dsio最后一位保持是1！
		addr>>=1;
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();
		}
	//3.ds1302根据我们提供的地址找到这个地址里的内容然后通过i/o返回给mcu

	//一旦地址被发送，DS1302 就会将该地址对应的寄存器的数据放在 DS1302 的 I/O 引脚上，而 MCU 则逐位地从这个引脚读取数据。
	//dat = 0;
	for(i = 0;i < 8;++i){
		tmp = DSIO;		 //这一行在最前面的原因是上一个for循环结束的时候是1到0的下降沿，而这个时候就已经可以开始读取d0的数据了 ,另外 写temp的原因是方便编译器理解，keil不支持sbit定义的东西的左右移动 ,当你向 DS1302 发送完寄存器地址后，DS1302 内部逻辑会准备相应寄存器的数据，并将其放置在 I/O 引脚上。  在这个阶段，DS1302 已经知道你要执行的是读取操作，并且知道了要读取的寄存器地址。
		dat |= (tmp << i);	//dsio里存储的第一个是低位，最后一个才是高位
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();				
	}
	//4.时序结束
	SCLK = 0;
	_nop_();
	RST = 0;
	_nop_();
	
	return dat;	  					  
}

/*-----------------------------------------------------------高层时序---------------------------------------------------------------*/
/*void ds1302_write_time(void){
	unsigned char i = 0;
	TIME[0] = 0x33;				// 对应 24s
	TIME[1] = 0x22;				// 对应 39m
	TIME[2] = 0x11;				// 对应 11h
	TIME[3] = 0x23;				// 对应 6日
	TIME[4] = 0x12;				// 对应 12月
	TIME[5] = 0x02;				// 对应 星期2
	TIME[6] = 0x28;				// 对应 2016年

	ds1302_write_reg(0x8e,0x00);  //关闭写保护
	for(i = 0;i < 7;++i){
		ds1302_write_reg(WRITE_RTC_ADDR[i], TIME[i]);
	}
	ds1302_write_reg(0x8e,0x80); //打开写保护
}

void ds1302_read_time(void){
	unsigned char i = 0;
	
	for(i = 0;i < 7;++i){//注意是读七个寄存器的数值	然后写到TIme数组里面方便后面的函数看
	  TIME[i] = ds1302_read_reg(READ_RTC_ADDR[i]);   
	}	
}	*/
/*-----------------------------------------两个重要的函数使得十进制还有bcd之间可以相互转换----------------------------------------------------------------*/
unsigned char bcd2decimal(unsigned char bcd){//接收的是bcd码，比如0x24，需要变成十进制24
	//第一步是从0x24得到2和4 ，
	//高四位是(bcd & oxf0)>>4，低四位是(bcd & 0x0f)
	
	//第二步是2*10+4
	
	return ((bcd & 0xf0)>>4)*10 + (bcd & 0x0f);	
}

unsigned char decimal2bcd(unsigned char decimal){
	//就是需要把decimal转为bcd
	 return ((decimal/10)<<4 | (decimal%10)) ; 
		
}

void ds1302_read_time_struct(void){
//【注意】 ds1302_read_reg直接读出来的是bcd码，直接去显示是有问题的，因此需要转为十进制
	mytime.year =  bcd2decimal(ds1302_read_reg(REG_ADDR_READ_YEAR))+2000;	  //从那个地址读出来的数字是两位数，所以需要加上2000以显示正确的年 
	mytime.month = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_MONTH));
	mytime.day = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_DAY));
	mytime.hour = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_HOUR));
	mytime.minute = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_MINUTE));
	mytime.second = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_SECOND));
	mytime.week = bcd2decimal(ds1302_read_reg(REG_ADDR_READ_WEEK));
}

void ds1302_write_time_struct(struct allTime t1){
	ds1302_write_reg(0x8e,0x00);  //关闭写保护

	ds1302_write_reg(REG_ADDR_WRITE_YEAR, decimal2bcd(t1.year-2000));
	ds1302_write_reg(REG_ADDR_WRITE_MONTH, decimal2bcd(t1.month));
	ds1302_write_reg(REG_ADDR_WRITE_DAY, decimal2bcd(t1.day));
    ds1302_write_reg(REG_ADDR_WRITE_HOUR, decimal2bcd(t1.hour));
    ds1302_write_reg(REG_ADDR_WRITE_MINUTE, decimal2bcd(t1.minute));
    ds1302_write_reg(REG_ADDR_WRITE_SECOND, decimal2bcd(t1.second));
    ds1302_write_reg(REG_ADDR_WRITE_WEEK, decimal2bcd(t1.week));

	ds1302_write_reg(0x8e,0x80); //打开写保护
}


