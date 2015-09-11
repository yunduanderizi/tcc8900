#include "LargeTableBase.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const static SQLiteColumn LargeTableSchemaParaCols[LARGE_TABLE_SCHEMA_PARA_BUTT] = {
//               Head                       Type                            not   no     auto   prim   Unique Default
//                                                                          Null  Case   Incre  Key           Value
    SQLiteColumn(SCHEMA_COL_NUMBER,         ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn(SCHEMA_COL_CURRENT_TABLE,  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0)),
    SQLiteColumn(SCHEMA_COL_CURRENT_INDEX,  ENUM_SQLite_COLUMN_INTEGER,     true, false, false, false, false, SQLiteValue(0))
};

static SQLiteColumn largeTableIndexCol(LARGE_TABLE_COL_INDEX, ENUM_SQLite_COLUMN_INTEGER, true, false, false, false, true);

void LargeTableBase::SetSchemaTable()
{
    for(int i = 0; i < LARGE_TABLE_SCHEMA_PARA_BUTT; i ++)
    {
        m_schemaTable.AddColumn(LargeTableSchemaParaCols[i]);
    }
}

void LargeTableBase::SetTable(SQLiteTable &table)
{
    for(int i = 0; i < m_colnum; i ++)
    {
        table.AddColumn(m_col[i]);
    }
    
    table.AddColumn(largeTableIndexCol);
}

int LargeTableBase::GetIntValue(int colid, int &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetIntValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetIntValue type not integer.");
        return -1;
    }
    
    value = GetSQLiteValueInt(m_values[col.GetHead()]);
    return 0;
}

int LargeTableBase::GetStringValue(int colid, string &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetStringValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_STRING)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetStringValue type not string.");
        return -1;
    }
    
    GetSQLiteValueString(m_values[col.GetHead()], value);
    return 0;
}

int LargeTableBase::GetFloatValue(int colid, float &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetFloatValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_FLOAT)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetFloatValue type not integer.");
        return -1;
    }

    value = GetSQLiteValueFloat(m_values[col.GetHead()]);
    return 0;
}

int LargeTableBase::GetSequenceValue(int colid, SQLiteSequence &value)
{
    const SQLiteColumn &col = m_col[colid];
    if(m_values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetSequenceValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_SEQUENCE)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetSequenceValue type not integer.");
        return -1;
    }
    
    GetSQLiteValueSequence(m_values[col.GetHead()], value);
    
    return 0;
}

int LargeTableBase::GetIntValue(int index, int colid, int &value)
{
    if(index >= GetResultsCount())
    {
        return -1;
    }
    SQLiteValuePair &values = m_returnResults[index];
    const SQLiteColumn &col = m_col[colid];
    if(values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetIntValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetIntValue type not integer.");
        return -1;
    }
    
    value = GetSQLiteValueInt(values[col.GetHead()]);
    return 0;
}

int LargeTableBase::GetStringValue(int index, int colid, string &value)
{
    if(index >= GetResultsCount())
    {
        return -1;
    }
    SQLiteValuePair &values = m_returnResults[index];
    const SQLiteColumn &col = m_col[colid];
    if(values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetStringValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_STRING)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetStringValue type not string.");
        return -1;
    }
    
    GetSQLiteValueString(values[col.GetHead()], value);
    return 0;
}

int LargeTableBase::GetFloatValue(int index, int colid, float &value)
{
    if(index >= GetResultsCount())
    {
        return -1;
    }
    SQLiteValuePair &values = m_returnResults[index];
    const SQLiteColumn &col = m_col[colid];
    if(values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetFloatValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_FLOAT)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetFloatValue type not integer.");
        return -1;
    }

    value = GetSQLiteValueFloat(values[col.GetHead()]);
    return 0;
}

