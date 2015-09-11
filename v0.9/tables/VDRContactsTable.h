#ifndef _VDR_CONTACTS_TABLE_H_
#define _VDR_CONTACTS_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
    VDR_CONTACTS_FLAG = 0,
    VDR_CONTACTS_PHONE_NUMBER,
    VDR_CONTACTS_NAME,
    VDR_CONTACTS_COLNUM_BUTT
}ENUM_VDR_CONTACTS_COL;

class VDRContactsTable : public MultiRowWithUniqueIndexTable
{
public:
    VDRContactsTable();
};


#endif
