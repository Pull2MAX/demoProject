#ifndef _UART_H_
#define _UART_H_
#include<reg51.h>
#include <stdio.h>

/*
  uart是做串口通信的，我们控制cpu给单片机发东西，然后单片机再回复给我们的主机，主机通过一些软件就可以看到实际的效果
  【注意】如果用的是usb，那么应该将开发板上的 usb的t和单片机的r接在一块，usb的r和单片机的t接在一块

  1.本次晶振是12.0MHZ ，八个数据位，1个停止位，没有校验位，【本次实验的波特率一律是4800】
  2.工作在12T情况下
*/

void uart_init(void);
void uart_send_byte(unsigned char content);
void uart_send_string(unsigned char *str); 
void uart_send_double2string(double value);//目的是将double转为string然后经过uart发出去

#endif