int LargeTableBase::GetSequenceValue(int index, int colid, SQLiteSequence &value)
{
    if(index >= GetResultsCount())
    {
        return -1;
    }
    SQLiteValuePair &values = m_returnResults[index];
    const SQLiteColumn &col = m_col[colid];
    if(values.count(col.GetHead()) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetSequenceValue no value available.");
        return -1;
    }

    if(col.GetType() != ENUM_SQLite_COLUMN_SEQUENCE)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetSequenceValue type not integer.");
        return -1;
    }
    
    GetSQLiteValueSequence(values[col.GetHead()], value);
    
    return 0;
}

int LargeTableBase::SetIntValue(int colid, int value)
{
    const SQLiteColumn &col = m_col[colid];
    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::SetIntValue type not integer.");
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

int LargeTableBase::SetStringValue(int colid, const string &value)
{
    const SQLiteColumn &col = m_col[colid];

    if(col.GetType() != ENUM_SQLite_COLUMN_STRING)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::SetStringValue type not string.");
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

int LargeTableBase::SetFloatValue(int colid, float value)
{
    const SQLiteColumn &col = m_col[colid];

    if(col.GetType() != ENUM_SQLite_COLUMN_FLOAT)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::SetFloatValue type not integer.");
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

int LargeTableBase::SetSequenceValue(int colid, SQLiteSequence &value)
{
    const SQLiteColumn &col = m_col[colid];

    if(col.GetType() != ENUM_SQLite_COLUMN_SEQUENCE)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::SetSequenceValue type not integer.");
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

int LargeTableBase::RefreshSchema()
{
    SQLiteValuePair values;
    set<string> results;

    if(m_schemaTable.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshSchema open table failed.");
        return -1;
    }
    values.clear();
    results.clear();
    
    if(m_schemaTable.QueryLatest(results, values))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshSchema query failed.");
        m_schemaTable.Close();
        return -1;
    }

    m_schemaTable.Close();

    if(values.count(SCHEMA_COL_NUMBER) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshSchema not col number returned.");
        return -1;
    }

    m_schemaColNum = GetSQLiteValueInt(values[SCHEMA_COL_NUMBER]);

    if(values.count(SCHEMA_COL_CURRENT_TABLE) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshSchema not col current table returned.");
        return -1;
    }

    m_schemaCurrentTable = GetSQLiteValueInt(values[SCHEMA_COL_CURRENT_TABLE]);

    if(values.count(SCHEMA_COL_CURRENT_INDEX) <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshSchema not col current index returned.");
        return -1;
    }

    m_schemaCurrentIndex = GetSQLiteValueInt(values[SCHEMA_COL_CURRENT_INDEX]);

    return 0;
}

int LargeTableBase::UpdateSchema()
{
    SQLiteValuePair values;
    SQLiteValuePair constrain;
    
    if(m_schemaTable.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::UpdateSchema open table failed.");
        return -1;
    }
    values.clear();
    constrain.clear();
    
    values[SCHEMA_COL_NUMBER] = SQLiteValue(m_schemaColNum);
    values[SCHEMA_COL_CURRENT_TABLE] = SQLiteValue(m_schemaCurrentTable);
    values[SCHEMA_COL_CURRENT_INDEX] = SQLiteValue(m_schemaCurrentIndex);
    
    if(m_schemaTable.Modify(constrain, values))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::UpdateSchema Modify failed.");
        m_schemaTable.Close();
        return -1;
    }

    m_schemaTable.Close();

    return 0;
}

int LargeTableBase::Add()
{
    SQLiteValuePair constrain;
    SQLiteValuePair pairs;
    set<string> results;
    set<SQLiteValuePair> returnResults;
    bool needAdd = false;
    int currentTable;
    int currentIndex;

    file_lock f_lock(m_file_lock.c_str());
    scoped_lock<file_lock> e_lock(f_lock);
    
    if(m_schemaColNum <= 0)
    {
        currentTable = 0;
        currentIndex = 0;
    }
    else
    {
        currentTable = m_schemaCurrentTable;
        currentIndex = m_schemaCurrentIndex;
        currentIndex ++;
        if(currentIndex >= m_unitrownum)
        {
            currentIndex = 0;
            currentTable ++;
        }
        if(currentTable >= m_tablenum)
        {
            currentTable = 0;
        }
    }
    
    SQLiteTable table(GetExactDbFullName(currentTable), m_tableName + numToString<int>(currentTable), m_unitrownum);
    SetTable(table);
    if(m_schemaColNum < m_maxrownum)
    {
        needAdd = true;
    }
    
    if(table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::Add open table failed.");
        return -1;
    }
    
    constrain.clear();
    results.clear();
    returnResults.clear();
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
        pairs[LARGE_TABLE_COL_INDEX] = SQLiteValue(currentIndex); 
        if(table.Add(pairs) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::Add failed to add.");
            needAdd = false;
        }
    }
    
    if(!needAdd)
    {
        constrain[LARGE_TABLE_COL_INDEX] = SQLiteValue(currentIndex); 
        if(table.Modify(constrain, pairs) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::Add failed to modify.");
            table.Close();
            return -1;
        }
    }
    
    for(int i = 0; i < m_colnum; i ++)
    {
        m_dirty[i] = false;
    }
    
    table.Close();
    
    m_schemaCurrentTable = currentTable;
    m_schemaCurrentIndex = currentIndex;
    if(m_schemaColNum < m_maxrownum)
    {
        m_schemaColNum ++;
    }

    if(UpdateSchema() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::Add failed to update schema.");
        return -1;
    }
    
    return 0;
}

int LargeTableBase::RefreshTop()
{
    SQLiteValuePair constrain;
    set<string> results;
    set<SQLiteValuePair> returnResults;
    int currentTable;
    int currentIndex;

    file_lock f_lock(m_file_lock.c_str());
    scoped_lock<file_lock> e_lock(f_lock);

    if(m_schemaColNum < m_maxrownum)
    {
        if(m_maxrownum - m_schemaColNum < LARGE_TABLE_OVERHEAD)
        {
            int tmpIndex = LARGE_TABLE_OVERHEAD - (m_maxrownum - m_schemaColNum);
            currentIndex = tmpIndex % m_unitrownum;
            currentTable = tmpIndex / m_unitrownum;
        }
        else
        {
            currentTable = 0;
            currentIndex = 0;
        }
    }
    else
    {
        currentTable = m_schemaCurrentTable;
        currentIndex = m_schemaCurrentIndex;

        currentIndex += LARGE_TABLE_OVERHEAD;
        currentTable += (currentIndex / m_unitrownum);
        currentIndex %= m_unitrownum;
        if(currentTable >= m_tablenum)
        {
            currentTable -= m_tablenum;
        }
    }

    SQLiteTable table(GetExactDbFullName(currentTable), m_tableName + numToString<int>(currentTable), m_unitrownum);
    SetTable(table);
    if(table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshTop open table failed.");
        return -1;
    }
    m_values.clear();
    results.clear();
    constrain[LARGE_TABLE_COL_INDEX] = SQLiteValue(currentIndex);

    if(table.Query(constrain, results, returnResults))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshTop query failed.");
        table.Close();
        return -1;
    }

    table.Close();
    
    if(returnResults.size() != 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshTop query results invalid(%d).", returnResults.size());
        return -1;
    }
    
    m_values = *(returnResults.begin());

    m_queryCurrentTable = m_schemaCurrentTable;
    m_queryCurrentIndex = m_schemaCurrentIndex;
    
    return currentTable * m_unitrownum + currentIndex;
}

