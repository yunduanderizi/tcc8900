#ifndef _VDR_PHONE_TABLE_H_
#define	_VDR_PHONE_TABLE_H_

#include "MultiRowWithUniqueIndexTable.h"
#include "TableNameDef.h"
#include "VDRContactsTable.h"

#define	ADD_DATA_FROM_SERVER_FLAG	(0)//for phonebook-database from server

#if ADD_DATA_FROM_SERVER_FLAG
	#define	NUMBER_FROM_LOCAL	(0)
	#define	NUMBER_FROM_SERVER	(1)
#endif

//#define	SMS_SEND_SUC	(1)
//#define	SMS_SEND_FAIL	(0)

using namespace std;


typedef enum
{
	VDR_PHONE_ID = 0,
	VDR_NAME,
	VDR_TELEPHONE,//mobile phone
	VDR_EMAIL,
	VDR_COMPANY,
	VDR_COM_ADDRESS,
	VDR_FAM_ADDRESS,
	VDR_PHOTO_PATH,
	VDR_PHONE,
	VDR_RING_PATH,
	VDR_VIDEO_FLAG,
	VDR_PHONEBOOK_COL_COUNTS,
}VDR_PHONE_BOOK_COL;

typedef enum
{
	VDR_RECIEVE_SMS_ID = 0,
	VDR_FROM_PHONE,
	VDR_TIME,
	VDR_CONTENT,
	VDR_POI_NAME,
	VDR_DLON,	//longitude
	VDR_DLAT,	//latitude
	VDR_STATUS,	//0:unread;1:read
	VDR_EVENT_TYPE,
	VDR_BOOK_ID,
	VDR_RECIEVE_SMS_COL_COUNTS,
}VDR_PHONE_RECIEVE_SMS_COL;

typedef enum
{
	VDR_SEND_SMS_ID = 0,
	VDR_TO_PHONENUMBER,
	VDR_SEND_TIME,
	VDR_TO_CONTENT,
	VDR_SEND_RESULT,
	VDR_SEND_SMS_BOOK_ID,
	VDR_SEND_SMS_COL_COUNTS,
}VDR_PHONE_SEND_SMS_COL;

typedef enum
{
	VDR_CALL_RECORD_ID = 0,
	VDR_CALL_TYPE,// 1:dialed; 2:missedcall; 3:answered
	VDR_PHONENUM,
	VDR_START_TIME,
	VDR_H_TIME,
	VDR_VIDEO_STATUS,
	VDR_RECORD_STATUS,//0:not read; 1:read
#if ADD_DATA_FROM_SERVER_FLAG
	VDR_NUMBER_FROM_SERVER,//0:from local; 1:from server
#endif
	VDR_RECORD_BOOK_ID,
	VDR_CALL_RECORD_COL_COUNTS,
}VDR_PHONE_CALL_RECORD_COL;

class VDRPhoneTableBase : public MultiRowWithUniqueIndexTable
{
	protected:
		VDRPhoneTableBase(string dbName, string tableName, int maxrownum, int colnum, const SQLiteColumn *col, int uniquecol, bool indexFlag = true, int cache = 0) :
        MultiRowWithUniqueIndexTable(dbName, tableName, maxrownum, colnum, col,uniquecol,indexFlag,cache)
		{
		}

	public:
		int RefreshFirstByRowId(bool asc = true);//true:earliest; false:latest
		int RefreshByCol(int colId,bool asc = true);
		int RefreshByEqualToStr(int colId,string str);

};
class VDRPhonePhonebookTable : public VDRPhoneTableBase
{
	public:
		VDRPhonePhonebookTable();
};

class VDRPhoneRecieveSMSTable : public VDRPhoneTableBase
{
	public:
		VDRPhoneRecieveSMSTable();
};

class VDRPhoneSendSMSTable : public VDRPhoneTableBase
{
	public:
		VDRPhoneSendSMSTable();
};

class VDRPhoneCallRecordTable : public VDRPhoneTableBase
{
	public:
		VDRPhoneCallRecordTable();
        int RefreshByCallType(int calltype, bool asc = true);
        int DeleteByCallType(int calltype);
};

class VDRPhonePhonebookFromServerTable : public VDRContactsTable
{
	public:
		VDRPhonePhonebookFromServerTable():VDRContactsTable()
		{
		}

	public:
		int RefreshAll(void);
};


#endif

