/* vim:set ts=4 shiftwidth=4 */
#ifdef VIQE_INCLUDE
#include 	<stdio.h>
#include 	<math.h>
//#include	"dev_viqe.h"

#include	"viqe_lib.h"
#include	"viqe_cmd.h"

//#include	"tcc_type.h"

#include "TCC89x_Structures.h"
#include "TCC89x_Physical.h"
#include "TCCUtil.h"
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>         // O_RDWR
#include <sys/poll.h>
#define LINUX_OS_INCLUDE
#define LOGICAL_ADDR_MIN	(4*1024)

static viqe_modes_t		viqe_modes;
static viqe_dma_t		viqe_dma;
static viqe_imgfmt_t	viqe_imgfmt;
static viqe_cfg_t		viqe_cfg;
static viqe_t			viqe;
static int				g_viqe_first;
static int				g_viqe_frm_cnt = 0;

static int 	frame_count = 0;
#endif

void 
TCC_DeinitalizeVIQE()
{
#ifdef VIQE_INCLUDE	
	tcc_viqe_deinit();
#endif	
}

void TCC_TurnOnVIQE()
{
#ifdef VIQE_INCLUDE	
	g_viqe_first = 1;
	g_viqe_frm_cnt = 0;
	frame_count = 0;
	tcc_viqe_init();
	viqe_swreset();		
#endif
}

