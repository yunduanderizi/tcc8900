#include "VDREventsTable.h"

const static SQLiteColumn VdrEventsCols[VDR_EVENTS_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColId",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn("ColContents",    ENUM_SQLite_COLUMN_STRING,      true, false, false, false, true),
};

VDREventsTable::VDREventsTable() :
    MultiRowWithUniqueIndexTable(VDR_EVENTS_DB_NAME,
                                 VDR_EVENTS_TABLE_NAME, 
                                 (int)VDR_EVENTS_TABLE_MAX_ROWS, 
                                 (int)VDR_EVENTS_COLNUM_BUTT, 
                                 VdrEventsCols, 
                                 (int)VDR_EVENTS_ID)

{
    ;
}
