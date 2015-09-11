#include "MultiRowWithUniqueIndexTable.h"

void MultiRowWithUniqueIndexTable::SetIndex()
{
    set<string> index;
    const SQLiteColumn &col = m_col[m_uniquecol];

    if((col.GetType() != ENUM_SQLite_COLUMN_INTEGER) && (col.GetType() != ENUM_SQLite_COLUMN_STRING) )
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::SetIndex index must be integer or string.");
        return;
    }
    
    index.insert(col.GetHead());
    m_table.AddIndex(m_tableName + "Index1", index, true);
    
    return;
}

int MultiRowWithUniqueIndexTable::GetIntValue(int colid, int &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetIntValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetIntValue type not integer.");
        return -1;
    }
    
    value = GetSQLiteValueInt(m_values[col.GetHead()]);
    return 0;
}

int MultiRowWithUniqueIndexTable::GetStringValue(int colid, string &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetStringValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_STRING)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetStringValue type not string.");
        return -1;
    }
    
    GetSQLiteValueString(m_values[col.GetHead()], value);
    return 0;
}

int MultiRowWithUniqueIndexTable::GetFloatValue(int colid, float &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetFloatValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_FLOAT)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetFloatValue type not integer.");
        return -1;
    }

    value = GetSQLiteValueFloat(m_values[col.GetHead()]);
    return 0;
}

int MultiRowWithUniqueIndexTable::GetSequenceValue(int colid, SQLiteSequence &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetSequenceValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_SEQUENCE)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetSequenceValue type not integer.");
        return -1;
    }
    
    GetSQLiteValueSequence(m_values[col.GetHead()], value);
    
    return 0;
}

int MultiRowWithUniqueIndexTable::GetIntValue(int index, int colid, int &value)
{
    if(index >= GetResultsCount())
    {
        return -1;
    }
    SQLiteValuePair &values = m_returnResults[index];
    const SQLiteColumn &col = m_col[colid];
    if(values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetIntValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetIntValue type not integer.");
        return -1;
    }
    
    value = GetSQLiteValueInt(values[col.GetHead()]);
    return 0;
}

int MultiRowWithUniqueIndexTable::GetStringValue(int index, int colid, string &value)
{
    if(index >= GetResultsCount())
    {
        return -1;
    }
    SQLiteValuePair &values = m_returnResults[index];
    const SQLiteColumn &col = m_col[colid];
    if(values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetStringValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_STRING)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetStringValue type not string.");
        return -1;
    }
    
    GetSQLiteValueString(values[col.GetHead()], value);
    return 0;
}

int MultiRowWithUniqueIndexTable::GetFloatValue(int index, int colid, float &value)
{
    if(index >= GetResultsCount())
    {
        return -1;
    }
    SQLiteValuePair &values = m_returnResults[index];
    const SQLiteColumn &col = m_col[colid];
    if(values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetFloatValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_FLOAT)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetFloatValue type not integer.");
        return -1;
    }

    value = GetSQLiteValueFloat(values[col.GetHead()]);
    return 0;
}

int MultiRowWithUniqueIndexTable::GetSequenceValue(int index, int colid, SQLiteSequence &value)
{
    if(index >= GetResultsCount())
    {
        return -1;
    }
    SQLiteValuePair &values = m_returnResults[index];
    const SQLiteColumn &col = m_col[colid];
    if(values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetSequenceValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_SEQUENCE)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetSequenceValue type not integer.");
        return -1;
    }
    
    GetSQLiteValueSequence(values[col.GetHead()], value);
    
    return 0;
}

int MultiRowWithUniqueIndexTable::SetIntValue(int colid, int value)
{
    const SQLiteColumn &col = m_col[colid];
    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::SetIntValue type not integer.");
        return -1;
    }

    if(m_values.count(col.GetHead()) <= 0)
    {
        m_values.insert(SQLiteValuePair::value_type(col.GetHead(), value));
    }
    else
    {
        m_values[col.GetHead()] = SQLiteValue(value);
    }
    m_dirty[colid] = true;
    
    return 0;
}

