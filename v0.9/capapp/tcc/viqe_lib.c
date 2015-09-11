/* vim:set ts=4 shiftwidth=4 */

#include 	<stdio.h>
#include 	<math.h>
#include	"dev_viqe.h"

#include	"viqe_lib.h"
#include 	"viqe_cmd.h"
#include "gamut_map.h"

#define VIQE_ASGN // no bitfield structure
#define LINUX_OS_INCLUDE
#ifdef LINUX_OS_INCLUDE
#ifndef VIQE_ASGN
#define VIQE_ASGN
#endif
#else	// LINUX_OS_INCLUDE
#include	"tcc_type.h"
#endif	// LINUX_OS_INCLUDE

//#define LINUX_OS_INCLUDE
#if 0 //ndef LINUX_OS_INCLUDE
#define	HwVIPET			((volatile VIPET *)0xf0252000)
#define	HwVIPET1		((volatile VIPET *)0xf0253000)
#define	pDDIConfigReg	((volatile DDICONFIG *) 0xF0251000)
#else
#include "TCC89x_Structures.h"
#include "TCC89x_Physical.h"
#include "TCCUtil.h"
#define LOGICAL_ADDR_MIN	(4*1024)


#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>         // O_RDWR
#include <sys/poll.h>
volatile VIPET 			*HwVIPET;
//volatile VIPET 			*HwVIPET1;
static int 				viqe_dev_fd = 0;
volatile DDICONFIG		*pDDIConfigReg = 0;
#endif	// LINUX_OS_INCLUDE

int						gi_viqe_done;

#define	VERYLOW		0
#define	LOW		1
#define	MEDIUM		2
#define	STRONG		3
#define	VERYSTRONG	4

#define ABS(A)	( (A) > 0 ? (A) : (-A) )

#define min(a,b)  (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))
static yuvpixel YUVCube [9][9][9];
#define IlluminantC 0.3101, 0.3162
#define IlluminantD65   0.3127, 0.3291
#define GAMMA_REC709 0.

static struct colourSystem const
    NTSCsystem  =  { "NTSC",               0.67,  0.33,  0.21,  0.71,  0.14,  0.08,  IlluminantC,   GAMMA_REC709},
    EBUsystem   =  { "EBU (PAL/SECAM)",    0.64,  0.33,  0.29,  0.60,  0.15,  0.06,  IlluminantD65, GAMMA_REC709},
    SMPTEsystem =  { "SMPTE",             0.630, 0.340, 0.310, 0.595, 0.155, 0.070, IlluminantD65,  GAMMA_REC709},
    HDTVsystem  =  { "HDTV",              0.670, 0.330, 0.210, 0.710, 0.150, 0.060, IlluminantD65,  GAMMA_REC709},
/* Huh? -ajsh
    CIEsystem   =  { "CIE",              0.7355,0.2645,0.2658,0.7243,0.1669,0.0085, 0.3894,0.3324,  GAMMA_REC709},
*/
    CIEsystem   =  { "CIE",              0.7355,0.2645,0.2658,0.7243,0.1669,0.0085, 0.33333333,0.33333333,  GAMMA_REC709},
    Rec709system = { "CIE REC 709",        0.64,  0.33,  0.30,  0.60,  0.15,  0.06,    IlluminantD65, GAMMA_REC709};

/* Customsystem  is a variable that is initialized to CIE Rec 709, but
   we modify it with information specified by the user's options.
*/
static struct colourSystem Customsystem = 
{ "Custom",             0.64,  0.33,  0.30,  0.60,  0.15,  0.06,  
  IlluminantD65, GAMMA_REC709};
static int gDeintlOnOff = 0;
static int gOddFirstOnOff = 0;

int
VIQE_check_filmmode( int frmnum ) {

 if ( HwVIPET->uDI_PD_STATUS.bDI_PD_STATUS.check_out == 0 ) {
//  printf( "[%8d] : check_out is zero\n", frmnum );
  return -1;
 }
 return 1;
}
//#ifdef LINUX_OS_INCLUDE
int 
tcc_viqe_init() {
	int mem;

	viqe_dev_fd = open("/dev/mem", O_RDWR | O_NDELAY);
	if(viqe_dev_fd<0)
	{
		printf("physical memory open error!!\n");
		return -1;  	
	}

	mem = (int )mmap(0, 2*LOGICAL_ADDR_MIN, PROT_READ | PROT_WRITE, MAP_SHARED, viqe_dev_fd, 0xF0252000);//&HwVIQE_BASE);
	if(mem<0)
	{
		printf("VIQE BASE mmap fail!\n");
		return -1;
	}
	HwVIPET = mem;

	mem = (int)mmap(0, LOGICAL_ADDR_MIN, PROT_READ | PROT_WRITE, MAP_SHARED, viqe_dev_fd, 0xF0251000);//&HwDDI_CONFIG_BASE);
	if(mem<0)
	{
		printf("DDI Control register memory open error!!\n");
		return -1;
	}
	pDDIConfigReg = (volatile DDICONFIG	*)mem;

	BITSET(pDDIConfigReg->SWRESET,	HwSWRESET_VIQE);
	BITCLR(pDDIConfigReg->PWDN, 	HwBCLKCTR_VIQE);
	BITCLR(pDDIConfigReg->SWRESET,	HwSWRESET_VIQE);

	viqe_set_disable_all();
	viqe_clear_internal();
	
	return 0;
}

void 
tcc_viqe_deinit( void ) {	
	BITSET(pDDIConfigReg->SWRESET,	HwSWRESET_VIQE);
	BITSET(pDDIConfigReg->PWDN, 	HwBCLKCTR_VIQE);
	BITCLR(pDDIConfigReg->SWRESET,	HwSWRESET_VIQE);		

	if(HwVIPET)
		munmap((void *)HwVIPET, 2*LOGICAL_ADDR_MIN);
	
	if(pDDIConfigReg)
		munmap((void *)pDDIConfigReg, LOGICAL_ADDR_MIN);

	if(viqe_dev_fd)
		close(viqe_dev_fd);	
}


// -----------------------------------------------------------------
// viqe reset
//
// 		input : 	-
// 		output :	-
// -----------------------------------------------------------------
void
viqe_swreset( void )	{

#ifdef LINUX_OS_INCLUDE
	BITSET(pDDIConfigReg->SWRESET,	HwSWRESET_VIQE);
	BITCLR(pDDIConfigReg->SWRESET,	HwSWRESET_VIQE);	
#else	
	*(unsigned int *)0xf0251034 |= (1<<1);
	*(unsigned int *)0xf0251034 &= ~(1<<1);	
#endif
}

// -----------------------------------------------------------------
// clearing internal states and internal registers in all blocks
//
// 		input : 	-
// 		output :	return success or fail(-1)
// -----------------------------------------------------------------
int
viqe_clear_internal( void )	{
	int		ret = 0;
	
	// avoid clkgating...
	HwVIPET->uTOP_CLKGATE.nTOP_CLKGATE |= 0x3F; // 20090429

	viqe_swreset();

#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_MISC_CTRL.nTOP_MISC_CTRL |= 0xE0000000;	// 20090429
#else
	HwVIPET->uTOP_MISC_CTRL.bTOP_MISC_CTRL.fmt_conv_flush = 1;
	HwVIPET->uTOP_MISC_CTRL.bTOP_MISC_CTRL.lumadly_flush = 1;
	HwVIPET->uTOP_MISC_CTRL.bTOP_MISC_CTRL.tmpfifo_flush = 1;
#endif

	// di
#ifdef VIQE_ASGN		// 20090428
	// 20090428
	HwVIPET->uDI_CTRL.nDI_CTRL |= 0xC0000000;	
#else
	HwVIPET->uDI_CTRL.bDI_CTRL.flush_synch_fifo	= 1;
	HwVIPET->uDI_CTRL.bDI_CTRL.internal_reg_clear = 1;
#endif

	// dn
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uDN_CTRL.nDN_CTRL |= 0xC0000000;	
	HwVIPET->uDN_CTRL.nDN_CTRL &= ~0xC0000000;	
#else
	HwVIPET->uDN_CTRL.bDN_CTRL.fifo_flush = 3;
	HwVIPET->uDN_CTRL.bDN_CTRL.fifo_flush = 0;
#endif	
	
	// rdma2
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_CTRL.nRD_CTRL |= 0x00010000;	
	HwVIPET->uRD_CTRL.nRD_CTRL &= ~0x00010000;	
#else
	HwVIPET->uRD_CTRL.bRD_CTRL.clear_decomp_buff = 1;
	HwVIPET->uRD_CTRL.bRD_CTRL.clear_decomp_buff = 0;
#endif	
	
	// cdma
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uCD_CTRL.nCD_CTRL |= 0x00008000;		
	HwVIPET->uCD_CTRL.nCD_CTRL &= ~0x00008000;			
#else
	HwVIPET->uCD_CTRL.bCD_CTRL.clear_comp_buff = 1;
	HwVIPET->uCD_CTRL.bCD_CTRL.clear_comp_buff = 0;
#endif	

	// hi
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uHI_CTRL.nHI_CTRL |= 0x80000000;		
	HwVIPET->uHI_CTRL.nHI_CTRL &= ~0x80000000;
#else
	HwVIPET->uHI_CTRL.bHI_CTRL.clear_internal_vars = 1;	
	HwVIPET->uHI_CTRL.bHI_CTRL.clear_internal_vars = 0;	
#endif	
	
	// outdma
	HwVIPET->uOD_CONTROL.nOD_CONTROL |= ( ((unsigned int)(1)<<31)|(1<<8) );
	HwVIPET->uOD_CONTROL.nOD_CONTROL &= 0x7ffffeff;

#ifdef VIQE_ASGN		// 20090428
	ret = 	( HwVIPET->uTOP_MISC_CTRL.nTOP_MISC_CTRL & 0xE0000000 ) |
			( HwVIPET->uDI_CTRL.nDI_CTRL & 0xC0000000 )	|
			( HwVIPET->uDN_CTRL.nDN_CTRL & 0xC0000000 ) |
			( HwVIPET->uRD_CTRL.nRD_CTRL & 0x00010000) |
			( HwVIPET->uCD_CTRL.nCD_CTRL & 0x00008000) |
			( HwVIPET->uHI_CTRL.nHI_CTRL & 0x80000000 );
#else
	ret = 	( HwVIPET->uTOP_MISC_CTRL.bTOP_MISC_CTRL.fmt_conv_flush & 1 ) |
			( HwVIPET->uTOP_MISC_CTRL.bTOP_MISC_CTRL.lumadly_flush & 1 ) |
			( HwVIPET->uTOP_MISC_CTRL.bTOP_MISC_CTRL.tmpfifo_flush & 1 ) |
			( HwVIPET->uDI_CTRL.bDI_CTRL.flush_synch_fifo & 1 )	|
			( HwVIPET->uDI_CTRL.bDI_CTRL.internal_reg_clear & 1 ) |
			( HwVIPET->uDN_CTRL.bDN_CTRL.fifo_flush & 3 ) |
			( HwVIPET->uRD_CTRL.bRD_CTRL.clear_decomp_buff & 1 ) |
			( HwVIPET->uCD_CTRL.bCD_CTRL.clear_comp_buff & 1 ) |
			( HwVIPET->uHI_CTRL.bHI_CTRL.clear_internal_vars & 1 );
#endif

	// reset clkgating...
	HwVIPET->uTOP_CLKGATE.nTOP_CLKGATE = 0;

	return ( ret == 0 )? 1 : -1;
}

// -----------------------------------------------------------------
// register writing of viqe_dma_t structure
//
// 		input : 	pointer of viqe_dma_t structure
// 		output :	base0 luminance address
// -----------------------------------------------------------------
unsigned int
viqe_set_dma( viqe_dma_t *dma )	{
	uint		cdma_comp0[3];
	uint		cdma_comp1[3];

	// control
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_CTRL.nRD_CTRL = (dma->opt 	? HwVIPET->uRD_CTRL.nRD_CTRL|Hw4 : HwVIPET->uRD_CTRL.nRD_CTRL&(~Hw4));
	HwVIPET->uRD_CTRL.nRD_CTRL = (dma->conti 	? HwVIPET->uRD_CTRL.nRD_CTRL|Hw8 : HwVIPET->uRD_CTRL.nRD_CTRL&(~Hw8));
	HwVIPET->uCD_CTRL.nCD_CTRL = (dma->opt		? HwVIPET->uCD_CTRL.nCD_CTRL|Hw4 : HwVIPET->uCD_CTRL.nCD_CTRL&(~Hw4));	
#else
	HwVIPET->uRD_CTRL.bRD_CTRL.rdopt = dma->opt;
	HwVIPET->uRD_CTRL.bRD_CTRL.rdma_cont = dma->conti;
	HwVIPET->uCD_CTRL.bCD_CTRL.cdopt = dma->opt;
#endif	

	// cur
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_IMG0_BASE0.nRD_BASEADDR = dma->addr_cur[0];
	HwVIPET->uRD_IMG0_BASE1.nRD_BASEADDR = dma->addr_cur[1];
	HwVIPET->uRD_IMG0_BASE2.nRD_BASEADDR = dma->addr_cur[2];
#else
	HwVIPET->uRD_IMG0_BASE0.bRD_BASEADDR.baseaddr = dma->addr_cur[0];
	HwVIPET->uRD_IMG0_BASE1.bRD_BASEADDR.baseaddr = dma->addr_cur[1];
	HwVIPET->uRD_IMG0_BASE2.bRD_BASEADDR.baseaddr = dma->addr_cur[2];
#endif	

	// pre
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_IMG1_BASE0.nRD_BASEADDR = dma->addr_pre[0];
	HwVIPET->uRD_IMG1_BASE1.nRD_BASEADDR = dma->addr_pre[1];
	HwVIPET->uRD_IMG1_BASE2.nRD_BASEADDR = dma->addr_pre[2];
#else
	HwVIPET->uRD_IMG1_BASE0.bRD_BASEADDR.baseaddr = dma->addr_pre[0];
	HwVIPET->uRD_IMG1_BASE1.bRD_BASEADDR.baseaddr = dma->addr_pre[1];
	HwVIPET->uRD_IMG1_BASE2.bRD_BASEADDR.baseaddr = dma->addr_pre[2];
#endif	

	// width offset for cur, pre
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_IMG0_OFS.nRD_BASEOFS = ((dma->i_offs[1]<<16) | (dma->i_offs[0]));
#else
	HwVIPET->uRD_IMG0_OFS.bRD_BASEOFS.base_ofs1 = (unsigned short)dma->i_offs[1];
	HwVIPET->uRD_IMG0_OFS.bRD_BASEOFS.base_ofs0 = (unsigned short)dma->i_offs[0];
#endif

#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_IMG1_OFS.nRD_BASEOFS = ((dma->i_offs[1]<<16) | (dma->i_offs[0]));
#else
	HwVIPET->uRD_IMG1_OFS.bRD_BASEOFS.base_ofs0 = (unsigned short)dma->i_offs[0];
	HwVIPET->uRD_IMG1_OFS.bRD_BASEOFS.base_ofs1 = (unsigned short)dma->i_offs[1];
#endif	

	// de-compressor dma
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_CTRL.nRD_CTRL = ((!dma->comp_pingpong) ? HwVIPET->uRD_CTRL.nRD_CTRL|Hw6 : HwVIPET->uRD_CTRL.nRD_CTRL&(~Hw6)); // 20090429
#else
	HwVIPET->uRD_CTRL.bRD_CTRL.toggle = !dma->comp_pingpong;
#endif	

#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_IMG2_BASE0_0.nRD_BASEADDR = dma->addr_comp0[0];
	HwVIPET->uRD_IMG2_BASE1_0.nRD_BASEADDR = dma->addr_comp0[1];
	HwVIPET->uRD_IMG2_BASE2_0.nRD_BASEADDR = dma->addr_comp0[2];
#else
	HwVIPET->uRD_IMG2_BASE0_0.bRD_BASEADDR.baseaddr = dma->addr_comp0[0];
	HwVIPET->uRD_IMG2_BASE1_0.bRD_BASEADDR.baseaddr = dma->addr_comp0[1];
	HwVIPET->uRD_IMG2_BASE2_0.bRD_BASEADDR.baseaddr = dma->addr_comp0[2];
#endif	

	//if ( !dma->addr_comp1[0] | !dma->addr_comp1[1] | !dma->addr_comp1[2] )	{	
	if (0 ) {
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uRD_IMG2_BASE0_1.nRD_BASEADDR = dma->addr_comp0[0];
		HwVIPET->uRD_IMG2_BASE1_1.nRD_BASEADDR = dma->addr_comp0[1];
		HwVIPET->uRD_IMG2_BASE2_1.nRD_BASEADDR = dma->addr_comp0[2];		
#else
		HwVIPET->uRD_IMG2_BASE0_1.bRD_BASEADDR.baseaddr = dma->addr_comp0[0];
		HwVIPET->uRD_IMG2_BASE1_1.bRD_BASEADDR.baseaddr = dma->addr_comp0[1];
		HwVIPET->uRD_IMG2_BASE2_1.bRD_BASEADDR.baseaddr = dma->addr_comp0[2];
#endif	
		
	}
	else	{
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uRD_IMG2_BASE0_1.nRD_BASEADDR = dma->addr_comp1[0];
		HwVIPET->uRD_IMG2_BASE1_1.nRD_BASEADDR = dma->addr_comp1[1];
		HwVIPET->uRD_IMG2_BASE2_1.nRD_BASEADDR = dma->addr_comp1[2];
#else
		HwVIPET->uRD_IMG2_BASE0_1.bRD_BASEADDR.baseaddr = dma->addr_comp1[0];
		HwVIPET->uRD_IMG2_BASE1_1.bRD_BASEADDR.baseaddr = dma->addr_comp1[1];
		HwVIPET->uRD_IMG2_BASE2_1.bRD_BASEADDR.baseaddr = dma->addr_comp1[2];
#endif			
	}

	// compressor dma	
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uCD_CTRL.nCD_CTRL = ((!(dma->comp_pingpong)) ? HwVIPET->uCD_CTRL.nCD_CTRL|Hw5 : HwVIPET->uCD_CTRL.nCD_CTRL&(~Hw5));
#else
	HwVIPET->uCD_CTRL.bCD_CTRL.toggle = !dma->comp_pingpong;
#endif	

	if ( dma->comp_pingpong )	{
		cdma_comp0[0] = dma->addr_comp1[0];
		cdma_comp0[1] = dma->addr_comp1[1];
		cdma_comp0[2] = dma->addr_comp1[2];
		cdma_comp1[0] = dma->addr_comp0[0];
		cdma_comp1[1] = dma->addr_comp0[1];
		cdma_comp1[2] = dma->addr_comp0[2];		
	}
	else	{
		cdma_comp0[0] = dma->addr_comp0[0];
		cdma_comp0[1] = dma->addr_comp0[1];
		cdma_comp0[2] = dma->addr_comp0[2];
		cdma_comp1[0] = dma->addr_comp1[0];
		cdma_comp1[1] = dma->addr_comp1[1];
		cdma_comp1[2] = dma->addr_comp1[2];		
	}
	
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uCD_BASE0_0.nCD_BASEADDR = cdma_comp0[0];
	HwVIPET->uCD_BASE1_0.nCD_BASEADDR = cdma_comp0[1];
	HwVIPET->uCD_BASE2_0.nCD_BASEADDR = cdma_comp0[2];
#else
	HwVIPET->uCD_BASE0_0.bCD_BASEADDR.baseaddr = cdma_comp0[0];
	HwVIPET->uCD_BASE1_0.bCD_BASEADDR.baseaddr = cdma_comp0[1];
	HwVIPET->uCD_BASE2_0.bCD_BASEADDR.baseaddr = cdma_comp0[2];
#endif	
	
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uCD_BASE0_1.nCD_BASEADDR = cdma_comp1[0];
	HwVIPET->uCD_BASE1_1.nCD_BASEADDR = cdma_comp1[1];
	HwVIPET->uCD_BASE2_1.nCD_BASEADDR = cdma_comp1[2];
#else
	HwVIPET->uCD_BASE0_1.bCD_BASEADDR.baseaddr = cdma_comp1[0];
	HwVIPET->uCD_BASE1_1.bCD_BASEADDR.baseaddr = cdma_comp1[1];
	HwVIPET->uCD_BASE2_1.bCD_BASEADDR.baseaddr = cdma_comp1[2];
#endif		

	// outdma
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uOD_BASE0.nOD_BASEADDR = dma->addr_out[0];
	HwVIPET->uOD_BASE1.nOD_BASEADDR = dma->addr_out[1];
	HwVIPET->uOD_BASE2.nOD_BASEADDR = dma->addr_out[2];
#else
	HwVIPET->uOD_BASE0.bOD_BASEADDR.baseaddr = dma->addr_out[0];
	HwVIPET->uOD_BASE1.bOD_BASEADDR.baseaddr = dma->addr_out[1];
	HwVIPET->uOD_BASE2.bOD_BASEADDR.baseaddr = dma->addr_out[2];
#endif	

#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uOD_OFS.nOD_BASEOFS = (dma->o_offs[1]<<16 | dma->o_offs[0]);
#else
	HwVIPET->uOD_OFS.bOD_BASEOFS.base_ofs0 = dma->o_offs[0];
	HwVIPET->uOD_OFS.bOD_BASEOFS.base_ofs1 = dma->o_offs[1];
#endif

#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uOD_CFG.nOD_CFG |= Hw6;
#else
	HwVIPET->uOD_CFG.bOD_CFG.outp_ready = ON;
#endif	
	
#ifdef VIQE_ASGN		// 20090428
	return ( HwVIPET->uRD_IMG0_BASE0.nRD_BASEADDR);
#else
	return ( HwVIPET->uRD_IMG0_BASE0.bRD_BASEADDR.baseaddr );
#endif		
}

