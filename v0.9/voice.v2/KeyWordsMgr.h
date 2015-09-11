/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： KeyWordsMgr.h
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

#ifndef KEYWORDSMGR_H
#define KEYWORDSMGR_H

#include <sqlite3.h>
#include <QList>
#include <QString>
#include "voiceDefine.h"

/**
*<CLASS+>********************************************************************
* 类 名 称: KeyWordsMgrC
* 功能描述: 关键词管理
* 其它说明: 
* 修改记录: 
* 修改日期			版 本		修改人		修改摘要
* -------------------------------------------------------------------------
* [2011/11/14]	    V1.0		于震洋		创建类
*<CLASS->*********************************************************************
*/
class KeyWordsMgrC
{
public:
    static KeyWordsMgrC *object();
    static void releaseObject();

    int insertKeyWord(KeyWordList keyWordList);
    int modifyKeyWord(const QString strSourceKeyWord, const QString strObjKeyWord);
    int getKeyWord(KeyWordList &keywordList);
    int getKeyWordId(QString &strKeywordId);
    int insertKeyWordId(const char *strKeyWordId);

private:
    KeyWordsMgrC();
    virtual ~KeyWordsMgrC();

    static KeyWordsMgrC *thisObject;

    bool _keyWordExist();
    int _createDatabase();
    int _uploadKeyword();
};

#endif