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
#include "cam/yuv2jpg.h"

ViqeDevice viqeDev;
//#define AJUST_N_P

/**************************************************************************/
/*                                                                        */
/*                         Camera FuncTion			                       */
/*                                                                        */
/**************************************************************************/

/*===========================================================================
FUNCTION
===========================================================================*/
static int _camif_init_format (CameraDevice *self, int width, int height)
{
    int result;
	
    memset(&self->vid_fmt, 0,  sizeof(struct v4l2_format));
    self->vid_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if((result = ioctl(self->fd, VIDIOC_G_FMT, &self->vid_fmt)) < 0)
	{
		bq_printf(" ERROR :: cam ioctl() in function VIDIOC_G_FMT failed!");
	}
	else
	{
#ifdef USE_PIX_FMT_YUV420
	    self->vid_fmt.type 					= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	    self->vid_fmt.fmt.pix.width 		= width;
	    self->vid_fmt.fmt.pix.height 		= height;
	    self->vid_fmt.fmt.pix.field       	= V4L2_FIELD_INTERLACED;
	    self->vid_fmt.fmt.pix.pixelformat 	= V4L2_PIX_FMT_YVU420;
	    self->vid_fmt.fmt.pix.sizeimage 	= (width * height * 3) / 2;
#else
	    self->vid_fmt.type 					= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	    self->vid_fmt.fmt.pix.width 		= width;
	    self->vid_fmt.fmt.pix.height 		= height;
	    self->vid_fmt.fmt.pix.field       	= V4L2_FIELD_INTERLACED;
	    self->vid_fmt.fmt.pix.pixelformat 	= V4L2_PIX_FMT_YUYV; //V4L2_PIX_FMT_YVU420 <- YUV420
	    self->vid_fmt.fmt.pix.sizeimage 	= width * height * 2;
#endif
		
	    if((result = ioctl(self->fd, VIDIOC_S_FMT, &self->vid_fmt)) < 0)
		{
			bq_printf(" ERROR :: cam ioctl() in function VIDIOC_S_FMT failed!");
	    }
	}
    return result;
}

/*===========================================================================
FUNCTION
===========================================================================*/
static int _camif_init_buffer(CameraDevice *self)
{
    int result;
    struct v4l2_buffer buf;
	
    memset(&self->vid_buf, 0,  sizeof(struct v4l2_requestbuffers));


	if(self->cam_mode == MODE_CAPTURE)
    	self->vid_buf.count  = 1;    
	else
    	self->vid_buf.count  = NUM_VIDBUF; 

	self->vid_buf.count = 1;

	printf("<==================type================>:%d\n", (int)self->cam_mode);
	printf("<==================self->vid_buf.count================>:%d\n", self->vid_buf.count);
	
    self->vid_buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    self->vid_buf.memory = V4L2_MEMORY_MMAP;

	// internal buffer allocation!!
    if((result = ioctl(self->fd, VIDIOC_REQBUFS, &self->vid_buf)) < 0)
	{
        bq_printf(" ERROR :: cam ioctl() in function VIDIOC_REQBUFS failed!");
        exit (EXIT_FAILURE);
	}
	else
	{    
		if (self->vid_buf.count < 1) 
		{
	        bq_printf("ERROR :: Insufficient buffer memory on camera\n");
	        exit (EXIT_FAILURE);
	    }
		bq_printf(" Buffer Count : %d ..\n", self->vid_buf.count);
	}
	
    memset(&buf, 0,  sizeof(struct v4l2_buffer));
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = 0;

	// get internal buffer and mmap!!
    for (buf.index=0; buf.index<self->vid_buf.count; buf.index++) 
	{
        if (ioctl (self->fd, VIDIOC_QUERYBUF, &buf) < 0) 
		{
	        bq_printf(" ERROR :: cam ioctl() in function VIDIOC_QUERYBUF failed!");
		    return -1;
		}

        self->buffers[buf.index] = mmap(0, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, self->fd, buf.m.offset);

		if (MAP_FAILED == self->buffers[buf.index]) 
		{
		    bq_printf("mmap failed\n");
		    return -1;
		}
    }
	
    return result;
}

