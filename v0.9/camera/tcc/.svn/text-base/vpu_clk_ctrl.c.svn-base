/*!
 ***********************************************************************
 \par Copyright
 \verbatim
  ________  _____           _____   _____           ____  ____   ____		
     /     /       /       /       /       /     /   /    /   \ /			
    /     /___    /       /___    /       /____ /   /    /____/ \___			
   /     /       /       /       /       /     /   /    /           \		
  /     /_____  /_____  /_____  /_____  /     / _ /_  _/_      _____/ 		
   																				
  Copyright (c) 2009 Telechips Inc.
  Korad Bldg, 1000-12 Daechi-dong, Kangnam-Ku, Seoul, Korea					
 \endverbatim
 ***********************************************************************
 */
/*!
 ***********************************************************************
 *
 * \file
 *		vpu_clk_ctrl.c
 * \date
 *		2009/10/09
 * \author
 *		AV algorithm group(AValgorithm@telechips.com) 
 * \brief
 *		vpu clock and mem. region control
 * \version
 *		0.0.1 : 2009/10/09	(2009/10/01 LINUX BSP)
 *
 ***********************************************************************
 */

#include "sys_memory.h"
#ifdef VPU_CLK_CONTROL
#include "vpu_clk_ctrl.h"

#ifdef SYS_LINUX
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>

#include <termios.h>
#include <math.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <ctype.h>

#define VPU_CKC_DEV_NAME	"/dev/tcc-ioctl-ckc"

#define FBUS_STEP_NUM 34
#define MAJOR_NUM 247
#define IOCTL_CKC_GET_FBUS          _IO(MAJOR_NUM, 12)
#define IOCTL_CKC_SET_CHANGEFBUS    _IO(MAJOR_NUM, 17)

typedef enum {
		FCORE_CPU = 0,
		FBUS_GRP,
		FBUS_VBUS,
		FBUS_VCODEC,
		FBUS_DDI,
		FBUS_SMU,
		FBUS_IOB,
		FMEM_BUS,
};

enum {
		CLKCTRL0 = 0,	//FCORE_CPU
		CLKCTRL1,		//FBUS_DDI
		CLKCTRL2,		//FMEM_BUS
		CLKCTRL3,		//FBUS_GRP
		CLKCTRL4,		//FBUS_IOB
		CLKCTRL5,		//FBUS_VBUS
		CLKCTRL6,		//FBUS_VCODEC
		CLKCTRL7,		//FBUS_SMU
};

typedef enum {
	FBUS_NULL = 0,
	FBUS_CKC_2640000,
	FBUS_CKC_2340000,
	FBUS_CKC_1760000,
	FBUS_CKC_1560000,
	FBUS_CKC_1320000,
	FBUS_CKC_1170000,
	FBUS_CKC_1056000,
	FBUS_CKC_936000,
	FBUS_CKC_880000, 
	FBUS_CKC_780000,
	FBUS_CKC_754285,
	FBUS_CKC_668571,
	FBUS_CKC_660000,
	FBUS_CKC_586666,
	FBUS_CKC_585000,
	FBUS_CKC_528000,
	FBUS_CKC_520000,
	FBUS_CKC_480000, 
	FBUS_CKC_468000,
	FBUS_CKC_440000,
	FBUS_CKC_425454,
	FBUS_CKC_406153,
	FBUS_CKC_390000,
	FBUS_CKC_377142,
	FBUS_CKC_360000,
	FBUS_CKC_352000,
	FBUS_CKC_334285,
	FBUS_CKC_330000,
	FBUS_CKC_312000,
	FBUS_CKC_292500,
	FBUS_CKC_60000,
	FBUS_CKC_30000,
	FBUS_CKC_10000,
	FBUS_CKC_0,
};


// For CKC Controller
typedef struct vpu_stckcioctl_t
{
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
	
} vpu_stckcioctl_t;

typedef struct vpu_stckcinfo_t
{
	unsigned int currentbusfreq;
	unsigned int currentsysfreq;
	unsigned int currentcpufreq;
	int pckcfreq; //return etc frequency
	unsigned int validpll[30];
	int retVal;
	unsigned int currentpriority;
	unsigned int state;
	int fbusfreq;	
} vpu_stckcinfo_t;

struct vpu_ckc_ioctl_t{
	vpu_stckcioctl_t  in_ckc;
	vpu_stckcinfo_t   out_ckc;
} vpu_ckc_ioctl_t;

