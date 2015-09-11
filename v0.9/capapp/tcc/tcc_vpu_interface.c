#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TCC_VPU_CODEC.h"
#include "sys_memory.h"
#include "test_util.h"
#include "vpu_clk_ctrl.h"
#include "common.h"
#include "libavutil/internal.h"
#include "libavcodec/dsputil.h"
#include "libavcodec/avcodec.h"
#include "libavcodec/mpegvideo.h"
#include "libavcodec/h264.h"
#include "libavcodec/vc1.h"
#include "libavcodec/rv34.h"
#include "libmpdemux/stheader.h"
#include "tcc_vpu_interface.h"

#include "pthread.h"

#define LEVEL_MAX		8
#define PROFILE_MAX		6
#define VCODEC_MAX 10

extern float rel_seek_secs;
extern int abs_seek_pos;

static pthread_mutex_t vpu_running = PTHREAD_MUTEX_INITIALIZER;

static rmff_frame_time_t gsRmff_frame_time;
static int gsRvTRDelta;
static int gsRvP_frame_cnt = 0;
static int gsRvReference_Flag = 1;

unsigned int gs_uiFilePlayMode = 1;
int iFameSearchorSkip = 0; 				


//test
static codec_addr_t bit_work_buf_addr[2] = {0,};
static unsigned int bit_work_buf_size = 0;
static int bit_work_buf_dev = -1;

static codec_addr_t reg_base_addr = 0;
static unsigned int reg_base_size = 0;
static int reg_base_dev = -1;
	
static codec_addr_t bitstream_buf_addr[2] = {0,};
static unsigned int bitstream_buf_size = 0;
static int bitstream_buf_dev =-1;

static codec_addr_t slice_buf_addr = 0;
static unsigned int slice_buf_size = 0;
static codec_addr_t sps_pps_buf_addr = 0;
static unsigned int sps_pps_buf_size = 0;

static codec_addr_t frame_buf_addr[2] = {0,};
static unsigned int frame_buf_size = 0;
static int frame_buf_dev = -1;

static codec_addr_t userdata_buf_addr[2] = {0,};
static unsigned int userdata_buf_size = 0;
static int userdata_buf_dev =-1;


static unsigned char* p_input = NULL;
static int input_size = 0;

static codec_handle_t dec_handle = 0;
static dec_init_t dec_init = {0,};
static dec_initial_info_t dec_initial_info = {0,};
static dec_input_t dec_input = {0,};
static dec_output_t dec_output = {0,};
static dec_buffer_t dec_buffer = { 0 };

static dec_disp_info_ctrl_t dec_disp_info_ctrl = {0, };
static dec_disp_info_t	dec_disp_info[32] = {0,};
static dec_disp_info_input_t	dec_disp_info_input = {0,};

// Variables for Ring Buffer Management 
static codec_addr_t input_packet_buffer_addr = 0; // Input stream packet Buffer.  
static unsigned int one_input_packet_size = 0;

#ifdef FRAME_BUFFER_CLEAR_DELAY
	int delayFrameBuf_prev[FRAME_BUFFER_CLEAR_DELAY] = {0,};
	int delayFrameBuf_delayCnt = 0;
#endif

static int is_first_frame;

int g_dec_time = 0;	g_video_frame = 0; g_audio_frame = 0;
static int skip_b_frame = 0;

int dispIFrameNum=0; 
int skipIFrameCnt=0;


static const char *strProfile[8][6] =
{
	//STD_AVC
	{ "Base Profile", "Main Profile", "Extended Profile", "High Profile", "Reserved Profile", "Reserved Profile" },
	//STD_VC1
	{ "Simple Profile", "Main Profile", "Advance Profile", "Reserved Profile", "Reserved Profile", "Reserved Profile" },
	//STD_MPEG2
	{ "High Profile", "Spatial Scalable Profile", "SNR Scalable Profile", "Main Profile", "Simple Profile", "Reserved Profile" },
	//STD_MPEG4
	{ "Simple Profile", "Advanced Simple Profile", "Advanced Code Efficiency Profile", "Reserved Profile", "Reserved Profile", "Reserved Profile" },
	//STD_H263
	{ " ", " ", " ", " ", " ", " " },
	//STD_DIV3
	{ " ", " ", " ", " ", " ", " " },
	//STD_RV
	{ "Real video Version 8", "Real video Version 9", "Real video Version 10", " ", " ", " " },
	//
	{ " ", " ", " ", " ", " ", " " }
};

static const char *strLevel[8][8] =
{
	//STD_AVC
	{ "Level_1B", "Level_", "Reserved Level", "Reserved Level", "Reserved Level", "Reserved Level", "Reserved Level", "Reserved Level" },
	//STD_VC1
	{ "Level_L0(LOW)", "Level_L1", "Level_L2(MEDIUM)", "Level_L3", "Level_L4(HIGH)", "Reserved Level", "Reserved Level", "Reserved Level" },
	//STD_MPEG2
	{ "High Level", "High 1440 Level", "Main Level", "Low Level", "Reserved Level", "Reserved Level", "Reserved Level", "Reserved Level" },
	//STD_MPEG4
	{ "Level_L0", "Level_L1", "Level_L2", "Level_L3", "Level_L4", "Level_L5", "Level_L6", "Reserved Level" },
	//STD_H263
	{ "", "", "", "", "", "", "", "" },
	//STD_DIV3
	{ "", "", "", "", "", "", "", "" },
	//STD_RV
	{ "", "", "", "", "", "", "", "" },
	//
	{ "", "", "", "", "", "", "", "" }
};

#ifdef VDEC_VPU_INDEX_MUTEX
pthread_mutex_t vpu_index_mutex;

#define	DISPLAY_INDEX_QUEUE_SIZE	64
static int g_disidx_queue[DISPLAY_INDEX_QUEUE_SIZE]; // 0~31
static int g_disidx_queue_read;
static int g_disidx_queue_write;
static int g_disidx_queue_entry = 0;

#define	USE_DISP_INDEX_CHK_QUEUE	1

#ifdef	USE_DISP_INDEX_CHK_QUEUE
#define	DISPLAY_INDEX_CHK_QUEUE_SIZE	(DISPLAY_INDEX_QUEUE_SIZE*4)
static int g_disidx_chk_queue[DISPLAY_INDEX_QUEUE_SIZE*4]; // 0~31
static int g_disidx_chk_queue_read;
static int g_disidx_chk_queue_write;
static int g_disidx_chk_queue_entry = 0;
static int chk_dequeue_vpu_displayindex(void);
#endif

static unsigned g_curr_vc1_skip_index = -1;
static unsigned g_prev_vc1_skip_index = -1;



static int	is_full_vpu_displayindex_queue ()
{
	if ( g_disidx_queue_entry == DISPLAY_INDEX_QUEUE_SIZE ) {
		return (1);
	} else {
		return (0);
	}
}

void enable_skip_b_frame(void)
{
	if (skip_b_frame == 2) return;
	skip_b_frame = 2;
	//av_log(NULL, AV_LOG_INFO,"skip B frame\n");
}

void disable_skip_b_frame(void)
{
	if (skip_b_frame == 0) return;
	skip_b_frame = 0;
	//av_log(NULL, AV_LOG_INFO,"play B frame\n");
}

static int	is_empty_vpu_displayindex_queue ()
{
	if ( g_disidx_queue_entry <= 0 ) {
		return (1);
	} else {
		return (0);
	}
}

static void queue_vpu_displayindex(int index)
{
#if 0
	if(g_disidx_queue_read==g_disidx_queue_write)
		printf("vpu display index queue full!!!!\n");
#endif
	if (is_full_vpu_displayindex_queue() ) {
//	if(g_disidx_queue_entry==DISPLAY_INDEX_QUEUE_SIZE)
		av_log(NULL, AV_LOG_INFO,"vpu display index queue full!!!!\n");
	}
	
	g_disidx_queue[g_disidx_queue_write] = index;
	g_disidx_queue_write++;
	g_disidx_queue_entry++;
	if(DISPLAY_INDEX_QUEUE_SIZE==g_disidx_queue_write)
		g_disidx_queue_write = 0;
}

static int dequeue_vpu_displayindex(void)
{
	int ret = -1;

/*
	if(g_disidx_queue_read==g_disidx_queue_write)
		return ret;
		*/
	if ( is_empty_vpu_displayindex_queue() ) return ret;
//	if ( g_disidx_queue_entry == 0 ) return ret;
	
	ret = g_disidx_queue[g_disidx_queue_read];
	g_disidx_queue_read++;	
	g_disidx_queue_entry--;
	if(DISPLAY_INDEX_QUEUE_SIZE==g_disidx_queue_read)
		g_disidx_queue_read = 0;	

	#ifdef	USE_DISP_INDEX_CHK_QUEUE
	{
		int	nChkIdx;
		nChkIdx = chk_dequeue_vpu_displayindex ();
		if ( nChkIdx & 0x40000000 ) {
			// VC1 Skip Frame
			nChkIdx &= ~0x40000000;
			if ( ret != nChkIdx ) {
				//printf ("%s : %d : disp_queue read mismatch error\n",__func__, __LINE__);
				//while (1);
			} else {
				if ( nChkIdx == g_prev_vc1_skip_index ) {
					return -1;
				} else {
					g_prev_vc1_skip_index = nChkIdx;
					return (g_prev_vc1_skip_index);
				}
			}
		} else {
			if ( ret != nChkIdx ) {
				//printf ("%s : %d : disp_queue read mismatch error\n",__func__, __LINE__);
				//while (1);
			}
		}
	}
	#endif

	return ret;
}

void	display_vpu_index_queue_entry (void)
{
	unsigned int new_idx;
	unsigned int i;

	#ifdef	USE_DISP_INDEX_CHK_QUEUE
	
	av_log(NULL, AV_LOG_INFO," vpu queue entry: window [");
	for ( i = 0 ; i < g_disidx_chk_queue_entry ; i++ ) {
		new_idx = g_disidx_chk_queue_read + i;
		if ( new_idx >= DISPLAY_INDEX_CHK_QUEUE_SIZE ) {
			new_idx -= DISPLAY_INDEX_CHK_QUEUE_SIZE;
		}
		av_log(NULL, AV_LOG_INFO," %02d", g_disidx_chk_queue[new_idx]);
	}
	av_log(NULL, AV_LOG_INFO,"]\n");
	#endif
}

void	check_vpu_index_queue(int index)
{
	unsigned int i;
	unsigned int new_idx;
	unsigned int error;

	#ifdef	USE_DISP_INDEX_CHK_QUEUE
	
	if ( index < 0 ) return;

	error = 0;
	for ( i = 0 ; i < g_disidx_chk_queue_entry ; i++ ) {
		new_idx = g_disidx_chk_queue_read + i;
		if ( new_idx >= DISPLAY_INDEX_CHK_QUEUE_SIZE ) {
			new_idx -= DISPLAY_INDEX_CHK_QUEUE_SIZE;
		}
		if ( index == g_disidx_chk_queue[new_idx] ) {
			error = 1;
		}
	}
	if ( error == 0 ) return;
	
	//printf (" vpu overwriting error : '%d' at window [", index);
	for ( i = 0 ; i < g_disidx_chk_queue_entry ; i++ ) {
		new_idx = g_disidx_chk_queue_read + i;
		if ( new_idx >= DISPLAY_INDEX_CHK_QUEUE_SIZE ) {
			new_idx -= DISPLAY_INDEX_CHK_QUEUE_SIZE;
		}
		//printf (" %02d", g_disidx_chk_queue[new_idx]);
	}
	//printf ("]\n");
	#endif
}

#ifdef	USE_DISP_INDEX_CHK_QUEUE
static int	is_full_vpu_displayindex_chk_queue ()
{
	if ( g_disidx_chk_queue_entry == DISPLAY_INDEX_CHK_QUEUE_SIZE ) {
		av_log(NULL, AV_LOG_INFO,"%s : %d : disp_queue check error\n",__func__, __LINE__);
		return (1);
	} else {
		return (0);
	}
}

static int	is_empty_vpu_displayindex_chk_queue ()
{
	if ( g_disidx_chk_queue_entry <= 0 ) {
		av_log(NULL, AV_LOG_INFO,"%s : %d : disp_queue check error\n",__func__, __LINE__);
		return (1);
	} else {
		return (0);
	}
}

static void chk_queue_vpu_displayindex(int index)
{
	if (is_full_vpu_displayindex_chk_queue() ) {
		av_log(NULL, AV_LOG_INFO,"%s : %d : disp_queue check full error\n",__func__, __LINE__);
	}

//	printf ("chk_queue : %d\n",index);
	g_disidx_chk_queue[g_disidx_chk_queue_write] = index;
	g_disidx_chk_queue_write++;
	g_disidx_chk_queue_entry++;
	if(DISPLAY_INDEX_CHK_QUEUE_SIZE==g_disidx_chk_queue_write)
		g_disidx_chk_queue_write = 0;
}

static int chk_dequeue_vpu_displayindex(void)
{
	int ret = -1;

	if ( is_empty_vpu_displayindex_chk_queue() ) {
		//printf ("%s : %d : disp_queue check error\n",__func__, __LINE__);
		return ret;
	}
	
	ret = g_disidx_chk_queue[g_disidx_chk_queue_read];
//	printf ("chk_dequeue : %d\n",ret);
	g_disidx_chk_queue_read++;	
	g_disidx_chk_queue_entry--;
	if(DISPLAY_INDEX_CHK_QUEUE_SIZE==g_disidx_chk_queue_read)
		g_disidx_chk_queue_read = 0;	

	return ret;
}

#endif

#endif //VDEC_VPU_INDEX_MUTEX

void lock_vpu_dmx(void)
{
	pthread_mutex_lock(&vpu_running);
}
void unlock_vpu_dmx(void)
{
	pthread_mutex_unlock(&vpu_running);
}

