#include "yuv2jpg.h"

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

