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

static int icvbs = 0;
ViqeDevice viqeDev;

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
		DBug_printf(" ERROR :: cam ioctl() in function VIDIOC_G_FMT failed!");
	}
	else
	{
#ifdef USE_PIX_FMT_YUV420
		self->vid_fmt.type 			= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		self->vid_fmt.fmt.pix.width 		= width;
		self->vid_fmt.fmt.pix.height 		= height;
		self->vid_fmt.fmt.pix.field       	= V4L2_FIELD_INTERLACED;
		self->vid_fmt.fmt.pix.pixelformat 	= V4L2_PIX_FMT_YVU420;
		self->vid_fmt.fmt.pix.sizeimage 	= (width * height * 3) / 2;
#else
		self->vid_fmt.type 			= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		self->vid_fmt.fmt.pix.width 		= width;
		self->vid_fmt.fmt.pix.height 		= height;
		self->vid_fmt.fmt.pix.field       	= V4L2_FIELD_INTERLACED;
		self->vid_fmt.fmt.pix.pixelformat 	= V4L2_PIX_FMT_YUYV; //V4L2_PIX_FMT_YVU420 <- YUV420
		self->vid_fmt.fmt.pix.sizeimage 	= width * height * 2;
#endif
		if((result = ioctl(self->fd, VIDIOC_S_FMT, &self->vid_fmt)) < 0)
		{
			DBug_printf(" ERROR :: cam ioctl() in function VIDIOC_S_FMT failed!");
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

	self->vid_buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	self->vid_buf.memory = V4L2_MEMORY_MMAP;

	// internal buffer allocation!!
	if((result = ioctl(self->fd, VIDIOC_REQBUFS, &self->vid_buf)) < 0)
	{
		DBug_printf(" ERROR :: cam ioctl() in function VIDIOC_REQBUFS failed!");
		exit (EXIT_FAILURE);
	}
	else
	{    
		if (self->vid_buf.count < 1) 
		{
			DBug_printf("ERROR :: Insufficient buffer memory on camera\n");
			exit (EXIT_FAILURE);
		}
		DBug_printf(" Buffer Count : %d ..\n", self->vid_buf.count);
	}

	memset(&buf, 0,  sizeof(struct v4l2_buffer));
	buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory      = V4L2_MEMORY_MMAP;
	buf.index       = 0;

	// get internal buffer and mmap!!
	for (buf.index=0; buf.index < self->vid_buf.count; buf.index++) 
	{
		if (ioctl (self->fd, VIDIOC_QUERYBUF, &buf) < 0) 
		{
			DBug_printf(" ERROR :: cam ioctl() in function VIDIOC_QUERYBUF failed!");
			return -1;
		}
		fprintf(stdout, "_camif_init_buffer-----0x%x\n",buf.m.offset);
		self->buffers[buf.index] = mmap(0, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, self->fd, buf.m.offset);

		if (MAP_FAILED == self->buffers[buf.index]) 
		{
			DBug_printf("mmap failed\n");
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
	int ret = 0;
	memset(&buf, 0,  sizeof(struct v4l2_buffer));

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	for (i=0; i<self->vid_buf.count; i++) 
	{
		if ( ioctl(self->fd, VIDIOC_QUERYBUF, &buf) < 0) 
		{
			fprintf(stdout, "------------>munmap: break\n");
			break;
		}
		ret = munmap(self->buffers[buf.index], buf.length);
		if(ret < 0)
			fprintf(stderr, "--------->munmap: %s\n", strerror(ret));
		else
			fprintf(stdout, "--------->munmap: success\n");
		buf.index++;
	}
	return;
}


/*===========================================================================
FUNCTION
===========================================================================*/
int open_device(CameraDevice* self, char *dev_name)
{	
    // 1. Open CAMERA Device !!
	if ((self->fd = open (dev_name, O_RDWR)) < 0) 
	{
		DBug_printf("ERROR ::  CAMERA Driver Open : fd[%d]\n", self->fd);
		return -1;
	}

	// 2. Open LCD Device !!
	if((self->fb_fd0 = open(FB_DEVICE_NAME, O_RDWR)) < 0)
	{
		DBug_printf("ERROR ::  LCD Driver Open : fd_fb0[%d]\n", self->fb_fd0);
		return -1;
	}
        icvbs = 0;
        ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_DUAL_DISPLAY_MODE, &icvbs);

	if((self->fb_fd1 = open(FB_DEVICE_NAME_FB1, O_RDWR)) < 0)
	{
		DBug_printf("ERROR ::  LCD Driver Open : fd_fb1[%d]\n", self->fb_fd1);
		return -1;
	}

	return 0;
}

/*===========================================================================
FUNCTION
===========================================================================*/
void close_device_fb(CameraDevice* self)
{
//	Close LCD Device!!	
	ioctl(self->fb_fd0, TCC_LCD_FB_IOCTL_DUAL_DISPLAY_MODE, &icvbs);
	close(self->fb_fd0);
	close(self->fb_fd1);
}

void close_device(CameraDevice* self)
{
	_camif_uninit_buffer(self);

	// 1. Close Camera Device!!
	close(self->fd);
}

/*===========================================================================
FUNCTION
===========================================================================*/
void  init_camera_data(CameraDevice *self)
{
	memset (self, 0, sizeof (CameraDevice));
	
	self->fd			= -1;
	self->preview_width		= PREVIEW_WIDTH;
	self->preview_height		= PREVIEW_HEIGHT;
	self->cam_mode			= MODE_START;
}

/*===========================================================================
FUNCTION
===========================================================================*/
int camif_get_dev_info (CameraDevice *self)
{
	int result;
	
	if((result = ioctl (self->fd, VIDIOC_QUERYCAP, &self->vid_cap)) < 0)
	{
		DBug_printf(" ERROR :: cam ioctl() in _init_device_info failed");
	}
	else 
	{
		DBug_printf("Driver: %s\n", self->vid_cap.driver);
		DBug_printf("Card: %s\n", self->vid_cap.card);
		DBug_printf("Capabilities: 0x%x\n", (unsigned int)(self->vid_cap.capabilities));
	}
	
	return result;
}

/*===========================================================================
FUNCTION
===========================================================================*/
int camif_get_frame(CameraDevice *self)
{
	int res;
	struct v4l2_buffer buf;

	memset(&buf, 0, sizeof(struct v4l2_buffer));

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	while ((res = ioctl (self->fd, VIDIOC_DQBUF, &buf)) < 0  && (errno == EINTR))
		DBug_printf(".\n");

	if (res < 0) 
	    return -1;

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
	
	M2M_ScalerForVideo(PREVIEW_WIDTH, PREVIEW_HEIGHT, VIEW_WIDTH, VIEW_HEIGHT, buf.m.offset, virtualFb0Addr, 0, 0);
//	M2M_ScalerForVideo_With_Address(176, 144, VIEW_WIDTH_IN, VIEW_HEIGHT_IN, virtualFb0Addr , virtualFb0Addr+176*144, virtualFb0Addr+176*144+176*144/4, virtualFb1Addr, 0, 0, 0);
	M2M_ScalerForVideo_With_Address(176,144, VIEW_WIDTH_IN, VIEW_HEIGHT_IN, VIDEO_PLAY_ADDRESS, VIDEO_PLAY_ADDRESS+176*144, VIDEO_PLAY_ADDRESS+176*144+176*144/4, virtualFb1Addr, 0, 0, 0);

	if (ioctl(self->fd, VIDIOC_QBUF, &buf) < 0) 
	{
	    DBug_printf("%s, VIDIOC_QBUF failed\n", __func__);
	    return -1;
	}

	return 0;
}

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
		DBug_printf("IOCTL() set value");
    }

    memset(&c, 0, sizeof(struct v4l2_control));
    c.id = ctrl_id;
	
    if (value < 0) 
		c.value = q.default_value;
    else 
		c.value = value;

    if (ioctl(self->fd, VIDIOC_S_CTRL, &c) < 0) 
	{
		DBug_printf("IOCTL() set value");
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
	DBug_printf("fd: %d, Address : 0x%x\n",siFile_fd, (unsigned int)buffer);
	read(siFile_fd,buffer,si_Data.width*si_Data.height*2);
	close(siFile_fd);
	
	if (ioctl (self->fd, VIDIOC_USER_CIF_OVERLAY, &si_Data) < 0) 
	{
	    DBug_printf("Unable to overlay (VIDIOC_USER_CIF_OVERLAY)");
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
			DBug_printf("VIDIOC_QBUF\n");
			exit(EXIT_FAILURE);
	    }
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl (self->fd, VIDIOC_STREAMON, &type) < 0) 
	{
		DBug_printf("ERROR :: Can't VIDIOC_STREAMON\n");
		exit(EXIT_FAILURE);
	}

	self->cam_mode = MODE_PREVIEW;
}