int print_pic_type( int iVideoType, int iPicType, int iPictureStructure )
{
	switch ( iVideoType )
	{
	case STD_MPEG2 :
		if( iPicType == PIC_TYPE_I )
			av_log(NULL, AV_LOG_INFO,"I :" );
		else if( iPicType == PIC_TYPE_P )
			av_log(NULL, AV_LOG_INFO,"P :" );
		else if( iPicType == PIC_TYPE_B )
			av_log(NULL, AV_LOG_INFO,"B :" );
		else
			av_log(NULL, AV_LOG_INFO,"D :" ); //D_TYPE
		break;

	case STD_MPEG4 :
		if( iPicType == PIC_TYPE_I )
			av_log(NULL, AV_LOG_INFO,"I :" );
		else if( iPicType == PIC_TYPE_P )
			av_log(NULL, AV_LOG_INFO,"P :" );
		else if( iPicType == PIC_TYPE_B )
			av_log(NULL, AV_LOG_INFO,"B :" );
		else if( iPicType == PIC_TYPE_B_PB ) //B of Packed PB-frame
			av_log(NULL, AV_LOG_INFO,"pB:" ); 
		else
			av_log(NULL, AV_LOG_INFO,"S :" ); //S_TYPE
		break;

	case STD_VC1 :
		if( iPictureStructure == 3) 
		{
			// FIELD_INTERLACED
			if( (iPicType>>3) == PIC_TYPE_I )
				av_log(NULL, AV_LOG_INFO,"TF_I   :" );	//TOP_FIELD = I
			else if( (iPicType>>3) == PIC_TYPE_P )
				av_log(NULL, AV_LOG_INFO,"TF_P   :" );	//TOP_FIELD = P
			else if( (iPicType>>3) == 2 )
				av_log(NULL, AV_LOG_INFO,"TF_BI  :" );	//TOP_FIELD = BI_TYPE
			else if( (iPicType>>3) == 3 )
				av_log(NULL, AV_LOG_INFO,"TF_B   :" );	//TOP_FIELD = B_TYPE
			else if( (iPicType>>3) == 4 )
				av_log(NULL, AV_LOG_INFO,"TF_SKIP:" );	//TOP_FIELD = SKIP_TYPE
			else
				av_log(NULL, AV_LOG_INFO,"TF_FORBIDDEN :" ); //TOP_FIELD = FORBIDDEN

			if( (iPicType&0x7) == PIC_TYPE_I )
				av_log(NULL, AV_LOG_INFO,"BF_I   :" );	//BOTTOM_FIELD = I
			else if( (iPicType&0x7) == PIC_TYPE_P )
				av_log(NULL, AV_LOG_INFO,"BF_P   :" );	//BOTTOM_FIELD = P
			else if( (iPicType&0x7) == 2 )
				av_log(NULL, AV_LOG_INFO,"BF_BI  :" );	//BOTTOM_FIELD = BI_TYPE
			else if( (iPicType&0x7) == 3 )
				av_log(NULL, AV_LOG_INFO,"BF_B   :" );	//BOTTOM_FIELD = B_TYPE
			else if( (iPicType&0x7) == 4 )
				av_log(NULL, AV_LOG_INFO,"BF_SKIP:" );	//BOTTOM_FIELD = SKIP_TYPE
			else
				av_log(NULL, AV_LOG_INFO,"BF_FORBIDDEN :" ); //BOTTOM_FIELD = FORBIDDEN
		}
		else 
		{
			iPicType = iPicType >> 3;
			if( iPicType == PIC_TYPE_I )
				av_log(NULL, AV_LOG_INFO,"I   :" );
			else if( iPicType == PIC_TYPE_P )
				av_log(NULL, AV_LOG_INFO,"P   :" );
			else if( iPicType == 2 )
				av_log(NULL, AV_LOG_INFO,"BI  :" );
			else if( iPicType == 3 )
				av_log(NULL, AV_LOG_INFO,"B   :" );
			else if( iPicType == 4 )
				av_log(NULL, AV_LOG_INFO,"SKIP:" );
			else
				av_log(NULL, AV_LOG_INFO,"FORBIDDEN :" ); //FORBIDDEN

		}
		break;
	default:
		if( iPicType == PIC_TYPE_I )
			av_log(NULL, AV_LOG_INFO,"I :" );
		else if( iPicType == PIC_TYPE_P )
			av_log(NULL, AV_LOG_INFO,"P :" );
		else if( iPicType == PIC_TYPE_B )
			av_log(NULL, AV_LOG_INFO,"B :" );
		else
			av_log(NULL, AV_LOG_INFO,"U :" ); //Unknown
	}
	return iPicType;
}


static int g_Isinterlace=0;
int IsInterlace_VPU()
{
	return g_Isinterlace;
}

void set_g_Isinterlace(int val)
{
	g_Isinterlace=val;
}
static void print_dec_initial_info( dec_init_t* pDecInit, dec_initial_info_t* pInitialInfo )
{
	unsigned int fRateInfoRes = pInitialInfo->m_uiFrameRateRes;
	unsigned int fRateInfoDiv = pInitialInfo->m_uiFrameRateDiv;
	int userDataEnable = 0;
	int profile = 0;
	int level =0;
	g_Isinterlace=0;
	av_log(NULL, AV_LOG_INFO,"---------------VIDEO INITIAL INFO-----------------\n");
	if (pDecInit->m_iBitstreamFormat == STD_MPEG4) {
		av_log(NULL, AV_LOG_INFO,"[VDEC]Data Partition Enable Flag [%1d]\n", pInitialInfo->m_iM4vDataPartitionEnable);
		av_log(NULL, AV_LOG_INFO,"[VDEC]Reversible VLC Enable Flag [%1d]\n", pInitialInfo->m_iM4vReversibleVlcEnable);
		if (pInitialInfo->m_iM4vShortVideoHeader) {			
			av_log(NULL, AV_LOG_INFO,"[VDEC]Short Video Header\n");
			av_log(NULL, AV_LOG_INFO,"[VDEC]AnnexJ Enable Flag [%1d]\n", pInitialInfo->m_iM4vH263AnnexJEnable);
		} else
			av_log(NULL, AV_LOG_INFO,"[VDEC]Not Short Video\n");		
	}

	switch(pDecInit->m_iBitstreamFormat) {
	case STD_MPEG2:
		profile = (pInitialInfo->m_iProfile==0 || pInitialInfo->m_iProfile>5) ? 5 : (pInitialInfo->m_iProfile-1);
		level = pInitialInfo->m_iLevel==4 ? 0 : pInitialInfo->m_iLevel==6 ? 1 : pInitialInfo->m_iLevel==8 ? 2 : pInitialInfo->m_iLevel==10 ? 3 : 4;
		break;
	case STD_MPEG4:
		if (pInitialInfo->m_iLevel & 0x80) 
		{
			// if VOS Header 

			if (pInitialInfo->m_iLevel == 8 && pInitialInfo->m_iProfile == 0) {
				level = 0; profile = 0; // Simple, Level_L0
			} else {
				switch(pInitialInfo->m_iProfile) {
					case 0xB:	profile = 2; break;
					case 0xF:	if( (pInitialInfo->m_iLevel&8) == 0) 
									profile = 1; 
								else
									profile = 5;
								break;
					case 0x0:	profile = 0; break;
					default :	profile = 5; break;
				}
				level = pInitialInfo->m_iLevel;
			}
			
			av_log(NULL, AV_LOG_INFO,"[VDEC]VOS Header:%d, %d\n", profile, level);
		} 
		else 
		{ 
			// Vol Header Only
			level = 7; // reserved level
			switch(pInitialInfo->m_iProfile) {
				case  0x1: profile = 0; break; // simple object
				case  0xC: profile = 2; break; // advanced coding efficiency object
				case 0x11: profile = 1; break; // advanced simple object
				default  : profile = 5; break; // reserved
			}
			av_log(NULL, AV_LOG_INFO,"[VDEC]VOL Header:%d, %d\n", profile, level);
		}

		if( level > 7 )
			level = 0;
		break;
	case STD_VC1:
		profile = pInitialInfo->m_iProfile;
		level = pInitialInfo->m_iLevel;
		break;
	case STD_AVC:
		profile = (pInitialInfo->m_iProfile==66) ? 0 : (pInitialInfo->m_iProfile==77) ? 1 : (pInitialInfo->m_iProfile==88) ? 2 : (pInitialInfo->m_iProfile==100) ? 3 : 4;
		if(profile<3) {
			// BP, MP, EP
			level = (pInitialInfo->m_iLevel==11 && pInitialInfo->m_iAvcConstraintSetFlag[3] == 1) ? 0 /*1b*/ 
				: (pInitialInfo->m_iLevel>=10 && pInitialInfo->m_iLevel <= 51) ? 1 : 2;
		} else {
			// HP
			level = (pInitialInfo->m_iLevel==9) ? 0 : (pInitialInfo->m_iLevel>=10 && pInitialInfo->m_iLevel <= 51) ? 1 : 2;
		}
		
		break;
	case STD_RV:
		profile = pInitialInfo->m_iProfile - 8;
		level = pInitialInfo->m_iLevel;
		break;
	case STD_H263:
		profile = pInitialInfo->m_iProfile;
		level = pInitialInfo->m_iLevel;
		break;
	case STD_DIV3:
		profile = pInitialInfo->m_iProfile;
		level = pInitialInfo->m_iLevel;
		break;
	case STD_SORENSON263:
	case STD_WMV78:
		profile = 0;
		level = 0;
		break;
	default: // STD_MJPG
		;
	}

	if( level >= LEVEL_MAX )
	{
		av_log(NULL, AV_LOG_INFO,"[VDEC]Invalid \"level\" value: %d", level);
		level = 0;
	}
	if( profile >= PROFILE_MAX )
	{
		av_log(NULL, AV_LOG_INFO,"[VDEC]Invalid \"profile\" value: %d", profile);
		profile = 0;
	}
	if( pDecInit->m_iBitstreamFormat >= VCODEC_MAX )
	{
		av_log(NULL, AV_LOG_INFO,"[VDEC]Invalid \"m_iBitstreamFormat\" value: %d", pDecInit->m_iBitstreamFormat);
		pDecInit->m_iBitstreamFormat = 0;
	}

	if (pDecInit->m_iBitstreamFormat != STD_WMV78)	// No Profile and Level information in WMV78
	{
		av_log(NULL, AV_LOG_INFO,"[VDEC]%s\n\r", strProfile[pDecInit->m_iBitstreamFormat][profile]);
		if (pDecInit->m_iBitstreamFormat != STD_RV) { // No level information in Rv.
			if (pDecInit->m_iBitstreamFormat == STD_AVC && level != 0 && level != 2)
				av_log(NULL, AV_LOG_INFO,"[VDEC]%s%.1f\n\r", strLevel[pDecInit->m_iBitstreamFormat][level], (float)pInitialInfo->m_iLevel/10);
			else
				av_log(NULL, AV_LOG_INFO,"[VDEC]%s\n\r", strLevel[pDecInit->m_iBitstreamFormat][level]);
		}
	}
	
	if(pDecInit->m_iBitstreamFormat == STD_AVC) {
		av_log(NULL, AV_LOG_INFO,"[VDEC]frame_mbs_only_flag : %d\n", pInitialInfo->m_iInterlace);
	} else if (pDecInit->m_iBitstreamFormat != STD_RV) {// No interlace information in Rv.
		if (pInitialInfo->m_iInterlace)
		{
			av_log(NULL, AV_LOG_INFO,"[VDEC]%s\n", "Interlaced Sequence");
			g_Isinterlace=1;
		}
		else
			av_log(NULL, AV_LOG_INFO,"[VDEC]%s\n", "Progressive Sequence");
	}

	if (pDecInit->m_iBitstreamFormat == STD_VC1) {
		if (pInitialInfo->m_iVc1Psf)
			av_log(NULL, AV_LOG_INFO,"[VDEC]%s\n", "VC1 - Progressive Segmented Frame");
		else
			av_log(NULL, AV_LOG_INFO,"[VDEC]%s\n", "VC1 - Not Progressive Segmented Frame");
	}

	av_log(NULL, AV_LOG_INFO,"[VDEC]Aspect Ratio [%1d]\n", pInitialInfo->m_iAspectRateInfo);
				
	switch (pDecInit->m_iBitstreamFormat) {
	case STD_AVC:
        	av_log(NULL, AV_LOG_INFO,"[VDEC]H.264 Profile:%d Level:%d FrameMbsOnlyFlag:%d\n",
			pInitialInfo->m_iProfile, pInitialInfo->m_iLevel, pInitialInfo->m_iInterlace);
		
		if(pInitialInfo->m_iAspectRateInfo) {
			int aspect_ratio_idc;
			int sar_width, sar_height;

			if( (pInitialInfo->m_iAspectRateInfo>>16)==0 ) {
				aspect_ratio_idc = (pInitialInfo->m_iAspectRateInfo & 0xFF);
				av_log(NULL, AV_LOG_INFO,"[VDEC]aspect_ratio_idc :%d\n", aspect_ratio_idc);
			}
			else {
				sar_width  = (pInitialInfo->m_iAspectRateInfo >> 16);
				sar_height  = (pInitialInfo->m_iAspectRateInfo & 0xFFFF);
				av_log(NULL, AV_LOG_INFO,"[VDEC]sar_width  : %d\nsar_height : %d", sar_width, sar_height);				
			}
		} else {
			av_log(NULL, AV_LOG_INFO,"[VDEC]Aspect Ratio is not present\n");
		}

		break;
	case STD_VC1:
		if(pInitialInfo->m_iProfile == 0)
			av_log(NULL, AV_LOG_INFO,"[VDEC]VC1 Profile: Simple\n");
		else if(pInitialInfo->m_iProfile == 1)
			av_log(NULL, AV_LOG_INFO,"[VDEC]VC1 Profile: Main\n");
		else if(pInitialInfo->m_iProfile == 2)
			av_log(NULL, AV_LOG_INFO,"[VDEC]VC1 Profile: Advanced\n");
		
		av_log(NULL, AV_LOG_INFO,"[VDEC]Level: %d Interlace: %d PSF: %d\n", 
			pInitialInfo->m_iLevel, pInitialInfo->m_iInterlace, pInitialInfo->m_iVc1Psf);

		if(pInitialInfo->m_iAspectRateInfo)
			av_log(NULL, AV_LOG_INFO,"[VDEC]Aspect Ratio [X, Y]:[%3d, %3d]\n", (pInitialInfo->m_iAspectRateInfo>>8)&0xff,
					(pInitialInfo->m_iAspectRateInfo)&0xff);
		else
			av_log(NULL, AV_LOG_INFO,"[VDEC]Aspect Ratio is not present\n");

		break;
	case STD_MPEG2:
        	av_log(NULL, AV_LOG_INFO,"[VDEC]Mpeg2 Profile:%d Level:%d Progressive Sequence Flag:%d\n",
			pInitialInfo->m_iProfile, pInitialInfo->m_iLevel, pInitialInfo->m_iInterlace);
		// Profile: 3'b101: Simple, 3'b100: Main, 3'b011: SNR Scalable, 
		// 3'b10: Spatially Scalable, 3'b001: High
		// Level: 4'b1010: Low, 4'b1000: Main, 4'b0110: High 1440, 4'b0100: High
		if(pInitialInfo->m_iAspectRateInfo)
			av_log(NULL, AV_LOG_INFO,"[VDEC]Aspect Ratio Table index :%d\n", pInitialInfo->m_iAspectRateInfo);
		else
			av_log(NULL, AV_LOG_INFO,"[VDEC]Aspect Ratio is not present\n");
        	break;

	case STD_MPEG4:
        	av_log(NULL, AV_LOG_INFO,"[VDEC]Mpeg4 Profile: %d Level: %d Interlaced: %d\n",
			pInitialInfo->m_iProfile, pInitialInfo->m_iLevel, pInitialInfo->m_iInterlace);
		// Profile: 8'b00000000: SP, 8'b00010001: ASP
		// Level: 4'b0000: L0, 4'b0001: L1, 4'b0010: L2, 4'b0011: L3, ...
		// SP: 1/2/3/4a/5/6, ASP: 0/1/2/3/4/5
		
		if(pInitialInfo->m_iAspectRateInfo)
			av_log(NULL, AV_LOG_INFO,"[VDEC]Aspect Ratio Table index :%d\n", pInitialInfo->m_iAspectRateInfo);
		else
			av_log(NULL, AV_LOG_INFO,"[VDEC]Aspect Ratio is not present\n");
		break;

	case STD_RV:
        	av_log(NULL, AV_LOG_INFO,"[VDEC]Real Video Version %d\n",	pInitialInfo->m_iProfile);
        	break;

#ifdef INCLUDE_SORENSON263_DEC
	case STD_SORENSON263:
			av_log(NULL, AV_LOG_INFO,"[VDEC]Sorenson's H.263 \n");
        	break;
#endif
   	}

	if (pDecInit->m_iBitstreamFormat == STD_RV) // RV has no user data
		userDataEnable = 0;


	av_log(NULL, AV_LOG_INFO,"[VDEC]Dec InitialInfo =>\n m_iPicWidth: %u\n m_iPicHeight: %u\n frameRate: %.2f\n frRes: %u\n frDiv: %u\n",
		pInitialInfo->m_iPicWidth, pInitialInfo->m_iPicHeight, (double)fRateInfoRes/fRateInfoDiv, fRateInfoRes, fRateInfoDiv);
	//av_log(NULL,AV_LOG_INFO,"pInitialInfo->m_iInterlace=0x%X!!!!\n",pInitialInfo->m_iInterlace);
	//if(pInitialInfo->m_iInterlace) g_Isinterlace=1;
#ifdef INCLUDE_SORENSON263_DEC
	if (pDecInit->m_iBitstreamFormat == STD_SORENSON263) {		
		int disp_width = pInitialInfo->m_iPicWidth;
		int disp_height = pInitialInfo->m_iPicHeight;
		int crop_left = pInitialInfo->m_iAvcPicCrop.m_iCropLeft;
		int crop_right = pInitialInfo->m_iAvcPicCrop.m_iCropRight;
		int crop_top = pInitialInfo->m_iAvcPicCrop.m_iCropTop;
		int crop_bottom = pInitialInfo->m_iAvcPicCrop.m_iCropBottom;

		if( crop_left | crop_right | crop_top | crop_bottom ){
			disp_width = disp_width - ( crop_left + crop_right );
			disp_height = disp_height - ( crop_top + crop_bottom );

			av_log(NULL, AV_LOG_INFO,"[VDEC]Dec InitialInfo =>\n Display_PicWidth: %u\n Display_PicHeight: %u\n",
				disp_width, disp_height);
		}
	}
#endif

	
	av_log(NULL, AV_LOG_INFO,"---------------------------------------------------\n");
}


