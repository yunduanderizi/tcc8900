#include "ElectronicFenceTable.h"

const static SQLiteColumn VdrCircleCols[VDR_CIRCLE_COLNUM_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn("ColRegionID",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColRegionAttribute",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColCenterLatitude",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColCenterLongitude",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColRadius",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColBeginTime",        ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColEndTime",          ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColMaxSpeed",         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOverSpeedDura",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

const static SQLiteColumn VdrRectangleCols[VDR_RECT_COLNUM_BUTT] = {
    SQLiteColumn("ColRegionID",             ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColRegionAttribute",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColUpperLeftLatitude",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColUpperLeftLongitude",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLowerRightLatitude",   ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColLowerRightLongitude",  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColBeginTime",            ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColEndTime",              ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColMaxSpeed",             ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOverSpeedDura",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

const static SQLiteColumn VdrPolygonCols[VDR_POL_COLNUM_BUTT] = {
    SQLiteColumn("ColRegionID",             ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColRegionAttribute",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColBeginTime",            ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColEndTime",              ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColMaxSpeed",             ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColOverSpeedDura",        ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColPointCount",           ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColPointLatLong",         ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false)
};

const static SQLiteColumn VdrLineCols[VDR_LINE_COLNUM_BUTT] = {
    SQLiteColumn("ColRegionID",             ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
    SQLiteColumn("ColRegionAttribute",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColBeginTime",            ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColEndTime",              ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false),
    SQLiteColumn("ColInflexionCount",       ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),
    SQLiteColumn("ColInflexionInfo",        ENUM_SQLite_COLUMN_SEQUENCE,    true, false, false, false, false)
};

VDRCircleFenceTable::VDRCircleFenceTable():
    MultiRowWithUniqueIndexTable(VDR_ELECTRONIC_FENCE_DB_NAME,
                                 VDR_CIRCLE_FENCE_TABLE_NAME, 
                                 (int)VDR_FENCE_CIRCLE_TABLE_MAX_ROWS, 
                                 (int)VDR_CIRCLE_COLNUM_BUTT, 
                                 VdrCircleCols, 
                                 (int)VDR_CIRCLE_REGION_ID)
{
    ;
}

VDRRectangleFenceTable::VDRRectangleFenceTable():
    MultiRowWithUniqueIndexTable(VDR_ELECTRONIC_FENCE_DB_NAME,
                                 VDR_RECTANGLE_FENCE_TABLE_NAME, 
                                 (int)VDR_FENCE_RECTANGLE_TABLE_MAX_ROWS, 
                                 (int)VDR_RECT_COLNUM_BUTT, 
                                 VdrRectangleCols, 
                                 (int)VDR_RECT_REGION_ID)
{
    ;
}

VDRPolygonFenceTable::VDRPolygonFenceTable():
    MultiRowWithUniqueIndexTable(VDR_ELECTRONIC_FENCE_DB_NAME,
                                 VDR_POLYGON_FENCE_TABLE_NAME, 
                                 (int)VDR_FENCE_POLYGON_TABLE_MAX_ROWS, 
                                 (int)VDR_POL_COLNUM_BUTT, 
                                 VdrPolygonCols, 
                                 (int)VDR_POL_REGION_ID)
{
    ;
}

VDRLineFenceTable::VDRLineFenceTable():
    MultiRowWithUniqueIndexTable(VDR_ELECTRONIC_FENCE_DB_NAME,
                                 VDR_LINE_FENCE_TABLE_NAME, 
                                 (int)VDR_FENCE_LINE_TABLE_MAX_ROWS, 
                                 (int)VDR_LINE_COLNUM_BUTT, 
                                 VdrLineCols, 
                                 (int)VDR_LINE_REGION_ID)
{
    ;
}
