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


static pthread_t frame_threads;	
CameraDevice dev;
//static CameraDevice camera;
static bool flagexit = false;

/**************************************************************************/
/*                                                                        */
/*                         Main FuncTion			                       */
/*                                                                        */
/**************************************************************************/

/*===========================================================================
FUNCTION
===========================================================================*/
void* handle_camera(void *param)
{
    CameraDevice *dev = (CameraDevice *)param;
	
	int num = 0;
	while(!flagexit)
	{
		switch(dev->cam_mode)
		{
			case MODE_PREVIEW:
				camif_get_frame(dev);
				break;
			case MODE_CAPTURE:
				camera_save_picture(dev, num++);
				if(num > 2)
				{
					dev->cam_mode = MODE_DELAY;
				}
				break;
			default:
				rsc_sched_delay(100);
		}
	}

	fprintf(stdout, "will exit pthread\n");
	pthread_exit(NULL);
}

#ifdef CONSOLE_INPUT_TEST

/*===========================================================================
FUNCTION
===========================================================================*/
void* handle_stdin(void* param, char* cmdline)
{
	CameraDevice *dev = (CameraDevice *)param;
	int r;

		// 2. Process Function!!
	if(cmdline == NULL)
		return;

	    if (!strncmp("start", cmdline, 5)) 
		{
			if(dev->buffer_inited == 0)
			{
				camif_set_resolution(dev, dev->preview_width, dev->preview_height);
				camif_start_stream(dev);
			}
			
			rsc_set_lcd_ch0(dev, 1);
			dev->buffer_inited = 1;
			dev->cam_mode = MODE_PREVIEW;
	    }
	    else if (!strncmp("quit", cmdline, 4)) 
		{
			exit(1);
	    }
	    else if (!strncmp("stop", cmdline, 4)) 
		{
			camif_stop_stream(dev);
	//		rsc_set_lcd_ch0(dev, 0);
	    }
	    else if (!strncmp("bright", cmdline, 6)) 
		{
	    	char *args = cmdline + 7;
			
	    	if (*(args) < '0' || *(args) > '4') 
			{
	    	    DBug_printf("usage : bright [brightness_value]\n");
	    	    DBug_printf("ex) bright 3\n");
	    	    return NULL;
	    	}  
	    	camif_set_queryctrl(dev, V4L2_CID_BRIGHTNESS, *(args)-'0');   
	    }
	    else if (!strncmp("wb", cmdline, 2)) 
		{
	    	char *args = cmdline + 3;
			
	    	if (*(args) < '0' || *(args) > '5') 
			{
	    	    DBug_printf("usage : AWB [awb_value]\n");
	    	    DBug_printf("ex) wb 3\n");
	    	    return NULL;
	    	}  
	    	camif_set_queryctrl(dev, V4L2_CID_AUTO_WHITE_BALANCE, *(args)-'0');   
	    }
	    else if (!strncmp("iso", cmdline, 3)) 
		{
	    	char *args = cmdline + 4;
			
	    	if (*(args) < '0' || *(args) > '3') 
			{
	    	    DBug_printf("usage : ISO [iso_value]\n");
	    	    DBug_printf("ex) iso 3\n");
	    	    return NULL;
	    	}  
	    	camif_set_queryctrl(dev, V4L2_CID_ISO, *(args)-'0');   
	    }
	    else if (!strncmp("flip", cmdline, 4)) 
		{
	    	char *args = cmdline + 5;
			
	    	if (*(args) < '0' || *(args) > '3') 
			{
	    	    DBug_printf("usage : FLIP [flip_value]\n");
	    	    DBug_printf("ex) flip 3\n");
	    	    return NULL;
	    	}  
	    	camif_set_queryctrl(dev, V4L2_CID_FLIP, *(args)-'0');   
	    }
	    else if (!strncmp("effect", cmdline, 6)) 
		{
	    	char *args = cmdline + 7;
			
	    	if (*(args) < '0' || *(args) > '9') 
			{
	    	    DBug_printf("usage : effect [effect_value]\n");
	    	    DBug_printf("ex) effect 3\n");
	    	    return NULL;
	    	}   
	    	camif_set_queryctrl(dev, V4L2_CID_EFFECT, *(args)-'0');   
	    }
	    else if (!strncmp("zoom", cmdline, 4)) 
		{
	    	char *args = cmdline + 5;
			char zoom_val;

			zoom_val = atoi(args);
			
	    	if (zoom_val > 25) 
			{
	    	    DBug_printf("usage : zoom [zoom_value]\n");
	    	    DBug_printf("ex) zoom 1\n");
	    	    return NULL;
	    	}   
	    	camif_set_queryctrl(dev, V4L2_CID_ZOOM, zoom_val);   
	    }
		else if (!strncmp("overlay", cmdline, 7))
		{
			char *args = cmdline + 8;
			
	    	if (*(args) < '0' || *(args) > '1') 
			{
	    	    DBug_printf("usage : overlay [overlay_value]\n");
	    	    DBug_printf("ex) overlay 1\n");
	    	    return NULL;
	    	}   
			camif_set_overlay(dev, *(args)-'0');
		}
	    else if (!strncmp("capture", cmdline, 7)) 
		{	
			if(dev->buffer_inited == 0)
			{
				camif_set_resolution(dev, dev->preview_width, dev->preview_height);
				camif_start_capture(dev);
			}
			dev->buffer_inited = 1;
			
			dev->cam_mode = MODE_CAPTURE;
		}

	return NULL;
}
#endif

