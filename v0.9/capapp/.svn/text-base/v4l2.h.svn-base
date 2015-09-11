/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               Camera    I n t e r f a c e    M O D U L E

                        EDIT HISTORY FOR MODULE

when        who       what, where, why
--------    ---       -------------------------------------------------------
10/xx/08   ZzaU      Created file.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


#ifndef __CAM_V4L2_H__
#include "main.h"
#include <linux/videodev2.h>
#include "cam/queue.h"

#define __CAM_V4L2_H__

#define NUM_VIDBUF      8
//#define NUM_VIDBUF      1

#if   defined(TCC83XX_CODE)
#include "include/tcc83xx_jpeg.h"
#include <asm-arm/arch/tcc8300fb_ctrl.h>
#include <asm-arm/arch/tcc83xx_grp_ioctrl.h>
#elif defined(TCC8900_CODE)
#include "tcc/tccfb_ioctl.h"
#endif

#define VIDIOC_USER_CIF_OVERLAY  		_IOWR ('V', BASE_VIDIOC_PRIVATE, 	cif_SuperImpose)
#define VIDIOC_USER_JPEG_CAPTURE  		_IOWR ('V', BASE_VIDIOC_PRIVATE+1, 	int)
#define VIDIOC_USER_GET_CAPTURE_INFO  	_IOWR ('V', BASE_VIDIOC_PRIVATE+2,  TCCXXX_JPEG_ENC_DATA)

/* Our own specific controls */
#define V4L2_CID_ISO 					V4L2_CID_PRIVATE_BASE+0
#define V4L2_CID_EFFECT  				V4L2_CID_PRIVATE_BASE+1
#define V4L2_CID_ZOOM					V4L2_CID_PRIVATE_BASE+2
#define V4L2_CID_FLIP					V4L2_CID_PRIVATE_BASE+3
#define V4L2_CID_SCENE					V4L2_CID_PRIVATE_BASE+4
#define V4L2_CID_METERING_EXPOSURE		V4L2_CID_PRIVATE_BASE+5
#define V4L2_CID_FLASH					V4L2_CID_PRIVATE_BASE+6
#define V4L2_CID_LAST_PRIV				V4L2_CID_FLASH
#define V4L2_CID_MAX					V4L2_CID_LAST_PRIV+1

#ifdef _8925_VERSION
#define CAPTURE_FILE_PATH	"/nand2/innovdata/photo/"
#else
#define CAPTURE_FILE_PATH	"/nand3/innovdata/photo/"
#endif

typedef enum{
	MODE_START = 0,
	MODE_PREVIEW,
	MODE_PREVIEW_STOP,
	MODE_CAPTURE,
	MODE_CAPREC,
	MODE_DELSY
}camera_mode;


typedef struct
{
	unsigned short 			chromakey;
	
	unsigned char			mask_r;
	unsigned char			mask_g;
	unsigned char			mask_b;
	
	unsigned char			key_y;
	unsigned char			key_u;
	unsigned char			key_v;
	
}si_chromakey_info;

typedef struct
{
	unsigned short 			start_x;
	unsigned short 			start_y;
	unsigned short 			width;
	unsigned short 			height;
	
	unsigned int 			buff_offset;

	si_chromakey_info		chromakey_info;			
}cif_SuperImpose;

#if defined(TCC83XX_CODE)
typedef struct {
	unsigned int				width;
	unsigned int				height;

	unsigned char				jpeg_quality;
	unsigned char				jpeg_capture;
	
	TCCXXX_JPEG_ENC_DATA 		jpeg_save_info;
}CaptureInfo;
#endif

struct pic_info{
	int m_channel;
    int m_command; 
    int m_interval;
    int m_flag;
    int m_resolution;
    int m_quality;
    int m_brightness;
    int m_saturation;
    int m_chroma;
	int m_captime;
	int m_event;
	char name[1024];
};

typedef struct {
    int 						fd;	
	int							preview_width;
	int							preview_height;
	camera_mode					cam_mode;
	unsigned int			    picture_num;

    struct v4l2_capability		vid_cap;
    struct v4l2_format			vid_fmt;
    struct v4l2_streamparm		vid_parm;
    struct v4l2_requestbuffers	vid_buf;
	
	queue 						Q;
	unsigned int 				device_init;
	unsigned int 				resolution;
	unsigned long				cap_time;
	char 						name[1024];

	struct pic_info				picinfo;

    unsigned char				*buffers[NUM_VIDBUF];

#if defined(TCC83XX_CODE)
	CaptureInfo					cap_info;
#endif

//#if defined(TCC82XX_CODE) || defined(TCC8900_CODE)
//#ifdef  TCC8900_CODE
	//LCD
    int							fb_fd0;
    int							mem_len;
    void						*outbuf; //phy_addr
//#endif

#if defined(TCC83XX_CODE)
	//Preview-Rotate
	G2D_OP_MODE					rt_mode;
	int							rt_fd;
#else
	int			 				rt_mode;
#endif
	int 						video_mode;
	int 	(* video_init)();
	int 	(* video_type_change)();
	int 	(* video_restart)();
} CameraDevice;

typedef struct {
	int even_frame; //0:odd frame; 1:even frame
	unsigned int image[6];
	struct v4l2_buffer prev_buf;
}ViqeDevice;


extern void 	open_device 				(CameraDevice *self, char *dev_name);
extern void 	close_device 			(CameraDevice *self);
extern void 	init_camera_data 		(CameraDevice *self);
extern int 		camif_get_dev_info 	(CameraDevice *self);
extern int		camif_get_frame			(CameraDevice *self);
extern void 	camif_set_queryctrl		(CameraDevice *self, unsigned int ctrl_id, int value);
extern void 	camif_set_resolution 	(CameraDevice *self, int width, int height);
extern void 	camif_set_overlay		(CameraDevice *self, int buffer_value);
extern void 	camif_start_stream		(CameraDevice *self);
extern void 	camif_stop_stream		(CameraDevice *self);
extern void 	camif_capture			(CameraDevice *self);
extern void  	camif_encode_jpeg		(CameraDevice *self);
extern void 	camif_save 				(CameraDevice *self, unsigned short* filename);

int camera_save_picture(CameraDevice *self, int i);
int camif_set_blank(CameraDevice *self);
void camif_start_capture(CameraDevice *self);

#endif /* __CAM_V4L2_H__ */