// -----------------------------------------------------------------
// register writing of varying dma addresses per frame processing
//
// 		input : 	pointer of viqe_dma_t structure
// 		output :	base0 luminance address
// -----------------------------------------------------------------
unsigned int
viqe_set_dma_addr( viqe_dma_t *dma )	{

	// cur
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_IMG0_BASE0.nRD_BASEADDR = dma->addr_cur[0];
	HwVIPET->uRD_IMG0_BASE1.nRD_BASEADDR = dma->addr_cur[1];
	HwVIPET->uRD_IMG0_BASE2.nRD_BASEADDR = dma->addr_cur[2];
#else
	HwVIPET->uRD_IMG0_BASE0.bRD_BASEADDR.baseaddr = dma->addr_cur[0];
	HwVIPET->uRD_IMG0_BASE1.bRD_BASEADDR.baseaddr = dma->addr_cur[1];
	HwVIPET->uRD_IMG0_BASE2.bRD_BASEADDR.baseaddr = dma->addr_cur[2];
#endif	

	// pre
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_IMG1_BASE0.nRD_BASEADDR = dma->addr_pre[0];
	HwVIPET->uRD_IMG1_BASE1.nRD_BASEADDR = dma->addr_pre[1];
	HwVIPET->uRD_IMG1_BASE2.nRD_BASEADDR = dma->addr_pre[2];	
#else
	HwVIPET->uRD_IMG1_BASE0.bRD_BASEADDR.baseaddr = dma->addr_pre[0];
	HwVIPET->uRD_IMG1_BASE1.bRD_BASEADDR.baseaddr = dma->addr_pre[1];
	HwVIPET->uRD_IMG1_BASE2.bRD_BASEADDR.baseaddr = dma->addr_pre[2];
#endif	

	// outdma
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uOD_BASE0.nOD_BASEADDR = dma->addr_out[0];
	HwVIPET->uOD_BASE1.nOD_BASEADDR = dma->addr_out[1];
	HwVIPET->uOD_BASE2.nOD_BASEADDR = dma->addr_out[2];
#else
	HwVIPET->uOD_BASE0.bOD_BASEADDR.baseaddr = dma->addr_out[0];
	HwVIPET->uOD_BASE1.bOD_BASEADDR.baseaddr = dma->addr_out[1];
	HwVIPET->uOD_BASE2.bOD_BASEADDR.baseaddr = dma->addr_out[2];
#endif	
	
#ifdef VIQE_ASGN		// 20090428
	return ( HwVIPET->uRD_IMG0_BASE0.nRD_BASEADDR);
#else
	return ( HwVIPET->uRD_IMG0_BASE0.bRD_BASEADDR.baseaddr );
#endif		
	
}

// -----------------------------------------------------------------
// register writing of viqe_imgfmt_t structure
//
// 		input : 	pointer of viqe_imgfmt_t structure
// 		output :	-
// -----------------------------------------------------------------
int
viqe_set_imgfmt( viqe_imgfmt_t *imgfmt )	{
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT |= imgfmt->i_fmt;
	HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT = (imgfmt->force_420 ? HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT|Hw5 : HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT&(~Hw5));
	HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT = (imgfmt->first_frm_init ? HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT|Hw6 : HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT&(~Hw6));
#else
	HwVIPET->uTOP_IMGFMT.bTOP_IMGFMT.imgfmt = imgfmt->i_fmt;
	HwVIPET->uTOP_IMGFMT.bTOP_IMGFMT.chroma_read_twice = imgfmt->force_420;
	HwVIPET->uTOP_IMGFMT.bTOP_IMGFMT.first_prev_frame = imgfmt->first_frm_init;
#endif

#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_CTRL.nTOP_CTRL = (imgfmt->fmt_conv_disable ? HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw20 : HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw20));
#else
	HwVIPET->uTOP_CTRL.bTOP_CTRL.fmt_conv_disable = imgfmt->fmt_conv_disable;
#endif

#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uOD_CFG.nOD_CFG |= imgfmt->o_fmt;
#else
	HwVIPET->uOD_CFG.bOD_CFG.dsttype = imgfmt->o_fmt;
#endif

//	HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT |= (1<<5); // twice read mode //srlee
//	HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT &= ~(1<<5); // twice read mode // srlee
	return 1;
}

// -----------------------------------------------------------------
// register writing of viqe_cfg_t structure
//
// 		input : 	pointer of viqe_cfg_t structure
// 		output :	-
// -----------------------------------------------------------------
int
viqe_set_cfg( viqe_cfg_t *cfg )	{
	// overlap blocks
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_MISC_CTRL.nTOP_MISC_CTRL = (cfg->overlap_disable ? HwVIPET->uTOP_MISC_CTRL.nTOP_MISC_CTRL|Hw23 : HwVIPET->uTOP_MISC_CTRL.nTOP_MISC_CTRL&(~Hw23));
#else
	HwVIPET->uTOP_MISC_CTRL.bTOP_MISC_CTRL.overlap_blocks_disable = cfg->overlap_disable;
#endif	

	// frame manual restart
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_MISC_CTRL.nTOP_MISC_CTRL |= cfg->frm_manual_restart;
#else
	HwVIPET->uTOP_MISC_CTRL.bTOP_MISC_CTRL.frmupdate_manual = cfg->frm_manual_restart;	
#endif
	
	// directpath enable
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uOD_CFG.nOD_CFG = (cfg->directpath ? HwVIPET->uOD_CFG.nOD_CFG|Hw4 : HwVIPET->uOD_CFG.nOD_CFG&(~Hw4));
#else
	HwVIPET->uOD_CFG.bOD_CFG.outpath = cfg->directpath;
#endif		
		
	// de-noiser compressor enable
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_CTRL.nRD_CTRL = ( cfg->comp_en ? HwVIPET->uRD_CTRL.nRD_CTRL&(~Hw7) : HwVIPET->uRD_CTRL.nRD_CTRL|Hw7);
	HwVIPET->uCD_CTRL.nCD_CTRL = ( cfg->comp_en ? HwVIPET->uCD_CTRL.nCD_CTRL &(~Hw6) : HwVIPET->uCD_CTRL.nCD_CTRL |Hw6);
#else
	HwVIPET->uRD_CTRL.bRD_CTRL.bypass_comp = ( cfg->comp_en )? 0 : 1;
	HwVIPET->uCD_CTRL.bCD_CTRL.bypass_comp = ( cfg->comp_en )? 0 : 1;
#endif	

	return 1;
}

// -----------------------------------------------------------------
// disable all blocks in viqe
//
// 		input : 	-
// 		output :	return success or fail(-1)
// -----------------------------------------------------------------
int
viqe_set_disable_all( void )	{
	int		ret = 0;

	// frmupdate_ctrl_disable
#ifdef LINUX_OS_INCLUDE	
	//*(unsigned int *)(HwVIPET+0x18) |= (1<<24);
	HwVIPET->uTOP_MISC_CTRL.nTOP_MISC_CTRL |= (1<<24);
#else	
	*(unsigned int *)0xf0252018 |= (1<<24);
#endif


#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_CTRL.nTOP_CTRL &= ~(Hw6 | Hw5 | Hw4 | Hw10 | Hw12 |
									Hw2 | Hw3 | Hw7 |
									Hw18 | Hw17 |
									Hw0 | Hw1 | Hw16
									);

#else
	// block enables
	HwVIPET->uTOP_CTRL.bTOP_CTRL.dnsp_en = OFF;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.dnts_en = OFF;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.di_en = OFF;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.hpf_mode = OFF;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.di_spatial_only_en = OFF;

	HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma1_en = OFF;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma2_en = OFF;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.cdma_en = OFF;

	HwVIPET->uTOP_CTRL.bTOP_CTRL.his_en = OFF;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.gamut_en = OFF;

	// main disables
	HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma_gen = 
	HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma0_en = 
	HwVIPET->uTOP_CTRL.bTOP_CTRL.outdma_en = OFF;
#endif	


#ifdef VIQE_ASGN		// 20090428
 	ret = HwVIPET->uTOP_CTRL.nTOP_CTRL &
									 (Hw6 | Hw5 | Hw4 | Hw10 | Hw12 |
									Hw2 | Hw3 | Hw7|
									Hw18 | Hw17|
									Hw0 | Hw1 | Hw16	);
#else
	ret = HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma_gen |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma0_en |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.outdma_en |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.his_en |	
		HwVIPET->uTOP_CTRL.bTOP_CTRL.gamut_en |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma1_en |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma2_en |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.cdma_en |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.dnsp_en |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.dnts_en |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.di_en |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.hpf_mode |
		HwVIPET->uTOP_CTRL.bTOP_CTRL.di_spatial_only_en;
#endif	

	// frmupdate_ctrl_disable
#ifdef LINUX_OS_INCLUDE	
//	*(unsigned int *)(HwVIPET+0x18) &= ~(1<<24);
	HwVIPET->uTOP_MISC_CTRL.nTOP_MISC_CTRL  &= ~(1<<24);
#else		
	*(unsigned int *)0xf0252018 &= ~(1<<24);
#endif
	
	return ( ret == 0 )? 1 : -1;
}

// -----------------------------------------------------------------
// viqe initialization
// this function is an example, and there can be other initialization sequences
//
// 		input : 	viqe_t structure
// 		output :	return success or fail(-1)
// -----------------------------------------------------------------
int
viqe_init( viqe_t *viqe )	{

	// package limitation
	HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT &= 0x0000FFFF;
	HwVIPET->uTOP_IMGFMT.nTOP_IMGFMT |= ((unsigned int)(0xbacb))<<16;

	// frm update : always on
	HwVIPET->uTOP_MISC_CTRL.nTOP_MISC_CTRL |= (1<<27);
	HwVIPET->uTOP_MISC_CTRL.nTOP_MISC_CTRL |= (1<<24);

	// width, height	
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_SIZE.nTOP_SIZE = ((viqe->height)<<16 | viqe->width);
	HwVIPET->uOD_SIZE.nOD_SIZE = ((viqe->height)<<16 | viqe->width);
#else
	HwVIPET->uTOP_SIZE.bTOP_SIZE.width = viqe->width;
	HwVIPET->uTOP_SIZE.bTOP_SIZE.height = viqe->height;
	HwVIPET->uOD_SIZE.bOD_SIZE.width = viqe->width;
	HwVIPET->uOD_SIZE.bOD_SIZE.height = viqe->height;
#endif	

	// de-compressor size
#ifdef VIQE_ASGN		// 20090428
	if ( viqe->imgfmt->force_420 == ON )	{
		HwVIPET->uRD_COMP_PL0.nRD_COMP_PL = (viqe->width * viqe->height);
		HwVIPET->uRD_COMP_PL1.nRD_COMP_PL = (viqe->width * viqe->height >> 2);
	}
	else	{
		HwVIPET->uRD_COMP_PL0.nRD_COMP_PL = (viqe->width * viqe->height);
		HwVIPET->uRD_COMP_PL1.nRD_COMP_PL = (viqe->width * viqe->height >> 2);
	}
#else
	if ( viqe->imgfmt->force_420 == ON )	{
		HwVIPET->uRD_COMP_PL0.bRD_COMP_PL.comp_pixel_limit = (viqe->width * viqe->height);
		HwVIPET->uRD_COMP_PL1.bRD_COMP_PL.comp_pixel_limit = (viqe->width * viqe->height >> 2);
	}
	else	{
		HwVIPET->uRD_COMP_PL0.bRD_COMP_PL.comp_pixel_limit = (viqe->width * viqe->height);
		HwVIPET->uRD_COMP_PL1.bRD_COMP_PL.comp_pixel_limit = (viqe->width * viqe->height >> 2);
	}
#endif	
	viqe_set_imgfmt( viqe->imgfmt );

	// cfg
	viqe_set_cfg( viqe->cfg );

	// dma	
	viqe_set_dma( viqe->dma );

	// -------------------------------------------
	// default setting of blocks
	// -------------------------------------------
	// deintl
	viqe_set_deintl_default_conf( 0, viqe->width );
	
	// denoise
	//viqe_set_denoise_comp( OFF );		// comp_en
	//viqe_set_denoise_comp( ON );		// srlee

	// gamut
	viqe_set_gamut_map_init();

	// his
	//viqe_set_histogram_basic_conf( viqe->width, viqe->height, OFF, ON );
	viqe_set_histogram_basic_conf( viqe->width, viqe->height, OFF, OFF );


	//HwVIPET->uGM_CTRL.nGM_CTRL = 0x80000000;
	HwVIPET->uGM_CTRL.nGM_CTRL = 0x0;		// bypass off
	if ( viqe->cfg->directpath )	{
		
#ifdef LINUX_OS_INCLUDE
//		pDDIConfigReg->ON_THE_FLY = (pDDIConfigReg->ON_THE_FLY & 0xfffffffc)  | 0x00000002;
		pDDIConfigReg->ON_THE_FLY = (pDDIConfigReg->ON_THE_FLY & 0xfffffffc)  | 0x00000003;
#else
//		*(volatile int *)0xf0251038 = ((*(volatile int *)0xf0251038) & 0xfffffffc) | 0x00000002;
		*(volatile int *)0xf0251038 = ((*(volatile int *)0xf0251038) & 0xfffffffc) | 0x00000003;
#endif
		// mscl, src direct path
		//*(volatile int *)0xf0210044 = (*(volatile int *)0xf0210044) | 1<<24;
	}
	return 1;
}

void
viqe_set_resolution(viqe_t *viqe, int width, int height)
{
	viqe->width = width;
	viqe->height = height;
	// width, height	
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_SIZE.nTOP_SIZE = ((viqe->height)<<16 | viqe->width);
	HwVIPET->uOD_SIZE.nOD_SIZE = ((viqe->height)<<16 | viqe->width);
#else
	HwVIPET->uTOP_SIZE.bTOP_SIZE.width = viqe->width;
	HwVIPET->uTOP_SIZE.bTOP_SIZE.height = viqe->height;
	HwVIPET->uOD_SIZE.bOD_SIZE.width = viqe->width;
	HwVIPET->uOD_SIZE.bOD_SIZE.height = viqe->height;
#endif	
}
// -----------------------------------------------------------------
// return whether current frame processing is done or not
//
// 		input : 	-
// 		output :	return done(1) or not(0)
// -----------------------------------------------------------------
int
viqe_get_frame_end( void )	{

	return ( HwVIPET->uOD_STATE & 0x4 ) >> 2;
}

// -----------------------------------------------------------------
// reset frame ending signal
// 	it should be called before restart new frame if viqe_get_frame_end() is high at current frame
//
// 		input : 	-
// 		output :	-
// -----------------------------------------------------------------
void
viqe_set_frame_end( void )	{

	HwVIPET->uOD_STATE |= (1<<2);
}

// -----------------------------------------------------------------
// enable/disable compressor in temporal-spatial denoiser( dnts )
//
// 		input : 	enable(1) or disable(0)
// 		output :	-
// -----------------------------------------------------------------
void
viqe_set_denoise_comp( int comp_en )	{

	// compression enables, default is enabled
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uRD_CTRL.nRD_CTRL = ( comp_en ? (HwVIPET->uRD_CTRL.nRD_CTRL&(~Hw7)) : (HwVIPET->uRD_CTRL.nRD_CTRL|Hw7));
	HwVIPET->uCD_CTRL.nCD_CTRL = ( comp_en ? (HwVIPET->uCD_CTRL.nCD_CTRL&(~Hw6)) : (HwVIPET->uCD_CTRL.nCD_CTRL|Hw6));	
#else
	HwVIPET->uRD_CTRL.bRD_CTRL.bypass_comp = ( comp_en )? 0 : 1;
	HwVIPET->uCD_CTRL.bCD_CTRL.bypass_comp = ( comp_en )? 0 : 1;
#endif	
}

// ------------------------------------------------------------------------------
// viqe blocks enable settings
// register writing of viqe_modes_t structure
//
// 		input : 	viqe_modes_t structure
// 		output :	return success or fail( FAIL CODES )
// ------------------------------------------------------------------------------
int
viqe_set_modes( viqe_modes_t *modes )	{
	int		ret;
	
	// block enables
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( (modes->dnsp | modes->hpf) ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw6) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw6)));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( modes->dnts ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw5) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw5)));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( modes->di ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw4) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw4)));	
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( modes->hpf ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw10) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw10)));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( modes->disp ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw12) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw12)));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( modes->his ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw18) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw18)));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( modes->gamut ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw17) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw17)));	
#else
	//HwVIPET->uTOP_CTRL.bTOP_CTRL.dnsp_en = modes->dnsp;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.dnsp_en = modes->dnsp | modes->hpf; 
	HwVIPET->uTOP_CTRL.bTOP_CTRL.dnts_en = modes->dnts;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.di_en = modes->di;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.hpf_mode = modes->hpf;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.di_spatial_only_en = modes->disp;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.his_en = modes->his;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.gamut_en = modes->gamut;
