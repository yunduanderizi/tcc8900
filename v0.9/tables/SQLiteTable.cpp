#include "SQLiteTable.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace boost::interprocess;

SQLiteColumn::SQLiteColumn()
{
    Initialize();
}

SQLiteColumn::SQLiteColumn(string head, EnumSQLiteColumnType type)
{
    Initialize();
    m_head = head;
    m_type = type;
}

SQLiteColumn::SQLiteColumn(string head, EnumSQLiteColumnType type, bool notNull, bool noCase, bool autoIncrement, bool primaryKey, bool unique)
{
    Initialize();
    m_head = head;
    m_type = type;
    m_notNull = notNull;
    m_noCase = noCase;
    m_autoIncrement = autoIncrement;
    m_primaryKey = primaryKey;
    m_unique = unique;
}

SQLiteColumn::SQLiteColumn(string head, EnumSQLiteColumnType type, bool notNull, bool noCase, bool autoIncrement, bool primaryKey, bool unique, SQLiteValue value)
{
    Initialize();
    m_head = head;
    m_type = type;
    m_notNull = notNull;
    m_noCase = noCase;
    m_autoIncrement = autoIncrement;
    m_primaryKey = primaryKey;
    m_unique = unique;
    m_defaultValue = value;
    m_haveDefaultValue = true;
}

void SQLiteColumn::Initialize()
{
    m_head = "";
    m_notNull = false;
    m_noCase = true;
    m_autoIncrement = false;
    m_primaryKey = false;
    m_unique = false;
    m_type = ENUM_SQLite_COLUMN_STRING;
    m_haveDefaultValue = false;
}

void SQLiteColumn::SetHead(string head)
{
    m_head = head;
}

void SQLiteColumn::SetType(EnumSQLiteColumnType type)
{
    m_type = type;
}

void SQLiteColumn::SetNotNull(bool notNull)
{
    m_notNull = notNull;
}

void SQLiteColumn::SetNoCase(bool noCase)
{
    m_noCase = noCase;
}

void SQLiteColumn::SetAutoIncrement(bool autoIncrement)
{
    m_autoIncrement = autoIncrement;
}

void SQLiteColumn::SetPrimaryKey(bool primaryKey)
{
    m_primaryKey = primaryKey;
}

void SQLiteColumn::SetUnique(bool unique)
{
    m_unique = unique;
}

void SQLiteColumn::SetDefaultValue(string defaultValue)
{
    m_haveDefaultValue = true;
    m_defaultValue = defaultValue;
}

void SQLiteColumn::SetDefaultValue(float defaultValue)
{
    m_haveDefaultValue = true;
    m_defaultValue = defaultValue;
}

void SQLiteColumn::SetDefaultValue(int defaultValue)
{
    m_haveDefaultValue = true;
    m_defaultValue = defaultValue;
}

void SQLiteColumn::SetDefaultValue(SQLiteSequence defaultValue)
{
    m_haveDefaultValue = true;
    m_defaultValue = defaultValue;
}

string SQLiteColumn::DefaultValueToString()
{
    if(m_haveDefaultValue == false)
    {
        return "";
    }
    
    return SQLiteValueToString(m_type, m_defaultValue);
}

SQLiteDBLock SQLiteTable::m_dbLock;

SQLiteTable::SQLiteTable()
{
    Initialize();
}

SQLiteTable::SQLiteTable(string dbName, string tableName, unsigned int maxRows)
{
    Initialize();   
    m_dbName = dbName;
    m_tableName = tableName;
    m_maxRows = maxRows;
    
    m_mutex = m_dbLock.DeclareMutex(dbName);
    GetFileLock();
}

SQLiteTable::~SQLiteTable()
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable destruct.");
    Close();
}

void SQLiteTable::Initialize()
{
    m_dbName = "";
    m_tableName = "";
    m_columns.clear();
    m_columnsMap.clear();
    m_maxRows = 0;
    m_createIfNotExist = true;
    m_db = NULL;
    m_orderTitle = "rowid";
}

void SQLiteTable::GetFileLock()
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

    size_t pos = m_dbName.find_last_of('/');
    if(pos != string::npos)
    {
        m_file_lock += m_dbName.substr(pos+1);
    }
    else
    {
        m_file_lock += m_dbName;
    }

    m_file_lock += ".LCK";
    
    if(access(m_file_lock.c_str(), F_OK) < 0)
    {
        int fd = creat(m_file_lock.c_str(), S_IRWXU|S_IRGRP|S_IRWXO);
        if(fd < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::GetFileLock failed to create lock file(%s).", m_file_lock.c_str());
        }
        else
        {
            close(fd);
        }
    }
}

void SQLiteTable::SetDbName(string dbName)
{
    m_dbName = dbName;
    m_mutex = m_dbLock.DeclareMutex(dbName);
    GetFileLock();
}

void SQLiteTable::SetTableName(string tableName)
{
    m_tableName = tableName;

}

void SQLiteTable::SetMaxRows(unsigned int maxRows)
{
    m_maxRows = maxRows;
}

void SQLiteTable::SetCreateIfNotExist(bool createIfNotExist)
{
    m_createIfNotExist = createIfNotExist;
}

void SQLiteTable::SetOrderTitle(string orderTitle)
{
    m_orderTitle = orderTitle;
}

int SQLiteTable::AddColumn(SQLiteColumn column)
{
    if(m_columnsMap.count(column.GetHead()))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddColumn already have the column with the same head.");
        return -1;
    }
    m_columns.push_back(column);
    m_columnsMap.insert(map<string, SQLiteColumn>::value_type(column.GetHead(), column));
    return 0;
}