/*===========================================================================
FUNCTION
===========================================================================*/
static void _camif_uninit_buffer(CameraDevice *self)
{
	struct v4l2_buffer buf;
	int i;

	memset(&buf, 0,  sizeof(struct v4l2_buffer));

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	for (i=0; i<self->vid_buf.count; i++) 
	{
	    if ( ioctl(self->fd, VIDIOC_QUERYBUF, &buf) < 0) 
		{
			break;
	    }
	    munmap(self->buffers[buf.index], buf.length);
	}

}


/*===========================================================================
FUNCTION
===========================================================================*/
void open_device(CameraDevice* self, char *dev_name)
{	
    // 1. Open CAMERA Device !!
    if ((self->fd = open (dev_name, O_RDWR)) < 0) 
	{
		bq_printf("ERROR ::  CAMERA Driver Open : fd[%d]\n", self->fd);
		exit(1);
    }

#if defined(TCC83XX_CODE)
	// 2. Open LCD Device !!
    if((self->fb_fd0 = open(FB_DEVICE_NAME, O_RDWR)) < 0)
	{
		bq_printf("ERROR ::  LCD Driver Open : fd[%d]\n", self->fb_fd0);
		exit(1);
    }
	
	// 3. Open Etc Device !! For Rotate..   
    if((self->rt_fd = open("/dev/graphic",O_RDWR)) < 0)
	{
		bq_printf("ERROR ::  GRAPHIC Driver Open : fd[%d]\n", self->rt_fd);
    }
#elif defined(TCC8900_CODE)
	// 2. Open LCD Device !!
    if((self->fb_fd0 = open(FB_DEVICE_NAME, O_RDWR)) < 0)
	{
		bq_printf("ERROR ::  LCD Driver Open : fd[%d]\n", self->fb_fd0);
		exit(1);
    }
#endif

	return;
}

/*===========================================================================
FUNCTION
===========================================================================*/
void close_device(CameraDevice* self)
{
	camif_stop_stream(self);
	_camif_uninit_buffer(self);
	self->device_init = 0;

	// 1. Close Camera Device!!
    close(self->fd);

#if   defined(TCC83XX_CODE)
	// 2. Close LCD Device!!	
    close(self->fb_fd0);

	// 3. Clode etc Device!!
	if(self->rt_fd)
		close(self->rt_fd);
#elif defined(TCC8900_CODE)
	// 2. Close LCD Device!!	
    close(self->fb_fd0);
#endif
}

/*===========================================================================
FUNCTION
===========================================================================*/
void  init_camera_data(CameraDevice *self)
{
    memset (self, 0, sizeof (CameraDevice));
	
    self->fd 						= -1;
    self->preview_width				= PREVIEW_WIDTH;
    self->preview_height			= PREVIEW_HEIGHT;
	self->cam_mode					= MODE_START;
	self->device_init				= 0;
	init_queue(&self->Q);

#if defined(TCC83XX_CODE)
	self->cap_info.width 			= 1600;
	self->cap_info.height 			= 1200;
	self->cap_info.jpeg_quality 	= 3;

	self->rt_mode					= ROTATE_90;	
#endif
}

/*===========================================================================
FUNCTION
===========================================================================*/
int camif_get_dev_info (CameraDevice *self)
{
    int result;
	
    if((result = ioctl (self->fd, VIDIOC_QUERYCAP, &self->vid_cap)) < 0)
	{
		bq_printf(" ERROR :: cam ioctl() in _init_device_info failed");
    }
    else 
	{
		bq_printf("Driver: %s\n", self->vid_cap.driver);
		bq_printf("Card: %s\n", self->vid_cap.card);
		bq_printf("Capabilities: 0x%x\n", (unsigned int)(self->vid_cap.capabilities));
    }
	
    return result;
}

