/****************************************************************************
 *   FileName    : ioctl_ckcstr.h
 *   Description : 
 ****************************************************************************
*
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
*
 ****************************************************************************/


/************************************************************************************************
*                                    Revision History                                           *
*                                                                                               *
* Version : 1.0    : 2009, 2, 04                                                                *
************************************************************************************************/

#ifndef __IOCTL_STR_H__
#define __IOCTL_STR_H__

// For CKC Controller
typedef struct _stckcioctl{
	unsigned int ioctlcode;
	//Reset or bus Enable name
	unsigned int prbname; 
	//Peri Clock
	unsigned int pckcname; 
	unsigned int pckcenable; 
	unsigned int pckcsource; 
	unsigned int pckcfreq;
	//PLL Cllock
	unsigned int pllchannel;
	unsigned int pllvalue;
	unsigned int P;
	unsigned int M;
	unsigned int S;
	//CPU Cllock
	unsigned int cpuvalue; 
	//BUS Cllock
	unsigned int busvalue;
	//mode 
	unsigned int mode; // Enable, Disable, ahalf, athird

	unsigned int priority;

	unsigned int cpudivider;
	unsigned int pmuoffname;
	
	unsigned int bspmax;	
	//Fbus Clock
	unsigned int fbusname;
	unsigned int fbusenable; 
	unsigned int fbussource; 
	unsigned int fbusfreq;
	
	//DDI PWDN
	unsigned int ddipdname;

	//ETC Block
	unsigned int etcblock;

	//Dynamic Min/Max
	unsigned int dynamicmax;
	unsigned int dynamicmin;
	unsigned int dynamiccycle;

	// Video BUS CFG
	unsigned int videobuscfgname;
	
}stckcioctl;


typedef struct _stckcinfo{
	unsigned int currentbusfreq;
	unsigned int currentsysfreq;
	unsigned int currentcpufreq;
	int pckcfreq; //return etc frequency
	unsigned int validpll[30];
	int retVal;
	unsigned int currentpriority;

	unsigned int state;

	int fbusfreq;
	
}stckcinfo;

#endif /* __IOCTL_STR_H__ */
