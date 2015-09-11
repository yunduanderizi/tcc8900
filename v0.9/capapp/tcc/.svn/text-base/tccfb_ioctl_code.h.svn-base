/****************************************************************************
 *   FileName    : tccfb_ioctl_code.h
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/
#ifndef __TCCFB_IOCTL_CODE_H_
#define __TCCFB_IOCTL_CODE_H_

#ifdef TCC_79XX_INCLUDE
//
#define TCC_LCD_FB_IOCTL_DISP_ONOFF				0x10
#define TCC_LCD_FB_IOCTL_ALPHA_ONOFF			0x11
#define TCC_LCD_FB_IOCTL_ALPHA_SET				0x12
#define TCC_LCD_FB_IOCTL_CHROMAKEY_ONOFF		0x13
#define TCC_LCD_FB_IOCTL_CHROMAKEY_SET_VALUE	0x14
#define TCC_LCD_FB_IOCTL_CHROMAKEY_SET_MASK		0x15

// LCDC Info set/get contorl (new method!)
// refer to tcc_lcdc_info.h
#define TCC_LCD_FB_IOCTL_LCDC_SETALLINFO        0x20
#define TCC_LCD_FB_IOCTL_LCDC_GETALLINFO        0x21
#define TCC_LCD_FB_IOCTL_LCDC_SETDISPLAYINFO    0x22
#define TCC_LCD_FB_IOCTL_LCDC_GETDISPLAYINFO    0x23
#define TCC_LCD_FB_IOCTL_LCDC_SETINTERFACEINFO  0x24
#define TCC_LCD_FB_IOCTL_LCDC_GETINTERFACEINFO  0x25
#define TCC_LCD_FB_IOCTL_LCDC_SETIMGCH0INFO     0x26
#define TCC_LCD_FB_IOCTL_LCDC_SETIMGCH1INFO     0x27
#define TCC_LCD_FB_IOCTL_LCDC_SETIMGCH2INFO     0x28
#define TCC_LCD_FB_IOCTL_LCDC_GETIMGCH0INFO     0x29
#define TCC_LCD_FB_IOCTL_LCDC_GETIMGCH1INFO     0x2A
#define TCC_LCD_FB_IOCTL_LCDC_GETIMGCH2INFO     0x2B
#define TCC_LCD_FB_IOCTL_LCDC_SETLAYER1INFO     0x2C
#define TCC_LCD_FB_IOCTL_LCDC_SETLAYER2INFO     0x2D
#define TCC_LCD_FB_IOCTL_LCDC_GETLAYER1INFO     0x2E
#define TCC_LCD_FB_IOCTL_LCDC_GETLAYER2INFO     0x2F

//TVOUT Controller IOCTL CODE
//(for TCC79x)
#define TCC_LCD_FB_IOCTL_TVOUT_TYPE				0x60
#define TCC_LCD_FB_IOCTL_TVOUT_OPEN				0x61
#define TCC_LCD_FB_IOCTL_TVOUT_CLOSE			0x62
#define NTSC_M			0x0000
#define NTSC_M_J			0x0001
#define NTSC_N			0x0010
#define NTSC_N_J			0x0011
#define NTSC_443			0x0020
#define PAL_M			0x0100
#define PAL_N			0x0110
#define PAL_B			0x0120
#define PAL_G			0x0130
#define PAL_H			0x0140
#define PAL_I				0x0150
#define PSEUDO_NTSC 		0x1000
#define PSEUDO_PAL		0x1010
#endif //#ifdef TCC_79XX_INCLUDE


#ifdef TCC_89XX_INCLUDE
//
#define TCC_LCD_FB_IOCTL_DISP_ONOFF				0x10
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[1]		- 0: Off, 1: On
 *---------------------------------------------------------------------------------------*/
 

#define TCC_LCD_FB_IOCTL_ALPHA_ONOFF			0x11
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[1]		- 0: Off, 1: On
 *---------------------------------------------------------------------------------------*/
 

#define TCC_LCD_FB_IOCTL_ALPHA_SET				0x12
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned char	[1]		- Alpah value(0~0xFF)
 *---------------------------------------------------------------------------------------*/
 

#define TCC_LCD_FB_IOCTL_CHROMAKEY_ONOFF		0x13
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[1]		- 0: Off, 1: On
 *---------------------------------------------------------------------------------------*/
 

#define TCC_LCD_FB_IOCTL_CHROMAKEY_SET_VALUE	0x14
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned char	[3]		- [0]: red
 *									  [1]: green
 *									  [2]: blue
 *---------------------------------------------------------------------------------------*/