/*===========================================================================
FUNCTION
===========================================================================*/
int camif_set_blank(CameraDevice *self)
{
	int res;
	struct v4l2_buffer buf;
	
	memset(&buf, 0, sizeof(struct v4l2_buffer));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	while ((res = ioctl (self->fd, VIDIOC_DQBUF, &buf)) < 0  && (errno == EINTR))
		bq_printf(".\n");
	
	if (res < 0) 
	    return -1;

	memset(&buf, 0xff, sizeof(struct v4l2_buffer));

	//bq_printf("Cap : %d \n", buf.index);
	//pic_buf = self->buffers[buf.index];	
/*	if(self->rt_mode)
		rsc_process_image(self, (uint8_t*)(buf.m.offset));
	else*/
	if(/*viqeDev.even_frame ==*/ 0)
	{
		viqeDev.even_frame = 1;
		memcpy(&viqeDev.prev_buf, &buf, sizeof(struct v4l2_buffer));
	}else{
		viqeDev.even_frame = 0;
		memset(&viqeDev.prev_buf, 0, sizeof(struct v4l2_buffer));
		viqeDev.image[0] = buf.m.offset;
		viqeDev.image[1] = viqeDev.image[0] + PREVIEW_WIDTH*PREVIEW_HEIGHT;
#ifdef USE_PIX_FMT_YUV420
		viqeDev.image[2] = viqeDev.image[1] + PREVIEW_WIDTH*PREVIEW_HEIGHT/4;
#else
		viqeDev.image[2] = viqeDev.image[1] + PREVIEW_WIDTH*PREVIEW_HEIGHT/2;
#endif
		viqeDev.image[3] = viqeDev.prev_buf.m.offset;
		viqeDev.image[4] = viqeDev.image[3] + PREVIEW_WIDTH*PREVIEW_HEIGHT;
		viqeDev.image[5] = viqeDev.image[4] + PREVIEW_WIDTH*PREVIEW_HEIGHT/2;
		
		//TCC_ExecuteVIQE(PREVIEW_WIDTH,PREVIEW_HEIGHT,VIEW_WIDTH,VIEW_HEIGHT,viqeDev.image,0x47300000,0,0,0,PREVIEW_WIDTH);
		//M2M_ScalerForVideo_With_Trans(PREVIEW_WIDTH, PREVIEW_HEIGHT,  VIEW_WIDTH, VIEW_HEIGHT, viqeDev.image[0], viqeDev.image[1], viqeDev.image[2],  0x47300000, 0, 0, 0);
#ifdef USE_PIX_FMT_YUV420
		//M2M_ScalerForVideo_With_Trans(PREVIEW_WIDTH, PREVIEW_HEIGHT,  VIEW_WIDTH, VIEW_HEIGHT, viqeDev.image[0], viqeDev.image[1], viqeDev.image[2],  0x4b600000, 0, 0, 0);
		M2M_ScalerForVideo(PREVIEW_WIDTH, PREVIEW_HEIGHT,  VIEW_WIDTH, VIEW_HEIGHT, buf.m.offset,0x41600000, 0, 0);
	//	M2M_ScalerForVideo_With_Address_noviqe(PREVIEW_WIDTH, PREVIEW_HEIGHT,  VIEW_WIDTH, VIEW_HEIGHT, viqeDev.image[0], viqeDev.image[1], viqeDev.image[2],  0x4b600000, 0, 0, 0);
#else
		M2M_ScalerForVideo_With_Address_noviqe(PREVIEW_WIDTH, PREVIEW_HEIGHT,  VIEW_WIDTH, VIEW_HEIGHT, viqeDev.image[0], viqeDev.image[1], viqeDev.image[2],  0x41200000, 0, 0, 0);
#endif
		//rsc_set_lcd_addr(self,buf.m.offset);
		//rsc_set_lcd_addr(self,0x4FC00000);
		//rsc_set_lcd_addr(self,0x41200000);
		rsc_set_lcd_addr(self,0x41600000);
		//usleep(50);
	}

	rsc_draw_lcd(self);

	if (ioctl(self->fd, VIDIOC_QBUF, &buf) < 0) 
	{
	    bq_printf("%s, VIDIOC_QBUF failed\n", __func__);
	    return -1;
	}
	
    return 0;
}
int camif_get_frame(CameraDevice *self)
{
    int res;
	struct v4l2_buffer buf;
	int mode_change = 0;
	
	memset(&buf, 0, sizeof(struct v4l2_buffer));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	while ((res = ioctl (self->fd, VIDIOC_DQBUF, &buf)) < 0  && (errno == EINTR))
		bq_printf(".\n");
	
	if (res < 0) 
	    return -1;

	//bq_printf("Cap : %d \n", buf.index);
	//pic_buf = self->buffers[buf.index];	
/*	if(self->rt_mode)
		rsc_process_image(self, (uint8_t*)(buf.m.offset));
	else*/
	if(/*viqeDev.even_frame ==*/ 0)
	{
		viqeDev.even_frame = 1;
		memcpy(&viqeDev.prev_buf, &buf, sizeof(struct v4l2_buffer));
	}else{
		viqeDev.even_frame = 0;
		memset(&viqeDev.prev_buf, 0, sizeof(struct v4l2_buffer));
		viqeDev.image[0] = buf.m.offset;
		viqeDev.image[1] = viqeDev.image[0] + PREVIEW_WIDTH*PREVIEW_HEIGHT;
#ifdef USE_PIX_FMT_YUV420
		viqeDev.image[2] = viqeDev.image[1] + PREVIEW_WIDTH*PREVIEW_HEIGHT/4;
#else
		viqeDev.image[2] = viqeDev.image[1] + PREVIEW_WIDTH*PREVIEW_HEIGHT/2;
#endif
		viqeDev.image[3] = viqeDev.prev_buf.m.offset;
		viqeDev.image[4] = viqeDev.image[3] + PREVIEW_WIDTH*PREVIEW_HEIGHT;
		viqeDev.image[5] = viqeDev.image[4] + PREVIEW_WIDTH*PREVIEW_HEIGHT/2;
		
		//TCC_ExecuteVIQE(PREVIEW_WIDTH,PREVIEW_HEIGHT,VIEW_WIDTH,VIEW_HEIGHT,viqeDev.image,0x47300000,0,0,0,PREVIEW_WIDTH);
		//M2M_ScalerForVideo_With_Trans(PREVIEW_WIDTH, PREVIEW_HEIGHT,  VIEW_WIDTH, VIEW_HEIGHT, viqeDev.image[0], viqeDev.image[1], viqeDev.image[2],  0x47300000, 0, 0, 0);

#ifdef AJUST_N_P
		if(self->video_type_change())
		{
			mode_change = 1;
			goto video_restart;
		}
#endif		
		
#ifdef USE_PIX_FMT_YUV420
		//M2M_ScalerForVideo_With_Trans(PREVIEW_WIDTH, PREVIEW_HEIGHT,  VIEW_WIDTH, VIEW_HEIGHT, viqeDev.image[0], viqeDev.image[1], viqeDev.image[2],  0x4b600000, 0, 0, 0);
		//M2M_ScalerForVideo(PREVIEW_WIDTH, PREVIEW_HEIGHT,  VIEW_WIDTH, VIEW_HEIGHT_MODIFY, buf.m.offset,0x41600000, 0, 0);		
		M2M_ScalerForVideo(PREVIEW_WIDTH, self->preview_height,  VIEW_WIDTH, VIEW_HEIGHT_MODIFY, buf.m.offset,0x41600000, 0, 0);
	//	M2M_ScalerForVideo_With_Address_noviqe(PREVIEW_WIDTH, PREVIEW_HEIGHT,  VIEW_WIDTH, VIEW_HEIGHT, viqeDev.image[0], viqeDev.image[1], viqeDev.image[2],  0x4b600000, 0, 0, 0);
#else
		M2M_ScalerForVideo_With_Address_noviqe(PREVIEW_WIDTH, PREVIEW_HEIGHT,  VIEW_WIDTH, VIEW_HEIGHT, viqeDev.image[0], viqeDev.image[1], viqeDev.image[2],  0x41200000, 0, 0, 0);
#endif
		//rsc_set_lcd_addr(self,buf.m.offset);
		//rsc_set_lcd_addr(self,0x4FC00000);
		//rsc_set_lcd_addr(self,0x41200000);
		rsc_set_lcd_addr(self,0x41600000);
		usleep(10*500);
	}

	rsc_draw_lcd(self);

video_restart:
	if (ioctl(self->fd, VIDIOC_QBUF, &buf) < 0) 
	{
	    bq_printf("%s, VIDIOC_QBUF failed\n", __func__);
	    return -1;
	}

#ifdef AJUST_N_P
	if(mode_change)
		self->video_restart();
#endif
	
    return 0;
}

