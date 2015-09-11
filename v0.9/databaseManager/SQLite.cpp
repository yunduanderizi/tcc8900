#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SQLite.h"
#include "debugMonitor.h"

static int g_nPoiMsgCount = 0;		//信箱数据条数
static int g_nTeleBookCount = 0;     //通讯录数据条数
static int g_nTeleHistoryCount =0 ; //通话记录中未接电话数据条数
static int g_nTeleCalledCount=0;//通话记录中已接电话数据条数
static int g_nTeleDailedCount=0;//通话记录中已拨电话数据条数
static int g_nSendMsgBoxCount = 0; // 发件箱数据条数
static int g_nTeleBookNameCount = 0;//通讯录中同人名个数
static int g_nLatestPoiMsgCount = 0;
int PoiMsg_callback(void *, int , char **argv, char **){
	g_nPoiMsgCount = atoi(argv[0]);	
	return 0;
}

int Sql_ReadPoiMsg(SmsInfo **pPoiMsg, int *nCount)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pSqlSel = (char*)"select PoiMsg.*, TelephoneBook.Name,TelephoneBook.Id from PoiMsg left join TelephoneBook  on TelephoneBook.PhoneNum = PoiMsg.FromPhone or TelephoneBook.TelephoneNum = PoiMsg.FromPhone order by Time desc limit 100";
	char *pSqlQueryCount = (char*)"select count(*) from PoiMsg";

	int i = 0;

	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//统计数据条数
	ret = sqlite3_exec(pDB, pSqlQueryCount, PoiMsg_callback, 0, &pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//分配使用的内存
	if (g_nPoiMsgCount <= 0)
		{
		sqlite3_close(pDB);
		return -1;
		}

		int count = -1;
	
	if(g_nPoiMsgCount  > 100)
	{
		*nCount = 100 ;
		count = 100;
	}
	else 
	{
		*nCount = g_nPoiMsgCount;
		count =g_nPoiMsgCount ;
	}
	
	*pPoiMsg = (SmsInfo*)malloc(count * sizeof(SmsInfo));
	if (*pPoiMsg == NULL)
	{
		sqlite3_close(pDB);
		return -1;
	}
	memset(*pPoiMsg, 0, count * sizeof(SmsInfo));
	
	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//读取全部数据
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
			break;
		
		((*pPoiMsg)[i]).nId = sqlite3_column_int(pStmt, 0);
		
		char *pNum = (char*)sqlite3_column_text(pStmt, 1);
		if (pNum != NULL)
		{
			if (strlen(pNum)>0)
				strcpy(((*pPoiMsg)[i]).chFromPhone, pNum);
		}
		((*pPoiMsg)[i]).byStatus = sqlite3_column_int(pStmt, 2);
		char *pTime =  (char*)sqlite3_column_text(pStmt, 3);
		if (pTime != NULL)
		{
			if (strlen(pTime)>0)
				strcpy(((*pPoiMsg)[i]).chTime, pTime);
		}
		char *pPoiName =  (char*)sqlite3_column_text(pStmt, 4);
		if (pPoiName != NULL)
		{
			if (strlen(pPoiName)>0)
				strcpy(((*pPoiMsg)[i]).chPoiName, pPoiName);
		}		
		char *pDesc = (char*)sqlite3_column_blob(pStmt, 5);
		if (pDesc != NULL)
		{
			if (strlen(pDesc) > 0)
				strcpy(((*pPoiMsg)[i]).chContent, pDesc);
		}
		((*pPoiMsg)[i]).dLon = sqlite3_column_double(pStmt, 6);
		((*pPoiMsg)[i]).dLat = sqlite3_column_double(pStmt, 7);

		char *phoneName=(char*)sqlite3_column_text(pStmt, 9);

		if (phoneName != NULL)
		{
			if (strlen(phoneName)>0)
				strcpy(((*pPoiMsg)[i]).name, (char*)sqlite3_column_text(pStmt, 9));
		}

		((*pPoiMsg)[i]).bookId = sqlite3_column_int(pStmt, 10);

 		i++;
	}
 
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 1;
}

//函数名：Sql_ReleasePoiMsg
//功    能：释放信箱数据库数据
//入    参：无
//出	   参：pPoiMsg 数据集，nCount 数据条数
//返回值：>0成功，-1失败
int Sql_ReleasePoiMsg(SmsInfo **pPoiMsg, int *nCount)
{
	if( nCount > 0 && *pPoiMsg != NULL )
		free (*pPoiMsg);
	*pPoiMsg = NULL;
	nCount =0;

	return 0;
}

int PoiMsgLatest_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	g_nLatestPoiMsgCount= atoi(argv[0]);	
	return 0;
}

int Sql_ReadLatestPoiMsg(SmsInfo *pPoiMsg)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pSqlSel = (char*)"select PoiMsg.*, TelephoneBook.Name,TelephoneBook.Id from PoiMsg left join TelephoneBook  on TelephoneBook.PhoneNum = PoiMsg.FromPhone or TelephoneBook.TelephoneNum = PoiMsg.FromPhone order by Time desc limit 1";

	int i = 0;

	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END
		sqlite3_close(pDB);
		return -1;
	}
	 	
	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//读取全部数据
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
			break;
		
		 pPoiMsg->nId = sqlite3_column_int(pStmt, 0);
		
		char *pNum = (char*)sqlite3_column_text(pStmt, 1);
		if (pNum != NULL)
		{
			if (strlen(pNum)>0)
				strcpy( pPoiMsg->chFromPhone, pNum);
		}
		 pPoiMsg->byStatus = sqlite3_column_int(pStmt, 2);
		char *pTime =  (char*)sqlite3_column_text(pStmt, 3);
		if (pTime != NULL)
		{
			if (strlen(pTime)>0)
				strcpy( pPoiMsg->chTime, pTime);
		}
		char *pPoiName =  (char*)sqlite3_column_text(pStmt, 4);
		if (pPoiName != NULL)
		{
			if (strlen(pPoiName)>0)
				strcpy(pPoiMsg->chPoiName, pPoiName);
		}		
		char *pDesc = (char*)sqlite3_column_blob(pStmt, 5);
		if (pDesc != NULL)
		{
			if (strlen(pDesc) > 0)
				strcpy( pPoiMsg->chContent, pDesc);
		}
		pPoiMsg->dLon = sqlite3_column_double(pStmt, 6);
		pPoiMsg->dLat = sqlite3_column_double(pStmt, 7);

		char *phoneName=(char*)sqlite3_column_text(pStmt, 9);

		if (phoneName != NULL)
		{
			if (strlen(phoneName)>0)
				strcpy(pPoiMsg->name, phoneName);
		}

		else
		{
			memset(pPoiMsg->name, 0, sizeof(pPoiMsg->name));
		}
		pPoiMsg->bookId = sqlite3_column_int(pStmt, 10);

 		i++;
	}
 
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 1;
}