int SQLiteTable::AddIndex(string name, set<string> &index, bool unique)
{
    if(m_indexs.count(name))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddIndex already have the index with the same name.");
        return -1;
    }
    m_indexs.insert(map<string, SQLiteIndex>::value_type(name, SQLiteIndex(name, index, unique)));
    return 0;   
}

int SQLiteTable::Open()
{
    int ret;
    char **result;
    string sql;
    int nrows, ncols;
    char *err;
    
    if(m_dbName.length() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Open dbName is null.");
        return -1;
    }
    
    Close();
    
    ret = sqlite3_open(m_dbName.c_str(), &m_db);
    if(ret != SQLITE_OK)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Open sqlite3_open failed.");
        return -1;
    }

    sql = "select * from " + m_tableName;
    ret = sqlite3_get_table(m_db, sql.c_str(), &result, &nrows, &ncols, &err);
    sqlite3_free_table(result);
    if(ret != SQLITE_OK)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Open get table failed(%x).", ret);
        if (err != NULL)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Open ERROR(%s).", err);
            sqlite3_free(err);
        }
        if((ret == SQLITE_ERROR) && (m_createIfNotExist == true))
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Open try to create table.");
            if(Create() < 0)
            {
                LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Open failed to create table.");
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        if(nrows <= 0)
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Open 0 row.");
            if(InsertDefaultRow() < 0)
            {
                LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Open failed to InsertDefaultRow.");
            }
        }
    }

    return 0;
}

int SQLiteTable::Close()
{
    if(m_db)
    {
        int ret = sqlite3_close(m_db);
        if(ret != SQLITE_OK)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Close sqlite3_close failed.");
            return -1;
        }
        m_db = NULL;
    }
    
    return 0;
}

int SQLiteTable::Create()
{
    string sql;
    int ret;
    char *err;

    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Create db not opened yet.");
        return -1;
    }
    
    if(m_columns.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Create no column defined.");
        return -1;
    }
    
    sql = "create table " + m_tableName + " (";
    
    for(vector<SQLiteColumn>::iterator column = m_columns.begin(); column != m_columns.end(); column ++)
    {
        SQLiteColumn &col = (*column);
        if(column != m_columns.begin())
        {
            sql += ",\r\n";
        }
        sql += col.GetHead();
        switch(col.GetType())
        {
            case ENUM_SQLite_COLUMN_INTEGER:
                sql += " ";
                sql += "INTEGER";
                break;
            case ENUM_SQLite_COLUMN_FLOAT:
                sql += " ";
                sql += "REAL";
                break;
            case ENUM_SQLite_COLUMN_STRING:
            default:
                sql += " ";
                sql += "TEXT";
                break;
            case ENUM_SQLite_COLUMN_SEQUENCE:
                sql += " ";
                sql += "BLOB";
                break;
        }
        
        if(col.GetPrimaryKey())
        {
            sql += " PRIMARY KEY";
        }

        if(col.GetAutoIncrement())
        {
            sql += " AUTOINCREMENT";
        }

        if(col.GetUnique())
        {
            sql += " UNIQUE";
        }

        if(col.GetNotNull())
        {
            sql += " NOT NULL";
        }

        if(col.GetNoCase() && (col.GetType() == ENUM_SQLite_COLUMN_STRING))
        {
            sql += " COLLATE NOCASE";
        }
        
        if(col.GetHaveDefaultValue())
        {
            sql += " DEFAULT ";
            sql += col.DefaultValueToString();
        }
    }
    
    sql += ");";
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Create SQL:\r\n%s", sql.c_str());
    
    {
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
        file_lock f_lock(m_file_lock.c_str());
        scoped_lock<file_lock> e_lock(f_lock);
#else
        CommonThreadLock threadLock(m_mutex);
#endif

        ret = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, &err);
        if(ret != SQLITE_OK)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Create failed to exec sql.");
            if (err != NULL)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Create ERROR(%s).", err);
                sqlite3_free(err);
            }
            return -1;
        }
    }
    
    if(m_indexs.size())
    {
        for(map<string, SQLiteIndex>::iterator index = m_indexs.begin(); index != m_indexs.end(); index ++)
        {
            string name = index->first;
            SQLiteIndex &currentIndex  = index->second;
            set<string> &indexs = currentIndex.GetIndex();
            sql = "CREATE";
            if(currentIndex.GetUnique())
            {
                sql += " UNIQUE";
            }
            sql += " INDEX ";
            sql += name;
            sql += " ON ";
            sql += m_tableName;
            sql += "(";
            for(set<string>::iterator innerIndex = indexs.begin(); innerIndex != indexs.end(); innerIndex ++)
            {
                if(innerIndex != indexs.begin())
                {
                    sql += ", ";
                }
                string indexString = (*innerIndex);
                sql += indexString;
            }
            sql += ");";

            LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Create SQL:\r\n%s", sql.c_str());

#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
            file_lock f_lock(m_file_lock.c_str());
            scoped_lock<file_lock> e_lock(f_lock);
#else
            CommonThreadLock threadLock(m_mutex);
#endif

            ret = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, &err);
            if(ret != SQLITE_OK)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Create failed to exec sql to create index.");
                if (err != NULL)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Create ERROR(%s).", err);
                    sqlite3_free(err);
                }
                return -1;
            }
        }
    }
    
    InsertDefaultRow();
    
    return 0;
}

int SQLiteTable::InsertDefaultRow()
{
    SQLiteValuePair values;
    
    values.clear();
    for(vector<SQLiteColumn>::iterator column = m_columns.begin(); column != m_columns.end(); column ++)
    {
        SQLiteColumn &col = (*column);
        if(col.GetHaveDefaultValue() != true)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::InsertDefaultRow COL(%s) does not have default value.", col.GetHead().c_str());
            return -1;
        }
        
        values[col.GetHead()] = col.GetDefaultValue();
    }
    
    if(Add(values) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::InsertDefaultRow add failed.");
        return -1;
    }
    
    return 0;
}

