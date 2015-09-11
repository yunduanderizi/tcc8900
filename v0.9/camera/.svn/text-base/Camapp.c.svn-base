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


static pthread_t frame_threads;	
static CameraDevice dev;
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
	while(!flagexit)
	{
//		fprintf(stdout, "in handle_camera while\n");
		if(dev->cam_mode == MODE_PREVIEW)
			camif_get_frame(dev);
		else
			rsc_sched_delay(100);
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
			camif_set_resolution(dev, dev->preview_width, dev->preview_height);
			rsc_set_lcd_ch0(dev, 1);
			camif_start_stream(dev);
	    }
	    else if (!strncmp("quit", cmdline, 4)) 
		{
			exit(1);
	    }
	    else if (!strncmp("stop", cmdline, 4)) 
		{
			camif_stop_stream(dev);
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
	    }

	return NULL;
}
#endif

void init_viqe(CameraDevice *self)
{
	M2M_ScalerForVideo_Enable(1);
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

static int initcam()
{
    // 1. Open Device (CAMERA, LCD, Etc..) !!
	int ret = 0;
	init_camera_data(&dev);

	ret = open_device(&dev, CAMERA_DEVICE_NAME);
	if(ret < 0)
		return -1;
	camif_get_dev_info(&dev);
	init_viqe(&dev);
	bak_lcd_init(&dev);
	rsc_init_lcd(&dev, 0);
	clear_fb(&dev);

    // 2. Creat Thread (Get-Frame) !!
	pthread_create(&frame_threads, NULL, handle_camera, (void *)&dev);
	DBug_printf("CAMERA Start Ready!!! \n");

	dev.cam_mode = MODE_START;

	return 0;
}

int camStart()
{
	int ret = 0;
	flagexit = false;
	ret = initcam();

	handle_stdin(&dev, "start");
	return ret;
}

void camStop()
{
	rsc_set_lcd_ch0(&dev, 0);
	clear_fb(&dev);
	recovery_lcd_init(&dev);

	flagexit = true;
	pthread_join(frame_threads, 0);
	handle_stdin(&dev, "stop");
	uninit_viqe();
	close_device_fb(&dev);
	close_device(&dev);
	fprintf(stdout, "will return Quit\n");

	return;
}

void camQuit()
{
	
	return;
}
