#ifndef __ADC0832_H_
#define __ADC0832_H_

//---����ͷ�ļ�---//
#include<reg51.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif
//******************adc0832****************************//
sbit CLK=	P3^2;//ʱ��,��ʼ״̬Ϊ0
sbit DI=	P3^3;//��������
sbit DO=	P3^4; // �������
sbit CS=	P3^5;
//---����ȫ�ֺ���---//
uchar ADC(uchar ch);

#endif