#endif	
			
	// en_di and en_dnsp, en_hpf
	if ( modes->di & !modes->disp )	{
		if ( modes->dnsp | modes->hpf )
			//return -1;
			return RET_VIQE_EN_DI_DNSP;
	}

	// dn, hpf
	if ( modes->dnts & modes->dnsp )	{
		//return -1;
		//return RET_VIQE_EN_DN_TSSP;
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw9;		
#else
		HwVIPET->uTOP_CTRL.bTOP_CTRL.denoise_mode = 2;
#endif		
	}
	else if ( modes->dnts & modes->hpf ){
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw9;		
#else
		HwVIPET->uTOP_CTRL.bTOP_CTRL.denoise_mode = 2;
#endif		
	}
	else if ( modes->dnts ){
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
#else
		HwVIPET->uTOP_CTRL.bTOP_CTRL.denoise_mode = 0;
#endif		
	}
	else if ( modes->dnsp | modes->hpf ){
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw8;		
#else
		HwVIPET->uTOP_CTRL.bTOP_CTRL.denoise_mode = 1;
#endif		
	}
	
	// deinterlacer first
	//if ( en_di & !di_sp & ( en_hpf | en_dnsp ) )	{
	if ( modes->di & !modes->disp & modes->hpf )	{		// only hpf
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw13;		
#else
		HwVIPET->uTOP_CTRL.bTOP_CTRL.first_deintl_en = 1;
#endif			
	}
	else	{
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uTOP_CTRL.nTOP_CTRL &= ~Hw13;
#else
		HwVIPET->uTOP_CTRL.bTOP_CTRL.first_deintl_en = 0;
#endif		
	}

	// need previous frame data
	if ( modes->di & !modes->disp | modes->dnts ){
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw2;		
#else
		HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma1_en = 1;
#endif		
	}
	else{
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uTOP_CTRL.nTOP_CTRL &= ~Hw2;		
#else
		HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma1_en = 0;
#endif		
	}

	// compressor for de-noiser recursive path
	if ( modes->dnts )	{
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw3;
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw7;	
#else
		HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma2_en = 1;
		HwVIPET->uTOP_CTRL.bTOP_CTRL.cdma_en = 1;
#endif		
	}
	else	{
#ifdef VIQE_ASGN		// 20090428
		HwVIPET->uTOP_CTRL.nTOP_CTRL &= ~Hw3;
		HwVIPET->uTOP_CTRL.nTOP_CTRL &= ~Hw7;	
#else
		HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma2_en = 0;
		HwVIPET->uTOP_CTRL.bTOP_CTRL.cdma_en = 0;
#endif		
	}

	// main enables
#ifdef VIQE_ASGN		// 20090428
//	ret = (modes->di | modes->disp | modes->dnts | modes->dnsp | modes->hpf | modes->his | modes->gamut);
	ret = 1;
	HwVIPET->uTOP_CTRL.nTOP_CTRL = HwVIPET->uTOP_CTRL.nTOP_CTRL | Hw16;
	HwVIPET->uTOP_CTRL.nTOP_CTRL = HwVIPET->uTOP_CTRL.nTOP_CTRL | Hw1;
	HwVIPET->uTOP_CTRL.nTOP_CTRL = HwVIPET->uTOP_CTRL.nTOP_CTRL | Hw0;
#else
	HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma_gen = 
	HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma0_en = 
	//HwVIPET->uTOP_CTRL.bTOP_CTRL.outdma_en = modes->di | modes->disp | modes->dnts | modes->dnsp | modes->hpf | modes->his | modes->gamut;
	HwVIPET->uTOP_CTRL.bTOP_CTRL.outdma_en = 1;
#endif	

#ifdef VIQE_ASGN		// 20090428
	ret = ( (HwVIPET->uTOP_INT.nTOP_INT&(Hw12|Hw13|Hw14|Hw15)) == 0 ) ? 1 : RET_VIQE_EN_ERRREG;
#else
	ret = ( HwVIPET->uTOP_INT.bTOP_INT.err_regset == 0 )? 1 : RET_VIQE_EN_ERRREG;
#endif
	return ret;
}

// ------------------------------------------------------------------------------
// return current modes values from reading enable registers
//
// 		input : 	pointer of viqe_modes_t structure to be written
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_get_modes( viqe_modes_t *modes )	{
#ifdef VIQE_ASGN		// 20090428
	modes->dnsp = HwVIPET->uTOP_CTRL.nTOP_CTRL&Hw6;
	modes->dnts = HwVIPET->uTOP_CTRL.nTOP_CTRL&Hw5;
	modes->di = HwVIPET->uTOP_CTRL.nTOP_CTRL&Hw4;
	modes->hpf = HwVIPET->uTOP_CTRL.nTOP_CTRL&Hw10;
	modes->disp = HwVIPET->uTOP_CTRL.nTOP_CTRL&Hw12;
	modes->his = HwVIPET->uTOP_CTRL.nTOP_CTRL&Hw18;
	modes->gamut = HwVIPET->uTOP_CTRL.nTOP_CTRL&Hw17;
#else
	modes->dnsp = HwVIPET->uTOP_CTRL.bTOP_CTRL.dnsp_en;
	modes->dnts = HwVIPET->uTOP_CTRL.bTOP_CTRL.dnts_en;
	modes->di = HwVIPET->uTOP_CTRL.bTOP_CTRL.di_en;
	modes->hpf = HwVIPET->uTOP_CTRL.bTOP_CTRL.hpf_mode;
	modes->disp = HwVIPET->uTOP_CTRL.bTOP_CTRL.di_spatial_only_en;
	modes->his = HwVIPET->uTOP_CTRL.bTOP_CTRL.his_en;
	modes->gamut = HwVIPET->uTOP_CTRL.bTOP_CTRL.gamut_en;
#endif
}

// ------------------------------------------------------------------------------
// set restart command
// 	valid only when viqe is running at frame manually control mode
//
// 		input : 	-
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_frm_restart( void )	{
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_FRM_CTRL.nTOP_FRM_CTRL |= Hw0;	
#else
	HwVIPET->uTOP_FRM_CTRL.bTOP_FRM_CTRL.frmupdate_restart = 1;
#endif	
}

// ------------------------------------------------------------------------------
// interrupt setting
//
// 		input : 	-
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_interrupts( void )	{
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_INTMASK.nTOP_INTMASK |= 0x000000ff;
#else
	HwVIPET->uTOP_INTMASK.bTOP_INTMASK.intmask = 0x00ff;
#endif

	// di
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uDI_INT.nDI_INT |= 0xffff0000;	
#else
	HwVIPET->uDI_INT.bDI_INT.intmask = 0xffff;
#endif	
}

void
viqe_clear_interrupts( void )	{
	int		interrupts;
	interrupts = HwVIPET->uTOP_INT.nTOP_INT;
#ifdef VIQE_ASGN

	if ( HwVIPET->uTOP_INT.nTOP_INT & Hw2)	{
		int		di_int;
		di_int = HwVIPET->uDI_INT.nDI_INT;
		//
		HwVIPET->uDI_INT.nDI_INT = di_int;
	}
	if ( HwVIPET->uTOP_INT.nTOP_INT & Hw0)	{
	}
	if ( HwVIPET->uTOP_INT.nTOP_INT & Hw1 )	{
	}
	if ( HwVIPET->uTOP_INT.nTOP_INT & Hw3 )	{
	}
	if ( HwVIPET->uTOP_INT.nTOP_INT & Hw5)	{
		int		pm_int;
		pm_int = HwVIPET->uHI_INT.nHI_INT;
		if ( pm_int == 0 )	{
			pm_int = HwVIPET->uGM_INT.nGM_INT;
			HwVIPET->uGM_INT.nGM_INT = pm_int;
		}
		else	{
			pm_int = HwVIPET->uHI_INT.nHI_INT;
			HwVIPET->uHI_INT.nHI_INT = pm_int;
		}
	}
#else	// 20090428	
	if ( HwVIPET->uTOP_INT.bTOP_INT.di )	{
		int		di_int;
		di_int = HwVIPET->uDI_INT.nDI_INT;
		//
		HwVIPET->uDI_INT.nDI_INT = di_int;
	}
	if ( HwVIPET->uTOP_INT.bTOP_INT.rdma )	{
	}
	if ( HwVIPET->uTOP_INT.bTOP_INT.dn )	{
	}
	if ( HwVIPET->uTOP_INT.bTOP_INT.whdma )	{
	}
	if ( HwVIPET->uTOP_INT.bTOP_INT.pm )	{
		int		pm_int;
		pm_int = HwVIPET->uHI_INT.nHI_INT;
		if ( pm_int == 0 )	{
			pm_int = HwVIPET->uGM_INT.nGM_INT;
			HwVIPET->uGM_INT.nGM_INT = pm_int;
		}
		else	{
			pm_int = HwVIPET->uHI_INT.nHI_INT;
			HwVIPET->uHI_INT.nHI_INT = pm_int;
		}
	}

#endif
	HwVIPET->uTOP_INT.nTOP_INT = interrupts;
}


// -----------------------------------------------------------------
// bypassing de-interlacer block
//
// 		input : 	bypass on/off
// 		output :	-
// -----------------------------------------------------------------
void 
viqe_set_deintl_bypass( int bypass )	{
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uDI_CTRL.nDI_CTRL = bypass ? (HwVIPET->uDI_CTRL.nDI_CTRL|Hw29) : (HwVIPET->uDI_CTRL.nDI_CTRL&(~Hw29));
#else
	HwVIPET->uDI_CTRL.bDI_CTRL.bypass = bypass;
#endif	
}

// ------------------------------------------------------------------------------
// default setting for de-interlacer
//
// 		input : 	odd_first parameter
// 		output :	-
// ------------------------------------------------------------------------------
void 
viqe_set_deintl_default_conf( int odd_first, int ImgWidth ) {
	int		dm_stati_thres_disable;
	int		maxrange_tmp_on;
	int		maxrange_spa_on;
	int		gen_thres_jaggy_en;
	int		spatial_jaggycheck;
	int		pulldown_on;
	int		pd_judder_on;
	//int		pd_gen_thres_judder_on;
	int		pd_use_prevent_flag_on;
	int		edi_thres_dir_adap;
	int		edi_thres_dir_hor;
	int		dm_thres_sad;
	int		dm_thres_pixel;
	int		maxrange_spa_type;
	int		edi_dir_adap_disable;
	int		dm_stati_thres_mul;
	int		dm_stati_thres_weight;
	int		gen_thres_jaggy_minthres;
	int		gen_thres_jaggy_maxthres;
	int		pd_thres_pulldown_cnts;
	int		pd_thres_weight;
	int		pd_thres_user_val_disable;
	int		pd_user_checkout;
	int		pd_cnts_thres_pulldown_checkout;
	int		pd_thres_pulldown_0;
	int		pd_thres_pulldown_1;
	int		pd_thres_pulldown_2;
	int		pd_thres_cnt_judder;
	int		pd_judder_downline_margin;
	int		pd_judder_horline_margin;
	int		pd_gen_thres_judder_minthres;
	int		pd_gen_thres_judder_maxthres;
//#include "../vlog_tb.v"
	{
		maxrange_tmp_on = 1;//1;//1;
		maxrange_spa_on = 1;//1;//1;//0;//1;
		maxrange_spa_type = 3;

		gen_thres_jaggy_en = 1;
		spatial_jaggycheck = 1;
		pulldown_on = 1;
		pd_judder_on = 1;
		pd_use_prevent_flag_on = 1;

		edi_thres_dir_adap = 255;//128;
		edi_thres_dir_hor = 16;//32;//128;//32;
		edi_dir_adap_disable = 0;

		//
		dm_stati_thres_disable = 1;//1;//1;
		//
		dm_thres_sad = 4;//4;//8;//4;//8;//4;//15;//24;
		dm_thres_pixel = 4;//4//4;//8;//16;
		dm_stati_thres_mul = 15;//8;//16;//4;
		dm_stati_thres_weight = 1;

		gen_thres_jaggy_minthres = 4;//8;//12;//23;//24;
		gen_thres_jaggy_maxthres = 16;//24;//80;//128;

		pd_thres_pulldown_cnts = 500;
		pd_thres_weight = 1;//2;
		pd_thres_user_val_disable = 1;
		pd_user_checkout = 0;
		pd_cnts_thres_pulldown_checkout = 0;//5;
		pd_thres_pulldown_0 = 15;//10;//8;
		pd_thres_pulldown_1 = 20;//15;//12;
		pd_thres_pulldown_2 = 30;//20;//16;
		pd_thres_cnt_judder = ImgWidth/64;//7; //30;
		pd_judder_downline_margin = 8;
		pd_judder_horline_margin = 8;
		pd_gen_thres_judder_minthres = 64;
		pd_gen_thres_judder_maxthres = 111;
	}

	// ctrl
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uDI_CTRL.nDI_CTRL  = dm_stati_thres_disable 	? (HwVIPET->uDI_CTRL.nDI_CTRL|Hw3) : (HwVIPET->uDI_CTRL.nDI_CTRL&~(Hw3));
	HwVIPET->uDI_CTRL.nDI_CTRL  = maxrange_tmp_on 		? (HwVIPET->uDI_CTRL.nDI_CTRL|Hw4) : (HwVIPET->uDI_CTRL.nDI_CTRL&~(Hw4));
	HwVIPET->uDI_CTRL.nDI_CTRL  = maxrange_spa_on 		? (HwVIPET->uDI_CTRL.nDI_CTRL|Hw5) : (HwVIPET->uDI_CTRL.nDI_CTRL&~(Hw5));
	HwVIPET->uDI_CTRL.nDI_CTRL  = gen_thres_jaggy_en 		? (HwVIPET->uDI_CTRL.nDI_CTRL|Hw6) : (HwVIPET->uDI_CTRL.nDI_CTRL&~(Hw6));
	HwVIPET->uDI_CTRL.nDI_CTRL  = spatial_jaggycheck 		? (HwVIPET->uDI_CTRL.nDI_CTRL|Hw7) : (HwVIPET->uDI_CTRL.nDI_CTRL&~(Hw7));

	HwVIPET->uDI_CTRL.nDI_CTRL  = pulldown_on 			? (HwVIPET->uDI_CTRL.nDI_CTRL|Hw8) : (HwVIPET->uDI_CTRL.nDI_CTRL&~(Hw8));
	HwVIPET->uDI_CTRL.nDI_CTRL  = pd_judder_on 			? (HwVIPET->uDI_CTRL.nDI_CTRL|Hw9) : (HwVIPET->uDI_CTRL.nDI_CTRL&~(Hw9));
	HwVIPET->uDI_CTRL.nDI_CTRL  |= Hw10;
	HwVIPET->uDI_CTRL.nDI_CTRL  = pd_use_prevent_flag_on 	? (HwVIPET->uDI_CTRL.nDI_CTRL|Hw11) : (HwVIPET->uDI_CTRL.nDI_CTRL&~(Hw11));
#else
	HwVIPET->uDI_CTRL.bDI_CTRL.dm_stati_thres_disable = dm_stati_thres_disable ;
	HwVIPET->uDI_CTRL.bDI_CTRL.maxrange_tmp_on = maxrange_tmp_on ;
	HwVIPET->uDI_CTRL.bDI_CTRL.maxrange_spa_on = maxrange_spa_on ;
	HwVIPET->uDI_CTRL.bDI_CTRL.gen_thres_jaggy_en = gen_thres_jaggy_en ;
	HwVIPET->uDI_CTRL.bDI_CTRL.spatial_jaggycheck = spatial_jaggycheck ;

	HwVIPET->uDI_CTRL.bDI_CTRL.pulldown_on = pulldown_on ;
	HwVIPET->uDI_CTRL.bDI_CTRL.pd_judder_on = pd_judder_on ;
	HwVIPET->uDI_CTRL.bDI_CTRL.pd_gen_thres_judder_en = 1 ;
	HwVIPET->uDI_CTRL.bDI_CTRL.pd_use_prevent_flag_on = pd_use_prevent_flag_on ;
#endif	

	//HwVIPET->uDI_CTRL.bDI_CTRL.pd_clear_internal_frm_idx;

	//HwVIPET->uDI_CTRL.bDI_CTRL.bypass;
	//HwVIPET->uDI_CTRL.bDI_CTRL.flush_synch_fifo;
	//HwVIPET->uDI_CTRL.bDI_CTRL.internal_reg_clear;

	// engine0;
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uDI_ENGINE0.nDI_ENGINE0 = (HwVIPET->uDI_ENGINE0.nDI_ENGINE0 & 0x00FFFFFF) | (edi_thres_dir_adap<<24);
	HwVIPET->uDI_ENGINE0.nDI_ENGINE0 = (HwVIPET->uDI_ENGINE0.nDI_ENGINE0 & 0xFFFFFF00) | (edi_thres_dir_hor);	
	HwVIPET->uDI_ENGINE0.nDI_ENGINE0 = (HwVIPET->uDI_ENGINE0.nDI_ENGINE0 & 0xFFFF00FF) | (dm_thres_sad<<8);
	HwVIPET->uDI_ENGINE0.nDI_ENGINE0 = (HwVIPET->uDI_ENGINE0.nDI_ENGINE0 & 0xFF00FFFF) | (dm_thres_pixel<<16);
#else
	HwVIPET->uDI_ENGINE0.bDI_ENGINE0.edi_thres_dir_adap = edi_thres_dir_adap ;
	HwVIPET->uDI_ENGINE0.bDI_ENGINE0.edi_thres_dir_hor = edi_thres_dir_hor ;
	HwVIPET->uDI_ENGINE0.bDI_ENGINE0.dm_thres_sad = dm_thres_sad ;
	HwVIPET->uDI_ENGINE0.bDI_ENGINE0.dm_thres_pixel = dm_thres_pixel ;
#endif	

	// engine1;
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uDI_ENGINE1.nDI_ENGINE1 = (HwVIPET->uDI_ENGINE1.nDI_ENGINE1 & ~ (Hw0|Hw1)) | maxrange_spa_type;
	HwVIPET->uDI_ENGINE1.nDI_ENGINE1 = edi_dir_adap_disable ? (HwVIPET->uDI_ENGINE1.nDI_ENGINE1|Hw4) : (HwVIPET->uDI_ENGINE1.nDI_ENGINE1&(~Hw4));
	HwVIPET->uDI_ENGINE1.nDI_ENGINE1 = (HwVIPET->uDI_ENGINE1.nDI_ENGINE1 & ~(Hw8|Hw9|Hw10|Hw11)) | (dm_stati_thres_mul<<8); 	// 20090429
	HwVIPET->uDI_ENGINE1.nDI_ENGINE1 = (HwVIPET->uDI_ENGINE1.nDI_ENGINE1 & ~(Hw12|Hw13|Hw14|Hw15)) | (dm_stati_thres_weight<<12);	
	HwVIPET->uDI_ENGINE1.nDI_ENGINE1 = (HwVIPET->uDI_ENGINE1.nDI_ENGINE1 & 0xFF00FFFF) | (gen_thres_jaggy_minthres<<16);	// 20090429
	HwVIPET->uDI_ENGINE1.nDI_ENGINE1 = (HwVIPET->uDI_ENGINE1.nDI_ENGINE1 & 0x00FFFFFF) | (gen_thres_jaggy_maxthres<<24);	// 20090429	
#else
	HwVIPET->uDI_ENGINE1.bDI_ENGINE1.maxrange_spa_type = maxrange_spa_type ;
	HwVIPET->uDI_ENGINE1.bDI_ENGINE1.edi_dir_adap_disable = edi_dir_adap_disable ;
	HwVIPET->uDI_ENGINE1.bDI_ENGINE1.dm_stati_thres_mul = dm_stati_thres_mul ;
	HwVIPET->uDI_ENGINE1.bDI_ENGINE1.dm_stati_thres_weight = dm_stati_thres_weight ;
	HwVIPET->uDI_ENGINE1.bDI_ENGINE1.gen_thres_jaggy_minthres = gen_thres_jaggy_minthres ;
	HwVIPET->uDI_ENGINE1.bDI_ENGINE1.gen_thres_jaggy_maxthres = gen_thres_jaggy_maxthres ;	
#endif	

	// pd_thres0;
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 = (HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 & 0xFFFFFC00) | (pd_thres_pulldown_cnts);	// 20090429
	HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 = (HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 & 0xFFFFE3FF) | (pd_thres_weight<<10);	// 20090429
	HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 = pd_thres_user_val_disable ? (HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0|Hw15) : (HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0&~(Hw15));
	HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 = (HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 & ~(Hw16|Hw17)) | (pd_user_checkout<<16);
	HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 = (HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 & ~(Hw20|Hw21|Hw22|Hw23)) | (pd_cnts_thres_pulldown_checkout<<20); // 20090429
#else
	HwVIPET->uDI_PD_THRES0.bDI_PD_THRES0.pd_thres_pulldown_cnts = pd_thres_pulldown_cnts ;
	HwVIPET->uDI_PD_THRES0.bDI_PD_THRES0.pd_thres_weight = pd_thres_weight ;
	HwVIPET->uDI_PD_THRES0.bDI_PD_THRES0.pd_thres_user_val_disable = pd_thres_user_val_disable ;
	HwVIPET->uDI_PD_THRES0.bDI_PD_THRES0.pd_user_checkout = pd_user_checkout ;
	HwVIPET->uDI_PD_THRES0.bDI_PD_THRES0.pd_cnts_thres_pulldown_checkout = pd_cnts_thres_pulldown_checkout ;
#endif		
	
	// pd_thres1;
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uDI_PD_THRES1.nDI_PD_THRES1 = (HwVIPET->uDI_PD_THRES1.nDI_PD_THRES1 & 0xFFFFFF00) | (pd_thres_pulldown_0);	
	HwVIPET->uDI_PD_THRES1.nDI_PD_THRES1 = (HwVIPET->uDI_PD_THRES1.nDI_PD_THRES1 & 0xFFFF00FF) | (pd_thres_pulldown_1<<8);
	HwVIPET->uDI_PD_THRES1.nDI_PD_THRES1 = (HwVIPET->uDI_PD_THRES1.nDI_PD_THRES1 & 0xFF00FFFF) | (pd_thres_pulldown_2<<16);
#else
	HwVIPET->uDI_PD_THRES1.bDI_PD_THRES1.pd_thres_pulldown_0 = pd_thres_pulldown_0 ;
	HwVIPET->uDI_PD_THRES1.bDI_PD_THRES1.pd_thres_pulldown_1 = pd_thres_pulldown_1 ;
	HwVIPET->uDI_PD_THRES1.bDI_PD_THRES1.pd_thres_pulldown_2 = pd_thres_pulldown_2 ;
#endif	

	// pd_judder;
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uDI_PD_JUDDER.nDI_PD_JUDDER = (HwVIPET->uDI_PD_JUDDER.nDI_PD_JUDDER & 0xFFFFFF00) | (pd_thres_cnt_judder);	
	HwVIPET->uDI_PD_JUDDER.nDI_PD_JUDDER = (HwVIPET->uDI_PD_JUDDER.nDI_PD_JUDDER & 0xFFFFF0FF) | (pd_judder_downline_margin<<8);	
	HwVIPET->uDI_PD_JUDDER.nDI_PD_JUDDER = (HwVIPET->uDI_PD_JUDDER.nDI_PD_JUDDER & 0xFFFF0FFF) | (pd_judder_horline_margin<<12);	
	HwVIPET->uDI_PD_JUDDER.nDI_PD_JUDDER = (HwVIPET->uDI_PD_JUDDER.nDI_PD_JUDDER & 0xFF00FFFF) | (pd_gen_thres_judder_minthres<<16);	
	HwVIPET->uDI_PD_JUDDER.nDI_PD_JUDDER = (HwVIPET->uDI_PD_JUDDER.nDI_PD_JUDDER & 0x00FFFFFF) | (pd_gen_thres_judder_maxthres<<24);		
#else
	HwVIPET->uDI_PD_JUDDER.bDI_PD_JUDDER.pd_thres_cnt_judder = pd_thres_cnt_judder ;
	HwVIPET->uDI_PD_JUDDER.bDI_PD_JUDDER.pd_judder_downline_margin = pd_judder_downline_margin ;
	HwVIPET->uDI_PD_JUDDER.bDI_PD_JUDDER.pd_judder_horline_margin = pd_judder_horline_margin ;
	HwVIPET->uDI_PD_JUDDER.bDI_PD_JUDDER.pd_gen_thres_judder_minthres = pd_gen_thres_judder_minthres ;
	HwVIPET->uDI_PD_JUDDER.bDI_PD_JUDDER.pd_gen_thres_judder_maxthres = pd_gen_thres_judder_maxthres ;
#endif	

	// from function argument
	// odd fist set
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uTOP_IMGCONF.nTOP_IMGCONF= odd_first ? (HwVIPET->uTOP_IMGCONF.nTOP_IMGCONF|Hw0) : (HwVIPET->uTOP_IMGCONF.nTOP_IMGCONF&(~Hw0));
#else
	HwVIPET->uTOP_IMGCONF.bTOP_IMGCONF.odd_first = odd_first;
#endif	

}

