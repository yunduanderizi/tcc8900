#include "LogUtility.h"
#include "VDRPhoneTable.h"
//#include "MultiRowWithUniqueIndexTable.h"
//#include "TableNameDef.h"

const static SQLiteColumn VDRPhonePhonebookCols[VDR_PHONEBOOK_COL_COUNTS] = 
{
    SQLiteColumn("id",				ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, true),
    SQLiteColumn("name",			ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("telephone",		ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("email",			ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("company",			ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("corAddress",		ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("famAddress",		ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("photoPath",		ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("phone",			ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
	SQLiteColumn("ringPath",		ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("videoFlag",		ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
};


const static SQLiteColumn VDRPhoneRecieveSMSCols[VDR_RECIEVE_SMS_COL_COUNTS] = 
{
    SQLiteColumn("id",				ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, true),
    SQLiteColumn("fromPhone",		ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("recieveTime",		ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("content",			ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("poiName",			ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("longitude",		ENUM_SQLite_COLUMN_FLOAT,		true, false, false, false, false),
    SQLiteColumn("latitude",		ENUM_SQLite_COLUMN_FLOAT,		true, false, false, false, false),
    SQLiteColumn("status",			ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
    SQLiteColumn("eventType",		ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
    SQLiteColumn("bookId",			ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
	
};


const static SQLiteColumn VDRPhoneSendSMSCols[VDR_SEND_SMS_COL_COUNTS] = 
{
    SQLiteColumn("id",				ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, true),
    SQLiteColumn("toPhoneNumber",	ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("sendTime",		ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("content",			ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("sendResult",		ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
    SQLiteColumn("bookId",			ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
	
};

const static SQLiteColumn VDRPhoneCallRecordCols[VDR_CALL_RECORD_COL_COUNTS] = 
{
    SQLiteColumn("id",				ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, true),
    SQLiteColumn("callType",		ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
    SQLiteColumn("phoneNum",		ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("startTime",		ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("hTime",			ENUM_SQLite_COLUMN_STRING,		true, false, false, false, false),
    SQLiteColumn("videoFlag",		ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
    SQLiteColumn("status",			ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
#if ADD_DATA_FROM_SERVER_FLAG
    SQLiteColumn("fromServerFlag",	ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
#endif
    SQLiteColumn("bookId",			ENUM_SQLite_COLUMN_INTEGER,		true, false, false, false, false),
};

VDRPhonePhonebookTable::VDRPhonePhonebookTable():
	VDRPhoneTableBase	(	VDR_PHONEBOOK_DB_NAME,
							VDR_PHONEBOOK_TABLE_NAME,
							(int)VDR_PHONE_PHONEBOOK_TABLE_MAX_ROWS,
							(int)VDR_PHONEBOOK_COL_COUNTS,
							VDRPhonePhonebookCols,
							(int)VDR_PHONE_ID
						)
{
	;
}

VDRPhoneRecieveSMSTable::VDRPhoneRecieveSMSTable():
	VDRPhoneTableBase	(	VDR_PHONE_RECEIVE_SMS_DB_NAME,
							VDR_PHONE_RECEIVE_SMS_TABEL_NAME,
							(int)VDR_PHONE_RECIEVE_SMS_TABLE_MAX_ROWS,
							(int)VDR_RECIEVE_SMS_COL_COUNTS,
							VDRPhoneRecieveSMSCols,
							(int)VDR_RECIEVE_SMS_ID
						)
{
}

VDRPhoneSendSMSTable::VDRPhoneSendSMSTable():
	VDRPhoneTableBase	(	VDR_PHONE_SEND_SMS_DB_NAME,
							VDR_PHONE_SEND_SMS_TABEL_NAME,
							(int)VDR_PHONE_SEND_SMS_TABLE_MAX_ROWS,
							(int)VDR_SEND_SMS_COL_COUNTS,
							VDRPhoneSendSMSCols,
							(int)VDR_SEND_SMS_ID
						)
{
}

VDRPhoneCallRecordTable::VDRPhoneCallRecordTable():
	VDRPhoneTableBase	(	VDR_PHONE_CALL_RECORD_DB_NAME,
							VDR_PHONE_CALL_RECORD_TABEL_NAME,
							(int)VDR_PHONE_CALL_RECORD_TABLE_MAX_ROWS,
							(int)VDR_CALL_RECORD_COL_COUNTS,
							VDRPhoneCallRecordCols,
							(int)VDR_CALL_RECORD_ID
						)
{
}

int VDRPhoneCallRecordTable::RefreshByCallType(int calltype, bool asc)
{
    SQLiteValuePair constrain;
    set<string> results;
    const SQLiteColumn &col = m_col[(int)VDR_CALL_TYPE];
    const SQLiteColumn &order_col = m_col[(int)VDR_CALL_RECORD_ID];

    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::RefreshByCallType open table failed.");
        return -1;
    }
    
    m_returnResults.clear();
    results.clear();

    constrain[col.GetHead()] = calltype;

    if(m_table.Query(constrain, results, m_returnResults, order_col.GetHead(), asc))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::RefreshByCallType query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    return 0;
}

int VDRPhoneCallRecordTable::DeleteByCallType(int calltype)
{
    SQLiteValuePair constrain;
    const SQLiteColumn &col = m_col[(int)VDR_CALL_TYPE];

    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::DeleteByCallType open table failed.");
        return -1;
    }

    constrain[col.GetHead()] = calltype;

    if(m_table.Delete(constrain))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::DeleteByCallType delete failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    return 0;
}

int VDRPhoneTableBase::RefreshFirstByRowId(bool asc)
{
    set<string> results;
	SQLiteValuePair returnResult;

    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::RefreshFirstByRowId open table failed.");
        return -1;
    }
    
    results.clear();
	returnResult.clear();


    if(m_table.QueryTopBottom("rowid", results, returnResult, asc) )
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::RefreshFirstByRowId query failed.");
        m_table.Close();
        return -1;
    }
	else
	{
		m_values.clear();
    	m_returnResults.clear();
		m_values = returnResult;
	}

    m_table.Close();
    
    return 0;
	
}

int VDRPhoneTableBase::RefreshByCol(int colId, bool asc)
{
    set<string> results;
	SQLiteValuePair constrain;

    if(colId >= m_colnum)
    {   
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshByCol invalid colid.");
        return -1; 
    } 

    const SQLiteColumn &col = m_col[colId];
    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {   
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshByCol col must be integer.");
        return -1; 
    }   

    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::RefreshByCol open table failed.");
        return -1;
    }
    
    m_returnResults.clear();
    results.clear();
	constrain.clear();


    if(m_table.Query(constrain, results, m_returnResults, col.GetHead(), asc) )
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::RefreshByCol query failed.");
        m_table.Close();
        return -1;
    }
	else
	{
		m_values.clear();
    	//m_returnResults.clear();
	}

    m_table.Close();
    
    return 0;
	
}

int VDRPhoneTableBase::RefreshByEqualToStr(int colId,string str)
{
    set<string> results;
	SQLiteValuePair constrain;
	set<SQLiteValuePair> returnResults;
	//SQLiteValuePair m_values

    if(colId >= m_colnum)
    {   
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshByEqualToStr invalid colid.");
        return -1; 
    }  
	if(str.size() == 0)
    {   
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshByEqualToStr string can't be emply.");
        return -1; 
    }  
	
	SQLiteValue tmpSQLiteValue = SQLiteValue(str.c_str());
    const SQLiteColumn &col = m_col[colId];

    if(col.GetType() != ENUM_SQLite_COLUMN_STRING)
    {   
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshByEqualToStr col must be string.");
        return -1; 
    }   

    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::RefreshByEqualToStr open table failed.");
        return -1;
    }
    
    m_returnResults.clear();
    results.clear();
	constrain.clear();

	constrain[col.GetHead()] = tmpSQLiteValue;

    if(m_table.Query(constrain, results, returnResults))//m_returnResults) )
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::RefreshByEqualToStr query failed.");
        m_table.Close();
        return -1;
    }
	else
	{
		m_values.clear();
    	m_returnResults.clear();
	}

	for(set<SQLiteValuePair>::iterator si = returnResults.begin(); si != returnResults.end(); si++)
	{
		m_returnResults.push_back(*si);
	}


    m_table.Close();
    
    return 0;

}

int VDRPhonePhonebookFromServerTable::RefreshAll(void)
{
    set<string> results;
	SQLiteValuePair constrain;
	set<SQLiteValuePair> returnResults;

	

    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::RefreshByEqualToStr open table failed.");
        return -1;
    }
    
    m_returnResults.clear();
    results.clear();
	constrain.clear();


    if(m_table.Query(constrain, results, returnResults))//m_returnResults) )
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "VDRPhoneCallRecordTable::RefreshByEqualToStr query failed.");
        m_table.Close();
        return -1;
    }
	else
	{
		m_values.clear();
    	m_returnResults.clear();
	}

	for(set<SQLiteValuePair>::iterator si = returnResults.begin(); si != returnResults.end(); si++)
	{
		m_returnResults.push_back(*si);
	}


    m_table.Close();

	return 0;
}



