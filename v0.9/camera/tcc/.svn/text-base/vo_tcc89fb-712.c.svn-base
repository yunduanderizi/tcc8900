/* 
 *  vo_tcc89fb.c
 *
 *	Copyright (C) Telechips - 08/2009
 *
 *  This file is part of MPlayer, a free movie player.
 *	
 *  MPlayer is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  MPlayer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License along  
 *  with MPlayer; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <pthread.h>


#include "config.h"
#include "video_out.h"
#include "video_out_internal.h"
#include "aspect.h"
#include "mp_msg.h"
#include "help_mp.h"
#include "tccfb_ioctl.h"
#include "fastmemcpy.h"

#include "libmpcodecs/vf.h" //ken add

//#include "uie.h" //ken add

static vo_info_t info = 
{
	"Framebuffer Device",
	"tcc89fb",
	"Telechips",
	""
};

// defined in video_out_internal.h
LIBVO_EXTERN(tcc89fb)	


long int screensize;
char *mediafb=NULL;
int phyfbmem;
//int test_phy;
//char *test_virt;
char *mediafbY;
char *mediafbU;
char *mediafbV;
//int tcc89fb_mem_fd=-1;
char tcc89mscflag;
char tcc89viqelag;
static char *tcc89fb_dev_name = NULL; 
static int tcc89fb_dev_fd;
static struct fb_fix_screeninfo tcc89fb_finfo;
//static struct fb_var_screeninfo tcc89fb_orig_vinfo;
static struct fb_var_screeninfo tcc89fb_vinfo;

static int tcc89fb_xres;
static int tcc89fb_yres;
static uint32_t tcc89img_fmt; // The real input format
static uint32_t tcc89src_width, tcc89src_height, tcc89src_fmt, tcc89src_bpp;
static uint32_t tcc89dst_width, tcc89dst_height;
static uint32_t input_src_width,input_src_height,out_w_offset, out_h_offset;
static int check_res_mutiple = 0;
static unsigned int sizediff_of_W =0;
static unsigned int sizediff_of_H = 0; // difference of original size with scaler input size

static uint32_t tcc89buffer_size; // Max size
static uint32_t tcc89buffer_stride[3];
static int fs;
static void (*draw_alpha_fnc) (int x0, int y0, int w, int h,
                               unsigned char *src, unsigned char *srca,
                               int stride,char *str);//ken add
//static int osd_changed;
static int g_fb1_fd=-1;
FILE *test_fd=NULL;
//static int test_cnt=0;
static unsigned char *mem_fb1=NULL;
#define FB0_SIZE (8*1024*1024)
#define FB1_SIZE (2*1920*1080)
#define					ZOOM_MOVE_STEP		32
#define				 	ZOOM_STEP_X			32*2
#define					ZOOM_STEP_Y			24*2
#define	FB_WDMAX		1920
#define	FB_HTMAX		1080

static void fb1_init()
{
 	int fmt,ret;
	int on=1;
	unsigned char *p;
	struct fb_var_screeninfo fbvar;   
	struct fb_fix_screeninfo fbfix;
//	unsigned int parms[4] = {0, 0, 0, 0};
	
 	g_fb1_fd=open("/dev/fb1",O_RDWR);
	if(g_fb1_fd<0)
	{
		return;
	}
	
	
//	mem_fb1=(char *)mmap(NULL, fb1_size, PROT_READ | PROT_WRITE, MAP_SHARED,g_fb1_fd, 0);
	ioctl(g_fb1_fd,TCC_LCD_FB_IOCTL_DISP_ONOFF,&on);
	ioctl(g_fb1_fd, TCC_LCD_FB_IOCTL_ALPHA_ONOFF, &on);
	ioctl(g_fb1_fd, TCC_LCD_FB_IOCTL_ALPHA_SELECTION, &on); //alpha,pixel
	
	printf("Set format to 4444\n");
	fmt=IMGFMT_RGB444;	
	ioctl(g_fb1_fd, TCC_LCD_FB_IOCTL_SET_FORMAT, &fmt);	 

	ret = ioctl(g_fb1_fd, FBIOGET_VSCREENINFO, &fbvar);   
	ret = ioctl(g_fb1_fd, FBIOGET_FSCREENINFO, &fbfix);   

#if 0
	printf("x res      : %d\n", fbvar.xres);   
	printf("y res      : %d\n", fbvar.yres);   
	printf("x res virt : %d\n", fbvar.xres_virtual);   
	printf("y res virt : %d\n", fbvar.yres_virtual);   
	printf("bpp        : %d\n", fbvar.bits_per_pixel);   
	printf("base       : %x\n", fbfix.smem_start);   
	printf("buffer mem len : %d\n", fbfix.smem_len);   
#endif	
	// read before set it...is it a bug of frame buffer?
	ioctl(g_fb1_fd, FBIOGET_VSCREENINFO, &fbvar);   
	ioctl(g_fb1_fd, FBIOGET_FSCREENINFO, &fbfix);   
	
	// set the size of OSD layer same to FB0
//	parms[0] = 0;
//	parms[1] = 0;
//	parms[2] = tcc89fb_vinfo.xres;
//	parms[3] = tcc89fb_vinfo.yres;
	fbvar.xres = tcc89fb_vinfo.xres;
	fbvar.yres = tcc89fb_vinfo.yres;
	fbvar.xres_virtual = tcc89fb_vinfo.xres;

	ioctl(g_fb1_fd, FBIOPUT_VSCREENINFO, &fbvar);
	printf("Set OSD resolution to %dx%d\n", tcc89fb_vinfo.xres, tcc89fb_vinfo.yres);
	ioctl(g_fb1_fd, FBIOGET_VSCREENINFO, &fbvar);   
	ioctl(g_fb1_fd, FBIOGET_FSCREENINFO, &fbfix);   
#if 0
	printf("---------- Set OSD resolution ------------\n");
	printf("x res      : %d\n", fbvar.xres);   
	printf("y res      : %d\n", fbvar.yres);   
	printf("x res virt : %d\n", fbvar.xres_virtual);   
	printf("y res virt : %d\n", fbvar.yres_virtual);   
	printf("bpp        : %d\n", fbvar.bits_per_pixel);   
	printf("base       : %x\n", fbfix.smem_start);   
	printf("buffer mem len : %d\n", fbfix.smem_len);   
#endif	
	mem_fb1=(char *)mmap(NULL, FB1_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,g_fb1_fd, 0);	
	memset(mem_fb1,0x0,FB1_SIZE);
	
	p = mem_fb1;
#if 1
	unsigned short c;
	unsigned char r,g,b,alpha;
	int i,j;
	r=0xff;
	g=0x0;
	b=0x0;
	alpha=0x7f;
	c = ((long)(r & 0xf0)) << 4;
	c |= ((long)(g) & 0xf0);
	c |= (b >> 4);
	c |= ((long)(alpha & 0xf0)) << 8;
		for (i = 0; i <480; i++)
	{
		for (j = 0; j <  800;j++)
		{
			*(p+j) = c;//c_pattern;
		}
		p += 800;
	}
	#endif
	}

static void fb1_deinit()
{
 	if(mem_fb1!=NULL)
 		{
 		munmap(mem_fb1, FB1_SIZE);
		mem_fb1=NULL;
 		}
	if(g_fb1_fd!=-1)
		{
		close(g_fb1_fd);
		g_fb1_fd=-1;
		}
}
void tc_lcd_set_center(int x,int y,int wd,int ht)
 {
	unsigned int fb_window[4];
	int       guifb;
	struct 	fb_var_screeninfo vscr_info;
	struct	fb_fix_screeninfo fscr_info;
			
	guifb = open ("/dev/fb0", O_RDWR);

	ioctl (guifb, FBIOGET_VSCREENINFO, &vscr_info);
	ioctl (guifb, FBIOGET_FSCREENINFO, &fscr_info);

	fb_window[0] = x; 
	fb_window[1] = y; 
	fb_window[2] = wd; 
	fb_window[3] = ht;
	ioctl(guifb, TCC_LCD_FB_IOCTL_SET_IMGWINDOW, fb_window); 

	//add_info[0]= wd;
	//add_info[1]= ht;
	//ioctl(guifb, TCC_LCD_FB_IOCTL_SET_IMGWINDOW, fb_window); 
	close(guifb);
}

static void tcc_vo_draw_alpha_rgb16(int w,int h, 
	unsigned char* src, unsigned char *srca, 
	int srcstride, unsigned char* dstbase,int dststride)
{
	int y;
	unsigned char *p_src = src;
	unsigned char *p_srca = srca;
	unsigned char *p_dst = dstbase;
		
	for(y = 0; y < h; y++)
	{
        unsigned short *dst = (unsigned short*) p_dst;
        register int x;
        for(x = 0;x < w; x++)
        {
			int alpha;
			int r;
			int g;
			int b;
			
			if (p_srca[x] != 0)
			{
				alpha = p_srca[x]>>4;
				r = g = b = (p_src[x]) >> 4;
				dst[x]=((0x0f-alpha)<<12)|(b<<8)|(g<<4)|(r);
			}
			else
			{
				dst[x]=0x0;
		}
			//r = g = b = 0x0f;
			//alpha = 0xf;
			//dst[x]=(r<<12)|(g<<8)|(b<<4)|(alpha);
			//printf("(%x,%x,%x,%x)->%x,\n", r,g,b, alpha, dst[x]); 
			
        }
        p_src += srcstride;
        p_srca += srcstride;
        p_dst += dststride;
    }
//	memset(opt,0x0,w*h*16);
    return;
}
static void writetomscfb(uint8_t *image[],  int src_w,int src_h,int dst_w,int dst_h)
{
	int 	gLCDC_KernelAddress;
	int param = IMGFMT_YUV422P;

	ioctl(tcc89fb_dev_fd, TCC_LCD_FB_IOCTL_SET_FORMAT, 	&param); 
	//M2M_ScalerForVideo_Enable(0);
	gLCDC_KernelAddress=phyfbmem;
	M2M_ScalerForVideo_With_Address(src_w, src_h, dst_w, dst_h, image[0], image[1], image[2], 
							gLCDC_KernelAddress, 0, 0,0);

}

//
// copy decoded frame data to frame buffer directly, without any resize
//
static void writetovideofb(uint8_t *image[], int stride[], int w,int h,int x,int y,int fmt)
{
	int format;

	uint8_t *srcPtr0= (char*)image[0];	
	uint8_t *srcPtr1= (char*)image[1];	
	uint8_t *srcPtr2= (char*)image[2];	
	int address[3];
	
	format =IMGFMT_YUV420;// IMGFMT_YUV420I0;//IMGFMT_YUV420;
	ioctl(tcc89fb_dev_fd, TCC_LCD_FB_IOCTL_SET_FORMAT, &format);
	tc_lcd_set_center(x,y,w,h);

	address[0] = srcPtr0;
	address[1] = srcPtr1;
	address[2] = srcPtr2;	
	
	ioctl(tcc89fb_dev_fd, TCC_LCD_FB_IOCTL_SET_BASEADDR, address); 	
}


static int draw_slice(uint8_t *image[], int stride[], int w,int h,int x,int y)
{
}

//
//	draw OSD
//
static void draw_osd(void)
{
{
      vo_draw_text( tcc89fb_vinfo.xres,  tcc89fb_vinfo.yres, draw_alpha_fnc);
      //memset(opt,0x0,w*h*16);
}
}

static void flip_page(void)
{
}

static int draw_frame(uint8_t *src[])
{
  	int stride[] = { tcc89buffer_stride[0], tcc89buffer_stride[1], tcc89buffer_stride[2]};
	printf("tcc89mscfb222:%d\n", tcc89mscflag);
	return draw_slice(src,stride,tcc89src_width, tcc89src_height,0,0);
}

static int query_format(uint32_t format)
{
  switch(format) 
  {
  case IMGFMT_BGR8:
  case IMGFMT_BGR15:
    return 0;
  case IMGFMT_YUY2:
  case IMGFMT_UYVY:
  case IMGFMT_YV12:
  case IMGFMT_I420:
  case IMGFMT_BGR16:
	return VFCAP_CSP_SUPPORTED | VFCAP_CSP_SUPPORTED_BY_HW | VFCAP_HWSCALE_UP | VFCAP_HWSCALE_DOWN |VFCAP_ACCEPT_STRIDE;
  }
  return 0;
}


void Set_DisplayPosition(unsigned int display_scr_width, unsigned int display_scr_height,unsigned int display_dst_width, unsigned int display_dst_height,
						unsigned int check_multiple,unsigned int SizediffofW,unsigned int SizediffofH, unsigned int src_addr )
{
	unsigned int 	fb_window[4];
	unsigned int	fb_addroffset[2];
	int 			address[15];

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

//	if (memcmp(fb_window_pre, fb_window, sizeof(unsigned int)*4) || memcmp(fb_addroffset_pre, fb_addroffset, sizeof(unsigned int)*2) )
	{
	int 	param = IMGFMT_YUV422P;
	ioctl(tcc89fb_dev_fd, TCC_LCD_FB_IOCTL_SET_FORMAT, 	&param); 
	ioctl(tcc89fb_dev_fd, TCC_LCD_FB_IOCTL_SET_IMGWINDOW, fb_window); 

	if(check_multiple)
		ioctl(tcc89fb_dev_fd, TCC_LCD_FB_IOCTL_SET_ADDROFFSET, fb_addroffset); 

	//	memcpy(fb_window_pre, fb_window, sizeof(unsigned int)*4);
	//	memcpy(fb_addroffset_pre, fb_addroffset, sizeof(unsigned int)*2);
	}
	ioctl(tcc89fb_dev_fd, TCC_LCD_FB_IOCTL_SET_BASEADDR, address); 	
}

void CalculateDisplaySize(unsigned int Src_Width,unsigned int Src_Height,unsigned int Org_Src_Width, unsigned int Org_Src_Height,
							unsigned int dst_width,unsigned int dst_height,unsigned int *check_multiple,unsigned int *Sizediff_W,unsigned int *Sizediff_H)
{
	float tmp_diffSizeW=0,tmp_diffSizeW_1=0;
	float tmp_diffSizeH=0,tmp_diffSizeH_1=0;
	float scale_val_w=0;
	float scale_val_h=0;
	float residue = 0;

	unsigned int tmp_diffSizeW_2 =0;
	unsigned int tmp_diffSizeH_2=0;
	
	//check the resolution of src contents if that is multiple of 16, check_res_mutiple = 0
	if(Src_Width>Org_Src_Width||Src_Height>Org_Src_Height)
	{
		*check_multiple = 1;
		tmp_diffSizeW =  Src_Width -Org_Src_Width;
		tmp_diffSizeH =  Src_Height -Org_Src_Height;

		scale_val_w = (float)dst_width/(float)Src_Width;
		scale_val_h =  (float)dst_height/(float)Src_Height;

		tmp_diffSizeW_1 = (scale_val_w*tmp_diffSizeW);
		tmp_diffSizeW_2 = (int)(scale_val_w*tmp_diffSizeW);

		residue = tmp_diffSizeW_1-(float)tmp_diffSizeW_2;

		if(0<residue)
			tmp_diffSizeW_2 = tmp_diffSizeW_2+1;
				
		tmp_diffSizeH_1  =  (scale_val_h*tmp_diffSizeH);
		tmp_diffSizeH_2 = (int)(scale_val_h*tmp_diffSizeH);
		
		residue = tmp_diffSizeH_1-(float)tmp_diffSizeH_2;	

		if(0<residue)
			tmp_diffSizeH_2= tmp_diffSizeH_2+1;

		*Sizediff_W =  ((int)tmp_diffSizeW_2 + 1) & 0xfffffffe;
		*Sizediff_H  = ((int)tmp_diffSizeH_2+ 1) & 0xfffffffe;
	}
	else
		*check_multiple = 0;	
}

static void Cal_Reset_DstSize(void)
{
	unsigned long fScaleFactor,fScaleFactor1;
	int img_wd,img_ht,gZoomLevel;
	int Level_X, Level_Y;
	
	img_wd=tcc89dst_width;
	img_ht=tcc89dst_height;
	fScaleFactor = (tcc89src_width * tcc89src_height *1000) / (img_wd * img_ht);
	gZoomLevel = 0;
	if(fScaleFactor < 64)   //for fix bug of tcc89xx scale limit 
	{
		while(1) 
		{
			gZoomLevel++;
			Level_X = gZoomLevel*ZOOM_STEP_X;
			Level_Y = gZoomLevel*ZOOM_STEP_Y;
			img_wd = tcc89dst_width-Level_X;
			img_ht = tcc89dst_height-Level_Y;
			fScaleFactor1= (tcc89src_width * tcc89src_height * 1000) / (img_wd * img_ht);
			if(fScaleFactor1 > 64)
				break;
		}
	}
	tcc89dst_width=img_wd;
	tcc89dst_height=img_ht;
	
}

static int config(uint32_t width, uint32_t height, uint32_t d_width, uint32_t d_height, uint32_t flags, char *title, uint32_t format)
{
	int fmt;
 //	int vm   = flags & VOFLAG_MODESWITCHING;
 	int zoom = flags & VOFLAG_SWSCALE;
       unsigned image_width, image_height;
	int       new_width,new_height;//out_w_offset, out_h_offset;

	fs = flags & VOFLAG_FULLSCREEN;


	printf("*** TCC8900 VO %s %d %s()\n",__FILE__,__LINE__,__FUNCTION__);
 	if(tcc89fb_dev_fd < 0)
		return 1;

    	tcc89fb_xres = tcc89fb_vinfo.xres;
    	tcc89fb_yres = tcc89fb_vinfo.yres;

	new_width = ((width + 15) & 0xfffffff0);
	new_height = ((height + 15) & 0xfffffff0);

	input_src_width=width;
	input_src_height=height;
	out_w_offset=new_width-width;
	out_h_offset=new_height-height;
	printf("tcc89mscfb org:%d %d,%d,%d,%d,%d\n", new_width,new_height,d_width, d_height,tcc89fb_xres, tcc89fb_yres);	
	// full screen ?
	if((new_width > tcc89fb_xres) || (new_height > tcc89fb_yres)) //when widh or height bigger than fb,we should set fs=1 
		fs=1;
	if ((zoom || fs) || (new_width < tcc89fb_xres) || (new_height < tcc89fb_yres))
	{
		printf("tcc89mscfb 1\n");
		aspect_save_orig(new_width, new_height);
		aspect_save_prescale(d_width, d_height);
		aspect_save_screenres(tcc89fb_xres, tcc89fb_yres);
		aspect(&image_width, &image_height, fs ? A_ZOOM : A_NOZOOM);

	} 
	else 
	{
	printf("tcc89mscfb 2\n");
		image_width  = (new_width < tcc89fb_xres)?new_width:tcc89fb_xres;
		image_height = (new_height < tcc89fb_yres)?new_height:tcc89fb_yres;
		//image_width = new_width;
		//image_height = new_height;
	}
	image_width = ((image_width + 15) & 0xfffffff0);
	image_height = ((image_height + 15) & 0xfffffff0);
	
	if(image_width>FB_WDMAX)
		image_width=FB_WDMAX;
	if(image_height>FB_HTMAX)
		image_height=FB_HTMAX;

	
	tcc89dst_width  = image_width;
	tcc89dst_height = image_height;

	tcc89src_width =new_width;
	tcc89src_height =new_height;
	tcc89buffer_size = 0;
  	tcc89buffer_stride[0] = 0;
  	tcc89src_fmt = format;



 //	if((tcc89src_width!=tcc89dst_width)||(tcc89src_height!=tcc89dst_height))
 	{
 		tcc89mscflag=1;
		M2M_ScalerForVideo_Enable(0);
 	}
//	else
 //		tcc89mscflag=0;

	Cal_Reset_DstSize();  //reset dst size,to avoid m2m scale limit
	CalculateDisplaySize(tcc89src_width,tcc89src_height,input_src_width,input_src_height,
			tcc89dst_width,tcc89dst_height,&check_res_mutiple,&sizediff_of_W,&sizediff_of_H); //cal unregular origal video

	Set_DisplayPosition(tcc89dst_width,tcc89dst_height,tcc89dst_width,tcc89dst_height,check_res_mutiple,
						sizediff_of_W,sizediff_of_H,phyfbmem);
	printf("tcc89mscfb last:%d %d,%d,%d,%d,%d\n", tcc89src_width,tcc89src_height,tcc89dst_width, tcc89dst_height,tcc89fb_xres, tcc89fb_yres);	
	printf("tcc89mscfb:%d,%d,%d,%d\n", tcc89mscflag,check_res_mutiple,sizediff_of_W,sizediff_of_H);

	tcc89viqelag=0;
	set_viqe_stop(0);
	set_viqe_wait(0);
	return 0;
}

static void uninit(void)
{
	if(tcc89mscflag==1)
		M2M_ScalerForVideo_Disable();
	fb1_deinit();
	TurnOnOffScaler1Clk (0);
	TurnOnOffVIQEClk (0);	
	TCC_DeinitalizeVIQE();
	if(mediafb!=NULL)
	{
		munmap(mediafb, screensize);
		mediafb=NULL;
	}
	close(tcc89fb_dev_fd);
	set_viqe_stop(1);
//	close(tcc89fb_mem_fd);	
}


static void check_events(void)
{
}
#if 0
#define fb1_size (4*1024*1024)
static void clear_fb1()
{
    int fd=open("/dev/fb1",O_RDWR);
	char *buf=(char *)mmap(0, fb1_size, PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0);
	memset(buf,0x0,fb1_size);
	munmap(buf, fb1_size);
	close(fd);
}
#endif
extern int vo_osd_changed_flag;
static int last_w=0,last_h=0,last_x0=0,last_y0=0;

// clear the old contents b boz we use 2 layers to display video & subtitle
void tcc_clear_old_osd(void)
{
	memset(mem_fb1+ 2 * (last_y0 * tcc89fb_vinfo.xres),0x0,tcc89fb_vinfo.xres*last_h*2);
}

void tcc_clear_time_osd()
{
	int local_dy=get_osd_dy();
	int local_h=get_osd_h();
	memset(mem_fb1+ 2 * (local_dy * tcc89fb_vinfo.xres),0x0,tcc89fb_vinfo.xres*local_h*2);
}
static int timeosd_change_flag=0;
static int percentosd_change_flag=0;
void set_timeosd_change(int val)
{
	timeosd_change_flag=val;
}

int is_timeosd_change_flag()
{
	return timeosd_change_flag;
}
void set_percentosd_change(int val)
{
	percentosd_change_flag=val;
}

int is_percentosd_change_flag()
{
	return percentosd_change_flag;
}
static pthread_mutex_t osd_lock=PTHREAD_MUTEX_INITIALIZER;
void lock_osd_change()
{
//	pthread_mutex_lock(&osd_lock);
}
void unlock_osd_change()
{
//	pthread_mutex_unlock(&osd_lock);
}
static void draw_alpha_32(int x0, int y0, int w, int h, unsigned char *src,
                          unsigned char *srca, int stride,char *str)
{
	//static int clear_flag=0;
	if(vo_osd_changed_flag==2  )
	{
		
		if(!is_osdtype_osd() )
		{
			tcc_clear_old_osd();
			//set_osd_change(0);
		}
		else
		{
			set_osd_dy(y0);
			set_osd_h(h);
		}
	
		tcc_vo_draw_alpha_rgb16(w, h, src, srca, stride,
			mem_fb1+ 2 * (y0 *  tcc89fb_vinfo.xres+ x0),
			2 *  tcc89fb_vinfo.xres);
 		 if(!is_osdtype_osd())
 		 {
			last_w=w;
			last_h=h;
			last_x0=x0;
			last_y0=y0;
		//	set_osd_change(0);
 		 }
	}

	
}


static int preinit(const char *arg)
{
	static int fb_preinit_done = 0;
	static int fb_works = 0;
	int fb_dev_fd1,fb_dev_fd2;
	unsigned int off=0;


//while(1);
	printf("tccfb init+++++\n");
//fb1_init();
	draw_alpha_fnc = draw_alpha_32;
	#if 0
	if (fb_preinit_done)
		return fb_works;
	#endif
	if (!tcc89fb_dev_name && !(tcc89fb_dev_name = getenv("FRAMEBUFFER")))
		tcc89fb_dev_name = strdup("/dev/fb0");
	
	printf("using %s\n", tcc89fb_dev_name);

	if ((tcc89fb_dev_fd = open(tcc89fb_dev_name, O_RDWR)) == -1) 
	{
		mp_msg(MSGT_VO, MSGL_ERR, "Can't open %s: %s\n", tcc89fb_dev_name, strerror(errno));
		goto err_out;
	}
	if (ioctl(tcc89fb_dev_fd, FBIOGET_FSCREENINFO, &tcc89fb_finfo)) 
	{
		mp_msg(MSGT_VO, MSGL_ERR, "Can't get VSCREENINFO: %s\n", strerror(errno));
		goto err_out_fd;
	}	
	phyfbmem=tcc89fb_finfo.smem_start;
	printf("phyfbmem=%x\n",phyfbmem);
	if (ioctl(tcc89fb_dev_fd, FBIOGET_VSCREENINFO, &tcc89fb_vinfo)) 
	{
		mp_msg(MSGT_VO, MSGL_ERR, "Can't get VSCREENINFO: %s\n", strerror(errno));
		goto err_out_fd;
	}
	//tcc89fb_orig_vinfo = tcc89fb_vinfo;


	printf("%dx%d, %dbpp\n", tcc89fb_vinfo.xres, tcc89fb_vinfo.yres, tcc89fb_vinfo.bits_per_pixel );
  // Figure out the size of the screen in bytes
  	screensize = FB0_SIZE;// tcc89fb_vinfo.xres * tcc89fb_vinfo.yres *tcc89fb_vinfo.bits_per_pixel / 8;

	if(mediafb==NULL)
		mediafb = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,tcc89fb_dev_fd, 0);
	if ((int)mediafb == -1) 
     	{
     		printf("Error: failed to map framebuffer device to memory.\n");
      	}
	 mediafbY= mediafb;
	 mediafbU=mediafbY+tcc89fb_vinfo.xres * tcc89fb_vinfo.yres;
	 mediafbV=mediafbU+tcc89fb_vinfo.xres * tcc89fb_vinfo.yres/4;
/*
	tcc89fb_mem_fd= open("/dev/mem", O_RDWR);
	if(tcc89fb_mem_fd<0)
		tcc89fb_mem_fd= open("/dev/mem", O_RDWR);
	if (tcc89fb_mem_fd<=0)
	{
		mp_msg(MSGT_VO, MSGL_ERR, "Could not open /dev/mem\n");
		printf("Could not open /dev/mem\n");
		goto err_out;
	}
	test_phy=phyfbmem+4*1024*1024;
	test_virt = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,tcc89fb_mem_fd, test_phy);
	if ((int)test_virt == -1) 
     	{
     		printf("Error: failed to map framebuffer test_virt device to memory.\n");
      	}
*/	
	if ((fb_dev_fd1 = open("/dev/fb1", O_RDWR))	== -1) 
	{
		printf("Can't open /dev/fb1: %s\n",strerror(errno));
		return 1;
	}	

	if ((fb_dev_fd2 = open("/dev/fb2", O_RDWR))	== -1) 
	{
		printf("Can't open /dev/fb2: %s\n",strerror(errno));
		return 1;
	}	
	//ioctl(fb_dev_fd1, TCC_LCD_FB_IOCTL_DISP_ONOFF, &off);	//temp,first close fb1 and fb2 ken del
	ioctl(fb_dev_fd2, TCC_LCD_FB_IOCTL_DISP_ONOFF, &off);	
	fb1_deinit();
	fb1_init();
	Init_TccBuffer(phyfbmem+4*1024*1024,mediafb+4*1024*1024);
	tcc89mscflag=0;
 	fb_preinit_done = 1;
	fb_works = 1;
	return 0;