// ------------------------------------------------------------------------------
// region setting for de-interlacer
//
// 		input : 	region_enable on/off, top-left point and bottom-right point
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_deintl_region( int region_enable, int region_idx_x_start, int region_idx_x_end, int region_idx_y_start, int region_idx_y_end )	{
	// region0;
#ifndef VIQE_ASGN
	HwVIPET->uDI_REGION_0.bDI_REGION_0.region_idx_x_start = region_idx_x_start ;
	HwVIPET->uDI_REGION_0.bDI_REGION_0.region_idx_x_end = region_idx_x_end ;
	HwVIPET->uDI_REGION_0.bDI_REGION_0.region_enable = region_enable ;

	// region1;
	HwVIPET->uDI_REGION_1.bDI_REGION_1.region_idx_y_start = region_idx_y_start ;
	HwVIPET->uDI_REGION_1.bDI_REGION_1.region_idx_y_end = region_idx_y_end ;
#else
	// region0;
	HwVIPET->uDI_REGION_0.nDI_REGION_0 = 0;
	HwVIPET->uDI_REGION_0.nDI_REGION_0 |= (region_idx_x_start);
	HwVIPET->uDI_REGION_0.nDI_REGION_0 |= (region_idx_x_end << 16);
	HwVIPET->uDI_REGION_0.nDI_REGION_0 |= (region_enable << 31);
	// region1;
	HwVIPET->uDI_REGION_1.nDI_REGION_1 = 0;
	HwVIPET->uDI_REGION_1.nDI_REGION_1 |= (region_idx_y_start);
	HwVIPET->uDI_REGION_1.nDI_REGION_1 |= (region_idx_y_end << 16);
#endif
}
	
// ------------------------------------------------------------------------------
// simple configuration for de-interlacer
// 	pulldown( film-mode ) detector on/off, odd_first parameter
//
// 		input : 	pulldown on/off, odd_first parameter
// 		output :	-
// ------------------------------------------------------------------------------
void 
viqe_set_deintl_basic_conf( int pd_on, int odd_first ) {
#ifndef VIQE_ASGN
	// pulldown on/off
	HwVIPET->uDI_CTRL.bDI_CTRL.pulldown_on = pd_on & 0x1;

	// odd first on/off
	HwVIPET->uTOP_IMGCONF.bTOP_IMGCONF.odd_first = odd_first & 0x1;
#else
	HwVIPET->uDI_CTRL.nDI_CTRL = ((pd_on & 0x1) == 1) ? (HwVIPET->uDI_CTRL.nDI_CTRL|Hw8) : (HwVIPET->uDI_CTRL.nDI_CTRL&(~Hw8));
	HwVIPET->uTOP_IMGCONF.nTOP_IMGCONF = ((odd_first & 0x1) == 1) ? (HwVIPET->uTOP_IMGCONF.nTOP_IMGCONF|Hw0) : (HwVIPET->uTOP_IMGCONF.nTOP_IMGCONF&(~Hw0));
#endif
}


// ------------------------------------------------------------------------------
// alternative software implementation for film mode detector of de-interlacer
// 	useful when another statical anaylsis is needed.
// 	this function is an example.
// 	it uses the register values of film mode detector internal variables
//	viqe_set_pd_user_checkout( int numfrm, int width, int height )
//
// 		input : 	frame number, width, height
// 		output :	check_out value
// ------------------------------------------------------------------------------
typedef	struct	{
	int		val;
	int		posx;
	int		posy;
}	val_idx_t;
static val_idx_t		cnt_val[3][5];
static int				frm_val_current[3];
static int				frm_val_predict[3];
static int				flag[5];

static int
find_min( int cnt_val[5], int except_idx )	{
	int		d_1_2_idx;
	int		d_3_4_idx;
	int		min_idx;

	if ( except_idx == 1 )	{
		d_1_2_idx = 2;
	}
	else if ( except_idx == 2 )	{
		d_1_2_idx = 1;
	}
	else	{
		d_1_2_idx = ( cnt_val[1] > cnt_val[2] )? 2 : 1;
	}

	if ( except_idx == 3 )	{
		d_3_4_idx = 4;
	}
	else if ( except_idx == 4 )	{
		d_3_4_idx = 3;
	}
	else	{
		d_3_4_idx = ( cnt_val[3] > cnt_val[4] )? 4 : 3;
	}

	if ( except_idx == 0 )	{
		min_idx = ( cnt_val[d_3_4_idx] > cnt_val[d_1_2_idx] )? d_1_2_idx : d_3_4_idx;
	}
	else	{
		if ( cnt_val[d_3_4_idx] > cnt_val[d_1_2_idx] )	{
			min_idx = ( cnt_val[d_1_2_idx] > cnt_val[0] )? 0 : d_1_2_idx;
		}
		else	{
			min_idx = ( cnt_val[d_3_4_idx] > cnt_val[0] )? 0 : d_3_4_idx;
		}
	}

	return min_idx;
}


int
viqe_set_pd_user_checkout( int numfrm, int width, int height )	{
	int				prevent_flag_on;
	int				num_fields;
	int				check_pulldown[2];
	int				minpos_offset;
	int				check_out = 0;
	int				n, i, j;
	int				cur_idx;
	
	num_fields = numfrm * 2;

	cnt_val[0][(num_fields)%5].val = HwVIPET->uDI_PD_CNT0_0.nDI_PD_CNT0_0;
	cnt_val[1][(num_fields)%5].val = HwVIPET->uDI_PD_CNT0_1.nDI_PD_CNT0_1;
	cnt_val[2][(num_fields)%5].val = HwVIPET->uDI_PD_CNT0_2.nDI_PD_CNT0_2;

	cnt_val[0][(num_fields+1)%5].val = HwVIPET->uDI_PD_CNT1_0.nDI_PD_CNT1_0;
	cnt_val[1][(num_fields+1)%5].val = HwVIPET->uDI_PD_CNT1_1.nDI_PD_CNT1_1;
	cnt_val[2][(num_fields+1)%5].val = HwVIPET->uDI_PD_CNT1_2.nDI_PD_CNT1_2;

	for ( n = 0; n < 2; n++ )	{

		cur_idx = num_fields % 5;

		prevent_flag_on = 0;
		for ( i = 0; i < 3; i++ )	{
			if ( 1 )	{
				int		min_idx, min_2nd_idx;
				int		new_cnts[5];
				for ( j = 0; j < 5; j++ )	{
					new_cnts[j] = cnt_val[i][j].val;
				}

				min_idx = find_min( new_cnts, -1 );
				min_2nd_idx = find_min( new_cnts, min_idx );
		
				if ( abs( new_cnts[min_idx] - new_cnts[min_2nd_idx] ) < (new_cnts[min_idx] * 2) )	{
					prevent_flag_on = 0x1;
				}

				frm_val_current[i] = min_idx;
			}
		}
		if ( prevent_flag_on )	{
			flag[cur_idx] = 0;
		}
		else	{
			if ( frm_val_current[0] == frm_val_predict[0] &&
				 frm_val_current[1] == frm_val_predict[1] &&
				 frm_val_current[2] == frm_val_predict[2] )	{

				flag[cur_idx] = 1;
			}
			else	{
				flag[cur_idx] = 0;
			}
		}

		// check flags
		if ( num_fields > 5 )	{
			if ( flag[0] &
				 flag[1] &
				 flag[2] &
				 flag[3] &
				 flag[4] )	{

				check_pulldown[n] = 1;
			}
			else	{
				check_pulldown[n] = 0;
			}
		}
		else	{
			check_pulldown[0] = 0;
			check_pulldown[1] = 0;
		}

		frm_val_predict[0] = frm_val_current[0];
		frm_val_predict[1] = frm_val_current[1];
		frm_val_predict[2] = frm_val_current[2];

		num_fields++;
	}

	check_out = 0;
	if ( check_pulldown[0] & check_pulldown[1] )	{
		// num_fields : always even ( indicates the first field )
		//num_fields = imgin->numframes * 2;
		num_fields = numfrm * 2;
		minpos_offset = ( num_fields - frm_val_current[1] ) % 5;

		switch ( minpos_offset )	{
			case 3 :		// next 0
				check_out = 3;				// weaving current first field and second field of previous frame
				break;
			case 0 :		// next 2
				check_out = 3;				// weaving current first field and second field of previous frame
				break;
			case 2 :		// next 4
				check_out = 2;				// just weaving
				break;
			case 4 :		// next 1
				check_out = 2;				// just weaving
				break;
			case 1 :		// next 3
				check_out = 2;				// just weaving
				break;
		}
	}

	if ( check_out != HwVIPET->uDI_PD_STATUS.bDI_PD_STATUS.check_out )	{
#ifdef VSIMUL
		sim_debug( "[VIPET] Error!! pulldown detection error\0" );
		sim_fail();
#endif
		return -1;
	}
	return 1;

}


// ------------------------------------------------------------------------------
// disable region setting for spatial-temporal de-noiser
// 	In specified image region, the output of de-noiser is bypassed as original pixel value
//
// 		input : 	region_enable - on/off
//				region_swap - 0 : left region is original and right region is processed, 1 : the opposite
//				region_hor_center : horizontal region boundary
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_dnts_region( int region_enable, int region_swap, int region_hor_center )	{
#ifndef VIQE_ASGN
	HwVIPET->uDN_DIV_IMG_3D.bDN_DIV_IMG.den = region_enable;
	
	HwVIPET->uDN_DIV_IMG_3D.bDN_DIV_IMG.dtog = !region_swap;
	HwVIPET->uDN_DIV_IMG_3D.bDN_DIV_IMG.pos = region_hor_center;
#else
	HwVIPET->uDN_DIV_IMG_3D.nDN_DIV_IMG = 0;
	HwVIPET->uDN_DIV_IMG_3D.nDN_DIV_IMG |= (region_enable << 15);
	HwVIPET->uDN_DIV_IMG_3D.nDN_DIV_IMG |= (!region_swap << 14);
	HwVIPET->uDN_DIV_IMG_3D.nDN_DIV_IMG |= (region_hor_center);
#endif
}

// ------------------------------------------------------------------------------
// disable region setting for spatial de-noiser
// 	In specified image region, the output of de-noiser is bypassed as original pixel value
//
// 		input : 	region_enable - on/off
//				region_swap - 0 : left region is original and right region is processed, 1 : the opposite
//				region_hor_center : horizontal region boundary
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_dnsp_region( int region_enable, int region_swap, int region_hor_center )	{
#ifndef VIQE_ASGN
	HwVIPET->uDN_DIV_IMG_2D.bDN_DIV_IMG.den = region_enable;
	
	HwVIPET->uDN_DIV_IMG_2D.bDN_DIV_IMG.dtog = !region_swap;
	HwVIPET->uDN_DIV_IMG_2D.bDN_DIV_IMG.pos = region_hor_center;
#else
	HwVIPET->uDN_DIV_IMG_2D.nDN_DIV_IMG = 0;
	HwVIPET->uDN_DIV_IMG_2D.nDN_DIV_IMG |= (region_enable << 15);
	HwVIPET->uDN_DIV_IMG_2D.nDN_DIV_IMG |= (!region_swap << 14);
	HwVIPET->uDN_DIV_IMG_2D.nDN_DIV_IMG |= (region_hor_center);
#endif
}


// ------------------------------------------------------------------------------
// default setting for histgram hardware
//
// 		input : 	width, height, CDF on/off, LUT use on/off
// 		output :	success of fail(-1)
// ------------------------------------------------------------------------------
int
viqe_set_histogram_basic_conf( int width, int height, int cdf_en, int lut_use )	{
	int		i;

	int		h_offset_size;
	int		v_offset_size;
	int		sample_size;

	// size
	h_offset_size = 10;
	v_offset_size = 10;
	printf ("width = %d, height = %d\n", width, height);
	if ((height % v_offset_size) == 0)
	{
		sample_size = (width/h_offset_size) * ((height / v_offset_size));
	}
	else
	{
		sample_size = (width/h_offset_size) * ((height / v_offset_size)+1);
	}
	printf ("SampleSize = %d\n", sample_size);
#ifndef VIQE_ASGN
	HwVIPET->uHI_CONFIG.bHI_CONFIG.h_offset_size = h_offset_size;
	HwVIPET->uHI_CONFIG.bHI_CONFIG.v_offset_size = v_offset_size;
	HwVIPET->uHI_CONFIG.bHI_CONFIG.sample_size = sample_size;

	// measure auto
	HwVIPET->uHI_CTRL.bHI_CTRL.measure_auto = 1;
	// multi frames
	HwVIPET->uHI_CTRL.bHI_CTRL.multi_frames = 0;
#else
	HwVIPET->uHI_CONFIG.nHI_CONFIG = 0;
	HwVIPET->uHI_CONFIG.nHI_CONFIG |= (h_offset_size);
	HwVIPET->uHI_CONFIG.nHI_CONFIG |= (v_offset_size << 8);
	HwVIPET->uHI_CONFIG.nHI_CONFIG |= (sample_size << 16);

	// measure auto
	HwVIPET->uHI_CTRL.nHI_CTRL |= (1 << 16);
	// multi frames
	HwVIPET->uHI_CTRL.nHI_CTRL &= ~(Hw17 | Hw18 | Hw19);
	HwVIPET->uHI_CTRL.nHI_CTRL |= (0 << 17);
#endif


	// segments init
	//for ( i = 0; i < 15; i++ )	{
	for ( i = 0; i < 16; i += 4 )	{
		HwVIPET->bHI_SEGS.segs[i/4] = (16*(i+4))<<24|(16*(i+3))<<16|(16*(i+2))<<8|(16*(i+1));
	}
	

	// scale init
	for ( i = 0; i < 4; i++ )	{
		//HwVIPET->bHI_SEGS.seg[i] = 16*i;
		HwVIPET->bHI_SCALES.scales[i] = 0xffffffff;
	}

	// top enable
	//HwVIPET->uTOP_CTRL.bTOP_CTRL.his_en = 1;
#ifndef VIQE_ASGN
	HwVIPET->uTOP_CTRL.bTOP_CTRL.his_cdf_or_lut_en = ( cdf_en | lut_use );

	// -------------------------------------
	// lut_direct_init
	// avoid clkgating...
	HwVIPET->uTOP_CLKGATE.bTOP_CLKGATE.clkgate_pm = 1;

	// first, check histgram hardware turns off
	// TODO:
	
	HwVIPET->uHI_CTRL.bHI_CTRL.lut_direct_init = 1;
#else
	HwVIPET->uTOP_CTRL.nTOP_CTRL |= (( cdf_en | lut_use ) << 19);

	// -------------------------------------
	// lut_direct_init
	// avoid clkgating...
	HwVIPET->uTOP_CLKGATE.nTOP_CLKGATE |= (1 << 5);

	// first, check histgram hardware turns off
	// TODO:
	
	HwVIPET->uHI_CTRL.nHI_CTRL |= (1 << 9);
#endif

	// wait
	if ( 1 )	{
		int		x = 0;
		while( 1 )	{
			if ( !HwVIPET->uHI_STATUS.bHI_STATUS.lut_user_update_busy )	{
				break;
			}
			x++;
			if ( x > 1000000 )		// check infinite-loop situation
				return -1;
		}
	}
#ifndef VIQE_ASGN
	HwVIPET->uTOP_CLKGATE.bTOP_CLKGATE.clkgate_pm = 0;
	// -------------------------------------

	// cdf_en
	HwVIPET->uHI_CTRL.bHI_CTRL.cdf_en = cdf_en;

	// lut_use
	HwVIPET->uHI_CTRL.bHI_CTRL.lut_use = lut_use;
#else
	HwVIPET->uTOP_CLKGATE.nTOP_CLKGATE &= ~Hw5;
	// -------------------------------------

	// cdf_en
	HwVIPET->uHI_CTRL.nHI_CTRL = cdf_en ? HwVIPET->uHI_CTRL.nHI_CTRL | Hw4 : HwVIPET->uHI_CTRL.nHI_CTRL & (~Hw4);

	// lut_use
	HwVIPET->uHI_CTRL.nHI_CTRL = lut_use ? HwVIPET->uHI_CTRL.nHI_CTRL | Hw7 : HwVIPET->uHI_CTRL.nHI_CTRL & (~Hw7);
#endif
	return 1;
}

