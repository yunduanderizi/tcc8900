/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               Camera    I n t e r f a c e    M O D U L E

                        EDIT HISTORY FOR MODULE

when        who       what, where, why
--------    ---       -------------------------------------------------------
10/xx/08   ZzaU      Created file.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


#ifndef __CAM_MAIN_H__
#define __CAM_MAIN_H__


/*=============================================================================

                                 FEATURE 

=============================================================================*/

#define CONSOLE_INPUT_TEST

#define TCC8900_CODE

#define CAMERA_DEVICE_NAME 		"/dev/video0"
#define FB_DEVICE_NAME			"/dev/fb0"

#define SAVE_WIDTH	640
#define SAVE_HEIGHT 480

#define PREVIEW_WIDTH	 720

#ifdef CAMERA_DEVICE_N
#define PREVIEW_HEIGHT	 475
#else
#define PREVIEW_HEIGHT	 576
#endif

#define VIEW_WIDTH		800
#define VIEW_HEIGHT 	480

//#define USE_PIX_FMT_YUV420		// yuv420

#define DBug_printf printf

/*===========================================================================

                           INCLUDE FILES FOR MODULE

===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/kd.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/poll.h>


#include "v4l2.h"
#include "rsc.h"
#include "saclejpeg.h"
#include "yuv2jpg.h"
//#include "tcc/viqe_lib.h"

unsigned int phyFbAddr;
unsigned int virtualFb0Addr;
unsigned int virtualFb1Addr;
unsigned int virtualFb2Addr;

#define	VIRTUAL_FB0_GAIN_SIZE	(6 * 1024 * 1024)
#define VIRTUAL_FB1_GAIN_SIZE	(8 * 1024 * 1024)
#define VIRTUAL_FB2_GAIN_SIZE	(12 * 1024 * 1024)

//void init_viqe(CameraDevice *self);
int initCam();
int initcam();

int camStart();
int camCap(int, int);

void camStop();

void camQuit();
#endif