err_out_fd:
	close(tcc89fb_dev_fd);
	tcc89fb_dev_fd = -1;
err_out:
	fb_preinit_done = 1;
	fb_works = 0;
	return 1;
}

static uint32_t get_image(mp_image_t *mpi) 
{
//	int buf = 0;

  // Currently read are too slow bcs we read from the
  // agp aperture and not the memory directly
  //if(mpi->flags&MP_IMGFLAG_READABLE) 
	return VO_TRUE;
}

void copy_to_scalesrc(uint8_t *image[], int stride[], int w,int h)
{
	int i;
	int width,height;
	
	uint8_t *srcPtr0= (char*)image[0];	
	uint8_t *srcPtr1= (char*)image[1];	
	uint8_t *srcPtr2= (char*)image[2];	
	uint8_t *dstPtr0;	
	uint8_t *dstPtr1;	
	uint8_t *dstPtr2;	

	width=w;
	height=h;
	dstPtr0=tcc_malloc(0);
	dstPtr1=dstPtr0+w*h;
	dstPtr2=dstPtr1+w*h/4;

	for(i=0;i<height;i++)		
	{			
		fast_memcpy(dstPtr0,srcPtr0,stride[0]);			
		srcPtr0+=stride[0];			
		dstPtr0+=width;		
	}		
	for(i=0;i<height/2;i++)		
	{			
		fast_memcpy(dstPtr1,srcPtr1,stride[1]);			
		fast_memcpy(dstPtr2,srcPtr2,stride[2]);						
		srcPtr1+=stride[1];			
		dstPtr1+=width/2;			
		srcPtr2+=stride[2];			
		dstPtr2+=width/2;		
	}
	return;
}
#include <pthread.h>
static int viqe_wait=0;
static int viqe_stop=0;
static pthread_mutex_t viqe_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_viqe=PTHREAD_COND_INITIALIZER;
static int count_viqe_thread=0;
static int count_main_thread=0;
static int viqe_exit=1;
void lock_viqe_m2m()
{
	pthread_mutex_lock(&viqe_lock);
}