void 
TCC_InitalizeVIQE( unsigned int FrmWidth, unsigned int FrmHeight, int input_color_format, int offset)
{
#ifdef VIQE_INCLUDE
	unsigned int	framebufWidth = FrmWidth;
	unsigned int	framebufHeight = FrmHeight;
	int			compbufHeight;
	int			complumaHeight;
	int			compOneFrameSize;
	unsigned int 	RecursiveMEMBase = get_scaler_physical_address();
	
	memset( &viqe_modes, 0, sizeof(viqe_modes_t) );
	memset( &viqe_dma, 0, sizeof(viqe_dma_t) );
	memset( &viqe_imgfmt, 0, sizeof(viqe_imgfmt_t) );				
	memset( &viqe_cfg, 0, sizeof(viqe_cfg_t) );				
	viqe.modes = &viqe_modes;
	viqe.dma = &viqe_dma;
	viqe.imgfmt = &viqe_imgfmt;
	viqe.cfg = &viqe_cfg;

	viqe.cfg->comp_en = OFF; //srlee 
//	viqe.cfg->comp_en = ON; //srlee 
//	viqe.cfg->frm_manual_restart = ON;
	viqe.cfg->frm_manual_restart = OFF;
	//viqe.cfg->directpath = ON;
//	viqe.cfg->directpath = ON;
	viqe.cfg->directpath = OFF; // M2M
	viqe.cfg->overlap_disable = OFF;


	//viqe.width = framebufWidth;
	//viqe.height = framebufHeight;
	viqe.width = ((framebufWidth + 15) & 0xfffffff0);
	viqe.height = ((framebufHeight + 15) & 0xfffffff0);
	
//	viqe.imgfmt->i_fmt = input_color_format;
//	viqe.imgfmt->i_fmt = FMT_YUV420IL1;
//	viqe.imgfmt->i_fmt = FMT_YUV420SEP;
	viqe.imgfmt->i_fmt = FMT_YUV422SEP;
/*	if ( viqe.cfg->directpath )
		viqe.imgfmt->o_fmt = OD_YUV422SEQ0;
	else
		viqe.imgfmt->o_fmt = OD_YUV420SEP;
*/
	viqe.imgfmt->o_fmt = OD_YUV422SEP;
	
//	viqe.imgfmt->force_420 = OFF;
	viqe.imgfmt->force_420 = OFF;
	viqe.imgfmt->first_frm_init = OFF;
	viqe.imgfmt->fmt_conv_disable = viqe.imgfmt->force_420;
//	viqe.imgfmt->fmt_conv_disable = OFF;
//	viqe.imgfmt->fmt_conv_disable = ON;

	viqe.dma->conti = ON;
	viqe.dma->opt = ON;
	//viqe.dma->opt = OFF;
	//viqe.dma->comp_pingpong = ON;
	viqe.dma->comp_pingpong = OFF;

	viqe.dma->i_offs[0] = framebufWidth;
	if ( viqe.imgfmt->i_fmt == FMT_YUV420SEP ) 
		viqe.dma->i_offs[1] = framebufWidth>>1;
	else if ( (viqe.imgfmt->i_fmt == FMT_YUV420IL0) || (viqe.imgfmt->i_fmt == FMT_YUV420IL1) )
		viqe.dma->i_offs[1] = framebufWidth;
	else
		viqe.dma->i_offs[1] = framebufWidth>>1;
	
	viqe.dma->o_offs[0] = framebufWidth;
	if( (viqe.imgfmt->o_fmt == OD_YUV422SEQ0) || (viqe.imgfmt->o_fmt == OD_YUV422INT) || (viqe.imgfmt->o_fmt == OD_YUV420INT) )
		viqe.dma->o_offs[1] = framebufWidth;
	else
		viqe.dma->o_offs[1] = framebufWidth>>1;
	
	//viqe.dma->o_offs[0] = (*ppVpuDec)->m_iFrameBufferWidth<<1;
	//viqe.dma->o_offs[1] = (*ppVpuDec)->m_iFrameBufferWidth>>1;
	
	complumaHeight = framebufHeight;
	if (viqe.imgfmt->force_420 == ON) compbufHeight = 2*framebufHeight;
	else compbufHeight = framebufHeight;
	
	if ( viqe.dma->comp_pingpong == ON )	{
		viqe.dma->addr_comp0[0] = RecursiveMEMBase;
		viqe.dma->addr_comp0[1] = viqe.dma->addr_comp0[0] + ( framebufWidth*complumaHeight) / 2;
		viqe.dma->addr_comp0[2] = viqe.dma->addr_comp0[1] + ( (framebufWidth*(compbufHeight>>2)) ) /2;
		compOneFrameSize = framebufWidth*((compbufHeight >> 1) + complumaHeight);
		viqe.dma->addr_comp1[0] = viqe.dma->addr_comp0[0] + (compOneFrameSize) / 2;
		viqe.dma->addr_comp1[1] = viqe.dma->addr_comp0[1] + (compOneFrameSize) / 2;
		viqe.dma->addr_comp1[2] = viqe.dma->addr_comp0[2] + (compOneFrameSize) / 2;
	}
	else
	{
		viqe.dma->addr_comp0[0] = RecursiveMEMBase;
		viqe.dma->addr_comp0[1] = viqe.dma->addr_comp0[0] + (( framebufWidth*complumaHeight )/2);
		viqe.dma->addr_comp0[2] = viqe.dma->addr_comp0[1] + (( (framebufWidth*(compbufHeight>>2)) )/2);
	}

	viqe_init( &viqe );
	// XXX: frame-by-frame mode in rdma
	//*(unsigned int *)0xf02521d0 &= ~(1<<8);
	
	// ddi config, viqe->mscl direct path
//	if ( viqe.cfg->directpath )	{
		
		//*(volatile int *)0xf0251038 = ((*(volatile int *)0xf0251038) & 0xfffffffc) | 0x00000002;
//		pDDIConfigReg->ON_THE_FLY = (pDDIConfigReg->ON_THE_FLY & 0xfffffffc)  | 0x00000002;
		//dump_frame(pDDIConfigBase, 0x100);
		// mscl, src direct path
		//*(volatile int *)0xf0210044 = (*(volatile int *)0xf0210044) | 1<<24;
//	}
	// -------------------------------------------
	// mode selection
	viqe.modes->di = OFF;
	//viqe.modes->di =  ON;
	viqe.modes->disp = OFF;
	//viqe.modes->disp = ON;
	viqe.modes->dnts = OFF;
	//viqe.modes->dnts = ON;
	viqe.modes->dnsp = OFF;
	viqe.modes->hpf = OFF;
	//viqe.modes->his = ON;
	//viqe.modes->gamut = ON;
	viqe.modes->his = OFF;
	viqe.modes->gamut = OFF;

	viqe_bypass_gamut( ON );
	// -------------------------------------------
	// Configuration
	viqe_set_deintl_basic_conf( ON, ON );
	viqe_set_deintl_region( ON, 0, framebufWidth, 0, framebufHeight );

	// gamut
//	viqe_expand_0_gamut_map( 5.0 );
//	viqe_comp_rb_gamut_map();
//	viqe_set_gamut_region( ON, 0, framebufWidth,  0, framebufHeight);
	//CLUTGen_Interface_Example();

	#if 1
	// denoise
//	viqe_set_dnts_region( ON, OFF, framebufWidth);
//	viqe_set_dnsp_region( ON, OFF, framebufWidth);

	// histogram
	//viqe_set_histogram_enable( OFF, OFF, OFF );
	if ( viqe.modes->his )	{
//		viqe_set_histogram_scales( 0x30, NULL );
		//viqe_set_histogram_enable( ON, ON, ON );
		viqe_set_histogram_enable( ON, ON, OFF );
	}

	// gamut
	//viqe_expand_0_gamut_map( 1.5 );
	//viqe_set_gamut_region( ON, 0, framebufWidth,  0, framebufHeight);
	//CLUTGen_Interface_Example();

	#endif
	// scaler reset
	//*(unsigned int*)0xf0210044 |= (1<<8);
	return;
#endif	
}

