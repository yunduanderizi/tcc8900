#ifndef VDRJT8080_TEXT_RECORD_TABLE_H
#define VDRJT8080_TEXT_RECORD_TABLE_H

#include "VDRPhoneTable.h"
#include "MultiRowWithUniqueIndexTable.h"
#include "TableNameDef.h"

using namespace std;

typedef enum
{
        VDR_JT808_TEXT_ID = 0,
        VDR_JT808_TEXT_TIME,
        VDR_JT808_TEXT_EMERGENCY,
        VDR_JT808_TEXT_CONTENT,
        VDR_JT808_TEXT_COUNTS,
}VDR_JT808_TEXT_RECORD_COL;

class VDRJT808TextRecordTable : public VDRPhoneTableBase
{
        public:
                VDRJT808TextRecordTable();
};
#endif

