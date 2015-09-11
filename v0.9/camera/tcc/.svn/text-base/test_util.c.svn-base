#include <stdio.h>
#include <stdlib.h>

#if ( _MSC_VER >= 1400 ) //
	#pragma warning(disable:4996) //: _CRT_SECURE_NO_WARNINGS
	#pragma warning(disable: 4311 4312)
#endif

void 
yuvi_to_yuv420( unsigned char** ppY, unsigned char** ppCb, unsigned char** ppCr, unsigned char* pDst,
			    int picWidth, int picHeight, int stride )
{
	int y, nY, nCb, nCr/*, bufferWidth, bufferHeight*/;
	int addr;
	unsigned char* puc;

	if( !ppY || !ppCb || !ppCr )
		return;

	nY = picHeight;
	nCb = nCr = picHeight/2;

	puc = pDst;
	addr = (int)*ppY;

	if( picWidth == stride ) // for fast read
	{
		unsigned char t0,t1,t2,t3,t4,t5,t6,t7;
		int i;
		unsigned char * pTemp;
		unsigned char * dstAddrCb;
		unsigned char * dstAddrCr;

		memcpy( puc, addr, ( picWidth * picHeight ) );

		addr = (int)*ppCb;
		dstAddrCb = (unsigned char*)(puc + picWidth * picHeight);
		dstAddrCr = (unsigned char*)(dstAddrCb + picWidth/2 * picHeight/2);

		pTemp = malloc(picWidth);
		if (!pTemp) {
			printf( "malloc() failed \n");
		}

		for (y = 0; y < nY / 2; ++y) 
		{
			memcpy( pTemp, addr + stride * y, picWidth );
			for (i = 0; i < picWidth; i += 8) {
				t0 = pTemp[i];
				t1 = pTemp[i + 1];
				t2 = pTemp[i + 2];
				t3 = pTemp[i + 3];
				t4 = pTemp[i + 4];
				t5 = pTemp[i + 5];
				t6 = pTemp[i + 6];
				t7 = pTemp[i + 7];
				*dstAddrCb++ = t0;
				*dstAddrCb++ = t2;
				*dstAddrCb++ = t4;
				*dstAddrCb++ = t6;
				*dstAddrCr++ = t1;
				*dstAddrCr++ = t3;
				*dstAddrCr++ = t5;
				*dstAddrCr++ = t7;
			}
		}
		free(pTemp);
	}
	else
	{
		int i;
		unsigned char * pTemp;
		unsigned char * dstAddrCb;
		unsigned char * dstAddrCr;

		for (y = 0; y < nY; ++y) {
			memcpy( puc + y * picWidth, addr + stride * y, ((picWidth+7)/8)*8 );
		}

		addr = (int)*ppCb;
		stride = stride;
		dstAddrCb = (unsigned char *)(puc + picWidth * picHeight);
		dstAddrCr = (unsigned char *)(dstAddrCb + picWidth/2 * picHeight/2);

		pTemp = malloc((picWidth + 7) /8 *8);
		if (!pTemp) {
			printf( "malloc() failed \n");
		}
		for (y = 0; y < nY / 2; y++) {
			memcpy( pTemp, addr + stride * y, (picWidth + 7) /8 *8 );
			for (i = 0; (i < picWidth); i+=2) {
				*dstAddrCb++ = pTemp[(i/8)*8 + (i%8)];
				*dstAddrCr++ = pTemp[(i/8)*8 + (i%8) + 1];
			}
		}
		free(pTemp);
	}

	*ppY  = pDst;
	*ppCb = *ppY  + stride * picHeight;
	*ppCr = *ppCb + stride/2 * picHeight/2;

}


