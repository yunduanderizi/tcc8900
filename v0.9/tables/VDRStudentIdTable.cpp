#include "VDRMonitorTable.h"
#include "VDRStudentIdTable.h"

const static SQLiteColumn VdrStudentIdUpCols[VDR_STUDENT_ID_DETAIL_BUTT] = {	
    SQLiteColumn("ColStudentId",     ENUM_SQLite_COLUMN_STRING,      true, false, false, false, false),
    SQLiteColumn("ColReadTime",      ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false),	
    SQLiteColumn("ColParamIndex",    ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, true),
};


const static SQLiteColumn VdrIdCountCols[VDR_STUDENT_ID_COUNT_BUTT] = {	
    SQLiteColumn("ColDayTime",     		ENUM_SQLite_COLUMN_INTEGER,      true, false, false, false, true),
    SQLiteColumn("ColIdCont",      		ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false)
};

VDRIdCountTable::VDRIdCountTable():
	MultiRowWithUniqueIndexTable(VDR_SCHOOL_STUDENTS_CNT_DB_NAME,
                                 VDR_STUDENTS_ID_CNT_TABLE_NAME, 
                                 (int)VDR_STUDENT_ID_TABLE_MAX_ROWS, 
                                 (int)VDR_STUDENT_ID_COUNT_BUTT, 
                                 VdrIdCountCols, 
                                 (int)VDR_STUDENT_ID_DAYTIME)
{
	;
}


VDRStudentIdUpTable::VDRStudentIdUpTable():
	MultiRowWithUniqueIndexTable(VDR_SCHOOL_STUDENTS_DB_NAME,
                                 VDR_STUDENTS_ID_UP_TABLE_NAME, 
                                 (int)VDR_STUDENT_ID_TABLE_MAX_ROWS, 
                                 (int)VDR_STUDENT_ID_DETAIL_BUTT, 
                                 VdrStudentIdUpCols, 
                                 (int)VDR_STUDENT_ID_INDEX)
{
	;
}

int VDRStudentIdUpTable::UpRefreshByEqualToStr(int colId,string str)
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

