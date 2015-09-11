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
#include "Camapp.h"


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

static ImageInfoType ImgInfo; //set fb0 for video
static ImageInfoType ImgInfo1; //set fb1 for video

ImageInfoType ImgInfo_lcd_bak_fb0;
unsigned int bak_lcd_base_fb0[3] = {0, 0, 0};

ImageInfoType ImgInfo_lcd_bak_fb1;
unsigned int bak_lcd_base_fb1[3] = {0, 0, 0};

int bak_lcd_init(CameraDevice* self)
{
	memset(&ImgInfo_lcd_bak_fb0, 0, sizeof(ImageInfoType));
	memset(&ImgInfo_lcd_bak_fb1, 0, sizeof(ImageInfoType));

	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_GET_BASEADDR, bak_lcd_base_fb0);
        ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_GET_FORMAT,&ImgInfo_lcd_bak_fb0.ImageFmt);
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_GET_IMGWINDOW,&ImgInfo_lcd_bak_fb0.pos_x);
	fprintf(stdout, "bak_fb0 finish...base = %x, x = %d, y = %d, w = %d, h = %d, fmt = %d\n", 
		bak_lcd_base_fb0[0], ImgInfo_lcd_bak_fb0.pos_x, ImgInfo_lcd_bak_fb0.pos_y, 
		ImgInfo_lcd_bak_fb0.width, ImgInfo_lcd_bak_fb0.height, ImgInfo_lcd_bak_fb0.ImageFmt);
	
	ioctl(self->fb_fd1, TCC_LCD_FB_IOCTL_GET_BASEADDR, bak_lcd_base_fb1);
        ioctl(self->fb_fd1, TCC_LCD_FB_IOCTL_GET_FORMAT,&ImgInfo_lcd_bak_fb1.ImageFmt);
	ioctl(self->fb_fd1, TCC_LCD_FB_IOCTL_GET_IMGWINDOW,&ImgInfo_lcd_bak_fb1.pos_x);
	fprintf(stdout, "bak_fb1 finish...base = %x, x = %d, y = %d, w = %d, h = %d, fmt = %d\n", 
		bak_lcd_base_fb1[0], ImgInfo_lcd_bak_fb1.pos_x, ImgInfo_lcd_bak_fb1.pos_y, 
		ImgInfo_lcd_bak_fb1.width, ImgInfo_lcd_bak_fb1.height, ImgInfo_lcd_bak_fb1.ImageFmt);

	return 0;
}

int recovery_lcd_init(CameraDevice* self)
{
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_BASEADDR, bak_lcd_base_fb0);
        ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_FORMAT,&ImgInfo_lcd_bak_fb0.ImageFmt);
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_IMGWINDOW,&ImgInfo_lcd_bak_fb0.pos_x);
	fprintf(stdout, "recovery_fb0 finish...base = %x, x = %d, y = %d, w = %d, h = %d, fmt = %d\n", 
		bak_lcd_base_fb0[0], ImgInfo_lcd_bak_fb0.pos_x, ImgInfo_lcd_bak_fb0.pos_y,
		 ImgInfo_lcd_bak_fb0.width, ImgInfo_lcd_bak_fb0.height, ImgInfo_lcd_bak_fb0.ImageFmt);
	
	ioctl(self->fb_fd1, TCC_LCD_FB_IOCTL_SET_BASEADDR, bak_lcd_base_fb1);
        ioctl(self->fb_fd1, TCC_LCD_FB_IOCTL_SET_FORMAT,&ImgInfo_lcd_bak_fb1.ImageFmt);
	ioctl(self->fb_fd1, TCC_LCD_FB_IOCTL_SET_IMGWINDOW,&ImgInfo_lcd_bak_fb1.pos_x);
	fprintf(stdout, "recovery_fb1 finish...base = %x, x = %d, y = %d, w = %d, h = %d, fmt = %d\n", 
		bak_lcd_base_fb1[0], ImgInfo_lcd_bak_fb1.pos_x, ImgInfo_lcd_bak_fb1.pos_y,
		 ImgInfo_lcd_bak_fb1.width, ImgInfo_lcd_bak_fb1.height, ImgInfo_lcd_bak_fb1.ImageFmt);
	return 0;
}

