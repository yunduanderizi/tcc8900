#include "LogUtility.h"
#include "VDRPreTiredTable.h"

const static SQLiteColumn VdrHyPreTiredCols[VDR_HY_PRETIRED_COLNUM_BUTT] = {
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
    SQLiteColumn("ColRuntotalTime",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColTripMeterCurDay", ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColRunTimeCurDay",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColFlagTired",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

VDRHyPreTiredTable::VDRHyPreTiredTable():
    MultiRowWithUniqueIndexTable(VDR_HY_PRE_TIRED_DB_NAME,
                                 VDR_HY_PRETIRED_DRIVING_TABLE_NAME, 
                                 (int)VDR_TRACK_PRETIRED_DRIVING_TABLE_MAX_ROWS, 
                                 (int)VDR_HY_PRETIRED_COLNUM_BUTT, 
                                 VdrHyPreTiredCols, 
                                 (int)VDR_HY_PRETIRED_DRIVER_LICENSE_NUM)
{
    ;
}

    
