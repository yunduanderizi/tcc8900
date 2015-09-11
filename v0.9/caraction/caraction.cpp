#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <i2ctl.h>
#include <stdarg.h> 
#include <tinyxml.h>
#include <string>
#include <termios.h>
#include <sys/mman.h>
#include "DevCtrl.h"

#ifdef _8925_VERSION
#define GPA_BASE                        0x74200000
#else
#define GPA_BASE                        0xF0102000
#endif
#define GPA_DAT				0x00
#define GPA_EN				0x04
#define OFFSET_FUN0                     0x24
#define OFFSET_FUN1                     0x28
#define OFFSET_FUN2                     0x2c
#define OFFSET_FUN3                     0x30

static volatile int ThreadCommand = 0;
char *gpa_vbase;
int fd = -1;  
int isheadset = 0;


int open_serialport(char *dev);
int at_command(int fd, char *cmd, int to);
int InitGF();
int MuteGF();
int UnmuteGF();
int DetectGF();
int PowerOffGF();

int headinit();
int headclose();

int headclose()
{
	close(fd);
 	munmap(gpa_vbase, OFFSET_FUN3);
	return 0;
}
int headinit()
{
       
        fd = open("/dev/mem",O_RDWR|O_SYNC);
        if(fd < 0){
                fprintf(stderr,"mem:open()\n");
                return -1;
        }
        gpa_vbase = (char *) mmap( NULL, OFFSET_FUN3,  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, fd, GPA_BASE);
	if(gpa_vbase == NULL || gpa_vbase ==  MAP_FAILED ){
		fprintf(stderr,"mmap:map file error \n");
		return -1;
	}
#ifdef _8925_VERSION
	gpa_vbase += 0x100;
	*(volatile unsigned int*)(gpa_vbase + OFFSET_FUN2) &= ~(0xf); 
	*(volatile unsigned int*)(gpa_vbase + GPA_EN) &= ~(0x1 << 16);
#else
	*(volatile unsigned int*)(gpa_vbase + OFFSET_FUN1) &= ~(0xf << 12); 
	*(volatile unsigned int*)(gpa_vbase + GPA_EN) &= ~(0x1 << 11);
#endif
			
	
        return 0;
}

void * WatchGFThread(void*arg)
{	
	int i = 0;
	while( ThreadCommand == 0 )
	{
#ifdef _8925_VERSION
		i  = *(volatile unsigned int*)(gpa_vbase + GPA_DAT) & (0x1 << 16); 
#else
		i  = *(volatile unsigned int*)(gpa_vbase + GPA_DAT) & (0x1 << 11); 
#endif
		if(i <= 0){
			fprintf(stderr,"MuteGF:headset has inserted !\n");
			MuteGF();
		}else{
			UnmuteGF();
		}
		//sleep(2); /*靠靠靠靠*/
#ifdef _8925_VERSION
		usleep( 500000 );// 1 ms
#else
		usleep( 1000 );// 1 ms
#endif
		DetectGF();
	}
	pthread_detach (pthread_self());
	return 0;
}

int main(int argc, char* argv[])
{
	
#ifndef _8925_VERSION
	headinit();
#endif
	if( argc == 1 )
	{
		//改在启动脚本初始化功放，此处不考虑初始化功放
//		InitGF();
//		UnmuteGF();

		//启动功放过载保护		
		pthread_t idGF = 0;
		pthread_create( &idGF, 0, WatchGFThread, 0 );
		while( 1 )
		{
			sleep(10);
		}
	}
	else if( argc == 2 )
	{
		if( strcmp( argv[1], "opengf" ) == 0  )
		{
			InitGF();
		}
		else if( strcmp( argv[1], "mutegf" ) == 0  )
		{
			MuteGF();
		}
		else if( strcmp( argv[1], "unmutegf" ) == 0  )
		{
			UnmuteGF();
		}
		else if( strcmp( argv[1], "poweroffgf" ) == 0  )
		{
			PowerOffGF();
		}
		else if( strcmp( argv[1], "revivegf" ) == 0  )
		{
			DetectGF();
		}
		else if( strcmp( argv[1], "--help" ) == 0  )
			printf("using as:\n\tcaraction opengf\n\tcaraction mutegf\n\tcaraction unmutegf\n\tcaraction poweroffgf\n\tcaraction revivegf\n");
	}
END:	
	headclose();
	return 0;

}


unsigned char tas5414_master[] =			// iic address: 0x6c
{
	0X08,0X55,            //gain choose.0x00-12db,0x55-20db,0xaa-26db,0xff-32db
	0X09,0X00,            //dc offset reduction and current limit
	0X0A,0X0D,            //switch frequency choose
	0X0B,0X40,            //master and slaver choose
	0X0D,0X00,            //output control
	0X0E,0X8E,            //positive dc detect threshold selection
	0X0F,0X3D,            //negative dc detect threshold selection
};