int LargeTableBase::RefreshBottom()
{
    SQLiteValuePair constrain;
    set<string> results;
    set<SQLiteValuePair> returnResults;
    int currentTable;
    int currentIndex;

    file_lock f_lock(m_file_lock.c_str());
    scoped_lock<file_lock> e_lock(f_lock);
    
    currentTable = m_schemaCurrentTable;
    currentIndex = m_schemaCurrentIndex;

    SQLiteTable table(GetExactDbFullName(currentTable), m_tableName + numToString<int>(currentTable), m_unitrownum);
    SetTable(table);
    if(table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshTop open table failed.");
        return -1;
    }
    m_values.clear();
    results.clear();
    constrain[LARGE_TABLE_COL_INDEX] = SQLiteValue(currentIndex);

    if(table.Query(constrain, results, returnResults))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshTop query failed.");
        table.Close();
        return -1;
    }

    table.Close();
    
    if(returnResults.size() != 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshTop query results invalid.");
        return -1;
    }
    
    m_values = *(returnResults.begin());

    if(m_schemaColNum < m_maxrownum)
    {
        if(m_maxrownum - m_schemaColNum < LARGE_TABLE_OVERHEAD)
        {
            int tmpIndex = LARGE_TABLE_OVERHEAD - (m_maxrownum - m_schemaColNum);
            m_queryCurrentIndex = tmpIndex % m_unitrownum;
            m_queryCurrentTable = tmpIndex / m_unitrownum;
        }
        else
        {
            m_queryCurrentIndex = 0;
            m_queryCurrentTable = 0;
        }
    }
    else
    {
        m_queryCurrentTable = m_schemaCurrentTable;
        m_queryCurrentIndex = m_schemaCurrentIndex;
        m_queryCurrentIndex += LARGE_TABLE_OVERHEAD;
        m_queryCurrentTable += (m_queryCurrentIndex / m_unitrownum);
        m_queryCurrentIndex %= m_unitrownum;
        if(m_queryCurrentTable >= m_tablenum)
        {
            m_queryCurrentTable -= m_tablenum;
        }
    }
    
    return currentTable * m_unitrownum + currentIndex;
}

