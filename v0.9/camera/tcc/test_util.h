#ifndef _TEST_UTIL_H_
#define _TEST_UTIL_H_

//#define RAW_BITSTREAM_TEST

#if ( _MSC_VER >= 1400 ) //
	#pragma warning(disable:4996) //: _CRT_SECURE_NO_WARNINGS
	#pragma warning(disable: 4311 4312)
#endif

int
compare_i420_2( void* fpRefFile, 
			    unsigned char* pRef, unsigned char* pOutY, unsigned char* pOutU, unsigned char* pOutV,
				int OutStride, int Width, int Height, 
				int CropLeft, int CropRight, int CropTop, int CropBottom,
				int OutFrmNum );

int 
save_yuv_image( void *yuvFp, unsigned char* pOutYuv, 
			    unsigned char* pInpY,
				unsigned char* pInpCb,
				unsigned char* pInpCr,
				int picWidth,
				int picHeight,
				int stride,
				int CropLeft, int CropRight, int CropTop, int CropBottom,
				unsigned int uiCbCrInterleave );

void 
yuvi_to_yuv420( unsigned char** ppY, unsigned char** ppCb, unsigned char** ppCr, unsigned char* pDst,
			    int picWidth, int picHeight, int stride );


#define MAX_FILE_PATH 256

typedef struct conf_dec_t
{
	char m_InpFileName[MAX_FILE_PATH];
	char m_OutFileName[MAX_FILE_PATH];
	char m_RefFileName[MAX_FILE_PATH];
	int m_iTotalFrames;
	int m_bStoreOutFile;
	int m_bCompareFile;
	int m_iDeblkEnable;
} conf_dec_t;

extern conf_dec_t gConfDec[4];
int 
read_dec_conf_file( char* pConfFileName );

typedef struct conf_enc_t
{
	char m_InpFileName[MAX_FILE_PATH];
	char m_OutFileName[MAX_FILE_PATH];
	int m_iTotalFrames;
	int m_iWidth;
	int m_iHeight;
	int m_iFramesPerSecond;
	int m_iTargetKbps;
	int m_iKeyInterval;
	int m_iBitFormat;
} conf_enc_t;
extern conf_enc_t gConfEnc[4];
int read_enc_conf_file( char* pConfFileName );

typedef struct vpu_conf_t
{
	char m_InpYuvFileName[MAX_FILE_PATH];
	char m_EncOutFileName[MAX_FILE_PATH];
	char m_DecOutFileName[MAX_FILE_PATH];
	int m_iTotalFrames;
	int m_iWidth;
	int m_iHeight;
	int m_iFramesPerSecond;
	int m_iTargetKbps;
	int m_iKeyInterval;
	int m_iBitFormat;
	int m_bEncStoreOutFile;
	int m_bDecStoreOutFile;
}vpu_conf_t;
extern vpu_conf_t gConf[4];
int read_conf_file( char* pConfFileName );


typedef struct conf_multi_dec_t
{
	char m_InpFileName[MAX_FILE_PATH];
	char m_OutFileName[MAX_FILE_PATH];
	char m_RefFileName[MAX_FILE_PATH];
	int m_iTotalFrames;
	int m_bStoreOutFile;
	int m_bCompareFile;
	int m_iDeblkEnable;
	int m_iBitFormat;
} conf_multi_dec_t;

extern conf_multi_dec_t gConfMultiDec[4];
int 
read_multi_dec_conf_file( char* pConfFileName );

#endif//_TEST_UTIL_H_