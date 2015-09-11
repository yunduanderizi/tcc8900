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
#include "camapp.h"


/*===========================================================================
FUNCTION
===========================================================================*/
typedef struct _ImageInfoType {
	unsigned int ImageFmt;
	unsigned int pos_x;
	unsigned int pos_y;
	unsigned int width;
	unsigned int height;
}ImageInfoType;


ImageInfoType ImgInfo_lcd_bak;
unsigned int bak_lcd_base[3] = {0, 0, 0};

int bak_lcd_init(CameraDevice* self)
{
	memset(&ImgInfo_lcd_bak, 0, sizeof(ImageInfoType));


	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_GET_BASEADDR, bak_lcd_base);	
    ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_GET_FORMAT,&ImgInfo_lcd_bak.ImageFmt);
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_GET_IMGWINDOW,&ImgInfo_lcd_bak.pos_x);
	fprintf(stdout, "bak finish...base = %x, x = %d, y = %d, w = %d, h = %d, fmt = %d\n", 
		bak_lcd_base[0], ImgInfo_lcd_bak.pos_x, ImgInfo_lcd_bak.pos_y, ImgInfo_lcd_bak.width, ImgInfo_lcd_bak.height, ImgInfo_lcd_bak.ImageFmt);
	return 0;
}

int recovery_lcd_init(CameraDevice* self)
{
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_BASEADDR, bak_lcd_base);
        ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_FORMAT,&ImgInfo_lcd_bak.ImageFmt);
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_IMGWINDOW,&ImgInfo_lcd_bak.pos_x);
	fprintf(stdout, "recovery finish...base = %x, x = %d, y = %d, w = %d, h = %d, fmt = %d\n", 
		bak_lcd_base[0], ImgInfo_lcd_bak.pos_x, ImgInfo_lcd_bak.pos_y, ImgInfo_lcd_bak.width, ImgInfo_lcd_bak.height, ImgInfo_lcd_bak.ImageFmt);
	return 0;
}

void rsc_init_lcd(CameraDevice* self, unsigned int view_mode)
{
	static struct fb_fix_screeninfo fb_fix;
	static struct fb_fix_screeninfo fb1_fix;
	static struct fb_fix_screeninfo fb2_fix;

	
	static struct fb_var_screeninfo fb_info;
	
	ImageInfoType ImgInfo;
    	
	ioctl(self->fb_fd0, FBIOGET_FSCREENINFO, &fb_fix);
	ioctl(self->fb_fd0, FBIOGET_VSCREENINFO, &fb_info);
	
	phyFbAddr=fb_fix.smem_start;/* Start of frame buffer mem *//* (physical address) */
	fprintf(stdout, "--------------------------->phyFbAddr = 0x%x\n", phyFbAddr);
	/*获得fb0的虚拟地址*/
	virtualFb0Addr = phyFbAddr + VIRTUAL_FB0_GAIN_SIZE;
#if 0	
	virtualFb1Addr = phyFbAddr + VIRTUAL_FB1_GAIN_SIZE;
	virtualFb2Addr = phyFbAddr + VIRTUAL_FB2_GAIN_SIZE;

	int lcd_fb = 0;
	lcd_fb = open("/dev/fb1", O_RDWR);
	ioctl(lcd_fb, FBIOGET_FSCREENINFO, &fb1_fix);
	virtualFb1Addr = fb1_fix.smem_start;	
	fprintf(stdout, "--------------------------->phyFb1Addr = 0x%x\n", virtualFb1Addr);
	close(lcd_fb);

    	lcd_fb = open("/dev/fb2", O_RDWR);
	ioctl(lcd_fb, FBIOGET_FSCREENINFO, &fb2_fix);
	virtualFb2Addr = fb2_fix.smem_start;	
	fprintf(stdout, "--------------------------->phyFb2Addr = 0x%x\n", virtualFb2Addr);
	close(lcd_fb);
#endif	 
	ImgInfo.width 	= VIEW_WIDTH;//self->preview_width;
	ImgInfo.height 	= VIEW_HEIGHT;//self->preview_height;
	ImgInfo.pos_x 	= (fb_info.xres > ImgInfo.width)  ? (fb_info.xres-ImgInfo.width)/2  : 0;
	ImgInfo.pos_y 	= (fb_info.yres > ImgInfo.height) ? (fb_info.yres-ImgInfo.height)/2 : 0;

	/*设置frambuffer的图像格式和起始地址*/
#ifdef USE_PIX_FMT_YUV420
	ImgInfo.ImageFmt = IMGFMT_YUV420;
#else
	ImgInfo.ImageFmt = IMGFMT_YUV422;
#endif
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_IMGWINDOW,&ImgInfo.pos_x);
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_FORMAT,&ImgInfo.ImageFmt);
	
	rsc_set_lcd_addr(self, virtualFb0Addr);	
}

/*===========================================================================
FUNCTION
===========================================================================*/
void rsc_set_lcd_ch0(CameraDevice* self, unsigned char en)
{
	if(self->fb_fd0 >= 0)
	{
		unsigned int onoff;
		int lcd_fb;
		if(en)
		{
			// Set Other-Channel!!
			onoff = 0;
			lcd_fb = open("/dev/fb1", O_RDWR);//close fb1
			ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
			close(lcd_fb);

			lcd_fb = open("/dev/fb2", O_RDWR);//close fb2
			ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
			close(lcd_fb);

			onoff = 1;//open fb0
			ioctl(self->fb_fd0,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
		}
		else
		{
			onoff = 1;//open fb0
			ioctl(self->fb_fd0,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);

			// Set Other-Channel!!

			onoff = 1;//open fb1
			lcd_fb = open("/dev/fb1", O_RDWR);
			ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
			close(lcd_fb);
		
			onoff = 1;//open fb2
			lcd_fb = open("/dev/fb2", O_RDWR);
			ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
			close(lcd_fb);
		}
	}
}

/*===========================================================================
FUNCTION
===========================================================================*/
void rsc_set_lcd_addr(CameraDevice* self, unsigned int addr)
{
	unsigned int base[3] = {0, 0, 0};

#ifdef USE_PIX_FMT_YUV420
	base[0] = addr;
	base[1] = base[0] + (self->preview_width*self->preview_height);
	base[2] = base[1] + (self->preview_width*self->preview_height/4);

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
}

/*===========================================================================
FUNCTION
===========================================================================*/
void rsc_draw_lcd(CameraDevice* self)
{
#ifdef TCC83XX_CODE
	ioctl(self->fb_fd0, TCC83X_LCDFB_IOCTL_DRAW, 0);
#endif
	return;
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
		DBug_printf("YUV Captured -> \n");
		
		jpeg_fd = open("/dev/jpeg",O_RDWR);
		
		self->cap_info.jpeg_save_info.src 	 = (void*)TCC8300_CAPTURED_IMAGE_BASE;
		self->cap_info.jpeg_save_info.width  = self->cap_info.width;
		self->cap_info.jpeg_save_info.height = self->cap_info.height;
		ioctl(jpeg_fd,TCC8300_JPEG_IOCTL_V4L2_ENC, &(self->cap_info.jpeg_save_info));

		close(jpeg_fd);

		DBug_printf("JPEG -> \n");
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
	
	DBug_printf(" %d KB (%d / %d) Saved!!\n", (self->cap_info.jpeg_save_info.bitstream_size + self->cap_info.jpeg_save_info.header_size)/1024,
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

