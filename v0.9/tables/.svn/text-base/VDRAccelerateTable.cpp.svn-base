#include "VDRAccelerateTable.h"

const static SQLiteColumn VDRAccelerationCols[VDR_ACCELERATION_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColOffsetAngle",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColSideTurnAngle",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(30)),
    SQLiteColumn("ColCollideMin",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(50)),
	SQLiteColumn("ColCollideTime",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(250))
};

VDRAccelerationTable::VDRAccelerationTable(): SingleRowTable(VDR_ACCELERATION_DB_NAME, VDR_ACCELERATION_TABLE_NAME, 1, (int)VDR_ACCELERATION_BUTT, VDRAccelerationCols)
{
    ;
}