int LargeTableBase::RefreshBottom(int count)
{
    SQLiteValuePair constrain;
    set<string> results;
    int currentTable;
    int currentIndex;
    int min, max;

    file_lock f_lock(m_file_lock.c_str());
    scoped_lock<file_lock> e_lock(f_lock);
    
    if(count > m_schemaColNum)
    {
        count = m_schemaColNum;
    }
    currentTable = m_schemaCurrentTable;
    currentIndex = m_schemaCurrentIndex;
    m_returnResults.clear();

    while((int)m_returnResults.size() < count)
    {
        int countleft = count - m_returnResults.size();
        SQLiteTable table(GetExactDbFullName(currentTable), m_tableName + numToString<int>(currentTable), m_unitrownum);
        SetTable(table);
        if(table.Open() < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshBottom open table failed.");
            return -1;
        }
        
        max = currentIndex;
        min = 0;
        
        if(currentIndex > countleft)
        {
            min = currentIndex - countleft + 1;
        }
        results.clear();

        if(table.QueryBetween(LARGE_TABLE_COL_INDEX, min, max, results, m_returnResults, false))
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshBottom query failed.");
            table.Close();
            return -1;
        }

        table.Close();
        if(m_schemaColNum < m_maxrownum)
        {
            if(currentTable <= 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshBottom reach top.");
                break;
            }
            currentIndex = m_unitrownum - 1;
            currentTable --;
        }
        else
        {
            currentIndex = m_unitrownum - 1;
            currentTable --;
            if(currentTable < 0)
            {
                currentTable = m_tablenum - 1;
            }
        }
    }
    
    return 0;
}

