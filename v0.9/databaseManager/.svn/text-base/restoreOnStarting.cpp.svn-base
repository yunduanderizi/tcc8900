#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SQLite.h"
#include "debugMonitor.h"
#include "common_config.h"

#define DB_PATH "/nand3/userdata/"
#define DB_SERV_NAME "Service.db"

int Sql_ReadFunctionLast(QString& qFunctionName)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pQueryTablesSel = (char*)"select * from FunctionLast";

	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s\n", sqlite3_errmsg(pDB));
		return -1;
	}

	ret = sqlite3_prepare(pDB, pQueryTablesSel, strlen(pQueryTablesSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table content error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_ROW)
	{
		//FunctionLast is empty
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);

		qFunctionName = "";
		return 0;
	}
	
	qFunctionName = (char*)sqlite3_column_text(pStmt, 1);
	
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	
	return 0;
}

int Sql_SetFunctionLast(const char* pFunctionName)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = "delete from FunctionLast";
	char pInsertTablesSel[128] = {0};
	
	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s\n", sqlite3_errmsg(pDB));
		return -1;
	}	
	
	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_step error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	sqlite3_finalize(pStmt);
	
	sprintf(pInsertTablesSel, "insert into FunctionLast values('%s')", pFunctionName);

	ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_step error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	
	return 0;	
}