static void
disp_pic_info( int Opcode, void* pParam1, void *pParam2, void *pParam3)
{
	int i;
	dec_disp_info_ctrl_t  *pInfoCtrl = (dec_disp_info_ctrl_t*)pParam1;
	dec_disp_info_t		  *pInfo = (dec_disp_info_t *)pParam2;
	dec_disp_info_input_t *pInfoInput = (dec_disp_info_input_t*)pParam3;

	switch( Opcode )
	{
	case VDEC_DISP_INFO_INIT:	//init.
		pInfoCtrl->m_iStdType = pInfoInput->m_iStdType;			
		pInfoCtrl->m_iTimeStampType = pInfoInput->m_iTimeStampType;
	case VDEC_DISP_INFO_RESET: //reset
		for( i=0 ; i<32 ; i++ )
		{
			pInfoCtrl->m_iRegIdxPTS[i] = -1;	//unused
			pInfoCtrl->m_pRegInfoPTS[i] = (void*)&pInfo[i];
		}
		pInfoCtrl->m_iUsedIdxPTS = 0;

		if( pInfoCtrl->m_iTimeStampType == VDEC_DISP_INFO_DTS_MODE )	//Decode Timestamp (Decode order)
		{
			pInfoCtrl->m_iDecodeIdxDTS = 0;
			pInfoCtrl->m_iDispIdxDTS = 0;
			for( i=0 ; i<32 ; i++ )
			{
				pInfoCtrl->m_iDTS[i] = 0;
			}
		}

		sys_memset(&gsRmff_frame_time, 0, sizeof(rmff_frame_time_t));
		gsRvReference_Flag = 1;
		gsRvP_frame_cnt = 0;
		break;
	case VDEC_DISP_INFO_UPDATE: //update
		{
			int iDecodedIdx;
			int usedIdx, startIdx, regIdx;
			dec_disp_info_t * pdec_disp_info;

			iDecodedIdx = pInfoInput->m_iFrameIdx;

			//Presentation Timestamp (Display order)
			{
				//sort
				usedIdx=0;
				startIdx = -1;
				for( i=0 ; i<32 ; i++ )
				{
					if( pInfoCtrl->m_iRegIdxPTS[i] > -1 )
					{
						if( startIdx == -1 )
						{
							startIdx = i;
						}
						usedIdx++;
					}
				}

				if( usedIdx > 0 )
				{
					regIdx = 0;
					for( i=startIdx ; i<32 ; i++ )
					{
						if( pInfoCtrl->m_iRegIdxPTS[i] > -1 )
						{
							if( i != regIdx )
							{
								void * pswap;
								int iswap;

								iswap = pInfoCtrl->m_iRegIdxPTS[regIdx];
								pswap = pInfoCtrl->m_pRegInfoPTS[regIdx];
								
								pInfoCtrl->m_iRegIdxPTS[regIdx] = pInfoCtrl->m_iRegIdxPTS[i];
								pInfoCtrl->m_pRegInfoPTS[regIdx] = pInfoCtrl->m_pRegInfoPTS[i];

								pInfoCtrl->m_iRegIdxPTS[i] = iswap;
								pInfoCtrl->m_pRegInfoPTS[i] = pswap;
							}
							regIdx++;
							if( regIdx == usedIdx )
								break;
						}
					}
				}

				//save the side info.
				pInfoCtrl->m_iRegIdxPTS[usedIdx] = iDecodedIdx;
				pdec_disp_info = (dec_disp_info_t*)pInfoCtrl->m_pRegInfoPTS[usedIdx];

				pdec_disp_info->m_iTimeStamp = pInfo->m_iTimeStamp;
				pdec_disp_info->m_iFrameType = pInfo->m_iFrameType;
				pdec_disp_info->m_iPicStructure = pInfo->m_iPicStructure;
				pdec_disp_info->m_iRvTimeStamp = pInfo->m_iRvTimeStamp;
				pdec_disp_info->m_iM2vFieldSequence = pInfo->m_iM2vFieldSequence;
				pdec_disp_info->m_iFrameSize = pInfo->m_iFrameSize;
				
				if( pInfoCtrl->m_iStdType  == STD_RV )
				{
					int curTimestamp, rvTimestamp, rvFrameType;

					curTimestamp = pInfo->m_iTimeStamp;
					rvTimestamp = pInfo->m_iRvTimeStamp;
					rvFrameType = pInfo->m_iFrameType;
								
					if(gsRvReference_Flag)
					{
						gsRvReference_Flag = 0;
						gsRmff_frame_time.ref_frame.Current_time_stamp = curTimestamp;
						gsRmff_frame_time.ref_frame.Previous_TR = rvTimestamp;
						gsRmff_frame_time.frame_P2.Current_time_stamp = curTimestamp;
						gsRmff_frame_time.frame_P2.Current_TR = rvTimestamp;
					}
					else
					{
						gsRvTRDelta = rvTimestamp - gsRmff_frame_time.ref_frame.Current_TR;
						if(gsRvTRDelta < 0)
						{
							gsRvTRDelta += 8192;
						}

						if(rvFrameType == 2) //B-frame
						{
							curTimestamp = gsRmff_frame_time.ref_frame.Current_time_stamp + gsRvTRDelta;
						}
						else
						{
							gsRvP_frame_cnt++;
						}
					}

					if( gsRvP_frame_cnt == 1)
					{
						gsRmff_frame_time.frame_P1.Current_TR = rvTimestamp;
						gsRmff_frame_time.frame_P1.Current_time_stamp = curTimestamp;

						gsRmff_frame_time.ref_frame.Current_time_stamp = gsRmff_frame_time.frame_P2.Current_time_stamp;
						gsRmff_frame_time.ref_frame.Current_TR = gsRmff_frame_time.frame_P2.Current_TR;
					}
					else if( gsRvP_frame_cnt == 2)
					{
						gsRvP_frame_cnt = 0;
						gsRmff_frame_time.frame_P2.Current_TR = rvTimestamp;
						gsRmff_frame_time.frame_P2.Current_time_stamp = curTimestamp;

						gsRmff_frame_time.ref_frame.Current_time_stamp = gsRmff_frame_time.frame_P1.Current_time_stamp;
						gsRmff_frame_time.ref_frame.Current_TR = gsRmff_frame_time.frame_P1.Current_TR;
					}

					pdec_disp_info->m_iRvTimeStamp = curTimestamp;
				}

				pInfoCtrl->m_iUsedIdxPTS = usedIdx + 1;
				if( pInfoCtrl->m_iUsedIdxPTS > 31 )
				{
					av_log(NULL, AV_LOG_INFO,"[CDK_CORE] disp_pic_info index failed\n" );
					for( i=0 ; i<32 ; i++ )
					{
						pInfoCtrl->m_iRegIdxPTS[i] = -1;
					}
				}
			}

			if( pInfoCtrl->m_iTimeStampType == VDEC_DISP_INFO_DTS_MODE )	//Decode Timestamp (Decode order)
			{
				if( iDecodedIdx >= 0 )
				{
					pInfoCtrl->m_iDTS[pInfoCtrl->m_iDecodeIdxDTS] = pInfo->m_iTimeStamp;
					pInfoCtrl->m_iDecodeIdxDTS = ( pInfoCtrl->m_iDecodeIdxDTS + 1 ) & 31;
				}
			}
		}
		break;
	case VDEC_DISP_INFO_GET:	//display
		{
			dec_disp_info_t **pInfo = (dec_disp_info_t **)pParam2;
			int dispOutIdx = pInfoInput->m_iFrameIdx;

			//Presentation Timestamp (Display order)
			{
				for( i=0; i<pInfoCtrl->m_iUsedIdxPTS ; i++ )
				{
					if( dispOutIdx == pInfoCtrl->m_iRegIdxPTS[i] )
					{
						*pInfo = (dec_disp_info_t*)pInfoCtrl->m_pRegInfoPTS[i];
						pInfoCtrl->m_iRegIdxPTS[i] = -1; //unused
						pInfoCtrl->m_iUsedIdxPTS--;
						break;
					}
				}
			}

			if( pInfoCtrl->m_iTimeStampType == VDEC_DISP_INFO_DTS_MODE )	//Decode Timestamp (Decode order)
			{
				(*pInfo)->m_iTimeStamp =
				(*pInfo)->m_iRvTimeStamp = pInfoCtrl->m_iDTS[pInfoCtrl->m_iDispIdxDTS];
				pInfoCtrl->m_iDispIdxDTS = ( pInfoCtrl->m_iDispIdxDTS + 1 ) & 31;
			}
		}
		break;
	}

	return;
}

// return the VP of bitstream buffer
long tcc_vpu_get_buffer_va(int format)
{
	long va_addr = 0;
	
	tcc_vpu_codec_init(format, 0, 1920, 1080);
	va_addr = bitstream_buf_addr[VA];
	av_log(NULL, AV_LOG_INFO,"Get VPU VA %x\n",va_addr);
		
	return va_addr;
}