static void 
plane_copy( unsigned char* pDest, int DestStride, 
		   unsigned char* pSour, int SourStride, int SourWidth, int SourHeight )
{
	do 
	{
		memcpy( pDest, pSour, SourWidth );
		pDest += DestStride;
		pSour += SourStride;
	} while( --SourHeight );
}
static void 
yuv420_to_yuv420_crop( unsigned char* pInpY, unsigned char* pInpCb, unsigned char* pInpCr,
					   int picWidth, int picHeight, int stride, int CropLeft, int CropRight, int CropTop, int CropBottom,
                       unsigned char* pDest, int* pDestWidth, int* pDestHeight )
{
	unsigned char* p_sour[3];
	unsigned char* p_dest[3];

	int inp_width  = picWidth;
	int inp_height = picHeight;
	int out_width  = picWidth;
	int out_height = picHeight;

	p_sour[0] = pInpY;
	p_sour[1] = pInpCb;
	p_sour[2] = pInpCr;

	if( CropLeft || CropRight || CropTop || CropBottom )
	{
		p_sour[0] = pInpY  + ( CropTop   * stride   + CropLeft   );
		p_sour[1] = pInpCb + ( CropTop/2 * stride/2 + CropLeft/2 );
		p_sour[2] = pInpCr + ( CropTop/2 * stride/2 + CropLeft/2 );

		inp_width  = inp_width  - ( CropLeft + CropRight  );
		inp_height = inp_height - ( CropTop  + CropBottom );
		out_width  = inp_width;
		out_height = inp_height;
	}

	*pDestWidth  = out_width;
	*pDestHeight = out_height;


	p_dest[0] = pDest;
	p_dest[1] = p_dest[0] + out_width	* out_height;
	p_dest[2] = p_dest[1] + out_width/2 * out_height/2;

	plane_copy( p_dest[0], out_width,   p_sour[0], stride  , inp_width,   inp_height   );
	plane_copy( p_dest[1], out_width/2, p_sour[1], stride/2, inp_width/2, inp_height/2 );
	plane_copy( p_dest[2], out_width/2, p_sour[2], stride/2, inp_width/2, inp_height/2 );
}

int 
save_yuv_image( void *yuvFp, unsigned char* pOutYuv, 
			    unsigned char* pInpY,
				unsigned char* pInpCb,
				unsigned char* pInpCr,
				int picWidth,
				int picHeight,
				int stride,
				int CropLeft, int CropRight, int CropTop, int CropBottom,
				unsigned int uiCbCrInterleave )
{
	int dest_width = 0;
	int dest_height = 0;

	if( !pInpY || !pInpCb || !pInpCr )
		return 1;

	if( uiCbCrInterleave )
	{
		//printf( "save_yuv_image2\n");
		yuvi_to_yuv420( &pInpY, &pInpCb, &pInpCr, pOutYuv, picWidth, picHeight, stride );
		if( !CropLeft && !CropRight && !CropTop && !CropBottom )
		{
			dest_width = stride;
			dest_height = picHeight;
			goto SAVE_YUV_END;
		}
	}

	yuv420_to_yuv420_crop( pInpY, pInpCb, pInpCr, picWidth, picHeight, stride, CropLeft, CropRight, CropTop, CropBottom,
		pOutYuv, &dest_width, &dest_height );

SAVE_YUV_END:
	if( !fwrite(pOutYuv, sizeof(unsigned char), dest_width * dest_height * 3 / 2 , yuvFp) ) 
	{
		printf( "picWidth = %d, picHeight = %d, stride = %d \n", picWidth, picHeight, stride );
		printf( "rcCrop.left %d|| rcCrop.top %d|| rcCrop.bottom %d|| rcCrop.right %d\n", CropLeft, CropTop,CropBottom, CropRight );
		printf( "Frame Data fwrite failed file handle is 0x%x, width = %d, height = %d \n", yuvFp, dest_width, dest_height );
		return 0;			
	}
	return 1;
} 

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
conf_dec_t gConfDec[20] = {0,};
int
read_dec_conf_file( char* pConfFileName )
{
	char temp[MAX_FILE_PATH];
	char inp_path[MAX_FILE_PATH];
	char out_path[MAX_FILE_PATH];
	char file_name[MAX_FILE_PATH];

	int i, total_test_number, start_number;
	FILE* fp_conf_file;

	if( ( fp_conf_file = fopen(pConfFileName, "rb") ) == 0 )
	{
		printf( "file open error = %s\r\n", pConfFileName );
		return 0;
	}

	fscanf( fp_conf_file, "%s %s %d", temp, temp, &start_number );
	printf( "start_number = %d\r\n", start_number);

	fscanf( fp_conf_file, "%s %s %d", temp, temp, &total_test_number );
	printf( "total_test_number = %d\r\n", total_test_number);

	fscanf( fp_conf_file, "%s %s %s", temp, temp, inp_path );
	fscanf( fp_conf_file, "%s %s %s", temp, temp, out_path );

	// skip
	for(i = 0; i < start_number-1; i++) 
	{
		// input file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// output file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// original file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// total frame number
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// m_bStoreOutFile
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// m_bCompareFile
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// m_iDeblkEnable
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
	}

	for(i = 0; i < total_test_number; i++) 
	{
		// input file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConfDec[i].m_InpFileName, "%s/%s", inp_path, file_name );
		printf( "[%2d] %s\n", i+1, gConfDec[i].m_InpFileName );

		// output file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConfDec[i].m_OutFileName, "%s/%s", out_path, file_name );
		printf( "- Output File: %s\n", gConfDec[i].m_OutFileName );

		// original file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConfDec[i].m_RefFileName, "%s/%s", inp_path, file_name );

		// total frame number
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfDec[i].m_iTotalFrames);
		printf( "- Total frames: %d\n", gConfDec[i].m_iTotalFrames );

		// m_bStoreOutFile
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfDec[i].m_bStoreOutFile);
		printf( "- Store out file: %d\n", gConfDec[i].m_bStoreOutFile );

		// m_bCompareFile
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfDec[i].m_bCompareFile);
		printf( "- Compare file: %d\n", gConfDec[i].m_bCompareFile );

		// m_iDeblkEnable
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfDec[i].m_iDeblkEnable);
		printf( "- Deblocking Enable: %d\n", gConfDec[i].m_iDeblkEnable );
	}

	fclose( fp_conf_file );

	return total_test_number;
}

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
conf_enc_t gConfEnc[20] = {0,};