static int 
vpu_fbus_ckc_set(char *fbus, char mode)
{
    int file_desc, ret;
    struct vpu_ckc_ioctl_t st;
    int tmp, ckc_src, flag;
	unsigned int FbusItemValue[FBUS_STEP_NUM] = {
		2640000, 2340000, 1760000, 1560000, 1320000, 1170000, 1056000, 936000, 880000, 
		780000, 754285, 668571, 660000, 586666, 585000, 528000, 520000, 480000, 
		468000, 440000, 425454, 406153, 390000, 377142, 360000, 352000, 334285,
		330000, 312000, 292500, 60000, 30000, 10000, 0,
	};


    printf("fbus_ckc_set %d\n",mode);

    tmp = *fbus;
    if(tmp < 1 || tmp > FBUS_STEP_NUM){
        printf("Enter fbus value (1 ~ %d)\n", FBUS_STEP_NUM);
        return -1;
    }

    ckc_src = FbusItemValue[tmp-1];
    printf("ckc_src : %d\n", ckc_src);
    file_desc = open(VPU_CKC_DEV_NAME, 0);
    if(file_desc < 0)
    {
        printf("Can't open device file : %s\n", VPU_CKC_DEV_NAME);
        return -1;
    }

    memset(&st, 0, sizeof(vpu_ckc_ioctl_t));


	if ( mode == FBUS_VBUS ) {
        st.in_ckc.fbusname = CLKCTRL5; // FVBUS
    } else if ( mode == FBUS_VCODEC ) {
        st.in_ckc.fbusname = CLKCTRL6; // FVCOD
    }

    ret = ioctl(file_desc, IOCTL_CKC_GET_FBUS, &st);

    if(ret < 0) {
        printf("ioctl error\n");
        return -1;
    }
    printf("Before PeriFreq(FBUS) : %d\n", st.out_ckc.fbusfreq);


    for (tmp = 0; tmp < FBUS_STEP_NUM; tmp++)  
    {
        if( ckc_src == FbusItemValue[tmp] )
		{
            flag = 1;  
            break;  
        }  
    }  

    //printf("IOCTL SET [ON] : FBUS\n");

    if(flag == 1) {
        st.in_ckc.fbusfreq          = ckc_src; 
    }

    ret = ioctl(file_desc, IOCTL_CKC_SET_CHANGEFBUS, &st);
    if(ret < 0)
    {
        printf ("ioctl_set_msg failed: %d\n", ret);
        return -1;
    }

    /* AFTER SET: IOCTL GET TEST */
    //printf("IOCTL GET : FBUS\n");
    ret = ioctl(file_desc, IOCTL_CKC_GET_FBUS, &st);

    if(ret < 0) {
        printf("ioctl error\n");
        return -1;
    }
    printf("After PeriFreq(FBUS) : %d\n", st.out_ckc.fbusfreq);

    close(file_desc);

	return 0;
}


int 
vpu_clock_init(void)
{
	int  fbusvalue;
	int iRet;
	
	fbusvalue = FBUS_CKC_2640000;
	iRet = vpu_fbus_ckc_set(&fbusvalue,FBUS_VBUS);
	if( iRet )
	{
		return -1;
	}

	fbusvalue = FBUS_CKC_2640000;
	iRet = vpu_fbus_ckc_set(&fbusvalue,FBUS_VCODEC); 
	if( iRet )
	{
		return -1;
	}
	
	return 0;
}

int 
vpu_clock_deinit(void)
{
	int  fbusvalue = FBUS_CKC_60000;

	vpu_fbus_ckc_set(&fbusvalue,FBUS_VBUS);
	vpu_fbus_ckc_set(&fbusvalue,FBUS_VCODEC);

	return 0;
}

#endif //ARM_LINUX
#ifdef SYS_WINCE
#include "../system_wince/ioctl_code.h"
#include "../system_wince/globals.h"
#include "../system_wince/ioctl_ckcstr.h"
#include "../system_wince/Clock.H"
#include "../system_wince/windev.h"
#include <WinIoCtl.h>

#define __TCC89XX_WINCE__

int TWS_CKC_GetBUS(int pckname,int *pckcfreq)
{     
#ifdef __TCC89XX_WINCE__
	int retVal = -1;
	stckcioctl pCKCIOCTL;
	stckcinfo pCKCINFO;
	unsigned long returnedbyte;

	pCKCIOCTL.ioctlcode = IOCTL_CKC_GET_CLOCKINFO;

	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);
	printf("======================================\n");
	printf("[TWS_CKC_GetBUS] PLL: %10d\n", pCKCINFO.currentsysfreq);
	printf("[TWS_CKC_GetBUS] CPU: %10d\n",pCKCINFO.currentcpufreq);
	printf("[TWS_CKC_GetBUS] MEM: %10d\n",pCKCINFO.currentbusfreq);
	printf("======================================\n");

	if(pckcfreq!=NULL)
	{
		*pckcfreq = pCKCINFO.currentbusfreq;
	}

	return pCKCINFO.currentbusfreq;
