#include "yuv2jpg.h"

#include <jpeglib.h>
#include <jerror.h>

#if 0
METHODDEF(void) init_destination(j_compress_ptr cinfo) 
{
  	mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;
  	dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE, OUTPUT_BUF_SIZE * sizeof(JOCTET));
  	*(dest->written) = 0;
  	dest->pub.next_output_byte = dest->buffer;
  	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

METHODDEF(boolean) empty_output_buffer(j_compress_ptr cinfo)
{
  	mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;
  	memcpy(dest->outbuffer_cursor, dest->buffer, OUTPUT_BUF_SIZE);
  	dest->outbuffer_cursor += OUTPUT_BUF_SIZE;
  	*(dest->written) += OUTPUT_BUF_SIZE;
  	dest->pub.next_output_byte = dest->buffer;
  	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
  	return TRUE;
}

METHODDEF(void) term_destination(j_compress_ptr cinfo) 
{
  	mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;
  	size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;
  	/* Write any data remaining in the buffer */
  	memcpy(dest->outbuffer_cursor, dest->buffer, datacount);
  	dest->outbuffer_cursor += datacount;
  	*(dest->written) += datacount;
}
void dest_buffer(j_compress_ptr cinfo, unsigned char *buffer, int size, int *written)
{
  	mjpg_dest_ptr dest;
	
  	if (cinfo->dest == NULL)
	{
    	cinfo->dest = 
		(struct jpeg_destination_mgr *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, 
							sizeof(mjpg_destination_mgr));
  	}
	
  	dest = (mjpg_dest_ptr)cinfo->dest;
  	dest->pub.init_destination = init_destination;
  	dest->pub.empty_output_buffer = empty_output_buffer;
  	dest->pub.term_destination = term_destination;
	
  	dest->outbuffer = buffer;
  	dest->outbuffer_size = size;
  	dest->outbuffer_cursor = buffer;
  	dest->written = written;
}

