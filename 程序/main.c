#include "reg51.h"
#include "ADC0832.h"
#include "lcd1602.h"
#include "SHT11.h"
#define uchar unsigned char
#define uint unsigned int
sbit AA=P2^4;//电机
sbit BB=P2^5;
sbit CC=P2^6;
sbit DD=P2^7;
sbit led=P2^3;//指示灯
sbit man=P3^6;//人体红外

uchar yan=0,light=0;//烟雾、光照
uchar lim1=50,lim2=50,lim3=25,lim4=70;//阀值
uchar mode=0,time=0;
uchar disp1[]="00";
uchar disp2[]="000";

uchar key_scan()//按键检测
{
uchar i,j;
i=0;
j=0;
P1=0x0f;
if(P1!=0x0f) //检测有无按下
{
 switch(P1)//检测行
 {
  case 0x0e:i=1;break;
  case 0x0d:i=5;break;
  case 0x0b:i=9;break;
  case 0x07:i=13;
 }
 P1=0xf0;
 switch(P1)//检测列
 {
  case 0xe0:j=0;break;
  case 0xd0:j=1;break;
  case 0xb0:j=2;break;
  case 0x70:j=3;
 }
 while(P1!=0xf0);//等待按键松开
}
return i+j;
}

void zheng()//正转
{	
	uchar i=0;
	for(i=0;i<20;i++)
	{
	BB=1;delay_uint(5000);
	AA=0;delay_uint(5000);
	CC=1;delay_uint(5000);
	BB=0;delay_uint(5000);
	DD=1;delay_uint(5000);
	CC=0;delay_uint(5000);
	AA=1;delay_uint(5000);
	DD=0;delay_uint(5000);
	}
	led=0;
}
void fan() //反转
{
	uchar i=0;
	for(i=0;i<20;i++)
	{
		DD=1;delay_uint(5000);
		AA=0;delay_uint(5000);
		CC=1;delay_uint(5000);
		DD=0;delay_uint(5000);
		BB=1;delay_uint(5000);
		CC=0;delay_uint(5000);
		AA=1;delay_uint(5000);
		BB=0;delay_uint(5000);
	}
	led=1;
}
//主函数
void main()
{
	uchar i=0;
	init_1602();
	TMOD|=0X01;
	TH0=0X3C;
	TL0=0XB0;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器
	while(1)
	{
		 i=key_scan();//按键检测
		 if(i==1)//设置
		 {
		 	mode=1;
			write_string(1,0,"                ");
			write_string(2,0,"                ");
		}
		if(i==2)//OK
		{
			mode=0;
			write_string(1,0,"                ");
			write_string(2,0,"                ");
			}
		//判断，窗户控制
		if((yan>lim1)||(sht_temp>lim3)||(sht_humi>lim4))
		{
			if(led)
				zheng();//开窗
		}
		else
		{
			if((light>lim2)|| !man)
			{
				if(!led)
					fan();//关窗
			}
		}
		//按键设置
		if(mode==1)
		{
			if(i==3)//温度
			{
				if(lim3<99)
					lim3++;
			}
			if(i==4)
			{
				if(lim3>0)
					lim3--;
			}
			if(i==5)//烟雾
			{
				if(lim1<99)
					lim1++;
			}
			if(i==6)
			{
				if(lim1>0)
					lim1--;
			}
			if(i==7)//湿度
			{
				if(lim4<99)
					lim4++;
			}
			if(i==8)
			{
				if(lim4>0)
					lim4--;
			}
			if(i==9)//光照
			{
				if(lim2<255)
					lim2++;
			}
			if(i==10)
			{
				if(lim2>0)
					lim2--;
			}
		}
	}
}
//定时器0中断
void Timer0() interrupt 1
{
	uint i;
	if(time<10)//延时采样
		time++;
	else
	{
		time=0;
		yan=ADC(1);//测量烟雾
		light=ADC(2);//测量光照
		i=yan;
		i=i*99/128;
		yan=i;
		
		ReadShtData();//测量温湿度

		//显示
		if(mode==0)
		{
			disp1[0]=yan/10+0x30;
			disp1[1]=yan%10+0x30;
			write_string(1,0,"Y:");
			write_string(1,2,disp1);

			disp2[0]=light/100+0x30;
			disp2[1]=light%100/10+0x30;
			disp2[2]=light%10+0x30;
			write_string(1,4,"%   G:");
			write_string(1,10,disp2);
			write_string(1,13,"Lx");

			disp1[0]=sht_temp/10+0x30;
			disp1[1]=sht_temp%10+0x30;
			write_string(2,0,"T:");
			write_string(2,2,disp1);

			disp1[0]=sht_humi/10+0x30;
			disp1[1]=sht_humi%10+0x30;
			write_string(2,4,"C   S:");
			write_string(2,10,disp1);
			write_string(2,12,"% ");
		}
		else
		{
			disp1[0]=lim1/10+0x30;
			disp1[1]=lim1%10+0x30;
			write_string(1,0,"set:");
			write_string(1,4,disp1);

			disp2[0]=lim2/100+0x30;
			disp2[1]=lim2%100/10+0x30;
			disp2[2]=lim2%10+0x30;
			write_string(1,6,"%  ");
			write_string(1,9,disp2);
			write_string(1,12,"Lx");

			disp1[0]=lim3/10+0x30;
			disp1[1]=lim3%10+0x30;
			write_string(2,4,disp1);

			disp1[0]=lim4/10+0x30;
			disp1[1]=lim4%10+0x30;
			write_string(2,6,"C  ");
			write_string(2,9,disp1);
			write_string(2,11,"% ");
		}
	}
	TH0=0X3C;
	TL0=0XB0;
}