int MultiRowWithUniqueIndexTable::SetStringValue(int colid, const string &value)
{
    const SQLiteColumn &col = m_col[colid];

    if(col.GetType() != ENUM_SQLite_COLUMN_STRING)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::SetStringValue type not string.");
        return -1;
    }

    if(m_values.count(col.GetHead()) <= 0)
    {
        m_values.insert(SQLiteValuePair::value_type(col.GetHead(), value));
    }
    else
    {
        m_values[col.GetHead()] = SQLiteValue(value);
    }
    m_dirty[colid] = true;

    return 0;
}

int MultiRowWithUniqueIndexTable::SetFloatValue(int colid, float value)
{
    const SQLiteColumn &col = m_col[colid];

    if(col.GetType() != ENUM_SQLite_COLUMN_FLOAT)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::SetFloatValue type not integer.");
        return -1;
    }

    if(m_values.count(col.GetHead()) <= 0)
    {
        m_values.insert(SQLiteValuePair::value_type(col.GetHead(), value));
    }
    else
    {
        m_values[col.GetHead()] = SQLiteValue(value);
    }
    m_dirty[colid] = true;
    
    return 0;
}

int MultiRowWithUniqueIndexTable::SetSequenceValue(int colid, SQLiteSequence &value)
{
    const SQLiteColumn &col = m_col[colid];

    if(col.GetType() != ENUM_SQLite_COLUMN_SEQUENCE)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::SetSequenceValue type not integer.");
        return -1;
    }

    if(m_values.count(col.GetHead()) <= 0)
    {
        m_values.insert(SQLiteValuePair::value_type(col.GetHead(), value));
    }
    else
    {
        m_values[col.GetHead()] = SQLiteValue(value);
    }
    m_dirty[colid] = true;
    
    return 0;
}

int MultiRowWithUniqueIndexTable::Delete(int uniqueValue)
{
    SQLiteValuePair constrain;
    const SQLiteColumn &col = m_col[m_uniquecol];
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Delete open table failed.");
        return -1;
    }
    constrain.clear();
    
    constrain[col.GetHead()] = SQLiteValue(uniqueValue);

    if(m_table.Delete(constrain))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Delete delete failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();

    return 0;
}

int MultiRowWithUniqueIndexTable::Delete(string uniqueValue)
{
    SQLiteValuePair constrain;
    const SQLiteColumn &col = m_col[m_uniquecol];
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Delete open table failed.");
        return -1;
    }
    constrain.clear();
    
    constrain[col.GetHead()] = SQLiteValue(uniqueValue);

    if(m_table.Delete(constrain))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Delete delete failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();

    return 0;
}

int MultiRowWithUniqueIndexTable::DeleteAll()
{
    SQLiteValuePair constrain;

    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::DeleteAll open table failed.");
        return -1;
    }

    constrain.clear();

    if(m_table.Delete(constrain))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::DeleteAll delete failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();

    return 0;
}

int MultiRowWithUniqueIndexTable::Refresh(int uniqueValue)
{
    SQLiteValuePair constrain;
    set<string> results;
    const SQLiteColumn &col = m_col[m_uniquecol];
    set<SQLiteValuePair> returnResults;
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Refresh open table failed.");
        return -1;
    }
    m_values.clear();
    returnResults.clear();
    results.clear();
    constrain.clear();
    
    constrain[col.GetHead()] = SQLiteValue(uniqueValue);

    if(m_table.Query(constrain, results, returnResults))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Refresh query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    if(returnResults.size() != 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Refresh query failed.");
        m_table.Close();
        return -1;
    }
    
    m_values = *(returnResults.begin());

    return 0;
}

int MultiRowWithUniqueIndexTable::Refresh(string uniqueValue)
{
    SQLiteValuePair constrain;
    set<string> results;
    const SQLiteColumn &col = m_col[m_uniquecol];
    set<SQLiteValuePair> returnResults;
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Refresh open table failed.");
        return -1;
    }
    m_values.clear();
    returnResults.clear();
    results.clear();
    constrain.clear();
    
    constrain[col.GetHead()] = SQLiteValue(uniqueValue);

    if(m_table.Query(constrain, results, returnResults))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Refresh query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    if(returnResults.size() != 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Refresh query failed.");
        m_table.Close();
        return -1;
    }
    
    m_values = *(returnResults.begin());

    return 0;
}

