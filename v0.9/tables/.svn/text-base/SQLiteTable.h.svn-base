#ifndef _SQLITE_TABLE_H_
#define _SQLITE_TABLE_H_

#include "sqlite3.h"
#include "boost/variant.hpp"
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "Utility.h"
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <pthread.h>
#include "LogUtility.h"
#include "Utility.h"

#define INNOV_SQLITE_TABLE_FILE_LOCK    1

using namespace std;

typedef enum
{
    ENUM_SQLite_COLUMN_INTEGER,
    ENUM_SQLite_COLUMN_FLOAT,
    ENUM_SQLite_COLUMN_STRING,
    ENUM_SQLite_COLUMN_SEQUENCE
} EnumSQLiteColumnType;

typedef vector<unsigned char> SQLiteSequence;
typedef boost::variant<int, float, string, SQLiteSequence> SQLiteValue;

inline void GetSQLiteValueString(SQLiteValue &value, string &stringValue)
{
    string *ptr = boost::get<string>(&value);
    if(ptr == NULL)
    {
        return;
    }
    else
    {
        stringValue = *ptr;
        return;
    }
}

inline int GetSQLiteValueInt(SQLiteValue &value)
{
    int *ptr = boost::get<int>(&value);
    if(ptr == NULL)
    {
        return -1;
    }
    else
    {
        return *ptr;
    }
}

inline float GetSQLiteValueFloat(SQLiteValue &value)
{
    float *ptr = boost::get<float>(&value);
    if(ptr == NULL)
    {
        return -1.0;
    }
    else
    {
        return *ptr;
    }
}

inline void GetSQLiteValueSequence(SQLiteValue &value, SQLiteSequence &sequence)
{
    SQLiteSequence *ptr =  boost::get<SQLiteSequence>(&value);
    if(ptr == NULL)
    {
        return;
    }
    else
    {
        sequence = *ptr;
    }
    
    return;
}

// This function is used to create sql.
inline string SQLiteValueToString(EnumSQLiteColumnType type, SQLiteValue &value)
{
    string stringValue = "";

    switch(type)
    {
        case ENUM_SQLite_COLUMN_INTEGER:
            return numToString<int>(GetSQLiteValueInt(value));
        case ENUM_SQLite_COLUMN_FLOAT:
            return numToString<float>(GetSQLiteValueFloat(value));
        case ENUM_SQLite_COLUMN_STRING:
        default:
            GetSQLiteValueString(value, stringValue);
            return "'" + stringValue + "'";
        case ENUM_SQLite_COLUMN_SEQUENCE:
            string hex = "x'";
            SQLiteSequence seq;
            GetSQLiteValueSequence(value, seq);
            for(int i = 0; i < (int)seq.size(); i ++)
            {
                unsigned char ch = seq[i];
                hex += hexToString((int)ch);
            }
            hex += "'";
            return hex;
    }
}

// This function is used to print out values. Don't use it to create sql.
inline string SQLiteValueToString(SQLiteValue &value)
{
    string *ptrstring = boost::get<string>(&value);
    if(ptrstring != NULL)
    {
        return "'" + *ptrstring + "'";
    }

    int *ptrint = boost::get<int>(&value);
    if(ptrint != NULL)
    {
        return numToString<int>(*ptrint);
    }

    float *ptrfloat = boost::get<float>(&value);
    if(ptrfloat != NULL)
    {
        return numToString<float>(*ptrfloat);
    }

    SQLiteSequence *ptrseq =  boost::get<SQLiteSequence>(&value);
    if(ptrseq != NULL)
    {
        string hex = "x'";
        SQLiteSequence seq;
        GetSQLiteValueSequence(value, seq);
        for(int i = 0; i < (int)seq.size(); i ++)
        {
            unsigned char ch = seq[i];
            hex += hexToString((int)ch);
        }
        hex += "'";
        return hex;
    }

    return "";
}

class SQLiteColumn
{
public:
    SQLiteColumn();
    SQLiteColumn(string head, EnumSQLiteColumnType type);
    SQLiteColumn(string head, EnumSQLiteColumnType type, bool notNull, bool noCase, bool autoIncrement, bool primaryKey, bool unique);
    SQLiteColumn(string head, EnumSQLiteColumnType type, bool notNull, bool noCase, bool autoIncrement, bool primaryKey, bool unique, SQLiteValue value);
    void SetHead(string head);
    void SetType(EnumSQLiteColumnType type);
    void SetNotNull(bool notNull);
    void SetNoCase(bool noCase);
    void SetAutoIncrement(bool autoIncrement);
    void SetPrimaryKey(bool primaryKey);
    void SetUnique(bool unique);
    void SetDefaultValue(string defaultValue);
    void SetDefaultValue(float defaultValue);
    void SetDefaultValue(int defaultValue);
    void SetDefaultValue(SQLiteSequence defaultValue);
    string DefaultValueToString();
    
    inline string GetHead() const
    {
        return m_head;
    }
    
    inline EnumSQLiteColumnType GetType() const
    {
        return m_type;
    }
    
    inline bool GetNotNull() const
    {
        return m_notNull;
    }
    
    inline bool GetNoCase() const
    {
        return m_noCase;
    }
    
    inline bool GetAutoIncrement() const
    {
        return m_autoIncrement;
    }
    
    inline bool GetPrimaryKey() const
    {
        return m_primaryKey;
    }