int Sql_ReadLatestSendMsg(sendInfo *pPoiMsg)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pSqlSel = (char*)"select SendMsg.*, TelephoneBook.Name,TelephoneBook.Id from SendMsg left join TelephoneBook  on TelephoneBook.PhoneNum = SendMsg.ToPhoneNumber or TelephoneBook.TelephoneNum = SendMsg.ToPhoneNumber order by Time desc limit 1";

	int i = 0;

	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END
		sqlite3_close(pDB);
		return -1;
	}
	 	
	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//读取全部数据
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
			break;
		
		 pPoiMsg->nId = sqlite3_column_int(pStmt, 0);

		
		char *pNum = (char*)sqlite3_column_text(pStmt, 1);
		if (pNum != NULL)
		{
			if (strlen(pNum)>0)
				strcpy( pPoiMsg->chToPhoneNumber, pNum);
		}
		char *pTime =  (char*)sqlite3_column_text(pStmt, 2);
		if (pTime != NULL)
		{
			if (strlen(pTime)>0)
				strcpy( pPoiMsg->chToTime, pTime);
		}
		char *pDesc = (char*)sqlite3_column_blob(pStmt, 3);
		if (pDesc != NULL)
		{
			if (strlen(pDesc) > 0)
				strcpy( pPoiMsg->chToContent, pDesc);
		}

		char *phoneName=(char*)sqlite3_column_text(pStmt, 4);

		if (phoneName != NULL)
		{
			if (strlen(phoneName)>0)
				strcpy(pPoiMsg->name, phoneName);
		}

		else
		{
			memset(pPoiMsg->name, 0, sizeof(pPoiMsg->name));
		}
		pPoiMsg->bookId = sqlite3_column_int(pStmt, 5);

 		i++;
	}
 
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 1;

}

//读取当前最新的电话记录的id
int Sql_ReadLatestTelephoneHistoryId(int *id)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pSqlSel = (char*)"select TelephoneHistory.Id from TelephoneHistory order by Id desc limit 1";

	int i = 0;

	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END
		sqlite3_close(pDB);
		return -1;
	}
	 	
	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//读取全部数据
	ret = sqlite3_step(pStmt);		//移动记录集
	if (ret != SQLITE_ROW)
	{
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);   
		return -1;
	}
		
	*id = sqlite3_column_int(pStmt, 0);
 
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 0;

}

//读取当前最新的通讯录的id
int Sql_ReadLatestTelephoneBookId(int *id)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pSqlSel = (char*)"select TelephoneBook.Id from TelephoneBook order by Id desc limit 1";

	int i = 0;

	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END
		sqlite3_close(pDB);
		return -1;
	}
	 	
	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//读取全部数据
	ret = sqlite3_step(pStmt);		//移动记录集
	if (ret != SQLITE_ROW)
	{
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB); 		
		return -1;
	}
		
	*id = sqlite3_column_int(pStmt, 0);
 
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 0;

}
//读取当前最新的发件箱的id
int  Sql_ReadLatestSendMsgId(int *id)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pSqlSel = (char*)"select SendMsg.Id from SendMsg order by Id desc limit 1";

	int i = 0;

	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END
		sqlite3_close(pDB);
		return -1;
	}
	 	
	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//读取全部数据
	ret = sqlite3_step(pStmt);		//移动记录集
	if (ret != SQLITE_ROW)
	{
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB); 		
		return -1;
	}
		
	*id = sqlite3_column_int(pStmt, 0);
 
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 0;


}


//读取通讯录接口函数
int TelephoneBook_callback(void *, int , char **argv, char **)
{
	//printf("TelephoneBook_callback start --->int =%d argv = %s azColName = %s \n", argc, argv[0], azColName[0]);
	g_nTeleBookCount = atoi(argv[0]);
	//printf("TelephoneBook_callback over --->nCount = %d \n", g_nTeleBookCount);
	return 0;
}