int SQLiteTable::Add(SQLiteValuePair &values)
{
    string sql;
    string valueString = "";
    int ret;
    char *err;
    int rowNumber;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Add db not opened yet.");
        return -1;
    }
    
    if(values.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Add values are null.");
        return -1;
    }
    
    rowNumber = GetRowCount();
    if(rowNumber < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Add failed to get row count.");
        return -1;
    }
    
    if((m_maxRows !=0) && (rowNumber >= (int)m_maxRows) && (m_orderTitle.length() == 0))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Add table is full(%d,%d).", rowNumber, m_maxRows);
        return -1;
    }

    sql = "INSERT INTO ";
    sql += m_tableName;
    
    sql += "(";
    for(SQLiteValuePair::iterator pair = values.begin(); pair != values.end(); pair ++)
    {
        string colName = pair->first;
        SQLiteValue &value = pair->second;
        if(m_columnsMap.count(colName) <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Add unknown column(%s).", colName.c_str());
            return -1;
        }
        if(pair != values.begin())
        {
            sql += ", ";
            valueString += ", ";
        }
        sql += colName;
        SQLiteColumn &col = m_columnsMap[colName];
        valueString += SQLiteValueToString(col.GetType(), value);
    }
    sql += ") VALUES (";
    sql += valueString;
    sql += ");";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Add SQL:\r\n%s", sql.c_str());
    if(sql.length())
    {
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
        file_lock f_lock(m_file_lock.c_str());
        scoped_lock<file_lock> e_lock(f_lock);
#else
        CommonThreadLock threadLock(m_mutex);
#endif
        
        ret = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, &err);
        if(ret != SQLITE_OK)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Add failed to exec sql.");
            if (err != NULL)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Add ERROR(%s).", err);
                sqlite3_free(err);
            }
            return -1;
        }
        
    }

    if((m_maxRows !=0) && (rowNumber >= (int)m_maxRows))
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Add try to remove the top record.");
        set<string> results;
        SQLiteValuePair returnResults;
        int rowid;
        
        results.clear();
        results.insert("rowid");
        returnResults.clear();
        if(QueryTopBottom(m_orderTitle, results, returnResults, true) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Add QueryTopBottom failed.");
            return -1;
        }
        
        rowid = GetSQLiteValueInt(returnResults["rowid"]);
        LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Add top row id is %d.", rowid);
        if(Delete(returnResults) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Add failed to delete the top row.");
            return -1;
        }
    }
    
    return 0;
}

int SQLiteTable::AddBatch(set<SQLiteValuePair> &values)
{
    string sql;
    string valueString = "";
    int ret;
    char *err;
    int rowNumber;
    sqlite3_stmt *stmt;
    const char *tail;
    SQLiteValuePair pairValues;
    vector<string> titles;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch db not opened yet.");
        return -1;
    }
    
    if(values.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch values are null.");
        return -1;
    }
    
    rowNumber = GetRowCount();
    if(rowNumber < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch failed to get row count.");
        return -1;
    }
    
    if((m_maxRows !=0) && (rowNumber >= (int)m_maxRows) && (m_orderTitle.length() == 0))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch table is full(%d,%d).", rowNumber, m_maxRows);
        return -1;
    }

    sql = "INSERT INTO ";
    sql += m_tableName;
    
    sql += "(";
    pairValues = *(values.begin());
    for(SQLiteValuePair::iterator pair = pairValues.begin(); pair != pairValues.end(); pair ++)
    {
        string colName = pair->first;
        if(m_columnsMap.count(colName) <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Add unknown column(%s).", colName.c_str());
            return -1;
        }
        if(pair != pairValues.begin())
        {
            sql += ", ";
            valueString += ", ";
        }
        sql += colName;
        titles.push_back(colName);
        valueString += "?";
    }
    sql += ") VALUES (";
    sql += valueString;
    sql += ");";

    if(sql.length())
    {
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
        file_lock f_lock(m_file_lock.c_str());
        scoped_lock<file_lock> e_lock(f_lock);
#else
        CommonThreadLock threadLock(m_mutex);
#endif
        ret = sqlite3_exec(m_db, "BEGIN TRANSACTION", NULL, NULL, &err);
        if(ret != SQLITE_OK)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch failed to exec start transaction.");
            if (err != NULL)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch ERROR(%s).", err);
                sqlite3_free(err);
            }
            return -1;
        }

        LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::AddBatch SQL:\r\n%s", sql.c_str());
        sqlite3_prepare(m_db, sql.c_str(), sql.length(), &stmt, &tail);

        for(set<SQLiteValuePair>::iterator it = values.begin(); it != values.end(); it ++)
        {
            int colcount = 1;
            SQLiteValuePair pairValues = *it;
            for(int i = 0; i < (int)titles.size(); i ++)
            {
                string &colName = titles[i];
                if(pairValues.count(colName) <= 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch inconsistency rows(%s).", titles[i].c_str());
                    continue;
                }
                SQLiteValue &value = pairValues[colName];
                SQLiteColumn &col = m_columnsMap[colName];
                string valueString;
                unsigned char *ptr;
                SQLiteSequence valueSeq;
                switch(col.GetType())
                {
                    case ENUM_SQLite_COLUMN_INTEGER:
                        sqlite3_bind_int(stmt, colcount, GetSQLiteValueInt(value));
                        break;
                    case ENUM_SQLite_COLUMN_FLOAT:
                        sqlite3_bind_double(stmt, colcount, (double)GetSQLiteValueFloat(value));
                        break;
                    case ENUM_SQLite_COLUMN_STRING:
                        GetSQLiteValueString(value, valueString);
                        sqlite3_bind_text(stmt, colcount, valueString.c_str(), valueString.length(), SQLITE_TRANSIENT);
                        break;
                    case ENUM_SQLite_COLUMN_SEQUENCE:
                        GetSQLiteValueSequence(value, valueSeq);
                        ptr = (unsigned char *)malloc(valueSeq.size());
                        for(int i = 0; i < (int)valueSeq.size(); i ++)
                        {
                            *(ptr + i) = valueSeq[i];
                        }
                        sqlite3_bind_blob(stmt, colcount, ptr, valueSeq.size(), SQLITE_TRANSIENT);
                        free(ptr);
                        break;
                }
                colcount ++;
            }
            sqlite3_step(stmt);
            sqlite3_clear_bindings(stmt);
            sqlite3_reset(stmt);
        }
        ret = sqlite3_exec(m_db, "END TRANSACTION", NULL, NULL, &err);
        if(ret != SQLITE_OK)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch failed to exec end transaction.");
            if (err != NULL)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch ERROR(%s).", err);
                sqlite3_free(err);
            }
            return -1;
        }
        sqlite3_finalize(stmt);
    }
    if((m_maxRows !=0) && (rowNumber + values.size() > m_maxRows))
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::AddBatch try to remove the top record.");
        set<string> results;
        vector<SQLiteValuePair> returnResults;
        int minid = 0x0FFFFFFF, maxid = 0;
        
        results.clear();
        results.insert("rowid");
        returnResults.clear();
        if(QueryTopBottomMultiple(m_orderTitle, results, returnResults, rowNumber + values.size() - m_maxRows, true) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch QueryTopBottomMultiple failed.");
            return -1;
        }

        for(vector<SQLiteValuePair>::iterator it = returnResults.begin(); it != returnResults.end(); it ++)
        {
            SQLiteValuePair pairValues = *it;
            if(pairValues.count("rowid") <= 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch no rowid.");
                continue;
            }
            SQLiteValue &value = pairValues["rowid"];
            int row = GetSQLiteValueInt(value);
            if(minid > row)
            {
                minid = row;
            }
            if(maxid < row)
            {
                maxid = row;
            }
        }
        
        sql = "DELETE FROM ";
        sql += m_tableName;
        sql += " WHERE ";
        sql += "rowid>=";
        sql += numToString<int>(minid);
        sql += " AND rowid<=";
        sql += numToString<int>(maxid);
        sql += ";";
        LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::AddBatch SQL:\r\n%s", sql.c_str());
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
        file_lock f_lock(m_file_lock.c_str());
        scoped_lock<file_lock> e_lock(f_lock);
