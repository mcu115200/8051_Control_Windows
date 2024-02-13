#ifndef __SHT11_H_
#define __SHT11_H_

#include <reg51.h>
sbit Data_P= P3^1;			// SHT11�����������ݹܽ�
sbit Sck_P= P3^0;			// SHT11��������ʱ�ӹܽ�

extern unsigned char sht_temp;			// �����¶�
extern unsigned char sht_humi;			// ����ʪ��

void ReadShtData();
#endif
