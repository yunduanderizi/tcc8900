#ifndef _YVV_TO_JPG_H_
#define _YVV_TO_JPG_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */

#include "jpeglib.h"

#define OUTPUT_BUF_SIZE  4096
#define WIDTH  640//720
#define HEIGHT 480//475

typedef struct {
	struct jpeg_destination_mgr pub;
  	JOCTET * buffer; 
  	unsigned char *outbuffer;
  	int outbuffer_size;
  	unsigned char *outbuffer_cursor;
  	int *written; 
}mjpg_destination_mgr;

typedef mjpg_destination_mgr *mjpg_dest_ptr;

METHODDEF(void) init_destination(j_compress_ptr cinfo);
METHODDEF(boolean) empty_output_buffer(j_compress_ptr cinfo);
METHODDEF(void) term_destination(j_compress_ptr cinfo); 

void dest_buffer(j_compress_ptr cinfo, unsigned char *buffer, int size, int *written);
int compress_yuyv_to_jpeg(unsigned char *buf, unsigned char *buffer, int size, int quality); 

int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

#endif