#define FB0_SIZE ((8*1024*1024) - (4*1024*1024))
#define FB0_SIZE_OFFSET (4*1024*1024)
#define FB1_SIZE (4*1024*1024)  //(2*1920*1080)
#define FB1_SIZE_H (2*1024*1024)
#define FB1_SIZE_OFFSET (2*1024*1024)

char *mediafb_0 = NULL;
char *mediafb_1 = NULL;
char *mediafbplay_1 = NULL;

void clear_fb(CameraDevice* self)
{
	unsigned short *ptr;
	unsigned char *ptr_play;
	unsigned char *ptr_fb0;
	
	static int dommap = 0;
	if(dommap == 0){
		mediafb_0 = (char *)mmap(0, FB0_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, self->fb_fd0, FB0_SIZE_OFFSET);
		if(mediafb_0 == NULL)
			fprintf(stdout, "mmap mediafb_0 error------------->\n", virtualFb0Addr);

		mediafb_1 = (char *)mmap(0, FB1_SIZE_H, PROT_READ | PROT_WRITE, MAP_SHARED, self->fb_fd1, 0);	
		if(mediafb_1 == NULL)
			fprintf(stdout, "mmap mediafb_1 error------------->\n", virtualFb1Addr);

		mediafbplay_1 = (char *)mmap(0, FB1_SIZE_H, PROT_READ | PROT_WRITE, MAP_SHARED, self->fb_fd1, FB1_SIZE_OFFSET);	
		if(mediafb_1 == NULL)
			fprintf(stdout, "mmap mediafb_1 error------------->\n", virtualFb1Addr);

		dommap = 1;
	}
	int i, j;
	
	ptr_fb0 = mediafb_0;
	for(i=0;i<ImgInfo.height * ImgInfo.width;i++)
			*ptr_fb0++=0x00;

	for(i=0;i<ImgInfo.height * ImgInfo.width / 2;i++)
			*ptr_fb0++=0x80;

	ptr = mediafb_1;
	for(i=0;i<ImgInfo1.height * ImgInfo1.width;i++)
			*ptr++=0x8000;
	
	ptr_play = mediafbplay_1;
	for(i=0;i<ImgInfo.height * ImgInfo.width;i++)
			*ptr_play++=0x00;
	for(i=0;i<ImgInfo.height * ImgInfo.width / 2;i++)
			*ptr_play++=0x80;

	fprintf(stdout, "clear fb0, fb1 finish...\n");

	return;
}