    inline bool GetUnique() const
    {
        return m_unique;
    }
    
    inline bool GetHaveDefaultValue() const
    {
        return m_haveDefaultValue;
    }
    
    inline SQLiteValue GetDefaultValue() const
    {
        return m_defaultValue;
    }
    
    inline void GetDefaultValueString(string &value)
    {
        GetSQLiteValueString(m_defaultValue, value);
    }

    inline int GetDefaultValueInt()
    {
        return GetSQLiteValueInt(m_defaultValue);
    }

    inline float GetDefaultValueFloat()
    {
        return GetSQLiteValueFloat(m_defaultValue);
    }

    inline void GetDefaultValueSequence(SQLiteSequence &seq)
    {
        return GetSQLiteValueSequence(m_defaultValue, seq);
    }
    
protected:
    void Initialize();
    
    string m_head;
    bool m_notNull;
    bool m_noCase;
    bool m_autoIncrement;
    bool m_primaryKey;
    bool m_unique;
    EnumSQLiteColumnType m_type;
    SQLiteValue m_defaultValue;
    bool m_haveDefaultValue;
};

typedef map<string, SQLiteValue> SQLiteValuePair;

class SQLiteIndex
{
public:
    SQLiteIndex(string name, set<string> index, bool unique)
    {
        m_name = name;
        m_index = index;
        m_unique = unique;
    }

    string GetName()
    {
        return m_name;
    }
    
    bool GetUnique()
    {
        return m_unique;
    }
    
    set<string> &GetIndex()
    {
        return m_index;
    }
    
private:
    set<string> m_index;
    string m_name;
    bool m_unique;
};

class SQLiteDBLock
{
public:
    SQLiteDBLock()
    {
        if(pthread_mutex_init(&m_locklock, NULL))
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteDBLock::SQLiteDBLock: mutex init failed.");
            return;
        }
    }

    ~SQLiteDBLock()
    {
        for(map<string, pthread_mutex_t>::iterator it = m_mutexs.begin(); it != m_mutexs.end(); it ++)
        {
            pthread_mutex_t *mutex = &(it->second);
            pthread_mutex_destroy(mutex);
        }
        pthread_mutex_destroy(&m_locklock);
    }

    
    pthread_mutex_t *DeclareMutex(string dbName)
    {
        CommonThreadLock thread_lock(&m_locklock);
        if(m_mutexs.count(dbName) > 0)
        {
            ;
        }
        else
        {
            m_mutexs.insert(map<string, pthread_mutex_t>::value_type(dbName, pthread_mutex_t()));
        }
        pthread_mutex_t *mutex = &(m_mutexs[dbName]);
        if(pthread_mutex_init(mutex, NULL))
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "SQLiteDBLock::DeclareMutex: %s mutex init failed.", dbName.c_str());
            return NULL;
        }
        
        return mutex;
    }

private:
    map<string, pthread_mutex_t> m_mutexs;
    pthread_mutex_t m_locklock;
};

class SQLiteTable
{
public:
    SQLiteTable();
    ~SQLiteTable();
    SQLiteTable(string dbName, string tableName, unsigned int maxRows);
    void SetDbName(string dbName);
    void SetTableName(string tableName);
    void SetMaxRows(unsigned int maxRows);
    void SetCreateIfNotExist(bool createIfNotExist);
    void SetOrderTitle(string orderTitle);
    int AddColumn(SQLiteColumn column);
    int Open();
    int Close();
    int Add(SQLiteValuePair &values);
    int AddBatch(set<SQLiteValuePair> &values);
    int AddIndex(string name, set<string> &index, bool unique);
    int Query(SQLiteValuePair &constrain, set<string> &results, set<SQLiteValuePair> &returnResults);
    int Query(SQLiteValuePair &constrain, set<string> &results, vector<SQLiteValuePair> &returnResults, const string &orderTitle, bool asc);
    int QueryNearest(string orderTitle, SQLiteValuePair &constrain, set<string> &results, SQLiteValuePair &returnResults, bool top);
    int QueryLatest(set<string> &results, SQLiteValuePair &returnResults);
    int QueryTopBottom(const string &orderTitle, set<string> &results, SQLiteValuePair &returnResults, bool top);
    int QueryTopBottomMultiple(const string &orderTitle, set<string> &results, vector<SQLiteValuePair> &returnResults, int count, bool top);
    int QueryNext(const string &orderTitle, set<string> &results, SQLiteValuePair &returnResults, SQLiteValue &currentValue);
    int QueryBetween(const string &orderTitle, int min, int max, set<string> &results, vector<SQLiteValuePair> &returnResults, bool asc);
    int Delete(SQLiteValuePair &constrain);
    int Modify(SQLiteValuePair &constrain, SQLiteValuePair &values);
    int GetRowCount();
    
protected:
    void Initialize();
    int Create();
    int RemoveLatest();
    int InsertDefaultRow();
    void GetFileLock();
    
    string m_dbName;
    string m_tableName;
    vector<SQLiteColumn> m_columns;
    map<string, SQLiteColumn> m_columnsMap;
    map<string, SQLiteIndex> m_indexs;
    unsigned int m_maxRows;
    bool m_createIfNotExist;
    string m_orderTitle;
    sqlite3 *m_db;
    pthread_mutex_t *m_mutex;
    string m_file_lock;
    static SQLiteDBLock m_dbLock;
};

#endif