#else
	return 0;
#endif
}


void TWS_CKC_SetFCPU(int pckcfreq)
{
#ifdef __TCC89XX_WINCE__
	int retVal = -1;
	stckcioctl pCKCIOCTL;
	stckcinfo pCKCINFO;
	unsigned long	returnedbyte;

	pCKCIOCTL.ioctlcode = IOCTL_CKC_SET_CHANGECPU;
	pCKCIOCTL.cpuvalue = pckcfreq;

	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);

	TWS_CKC_GetBUS(NULL, NULL);
#endif
}

void TWS_CKC_SetFMBUS(int pckcfreq)
{
#ifdef __TCC89XX_WINCE__
	int retVal = -1;
	stckcioctl pCKCIOCTL;
	stckcinfo pCKCINFO;
	unsigned long	returnedbyte;

	pCKCIOCTL.ioctlcode = IOCTL_CKC_SET_CHANGEMEM;
	pCKCIOCTL.busvalue = pckcfreq;

	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);

	TWS_CKC_GetBUS(NULL, NULL);
#endif
}

void TWS_CKC_SetFBUS(int pckname,int pckcenable,int mode, int pckcfreq, int source)
{           
#ifdef __TCC89XX_WINCE__
	int retVal = -1;
	stckcioctl    pCKCIOCTL;
	stckcinfo	 pCKCINFO;
	unsigned long returnedbyte;
	int ret;

	pCKCIOCTL.ioctlcode	=  IOCTL_CKC_SET_CHANGEFBUS;
	pCKCIOCTL.fbusname	= pckname;
	pCKCIOCTL.fbusfreq	= pckcfreq;

	ret = KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);

	// check a frequency	
	printf("[TWS_CKC_SetFBUS] %d, %10d\n", pckname, TWS_CKC_GetFBUS(pckname));

	// check a relative PMU status
	switch(pckname)
	{
	case CLKCTRL3:
		{
			pCKCIOCTL.ioctlcode = IOCTL_CKC_GET_PMUPOWER;
			pCKCIOCTL.pmuoffname = PMU_GRAPHICBUS;
			KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);
			if(pCKCINFO.state == 1) // 1 : Disable, 0: Enable
				printf("[TWS_CKC_SetFBUS]PMU_GRAPHICBUS:    Disable\n");
			else
				printf("[TWS_CKC_SetFBUS]PMU_GRAPHICBUS:    Enable\n");
		}
		break;

	case CLKCTRL5:
		{
			pCKCIOCTL.ioctlcode = IOCTL_CKC_GET_PMUPOWER;
			pCKCIOCTL.pmuoffname = PMU_VIDEOBUS;
			KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);
			if(pCKCINFO.state == 1) // 1 : Disable, 0: Enable
				printf("[TWS_CKC_SetFBUS]PMU_VIDEOBUS:    Disable\n");
			else
				printf("[TWS_CKC_SetFBUS]PMU_VIDEOBUS:    Enable\n");
		}
		break;
	}
#endif
}

int TWS_CKC_GetFBUS(int pckname)
{                     
#ifdef __TCC89XX_WINCE__
	int retVal = -1;   
	stckcioctl pCKCIOCTL;
	stckcinfo pCKCINFO;
	unsigned long returnedbyte;

	pCKCIOCTL.ioctlcode =  IOCTL_CKC_GET_FBUS;  
	pCKCIOCTL.fbusname = pckname;

	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);

	return pCKCINFO.fbusfreq;
#else
	return 0;
#endif
}

