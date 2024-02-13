#ifndef __LCD1602_H_
#define __LCD1602_H_
/**********************************
��ʹ�õ���4λ���ݴ����ʱ���壬
ʹ��8λȡ���������
**********************************/
//#define LCD1602_4PINS

/**********************************
����ͷ�ļ�
**********************************/
#include<reg51.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/**********************************
PIN�ڶ���
**********************************/
sbit rs=P2^0;	 //1602����/����ѡ������ H:����      	L:����
sbit rw=P2^1;	 //1602��д����	         H:���ݼĴ���  	L:ָ��Ĵ���
sbit e =P2^2;	 //1602ʹ������          �½��ش���

/**********************************
��������
**********************************/
void write_com(uchar com);						   //LCD1602д��8λ�����Ӻ���	
void write_data(uchar dat);						   //1602д���ݺ���		
void init_1602();								   //LCD1602��ʼ���ӳ���
void write_string(uchar hang,uchar add,uchar *p);  //��ʾ�ַ���
void delay_uint(uint i);
#endif
