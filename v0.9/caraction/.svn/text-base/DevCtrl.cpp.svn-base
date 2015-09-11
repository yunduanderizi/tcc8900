#include "DevCtrl.h"
#include "i2ctl.h"
#include "NetApi.h"

#define MCUADDR 0x50


#define addr_POWER_read 			0x00
#define addr_NEC_read 				0x01
#define addr_KEY_read 				0x02
#define addr_Light_read 				0x03
#define addr_Shake_read				0x04
#define addr_Fan_read				0x07
#define addr_Lamp_read				0x08
#define addr_Speed_read				0x09
#define addr_Dsp_read				0x10
#define addr_AudioOut_read			0x12
#define addr_Amp_read				0x13
#define addr_AudioIn_read			0x15
#define addr_Version_read			0x20

#define addr_POWER_write 			0x00
#define addr_Dsp_write				0x11
#define addr_AudioOut_write			0x12
#define addr_Amp_write				0x14
#define addr_AudioIn_write			0x15
#define addr_Phonate_write			0x16


static int i2c = -1;
static void openi2c()
{
	i2c = i2copen(0);
}

int Incar_PowerOff()
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	i2cset(i2c,0x6f,0x0c,0x1f,'b');
	i2cset(i2c,0x6c,0x0c,0x1f,'b');

	return i2cset( i2c , MCUADDR , addr_POWER_write , 0x00 , 'b' );
}
int Incar_Reset()
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	return i2cset( i2c , MCUADDR , addr_POWER_write , 0x01 , 'b' );
}
/*
enum
{
	Incar_Power_Gps = 1,
	Incar_Power_Gprs,
	Incar_Power_Lcd,
	Incar_Power_Amp,
	Incar_Power_Fan,
	Incar_Power_Dvd,
};
*/
	
int Incar_PowerEnable( int device , int enable )
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	if( device >= Incar_Power_Gps && device <= Incar_Power_Dvd )
	{
		
		int value = i2cget( i2c , MCUADDR , addr_POWER_read , 'b' );
		if( enable )
			value |= 1<<(device+1);
		else
			value &= ~(1<<(device+1));
		return i2cset( i2c , MCUADDR , addr_POWER_write , value , 'b' );
	}
	else
		return -1;
}

int Incar_initvolume()
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	unsigned char aa[2];
	aa[0] = 0x0c;
	i2cset(i2c,0x44,aa,1);
	aa[0] = 0x80;
	aa[1] = 0x90;
	i2cset(i2c,0x44,aa,2);
	aa[0] = 0x40;
	aa[1] = 0x50;
	i2cset(i2c,0x44,aa,2);
	aa[0] = 0x00;
	aa[1] = 0x10;
	i2cset(i2c,0x44,aa,2);
	aa[0] = 0x20;
	aa[1] = 0x30;
	i2cset(i2c,0x44,aa,2);
	aa[0] = 0x60;
	aa[1] = 0x70;
	i2cset(i2c,0x44,aa,2);
	aa[0] = 0xa0;
	aa[1] = 0xb0;
	i2cset(i2c,0x44,aa,2);
	aa[0] = 0xd7;
	aa[1] = 0xe9;
	i2cset(i2c,0x44,aa,2);	
	aa[0] = 0xf8;
	i2cset(i2c,0x44,aa,1);
	return 0;
}

//volume·¶Î§ (  0 - 79 )
int Incar_Setvolume(int value)	
{
	unsigned char aa[2];
	aa[0] = value /10 + 0xd0;
	aa[1] = value % 10 + 0xe0 ;
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	return	i2cset(i2c,0x44,aa,2);
}

int Incar_Muteon()
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	unsigned char aa[1] ;
	aa[0] = 0xf9;
	return	i2cset(i2c,0x44,aa,1);

}
int Incar_Muteoff()
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	unsigned char aa[1] ;
	aa[0] = 0xf8;
	return	i2cset(i2c,0x44,aa,1);
}

int Incar_brightnessSet(int value)
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	int i;
	i = i2cget(i2c,0x45,0x70,'b');
	i = i | 0x40;
	i2cset(i2c,0x45,0x70,i,'b');
	i2cset(i2c,0x45,0x74,value,'b');
	i2cset(i2c,0x45,0x75,value,'b');
	i2cset(i2c,0x45,0x76,value,'b');
	return 0;
}
int Incar_brightnessGet()
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	return i2cget(i2c,0x45,0x74,'b');
}
int Incar_backclothSet(int value)
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	if(value > 0x40) value = 0x40;
	if(value < 0 ) value = 0;

	i2cset(i2c,0x45,0xc4,value,'b');
	return 0;
}
int Incar_backclothGet()
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	return i2cget(i2c,0x45,0xc4,'b')	;
}
int Incar_ContrastSet(int value)
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	int i;
	i = i2cget(i2c,0x45,0x70,'b');
	i = i | 0x40;
	i2cset(i2c,0x45,0x70,i,'b');
	i2cset(i2c,0x45,0x71,value,'b');
	i2cset(i2c,0x45,0x72,value,'b');
	i2cset(i2c,0x45,0x73,value,'b');
	return 0;
}