//#ifdef ADD_CAMERA_MODE

static long gain_cur_time(char *timebuf)
{
	if(NULL == timebuf)
	{
		printf("error param!\n");
		return NULL;
	}
	
	time_t now;
    now = time(NULL); 
	struct tm *tm;
	tm = localtime(&now);
	sprintf(timebuf, "%d%02d%02d_%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec);

	return now;
}
int camera_save_picture(CameraDevice *self, int i)
{
	if(NULL == self)
	{
		printf("error input param!\n");
		return -1;
	}
	
	
	struct v4l2_buffer buf;
	memset(&buf, 0, sizeof(struct v4l2_buffer));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	int res;
	while ((res = ioctl (self->fd, VIDIOC_DQBUF, &buf)) < 0  && (errno == EINTR))
		bq_printf(".\n");
	
	if (res < 0) 
	    return -1;

	unsigned char src[buf.length + 1];
	unsigned char dest[buf.length *2];

	memcpy(src, self->buffers[buf.index], buf.length);

	if(i < 2)
	{
		if (ioctl(self->fd, VIDIOC_QBUF, &buf) < 0) 
		{
		    bq_printf("%s, VIDIOC_QBUF failed\n", __func__);
		    return -1;
		}

		return 0;
	}

	self->picture_num++;
	printf("num:%d\n", self->picture_num);
	
	//保存图片
	FILE* file_fd;
	char timebuf[1024] = {0};
	self->picinfo.m_captime = gain_cur_time(timebuf);
	self->cap_time = self->picinfo.m_captime;
	char namebuf[1024] = {0};
	char namebufscale[1024] = {0};
	sprintf(namebuf, "%s%s_%d%03u_src.jpg", CAPTURE_FILE_PATH, timebuf, self->picinfo.m_channel, self->picture_num);
	sprintf(namebufscale, "%s%s_%d%03u", CAPTURE_FILE_PATH, timebuf, self->picinfo.m_channel, self->picture_num);
	file_fd = fopen(namebuf,"w+");

	//int ret = compress_yuyv_to_jpeg_suoxiao(src, dest, self->preview_width , self->preview_height, 80);//数据转换
	int ret = put_jpeg_yuv420p_memory(dest, buf.length, src, self->preview_width, self->preview_height, 80);
	fwrite(dest, ret, 1, file_fd);//转换后的数据写入
	fflush(file_fd);
	fclose(file_fd);


	memset(self->name, 0, 1024);
	strcpy(self->name, namebufscale);
	memset(self->picinfo.name, 0, 1024);
	strcpy(self->picinfo.name, namebufscale);
	en_queue(&self->Q, &self->picinfo, sizeof(struct pic_info));

#ifndef CAPTURE_MODE
	rsc_draw_lcd(self);
#endif

	if (ioctl(self->fd, VIDIOC_QBUF, &buf) < 0) 
	{
	    bq_printf("%s, VIDIOC_QBUF failed\n", __func__);
	    return -1;
	}
	
    return 0;

}
//#endif

/*===========================================================================
FUNCTION
===========================================================================*/
void camif_set_queryctrl(CameraDevice *self, unsigned int ctrl_id, int value)
{
    struct v4l2_queryctrl q;
    struct v4l2_control c;
	
    memset(&q, 0, sizeof(struct v4l2_queryctrl));
    q.id = ctrl_id;

    if (ioctl(self->fd, VIDIOC_QUERYCTRL, &q) < 0 ) 
	{
		bq_printf("IOCTL() set value");
    }

    memset(&c, 0, sizeof(struct v4l2_control));
    c.id = ctrl_id;
	
    if (value < 0) 
		c.value = q.default_value;
    else 
		c.value = value;

    if (ioctl(self->fd, VIDIOC_S_CTRL, &c) < 0) 
	{
		bq_printf("IOCTL() set value");
    }
}

/*===========================================================================
FUNCTION
===========================================================================*/
void camif_set_resolution (CameraDevice *self, int width, int height)
{
	_camif_uninit_buffer(self);

    if (_camif_init_format(self, width, height) < 0) 
	{
    	exit(-1);
    }

    if (_camif_init_buffer(self) < 0) 
	{
		exit(-1);
    }	
}

/*===========================================================================
FUNCTION
===========================================================================*/
void camif_set_overlay(CameraDevice *self, int buffer_value)
{
#if defined(TCC83XX_CODE)
	cif_SuperImpose si_Data;
	char *buffer;
	int siFile_fd;

	si_Data.chromakey_info.chromakey = 0x27E0;
	si_Data.chromakey_info.mask_r = si_Data.chromakey_info.mask_g = si_Data.chromakey_info.mask_b = 0xF0;
	si_Data.chromakey_info.key_y = 0x32;
	si_Data.chromakey_info.key_u = 0x97;
	si_Data.chromakey_info.key_v = 0x31;

	si_Data.start_x = si_Data.start_y = 0;
	si_Data.width = 320;
	si_Data.height = 240;

	if(buffer_value)	
		si_Data.buff_offset = TCC8300_JPEG_STREAM_BASE - 0x26000; //temp
	else
		si_Data.buff_offset = 0;
	

	buffer = mmap(NULL,TCC8300_JPEG_FULL_MMAP_SIZE,PROT_READ|PROT_WRITE, MAP_SHARED,self->fd,TCC8300_CAPTURED_IMAGE_BASE);
	buffer += si_Data.buff_offset;

	siFile_fd = open("overlay.dat",O_RDONLY);
	bq_printf("fd: %d, Address : 0x%x\n",siFile_fd, (unsigned int)buffer);
	read(siFile_fd,buffer,si_Data.width*si_Data.height*2);
	close(siFile_fd);
	
	if (ioctl (self->fd, VIDIOC_USER_CIF_OVERLAY, &si_Data) < 0) 
	{
	    bq_printf("Unable to overlay (VIDIOC_USER_CIF_OVERLAY)");
	    return;
	}
#endif
}

/*===========================================================================
FUNCTION
===========================================================================*/
void camif_start_stream(CameraDevice *self)
{
    struct v4l2_buffer buf;
    int type;

	if(self->cam_mode == MODE_PREVIEW)
		return;

	for (buf.index=0; buf.index<self->vid_buf.count; buf.index++) 
	{
	    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	    buf.memory      = V4L2_MEMORY_MMAP;

	    if (ioctl (self->fd, VIDIOC_QBUF, &buf) < 0) 
		{
			bq_printf("VIDIOC_QBUF\n");
			exit(EXIT_FAILURE);
	    }
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl (self->fd, VIDIOC_STREAMON, &type) < 0) 
	{
		bq_printf("ERROR :: Can't VIDIOC_STREAMON\n");
		exit(EXIT_FAILURE);
	}

	self->cam_mode = MODE_PREVIEW;
}

void camif_start_capture(CameraDevice *self)
{
    struct v4l2_buffer buf;
    int type;

	if(self->cam_mode == MODE_PREVIEW)
		return;

	for (buf.index=0; buf.index<self->vid_buf.count; buf.index++) 
	{
	    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	    buf.memory      = V4L2_MEMORY_MMAP;

	    if (ioctl (self->fd, VIDIOC_QBUF, &buf) < 0) 
		{
			bq_printf("VIDIOC_QBUF\n");
			exit(EXIT_FAILURE);
	    }
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl (self->fd, VIDIOC_STREAMON, &type) < 0) 
	{
		bq_printf("ERROR :: Can't VIDIOC_STREAMON\n");
		exit(EXIT_FAILURE);
	}

	self->cam_mode = MODE_CAPTURE;
}


/*===========================================================================
FUNCTION
===========================================================================*/
void camif_stop_stream(CameraDevice *self)
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(self->cam_mode != MODE_PREVIEW && self->cam_mode != MODE_CAPTURE)
		return;
	
	if (ioctl (self->fd, VIDIOC_STREAMOFF, &type) < 0) 
	{
		bq_printf("ERROR :: Can't VIDIOC_STREAMOFF\n");
		exit(EXIT_FAILURE);
	}

	self->cam_mode = MODE_PREVIEW_STOP;
}