int LargeTableBase::RefreshNext(int nowIndex)
{
    SQLiteValuePair constrain;
    set<string> results;
    set<SQLiteValuePair> returnResults;
    int currentTable;
    int currentIndex;

    file_lock f_lock(m_file_lock.c_str());
    scoped_lock<file_lock> e_lock(f_lock);

    currentTable = nowIndex / m_unitrownum;
    currentIndex = nowIndex % m_unitrownum;
    if((currentTable == m_queryCurrentTable) && (currentIndex == m_queryCurrentIndex))
    {
        LogUtility::Log(LOG_LEVEL_INFO, "LargeTableBase::RefreshNext query results end(%d, %d, %d, %d).", currentTable, currentIndex, m_queryCurrentTable, m_queryCurrentIndex);
        return -1;
    }

    currentIndex ++;

    if(currentIndex >= m_unitrownum)
    {
        currentIndex = 0;
        currentTable ++;
    }
    if(currentTable >= m_tablenum)
    {
        currentTable = 0;
    }

    if(m_schemaColNum < m_maxrownum)
    {
        if(currentIndex > (m_schemaCurrentTable * m_unitrownum + m_schemaCurrentIndex))
        {
            LogUtility::Log(LOG_LEVEL_INFO, "LargeTableBase::RefreshNext index beyond table end(%d, %d, %d, %d).", currentTable, currentIndex, m_schemaCurrentTable, m_schemaCurrentIndex);
            return -1;
        }
    }

    SQLiteTable table(GetExactDbFullName(currentTable), m_tableName + numToString<int>(currentTable), m_unitrownum);
    SetTable(table);
    if(table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshTop open table failed.");
        return -1;
    }
    m_values.clear();
    results.clear();
    constrain[LARGE_TABLE_COL_INDEX] = SQLiteValue(currentIndex);

    if(table.Query(constrain, results, returnResults))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshTop query failed.");
        table.Close();
        return -1;
    }

    table.Close();
    
    if(returnResults.size() != 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshTop query results invalid.");
        return -1;
    }
    
    m_values = *(returnResults.begin());

    m_queryCurrentTable = m_schemaCurrentTable;
    m_queryCurrentIndex = m_schemaCurrentIndex;
    
    return currentTable * m_unitrownum + currentIndex;
}

int LargeTableBase::RefreshPrevious(int nowIndex)
{
    SQLiteValuePair constrain;
    set<string> results;
    set<SQLiteValuePair> returnResults;
    int currentTable;
    int currentIndex;

    file_lock f_lock(m_file_lock.c_str());
    scoped_lock<file_lock> e_lock(f_lock);

    currentTable = nowIndex / m_unitrownum;
    currentIndex = nowIndex % m_unitrownum;
    if((currentTable == m_queryCurrentTable) && (currentIndex == m_queryCurrentIndex))
    {
        LogUtility::Log(LOG_LEVEL_INFO, "LargeTableBase::RefreshPrevious query results end(%d, %d, %d, %d).", currentTable, currentIndex, m_queryCurrentTable, m_queryCurrentIndex);
        return -1;
    }

    if(currentIndex > 0)
    {
        currentIndex --;
    }
    else
    {
        currentIndex = m_unitrownum - 1;

        if(currentTable > 0)
        {
            currentTable --;
        }
        else
        {
            currentTable = m_tablenum - 1;
        }
    }

    if(m_schemaColNum < m_maxrownum)
    {
        if((currentTable * m_unitrownum + currentIndex) > (m_schemaCurrentTable * m_unitrownum + m_schemaCurrentIndex))
        {
            LogUtility::Log(LOG_LEVEL_INFO, "LargeTableBase::RefreshPrevious index beyond table end(%d, %d, %d, %d).", currentTable, currentIndex, m_schemaCurrentTable, m_schemaCurrentIndex);
            return -1;
        }
    }

    SQLiteTable table(GetExactDbFullName(currentTable), m_tableName + numToString<int>(currentTable), m_unitrownum);
    SetTable(table);
    if(table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshPrevious open table failed.");
        return -1;
    }
    m_values.clear();
    results.clear();
    constrain[LARGE_TABLE_COL_INDEX] = SQLiteValue(currentIndex);

    if(table.Query(constrain, results, returnResults) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshPrevious query failed.");
        table.Close();
        return -1;
    }

    table.Close();
    
    if(returnResults.size() != 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshPrevious query results invalid.");
        return -1;
    }
    
    m_values = *(returnResults.begin());
    
    return currentTable * m_unitrownum + currentIndex;
}

