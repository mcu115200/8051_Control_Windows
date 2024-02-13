#ifndef __SHT11_H_
#define __SHT11_H_

#include <reg51.h>
sbit Data_P= P3^1;			// SHT11传感器的数据管脚
sbit Sck_P= P3^0;			// SHT11传感器的时钟管脚

extern unsigned char sht_temp;			// 保存温度
extern unsigned char sht_humi;			// 保存湿度

void ReadShtData();
#endif