//函数名：Sql_ReadTelephoneBook
//功    能：读取通讯录数据库
//入    参：无
//出	   参：pTeleBook数据集，nCount 数据条数
//返回值：>0成功，-1失败
int Sql_ReadTelephoneBook(teleBookInfo **pTeleBook, int *nCount)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pSqlSel = (char*)"select * from TelephoneBook limit 100";
	char *pSqlQueryCount = (char*)"select count(*) from TelephoneBook";
	int i = 0;
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	//printf("before open database: %s\n", chSqlDBPath);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		//fprintf(stderr, "can't open database: %s\n", sqlite3_errmsg(pDB));
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//统计数据条数
	ret = sqlite3_exec(pDB, pSqlQueryCount, TelephoneBook_callback, 0, &pError);
	if (ret != SQLITE_OK)
	{
		//printf("query count error: %s", pError);

		sqlite3_close(pDB);
		return -1;
	}
	//分配使用的内存
	if (g_nTeleBookCount <= 0)
	{
		sqlite3_close(pDB);
		return -1;
	}

	int count = -1;
	/*
	if(g_nTeleBookCount  > 100)
	{
		*nCount = 100 ;
		count = 100;
	}
	else 
	{
	*/
		*nCount = g_nTeleBookCount;
		count =g_nTeleBookCount ;
	//}

	//printf("pPoiMsg malloc start ---> p = %x, g_nPoiMsgCount = %d \n", pDB, g_nPoiMsgCount);
	
	//demo处理 初始化
	*pTeleBook = (teleBookInfo*)malloc(count * sizeof(teleBookInfo));
	if (*pTeleBook == NULL)
	{
		//printf("pPoiMsg malloc error");
		sqlite3_close(pDB);
		return -1;
	}
	memset(*pTeleBook, 0, count * sizeof(teleBookInfo));
	//printf("pPoiMsg malloc over \n");

	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		//printf("sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
	}
	//读取全部数据
	printf("db read start \n");
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
			break;

		((*pTeleBook)[i]).nId = sqlite3_column_int(pStmt, 0);
		strcpy(((*pTeleBook)[i]).friendName, (char*)sqlite3_column_blob(pStmt, 1));
		strcpy(((*pTeleBook)[i]).friendPhone, (char*)sqlite3_column_text(pStmt, 2));
		strcpy(((*pTeleBook)[i]).friendEmail, (char*)sqlite3_column_text(pStmt, 3));
		strcpy(((*pTeleBook)[i]).friendCompany, (char*)sqlite3_column_blob(pStmt, 4));
		strcpy(((*pTeleBook)[i]).friendComAddress, (char*)sqlite3_column_blob(pStmt, 5));
		strcpy(((*pTeleBook)[i]).friendFamAddress, (char*)sqlite3_column_blob(pStmt, 6));
		strcpy(((*pTeleBook)[i]).friendPhoto, (char*)sqlite3_column_text(pStmt, 7));
		strcpy(((*pTeleBook)[i]).friendTelephoneNum, (char*)sqlite3_column_text(pStmt, 8));
		strcpy(((*pTeleBook)[i]).friendSound, (char*)sqlite3_column_text(pStmt, 9));
		((*pTeleBook)[i]).friendVideo = sqlite3_column_int(pStmt, 10);
		//fprintf(stderr, "#################: %s\n", ((*pTeleBook)[i]).friendTelephoneNum);
		i++;
	}
//	printf("---TelephoneBook---db read over \n");

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 1;
}

//函数名：Sql_ReleaseTelephoneBook
//功    能：释放通讯录数据库
//入    参：无
//出	   参：pTeleBook数据集，nCount 数据条数
//返回值：>0成功，-1失败
int Sql_ReleaseTelephoneBook(teleBookInfo **pTeleBook, int *nCount)
{
	if( nCount > 0 && *pTeleBook != NULL )
		free (*pTeleBook);
	*pTeleBook = NULL;
	g_nTeleBookCount =0;

	return 0;
}

//删除表中所有数据
int  Sql_DeleteAllMsg()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128]  = {0};
	char chSqlDBPath[128] = {0};

	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
//		fprintf(stderr, "cjtserv------can't open database: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	//声称删除SQL语句
	sprintf(chSqlCmd, "delete from PoiMsg");

	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
//		printf("cjtserv --- sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
	}

	//执行记录集
	while (true)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}

	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
}



//更新短信状态
int Sql_UpdateMsg(int msgId)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	//sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[256] = {0};
	char chSqlDBPath[128] = {0};

	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
//		fprintf(stderr, "can't open database: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

//	printf("cjtserv------PoiMsg--open db success! \n");

	//组织声称更新数据sql语句
	sprintf(chSqlCmd, "update PoiMsg set Status = '%d'  where ID = '%d'",
				1,msgId);

//"update %s set Longitude = '%.8f', Latitude = '%.8f', Direct = '%d', Speed = '%d', Time = '%s' where ID = '1'"
	//ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	ret = sqlite3_exec(pDB, chSqlCmd, 0, 0, &pError);
	if (ret != SQLITE_OK)
	{
//		printf("sqlite3 exec error \n");
		sqlite3_close(pDB);
		return -1;
	}

//	printf("cjtserv------db update success\n");
	sqlite3_close(pDB);
	return ret;
}




