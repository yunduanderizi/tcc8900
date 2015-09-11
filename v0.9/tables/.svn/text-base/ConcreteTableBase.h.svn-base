#ifndef _CONCRETE_TABLE_BASE_H_
#define _CONCRETE_TABLE_BASE_H_

#include "SQLiteTable.h"
#include <ostream>
#include <string>
#include <stdlib.h>

using namespace std;

class ConcreteTableBase
{
public:
    const string &GetDbName() const { return m_dbName; }
    const string &GetTableName() const { return m_tableName; }
protected:
    ConcreteTableBase()
    {
        ;
    }
    
    ConcreteTableBase(string dbName, string tableName, int maxrownum, int colnum, const SQLiteColumn *col, bool important = false) : m_table(GetDbFullName(dbName,important), tableName, maxrownum)
    {
        m_dbName = dbName;
        m_tableName = tableName;
        m_maxrownum = maxrownum;
        m_colnum = colnum;
        m_col = col;
        SetTable();
    }

    void SetTable();
    
    string GetDbFullName(string dbName, bool important)
    {
        char *path;
        
        if(important)
            path = getenv("INNOV_IMPORTANT_TABLE_DIR");
        else
            path = getenv("INNOV_TABLE_DIR");
        
        if(path)
        {
            return path + dbName; 
        }
        else
        {
            return dbName;
        }
    }

    string m_dbName;
    string m_tableName;
    int m_colnum;
    const SQLiteColumn *m_col;
    int m_maxrownum;
    SQLiteTable m_table;
};

#endif
