#ifndef _SINGLE_ROW_TABLE_H_
#define _SINGLE_ROW_TABLE_H_

#include "LogUtility.h"
#include "ConcreteTableBase.h"

using namespace std;

class SingleRowTable : public ConcreteTableBase
{
public:
    int Refresh();
    int Commit();
    const SQLiteValuePair &GetValues() const
    {
        return m_values;
    }
    
    int GetIntValue(int colid, int &value);
    int GetStringValue(int colid, string &value);
    int GetFloatValue(int colid, float &value);
    int GetSequenceValue(int colid, SQLiteSequence &value);

    int SetIntValue(int colid, int value);
    int SetStringValue(int colid, const string &value);
    int SetFloatValue(int colid, float value);
    int SetSequenceValue(int colid, SQLiteSequence &value);

protected:
    SingleRowTable(string dbName, string tableName, int maxrownum, int colnum, const SQLiteColumn *col) : ConcreteTableBase(dbName, tableName, maxrownum, colnum, col, true)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::SingleRowTable colnum %d.", m_colnum);
        for(int i = 0; i < m_colnum; i ++)
        {
            m_dirty.push_back(false);
        }
        Refresh();
    }

private:
    SQLiteValuePair m_values;
    vector<bool> m_dirty;
};

ostream& operator <<(ostream& os, const SingleRowTable& table);


#endif