int read_enc_conf_file( char* pConfFileName )
{
	char temp[MAX_FILE_PATH];
	char inp_path[MAX_FILE_PATH];
	char out_path[MAX_FILE_PATH];
	char file_name[MAX_FILE_PATH];

	int i, total_test_number, start_number;
	FILE* fp_conf_file;

	if( ( fp_conf_file = fopen(pConfFileName, "rb") ) == 0 )
	{
		printf( "file open error\r\n" );
		return 0;
	}

	fscanf( fp_conf_file, "%s %s %d", temp, temp, &start_number );
	printf( "start_number = %d\r\n", start_number);

	fscanf( fp_conf_file, "%s %s %d", temp, temp, &total_test_number );
	printf( "total_test_number = %d\r\n", total_test_number);

	fscanf( fp_conf_file, "%s %s %s", temp, temp, inp_path );
	fscanf( fp_conf_file, "%s %s %s", temp, temp, out_path );

	// skip
	for(i = 0; i < start_number-1; i++) 
	{
		// input file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// output file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// TotalFrames
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// Width
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// Height
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// FramesPerSecond
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// TargetKbps
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// KeyInterval
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// BitFormat
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
	}

	for(i = 0; i < total_test_number; i++) 
	{
		// input file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConfEnc[i].m_InpFileName, "%s/%s", inp_path, file_name );
		printf( "[%2d] %s\n", i+1, gConfEnc[i].m_InpFileName );

		// output file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConfEnc[i].m_OutFileName, "%s/%s", out_path, file_name );
		printf( "- Output File: %s\n", gConfEnc[i].m_OutFileName );

		// total frame number
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfEnc[i].m_iTotalFrames);
		printf( "- Total frames: %d\n", gConfEnc[i].m_iTotalFrames );

		// Width
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfEnc[i].m_iWidth);
		printf( "- Width: %d\n", gConfEnc[i].m_iWidth );

		// Height
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfEnc[i].m_iHeight);
		printf( "- Height: %d\n", gConfEnc[i].m_iHeight );

		// FramesPerSecond
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfEnc[i].m_iFramesPerSecond);
		printf( "- FramesPerSecond: %d\n", gConfEnc[i].m_iFramesPerSecond );

		// TargetKbps
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfEnc[i].m_iTargetKbps);
		printf( "- TargetKbps: %d\n", gConfEnc[i].m_iTargetKbps );

		// KeyInterval
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfEnc[i].m_iKeyInterval);
		printf( "- KeyInterval: %d\n", gConfEnc[i].m_iKeyInterval );

		// BitFormat
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfEnc[i].m_iBitFormat);
		printf( "- KeyInterval: %d\n", gConfEnc[i].m_iBitFormat );
	}

	fclose( fp_conf_file );

	return total_test_number;
}

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