int LargeTableBase::RefreshPrevious(int nowIndex, int topIndex)
{
    set<string> results;
    int currentTable;
    int currentIndex;
    int min, max;
    bool invalid_flag = false;

    file_lock f_lock(m_file_lock.c_str());
    scoped_lock<file_lock> e_lock(f_lock);

    if(m_schemaColNum < m_maxrownum)
    {
        if(topIndex > nowIndex)
        {
            invalid_flag = true;
        }
    }
    else
    {
        if((topIndex > nowIndex) && (topIndex < m_schemaCurrentTable * m_unitrownum + m_schemaCurrentIndex))
        {
            invalid_flag = true;
        }
        else if((topIndex < m_schemaCurrentTable * m_unitrownum + m_schemaCurrentIndex) && (nowIndex > m_schemaCurrentTable * m_unitrownum + m_schemaCurrentIndex))
        {
            invalid_flag = true;
        }
    }

    if(invalid_flag)
    {
        LogUtility::Log(LOG_LEVEL_INFO, "LargeTableBase::RefreshPrevious invalid topindex(%d, %d, %d, %d).", topIndex, nowIndex, m_schemaCurrentTable, m_schemaCurrentIndex);
        return -1;
    }
    
    currentTable = nowIndex / m_unitrownum;
    currentIndex = nowIndex % m_unitrownum;
    if((currentTable == m_queryCurrentTable) && (currentIndex == m_queryCurrentIndex))
    {
        LogUtility::Log(LOG_LEVEL_INFO, "LargeTableBase::RefreshPrevious query results end(%d, %d, %d, %d).", currentTable, currentIndex, m_queryCurrentTable, m_queryCurrentIndex);
        return -1;
    }

    if(currentIndex > 0)
    {
        currentIndex --;
    }
    else
    {
        currentIndex = m_unitrownum - 1;

        if(currentTable > 0)
        {
            currentTable --;
        }
        else
        {
            currentTable = m_tablenum - 1;
        }
    }

    if(m_schemaColNum < m_maxrownum)
    {
        if((currentTable * m_unitrownum + currentIndex) > (m_schemaCurrentTable * m_unitrownum + m_schemaCurrentIndex))
        {
            LogUtility::Log(LOG_LEVEL_INFO, "LargeTableBase::RefreshPrevious index beyond table end(%d, %d, %d, %d).", currentTable, currentIndex, m_schemaCurrentTable, m_schemaCurrentIndex);
            return -1;
        }
    }

    max = currentIndex;
    if(topIndex >= currentTable * m_unitrownum)
    {
        min = topIndex - currentTable * m_unitrownum;
    }
    else
    {
        min = 0;
    }
    
    if(min >= m_unitrownum)
    {
        min = 0;
    }
    else if(min > max)
    {
        LogUtility::Log(LOG_LEVEL_INFO, "LargeTableBase::RefreshPrevious min exceed max(%d, %d).", min, max);
        return -1;
    }
    
    SQLiteTable table(GetExactDbFullName(currentTable), m_tableName + numToString<int>(currentTable), m_unitrownum);
    SetTable(table);
    if(table.Open() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshPrevious open table failed.");
        return -1;
    }
    m_values.clear();
    results.clear();
    m_returnResults.clear();
    
    if(table.QueryBetween(LARGE_TABLE_COL_INDEX, min, max, results, m_returnResults, false))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshPrevious query failed.");
        table.Close();
        return -1;
    }

    table.Close();
    
    if(m_returnResults.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshPrevious query results invalid.");
        return -1;
    }
    
    return currentTable * m_unitrownum + min;
}