/*===========================================================================
FUNCTION
===========================================================================*/
void  camif_capture(CameraDevice *self)
{
#if defined(TCC83XX_CODE)
	int ret, retry_cnt, jpeg_qval;
	struct pollfd capture_event;
    struct v4l2_buffer buf;

	retry_cnt = 0;
	jpeg_qval = self->cap_info.jpeg_quality;
	
    bq_printf("Capture -> \n");

	for (buf.index=0; buf.index<self->vid_buf.count; buf.index++) 
	{
	    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	    buf.memory      = V4L2_MEMORY_MMAP;

	    if (ioctl (self->fd, VIDIOC_QBUF, &buf) < 0) 
		{
			bq_printf("VIDIOC_QBUF\n");
			exit(EXIT_FAILURE);
	    }
	}

	
retry_capture:	
	if (ioctl (self->fd, VIDIOC_USER_JPEG_CAPTURE, &jpeg_qval) < 0) 
	{
	    bq_printf("Capture FAIL!! (VIDIOC_USER_JPEG_CAPTURE)\n");
	    return;
	}

	capture_event.fd = self->fd;
	capture_event.events = POLLIN|POLLERR; // Zzau_Temp: POLLERR阑 overflow侩栏肺 烙矫荤侩..
	bq_printf("Poll -> \n");
	if((ret = poll((struct pollfd*)&capture_event, 1, 2000)) < 0)
	{
		bq_printf("Capture POLL Error!!\n");
		exit(1);
	}

	if(ret == 0 && retry_cnt < 3)
	{
		retry_cnt++;
		bq_printf("Retry-Cap %d because of No Interrupt -> \n", retry_cnt);
		goto retry_capture;
	}
	else if(capture_event.revents & POLLERR)
	{
		jpeg_qval++;
		bq_printf("Retry-Cap %d, %d because of Overflow -> \n", retry_cnt, jpeg_qval);

		if(jpeg_qval <= 8)
			goto retry_capture;
		else
			return;
	}

	if (ioctl (self->fd, VIDIOC_USER_GET_CAPTURE_INFO, &(self->cap_info.jpeg_save_info)) < 0) 
	{
	    bq_printf("Capture FAIL!! (VIDIOC_USER_JPEG_CAPTURE)\n");
	    return;
	}
#endif		
}

