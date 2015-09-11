#ifndef _CAM_CAP_H_
#define _CAM_CAP_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
#include "i2ccontrol.h"

#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>  
#include <linux/input.h> 
#include <linux/i2c.h>  
#include <linux/i2c-dev.h>  

struct cap_obj
{	
	int capcur_status;
	int capold_status;
	int pthoto_status;
	int chancur_status;
	int chanold_status;

	int school_custom;

	void *pri;
};

struct LocStatusData
{
    int type;
    int m_flag;     //full min flag 
    int up_alarm_flag;
    int upload_flag;
    int itime;
    int alarm_signal;
    int status_falg;
    int latitude;
    int longitude;
    int altitude;
    int orientation;
    int gps_speed;
    int total_mileage;
    int oil_mass;
    int vdr_status_old_hb;
    int vdr_status_new_hb;
    int vdr_speed;
    int vdr_distance;
    int overspeed_flag;
    int overspeed_id;
    int fence_inout_alarm;
    int fence_inout_id;
    int fence_inout_direction;
    int fence_line_id;
    int fence_line_time;
    int fence_line_result;
    int emergency_num;    
    int io_status;
    int analog_quantity;
    int wireless_intensity;
    int gnss_satellite_sum;
};

#ifdef __cplusplus
}
#endif

#include "JT808DbusMessage.h"
#include "dbusApplication.h"
#include "dbusClient.h"
#include "VDRMonitorTable.h"
#include "DeviceManagerDbusMessage.h"
#include "GeneralTimer.h"
#include "CapappDBusMessage.h"
#include "VDRTrackInfoTable.h"
#include "VDRMonitorTable.h"

#define PHOTO_IN_BACKEND	0	
#define PHOTO_IN_TIMER		1
#define PHOTO_IN_ROBBERY	2
#define PHOTO_IN_COLLISION	3
#define PHOTO_IN_OPENDOOR	4
#define PHOTO_IN_CLOEDOOR	5
#define PHOTO_IN_OTOCDOOR	6
#define PHOTO_IN_DISTANCE	7
#define PHOTO_IN_OHTERALARM 8

#define CHANNEL_IN_IN1			0
#define CHANNEL_IN_IN2			1
#define CHANNEL_IN_DIA			2
#define CHANNEL_IN_CVBS			3
#define CHANNEL_IN_REVERSE		3
#define CHANNEL_IN_MAX			3
#define offset(n)				((n) & CHANNEL_IN_MAX)

#define CAPAPP_IN_DELAY			0
#define CAPAPP_IN_PHOTO			1
#define CAPAPP_IN_VIDEO			2
#define CAPAPP_IN_REVERSE		3

#define PAL_IN_VALUE			17
#define NTSC_IN_VALUE 	 		7
#define NTSC_IN_MODE			0
#define PAL_IN_MODE				1

#define SENSER_I2C_ADDRESS		0x45
#define SENSER_I2C_CHANNEL		0x02
#define SENSER_I2C_VIDETYPE		0x1C

#endif