conf_multi_dec_t gConfMultiDec[4];
int
read_multi_dec_conf_file( char* pConfFileName )
{
	char temp[MAX_FILE_PATH];
	char inp_path[MAX_FILE_PATH];
	char out_path[MAX_FILE_PATH];
	char file_name[MAX_FILE_PATH];

	int i, total_test_number, start_number;
	FILE* fp_conf_file;

	if( ( fp_conf_file = fopen(pConfFileName, "rb") ) == 0 )
	{
		printf( "file open error = %s\r\n", pConfFileName );
		return 0;
	}

	fscanf( fp_conf_file, "%s %s %d", temp, temp, &start_number );
	printf( "start_number = %d\r\n", start_number);

	fscanf( fp_conf_file, "%s %s %d", temp, temp, &total_test_number );
	printf( "total_test_number = %d\r\n", total_test_number);

	fscanf( fp_conf_file, "%s %s %s", temp, temp, inp_path );
	fscanf( fp_conf_file, "%s %s %s", temp, temp, out_path );

	// skip
	for(i = 0; i < start_number-1; i++) 
	{
		// input file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// output file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// original file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// total frame number
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// m_bStoreOutFile
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// m_bCompareFile
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// m_iDeblkEnable
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// m_iBitFormat
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
	}

	for(i = 0; i < total_test_number; i++) 
	{
		// input file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConfMultiDec[i].m_InpFileName, "%s/%s", inp_path, file_name );
		printf( "[%2d] %s\n", i+1, gConfMultiDec[i].m_InpFileName );

		// output file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConfMultiDec[i].m_OutFileName, "%s/%s", out_path, file_name );
		printf( "- Output File: %s\n", gConfMultiDec[i].m_OutFileName );

		// original file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConfMultiDec[i].m_RefFileName, "%s/%s", inp_path, file_name );

		// total frame number
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfMultiDec[i].m_iTotalFrames);
		printf( "- Total frames: %d\n", gConfMultiDec[i].m_iTotalFrames );

		// m_bStoreOutFile
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfMultiDec[i].m_bStoreOutFile);
		printf( "- Store out file: %d\n", gConfMultiDec[i].m_bStoreOutFile );

		// m_bCompareFile
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfMultiDec[i].m_bCompareFile);
		printf( "- Compare file: %d\n", gConfMultiDec[i].m_bCompareFile );

		// m_iDeblkEnable
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfMultiDec[i].m_iDeblkEnable);
		printf( "- Deblocking Enable: %d\n", gConfMultiDec[i].m_iDeblkEnable );

		// m_iBitFormat
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConfMultiDec[i].m_iBitFormat);
		printf( "- m_iBitFormat: %d\n", gConfMultiDec[i].m_iBitFormat );
	}

	fclose( fp_conf_file );

	return total_test_number;
}