/*===========================================================================
FUNCTION
===========================================================================*/
void camif_stop_stream(CameraDevice *self)
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(self->cam_mode != MODE_PREVIEW)
		return;
	
	if (ioctl (self->fd, VIDIOC_STREAMOFF, &type) < 0) 
	{
		DBug_printf("ERROR :: Can't VIDIOC_STREAMOFF\n");
//		exit(EXIT_FAILURE);
		return;
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
	
    DBug_printf("Capture -> \n");

	for (buf.index=0; buf.index<self->vid_buf.count; buf.index++) 
	{
	    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	    buf.memory      = V4L2_MEMORY_MMAP;

	    if (ioctl (self->fd, VIDIOC_QBUF, &buf) < 0) 
		{
			DBug_printf("VIDIOC_QBUF\n");
			exit(EXIT_FAILURE);
	    }
	}

	
retry_capture:	
	if (ioctl (self->fd, VIDIOC_USER_JPEG_CAPTURE, &jpeg_qval) < 0) 
	{
	    DBug_printf("Capture FAIL!! (VIDIOC_USER_JPEG_CAPTURE)\n");
	    return;
	}

	capture_event.fd = self->fd;
	capture_event.events = POLLIN|POLLERR; // Zzau_Temp: POLLERR을 overflow용으로 임시사용..
	DBug_printf("Poll -> \n");
	if((ret = poll((struct pollfd*)&capture_event, 1, 2000)) < 0)
	{
		DBug_printf("Capture POLL Error!!\n");
		exit(1);
	}

	if(ret == 0 && retry_cnt < 3)
	{
		retry_cnt++;
		DBug_printf("Retry-Cap %d because of No Interrupt -> \n", retry_cnt);
		goto retry_capture;
	}
	else if(capture_event.revents & POLLERR)
	{
		jpeg_qval++;
		DBug_printf("Retry-Cap %d, %d because of Overflow -> \n", retry_cnt, jpeg_qval);

		if(jpeg_qval <= 8)
			goto retry_capture;
		else
			return;
	}

	if (ioctl (self->fd, VIDIOC_USER_GET_CAPTURE_INFO, &(self->cap_info.jpeg_save_info)) < 0) 
	{
	    DBug_printf("Capture FAIL!! (VIDIOC_USER_JPEG_CAPTURE)\n");
	    return;
	}
#endif		
}
