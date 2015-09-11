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
 *		vpu_clk_ctrl.h
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
#ifndef _VPU_CLK_CTRL_H_
#define _VPU_CLK_CTRL_H_

#include "sys_memory.h"

#ifdef VPU_CLK_CONTROL

/************************************************************************/
/*						                                                */
/* Defines				                                                */
/*						                                                */
/************************************************************************/


#define VPU_CLK_CTRL_VBUS_PWR_DOWN_REG			0xF0702000
#define VPU_CLK_CTRL_VBUS_PWR_DOWN_CODEC_MASK	(1<<2)

#define VPU_CLK_CTRL_CLKCTRL_REG_BASE			0xF0400000	//CLK Register Base Address
#define VPU_CLK_CTRL_CLKCTRL5_REG				0xF0400014	//Bus Clock Control Register for VIdeo Codec
#define VPU_CLK_CTRL_CLKCTRL6_REG				0xF0400018	//Core Clock Control Register for VIdeo Codec
#define VPU_CLK_CTRL_CKLCTRL_SEL				( 0)
#define VPU_CLK_CTRL_CKLCTRL_SYNC				( 3)
#define VPU_CLK_CTRL_CKLCTRL_CONFIG				( 4)
#define VPU_CLK_CTRL_CKLCTRL_MD					(20)
#define VPU_CLK_CTRL_CKLCTRL_EN					(21)

/*!
 ***********************************************************************
 * \brief
 *		vpu_clock_init	: vpu clock init. function for LINUX/WINCE
 ***********************************************************************
 */
int
vpu_clock_init(void);

/*!
 ***********************************************************************
 * \brief
 *		vpu_clock_deinit : vpu clock deinit. function for LINUX/WINCE
 ***********************************************************************
 */
int
vpu_clock_deinit(void);

#endif
#endif //_VPU_CLK_CTRL_H_