#define TCC_LCD_FB_IOCTL_CHROMAKEY_SET_MASK		0x15
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned char	[3]		- [0]: red
 *									  [1]: green
 *									  [2]: blue
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_ALPHA_SELECTION		0x16	
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[1]		- 0 : A10 bits and A11 are used as alpha value
 * 									  1 : Alpha bits in the pixel are used as alpha value.
 *---------------------------------------------------------------------------------------*/


#define TCC_LCD_FB_IOCTL_SET_FORMAT				0x17
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[1]		- Ref to IMGCH_FMT_t Type
 *---------------------------------------------------------------------------------------*/
enum {
	IMGFMT_1BPP		= 0,	// 1bpp indexed color
	IMGFMT_2BPP		= 1,	// 2bpp indexed color
	IMGFMT_4BPP		= 2,	// 4bpp indexed color
	IMGFMT_8BPP		= 3,	// 8bpp indexed color
	//...
	IMGFMT_RGB332	= 8,	// RGB332 - 1bytes aligned - R[7:5],G[4:2],B[1:0]
	IMGFMT_RGB444	= 9,	// RGB444 - 2bytes aligned - A[15:12],R[11:8],G[7:3],B[3:0]
	IMGFMT_RGB565	= 10,	// RGB565 - 2bytes aligned - R[15:11],G[10:5],B[4:0]
	IMGFMT_RGB555	= 11,	// RGB555 - 2bytes aligned - A[15],R[14:10],G[9:5],B[4:0]
	IMGFMT_RGB888	= 12,	// RGB888 - 4bytes aligned - A[31:24],R[23:16],G[15:8],B[7:0]
	IMGFMT_RGB666	= 13,	// RGB666 - 4bytes aligned - A[23:18],R[17:12],G[11:6],B[5:0]
	//...
	IMGFMT_YUV420	= 24,	// YCbCr 4:2:0 Separated format - Not Supported for Image 1 and 2
	IMGFMT_YUV422	= 25,	// YCbCr 4:2:2 Separated format - Not Supported for Image 1 and 2
	IMGFMT_YUV422P	= 26,	// YCbCr 4:2:2 Sequential format
	//...
	IMGFMT_YUV420I0	= 28,	// YCbCr 4:2:0 interleved type 0 format - Not Supported for Image 1 and 2
	IMGFMT_YUV420I1	= 29,	// YCbCr 4:2:0 interleved type 1 format - Not Supported for Image 1 and 2
	IMGFMT_YUV422I0	= 30,	// YCbCr 4:2:2 interleved type 0 format - Not Supported for Image 1 and 2
	IMGFMT_YUV422I1	= 31,	// YCbCr 4:2:2 interleved type 1 format - Not Supported for Image 1 and 2

} IMGCH_FMT_t;
/*---------------------------------------------------------------------------------------*/


#define TCC_LCD_FB_IOCTL_SET_BASEADDR			0x18
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[3]		- [0]: base0 - IMGx base address. IMG0 Y base address.
 *									  [1]: base1 -                    IMG0 U base address.
 *									  [2]: base2 -                    IMG0 V base address. 
 *---------------------------------------------------------------------------------------*/


#define TCC_LCD_FB_IOCTL_GET_BASEADDR			0x19
/*----------------------------------------------------------------------------------------
 * [OUT]	unsigned int	[3]		- [0]: base0 - IMGx base address. IMG0 Y base address.
 *									  [1]: base1 -                    IMG0 U base address.
 *									  [2]: base2 -                    IMG0 V base address. 
 *---------------------------------------------------------------------------------------*/


#define TCC_LCD_FB_IOCTL_SET_ADDROFFSET			0x1A
/*----------------------------------------------------------------------------------------
 * [OUT]	unsigned int	[2]		- [0]: offset0 - Address Offset Y or RGB channel of FIFO(0)
 *									  [1]: offset1 - Address Offset In U or V channel of FIFO(1,2)
 *                                                   Invalid for Image1 and Image2
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_SET_IMGWINDOW			0x1B
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[4]		- [0]: Image start offset X
 *									  [1]: Image start offset Y
 *									  [2]: Image width
 *									  [3]: Image height
 *---------------------------------------------------------------------------------------*/


#define TCC_LCD_FB_IOCTL_LCD_ONOFF				0x1C
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[1]		- 0: Off, 1: On
 *---------------------------------------------------------------------------------------*/

#endif //#ifdef TCC_89XX_INCLUDE

#endif //__TCCFB_IOCTL_CODE_H_