//读取通话记中未接来电接口
int TelephoneHistory_callback(void *, int , char **argv, char **)
{
	g_nTeleHistoryCount = atoi(argv[0]);
	return 0;
}
int Sql_ReadTelephoneHistory(teleHistoryInfo **pTeleHistory, int *nCount )
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pSqlSel[320] = {0};
	
	sprintf(pSqlSel, "select TelephoneHistory.*, TelephoneBook.Name,TelephoneBook.Id from TelephoneHistory left join TelephoneBook  on TelephoneBook.PhoneNum = TelephoneHistory.PhoneNum or TelephoneBook.TelephoneNum = TelephoneHistory.PhoneNum  where TelephoneHistory.PhoneType = 2 order by TelephoneHistory.StartTime desc limit 100");

	char pSqlQueryCount[128] = {0};
	sprintf(pSqlQueryCount,"select count(*) from TelephoneHistory where PhoneType= 2");

	int i = 0;
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//统计数据条数
	ret = sqlite3_exec(pDB, pSqlQueryCount, TelephoneHistory_callback, 0, &pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}

	//分配使用的内存
	if (g_nTeleHistoryCount <= 0)
	{
		sqlite3_close(pDB);
		return -1;
	}

	int count = -1;
	
	if(g_nTeleHistoryCount  > 100)
	{
		*nCount = 100 ;
		count = 100;
	}
	else 
	{
		*nCount = g_nTeleHistoryCount;
		count =g_nTeleHistoryCount ;
	}
	
	*pTeleHistory = (teleHistoryInfo*)malloc(count * sizeof(teleHistoryInfo));
	if (*pTeleHistory == NULL)
	{
		sqlite3_close(pDB);
		return -1;
	}
	memset(*pTeleHistory, 0, count * sizeof(teleHistoryInfo));
	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}

	//读取全部数据
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
			break;

		((*pTeleHistory)[i]).nId = sqlite3_column_int(pStmt, 0);
		((*pTeleHistory)[i]).phoneType=sqlite3_column_int(pStmt, 1);
		strcpy(((*pTeleHistory)[i]).phoneNum, (char*)sqlite3_column_text(pStmt, 2));
		strcpy(((*pTeleHistory)[i]).startTime, (char*)sqlite3_column_text(pStmt, 3));
		strcpy(((*pTeleHistory)[i]).hTime, (char*)sqlite3_column_text(pStmt, 4));
		((*pTeleHistory)[i]).video = sqlite3_column_int(pStmt, 5);
		char *phoneName=(char*)sqlite3_column_text(pStmt, 6);
		if (phoneName != NULL)
		{
			if (strlen(phoneName)>0)
				strcpy(((*pTeleHistory)[i]).name, (char*)sqlite3_column_text(pStmt, 6));
		}

		((*pTeleHistory)[i]).bookId = sqlite3_column_int(pStmt, 7);
		i++;
	}
//	printf("---TelephoneBook---db read over \n");

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 1;
}


int Sql_ReleaseTelephoneHistory(teleHistoryInfo **pTeleHistory,  int *nCount)
{
	if( nCount > 0 && *pTeleHistory != NULL )
		free (*pTeleHistory);
	*pTeleHistory = NULL;
	nCount =0;
	return 0;
}

//读取通话记中已接来电接口
int TelephoneCalled_callback(void *, int , char **argv, char **)
{
//	printf("TelephoneCalled_callback start --->int =%d argv = %s azColName = %s \n", argc, argv[0], azColName[0]);
	g_nTeleCalledCount = atoi(argv[0]);
//	printf("TelephoneCalled_callback over --->nCount = %d \n", g_nTeleCalledCount);
	return 0;
}
int Sql_ReadTelephoneCalled(teleHistoryInfo **pTeleHistory, int *nCount )
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pSqlSel[320] = {0};
	sprintf(pSqlSel, "select TelephoneHistory.*, TelephoneBook.Name,TelephoneBook.Id from TelephoneHistory left join TelephoneBook  on  TelephoneBook.PhoneNum = TelephoneHistory.PhoneNum  or TelephoneBook.TelephoneNum = TelephoneHistory.PhoneNum where TelephoneHistory.PhoneType = 3 order by TelephoneHistory.StartTime desc limit 100");

	char pSqlQueryCount[128] = {0};
	sprintf(pSqlQueryCount,"select count(*) from TelephoneHistory where PhoneType= 3");

	int i = 0;
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
//		fprintf(stderr, "can't open database: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	//统计数据条数
	ret = sqlite3_exec(pDB, pSqlQueryCount, TelephoneCalled_callback, 0, &pError);
	if (ret != SQLITE_OK)
	{
//		printf("query count error: %s", pError);
		sqlite3_close(pDB);
		return -1;
	}

	//分配使用的内存
	if (g_nTeleCalledCount <= 0)
	{
		sqlite3_close(pDB);
		return -1;
	}
	
	int count = -1;
	
	if(g_nTeleCalledCount  > 100)
	{
		*nCount = 100 ;
		count = 100;
	}
	else 
	{
		*nCount = g_nTeleCalledCount;
		count =g_nTeleCalledCount ;
	}
		
	*pTeleHistory = (teleHistoryInfo*)malloc(count * sizeof(teleHistoryInfo));
	if (*pTeleHistory == NULL)
	{
		sqlite3_close(pDB);
		return -1;
	}
	memset(*pTeleHistory, 0, count * sizeof(teleHistoryInfo));
	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
//		printf("sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
	}

	//读取全部数据
//	printf("db read start \n");
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
			break;

		((*pTeleHistory)[i]).nId = sqlite3_column_int(pStmt, 0);
		((*pTeleHistory)[i]).phoneType=sqlite3_column_int(pStmt, 1);
		strcpy(((*pTeleHistory)[i]).phoneNum, (char*)sqlite3_column_text(pStmt, 2));
		strcpy(((*pTeleHistory)[i]).startTime, (char*)sqlite3_column_text(pStmt, 3));
		strcpy(((*pTeleHistory)[i]).hTime, (char*)sqlite3_column_text(pStmt, 4));
		((*pTeleHistory)[i]).video = sqlite3_column_int(pStmt, 5);
		char *phoneName=(char*)sqlite3_column_text(pStmt, 6);
		if (phoneName != NULL)
		{
			if (strlen(phoneName)>0)
				strcpy(((*pTeleHistory)[i]).name, (char*)sqlite3_column_text(pStmt, 6));
		}

		((*pTeleHistory)[i]).bookId = sqlite3_column_int(pStmt, 7);

		i++;
	}
//	printf("---TelephoneBook---db read over \n");

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 1;
}