void TWS_CKC_SetFBUSForVPUOn(void)
{
#ifdef __TCC89XX_WINCE__
	if(TWS_CKC_GetBUS(NULL,NULL) >= 3300000 )
	{
		// Set Clock for Full HD
		printf("[TWS_CKC_SetFBUSForVPUOn]Set Fbus for VPU Full HD\n");
		//TWS_CKC_SetFBUS(CLKCTRL3, ENABLE, NORMAL_MD, 2640000, DIRECTPLL3); // FBUS_GRP
		TWS_CKC_SetFBUS(CLKCTRL5, ENABLE, NORMAL_MD, 2640000, DIRECTPLL1); // FBUS_VBUS
		TWS_CKC_SetFBUS(CLKCTRL6, ENABLE, NORMAL_MD, 2340000, DIRECTPLL3); // FBUS_VCODEC
	}
	else
	{
		// Set Clock for HD
		printf("[TWS_CKC_SetFBUSForVPUOn]Set Fbus for VPU HD\n");
		//TWS_CKC_SetFBUS(CLKCTRL3, ENABLE, NORMAL_MD, 1760000, DIRECTPLL3); // FBUS_GRP
		TWS_CKC_SetFBUS(CLKCTRL5, ENABLE, NORMAL_MD, 1760000, DIRECTPLL3); // FBUS_VBUS
		TWS_CKC_SetFBUS(CLKCTRL6, ENABLE, NORMAL_MD, 1560000, DIRECTPLL2); // FBUS_VCODEC
	}
#endif
}

void TWS_CKC_SetFBUSForVPUOff(void)
{
#ifdef __TCC89XX_WINCE__
	printf("[TWS_CKC_SetFBUSForVPUOn]Clear Fbus for VPU\n");
	TWS_CKC_SetFBUS(CLKCTRL5, ENABLE, NORMAL_MD, 0, DIRECTXIN); // FBUS_VBUS
	TWS_CKC_SetFBUS(CLKCTRL6, ENABLE, NORMAL_MD, 0, DIRECTXIN); // FBUS_VCODEC
#endif
}
void TWS_CKC_SetFBUSForVPU(int VBUS, int VCODEC)
{
#ifdef __TCC89XX_WINCE__
	// Set Clock for HD
	printf("[TWS_CKC_SetFBUSForVPU]Set Fbus for VPU\n");
	TWS_CKC_SetFBUS(CLKCTRL5, ENABLE, NORMAL_MD, VBUS, DIRECTPLL3); // FBUS_VBUS
	TWS_CKC_SetFBUS(CLKCTRL6, ENABLE, NORMAL_MD, VCODEC, DIRECTPLL2); // FBUS_VCODEC
#endif
}

void TWS_CKC_SetFBUSForGRPOn(void)
{
#ifdef __TCC89XX_WINCE__
	if(TWS_CKC_GetBUS(NULL,NULL) >= 3300000 )
	{
		// Set Clock for Full HD
		printf("[TWS_CKC_SetFBUSForGRPOn]Set FBUS_GRP for Full HD\n");
		TWS_CKC_SetFBUS(CLKCTRL3, ENABLE, NORMAL_MD, 2640000, DIRECTPLL3); // FBUS_GRP
	}
	else
	{
		// Set Clock for HD
		printf("[TWS_CKC_SetFBUSForGRPOn]Set FBUS_GRP for HD\n");
		TWS_CKC_SetFBUS(CLKCTRL3, ENABLE, NORMAL_MD, 1760000, DIRECTPLL3); // FBUS_GRP
	}
	//Set Overlay Mixer ON, 3D block OFF
	TWS_CKC_SetOverlayMix(1);
	TWS_CKC_Set3DGPU(0);
#endif
}

void TWS_CKC_SetFBUSForGRPOff(void)
{
#ifdef __TCC89XX_WINCE__
	printf("[TWS_CKC_SetFBUSForGRPOff]Clear Fbus for GRP\n");
	TWS_CKC_SetOverlayMix(0);//Set Overlay Mixer OFF
	TWS_CKC_SetFBUS(CLKCTRL3, ENABLE, NORMAL_MD, 0, DIRECTXIN); // FBUS_GRP
#endif
}

void TWS_CKC_SetDDIBUS(int ddibusname, unsigned char mode)
{
#ifdef __TCC89XX_WINCE__
	int retVal = -1;
	stckcioctl pCKCIOCTL;
	stckcinfo pCKCINFO;
	unsigned long	returnedbyte;

	if((ddibusname < DDIPWDN_CIF) || (ddibusname >= DDIPWDN_STEPMAX))
	{
		printf("[TWS_CKC_SetDDIBUS] Invalid DDI BUS : %d\n", ddibusname);
		return;
	}

	/* mode : 0(ddi bus disable), 1(ddi bus enable) */
	pCKCIOCTL.ioctlcode = IOCTL_CKC_SET_DDIPWDN;
	pCKCIOCTL.ddipdname = ddibusname;	
	pCKCIOCTL.mode = (mode>0)?1:0;

	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);

	pCKCIOCTL.ioctlcode = IOCTL_CKC_GET_DDIPWDN;	
	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);
	if(pCKCINFO.retVal == 0) // Enable
		printf("[TWS_CKC_SetDDIBUS]DDIBUS  : %d - Enable\n", pCKCIOCTL.ddipdname);
	else  // Disable 
		printf("[TWS_CKC_SetDDIBUS]DDIBUS  : %d - Disable\n", pCKCIOCTL.ddipdname);