void 
TCC_RunVIQE( unsigned int CurYBase, unsigned int CurUBase, unsigned int CurVBase, 
		unsigned int PrevYBase, unsigned int PrevUBase, unsigned int PrevVBase, unsigned int M2MMode, int OutputFormat, int offset){

//#ifdef VIQE_INCLUDE		
	unsigned int dmadone;
	unsigned int rdmadone;
	unsigned int Interlaced,OddFirst,OutputMEMBase;		
	pTCC_CUI_VIQE CUI_VIQE = Get_Tcc_Cui_VIQE();
	
	OutputMEMBase =0x4FC00000;//get_scaler_physical_address();		
	Interlaced = 0;//GetCodecInfoInterlaced(); //cary
	OddFirst = 1;//GetCodecInfoOddFirst();			//cary
#if 1
	viqe.dma->addr_cur[0] = CurYBase;
	viqe.dma->addr_cur[1] = CurUBase;
	viqe.dma->addr_cur[2] = CurVBase;
#else
	viqe.dma->addr_cur[0] = PrevYBase;
	viqe.dma->addr_cur[1] = PrevUBase;
	viqe.dma->addr_cur[2] = PrevVBase;
#endif
	viqe.dma->addr_pre[0] = PrevYBase;
	viqe.dma->addr_pre[1] = PrevUBase;
	viqe.dma->addr_pre[2] = PrevVBase;
	static int PrevM2MMode = -1;
	if (M2MMode == 1) viqe.cfg->directpath = OFF;
	else viqe.cfg->directpath = ON;
	viqe.imgfmt->o_fmt = OutputFormat;

	viqe.dma->i_offs[0] = offset;
	if ( viqe.imgfmt->i_fmt == FMT_YUV420SEP ) 
		viqe.dma->i_offs[1] = offset>>1;
	else if ( (viqe.imgfmt->i_fmt == FMT_YUV420IL0) || (viqe.imgfmt->i_fmt == FMT_YUV420IL1) )
		viqe.dma->i_offs[1] = offset;
	else 
		viqe.dma->i_offs[1] = offset>>1;
	
	viqe.dma->o_offs[0] = offset;
	if( (viqe.imgfmt->o_fmt == OD_YUV422SEQ0) || (viqe.imgfmt->o_fmt == OD_YUV422INT) || (viqe.imgfmt->o_fmt == OD_YUV420INT) )
		viqe.dma->o_offs[1] = offset;
	else
		viqe.dma->o_offs[1] = offset>>1;
	
	if (PrevM2MMode == -1 || PrevM2MMode != M2MMode || g_viqe_first == 1)
	{
		ODMAFlush ();
		VIQE_SetOutput (viqe.cfg->directpath, viqe.imgfmt->o_fmt, viqe.dma);
		ODMARST ();
		PrevM2MMode = M2MMode;
	}
	if ( !viqe.cfg->directpath )	{
		viqe.dma->addr_out[0] = (unsigned int)OutputMEMBase;
		viqe.dma->addr_out[1] = (unsigned int)OutputMEMBase + (viqe.width*viqe.height);
		viqe.dma->addr_out[2] = (unsigned int)OutputMEMBase + (viqe.width*viqe.height) + (viqe.width*viqe.height>>1);
	}

	//viqe_set_dma_addr( viqe.dma );
	viqe_set_dma( viqe.dma );
	VIQE_DeintlOnOff (Interlaced);
	VIQE_OddfirstOnOff (OddFirst);

	if ( g_viqe_first )	{
		if ( viqe_set_modes( viqe.modes ) < 0 )	{
			printf("RETCODE_FAILURE!!\n");
			return;
		}
		if (CUI_VIQE->Gamut.OnOff == 1)
		{
			CUI_VIQE->Gamut.LUTUpdated = 1;
		}
		g_viqe_first = 0;
		printf ("process first frm\n");
	}
	else	{
		if ( !g_viqe_first/* && viqe.cfg->frm_manual_restart */ )	{	
			TCC_ConfigVIQEMain (viqe.width, viqe.height, CUI_VIQE);
			viqe_set_frm_restart();	
		}	
	}
	if (M2MMode == 1) VIQEFinalize ();
	return;
//#endif //	#ifdef VIQE_INCLUDE	
}

