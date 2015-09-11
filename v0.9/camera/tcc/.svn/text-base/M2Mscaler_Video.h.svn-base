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

/******************************************************************************
* include 
******************************************************************************/
#ifndef	_M2M_SCALER_H__
#define	_M2M_SCALER_H__
extern void M2M_ScalerForVideo_Enable(int yuv_mode);
extern void M2M_ScalerForVideo_Disable(void);
extern void M2M_ScalerForVideo( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize, unsigned int s_y, unsigned int d_y ,unsigned int v_offset, unsigned int h_offset);
extern void M2M_ScalerForVideo_With_Address( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize, unsigned int s_y, unsigned int s_u, unsigned int s_v, unsigned int d_y , unsigned int v_offset, unsigned int h_offset, int hdmi_flag) ;
extern void M2M_ScalerForVideo_With_Offset( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize, unsigned int s_y, unsigned int s_u, unsigned int s_v, unsigned int d_y , unsigned int w_offset, int hdmi_flag) ;
extern void M2M_ScalerForVideo_Interlaced( unsigned int s_hsize, unsigned int s_vsize, unsigned int m_hsize, unsigned int m_vsize,unsigned int s_hsize_offset,unsigned int s_y, unsigned int d_y , unsigned int v_offset, unsigned int h_offset);
extern void M2M_ScalerForVideo_Enable1(void);
extern void M2M_ScalerForVideo_Disable1(void);
extern void M2M_ScalerForVideo_DVDSubpic( unsigned int s_hsize, unsigned int s_vsize, unsigned int d_hsize, unsigned int d_vsize, unsigned int srcaddr,unsigned int dstaddr,unsigned int colormode); 
extern unsigned int M2M_ScalerForVideo_GetSrcVAddr(void);
extern unsigned int M2M_ScalerForVideo_GetDstVAddr(void);
extern unsigned int M2M_ScalerForVideo_GetSrcPAddr(void);
extern unsigned int M2M_ScalerForVideo_GetDstPAddr(void);
#endif //M2M_SCALER

