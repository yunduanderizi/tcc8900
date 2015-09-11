#ifndef _DRIVERS_LOGIN_TABLE_H_
#define _DRIVERS_LOGIN_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_DRV_OCCUR_TIME = 0,
    VDR_DRV_LOGIN_LOGOUT,
    VDR_DRV_DRIVER_LICENSE_NUM,
    VDR_DRV_VAILD_DATA,
    VDR_DRV_QUALIFICATION_NUM,
    VDR_DRV_DRIVER_NAME,
    VDR_DRV_CERTIFYING_AUTHORITY,
    VDR_DRV_START_DRIVERING_TIME ,
    VDR_DRV_STOT_DRIVERING_TIME ,
    VDR_DRV_IC_CARD_READ_RESULT,
    VDR_DRV_COLNUM_BUTT
}ENUM_VDR_LOGIN_COL;

class DriversLoginTable : public MultiRowWithUniqueIndexTable
{
public:
    DriversLoginTable();
};


#endif
