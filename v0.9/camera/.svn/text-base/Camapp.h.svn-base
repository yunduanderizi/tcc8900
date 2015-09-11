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
#define FB_DEVICE_NAME_FB1			"/dev/fb1"

#define PREVIEW_WIDTH	 720

#ifdef CAMERA_DEVICE_N
#define PREVIEW_HEIGHT	 475 //N mode
#else
#define PREVIEW_HEIGHT	 576 //default is PAL
#endif

#define VIDEO_PLAY_ADDRESS        0x4c200000

#define POS_X_IN        227
#define POS_Y_IN        84
#define VIEW_WIDTH_IN   380
#define VIEW_HEIGHT_IN 316
#define POS_X   44
#define POS_Y   84
#define VIEW_WIDTH      176
#define VIEW_HEIGHT 144

#define POS_X_IN        242
#define POS_Y_IN        90
#define VIEW_WIDTH_IN   380
#define VIEW_HEIGHT_IN 316
#define POS_X   32
#define POS_Y   116
#define VIEW_WIDTH      176
#define VIEW_HEIGHT 144

#define USE_PIX_FMT_YUV420		// yuv420

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
//#include "tcc/viqe_lib.h"

unsigned int phyFbAddr;
unsigned int virtualFb0Addr;
unsigned int virtualFb1Addr;
unsigned int virtualFb2Addr;

#define	VIRTUAL_FB0_GAIN_SIZE	(4 * 1024 * 1024)
//#define VIRTUAL_FB1_GAIN_SIZE	(8 * 1024 * 1024)
//#define VIRTUAL_FB2_GAIN_SIZE	(12 * 1024 * 1024)

//void init_viqe(CameraDevice *self);
int initCam();

int camStart();

void camStop();

void camQuit();
#endif

