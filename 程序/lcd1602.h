#ifndef __LCD1602_H_
#define __LCD1602_H_
/**********************************
当使用的是4位数据传输的时候定义，
使用8位取消这个定义
**********************************/
//#define LCD1602_4PINS

/**********************************
包含头文件
**********************************/
#include<reg51.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/**********************************
PIN口定义
**********************************/
sbit rs=P2^0;	 //1602数据/命令选择引脚 H:数据      	L:命令
sbit rw=P2^1;	 //1602读写引脚	         H:数据寄存器  	L:指令寄存器
sbit e =P2^2;	 //1602使能引脚          下降沿触发

/**********************************
函数声明
**********************************/
void write_com(uchar com);						   //LCD1602写入8位数据子函数	
void write_data(uchar dat);						   //1602写数据函数		
void init_1602();								   //LCD1602初始化子程序
void write_string(uchar hang,uchar add,uchar *p);  //显示字符串
void delay_uint(uint i);
#endif
