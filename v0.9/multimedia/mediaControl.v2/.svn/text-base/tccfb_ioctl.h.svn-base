#ifndef __TCCFB_IOCTL_CODE_H_
#define __TCCFB_IOCTL_CODE_H_

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
typedef enum {
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



#define TCC_LCD_FB_IOCTL_SET_R2YCONV_OPTION		0x30
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[2]		- [0]: LCDC Index. (0 - LCDC0, 1 - LCDC1)
                                    - [1]: Ref to RGB2YUV_CONV_OPTION_t Type
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_GET_R2YCONV_OPTION		0x31
/*----------------------------------------------------------------------------------------
 * [IN/OUT]	unsigned int	[2]		- (IN ) [0]: LCDC Index. (0 - LCDC0, 1 - LCDC1)
 *                                  - (OUT) [1]: Ref to RGB2YUV_CONV_OPTION_t Type
 *---------------------------------------------------------------------------------------*/

typedef enum {
	R2Y_CONV_OPTION_0 = 0,		// * Y = 0.299*R + 0.587G + 0.114B 
								// * Cb = -0.172*R - 0.339*G + 0.511*B + 128 
								// * Cr = 0.511*R - 0.428*G - 0.083*B + 128 
								// The range for "RGB" is 16 ~ 235, "Studio Color". 
								// The result is "Studio Color" - Normally SDTV. 

	R2Y_CONV_OPTION_1 = 1,		// * Y = 0.257*R + 0.504*G + 0.098*B + 16 
								// * Cb = -0.148*R - 0.291*G + 0.439*B + 128 
								// * Cr = 0.439*R - 0.368*G - 0.071*B + 128 	
								// The range for "RGB" is 0 ~ 255, "Computer System Color" 
								// The result is "Studio Color" - Normally SDTV. 

	R2Y_CONV_OPTION_2 = 2,		// * Y = 0.213*R + 0.715*G + 0.072*B 
								// * Cb = -0.117*R - 0.394*G + 0.511*B + 128 
								// * Cr = 0.511*R - 0.464*G - 0.047*B + 128 
								// The range for "RGB" is 16 ~ 235, "Studio Color".. 
								// The result is "Studio Color" - Normally HDTV. 

	R2Y_CONV_OPTION_3 = 3,		// * Y = 0.183*R + 0.614*G + 0.062*B + 16 
								// * Cb = -0.101*R - 0.338*G + 0.439*B + 128 
								// * Cr = 0.439*R - 0.399*G - 0.040*B + 128
								// The range for "RGB" is 0 ~ 255, "Computer System Color". 
								// The result is "Studio Color" - Normally HDTV. 
} RGB2YUV_CONV_OPTION_t;



#define TCC_LCD_FB_IOCTL_SET_IMG_Y2RCONV_OPTION		0x32
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[1]		- [1]: Ref to YUV2RGB_CONV_OPTION_t Type
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_GET_IMG_Y2RCONV_OPTION		0x33
/*----------------------------------------------------------------------------------------
 * [OUT]	unsigned int	[1]		- [1]: Ref to YUV2RGB_CONV_OPTION_t Type
 *---------------------------------------------------------------------------------------*/

typedef enum {
	Y2R_CONV_OPTION_0 = 0,		// * R = Y + 1.371 * (Cr - 128) 
								// * G = Y + 0.336 * (Cb - 128) - 0.698 * (Cr - 128) 
								// * B = Y + 1.732 * (Cb - 128) 
								// The range for "YCbCr" is 16 ~ 235, "Studio Color". 
								// The result is "Studio Color" - Normally SDTV. 

	Y2R_CONV_OPTION_1 = 1,		// * R = 1.164 * (Y - 16) + 1.596 * (Cr - 128) 
								// * G = 1.164 * (Y - 16) - 0.391 * (Cb - 128) - 0.813 * (Cr - 128) 
								// * B = 1.164 * (Y - 16) + 2.018 * (Cb - 128) 
								// The range for "YCbCr" is 16 ~ 235, "Studio Color". 
								// The result is "Computer System Color" - Normally SDTV. 

	Y2R_CONV_OPTION_2 = 2,		// * R = Y + 1.540 * (Cr - 128) 
								// * G = Y - 0.183 * (Cb - 128) - 0.459 * (Cr - 128) 
								// * B = Y + 1.816 * (Cb - 128) 
								// The range for "YCbCr" is 16 ~ 235, "Studio Color". 
								// The result is "Studio Color" - Normally HDTV.

	Y2R_CONV_OPTION_3 = 3,		// * R = 1.164 * (Y - 16) + 1.793 * (Cr - 128) 
								// * G = 1.164 * (Y - 16) - 0.213 * (Cb - 128) - 0.534 * (Cr - 128) 
								// * B = 1.164 * (Y - 16) + 2.115 * (Cb - 128) 
								// The range for "YCbCr" is 16 ~ 235, "Studio Color". 
								// The result is "Computer System Color" - Normally HDTV.

} YUV2RGB_CONV_OPTION_t;


#define TCC_LCD_FB_IOCTL_SET_COLOR_ENAHNCEMENT	0x20
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[4]		- [0]: LCDC Index. (0 - LCDC0, 1 - LCDC1)
 *									  [1]: Hue
 *									  [2]: Brightness
 *									  [3]: Contrast
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_GET_COLOR_ENAHNCEMENT	0x21
/*----------------------------------------------------------------------------------------
 * [IN/OUT]	unsigned int	[4]		- (IN ) [0]: LCDC Index. (0 - LCDC0, 1 - LCDC1)
 *									  (OUT) [1]: Hue
 *									  (OUT) [2]: Brightness
 *									  (OUT) [3]: Contrast
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_SET_IMAGE_ENAHNCEMENT	0x22
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[3]		- [0]: Hue
 *									  [1]: Brightness
 *									  [2]: Contrast
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_GET_IMAGE_ENAHNCEMENT	0x23
/*----------------------------------------------------------------------------------------
 * [OUT]	unsigned int	[3]		- [0]: Hue
 *									  [1]: Brightness
 *									  [2]: Contrast
 *---------------------------------------------------------------------------------------*/

//	Hue Calibration Register - 2¡¯s complement signed value 
//	* -30 ~ 30 degree 
//	* 0x80 for -30 degree 
//	* 0x00 for 0 degree for default value 
//	* 0x7F for about 30 degree 

//	Brightness Calibration Register - 2¡¯s complement signed value 
//	* -128 ~ 128 value 
//	* 0x80 for -128 offset 
//	* 0x00 for 0 offset 
//	* 0x7F for 127 offset

//	Contrast Calibration Register - 2¡¯s complement signed value 
//	* -4 ~ 4 
//	* 0x80 for -4.0 value 
//	* 0xFF for -1.0 value 
//	* 0x20 for 1.0 value 
//	* 0x7F for about 4.0 value 


#define TCC_LCD_FB_IOCTL_SET_OVP				0x50
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned char	[1]		- [0]: ovp
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_SET_M2M				0x51
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[1]		- [0]: 0 - disable, 1 - enable
 *
 * !! M2M1 -->(On-the-Fly)-->LCDC1.IMG2
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_SET_M2M_ONTHEFLY			0x52
/*----------------------------------------------------------------------------------------
* [IN ]	tccfb_m2m_info_t
*
*---------------------------------------------------------------------------------------*/
typedef struct _tccfb_m2m_info {
	int enable;		// 0 - disable, 1 - enable
	int lcdc;		// 0 - LCDC0,   1 - LCDC1
	int img;		// 0 - IMG0, 1 - IMG1, 2 - IMG2
	int m2m;		// 0 - M2MSC0,  1 - M2MSC1
	int src_fmt;	// m2m source image type
	int src_width;	// m2m source width
	int src_height;	// m2m source height
	int dst_fmt;	// m2m destination image type
	int dst_width;	// m2m destination width
	int dst_height;	// m2m destination height

	// status
	int use;
} tccfb_m2m_info_t;

#define M2MSC_SRCFMT_MASK				(0x0000002f)		// INTM[4], INTLV[3], TYPE[2:0]
#define M2MSC_SRCFMT_YUV422_SEQ_0		(0x00)				// YUV422 sequential mode 0	: Y0-Cb0-Y1-Cr0
#define M2MSC_SRCFMT_YUV422_SEQ_1		(0x01)				// YUV422 sequential mode 1	: Cb0-Y0-Cr0-Y1
//#define M2MSC_SRCFMT_YUV422_SEP		(0x02)				// YUV422 separate
//#define M2MSC_SRCFMT_YUV422_INV_0		(0x02+0x08)			// YUV422 interleaved mode 0 : Cb0-Cr0-Cb1-Cb1
//#define M2MSC_SRCFMT_YUV422_INV_1		(0x02+0x08+0x20)	// YUV422 interleaved mode 1 : Cr0-Cb0-Cr1-Cr1
//#define M2MSC_SRCFMT_YUV420_SEP		(0x03)				// YUV420 separate
//#define M2MSC_SRCFMT_YUV420_INV_0		(0x03+0x08)			// YUV420 interleaved mode 0 : Cb0-Cr0-Cb1-Cb1
//#define M2MSC_SRCFMT_YUV420_INV_1		(0x03+0x08+0x20)	// YUV420 interleaved mode 1 : Cr0-Cb0-Cr1-Cr1
#define M2MSC_SRCFMT_RGB565				(0x04)				// RGB565
#define M2MSC_SRCFMT_RGB555				(0x05)				// RGB555
#define M2MSC_SRCFMT_RGB444				(0x06)				// RGB444
#define M2MSC_SRCFMT_RGB454				(0x07)				// RGB454

#define M2MSC_DSTFMT_MASK				(0x0000002f)		// INTM[4], INTLV[3], TYPE[2:0]
#define M2MSC_DSTFMT_YUV422_SEQ_0		(0x00)				// YUV422 sequential mode 0	: Y0-Cb0-Y1-Cr0
#define M2MSC_DSTFMT_YUV422_SEQ_1		(0x01)				// YUV422 sequential mode 1	: Cb0-Y0-Cr0-Y1
//#define M2MSC_DSTFMT_YUV422_SEP		(0x02)				// YUV422 separate
//#define M2MSC_DSTFMT_YUV422_INV_0		(0x02+0x08)			// YUV422 interleaved mode 0 : Cb0-Cr0-Cb1-Cb1
//#define M2MSC_DSTFMT_YUV422_INV_1		(0x02+0x08+0x20)	// YUV422 interleaved mode 1 : Cr0-Cb0-Cr1-Cr1
//#define M2MSC_DSTFMT_YUV420_SEP		(0x03)				// YUV420 separate
//#define M2MSC_DSTFMT_YUV420_INV_0		(0x03+0x08)			// YUV420 interleaved mode 0 : Cb0-Cr0-Cb1-Cb1
//#define M2MSC_DSTFMT_YUV420_INV_1		(0x03+0x08+0x20)	// YUV420 interleaved mode 1 : Cr0-Cb0-Cr1-Cr1
#define M2MSC_DSTFMT_RGB565				(0x04)				// RGB565
#define M2MSC_DSTFMT_RGB555				(0x05)				// RGB555
#define M2MSC_DSTFMT_RGB444				(0x06)				// RGB444
#define M2MSC_DSTFMT_RGB454				(0x07)				// RGB454
/*---------------------------------------------------------------------------------------*/


//TVOUT Controller IOCTL CODE
#define TCC_LCD_FB_IOCTL_TVOUT_TYPE             0X60
/*----------------------------------------------------------------------------------------
 * [IN ]	unsigned int	[1]		- mode 
 *---------------------------------------------------------------------------------------*/

#define NTSC_M      0x0000
#define NTSC_M_J    0x0001
#define NTSC_N      0x0010
#define NTSC_N_J    0x0011
#define NTSC_443    0x0020
#define PAL_M       0x0100
#define PAL_N       0x0110
#define PAL_B       0x0120
#define PAL_G       0x0130
#define PAL_H       0x0140
#define PAL_I       0x0150
#define PSEUDO_NTSC 0x1000
#define PSEUDO_PAL  0x1010

#define TCC_LCD_FB_IOCTL_TVOUT_OPEN             0x61
/*----------------------------------------------------------------------------------------
 * 
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_TVOUT_CLOSE            0x62
/*----------------------------------------------------------------------------------------
 * 
 *---------------------------------------------------------------------------------------*/

#define TCC_LCD_FB_IOCTL_TVOUT_CONNECT_LCDC		0x63
/*----------------------------------------------------------------------------------------
 * 
 *---------------------------------------------------------------------------------------*/


#endif 