//#define CORRECT_DI_ERR
void TCC_ExecuteVIQE( unsigned int src_width, unsigned int src_height, unsigned int dst_width, unsigned int dst_height, int *src_addr, unsigned int dest_y , unsigned int h_offset, unsigned int v_offset, int need_offset, int w_offset)
{
	int i, ChromaCorrectedScale;	

	ChromaCorrectedScale = 0;
/*
#ifdef CORRECT_DI_ERR
		for (i = 0; i <2; i++)
#endif
*/
		{
			if (ChromaCorrectedScale == 0)
			{
//				TurnOnOffScaler1Clk (1);
//				TurnOnOffVIQEClk (1);
				
				TCC_SetVIQEResolution(&viqe, src_width, src_height);
				TCC_RunVIQE (src_addr[0], src_addr[1], src_addr[2], src_addr[3], src_addr[4], src_addr[5], 1, OD_YUV422SEP, w_offset);
				/* for zoom function */
				
				{
					M2M_ScalerForVideo_With_Address(src_width, src_height, dst_width, dst_height, src_addr[0], src_addr[1], src_addr[2],  dest_y, v_offset, h_offset, 0);
				}
	//			VIQEFinalize ();
/*
#ifdef CORRECT_DI_ERR
				if (!IsFilmModeErr(frame_count, src_width, src_height)) 
				{
					TurnOnOffVIQEClk (0);
					TurnOnOffScaler1Clk (0);
					break;
				}
#endif
*/
	//			TurnOnOffVIQEClk (0);
	//			TurnOnOffScaler1Clk (0);
			}
			else
			{
/*				TurnOnOffVIQEClk (1);
				TCC_RunVIQE (src_addr[0], src_addr[1], src_addr[2], src_addr[3], src_addr[4], src_addr[5], 1, OD_YUV420INT, w_offset);
#ifdef CORRECT_DI_ERR
				if (IsFilmModeErr(frame_count, src_width, src_height) && i == 0) 
				{
					continue;
				}
#endif
				TurnOnOffScaler0Clk (1);
				TurnOnOffVIQEClk (0);
				M2M_ScalerForVideo_With_Address_Stg1(src_width, src_height/2, src_width, src_height, 
									get_scaler_physical_address()+src_width*src_height, src_addr[1], src_addr[2], 
									get_scaler_physical_address()+(src_width*src_height*3)/2, 
									v_offset, h_offset, 0);				
				M2M_ScalerForVideo_With_Address_Stg2(src_width, src_height, dst_width, dst_height, 
									get_scaler_physical_address(), 
									get_scaler_physical_address()+(src_width*src_height*3)/2, 
									get_scaler_physical_address()+(src_width*src_height*3)/2+ (src_width*src_height)/2, 
									dest_y, v_offset, h_offset, 0);
				TurnOnOffScaler0Clk (0);
#ifdef CORRECT_DI_ERR
				break;
#endif
*/
			}			
		}		

		frame_count++;
/*#else ////#ifdef VIQE_INCLUDE

		if(need_offset)
		{
			M2M_ScalerForVideo_With_Offset(src_width, src_height, dst_width, dst_height, src_addr[0], src_addr[1], src_addr[2], dest_y, w_offset, 0);
		}
		else
		{
			M2M_ScalerForVideo_With_Address(src_width, src_height, dst_width, dst_height, src_addr[0], src_addr[1], src_addr[2], dest_y, v_offset, h_offset, 0);
		}
#endif //#ifdef VIQE_INCLUDE*/
}

