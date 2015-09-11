#ifndef __TCC_VPU_INTERFACE_H__
#define __TCC_VPU_INTERFACE_H__

#include "TCC_VPU_CODEC.h"

#define VDEC_VPU_INDEX_MUTEX	1    			//	1	// INDEX MUTEX

#define VDEC_DISP_INFO_PTS_MODE	0	// Presentation Timestamp (Display order)
#define VDEC_DISP_INFO_DTS_MODE	1	// Decode Timestamp (Decode order)

#define VDEC_DISP_INFO_INIT		0
#define VDEC_DISP_INFO_UPDATE	1
#define VDEC_DISP_INFO_GET		2
#define VDEC_DISP_INFO_RESET	3

#define VDEC_SKIP_FRAME_DISABLE		0	//!< Skip disable
#define VDEC_SKIP_FRAME_EXCEPT_I	1	//!< Skip except I(IDR) picture
#define VDEC_SKIP_FRAME_ONLY_B		2	//!< Skip B picture(skip if nal_ref_idc==0 in H.264)
#define VDEC_SKIP_FRAME_UNCOND		3	//!< Unconditionally Skip one picture
//------------------------------------------------------
// 1. Sequence header
//------------------------------------------------------
#define ATTACH_SEQ_HEADER_FOR_VPU_DEC 	1// FIXME

typedef struct dec_disp_info_ctrl_t {
	int		m_iTimeStampType;	//TS(Timestamp) type (0: Presentation TS(default), 1:Decode TS)
	int		m_iStdType;			//STD type

	int		m_iUsedIdxPTS;		//total number of decoded index for PTS
	int		m_iRegIdxPTS[32];	//decoded index for PTS
	void	*m_pRegInfoPTS[32];	//side information of the decoded index for PTS

	int		m_iDecodeIdxDTS;	//stored DTS index of decoded frame
	int		m_iDispIdxDTS;		//display DTS index of DTS array
	int		m_iDTS[32];			//Decode Timestamp (decoding order)

	int		m_Reserved;
} dec_disp_info_ctrl_t;

typedef struct dec_disp_info_t {
	int m_iFrameType;			//Frame Type

	int m_iTimeStamp;			//Time Stamp
	int m_iRvTimeStamp;			//TR(RV)

	int m_iPicStructure;		//PictureStructure
	int m_iM2vFieldSequence;	//Field sequence(MPEG2) 

	int m_iFrameSize;
} dec_disp_info_t;

typedef struct dec_disp_info_input_t {
	int m_iFrameIdx;			//Display frame buffer index for VDEC_DISP_INFO_UPDATE command
								//Decoded frame buffer index for VDEC_DISP_INFO_GET command
	int m_iStdType;				//STD type for VDEC_DISP_INFO_INIT
	int m_iTimeStampType;		//TS(Timestamp) type (0: Presentation TS(default), 1:Decode TS) for VDEC_DISP_INFO_INIT
	int m_Reserved;
} dec_disp_info_input_t;

typedef struct rmff_frame_t{
	int Current_TR;
	int Previous_TR;
	int Current_time_stamp;
	int Previous_time_stamp;
} rmff_frame_t;

typedef struct rmff_frame_time_t {
	rmff_frame_t  ref_frame;		
	rmff_frame_t  frame_P1;
	rmff_frame_t  frame_P2;
} rmff_frame_time_t;


//#define FRAME_BUFFER_CLEAR_DELAY	3	//Frame buffer clear test
#ifdef FRAME_BUFFER_CLEAR_DELAY
#	if FRAME_BUFFER_CLEAR_DELAY < 3
#		undef FRAME_BUFFER_CLEAR_DELAY
#		define FRAME_BUFFER_CLEAR_DELAY 3
#	endif
#endif


//#define MP4_TEST1
//#define MP4_TEST2

//#define API_CR 
#define VIDEO_SIZE_MAX ( ((1920+64)*(1088+64)/4)*6 )

//extern int tcc_vpu_codec_init(void);
//extern int tcc_vpu_codec_deinit(void);
//extern int tcc_vpu_codec_decode(int bitstream_format, unsigned char *p_input, unsigned int input_len, unsigned char *p_output, unsigned int *output_len);
//extern int tcc_vpu_codec_get_RV_timestamp(void);
//extern unsigned int tcc_get_vpu_phyaddress(unsigned int *size);

#endif//__TCC_VPU_INTERFACE_H__
