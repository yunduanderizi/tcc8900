/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : TCCUtil.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef	_VIRTUAL_H__
#define	_VIRTUAL_H__

/******************************************************************************
* include 
******************************************************************************/
#include "globals.h"

//#define TCC_ANDROID_INCLUDE
#ifdef TCC_ANDROID_INCLUDE
//#define TCC_79XX_INCLUDE
#endif

/******************************************************************************
* TCC79XX defines 
******************************************************************************/
#ifdef TCC_79XX_INCLUDE
#include "TCC79x_Physical.h"
#define SHARED_MEM_PHYSADDR		0xA0000000

#ifdef TCC_ANDROID_INCLUDE
#define RAM128
#define FOR_ENCODER
#ifdef RAM128
#define HW_MEM_BASE				0x27800000
#else
#define HW_MEM_BASE				0x23800000
#endif
#else
#define HW_MEM_BASE				0x20000000 
#endif /*TCC_ANDROID_INCLUDE*/

#define HW_MEM_LIMIT			0x00400000
#define OUTBUFFER_BASE			HW_MEM_BASE
#define OUTBUFFER_LIMIT			0x00300000
#define INBUFFER_BASE			(HW_MEM_BASE + OUTBUFFER_LIMIT)
#define INBUFFER_LIMIT				0x00040000 
#define DATABUFFER_BASE		(HW_MEM_BASE + OUTBUFFER_LIMIT + INBUFFER_LIMIT)
#define DATABUFFER_LIMIT			0x00040000
#define SCALER_OUT1				(HW_MEM_BASE + OUTBUFFER_LIMIT + INBUFFER_LIMIT + DATABUFFER_LIMIT)
#define SCALER_OUT2				(HW_MEM_BASE + OUTBUFFER_LIMIT + INBUFFER_LIMIT + DATABUFFER_LIMIT + SCALER_LIMIT)
#define SCALER_LIMIT				0x00040000

#define DFLT_BASE_WMV9_ROBASE			0x10004000
#define DFLT_BASE_MPEG2_ROBASE			0x10008000
#define DFLT_BASE_MPEG2_HTBASE			0x10016000
#define DFLT_BASE_MPEG4SP_ROBASE		0x10004000 
#define DFLT_BASE_MPEG4SP_HTBASE		0x10016000

#define MBMEM0BASE				0x10010000
#define MBMEM0BASE_SIZE				6144

#define DFLT_BASE_JPEGDEC_ROBASE		0x10004000
#define DFLT_BASE_JPEGENC_ROBASE		0x10008000
#define PHY_ADDR_R_PINGPONG_1			0x10014000
#define PHY_ADDR_R_PINGPONG_2			0x10016000

#define	IMG_CH0			2
#define	IMG_CH1			4
#define	IMG_CH2			8
#define	LCD_CH			1

#define	IMG_LIT_ENDIAN	((0x0)<<7)
#define	IMG_BIG_ENDIAN	((0x1)<<7)
#define	IMG_RGB			((0x0)<<4)
#define	IMG_YUV420			((0x1)<<4)
#define	IMG_YUV422			((0x3)<<4)
#define	IMG_YUV422_DISP	((0x7)<<4)
#define	IMG_BPP_1			((0x0)<<0)
#define	IMG_BPP_2			((0x1)<<0)
#define	IMG_BPP_4			((0x2)<<0)
#define	IMG_BPP_332		((0x3)<<0)
#define	IMG_BPP_444		((0x4)<<0)
#define	IMG_BPP_565		((0x5)<<0)
#define	IMG_BPP_555		((0x6)<<0)
#define	IMG_BPP_888		((0x7)<<0)

#define	PXDW_4				((0x0))
#define	PXDW_8				((0x1))
#define	PXDW_888			((0x2))
#define	PXDW_565			((0x3))
#define	PXDW_555			((0x4))
#define	PXDW_18			((0x5))
#define	PXDW_8_UY_SEQ		((0x6))
#define	PXDW_8_VY_SEQ		((0x7))
#define	PXDW_16_YU_SEQ	((0x8))
#define	PXDW_16_YV_SEQ	((0x9))