#ifdef VIQE_INCLUDE
void TCC_SetVIQEResolution(viqe_t *viqe, int width, int height)
{
	viqe_set_resolution(viqe, width, height);
}

int 
GetVIQEFrmCnt( void )	{ 
	return g_viqe_frm_cnt; 
}

void 
IncVIQEFrmCnt( void )	{ 
//	g_viqe_frm_cnt++; 
}

void 
TCC_ConfigVIQEMain( unsigned int ImgWidth, unsigned int ImgHeight, pTCC_CUI_VIQE CUI_VIQE)	{
//	pTCC_CUI_VIQE CUI_VIQE = Get_Tcc_Cui_VIQE ();
	static int StepFirstTime = 1;

	if ( CUI_VIQE->Monitor == 1 )	{
		VIQE_show_fifo_state();
	}
	while (1)	{
		if (CUI_VIQE->StepOnOff == 1 && CUI_VIQE->StepEvent == 1) { StepFirstTime = 1; break; }
		if (CUI_VIQE->StepOnOff == 0) { StepFirstTime = 1; break; }
		if (StepFirstTime == 1)
		{
			StepFirstTime = 0;
		}
	}
	
	if (CUI_VIQE->StepEvent == 1) CUI_VIQE->StepEvent = 0;
	if (CUI_VIQE->RandomMode == 1)
	{
		RandomGenCUI ();
	}
	//SR_LCDCtrl (0x0, 0x25, 0);
	VIQE_FilterOnOff( CUI_VIQE );
	VIQE_SetProcessingRegion( CUI_VIQE->HalfProcessing, ImgWidth, ImgHeight );
	VIQE_SetDnspParam( CUI_VIQE -> Dnsp.Strength );
	VIQE_SetHpfParam( CUI_VIQE -> Hpf.LumaStrength, CUI_VIQE->Hpf.ChromaStrength );
	VIQE_SetDntsParam( CUI_VIQE -> Dnts.SpatialLumaStrength, CUI_VIQE -> Dnts.SpatialChromaStrength, 
		CUI_VIQE -> Dnts.TmpLumaStrength, CUI_VIQE -> Dnts.TmpChromaStrength );
	VIQE_SetHistogramParam( CUI_VIQE -> Hiseq.Strength, CUI_VIQE -> Hiseq.MultiFrame );
	VIQE_SetGMParam( CUI_VIQE );
//	VIQE_SetHuff( CUI_VIQE );
	return;
}

void VIQEFinalize ()
{
	int dmadone;
	int rdmadone;
	int i;
//	printf ("wait\n");
	do	{
		i++;
		dmadone = viqe_get_frame_end();
		rdmadone = viqe_get_rdma_done ();
	}	while( !dmadone || (!rdmadone));
	viqe_set_frame_end();	
//	printf ("return\n");
	return;
//	if ( i > 1 ) printf ("i = %d\n", i);
}

unsigned int 
GetPrevYBase( void ) { 
	return viqe.dma->addr_pre[0];
}


int 
TCC_VIQE_GetScaleOnOff( void )	{
	pTCC_CUI_VIQE CUI_VIQE = Get_Tcc_Cui_VIQE ();
	return CUI_VIQE->ScaleOnOff;
}


