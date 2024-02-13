#include "ADC0832.h"
#include "intrins.h"
/*********************************************
读取ADC
**********************************************/
uchar ADC(uchar ch)//通道ch	 1,2
{
	uchar temp0,temp1,i;
	CS=0;
	temp0=0;
	temp1=0;
	_nop_();
	_nop_();
	DI=1;//开始位
	_nop_();
	_nop_();
	CLK=1;
	_nop_();
	_nop_();
	CLK=0;
	_nop_();
	_nop_();
	DI=0;
	_nop_();
	_nop_();
//选择通道0
	DI=1;
	_nop_();
	CLK=1;//上升沿DI=1
	_nop_();
	CLK=0;//1个下降沿DI=1
	_nop_();
	if(ch==1)
	DI=0;
	if(ch==2)
	DI=1;
	_nop_();
	CLK=1;
	_nop_();
	CLK=0;//第3个上升沿DI=0
	_nop_();
   	DI=1;
//********通道选择结束开始读取转换后的二进制数****
//下降沿读数，一下进行判断和处理,共8次
for(i=0;i<8;i++)
   {
	temp0=temp0<<1;
	CLK=1;
    if(DO)
   	temp0++;
   	_nop_();
   	CLK=0;
   }
for(i=0;i<8;i++)
   {
	temp1=temp1>>1;
	CLK=1;
    if(DO)
   	temp1=temp1 +0x80;
   	_nop_();
   	CLK=0;
   }
	 CS=1;
   return temp0;
}