// ------------------------------------------------------------------------------
// enable function for histogram
//
// 		input : 	block enable, CDF generation enable, LUT use on/off
// 		output :	success or fail(-1)
// ------------------------------------------------------------------------------
int
viqe_set_histogram_enable( int enable, int cdf_en, int lut_use )	{
//	if ( !enable & ( cdf_en | lut_use ) )
//		return -1;
#ifndef VIQE_ASGN
	HwVIPET->uTOP_CTRL.bTOP_CTRL.his_en = enable;
	//HwVIPET->uTOP_CTRL.bTOP_CTRL.his_cdf_or_lut_en = enable & ( cdf_en | lut_use );
	HwVIPET->uTOP_CTRL.bTOP_CTRL.his_cdf_or_lut_en = ( cdf_en | lut_use );
	HwVIPET->uHI_CTRL.bHI_CTRL.cdf_en = cdf_en;
	HwVIPET->uHI_CTRL.bHI_CTRL.lut_use = lut_use;
#else
	HwVIPET->uTOP_CTRL.nTOP_CTRL = enable ? HwVIPET->uTOP_CTRL.nTOP_CTRL | Hw18 : HwVIPET->uTOP_CTRL.nTOP_CTRL & (~Hw18);
	//HwVIPET->uTOP_CTRL.nTOP_CTRL = (enable & ( cdf_en | lut_use )) ? HwVIPET->uTOP_CTRL.nTOP_CTRL | Hw19 : HwVIPET->uTOP_CTRL.nTOP_CTRL & (~Hw19);
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( cdf_en | lut_use ) ? HwVIPET->uTOP_CTRL.nTOP_CTRL | Hw19 : HwVIPET->uTOP_CTRL.nTOP_CTRL & (~Hw19);
	HwVIPET->uHI_CTRL.nHI_CTRL = cdf_en ? HwVIPET->uHI_CTRL.nHI_CTRL | Hw4 : HwVIPET->uHI_CTRL.nHI_CTRL & (~Hw4);
	HwVIPET->uHI_CTRL.nHI_CTRL = lut_use ? HwVIPET->uHI_CTRL.nHI_CTRL | Hw7 : HwVIPET->uHI_CTRL.nHI_CTRL & (~Hw7);
#endif
	return 1;
}

// ------------------------------------------------------------------------------
// detail configuration function for histogram
//
// 		input : 	width, height, CDF on/off, LUT use on/off, pixel counter offset,
// 					automode on/off, number of multi frames, segments, scales
// 		output :	success or fail(-1)
// ------------------------------------------------------------------------------
int
viqe_set_histogram_detail_conf( int width, int height, int cdf_en, int lut_use,
	   					   		int hoffset, int voffset, int automode, int multi_frames,
						   		unsigned char segs[16], unsigned char scales[16] )	{

	int		sample_size;
	int		i;

	sample_size = (width/hoffset) * ((height / voffset)+1);
#ifndef VIQE_ASGN
	HwVIPET->uHI_CONFIG.bHI_CONFIG.h_offset_size = hoffset;
	HwVIPET->uHI_CONFIG.bHI_CONFIG.v_offset_size = voffset;
	HwVIPET->uHI_CONFIG.bHI_CONFIG.sample_size = sample_size;

	// measure auto
	HwVIPET->uHI_CTRL.bHI_CTRL.measure_auto = automode;
	// multi frames
	HwVIPET->uHI_CTRL.bHI_CTRL.multi_frames = multi_frames;
#else
	HwVIPET->uHI_CONFIG.nHI_CONFIG = 0;
	HwVIPET->uHI_CONFIG.nHI_CONFIG |= (hoffset);
	HwVIPET->uHI_CONFIG.nHI_CONFIG |= (voffset << 8);
	HwVIPET->uHI_CONFIG.nHI_CONFIG |= (sample_size << 16);

	// measure auto
	HwVIPET->uHI_CTRL.nHI_CTRL = automode ? HwVIPET->uHI_CTRL.nHI_CTRL | Hw16 : HwVIPET->uHI_CTRL.nHI_CTRL & (~Hw16);
	// multi frames
	HwVIPET->uHI_CTRL.nHI_CTRL &= ~(Hw17 | Hw18 | Hw19);
	HwVIPET->uHI_CTRL.nHI_CTRL |= (multi_frames << 17);
#endif
	// segments init
	for ( i = 0; i < 16; i += 4 )	{
		HwVIPET->bHI_SEGS.segs[i/4] = (segs[i*4+3])<<24|(segs[i*4+2])<<16|(segs[i*4+1])<<8|(segs[i*4+0]);
	}

	// scale init
	for ( i = 0; i < 4; i++ )	{
		HwVIPET->bHI_SCALES.scales[i] = (scales[i*4+3])<<24|(scales[i*4+2])<<16|(scales[i*4+1])<<8|(scales[i*4+0]);
	}

	// top enable
	//HwVIPET->uTOP_CTRL.bTOP_CTRL.his_en = 1;
#ifndef VIQE_ASGN
	HwVIPET->uTOP_CTRL.bTOP_CTRL.his_cdf_or_lut_en = ( cdf_en | lut_use );

	// -------------------------------------
	// lut_direct_init
	// avoid clkgating...
	HwVIPET->uTOP_CLKGATE.bTOP_CLKGATE.clkgate_pm = 1;

	// first, check histgram hardware turns off
	// TODO:
	
	HwVIPET->uHI_CTRL.bHI_CTRL.lut_direct_init = 1;
#else
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( cdf_en | lut_use ) ? HwVIPET->uTOP_CTRL.nTOP_CTRL | Hw19 : HwVIPET->uTOP_CTRL.nTOP_CTRL & (~Hw19);

	// -------------------------------------
	// lut_direct_init
	// avoid clkgating...
	HwVIPET->uTOP_CLKGATE.nTOP_CLKGATE |= (1 << 5);

	// first, check histgram hardware turns off
	// TODO:
	
	HwVIPET->uHI_CTRL.nHI_CTRL |= (1 << 9);
#endif

	// wait
	if ( 1 )	{
		int		x = 0;
		while( 1 )	{
			if ( !HwVIPET->uHI_STATUS.bHI_STATUS.lut_user_update_busy )	{
				break;
			}
			x++;
			if ( x > 1000000 )		// check infinite-loop situation
				return -1;
		}
	}
#ifndef VIQE_ASGN
	HwVIPET->uTOP_CLKGATE.bTOP_CLKGATE.clkgate_pm = 0;
	// -------------------------------------

	// cdf_en
	HwVIPET->uHI_CTRL.bHI_CTRL.cdf_en = cdf_en;

	// lut_use
	HwVIPET->uHI_CTRL.bHI_CTRL.lut_use = lut_use;
#else
	HwVIPET->uTOP_CLKGATE.nTOP_CLKGATE &= (~Hw5);
	// -------------------------------------
	// cdf_en
	HwVIPET->uHI_CTRL.nHI_CTRL = cdf_en ? HwVIPET->uHI_CTRL.nHI_CTRL | Hw4 : HwVIPET->uHI_CTRL.nHI_CTRL & (~Hw4);

	// lut_use
	HwVIPET->uHI_CTRL.nHI_CTRL = lut_use ? HwVIPET->uHI_CTRL.nHI_CTRL | Hw7 : HwVIPET->uHI_CTRL.nHI_CTRL & (~Hw7);
#endif
	return 1;
}

// ------------------------------------------------------------------------------
// segments configuration function for histogram
//
// 		input : 	segments values having 16 unsigned char array
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_histogram_segments( unsigned char segs[16] )	{
	int		i;
	for ( i = 0; i < 16; i += 4 )	{
		HwVIPET->bHI_SEGS.segs[i/4] = (segs[i+3])<<24|(segs[i+2])<<16|(segs[i+1])<<8|(segs[i+0]);
	}
}

// ------------------------------------------------------------------------------
// return segments values in hardware register
//
// 		input : 	pointer of 16 elements for segments writing
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_get_histogram_segments( unsigned char segs[16] )	{
	int		i;
	for ( i = 0; i < 16; i += 4 )	{
		segs[i+3] = (HwVIPET->bHI_SEGS.segs[i/4] << 24) & 0xff;
		segs[i+2] = (HwVIPET->bHI_SEGS.segs[i/4] << 16) & 0xff;
		segs[i+1] = (HwVIPET->bHI_SEGS.segs[i/4] <<  8) & 0xff;
		segs[i+0] = (HwVIPET->bHI_SEGS.segs[i/4] <<  0) & 0xff;
	}
}

// ------------------------------------------------------------------------------
// scale configuration function for histogram
// if global scale value is not zero, scales array values are ignored.
//
// 		input : 	global scale value, scale values having 16 unsigned char array
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_histogram_scales( int gscale, unsigned char scales[16] )	{
	int		i;
	if ( gscale != 0 )	{
		int		g_scale = CLIP3( gscale, 0, 0xff );
		for ( i = 0; i < 16; i += 4 )	{
			HwVIPET->bHI_SCALES.scales[i/4] = (g_scale)<<24|(g_scale)<<16|(g_scale)<<8|(g_scale);
		}
	}
	else	{
		for ( i = 0; i < 16; i += 4 )	{
			HwVIPET->bHI_SCALES.scales[i/4] = (scales[i+3])<<24|(scales[i+2])<<16|(scales[i+1])<<8|(scales[i+0]);
		}
	}
}

// ------------------------------------------------------------------------------
// return scales values in hardware register
//
// 		input : 	pointer of 16 elements for scales writing
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_get_histogram_scales( unsigned char scales[16] )	{
	int		i;
	for ( i = 0; i < 16; i += 4 )	{
		scales[i+3] = (HwVIPET->bHI_SCALES.scales[i/4] << 24) & 0xff;
		scales[i+2] = (HwVIPET->bHI_SCALES.scales[i/4] << 16) & 0xff;
		scales[i+1] = (HwVIPET->bHI_SCALES.scales[i/4] <<  8) & 0xff;
		scales[i+0] = (HwVIPET->bHI_SCALES.scales[i/4] <<  0) & 0xff;
	}
}

// ------------------------------------------------------------------------------
// LUT configuration function for histogram
//
// 		input : 	LUT values having 256 unsigned char array
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_histogram_luts( unsigned char luts[256] )	{
	int		i;
	for ( i = 0; i < 256; i += 4 )	{
		HwVIPET->bHI_LUTS.luts[i/4] = (luts[i+3])<<24|(luts[i+2])<<16|(luts[i+1])<<8|(luts[i+0]);
	}
}

// ------------------------------------------------------------------------------
// return LUT values stored in hardware
//
// 		input : 	pointer of 256 elements for LUT writing
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_get_histogram_luts( unsigned char luts[256] )	{
	int		i;
	for ( i = 0; i < 256; i += 4 )	{
		luts[i+3] = (HwVIPET->bHI_LUTS.luts[i/4] << 24) & 0xff;
		luts[i+2] = (HwVIPET->bHI_LUTS.luts[i/4] << 16) & 0xff;
		luts[i+1] = (HwVIPET->bHI_LUTS.luts[i/4] <<  8) & 0xff;
		luts[i+0] = (HwVIPET->bHI_LUTS.luts[i/4] <<  0) & 0xff;
	}
}

// ------------------------------------------------------------------------------
// region setting for histogram
//
// 		input : 	region_enable on/off, top-left point and bottom-right point
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_his_region( int region_enable, int region_idx_x_start, int region_idx_x_end, int region_idx_y_start, int region_idx_y_end )	{
	// region0;
#ifndef VIQE_ASGN
	HwVIPET->uHI_REGION_0.bHI_REGION_0.region_x_start = region_idx_x_start ;
	HwVIPET->uHI_REGION_0.bHI_REGION_0.region_x_end = region_idx_x_end ;
	HwVIPET->uHI_REGION_0.bHI_REGION_0.region_enable = region_enable ;

	// region1;
	HwVIPET->uHI_REGION_1.bHI_REGION_1.region_y_start = region_idx_y_start ;
	HwVIPET->uHI_REGION_1.bHI_REGION_1.region_y_end = region_idx_y_end ;
#else
	HwVIPET->uHI_REGION_0.nHI_REGION_0 = 0;
	HwVIPET->uHI_REGION_0.nHI_REGION_0 |= (region_idx_x_start);
	HwVIPET->uHI_REGION_0.nHI_REGION_0 |= (region_idx_x_end << 16);
	HwVIPET->uHI_REGION_0.nHI_REGION_0 |= (region_enable << 31);

	// region1;
	HwVIPET->uHI_REGION_1.nHI_REGION_1 = 0;
	HwVIPET->uHI_REGION_1.nHI_REGION_1 |= (region_idx_y_start);
	HwVIPET->uHI_REGION_1.nHI_REGION_1 |= (region_idx_y_end << 16);
#endif
}


// ------------------------------------------------------------------------------
// gamut map hardware enable function
//
// 		input : 	enable on/off
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_gamut_enable( int enable )	{
#ifndef VIQE_ASGN
	HwVIPET->uTOP_CTRL.bTOP_CTRL.gamut_en = enable;
#else
	HwVIPET->uTOP_CTRL.nTOP_CTRL = enable ? HwVIPET->uTOP_CTRL.nTOP_CTRL | Hw17 : HwVIPET->uTOP_CTRL.nTOP_CTRL & (~Hw17);
#endif
}

// ------------------------------------------------------------------------------
// gamut mapping hardware initialization function
//
// 		input : 	-
// 		output :	success of fail(-1)
// ------------------------------------------------------------------------------
int
viqe_set_gamut_map_init( void )	{
	// avoid clkgating...
#ifndef VIQE_ASGN
	HwVIPET->uTOP_CLKGATE.bTOP_CLKGATE.clkgate_pm = 1;

	HwVIPET->uGM_CTRL.bGM_CTRL.init = 1;
#else
	HwVIPET->uTOP_CLKGATE.nTOP_CLKGATE |= (1 << 5);

	HwVIPET->uGM_CTRL.nGM_CTRL |= (1);
#endif
	if ( 1 )	{
		int		x = 0;
		while( 1 )	{
			if ( HwVIPET->uGM_STATUS.bGM_STATUS.done_init )	{
				break;
			}
			x++;
			if ( x > 1000000 )		// check infinite-loop situation
				return -1;
		}
	}
#ifndef VIQE_ASGN
	HwVIPET->uTOP_CLKGATE.bTOP_CLKGATE.clkgate_pm = 0;
#else
	HwVIPET->uTOP_CLKGATE.nTOP_CLKGATE &= (~Hw5);
#endif

	return 1;
}

// ------------------------------------------------------------------------------
// region setting for gamut-map
//
// 		input : 	region_enable on/off, top-left point and bottom-right point
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_set_gamut_region( int region_enable, int region_idx_x_start, int region_idx_x_end, int region_idx_y_start, int region_idx_y_end )	{
	// region0;
#ifndef VIQE_ASGN
	HwVIPET->uGM_REGION_0.bGM_REGION_0.region_x_start = region_idx_x_start ;
	HwVIPET->uGM_REGION_0.bGM_REGION_0.region_x_end = region_idx_x_end ;
	HwVIPET->uGM_REGION_0.bGM_REGION_0.region_enable = region_enable ;

	// region1;
	HwVIPET->uGM_REGION_1.bGM_REGION_1.region_y_start = region_idx_y_start ;
	HwVIPET->uGM_REGION_1.bGM_REGION_1.region_y_end = region_idx_y_end ;
#else
	HwVIPET->uGM_REGION_0.nGM_REGION_0 &= ~0x7fffffff;
//	HwVIPET->uGM_REGION_0.nGM_REGION_0 = 0;
	HwVIPET->uGM_REGION_0.nGM_REGION_0 |= (region_idx_x_start);
	HwVIPET->uGM_REGION_0.nGM_REGION_0 |= (region_idx_x_end << 16);
	HwVIPET->uGM_REGION_0.nGM_REGION_0 |= (region_enable << 31);

	// region1;
	HwVIPET->uGM_REGION_1.nGM_REGION_1 = 0;
	HwVIPET->uGM_REGION_1.nGM_REGION_1 |= (region_idx_y_start);
	HwVIPET->uGM_REGION_1.nGM_REGION_1 |= (region_idx_y_end << 16);
#endif
}

// ------------------------------------------------------------------------------
// bypass setting for gamut-map
//
// 		input : 	bypassing gamut-map hardware when bypass valuable is 1
// 		output :	-
// ------------------------------------------------------------------------------
void
viqe_bypass_gamut( int bypass )	{
	HwVIPET->uGM_CTRL.nGM_CTRL = ( bypass )? HwVIPET->uGM_CTRL.nGM_CTRL | Hw31 : HwVIPET->uGM_CTRL.nGM_CTRL & (~Hw31);
}


// ------------------------------------------------------------------------------
// get hardware address for viqe
//
// 		input : 	bypassing gamut-map hardware when bypass valuable is 1
// 		output :	-
// ------------------------------------------------------------------------------
VIPET * 
VIQE_get_haddr( void )	{
	return HwVIPET;
}