int tcc_vpu_codec_init(int format,int m_iDeblkEnable,int width,int height)
{
	int ret=0,i;
	
	// yes, this function was invoked when demux a TS AVC file, no need to run it one more timr
	if (bitstream_buf_addr[PA] != 0) return 0;
	g_dec_time = 0;
	g_video_frame = 0;
	g_audio_frame = 0;

#if defined(VPU_CLK_CONTROL)
	vpu_clock_init();
#endif
	av_log(NULL, AV_LOG_INFO,"-------------------DECODER INIT-------------------format %d\n",format);

	sys_init_physical_size();

	gs_uiFilePlayMode=1;
	dec_init.m_iFilePlayEnable = gs_uiFilePlayMode; // File Play Mode 1 or Streaming Mode 0 (ring buffer)
	
	//*/

	if ( !dec_init.m_iFilePlayEnable ) // Ring Buffer
	{
		one_input_packet_size = 300*1024;
		input_packet_buffer_addr = (codec_addr_t)malloc(one_input_packet_size); // 
	}


	//------------------------------------------------------------
	//! [x] reg base for VPU 
	//------------------------------------------------------------
   	reg_base_size = 8 * 1024 ;  
	av_log(NULL, AV_LOG_INFO,"reg_base_addr[PA] = 0x%x, 0x%x \n", (codec_addr_t)0xF0700000, reg_base_size );
	reg_base_addr = (codec_addr_t)sys_malloc_virtual_addr( &reg_base_dev, 0xF0700000, reg_base_size );
	gRegBaseAddr = reg_base_addr;
	if( reg_base_addr == 0 ) 
	{
		av_log(NULL, AV_LOG_INFO,"reg_base_addr malloc() failed \n");
		ret=-1;
		return ret;
	}
	av_log(NULL, AV_LOG_INFO,"reg_base_addr[VA] = 0x%x, 0x%x \n", (codec_addr_t)reg_base_addr, reg_base_size );


	//------------------------------------------------------------
	//! [x] bitstream buffer for each VPU decoder
	//------------------------------------------------------------
	
	if (dec_init.m_iFilePlayEnable == 1)
		bitstream_buf_size = LARGE_STREAM_BUF_SIZE;
	else
		bitstream_buf_size = 3*1024*1024; // 3MB RING BUFFER SIZE

	bitstream_buf_size = ALIGNED_BUFF( bitstream_buf_size, 4*1024 );
	bitstream_buf_addr[PA] = (codec_addr_t)sys_malloc_physical_addr( bitstream_buf_size );
	if( bitstream_buf_addr[PA] == 0 ) 
	{
		av_log(NULL, AV_LOG_INFO,"bitstream_buf_addr[PA] malloc() failed \n");
		ret=-1;
		return ret;
	}
	av_log(NULL, AV_LOG_INFO,"bitstream_buf_addr[PA] = 0x%x, 0x%x \n", (codec_addr_t)bitstream_buf_addr[PA], bitstream_buf_size );
	bitstream_buf_addr[VA] = (codec_addr_t)sys_malloc_virtual_addr( &bitstream_buf_dev, bitstream_buf_addr[PA], bitstream_buf_size );
	if( bitstream_buf_addr[VA] == 0 ) 
	{
		av_log(NULL, AV_LOG_INFO,"bitstream_buf_addr[VA] malloc() failed \n");
		ret=-1;
		return ret;
	}
	sys_memset( (void*)bitstream_buf_addr[VA], 0 , bitstream_buf_size);
	av_log(NULL, AV_LOG_INFO,"bitstream_buf_addr[VA] = 0x%x, 0x%x \n", (codec_addr_t)bitstream_buf_addr[VA], bitstream_buf_size );
	p_input = (unsigned char*)bitstream_buf_addr[VA];



	//------------------------------------------------------------
	//! [x] code buffer, work buffer and parameter buffer for VPU 
	//------------------------------------------------------------
	bit_work_buf_size = WORK_CODE_PARA_BUF_SIZE;  
	bit_work_buf_size = ALIGNED_BUFF(bit_work_buf_size, 4*1024); 
	bit_work_buf_addr[PA] = (codec_addr_t)sys_malloc_physical_addr( bit_work_buf_size );
	if( bit_work_buf_addr[PA] == 0 ) 
	{
		av_log(NULL, AV_LOG_INFO,"bit_work_buf_addr[PA] malloc() failed \n");
		ret=-1;
		return ret;
	}
	av_log(NULL, AV_LOG_INFO,"bit_work_buf_addr[PA] = 0x%x, 0x%x \n", (codec_addr_t)bit_work_buf_addr[PA], bit_work_buf_size );
	bit_work_buf_addr[VA] = (codec_addr_t)sys_malloc_virtual_addr( &bit_work_buf_dev, bit_work_buf_addr[PA], bit_work_buf_size );
	if( bit_work_buf_addr[VA] == 0 ) 
	{
		av_log(NULL, AV_LOG_INFO,"bit_work_buf_addr[VA] malloc() failed \n");
		ret=-1;
		return ret;
	}
	av_log(NULL, AV_LOG_INFO,"bit_work_buf_addr[VA] = 0x%x, 0x%x \n", (codec_addr_t)bit_work_buf_addr[VA], bit_work_buf_size );
	

	//------------------------------------------------------------
	//! [x] UserData buffer for each VPU decoder
	//------------------------------------------------------------

	userdata_buf_size = 50 * 1024;
	userdata_buf_size = ALIGNED_BUFF( userdata_buf_size, 4*1024 );
	userdata_buf_addr[PA] = (codec_addr_t)sys_malloc_physical_addr( userdata_buf_size );
	if( userdata_buf_addr[PA] == 0 ) 
	{
		av_log(NULL, AV_LOG_INFO,"userdata_buf_addr[PA] malloc() failed \n");
		ret=-1;
		return ret;
	}
	av_log(NULL, AV_LOG_INFO,"userdata_buf_addr[PA] = 0x%x, 0x%x \n", (codec_addr_t)userdata_buf_addr[PA], userdata_buf_size );
	userdata_buf_addr[VA] = (codec_addr_t)sys_malloc_virtual_addr( &userdata_buf_dev, userdata_buf_addr[PA], userdata_buf_size );
	if( userdata_buf_addr[VA] == 0 ) 
	{
		av_log(NULL, AV_LOG_INFO,"userdata_buf_addr[VA] malloc() failed \n");
		ret=-1;
		return ret;
	}
	sys_memset( (void*)userdata_buf_addr[VA], 0 , userdata_buf_size);
	av_log(NULL, AV_LOG_INFO,"userdata_buf_addr[VA] = 0x%x, 0x%x \n", (codec_addr_t)userdata_buf_addr[VA], userdata_buf_size );

	//! Set Decoder Init
	dec_init.m_iBitstreamFormat = format;
	dec_init.m_RegBaseVirtualAddr = reg_base_addr;
	dec_init.m_BitWorkAddr[PA] = bit_work_buf_addr[PA];
	dec_init.m_BitWorkAddr[VA] = bit_work_buf_addr[VA];
	dec_init.m_BitstreamBufAddr[PA] = bitstream_buf_addr[PA];
	dec_init.m_BitstreamBufAddr[VA] = bitstream_buf_addr[VA];
	dec_init.m_iBitstreamBufSize = bitstream_buf_size;
	dec_init.m_iPicWidth = width;	// resolution info. from demuxer or etc (0: unknown)
	dec_init.m_iPicHeight = height;	// resolution info. from demuxer or etc (0: unknown)
	dec_init.m_bEnableUserData = 1;



	//------------------------------------------------------------
	//! [x] PS(SPS/PPS) buffer for each VPU decoder
	//------------------------------------------------------------
	if( dec_init.m_iBitstreamFormat == STD_AVC )
	{
		sps_pps_buf_size = PS_SAVE_SIZE;
		sps_pps_buf_size = ALIGNED_BUFF( sps_pps_buf_size, 4*1024 );
		sps_pps_buf_addr = (codec_addr_t)sys_malloc_physical_addr( sps_pps_buf_size );
		if( sps_pps_buf_addr == 0 ) 
		{
			av_log(NULL, AV_LOG_INFO,"sps_pps_buf_addr malloc() failed \n");
			ret=-1;
			return ret;
		}
		av_log(NULL, AV_LOG_INFO,"sps_pps_buf_addr = 0x%x, 0x%x \n", (codec_addr_t)sps_pps_buf_addr, sps_pps_buf_size );

		dec_init.m_pSpsPpsSaveBuffer = (unsigned char*)sps_pps_buf_addr;
		dec_init.m_iSpsPpsSaveBufferSize = sps_pps_buf_size;
	}
	else
	{
		sps_pps_buf_addr = 0;
		slice_buf_addr = 0;
	
	}
	dec_init.m_uiDecOptFlags = 0;
/*
	if( dec_init.m_iBitstreamFormat == STD_MPEG4 )
	{
			if( m_iDeblkEnable)
				dec_init.m_uiDecOptFlags |= M4V_DEBLK_ENABLE;
	}
	if( dec_init.m_uiDecOptFlags )
	{
			if( dec_init.m_uiDecOptFlags & 0x2 )
			{
				dec_init.m_uiDecOptFlags |= M4V_GMC_FRAME_SKIP;
			}
			if( dec_init.m_uiDecOptFlags >> 16 )
			{
				dec_init.m_uiDecOptFlags |= ( dec_init.m_uiDecOptFlags & 0xFFFF0000 );
			}
	}
*/	
	if( dec_init.m_iBitstreamFormat == STD_MPEG4 )
	{
		if(m_iDeblkEnable==1)
			dec_init.m_uiDecOptFlags |= M4V_DEBLK_ENABLE;
	}
	dec_init.m_uiDecOptFlags |= M4V_GMC_FRAME_SKIP;

	dec_init.m_bEnableVideoCache = 1;
	dec_init.m_bCbCrInterleaveMode =1;//1;
	dec_init.m_iMaxResolution = RESOLUTION_1080_HD;

	if( dec_init.m_bEnableVideoCache )
		av_log(NULL, AV_LOG_INFO,"VideoCache ON\n" );
	else
		av_log(NULL, AV_LOG_INFO,"VideoCache OFF\n" );

	if( dec_init.m_bCbCrInterleaveMode )
            av_log(NULL, AV_LOG_INFO,"InterleaveMode ON\n" );
	else
		av_log(NULL, AV_LOG_INFO,"InterleaveMode OFF: Progressive Mode\n" );

	if( dec_init.m_uiDecOptFlags&M4V_DEBLK_ENABLE )
		av_log(NULL, AV_LOG_INFO,"[VDEC] MPEG-4 Deblocking ON\n" );
//	if(dec_init.m_uiDecOptFlags&M4V_GMC_FRAME_SKIP )
//		av_log(NULL, AV_LOG_INFO,"[VDEC] MPEG-4 GMC Frame Skip\n" );

	dec_init.m_Memcpy	 = (void* (*) ( void*, const void*, unsigned int ))sys_memcpy;
	dec_init.m_Memset	 = (void  (*) ( void*, int, unsigned int ))sys_memset;
	dec_init.m_Interrupt = (int  (*) ( void ))sys_interrupt;

	dec_disp_info_input.m_iTimeStampType = VDEC_DISP_INFO_PTS_MODE;
	dec_disp_info_input.m_iStdType = dec_init.m_iBitstreamFormat;
	disp_pic_info( VDEC_DISP_INFO_INIT, (void*)&dec_disp_info_ctrl, (void*)dec_disp_info, (void*)&dec_disp_info_input);

	ret = TCC_VPU_DEC( VPU_DEC_INIT, &dec_handle, &dec_init, NULL );
	if( ret != RETCODE_SUCCESS )
	{
		av_log(NULL, AV_LOG_INFO,"VPU_DEC_INIT failed Error code is 0x%x \n", ret );
		ret=-1;
		return ret;
	}
	is_first_frame=1;

#ifdef VDEC_VPU_INDEX_MUTEX
	for(i=0;i<DISPLAY_INDEX_QUEUE_SIZE;i++)
	{
		g_disidx_queue[i] = 0;
	}
	g_disidx_queue_read = 0;
	g_disidx_queue_write = 0;
	g_disidx_queue_entry = 0;
	pthread_mutex_init(&vpu_index_mutex, NULL);		
#ifdef	USE_DISP_INDEX_CHK_QUEUE
	for(i=0;i<DISPLAY_INDEX_CHK_QUEUE_SIZE;i++)
	{
		g_disidx_chk_queue[i] = 0;
	}
	g_disidx_chk_queue_read = 0;
	g_disidx_chk_queue_write = 0;
	g_disidx_chk_queue_entry = 0;
#endif

	g_curr_vc1_skip_index= -1;
	g_prev_vc1_skip_index= -1;
#endif	
	dispIFrameNum=0; 
	skipIFrameCnt=0;
	av_log(NULL, AV_LOG_INFO,"TCC_VPU_DEC VPU_DEC_INIT OK!\n");
	return ret;
}

/********************************************
	MPEG2
********************************************/
int  Vpu_FirstMpeg2frame_with_seqheader(AVCodecContext *avctx,AVPacket *avpkt)
{
  	const uint8_t *buf = avpkt->data;
	int ret,buf_size = avpkt->size;

	tcc_vpu_decode_header(buf_size,buf);
	ret=tcc_vpu_decode_Frame(buf_size,buf);
	return ret;
}

int Vpu_Decode_Mpeg2frame(AVCodecContext *avctx,AVPacket *avpkt)
{
  	const uint8_t *buf = avpkt->data;
	int buf_size = avpkt->size;
	int ret;
	
	ret=tcc_vpu_decode_Frame(buf_size,buf);
	return ret;
}

/********************************************
	End of MPEG2
********************************************/

/********************************************
	DIV3
********************************************/
int Vpu_FirstDiv3frame_with_seqheader(AVCodecContext *avctx,AVPacket *avpkt)
{
  	const uint8_t *buf = avpkt->data;
	int ret,buf_size = avpkt->size;

	tcc_vpu_decode_header(buf_size,buf);
	ret=tcc_vpu_decode_Frame(buf_size,buf);
	return ret;
}

int Vpu_Decode_Div3frame(AVCodecContext *avctx,AVPacket *avpkt)
{
  	const uint8_t *buf = avpkt->data;
	int buf_size = avpkt->size;
	int ret;
	
	ret=tcc_vpu_decode_Frame(buf_size,buf);
	return ret;
}

/********************************************
	End of DIV3
********************************************/

/********************************************
	MPEG4
********************************************/
int Vpu_Firstmpeg4frame_with_seqheader(AVCodecContext *avctx,AVPacket *avpkt)
{
  	const uint8_t *buf = avpkt->data;
	int ret,buf_size = avpkt->size;

	tcc_vpu_decode_header(buf_size,buf);
	ret=tcc_vpu_decode_Frame(buf_size,buf);
	return ret;
}

int Vpu_Decode_Mpeg4frame(AVCodecContext *avctx,AVPacket *avpkt)
{
  	const uint8_t *buf = avpkt->data;
	int buf_size = avpkt->size;
	int ret;
	
	ret=tcc_vpu_decode_Frame(buf_size,buf);
	return ret;
}

/********************************************
	End of MPEG4
********************************************/

/********************************************
	AVC
********************************************/
/************* for Telechips decoder ***************/
static char *gpDmxNal;
static int gDmxNalLength;
static int gDmxNalLengthSize;
static char *gpExtBuf;
/***************************************************/

static int giNalLengthSize;
#define BE_16(x) ((((unsigned char*)(x))[0] << 8) | ((unsigned char*)(x))[1])
static int avcc_spspps_to_annexb_spspps( unsigned char* pExtra, int iExtraLen, unsigned char* pDest, int* piDestLen )
{
	char startcode[8] = {0,0,0,1,0,0,0,0};
	unsigned int buf_index = 0;
	unsigned char* p_buf = pDest;

	unsigned char* p = (unsigned char*)pExtra;
	unsigned char* p_extra = (unsigned char*)pExtra;
	unsigned int len = iExtraLen;
	int cnt, i, nalsize, nal_length_size;

	int first_offset = 0;

	if( len < 7 ) 
	{
		av_log(NULL, AV_LOG_INFO,"[CDMX] avcC too short length is 0x%x \n", len );
		return -1;
	}
	if( *p != 1 ) 
	{
		av_log(NULL, AV_LOG_INFO,"[CDMX] Unknown avcC version %d\n", *p);
		return -1;
	}
	/* sps and pps in the avcC always have length coded with 2 bytes,
	so put a fake nal_length_size = 2 while parsing them */
	nal_length_size = 2;
	// Decode sps from avcC
	cnt = *(p+5) & 0x1f; // Number of sps

	p += 6;
	for( i = 0; i < cnt; i++ ) 
	{
		nalsize = BE_16(p);

		p_buf[buf_index+0] = 0; p_buf[buf_index+1] = 0; 
		p_buf[buf_index+2] = 0; p_buf[buf_index+3] = 1;
		memcpy( &p_buf[buf_index+4], p+2, nalsize );
		buf_index += ( nalsize + 4 );

		p += (nalsize+2);
	}
	// Decode pps from avcC
	cnt = *(p++); // Number of pps

	for( i = 0; i < cnt; i++ ) 
	{
		nalsize = BE_16(p);

		p_buf[buf_index+0] = 0; p_buf[buf_index+1] = 0; 
		p_buf[buf_index+2] = 0; p_buf[buf_index+3] = 1;
		memcpy( &p_buf[buf_index+4], p+2, nalsize );
		buf_index += ( nalsize + 4 );

		p += (nalsize+2);
	}

#ifndef ATTACH_SEQ_HEADER_FOR_VPU_DEC
	p_buf[buf_index+0] = 0; p_buf[buf_index+1] = 0; 
	p_buf[buf_index+2] = 0; p_buf[buf_index+3] = 1;
	buf_index += 4;
#endif
	// Now store right nal length size, that will be use to parse all other nals
	nal_length_size = ((*(((char*)(p_extra))+4))&0x03)+1;
	*piDestLen = buf_index;

	return nal_length_size;
}

static int avcc_to_annexb( unsigned char* pSour, int* piSourLen, unsigned char** ppDmxNal, int* piMaxNalLen, int iNalLengthSize )
{
	char startcode[8] = {0,0,0,1,0,0,0,0};

	int i, buf_index = 0;
	unsigned char* p_buf = pSour;
	int buf_size = *piSourLen;
	int dest_index = 0;

//	av_log(NULL, AV_LOG_INFO,"iNalLengthSize = %d \n", iNalLengthSize );
	if( iNalLengthSize == 4 )
	{
		for(;;)
		{
			int nalsize = 0;    
			if( buf_index >= buf_size ) 
				break;

			for( i = 0; i < iNalLengthSize; i++ )
				nalsize = (nalsize<<8) | p_buf[buf_index++];
			if( (nalsize <= 0) || (nalsize > (buf_size-4)) )
				return -11;

			//av_log(NULL, AV_LOG_INFO,"nalsize = %d buf_index = %d \n", nalsize, buf_index );
			p_buf[dest_index +0] = 0; p_buf[dest_index +1] = 0;
			p_buf[dest_index +2] = 0; p_buf[dest_index +3] = 1;
			dest_index += 4;

			buf_index += nalsize;
			dest_index += nalsize;
		}
	}
	else
	{
		unsigned char* p_dest;

		if( *piMaxNalLen < (buf_size+2*10) )
		{
			void* new_ptr;
			*piMaxNalLen = buf_size + 2*10;
			new_ptr = av_realloc( *ppDmxNal, *piMaxNalLen );
			if( new_ptr == NULL ) 
			{
				av_log(NULL, AV_LOG_INFO,"[CDMX:Err] avcc_to_annexb realloc failed \n");
				return -1;
			}

			memset( new_ptr, 0, *piMaxNalLen );
			*ppDmxNal = new_ptr;
		}
		p_dest = *ppDmxNal;
		for(;;)
		{
			int nalsize = 0;    
			if( buf_index >= buf_size ) 
				break;

			p_dest[dest_index +0] = 0; p_dest[dest_index +1] = 0;
			p_dest[dest_index +2] = 0; p_dest[dest_index +3] = 1;

			for( i = 0; i < iNalLengthSize; i++ )
				nalsize = (nalsize<<8) | p_buf[buf_index++];

			dest_index += 4;
			memcpy( &p_dest[dest_index], &p_buf[buf_index], nalsize );

			buf_index += nalsize;
			dest_index += nalsize;
		}
		memcpy( p_buf, p_dest, dest_index );

#ifdef _DEBUG
		//Dav_log(NULL, AV_LOG_INFO,"[CDMX] giNalLengthSize = %d\n", iNalLengthSize);
		if( dest_index < 10 )
		{
			int kkk;
			av_log(NULL, AV_LOG_INFO,"[CDMX] Frame Size = %d\n", dest_index);
			for( kkk = 0; kkk < dest_index; kkk++ )
				Dav_log(NULL, AV_LOG_INFO," %02X ", *(p_buf+kkk) );
			av_log(NULL, AV_LOG_INFO,"\n");
		}
#endif
	}

	*piSourLen = dest_index;

	return 0;
}