#else
        CommonThreadLock threadLock(m_mutex);
#endif

        ret = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, &err);
        if(ret != SQLITE_OK)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::AddBatch failed to exec sql.");
            if (err != NULL)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Delete AddBatch(%s).", err);
                sqlite3_free(err);
            }
            return -1;
        }
    }
    
    return 0;
}

int SQLiteTable::Query(SQLiteValuePair &constrain, set<string> &results, set<SQLiteValuePair> &returnResults)
{
    string sql;
    int ret;
    sqlite3_stmt *stmt;
    int ncols = 0;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Query db not opened yet.");
        return -1;
    }

    sql = "SELECT ";

    if(results.size())
    {
        for(set<string>::iterator result = results.begin(); result != results.end(); result ++)
        {
            if(result != results.begin())
            {
                sql += ", ";
            }
            string resultName = *result;
            sql += resultName;
        }
    }
    else
    {
        sql += "*";
    }
    
    sql += " FROM ";
    sql += m_tableName;
    if(constrain.size())
    {
        sql += " WHERE ";
        for(SQLiteValuePair::iterator pair = constrain.begin(); pair != constrain.end(); pair ++)
        {
            string colName = pair->first;
            SQLiteValue &value = pair->second;
            if(m_columnsMap.count(colName) <= 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Query unknown column(%s).", colName.c_str());
                return -1;
            }
            if(pair != constrain.begin())
            {
                sql += " AND ";
            }
            sql += colName;
            sql += "=";
            SQLiteColumn &col = m_columnsMap[colName];
            sql += SQLiteValueToString(col.GetType(), value);
        }
    }
    sql += ";";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Query SQL:\r\n%s", sql.c_str());

#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
    file_lock f_lock(m_file_lock.c_str());
    sharable_lock<file_lock> sh_lock(f_lock);
#else
    CommonThreadLock threadLock(m_mutex);
#endif
    
    sqlite3_prepare(m_db, sql.c_str(), sql.length(), &stmt, NULL);
    ncols = sqlite3_column_count(stmt);
    ret = sqlite3_step(stmt);
    
    returnResults.clear();
    
    while(ret == SQLITE_ROW)
    {
        SQLiteValuePair valuePair;
        valuePair.clear();
        for(int i=0; i < ncols; i++)
        {
            int type = sqlite3_column_type(stmt, i);
            switch(type)
            {
                case SQLITE_INTEGER:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), sqlite3_column_int(stmt, i)));
                    break;
                case SQLITE_FLOAT:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), (float)(sqlite3_column_double(stmt, i))));
                    break;
                case SQLITE_TEXT:
                default:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), string((const char *)(sqlite3_column_text(stmt, i)))));
                    break;
                case SQLITE_BLOB:
                    const unsigned char *ptr = (const unsigned char *)sqlite3_column_blob(stmt, i);
                    const unsigned char *ptrEnd = ptr + sqlite3_column_bytes(stmt, i);
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), SQLiteSequence(ptr, ptrEnd)));
                    break;
            }
        }
        returnResults.insert(valuePair);
        ret = sqlite3_step(stmt);
    }
    
    sqlite3_finalize(stmt);

    return 0;
}