int MultiRowWithUniqueIndexTable::RefreshBottom()
{
    set<string> results;
    const SQLiteColumn &col = m_col[m_uniquecol];
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshBottom open table failed.");
        return -1;
    }
    m_values.clear();
    results.clear();

    if(m_table.QueryTopBottom(col.GetHead(), results, m_values, false))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshBottom query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    return 0;
}

int MultiRowWithUniqueIndexTable::RefreshTop()
{
    set<string> results;
    const SQLiteColumn &col = m_col[m_uniquecol];
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshTop open table failed.");
        return -1;
    }
    m_values.clear();
    results.clear();

    if(m_table.QueryTopBottom(col.GetHead(), results, m_values, true))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshTop query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    return 0;
}

int MultiRowWithUniqueIndexTable::RefreshNext()
{
    set<string> results;
    const SQLiteColumn &col = m_col[m_uniquecol];
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshNext open table failed.");
        return -1;
    }
    
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshNext no current value.");
        return -1;
    }
    
    SQLiteValue value = m_values[col.GetHead()];

    m_values.clear();
    results.clear();

    if(m_table.QueryNext(col.GetHead(), results, m_values, value))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshNext query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    return 0;
}

int MultiRowWithUniqueIndexTable::RefreshNext(int currentValue)
{
    set<string> results;
    const SQLiteColumn &col = m_col[m_uniquecol];
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshNext open table failed.");
        return -1;
    }
    
    m_values.clear();
    results.clear();

    SQLiteValue value(currentValue);
    
    if(m_table.QueryNext(col.GetHead(), results, m_values, value))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshNext query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    return 0;
}

int MultiRowWithUniqueIndexTable::RefreshNext(string currentValue)
{
    set<string> results;
    const SQLiteColumn &col = m_col[m_uniquecol];
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshNext open table failed.");
        return -1;
    }
    
    m_values.clear();
    results.clear();

    SQLiteValue value(currentValue);
    
    if(m_table.QueryNext(col.GetHead(), results, m_values, value))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshNext query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    return 0;
}

int MultiRowWithUniqueIndexTable::CommitCache()
{
    SQLiteValuePair constrain;
    SQLiteValuePair pairs;
    const SQLiteColumn &col = m_col[m_uniquecol];
    set<string> results;
    set<SQLiteValuePair> returnResults;
    int rowCount;

    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::CommitCache index value not set yet.");
        return -1;
    }

    if(m_cachetable.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::CommitCache open cache table failed.");
        return -1;
    }

    rowCount = m_cachetable.GetRowCount();
    if(rowCount >= (m_cache - 1))
    {
    
        constrain.clear();
        results.clear();
        returnResults.clear();
        if(m_cachetable.Query(constrain, results, returnResults) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::CommitCache Query failed.");
            return -1;
        }
        returnResults.insert(m_values);
        if(m_table.Open() < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::CommitCache open table failed.");
            m_cachetable.Close();
            return -1;
        }
        m_table.AddBatch(returnResults);
        m_table.Close();
        
        constrain.clear();
    
        if(m_cachetable.Delete(constrain))
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::CommitCache delete failed.");
            m_cachetable.Close();
            return -1;
        }
    }
    else
    {
        if(m_cachetable.Open() < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::CommitCache open cache failed.");
            return -1;
        }
        
        pairs.clear();
        
        for(int i = 0; i < m_colnum; i ++)
        {
            if(m_dirty[i] == false)
            {
                continue;
            }
            
            const SQLiteColumn col = m_col[i];
            pairs[col.GetHead()] = m_values[col.GetHead()];
        }
        
        if(m_cachetable.Add(pairs) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::CommitCache failed to add.");
            m_cachetable.Close();
            return -1;
        }
        
        for(int i = 0; i < m_colnum; i ++)
        {
            m_dirty[i] = false;
        }
    }

    m_cachetable.Close();
    
    return 0;
}