static int set_first_frame_with_seqheader(  unsigned char* pExtraData, int iExtraDataLen,unsigned char* pFirst, int* piFirstLen, 
							    unsigned char** ppDmxNal, int* piDmxNalLen, int bAvcC, int iNalLengthSize )
{
	char startcode[8] = {0,0,0,1,0,0,0,0};
	int i, buf_index = 0;
	unsigned char* p_buf = pFirst;
	int buf_size;// = iSeqLen + *piFirstLen;
	int dest_index = 0;
	unsigned char* p_dest;
	unsigned char pSeq[1000];
	int iSeqLen,gsiNalLengthSize;

	if( iExtraDataLen)
	{
			memset( pSeq, 0,iExtraDataLen );
			if( bAvcC )
			{
				gDmxNalLengthSize=iNalLengthSize=gsiNalLengthSize = avcc_spspps_to_annexb_spspps( pExtraData, 
																	iExtraDataLen,
																	pSeq, 
																	&iSeqLen );
				av_log(NULL, AV_LOG_INFO, "[CDMX] giNalLengthSize = %d, \n", gsiNalLengthSize );
			}
			else 
			{
				iSeqLen = iExtraDataLen;
				memcpy(pSeq,pExtraData,iSeqLen);
				av_log(NULL, AV_LOG_INFO, "[CDMX] SeqHeader Len = %d, \n", iSeqLen );
			}
	}
	else
		iSeqLen=0;

	buf_size = iSeqLen + *piFirstLen;;
	*piDmxNalLen = buf_size;

	if( *ppDmxNal == 0 )
	{
		*ppDmxNal = av_malloc( 512*1024/*buf_size*/ );
		if( *ppDmxNal == NULL ) 
		{
			av_log(NULL, AV_LOG_INFO,"[CDMX:Err] set_first_frame_with_seqheader malloc failed \n");
			return -1;
		}
		*piDmxNalLen = buf_size;
		memset( *ppDmxNal, 0, buf_size );
	}

	if( bAvcC )
	{
		p_dest = *ppDmxNal;
		buf_size = *piFirstLen;

		for(;;)
		{
			int nalsize = 0;    
			if( buf_index >= buf_size ) 
				break;

			p_dest[dest_index +0] = 0; p_dest[dest_index +1] = 0;
			p_dest[dest_index +2] = 0; p_dest[dest_index +3] = 1;

			for( i = 0; i < iNalLengthSize; i++ )
				nalsize = (nalsize<<8) | p_buf[buf_index++];

			dest_index += 4;
			memcpy( &p_dest[dest_index], &p_buf[buf_index], nalsize );

			buf_index += nalsize;
			dest_index += nalsize;
		}

		*piFirstLen = iSeqLen + dest_index;
		if( iSeqLen ) //!< bug fixed:20090629
		memcpy( pFirst, pSeq, iSeqLen );
		memcpy( pFirst+iSeqLen, p_dest, dest_index );
	}
	else
	{
		if( iSeqLen ) //!< bug fixed:20090629
		{
			memcpy( *ppDmxNal, pSeq, iSeqLen );
		}
		memcpy( *ppDmxNal+iSeqLen, pFirst, *piFirstLen );
		*piFirstLen = iSeqLen + *piFirstLen;
		memcpy( pFirst, *ppDmxNal, *piFirstLen );
	}


#if 0//def _DEBUG
	{
		int kkk;
		av_log(NULL, AV_LOG_INFO,"[CDMX] Seq Header Size = %d\n", iSeqLen);
		for( kkk = 0; kkk < iSeqLen; kkk++ )
			av_log(NULL, AV_LOG_INFO," %02X ", *(pFirst+kkk) );
		av_log(NULL, AV_LOG_INFO,"\n");

		av_log(NULL, AV_LOG_INFO,"[CDMX] Seq Header + First Frame Size = %d\n", *piFirstLen);
		for( kkk = 0; kkk < 40; kkk++ )
			av_log(NULL, AV_LOG_INFO," %02X ", *(pFirst+iSeqLen+kkk) );
		av_log(NULL, AV_LOG_INFO,"\n");
	}
#endif

	return 0;
}

 int Vpu_FirstAvcframe_with_seqheader(AVCodecContext *avctx,AVPacket *avpkt)
 {
 	const uint8_t *buf = avpkt->data;
	int ret,buf_size = avpkt->size;
	H264Context *h = avctx->priv_data;

 	gpDmxNal = av_malloc(512*1024);
	memset(gpDmxNal, 0, 512*1024);
	
	gpExtBuf = av_malloc(512*1024);
	av_log(NULL, AV_LOG_INFO,"-------- decode_init %x --------\n", gpDmxNal);
	memcpy(gpExtBuf, buf, buf_size);		
	set_first_frame_with_seqheader(avctx->extradata,avctx->extradata_size, 
			gpExtBuf, &buf_size, gpDmxNal, &gDmxNalLength, h->is_avc, gDmxNalLengthSize);
	tcc_vpu_decode_header(buf_size,gpExtBuf);
	ret=tcc_vpu_decode_Frame(buf_size,gpExtBuf);
	return ret;
 }

int Vpu_Decode_Avcframe(AVCodecContext *avctx,AVPacket *avpkt)
{
	const uint8_t *buf = avpkt->data;
	int buf_size = avpkt->size,ret;
	H264Context *h = avctx->priv_data;
	char *pData;


//	av_log(NULL, AV_LOG_INFO,"-------- Vpu_Decode_Avcframe %x --------\n", gpDmxNal);
//	memcpy(gpExtBuf, buf, buf_size);
//	gpExtBuf=buf;
	if( h->is_avc )	
		avcc_to_annexb( buf, &buf_size, gpDmxNal, &gDmxNalLength, gDmxNalLengthSize );

	pData = buf;
	
	ret=tcc_vpu_decode_Frame(buf_size,pData);
	return ret;
}
/********************************************
	End of AVC
********************************************/


/********************************************
	VC1
********************************************/

static int Set_first_frame_with_seqheader_VC1_SP_MP( AVCodecContext *avctx,AVPacket *avpkt)
{
  	const uint8_t *buf = avpkt->data;
	int buf_size = avpkt->size;
	VC1Context *v = avctx->priv_data;	
 	MpegEncContext *s = &v->s;
	unsigned char* pDst;
	unsigned int width, height, rcv_additional_header_size=12, bitrate=0, framerate, timestamp;
	unsigned int firstFrameDataLength_rcv;
	unsigned int pre_roll = 0xf0000000;
	unsigned int rcv_version = 0xc5000000;
	unsigned int TotalLength;
	int ret,iSeqLen;
	unsigned char *pSeq = avctx->extradata;
    	sh_video_t *sh = avctx->opaque;

	TotalLength = 40 + buf_size + avctx->extradata_size;
	
	width     = s->width;
	height    = s->height;
	bitrate = avctx->bit_rate;
//	framerate = avctx->rate_emu;
//	timestamp = avpkt->pts;
	framerate = sh->fps;
	timestamp =  sh->pts;
	iSeqLen = avctx->extradata_size;	
	firstFrameDataLength_rcv = buf_size;
//	if ( gs_stAsfDmxOutStream.m_bKeyFrame )
		firstFrameDataLength_rcv |= 0x80000000;

	pDst = av_malloc(buf_size+60);

	memcpy( pDst,    &rcv_version, sizeof(unsigned int) );
	memcpy( pDst+4,  &iSeqLen,     sizeof(unsigned int) );
	memcpy( pDst+8,  pSeq,  iSeqLen*sizeof(unsigned char) );
	memcpy( pDst+8+iSeqLen, &height, sizeof(unsigned int) );
	memcpy( pDst+12+iSeqLen, &width, sizeof(unsigned int) );
	memcpy( pDst+16+iSeqLen, &rcv_additional_header_size, sizeof(unsigned int) );
	memcpy( pDst+20+iSeqLen, &pre_roll, sizeof(unsigned int) );
	memcpy( pDst+24+iSeqLen, &bitrate, sizeof(unsigned int) );
	memcpy( pDst+28+iSeqLen, &framerate, sizeof(unsigned int) );
	memcpy( pDst+32+iSeqLen, &firstFrameDataLength_rcv, sizeof(unsigned int) );
	memcpy( pDst+36+iSeqLen, &timestamp, sizeof(unsigned int) );
	memcpy( pDst+40+iSeqLen, buf, buf_size*sizeof(unsigned char) );
	tcc_vpu_decode_header(TotalLength,pDst);
	ret=tcc_vpu_decode_Frame(TotalLength,pDst);
	if (pDst) av_free(pDst);
	return ret;
}

static int Set_first_frame_with_seqheader_VC1_AP( AVCodecContext *avctx,AVPacket *avpkt)//( unsigned char* pSeq, int iSeqLen, unsigned char* pFirst, int* piFirstLen)
{
	unsigned int start_code = 0x0d010000;
 	const uint8_t *buf = avpkt->data;
	int buf_size = avpkt->size;
	unsigned char* pDst,*p_buf;
//	int buf_size = iSeqLen + *piFirstLen + 3;
	int ret,iSeqLen,piFirstLen;
	unsigned char *pSeq =avctx->extradata;

	pDst = av_malloc(buf_size+60);
	iSeqLen = avctx->extradata_size;	
	p_buf=buf;
	iSeqLen --;
	memcpy( pDst, pSeq+1, iSeqLen );

	if ( (p_buf[0]==0)&&(p_buf[1]==0)&&(p_buf[2]==1) )
	{
		memcpy( pDst+iSeqLen, p_buf,buf_size );
		piFirstLen = iSeqLen + buf_size;
	}
	else
	{
		memcpy( pDst+iSeqLen, &start_code, sizeof(unsigned int));
		memcpy( pDst+iSeqLen+4, p_buf, buf_size );
		piFirstLen = iSeqLen + 4 + piFirstLen;
	}
	tcc_vpu_decode_header(piFirstLen,pDst);
	ret=tcc_vpu_decode_Frame(piFirstLen,pDst);
	if (pDst) av_free(pDst);
	return ret;
}

static int Set_frame_VC1_SP_MP(AVCodecContext *avctx,AVPacket *avpkt)
{
 	const uint8_t *buf = avpkt->data;
	int buf_size = avpkt->size;
	unsigned char* pDst;
	unsigned int ret,timestamp,FrameDataLength_rcv,IsKeyFrame=0;//temp,IsKeyFrame=0
    	sh_video_t *sh = avctx->opaque;

	FrameDataLength_rcv = buf_size;//*piFrameLen;
	if ( IsKeyFrame )
		FrameDataLength_rcv |= 0x80000000;

//	timestamp = avpkt->pts;
	timestamp =  sh->pts;
	pDst = av_malloc(buf_size+60);
	memcpy( pDst, &FrameDataLength_rcv, sizeof(unsigned int));
	memcpy( pDst+4, &timestamp, sizeof(unsigned int));
	memcpy( pDst+8, buf, buf_size);

	ret=tcc_vpu_decode_Frame(buf_size+8,pDst);
	if (pDst) av_free(pDst);
	return ret;
}

static int Set_frame_VC1_AP(AVCodecContext *avctx,AVPacket *avpkt)
{
 	const uint8_t *buf = avpkt->data;
	int buf_size = avpkt->size,piFrameLen,ret;
	unsigned char* pDst,*p_buf;
	unsigned int start_code = 0x0d010000;

	pDst = av_malloc(buf_size+60);
	p_buf=buf;
	if ( (p_buf[0]==0)&&(p_buf[1]==0)&&(p_buf[2]==1) )
	{
		memcpy( pDst, p_buf, buf_size );
		piFrameLen=buf_size;
	}
	else
	{
		memcpy(pDst, &start_code, sizeof(unsigned int));
		memcpy(pDst+4, p_buf, buf_size);
		piFrameLen = buf_size+4;
	}
	ret=tcc_vpu_decode_Frame(piFrameLen,pDst);
	if (pDst) av_free(pDst);
	return ret;
}

int Vpu_Firstvc1frame_with_seqheader(AVCodecContext *avctx,AVPacket *avpkt)
{
 	const uint8_t *buf = avpkt->data;
    	int buf_size = avpkt->size;
   	 VC1Context *v = avctx->priv_data;
   	 MpegEncContext *s = &v->s;
    	uint8_t *buf2 = NULL;
    	const uint8_t *buf_start = buf;

 

    /* We need to set current_picture_ptr before reading the header,
     * otherwise we cannot store anything in there. */
    	if(s->current_picture_ptr==NULL || s->current_picture_ptr->data[0]){
       	 int i= ff_find_unused_picture(s, 0);
        	s->current_picture_ptr= &s->picture[i];
    	}

    	if (s->avctx->codec->capabilities&CODEC_CAP_HWACCEL_VDPAU){
       	 if (v->profile < PROFILE_ADVANCED)
            		avctx->pix_fmt = PIX_FMT_VDPAU_WMV3;
        	else
            		avctx->pix_fmt = PIX_FMT_VDPAU_VC1;
    	}

    //for advanced profile we may need to parse and unescape data
    	if (avctx->codec_id == CODEC_ID_VC1) {
       	int buf_size2 = 0;
        	buf2 = av_mallocz(buf_size + FF_INPUT_BUFFER_PADDING_SIZE);

        	if(IS_MARKER(AV_RB32(buf))){ /* frame starts with marker and needs to be parsed */
            	const uint8_t *start, *end, *next;
            int size;

            next = buf;
            for(start = buf, end = buf + buf_size; next < end; start = next){
                next = find_next_marker(start + 4, end);
                size = next - start - 4;
                if(size <= 0) continue;
                switch(AV_RB32(start)){
                case VC1_CODE_FRAME:
                    if (avctx->hwaccel ||
                        s->avctx->codec->capabilities&CODEC_CAP_HWACCEL_VDPAU)
                        buf_start = start;
                    buf_size2 = vc1_unescape_buffer(start + 4, size, buf2);
                    break;
                case VC1_CODE_ENTRYPOINT: /* it should be before frame data */
                    buf_size2 = vc1_unescape_buffer(start + 4, size, buf2);
                    init_get_bits(&s->gb, buf2, buf_size2*8);
                    vc1_decode_entry_point(avctx, v, &s->gb);
                    break;
                case VC1_CODE_SLICE:
                    av_log(avctx, AV_LOG_ERROR, "Sliced decoding is not implemented (yet)\n");
                    av_free(buf2);
                    return -1;
                }
            }
        }
	else if(v->interlace && ((buf[0] & 0xC0) == 0xC0)){ /* WVC1 interlaced stores both fields divided by marker */
            const uint8_t *divider;

            divider = find_next_marker(buf, buf + buf_size);
            if((divider == (buf + buf_size)) || AV_RB32(divider) != VC1_CODE_FIELD){
                av_log(avctx, AV_LOG_ERROR, "Error in WVC1 interlaced frame\n");
                av_free(buf2);
                return -1;
            }

            buf_size2 = vc1_unescape_buffer(buf, divider - buf, buf2);
            // TODO
            if(!v->warn_interlaced++)
                av_log(v->s.avctx, AV_LOG_ERROR, "Interlaced WVC1 support is not implemented\n");
            av_free(buf2);return -1;
        }
	else
	{
            buf_size2 = vc1_unescape_buffer(buf, buf_size, buf2);
        }
        init_get_bits(&s->gb, buf2, buf_size2*8);
    } 
	else
        init_get_bits(&s->gb, buf, buf_size*8);
    // do parse frame header
    if(v->profile < PROFILE_ADVANCED) {
        if(vc1_parse_frame_header(v, &s->gb) == -1) {
            av_free(buf2);
            return -1;
        }
    } else {
        if(vc1_parse_frame_header_adv(v, &s->gb) == -1) {
            av_free(buf2);
            return -1;
        }
    }

    if(s->pict_type != FF_I_TYPE && !v->res_rtm_flag){
        av_free(buf2);
        return -1;
    }

    	if(s->pict_type != FF_I_TYPE)
	{
		av_log(NULL, AV_LOG_INFO,"fist frame not I frame,should return \n");	
		return -1;
    	}
    	av_free(buf2);

		if(avctx->codec_tag == FOURCC_WMV3|| avctx->codec_tag == FOURCC_WVC1||avctx->codec_tag ==FOURCC_WMVA|| avctx->codec_tag ==FOURCC_VC1\
      			||avctx->codec_tag ==FOURCC_wmv3|| avctx->codec_tag ==FOURCC_wvc1||avctx->codec_tag ==FOURCC_wmva|| avctx->codec_tag ==FOURCC_vc1)//
		{
			if(avctx->codec_tag == FOURCC_WMV3||avctx->codec_tag ==FOURCC_wmv3)
			{
				return Set_first_frame_with_seqheader_VC1_SP_MP(avctx,avpkt);
			}
			else
			{
				return Set_first_frame_with_seqheader_VC1_AP(avctx,avpkt);
			}

		}
}

