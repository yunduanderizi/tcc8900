#ifndef _VDR_MONITOR_TABLE_H_
#define _VDR_MONITOR_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_RECORD_FORMAT_WAV = 0,
    VDR_RECORD_FORMAT_MP3
}ENUM_VDR_RECORD_FORMAT;

typedef enum
{
    VDR_REC_FILE_PATH = 0,
    VDR_REC_FILE_TIME,
    VDR_REC_FILE_TYPE,
    VDR_REC_RECORD_DURA,
    VDR_REC_EVENT,
    VDR_REC_SAMPLE_RATE,
    VDR_REC_ALARM_SIGNAL,
    VDR_REC_STATUS_FLAG,
    VDR_REC_LATITUDE,
    VDR_REC_LONGITUDE,
    VDR_REC_ALTITUDE,
    VDR_REC_ORIENTATION,   
    VDR_REC_GPS_SPEED,
    VDR_REC_OCCUR_TIME,
    VDR_REC_COLNUM_BUTT
}ENUM_VDR_RECORD_COL;

typedef enum
{
    VDR_CAPTURE_FORMAT_JPG = 0,
    VDR_CAPTURE_FORMAT_TIF
}ENUM_VDR_CAPTURE_FORMAT;

typedef enum
{
    VDR_CAP_FILE_PATH = 0,
    VDR_CAP_FILE_TIME,
    VDR_CAP_FILE_TYPE,
    VDR_CAP_FILE_QUALITY,
    VDR_CAP_RESOLUTION,
    VDR_CAP_BRIGHTNESS,
    VDR_CAP_CONTRAST,
    VDR_CAP_SATUATION,
    VDR_CAP_CHROMA,
    VDR_CAP_EVENT,
    VDR_CAP_CHANNEL_ID,
    VDR_CAP_ALARM_SIGNAL,
    VDR_CAP_STATUS_FLAG,
    VDR_CAP_LATITUDE,
    VDR_CAP_LONGITUDE,
    VDR_CAP_ALTITUDE,
    VDR_CAP_ORIENTATION,   
    VDR_CAP_GPS_SPEED,
    VDR_CAP_OCCUR_TIME,
    VDR_CAP_COLNUM_BUTT
}ENUM_VDR_CAPTURE_COL;


class VDRRecordTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRRecordTable();
};

class VDRCaptureTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRCaptureTable();
};

#endif