int MultiRowWithUniqueIndexTable::Commit(bool queryFlag)
{
    SQLiteValuePair constrain;
    SQLiteValuePair pairs;
    const SQLiteColumn &col = m_col[m_uniquecol];
    set<string> results;
    set<SQLiteValuePair> returnResults;
    bool needAdd = false;

    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Commit index value not set yet.");
        return -1;
    }
    
    if(m_cache)
    {
        return CommitCache();
    }
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Commit open table failed.");
        return -1;
    }
    
    constrain.clear();
    results.clear();
    returnResults.clear();
    
    if(queryFlag)
    {
        constrain[col.GetHead()] = m_values[col.GetHead()];
        if(m_table.Query(constrain, results, returnResults) < 0)
        {
            LogUtility::Log(LOG_LEVEL_INFO, "MultiRowWithUniqueIndexTable::Commit query failed.");
            return -1;
        }
        
        if(returnResults.size() <= 0)
        {
            needAdd = true;
        }
    }
    else
    {
        needAdd = true;
    }
    pairs.clear();
    
    for(int i = 0; i < m_colnum; i ++)
    {
        if(m_dirty[i] == false)
        {
            continue;
        }
        
        const SQLiteColumn col = m_col[i];
        pairs[col.GetHead()] = m_values[col.GetHead()];
    }
    
    if(needAdd)
    {
        if(m_table.Add(pairs) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Commit failed to add.");
            m_table.Close();
            return -1;
        }
    }
    else
    {
        if(m_table.Modify(constrain, pairs) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::Commit failed to modify.");
            m_table.Close();
            return -1;
        }
    }
    
    for(int i = 0; i < m_colnum; i ++)
    {
        m_dirty[i] = false;
    }
    
    m_table.Close();
    
    return 0;
}

int MultiRowWithUniqueIndexTable::QueryAll(set<SQLiteValuePair> &returnResults)
{
    SQLiteValuePair constrain;
    set<string> results;
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshAll open table failed.");
        return -1;
    }
    
    returnResults.clear();
    results.clear();

    if(m_table.Query(constrain, results, returnResults))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshAll query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    return 0;
}

int MultiRowWithUniqueIndexTable::RefreshBetween(int colid, int min, int max, bool asc)
{
    set<string> results;

    if(colid >= m_colnum)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshBetween invalid colid.");
        return -1;
    }
    
    const SQLiteColumn &col = m_col[colid];
    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshBetween col must be integer.");
        return -1;
    }

    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshBetween open table failed.");
        return -1;
    }
    
    m_returnResults.clear();
    results.clear();

    if(m_table.QueryBetween(col.GetHead(), min, max, results, m_returnResults, asc))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshBetween query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    return 0;
}

int MultiRowWithUniqueIndexTable::RefreshAll(int colid, bool asc)
{
    SQLiteValuePair constrain;
    set<string> results;

    if(colid >= m_colnum)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshAll invalid colid.");
        return -1;
    }
    
    const SQLiteColumn &col = m_col[colid];
    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshAll col must be integer.");
        return -1;
    }

    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshAll open table failed.");
        return -1;
    }
    
    m_returnResults.clear();
    results.clear();

    if(m_table.Query(constrain, results, m_returnResults, col.GetHead(), asc))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::RefreshAll query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();
    
    return 0;
}

int MultiRowWithUniqueIndexTable::GetRowCount()
{
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "MultiRowWithUniqueIndexTable::GetRowCount open table failed.");
        return -1;
    }

    int rowcount = m_table.GetRowCount();

    m_table.Close();
    
    return rowcount;
}

ostream& operator <<(ostream& os, const MultiRowWithUniqueIndexTable& table)
{
    const SQLiteValuePair &values = table.GetValues();
    os << "MultiRowWithUniqueIndexTable " << table.GetDbName() << " " << table.GetTableName() << endl;
    for(SQLiteValuePair::const_iterator it = values.begin(); it != values.end(); it ++)
    {
        string name = it->first;
        SQLiteValue value = it->second;
        os << name;
        os << " = ";
        os << SQLiteValueToString(value);
        os << endl;
    }

    return os;
}
