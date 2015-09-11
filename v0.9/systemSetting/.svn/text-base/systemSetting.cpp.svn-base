#include "systemSetting.h"
#include <QMutex>
#include "sqlite3.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "common_config.h"

#ifndef FILE_NAME
#define FILE_NAME  INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY	
#endif

static QMutex gs_mutexForThreadSafe;

SystemSettingC  *SystemSettingC::thisObject = NULL;
SystemSettingC  *SystemSettingC::object()
{
	SystemSettingC *pTmp = NULL;
		
	gs_mutexForThreadSafe.lock();
	
	if( !thisObject )
	{
		thisObject = new SystemSettingC();
	}

	pTmp = thisObject;

	gs_mutexForThreadSafe.unlock();
	
	return pTmp;
}

void SystemSettingC::releaseObject()
{
	gs_mutexForThreadSafe.lock();

	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	
	gs_mutexForThreadSafe.unlock();
}

SystemSettingC::SystemSettingC()
{

}

SystemSettingC ::~SystemSettingC()
{
	
}

int SystemSettingC::setNewCallRing(const QString ringNameWithPath)
{
	int retval = SYSSETTING_E_OK;

	gs_mutexForThreadSafe.lock();

	//todo

	int fd = 0;
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = "delete from CallRing";
	char pInsertTablesSel[128] = {0};

	fprintf(stdout, "in setNewCallRing func\n");

	//if (strcmp(ringNameWithPath, "") == 0)
	if (ringNameWithPath.isEmpty() == true)
	{
		printf("in strcmp func\n");
		gs_mutexForThreadSafe.unlock();
		return SYSSETTING_E_INVALID_FILE; 
	}

	if ((fd = open((char *)ringNameWithPath.data(), O_RDONLY)) == -1)
	{
		printf("open music file error\n");
		gs_mutexForThreadSafe.unlock();
		return SYSSETTING_E_INVALID_FILE; 
	}
	close(fd);

	ret = sqlite3_open(FILE_NAME, &pDB);
	if (ret != SQLITE_OK)
	{
		printf("open Service.db error\n");
		gs_mutexForThreadSafe.unlock();
//		IBD_PRINTF(DEBUG_INFOMATION, "%s :can't open database : %s!\n", FILE_NAME, sqlite3_errmsg(pDB))	
		return SYSSETTING_E_SAVE_DATA_ERROR;
	}

	//准备执行语句
	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		printf("sqlite3_prepare error\n");
		gs_mutexForThreadSafe.unlock();
//		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		return SYSSETTING_E_SAVE_DATA_ERROR;
	}

	//执行记录集
	while (1)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}

	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);

	//插入记录条
	sprintf(pInsertTablesSel, "insert into CallRing values('%s')", ringNameWithPath.toAscii().data());

	//准备插入数据
	ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
       if (ret != SQLITE_OK)
	{
		printf("insert data to Service.db error\n");
		gs_mutexForThreadSafe.unlock();
	//	IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))

		return SYSSETTING_E_SAVE_DATA_ERROR;
	}

	//执行绑定以后的语句
	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
	//	IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
	gs_mutexForThreadSafe.unlock();

		return SYSSETTING_E_SAVE_DATA_ERROR;
	}

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	printf("\n---------- save music file ok! \n");	
	gs_mutexForThreadSafe.unlock();
	return SYSSETTING_E_OK;

//	return retval;
}

int SystemSettingC::getNewCallRing(QString& ringNameWithPath)
{
	int retval = SYSSETTING_E_OK;

	gs_mutexForThreadSafe.lock();

	//todo
	char musicFilePath[64] = {0};
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pQueryTablesSel = (char*)"SELECT * FROM CallRing";//查表语句
	
//	printf("in PhoneAudioControlC::playIncomingCallAudio func \n");
//	printf("database file path : [%s]\n", FILE_NAME);
	retval = sqlite3_open(FILE_NAME, &pDB);
	if (retval != SQLITE_OK)
	{
		printf("xtx get file path: open database error\n");
//		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s!\n", sqlite3_errmsg(pDB))
		sqlite3_close(pDB);
		gs_mutexForThreadSafe.unlock();
		return SYSSETTING_E_GET_DATA_ERROR;
	}

	retval = sqlite3_prepare(pDB, pQueryTablesSel, strlen(pQueryTablesSel), &pStmt, (const char**)&pError);
	if (retval != SQLITE_OK)
	{
		printf("xtx get file path: sqlite3_prepare error\n");
		sqlite3_close(pDB);
		gs_mutexForThreadSafe.unlock();
		return SYSSETTING_E_GET_DATA_ERROR;
	}

	retval = sqlite3_step(pStmt);
	if (retval != SQLITE_ROW)
	{
		//AuthenticationInfo is empty, because we first use AuthenticationInfo
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);
		gs_mutexForThreadSafe.unlock();
		return SYSSETTING_E_GET_DATA_ERROR;
	}
	strcpy(musicFilePath, (char*)sqlite3_column_text(pStmt, 0));

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);	
	ringNameWithPath = QString(musicFilePath);	
	
	gs_mutexForThreadSafe.unlock();

	return retval;
}

