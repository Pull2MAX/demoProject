#ifndef _UART_H_
#define _UART_H_
#include<reg51.h>
#include <stdio.h>

/*
  uart��������ͨ�ŵģ����ǿ���cpu����Ƭ����������Ȼ��Ƭ���ٻظ������ǵ�����������ͨ��һЩ����Ϳ��Կ���ʵ�ʵ�Ч��
  ��ע�⡿����õ���usb����ôӦ�ý��������ϵ� usb��t�͵�Ƭ����r����һ�飬usb��r�͵�Ƭ����t����һ��

  1.���ξ�����12.0MHZ ���˸�����λ��1��ֹͣλ��û��У��λ��������ʵ��Ĳ�����һ����4800��
  2.������12T�����
*/

void uart_init(void);
void uart_send_byte(unsigned char content);
void uart_send_string(unsigned char *str); 
void uart_send_double2string(double value);//Ŀ���ǽ�doubleתΪstringȻ�󾭹�uart����ȥ

#endif