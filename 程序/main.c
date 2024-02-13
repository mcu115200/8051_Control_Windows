#include "reg51.h"
#include "ADC0832.h"
#include "lcd1602.h"
#include "SHT11.h"
#define uchar unsigned char
#define uint unsigned int
sbit AA=P2^4;//���
sbit BB=P2^5;
sbit CC=P2^6;
sbit DD=P2^7;
sbit led=P2^3;//ָʾ��
sbit man=P3^6;//�������

uchar yan=0,light=0;//��������
uchar lim1=50,lim2=50,lim3=25,lim4=70;//��ֵ
uchar mode=0,time=0;
uchar disp1[]="00";
uchar disp2[]="000";

uchar key_scan()//�������
{
uchar i,j;
i=0;
j=0;
P1=0x0f;
if(P1!=0x0f) //������ް���
{
 switch(P1)//�����
 {
  case 0x0e:i=1;break;
  case 0x0d:i=5;break;
  case 0x0b:i=9;break;
  case 0x07:i=13;
 }
 P1=0xf0;
 switch(P1)//�����
 {
  case 0xe0:j=0;break;
  case 0xd0:j=1;break;
  case 0xb0:j=2;break;
  case 0x70:j=3;
 }
 while(P1!=0xf0);//�ȴ������ɿ�
}
return i+j;
}

void zheng()//��ת
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
void fan() //��ת
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
//������
void main()
{
	uchar i=0;
	init_1602();
	TMOD|=0X01;
	TH0=0X3C;
	TL0=0XB0;	
	ET0=1;//�򿪶�ʱ��0�ж�����
	EA=1;//�����ж�
	TR0=1;//�򿪶�ʱ��
	while(1)
	{
		 i=key_scan();//�������
		 if(i==1)//����
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
		//�жϣ���������
		if((yan>lim1)||(sht_temp>lim3)||(sht_humi>lim4))
		{
			if(led)
				zheng();//����
		}
		else
		{
			if((light>lim2)|| !man)
			{
				if(!led)
					fan();//�ش�
			}
		}
		//��������
		if(mode==1)
		{
			if(i==3)//�¶�
			{
				if(lim3<99)
					lim3++;
			}
			if(i==4)
			{
				if(lim3>0)
					lim3--;
			}
			if(i==5)//����
			{
				if(lim1<99)
					lim1++;
			}
			if(i==6)
			{
				if(lim1>0)
					lim1--;
			}
			if(i==7)//ʪ��
			{
				if(lim4<99)
					lim4++;
			}
			if(i==8)
			{
				if(lim4>0)
					lim4--;
			}
			if(i==9)//����
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
//��ʱ��0�ж�
void Timer0() interrupt 1
{
	uint i;
	if(time<10)//��ʱ����
		time++;
	else
	{
		time=0;
		yan=ADC(1);//��������
		light=ADC(2);//��������
		i=yan;
		i=i*99/128;
		yan=i;
		
		ReadShtData();//������ʪ��

		//��ʾ
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