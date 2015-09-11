#include "VDRRunParaTable.h"

const static SQLiteColumn VdrParaCols[VDR_PARA_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColHeartBeatInterval",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(30)),
    SQLiteColumn("ColTcpReplyTimeOut",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(15)),
    SQLiteColumn("ColTcpRetryTimes",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(3)),
    SQLiteColumn("ColUdpReplyTimeOut",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(15)),
    SQLiteColumn("ColUdpRetryTimes",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(3)),
    SQLiteColumn("ColSmsReplyTimeOut",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(60)),
    SQLiteColumn("ColSmsRetryTimes",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(3)),
    SQLiteColumn("ColMainServerApn",        ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("null")),
    SQLiteColumn("ColMainServerUserName",   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("null")),
    SQLiteColumn("ColMainServerPassword",   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("null")),
    SQLiteColumn("ColMainServerAddress",    ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("111.75.223.134")),
    SQLiteColumn("ColSecondServerApn",      ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("null")),
    SQLiteColumn("ColSecondServerUserName", ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("null")),
    SQLiteColumn("ColSecondServerPassword", ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("null")),
    SQLiteColumn("ColSecondServerAddress",  ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("111.75.223.134")),
    SQLiteColumn("ColTcpPort",              ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(9094)),
    SQLiteColumn("ColUdpPort",              ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(9094)),
    SQLiteColumn("ColLocReportPolicy",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColLocReportSenario",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColNologinReportInterv",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(60)),
    SQLiteColumn("ColSleepReportInterv",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1800)),
    SQLiteColumn("ColEmergeReportInterv",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(30)),
    SQLiteColumn("ColDefaultReportInterv",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(60)),
    SQLiteColumn("ColDefaultReportDist",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1000)),
    SQLiteColumn("ColNologinReportDist",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(500)),
    SQLiteColumn("ColSleepReportDist",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1000)),
    SQLiteColumn("ColEmergReportDist",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(500)),
    SQLiteColumn("ColTurnAngle",            ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(45)),
    SQLiteColumn("ColPlatformTel",          ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("01088892871")),
    SQLiteColumn("ColResetTel",             ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("01088892871")),
    SQLiteColumn("ColFactoryResetTel",      ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("01088892871")),
    SQLiteColumn("ColPlatformSms",          ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("01088892871")),
    SQLiteColumn("ColReportSms",            ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("01088892871")),
    SQLiteColumn("ColCallAnswerPolicy",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1)),
    SQLiteColumn("ColCallDuraOnce",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(60)),
    SQLiteColumn("ColCallDuraMonth",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(3600)),
    SQLiteColumn("ColMonitorTel",           ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("01088892871")),
    SQLiteColumn("ColMonitorSms",           ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("01088892871")),
    SQLiteColumn("ColWarningMask",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColWarningSmsMask",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColWarningPhotoMask",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColWarningPhotoSaveMask", ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColWarningKeyMask",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColMaxSpeed",             ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(80)),
    SQLiteColumn("ColOverSpeedDura",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(10)),
    SQLiteColumn("ColContDriveThres",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(14400)),
    SQLiteColumn("ColDayDriveThres",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(28800)),
    SQLiteColumn("ColRestMin",              ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1200)),
    SQLiteColumn("ColParkMax",              ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(18000)),
    SQLiteColumn("ColPhotoQuality",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(5)),
    SQLiteColumn("ColPhotoBrightness",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(127)),
    SQLiteColumn("ColPhotoContrast",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(63)),
    SQLiteColumn("ColPhotoSaturation",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(63)),
    SQLiteColumn("ColPhotoChroma",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(127)),
    SQLiteColumn("ColFenceRadius",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColIcMainServerIpAddr",   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("111.75.223.134")),
    SQLiteColumn("ColIcMainServerTcpPort",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(9096)),
    SQLiteColumn("ColIcMainServerUdpPort",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(9096)),
    SQLiteColumn("ColIcSecondServerIpAddr", ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("111.75.223.134")),
    SQLiteColumn("ColOverspeedWarning",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(20)),
    SQLiteColumn("ColTiredDriveWarning",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(300)),
    SQLiteColumn("ColCollisionAlarm",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(2760)),
    SQLiteColumn("ColRolloverAlarm",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(30)),
    SQLiteColumn("ColTimeCapture",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColDistanceCapture",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColGnssLocateMode",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(3)),
    SQLiteColumn("ColGnssBaudRate",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1)),
    SQLiteColumn("ColGnssDataOutRate",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1)),
    SQLiteColumn("ColGnssDataCollectRate",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1)),
    SQLiteColumn("ColGnssDataUploadMode",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColGnssDataUploadSet",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1800)),
    SQLiteColumn("ColCan1CollectInterv",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColCan1UploadInterv",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColCan2CollectInterv",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColCan2UploadInterv",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColCanIdCollectSet",      ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColOtherCanIdCollectSet", ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false, SQLiteValue(0))
};

VDRRunParaTable::VDRRunParaTable(): SingleRowTable(VDR_SETTING_DB_NAME, VDR_RUN_PARA_TABLE_NAME, 1, (int)VDR_PARA_BUTT, VdrParaCols)
{
    ;
}

const static SQLiteColumn VdrOccupationCheckCols[VDR_CHECK_BUTT] = {
    SQLiteColumn("ColOccupationCheckTel",   ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false, SQLiteValue("15697924841"))
};
VDROccupationCheckTable::VDROccupationCheckTable():SingleRowTable(VDR_OCCUPATION_CHECK_DB_NAME, VDR_OCCUPATION_CHECK_TABLE_NAME, 1, (int)VDR_CHECK_BUTT, VdrOccupationCheckCols)
{
    ;
}