int Vpu_Decode_Vc1frame(AVCodecContext *avctx,AVPacket *avpkt)
{
	int ret=-1;
	if(avctx->codec_tag == FOURCC_WMV3|| avctx->codec_tag == FOURCC_WVC1||avctx->codec_tag ==FOURCC_WMVA|| avctx->codec_tag ==FOURCC_VC1\
      		||avctx->codec_tag ==FOURCC_wmv3|| avctx->codec_tag ==FOURCC_wvc1||avctx->codec_tag ==FOURCC_wmva|| avctx->codec_tag ==FOURCC_vc1)//
	{
		if(avctx->codec_tag == FOURCC_WMV3||avctx->codec_tag ==FOURCC_wmv3)
		{
			ret=Set_frame_VC1_SP_MP(avctx,avpkt);
		}
		else
		{
			ret=Set_frame_VC1_AP(avctx,avpkt);
		}
	}
	return ret;
}

/********************************************
	end of VC1
********************************************/

/********************************************
	RMVB
********************************************/
//! Frame segment information
typedef struct segment_struct
{
    int m_ibIsValid;
    unsigned long int m_ulOffset;
} segment;

static int get_slice_offset(AVCodecContext *avctx, const uint8_t *buf, int n)
{
    if(avctx->slice_count) return avctx->slice_offset[n];
    else                   return AV_RL32(buf + n*8 - 4) == 1 ? AV_RL32(buf + n*8) :  AV_RB32(buf + n*8);
}
 int Vpu_FirstRvframe_with_seqheader(AVCodecContext *avctx,AVPacket *avpkt)
{
	const uint8_t *buf = avpkt->data;
	int buf_size = avpkt->size;
	RV34DecContext *r = avctx->priv_data;
	MpegEncContext *s = &r->s;
	SliceInfo si;
	int slice_count;
	const uint8_t *slices_hdr = NULL;
	int last = 0;
   //	 int *buf_offset=NULL;	
    	sh_video_t *sh = avctx->opaque;
	
	int ret,totalSize=0;
	
	unsigned int offset;
	int i = 0,m_ibIsValid;
	unsigned char* tmpbuf = 0;
	unsigned int   g_ulLength;
	unsigned int   g_ulMOFTag;		//audio video flag
	unsigned int   g_ulSubMOFTag;	//video_version ID
	unsigned short int   g_usWidth;
	unsigned short int   g_usHeight;
	unsigned short int   g_usBitCount;
	unsigned short int   g_usPadWidth;
	unsigned short int   g_usPadHeight;
	unsigned int g_ufFramesPerSecond;
	unsigned int   g_ulOpaqueDataSize;
	unsigned char*    g_pOpaqueData;


	unsigned int             ulDataLen;
	unsigned char*              pData;
	unsigned int             ulTimestamp;
	unsigned short int             usSequenceNum;
	unsigned short int             usFlags;
	int             bLastPacket;
	unsigned int             ulNumSegments;
	char *pDst;
	segment *pSegment;
	
	g_ulLength = 26+avctx->extradata_size;//temp avctx->frame_size;//gsRMFFDmxInfo.m_sVideoInfo.m_ulLength;
	tmpbuf = &g_ulLength;
	g_ulLength      = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];

	g_ulMOFTag = MKBETAG('V', 'I', 'D', 'O');//gsRMFFDmxInfo.m_sVideoInfo.m_ulMOFTag;		//audio video flag
	tmpbuf = &g_ulMOFTag;
	g_ulMOFTag      = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];

	g_ulSubMOFTag = avctx->codec_tag;//gsRMFFDmxInfo.m_sVideoInfo.m_ulSubMOFTag;	//video_version ID
	tmpbuf = &g_ulSubMOFTag;
	g_ulSubMOFTag      =(tmpbuf[3] << 24) | (tmpbuf[2] << 16) | (tmpbuf[1] << 8) | tmpbuf[0];

	g_usWidth =  avctx->width;//gsRMFFDmxInfo.m_sVideoInfo.m_iWidth;
	tmpbuf = 0;
	tmpbuf = &g_usWidth;
	g_usWidth      = (unsigned short int)((tmpbuf[0] << 8) | (tmpbuf[1] ));

	tmpbuf = 0;
	g_usHeight = avctx->height;//gsRMFFDmxInfo.m_sVideoInfo.m_iHeight;
	tmpbuf = &g_usHeight;
	g_usHeight      = (unsigned short int)((tmpbuf[0] << 8) | (tmpbuf[1] ));

	tmpbuf = 0;
	g_usBitCount =avctx->bit_rate;//sh->i_bps;// gsRMFFDmxInfo.m_sVideoInfo.m_usBitCount;
	tmpbuf = &g_usBitCount;
	g_usBitCount      = (unsigned short int)((tmpbuf[0] << 8) | (tmpbuf[1] ));

	tmpbuf = 0;
	g_usPadWidth = 0; //!< padded width, in pixels, of a video frame
	tmpbuf = &g_usPadWidth;
	g_usPadWidth      = (unsigned short int)((tmpbuf[0] << 8) | (tmpbuf[1] ));

	tmpbuf = 0;
	g_usPadHeight = 0; //!< padded height, in pixels, of a video frame
	tmpbuf = &g_usPadHeight;
	g_usPadHeight      = (unsigned short int)((tmpbuf[0] << 8) | (tmpbuf[1] ));

	g_ufFramesPerSecond = sh->fps;//gsRMFFDmxInfo.m_sVideoInfo.m_ulFramesPerSecond;
	tmpbuf = &g_ufFramesPerSecond;
	g_ufFramesPerSecond      = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];

	g_ulOpaqueDataSize = avctx->extradata_size;//gsRMFFDmxInfo.m_sVideoInfo.m_ulOpaqueDataSize;
	g_pOpaqueData =  avctx->extradata;//gsRMFFDmxInfo.m_sVideoInfo.m_pOpaqueData;



	ulDataLen = buf_size;//gsRMFFDmxOutput.m_iDataLen;
	tmpbuf = &ulDataLen;
	ulDataLen = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];

	ulTimestamp =  sh->pts;//gsRMFFDmxOutput.m_iTimestamp;
	tmpbuf = &ulTimestamp;
	ulTimestamp = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];

	tmpbuf = 0;
	usSequenceNum = 0;//gsRMFFDmxOutput.m_sSequenceNum;
	tmpbuf = &usSequenceNum;
	usSequenceNum = (unsigned short int)((tmpbuf[0] << 8) | (tmpbuf[1])) ;

	tmpbuf = 0;
	usFlags =2;// gsRMFFDmxOutput.m_sFlags;
	tmpbuf = &usFlags;
	usFlags = (unsigned short int)((tmpbuf[0] << 8) | (tmpbuf[1])) ;

	bLastPacket = 0;//gsRMFFDmxOutput.m_ibLastPacket;
	tmpbuf = &bLastPacket;
	bLastPacket = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];

    	if(!avctx->slice_count)
	{
        	slice_count = (*buf++) + 1;
        	slices_hdr = buf + 4;
        	buf += 8 * slice_count;
    	}
	else
        	slice_count = avctx->slice_count;

	if(pSegment)
		av_free(pSegment);
	pSegment=av_malloc(sizeof(segment)*(slice_count));
	
//	buf_offset=av_malloc(slice_count*sizeof(int));
    	for(i=0; i<slice_count; i++)
	{
        	int offset;//= get_slice_offset(avctx, slices_hdr, i);
		m_ibIsValid=1;
		tmpbuf = &m_ibIsValid;
		pSegment[i].m_ibIsValid = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];
		offset= get_slice_offset(avctx, slices_hdr, i);
		tmpbuf = &offset;
		pSegment[i].m_ulOffset = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];

		av_log(NULL, AV_LOG_INFO,"buf_offset[i] %d\n",offset);
	}
		
	ulNumSegments = slice_count;//gsRMFFDmxOutput.m_iNumSegments;
	tmpbuf = &ulNumSegments;
	ulNumSegments = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];



	totalSize=26 + g_ulOpaqueDataSize + 20 + 8*slice_count +buf_size;
	pDst = av_malloc(totalSize);

	av_log(NULL, AV_LOG_INFO,"g_ulLength=%d\n ",26+avctx->extradata_size);
	av_log(NULL, AV_LOG_INFO,"g_ulMOFTag=0x%x\n ",MKBETAG('V', 'I', 'D', 'O') );
	av_log(NULL, AV_LOG_INFO,"g_ulSubMOFTag=0x%x\n ",avctx->codec_tag );
	av_log(NULL, AV_LOG_INFO,"g_usWidth=%d\n ",avctx->width );
	av_log(NULL, AV_LOG_INFO,"g_usHeight=%d\n ",avctx->height );
	av_log(NULL, AV_LOG_INFO,"g_usBitCount=%d\n ",avctx->bit_rate );
	av_log(NULL, AV_LOG_INFO,"g_usPadWidth=%d\n ",g_usPadWidth );
	av_log(NULL, AV_LOG_INFO,"g_usPadHeight=%d\n ",g_usPadHeight );
	av_log(NULL, AV_LOG_INFO,"g_ufFramesPerSecond=0x%x\n ",sh->fps );
	av_log(NULL, AV_LOG_INFO,"g_ulOpaqueDataSize=%d\n ",avctx->extradata_size );	
	av_log(NULL, AV_LOG_INFO,"ulDataLen=%d\n ",buf_size );
	av_log(NULL, AV_LOG_INFO,"ulTimestamp=%d\n ", sh->pts );
	av_log(NULL, AV_LOG_INFO,"usSequenceNum=%d\n ",usSequenceNum );
	av_log(NULL, AV_LOG_INFO,"usFlags=%d\n ",usFlags );
	av_log(NULL, AV_LOG_INFO,"bLastPacket=%d\n ",bLastPacket );
	av_log(NULL, AV_LOG_INFO,"ulNumSegments=%d ,%d\n ",slice_count);
		int kkk;
				av_log(NULL, AV_LOG_INFO,"size = %d\t", g_ulOpaqueDataSize);
				for( kkk = 0; kkk < avctx->extradata_size; kkk++ )
					av_log(NULL, AV_LOG_INFO,"%02X ", avctx->extradata[kkk] );
				av_log(NULL, AV_LOG_INFO,"\n");
				
	memcpy(pDst, &g_ulLength, 4);
	memcpy(pDst + 4, &g_ulMOFTag, 4);
	memcpy(pDst + 8, &g_ulSubMOFTag, 4);
	memcpy(pDst + 12, &g_usWidth, 2);
	memcpy(pDst + 14, &g_usHeight, 2);
	memcpy(pDst + 16, &g_usBitCount, 2);
	memcpy(pDst + 18, &g_usPadWidth, 2);
	memcpy(pDst + 20, &g_usPadHeight, 2);
	memcpy(pDst + 22, &g_ufFramesPerSecond, 4);
	memcpy(pDst + 26, g_pOpaqueData, g_ulOpaqueDataSize);
	memcpy(pDst + 26 + g_ulOpaqueDataSize, &ulDataLen, 4);
	memcpy(pDst + 26 + g_ulOpaqueDataSize + 4, &ulTimestamp, 4);
	memcpy(pDst + 26 + g_ulOpaqueDataSize + 8, &usSequenceNum, 2);
	memcpy(pDst + 26 + g_ulOpaqueDataSize + 10, &usFlags, 2);
	memcpy(pDst + 26 + g_ulOpaqueDataSize + 12, &bLastPacket, 4);
	memcpy(pDst + 26 + g_ulOpaqueDataSize + 16, &ulNumSegments, 4);
	memcpy(pDst+ 26 + g_ulOpaqueDataSize + 20, pSegment, 8*slice_count);

	memcpy(pDst + 26 + g_ulOpaqueDataSize + 20 + 8*slice_count, buf, buf_size);

	tcc_vpu_decode_header(totalSize,pDst);
	ret=tcc_vpu_decode_Frame(totalSize,pDst);	
	if(pSegment)
		av_free(pSegment);
	if (pDst) 
		av_free(pDst);
	//if (buf_offset) 
	//	av_free(buf_offset);
	return ret;
}