//读取通话记中已拨电话接口
int TelephoneDialed_callback(void *, int , char **argv, char **)
{
//	printf("TelephoneDialed_callback start --->int =%d argv = %s azColName = %s \n", argc, argv[0], azColName[0]);
	g_nTeleDailedCount = atoi(argv[0]);
//	printf("TelephoneHistory_callback over --->nCount = %d \n", g_nTeleHistoryCount);
	return 0;
}
int Sql_ReadTelephoneDialed(teleHistoryInfo **pTeleHistory, int *nCount )
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pSqlSel[320] = {0};
	
	sprintf(pSqlSel,  "select TelephoneHistory.*, TelephoneBook.Name,TelephoneBook.Id from TelephoneHistory left join TelephoneBook  on  TelephoneBook.PhoneNum = TelephoneHistory.PhoneNum  or TelephoneBook.TelephoneNum = TelephoneHistory.PhoneNum where TelephoneHistory.PhoneType = 1 order by TelephoneHistory.StartTime desc limit 100");

	char pSqlQueryCount[128] = {0};
	sprintf(pSqlQueryCount,"select count(*) from TelephoneHistory where PhoneType= 1");

	int i = 0;
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		//fprintf(stderr, "can't open database: %s\n", sqlite3_errmsg(pDB));
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//统计数据条数
	ret = sqlite3_exec(pDB, pSqlQueryCount, TelephoneDialed_callback, 0, &pError);
	if (ret != SQLITE_OK)
	{
		printf("query count error: %s", pError);
		sqlite3_close(pDB);
		return -1;
	}
	//分配使用的内存
	if (g_nTeleDailedCount <= 0)
	{
		sqlite3_close(pDB);
		return -1;
	}

	int count = -1;
	
	if(g_nTeleDailedCount  > 100)
	{
		*nCount = 100 ;
		count = 100;
	}
	else 
	{
		*nCount = g_nTeleDailedCount;
		count =g_nTeleDailedCount ;
	}
	
	*pTeleHistory = (teleHistoryInfo*)malloc(count * sizeof(teleHistoryInfo));
	if (*pTeleHistory == NULL)
	{
		sqlite3_close(pDB);
		return -1;
	}
	memset(*pTeleHistory, 0, count  * sizeof(teleHistoryInfo));
	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		printf("sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
	}
	//读取全部数据
	printf("db read start \n");
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
			break;
		((*pTeleHistory)[i]).nId = sqlite3_column_int(pStmt, 0);
		((*pTeleHistory)[i]).phoneType=sqlite3_column_int(pStmt, 1);
		strcpy(((*pTeleHistory)[i]).phoneNum, (char*)sqlite3_column_text(pStmt, 2));
		strcpy(((*pTeleHistory)[i]).startTime, (char*)sqlite3_column_text(pStmt, 3));
		strcpy(((*pTeleHistory)[i]).hTime, (char*)sqlite3_column_text(pStmt, 4));
		((*pTeleHistory)[i]).video = sqlite3_column_int(pStmt, 5);
		char *phoneName=(char*)sqlite3_column_text(pStmt, 6);
		if (phoneName != NULL)
		{
			if (strlen(phoneName)>0)
				strcpy(((*pTeleHistory)[i]).name, (char*)sqlite3_column_text(pStmt, 6));
		}

		((*pTeleHistory)[i]).bookId = sqlite3_column_int(pStmt, 7);

		i++;
	}
	printf("---TelephoneBook---db read over \n");
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 1;
}


//pinsertData  插入的数据， dataSize 插入的数据大小

int Sql_InsertTelephoneHistory(  void *pInsertData,int dataSize,int type)
{
	printf("############int Sql_InsertTelephoneHistory, type is:%d \n\n\n",type);
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[256] = {0};
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "can't open database: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	//生成插入sql语句
	teleHistoryInfo miscall;
	memset(&miscall, 0, sizeof(miscall));
	memcpy(&miscall, pInsertData, dataSize);


	printf("type = %d   ,   phoneNum = %s   , startTime = %s ,   hTime = %s ,   video = %d\n\n ",type,miscall.phoneNum, miscall.startTime,miscall.hTime,miscall.video);
	sprintf(chSqlCmd, "insert into TelephoneHistory values(null, %d, '%s', '%s', '%s','%d')", 
	type,  miscall.phoneNum, miscall.startTime,miscall.hTime,miscall.video);   
	//准备插入数据
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "cjtserv----prepare insert table error:%s \n", pError);
		sqlite3_close(pDB);
		return -1;
	}
	//执行绑定以后的语句
	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
		fprintf(stderr, "cjtserv----sqlite3 step error \n");
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);
		return -1;
	}
	fprintf(stderr, "cjtserv------------------sql insert success \n");
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
	
}
//删除未接来电数据
int  Sql_DeleteMiss()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128]  = {0};
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//声称删除SQL语句
	sprintf(chSqlCmd, "delete from TelephoneHistory where PhoneType=2");

	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		//printf("cjtserv --- sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
	}
	//执行记录集
	while (true)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}

	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
}
//删除已接来电数据
int  Sql_DeleteCalled()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128]  = {0};
	char chSqlDBPath[128] = {0};

	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}

	//声称删除SQL语句
	sprintf(chSqlCmd, "delete from TelephoneHistory where PhoneType=3");

	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		//printf("cjtserv --- sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
	}

	//执行记录集
	while (true)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}

	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
}
//删除已拨电话数据
int  Sql_DeleteDial()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128]  = {0};
	char chSqlDBPath[128] = {0};

	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
	//	fprintf(stderr, "cjtserv------can't open database: %s\n", sqlite3_errmsg(pDB));
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}

	//声称删除SQL语句
	sprintf(chSqlCmd, "delete from TelephoneHistory where PhoneType=1");

	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