void unlock_viqe_m2m()
{
	pthread_mutex_unlock(&viqe_lock);
}

void wait_viqe_m2m()
{
	pthread_cond_wait(&cond_viqe,&viqe_lock);
}
void send_viqe_m2m()
{
	pthread_cond_signal(&cond_viqe);
}
void set_viqe_wait(int val)
{
	viqe_wait=val;
}
void set_viqe_stop(int val)
{
	viqe_stop=val;
}
int get_viqe_wait()
{
	return viqe_wait;
}

int get_viqe_stop()
{
	return viqe_stop;
}

void reset_viqe_count()
{
	count_viqe_thread=count_main_thread=0;
}

void get_viqe_exit()
{
	return viqe_exit;
}
static void viqe_proc()
{
	int res=pthread_detach(pthread_self());
	viqe_exit=0; 
	while(!viqe_stop)
	{
		while(!viqe_wait) usleep(1);
		m2m_wait_interrupt();
		
		
		
		
		viqe_wait=0;
		
	}
	viqe_exit=1;
	printf("viqe_proc end!!!\n");
	
	pthread_exit(0);
}
static void start_check_viqe_thread()
{
	pthread_t pid_viqe;
	pthread_create(&pid_viqe, NULL, viqe_proc, 0);
//	printf("pid_audio=0x%X!!!!\n",pid_audio);
	//pthread_join(pid, NULL);
}
static uint32_t draw_image(mp_image_t *mpi)
{
  	int buf = 0,x,y;
  	uint8_t* planes[3];
  	uint8_t *image[6];
  	int stride[3];
//	int check_res_mutiple = 0;
//	unsigned int sizediff_of_W =0;
//	unsigned int sizediff_of_H = 0; // difference of original size with scaler input size

	//printf("tcc89mscfb111:0x%x\n", mpi->imgfmt);
	if (mpi->flags & MP_IMGFLAG_DIRECT) 
	{
    		buf = (int)mpi->priv;
  	}

	if(tcc89viqelag==0)
	{
		tcc89viqelag=1;
		TCC_TurnOnVIQE();
		TCC_InitalizeVIQE(tcc89dst_width, tcc89dst_height,GetYuvMode()?0:4,0); //DEC_OUT_YUV420INT
		TurnOnOffScaler1Clk (1);
		TurnOnOffVIQEClk (1);
		start_check_viqe_thread();

	}
  	switch (mpi->imgfmt) 
  	{
  		case IMGFMT_YUY2:
 		case IMGFMT_UYVY:
		case IMGFMT_BGR16:
      		// copy to vram
		planes[0] = mediafb;
		mem2agpcpy_pic(planes[0], mpi->planes[0], tcc89src_bpp * mpi->width, mpi->height, tcc89buffer_stride[0], mpi->stride[0]);	 
		break;

  		case IMGFMT_YV12:
		case IMGFMT_I420:
			stride[0] = mpi->stride[0];
			stride[1] = mpi->stride[1];
			stride[2] = mpi->stride[2];
			if(GetYuvMode()==0)  // VPU decoder
			{
				image[0] = ReturnPhyAddr(0);
				image[1] = ReturnPhyAddr(1);
				image[2] = ReturnPhyAddr(2);
				image[3] = ReturnVirtAddr(0);
				image[4] = ReturnVirtAddr(1);
				image[5] = ReturnVirtAddr(2);
			//	SetYuvMode(1);
			}
			else	// software decoder, copy data from allocated buffer to PHY address
			{
				image[0] =mpi->planes[0];
				image[1] =mpi->planes[1];
				image[2] =mpi->planes[2];		
				copy_to_scalesrc(image, stride, mpi->width,mpi->height);
				image[0] =ReturnTccPhyAddr(0);
				image[1] =image[0]+mpi->width*mpi->height;
				image[2] =image[1]+mpi->width*mpi->height/4;		
				image[3] =tcc_malloc(0);
				image[4] =image[3]+mpi->width*mpi->height;
				image[5] =image[4]+mpi->width*mpi->height/4;						
				set_dec_done(1);
			}
			
			
			x = (tcc89fb_vinfo.xres - tcc89dst_width) / 2;
			y = (tcc89fb_vinfo.yres - tcc89dst_height) / 2;
	//		printf("tcc89mscfb:%d %d,%d,%d,%d,%d,%d\n", tcc89mscflag,mpi->width,mpi->height,tcc89src_width,tcc89src_height,tcc89dst_width, tcc89dst_height);
	//		printf("tcc89mscfb:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",mpi->planes[0],mpi->planes[1],mpi->planes[2],image[0],image[1],image[2]);
			// memory to memory scaler
			if (tcc89mscflag == 1) 
			{
			//	writetomscfb(image, tcc89src_width,tcc89src_height,tcc89dst_width, tcc89dst_height);
				#if 1
				while(viqe_wait) usleep(1);
			//	count_main_thread++;
			//	lock_viqe_m2m();
			//	lock_viqe_m2m();
				TCC_ExecuteVIQE(tcc89src_width, tcc89src_height, tcc89dst_width, tcc89dst_height, image, phyfbmem, 
						0, 0, 0, tcc89src_width);
			//	unlock_viqe_m2m();
				//if(count_main_thread<count_viqe_thread)
				//start_check_viqe_thread();
				//	send_viqe_m2m();
			//	unlock_viqe_m2m();
				viqe_wait=1;
				
				
				#else
				TCC_ExecuteVIQE(320, 240, tcc89dst_width, tcc89dst_height, image, phyfbmem, 
						0, 0, 0, 320);
				#endif

			} 
			else 
			{
				x = (tcc89fb_vinfo.xres - mpi->width) / 2;
				y = (tcc89fb_vinfo.yres - mpi->height) / 2;		
				writetovideofb(image, stride, mpi->width,mpi->height, x, y, tcc89img_fmt);
			}
			#if 1
			if(timeosd_change_flag)
			{
				//printf("clear fb1!!!!\n");
				tcc_clear_time_osd();
				set_timeosd_change(0);
			}
			#endif
    		break;
		default:
			mp_msg(MSGT_VO,MSGL_WARN, MSGTR_LIBVO_TDFXVID_UnknownFormat,mpi->imgfmt);
			return VO_TRUE;
 	 }

  	return VO_TRUE;
}

static uint32_t fullscreen(void) 
{
  return VO_TRUE;
}

static uint32_t set_window(mp_win_t* w) 
{ 
  return VO_TRUE;
}

static uint32_t set_colorkey(mp_colorkey_t* colork) 
{
  return VO_TRUE;
}

static int control(uint32_t request, void *data, ...)
{
	switch (request) 
	{	
		case VOCTRL_QUERY_FORMAT:
			return query_format(*((uint32_t*)data));
		case VOCTRL_GET_IMAGE:
			return get_image(data);
		case VOCTRL_DRAW_IMAGE:
			return draw_image(data);
		//  case VOCTRL_START_SLICE:
		//   return start_slice(data);
		case VOCTRL_FULLSCREEN:
			return fullscreen();
		case VOCTRL_XOVERLAY_SUPPORT:
			return VO_TRUE;
		case VOCTRL_XOVERLAY_SET_COLORKEY:
			return set_colorkey(data);
		case VOCTRL_XOVERLAY_SET_WIN:
			return set_window(data);
		case VFCTRL_DRAW_OSD:
		       draw_osd(); 
			   return CONTROL_TRUE;
  }
  return VO_NOTIMPL;
}

