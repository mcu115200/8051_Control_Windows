#include <SHT11.h>
#include"intrins.h"
unsigned char sht_temp;			// 保存温度
unsigned char sht_humi;			// 保存湿度

typedef union              		//定义共用同类型
{
	unsigned int i;
	float f;
}value;

char ShtWriteByte(unsigned char value)
{
	unsigned char i,error=0;
	for(i=128;i>0;i>>=1)  // 高位为1，循环右移
	{
		if (i&value)
			Data_P=1;       	// 和要发送的数相与，结果为发送的位
		else
			Data_P=0;
		Sck_P=1;
		_nop_();						// 延时3us
		_nop_();
		_nop_();
		Sck_P=0;
	}
	Data_P=1;    					// 释放数据线
	Sck_P=1;
	error=Data_P;  				// 检查应答信号，确认通讯正常
	_nop_();
	_nop_();
	_nop_();
	Sck_P=0;
	Data_P=1;
	return error; 				// error=1 通讯错误
}

char ShtReadByte(unsigned char ack)
{
	unsigned char i,val=0;
	Data_P=1; 						// 释放数据线
	for(i=0x80;i>0;i>>=1)	// 高位为1，循环右移
	{
		Sck_P=1;
		if(Data_P)
			val=(val|i);    	// 读一位数据线的值
		Sck_P=0;
	}
	Data_P=!ack;    			// 如果是校验，读取完后结束通讯
	Sck_P=1;
	_nop_();							// 延时3us
	_nop_();
	_nop_();
	Sck_P=0;
	_nop_();
	_nop_();
	_nop_();
	Data_P=1; 						// 释放数据线
	return val;
}


void ShtTransStart(void)
{
	Data_P=1;
	Sck_P=0;
	_nop_();
	Sck_P=1;
	_nop_();
	Data_P=0;
	_nop_();
	Sck_P=0;
	_nop_();
	_nop_();
	_nop_();
	Sck_P=1;
	_nop_();
	Data_P=1;
	_nop_();
	Sck_P=0;
}

void ShtConnectReset(void)
{
	unsigned char i;
	Data_P=1; 		   		//准备
	Sck_P=0;
	for(i=0;i<9;i++)  	//DATA保持高，SCK时钟触发9次，发送启动传输，通迅即复位
	{
		Sck_P=1;
		Sck_P=0;
	}
	ShtTransStart();   	//启动传输
}

char ShtMeasure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode)
{
	unsigned error=0;
	unsigned int i;
	ShtTransStart();  		// 启动传输
	switch(mode)       		// 选择发送命令
	{
		case 1 :   					// 测量温度
			error+=ShtWriteByte(0x03);
			break;
		case 2 :   					// 测量湿度
			error+=ShtWriteByte(0x05);
			break;
		default:
			break;
	}
	for(i=0;i<65535;i++)
		if(Data_P==0)
			break;  					// 等待测量结束
		if(Data_P)
			error+=1;   			// 如果长时间数据线没有拉低，说明测量错误
	*(p_value) =ShtReadByte(1);  		// 读第一个字节，高字节 (MSB)
	*(p_value+1)=ShtReadByte(1); 		// 读第二个字节，低字节 (LSB)
	*p_checksum =ShtReadByte(0);  	// read CRC校验码
	return error;  									// error=1 通讯错误
}

void CalcSHT11(float *p_humidity ,float *p_temperature)
{
	const float C1=-4.0;	 			// 12位湿度精度 修正公式
	const float C2=+0.0405;			// 12位湿度精度 修正公式
	const float C3=-0.0000028;	// 12位湿度精度 修正公式
	const float T1=+0.01;	 			// 14位温度精度 5V条件 修正公式
	const float T2=+0.00008;	 	// 14位温度精度 5V条件 修正公式
	float rh=*p_humidity;	 			// rh: 12位 湿度
	float t=*p_temperature;			// t:  14位 温度
	float rh_lin;								// rh_lin: 湿度 linear值
	float rh_true;							// rh_true: 湿度 ture值
	float t_C;	 								// t_C : 温度 ℃
	t_C=t*0.01 - 40;	 					//补偿温度
	rh_lin=C3*rh*rh + C2*rh + C1;					//相对湿度非线性补偿
	rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;		//相对湿度对于温度依赖性补偿
	*p_temperature=t_C;	 				//返回温度结果
	*p_humidity=rh_true;	 			//返回湿度结果
}

unsigned char TempCorrect(int temp)
{
	if(temp<0)	temp=0;
	if(temp>970)  temp=970;
	if(temp>235)  temp=temp+10;
	if(temp>555)  temp=temp+10;
	if(temp>875)  temp=temp+10;
	temp=(temp%1000)/10;
	return temp;
}

unsigned char HumiCorrect(unsigned int humi)
{
	if(humi>999)  humi=999;
	if((humi>490)&&(humi<951))  humi=humi-10;
	humi=(humi%1000)/10;
	return humi+4;
}

void ReadShtData()
{
	value humi_val,temp_val;  	// 定义两个共同体，一个用于湿度，一个用于温度
	unsigned char error;  							// 用于检验是否出现错误
	unsigned char checksum;  						// CRC
	unsigned int temp1,humi1;						// 临时读取到的温湿度数据

	error=0; 										//初始化error=0，即没有错误
	error+=ShtMeasure((unsigned char*)&temp_val.i,&checksum,1); 	//温度测量
	error+=ShtMeasure((unsigned char*)&humi_val.i,&checksum,2); 	//湿度测量

	if(error!=0) 		  					//如果发生错误，系统复位
		ShtConnectReset();
	else
	{
		humi_val.f=(float)humi_val.i; 				//转换为浮点数
		temp_val.f=(float)temp_val.i;  				//转换为浮点数
		CalcSHT11(&humi_val.f,&temp_val.f);  	//修正相对湿度及温度
		temp1=temp_val.f*10;
		sht_temp=TempCorrect(temp1);
		humi1=humi_val.f*10-50;
		sht_humi=HumiCorrect(humi1);
		humi1=humi1-1;
	}

}