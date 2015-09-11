#include "DriversLoginTable.h"

const static unsigned char validdate[] = {0x20, 0x07, 0x12};

const static SQLiteColumn LoginCols[VDR_DRV_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColOccurTime",            ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, true),
    SQLiteColumn("ColLoginLogout",          ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, false,SQLiteValue(0)),
    SQLiteColumn("ColDriverLicenseNum",     ENUM_SQLite_COLUMN_STRING,     true, false, false, false, false,SQLiteValue("000000000000000000")),
    SQLiteColumn("ColVaildDate",            ENUM_SQLite_COLUMN_SEQUENCE,   true, false, false, false, false,SQLiteValue(vector<unsigned char>(validdate, validdate+ sizeof(validdate)))),
    SQLiteColumn("ColQualificationNum",     ENUM_SQLite_COLUMN_STRING,     true, false, false, false, false,SQLiteValue("00000000000000000000")),
    SQLiteColumn("ColDriverName",           ENUM_SQLite_COLUMN_STRING,     true, false, false, false, false,SQLiteValue("驾驶员")),
    SQLiteColumn("ColCertifyingAuthority",  ENUM_SQLite_COLUMN_STRING,     true, false, false, false, false,SQLiteValue("发证机构")),
    SQLiteColumn("ColStartDriveringTime",   ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, false,SQLiteValue(0)),
    SQLiteColumn("ColStopDriveringTime",    ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, false,SQLiteValue(0)),
    SQLiteColumn("ColIccardReadResult",    ENUM_SQLite_COLUMN_INTEGER,    true, false, false, false, false,SQLiteValue(0))
};

DriversLoginTable::DriversLoginTable():
    MultiRowWithUniqueIndexTable(VDR_DRIVER_INFO_DB_NAME,
                                 VDR_DRIVER_LOGIN_TABLE_NAME, 
                                 (int)VDR_DRIVER_LOGIN_TABLE_MAX_ROWS, 
                                 (int)VDR_DRV_COLNUM_BUTT, 
                                 LoginCols, 
                                 (int)VDR_DRV_OCCUR_TIME)
{
    ;
}