//		printf("cjtserv --- sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
	}

	//执行记录集
	while (true)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}

	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
}
//删除单条短信数据
int Sql_DeletePoiMsgByIndex( int nIndex)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128] = {0};
//	fprintf(stderr, "**%d\n",nIndex);					
	sprintf(chSqlCmd, "delete  from PoiMsg where Id = '%d'", nIndex);
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
//		fprintf(stderr, "cjtserv------can't open database: %s\n", sqlite3_errmsg(pDB));
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
//		printf("delete PoiMsg error!!\n");
		sqlite3_close(pDB);
		return -1;
	}

	//执行记录集
	while (true)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}

	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
	
}
int Sql_DeleteTeleHistoryByIndex(int nIndex)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128] = {0};
	sprintf(chSqlCmd, "delete  from TelephoneHistory where Id = '%d'", nIndex);
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//执行记录集
	while (true)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}
	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
}
int Sql_ReadTelephoneBookByTelephoneNum(teleBookInfo *pTeleBook,char *phoneNum)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128] = {0};
	
	sprintf(chSqlCmd, "select *  from TelephoneBook where TelephoneNum = '%s'", phoneNum);
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		//printf("sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
	}
	int result=-1;
	//执行记录集
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
		{
			break;
		}
		result=1;
		pTeleBook->nId = sqlite3_column_int(pStmt, 0);
		if((char*)sqlite3_column_blob(pStmt, 1)!=NULL)
		{
			strcpy(pTeleBook->friendName, (char*)sqlite3_column_blob(pStmt, 1));
		}
		strcpy(pTeleBook->friendPhone, (char*)sqlite3_column_text(pStmt, 2));
		strcpy(pTeleBook->friendEmail, (char*)sqlite3_column_text(pStmt, 3));
		strcpy(pTeleBook->friendCompany, (char*)sqlite3_column_blob(pStmt, 4));
		strcpy(pTeleBook->friendComAddress, (char*)sqlite3_column_blob(pStmt, 5));
		strcpy(pTeleBook->friendFamAddress, (char*)sqlite3_column_blob(pStmt, 6));
		if((char*)sqlite3_column_text(pStmt, 7)!=NULL){
			strcpy(pTeleBook->friendPhoto, (char*)sqlite3_column_text(pStmt, 7));
		}
		strcpy(pTeleBook->friendTelephoneNum, (char*)sqlite3_column_text(pStmt, 8));
		strcpy(pTeleBook->friendSound, (char*)sqlite3_column_text(pStmt, 9));
		pTeleBook->friendVideo = sqlite3_column_int(pStmt, 10);
	}
	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return result;
}

int Sql_ReadTelephoneBookByPhoneNum(teleBookInfo *pTeleBook,char *phoneNum)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128] = {0};
	char phoneNumAdd[30] = {0};
	char phoneNumSubtract[30] = {0};
	
	//sprintf(phoneNumAdd, "+86%s", phoneNum);
	
	//sprintf(phoneNumSubtract, "%s",phoneNum+3);

	//sprintf(chSqlCmd, "select *  from TelephoneBook where PhoneNum = '%s' or PhoneNum = '%s' or PhoneNum = '%s'", phoneNum,phoneNumAdd,phoneNumSubtract);
	sprintf(chSqlCmd, "select *  from TelephoneBook where PhoneNum = '%s' ", phoneNum);
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		//printf("sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
	}
	int result=-1;
	//执行记录集
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
		{
			break;
		}
		result=1;
		pTeleBook->nId = sqlite3_column_int(pStmt, 0);
		if((char*)sqlite3_column_blob(pStmt, 1)!=NULL)
		{
			strcpy(pTeleBook->friendName, (char*)sqlite3_column_blob(pStmt, 1));
		}
		strcpy(pTeleBook->friendPhone, (char*)sqlite3_column_text(pStmt, 2));
		strcpy(pTeleBook->friendEmail, (char*)sqlite3_column_text(pStmt, 3));
		strcpy(pTeleBook->friendCompany, (char*)sqlite3_column_blob(pStmt, 4));
		strcpy(pTeleBook->friendComAddress, (char*)sqlite3_column_blob(pStmt, 5));
		strcpy(pTeleBook->friendFamAddress, (char*)sqlite3_column_blob(pStmt, 6));
		if((char*)sqlite3_column_text(pStmt, 7)!=NULL){
			strcpy(pTeleBook->friendPhoto, (char*)sqlite3_column_text(pStmt, 7));
		}
		strcpy(pTeleBook->friendTelephoneNum, (char*)sqlite3_column_text(pStmt, 8));
		strcpy(pTeleBook->friendSound, (char*)sqlite3_column_text(pStmt, 9));
		pTeleBook->friendVideo = sqlite3_column_int(pStmt, 10);
	}
	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return result;
}

//删除通讯录数据
int  Sql_DeleteTelephoneBook()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128]  = {0};
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//声称删除SQL语句
	sprintf(chSqlCmd, "delete from TelephoneBook ");
	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
printf("--------------------------------------------->2");
		//printf("cjtserv --- sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
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
	sqlite3_close(pDB);
	return ret;
}

int Sql_DeleteTeleBookByIndex(int nIndex)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128] = {0};
	sprintf(chSqlCmd, "delete  from TelephoneBook where Id = '%d'", nIndex);
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
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
	sqlite3_close(pDB);
	return ret;
}