void rsc_init_lcd(CameraDevice* self, unsigned int view_mode)
{
	static struct fb_fix_screeninfo fb_fix;
	static struct fb_fix_screeninfo fb1_fix;
	static struct fb_fix_screeninfo fb2_fix;

	static struct fb_var_screeninfo fb_info;
	static struct fb_var_screeninfo fb1_info;


    	
	ioctl(self->fb_fd0, FBIOGET_FSCREENINFO, &fb_fix);
	ioctl(self->fb_fd0, FBIOGET_VSCREENINFO, &fb_info);
	
	ioctl(self->fb_fd1, FBIOGET_FSCREENINFO, &fb1_fix);
	ioctl(self->fb_fd1, FBIOGET_VSCREENINFO, &fb1_info);


	phyFbAddr=fb_fix.smem_start;
//	fprintf(stdout, "--------------------------->phyFbAddr = 0x%x\n", phyFbAddr);
	virtualFb0Addr = phyFbAddr + VIRTUAL_FB0_GAIN_SIZE;
	fprintf(stdout, "--------------------------->virtualFb0Addr = 0x%x\n", virtualFb0Addr);

	int lcd_fb = 0;
	lcd_fb = open("/dev/fb1", O_RDWR);
	ioctl(lcd_fb, FBIOGET_FSCREENINFO, &fb1_fix);
	virtualFb1Addr = fb1_fix.smem_start;	
	fprintf(stdout, "--------------------------->virtualFb1Addr = 0x%x\n", virtualFb1Addr);
	close(lcd_fb);

    	lcd_fb = open("/dev/fb2", O_RDWR);
	ioctl(lcd_fb, FBIOGET_FSCREENINFO, &fb2_fix);
	virtualFb2Addr = fb2_fix.smem_start;	
	fprintf(stdout, "--------------------------->virtualFb2Addr = 0x%x\n", virtualFb2Addr);
	close(lcd_fb);

	ImgInfo.width 	= VIEW_WIDTH;//self->preview_width;
	ImgInfo.height 	= VIEW_HEIGHT;//self->preview_height;
	ImgInfo.pos_x 	= POS_X;
	ImgInfo.pos_y 	= POS_Y;

	ImgInfo1.width 	= VIEW_WIDTH_IN;//self->preview_width;
	ImgInfo1.height	= VIEW_HEIGHT_IN;//self->preview_height;
	ImgInfo1.pos_x 	= POS_X_IN;
	ImgInfo1.pos_y 	= POS_Y_IN;

        ImgInfo.ImageFmt = IMGFMT_YUV420;
        ImgInfo1.ImageFmt = IMGFMT_YUV422P;           //FB1 need YUV422

	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_IMGWINDOW,&ImgInfo.pos_x);
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_FORMAT,&ImgInfo.ImageFmt);

        ioctl(self->fb_fd1, TCC_LCD_FB_IOCTL_SET_IMGWINDOW, &ImgInfo1.pos_x);
        ioctl(self->fb_fd1, TCC_LCD_FB_IOCTL_SET_FORMAT, &ImgInfo1.ImageFmt);

	rsc_set_lcd_addr(1, self, virtualFb0Addr);//fb is yuv 420
	rsc_set_lcd_addr(0, self, virtualFb1Addr);//fb1 is yuv 422

	return;
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
			onoff = 1;
			unsigned char mask[3]={255,255,255};
			unsigned int value[3]={0,0,0};
			lcd_fb = open("/dev/fb2", O_RDWR);
			ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
			ioctl(lcd_fb, TCC_LCD_FB_IOCTL_CHROMAKEY_SET_MASK, mask);
			ioctl(lcd_fb, TCC_LCD_FB_IOCTL_CHROMAKEY_SET_VALUE, value);
			ioctl(lcd_fb, TCC_LCD_FB_IOCTL_CHROMAKEY_ONOFF, &onoff);
			close(lcd_fb);
#if 0
			onoff = 1;
			lcd_fb = open("/dev/fb2", O_RDWR);//close fb2
			ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
			close(lcd_fb);
#endif		
			onoff = 1;
			lcd_fb = open("/dev/fb1", O_RDWR);//close fb1
			ioctl(lcd_fb,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
			close(lcd_fb);

			onoff = 1;//open fb0
			ioctl(self->fb_fd0,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);
		}
		else
		{
			onoff = 0;//open fb0
			ioctl(self->fb_fd0,TCC_LCD_FB_IOCTL_DISP_ONOFF, &onoff);

			// Set Other-Channel!!

			onoff = 0;//open fb1
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
void rsc_set_lcd_addr(int flagyuv420, CameraDevice* self, unsigned int addr)
{
	unsigned int base[3] = {0, 0, 0};

	if(flagyuv420)
	{
		base[0] = addr;
		base[1] = base[0] + (VIEW_WIDTH * VIEW_HEIGHT);
		base[2] = base[1] + ((VIEW_WIDTH * VIEW_HEIGHT) / 4);

		self->outbuf = (void*)base[0];
		self->mem_len = (VIEW_WIDTH * VIEW_HEIGHT * 3)/2;
		ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_SET_BASEADDR, base);
	}
	else
	{
		base[0] = addr;
		base[1] = 0;//(self->preview_width*self->preview_height);
		base[2] = 0;//(self->preview_width*self->preview_height/2);
		ioctl(self->fb_fd1, TCC_LCD_FB_IOCTL_SET_BASEADDR, base);
	}	
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

