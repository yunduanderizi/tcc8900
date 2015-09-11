
/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : Image.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
/*****************************************************************************
*
* Header Files Include
*
******************************************************************************/
#include "Virtual.h"
#include "TCCUtil.h"
#include "M2Mscaler_Video.h"
//#include <lcd_resolution.h>
#include "tccfb_ioctl_code.h"
#include <linux/fb.h> 
#define USE_M2M_INTRRUPT
#ifdef USE_M2M_INTRRUPT
#include <fcntl.h>         // O_RDWR
#include <sys/poll.h>
#ifdef TCC_79XX_INCLUDE
#define TCC_INTR_SC_DEV_NAME	"/dev/intr-sc"
#endif
#ifdef TCC_89XX_INCLUDE
#include "TCC89x_Physical.h"
#include "TCC89x_Structures.h"
#define TCC_INTR_SC_DEV_NAME	"/dev/tcc-intr-sc"
#define TCC_INTR_SC1_DEV_NAME	"/dev/tcc-intr-sc1"
#endif
#define TCC_INTR_DEV_MAJOR		246
#define IOCTL_INTR_SET_SCORE	_IO(TCC_INTR_DEV_MAJOR, 1)
#define IOCTL_INTR_GET_SCORE	_IO(TCC_INTR_DEV_MAJOR, 2)

static int fd = -1;
static int fd1 = -1;
static struct pollfd tcc_event[1];
static struct pollfd tcc_event1[1];
#endif
/*****************************************************************************
*
* Variables
*
******************************************************************************/
Virtual_t 				vM2MScaler;
#ifdef TCC_79XX_INCLUDE
volatile sCKCReg		*pClockReg;
#endif
#ifdef TCC_89XX_INCLUDE
volatile DDICONFIG	*pClockReg;
#endif
static int				g_yuv_color_mode;
unsigned int			M2MBaseAddress;

#ifdef TCC_89XX_INCLUDE
//For M2M Scaler #1
Virtual_t 				vM2MScaler1;
volatile DDICONFIG	*pClockReg1;

unsigned int			M2MBaseAddress1;

unsigned int			M2MSrcAddress;
unsigned int			M2MDstAddress;

static int FB2_fd;
static int FB2_PAddr;
#endif
/*****************************************************************************
*
* structures
*
******************************************************************************/
static void dump_frame(void * frame_addr,unsigned int frame_size)
{
		unsigned int *pFrameAddr;
		unsigned long ulCount=0;
		pFrameAddr = (unsigned int *)frame_addr;
		int i;

		frame_size /= 4; // byte unit
		
		printf("Address = [%08x] \t    Size=[%d] \n",(unsigned int)frame_addr,frame_size );
		printf("********************** < Dump Start > ********************\n");

		printf(" %x ",(unsigned int)pFrameAddr);
		for(i=0;i<frame_size;i++)
		{
			printf("%08X ",(unsigned int)*pFrameAddr);
			pFrameAddr++;
			ulCount++;

			if(ulCount ==8)
			{
				printf("\n %x ",(unsigned int)pFrameAddr);
			}
			
			if(ulCount > 15)
			{
				printf("\n %x ",(unsigned int)pFrameAddr);
				ulCount=0;
			}
			
		}	
		printf("\n*************************** < Dump End > **************************\n");
	//exit(1);
}

void SetYuvMode(int yuv_mode)
{
	g_yuv_color_mode = yuv_mode;
}

int GetYuvMode(void)
{
	return g_yuv_color_mode;
}
/******************************************************************************
*	FUNCTIONS			: M2M_Scaler_Enable
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
void M2M_ScalerForVideo_Enable(int yuv_mode)
{

	if(!Init_Virtual(&vM2MScaler, TCC_MEMORY_DEVICE_NAME))
	{
		printf("Init_Virtual Alloc Error!\n");
		return;
	}

	#ifdef TCC_89XX_INCLUDE
	pClockReg			= (DDICONFIG *) SetVirtual(&vM2MScaler, /*&HwDDI_CONFIG_BASE*/0xF0251000, PHYSICAL_REG_SIZE);
	#endif
	if(!pClockReg)
	{
		printf("pCKCReg Alloc Error!\n");
		return;
	}

	M2MBaseAddress	= (unsigned int)SetVirtual(&vM2MScaler, /*&HwMSC_BASE_1*/0xF0220000, PHYSICAL_REG_SIZE);	
	//M2MBaseAddress	= (unsigned int)SetVirtual(&vM2MScaler, /*&HwMSC_BASE*/0xF0210000, PHYSICAL_REG_SIZE);	
	if(!M2MBaseAddress)
	{
		printf("M2MBaseAddress Alloc Error!\n");		
		FreeVirtual(&vM2MScaler, M2MBaseAddress ,PHYSICAL_REG_SIZE);
		return;
	}
	#ifdef TCC_89XX_INCLUDE
/*
	BITSET(pClockReg->SWRESET, HwSWRESET_M2MS_1);
	BITCLR(pClockReg->PWDN,HwBCLKCTR_M2MS_1);	
	BITCLR(pClockReg->SWRESET, HwSWRESET_M2MS_1);
*/
	BITSET(pClockReg->SWRESET, Hw6);
	BITCLR(pClockReg->PWDN,Hw6);	
	BITCLR(pClockReg->SWRESET, Hw6);
	#endif
	

#ifdef USE_M2M_INTRRUPT
	//system("mknod /dev/intr-mb c 246 2");

	fd = open(TCC_INTR_SC1_DEV_NAME, O_RDWR);
	if (fd < 0) {
		printf("m2m fd open error\n");
		return -1;
	}	
#endif	

#ifdef VIQE_INCLUDE
	g_yuv_color_mode = yuv_mode;
#else
	g_yuv_color_mode = yuv_mode;
#endif

}