int LargeTableBase::RefreshNearest(int colid, int value, bool top)
{
    SQLiteValuePair constrain;
    set<string> results;
    SQLiteValuePair returnResults;
    int currentTable;
    int save_currentTable = -1;
    int save_currentIndex = -1;
    bool found = false;
    int left, right;
    int leftindex, rightindex, saveleft, saveright;

    if(colid >= m_colnum)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest invalid colid.");
        return -1;
    }
    
    const SQLiteColumn &col = m_col[colid];
    if(col.GetType() != ENUM_SQLite_COLUMN_INTEGER)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest col must be integer.");
        return -1;
    }
    
    results.clear();

    file_lock f_lock(m_file_lock.c_str());
    scoped_lock<file_lock> e_lock(f_lock);

    if(m_schemaColNum < m_maxrownum)
    {
        if(m_maxrownum - m_schemaColNum < LARGE_TABLE_OVERHEAD)
        {
            int tmpIndex = LARGE_TABLE_OVERHEAD - (m_maxrownum - m_schemaColNum);
            leftindex = tmpIndex % m_unitrownum;
            left = tmpIndex / m_unitrownum;
        }
        else
        {
            left = 0;
            leftindex = 0;
        }
        right = m_schemaCurrentTable;
        rightindex = m_schemaCurrentIndex;
    }
    else
    {
        left = m_schemaCurrentTable;
        leftindex = m_schemaCurrentIndex;

        leftindex += LARGE_TABLE_OVERHEAD;
        left += (leftindex / m_unitrownum);
        leftindex %= m_unitrownum;
        if(left >= m_tablenum)
        {
            left -= m_tablenum;
        }

        right = m_schemaCurrentTable;
        rightindex = m_schemaCurrentIndex;
    }

    saveleft = left;
    saveright = right;
    m_values.clear();
    
    while(!found)
    {
        if(left <= right)
        {
            currentTable = (left + right) / 2;
        }
        else
        {
            int distance = m_tablenum - (left - right);
            currentTable = right - (distance / 2);
            if(currentTable < 0)
            {
                currentTable += m_tablenum;
            }
        }

        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest left %d, right %d, current %d.", left, right, currentTable);
        SQLiteTable table(GetExactDbFullName(currentTable), m_tableName + numToString<int>(currentTable), m_unitrownum);
        SetTable(table);
        if(table.Open() < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest open table failed.");
            return -1;
        }
        constrain.clear();
        constrain.insert(SQLiteValuePair::value_type(col.GetHead(), value));
        if(m_schemaColNum >= m_maxrownum)
        {
            if((top) && (currentTable == saveright))
            {
                constrain.insert(SQLiteValuePair::value_type(LARGE_TABLE_COL_INDEX, rightindex));
            }
            else if((!top) && (currentTable == saveleft))
            {
                constrain.insert(SQLiteValuePair::value_type(LARGE_TABLE_COL_INDEX, leftindex));
            }
        }
        int ret = table.QueryNearest(LARGE_TABLE_COL_INDEX, constrain, results, returnResults, top);
        table.Close();
        if(ret < 0)
        {
            if(top)
            {
                if(left == currentTable)
                {
                    if(m_values.size())
                    {
                        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest value found 1.");
                        found =  true;
                        break;
                    }
                    else
                    {
                        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest reach left and no value found.");
                        break;
                    }
                }
                right = currentTable - 1;
                if(right < 0)
                {
                    right = m_tablenum - 1;
                }
            }
            else
            {
                if(right == currentTable)
                {
                    if(m_values.size())
                    {
                        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest value found 2.");
                        found =  true;
                        break;
                    }
                    else
                    {
                        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest reach right and no value found.");
                        break;
                    }
                }
                left = currentTable + 1;
                if(left >= m_tablenum)
                {
                    left = 0;
                }
            }
        }
        else
        {
            int index =  GetSQLiteValueInt(returnResults[LARGE_TABLE_COL_INDEX]);
            m_values = returnResults;
            save_currentTable = currentTable;
            save_currentIndex = index;
            if(top)
            {
                if(index >= (m_unitrownum - 1))
                {
                    if(right == currentTable)
                    {
                        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest value found 3.");
                        found =  true;
                        break;
                    }
                    left = currentTable + 1;
                    if(left >= m_tablenum)
                    {
                        left = 0;
                    }
                }
                else
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest value found 4.");
                    found = true;
                    break;
                }
            }
            else
            {
                if(index <= 0)
                {
                    if(left == currentTable)
                    {
                        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest value found 5.");
                        found =  true;
                        break;
                    }
                    right = currentTable - 1;
                    if(right < 0)
                    {
                        right = m_tablenum - 1;
                    }
                }
                else
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest value found 6.");
                    found = true;
                    break;
                }
            }
        }
    }

    if(!found)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest find nothing.");
        return -1;
    }
    
    if(top)
    {
        if((saveleft == save_currentTable) && (save_currentIndex < leftindex))
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest left side beyond the overhead.");
            return -1;
        }
        if((saveright == save_currentTable) && (save_currentIndex > rightindex))
        {
            LogUtility::Log(LOG_LEVEL_INFO, "LargeTableBase::RefreshNearest align the right index.");
            save_currentIndex = rightindex;
        }
        if(m_schemaColNum < m_maxrownum)
        {
            if(m_maxrownum - m_schemaColNum < LARGE_TABLE_OVERHEAD)
            {
                int tmpIndex = LARGE_TABLE_OVERHEAD - (m_maxrownum - m_schemaColNum);
                m_queryCurrentIndex = tmpIndex % m_unitrownum;
                m_queryCurrentTable = tmpIndex / m_unitrownum;
            }
            else
            {
                m_queryCurrentIndex = 0;
                m_queryCurrentTable = 0;
            }
        }
        else
        {
            m_queryCurrentTable = m_schemaCurrentTable;
            m_queryCurrentIndex = m_schemaCurrentIndex;
            m_queryCurrentIndex += LARGE_TABLE_OVERHEAD;
            m_queryCurrentTable += (m_queryCurrentIndex / m_unitrownum);
            m_queryCurrentIndex %= m_unitrownum;
            if(m_queryCurrentTable >= m_tablenum)
            {
                m_queryCurrentTable -= m_tablenum;
            }
        }
    }
    else
    {
        if((saveleft == save_currentTable) && (save_currentIndex < leftindex))
        {
            LogUtility::Log(LOG_LEVEL_INFO, "LargeTableBase::RefreshNearest align the left index.");
            save_currentIndex = leftindex;
        }
        if((saveright == save_currentTable) && (save_currentIndex > rightindex))
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::RefreshNearest right side beyond the overhead.");
            return -1;
        }

        m_queryCurrentTable = m_schemaCurrentTable;
        m_queryCurrentIndex = m_schemaCurrentIndex;
    }
    
    return save_currentTable * m_unitrownum + save_currentIndex;
}

void LargeTableBase::GetFileLock()
{
    char *dir;
    
    dir = getenv("INNOV_TABLE_LOCK_DIR");
    if(dir == NULL)
    {
        m_file_lock = "/var/lock/";
    }
    else
    {
        m_file_lock = dir;
    }

    m_file_lock += m_dbName;

    m_file_lock += ".LARGE.LCK";
    
    if(access(m_file_lock.c_str(), F_OK) < 0)
    {
        int fd = creat(m_file_lock.c_str(), S_IRWXU|S_IRGRP|S_IRWXO);
        if(fd < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "LargeTableBase::GetFileLock failed to create lock file(%s).", m_file_lock.c_str());
        }
        else
        {
            close(fd);
        }
    }
}