int InitGF()
{
	int file,i;
	file = i2copen(0);
	if( file == 0 )
		return -1;

	Incar_initvolume();

	//把声音设置为工作状态
	i2cset( file, 0x50, 0x00, 0xFF, 'b' );
	//必须进行延时，等待功放稳定，否则配置无效
	usleep( 200000 );

	//必须读0x00 和0x01地址，清除失败状态
	i2cget(file,0x6c,0x00,'b');
	i2cget(file,0x6c,0x01,'b');

	//以下功能检查是否输出短路
	i2cset( file, 0x50, 0x0b, 0x0F, 'b' );
	usleep( 200000 );//等 200 ms

	int nCount = 10;
	int nState = i2cget(file,0x6c,0x07,'b');
	while( (nState != 0x00) && (nCount-- > 0))
	{
		usleep( 35000 );//等 35 ms
		nState = i2cget(file,0x6c,0x07,'b');
		fprintf(stderr, "InitGF nState is %x, nCount is %d\n", nState, nCount);
	}

	int nState1 = i2cget(file,0x6c,0x02,'b');
	int nState2 = i2cget(file,0x6c,0x03,'b');
	if( nState1 != 0x00 || nState2 != 0x00 )
	{
		printf("tas5414_master 0x02 0x03 address is %x %x\n", nState1, nState2);
		return -2;
	}

	//写用户配置参数
	unsigned char* p = tas5414_master;
	for(i = 0;i < (int)(sizeof(tas5414_master)/sizeof(tas5414_master[0]));i+=2)
	{
		i2cset(file,0x6c,*p,*(p+1),'b');
		p += 2;
	}

 	i2cclose(file);

	usleep( 3000000 );//必须进行延时，等待功放稳定，否则立即切换界面不能屏蔽电流冲击声
	return 0;
}

int MuteGF()
{
	int file;
	file = i2copen(0);	
	if( file == 0 )
		return -1;

	i2cset(file,0x6c,0x0c,0x1f,'b');
	
	i2cclose(file);

	return 0;
}	

int UnmuteGF()
{
	int file;
	file = i2copen(0);	
	if( file == 0 )
		return -1;

	i2cset(file,0x6c,0x0c,0x00,'b');
	usleep( 35000 );//等 35 ms
	int nState = i2cget(file,0x6c,0x06,'b');
	int nCount = 3;
	while( (nState != 0x0f)  && (nCount-- > 0) )
	{
		usleep( 35000 );//等 35 ms
		nState = i2cget(file,0x6c,0x06,'b');
		fprintf(stderr, "UnmuteGF nState is %x, nCount is %d\n", nState, nCount);
	}

	i2cclose(file);
	return 0;
}	

int DetectGF()
{
	int file;
	file = i2copen(0);
	if(file == 0)
		return -1;

	int nState1 = i2cget(file,0x6c,0x00,'b');
	int nState2 = i2cget(file,0x6c,0x01,'b');
	if( nState1 != 0x00 || nState2 != 0x00 )
	{
		UnmuteGF();
	}
	i2cclose(file);
	return 0;
}

int PowerOffGF()
{
	int file;
	file = i2copen(0);	
	if( file == 0 )
		return -1;

	//由PLAY-MODE进入到MUTE-MODE
	i2cset(file,0x6c,0x0c,0x10,'b');
	int nCount = 10;
	int nState = i2cget(file,0x6c,0x06,'b');
	while( (nState != 0xf0) && (nCount-- > 0) )
	{
		usleep( 35000 );//等 35 ms
		nState = i2cget(file,0x6c,0x06,'b');
		fprintf(stderr, "PowerOffGF MUTE-MODE i2cget 0x6c,0x06 nState is %x, nCount is %d\n", nState, nCount);
	}

	//由MUTE-MODE 进入到LOW-LOW-MODE
	i2cset(file,0x6c,0x0d,0x0f,'b');
	nCount = 10;
	nState = i2cget(file,0x6c,0x05,'b');
	while( (nState != 0xf0) && (nCount-- > 0) )
	{
		usleep( 35000 );//等 35 ms
		nState = i2cget(file,0x6c,0x05,'b');
		fprintf(stderr, "PowerOffGF LOW-LOW-MODE i2cget 0x6c,0x05 nState is %x, nCount is %d\n", nState, nCount);
	}

	//由LOW-LOW-MODE 进入到HIZ-MODE
	i2cset(file,0x6c,0x0c,0x1f,'b');
	nCount = 10;
	nState = i2cget(file,0x6c,0x05,'b');
	while( (nState != 0x0f) && (nCount-- > 0) )
	{
		usleep( 35000 );//等 35 ms
		nState = i2cget(file,0x6c,0x05,'b');
		fprintf(stderr, "PowerOffGF HIZ-MODE i2cget 0x6c,0x05 nState is %x, nCount is %d\n", nState, nCount);
	}
	
	i2cclose(file);

	return 0;
}