int Vpu_Decode_Rvframe(AVCodecContext *avctx,AVPacket *avpkt)
{
	int ret=-1;
	const uint8_t *buf = avpkt->data;
	int buf_size = avpkt->size;
	RV34DecContext *r = avctx->priv_data;
	MpegEncContext *s = &r->s;
	SliceInfo si;
	int slice_count;
	const uint8_t *slices_hdr = NULL;
	int last = 0;
   	 int *buf_offset=NULL;		
	int totalsize=0;
	//	unsigned int offset;
	int i = 0,m_ibIsValid;
	unsigned char* tmpbuf = 0,*pDst;
	segment *pSegment;

		unsigned int             ulDataLen;
	//	unsigned char*              pData;
	unsigned int             ulTimestamp;
	unsigned short int             usSequenceNum;
	unsigned short int             usFlags;
	int             bLastPacket;
	unsigned int             ulNumSegments;


	ulDataLen = buf_size;//gsRMFFDmxOutput.m_iDataLen;
	tmpbuf = &ulDataLen;
	ulDataLen = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];
	
	ulTimestamp = avpkt->pts;//gsRMFFDmxOutput.m_iTimestamp;
	tmpbuf = &ulTimestamp;
	ulTimestamp = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];
	
	tmpbuf = 0;
	usSequenceNum = 0;//gsRMFFDmxOutput.m_sSequenceNum;
	tmpbuf = &usSequenceNum;
	usSequenceNum = (unsigned short int)((tmpbuf[0] << 8) | (tmpbuf[1])) ;

	tmpbuf = 0;
	usFlags =2;// gsRMFFDmxOutput.m_sFlags;
	tmpbuf = &usFlags;
	usFlags = (unsigned short int)((tmpbuf[0] << 8) | (tmpbuf[1])) ;

	bLastPacket =0;// gsRMFFDmxOutput.m_ibLastPacket;
	tmpbuf = &bLastPacket;
	bLastPacket = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];

 	if(!avctx->slice_count)
	{
        	slice_count = (*buf++) + 1;
        	slices_hdr = buf + 4;
        	buf += 8 * slice_count;
    	}
	else
        	slice_count = avctx->slice_count;

	buf_offset=av_malloc(slice_count*sizeof(int));
    	for(i=0; i<slice_count; i++)
	{
        	int offset= get_slice_offset(avctx, slices_hdr, i);
		buf_offset[i]=offset;
//		av_log(NULL, AV_LOG_INFO,"buf_offset[i] %d\n",offset);
	}
		
	ulNumSegments = slice_count;//gsRMFFDmxOutput.m_iNumSegments;
	tmpbuf = &ulNumSegments;
	ulNumSegments = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];

	if(pSegment)
		av_free(pSegment);
	pSegment=av_malloc(sizeof(segment)*(slice_count));
	
	////////////////////pSegment data///////////////////////////
	for(i = 0; i <slice_count; i++)
	{
		m_ibIsValid=1;
		tmpbuf = &m_ibIsValid;
		pSegment[i].m_ibIsValid = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];
		tmpbuf = &buf_offset[i];
		pSegment[i].m_ulOffset = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) | tmpbuf[3];
	}
	totalsize= 20 + 8*slice_count +buf_size;

	pDst = av_malloc(totalsize);

	memcpy(pDst , &ulDataLen, 4);
	memcpy(pDst + 4, &ulTimestamp, 4);
	memcpy(pDst + 8, &usSequenceNum, 2);
	memcpy(pDst + 10, &usFlags, 2);
	memcpy(pDst + 12, &bLastPacket, 4);
	memcpy(pDst + 16, &ulNumSegments, 4);
	memcpy(pDst + 20, pSegment, 8*slice_count);
	memcpy(pDst + 20 + 8*slice_count,  buf, buf_size);


	if(pSegment)
		av_free(pSegment);

	if (buf_offset) 
		av_free(buf_offset);

	ret=tcc_vpu_decode_Frame(totalsize,pDst);
	if (pDst) av_free(pDst);
	return ret;

}

/********************************************
	End of RMVB
********************************************/


int tcc_vpu_decode_FirstFrame(int bitstream_format,AVCodecContext *avctx,AVPacket *avpkt)//(int seq_stream_size, char *seq_stream_buf)
{
	int ret;
	char *seq_stream_buf;
	int seq_stream_size;

	switch(bitstream_format)
	{
		case STD_MPEG2:
			ret=Vpu_FirstMpeg2frame_with_seqheader(avctx,avpkt);
			break;
		case STD_DIV3:
			ret=Vpu_FirstDiv3frame_with_seqheader(avctx,avpkt);
			break;
		case STD_MPEG4:
			ret=Vpu_Firstmpeg4frame_with_seqheader(avctx,avpkt);
			break;
		case STD_VC1:
			ret=Vpu_Firstvc1frame_with_seqheader(avctx,avpkt);
			//ret=Vpu_Decode_Vc1frame(avctx,avpkt);
			break;
		case STD_RV:
			ret=Vpu_FirstRvframe_with_seqheader(avctx,avpkt);
			break;
		case STD_AVC:
			ret=Vpu_FirstAvcframe_with_seqheader(avctx,avpkt);
			break;			
		default:
			break;
	}
	return ret;
}


int tcc_vpu_decode_NextFrame(int bitstream_format,AVCodecContext *avctx,AVPacket *avpkt)
{
	int ret;
	char *seq_stream_buf;
	int seq_stream_size;

	//av_log(NULL, AV_LOG_INFO,"-------- tcc_vpu_decode_NextFrame %d --------\n", bitstream_format);

	switch(bitstream_format)
	{
		case STD_MPEG2:
			ret=Vpu_Decode_Mpeg2frame(avctx,avpkt);
			break;
		case STD_DIV3:
			ret=Vpu_Decode_Div3frame(avctx,avpkt);
			break;
		case STD_MPEG4:
			ret=Vpu_Decode_Mpeg4frame(avctx,avpkt);
			break;
		case STD_VC1:
			ret=Vpu_Decode_Vc1frame(avctx,avpkt);
			break;
		case STD_RV:
			ret=Vpu_Decode_Rvframe(avctx,avpkt);
			break;
		case STD_AVC:
		//	av_log(NULL, AV_LOG_INFO,"-------- tcc_vpu_decode_NextFrame 111 %d --------\n", bitstream_format);
			ret=Vpu_Decode_Avcframe(avctx,avpkt);
			break;
		default:
			break;
	}
	return ret;
}


int tcc_vpu_codec_decode(int bitstream_format,AVCodecContext *avctx,AVPacket *avpkt)
{
	int ret;
	MpegEncContext *s = avctx->priv_data;

	if(bitstream_format==STD_AVC)  //to fix bug of some mov file close first
	{
		if(gpDmxNal==NULL)
		{
			tcc_vpu_codec_init(STD_AVC,0,avctx->width,avctx->height);
		}
	}
	
	if(is_first_frame)
	{
		ret = tcc_vpu_decode_FirstFrame(bitstream_format,avctx,avpkt);
		if(ret>=0)
			is_first_frame=0;
		return ret;
	}
	ret=tcc_vpu_decode_NextFrame(bitstream_format,avctx,avpkt);
	return ret;
}



int tcc_vpu_decode_header(int seq_stream_size, char *seq_stream_buf)
{
	int ret;
	memcpy( p_input, seq_stream_buf, seq_stream_size);	
	av_log(NULL, AV_LOG_INFO,"[VDEC] VDEC_DEC_SEQ_HEADER start size %d\n",seq_stream_size );

	int kkk;
	av_log(NULL, AV_LOG_INFO,"size = %d\t", seq_stream_size);
	for( kkk = 0; kkk < 64; kkk++ )
		av_log(NULL, AV_LOG_INFO,"%02X ", p_input[kkk] );
	av_log(NULL, AV_LOG_INFO,"\n");
			
	ret = TCC_VPU_DEC( VPU_DEC_SEQ_HEADER, &dec_handle, (void*)seq_stream_size, &dec_initial_info );
	if( (ret != RETCODE_SUCCESS) || (ret == RETCODE_CODEC_SPECOUT) )
	{
		av_log(NULL, AV_LOG_INFO,"VPU_DEC_SEQ_HEADER failed Error code is 0x%x \n", ret );
		ret=-1;
		return ret;
	}
	av_log(NULL, AV_LOG_INFO,"TCC_VPU_DEC VPU_DEC_SEQ_HEADER OK!\n");

	print_dec_initial_info( &dec_init, &dec_initial_info );

	//------------------------------------------------------------
	//! [x] slice buffer for VPU
	//------------------------------------------------------------
	if( dec_init.m_iBitstreamFormat == STD_AVC )
	{
		slice_buf_size = SLICE_SAVE_SIZE;
		slice_buf_size = ALIGNED_BUFF( slice_buf_size, 4*1024 );
		slice_buf_addr = (codec_addr_t)sys_malloc_physical_addr( slice_buf_size );
		if( slice_buf_addr == 0 ) 
		{
			av_log(NULL, AV_LOG_INFO,"slice_buf_addr malloc() failed \n");
			ret=-1;
			return ret;
		}
		av_log(NULL, AV_LOG_INFO,"slice_buf_addr = 0x%x, 0x%x \n", (codec_addr_t)slice_buf_addr, slice_buf_size );

		dec_buffer.m_AvcSliceSaveBufferAddr = slice_buf_addr;
		dec_buffer.m_iAvcSliceSaveBufferSize = slice_buf_size;
	}		

	//------------------------------------------------------------
	//! [x] frame buffer for each VPU decoder
	//------------------------------------------------------------
	//dec_buffer.m_iFrameBufferCount = dec_initial_info.m_iMinFrameBufferCount;
	#ifdef FRAME_BUFFER_CLEAR_DELAY
	dec_buffer.m_iFrameBufferCount += FRAME_BUFFER_CLEAR_DELAY;
	#endif

//	if(dec_init.m_iBitstreamFormat == STD_AVC)
//		dec_buffer.m_iFrameBufferCount = dec_initial_info.m_iMinFrameBufferCount + 4 /* number of port buffer */ + 2 /* for avc */; 
//	else
		dec_buffer.m_iFrameBufferCount = dec_initial_info.m_iMinFrameBufferCount + 4 /* number of port buffer */;

	frame_buf_size = dec_buffer.m_iFrameBufferCount * dec_initial_info.m_iMinFrameBufferSize;
	frame_buf_size = ALIGNED_BUFF( frame_buf_size, 4*1024 );
	frame_buf_addr[PA] = (codec_addr_t)sys_malloc_physical_addr( frame_buf_size );
	if( frame_buf_addr[PA] == 0 ) 
	{
		av_log(NULL, AV_LOG_INFO,"frame_buf_addr[PA] malloc() failed \n");
		ret=-1;
		return ret;
	}	
	av_log(NULL, AV_LOG_INFO,"one frame_buf size %d bytes \n", dec_initial_info.m_iMinFrameBufferSize );
	av_log(NULL, AV_LOG_INFO,"frame_buf_addr[PA] = 0x%x, 0x%x \n", (codec_addr_t)frame_buf_addr[PA], frame_buf_size );
	frame_buf_addr[VA] = (codec_addr_t)sys_malloc_virtual_addr( &frame_buf_dev, frame_buf_addr[PA], frame_buf_size );
	if( frame_buf_addr[VA] == 0 ) 
	{
		av_log(NULL, AV_LOG_INFO,"frame_buf_addr[VA] malloc() failed \n");
		ret=-1;
		return ret;
	}
	av_log(NULL, AV_LOG_INFO,"frame_buf_addr[VA] = 0x%x, 0x%x \n", (codec_addr_t)frame_buf_addr[VA], frame_buf_size );

	dec_buffer.m_FrameBufferStartAddr[PA] = frame_buf_addr[PA];
	dec_buffer.m_FrameBufferStartAddr[VA] = frame_buf_addr[VA];
	
	if(dec_buffer.m_iFrameBufferCount > 32)  // if the frameBufferCount exceeds the maximum buffer count number 32..
	{
		av_log(NULL, AV_LOG_INFO,"DEC_REG_FRAME_BUFFER failed Error code is 0x%x \n", ret );
		ret=-1;
		return ret;
	}
	ret = TCC_VPU_DEC( VPU_DEC_REG_FRAME_BUFFER, &dec_handle, &dec_buffer, 0 );
	if( ret != RETCODE_SUCCESS )
	{
		av_log(NULL, AV_LOG_INFO,"DEC_REG_FRAME_BUFFER failed Error code is 0x%x \n", ret );
		ret=-1;
		return ret;
	}
	av_log(NULL, AV_LOG_INFO,"TCC_VPU_DEC VPU_DEC_REG_FRAME_BUFFER OK!\n");

	av_log(NULL, AV_LOG_INFO,"frame_buf_delay = %d\n", dec_initial_info.m_iFrameBufDelay );
	av_log(NULL, AV_LOG_INFO,"min frame_buf_count = %d\n", dec_initial_info.m_iMinFrameBufferCount );
	av_log(NULL, AV_LOG_INFO,"allocated frame_buf_count = %d\n", dec_buffer.m_iFrameBufferCount );
	iFameSearchorSkip=1;
	set_vpu_delay_time();
	return ret;

}

void tcc_vpu_freemem(void)
{
	Uninit_Vpufd();
	if( bit_work_buf_addr[PA] )
	{
		sys_free_physical_addr( (void*)bit_work_buf_addr[PA], bit_work_buf_size );
		bit_work_buf_addr[PA]=0;
	}
	if( bit_work_buf_addr[VA] )
	{
		sys_free_virtual_addr( &bit_work_buf_dev, bit_work_buf_addr[VA], bit_work_buf_size );
		bit_work_buf_addr[VA] =0;
	}
	if( reg_base_addr )
	{
		sys_free_virtual_addr( &reg_base_dev, reg_base_addr, reg_base_size );
		reg_base_addr=0;
	}
	if( bitstream_buf_addr[PA] )
	{
		sys_free_physical_addr( (void*)bitstream_buf_addr[PA], bitstream_buf_size );
		bitstream_buf_addr[PA]=0;
	}
	if( bitstream_buf_addr[VA] )
	{
		sys_free_virtual_addr( &bitstream_buf_dev, bitstream_buf_addr[VA], bitstream_buf_size );
		bitstream_buf_addr[VA]=0;
	}
	if( slice_buf_addr )
	{
		sys_free_physical_addr( (void*)slice_buf_addr, slice_buf_size );
		slice_buf_addr =0;
	}
	if( sps_pps_buf_addr )
	{
		sys_free_physical_addr( (void*)sps_pps_buf_addr, sps_pps_buf_size );
		sps_pps_buf_addr=0;
	}
	if( frame_buf_addr[PA] )
	{
		sys_free_physical_addr( (void*)frame_buf_addr[PA], frame_buf_size );
		frame_buf_addr[PA]=0;
	}
	if( frame_buf_addr[VA] )
	{
		sys_free_virtual_addr( &frame_buf_dev, frame_buf_addr[VA], frame_buf_size );	
		frame_buf_addr[VA]=0;
	}


	if (input_packet_buffer_addr )
		av_free ( (void*)input_packet_buffer_addr );
	input_packet_buffer_addr =NULL;
//    memset(h, 0, sizeof(H264Context));
	if (gpDmxNal) 
		av_free(gpDmxNal);
	if (gpExtBuf) 
		av_free(gpExtBuf);
	gpDmxNal=NULL;
	gpExtBuf=NULL;
#if defined(VPU_CLK_CONTROL)
	vpu_clock_deinit();
#endif
}

int tcc_vpu_codec_deinit(void)
{
	codec_result_t ret = 0;

	av_log(NULL, AV_LOG_INFO,"==>%s, dec:%d, video:%d, audio:%d\n", __FUNCTION__, g_dec_time, g_video_frame, g_audio_frame);

	//if there's a pandding output frame, flush output frame
	ret = TCC_VPU_DEC( VPU_DEC_FLUSH_OUTPUT, &dec_handle, &dec_input, &dec_output );

	if( dec_output.m_DecOutInfo.m_iOutputStatus == VPU_DEC_OUTPUT_SUCCESS ) 
	{
		TCC_VPU_DEC( VPU_DEC_BUF_FLAG_CLEAR, &dec_handle, &dec_output.m_DecOutInfo.m_iDispOutIdx, 0 );
	}
	// Now that we are done with decoding, close the open instance.
	ret = TCC_VPU_DEC( VPU_DEC_CLOSE, &dec_handle, 0, &dec_output );
//	av_log(NULL, AV_LOG_INFO,"\nDec End. Tot Frame %d\n", tot_idx );

	av_log(NULL, AV_LOG_INFO,"-------------------DECODER CLOSE2-------------------\n");

	tcc_vpu_freemem();
#ifdef VDEC_VPU_INDEX_MUTEX
	pthread_mutex_destroy(&vpu_index_mutex);
#endif
}