int
compare_i420_2( void* fpRefFile, 
			    unsigned char* pRef, unsigned char* pOutY, unsigned char* pOutU, unsigned char* pOutV,
				int OutStride, int Width, int Height, 
				int CropLeft, int CropRight, int CropTop, int CropBottom,
				int OutFrmNum )
{
	int x = 0, y = 0, i = 0, j = 0;
	int y_dim_mb = (Height+15)>>4, x_dim_mb = (Width+15)>>4;

	unsigned char* y1 = 0;
	unsigned char* u1 = 0;
	unsigned char* v1 = 0;
	unsigned char* y2 = 0;
	unsigned char* u2 = 0;
	unsigned char* v2 = 0;

	unsigned char* p_out_y = pOutY + CropLeft   + CropTop*OutStride;
	unsigned char* p_out_u = pOutU + CropLeft/2 + CropTop*OutStride/2;
	unsigned char* p_out_v = pOutV + CropLeft/2 + CropTop*OutStride/2;

	if( !pOutY || !pOutU || !pOutV )
		return;

	fseek( fpRefFile, OutFrmNum*(Width*Height>>2)*6, SEEK_SET );
	fread( pRef, 1, (Width*Height>>2)*6, fpRefFile );

	for(y = 0; y < y_dim_mb; y++) 
	{
		for(x = 0; x < x_dim_mb; x++) 
		{
			y1 = pRef + 16*x + 16*y*Width;
			u1 = pRef + Width*Height + 8*x + 4*y*Width;
			v1 = pRef + (Width*Height>>2)*5 + 8*x + 4*y*Width;

			y2 = p_out_y + 16*x + 16*y*OutStride;
			u2 = p_out_u +  8*x +  4*y*OutStride;
			v2 = p_out_v +  8*x +  4*y*OutStride;
			for(i = 0; i < 16; i++)
			{
				if( 16*y + i>= Height )
					continue;

				for(j = 0; j < 16; j++)
				{
					if( 16*x + j >= Width )
						continue;
					if(y1[j] != y2[j])
					{
						j = j;
						printf(" ERROR BLOCK[Y] is MB_X=%3d, MB_Y=%3d, ref=%3d, out=%3d ", x, y, y1[j], y2[j] );
						return -1;
					}
				}
				y1 += Width;
				y2 += OutStride;
			}
			for(i = 0; i < 8; i++)
			{
				if( 8*y + i>= Height/2 )
					continue;

				for(j = 0; j < 8; j++)
				{
					if( 8*x + j >= Width/2 )
						continue;

					if(u1[j] != u2[j])
					{
						j = j;
						printf(" ERROR BLOCK[U] is MB_X=%3d, MB_Y=%3d, ref=%3d, out=%3d ", x, y, u1[j], u2[j] );
						return -1;
					}
				}
				u1 += (Width>>1);
				u2 += (OutStride>>1);
			}
			for(i = 0; i < 8; i++)
			{
				if( 8*y + i>= Height/2 )
					continue;

				for(j = 0; j < 8; j++)
				{
					if( 8*x + j >= Width/2 )
						continue;

					if(v1[j] != v2[j])
					{
						j = j;
						printf(" ERROR BLOCK[V] is MB_X=%3d, MB_Y=%3d, ref=%3d, out=%3d ", x, y, v1[j], v2[j] );
						return -1;
					}
				}
				v1 += (Width>>1);
				v2 += (OutStride>>1);
			}
		}
	}

	return 0;
}

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
vpu_conf_t gConf[4];
int read_conf_file( char* pConfFileName )
{
	char temp[MAX_FILE_PATH];
	char inp_path[MAX_FILE_PATH];
	char out_path[MAX_FILE_PATH];
	char file_name[MAX_FILE_PATH];

	int i, total_test_number, start_number;
	FILE* fp_conf_file;

	if( ( fp_conf_file = fopen(pConfFileName, "rb") ) == 0 )
	{
		printf( "file open error\r\n" );
		return 0;
	}

	fscanf( fp_conf_file, "%s %s %d", temp, temp, &start_number );
	printf( "start_number = %d\r\n", start_number);

	fscanf( fp_conf_file, "%s %s %d", temp, temp, &total_test_number );
	printf( "total_test_number = %d\r\n", total_test_number);

	fscanf( fp_conf_file, "%s %s %s", temp, temp, inp_path );
	fscanf( fp_conf_file, "%s %s %s", temp, temp, out_path );

	// skip
	for(i = 0; i < start_number-1; i++) 
	{
		// input file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// output file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// output file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// TotalFrames
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// Width
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// Height
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// FramesPerSecond
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// TargetKbps
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// KeyInterval
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// BitFormat
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// 
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
		// 
		fscanf(fp_conf_file, "%s %s %s", temp, temp, temp);
	}

	for(i = 0; i < total_test_number; i++) 
	{
		// input file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConf[i].m_InpYuvFileName, "%s/%s", inp_path, file_name );
		printf( "[%2d] %s\n", i+1, gConf[i].m_InpYuvFileName );

		// output file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConf[i].m_EncOutFileName, "%s/%s", out_path, file_name );
		printf( "- Output File: %s\n", gConf[i].m_EncOutFileName );

		// output file name
		fscanf(fp_conf_file, "%s %s %s", temp, temp, file_name);
		sprintf( gConf[i].m_DecOutFileName, "%s/%s", out_path, file_name );
		printf( "- Output File: %s\n", gConf[i].m_DecOutFileName );

        // total frame number
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConf[i].m_iTotalFrames);
		printf( "- Total frames: %d\n", gConf[i].m_iTotalFrames );

		// Width
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConf[i].m_iWidth);
		printf( "- Width: %d\n", gConf[i].m_iWidth );

		// Height
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConf[i].m_iHeight);
		printf( "- Height: %d\n", gConf[i].m_iHeight );

		// FramesPerSecond
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConf[i].m_iFramesPerSecond);
		printf( "- FramesPerSecond: %d\n", gConf[i].m_iFramesPerSecond );

		// TargetKbps
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConf[i].m_iTargetKbps);
		printf( "- TargetKbps: %d\n", gConf[i].m_iTargetKbps );

		// KeyInterval
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConf[i].m_iKeyInterval);
		printf( "- KeyInterval: %d\n", gConf[i].m_iKeyInterval );

		// BitFormat
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConf[i].m_iBitFormat);
		printf( "- KeyInterval: %d\n", gConf[i].m_iBitFormat );

		// m_bEncStoreOutFile
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConf[i].m_bEncStoreOutFile);
		printf( "- EncStore: %d\n", gConf[i].m_bEncStoreOutFile );

		// m_bDecStoreOutFile
		fscanf(fp_conf_file, "%s %s %d", temp, temp, &gConf[i].m_bDecStoreOutFile);
		printf( "- DecStore: %d\n", gConf[i].m_bDecStoreOutFile );
	}

	fclose( fp_conf_file );

	return total_test_number;
}