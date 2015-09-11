/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               Camera    I n t e r f a c e    M O D U L E

                        EDIT HISTORY FOR MODULE

when        who       what, where, why
--------    ---       -------------------------------------------------------
10/xx/08   ZzaU      Created file.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


#ifndef __CAM_MAIN_H__
#define __CAM_MAIN_H__
#include "v4l2.h"

/*=============================================================================

                                 FEATURE 

=============================================================================*/

#define CONSOLE_INPUT_TEST

#define TCC8900_CODE

#define CAMERA_DEVICE_NAME 		"/dev/video0"
#define FB_DEVICE_NAME			"/dev/fb0"

#define PREVIEW_WIDTH	 720
#define PREVIEW_HEIGHT	 572
//#define PREVIEW_HEIGHT	 576
#define PREVIEW_HEIGHT_N	476

//#define VIEW_WIDTH	720
//#define VIEW_HEIGHT 576

#define VIEW_WIDTH	800
#define VIEW_HEIGHT 480
#define VIEW_HEIGHT_MODIFY	485

#define USE_PIX_FMT_YUV420		// yuv420

#if 1
#define bq_printf(fmt, args...) printf(fmt, ##args)
#else
#define bq_printf(fmt, args...)
#endif


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


#ifdef __cplusplus
extern "C" {
#endif
#include "v4l2.h"
#include "rsc.h"
#include "tcc/viqe_lib.h"
#include "tcc/tcc_viqe_interface.h"
#include "rsc.h"
#include "cam/queue.h"
#include "tcc/M2Mscaler_Video.h"
#ifdef __cplusplus
}
#endif

unsigned int phyFbAddr;

extern int camera_init(void *);
extern void camera_close();
extern int camera_switch();

extern long camera_cap(int resolution, char **name);
//extern long camera_cap(char **name);
extern int camera_preview();
extern int camera_delay();
extern unsigned long  my_time();
extern int scale_jpeg(char *buffer_src, char *buffer_det, int width, int height);
#endif