int Sql_ReadTelephoneBookByName(teleBookInfo *pTeleBook,char *phoneName)
{
	printf("1 \n");

	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128] = {0};
	sprintf(chSqlCmd, "select *  from TelephoneBook where Name = '%s'", phoneName);
	//printf("[%s] \n",phoneName);

	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		//printf("4 \n");
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		//printf("3 \n");

		//printf("sqlite3 select error \n");
		sqlite3_close(pDB);
		return -1;
	}
	int result=-1;
	//执行记录集
	while (1)
	{
		//printf("2 \n");

		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
		{
			//printf("3 \n");

			break;
		}
		result=1;
		pTeleBook->nId = sqlite3_column_int(pStmt, 0);
		if((char*)sqlite3_column_blob(pStmt, 1)!=NULL)
		{
			strcpy(pTeleBook->friendName, (char*)sqlite3_column_blob(pStmt, 1));
		}
		//printf("[%s] \n",pTeleBook->friendPhone);

		strcpy(pTeleBook->friendPhone, (char*)sqlite3_column_text(pStmt, 2));
		strcpy(pTeleBook->friendEmail, (char*)sqlite3_column_text(pStmt, 3));
		strcpy(pTeleBook->friendCompany, (char*)sqlite3_column_blob(pStmt, 4));
		strcpy(pTeleBook->friendComAddress, (char*)sqlite3_column_blob(pStmt, 5));
		strcpy(pTeleBook->friendFamAddress, (char*)sqlite3_column_blob(pStmt, 6));
		if((char*)sqlite3_column_text(pStmt, 7)!=NULL){
			strcpy(pTeleBook->friendPhoto, (char*)sqlite3_column_text(pStmt, 7));
		}
		strcpy(pTeleBook->friendTelephoneNum, (char*)sqlite3_column_text(pStmt, 8));
		strcpy(pTeleBook->friendSound, (char*)sqlite3_column_text(pStmt, 9));
		pTeleBook->friendVideo = sqlite3_column_int(pStmt, 10);
	}
	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return result;
}

int Sql_InsertTelephoneBook(  void *pInsertData,int dataSize)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[512] = {0};
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//生成插入sql语句
	teleBookInfo info;
	memset(&info, 0, sizeof(info));
	memcpy(&info, pInsertData, dataSize);

	//fprintf(stderr,"----$$$$$$$$$3----Sql_InsertTelephoneBook()----->%s---------------------\n",info.friendTelephoneNum);
	
	sprintf(chSqlCmd, "insert into TelephoneBook values(null, '%s', '%s', '%s', '%s','%s', '%s', '%s', '%s','%s',%d)", 
	info.friendName,info.friendPhone,info.friendEmail,info.friendCompany,info.friendComAddress,
	info.friendFamAddress,info.friendPhoto,info.friendTelephoneNum,info.friendSound,info.friendVideo
	);   
	//准备插入数据
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//执行绑定以后的语句
	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);
		return -1;
	}
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
	
}

//更新通讯录
int Sql_UpdateTelephoneBookById(void *pUpdateData,int dataSize,int id)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
//	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[256] = {0};
	char chSqlDBPath[128] = {0};

	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	teleBookInfo info;
	memset(&info, 0, sizeof(info));
	memcpy(&info, pUpdateData, dataSize);
	//组织声称更新数据sql语句
	sprintf(chSqlCmd, "update TelephoneBook set Name = '%s'  ,  PhoneNum ='%s', TelephoneNum='%s' where Id = '%d'",
	info.friendName,info.friendPhone,info.friendTelephoneNum,id);
	ret = sqlite3_exec(pDB, chSqlCmd, 0, 0, &pError);
	if (ret != SQLITE_OK)
	{
		//printf("1234567\n\n \n");

		sqlite3_close(pDB);
		return -1;
	}

	sqlite3_close(pDB);
	return ret;
}

//读取发件箱
int SendMsgBox_callback(void *, int , char **argv, char **)
{
	g_nSendMsgBoxCount = atoi(argv[0]);	
	return 0;
}
int Sql_ReadSendMsgBox( sendInfo**pPoiMsg, int *nCount)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	
	char *pSqlSel = (char*)"select SendMsg.*, TelephoneBook.Name,TelephoneBook.Id from SendMsg  left join TelephoneBook  on TelephoneBook.PhoneNum = SendMsg.ToPhoneNumber or TelephoneBook.TelephoneNum = SendMsg.ToPhoneNumber order by Time desc limit 100";
	char *pSqlQueryCount = (char*)"select count(*) from SendMsg";
	int i = 0;

	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//统计数据条数
	ret = sqlite3_exec(pDB, pSqlQueryCount, SendMsgBox_callback, 0, &pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//分配使用的内存
	if (g_nSendMsgBoxCount <= 0)
	{
		sqlite3_close(pDB);
		return -1;
	}
	
	int count = -1;
	
	if(g_nSendMsgBoxCount  > 100)
	{
		*nCount = 100 ;
		count = 100;
	}
	else 
	{
		*nCount = g_nSendMsgBoxCount;
		count =g_nSendMsgBoxCount ;
	}
	
	*pPoiMsg = (sendInfo*)malloc(count * sizeof(sendInfo));
	if (*pPoiMsg == NULL)
	{
		sqlite3_close(pDB);
		return -1;
	}
	memset(*pPoiMsg, 0, count * sizeof(sendInfo));
	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//读取全部数据
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
			break;
		
		((*pPoiMsg)[i]).nId = sqlite3_column_int(pStmt, 0);
		
		char *pNum = (char*)sqlite3_column_text(pStmt, 1);
		if (pNum != NULL)
		{
			if (strlen(pNum)>0)
				strcpy(((*pPoiMsg)[i]).chToPhoneNumber, pNum);
		}
		char *pTime =  (char*)sqlite3_column_text(pStmt, 2);
		if (pTime != NULL)
		{
			if (strlen(pTime)>0)
				strcpy(((*pPoiMsg)[i]).chToTime, pTime);
		}
		
		char *pContent = (char*)sqlite3_column_blob(pStmt, 3);
		if (pContent != NULL)
		{
			if (strlen(pContent) > 0)
				strcpy(((*pPoiMsg)[i]).chToContent, (char*)sqlite3_column_blob(pStmt, 3));
		}
		char *phoneName=(char*)sqlite3_column_text(pStmt, 4);
		if (phoneName != NULL)
		{
			if (strlen(phoneName)>0)
				strcpy(((*pPoiMsg)[i]).name, (char*)sqlite3_column_text(pStmt, 4));
		}

		((*pPoiMsg)[i]).bookId = sqlite3_column_int(pStmt, 5);

 		i++;
	}
 
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 1;
}
//插入发件箱
int Sql_InsertSendMsgBox(  void *pInsertData, int dataSize)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[600] = {0};
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{	
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//生成插入sql语句
	sendInfo infor;
	memset(&infor, 0, sizeof(infor));
	memcpy(&infor, pInsertData, dataSize);
	sprintf(chSqlCmd, "insert into SendMsg values(null, '%s', '%s', '%s')", 
	infor.chToPhoneNumber,infor.chToTime,infor.chToContent);   
	//准备插入数据
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//执行绑定以后的语句
	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
		return -1;
	}
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
}
//释放查询发件箱数据的内存
int Sql_ReleaseSendMsgBox(sendInfo **pPoiMsg, int *nCount)
{
	if( nCount > 0 && *pPoiMsg != NULL )
		free (*pPoiMsg);
	*pPoiMsg = NULL;
	g_nSendMsgBoxCount=0;

	return 0;
}