void 
RandomGenCUI( void ) {
	static int FirstTime = 1;
	int RandData;
	int ConfigRandData;
	pTCC_CUI_VIQE CUI_VIQE = Get_Tcc_Cui_VIQE ();
	if (FirstTime)
	{
		FirstTime = 0;
		srand ((unsigned) time (NULL));
	}
	RandData = rand () % 15;
	if (RandData == 0)
	{
	printf ("\n############ PrevState #############\n");
	printf ("Deintl = %d\n", CUI_VIQE->Deintl.OnOff);
	printf ("Dnts = %d\n", CUI_VIQE->Dnts.OnOff);
	printf ("Hiseq = %d\n", CUI_VIQE->Hiseq.OnOff);
	printf ("Dnsp = %d\n", CUI_VIQE->Dnsp.OnOff);
	printf ("Hpf = %d\n", CUI_VIQE->Hpf.OnOff);
	printf ("Gamut = %d\n", CUI_VIQE->Gamut.OnOff);
	printf ("HiseqStrength = %d\n", CUI_VIQE->Hiseq.Strength);
//	printf ("GamutStrength = %f\n", CUI_VIQE->Gamut.DefaultStrength);


	RandData = rand () % 8;
	
	if (RandData == 0) CUI_VIQE->Deintl.OnOff = !(CUI_VIQE->Deintl.OnOff);
	if (RandData == 1) CUI_VIQE->Dnts.OnOff = !(CUI_VIQE->Dnts.OnOff);
	if (RandData == 2) CUI_VIQE->Hiseq.OnOff = !(CUI_VIQE->Hiseq.OnOff);
	if (RandData == 3) CUI_VIQE->Dnsp.OnOff = !(CUI_VIQE->Dnsp.OnOff);
	if (RandData == 4) CUI_VIQE->Hpf.OnOff = !(CUI_VIQE->Hpf.OnOff);
	if (RandData == 5) CUI_VIQE->Gamut.OnOff = !(CUI_VIQE->Gamut.OnOff);

	if (RandData == 6) { 
		int i;
		for (i = 0; i < 16; i++)
		{
			ConfigRandData = rand() % 100; 
			CUI_VIQE->Hiseq.Strength[i] = ConfigRandData; 
		}
	}
//	if (RandData == 7) { ConfigRandData = rand() % 7; CUI_VIQE->Gamut.DefaultStrength= (float) ConfigRandData; }

	printf ("\n############ CurState #############\n");
	printf ("Deintl = %d\n", CUI_VIQE->Deintl.OnOff);
	printf ("Dnts = %d\n", CUI_VIQE->Dnts.OnOff);
	printf ("Hiseq = %d\n", CUI_VIQE->Hiseq.OnOff);
	printf ("Dnsp = %d\n", CUI_VIQE->Dnsp.OnOff);
	printf ("Hpf = %d\n", CUI_VIQE->Hpf.OnOff);
	printf ("Gamut = %d\n", CUI_VIQE->Gamut.OnOff);
	printf ("HiseqStrength = %d\n", CUI_VIQE->Hiseq.Strength);
//	printf ("GamutStrength = %f\n", CUI_VIQE->Gamut.DefaultStrength);
	}
	return;
}

