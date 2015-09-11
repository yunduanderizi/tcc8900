#ifndef _LARGE_TABLE_BASE_H_
#define _LARGE_TABLE_BASE_H_

#include "SQLiteTable.h"
#include "Utility.h"
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <ostream>
#include <string>
#include <stdlib.h>

using namespace std;
using namespace boost::interprocess;

typedef enum
{
    LARGE_TABLE_SCHEMA_PARA_NUMBER = 0,
    LARGE_TABLE_SCHEMA_PARA_CURRENT_TABLE,
    LARGE_TABLE_SCHEMA_PARA_CURRENT_INDEX,
    LARGE_TABLE_SCHEMA_PARA_BUTT
}ENUM_LARGE_TABLE_SCHEMA_PARA_COL;

#define LARGE_TABLE_OVERHEAD            100

#define SCHEMA_COL_NUMBER               "ColNumber"
#define SCHEMA_COL_CURRENT_TABLE        "ColCurrentTable"
#define SCHEMA_COL_CURRENT_INDEX        "ColCurrentIndex"

#define LARGE_TABLE_COL_INDEX           "ColLargeTableIndex"

class LargeTableBase
{
public:
    const string &GetDbName() const { return m_dbName; }
    const string &GetTableName() const { return m_tableName; }

    int GetIntValue(int colid, int &value);
    int GetStringValue(int colid, string &value);
    int GetFloatValue(int colid, float &value);
    int GetSequenceValue(int colid, SQLiteSequence &value);
    
    int GetResultsCount() { return m_returnResults.size(); }
    int GetIntValue(int index, int colid, int &value);
    int GetStringValue(int index, int colid, string &value);
    int GetFloatValue(int index, int colid, float &value);
    int GetSequenceValue(int index, int colid, SQLiteSequence &value);

    int SetIntValue(int colid, int value);
    int SetStringValue(int colid, const string &value);
    int SetFloatValue(int colid, float value);
    int SetSequenceValue(int colid, SQLiteSequence &value);
    
    int Add();
    int RefreshTop();
    int RefreshBottom();
    int RefreshBottom(int count);
    int RefreshNext(int currentIndex);
    int RefreshPrevious(int currentIndex);
    int RefreshPrevious(int currentIndex, int topindex);
    int RefreshNearest(int colid, int value, bool top);

    inline void GetCurrentTableAndIndex(int &colnum,int &table,int &index) { 
	    colnum=m_schemaColNum;
	    table=m_schemaCurrentTable;
	    index=m_schemaCurrentIndex;
    }
    inline void SetCurrentTableAndIndex(int colnum,int table,int index) { 
	    m_schemaColNum=colnum;
	    m_schemaCurrentTable=table;
	    m_schemaCurrentIndex=index;
    }
private:
    
    int RefreshSchema();
    int UpdateSchema();
    void GetFileLock();
    
protected:
    LargeTableBase()
    {
        ;
    }

    LargeTableBase(string dbName, string tableName, int maxrownum, int unitrownum, int colnum, const SQLiteColumn *col) : m_schemaTable(GetSchemaDbFullName(dbName), GetSchemaTableName(tableName), 1)
    {
        m_dbName = dbName;
        m_dbFullName = GetDbFullName(m_dbName);
        m_tableName = tableName;
        m_unitrownum = unitrownum;
        
        m_maxrownum = ((maxrownum + LARGE_TABLE_OVERHEAD) / m_unitrownum) * m_unitrownum;
        m_tablenum = m_maxrownum / m_unitrownum;
        m_colnum = colnum;
        m_col = col;
        GetFileLock();
        SetSchemaTable();
        file_lock f_lock(m_file_lock.c_str());
        scoped_lock<file_lock> e_lock(f_lock);
        RefreshSchema();
        for(int i = 0; i < m_colnum; i ++)
        {
            m_dirty.push_back(false);
        }
    }

    void SetSchemaTable();
    void SetTable(SQLiteTable &table);
    
    string GetSchemaDbFullName(string dbName)
    {
        char *path = getenv("INNOV_TABLE_DIR");
        if(path)
        {
            return path + dbName; 
        }
        else
        {
            return dbName;
        }
    }

    string GetDbFullName(string dbName)
    {
        char *path = getenv("INNOV_TABLE_DIR");
        if(path)
        {
            return path + dbName; 
        }
        else
        {
            return dbName;
        }
    }

    inline string GetExactDbFullName(int index)
    {
        return m_dbFullName + numToString<int>(index);
    }
    
    string &GetSchemaTableName(string tableName)
    {
        m_schemaTableName = tableName + "schema";
        return m_schemaTableName;
    }
    
    string GetTableName(int index)
    {
        return m_tableName + numToString<int>(index);
    }

    string m_dbName;
    string m_dbFullName;
    string m_tableName;
    string m_schemaTableName;
    int m_colnum;
    const SQLiteColumn *m_col;
    int m_maxrownum;
    int m_unitrownum;
    int m_tablenum;
    SQLiteTable m_schemaTable;

    SQLiteValuePair m_values;
    vector<SQLiteValuePair> m_returnResults;
    vector<bool> m_dirty;
    
    int m_schemaColNum;
    int m_schemaCurrentTable;
    int m_schemaCurrentIndex;
    
    int m_queryCurrentTable;
    int m_queryCurrentIndex;
    
    string m_file_lock;
};

#endif