int SQLiteTable::Query(SQLiteValuePair &constrain, set<string> &results, vector<SQLiteValuePair> &returnResults, const string &orderTitle, bool asc)
{
    string sql;
    int ret;
    sqlite3_stmt *stmt;
    int ncols = 0;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Query db not opened yet.");
        return -1;
    }

    if(orderTitle != "rowid")
    {
        if(m_columnsMap.count(orderTitle) <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Query invalid orderTitle.");
            return -1;
        }
    }
    
    sql = "SELECT ";

    if(results.size())
    {
        for(set<string>::iterator result = results.begin(); result != results.end(); result ++)
        {
            if(result != results.begin())
            {
                sql += ", ";
            }
            string resultName = *result;
            sql += resultName;
        }
    }
    else
    {
        sql += "*";
    }
    
    sql += " FROM ";
    sql += m_tableName;
    if(constrain.size())
    {
        sql += " WHERE ";
        for(SQLiteValuePair::iterator pair = constrain.begin(); pair != constrain.end(); pair ++)
        {
            string colName = pair->first;
            SQLiteValue &value = pair->second;
            if(m_columnsMap.count(colName) <= 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Query unknown column(%s).", colName.c_str());
                return -1;
            }
            if(pair != constrain.begin())
            {
                sql += " AND ";
            }
            sql += colName;
            sql += "=";
            SQLiteColumn &col = m_columnsMap[colName];
            sql += SQLiteValueToString(col.GetType(), value);
        }
    }
    sql += " ORDER BY ";
    sql += orderTitle;
    if(asc)
    {
        sql += " ASC";
    }
    else
    {
        sql += " DESC";
    }
    sql += ";";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Query SQL:\r\n%s", sql.c_str());

#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
    file_lock f_lock(m_file_lock.c_str());
    sharable_lock<file_lock> sh_lock(f_lock);
#else
    CommonThreadLock threadLock(m_mutex);
#endif
    
    sqlite3_prepare(m_db, sql.c_str(), sql.length(), &stmt, NULL);
    ncols = sqlite3_column_count(stmt);
    ret = sqlite3_step(stmt);
    
    returnResults.clear();
    
    while(ret == SQLITE_ROW)
    {
        SQLiteValuePair valuePair;
        valuePair.clear();
        for(int i=0; i < ncols; i++)
        {
            int type = sqlite3_column_type(stmt, i);
            switch(type)
            {
                case SQLITE_INTEGER:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), sqlite3_column_int(stmt, i)));
                    break;
                case SQLITE_FLOAT:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), (float)(sqlite3_column_double(stmt, i))));
                    break;
                case SQLITE_TEXT:
                default:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), string((const char *)(sqlite3_column_text(stmt, i)))));
                    break;
                case SQLITE_BLOB:
                    const unsigned char *ptr = (const unsigned char *)sqlite3_column_blob(stmt, i);
                    const unsigned char *ptrEnd = ptr + sqlite3_column_bytes(stmt, i);
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), SQLiteSequence(ptr, ptrEnd)));
                    break;
            }
        }
        returnResults.push_back(valuePair);
        ret = sqlite3_step(stmt);
    }
    
    sqlite3_finalize(stmt);

    return 0;
}

int SQLiteTable::QueryNearest(string orderTitle, SQLiteValuePair &constrain, set<string> &results, SQLiteValuePair &returnResults, bool top)
{
    string sql;
    int ret;
    sqlite3_stmt *stmt;
    int ncols = 0;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Query db not opened yet.");
        return -1;
    }

    sql = "SELECT ";

    if(results.size())
    {
        for(set<string>::iterator result = results.begin(); result != results.end(); result ++)
        {
            if(result != results.begin())
            {
                sql += ", ";
            }
            string resultName = *result;
            sql += resultName;
        }
    }
    else
    {
        sql += "*";
    }
    
    sql += " FROM ";
    sql += m_tableName;
    if(constrain.size())
    {
        sql += " WHERE ";
        for(SQLiteValuePair::iterator pair = constrain.begin(); pair != constrain.end(); pair ++)
        {
            string colName = pair->first;
            SQLiteValue &value = pair->second;
            if(m_columnsMap.count(colName) <= 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Query unknown column(%s).", colName.c_str());
                return -1;
            }
            if(pair != constrain.begin())
            {
                sql += " AND ";
            }
            sql += colName;
            if(top)
            {
                sql += "<=";
            }
            else
            {
                sql += ">=";
            }
            SQLiteColumn &col = m_columnsMap[colName];
            sql += SQLiteValueToString(col.GetType(), value);
        }
    }
    sql += " ORDER BY ";
    sql += orderTitle;
    if(top)
    {
        sql += " DESC ";
    }
    else
    {
        sql += " ASC ";
    }
    sql += "LIMIT 1";
    sql += ";";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Query SQL:\r\n%s", sql.c_str());

#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
    file_lock f_lock(m_file_lock.c_str());
    sharable_lock<file_lock> sh_lock(f_lock);
#else
    CommonThreadLock threadLock(m_mutex);
