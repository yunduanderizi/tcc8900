#ifndef _ELECTRONIC_FENCE_TABLE_H_
#define _ELECTRONIC_FENCE_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_CIRCLE_REGION_ID = 0,
    VDR_CIRCLE_REGION_ATTRIBUTE,
    VDR_CIRCLE_CENTER_LATITUDE,
    VDR_CIRCLE_CENTER_LONGITUDE,
    VDR_CIRCLE_RADIUS,
    VDR_CIRCLE_BEGIN_TIME,
    VDR_CIRCLE_END_TIME,
    VDR_CIRCLE_MAX_SPEED,
    VDR_CIRCLE_OVER_SPEED_DURA,
    VDR_CIRCLE_COLNUM_BUTT
}ENUM_VDR_CIRCLE_COL;

typedef enum
{
    VDR_RECT_REGION_ID = 0,
    VDR_RECT_REGION_ATTRIBUTE,
    VDR_RECT_UPLEFT_LATITUDE,
    VDR_RECT_UPLEFT_LONGITUDE,
    VDR_RECT_LOWRIGTH_LATITUDE,
    VDR_RECT_LOWRIGHT_LONGITUDE,
    VDR_RECT_BEGIN_TIME,
    VDR_RECT_END_TIME,
    VDR_RECT_MAX_SPEED,
    VDR_RECT_OVER_SPEED_DURA,
    VDR_RECT_COLNUM_BUTT
}ENUM_VDR_RECT_COL;

typedef enum
{
    VDR_POL_REGION_ID = 0,
    VDR_POL_REGION_ATTRIBUTE,
    VDR_POL_BEGIN_TIME,
    VDR_POL_END_TIME,
    VDR_POL_MAX_SPEED,
    VDR_POL_OVER_SPEED_DURA,
    VDR_POL_POINT_COUNT,
    VDR_POL_POINT_LATLONG,
    VDR_POL_COLNUM_BUTT
}ENUM_VDR_POLYGON_COL;

typedef enum
{
    VDR_LINE_REGION_ID = 0,
    VDR_LINE_REGION_ATTRIBUTE,
    VDR_LINE_BEGIN_TIME,
    VDR_LINE_END_TIME,
    VDR_LINE_INFLEXION_COUNT,
    VDR_LINE_INFLEXION_INFO,
    VDR_LINE_COLNUM_BUTT
}ENUM_VDR_LINE_COL;

class VDRCircleFenceTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRCircleFenceTable();
};

class VDRRectangleFenceTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRRectangleFenceTable();
};

class VDRPolygonFenceTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRPolygonFenceTable();
};

class VDRLineFenceTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRLineFenceTable();
};

#endif