#define 	LSCALE1				((0x0))
#define 	LSCALE2				((0x1))
#define 	LSCALE3				((0x2))
#define 	LSCALE4				((0x3))
#define 	LSCALE8				((0x7))

#define	IMG_ORDER_012		0x00000000
#define	IMG_ORDER_021		0x00000001
#define	IMG_ORDER_102		0x00000002
#define	IMG_ORDER_201		0x00000003
#define	IMG_ORDER_120		0x00000004
#define	IMG_ORDER_210		0x00000005

#define SET_IMAGE_SIZE				0x00000001
#define SET_IMAGE_POSITION			0x00000002
#define SET_IMAGE_OFFSET			0x00000004
#define SET_IMAGE_SCALE			0x00000008
#define SET_IMAGE_FORMAT			0x00000010
#define READ_IMAGE_POSITION		0x00000020
#define SET_IMAGE_OVERLAY_ORDER	0x00000040
#define SET_IMAGE_ALL				0x0000007F
#endif

#ifdef TCC_89XX_INCLUDE
//#include "TCC89x_Physical.h"
#define SHARED_MEM_PHYSADDR		0xA0000000


#define READ_FROM_PROC
#ifdef READ_FROM_PROC
extern unsigned int HW_MEM_BASE;
#else
//#define HW_MEM_BASE				0x47C00000 
#endif

#define HW_MEM_LIMIT			0x00400000
#define OUTBUFFER_BASE			HW_MEM_BASE
#define OUTBUFFER_LIMIT			0x00280000
#define INBUFFER_BASE			(HW_MEM_BASE + OUTBUFFER_LIMIT)
#define INBUFFER_LIMIT				0x00040000 
#define DATABUFFER_BASE		(HW_MEM_BASE + OUTBUFFER_LIMIT + INBUFFER_LIMIT)
#define DATABUFFER_LIMIT			0x00040000

#define SCALER_OUT1				(HW_MEM_BASE)
#define SCALER_OUT2				(HW_MEM_BASE + 4*1024*1024)
#define SCALER_LIMIT				0x400000

#define DFLT_BASE_WMV9_ROBASE			0x10004000
#define DFLT_BASE_MPEG2_ROBASE			0x10008000
#define DFLT_BASE_MPEG2_HTBASE			0x10016000
#define DFLT_BASE_MPEG4SP_ROBASE		0x10004000 
#define DFLT_BASE_MPEG4SP_HTBASE		0x10016000

#define MBMEM0BASE				0x10010000
#define MBMEM0BASE_SIZE				6144

#define DFLT_BASE_JPEGDEC_ROBASE		0x10004000
#define DFLT_BASE_JPEGENC_ROBASE		0x10008000
#define PHY_ADDR_R_PINGPONG_1			0x10014000
#define PHY_ADDR_R_PINGPONG_2			0x10016000

#define	IMG_CH0			2
#define	IMG_CH1			4
#define	IMG_CH2			8
#define	LCD_CH			1

#define	IMG_LIT_ENDIAN	((0x0)<<7)
#define	IMG_BIG_ENDIAN	((0x1)<<7)
#define	IMG_RGB			((0x0)<<4)
#define	IMG_YUV420			((0x1)<<4)
#define	IMG_YUV422			((0x3)<<4)
#define	IMG_YUV422_DISP	((0x7)<<4)
#define	IMG_BPP_1			((0x0)<<0)
#define	IMG_BPP_2			((0x1)<<0)
#define	IMG_BPP_4			((0x2)<<0)
#define	IMG_BPP_332		((0x3)<<0)
#define	IMG_BPP_444		((0x4)<<0)
#define	IMG_BPP_565		((0x5)<<0)
#define	IMG_BPP_555		((0x6)<<0)
#define	IMG_BPP_888		((0x7)<<0)

#define	PXDW_4				((0x0))
#define	PXDW_8				((0x1))
#define	PXDW_888			((0x2))
#define	PXDW_565			((0x3))
#define	PXDW_555			((0x4))
#define	PXDW_18			((0x5))
#define	PXDW_8_UY_SEQ		((0x6))
#define	PXDW_8_VY_SEQ		((0x7))
#define	PXDW_16_YU_SEQ	((0x8))
#define	PXDW_16_YV_SEQ	((0x9))