/******************************************************************************
*	FUNCTIONS			: M2M_Scaler_Disable
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
void M2M_ScalerForVideo_Disable(void)
{	
#ifdef USE_M2M_INTRRUPT	
	if(fd>0)
	{
		close(fd);
		fd = -1;
	}
#endif	

#if 0
	#ifdef TCC_89XX_INCLUDE
	BITSET(pClockReg->SWRESET, Hw6);
	BITSET(pClockReg->PWDN, Hw6);	
	BITCLR(pClockReg->SWRESET, Hw6);
	#endif
#endif	

	if(pClockReg){
		FreeVirtual(&vM2MScaler, pClockReg		,PHYSICAL_REG_SIZE);		
	}
	if(M2MBaseAddress)
		FreeVirtual(&vM2MScaler, M2MBaseAddress, PHYSICAL_REG_SIZE);	

	DeInit_Virtual(&vM2MScaler);
}



/******************************************************************************
*	FUNCTIONS			: M2M_Scaler
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

void M2M_ScalerForVideo( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize, unsigned int s_y, unsigned int d_y , unsigned int v_offset, unsigned int h_offset) 
{
	unsigned int		write_val;
	volatile unsigned int  IsM2MScalerEnd ;
	//unsigned int 		pM2MRegBase;
	unsigned long 		IFlag;
	
	//pM2MRegBase = M2MBaseAddress;
		
#if 0
	if( chroma==1 || chroma==4 ) // YUV420 or YUV422S
	{
		BITCSET(HwREG(pM2MRegBase+0x14), 0xffffffff, HwSRCCFG_420SEP);                     // 420sep
		BITCSET(HwREG(pM2MRegBase+0x34), 0xffffffff, HwDSTCFG_420SEP);                     // 420sep
	}
	else if( chroma == 2 || chroma == 3) // YUV422 or YUV444
	{
		BITCSET(HwREG(pM2MRegBase+0x14), 0xffffffff, HwSRCCFG_422SEP);                     // 422sep
		BITCSET(HwREG(pM2MRegBase+0x34), 0xffffffff, HwDSTCFG_422SEP);                     // 422sep	
	}
#else
	//#ifdef TCC_89XX_INCLUDE
	write_val  =  0x00; // interleave mode
	//write_val |= 0x08; // interleave mode
	write_val |= 0x03; // yuv420 mode
	//BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, write_val);                     // 420sep	
	BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_420SEP);                     // 420sep	
	//#endif
	BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_420SEP);                     // 420sep
#endif

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);

	BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_y+s_hsize*s_vsize);

	BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_y+s_hsize*s_vsize+s_hsize*s_vsize/4);

	s_vsize= ((s_vsize-v_offset)>>1)<<1; // 2bit align

	write_val = (s_vsize<<16)+s_hsize;                          // src size

	BITCSET(HwREG(M2MBaseAddress+0x0c), 0xffffffff, write_val);                         // 

	s_hsize = ((s_hsize-h_offset)>>3)<<3; // 8bit align

	write_val = ((s_hsize/2)<<16)+s_hsize;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize

	BITCSET(HwREG(M2MBaseAddress+0x10), 0xffffffff, write_val);                         // 



	BITCSET(HwREG(M2MBaseAddress+0x20), 0xffffffff, d_y);

	BITCSET(HwREG(M2MBaseAddress+0x24), 0xffffffff, d_y+m_hsize*m_vsize);

	BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/4);



	write_val = (m_vsize<<16) + m_hsize;                       // dst size

	BITCSET(HwREG(M2MBaseAddress+0x2c), 0xffffffff, write_val);            
	
#if 0
	if(b_zoom) // only zoom in mode
		write_val = ((m_hsize/2)<<16) + m_hsize;                 // dst frame offset : src offset : U/V = s_hsize/2, Y = s_hsize
	else
		write_val = ((DEST_WIDTH/2)<<16) + DEST_WIDTH;                 // dst frame offset : src offset : U/V = s_hsize/2, Y = s_hsize
#else
	write_val = ((m_hsize/2)<<16) + m_hsize;                 // dst frame offset : src offset : U/V = s_hsize/2, Y = s_hsize
#endif
	BITCSET(HwREG(M2MBaseAddress+0x30), 0xffffffff, write_val);            



	write_val = ((256*s_vsize/m_vsize)<<16) + (256*s_hsize/m_hsize);           // scale ratio

	BITCSET(HwREG(M2MBaseAddress+0x40), 0xffffffff, write_val);


	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x80000000);

	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);

	
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000003);                     // start

#ifdef USE_M2M_INTRRUPT
	while (1) {
		int ret;
		memset(tcc_event, 0, sizeof(tcc_event));
		tcc_event[0].fd = fd;
		tcc_event[0].events = POLLIN;
		ret = poll((struct pollfd *)&tcc_event, 1, 1000);
		if (ret < 0) {
			printf("m2m poll error\n");
			return 0;
		}else if (ret == 0) {
			printf("m2m poll timeout\n");		
			return 0;
		}else if (ret > 0) {
			if (tcc_event[0].revents & POLLERR) {
				printf("m2m poll POLLERR\n");
				return 0;
			} else if (tcc_event[0].revents & POLLIN) {
				IFlag = HwREG(M2MBaseAddress+0x48);
				HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag
				break;
			}
		}
	}	
#else
	 IsM2MScalerEnd =0;
	
	while(!IsM2MScalerEnd )
	{		
		sched_yield();		
		IFlag = HwREG(M2MBaseAddress+0x48);

		if (IFlag & HwMSCSTR_IRDY)
		{
		        IsM2MScalerEnd =1;
		}
		else if (IFlag & HwMSCSTR_IBUSY)
		{
			IsM2MScalerEnd =2;
		}		

	}
	HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag	
#endif
		
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);                     // end

}

#define LCDC_YUV422SEQ
void m2m_wait_interrupt()
{
	while (1) 
	{
		int ret;
		unsigned long 		IFlag;
		memset(tcc_event, 0, sizeof(tcc_event));
		tcc_event[0].fd = fd;
		tcc_event[0].events = POLLIN;
		//unsigned int start=GetTimer();
		ret = poll((struct pollfd *)&tcc_event, 1, 5000);
		//unsigned int end=GetTimer();
		//double times=(double)(end-start)*0.000001;
		//if(times>get_max_polltime())
		//	set_max_polltime(times);
		//sum_viqe_m2m_time(times);
		if (ret < 0) {
			printf("m2m poll error\n");
			return 0;
		}else if (ret == 0) {
			printf("m2m poll timeout\n");		
			return 0;
		}else if (ret > 0) {
			if (tcc_event[0].revents & POLLERR) {
				printf("m2m poll POLLERR\n");
				return 0;
			} else if (tcc_event[0].revents & POLLIN) {
				IFlag = HwREG(M2MBaseAddress+0x48);
				HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag
			//	break;
				break;
			}
			else
			{
				//printf("m2m else!!!!\n");
			}
		}
	}
}
void M2M_ScalerForVideo_With_Address( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize, unsigned int s_y, unsigned int s_u, unsigned int s_v, unsigned int d_y , unsigned int v_offset, unsigned int h_offset, int hdmi_flag) 
{
	unsigned int		write_val;
	volatile unsigned int  IsM2MScalerEnd ;
	unsigned long 		IFlag;
		
	//write_val  =  0x20; // interleave mode
	write_val  =  0x00; // interleave mode
	write_val |= 0x08; // interleave mode
	write_val |= 0x03; // yuv420 mode

#ifdef VIQE_INCLUDE
	if(g_yuv_color_mode)// yuv420
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_420SEP);
	else
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_422SEQ0);	
#else
	if(g_yuv_color_mode)// yuv420
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_420SEP);
	else
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, write_val);		
#endif

#ifdef	LCDC_YUV422SEQ
	if(hdmi_flag)
		BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_422SEQ1);
	else
		BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_422SEQ0);

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);

	BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_u);
	BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_v);
	
#else
	BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_420SEP);

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);
	if(hdmi_flag)
	{
		BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_v);
		BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_u);
	}
	else
	{
		BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_u);
		BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_v);
	}

#endif
	//HwREG(M2MBaseAddress+0x34) = (HwREG(M2MBaseAddress+0x34) & ~(0x7)) | HwDSTCFG_420SEP;
	//HwREG(M2MBaseAddress+0x34) |= (1<<4);



	s_vsize= ((s_vsize-v_offset)>>1)<<1; // 2bit align

	write_val = (s_vsize<<16)+s_hsize;                          // src size

	BITCSET(HwREG(M2MBaseAddress+0x0c), 0xffffffff, write_val);                         // 

	s_hsize = ((s_hsize-h_offset)>>3)<<3; // 8bit align

	if(g_yuv_color_mode) // yuv420
		write_val = ((s_hsize/2)<<16)+s_hsize;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize	
	else
		write_val = ((s_hsize)<<16)+s_hsize;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize		

	BITCSET(HwREG(M2MBaseAddress+0x10), 0xffffffff, write_val);                         // 



	BITCSET(HwREG(M2MBaseAddress+0x20), 0xffffffff, d_y);

	BITCSET(HwREG(M2MBaseAddress+0x24), 0xffffffff, d_y+m_hsize*m_vsize);

	if(g_yuv_color_mode) // yuv420
		BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/4);
	else
		BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/2);



	write_val = (m_vsize<<16) + m_hsize;                       // dst size

	BITCSET(HwREG(M2MBaseAddress+0x2c), 0xffffffff, write_val);            

#ifdef	LCDC_YUV422SEQ
	write_val = ((m_hsize)<<16) + m_hsize*2;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
	//write_val = ((m_hsize)<<16)+m_hsize; 
#else
	write_val = ((m_hsize/2)<<16) + m_hsize;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
#endif

	BITCSET(HwREG(M2MBaseAddress+0x30), 0xffffffff, write_val);            



	write_val = ((256*s_vsize/m_vsize)<<16) + (256*s_hsize/m_hsize);           // scale ratio

	BITCSET(HwREG(M2MBaseAddress+0x40), 0xffffffff, write_val);

#ifdef VIQE_INCLUDE
	if(g_yuv_color_mode)// yuv420
		BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);
	else
		BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x80000000)); // on the fly
//	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x80000100)); // on the fly
#else
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);
#endif


#ifdef VIQE_INCLUDE
	if(g_yuv_color_mode)// yuv420
		BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000003);                     		// start
	else
		BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x00000003));  // on the fly tart
#else	
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000003);                     		// start
#endif

#if 1
#ifdef USE_M2M_INTRRUPT
	while (1) {
		int ret;
		memset(tcc_event, 0, sizeof(tcc_event));
		tcc_event[0].fd = fd;
		tcc_event[0].events = POLLIN;
		ret = poll((struct pollfd *)&tcc_event, 1, 5000);
		if (ret < 0) {
			printf("m2m poll error\n");
			return 0;
		}else if (ret == 0) {
			printf("m2m poll timeout\n");		
			return 0;
		}else if (ret > 0) {
			if (tcc_event[0].revents & POLLERR) {
				printf("m2m poll POLLERR\n");
				return 0;
			} else if (tcc_event[0].revents & POLLIN) {
				IFlag = HwREG(M2MBaseAddress+0x48);
				HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag
				break;
			}
		}
	}	
#else
	 IsM2MScalerEnd =0;
	
	while(!IsM2MScalerEnd )
	{		
		sched_yield();		
		IFlag = HwREG(M2MBaseAddress+0x48);

		if (IFlag & HwMSCSTR_IRDY)
		{
		        IsM2MScalerEnd =1;
		}
		else if (IFlag & HwMSCSTR_IBUSY)
		{
			IsM2MScalerEnd =2;
		}		

	}
	HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag	
#endif
#endif		
	//BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);                     // end

}
void M2M_ScalerForVideo_With_Address_noviqe( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize, unsigned int s_y, unsigned int s_u, unsigned int s_v, unsigned int d_y , unsigned int v_offset, unsigned int h_offset, int hdmi_flag) 
{
	unsigned int		write_val;
	volatile unsigned int  IsM2MScalerEnd ;
	unsigned long 		IFlag;
		
	//write_val  =  0x20; // interleave mode
	write_val  =  0x00; // interleave mode
	write_val |= 0x08; // interleave mode
	write_val |= 0x03; // yuv420 mode

	//printf("s_hsize=%d,s_vsize=%d,v_offset=%d,h_offset=%d\n",s_hsize,s_vsize,v_offset,h_offset);

#if 0//def VIQE_INCLUDE
	if(g_yuv_color_mode)// yuv420
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_420SEP);
	else
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_422SEQ0);	
#else
	if(g_yuv_color_mode)// yuv420
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_420SEP);
	else
	{
		fprintf(stderr,"M2M_ScalerForVideo_With_Address_noviqe %d\n",write_val);
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, write_val);		
	}
#endif

#ifdef	LCDC_YUV422SEQ
	if(hdmi_flag)
		BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_422SEQ1);
	else
		BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_422SEQ0);

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);

	BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_u);
	BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_v);
	
#else
	BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_420SEP);

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);
	if(hdmi_flag)
	{
		BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_v);
		BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_u);
	}
	else
	{
		BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_u);
		BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_v);
	}

#endif
	//HwREG(M2MBaseAddress+0x34) = (HwREG(M2MBaseAddress+0x34) & ~(0x7)) | HwDSTCFG_420SEP;
	//HwREG(M2MBaseAddress+0x34) |= (1<<4);



	s_vsize= ((s_vsize-v_offset)>>1)<<1; // 2bit align

	write_val = (s_vsize<<16)+s_hsize;                          // src size

	BITCSET(HwREG(M2MBaseAddress+0x0c), 0xffffffff, write_val);                         // 

	s_hsize = ((s_hsize-h_offset)>>3)<<3; // 8bit align

	if(g_yuv_color_mode) // yuv420
		write_val = ((s_hsize/2)<<16)+s_hsize;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize	
	else
		write_val = ((s_hsize)<<16)+s_hsize;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize		

	BITCSET(HwREG(M2MBaseAddress+0x10), 0xffffffff, write_val);                         // 



	BITCSET(HwREG(M2MBaseAddress+0x20), 0xffffffff, d_y);

	BITCSET(HwREG(M2MBaseAddress+0x24), 0xffffffff, d_y+m_hsize*m_vsize);

	if(g_yuv_color_mode) // yuv420
		BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/4);
	else
		BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/2);



	write_val = (m_vsize<<16) + m_hsize;                       // dst size

	BITCSET(HwREG(M2MBaseAddress+0x2c), 0xffffffff, write_val);            

#ifdef	LCDC_YUV422SEQ
	write_val = ((m_hsize)<<16) + m_hsize*2;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
	//write_val = ((m_hsize)<<16)+m_hsize; 
#else
	write_val = ((m_hsize/2)<<16) + m_hsize;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
#endif

	BITCSET(HwREG(M2MBaseAddress+0x30), 0xffffffff, write_val);            


	write_val = ((256*s_vsize/m_vsize)<<16) + (256*s_hsize/m_hsize);           // scale ratio

	BITCSET(HwREG(M2MBaseAddress+0x40), 0xffffffff, write_val);

#if 0//def VIQE_INCLUDE
	if(g_yuv_color_mode)// yuv420
		BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);
	else
		BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x80000000)); // on the fly
//	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x80000100)); // on the fly
#else
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);
#endif


#if 0//def VIQE_INCLUDE
	if(g_yuv_color_mode)// yuv420
		BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000003);                     		// start
	else
		BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x00000003));  // on the fly tart
#else	
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000003);                     		// start
#endif

#if 0		
#ifdef USE_M2M_INTRRUPT
	while (1) {
		int ret;
		memset(tcc_event, 0, sizeof(tcc_event));
		tcc_event[0].fd = fd;
		tcc_event[0].events = POLLIN;
		ret = poll((struct pollfd *)&tcc_event, 1, 5000);
		if (ret < 0) {
			printf("m2m poll error\n");
			return 0;
		}else if (ret == 0) {
			printf("m2m poll timeout\n");		
			return 0;
		}else if (ret > 0) {
			if (tcc_event[0].revents & POLLERR) {
				printf("m2m poll POLLERR\n");
				return 0;
			} else if (tcc_event[0].revents & POLLIN) {
				IFlag = HwREG(M2MBaseAddress+0x48);
				HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag
				break;
			}
		}
	}	
#else
	 IsM2MScalerEnd =0;
	
	while(!IsM2MScalerEnd )
	{		
		sched_yield();		
		IFlag = HwREG(M2MBaseAddress+0x48);

		if (IFlag & HwMSCSTR_IRDY)
		{
		        IsM2MScalerEnd =1;
		}
		else if (IFlag & HwMSCSTR_IBUSY)
		{
			IsM2MScalerEnd =2;
		}		

	}
	HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag	
#endif
#endif
	//BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);                     // end
	//BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);                     // end

}

void M2M_ScalerForVideo_With_Offset( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize, unsigned int s_y, unsigned int s_u, unsigned int s_v, unsigned int d_y , unsigned int w_offset, int hdmi_flag) 
{
	unsigned int		write_val;
	volatile unsigned int  IsM2MScalerEnd ;
	unsigned long 		IFlag;
		
	write_val  =  0x00; // interleave mode
	write_val |= 0x08; // interleave mode
	write_val |= 0x03; // yuv420 mode

#ifdef VIQE_INCLUDE
	BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_422SEQ0);	
#else
	if(g_yuv_color_mode)// yuv420
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_420SEP);
	else
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, write_val);		
#endif


#ifdef	LCDC_YUV422SEQ
	if(hdmi_flag)
		BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_422SEQ1);
	else
		BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_422SEQ0);

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);
	BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_u);
	BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_v);	
#else
	BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_420SEP);

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);
	if(hdmi_flag)
	{
		BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_v);
		BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_u);
	}
	else
	{
		BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_u);
		BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_v);
	}
#endif


	write_val = (s_vsize<<16)+s_hsize;                          								// src width, height 
	BITCSET(HwREG(M2MBaseAddress+0x0c), 0xffffffff, write_val);                          


	if(g_yuv_color_mode) // yuv420
		write_val = ((w_offset/2)<<16)+w_offset;                    						// src frame stride
	else
		write_val = ((w_offset)<<16)+w_offset;                    							// src frame stride

	BITCSET(HwREG(M2MBaseAddress+0x10), 0xffffffff, write_val);                          



	BITCSET(HwREG(M2MBaseAddress+0x20), 0xffffffff, d_y);
	BITCSET(HwREG(M2MBaseAddress+0x24), 0xffffffff, d_y+m_hsize*m_vsize);
	if(g_yuv_color_mode) // yuv420
		BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/4);
	else
		BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/2);


	write_val = (m_vsize<<16) + m_hsize;                       // dst size

	BITCSET(HwREG(M2MBaseAddress+0x2c), 0xffffffff, write_val);            

#ifdef	LCDC_YUV422SEQ
	write_val = ((m_hsize)<<16) + m_hsize*2;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
	//write_val = ((m_hsize)<<16)+m_hsize; 
#else
	write_val = ((m_hsize/2)<<16) + m_hsize;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
#endif

	BITCSET(HwREG(M2MBaseAddress+0x30), 0xffffffff, write_val);            



	write_val = ((256*s_vsize/m_vsize)<<16) + (256*s_hsize/m_hsize);           // scale ratio

	BITCSET(HwREG(M2MBaseAddress+0x40), 0xffffffff, write_val);

#ifdef VIQE_INCLUDE
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x80000000)); // on the fly
//	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x80000100)); // on the fly
#else
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);
#endif


#ifdef VIQE_INCLUDE
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x00000003));  // on the fly tart
#else	
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000003);                     		// start
#endif

#ifdef USE_M2M_INTRRUPT
	while (1) {
		int ret;
		memset(tcc_event, 0, sizeof(tcc_event));
		tcc_event[0].fd = fd;
		tcc_event[0].events = POLLIN;
		ret = poll((struct pollfd *)&tcc_event, 1, 5000);
		if (ret < 0) {
			printf("m2m poll error\n");
			return 0;
		}else if (ret == 0) {
			printf("m2m poll timeout\n");		
			return 0;
		}else if (ret > 0) {
			if (tcc_event[0].revents & POLLERR) {
				printf("m2m poll POLLERR\n");
				return 0;
			} else if (tcc_event[0].revents & POLLIN) {
				IFlag = HwREG(M2MBaseAddress+0x48);
				HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag
				break;
			}
		}
	}	
#else
	 IsM2MScalerEnd =0;
	
	while(!IsM2MScalerEnd )
	{		
		sched_yield();		
		IFlag = HwREG(M2MBaseAddress+0x48);

		if (IFlag & HwMSCSTR_IRDY)
		{
		        IsM2MScalerEnd =1;
		}
		else if (IFlag & HwMSCSTR_IBUSY)
		{
			IsM2MScalerEnd =2;
		}		

	}
	HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag	
#endif
		
	//BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);                     // end

}


void M2M_ScalerForVideo_Interlaced( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize,unsigned int s_hsize_offset,unsigned int s_y, unsigned int d_y , unsigned int v_offset, unsigned int h_offset) 
{
	unsigned int		write_val;
	volatile unsigned int  IsM2MScalerEnd ;
	//unsigned int 		pM2MRegBase;
	unsigned long 		IFlag;
	unsigned int 		nSrcOffsetC;

	//pM2MRegBase = M2MBaseAddress;

		
#if 0
	if( chroma==1 || chroma==4 ) // YUV420 or YUV422S
	{
		BITCSET(HwREG(pM2MRegBase+0x14), 0xffffffff, HwSRCCFG_420SEP);                     // 420sep
		BITCSET(HwREG(pM2MRegBase+0x34), 0xffffffff, HwDSTCFG_420SEP);                     // 420sep
	}
	else if( chroma == 2 || chroma == 3) // YUV422 or YUV444
	{
		BITCSET(HwREG(pM2MRegBase+0x14), 0xffffffff, HwSRCCFG_422SEP);                     // 422sep
		BITCSET(HwREG(pM2MRegBase+0x34), 0xffffffff, HwDSTCFG_422SEP);                     // 422sep	
	}
#else
	#ifdef TCC_89XX_INCLUDE
	write_val  =  0x00; // interleave mode
	write_val |= 0x08; // interleave mode
	write_val |= 0x03; // yuv420 mode
	BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, write_val);                     // 420sep	
	#endif
	#ifdef TCC_79XX_INCLUDE
	BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwSRCCFG_420SEP);                     // 420sep
	#endif
	BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_420SEP);                     // 420sep
#endif

	nSrcOffsetC = s_hsize_offset/2;

	s_vsize= ((s_vsize-v_offset)>>1)<<1; // 2bit align
	s_hsize = ((s_hsize-h_offset)>>3)<<3; // 8bit align

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);

	BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_y+s_hsize_offset*s_vsize);	// source u

	BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_y+s_hsize_offset*s_vsize+s_hsize_offset*s_vsize/4);

	write_val = (s_vsize<<16) + s_hsize; //+s_hsize;                          // src size

	BITCSET(HwREG(M2MBaseAddress+0x0c), 0xffffffff, write_val);                         // 

	write_val = ((nSrcOffsetC)<<16) + s_hsize_offset;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize

	BITCSET(HwREG(M2MBaseAddress+0x10), 0xffffffff, write_val);                         // 



	BITCSET(HwREG(M2MBaseAddress+0x20), 0xffffffff, d_y);

	BITCSET(HwREG(M2MBaseAddress+0x24), 0xffffffff, d_y+m_hsize*m_vsize);

	BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/4);


	write_val = (m_vsize<<16) + m_hsize;                       // dst size

	BITCSET(HwREG(M2MBaseAddress+0x2c), 0xffffffff, write_val);            
	
#if 0
	if(b_zoom) // only zoom in mode
		write_val = ((m_hsize/2)<<16) + m_hsize;                 // dst frame offset : src offset : U/V = s_hsize/2, Y = s_hsize
	else
		write_val = ((DEST_WIDTH/2)<<16) + DEST_WIDTH;                 // dst frame offset : src offset : U/V = s_hsize/2, Y = s_hsize
#else
	write_val = ((m_hsize/2)<<16) + m_hsize;                 // dst frame offset : src offset : U/V = s_hsize/2, Y = s_hsize
#endif
	BITCSET(HwREG(M2MBaseAddress+0x30), 0xffffffff, write_val);            



	write_val = ((256*s_vsize/m_vsize)<<16) + (256*s_hsize/m_hsize);           // scale ratio

	BITCSET(HwREG(M2MBaseAddress+0x40), 0xffffffff, write_val);


	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x80000000);

	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);

	
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000003);                     // start

#ifdef USE_M2M_INTRRUPT
	while (1) {
		int ret;
		memset(tcc_event, 0, sizeof(tcc_event));
		tcc_event[0].fd = fd;
		tcc_event[0].events = POLLIN;
		ret = poll((struct pollfd *)&tcc_event, 1, 5000);
		if (ret < 0) {
			printf("m2m poll error\n");
			return 0;
		}else if (ret == 0) {
			printf("m2m poll timeout\n");		
			return 0;
		}else if (ret > 0) {
			if (tcc_event[0].revents & POLLERR) {
				printf("m2m poll POLLERR\n");
				return 0;
			} else if (tcc_event[0].revents & POLLIN) {
				IFlag = HwREG(M2MBaseAddress+0x48);
				HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag
				break;
			}
		}
	}	
#else
	 IsM2MScalerEnd =0;
	
	while(!IsM2MScalerEnd )
	{		
		sched_yield();		
		IFlag = HwREG(M2MBaseAddress+0x48);

		if (IFlag & HwMSCSTR_IRDY)
		{
		        IsM2MScalerEnd =1;
		}
		else if (IFlag & HwMSCSTR_IBUSY)
		{
			IsM2MScalerEnd =2;
		}		

	}
	HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag	
#endif
		
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);                     // end

}

#define LCDC_YUV422SEQ
void M2M_ScalerForVideo_With_Address_Stg1( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize, unsigned int s_y, unsigned int s_u, unsigned int s_v, unsigned int d_y , unsigned int v_offset, unsigned int h_offset, int hdmi_flag) 
{
	unsigned int		write_val;
	volatile unsigned int  IsM2MScalerEnd ;
	unsigned long 		IFlag;
		
	//write_val  =  0x20; // interleave mode
	write_val  =  0x00; // interleave mode
	write_val |= 0x08; // interleave mode
	write_val |= 0x03; // yuv420 mode

#if 0//def VIQE_INCLUDE
	BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_422SEQ0);	
#else
	if(1)//g_yuv_color_mode)// yuv420
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_420SEP);
	else
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, write_val);		
#endif

#ifdef	LCDC_YUV422SEQ
	if(hdmi_flag)
		BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_420SEP);
	else
		BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_420SEP);

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);

	BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_u);
	BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_v);
	
#else
	BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_420SEP);

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);
	if(hdmi_flag)
	{
		BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_v);
		BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_u);
	}
	else
	{
		BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_u);
		BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_v);
	}

#endif
	//HwREG(M2MBaseAddress+0x34) = (HwREG(M2MBaseAddress+0x34) & ~(0x7)) | HwDSTCFG_420SEP;
	//HwREG(M2MBaseAddress+0x34) |= (1<<4);



	s_vsize= ((s_vsize-v_offset)>>1)<<1; // 2bit align

	write_val = (s_vsize<<16)+s_hsize;                          // src size

	BITCSET(HwREG(M2MBaseAddress+0x0c), 0xffffffff, write_val);                         // 

	s_hsize = ((s_hsize-h_offset)>>3)<<3; // 8bit align

	if(1)//g_yuv_color_mode) // yuv420
//		write_val = ((s_hsize/2)<<16)+s_hsize;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize	
		write_val = ((0)<<16)+s_hsize;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize	
	else
		write_val = ((s_hsize)<<16)+s_hsize;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize		

	BITCSET(HwREG(M2MBaseAddress+0x10), 0xffffffff, write_val);                         // 



	BITCSET(HwREG(M2MBaseAddress+0x20), 0xffffffff, d_y);

	BITCSET(HwREG(M2MBaseAddress+0x24), 0xffffffff, d_y+m_hsize*m_vsize);

	if(1)//g_yuv_color_mode) // yuv420
		BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/4);
	else
		BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/2);



	write_val = (m_vsize<<16) + m_hsize;                       // dst size

	BITCSET(HwREG(M2MBaseAddress+0x2c), 0xffffffff, write_val);            

#ifdef	LCDC_YUV422SEQ
	write_val = ((0)<<16) + m_hsize;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
	//write_val = ((m_hsize)<<16) + m_hsize*2;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
	//write_val = ((m_hsize)<<16)+m_hsize; 
#else
	write_val = ((m_hsize/2)<<16) + m_hsize;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
#endif

	BITCSET(HwREG(M2MBaseAddress+0x30), 0xffffffff, write_val);            



	write_val = ((256*s_vsize/m_vsize)<<16) + (256*s_hsize/m_hsize);           // scale ratio

	BITCSET(HwREG(M2MBaseAddress+0x40), 0xffffffff, write_val);

#if 0//def VIQE_INCLUDE
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x80000000)); // on the fly
//	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x80000100)); // on the fly
#else
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);
#endif


#if 0//def VIQE_INCLUDE
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x00000003));  // on the fly tart
#else	
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000003);                     		// start
#endif

#ifdef USE_M2M_INTRRUPT
	while (1) {
		int ret;
		memset(tcc_event, 0, sizeof(tcc_event));
		tcc_event[0].fd = fd;
		tcc_event[0].events = POLLIN;
		ret = poll((struct pollfd *)&tcc_event, 1, 5000);
		if (ret < 0) {
			printf("m2m poll error\n");
			return 0;
		}else if (ret == 0) {
			printf("m2m poll timeout\n");		
			return 0;
		}else if (ret > 0) {
			if (tcc_event[0].revents & POLLERR) {
				printf("m2m poll POLLERR\n");
				return 0;
			} else if (tcc_event[0].revents & POLLIN) {
				IFlag = HwREG(M2MBaseAddress+0x48);
				HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag
				break;
			}
		}
	}	
#else
	 IsM2MScalerEnd =0;
	
	while(!IsM2MScalerEnd )
	{		
		sched_yield();		
		IFlag = HwREG(M2MBaseAddress+0x48);

		if (IFlag & HwMSCSTR_IRDY)
		{
		        IsM2MScalerEnd =1;
		}
		else if (IFlag & HwMSCSTR_IBUSY)
		{
			IsM2MScalerEnd =2;
		}		

	}
	HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag	
#endif
		
	//BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);                     // end

}

void M2M_ScalerForVideo_With_Address_Stg2( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize, unsigned int s_y, unsigned int s_u, unsigned int s_v, unsigned int d_y , unsigned int v_offset, unsigned int h_offset, int hdmi_flag) 
{
	unsigned int		write_val;
	volatile unsigned int  IsM2MScalerEnd ;
	unsigned long 		IFlag;
		
	//write_val  =  0x20; // interleave mode
	write_val  =  0x00; // interleave mode
	write_val |= 0x08; // interleave mode
	write_val |= 0x02; // yuv422 mode

#if 0//def VIQE_INCLUDE
	BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_422SEQ0);	
#else
	if(1)//g_yuv_color_mode)// yuv420
//		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, HwDSTCFG_420SEP|0x80);
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, write_val);
	else
		BITCSET(HwREG(M2MBaseAddress+0x14), 0xffffffff, write_val);		
#endif

#ifdef	LCDC_YUV422SEQ
	if(hdmi_flag)
		BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_422SEQ1);
	else
		BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_422SEQ0);

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);

	BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_u);
	BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_v);
	
#else
	BITCSET(HwREG(M2MBaseAddress+0x34), 0x0000000f, HwDSTCFG_420SEP);

	BITCSET(HwREG(M2MBaseAddress+0x00), 0xffffffff, s_y);
	if(hdmi_flag)
	{
		BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_v);
		BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_u);
	}
	else
	{
		BITCSET(HwREG(M2MBaseAddress+0x04), 0xffffffff, s_u);
		BITCSET(HwREG(M2MBaseAddress+0x08), 0xffffffff, s_v);
	}

#endif
	//HwREG(M2MBaseAddress+0x34) = (HwREG(M2MBaseAddress+0x34) & ~(0x7)) | HwDSTCFG_420SEP;
	//HwREG(M2MBaseAddress+0x34) |= (1<<4);



	s_vsize= ((s_vsize-v_offset)>>1)<<1; // 2bit align

	write_val = (s_vsize<<16)+s_hsize;                          // src size

	BITCSET(HwREG(M2MBaseAddress+0x0c), 0xffffffff, write_val);                         // 

	s_hsize = ((s_hsize-h_offset)>>3)<<3; // 8bit align

	if(0)//g_yuv_color_mode) // yuv420
		write_val = ((s_hsize/2)<<16)+s_hsize;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize	
	else
		write_val = ((s_hsize)<<16)+s_hsize;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize		

	BITCSET(HwREG(M2MBaseAddress+0x10), 0xffffffff, write_val);                         // 



	BITCSET(HwREG(M2MBaseAddress+0x20), 0xffffffff, d_y);

	BITCSET(HwREG(M2MBaseAddress+0x24), 0xffffffff, d_y+m_hsize*m_vsize);

	if(1)//g_yuv_color_mode) // yuv420
		BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/4);
	else
		BITCSET(HwREG(M2MBaseAddress+0x28), 0xffffffff, d_y+m_hsize*m_vsize+m_hsize*m_vsize/2);



	write_val = (m_vsize<<16) + m_hsize;                       // dst size

	BITCSET(HwREG(M2MBaseAddress+0x2c), 0xffffffff, write_val);            

#ifdef	LCDC_YUV422SEQ
	write_val = ((m_hsize)<<16) + m_hsize*2;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
	//write_val = ((m_hsize)<<16)+m_hsize; 
#else
	write_val = ((m_hsize/2)<<16) + m_hsize;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
#endif

	BITCSET(HwREG(M2MBaseAddress+0x30), 0xffffffff, write_val);            



	write_val = ((256*s_vsize/m_vsize)<<16) + (256*s_hsize/m_hsize);           // scale ratio

	BITCSET(HwREG(M2MBaseAddress+0x40), 0xffffffff, write_val);

#if 0//def VIQE_INCLUDE
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x80000000)); // on the fly
//	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x80000100)); // on the fly
#else
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);
#endif


#if 0//def VIQE_INCLUDE
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, (1<<24)|(0x00000003));  // on the fly tart
#else	
	BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000003);                     		// start
#endif

#ifdef USE_M2M_INTRRUPT
	while (1) {
		int ret;
		memset(tcc_event, 0, sizeof(tcc_event));
		tcc_event[0].fd = fd;
		tcc_event[0].events = POLLIN;
		ret = poll((struct pollfd *)&tcc_event, 1, 5000);
		if (ret < 0) {
			printf("m2m poll error\n");
			return 0;
		}else if (ret == 0) {
			printf("m2m poll timeout\n");		
			return 0;
		}else if (ret > 0) {
			if (tcc_event[0].revents & POLLERR) {
				printf("m2m poll POLLERR\n");
				return 0;
			} else if (tcc_event[0].revents & POLLIN) {
				IFlag = HwREG(M2MBaseAddress+0x48);
				HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag
				break;
			}
		}
	}	
#else
	 IsM2MScalerEnd =0;
	
	while(!IsM2MScalerEnd )
	{		
		sched_yield();		
		IFlag = HwREG(M2MBaseAddress+0x48);

		if (IFlag & HwMSCSTR_IRDY)
		{
		        IsM2MScalerEnd =1;
		}
		else if (IFlag & HwMSCSTR_IBUSY)
		{
			IsM2MScalerEnd =2;
		}		

	}
	HwREG(M2MBaseAddress+0x48) = IFlag; // clear interrupt flag	
#endif
		
	//BITCSET(HwREG(M2MBaseAddress+0x44), 0xffffffff, 0x00000000);                     // end

}


#ifdef TCC_89XX_INCLUDE
/*For using M2M Scaler #1 */
//#define USE_M2M_INTRRUPT1
void M2M_ScalerForVideo_Enable1(void)
{
	int ret;	
	unsigned int M2MSrcPhyAddr = HW_MEM_BASE;

printf("---------%s    %d\n",__func__,__LINE__);
	struct fb_fix_screeninfo fbfix;
	
	if(!Init_Virtual(&vM2MScaler1, TCC_MEMORY_DEVICE_NAME))
	{
		printf("Init_Virtual Alloc Error!\n");
		return;
	}

	pClockReg1			= (DDICONFIG *) SetVirtual(&vM2MScaler1, &HwDDI_CONFIG_BASE, PHYSICAL_REG_SIZE);

	if(!pClockReg1)
	{
		printf("pCKCReg Alloc Error!\n");
		return;
	}

	M2MBaseAddress1	= (unsigned int)SetVirtual(&vM2MScaler1, &HwMSC_BASE, PHYSICAL_REG_SIZE);	
	if(!M2MBaseAddress1)
	{
		printf("M2MBaseAddress Alloc Error!\n");		
		FreeVirtual(&vM2MScaler1, M2MBaseAddress1 ,PHYSICAL_REG_SIZE);
		return;
	}

	M2MSrcAddress = (unsigned short *)SetVirtual(&vM2MScaler1,M2MSrcPhyAddr,SCALER_LIMIT);
	printf("M2MSrcAddress : %08x \n",M2MSrcAddress);
	if(!M2MSrcAddress)
	{
		printf(">>>>>>>>>>M2MBaseAddress Alloc Error!\n");		
		FreeVirtual(&vM2MScaler1, M2MSrcAddress ,SCALER_LIMIT);
		return;
	}

	/*Getting FB2 Base Address (physical)*/
    FB2_fd = open ( "/dev/fb2" , O_RDWR);
	ret = ioctl(FB2_fd, FBIOGET_FSCREENINFO, &fbfix);  
	close(FB2_fd);
	
	FB2_PAddr = fbfix.smem_start;
	M2MDstAddress = (unsigned short *)SetVirtual(&vM2MScaler1,FB2_PAddr,SCALER_LIMIT);
	
	if(!M2MDstAddress)
	{
		printf("M2MBaseAddress Alloc Error!\n");		
		FreeVirtual(&vM2MScaler1, M2MDstAddress ,SCALER_LIMIT);
		return;
	}


	BITSET(pClockReg1->SWRESET, HwSWRESET_M2MS);
	BITCLR(pClockReg1->PWDN, HwBCLKCTR_M2MS);		
	BITCLR(pClockReg1->SWRESET, HwSWRESET_M2MS);

#ifdef USE_M2M_INTRRUPT1
	//system("mknod /dev/intr-mb c 246 2");

	fd1 = open(TCC_INTR_SC_DEV_NAME, O_RDWR);
	if (fd1 < 0) {
		printf("m2m #1 fd open error\n");
		return -1;
	}	
#endif	
	
	//g_yuv_color_mode1 = yuv_mode;

}

