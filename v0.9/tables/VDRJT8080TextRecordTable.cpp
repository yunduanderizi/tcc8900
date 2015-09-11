#include "VDRJT8080TextRecordTable.h"
const static SQLiteColumn VDRJT808TextRecordCols[VDR_JT808_TEXT_COUNTS] = 
{
    SQLiteColumn("id",                ENUM_SQLite_COLUMN_INTEGER,             true, false, false, false, true),
    SQLiteColumn("time",              ENUM_SQLite_COLUMN_STRING,              true, false, false, false, false),
    SQLiteColumn("emergency",         ENUM_SQLite_COLUMN_INTEGER,             true, false, false, false, false),
    SQLiteColumn("content",           ENUM_SQLite_COLUMN_STRING,              true, false, false, false, false),
};

VDRJT808TextRecordTable::VDRJT808TextRecordTable():
        VDRPhoneTableBase       (       VDR_JT808TEXT_DB_NAME,
                                                        VDR_JT808TEXT_TABLE_NAME,
                                                        (int)VDR_JT808_TEXT_TABLE_MAX_ROWS,
                                                        (int)VDR_JT808_TEXT_COUNTS,
                                                        VDRJT808TextRecordCols,
                                                        (int)VDR_JT808_TEXT_ID
                                                )
{
}