#endif
    
    sqlite3_prepare(m_db, sql.c_str(), sql.length(), &stmt, NULL);
    ncols = sqlite3_column_count(stmt);
    ret = sqlite3_step(stmt);
    
    returnResults.clear();
    
    if(ret == SQLITE_ROW)
    {
        for(int i=0; i < ncols; i++)
        {
            int type = sqlite3_column_type(stmt, i);
            switch(type)
            {
                case SQLITE_INTEGER:
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), sqlite3_column_int(stmt, i)));
                    break;
                case SQLITE_FLOAT:
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), (float)(sqlite3_column_double(stmt, i))));
                    break;
                case SQLITE_TEXT:
                default:
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), string((const char *)(sqlite3_column_text(stmt, i)))));
                    break;
                case SQLITE_BLOB:
                    const unsigned char *ptr = (const unsigned char *)sqlite3_column_blob(stmt, i);
                    const unsigned char *ptrEnd = ptr + sqlite3_column_bytes(stmt, i);
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), SQLiteSequence(ptr, ptrEnd)));
                    break;
            }
        }
    }
    else
    {
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);

    return 0;
}

int SQLiteTable::QueryLatest(set<string> &results, SQLiteValuePair &returnResults)
{
    return QueryTopBottom("rowid", results, returnResults, false);
}

int SQLiteTable::QueryTopBottom(const string &orderTitle, set<string> &results, SQLiteValuePair &returnResults, bool top)
{
    string sql;
    int ret;
    sqlite3_stmt *stmt;
    int ncols = 0;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::QueryTopBottom db not opened yet.");
        return -1;
    }

    if(orderTitle != "rowid")
    {
        if(m_columnsMap.count(orderTitle) <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::QueryTopBottom invalid orderTitle.");
            return -1;
        }
    }
    
    sql = "SELECT ";
    
    if(results.size())
    {
        for(set<string>::iterator result = results.begin(); result != results.end(); result ++)
        {
            if(result != results.begin())
            {
                sql += ", ";
            }
            string resultName = *result;
            sql += resultName;
        }
    }
    else
    {
        sql += "*";
    }
    
    sql += " FROM ";
    sql += m_tableName;
    sql += " ORDER BY ";
    sql += orderTitle;
    if(top)
    {
        sql += " ASC ";
    }
    else
    {
        sql += " DESC ";
    }
    sql += "LIMIT 1;";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::QueryTopBottom SQL:\r\n%s", sql.c_str());
    
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
    file_lock f_lock(m_file_lock.c_str());
    sharable_lock<file_lock> sh_lock(f_lock);
#else
    CommonThreadLock threadLock(m_mutex);
#endif
    
    sqlite3_prepare(m_db, sql.c_str(), sql.length(), &stmt, NULL);
    ncols = sqlite3_column_count(stmt);
    ret = sqlite3_step(stmt);
    
    returnResults.clear();
    
    if(ret == SQLITE_ROW)
    {
        for(int i=0; i < ncols; i++)
        {
            int type = sqlite3_column_type(stmt, i);
            switch(type)
            {
                case SQLITE_INTEGER:
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), sqlite3_column_int(stmt, i)));
                    break;
                case SQLITE_FLOAT:
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), (float)(sqlite3_column_double(stmt, i))));
                    break;
                case SQLITE_TEXT:
                default:
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), string((const char *)(sqlite3_column_text(stmt, i)))));
                    break;
                case SQLITE_BLOB:
                    const unsigned char *ptr = (const unsigned char *)sqlite3_column_blob(stmt, i);
                    const unsigned char *ptrEnd = ptr + sqlite3_column_bytes(stmt, i);
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), SQLiteSequence(ptr, ptrEnd)));
                    break;
            }
        }
    }
    else
    {
        sqlite3_finalize(stmt);
        return -1;
    }
    
    sqlite3_finalize(stmt);

    return 0;
}

int SQLiteTable::QueryTopBottomMultiple(const string &orderTitle, set<string> &results, vector<SQLiteValuePair> &returnResults, int count, bool top)
{
    string sql;
    int ret;
    sqlite3_stmt *stmt;
    int ncols = 0;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::QueryTopBottomMultiple db not opened yet.");
        return -1;
    }

    if(orderTitle != "rowid")
    {
        if(m_columnsMap.count(orderTitle) <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::QueryTopBottomMultiple invalid orderTitle.");
            return -1;
        }
    }
    
    sql = "SELECT ";
    
    if(results.size())
    {
        for(set<string>::iterator result = results.begin(); result != results.end(); result ++)
        {
            if(result != results.begin())
            {
                sql += ", ";
            }
            string resultName = *result;
            sql += resultName;
        }
    }
    else
    {
        sql += "*";
    }
    
    sql += " FROM ";
    sql += m_tableName;
    sql += " ORDER BY ";
    sql += orderTitle;
    if(top)
    {
        sql += " ASC ";
    }
    else
    {
        sql += " DESC ";
    }
    sql += "LIMIT ";
    sql += numToString<int>(count);
    sql += ";";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::QueryTopBottomMultiple SQL:\r\n%s", sql.c_str());
    
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
    file_lock f_lock(m_file_lock.c_str());
    sharable_lock<file_lock> sh_lock(f_lock);
#else
    CommonThreadLock threadLock(m_mutex);
#endif
    
    sqlite3_prepare(m_db, sql.c_str(), sql.length(), &stmt, NULL);
    ncols = sqlite3_column_count(stmt);
    ret = sqlite3_step(stmt);
    
    while(ret == SQLITE_ROW)
    {
        SQLiteValuePair valuePair;
        valuePair.clear();
        for(int i=0; i < ncols; i++)
        {
            int type = sqlite3_column_type(stmt, i);
            switch(type)
            {
                case SQLITE_INTEGER:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), sqlite3_column_int(stmt, i)));
                    break;
                case SQLITE_FLOAT:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), (float)(sqlite3_column_double(stmt, i))));
                    break;
                case SQLITE_TEXT:
                default:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), string((const char *)(sqlite3_column_text(stmt, i)))));
                    break;
                case SQLITE_BLOB:
                    const unsigned char *ptr = (const unsigned char *)sqlite3_column_blob(stmt, i);
                    const unsigned char *ptrEnd = ptr + sqlite3_column_bytes(stmt, i);
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), SQLiteSequence(ptr, ptrEnd)));
                    break;
            }
        }
        returnResults.push_back(valuePair);
        ret = sqlite3_step(stmt);
    }
    
    sqlite3_finalize(stmt);

    return 0;
}

