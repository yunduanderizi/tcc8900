/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： KeyWordsMgr.cpp
* 文件标识： (见配置管理计划书)
* 内容摘要： 
* 其它说明： 
* 当前版本： V1.0
* 作    者： 于震洋
* 完成日期： [2011/11/14]
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容： 
*<FH+>***************************************************************************
*/

#include "KeyWordsMgr.h"
#include "common_config.h"
#include "qisr.h"

KeyWordsMgrC *KeyWordsMgrC::thisObject = NULL;


/**
*<FUNC+>********************************************************************
* 函数名称： KeyWordsMgrC
* 功能描述： 构造函数
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
KeyWordsMgrC::KeyWordsMgrC()
{
}

/**
*<FUNC+>********************************************************************
* 函数名称： KeyWordsMgrC
* 功能描述： 析构函数
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
KeyWordsMgrC::~KeyWordsMgrC()
{
}

/**
*<FUNC+>********************************************************************
* 函数名称： object
* 功能描述： 构造对象
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 返回对象指针
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
KeyWordsMgrC* KeyWordsMgrC::object()
{
    KeyWordsMgrC *pTmp = NULL;

    if(thisObject == NULL)
    {
        thisObject = new KeyWordsMgrC;
    }

    pTmp = thisObject;

    return pTmp;
}

/**
*<FUNC+>********************************************************************
* 函数名称： releaseObject
* 功能描述： 释放对象
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void KeyWordsMgrC::releaseObject()
{
    if (thisObject != NULL)
    {
        delete thisObject;
        thisObject = NULL;
    }
}

/**
*<FUNC+>********************************************************************
* 函数名称： insertKeyWord
* 功能描述： 插入关键词到现有的数据库中，没有就创建数据库表
* 输入参数： keyWordList：关键词列表
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::insertKeyWord(KeyWordList keyWordList)
{
    if (keyWordList.empty())
    {
        printf("insertKeyWord, keyWordList is empty\n");
        return -1;
    }

    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;

    // 检查数据库中是否有表
    if (!_keyWordExist())
    {
        // 如果没有，先创建数据表，再增加数据
        iRet = _createDatabase();
        if (iRet != 0)
        {
            printf("insertKeyWord, _createDatabase failed %d\n", iRet);
            return iRet;
        }
    }

    // 将数据存进数据库中
    // 打开数据库
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);
    if (iRet != SQLITE_OK)
    {
        printf("insertKeyWord, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return -1;
    }

    KeyWordList::const_iterator keyWordIter = keyWordList.begin();
    for (; keyWordIter != keyWordList.end(); ++keyWordIter)
    {
        char strSqlCmd[2048];
        memset(strSqlCmd, 0, sizeof(strSqlCmd));

        // 拼接SQL语句
        sprintf(strSqlCmd, "insert into KeywordTable values('%s')", keyWordIter->toUtf8().data());
        printf("插入数据SQL语句%s\n", strSqlCmd);

        sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);
    }

    // 查找keywordstatus表是否有数据，如果有数据修改现有的，没有则插入一条数据
    char *strSearchStatusCmd = (char *)"select count(*) from KeywordStatusTable";
    int iRowNum = 0;

    iRet = sqlite3_prepare(pDb, strSearchStatusCmd, strlen(strSearchStatusCmd), &pStmt, (const char**)&strError);
    if (iRet != SQLITE_OK)
    {
        printf("insertKeyWord, sqlite3_prepare failed %d\n", iRet);
        sqlite3_close(pDb);

        return iRet;
    }

    while (sqlite3_step(pStmt) == SQLITE_ROW)
    {
        iRowNum = sqlite3_column_int(pStmt,0);
        sqlite3_finalize(pStmt);
    }

    if (iRowNum > 0)
    {
        // 有数据，修改里面Upload值
        char *strSqlCmd = (char *)"update KeywordStatusTable SET StatusValue='false' where StatusName='upload'";
        sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);
    }
    else
    {
        // 没有数据，插入数据
        char *strSqlCmd =(char *)"insert into KeywordStatusTable values('upload','false')";
        sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);

        strSqlCmd = (char *)"insert into KeywordStatusTable values('id','')";
        sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);

    // 上传到服务器中
    _uploadKeyword();
    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： modifyKeyWord
* 功能描述： 修改关键词
* 输入参数： strSourceKeyWord：原本的关键词，strObjKeyWord：要修改的关键词
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::modifyKeyWord(const QString strSourceKeyWord, 
                                const QString strObjKeyWord)
{
    return 0;
}

/**
*<FUNC+>********************************************************************
* 函数名称： getKeyWord
* 功能描述： 获取关键词
* 输入参数： 无
* 输出参数： keywordList：关键词列表
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::getKeyWord(KeyWordList &keywordList)
{
    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;
    keywordList.clear();

    // 打开数据库
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);
    if (iRet != SQLITE_OK)
    {
        printf("getKeyWord, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    char *strSqlCmd = (char *)"select * from KeywordTable";
    iRet = sqlite3_prepare(pDb, strSqlCmd, strlen(strSqlCmd), &pStmt, (const char**)&strError);
    if (iRet != SQLITE_OK)
    {
        printf("getKeyWord, sqlite3_prepare failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    // 获取数据
    while (1)
    {
        iRet = sqlite3_step(pStmt);

        if (iRet != SQLITE_ROW)
        {
            break;
        }

        const unsigned char *strKeyword = sqlite3_column_text(pStmt, 0);

        printf("Get from database %s\n", strKeyword);

        keywordList.append(QString::fromUtf8((const char*)strKeyword));
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： getKeyWordId
* 功能描述： 获取关键词ID
* 输入参数： 无
* 输出参数： strKeywordId：关键词ID
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/22]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::getKeyWordId(QString &strKeywordId)
{
    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;
    strKeywordId.clear();

    // 打开数据库
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);
    if (iRet != SQLITE_OK)
    {
        printf("getKeyWord, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    // 先获取状态，如果状态为true，则把ID反回去
    char *strSqlCmd = (char *)"select StatusValue from KeywordStatusTable where StatusName='upload'";

    // 下发SQL语句
    iRet = sqlite3_prepare(pDb, strSqlCmd, strlen(strSqlCmd), &pStmt, (const char**)&strError);
    if (iRet != SQLITE_OK)
    {
        printf("getKeyWord, sqlite3_prepare failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    // 获取数据
    iRet = sqlite3_step(pStmt);
    const unsigned char *strStatus = NULL;
    strStatus = sqlite3_column_text(pStmt, 0);

    printf("获取upload结果%s %d\n", strStatus, iRet);

    // 如果没有结果，直接返回
    if (iRet != SQLITE_ROW && iRet != SQLITE_DONE)
    {
        printf("getKeyWord, sqlite3_step failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_finalize(pStmt);
        sqlite3_close(pDb);
        return iRet;
    }

    // 只有为true才会返回ID，否则不返回
    if (strcmp((const char*)strStatus, "true") == 0)
    {
        sqlite3_finalize(pStmt);

        strSqlCmd = (char *)"select StatusValue from KeywordStatusTable where StatusName='id'";

        // 下发SQL语句
        iRet = sqlite3_prepare(pDb, strSqlCmd, strlen(strSqlCmd), &pStmt, (const char**)&strError);
        if (iRet != SQLITE_OK)
        {
            printf("getKeyWord, sqlite3_prepare failed %s\n", sqlite3_errmsg(pDb));
            sqlite3_close(pDb);
            return iRet;
        }

        // 获取ID并赋给出参
        iRet = sqlite3_step(pStmt);
        const unsigned char *strTempKeywordId = sqlite3_column_text(pStmt, 0);
        strKeywordId = (const char*)strTempKeywordId;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);

    return iRet;
}
/**
*<FUNC+>********************************************************************
* 函数名称： insertKeyWordId
* 功能描述： 存储关键词ID
* 输入参数： strKeyWordId：关键词ID
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::insertKeyWordId(const char *strKeyWordId)
{
    if (strKeyWordId == NULL)
    {
        printf("insertKeyWordId, strKeyWordId is null\n");
        return -1;
    }

    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;
    char strSqlCmd[2048] = {0};

    sprintf(strSqlCmd, "update KeywordStatusTable SET StatusValue='%s' where StatusName='id'", strKeyWordId);
    printf("插入关键词ID，SQL=%s\n", strSqlCmd);

    // 打开数据库
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);

    if (iRet != SQLITE_OK)
    {
        printf("insertKeyWordId, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    // 下发SQL语句
    iRet = sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);
    printf("存储关键词ID结束%d\n", iRet);

    // 修改状态
    memset(strSqlCmd, 0, sizeof(strSqlCmd));
    strcpy(strSqlCmd, "update KeywordStatusTable SET StatusValue='true' where StatusName='upload'");
    printf("修改状态，SQL=%s\n", strSqlCmd);

    iRet = sqlite3_exec(pDb, strSqlCmd, 0, 0, &strError);
    printf("修改状态完成%d\n", iRet);

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： _keyWordExist
* 功能描述： 检查关键词数据库是否存在
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 存在返回true，否则返回false
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
bool KeyWordsMgrC::_keyWordExist()
{
    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;
    char *strSelectTable = (char*)"SELECT name FROM sqlite_master";

    // 打开数据库
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);
    if (iRet != SQLITE_OK)
    {
        printf("_keyWordExist, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return false;
    }

    // 下发SQL语句
    iRet = sqlite3_prepare(pDb, strSelectTable, strlen(strSelectTable), &pStmt, (const char**)&strError);
    if (iRet != SQLITE_OK)
    {
        printf("_keyWordExist, sqlite3_prepare failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return false;
    }

    // 获取数据
    while (1)
    {
        iRet = sqlite3_step(pStmt);

        if (iRet != SQLITE_ROW)
        {
            break;
        }

        const unsigned char *strTableName = sqlite3_column_text(pStmt, 0);
        printf("Table name is %s\n", strTableName);

        if (strcmp("KeywordStatusTable", (const char*)strTableName) == 0
            || strcmp("KeywordTable", (const char*)strTableName) == 0)
        {
            sqlite3_finalize(pStmt);
            sqlite3_close(pDb);
            return true;
        }
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);

    return false;
}

/**
*<FUNC+>********************************************************************
* 函数名称： _createDatabase
* 功能描述： 创建数据库表
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/14]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::_createDatabase()
{
    printf("创建表开始\n");
    int iRet = 0;
    sqlite3 *pDb = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *strError = NULL;
    char *strCreateTable = (char*)"create table KeywordTable(Keyword text primary key)";

    // 打开数据库
    iRet = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDb);
    if (iRet != SQLITE_OK)
    {
        printf("_createDatabase, openDB failed %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return iRet;
    }

    // 下发SQL语句
    sqlite3_exec(pDb, strCreateTable, 0, 0, &strError);

    // 创建keyWordStatusTable表
    strCreateTable = (char *)"create table KeywordStatusTable(StatusName text, StatusValue text)";
    sqlite3_exec(pDb, strCreateTable, 0, 0, &strError);

    sqlite3_close(pDb);

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： _uploadKeyword
* 功能描述： 上传关键词到服务器中
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/22]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int KeyWordsMgrC::_uploadKeyword()
{
    int iRet = 0;
    KeyWordList keywordList;
    QString strKeyword;
    const char *strSessionId = NULL;

    // 先获取数据库中的关键词
    iRet = getKeyWord(keywordList);

    if (keywordList.empty())
    {
        printf("_uploadKeyword, keywordList is empty\n");
        return -1;
    }

    // 先拼接字符串为如下格式“词1,词2,词3”
    KeyWordList::iterator keywordIter = keywordList.begin();
    for (; keywordIter != keywordList.end(); ++keywordIter)
    {
        strKeyword.append(QString::fromUtf8(keywordIter->toUtf8().data()));
        strKeyword.append(",");
    }

    // 去掉最后一个逗号
    strKeyword.remove(strKeyword.length() - 1, 1);

    // 开始上传词库 初始化
    iRet = QISRInit("");
    if(iRet != 0)
    {
        printf("QISRInit with errorCode: %d \n", iRet);
        return iRet;
    }

    // 开启会话
    strSessionId = QISRSessionBegin(NULL, "ssm=1,sub=asr", &iRet);
    if (iRet != 0)
    {
        printf("QISRSessionBegin with errorCode: %d \n", iRet);

        QISRFini();
        return iRet;
    }

    // 上传到服务器中
    printf("upload keyword are %s\n", strKeyword.toUtf8().data());
    const char *strUploadId = QISRUploadData(strSessionId, "contact", 
        strKeyword.toUtf8().data(), strKeyword.length(), "dtt=keylist", &iRet);

    printf("upload id is %s\n", strUploadId);
    if (iRet != 0)
    {
        printf("QISRUploadData with errorCode: %d \n", iRet);

        QISRSessionEnd(strSessionId, "normal");
        QISRFini();

        return iRet;
    }

    // 上传成功，存在数据库中
    insertKeyWordId(strUploadId);

    QISRSessionEnd(strSessionId, "normal");
    QISRFini();

    return iRet;
}