int	get_frame_type_for_frame_skipping(int iStdType, int iPicType, int iPicStructure)
{
	int frameType = 0; //unknown

	switch ( iStdType )
	{
	case STD_VC1 :
		switch( (iPicType>>3) ) //Frame or // FIELD_INTERLACED(TOP FIELD)
		{
		case PIC_TYPE_I:	frameType = 1; break;//I
		case PIC_TYPE_P:	frameType = 2; break;//P
		case 2:				frameType = 3; break;//B //av_log(NULL, AV_LOG_INFO, "BI  :" );
		case 3:				frameType = 3; break;//B //av_log(NULL, AV_LOG_INFO, "B   :" );
		case 4:				frameType = 3; break;//B //av_log(NULL, AV_LOG_INFO, "SKIP:" );
		}
		if( iPicStructure == 3) 
		{
			switch( (iPicType&0x7) ) // FIELD_INTERLACED(BOTTOM FIELD)
			{
			case PIC_TYPE_I:	frameType = 1; break;//I
			case PIC_TYPE_P:	frameType = 2; break;//P
			case 2:				frameType = 3; break;//B //av_log(NULL, AV_LOG_INFO, "BI  :" );
			case 3:				frameType = 3; break;//B //av_log(NULL, AV_LOG_INFO, "B   :" );
			case 4:				frameType = 3; break;//B //av_log(NULL, AV_LOG_INFO, "SKIP:" );
			}
		}
		break;
	case STD_MPEG4 :
		switch( iPicType )
		{
		case PIC_TYPE_I:	frameType = 1;	break;//I
		case PIC_TYPE_P:	frameType = 2;	break;//P
		case PIC_TYPE_B:	frameType = 3;	break;//B
		case PIC_TYPE_B_PB: frameType = 4;	break;//B of Packed PB-frame
		}
		break;
	case STD_MPEG2 :
	default:
		switch( iPicType )
		{
		case PIC_TYPE_I:	frameType = 1;	break;//I
		case PIC_TYPE_P:	frameType = 2;	break;//P
		case PIC_TYPE_B:	frameType = 3;	break;//B
		}
	}
	return frameType;
}

int get_framebuffer_delay_num()
{
	return dec_initial_info.m_iFrameBufDelay; 
}
int tcc_vpu_decode_Frame(unsigned int input_len,unsigned char *p_input)
{
	int ret=1;
	 int	decode_result;

	int displayindex,is_full;
	double vpts;
	float frametime;
	{
	//	av_log(NULL, AV_LOG_INFO,"-------------------DECODER RUN--0-----------------%d\n",input_len);

		dec_input.m_BitstreamDataAddr[PA] = bitstream_buf_addr[PA];
		dec_input.m_BitstreamDataAddr[VA] = bitstream_buf_addr[VA];
		dec_input.m_iBitstreamDataSize = input_len;

		if(dec_init.m_bEnableUserData)
		{
			dec_input.m_UserDataAddr[PA] = userdata_buf_addr[PA];
			dec_input.m_UserDataAddr[VA] = userdata_buf_addr[VA];
			dec_input.m_iUserDataBufferSize = userdata_buf_size;
		}
		//av_log(NULL, AV_LOG_INFO,"-------------------DECODER RUN---1----------------\n");

		// avoid copy data to itself
		//if (dec_input.m_BitstreamDataAddr[VA] != p_input)
		memcpy(dec_input.m_BitstreamDataAddr[VA], p_input, input_len);
		//av_log(NULL, AV_LOG_INFO,"copy data from buf:%x\n", dec_input.m_BitstreamDataAddr[VA]);
		// to tell demuxer he can read next frame
		//unlock_video_dmx();
		set_dec_done(1);
		//av_log(NULL, AV_LOG_INFO,"-------------------DECODER RUN--2-----------------\n");
/*
	int kkk;
	av_log(NULL, AV_LOG_INFO,"size = %d\t", input_len);
	for( kkk = 0; kkk < 64; kkk++ )
		av_log(NULL, AV_LOG_INFO,"%02X ", p_input[kkk] );
	av_log(NULL, AV_LOG_INFO,"\n");
*/

#ifdef VDEC_VPU_INDEX_MUTEX
		while (1) 
		{	
			pthread_mutex_lock(&vpu_index_mutex);
			displayindex = dequeue_vpu_displayindex();
			pthread_mutex_unlock(&vpu_index_mutex);		
						
			//av_log(NULL, AV_LOG_INFO,"vpu_clear_idx : %d\n",displayindex);

			if(displayindex >= 0 )
			{				
				//if( ( ret = vdec_func( VDEC_BUF_FLAG_CLEAR, NULL, &displayindex, NULL ) ) < 0 )
				if(TCC_VPU_DEC( VPU_DEC_BUF_FLAG_CLEAR, &dec_handle, &displayindex, 0 )<0)
				{
					av_log(NULL, AV_LOG_INFO,"[[OMX_CDKVDEC]] VDEC_BUF_FLAG_CLEAR error %d\n", displayindex );
					while (1);
					return ret;
				}
			} 
			else 
			{
				break;
			}
		}
	
		pthread_mutex_lock(&vpu_index_mutex);
		is_full = is_full_vpu_displayindex_queue();
		pthread_mutex_unlock(&vpu_index_mutex);

		if ( is_full ) 
		{
			av_log(NULL, AV_LOG_INFO,"[OMX_CDKVDEC]vpu_displayindex_full \n");
			usleep(5000);
			return ret;
		}
#endif

		if(rel_seek_secs || abs_seek_pos) //seek.skip I frame
		{
				
			dec_input.m_iSkipFrameNum = 1;
			dec_input.m_iFrameSearchEnable = 0;	//I-frame (IDR-picture for H.264)
			
			if(skipIFrameCnt == 0 || dispIFrameNum == 0)
			{
				skipIFrameCnt++;
				dec_input.m_iSkipFrameMode = VDEC_SKIP_FRAME_EXCEPT_I;				
			}
			else
			{
				if(skipIFrameCnt <dispIFrameNum-1)
				{
					skipIFrameCnt++;
				}
				else
				{
					skipIFrameCnt = 0;
				}
					
				dec_input.m_iSkipFrameMode = VDEC_SKIP_FRAME_UNCOND;					
			}
		}
		else
		{
			if(iFameSearchorSkip == 1)
			{
				dec_input.m_iSkipFrameNum = 1;

				dec_input.m_iFrameSearchEnable = 0x201;	//I-frame (I-slice for H.264)
					//psVDecInput->m_iFrameSearchEnable = 0x001;	//I-frame (IDR-picture for H.264)

				dec_input.m_iSkipFrameMode = VDEC_SKIP_FRAME_DISABLE;
					//printf("[CDKVDEC] VDEC Input : iSearchFrame : %lld\n",pInputBuffer->nTimeStamp);
			}
			else if(iFameSearchorSkip == 2)
			{
				dec_input.m_iSkipFrameNum = 1;
				dec_input.m_iFrameSearchEnable = 0;	//I-frame (IDR-picture for H.264)
				//gsVDecInput.m_iFrameSearchEnable = 513;	//I-frame (I-slice for H.264)
				dec_input.m_iSkipFrameMode = VDEC_SKIP_FRAME_ONLY_B;				
				//printf("[CDKVDEC] VDEC Input : BFrame Skip : %lld \n",pInputBuffer->nTimeStamp);
			}
			else
			{
				dec_input.m_iSkipFrameNum = 0;
				dec_input.m_iFrameSearchEnable = 0;	//I-frame (IDR-picture for H.264)
				//gsVDecInput.m_iFrameSearchEnable = 513;	//I-frame (I-slice for H.264)
				dec_input.m_iSkipFrameMode = VDEC_SKIP_FRAME_DISABLE;
				//printf("[CDKVDEC] VDEC Input : No Skip :%lld \n",pInputBuffer->nTimeStamp);
			}
		}
			
		dec_input.m_iErrorMBsLimitaton = 0;
		if( dec_init.m_iBitstreamFormat == STD_AVC )
		{
			#define VPU_H264_DEC_ERR_MBS_CONTRAINT	0 //1024
			int error_mbs_limitation;

			error_mbs_limitation = VPU_H264_DEC_ERR_MBS_CONTRAINT;
			
			if( error_mbs_limitation < 0 )
			{
				error_mbs_limitation = 0;
			}

			if( error_mbs_limitation )
			{
				av_log(NULL, AV_LOG_INFO, "[VDEC] VPU_DEC_DECODE Limitation of Error MBs is %d \n", error_mbs_limitation );
			}
			dec_input.m_iErrorMBsLimitaton = error_mbs_limitation;
		}
		//av_log(NULL, AV_LOG_INFO,"m_iSkipFrameMode= %d :\n", dec_input.m_iSkipFrameMode);
		//av_log(NULL, AV_LOG_INFO,"m_iFrameSearchEnable= %d :\n", dec_input.m_iFrameSearchEnable);
		//av_log(NULL, AV_LOG_INFO,"m_iSkipFrameNum= %d :\n", dec_input.m_iSkipFrameNum);

		// Start decoding a frame.
		unsigned int start=GetTimer();
		ret = TCC_VPU_DEC( VPU_DEC_DECODE, &dec_handle, &dec_input, &dec_output );
		//av_log(NULL, AV_LOG_INFO,"-------------------DECODER RUN---3----------------%d\n",ret);
		unsigned int end=GetTimer();
		double times=(double)(end-start)*0.000001;
		sum_vpu_decode_time(times);
		if(ret == RETCODE_CODEC_EXIT)
		{
			av_log(NULL, AV_LOG_INFO,"TCC_VPU_HW_Reset %d\n", ret );
			TCC_VPU_HW_Reset();
			return;
		}
		
		if( ret == RETCODE_CODEC_FINISH )
			return ret;
		if( ret != RETCODE_SUCCESS )
		{
			av_log(NULL, AV_LOG_INFO,"VPU_DecStartOneFrame failed Error code is 0x%x \n", ret );

			ret=-1;
			return ret;
		}

	//	av_log(NULL, AV_LOG_INFO,"dec_output.m_DecOutInfo.m_iOutputStatus %d,dec_output.m_DecOutInfo.m_iDecodedIdx:%2d,dec_output.m_DecOutInfo.m_iDispOutIdx,%d\n", dec_output.m_DecOutInfo.m_iOutputStatus,dec_output.m_DecOutInfo.m_iDecodedIdx,dec_output.m_DecOutInfo.m_iDispOutIdx );	
		if(iFameSearchorSkip
				&& dec_output.m_DecOutInfo.m_iDecodedIdx >= 0 
				&& (  dec_output.m_DecOutInfo.m_iDecodingStatus == VPU_DEC_SUCCESS
			  || dec_output.m_DecOutInfo.m_iDecodingStatus == VPU_DEC_SUCCESS_FIELD_PICTURE )
		)
		{

	//			av_log(NULL, AV_LOG_INFO,"m_iBitstreamFormat %d \n", dec_init.m_iBitstreamFormat );

				if(dec_input.m_iFrameSearchEnable )
				{
					iFameSearchorSkip = 2;//I-frame Search Mode disable and B-frame Skip Mode enable
				}
				else if( dec_input.m_iSkipFrameMode == VDEC_SKIP_FRAME_ONLY_B )
				{
					iFameSearchorSkip = 0;	//B-frame Skip Mode disable		
				}
		}
		if(iFameSearchorSkip ==0)
			iFameSearchorSkip = skip_b_frame;	//B-frame Skip Mode disable
		//print_pic_type(dec_init.m_iBitstreamFormat, dec_output.m_DecOutInfo.m_iPicType, dec_output.m_DecOutInfo.m_iPictureStructure);
		//av_log(NULL, AV_LOG_INFO,"skip b frame:%d\n", skip_b_frame);
		// clear when dstatus is bottom field
#ifndef VDEC_VPU_INDEX_MUTEX
		if(dec_output.m_DecOutInfo.m_iDecodedIdx>=0)
			TCC_VPU_DEC( VPU_DEC_BUF_FLAG_CLEAR, &dec_handle, &dec_output.m_DecOutInfo.m_iDecodedIdx, 0 );
//		av_log(NULL, AV_LOG_INFO,", clearIdx:%2d, psVDecOutput->m_DecOutInfo.m_iDecodingStatus =%d\n", dec_output.m_DecOutInfo.m_iDecodedIdx, dec_output.m_DecOutInfo.m_iDecodingStatus  );
#else
		#ifdef USE_DISP_INDEX_CHK_QUEUE
		if (	(dec_init.m_iBitstreamFormat== STD_VC1)
				&& ( (dec_output.m_DecOutInfo.m_iPictureStructure != 3) && ((dec_output.m_DecOutInfo.m_iPicType>>3)==4) ) )
		{
			// do not check
			g_curr_vc1_skip_index = dec_output.m_DecOutInfo.m_iDecodedIdx;
		} 
		else
		{
			check_vpu_index_queue(dec_output.m_DecOutInfo.m_iDecodedIdx);
		}
		#endif
		#ifdef	USE_DISP_INDEX_CHK_QUEUE
		if ( dec_output.m_DecOutInfo.m_iDispOutIdx== g_curr_vc1_skip_index) 
		{
			chk_queue_vpu_displayindex (dec_output.m_DecOutInfo.m_iDispOutIdx|0x40000000);
		}
		else 
		{
			chk_queue_vpu_displayindex (dec_output.m_DecOutInfo.m_iDispOutIdx);
		}
		#endif
		queue_vpu_displayindex(dec_output.m_DecOutInfo.m_iDispOutIdx);
#endif
	}
//	vpts=Get_Vpts();
//	vpts=vpts-get_vpu_delay_time();
//	Set_Vpts(vpts);
	return ret;
}

int tcc_vpu_codec_get_RV_timestamp(void)
{
	//temp return dispIdx_info[dec_output.m_DecOutInfo.m_iDispOutIdx][3];
}

int GetVideoHeight(void)
{
	return dec_initial_info.m_iPicHeight;//-GetVideoHeightOffset();
}

int GetVideoWidth(void)
{
	return dec_initial_info.m_iPicWidth;//-GetVideoWidthOffset();
	
}

int GetVideoHeightOffset(void)
{
	return (dec_initial_info.m_iAvcPicCrop.m_iCropTop+dec_initial_info.m_iAvcPicCrop.m_iCropBottom);	
}

int GetVideoWidthOffset(void)
{
	return (dec_initial_info.m_iAvcPicCrop.m_iCropLeft+dec_initial_info.m_iAvcPicCrop.m_iCropRight);
	
}




int ReturnVirtAddr(int index)
{
//	av_log(NULL, AV_LOG_INFO,"addr %d is ox%d\n",index,dec_output.m_pDispOut[VA][index]);
	return dec_output.m_pDispOut[VA][index];
}

int ReturnPhyAddr(int index)
{
	//av_log(NULL, AV_LOG_INFO,"addr %d is ox%d\n",index,dec_output.m_pDispOut[VA][index]);
	return dec_output.m_pDispOut[PA][index];
}

int ReturnPhyAddr_prev(int index)
{
	return dec_output.m_pPrevOut[PA][index];
}

int GetCodecInfoInterlaced ()
{
	// MPEG2
	if ( ( dec_output.m_DecOutInfo.m_iM2vProgressiveFrame == 0 &&dec_output.m_DecOutInfo.m_iPictureStructure == 0x3 ) ||
		dec_output.m_DecOutInfo.m_iInterlacedFrame
		)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int GetCodecInfoOddFirst ()
{
	if ( dec_output.m_DecOutInfo.m_iTopFieldFirst )
		return 0;
	else
		return 1;
}
//#endif//TCC_89XX_INCLUDE
