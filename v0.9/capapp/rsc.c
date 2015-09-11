/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               Camera    I n t e r f a c e    M O D U L E

                        EDIT HISTORY FOR MODULE

when        who       what, where, why
--------    ---       -------------------------------------------------------
10/xx/08   ZzaU      Created file.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "main.h"
#include "tcc/tccfb_ioctl.h"
#include <linux/fb.h>


/*===========================================================================
FUNCTION
===========================================================================*/
#define FB0_SIZE (8*1024*1024)
void rsc_black_lcd(CameraDevice* self)
{
    static struct fb_fix_screeninfo fb_fix;
    static struct fb_var_screeninfo fb_info;

#if 1
	typedef struct _ImageInfoType {
		unsigned int ImageFmt;
		unsigned int pos_x;
		unsigned int pos_y;
		unsigned int width;
		unsigned int height;
	} ImageInfoType;

	ImageInfoType ImgInfo;
#endif

    ioctl(self->fb_fd0, FBIOGET_FSCREENINFO, &fb_fix);
    ioctl(self->fb_fd0, FBIOGET_VSCREENINFO, &fb_info);
	phyFbAddr=fb_fix.smem_start;

	bq_printf("LCD mode: %d, Preview: %d x %d\n", self->rt_mode, self->preview_width, self->preview_height);

	ImgInfo.width 	= VIEW_WIDTH;
	ImgInfo.height 	= VIEW_HEIGHT;

	printf("fb_info.xres:%d\n fb_info.yres:%d\n ImgInfo.width:%d\n ImgInfo.height:%d\n", fb_info.xres, fb_info.yres, ImgInfo.width, ImgInfo.height);
	ImgInfo.pos_x 	= (fb_info.xres > ImgInfo.width)  ? (fb_info.xres-ImgInfo.width)/2  : 2;
	ImgInfo.pos_y 	= (fb_info.yres > ImgInfo.height) ? (fb_info.yres-ImgInfo.height)/2 : 2;
	

	ImgInfo.ImageFmt = IMGFMT_YUV422P;

	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_FORMAT,&ImgInfo.ImageFmt);
	//usleep(20*1000);
	usleep(10);


    unsigned short *p;
	int fb_size = fb_fix.smem_len;
	char *frame_buffer=NULL;
    
    if ((frame_buffer = (char *) mmap(0, fb_size, PROT_READ | PROT_WRITE,
    	MAP_SHARED, self->fb_fd0, 0)) == (char *) -1) 
    {
        printf("Can't mmap /dev/fb0 %s\n", strerror(errno));
        return -1;
    }
    p= frame_buffer;

	int i = 0;
    for(i=0; i<fb_info.xres* fb_info.yres; i++)
    {
        *p++ = 128<<8;
    }
    p= frame_buffer+FB0_SIZE/2;

    for(i=0; i<fb_info.xres* fb_info.yres; i++)
    {
        *p++ = 128<<8;
    }        

}