// ------------------------------------------------------------------------------
// show current status for viqe
//
// 		input : 	-
// 		output :	-
// ------------------------------------------------------------------------------
void 
VIQE_show( void )	{
#ifdef LINUX_OS_INCLUDE
	int Offset;
	unsigned int *ptr;

	printf ("di_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.di_en);
	printf ("dnts_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.dnts_en);
	printf ("dnsp_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.dnsp_en);
	printf ("cdma_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.cdma_en);
	printf ("denoise_mode = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.denoise_mode);
	printf ("hpf_mode = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.hpf_mode);
	printf ("di_spatial_only_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.di_spatial_only_en);
	printf ("first_deintl_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.first_deintl_en);
	printf ("outdma_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.first_deintl_en);
	printf ("gamut_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.gamut_en);
	printf ("his_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.his_en);
	printf ("his_cdf_or_lut_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.his_cdf_or_lut_en);
	printf ("GM_CTRL = 0x%x\n", HwVIPET->uGM_CTRL.nGM_CTRL);
	printf ("GM_STATUS = 0x%x\n", HwVIPET->uGM_STATUS.nGM_STATUS);
	printf ("GM_INT = 0x%x\n", HwVIPET->uGM_INT.nGM_INT);
	printf ("OD_STATE = 0x%x\n", HwVIPET->uOD_STATE);
	printf ("RD_FIFOSTATE = 0x%x\n", HwVIPET->uRD_FIFOSTATE.nDN_FIFOSTATE);
	printf ("RD_FIFOSTATE = 0x%x\n", HwVIPET->uRD_FIFOSTATE.nDN_FIFOSTATE);
	printf ("RD_LINE_STATE0 = 0x%x\n", HwVIPET->uRD_LINE_STATE0.nRD_LINE_STATE);
	printf ("RD_LINE_STATE1 = 0x%x\n", HwVIPET->uRD_LINE_STATE1.nRD_LINE_STATE1);
	printf ("RD_CTRL = 0x%x\n", HwVIPET->uRD_CTRL.nRD_CTRL);
	printf ("dnts-deintl = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.first_deintl_en);
	printf ("fmt_conv_disable = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.fmt_conv_disable);
	printf ("rdma2_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma2_en);
	printf ("cdma_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.cdma_en);
	ptr = HwVIPET;

	for (Offset = 0; Offset < (sizeof (VIPET)/4); Offset += 1)
//	for (Offset = (0x620/4); Offset < (0x7FC/4); Offset += 1)
	{
		printf ("0x%x : 0x%x\n", 4*Offset, *(ptr + Offset));
	}
//	FILE *Output;
//	Output = fopen ("VIQE_STATE4", "wb");
//	fwrite (HwVIPET, sizeof (VIPET), 1, Output);
//	fclose (Output);
/*	{
		int i;
		unsigned int *ptr;
		printf ("####### ScalerState ########\n");
		ptr = (unsigned int *)mmap(0, LOGICAL_ADDR_MIN, PROT_READ | PROT_WRITE, MAP_SHARED, viqe_dev_fd, (unsigned int)0xF0210000);
		for (i = 0; i < 21; i++)
		{
			printf ("0x%x : 0x%x\n", 4*i, *(ptr+i));
		}
		munmap((void *)ptr, LOGICAL_ADDR_MIN);
	}*/
	
/*	{
		int i;
		unsigned int *ptr;
		printf ("####### LCDC ########\n");
		ptr = (unsigned int *)mmap(0, LOGICAL_ADDR_MIN, PROT_READ | PROT_WRITE, MAP_SHARED, viqe_dev_fd, (unsigned int)0xF0200000);
		for (i = 0; i <= (0xC0/4); i++)
		{
			printf ("0x%x : 0x%x\n", 4*i, *(ptr+i));
		}
		munmap((void *)ptr, LOGICAL_ADDR_MIN);
		printf ("####### END LCDC ########\n");
	}*/
#endif		// LINUX_OS_INCLUDE
	return;
}


// ------------------------------------------------------------------------------
// parameters for DNTS
// ------------------------------------------------------------------------------
typedef struct {
	unsigned int	coeff0;		// slope,ca,ba,a
	unsigned int	coeff1;		// Y_plane, NOT USED [31:8]
}	nD2D_LPF_COEFF;

typedef struct {
	unsigned int	coeff0;		// c, e, f , NOT USED [7:0]
	unsigned int	coeff1;		// g, m1, m2, l
	unsigned int 	coeff2;		// y_plane_h1, y_plane_h2
}	nD2D_HPF_COEFF;

typedef struct {
	unsigned int	strength;
	unsigned int	hor0;
	unsigned int	hor1;
	unsigned int	ver0;
	unsigned int	ver1;
}	nDNTS_COEFF_CH_LIST;

typedef struct {
	unsigned int	strength;
	unsigned int	tmp0;
	unsigned int	tmp1;
}	nDNTS_COEFF_TMP_CH_LIST;

typedef struct {
	unsigned int	strength;
	unsigned int	coeff0;
	unsigned int	coeff1;
	unsigned int	coeff2;
}	nD2D_HPF_CH_COEFF_LIST;

typedef struct {
	unsigned int 	strength;
	unsigned int	luma_coeff0;
	unsigned int	luma_coeff1;
	unsigned int	chroma_coeff0;
	unsigned int	chroma_coeff1;
}	nD2D_LPF_COEFF_LIST;

static nDNTS_COEFF_CH_LIST 	dnts_coeff_luma_list[] = {
	{VERYLOW,		0x154, 0x0, 0x154, 0x0},
//	{VERYLOW,		0x0, 0x0, 0x0, 0x0},
	{LOW,			0x5554, 0x0, 0x5554, 0x0},
	{MEDIUM,		0x15556a94, 0x0, 0x15556a94, 0x0},
	{STRONG,		0x55aaaa94, 0x155, 0x55aaaa94, 0x155},
	{VERYSTRONG,	0xaabffea4, 0x55555a, 0xaaaaaaa4, 0x55555a}
	};

static nDNTS_COEFF_CH_LIST 	dnts_coeff_chroma_list[] = {
	{VERYLOW, 		0x14, 0x0, 0x14, 0x0},
//	{VERYLOW, 		0x0, 0x0, 0x0, 0x0},
	{LOW,			0x154, 0x0, 0x154, 0x0},
	{MEDIUM,		0x555554, 0x0, 0x555554, 0x0},
	{STRONG,		0x15556a94, 0x0, 0x15556a94, 0x0},
	{VERYSTRONG,	0x55aaaa94, 0x155, 0x55aaaa94, 0x155}
	};

static nDNTS_COEFF_TMP_CH_LIST	dnts_coeff_tmp_luma_list[] = {
	{VERYLOW,		0x555554, 0x0},
//	{VERYLOW,		0x0, 0x0},
	{LOW,			0x15556a94, 0x0},
	{MEDIUM,		0x555aaa94, 0x5},
	{STRONG,		0x55aaaa94, 0x155},
	{VERYSTRONG, 	0x6aaaaa94, 0x15555}
};

static nDNTS_COEFF_TMP_CH_LIST	dnts_coeff_tmp_chroma_list[] = {
	{VERYLOW,		0x154, 0x0},
//	{VERYLOW,		0x0, 0x0},
	{LOW,			0x5554, 0x0},
	{MEDIUM,		0x555554, 0x0},
	{STRONG,		0x15556a94, 0x0},
	{VERYSTRONG, 	0x555aaa94, 0x5}
};

static nD2D_HPF_CH_COEFF_LIST	d2d_hpf_luma_coeff_list[] = {
	{ VERYLOW, 0x3a3c0200, 0x9b2040ff, 0x001c01fe},
	{ LOW, 0x573c0200, 0xff260ac, 0x002d01fd},
	{ MEDIUM, 0x742c0200, 0xff408081, 0x003801fc},
	{ STRONG, 0x7f3c0200, 0xff468c75, 0x003f01fc},
	{ VERYSTRONG, 0x913c0200, 0xff4ca068, 0x004901fb}
	};

static nD2D_HPF_CH_COEFF_LIST d2d_hpf_chroma_coeff_list[] = {
	{ VERYLOW, 0x2c2d0100, 0x952040ff, 0x001501ff},
	{ LOW, 0x422d0100, 0xff2c60ab, 0x002201ff},
	{ MEDIUM, 0x582d0100, 0xff408080, 0x002b01fe},
	{ STRONG, 0x602d0100, 0xff468c74, 0x002f01fe},	
	{ VERYSTRONG, 0x6e2d0100, 0xff4ca067, 0x003701fe}
	};

static nD2D_LPF_COEFF_LIST		d2d_lpf_coeff_list[] = {
	{ VERYLOW, 0x12320f0a, 0xe, 0x11190705, 0x6},
	{ LOW, 0x12641e14, 0x1c, 0x12320f0a, 0xe},
	{ MEDIUM, 0x12c83c28, 0x38, 0x12641e14, 0x1c},
	{ STRONG, 0x12ff5a3c, 0x54, 0x12962d1e, 0x2a},
	{ VERYSTRONG, 0x12ff7850, 0x70, 0x12c83c28, 0x38}
	};

static int 
cal_d2d_lpf_coeff_simple( VIPET *vipet_reg, int strength )	{
	unsigned int i, cnt, succ, nReg;
	unsigned int *nDst;
	unsigned int *nSrc;

	succ = 0;
	//nDst = (unsigned int *)(&(vipet_reg->uDN_COEFF1_2D_Y));
	nDst = (unsigned int *)(&(vipet_reg->uDN_COEFF0_2D_Y));

	for (i=0; i<5; i++)
	{
		if (d2d_lpf_coeff_list[i].strength == strength)
		{
			nSrc = &(d2d_lpf_coeff_list[i].luma_coeff0);

			for(cnt=0;cnt<4;cnt++)
			{
				if ((cnt==1) | (cnt==3))
				{
					nReg = *nDst & 0xffffff00;
					*nDst++ = nReg | *nSrc++;
					*nDst++;
					*nDst++;
				}
				else
				{
					*nDst++ = *nSrc++;
				}
			}
			succ = 1;
			break;
		}
	}
	return 1;
}

static int 
cal_d2d_hpf_luma_coeff_simple( VIPET *vipet_reg, int strength )	{
	unsigned int i, cnt, succ, nReg;
	unsigned int *nDst;
	unsigned int *nSrc;

	succ = 0;
	//nDst = (unsigned int *)(&(vipet_reg->uDN_COEFF1_2D_Y));
	nDst = (unsigned int *)(&(vipet_reg->uDN_COEFF0_2D_Y));

	for (i=0; i<5; i++)
	{
		if (d2d_hpf_luma_coeff_list[i].strength == strength)
		{
			nSrc = &(d2d_hpf_luma_coeff_list[i].coeff0);

			for(cnt=0;cnt<3;cnt++)
			{
				if ((cnt==0))
				{
					*nDst++;
					nReg = *nDst & 0xff;
					*nDst++ = nReg | *nSrc++;
				}
				else
				{
					*nDst++ = *nSrc++;
				}
			}
			succ = 1;
			break;
		}
	}
	return 1;
}

static int 
cal_d2d_hpf_chroma_coeff_simple( VIPET *vipet_reg, int strength )	{
	unsigned int i, cnt, succ, nReg;
	unsigned int *nDst;
	unsigned int *nSrc;

	succ = 0;
	//nDst = (unsigned int *)(&(vipet_reg->uDN_COEFF1_2D_Y));
	nDst = (unsigned int *)(&(vipet_reg->uDN_COEFF0_2D_C));

	for (i=0; i<5; i++)
	{
		if (d2d_hpf_chroma_coeff_list[i].strength == strength)
		{
			nSrc = &(d2d_hpf_chroma_coeff_list[i].coeff0);

			for(cnt=0;cnt<3;cnt++)
			{
				if ((cnt==0))
				{
					*nDst++;
					nReg = *nDst & 0xff;
					*nDst++ = nReg | *nSrc++;
				}
				else
				{
					*nDst++ = *nSrc++;
				}
			}
			succ = 1;
			break;
		}
	}
	return 1;
}

static int 
cal_dnts_coeff_luma_simple( VIPET *vipet_reg, int strength )	{
	unsigned int i,cnt, succ;
	unsigned int *nDst;
	unsigned int *nSrc;

	succ = 0;
	nDst = (unsigned int *)(&(vipet_reg->uDN_COEFF_HOR_Y0));
	
	for (i=0; i<5; i++)
	{
		if (dnts_coeff_luma_list[i].strength == strength)
		{
			nSrc = &(dnts_coeff_luma_list[i].hor0);

			for(cnt=0;cnt<4;cnt++)
			{
				*nDst++ = *nSrc++;
			}
			succ = 1;
			break;
		}
	}
	return 1;
}

static int 
cal_dnts_coeff_chroma_simple( VIPET *vipet_reg, int strength )	{
	unsigned int i,cnt, succ;
	unsigned int *nDst;
	unsigned int *nSrc;

	succ = 0;
	nDst = (unsigned int *)(&(vipet_reg->uDN_COEFF_HOR_C0));
	
	for (i=0; i<5; i++)
	{
		if (dnts_coeff_chroma_list[i].strength == strength)
		{
			nSrc = &(dnts_coeff_chroma_list[i].hor0);

			for(cnt=0;cnt<4;cnt++)
			{
				*nDst++ = *nSrc++;
			}
			succ = 1;
			break;
		}
	}
	return 1;
}

static int 
cal_dnts_coeff_tmp_luma_simple( VIPET *vipet_reg, int strength )	{
	unsigned int i,cnt, succ;
	unsigned int *nDst;
	unsigned int *nSrc;


	succ = 0;
	nDst = (unsigned int *)(&(vipet_reg->uDN_COEFF_TEM_Y0));
	
	for (i=0; i<5; i++)
	{
		if (dnts_coeff_tmp_luma_list[i].strength == strength)
		{
			nSrc = &(dnts_coeff_tmp_luma_list[i].tmp0);
			for(cnt=0;cnt<2;cnt++)
			{
				*nDst++ = *nSrc++;
			}
			succ = 1;
			break;
		}
	}
	return 1;
}

static int 
cal_dnts_coeff_tmp_chroma_simple( VIPET *vipet_reg, int strength )	{
	unsigned int i,cnt, succ;
	unsigned int *nDst;
	unsigned int *nSrc;

	succ = 0;
	nDst = (unsigned int *)(&(vipet_reg->uDN_COEFF_TEM_C0));
	
	for (i=0; i<5; i++)
	{
		if (dnts_coeff_tmp_chroma_list[i].strength == strength)
		{
			nSrc = &(dnts_coeff_tmp_chroma_list[i].tmp0);
			for(cnt=0;cnt<2;cnt++)
			{
				*nDst++ = *nSrc++;
			}
			succ = 1;
			break;
		}
	}
	return 1;
}



#if 1
VIPET *
GetHwVIPET( void )	{
	return (VIPET *)HwVIPET;
}
/*
VIPET *
GetHwVIPET1( void )	{
	return (VIPET *)HwVIPET1;
}
*/
// ------------------------------------------------------------------------------
// enable/diable histogram
//
// 		input : 	on/off
// 		output :	-
// ------------------------------------------------------------------------------
void 
VIQE_HistogramOnOff( int OnOff )	{
	VIPET	*vipet_reg = GetHwVIPET();

	unsigned int pdfs[8];
	unsigned int cdfs [4];
	unsigned int prev_luts[64];
	unsigned int luts[64];
	int i;
	unsigned int A;
	unsigned int B;
	unsigned int C;
	unsigned int D;
	static int HisState = 0;

//	vipet_reg->uHI_CTRL.nHI_CTRL = (OnOff == 0) ? vipet_reg->uHI_CTRL.nHI_CTRL | Hw9 : vipet_reg->uHI_CTRL.nHI_CTRL & (~Hw9);
	HwVIPET->uHI_CTRL.nHI_CTRL = (OnOff == 1) ? HwVIPET->uHI_CTRL.nHI_CTRL | Hw7 : HwVIPET->uHI_CTRL.nHI_CTRL & (~Hw7);
	return;
//	HwVIPET->uHI_CTRL.nHI_CTRL = HwVIPET->uHI_CTRL.nHI_CTRL | Hw7;
//	if (OnOff == 1)
	if (1)
	{
		if (HisState == 1)
		{
//			printf ("###### sample_size = %d\n", HwVIPET->uHI_CONFIG.bHI_CONFIG.sample_size);
//			printf ("######### PDFs\n");
/*			for (i = 0; i < 8; i++)
			{
				pdfs[i] = HwVIPET->bHI_PDFS.pdfs[i];
//				printf ("pdf[%d] = %d\n", 2*i, pdfs[i] & 0xffff);
//				printf ("pdf[%d] = %d\n", (2*i)+1, (pdfs[i]>>16)&0xffff);
			}*/
//			printf ("######### CDFs\n");
/*			for (i = 0; i < 4; i++)
			{
				cdfs[i] = HwVIPET->bHI_CDFS.cdfs[i];
//				printf ("cdf[%d] = 0x%x\n", i, cdfs[i]);
			}*/
//			printf ("######### LUTs\n");
			for (i = 0; i < 64; i++)
			{
				luts[i] = HwVIPET->bHI_LUTS.luts[i];
//				printf ("luts[%d] = 0x%x\n", i, luts[i]);
			}
			if (OnOff == 1)
			{
				HwVIPET->uHI_CTRL.nHI_CTRL |= Hw8;
				viqe_set_frm_restart();	
				for (i = 0; i < 64; i++)
				{
					 
					A = ((((luts[i]>>24)&0xff) + ((prev_luts[i]>>24)&0xff))/2);
					B = ((((luts[i]>>16)&0xff) + ((prev_luts[i]>>16)&0xff))/2);
					C = ((((luts[i]>>8)&0xff) + ((prev_luts[i]>>8)&0xff))/2);
					D = ((luts[i]&0xff) + (prev_luts[i]&0xff))/2;
					if (A >= 255) A = 255;
					if (B >= 255) B = 255;
					if (C >= 255) C = 255;
					if (D >= 255) D = 255;
					prev_luts[i] = A << 24 | B << 16 | C << 8 | D;
				}
				for (i = 0; i < 64; i++)
				{
					HwVIPET->bHI_LUTS.luts[i] = prev_luts[i];
				}
			}
			else
			{
				viqe_set_frm_restart();	
				for (i = 0; i < 64; i++)
				{
					 
					A = ((((luts[i]>>24)&0xff) + ((prev_luts[i]>>24)&0xff))/2);
					B = ((((luts[i]>>16)&0xff) + ((prev_luts[i]>>16)&0xff))/2);
					C = ((((luts[i]>>8)&0xff) + ((prev_luts[i]>>8)&0xff))/2);
					D = ((luts[i]&0xff) + (prev_luts[i]&0xff))/2;
					if (A >= 255) A = 255;
					if (B >= 255) B = 255;
					if (C >= 255) C = 255;
					if (D >= 255) D = 255;
					prev_luts[i] = A << 24 | B << 16 | C << 8 | D;
				}
			}
		}
		else
		{
			for (i = 0; i < 64; i++)
			{
				HwVIPET->bHI_LUTS.luts[i] = ((4*i+3)<<24) || ((4*i+2)<<16) || ((4*i+1)<<8) || (4*i);
				prev_luts[i] = ((4*i+3)<<24) || ((4*i+2)<<16) || ((4*i+1)<<8) || (4*i);
			}
			
			viqe_set_frm_restart();
		}
		HisState = 1;
	}
	else
	{
		HisState = 0;
	}
/*	if (OnOff == 1)
	{
		viqe_set_histogram_enable( ON, ON, OFF );
	}
	else
	{
		viqe_set_histogram_enable( OFF, OFF, OFF );
	}*/
	return;
}

// ------------------------------------------------------------------------------
// enable/diable gamut-map
//
// 		input : 	on/off
// 		output :	-
// ------------------------------------------------------------------------------
void 
VIQE_GMOnOff( int OnOff )	{
	VIPET	*vipet_reg = GetHwVIPET();

	vipet_reg->uGM_CTRL.nGM_CTRL = (OnOff == 0) ? vipet_reg->uGM_CTRL.nGM_CTRL | Hw31 : vipet_reg->uGM_CTRL.nGM_CTRL & (~Hw31);
	return; 
}

// ------------------------------------------------------------------------------
// enable/disable all blocks for CUI
//
// 		input : 	CUI_VIQE structure
// 		output :	-
// ------------------------------------------------------------------------------
void 
VIQE_FilterOnOff( pTCC_CUI_VIQE CUI_VIQE )	{
	int		DIOnOff;
	int		HpfOnOff;
	int		DntsOnOff;
	int		DnspOnOff;
	int		HiseqOnOff;
	int		GMOnOff;
	int		SpatialOnly;
	int 		OddFirstFlag;
	int		ret;

	VIPET	*vipet_reg = GetHwVIPET();

//	DIOnOff = CUI_VIQE -> Deintl.OnOff;
	if (CUI_VIQE->Deintl.ManualModeFlag == 0)
	{
		DIOnOff = gDeintlOnOff;
		OddFirstFlag = gOddFirstOnOff;	
	}
	else
	{
		DIOnOff = CUI_VIQE->Deintl.OnOff;
		OddFirstFlag = CUI_VIQE->Deintl.OddFirstFlag;
	}
	HpfOnOff = CUI_VIQE -> Hpf.OnOff;
	DntsOnOff = CUI_VIQE -> Dnts.OnOff;
	DnspOnOff = CUI_VIQE -> Dnsp.OnOff;
	HiseqOnOff = CUI_VIQE -> Hiseq.OnOff;
	GMOnOff = CUI_VIQE -> Gamut.OnOff;
	SpatialOnly = CUI_VIQE -> Deintl.SpatialFlag;
//	DnspOnOff = 1;
//	if (HiseqOnOff == 1) ShowVIQE();

	if ( DIOnOff == 0 || ( DIOnOff == 1 && SpatialOnly == 1 ) )	{
		if ( HpfOnOff == 1 && DnspOnOff == 1 )	{
//			printf ("error >> turn on only one filter (dnsp or hpf).\n");
			CUI_VIQE -> Hpf.OnOff = 0;
			CUI_VIQE -> Dnsp.OnOff = 0;
			HpfOnOff = 0;
			DnspOnOff = 0;
		}
	}
	else if ( DIOnOff == 1 && SpatialOnly == 0 )	{
		if ( HpfOnOff == 1 || DnspOnOff == 1 )	{
//			printf ("error >> hpf or dnsp filter is turned off. turn off deintl filter or turn on spatial only flag.\n");
			CUI_VIQE -> Hpf.OnOff = 0;
			CUI_VIQE -> Dnsp.OnOff = 0;
			HpfOnOff = 0;
			DnspOnOff = 0;
		}
	}

	VIQE_HistogramOnOff( HiseqOnOff );
	VIQE_GMOnOff( GMOnOff );
	vipet_reg->uTOP_IMGCONF.nTOP_IMGCONF= OddFirstFlag ? (vipet_reg->uTOP_IMGCONF.nTOP_IMGCONF|Hw0) : (HwVIPET->uTOP_IMGCONF.nTOP_IMGCONF&(~Hw0));
	vipet_reg->uTOP_CTRL.nTOP_CTRL = ( (DnspOnOff == 1 || HpfOnOff == 1) ? (vipet_reg->uTOP_CTRL.nTOP_CTRL|Hw6) : (vipet_reg->uTOP_CTRL.nTOP_CTRL&(~Hw6)));
	vipet_reg->uTOP_CTRL.nTOP_CTRL = ( (DntsOnOff == 1) ? (vipet_reg->uTOP_CTRL.nTOP_CTRL|Hw5) : (vipet_reg->uTOP_CTRL.nTOP_CTRL&(~Hw5)));
	vipet_reg->uTOP_CTRL.nTOP_CTRL = ( (DIOnOff == 1/* && SpatialOnly == 0*/) ? (vipet_reg->uTOP_CTRL.nTOP_CTRL|Hw4) : (vipet_reg->uTOP_CTRL.nTOP_CTRL&(~Hw4)));	
	vipet_reg->uTOP_CTRL.nTOP_CTRL = ( (HpfOnOff == 1) ? (vipet_reg->uTOP_CTRL.nTOP_CTRL|Hw10) : (vipet_reg->uTOP_CTRL.nTOP_CTRL&(~Hw10)));
	vipet_reg->uTOP_CTRL.nTOP_CTRL = ( (DIOnOff == 1 && SpatialOnly == 1) ? (vipet_reg->uTOP_CTRL.nTOP_CTRL|Hw12) : (vipet_reg->uTOP_CTRL.nTOP_CTRL&(~Hw12)));
	if ( DntsOnOff == 1 && DnspOnOff == 1 ) {
		vipet_reg->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
		vipet_reg->uTOP_CTRL.nTOP_CTRL |= Hw9;		
	}
	else if ( DntsOnOff == 1 && HpfOnOff == 1 ) {
		vipet_reg->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
		vipet_reg->uTOP_CTRL.nTOP_CTRL |= Hw9;		
	}
	else if ( DntsOnOff == 1 ) {
		vipet_reg->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
	}
	else if ( DnspOnOff == 1 || HpfOnOff == 1 ) {
		vipet_reg->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
		vipet_reg->uTOP_CTRL.nTOP_CTRL |= Hw8;		
	}
	// deinterlacer first
	if ( DIOnOff == 1 && SpatialOnly == 0 && HpfOnOff == 1 )	{	
		vipet_reg->uTOP_CTRL.nTOP_CTRL |= Hw13;		
	}
	else	{
		vipet_reg->uTOP_CTRL.nTOP_CTRL &= ~Hw13;
	}
	// need previous frame data
	if ( ( DIOnOff == 1 && SpatialOnly == 0 ) || DntsOnOff == 1 ) {
		vipet_reg->uTOP_CTRL.nTOP_CTRL |= Hw2;		
	}
	else {
		vipet_reg->uTOP_CTRL.nTOP_CTRL &= ~Hw2;		
	}

//	viqe_set_denoise_comp( ON );

	// compressor for de-noiser recursive path
	if ( DntsOnOff == 1 )	{
		vipet_reg->uTOP_CTRL.nTOP_CTRL |= Hw3;
		vipet_reg->uTOP_CTRL.nTOP_CTRL |= Hw7;	
	}
	else	{
		vipet_reg->uTOP_CTRL.nTOP_CTRL &= ~Hw3;
		vipet_reg->uTOP_CTRL.nTOP_CTRL &= ~Hw7;	
	}	

//	vipet_reg->uTOP_CTRL.nTOP_CTRL &= ~Hw3;
//	vipet_reg->uTOP_CTRL.nTOP_CTRL &= ~Hw7;	
	ret = 1;
	vipet_reg->uTOP_CTRL.nTOP_CTRL = ret ? (vipet_reg->uTOP_CTRL.nTOP_CTRL|Hw16) : (vipet_reg->uTOP_CTRL.nTOP_CTRL&(~Hw16));
	vipet_reg->uTOP_CTRL.nTOP_CTRL = ret ? (vipet_reg->uTOP_CTRL.nTOP_CTRL|Hw1) : (vipet_reg->uTOP_CTRL.nTOP_CTRL&(~Hw1));
	vipet_reg->uTOP_CTRL.nTOP_CTRL = ret ? (vipet_reg->uTOP_CTRL.nTOP_CTRL|Hw0) : (vipet_reg->uTOP_CTRL.nTOP_CTRL&(~Hw0));
	return;
}


// ------------------------------------------------------------------------------
// set parameter for dnsp( spatial-only denoiser )
//
// 		input : 	strength 0 ~ 4
// 		output :	-
// ------------------------------------------------------------------------------
void 
VIQE_SetDnspParam( int Strength )	{
	VIPET	*vipet_reg = GetHwVIPET();

	cal_d2d_lpf_coeff_simple( vipet_reg, Strength );
	return;
}

// ------------------------------------------------------------------------------
// set parameter for high-pass filter
//
// 		input : 	strength 0 ~ 4
// 		output :	-
// ------------------------------------------------------------------------------
void 
VIQE_SetHpfParam( int LumaStrength, int ChromaStrength )	{
	VIPET	*vipet_reg = GetHwVIPET();

	cal_d2d_hpf_luma_coeff_simple( vipet_reg, LumaStrength );
	cal_d2d_hpf_chroma_coeff_simple( vipet_reg, ChromaStrength );
	return;
}

// ------------------------------------------------------------------------------
// set parameter for dnts( temporal-spatial denoiser )
//
// 		input : 	spatial-luma, spatial-chroma, temporal-luma, temporal-chroma
//						strength range : 0 ~ 4
// 		output :	-
// ------------------------------------------------------------------------------
void 
VIQE_SetDntsParam( int SpatialLumaStrength, int SpatialChromaStrength,
		int TmpLumaStrength, int TmpChromaStrength )	{

	VIPET	*vipet_reg = GetHwVIPET();

	cal_dnts_coeff_luma_simple( vipet_reg, SpatialLumaStrength );
	cal_dnts_coeff_chroma_simple( vipet_reg, SpatialChromaStrength );
	cal_dnts_coeff_tmp_luma_simple( vipet_reg, TmpLumaStrength );
	cal_dnts_coeff_tmp_chroma_simple( vipet_reg, TmpChromaStrength );
	
	return;
}

// ------------------------------------------------------------------------------
// set parameter for histogram equalizer
//
// 		input : 	strength 0 ~ 255, multi-frame number
// 		output :	-
// ------------------------------------------------------------------------------
void 
VIQE_SetHistogramParam( int Strength [16], int MultiFrame )	{
	VIPET	*vipet_reg = GetHwVIPET();

	int		i;
	for ( i = 0; i < 16; i += 4 )	{
		vipet_reg->bHI_SCALES.scales[i/4] = (Strength[i+3])<<24|(Strength[i+2])<<16|(Strength[i+1])<<8|(Strength[i]);
	}
	vipet_reg->uHI_CTRL.nHI_CTRL &= ~(Hw17 | Hw18 | Hw19);
	vipet_reg->uHI_CTRL.nHI_CTRL |= (MultiFrame << 17);
}

// ------------------------------------------------------------------------------
// set parameter for histogram equalizer
//
// 		input : 	strength 0 ~ 255, multi-frame number
// 		output :	-
// ------------------------------------------------------------------------------
void 
VIQE_SetGMParam( pTCC_CUI_VIQE CUI_VIQE )	{
	VIPET	*vipet_reg = GetHwVIPET();

	static int FirstTime = 1; 
	if (/*FirstTime == 1 &&*/ CUI_VIQE->Gamut.OnOff == 1)
	{
		FirstTime = 0;
		if (CUI_VIQE->Gamut.LUTUpdated == 1)
		{
//			GamutLUTGenAlg0 (CUI_VIQE->Gamut.DefaultStrength);
			LoadHwLUT3D (HwVIPET);
			//printf ("LUT Loaded\n");
			CUI_VIQE->Gamut.LUTUpdated = 0;
		}
/*		switch (CUI_VIQE->Gamut.AlgorithmNumber)
		{
			case 0 : 
//				viqe_expand_0_gamut_map (CUI_VIQE->Gamut.DefaultStrength); 
				CLUTGen_Interface_Example (
//					HwVIPET1, HwVIPET, 
					CUI_VIQE->Gamut.LUVMode,
					CUI_VIQE->Gamut.BStrength,
					CUI_VIQE->Gamut.CyanStrength,
					CUI_VIQE->Gamut.GStrength,
					CUI_VIQE->Gamut.YellowStrength,
					CUI_VIQE->Gamut.RStrength,
					CUI_VIQE->Gamut.MagentaStrength
					);
				
				MakeHwLUT3D (HwVIPET1, HwVIPET);
				break;
			case 1 : 
				break;
		}*/
	}
	return;
}

void
VIQE_SetHuff( pTCC_CUI_VIQE CUI_VIQE )	{
	VIPET	*vipet_reg = GetHwVIPET();

	vipet_reg->uRD_CTRL.nRD_CTRL &= (Hw12 | Hw13 | Hw14 | Hw15);
	vipet_reg->uRD_CTRL.nRD_CTRL |= (Hw13 | Hw14);
	vipet_reg->uCD_CTRL.nCD_CTRL &= (Hw8 | Hw9 | Hw10 | Hw11);
	vipet_reg->uCD_CTRL.nCD_CTRL |= (Hw9 | Hw10);
	return;
}

void 
VIQE_SetTimeGen( pTCC_CUI_VIQE CUI_VIQE )	{
	VIPET	*vipet_reg = GetHwVIPET();

	vipet_reg->uTOP_TIMEGEN.nTOP_TIMEGEN &= ~0x00000fff;
	return;
}

// ------------------------------------------------------------------------------
// region setting for all blocks
//
// 		input : 	 on/off, top-left point and bottom-right point
// 		output :	-
// ------------------------------------------------------------------------------
void 
VIQE_SetProcessingRegion( int HalfFlag, unsigned int Width, unsigned int Height )	{
	viqe_set_deintl_region( HalfFlag, 0, Width/2, 0, Height );
	viqe_set_dnts_region( HalfFlag, ON, Width/2 );
	viqe_set_dnsp_region( HalfFlag, ON, Width/2 );
	if (HalfFlag == 1)
		viqe_set_gamut_region( 1, 0, Width/2, 0, Height );
	else
		viqe_set_gamut_region( 1, 0, 0, 0, Height );
	viqe_set_his_region( HalfFlag, 0, Width/2, 0, Height );
	return;
}

void 
VIQE_ShowHuffCnt( int DntsOnOff )	{
#ifdef LINUX_OS_INCLUDE
	VIPET *TmpVIPET = GetHwVIPET();
	pTCC_CUI_VIQE CUI_VIQE = Get_Tcc_Cui_VIQE();
	if ( CUI_VIQE->Dnts.OnOff == 1 )	{
		printf( "%d, %d, %d\n", TmpVIPET->uCD_COMP_COEFF0.uCOMP_COEFF0,
						TmpVIPET->uCD_COMP_COEFF1.uCOMP_COEFF1,
						TmpVIPET->uCD_COMP_COEFF2.uCOMP_COEFF2 );
	}
	return;
#endif		// LINUX_OS_INCLUDE
}

void
VIQE_show_fifo_state( void )	{
#ifdef LINUX_OS_INCLUDE
	VIPET *TmpVIPET = GetHwVIPET();
	printf ("err!! > OD_STATE = 0x%x, RD_FIFOSTATE = 0x%x, DN_FIFOSTATE = 0x%x\n", TmpVIPET->uOD_STATE, 
		TmpVIPET->uRD_FIFOSTATE.nDN_FIFOSTATE, TmpVIPET->uDN_FIFOSTATE.nDN_FIFOSTATE);
#endif		// LINUX_OS_INCLUDE
}
#endif

void 
VIQE_get_his_status( void )	{
#ifdef LINUX_OS_INCLUDE
	printf ("############ HIS\n");
	printf ("CTRL = 0x%x\n", HwVIPET->uHI_CTRL.nHI_CTRL);
	printf ("STATUS = 0x%x\n", HwVIPET->uHI_STATUS.nHI_STATUS);
	printf ("CONFIG = 0x%x\n", HwVIPET->uHI_CONFIG.nHI_CONFIG);
	printf ("REGION_0 = 0x%x\n", HwVIPET->uHI_REGION_0.nHI_REGION_0);
	printf ("REGION_1 = 0x%x\n", HwVIPET->uHI_REGION_1.nHI_REGION_1);
#endif		// LINUX_OS_INCLUDE
	return;
}



#if 0
void 
TCC_VIQE_HistogramOnOff( int OnOff )	{
	HwVIPET->uHI_CTRL.nHI_CTRL = (OnOff == 1) ? HwVIPET->uHI_CTRL.nHI_CTRL | Hw7 : HwVIPET->uHI_CTRL.nHI_CTRL & (~Hw7);
	return;
}

void 
TCC_VIQE_GMOnOff( int OnOff )	{
	HwVIPET->uGM_CTRL.nGM_CTRL = (OnOff == 0) ? HwVIPET->uGM_CTRL.nGM_CTRL | Hw31 : HwVIPET->uGM_CTRL.nGM_CTRL & (~Hw31);
	return; 
}

void 
TCC_VIQE_FilterOnOff( int Deintl_OnOff, int Deintl_SpatialOnlyFlag,
		int Dnts_OnOff, int Dnsp_OnOff,
		int Hpf_OnOff, int Hiseq_OnOff,
		int Gamut_OnOff )	{
	int DIOnOff;
	int HpfOnOff;
	int DntsOnOff;
	int DnspOnOff;
	int HiseqOnOff;
	int GMOnOff;
	int SpatialOnly;
	int ret;
	DIOnOff = Deintl_OnOff;
	HpfOnOff = Hpf_OnOff;
	DntsOnOff = Dnts_OnOff;
	DnspOnOff = Dnsp_OnOff;
	HiseqOnOff = Hiseq_OnOff;
	GMOnOff = Gamut_OnOff;
	SpatialOnly = Deintl_SpatialOnlyFlag;
//	DnspOnOff = 1;

/*	if (DIOnOff == 0 || (DIOnOff == 1 && SpatialOnly == 1))
	{
		if (HpfOnOff == 1 && DnspOnOff == 1)
		{
//			printf ("error >> turn on only one filter (dnsp or hpf).\n");
			CUI_VIQE -> Hpf.OnOff = 0;
			CUI_VIQE -> Dnsp.OnOff = 0;
			HpfOnOff = 0;
			DnspOnOff = 0;
		}
	}
	else if (DIOnOff == 1 && SpatialOnly == 0)
	{
		if (HpfOnOff == 1 || DnspOnOff == 1)
		{
//			printf ("error >> hpf or dnsp filter is turned off. turn off deintl filter or turn on spatial only flag.\n");
			CUI_VIQE -> Hpf.OnOff = 0;
			CUI_VIQE -> Dnsp.OnOff = 0;
			HpfOnOff = 0;
			DnspOnOff = 0;
		}
	}
*/
	TCC_VIQE_HistogramOnOff( HiseqOnOff );
	TCC_VIQE_GMOnOff( GMOnOff );

	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( (DnspOnOff == 1 || HpfOnOff == 1) ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw6) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw6)));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( (DntsOnOff == 1) ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw5) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw5)));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( (DIOnOff == 1/* && SpatialOnly == 0*/) ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw4) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw4)));	
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( (HpfOnOff == 1) ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw10) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw10)));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ( (DIOnOff == 1 && SpatialOnly == 1) ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw12) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw12)));
	if (DntsOnOff == 1 && DnspOnOff == 1) {
		HwVIPET->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw9;		
	}
	else if (DntsOnOff == 1 && HpfOnOff == 1) {
		HwVIPET->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw9;		
	}
	else if (DntsOnOff == 1) {
		HwVIPET->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
	}
	else if (DnspOnOff == 1 || HpfOnOff == 1) {
		HwVIPET->uTOP_CTRL.nTOP_CTRL &=~ (Hw8|Hw9);
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw8;		
	}
	// deinterlacer first
	if ( DIOnOff == 1 && SpatialOnly == 0 && HpfOnOff == 1 )	{	
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw13;		
	}
	else	{
		HwVIPET->uTOP_CTRL.nTOP_CTRL &= ~Hw13;
	}
	// need previous frame data
	if ( (DIOnOff == 1 && SpatialOnly == 0) || DntsOnOff == 1) {
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw2;		
	}
	else {
		HwVIPET->uTOP_CTRL.nTOP_CTRL &= ~Hw2;		
	}
//	viqe_set_denoise_comp( ON );
	if ( DntsOnOff == 1)	{
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw3;
		HwVIPET->uTOP_CTRL.nTOP_CTRL |= Hw7;	
	}
	else	{
		HwVIPET->uTOP_CTRL.nTOP_CTRL &= ~Hw3;
		HwVIPET->uTOP_CTRL.nTOP_CTRL &= ~Hw7;	
	}	
//	vipet_reg->uTOP_CTRL.nTOP_CTRL &= ~Hw3;
//	vipet_reg->uTOP_CTRL.nTOP_CTRL &= ~Hw7;	
	ret = 1;
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ret ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw16) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw16));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ret ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw1) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw1));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = ret ? (HwVIPET->uTOP_CTRL.nTOP_CTRL|Hw0) : (HwVIPET->uTOP_CTRL.nTOP_CTRL&(~Hw0));
	return;
}