int SQLiteTable::QueryNext(const string &orderTitle, set<string> &results, SQLiteValuePair &returnResults, SQLiteValue &currentValue)
{
    string sql;
    int ret;
    sqlite3_stmt *stmt;
    int ncols = 0;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::QueryNext db not opened yet.");
        return -1;
    }

    if(orderTitle != "rowid")
    {
        if(m_columnsMap.count(orderTitle) <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::QueryNext invalid orderTitle.");
            return -1;
        }
    }
    
    sql = "SELECT ";
    if(results.size())
    {
        for(set<string>::iterator result = results.begin(); result != results.end(); result ++)
        {
            if(result != results.begin())
            {
                sql += ", ";
            }
            string resultName = *result;
            sql += resultName;
        }
    }
    else
    {
        sql += "*";
    }
    
    sql += " FROM ";
    sql += m_tableName;
    sql += " WHERE ";
    sql += orderTitle;
    sql += " > ";
    if(orderTitle != "rowid")
    {
        SQLiteColumn &col = m_columnsMap[orderTitle];
        sql += SQLiteValueToString(col.GetType(), currentValue);
    }
    else
    {
        sql += SQLiteValueToString(ENUM_SQLite_COLUMN_INTEGER, currentValue);
    }
    sql += " ORDER BY ";
    sql += orderTitle;
    sql += " ASC ";
    sql += "LIMIT 1;";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::QueryNext SQL:\r\n%s", sql.c_str());
    
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
    file_lock f_lock(m_file_lock.c_str());
    sharable_lock<file_lock> sh_lock(f_lock);
#else
    CommonThreadLock threadLock(m_mutex);
#endif
    
    sqlite3_prepare(m_db, sql.c_str(), sql.length(), &stmt, NULL);
    ncols = sqlite3_column_count(stmt);
    ret = sqlite3_step(stmt);
    
    returnResults.clear();
    
    if(ret == SQLITE_ROW)
    {
        for(int i=0; i < ncols; i++)
        {
            int type = sqlite3_column_type(stmt, i);
            switch(type)
            {
                case SQLITE_INTEGER:
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), sqlite3_column_int(stmt, i)));
                    break;
                case SQLITE_FLOAT:
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), (float)(sqlite3_column_double(stmt, i))));
                    break;
                case SQLITE_TEXT:
                default:
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), string((const char *)(sqlite3_column_text(stmt, i)))));
                    break;
                case SQLITE_BLOB:
                    const unsigned char *ptr = (const unsigned char *)sqlite3_column_blob(stmt, i);
                    const unsigned char *ptrEnd = ptr + sqlite3_column_bytes(stmt, i);
                    returnResults.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), SQLiteSequence(ptr, ptrEnd)));
                    break;
            }
        }
    }
    else
    {
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);

    return 0;
}

int SQLiteTable::QueryBetween(const string &orderTitle, int min, int max, set<string> &results, vector<SQLiteValuePair> &returnResults, bool asc)
{
    string sql;
    int ret;
    sqlite3_stmt *stmt;
    int ncols = 0;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::QueryBetween db not opened yet.");
        return -1;
    }

    if(orderTitle != "rowid")
    {
        if(m_columnsMap.count(orderTitle) <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::QueryBetween invalid orderTitle.");
            return -1;
        }
    }
    
    sql = "SELECT ";
    
    if(results.size())
    {
        for(set<string>::iterator result = results.begin(); result != results.end(); result ++)
        {
            if(result != results.begin())
            {
                sql += ", ";
            }
            string resultName = *result;
            sql += resultName;
        }
    }
    else
    {
        sql += "*";
    }
    
    sql += " FROM ";
    sql += m_tableName;
    sql += " WHERE ";
    sql += orderTitle;
    sql += " <= ";
    sql += numToString<int>(max);
    sql += " AND ";
    sql += orderTitle;
    sql += " >= ";
    sql += numToString<int>(min);
    sql += " ORDER BY ";
    sql += orderTitle;
    if(asc)
    {
        sql += " ASC";
    }
    else
    {
        sql += " DESC";
    }
    sql += ";";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::QueryBetween SQL:\r\n%s", sql.c_str());
    
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
    file_lock f_lock(m_file_lock.c_str());
    sharable_lock<file_lock> sh_lock(f_lock);
#else
    CommonThreadLock threadLock(m_mutex);
#endif
    
    sqlite3_prepare(m_db, sql.c_str(), sql.length(), &stmt, NULL);
    ncols = sqlite3_column_count(stmt);
    ret = sqlite3_step(stmt);
    
    while(ret == SQLITE_ROW)
    {
        SQLiteValuePair valuePair;
        valuePair.clear();
        for(int i=0; i < ncols; i++)
        {
            int type = sqlite3_column_type(stmt, i);
            switch(type)
            {
                case SQLITE_INTEGER:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), sqlite3_column_int(stmt, i)));
                    break;
                case SQLITE_FLOAT:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), (float)(sqlite3_column_double(stmt, i))));
                    break;
                case SQLITE_TEXT:
                default:
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), string((const char *)(sqlite3_column_text(stmt, i)))));
                    break;
                case SQLITE_BLOB:
                    const unsigned char *ptr = (const unsigned char *)sqlite3_column_blob(stmt, i);
                    const unsigned char *ptrEnd = ptr + sqlite3_column_bytes(stmt, i);
                    valuePair.insert(SQLiteValuePair::value_type(sqlite3_column_name(stmt, i), SQLiteSequence(ptr, ptrEnd)));
                    break;
            }
        }
        returnResults.push_back(valuePair);
        ret = sqlite3_step(stmt);
    }
    
    sqlite3_finalize(stmt);

    return 0;
}

