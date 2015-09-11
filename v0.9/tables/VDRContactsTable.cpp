#include "VDRContactsTable.h"

const static SQLiteColumn VdrContactsCols[VDR_CONTACTS_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColFlag",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(3)),
    SQLiteColumn("ColPhoneNumber",    ENUM_SQLite_COLUMN_STRING,      true, false, false, false, true),
    SQLiteColumn("ColName",           ENUM_SQLite_COLUMN_STRING,      true, false, false, false, true)
};

VDRContactsTable::VDRContactsTable() :
    MultiRowWithUniqueIndexTable(VDR_CONTACTS_DB_NAME,
                                 VDR_CONTACTS_TABLE_NAME, 
                                 (int)VDR_CONTACTS_TABLE_MAX_ROWS, 
                                 (int)VDR_CONTACTS_COLNUM_BUTT, 
                                 VdrContactsCols, 
                                 (int)VDR_CONTACTS_NAME)

{
    ;
}