//摄像头采集帧图像的YUYV格式转换为JPEG格式
int compress_yuyv_to_jpeg_suoxiao(unsigned char *buf, unsigned char *buffer, int width, int height, int quality) 
{
	  struct jpeg_compress_struct cinfo;
	  struct jpeg_error_mgr jerr;
	  JSAMPROW row_pointer[1];
	  unsigned char *line_buffer, *yuyv0, *yuyv01, *yuyv1, *yuyv2;
	  int z;
	  static int written;

	  line_buffer = (unsigned char *)calloc (width * 3, 1);
	  yuyv0 = buf;//将YUYV格式的图片数据赋给YUYV指针	
	  yuyv1 = buf + width*height;
	  yuyv2 = buf + width*height;
	  
	  cinfo.err = jpeg_std_error (&jerr);
	  jpeg_create_compress (&cinfo);
	  
	  /* jpeg_stdio_dest (&cinfo, file); */
	  dest_buffer(&cinfo, buffer, width*height, &written);
	  cinfo.image_width = width;
	  cinfo.image_height = height;
	  cinfo.input_components = 3;
	  cinfo.in_color_space = JCS_RGB;
	  
	  jpeg_set_defaults (&cinfo);
	  jpeg_set_quality (&cinfo, quality, TRUE);
	  jpeg_start_compress (&cinfo, TRUE);
	  z = 0;
	  
	  while (cinfo.next_scanline < height) 
	  {
	  	int x;
	    unsigned char *ptr = line_buffer;
	    for (x = 0; x < width; x+=2)
		{		
			int r, g, b;
	      	int y, u, v;

			y = *yuyv0;
			u = *yuyv1;
			v = *yuyv2;
			
	      	r =  y + 0.01 *(v - 128);
	      	g =  y - 0.1 *(u - 128) - 0.4 *(v - 128);
	      	b =  y + 10.0 *(u - 128);
		
	      	*(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
	      	*(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
	      	*(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

			yuyv0++;
			y = *yuyv0;
			
	      	r =  y + 0.01 *(v - 128);
	      	g =  y - 0.1 *(u - 128) - 0.4 *(v - 128);
	      	b =  y + 10.0 *(u - 128);
		
	      	*(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
	      	*(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
	      	*(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

			yuyv0 ++;
			yuyv1 ++;
			yuyv2 ++;
	
	    } 
		
	    row_pointer[0] = line_buffer;
	    jpeg_write_scanlines (&cinfo, row_pointer, 1);
		
	  }
	  
	  jpeg_finish_compress (&cinfo);
	  jpeg_destroy_compress (&cinfo);
	  free (line_buffer);
	  
	  return (written);
}

//摄像头采集帧图像的YUYV格式转换为JPEG格式
int compress_420p_to_jpeg_suoxiao(unsigned char *buf, unsigned char *buffer, int width, int height, int quality) 
{
	  struct jpeg_compress_struct cinfo;
	  struct jpeg_error_mgr jerr;
	  JSAMPROW row_pointer[1];
	  unsigned char *line_buffer, *yuyv0, *yuyv01, *yuyv1, *yuyv2;
	  int z;
	  static int written;

	  line_buffer = (unsigned char *)calloc (width * 3, 1);
	  yuyv0 = buf;//将YUYV格式的图片数据赋给YUYV指针	
	  yuyv1 = buf + width*height;
	  yuyv2 = yuyv1+ width*height/4;
	  
	  cinfo.err = jpeg_std_error (&jerr);
	  jpeg_create_compress (&cinfo);
	  
	  /* jpeg_stdio_dest (&cinfo, file); */
	  dest_buffer(&cinfo, buffer, width*height, &written);
	  cinfo.image_width = width;
	  cinfo.image_height = height;
	  cinfo.input_components = 3;
	  cinfo.in_color_space = JCS_RGB;
	  
	  jpeg_set_defaults (&cinfo);
	  jpeg_set_quality (&cinfo, quality, TRUE);
	  jpeg_start_compress (&cinfo, TRUE);
	  z = 0;
	  
	  while (cinfo.next_scanline < height) 
	  {
	  	int x;
	    unsigned char *ptr = line_buffer;
	    for (x = 0; x < width; x+=2)
		{		
			int r, g, b;
	      	int y, u, v;

			y = *yuyv0;
			u = *yuyv1;
			v = *yuyv2;
			
	      	//r =  y + 0.01 *(v - 128);
	      	//g =  y - 0.1 *(u - 128) - 0.4 *(v - 128);
	      	//b =  y + 10.0 *(u - 128);

			r = (y << 8 + 359*(v-128)) >> 8;
			g = (y << 8 - 88*(u-128) - 183*(v -128)) >> 8;
			b = (y << 8 + 454*(u-128)) >> 8;
		
	      	*(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
	      	*(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
	      	*(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

			yuyv0++;
			y = *yuyv0;
			
	      	//r =  y + 0.01 *(v - 128);
	      	//g =  y - 0.1 *(u - 128) - 0.4 *(v - 128);
	      	//b =  y + 10.0 *(u - 128);
	      	r = (y << 8 + 359*(v-128)) >> 8;
			g = (y << 8 - 88*(u-128) - 183*(v -128)) >> 8;
			b = (y << 8 + 454*(u-128)) >> 8;
	      	
		
	      	*(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
	      	*(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
	      	*(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

			yuyv0 ++;
			yuyv1 ++;
			yuyv2 ++;
	
	    } 
		
	    row_pointer[0] = line_buffer;
	    jpeg_write_scanlines (&cinfo, row_pointer, 1);
		
	  }
	  
	  jpeg_finish_compress (&cinfo);
	  jpeg_destroy_compress (&cinfo);
	  free (line_buffer);
	  
	  return (written);
}

#else
typedef struct {
        struct jpeg_destination_mgr pub;
        JOCTET *buf;
        size_t bufsize;
        size_t jpegsize;
} mem_destination_mgr;

typedef mem_destination_mgr *mem_dest_ptr;


METHODDEF(void) init_destination(j_compress_ptr cinfo)
{
        mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;

        dest->pub.next_output_byte = dest->buf;
        dest->pub.free_in_buffer = dest->bufsize;
        dest->jpegsize = 0;
}

METHODDEF(boolean) empty_output_buffer(j_compress_ptr cinfo)
{
        mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;

        dest->pub.next_output_byte = dest->buf;
        dest->pub.free_in_buffer = dest->bufsize;

        return FALSE;
        ERREXIT(cinfo, JERR_BUFFER_SIZE);
}

METHODDEF(void) term_destination(j_compress_ptr cinfo)
{
        mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
        dest->jpegsize = dest->bufsize - dest->pub.free_in_buffer;
}

static GLOBAL(void) jpeg_mem_dest(j_compress_ptr cinfo, JOCTET* buf, size_t bufsize)
{
        mem_dest_ptr dest;

        if (cinfo->dest == NULL) {
                cinfo->dest = (struct jpeg_destination_mgr *)
                    (*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
                    sizeof(mem_destination_mgr));
        }

        dest = (mem_dest_ptr) cinfo->dest;

        dest->pub.init_destination    = init_destination;
        dest->pub.empty_output_buffer = empty_output_buffer;
        dest->pub.term_destination    = term_destination;

        dest->buf      = buf;
        dest->bufsize  = bufsize;
        dest->jpegsize = 0;
}

static GLOBAL(int) jpeg_mem_size(j_compress_ptr cinfo)
{
        mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
        return dest->jpegsize;
}


int put_jpeg_yuv420p_memory(unsigned char *dest_image, int image_size,
                            unsigned char *input_image, int width, int height, int quality)
{
	int i, j, jpeg_image_size;

	JSAMPROW y[16],cb[16],cr[16]; // y[2][5] = color sample of row 2 and pixel column 5; (one plane)
	JSAMPARRAY data[3]; // t[0][2][5] = color sample 0 of row 2 and column 5

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	data[0] = y;
	data[1] = cb;
	data[2] = cr;

	cinfo.err = jpeg_std_error(&jerr);  // errors get written to stderr 
	
	jpeg_create_compress(&cinfo);
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	jpeg_set_defaults (&cinfo);

	jpeg_set_colorspace(&cinfo, JCS_YCbCr);

	cinfo.raw_data_in = TRUE; // supply downsampled data
	cinfo.comp_info[0].h_samp_factor = 2;
	cinfo.comp_info[0].v_samp_factor = 2;
	cinfo.comp_info[1].h_samp_factor = 1;
	cinfo.comp_info[1].v_samp_factor = 1;
	cinfo.comp_info[2].h_samp_factor = 1;
	cinfo.comp_info[2].v_samp_factor = 1;

	jpeg_set_quality(&cinfo, quality, TRUE);
	cinfo.dct_method = JDCT_FASTEST;

	jpeg_mem_dest(&cinfo, dest_image, image_size);	// data written to mem
	
	jpeg_start_compress (&cinfo, TRUE);

	for (j=0; j<height; j+=16) {
		for (i=0; i<16; i++) {
			y[i] = input_image + width*(i+j);
			if (i%2 == 0) {
				cb[i/2] = input_image + width*height + width/2*((i+j)/2);
				cr[i/2] = input_image + width*height + width*height/4 + width/2*((i+j)/2);
			}
		}
		jpeg_write_raw_data(&cinfo, data, 16);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_image_size = jpeg_mem_size(&cinfo);
	jpeg_destroy_compress(&cinfo);
	
	return jpeg_image_size;
}
#endif