void rsc_init_lcd(CameraDevice* self, unsigned int view_mode)
{
#if defined(TCC83XX_CODE)

	int lcd_fb;
	DImageInfoType ImgInfo;
    static struct fb_fix_screeninfo fb_fix;
    static struct fb_var_screeninfo fb_info;

    ioctl(self->fb_fd0, FBIOGET_FSCREENINFO, &fb_fix);
    ioctl(self->fb_fd0, FBIOGET_VSCREENINFO, &fb_info);

	if(view_mode == 0) //icon_mode
	{
		self->rt_mode = ROTATE_90;
		self->preview_width  = fb_info.xres;
		self->preview_height = fb_info.yres;
	}
	else
	{
		self->rt_mode = NOOP;
		self->preview_width  = fb_info.xres;
		self->preview_height = (fb_info.xres/4) *3;
	}

	bq_printf("LCD mode: %d, Preview: %d x %d \n", self->rt_mode, self->preview_width, self->preview_height);


	// Set LCD_Info!!
	if(self->rt_mode == ROTATE_90 || self->rt_mode == ROTATE_270)
	{
		ImgInfo.width 	= self->preview_height;
		ImgInfo.height 	= self->preview_width;
	}
	else
	{
		ImgInfo.width 	= self->preview_width;
		ImgInfo.height 	= self->preview_height;
	}
	ImgInfo.pos_x 	= (fb_info.xres-ImgInfo.width)/2;
	ImgInfo.pos_y 	= (fb_info.yres-ImgInfo.height)/2;
	
	ImgInfo.ImageFmt = FMT_YUV422SP;
	ioctl(self->fb_fd0, TCC83X_LCDFB_IOCTL_INFO_SET,&ImgInfo);

	if(self->rt_mode)
		rsc_set_lcd_addr(self, TCC8300_CAPTURED_IMAGE_BASE+(self->preview_width*self->preview_height*(NUM_VIDBUF+1)*2));
	else
		rsc_set_lcd_addr(self, TCC8300_CAPTURED_IMAGE_BASE);

	// Set Other-Channel!!
    lcd_fb = open("/dev/fb1", O_RDWR);
	ioctl(lcd_fb,TCC83X_LCDFB_IOCTL_CHANNEL_EN, DC_DISABLE);
	close(lcd_fb);

    lcd_fb = open("/dev/fb2", O_RDWR);
	ioctl(lcd_fb,TCC83X_LCDFB_IOCTL_CHANNEL_EN, DC_DISABLE);
	close(lcd_fb);
	
#elif defined(TCC8900_CODE)

    static struct fb_fix_screeninfo fb_fix;
    static struct fb_var_screeninfo fb_info;

#if 1
	typedef struct _ImageInfoType {
		unsigned int ImageFmt;
		unsigned int pos_x;
		unsigned int pos_y;
		unsigned int width;
		unsigned int height;
	} ImageInfoType;

	ImageInfoType ImgInfo;
#endif

    ioctl(self->fb_fd0, FBIOGET_FSCREENINFO, &fb_fix);
    ioctl(self->fb_fd0, FBIOGET_VSCREENINFO, &fb_info);
	phyFbAddr=fb_fix.smem_start;
//	if(view_mode == 0) //icon_mode
//	{
//		self->rt_mode = ROTATE_90;
//		self->preview_width  = fb_info.xres;
//		self->preview_height = fb_info.yres;
//	}
//	else
//	{
//		self->rt_mode = NOOP;
//		self->preview_width  = fb_info.xres;
//		self->preview_height = (fb_info.xres/4) *3;
//	}
//	self->preview_width  = fb_info.xres;
//	self->preview_height = fb_info.yres;

	bq_printf("LCD mode: %d, Preview: %d x %d\n", self->rt_mode, self->preview_width, self->preview_height);


	// Set LCD_Info!!
//	if(self->rt_mode == ROTATE_90 || self->rt_mode == ROTATE_270)
//	{
//		ImgInfo.width 	= self->preview_height;
//		ImgInfo.height 	= self->preview_width;
//	}
//	else
//	{
		ImgInfo.width 	= VIEW_WIDTH;//self->preview_width;
		ImgInfo.height 	= VIEW_HEIGHT;//self->preview_height;
//	}
	printf("fb_info.xres:%d\n fb_info.yres:%d\n ImgInfo.width:%d\n ImgInfo.height:%d\n", fb_info.xres, fb_info.yres, ImgInfo.width, ImgInfo.height);
	ImgInfo.pos_x 	= (fb_info.xres > ImgInfo.width)  ? (fb_info.xres-ImgInfo.width)/2  : 2;
	ImgInfo.pos_y 	= (fb_info.yres > ImgInfo.height) ? (fb_info.yres-ImgInfo.height)/2 : 0;
	
#ifdef USE_PIX_FMT_YUV420
	ImgInfo.ImageFmt = IMGFMT_YUV420;
	//ImgInfo.ImageFmt = IMGFMT_YUV422P;
#else
	ImgInfo.ImageFmt = IMGFMT_YUV422;
#endif
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_IMGWINDOW,&ImgInfo.pos_x);
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_FORMAT,&ImgInfo.ImageFmt);
	usleep(20*100);

//	if(self->rt_mode)
//		rsc_set_lcd_addr(self, TCC8900_CAPTURED_IMAGE_BASE+(self->preview_width*self->preview_height*(NUM_VIDBUF+1)*2));
//	else
//		rsc_set_lcd_addr(self, TCC8900_CAPTURED_IMAGE_BASE);

	#if 0
	// Set Other-Channel!!
	{
		int lcd_fb;
		unsigned int onoff;
		onoff = 0;
	    lcd_fb = open("/dev/fb1", O_RDWR);
		ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
		close(lcd_fb);

	    lcd_fb = open("/dev/fb2", O_RDWR);
		ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
		close(lcd_fb);
	}
	#endif

#endif
}