int Incar_ContrastGet()
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	return i2cget(i2c,0x45,0x71,'b');	
}

int Incar_AirconditionGet(structAirInfo*test)
{
	int error = 0;
	unsigned char buf[6];
	
	if( !test )
		return -1;
	
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			error = -2;
	}

	if( !error )
	{
		buf[0] = 0x0a;
		int try1 = 2;
		while(try1 --)
		{
			i2cset(i2c,0x50,buf,1);
			i2cget(i2c,0x50,buf,6);
			int i;
			i = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] ;
			i = i & 0x7f;
			if(((buf[5]>>7)==0) && (i==0x7a)) 
				break;
			
		}
		if( try1 <= 0 ) 
			error = -3;
	}
	
	if( !error )
	{
		test->fanspeed = buf[0] & 0x07;
		test->B13 = (buf[0] & 0x08)>>3;
		test->B14 = (buf[0] & 0x10)>>4;
		test->B15 = (buf[0] & 0x20)>>5;
		test->B16 = (buf[0] & 0x40)>>6;
		test->B20 = (buf[1] & 0x01);
		test->B21 = (buf[1] & 0x02)>>1;
		test->B22 = (buf[1] & 0x04)>>2;
		test->B23 = (buf[1] & 0X08)>>3;
		test->B24 = (buf[1] & 0X10)>>4;
		test->B25 = (buf[1] & 0x20)>>5;
		test->B26 = (buf[1] & 0x40)>>6;
		test->B30 = (buf[2] & 0x01);
		test->B31 = (buf[2] & 0x02)>>1;
		test->B32 = (buf[2] & 0x04)>>2;
		test->TenTem = (buf[2] & 0x78)>>3;
		test->OneTem = (buf[3] & 0x78)>>3;
		test->OutTem = (buf[4] & 0x7f) - 40;
	}

	if( error )
		TraceMsg( "Incar_AirconditionGet fail %d\n" , error );
	return error;
}
/*
enum
{
	Incar_Dev_Light = 1,
	Incar_Dev_Shake,
	Incar_Dev_Tem,
	Incar_Dev_Dsp,
	Incar_Dev_AudioOut,
	Incar_Dev_Amp,
	Incar_Dev_AudioIn,

	Incar_Dev_Version,

};
*/
int Incar_Device_Read( int device , unsigned char*value )
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	if( device >= Incar_Dev_Light && device <= Incar_Dev_Version )
	{
		switch(device)
		{
			case Incar_Dev_Light: 
				*value = i2cget(i2c,MCUADDR,addr_Light_read,'b'); 
				break;
			case Incar_Dev_Shake: 
				*value = i2cget(i2c,MCUADDR,addr_Shake_read,'b'); 
				break;
			case Incar_Dev_Fan:
				*value = i2cget(i2c,MCUADDR,addr_Fan_read,'b');
				break;
			case Incar_Dev_Lamp:
				*value = i2cget(i2c,MCUADDR,addr_Lamp_read,'b');
				break;
			case Incar_Dev_Speed:
				*value = i2cget(i2c,MCUADDR,addr_Speed_read,'b');
				break;
			case Incar_Dev_Dsp:
				*value = i2cget(i2c,MCUADDR,addr_Dsp_read,'b'); 
				break;
			case Incar_Dev_AudioOut:
				*value = i2cget(i2c,MCUADDR,addr_AudioOut_read,'b');
				break;
			case Incar_Dev_Amp:
				*value = i2cget(i2c,MCUADDR,addr_Amp_read,'b');
				break;
			case Incar_Dev_AudioIn:
				*value = i2cget(i2c,MCUADDR,addr_AudioIn_read,'b');
				break;
			case Incar_Dev_Version:
				*value = i2cget(i2c,MCUADDR,addr_Version_read,'b');
				break;
		}
	}
	return *value;
}
int Incar_Device_Write( int device , unsigned char value )
{
	if( i2c < 0 )
	{
		openi2c();
		if( i2c < 0 )
			return -1;
	}
	if( device >= Incar_Dev_Light && device <= Incar_Dev_Version )
	{
		switch(device)
		{
			case Incar_Dev_Dsp:
				return i2cset(i2c,MCUADDR,addr_Dsp_write,value,'b'); 
			case Incar_Dev_AudioOut:
				return i2cset(i2c,MCUADDR,addr_AudioOut_write,value,'b');
			case Incar_Dev_Amp:
				return i2cset(i2c,MCUADDR,addr_Amp_write,value,'b');
			case Incar_Dev_AudioIn:
				return i2cset(i2c,MCUADDR,addr_AudioIn_write,value,'b');
			case Incar_Dev_Phonate:
				return i2cset(i2c,MCUADDR,addr_Phonate_write,value,'b');
		}
	}
	return 0;
}
int Incar_Can_Read( int device , unsigned char*buf , int* cnt  );
int Incar_Can_Write( int device , unsigned char* buf , int cnt );


