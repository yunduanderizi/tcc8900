#ifndef _VDR_DEPLOY_PARA_TABLE_H_
#define _VDR_DEPLOY_PARA_TABLE_H_

#include "SingleRowTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_DEPLOY_PARA_PROVINCE_ID = 0,
    VDR_DEPLOY_PARA_CITY_ID,
    VDR_DEPLOY_PARA_MANUFACTORY_ID,
    VDR_DEPLOY_PARA_MODEL_NO,
    VDR_DEPLOY_PARA_ID,
    VDR_DEPLOY_PARA_PLATE_COLOR,
    VDR_DEPLOY_PARA_PLATE_ID,
    VDR_DEPLOY_PARA_PLATE_SORT,
    VDR_DEPLOY_PARA_CAR_VIN,
    VDR_DEPLOY_PARA_VERSION,
    VDR_DEPLOY_PARA_DEPLOY_TIME,
    VDR_DEPLOY_PARA_DEPLOY_MILEAGE,
    VDR_DEPLOY_PARA_SPEED_PULSE_FACTOR,
    VDR_DEPLOY_PARA_LAP_PULSE_FACTOR,
    VDR_DEPLOY_PARA_PHONE_NUMBER,
    VDR_DEPLOY_PARA_AUTH_TOKEN,
    VDR_DEPLOY_PARA_VDR_TYPE,
    VDR_DEPLOY_PARA_VDR_SIM_ICCID,
    VDR_DEPLOY_PARA_HARDWARE_VERSION,
    VDR_DEPLOY_PARA_GNSS_ATTRIBUTE,
    VDR_DEPLOY_PARA_COMMUNICATE_ATTRIBUTE,
    VDR_DEPLOY_PARA_BUTT
}ENUM_VDR_DEPLOY_PARA_COL;

class VDRDeployParaTable : public SingleRowTable
{
public:
    VDRDeployParaTable();
};

typedef enum
{
    VDR_DEPLOY_STATUS_D0 = 0,
    VDR_DEPLOY_STATUS_D1,
    VDR_DEPLOY_STATUS_D2,
    VDR_DEPLOY_STATUS_D3,
    VDR_DEPLOY_STATUS_D4,
    VDR_DEPLOY_STATUS_D5,
    VDR_DEPLOY_STATUS_D6,
    VDR_DEPLOY_STATUS_D7,
    VDR_DEPLOY_STATUS_ENABLE, // For each bit, 1 means the status bit is enabled.
    VDR_DEPLOY_STATUS_BITSET, // For each bit, 1 means the status is on when level is high.
    VDR_DEPLOY_STATUS_BUTT
}ENUM_VDR_DEPLOY_STATUS_COL;

class VDRDeployStatusConfigTable : public SingleRowTable
{
public:
    VDRDeployStatusConfigTable();
};

typedef enum
{
        VDR_STU_SERVER_ADDRESS = 0,
        VDR_STU_SERVER_BUTT
}ENUM_VDR_STU_SERVER_COL;

class VDRStuServerTable : public SingleRowTable
{
public:
        VDRStuServerTable();
};


#endif
