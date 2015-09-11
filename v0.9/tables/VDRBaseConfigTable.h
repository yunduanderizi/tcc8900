#ifndef _VDR_BASE_CONFIG_TABLE_H_
#define _VDR_BASE_CONFIG_TABLE_H_

#include "SingleRowTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_BASE_CONFIG_D0 = 0,
    VDR_BASE_CONFIG_D1,
    VDR_BASE_CONFIG_D2,
    VDR_BASE_CONFIG_D3,
    VDR_BASE_CONFIG_D4,
    VDR_BASE_CONFIG_D5,
    VDR_BASE_CONFIG_D6,
    VDR_BASE_CONFIG_D7,
    VDR_BASE_STATUS_D0,
    VDR_BASE_STATUS_D1,
    VDR_BASE_STATUS_D2,
    VDR_BASE_STATUS_D3,
    VDR_BASE_STATUS_D4,
    VDR_BASE_STATUS_D5,
    VDR_BASE_STATUS_D6,
    VDR_BASE_STATUS_D7,    
    VDR_BASE_CONFIG_BUTT
}ENUM_VDR_BASE_CONFIG_COL;

class VDRBaseConfigTable : public SingleRowTable
{
public:
    VDRBaseConfigTable();
};

#endif