#endif
}

void TWS_CKC_SetIOBUS(int iobusname, unsigned char mode)
{
#ifdef __TCC89XX_WINCE__
	int retVal = -1;
	stckcioctl pCKCIOCTL;
	stckcinfo pCKCINFO;
	unsigned long	returnedbyte;

	if((iobusname < RB_USB11H) || (iobusname >= RB_ALLPERIPERALS))
	{
		printf("[TWS_CKC_SetIOBUS] Invalid IO BUS : %d\n", iobusname);
		return;
	}

	/* mode : 0(io bus disable), 1(io bus enable) */
	pCKCIOCTL.ioctlcode = IOCTL_CKC_SET_PERIBUS;
	pCKCIOCTL.prbname = iobusname;
	pCKCIOCTL.mode = (mode>0)?1:0;

	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);

	pCKCIOCTL.ioctlcode = IOCTL_CKC_GET_PERIBUS;
	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);
	if(pCKCINFO.retVal == 0)
		printf("[TWS_CKC_SetIOBUS]IOBUS: %d - Disable\n", pCKCIOCTL.prbname);
	else 
		printf("[TWS_CKC_SetIOBUS]IOBUS: %d - Enable\n", pCKCIOCTL.prbname);
#endif
}

void TWS_CKC_SetETC(unsigned char mode)
{
#ifdef __TCC89XX_WINCE__
	int retVal = -1;
	stckcioctl pCKCIOCTL;
	stckcinfo pCKCINFO;
	unsigned long	returnedbyte;

	pCKCIOCTL.ioctlcode = IOCTL_CKC_SET_ETCBLOCK;
	pCKCIOCTL.etcblock = (mode>0)?ETCMASK_USBPHYON:ETCMASK_USBPHYOFF;

	printf("[TWS_CKC_SetETC]Set pCKCIOCTL.etcblock(0x1 or 0x2) : %d\n", pCKCIOCTL.etcblock);

	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);
#endif
}

void TWS_CKC_SetOverlayMix(unsigned char mode)
{
#ifdef __TCC89XX_WINCE__
	int retVal = -1;
	stckcioctl pCKCIOCTL;
	stckcinfo pCKCINFO;
	unsigned long	returnedbyte;

	pCKCIOCTL.ioctlcode = IOCTL_CKC_SET_ETCBLOCK;
	pCKCIOCTL.etcblock = (mode>0)?ETCMASK_OVERLAYMIXERON:ETCMASK_OVERLAYMIXEROFF;

	printf("[TWS_CKC_SetOverlayMix]Set pCKCIOCTL.etcblock(0x1 or 0x2) : %d\n", pCKCIOCTL.etcblock);

	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);
#endif
}

void TWS_CKC_Set3DGPU(unsigned char mode)
{
#ifdef __TCC89XX_WINCE__
	int retVal = -1;
	stckcioctl pCKCIOCTL;
	stckcinfo pCKCINFO;
	unsigned long	returnedbyte;

	pCKCIOCTL.ioctlcode = IOCTL_CKC_SET_ETCBLOCK;
	pCKCIOCTL.etcblock = (mode>0)?ETCMASK_3DGPUON:ETCMASK_3DGPUOFF;

	printf("[TWS_CKC_Set3DGPU]Set pCKCIOCTL.etcblock(0x1 or 0x2) : %d\n", pCKCIOCTL.etcblock);

	KernelIoControl(IOCTL_HAL_TCCCKC, &pCKCIOCTL, sizeof(stckcioctl), &pCKCINFO, sizeof(stckcinfo), &returnedbyte);
#endif
}


int 
vpu_clock_init(void)
{
	TWS_CKC_SetFBUSForVPUOn();
	return 0;
}

int 
vpu_clock_deinit(void)
{
	//TWS_CKC_SetFBUSForVPUOff();
	TWS_CKC_SetFBUSForVPU(60000, 60000);
	return 0;
}



#undef __TCC89XX_WINCE__
#endif//ARM_WINCE

#endif//#ifdef VPU_CLK_CONTROL