/*===========================================================================
FUNCTION
===========================================================================*/
void rsc_set_lcd_ch0(CameraDevice* self, unsigned char en)
{
#if defined(TCC83XX_CODE)
	if(self->fb_fd0 >= 0)
	{
		if(en)
			ioctl(self->fb_fd0,TCC83X_LCDFB_IOCTL_CHANNEL_EN, DC_ENABLE);
		else
			ioctl(self->fb_fd0,TCC83X_LCDFB_IOCTL_CHANNEL_EN, DC_DISABLE);
	}
#elif defined(TCC8900_CODE)
	unsigned int onoff = 0;
	if(self->fb_fd0 >= 0)
	{
		if(en)
		{
			// Set Other-Channel!!
			{
				int lcd_fb;
				unsigned int onoff;
				onoff = 0;
			    //lcd_fb = open("/dev/fb1", O_RDWR);
				//ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
				//close(lcd_fb);

			    lcd_fb = open("/dev/fb2", O_RDWR);
				ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
				close(lcd_fb);
			}

			onoff = 1;
			ioctl(self->fb_fd0,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
		}
		else
		{
			onoff = 0;
			ioctl(self->fb_fd0,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);

			// Set Other-Channel!!
			{
				int lcd_fb;
				unsigned int onoff;
				onoff = 1;
			    //lcd_fb = open("/dev/fb1", O_RDWR);
				//ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
				//close(lcd_fb);

			    lcd_fb = open("/dev/fb2", O_RDWR);
				ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
				close(lcd_fb);
			}

		}
	}
#endif
}

/*===========================================================================
FUNCTION
===========================================================================*/
void rsc_set_lcd_addr(CameraDevice* self, unsigned int addr)
{
#if defined(TCC83XX_CODE)
	DImageAddressType Imgaddr;

	Imgaddr.UseDefaultAddress = 0;
	Imgaddr.addressY = (char*)addr;
	Imgaddr.addressU = (char*)Imgaddr.addressY + (self->preview_width*self->preview_height);
	Imgaddr.addressV = (char*)Imgaddr.addressU + (self->preview_width*self->preview_height/2);

	self->outbuf = (void*)Imgaddr.addressY;
	self->mem_len = self->preview_width*self->preview_height*2;
	ioctl(self->fb_fd0, TCC83X_LCDFB_IOCTL_ADDRESS_SET, &Imgaddr);

#elif defined(TCC8900_CODE)

	unsigned int base[3] = {0, 0, 0};

#ifdef USE_PIX_FMT_YUV420
	base[0] = addr;
	base[1] = base[0] + VIEW_WIDTH*VIEW_HEIGHT_MODIFY;
	base[2] = base[1] + VIEW_WIDTH*VIEW_HEIGHT_MODIFY/4; 

	self->outbuf = (void*)base[0];
	self->mem_len = (self->preview_width*self->preview_height*3)/2;
#else
	base[0] = addr;
	base[1] = base[0] +  VIEW_WIDTH*VIEW_HEIGHT;//(self->preview_width*self->preview_height);
	base[2] = base[1] +  VIEW_WIDTH*VIEW_HEIGHT/2;//(self->preview_width*self->preview_height/2);
// printf("y=0x%x,u=0x%x,v=0x%x\n",base[0],base[1],base[2]);
	self->outbuf = (void*)base[0];
	self->mem_len =  VIEW_WIDTH*VIEW_HEIGHT*2;//self->preview_width*self->preview_height*2;
#endif
	
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_BASEADDR, base);

#endif
}

/*===========================================================================
FUNCTION
===========================================================================*/
void rsc_draw_lcd(CameraDevice* self)
{
#ifdef TCC83XX_CODE
	ioctl(self->fb_fd0, TCC83X_LCDFB_IOCTL_DRAW, 0);
#endif
}

/*===========================================================================
FUNCTION
===========================================================================*/
void rsc_process_image(CameraDevice *self, uint8_t *source)
{
#ifdef TCC83XX_CODE
	G2D_COMMON_TYPE grp_arg;

	grp_arg.reponsetype = G2D_POLLING;
	grp_arg.src0		= (unsigned int)source;
    grp_arg.srcfm 		= GE_YUV422_sp;
    grp_arg.src_imgx 	= self->preview_width;
    grp_arg.src_imgy 	= self->preview_height;
	grp_arg.DefaultBuffer = 0;

	grp_arg.tgt0		= (unsigned int)self->outbuf;
	grp_arg.tgtfm 		= GE_YUV422_sp;
    grp_arg.ch_mode 	= self->rt_mode;

	ioctl(self->rt_fd, TCC8300_GRP_COMMON_IOCTRL, &grp_arg);
#endif
}

/*===========================================================================
FUNCTION
===========================================================================*/
void  rsc_encode_jpeg(CameraDevice *self)
{
#ifdef TCC83XX_CODE
	int jpeg_fd;
	
	if(self->cap_info.jpeg_save_info.bitstream_size == 0)
	{
		bq_printf("YUV Captured -> \n");
		
		jpeg_fd = open("/dev/jpeg",O_RDWR);
		
		self->cap_info.jpeg_save_info.src 	 = (void*)TCC8300_CAPTURED_IMAGE_BASE;
		self->cap_info.jpeg_save_info.width  = self->cap_info.width;
		self->cap_info.jpeg_save_info.height = self->cap_info.height;
		ioctl(jpeg_fd,TCC8300_JPEG_IOCTL_V4L2_ENC, &(self->cap_info.jpeg_save_info));

		close(jpeg_fd);

		bq_printf("JPEG -> \n");
	}
#endif
}

