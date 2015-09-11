#include "LogUtility.h"
#include "VDRTrackInfoTable.h"

const static SQLiteColumn VdrLocStatusCols[VDR_LOCS_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColOccurTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColAlarmSignal",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColStatusFlag",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLatitude",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLongitude",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColAltitude",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOrientation",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColGPSSpeed",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColTotalMileage",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOilMass",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColVDRSpeed",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColVDRStatus",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
	SQLiteColumn("ColVDRNewHbStatus",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColVDRDistance",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOverSpeedAlarm",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOverSpeedRegionID",ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, false),
    SQLiteColumn("ColInOutAlarm",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColInOutRegionID",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColInOutDirection",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColShortLongLineID", ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColShortLongTime",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColShortLongResult", ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEmergAlarmId",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColIoStatus",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColAnalogQuantity",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColWirelessIntensity", ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColGnssSatelliteSum",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

const static SQLiteColumn VdrRunStatusCols[VDR_RUNS_COLNUM_BUTT] = {
    SQLiteColumn("ColOccurTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColSpeed",           ENUM_SQLite_COLUMN_SEQUENCE,     true, false, false, false, false),
    SQLiteColumn("ColStatus",          ENUM_SQLite_COLUMN_SEQUENCE,     true, false, false, false, false)
};

const static SQLiteColumn VdrAccidentCols[VDR_ACCIDENT_COLNUM_BUTT] = {
    SQLiteColumn("ColOccurTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColSpeed",           ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColStatus",          ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColDriverLicenseNum",ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false),
    SQLiteColumn("ColLongitude",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLatitude",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColAltitude",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

const static SQLiteColumn VdrTiredCols[VDR_TIRED_COLNUM_BUTT] = {
    SQLiteColumn("ColBeginTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColBeginLongitude",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColBeginLatitude",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColBeginAltitude",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEndTime",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEndLongitude",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEndLatitude",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEndAltitude",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColDriverLicenseNum",ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false)
};

const static SQLiteColumn VdrPreTiredCols[VDR_PRETIRED_COLNUM_BUTT] = {
    SQLiteColumn("ColDriverLicenseNum",ENUM_SQLite_COLUMN_STRING,      true, false, false, false, true),
    SQLiteColumn("ColBeginTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColBeginLongitude",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColBeginLatitude",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColBeginAltitude",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEndTime",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEndLongitude",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEndLatitude",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEndAltitude",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColRunningTime",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColTripMeterCurDay", ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColRunTimeCurDay",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColFlagTired",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

const static SQLiteColumn VdrOffStatusCols[VDR_OFFS_COLNUM_BUTT] = {
    SQLiteColumn("ColAccOnTime",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColAccOffTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColTotalMileage",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColParkedDura",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
};

const static SQLiteColumn VdrDayInfoCols[VDR_DAY_COLNUM_BUTT] = {
    SQLiteColumn("ColOccurTime",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColDriveDura",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColDriveMileage",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColTotalMileage",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

const static SQLiteColumn VdrParamInfoCols[VDR_PARAM_COLNUM_BUTT] = {
    SQLiteColumn("ColParamIndex",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColOccurTime",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEventID",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

const static SQLiteColumn VdrSpeedInfoCols[VDR_SPEED_COLNUM_BUTT] = {
    SQLiteColumn("ColBeginTime",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColEndTime",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColSpeedStatus",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColVDRSpeed",         ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColGPSSpeed",         ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false)
};

VDRLocStatus1Table::VDRLocStatus1Table():
                  LargeTableBase(VDR_LOCATION_STATUS_1_DB_NAME,
                                 VDR_LOCATION_STATUS_1_TABLE_NAME, 
                                 (int)VDR_TRACK_LOCATION_STATUS1_TABLE_MAX_ROWS, 
                                 100,
                                 (int)VDR_LOCS_COLNUM_BUTT, 
                                 VdrLocStatusCols)
{
    ;
}

VDRLocStatus2Table::VDRLocStatus2Table():
                  LargeTableBase(VDR_LOCATION_STATUS_2_DB_NAME,
                                 VDR_LOCATION_STATUS_2_TABLE_NAME, 
                                 (int)VDR_TRACK_LOCATION_STATUS2_TABLE_MAX_ROWS, 
                                 100,
                                 (int)VDR_LOCS_COLNUM_BUTT, 
                                 VdrLocStatusCols)
{
    ;
}

VDRRunStatusTable::VDRRunStatusTable():
                  LargeTableBase(VDR_TRACK_RUN_STATUS_DB_NAME,
                                 VDR_TRACK_RUN_STATUS_TABLE_NAME, 
                                 (int)VDR_TRACK_RUN_STATUS_TABLE_MAX_ROWS, 
                                 100,
                                 (int)VDR_RUNS_COLNUM_BUTT, 
                                 VdrRunStatusCols)
{
    ;
}

VDRAccidentTable::VDRAccidentTable():
    MultiRowWithUniqueIndexTable(VDR_TRACK_DB_NAME,
                                 VDR_ACCIDENT_RECORD_TABLE_NAME, 
                                 (int)VDR_TRACK_ACCIDENT_TABLE_MAX_ROWS, 
                                 (int)VDR_ACCIDENT_COLNUM_BUTT, 
                                 VdrAccidentCols, 
                                 (int)VDR_ACCIDENT_OCCUR_TIME)
{
    ;
}

VDRTiredTable::VDRTiredTable():
    MultiRowWithUniqueIndexTable(VDR_TIRED_DB_NAME,
                                 VDR_TIRED_DRIVING_TABLE_NAME, 
                                 (int)VDR_TRACK_TIRED_DRIVING_TABLE_MAX_ROWS, 
                                 (int)VDR_TIRED_COLNUM_BUTT, 
                                 VdrTiredCols, 
                                 (int)VDR_TIRED_BEGIN_TIME)
{
    ;
}

VDRPreTiredTable::VDRPreTiredTable():
    MultiRowWithUniqueIndexTable(VDR_PRE_TIRED_DB_NAME,
                                 VDR_PRETIRED_DRIVING_TABLE_NAME, 
                                 (int)VDR_TRACK_PRETIRED_DRIVING_TABLE_MAX_ROWS, 
                                 (int)VDR_PRETIRED_COLNUM_BUTT, 
                                 VdrPreTiredCols, 
                                 (int)VDR_PRETIRED_DRIVER_LICENSE_NUM)
{
    ;
}

VDROFFStatusTable::VDROFFStatusTable():
    MultiRowWithUniqueIndexTable(VDR_OFF_STATUS_DB_NAME,
                                 VDR_OFF_STATUS_TABLE_NAME, 
                                 (int)VDR_TRACK_OFF_STATUS_TABLE_MAX_ROWS, 
                                 (int)VDR_OFFS_COLNUM_BUTT, 
                                 VdrOffStatusCols, 
                                 (int)VDR_OFFS_ACC_ON_TIME)
{
    ;
}

VDRDayInfoTable::VDRDayInfoTable():
    MultiRowWithUniqueIndexTable(VDR_DAY_INFO_DB_NAME,
                                 VDR_DAY_INFO_TABLE_NAME, 
                                 (int)VDR_TRACK_DAY_INFO_TABLE_MAX_ROWS, 
                                 (int)VDR_DAY_COLNUM_BUTT, 
                                 VdrDayInfoCols, 
                                 (int)VDR_DAY_OCCUR_TIME)
{
    ;
}

VDRParamInfoTable::VDRParamInfoTable():
    MultiRowWithUniqueIndexTable(VDR_LOG_INFO_DB_NAME,
                                 VDR_PARAM_INFO_TABLE_NAME, 
                                 (int)VDR_TRACK_PARAM_INFO_TABLE_MAX_ROWS, 
                                 (int)VDR_PARAM_COLNUM_BUTT, 
                                 VdrParamInfoCols, 
                                 (int)VDR_PARAM_INFO_INDEX)
{
    ;
}

VDRSpeedInfoTable::VDRSpeedInfoTable():
    MultiRowWithUniqueIndexTable(VDR_LOG_INFO_DB_NAME,
                                 VDR_SPEED_INFO_TABLE_NAME, 
                                 (int)VDR_TRACK_SPEED_INFO_TABLE_MAX_ROWS, 
                                 (int)VDR_SPEED_COLNUM_BUTT, 
                                 VdrSpeedInfoCols, 
                                 (int)VDR_SPEED_BEGIN_TIME)
{
    ;
}
    