int SQLiteTable::Delete(SQLiteValuePair &constrain)
{
    string sql;
    int ret;
    char *err;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Delete db not opened yet.");
        return -1;
    }

    sql = "DELETE FROM ";
    sql += m_tableName;
    if(constrain.size() > 0)
    {
        sql += " WHERE ";

        for(SQLiteValuePair::iterator pair = constrain.begin(); pair != constrain.end(); pair ++)
        {
            string colName = pair->first;
            SQLiteValue &value = pair->second;
            if(colName != "rowid")
            {
                if(m_columnsMap.count(colName) <= 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Delete unknown column(%s).", colName.c_str());
                    return -1;
                }
            }
            if(pair != constrain.begin())
            {
                sql += " AND ";
            }
            sql += colName;
            sql += "=";
            if(colName != "rowid")
            {
                SQLiteColumn &col = m_columnsMap[colName];
                sql += SQLiteValueToString(col.GetType(), value);
            }
            else
            {
                sql += SQLiteValueToString(ENUM_SQLite_COLUMN_INTEGER, value);
            }
        }
    }
    sql += ";";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Delete SQL:\r\n%s", sql.c_str());
    
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
    file_lock f_lock(m_file_lock.c_str());
    scoped_lock<file_lock> e_lock(f_lock);
#else
    CommonThreadLock threadLock(m_mutex);
#endif
    
    ret = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, &err);
    if(ret != SQLITE_OK)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Delete failed to exec sql.");
        if (err != NULL)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Delete ERROR(%s).", err);
            sqlite3_free(err);
        }
        return -1;
    }
    
    return 0;
}

int SQLiteTable::Modify(SQLiteValuePair &constrain, SQLiteValuePair &values)
{
    string sql;
    int ret;
    char *err;
    
    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Modify db not opened yet.");
        return -1;
    }

    if(values.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Modify values are null.");
        return -1;
    }

    sql = "UPDATE ";
    sql += m_tableName;
    
    sql += " SET ";
    for(SQLiteValuePair::iterator pair = values.begin(); pair != values.end(); pair ++)
    {
        string colName = pair->first;
        SQLiteValue &value = pair->second;
        if(m_columnsMap.count(colName) <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Modify unknown column(%s).", colName.c_str());
            return -1;
        }
        if(pair != values.begin())
        {
            sql += ", ";
        }
        sql += colName;
        sql += "=";
        SQLiteColumn &col = m_columnsMap[colName];
        sql += SQLiteValueToString(col.GetType(), value);
    }

    if(constrain.size())
    {
        sql += " WHERE ";
        for(SQLiteValuePair::iterator pair = constrain.begin(); pair != constrain.end(); pair ++)
        {
            string colName = pair->first;
            SQLiteValue &value = pair->second;
            if(m_columnsMap.count(colName) <= 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Modify unknown column(%s).", colName.c_str());
                return -1;
            }
            if(pair != constrain.begin())
            {
                sql += " AND ";
            }
            sql += colName;
            sql += "=";
            SQLiteColumn &col = m_columnsMap[colName];
            sql += SQLiteValueToString(col.GetType(), value);
        }
    }
    sql += ";";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::Modify SQL:\r\n%s", sql.c_str());
    
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
    file_lock f_lock(m_file_lock.c_str());
    scoped_lock<file_lock> e_lock(f_lock);
#else
    CommonThreadLock threadLock(m_mutex);
#endif
    
    ret = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, &err);
    if(ret != SQLITE_OK)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Modify failed to exec sql.");
        if (err != NULL)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::Modify ERROR(%s).", err);
            sqlite3_free(err);
        }
        return -1;
    }
    
    return 0;
}

int SQLiteTable::GetRowCount()
{
    string sql;
    int ret;
    int nrows, ncols;
    sqlite3_stmt *stmt;

    if(m_db == NULL)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::GetRowCount db not opened yet.");
        return -1;
    }

    sql = "SELECT COUNT(*) FROM ";
    sql += m_tableName;
    sql += ";";

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::GetRowCount SQL:\r\n%s", sql.c_str());
    
#ifdef INNOV_SQLITE_TABLE_FILE_LOCK
    file_lock f_lock(m_file_lock.c_str());
    sharable_lock<file_lock> sh_lock(f_lock);
#else
    CommonThreadLock threadLock(m_mutex);
#endif

    sqlite3_prepare(m_db, sql.c_str(), sql.length(), &stmt, NULL);
    ncols = sqlite3_column_count(stmt);
    ret = sqlite3_step(stmt);
    
    if(ncols != 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::GetRowCount count invalid(%d).", ncols);
        sqlite3_finalize(stmt);
        return -1;
    }
    
    if(ret == SQLITE_ROW)
    {
        int type = sqlite3_column_type(stmt, 0);
        if(type != SQLITE_INTEGER)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteTable::GetRowCount result type invalid(%d).", type);
            sqlite3_finalize(stmt);
            return -1;
        }
        nrows = sqlite3_column_int(stmt, 0);
    }
    else
    {
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);

    LogUtility::Log(LOG_LEVEL_DEBUG, "SQLiteTable::GetRowCount %d.", nrows);
    return nrows;
}
