#ifndef SERVERMAIN_H
#define SERVERMAIN_H

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "JT808DbusMessage.h"
#include "dbusMessage.h"
#include "dbusClient.h"
#include "dbusApplication.h"

#include "VDRMonitorTable.h"
#include "VDRTrackInfoTable.h"

//record status
#define RECORD_STATUS_READY 0
#define RECORD_STATUS_START 1
#define RECORD_STATUS_STOP 2

//record file format
#define WAV 0
#define MP3 1

//m_flag
#define UPLOAD 0
#define SAVE 1
//m_command
#define START_RECORD 1
#define STOP_RECORD 0

//phony:prepare Data Chunk length in wav header
#define WAV_FILE_LENGTH 125 //

#define MAX_TIME_LENGTH 600   //max record time is 10 minites (ADD by leihailong, 2012-11-13, [00035--00038])
#define MAX_RECORD_STORAGE 80  //(MB), max storage space for record files saved in local.
#define MAX_RECORD_TABLE_ROWS 50

typedef struct RecordTableInfo{
    char filepath[128];
    int filetime;
    int filetype;
    int record_duration;
    int sample_rate;
}RecordTableInfo_t;

typedef struct ThreadArg {  //ADD: leihailong, 2012-11-21, [48-52]
    int arg_1;
    int arg_2;
} ThreadArg_t;

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

RecordTableInfo recordTableInfo;

DBusClientC *pClient;
volatile int record_status;

int m_sample;//from server notify (0,1,2,3)
int m_flag;
int wavFd;//wav file
int amrfd;

pthread_t pthread_control_record;
pthread_t pthread_storageControl;  //ADD: leihailong, 2012-11-20
pthread_mutex_t control_record_mutex;
pthread_cond_t control_record_cond;

void *pthread_control_record_handler(void *arg);

void *rm_old_wavFile(void * arg);  //ADD by leihailong, 2012-11-21

void write_record_table(RecordTableInfo_t *recordTableInfo);

char* generate_filename(time_t ctime);

void send_record_finishReq(int filetime);

int write_RecordTableInfo_Struct(int recordTimeLen, time_t filetime);

void write_record_table(RecordTableInfo_t *recordTableInfo);

int TransformPcmToWAV(int pcmRecFd, int pcmDataSize, int m_sample);

void ClosePcmAndRecordFile();

int TimeLenRecordLoops(int timeLen);

int TimeLenRecord(int timeLen);

int record_timeLen_calc(int readPcmCount);

int ControlRecord();

void RecordNotifyHandler(DBusNotifyC *notify);

#endif
