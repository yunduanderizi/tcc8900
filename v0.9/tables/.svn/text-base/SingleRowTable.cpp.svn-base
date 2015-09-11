#include "SingleRowTable.h"

int SingleRowTable::GetIntValue(int colid, int &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::GetIntValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::GetIntValue type not integer.");
        return -1;
    }
    
    value = GetSQLiteValueInt(m_values[col.GetHead()]);
    return 0;
}

int SingleRowTable::GetStringValue(int colid, string &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::GetStringValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_STRING)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::GetStringValue type not string.");
        return -1;
    }
    
    GetSQLiteValueString(m_values[col.GetHead()], value);
    return 0;
}

int SingleRowTable::GetFloatValue(int colid, float &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::GetFloatValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_FLOAT)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::GetFloatValue type not integer.");
        return -1;
    }

    value = GetSQLiteValueFloat(m_values[col.GetHead()]);
    return 0;
}

int SingleRowTable::GetSequenceValue(int colid, SQLiteSequence &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::GetSequenceValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_SEQUENCE)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::GetSequenceValue type not integer.");
        return -1;
    }
    
    GetSQLiteValueSequence(m_values[col.GetHead()], value);
    
    return 0;
}

int SingleRowTable::SetIntValue(int colid, int value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::SetIntValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::SetIntValue type not integer.");
        return -1;
    }

    m_values[col.GetHead()] = SQLiteValue(value);
    m_dirty[colid] = true;
    
    return 0;
}

int SingleRowTable::SetStringValue(int colid, const string &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::SetStringValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_STRING)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::SetStringValue type not string.");
        return -1;
    }

    m_values[col.GetHead()] = SQLiteValue(value);
    m_dirty[colid] = true;

    return 0;
}

int SingleRowTable::SetFloatValue(int colid, float value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::SetFloatValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_FLOAT)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::SetFloatValue type not integer.");
        return -1;
    }

    m_values[col.GetHead()] = SQLiteValue(value);
    m_dirty[colid] = true;
    
    return 0;
}

int SingleRowTable::SetSequenceValue(int colid, SQLiteSequence &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::SetSequenceValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_SEQUENCE)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::SetSequenceValue type not integer.");
        return -1;
    }

    m_values[col.GetHead()] = SQLiteValue(value);
    m_dirty[colid] = true;
    
    return 0;
}

int SingleRowTable::Refresh()
{
    set<string> results;
    
    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::Refresh open table failed.");
        return -1;
    }
    m_values.clear();
    results.clear();
    
    if(m_table.QueryLatest(results, m_values))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::Refresh query failed.");
        m_table.Close();
        return -1;
    }

    m_table.Close();

    return 0;
}

int SingleRowTable::Commit()
{
    SQLiteValuePair constrain;
    SQLiteValuePair pairs;

    if(m_table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::Commit open table failed.");
        return -1;
    }
    
    constrain.clear();
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
    
    if(m_table.Modify(constrain, pairs) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SingleRowTable::Commit failed to modify.");
        m_table.Close();
        return -1;
    }
    
    for(int i = 0; i < m_colnum; i ++)
    {
        m_dirty[i] = false;
    }
    
    m_table.Close();
    
    return 0;
}

ostream& operator <<(ostream& os, const SingleRowTable& table)
{
    const SQLiteValuePair &values = table.GetValues();
    os << "SingleRowTable " << table.GetDbName() << " " << table.GetTableName() << endl;
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
