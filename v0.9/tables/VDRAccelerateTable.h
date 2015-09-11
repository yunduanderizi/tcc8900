#ifndef _VDR_ACCELERATION_TABLE_H_
#define _VDR_ACCELERATION_TABLE_H_

#include "SingleRowTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_ACCELERATION_OFFSET_ANGLE = 0,
    VDR_ACCELERATION_SIDE_TURN_ANGLE,
    VDR_ACCELERATION_COLLIDE_MIN,
    VDR_ACCELERATION_COLLIDE_TIME,
    VDR_ACCELERATION_BUTT
}ENUM_VDR_ACCELERATION_COL;


class VDRAccelerationTable : public SingleRowTable
{
public:
    VDRAccelerationTable();
};

#endif
