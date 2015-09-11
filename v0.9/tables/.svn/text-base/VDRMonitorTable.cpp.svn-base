#include "VDRMonitorTable.h"

const static SQLiteColumn VdrRecordCols[VDR_REC_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColFilePath",       ENUM_SQLite_COLUMN_STRING,      true, false, false, false, true),
    SQLiteColumn("ColFileTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColFileType",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColRecordDuration", ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColEvent",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColSampleRate",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColAlarmSignal",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColStatusFlag",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLatitude",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLongitude",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColAltitude",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOrientation",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColGPSSpeed",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOccurTime",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

const static SQLiteColumn VdrCaptureCols[VDR_CAP_COLNUM_BUTT] = {
    SQLiteColumn("ColFilePath",       ENUM_SQLite_COLUMN_STRING,      true, false, false, false, true),
    SQLiteColumn("ColFileTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColFileType",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColFileQuality",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColResolution",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(1)),
    SQLiteColumn("ColBrightness",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(128)),
    SQLiteColumn("ColContrast",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(64)),
    SQLiteColumn("ColSaturation",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(64)),
    SQLiteColumn("ColChroma",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(128)),
    SQLiteColumn("ColEvent",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColChanID",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColAlarmSignal",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColStatusFlag",     ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLatitude",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLongitude",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColAltitude",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOrientation",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColGPSSpeed",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOccurTime",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

VDRRecordTable::VDRRecordTable() :
    MultiRowWithUniqueIndexTable(VDR_MONITOR_DB_NAME,
                                 VDR_RECORD_INFO_TABLE_NAME, 
                                 (int)VDR_MONITOR_RECORD_TABLE_MAX_ROWS, 
                                 (int)VDR_REC_COLNUM_BUTT, 
                                 VdrRecordCols, 
                                 (int)VDR_REC_FILE_TIME)

{
    ;
}

VDRCaptureTable::VDRCaptureTable() :
    MultiRowWithUniqueIndexTable(VDR_MONITOR_DB_NAME,
                                 VDR_CAPTURE_INFO_TABLE_NAME, 
                                 (int)VDR_MONITOR_CAPTURE_TABLE_MAX_ROWS, 
                                 (int)VDR_CAP_COLNUM_BUTT, 
                                 VdrCaptureCols, 
                                 (int)VDR_CAP_FILE_TIME)

{
    ;
}