#define 	LSCALE1				((0x0))
#define 	LSCALE2				((0x1))
#define 	LSCALE3				((0x2))
#define 	LSCALE4				((0x3))
#define 	LSCALE8				((0x7))

#define	IMG_ORDER_012		0x00000000
#define	IMG_ORDER_021		0x00000001
#define	IMG_ORDER_102		0x00000002
#define	IMG_ORDER_201		0x00000003
#define	IMG_ORDER_120		0x00000004
#define	IMG_ORDER_210		0x00000005

#define SET_IMAGE_SIZE				0x00000001
#define SET_IMAGE_POSITION			0x00000002
#define SET_IMAGE_OFFSET			0x00000004
#define SET_IMAGE_SCALE			0x00000008
#define SET_IMAGE_FORMAT			0x00000010
#define READ_IMAGE_POSITION		0x00000020
#define SET_IMAGE_OVERLAY_ORDER	0x00000040
#define SET_IMAGE_ALL				0x0000007F
#endif

#define	TCC_MEMORY_DEVICE_NAME	"/dev/mem"


#define	TCC_VIRTUAL_DEVICE_NAME_SIZE	128


#ifdef TCC_89XX_INCLUDE
#define 			PHYSICAL_REG_SIZE			1024*4
#define 			INT_BASE					0xF0401000
#define 			CKC_BASE					0xF0400000

#define			MAX_STREAM_BUF			100   // kbyte unit
#define 			IMAGE_PHY_MMAP_ADDR 		0x45E00000
#define 			IMAGE_PHY_MMAP_LEN 		(1024 * 1024 * 30)
#endif

/******************************************************************************
* typedefs & structure
******************************************************************************/

typedef struct _DEC_MEM_SET{
	unsigned int OutputBufferBase;
	unsigned int OutputBufferSize;
	unsigned int InputBufferBase;
	unsigned int InputBufferSize;
	unsigned int DataBufferBase;
	unsigned int DataBufferSize;
}DEC_MEM_SET; // Decoder(Subcore) Physical Memory Base Setting

typedef struct  Virtual_Structrue
{
	char device_name[TCC_VIRTUAL_DEVICE_NAME_SIZE];
	int	fd;			// Virtual File Handler 
}Virtual_t; 


/******************************************************************************
* globals
******************************************************************************/


/******************************************************************************
* locals
******************************************************************************/

/******************************************************************************
* declarations
******************************************************************************/
int  Init_Virtual(Virtual_t * virtual, char * device_name);
int DeInit_Virtual(Virtual_t * virtual);
void *SetVirtual(Virtual_t * virtual,unsigned int physical_addr, unsigned int mem_size);
int  FreeVirtual(Virtual_t * virtual,void *virtual_ptr, unsigned int mem_size);

#ifdef  READ_FROM_PROC
extern void Set_Scaler_PhyAddress(unsigned int address, unsigned int offset);
#endif

unsigned int get_scaler_physical_address(void);
	
void sys_init_physical_size(void);
void* sys_malloc_virtual_addr(int* pDev, unsigned int PAddr, unsigned int uiSize);
void sys_free_virtual_addr(int* pDev, void* virtual_ptr, unsigned int mem_size);
void* sys_malloc_physical_addr(unsigned int uiSize);
void sys_free_physical_addr(void* pPtr,unsigned int uiSize);


#ifdef TCC_79XX_INCLUDE
extern int 			LCDC_Init(void);
extern void 			LCDC_DeInit(void);
extern void			LCDC_SetVideoImageBase (unsigned int uiType, unsigned int width, unsigned int height);
extern void			LCDC_SetVideoImageBaseWithOffset(unsigned int uiType, unsigned int addr, unsigned int width, unsigned int height, int offset);
extern void 			LCDC_SetUIImageBase(unsigned int uiType);
extern unsigned int 	LCDC_SetImageInfo (unsigned int uiFlag, unsigned int uiType, unsigned int uiWidth, unsigned int uiHeight, unsigned int uiHPos, unsigned int uiVPos, unsigned int uiHOffset0, unsigned int uiHOffset1, unsigned int uiHScale, unsigned int uiVScale, unsigned int uiFormat, unsigned int uiOverlayOrder);
#endif

#endif //_VIRTUAL_H__