/*===========================================================================
FUNCTION
===========================================================================*/
void  rsc_save_file(CameraDevice *self, unsigned short* filename)
{
#ifdef TCC83XX_CODE
	int saved_fd;
	char *map_addr, *jpeg_buffer;

	map_addr = mmap(NULL,TCC8300_JPEG_FULL_MMAP_SIZE,PROT_READ|PROT_WRITE, MAP_SHARED,self->fd,TCC8300_CAPTURED_IMAGE_BASE);
	jpeg_buffer = map_addr + TCC8300_JPEG_MMAP_SIZE;		
	
	saved_fd = fi_open(filename, O_CREAT|O_WRONLY);
	if(saved_fd >= 0)
	{
		fi_write(saved_fd,jpeg_buffer+self->cap_info.jpeg_save_info.bitstream_size,self->cap_info.jpeg_save_info.header_size);
		fi_write(saved_fd,jpeg_buffer,self->cap_info.jpeg_save_info.bitstream_size);
		fi_close(saved_fd);
	}
	
	munmap(map_addr, TCC8300_JPEG_FULL_MMAP_SIZE);
	
	bq_printf(" %d KB (%d / %d) Saved!!\n", (self->cap_info.jpeg_save_info.bitstream_size + self->cap_info.jpeg_save_info.header_size)/1024,
									self->cap_info.jpeg_save_info.header_size/1024, self->cap_info.jpeg_save_info.bitstream_size/1024);
#endif
}


/*===========================================================================
FUNCTION
===========================================================================*/
void rsc_sched_delay(int ms)
{
	usleep(ms*1000);
}

void Set_DisplayPosition(CameraDevice *self, unsigned int display_scr_width, unsigned int display_scr_height,unsigned int display_dst_width, unsigned int display_dst_height,
						unsigned int check_multiple,unsigned int SizediffofW,unsigned int SizediffofH, unsigned int src_addr )
{
	unsigned int 	fb_window[4];
	unsigned int	fb_addroffset[2];
	int 			address[15];
//	 static struct fb_fix_screeninfo fb_fix;
    static struct fb_var_screeninfo tcc89fb_vinfo;

 //   ioctl(self->fb_fd0, FBIOGET_FSCREENINFO, &fb_fix);
    ioctl(self->fb_fd0, FBIOGET_VSCREENINFO, &tcc89fb_vinfo);

	if(check_multiple)
	{
		fb_window[2] =  display_scr_width-SizediffofW; 
		fb_window[3] =  display_scr_height-SizediffofH;
		fb_window[0] =(tcc89fb_vinfo.xres - fb_window[2]) >>1;// (display_dst_width-fb_window[2])>>1; 
		fb_window[1] =(tcc89fb_vinfo.yres - fb_window[3]) >>1;// (display_dst_height-fb_window[3])>>1; 

		/*Set Stride Offset*/
		fb_addroffset[0] = display_scr_width<<1;
		fb_addroffset[1] = display_scr_width;
	}
	else
	{
		fb_window[2] = display_scr_width; 
		fb_window[3] = display_scr_height;
//		fb_window[0] = (display_dst_width-fb_window[2])>>1; 
//		fb_window[1] = (display_dst_height-fb_window[3])>>1; 
		fb_window[0] =(tcc89fb_vinfo.xres - fb_window[2]) >>1;// (display_dst_width-fb_window[2])>>1; 
		fb_window[1] =(tcc89fb_vinfo.yres - fb_window[3]) >>1;// (display_dst_height-fb_window[3])>>1; 

		/*Set Stride Offset*/
		fb_addroffset[0] = 0;
		fb_addroffset[1] = 0;
	}
	address[0] = src_addr;
	address[1] = 0;
	address[2] = 0;
	//if (memcmp(fb_window_pre, fb_window, sizeof(unsigned int)*4) || memcmp(fb_addroffset_pre, fb_addroffset, sizeof(unsigned int)*2) )
	{
	int 	param = IMGFMT_YUV422P;
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_FORMAT, 	&param); 
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_IMGWINDOW, fb_window); 

	if(check_multiple)
		ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_ADDROFFSET, fb_addroffset); 

//		memcpy(fb_window_pre, fb_window, sizeof(unsigned int)*4);
//		memcpy(fb_addroffset_pre, fb_addroffset, sizeof(unsigned int)*2);
	}
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_BASEADDR, address); 	
}

