#ifndef _MULTI_ROW__WITH_UNIQUE_INDEX_TABLE_H_
#define _MULTI_ROW__WITH_UNIQUE_INDEX_TABLE_H_

#include "LogUtility.h"
#include "ConcreteTableBase.h"

using namespace std;

class MultiRowWithUniqueIndexTable : public ConcreteTableBase
{
public:
    int Refresh(int uniqueValue);
    int Delete(int uniqueValue);
    int Refresh(string uniqueValue);
    int Delete(string uniqueValue);
    int DeleteAll();
    int RefreshBottom();
    int RefreshTop();
    int RefreshNext();
    int RefreshNext(int currentValue);
    int RefreshNext(string currentValue);
    int RefreshBetween(int colid, int min, int max, bool asc = true);
    int RefreshAll(int colid, bool asc = true);
    int Commit(bool queryFlag = true);
    int CommitCache();
    int QueryAll(set<SQLiteValuePair> &returnResults);
    const SQLiteValuePair &GetValues() const
    {
        return m_values;
    }
    int GetRowCount();
    
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

protected:
    MultiRowWithUniqueIndexTable(string dbName, string tableName, int maxrownum, int colnum, const SQLiteColumn *col, int uniquecol, bool indexFlag = true, int cache = 0) :
        ConcreteTableBase(dbName, tableName, maxrownum, colnum, col)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::MultiRowWithUniqueIndexTable colnum %d.", m_colnum);
        for(int i = 0; i < m_colnum; i ++)
        {
            m_dirty.push_back(false);
        }
        m_uniquecol = uniquecol;
        if(m_uniquecol >= m_colnum)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::MultiRowWithUniqueIndexTable unique col too big %d, %d.", m_uniquecol, m_colnum);
            m_uniquecol = 0;
        }
        if(indexFlag)
        {
            SetIndex();
        }
        m_cache = cache;
        if(cache)
        {
            m_cachetable.SetDbName(GetDbFullName(dbName, false) + "." + tableName);
            m_cachetable.SetMaxRows(cache);
            m_cachetable.SetTableName(tableName);
            for(int i = 0; i < m_colnum; i ++)
            {
                m_cachetable.AddColumn(m_col[i]);
            }
        }
    }
    
    void SetIndex();

    SQLiteValuePair m_values;
    vector<SQLiteValuePair> m_returnResults;
    vector<bool> m_dirty;
    SQLiteTable m_cachetable;
    int m_cache;
    int m_uniquecol;
};

ostream& operator <<(ostream& os, const MultiRowWithUniqueIndexTable& table);


#endif