void init_viqe(CameraDevice *self)
{
	M2M_ScalerForVideo_Enable(0);
}

void uninit_viqe()
{
	M2M_ScalerForVideo_Disable();
}
/*===========================================================================
FUNCTION
===========================================================================*/
int initCam()
{	
	return 0;
}

int initcam(int width, int height)
{
	int ret = -1;

	if(dev.device_inited)
	{
		printf("device already inited!\n");
		return 0;
	}

	ret = camera_init(&dev, width, height);
	if(ret < 0)
	{
		printf("init camera error!\n");
		return -1;
	}

	ret = camera_open(&dev);
	if(ret < 0)
	{
		printf("open camera error!\n");
		return -1;
	}

	ret = fb_open(&dev);
	if(ret < 0)
	{
		printf("open camera error!\n");
		return -1;
	}

	ret = camera_get_info(&dev);
	if(ret < 0)
	{
		printf("init camera error!\n");
		return -1;
	}


	init_viqe(&dev);
	bak_lcd_init(&dev);
	rsc_init_lcd(&dev, 0);
	
	pthread_create(&frame_threads, NULL, handle_camera, (void *)&dev);
	DBug_printf("CAMERA Start Ready!!! \n");

	dev.cam_mode = MODE_START;

	dev.device_inited = 1;

	return 0;
}

int camStart()
{
	flagexit = false;
	
	initcam(PREVIEW_WIDTH, PREVIEW_HEIGHT);
	 
	handle_stdin(&dev, "start");

	return 0;
}

int camCap(int width, int height)
{
	if(width < 0 || height < 0)
	{
		width = 320;
		height = 240;
	}
	
	flagexit = false;
	
	initcam(PREVIEW_WIDTH, PREVIEW_HEIGHT);

	dev.cap_witdh = width;
	dev.cap_height = height;
	 
	handle_stdin(&dev, "capture");
	
	return 0;
}

void camStop()
{
	rsc_set_lcd_ch0(&dev, 0);
	recovery_lcd_init(&dev);

	flagexit = true;
	pthread_join(frame_threads, 0);
	fprintf(stdout, "will stop\n");
	handle_stdin(&dev, "stop");
	uninit_viqe();
	close_device_fb(&dev);
	close_device(&dev);
	fprintf(stdout, "will return Quit\n");
	dev.buffer_inited = 0;
	dev.device_inited = 0;

	return;
}

void camQuit()
{
	return;
}
