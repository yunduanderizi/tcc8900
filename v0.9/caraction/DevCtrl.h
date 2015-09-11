#ifndef INCAE_DEV_CTRL_H
#define INCAE_DEV_CTRL_H

struct structAirInfo{
	unsigned char fanspeed;
	unsigned char B13;
	unsigned char B14;
	unsigned char B15;
	unsigned char B16;
	unsigned char B20;
	unsigned char B21;
	unsigned char B22;
	unsigned char B23;
	unsigned char B24;
	unsigned char B25;
	unsigned char B26;
	unsigned char B30;
	unsigned char B31;
	unsigned char B32;
	unsigned char TenTem;
	unsigned char OneTem;
	unsigned char OutTem;

	structAirInfo()
	{
		fanspeed	= 0;
		B13			= 0;
		B14			= 0;
		B15			= 0;
		B16			= 0;
		B20			= 0;
		B21			= 0;
		B22			= 0;
		B23			= 0;
		B24			= 0;
		B25			= 0;
		B26			= 0;
		B30			= 0;
		B31			= 0;
		B32			= 0;
		TenTem		= 0;
		OneTem		= 0;
		OutTem		= 0;
	}
};



int Incar_PowerOff();
int Incar_Reset();
int Incar_initvolume();
int Incar_Setvolume(int value);
int Incar_Muteon();
int Incar_Muteoff();
int Incar_brightnessSet(int value);
int Incar_brightnessGet();
int Incar_backclothSet(int value);
int Incar_backclothGet();
int Incar_ContrastSet(int value);
int Incar_ContrastGet();
int Incar_AirconditionGet(structAirInfo*);

enum
{
	Incar_Power_Gps = 1,
	Incar_Power_Gprs,
	Incar_Power_Lcd,
	Incar_Power_Amp,
	Incar_Power_Fan,
	Incar_Power_Dvd,
};
int Incar_PowerEnable( int device , int enable );

enum
{
	Incar_Dev_Light = 1,			//光亮度值
	Incar_Dev_Shake,			//运动颠簸值 
	Incar_Dev_Fan,				//风扇状态
	Incar_Dev_Lamp,				//远照灯状态
	Incar_Dev_Speed,			//SPEED值
	Incar_Dev_Airconditioning,		//空调状态
	Incar_Dev_Dsp,				//Dsp工作模式
	Incar_Dev_AudioOut,			//音频输出通道切换
	Incar_Dev_Amp,				//功放状态切换
	Incar_Dev_AudioIn,			//音频输入捕捉通道切换
	Incar_Dev_Phonate,			//车内喇叭发声请求
	Incar_Dev_Version,			//MCU版本

};
int Incar_Device_Read( int device , unsigned char*value );
int Incar_Device_Write( int device , unsigned char value );

int Incar_Can_Read( int device , unsigned char*buf , int* cnt  );
int Incar_Can_Write( int device , unsigned char* buf , int cnt );

#endif