void M2M_ScalerForVideo_Disable1(void)
{	
	
#ifdef USE_M2M_INTRRUPT1	
	if(fd1>0)
	{
		close(fd1);
		fd1 = -1;
	}
#endif	
	//ose(FB2_fd);
	BITSET(pClockReg1->SWRESET, HwSWRESET_M2MS);
	BITSET(pClockReg1->PWDN, HwBCLKCTR_M2MS);		
	BITCLR(pClockReg1->SWRESET, HwSWRESET_M2MS);

	if(pClockReg1){
		FreeVirtual(&vM2MScaler1, pClockReg1		,PHYSICAL_REG_SIZE);		
	}

	if(M2MBaseAddress1)
		FreeVirtual(&vM2MScaler1, M2MBaseAddress1, PHYSICAL_REG_SIZE);	

	if(M2MSrcAddress)
		FreeVirtual(&vM2MScaler1, M2MSrcAddress, SCALER_LIMIT);
	
	if(M2MDstAddress)
		FreeVirtual(&vM2MScaler1, M2MDstAddress, SCALER_LIMIT);

	DeInit_Virtual(&vM2MScaler1);
}



void M2M_ScalerForVideo_DVDSubpic( unsigned int s_hsize, unsigned int s_vsize, unsigned int d_hsize, unsigned int d_vsize, unsigned int srcaddr,unsigned int dstaddr,unsigned int colormode) 
{
	unsigned int		write_val;
	volatile unsigned int  IsM2MScalerEnd ;
	unsigned long 		IFlag;


	/*Source Color Format*/
	if(colormode)
	{
		BITCSET(HwREG(M2MBaseAddress1+0x14), 0xffffffff, HwDSTCFG_565);
	}
	else
	{
		BITCSET(HwREG(M2MBaseAddress1+0x14), 0xffffffff, HwDSTCFG_444);	
	}
	
	/*Destination Color Format*/

	if(1)//colormode)
	{
		BITCSET(HwREG(M2MBaseAddress1+0x34), 0x0000000f, HwDSTCFG_565);
	}
	else
	{
		BITCSET(HwREG(M2MBaseAddress1+0x34), 0x0000000f, HwDSTCFG_444);
	}


	/*Source Image Base Address*/
	BITCSET(HwREG(M2MBaseAddress1+0x00), 0xffffffff, srcaddr);

	/*Source Image Size*/
	write_val = (s_vsize<<16)+s_hsize;                       
	BITCSET(HwREG(M2MBaseAddress1+0x0c), 0xffffffff, write_val);                         

	/*Source Image Offset*/
	write_val = ((s_hsize*2)<<16)+s_hsize*2;                    // src frame offset : U/V = s_hsize/2, Y = s_hsize		
	BITCSET(HwREG(M2MBaseAddress1+0x10), 0xffffffff, write_val);                          


	/*Destination Image Base Address*/
	BITCSET(HwREG(M2MBaseAddress1+0x20), 0xffffffff, dstaddr);

	/*Destination Image Size*/
	write_val = (d_vsize<<16) + d_hsize;                       // dst size
	BITCSET(HwREG(M2MBaseAddress1+0x2c), 0xffffffff, write_val);            


	/*Destination Image Offset*/
	write_val = ((d_hsize*2)<<16) + d_hsize*2;                 // dst frame offset : src offset : U/V = s_hsize, Y = s_hsize*2
	BITCSET(HwREG(M2MBaseAddress1+0x30), 0xffffffff, write_val);            


	/*MSC Information*/
	write_val = ((256*s_vsize/d_vsize)<<16) + (256*s_hsize/d_hsize);           // scale ratio
	BITCSET(HwREG(M2MBaseAddress1+0x40), 0xffffffff, write_val);

	/*MSC Control*/
	BITCSET(HwREG(M2MBaseAddress1+0x44), 0xffffffff, 0x00000000);
	BITCSET(HwREG(M2MBaseAddress1+0x44), 0xffffffff, 0x00000003);               // start


#ifdef USE_M2M_INTRRUPT1
	while (1) {
		int ret;
		memset(tcc_event1, 0, sizeof(tcc_event1));
		tcc_event1[0].fd = fd1;
		tcc_event1[0].events = POLLIN;
		ret = poll((struct pollfd *)&tcc_event1, 1, 5000);
		if (ret < 0) {
			printf("m2m #1 poll error\n");
			return 0;
		}else if (ret == 0) {
			printf("m2m #1 poll timeout\n");		
			return 0;
		}else if (ret > 0) {
			if (tcc_event[0].revents & POLLERR) {
				printf("m2m #1 poll POLLERR\n");
				return 0;
			} else if (tcc_event1[0].revents & POLLIN) {
				IFlag = HwREG(M2MBaseAddress1+0x48);
				HwREG(M2MBaseAddress1+0x48) = IFlag; // clear interrupt flag
				break;
			}
		}
	}	
#else
	IsM2MScalerEnd =0;
	
	while(!IsM2MScalerEnd )
	{		
		sched_yield();		
		IFlag = HwREG(M2MBaseAddress1+0x48);

		if (IFlag & HwMSCSTR_IRDY)
		{
		        IsM2MScalerEnd =1;
		}
		else if (IFlag & HwMSCSTR_IBUSY)
		{
			IsM2MScalerEnd =2;
		}		

	}

	HwREG(M2MBaseAddress1+0x48) = IFlag; // clear interrupt flag	
#endif
}


unsigned int M2M_ScalerForVideo_GetSrcVAddr(void)
{
	return M2MSrcAddress;
}

unsigned int M2M_ScalerForVideo_GetDstVAddr(void)
{
	return M2MDstAddress;
}

unsigned int M2M_ScalerForVideo_GetSrcPAddr(void)
{
	return HW_MEM_BASE;
}

unsigned int M2M_ScalerForVideo_GetDstPAddr(void)
{
	return FB2_PAddr;
}

#endif /*TCC_89XX_INCLUDE : For M2M Scaler #1*/