void  GamutInterfaceMain (pTCC_CUI_VIQE CUI_VIQE)
{
	int AlgNo;
	int AdvancedMode;
	AlgNo = CUI_VIQE->Gamut.AlgorithmNumber;
	switch (AlgNo)
	{
		case 0 : case 1 : 
				if (AlgNo == 0) AdvancedMode = 0;
				else AdvancedMode = 1;
				GamutLUTGenAlg0 (
					AdvancedMode,
					CUI_VIQE->Gamut.LUVMode,
					CUI_VIQE->Gamut.Alg01.StrDefault,
					CUI_VIQE->Gamut.Alg01.StrYishG, 
					CUI_VIQE->Gamut.Alg01.StrYG, 
					CUI_VIQE->Gamut.Alg01.StrGishY,
					CUI_VIQE->Gamut.Alg01.StrY, 
					CUI_VIQE->Gamut.Alg01.StrYishO, 
					CUI_VIQE->Gamut.Alg01.StrO,
					CUI_VIQE->Gamut.Alg01.StrOPin, 
					CUI_VIQE->Gamut.Alg01.StrPin, 
					CUI_VIQE->Gamut.Alg01.StrPishPin,
					CUI_VIQE->Gamut.Alg01.StrRishO, 
					CUI_VIQE->Gamut.Alg01.StrR, 
					CUI_VIQE->Gamut.Alg01.StrPishR,
					CUI_VIQE->Gamut.Alg01.StrRP, 
					CUI_VIQE->Gamut.Alg01.StrRishP, 
					CUI_VIQE->Gamut.Alg01.StrP,
					CUI_VIQE->Gamut.Alg01.StrBishP, 
					CUI_VIQE->Gamut.Alg01.StrPishB, 
					CUI_VIQE->Gamut.Alg01.StrB,
					CUI_VIQE->Gamut.Alg01.StrGishB, 
					CUI_VIQE->Gamut.Alg01.StrBG, 
					CUI_VIQE->Gamut.Alg01.StrBishG,
					CUI_VIQE->Gamut.Alg01.StrG); 
				CUI_VIQE->Gamut.LUTUpdated = 1;
				break;
		case 2 : case 3 : 
				if (AlgNo == 2) AdvancedMode = 0;
				else AdvancedMode = 1;
				GamutLUTGenAlg2(
					AdvancedMode, 
					CUI_VIQE->Gamut.LUVMode,

					CUI_VIQE->Gamut.Alg23.BrightStrDefault, 
					CUI_VIQE->Gamut.Alg23.BrightStrYishG, 
					CUI_VIQE->Gamut.Alg23.BrightStrYG, 
					CUI_VIQE->Gamut.Alg23.BrightStrGishY,
					CUI_VIQE->Gamut.Alg23.BrightStrY, 
					CUI_VIQE->Gamut.Alg23.BrightStrYishO, 
					CUI_VIQE->Gamut.Alg23.BrightStrO,
					CUI_VIQE->Gamut.Alg23.BrightStrOPin, 
					CUI_VIQE->Gamut.Alg23.BrightStrPin, 
					CUI_VIQE->Gamut.Alg23.BrightStrPishPin,
					CUI_VIQE->Gamut.Alg23.BrightStrRishO, 
					CUI_VIQE->Gamut.Alg23.BrightStrR, 
					CUI_VIQE->Gamut.Alg23.BrightStrPishR,
					CUI_VIQE->Gamut.Alg23.BrightStrRP, 
					CUI_VIQE->Gamut.Alg23.BrightStrRishP, 
					CUI_VIQE->Gamut.Alg23.BrightStrP,
					CUI_VIQE->Gamut.Alg23.BrightStrBishP, 
					CUI_VIQE->Gamut.Alg23.BrightStrPishB, 
					CUI_VIQE->Gamut.Alg23.BrightStrB,
					CUI_VIQE->Gamut.Alg23.BrightStrGishB, 
					CUI_VIQE->Gamut.Alg23.BrightStrBG, 
					CUI_VIQE->Gamut.Alg23.BrightStrBishG,
					CUI_VIQE->Gamut.Alg23.BrightStrG,

					CUI_VIQE->Gamut.Alg23.ContStrDefault, // + 100,
					CUI_VIQE->Gamut.Alg23.ContStrYishG, 
					CUI_VIQE->Gamut.Alg23.ContStrYG, 
					CUI_VIQE->Gamut.Alg23.ContStrGishY,
					CUI_VIQE->Gamut.Alg23.ContStrY, 
					CUI_VIQE->Gamut.Alg23.ContStrYishO, 
					CUI_VIQE->Gamut.Alg23.ContStrO,
					CUI_VIQE->Gamut.Alg23.ContStrOPin, 
					CUI_VIQE->Gamut.Alg23.ContStrPin, 
					CUI_VIQE->Gamut.Alg23.ContStrPishPin,
					CUI_VIQE->Gamut.Alg23.ContStrRishO, 
					CUI_VIQE->Gamut.Alg23.ContStrR, 
					CUI_VIQE->Gamut.Alg23.ContStrPishR,
					CUI_VIQE->Gamut.Alg23.ContStrRP, 
					CUI_VIQE->Gamut.Alg23.ContStrRishP, 
					CUI_VIQE->Gamut.Alg23.ContStrP,
					CUI_VIQE->Gamut.Alg23.ContStrBishP, 
					CUI_VIQE->Gamut.Alg23.ContStrPishB, 
					CUI_VIQE->Gamut.Alg23.ContStrB,
					CUI_VIQE->Gamut.Alg23.ContStrGishB, 
					CUI_VIQE->Gamut.Alg23.ContStrBG, 
					CUI_VIQE->Gamut.Alg23.ContStrBishG,
					CUI_VIQE->Gamut.Alg23.ContStrG,

					CUI_VIQE->Gamut.Alg23.HueStrDefault, 
					CUI_VIQE->Gamut.Alg23.HueStrYishG, 
					CUI_VIQE->Gamut.Alg23.HueStrYG, 
					CUI_VIQE->Gamut.Alg23.HueStrGishY,
					CUI_VIQE->Gamut.Alg23.HueStrY, 
					CUI_VIQE->Gamut.Alg23.HueStrYishO, 
					CUI_VIQE->Gamut.Alg23.HueStrO,
					CUI_VIQE->Gamut.Alg23.HueStrOPin, 
					CUI_VIQE->Gamut.Alg23.HueStrPin, 
					CUI_VIQE->Gamut.Alg23.HueStrPishPin,
					CUI_VIQE->Gamut.Alg23.HueStrRishO, 
					CUI_VIQE->Gamut.Alg23.HueStrR, 
					CUI_VIQE->Gamut.Alg23.HueStrPishR,
					CUI_VIQE->Gamut.Alg23.HueStrRP, 
					CUI_VIQE->Gamut.Alg23.HueStrRishP, 
					CUI_VIQE->Gamut.Alg23.HueStrP,
					CUI_VIQE->Gamut.Alg23.HueStrBishP, 
					CUI_VIQE->Gamut.Alg23.HueStrPishB, 
					CUI_VIQE->Gamut.Alg23.HueStrB,
					CUI_VIQE->Gamut.Alg23.HueStrGishB, 
					CUI_VIQE->Gamut.Alg23.HueStrBG, 
					CUI_VIQE->Gamut.Alg23.HueStrBishG,
					CUI_VIQE->Gamut.Alg23.HueStrG,
					
					CUI_VIQE->Gamut.Alg23.SatStrDefault, // + 100,
					CUI_VIQE->Gamut.Alg23.SatStrYishG, 
					CUI_VIQE->Gamut.Alg23.SatStrYG, 
					CUI_VIQE->Gamut.Alg23.SatStrGishY,
					CUI_VIQE->Gamut.Alg23.SatStrY, 
					CUI_VIQE->Gamut.Alg23.SatStrYishO, 
					CUI_VIQE->Gamut.Alg23.SatStrO,
					CUI_VIQE->Gamut.Alg23.SatStrOPin, 
					CUI_VIQE->Gamut.Alg23.SatStrPin, 
					CUI_VIQE->Gamut.Alg23.SatStrPishPin,
					CUI_VIQE->Gamut.Alg23.SatStrRishO, 
					CUI_VIQE->Gamut.Alg23.SatStrR, 
					CUI_VIQE->Gamut.Alg23.SatStrPishR,
					CUI_VIQE->Gamut.Alg23.SatStrRP, 
					CUI_VIQE->Gamut.Alg23.SatStrRishP, 
					CUI_VIQE->Gamut.Alg23.SatStrP,
					CUI_VIQE->Gamut.Alg23.SatStrBishP, 
					CUI_VIQE->Gamut.Alg23.SatStrPishB, 
					CUI_VIQE->Gamut.Alg23.SatStrB,
					CUI_VIQE->Gamut.Alg23.SatStrGishB, 
					CUI_VIQE->Gamut.Alg23.SatStrBG, 
					CUI_VIQE->Gamut.Alg23.SatStrBishG,
					CUI_VIQE->Gamut.Alg23.SatStrG
						);
//				GamutLUTGenAlg2(CUI_VIQE->Gamut.Bright, CUI_VIQE->Gamut.Cont + 100,
//						CUI_VIQE->Gamut.Hue, CUI_VIQE->Gamut.Sat + 100);
				CUI_VIQE->Gamut.LUTUpdated = 1;
				break;
		case 4 : GamutLUTGenAlg1 (); 
				CUI_VIQE->Gamut.LUTUpdated = 1;
				break;
		default : break;
	}	
	return;
}
#endif //#ifdef VIQE_INCLUDE

