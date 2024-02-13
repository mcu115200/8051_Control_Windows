#ifndef __ADC0832_H_
#define __ADC0832_H_

//---包含头文件---//
#include<reg51.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif
//******************adc0832****************************//
sbit CLK=	P3^2;//时钟,初始状态为0
sbit DI=	P3^3;//数据输入
sbit DO=	P3^4; // 数据输出
sbit CS=	P3^5;
//---定义全局函数---//
uchar ADC(uchar ch);

#endif