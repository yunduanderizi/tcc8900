#include "main.h"
#include "tcc/tcc_viqe_interface.h"
#include "rsc.h"
#include "saclejpeg.h"

CameraDevice dev;
pthread_t frame_threads;	
static bool flagexit = false;

int scale_jpeg(char *buffer_src, char *buffer_det, int width, int height)
{
	return scalejpeg(buffer_src, buffer_det, width, height);
}

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
					dev->cam_mode = MODE_DELSY;
				}
				break;			
			default:
				rsc_sched_delay(100);
		}
	}

	return NULL;
}


/*===========================================================================
FUNCTION
===========================================================================*/
void* handle_stdin(char *cmdline)
{

    if (!strncmp("start", cmdline, 5)) 
	{
		rsc_init_lcd(&dev, 0);
		if(dev.device_init ==  0)
		{
			dev.video_init();
			camif_set_resolution(&dev, dev.preview_width, dev.preview_height);
			camif_start_stream(&dev);
		}

		dev.device_init = 1;
		dev.cam_mode = MODE_PREVIEW;
		usleep(100000);
		rsc_set_lcd_ch0(&dev, 1);

    }
	else if (!strncmp("capture", cmdline, 7)) 
	{
		rsc_init_lcd(&dev, 0);
		if(dev.device_init == 0)
		{				
			dev.video_init();
			camif_set_resolution(&dev, dev.preview_width, dev.preview_height);
			camif_start_capture(&dev);
		}

		dev.device_init = 1;
		dev.cam_mode = MODE_CAPTURE;
		while(dev.cam_mode == MODE_CAPTURE)
		{
			usleep(50);
		}
    }
	else if (!strncmp("delay", cmdline, 7)) 
	{
		dev.cam_mode = MODE_DELSY;
		rsc_set_lcd_ch0(&dev, 0);
		rsc_black_lcd(&dev);
	}
    else if (!strncmp("quit", cmdline, 4)) 
	{
		exit(1);
    }
    else if (!strncmp("stop", cmdline, 4)) 
	{
		camif_stop_stream(&dev);
		rsc_set_lcd_ch0(&dev, 0);
    }
	else if(!strncmp("switch", cmdline, 6))
	{			
		camif_stop_stream(&dev);
	}
    else if (!strncmp("bright", cmdline, 6)) 
	{
    	char *args = cmdline + 7;
		
    	if (*(args) < '0' || *(args) > '4') 
		{
    	    bq_printf("usage : bright [brightness_value]\n");
    	    bq_printf("ex) bright 3\n");
    	    return NULL;
    	}  
    	camif_set_queryctrl(&dev, V4L2_CID_BRIGHTNESS, *(args)-'0');   
    }
    else if (!strncmp("wb", cmdline, 2)) 
	{
    	char *args = cmdline + 3;
		
    	if (*(args) < '0' || *(args) > '5') 
		{
    	    bq_printf("usage : AWB [awb_value]\n");
    	    bq_printf("ex) wb 3\n");
    	    return NULL;
    	}  
    	camif_set_queryctrl(&dev, V4L2_CID_AUTO_WHITE_BALANCE, *(args)-'0');   
    }
    else if (!strncmp("iso", cmdline, 3)) 
	{
    	char *args = cmdline + 4;
		
    	if (*(args) < '0' || *(args) > '3') 
		{
    	    bq_printf("usage : ISO [iso_value]\n");
    	    bq_printf("ex) iso 3\n");
    	    return NULL;
    	}  
    	camif_set_queryctrl(&dev, V4L2_CID_ISO, *(args)-'0');   
    }
    else if (!strncmp("flip", cmdline, 4)) 
	{
    	char *args = cmdline + 5;
		
    	if (*(args) < '0' || *(args) > '3') 
		{
    	    bq_printf("usage : FLIP [flip_value]\n");
    	    bq_printf("ex) flip 3\n");
    	    return NULL;
    	}  
    	camif_set_queryctrl(&dev, V4L2_CID_FLIP, *(args)-'0');   
    }
    else if (!strncmp("effect", cmdline, 6)) 
	{
    	char *args = cmdline + 7;
		
    	if (*(args) < '0' || *(args) > '9') 
		{
    	    bq_printf("usage : effect [effect_value]\n");
    	    bq_printf("ex) effect 3\n");
    	    return NULL;
    	}   
    	camif_set_queryctrl(&dev, V4L2_CID_EFFECT, *(args)-'0');   
    }
    else if (!strncmp("zoom", cmdline, 4)) 
	{
    	char *args = cmdline + 5;
		char zoom_val;

		zoom_val = atoi(args);
		
    	if (zoom_val > 25) 
		{
    	    bq_printf("usage : zoom [zoom_value]\n");
    	    bq_printf("ex) zoom 1\n");
    	    return NULL;
    	}   
    	camif_set_queryctrl(&dev, V4L2_CID_ZOOM, zoom_val);   
    }
	else if (!strncmp("overlay", cmdline, 7))
	{
		char *args = cmdline + 8;
		
    	if (*(args) < '0' || *(args) > '1') 
		{
    	    bq_printf("usage : overlay [overlay_value]\n");
    	    bq_printf("ex) overlay 1\n");
    	    return NULL;
    	}   
		camif_set_overlay(&dev, *(args)-'0');
	}

	return NULL;
}

void init_viqe(CameraDevice *self)
{
	if(self)
	{
		bq_printf("0x:%d\n", self);
	}
	
	TCC_TurnOnVIQE();
	TCC_InitalizeVIQE(720,486,0,0);
	TurnOnOffScaler1Clk(1);
	TurnOnOffVIQEClk(1);
#ifdef USE_PIX_FMT_YUV420
	M2M_ScalerForVideo_Enable(1);
	SetYuvMode(1);
#else
	M2M_ScalerForVideo_Enable(0);
#endif
}

void deinit_vique()
{
	M2M_ScalerForVideo_Disable();
	TCC_TurnOFFVIQE();
}

int camera_init(void *check_video_type)
{
	int ret = -1;
	
    init_camera_data(&dev);
    open_device(&dev, CAMERA_DEVICE_NAME);
    camif_get_dev_info(&dev);
	
	init_viqe(&dev);

	ret = pthread_create(&frame_threads, NULL, handle_camera, (void *)&dev);
	if(ret < 0)
	{
		bq_printf("create handle_camera error!\n");
		return ret;
	}
	bq_printf("CAMERA Start Ready!!! \n");

	dev.cam_mode = MODE_START;
	dev.video_init = check_video_type;
	
}

void camera_close()
{
	flagexit = true;
	pthread_join(frame_threads,0);
	deinit_vique();
	
    close_device(&dev);
}


long camera_cap(int resolution, char **name)
{
	if(resolution < 0 || resolution > 8)
		return -1;
	
	dev.resolution = resolution;
	handle_stdin("capture");
	strcpy(*name, dev.name);

	return dev.cap_time;
}

int camera_preview()
{
	handle_stdin("start");

	return 0;
}

int camera_delay()
{
	handle_stdin("delay");

	return 0;
}

int camera_switch()
{
	handle_stdin("switch");

	return 0;	
}

unsigned long my_time()
{
	return time(NULL);
}