void TCC_VIQE_SetDnspParam (int Strength)
{
	cal_d2d_lpf_coeff_simple (HwVIPET, Strength);
	return;
}

void TCC_VIQE_SetHpfParam (int LumaStrength, int ChromaStrength)
{
	cal_d2d_hpf_luma_coeff_simple (HwVIPET, LumaStrength);
	cal_d2d_hpf_chroma_coeff_simple (HwVIPET, ChromaStrength);
	return;
}

void TCC_VIQE_SetDntsParam (VIPET *vipet_reg, 
	int SpatialLumaStrength, 
	int SpatialChromaStrength,
	int TmpLumaStrength,
	int TmpChromaStrength)
{
	cal_dnts_coeff_luma_simple (vipet_reg, SpatialLumaStrength);
	cal_dnts_coeff_chroma_simple (vipet_reg, SpatialChromaStrength);
	cal_dnts_coeff_tmp_luma_simple(vipet_reg, TmpLumaStrength);
	cal_dnts_coeff_tmp_chroma_simple(vipet_reg, TmpChromaStrength);
	
	return;
}

void TCC_VIQE_SetHistogramParam (int Strength [16])	{
	int		i;
	for ( i = 0; i < 16; i += 4 )	{
		HwVIPET->bHI_SCALES.scales[i/4] = (Strength[i+3])<<24|(Strength[i+2])<<16|(Strength[i+1])<<8|(Strength[i]);
	}
//	HwVIPET->uHI_CTRL.nHI_CTRL &= ~(Hw17 | Hw18 | Hw19); // multi_frame setting
//	HwVIPET->uHI_CTRL.nHI_CTRL |= (MultiFrame << 17);
}

