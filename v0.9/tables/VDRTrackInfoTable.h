#ifndef _VDR_TRACK_INFO_TABLE_H_
#define _VDR_TRACK_INFO_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "LargeTableBase.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_LOCS_OCCUR_TIME = 0,
    VDR_LOCS_ALARM_SIGNAL,
    VDR_LOCS_STATUS_FLAG,
    VDR_LOCS_LATITUDE,
    VDR_LOCS_LONGITUDE,
    VDR_LOCS_ALTITUDE,
    VDR_LOCS_ORIENTATION,   
    VDR_LOCS_GPS_SPEED,
    VDR_LOCS_TOTAL_MILEAGE,
    VDR_LOCS_OIL_MASS,
    VDR_LOCS_VDR_SPEED,
    VDR_LOCS_VDR_STATUS,
    VDR_LOCS_VDR_NEW_HB_STATUS,
    VDR_LOCS_VDR_DISTANCE,
    VDR_LOCS_OVERSPEED_ALARM,
    VDR_LOCS_OVERSPEED_REGIONID,
    VDR_LOCS_INOUT_ALARM,
    VDR_LOCS_INOUT_REGIONID,
    VDR_LOCS_INOUT_DIRECTION,
    VDR_LOCS_SHORT_LONG_LINEID,
    VDR_LOCS_SHORT_LONG_TIME,
    VDR_LOCS_SHORT_LONG_RESULT,
    VDR_LOCS_EMERGENCY_ALARM_ID,
    VDR_LOCS_IO_STATUS,
    VDR_LOCS_ANALOG_QUANTITY,
    VDR_LOCS_WIRELESS_INTENSITY,
    VDR_LOCS_GNSS_SATELLITE_SUM,
    VDR_LOCS_COLNUM_BUTT
}ENUM_VDR_LOCSTATUS_COL;

typedef enum
{
    VDR_RUNS_OCCUR_TIME = 0,
    VDR_RUNS_SPEED,
    VDR_RUNS_STATUS,
    VDR_RUNS_COLNUM_BUTT
}ENUM_VDR_RUNSTATUS_COL;

typedef enum
{
    VDR_ACCIDENT_OCCUR_TIME = 0,
    VDR_ACCIDENT_SPEED,
    VDR_ACCIDENT_STATUS,
    VDR_ACCIDENT_DRIVER_LICENSE_NUM,
    VDR_ACCIDENT_LONGITUDE,
    VDR_ACCIDENT_LATITUDE,
    VDR_ACCIDENT_ALTITUDE,
    VDR_ACCIDENT_COLNUM_BUTT
}ENUM_VDR_ACCIDENT_COL;


typedef enum
{
    VDR_TIRED_BEGIN_TIME = 0,
    VDR_TIRED_BEGIN_LONGITUDE,
    VDR_TIRED_BEGIN_LATITUDE,
    VDR_TIRED_BEGIN_ALTITUDE,
    VDR_TIRED_END_TIME,
    VDR_TIRED_END_LONGITUDE,
    VDR_TIRED_END_LATITUDE,
    VDR_TIRED_END_ALTITUDE,
    VDR_TIRED_DRIVER_LICENSE_NUM,
    VDR_TIRED_COLNUM_BUTT
}ENUM_VDR_TIRED_COL;

typedef enum
{
    VDR_PRETIRED_DRIVER_LICENSE_NUM=0,
    VDR_PRETIRED_BEGIN_TIME,
    VDR_PRETIRED_BEGIN_LONGITUDE,
    VDR_PRETIRED_BEGIN_LATITUDE,
    VDR_PRETIRED_BEGIN_ALTITUDE,
    VDR_PRETIRED_END_TIME,
    VDR_PRETIRED_END_LONGITUDE,
    VDR_PRETIRED_END_LATITUDE,
    VDR_PRETIRED_END_ALTITUDE,
    VDR_PRETIRED_RUNNING_TIME,
    VDR_PRETIRED_METER_TODAY,
    VDR_PRETIRED_RUN_TIME,
    VDR_PRETIRED_FLAG_TIRED,
    VDR_PRETIRED_COLNUM_BUTT
}ENUM_VDR_PRETIRED_COL;

typedef enum
{
    VDR_OFFS_ACC_ON_TIME = 0,
    VDR_OFFS_ACC_OFF_TIME,
    VDR_OFFS_TOTAL_MILEAGE,
    VDR_OFFS_PARKED_DURA,
    VDR_OFFS_COLNUM_BUTT
}ENUM_VDR_OFFSTATUS_COL;

typedef enum
{
    VDR_DAY_OCCUR_TIME = 0,
    VDR_DAY_DRIVE_DURA,
    VDR_DAY_DRIVE_MILEAGE,
    VDR_DAY_TOTAL_MILEAGE,
    VDR_DAY_COLNUM_BUTT
}ENUM_VDR_DAYINFO_COL;

typedef enum
{
    VDR_PARAM_INFO_INDEX = 0,
    VDR_PARAM_OCCUR_TIME,
    VDR_PARAM_EVENT_ID,
    VDR_PARAM_COLNUM_BUTT
}ENUM_VDR_PARAMINFO_COL;

typedef enum
{
    VDR_SPEED_BEGIN_TIME = 0,
    VDR_SPEED_END_TIME,
    VDR_SPEED_STATUS,
    VDR_SPEED_VDR_SPEED,
    VDR_SPEED_GPS_SPEED,
    VDR_SPEED_COLNUM_BUTT
}ENUM_VDR_SPEEDINFO_COL;

class VDRLocStatus1Table : public LargeTableBase
{
public:
    VDRLocStatus1Table();
};

class VDRLocStatus2Table : public LargeTableBase
{
public:
    VDRLocStatus2Table();
};

class VDRRunStatusTable : public LargeTableBase
{
public:
    VDRRunStatusTable();
};

class VDRAccidentTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRAccidentTable();
};


class VDRTiredTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRTiredTable();
};

class VDRPreTiredTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRPreTiredTable();
};
class VDROFFStatusTable : public MultiRowWithUniqueIndexTable
{
public:
    VDROFFStatusTable();
};

class VDRDayInfoTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRDayInfoTable();
};

class VDRParamInfoTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRParamInfoTable();
};

class VDRSpeedInfoTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRSpeedInfoTable();
};

#endif
