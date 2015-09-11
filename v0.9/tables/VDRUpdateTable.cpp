#include "LogUtility.h"
#include "VDRUpdateTable.h"

const static unsigned char manufactoryID[] = {0x37, 0x30, 0x33, 0x31, 0x39};

const static SQLiteColumn UpdateCols[VDR_UPDATE_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColStartTime",               ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColUpdateType",            ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColManufactoryId",     	ENUM_SQLite_COLUMN_SEQUENCE,   true, false, false, false, false),
    SQLiteColumn("ColVersion",                  ENUM_SQLite_COLUMN_STRING,       true, false, false, false, false),
    SQLiteColumn("ColUpdatePackageUrl",   ENUM_SQLite_COLUMN_STRING,       true, false, false, false, false),
    SQLiteColumn("ColUpdateResult",          ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColUpdateFinishTime",  	ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};


VDRUpdateTable::VDRUpdateTable():
    MultiRowWithUniqueIndexTable(VDR_UPDATE_DB_NAME,
                                 VDR_UPDATE_TABEL_NAME, 
                                 (int)VDR_UPDATE_TABLE_MAX_ROWS, 
                                 (int)VDR_UPDATE_COLNUM_BUTT, 
                                 UpdateCols, 
                                 (int)VDR_UPDATE_START_TIME)
{
    ;
}