void TCC_VIQE_SetGMParam (int Gamut_OnOff, int Gamut_AlgorithmNumber, double Gamut_Strength)
{
	if (Gamut_OnOff == 1)
	{
		switch (Gamut_AlgorithmNumber)
		{
			case 0 : 
//				printf ("GMStrength = %f\n", Gamut_Strength);
//				viqe_expand_0_gamut_map (Gamut_Strength); 
//				CLUTGen_Interface_Example (HwVIPET1, HwVIPET);
				break;
			case 1 : 
				break;
		}
	}
	return;
}

void TCC_VIQE_SetHalfMode (int HalfFlag, unsigned int Width, unsigned int Height)
{
	viqe_set_deintl_region( HalfFlag, 0, Width/2, 0, Height);
	viqe_set_dnts_region( HalfFlag, ON, Width/2);
	viqe_set_dnsp_region( HalfFlag, ON, Width/2);

	viqe_set_gamut_region( HalfFlag, 0, Width/2, 0, Height );
	return;
}
#endif		// #if 0

void gen_luts_addr( int luts_addr[8], int y, int u, int v )	{

	luts_addr[0] = (( v + 1 ) >>1) + 5 * (( u + 1 ) >>1) + 25 * (( y + 1 ) >>1);
	luts_addr[1] = (( v     ) >>1) + 4 * (( u + 1 ) >>1) + 20 * (( y + 1 ) >>1);
	luts_addr[2] = (( v + 1 ) >>1) + 5 * (( u     ) >>1) + 20 * (( y + 1 ) >>1);
	luts_addr[3] = (( v     ) >>1) + 4 * (( u     ) >>1) + 16 * (( y + 1 ) >>1);
	luts_addr[4] = (( v + 1 ) >>1) + 5 * (( u + 1 ) >>1) + 25 * (( y     ) >>1);
	luts_addr[5] = (( v     ) >>1) + 4 * (( u + 1 ) >>1) + 20 * (( y     ) >>1);
	luts_addr[6] = (( v + 1 ) >>1) + 5 * (( u     ) >>1) + 20 * (( y     ) >>1);
	luts_addr[7] = (( v     ) >>1) + 4 * (( u     ) >>1) + 16 * (( y     ) >>1);
}

void ShowVIQE ()
{
	printf ("di_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.di_en);
	printf ("dnts_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.dnts_en);
	printf ("dnsp_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.dnsp_en);
	printf ("cdma_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.cdma_en);
	printf ("denoise_mode = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.denoise_mode);
	printf ("hpf_mode = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.hpf_mode);
	printf ("di_spatial_only_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.di_spatial_only_en);
	printf ("first_deintl_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.first_deintl_en);
	printf ("outdma_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.first_deintl_en);
	printf ("gamut_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.gamut_en);
	printf ("his_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.his_en);
	printf ("his_cdf_or_lut_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.his_cdf_or_lut_en);
	printf ("GM_CTRL = 0x%x\n", HwVIPET->uGM_CTRL.nGM_CTRL);
	printf ("GM_STATUS = 0x%x\n", HwVIPET->uGM_STATUS.nGM_STATUS);
	printf ("GM_INT = 0x%x\n", HwVIPET->uGM_INT.nGM_INT);
	printf ("OD_STATE = 0x%x\n", HwVIPET->uOD_STATE);
	printf ("RD_FIFOSTATE = 0x%x\n", HwVIPET->uRD_FIFOSTATE.nDN_FIFOSTATE);
	printf ("RD_FIFOSTATE = 0x%x\n", HwVIPET->uRD_FIFOSTATE.nDN_FIFOSTATE);
	printf ("RD_LINE_STATE0 = 0x%x\n", HwVIPET->uRD_LINE_STATE0.nRD_LINE_STATE);
	printf ("RD_LINE_STATE1 = 0x%x\n", HwVIPET->uRD_LINE_STATE1.nRD_LINE_STATE1);
	printf ("RD_CTRL = 0x%x\n", HwVIPET->uRD_CTRL.nRD_CTRL);
	printf ("dnts-deintl = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.first_deintl_en);
	printf ("fmt_conv_disable = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.fmt_conv_disable);
	printf ("rdma2_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.rdma2_en);
	printf ("cdma_en = %d\n", HwVIPET->uTOP_CTRL.bTOP_CTRL.cdma_en);
	int Offset;
	unsigned int *ptr;
	ptr = HwVIPET;
	for (Offset = 0; Offset < (sizeof (VIPET)/4); Offset += 1)
//	for (Offset = (0x620/4); Offset < (0x7FC/4); Offset += 1)
	{
		printf ("0x%x : 0x%x\n", 4*Offset, *(ptr + Offset));
	}
//	FILE *Output;
//	Output = fopen ("VIQE_STATE4", "wb");
//	fwrite (HwVIPET, sizeof (VIPET), 1, Output);
//	fclose (Output);
/*	{
		int i;
		unsigned int *ptr;
		printf ("####### ScalerState ########\n");
		ptr = (unsigned int *)mmap(0, LOGICAL_ADDR_MIN, PROT_READ | PROT_WRITE, MAP_SHARED, viqe_dev_fd, (unsigned int)0xF0210000);
		for (i = 0; i < 21; i++)
		{
			printf ("0x%x : 0x%x\n", 4*i, *(ptr+i));
		}
		munmap((void *)ptr, LOGICAL_ADDR_MIN);
	}*/
/*	{
		int i;
		unsigned int *ptr;
		printf ("####### ScalerState ########\n");
		ptr = GetCodecBase ();
		for (i = 0; i < 100; i++)
		{
			printf ("0x%x : 0x%x\n", 4*i, *(ptr+i));
		}*/
//		munmap((void *)ptr, LOGICAL_ADDR_MIN);
//	}
/*	{
		int i;
		unsigned int *ptr;
		printf ("####### LCDC ########\n");
		ptr = (unsigned int *)mmap(0, LOGICAL_ADDR_MIN, PROT_READ | PROT_WRITE, MAP_SHARED, viqe_dev_fd, (unsigned int)0xF0200000);
		for (i = 0; i <= (0xC0/4); i++)
		{
			printf ("0x%x : 0x%x\n", 4*i, *(ptr+i));
		}
		munmap((void *)ptr, LOGICAL_ADDR_MIN);
		printf ("####### END LCDC ########\n");
	}*/
	return;
}

#define	BITCSET(X, CMASK, SMASK)	( (X) = ((((U32)(X)) & ~((U32)(CMASK))) | ((U32)(SMASK))) )

void SR_LCDCtrl (int Brightness, int Contrast, int Hue)
{
	unsigned int *ptr;
	ptr = (unsigned int *)mmap(0, LOGICAL_ADDR_MIN, PROT_READ | PROT_WRITE, MAP_SHARED, viqe_dev_fd, (unsigned int)0xF0204000);
	BITCSET (*(ptr + (0x70/4)), 0x00000600, 3 << 9);
	BITCSET (*(ptr + (0x70/4)), 0x00ff0000, Hue << 16);
	BITCSET (*(ptr + (0x70/4)), 0x0000ff00, Brightness << 8);
	BITCSET (*(ptr + (0x70/4)), 0x000000ff, Contrast << 0);
	munmap ((void *)ptr, LOGICAL_ADDR_MIN);
	return;
}

void VIQE_OddfirstOnOff (int OddFirstFlag)
{
	gOddFirstOnOff = OddFirstFlag;
	return;
}

void VIQE_DeintlOnOff (int OnOff)
{
	gDeintlOnOff = OnOff;
	return;
}


int
IsFilmModeErr ( int FrmCnt, int width, int height ) {
	int RetVal = 0;
	static int PrevCheck = 0;
	static int repeat_cnt_check_out = 0;
	int CurCheck;
	int	repeat_cond_0 = 0;
	int	repeat_cond_1 = 0;
	int	cnt_0 = 0;
	int	cnt_1 = 0;
	int	thres = width*height/7;
//	if (PrevCheck == 0 && CurCheck != 0 || PrevCheck != 0 && CurCheck == 0 ) printf ("CurCheck = %d\n", CurCheck);

/*	
	CurCheck = HwVIPET->uDI_PD_STATUS.bDI_PD_STATUS.check_out;
	printf ("check : %d\n", CurCheck);
	if (PrevCheck != 0 && CurCheck == 0) RetVal = 1;
	else RetVal = 0;
	PrevCheck = CurCheck;
	if (RetVal == 1) printf ("Film mode err\n");
	return RetVal;
*/
	CurCheck = HwVIPET->uDI_PD_STATUS.bDI_PD_STATUS.check_out;

	cnt_0 = HwVIPET->uDI_PD_CNT0_0.nDI_PD_CNT0_0;
	cnt_1 = HwVIPET->uDI_PD_CNT1_0.nDI_PD_CNT1_0;
	
//	printf( "[%d] check_out = %d, maxrange_spa_on = %d\n", FrmCnt, CurCheck, HwVIPET->uDI_CTRL.bDI_CTRL.maxrange_spa_on );
//	printf (">>> check_out = %d, prev_check_out = %d, cnt_0 = %d, cnt_1 = %d\n", CurCheck, PrevCheck, cnt_0, cnt_1 );
//	printf (">>> repeat_cnt_check_out = %d\n", repeat_cnt_check_out );	
	
	// in film-mode, maxrange_spa_on is off
	///*
	if ( CurCheck != 0 )	{
#ifdef VIQE_ASGN
		HwVIPET->uDI_CTRL.nDI_CTRL = HwVIPET->uDI_CTRL.nDI_CTRL & (~Hw5);
#else
		HwVIPET->uDI_CTRL.bDI_CTRL.maxrange_spa_on = 0;
#endif
	}
	else	{
#ifdef VIQE_ASGN
		HwVIPET->uDI_CTRL.nDI_CTRL = HwVIPET->uDI_CTRL.nDI_CTRL | (Hw5);
#else
		HwVIPET->uDI_CTRL.bDI_CTRL.maxrange_spa_on = 1;
#endif

	}
	//*/
/*
	if ( repeat_cnt_check_out != 0 && repeat_cnt_check_out != 4 )	{
		repeat_cnt_check_out++;
#ifdef VIQE_ASGN
		HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 = HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 & (~(Hw14|Hw15));
#else
		HwVIPET->uDI_PD_THRES0.bDI_PD_THRES0.pd_user_checkout = 0;
#endif
		CurCheck = 0;
	}
	else	 if ( repeat_cnt_check_out != 0 )	{
		repeat_cnt_check_out = 0;
#ifdef VIQE_ASGN
		HwVIPET->uDI_CTRL.nDI_CTRL = HwVIPET->uDI_CTRL.nDI_CTRL & (~Hw12);
#else
		HwVIPET->uDI_CTRL.bDI_CTRL.pd_user_checkout_enable = 0;
#endif
	}


	repeat_cond_0 = ( CurCheck == 0 && PrevCheck != 0 );
	if ( repeat_cnt_check_out != 0 && repeat_cnt_check_out != 4 )	{
		repeat_cond_1 = 0;
	}
	else	{
		cnt_0 = HwVIPET->uDI_PD_CNT0_0.nDI_PD_CNT0_0;
		cnt_1 = HwVIPET->uDI_PD_CNT1_0.nDI_PD_CNT1_0;		
		repeat_cond_1 = ( CurCheck != 0 && ( cnt_0 > thres || cnt_1 > thres ) );
	}
*/
	repeat_cond_0 = ( CurCheck == 0 && PrevCheck != 0 );


	if ( repeat_cond_0 || repeat_cond_1 )	{
//		printf ("%d) Film mode err : cond_0 = %d, cond_1 = %d\n", FrmCnt, repeat_cond_0, repeat_cond_1 );
//		printf (">>> check_out = %d, prev_check_out = %d, cnt_0 = %d, cnt_1 = %d\n", CurCheck, PrevCheck, cnt_0, cnt_1 );
/*		
#ifdef VIQE_ASGN
		HwVIPET->uDI_CTRL.nDI_CTRL = HwVIPET->uDI_CTRL.nDI_CTRL | (Hw12);
		HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 = HwVIPET->uDI_PD_THRES0.nDI_PD_THRES0 & (~(Hw14|Hw15));
#else
		HwVIPET->uDI_CTRL.bDI_CTRL.pd_user_checkout_enable = 1;
		HwVIPET->uDI_PD_THRES0.bDI_PD_THRES0.pd_user_checkout = 0;
#endif
*/
		CurCheck = 0;

		repeat_cnt_check_out = 1;

		RetVal = 1;
	}
	else	{
		RetVal = 0;
	}
	
	PrevCheck = CurCheck;
	//if (RetVal == 1) printf ("Film mode err\n");
	return RetVal;
	
}

void VIQE_SetOutput (int DirectPahOn, int OutputFormat, viqe_dma_t *dma)
{
	// directpath enable
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uOD_CFG.nOD_CFG = (DirectPahOn ? HwVIPET->uOD_CFG.nOD_CFG|Hw4 : HwVIPET->uOD_CFG.nOD_CFG&(~Hw4));
#else
	HwVIPET->uOD_CFG.bOD_CFG.outpath = cfg->directpath;
#endif		
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uOD_CFG.nOD_CFG |= OutputFormat;
#else
	HwVIPET->uOD_CFG.bOD_CFG.dsttype = OutputFormat;
#endif
#ifdef VIQE_ASGN		// 20090428
	HwVIPET->uOD_OFS.nOD_BASEOFS = (dma->o_offs[1]<<16 | dma->o_offs[0]);
#else
	HwVIPET->uOD_OFS.bOD_BASEOFS.base_ofs0 = dma->o_offs[0];
	HwVIPET->uOD_OFS.bOD_BASEOFS.base_ofs1 = dma->o_offs[1];
#endif
#if 1
	if ( DirectPahOn == 1 )	{
		
#ifdef LINUX_OS_INCLUDE
//		pDDIConfigReg->ON_THE_FLY = (pDDIConfigReg->ON_THE_FLY & 0xfffffffc)  | 0x00000002; // M2M Scaler 0
		pDDIConfigReg->ON_THE_FLY = (pDDIConfigReg->ON_THE_FLY & 0xfffffffc)  | 0x00000003; // M2M Scaler 1
#else
//		*(volatile int *)0xf0251038 = ((*(volatile int *)0xf0251038) & 0xfffffffc) | 0x00000002;
		*(volatile int *)0xf0251038 = ((*(volatile int *)0xf0251038) & 0xfffffffc) | 0x00000003;
#endif
		// mscl, src direct path
		//*(volatile int *)0xf0210044 = (*(volatile int *)0xf0210044) | 1<<24;
	}
	else	{
		
#ifdef LINUX_OS_INCLUDE
		pDDIConfigReg->ON_THE_FLY = (pDDIConfigReg->ON_THE_FLY & 0xfffffffc);
#else
		*(volatile int *)0xf0251038 = ((*(volatile int *)0xf0251038) & 0xfffffffc);
#endif
		// mscl, src direct path
		//*(volatile int *)0xf0210044 = (*(volatile int *)0xf0210044) | 1<<24;
	}
#endif
	return;
}

void TurnOnOffVIQEClk (int OnOff)
{
	pDDIConfigReg->PWDN = (OnOff == 0) ? pDDIConfigReg->PWDN | (Hw1) : pDDIConfigReg->PWDN & (~Hw1);
	return;
}

void TurnOnOffScaler0Clk (int OnOff)
{
	pDDIConfigReg->PWDN = (OnOff == 0) ? pDDIConfigReg->PWDN | (Hw5) : pDDIConfigReg->PWDN & (~Hw5);
	return;
}

void TurnOnOffScaler1Clk (int OnOff)
{
	pDDIConfigReg->PWDN = (OnOff == 0) ? pDDIConfigReg->PWDN | (Hw6) : pDDIConfigReg->PWDN & (~Hw6);
	return;
}

int viqe_get_rdma_done ()	{

	return (( HwVIPET->uRD_LINE_STATE1.nRD_LINE_STATE1)>>31);
}

void ODMARST ()
{
	HwVIPET->uTOP_CTRL.nTOP_CTRL = (HwVIPET->uTOP_CTRL.nTOP_CTRL & (~Hw16));
	HwVIPET->uOD_CONTROL.nOD_CONTROL = (HwVIPET->uOD_CONTROL.nOD_CONTROL | Hw8);
	HwVIPET->uOD_CONTROL.nOD_CONTROL = (HwVIPET->uOD_CONTROL.nOD_CONTROL & (~Hw8));
	HwVIPET->uTOP_CTRL.nTOP_CTRL = (HwVIPET->uTOP_CTRL.nTOP_CTRL | (Hw16));
	return;
}

void ODMAFlush ()
{
	HwVIPET->uOD_CONTROL.nOD_CONTROL = (HwVIPET->uOD_CONTROL.nOD_CONTROL | (Hw31));	
	HwVIPET->uOD_CONTROL.nOD_CONTROL = (HwVIPET->uOD_CONTROL.nOD_CONTROL & (~Hw31));	
	return;
}