//清空发件箱
int Sql_DeleteAllSendMsgBox()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128]  = {0};
	char chSqlDBPath[128] = {0};

	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//声称删除SQL语句
	sprintf(chSqlCmd, "delete from SendMsg");
	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//执行记录集
	while (true)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}
	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
}
//删除单条发件箱中记录
int Sql_DeleteSendMsgBoxByIndex(int nIndex)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char chSqlCmd[128] = {0};
	sprintf(chSqlCmd, "delete  from SendMsg where Id = '%d'", nIndex);
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		sqlite3_close(pDB);
		return -1;
	}
	//准备执行语句
	ret = sqlite3_prepare(pDB, chSqlCmd, strlen(chSqlCmd), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return -1;
	}
	//执行记录集
	while (true)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}
	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return ret;
}

//匹配人名的通讯录记录
int TelephoneBook_Allcallback(void *, int , char **argv, char **)
{
	g_nTeleBookNameCount = atoi(argv[0]);
	return 0;
}
int Sql_ReadAllTelephoneBookByName(teleBookInfo **pTeleBook, int *nCount, char *phoneName)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	
	char pSqlSel[128];
	char pSqlQueryCount[128];
	
	sprintf(pSqlSel, "select *  from TelephoneBook where Name LIKE '%%%s%%' ", phoneName);

	sprintf(pSqlQueryCount,"select count(*) from TelephoneBook where Name LIKE '%%%s%%' ",phoneName);
	
	int i = 0;
	char chSqlDBPath[128] = {0};
	//打开数据库
	sprintf(chSqlDBPath, "%s%s", DB_PATH, DB_SERV_NAME);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		const char* tmp = "can't open database   ";
		IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<IBD_ENDL<<IBD_END

		//fprintf(stderr, "can't open database: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	//统计数据条数
	ret = sqlite3_exec(pDB, pSqlQueryCount, TelephoneBook_callback, 0, &pError);
	if (ret != SQLITE_OK)
	{
		//printf("query count error: %s", pError);
		sqlite3_close(pDB);
		return -1;
	}
	//分配使用的内存
	if (g_nTeleBookCount <= 0)
	{
		sqlite3_close(pDB);
		return -1;
	}
	*nCount = g_nTeleBookCount;
	
	if (*pTeleBook != NULL)
	{
		free(*pTeleBook);
		*pTeleBook = NULL;
	}
	*pTeleBook = (teleBookInfo*)malloc(g_nTeleBookCount * sizeof(teleBookInfo));
	if (*pTeleBook == NULL)
	{
		//printf("pPoiMsg malloc error");
		sqlite3_close(pDB);
		return -1;
	}
	memset(*pTeleBook, 0, g_nTeleBookCount * sizeof(teleBookInfo));

	//准备读取数据
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(pDB);
		//printf("sqlite3 select error \n");
		return -1;
	}
	//读取全部数据
	printf("db read start \n");
	while (1)
	{
		ret = sqlite3_step(pStmt);		//移动记录集
		if (ret != SQLITE_ROW)
			break;

		((*pTeleBook)[i]).nId = sqlite3_column_int(pStmt, 0);
		strcpy(((*pTeleBook)[i]).friendName, (char*)sqlite3_column_blob(pStmt, 1));
		strcpy(((*pTeleBook)[i]).friendPhone, (char*)sqlite3_column_text(pStmt, 2));
		strcpy(((*pTeleBook)[i]).friendEmail, (char*)sqlite3_column_text(pStmt, 3));
		strcpy(((*pTeleBook)[i]).friendCompany, (char*)sqlite3_column_blob(pStmt, 4));
		strcpy(((*pTeleBook)[i]).friendComAddress, (char*)sqlite3_column_blob(pStmt, 5));
		strcpy(((*pTeleBook)[i]).friendFamAddress, (char*)sqlite3_column_blob(pStmt, 6));
		strcpy(((*pTeleBook)[i]).friendPhoto, (char*)sqlite3_column_text(pStmt, 7));
		strcpy(((*pTeleBook)[i]).friendTelephoneNum, (char*)sqlite3_column_text(pStmt, 8));
		strcpy(((*pTeleBook)[i]).friendSound, (char*)sqlite3_column_text(pStmt, 9));
		((*pTeleBook)[i]).friendVideo = sqlite3_column_int(pStmt, 10);
		//fprintf(stderr, "#################: %s\n", ((*pTeleBook)[i]).friendTelephoneNum);
		i++;
	}

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);        
	return 1;
}
//释放查询的结果的内存
int Sql_ReleaseAllTelephoneBook(teleBookInfo **pTeleBook, int *nCount)
{
	
	if( nCount > 0 && *pTeleBook != NULL )
		free (*pTeleBook);
	*pTeleBook = NULL;
	g_nTeleBookNameCount =0;

	return 0;
}

