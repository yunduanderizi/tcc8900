#ifndef _VDR_UPDATE_TABLE_H_
#define _VDR_UPDATE_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_UPDATE_START_TIME = 0,
    VDR_UPDATE_TYPE,
    VDR_UPDATE_MANUFACTORY_ID,
    VDR_UPDATE_VERSION,
    VDR_UPDATE_PACKAGE_URL,
    VDR_UPDATE_RESULT,
    VDR_UPDATE_FINISH_TIME,
    VDR_UPDATE_COLNUM_BUTT
}ENUM_UPDATE_COL;

class VDRUpdateTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRUpdateTable();
};


#endif
