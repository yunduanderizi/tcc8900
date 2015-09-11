/**
 * @file   dataManager.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111017
 * 
 * @brief  
 *      cpp file of dataManager module's function class 
 * 
 */


#include "dataManager.h"
#include <QTime>
#include <sqlite3.h>
#include "common_config.h"
#include <sys/vfs.h>


#define  TMP_LOG_FILE  "/nand2/df_tmp.log"
#define USERRUBAREA  "nand3"

static QMutex gs_mutexForThreadSafe;
static QMutex gs_mutexWaitCondition;
namespace DataManager
{

typedef QList<USER_DATA_SURPOR_TYPE>User_Data_Error_List;

static int _readErrorInfoFromDatabase(User_Data_Error_List *dataTypeList)
{
	int retval = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pQueryTablesSel = (char*)"SELECT * FROM UserDataErrorInfo";

	retval = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database: %s!\n", sqlite3_errmsg(pDB))
		sqlite3_close(pDB);
		return -1;
	}

	retval = sqlite3_busy_timeout(pDB, 1000);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	retval = sqlite3_prepare(pDB, pQueryTablesSel, strlen(pQueryTablesSel), &pStmt, (const char**)&pError);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))

		sqlite3_close(pDB);
		return -1;
	}
	int i = 0;
	while(1)
	{
		retval = sqlite3_step(pStmt);
		if (retval != SQLITE_ROW)
		{
			sqlite3_finalize(pStmt);
			sqlite3_close(pDB);
			break;
		}
		else
		{
			int dataType = sqlite3_column_int(pStmt,0);
			if( dataType < 0)
			{
				sqlite3_finalize(pStmt);
				sqlite3_close(pDB);
				return -1;
			}
			dataTypeList->append((USER_DATA_SURPOR_TYPE)dataType);
			i++;
		}
 
		
	}
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	return 0;
}
static int _saveErrorInfoToDatabase(USER_DATA_SURPOR_TYPE dataType)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pInsertTablesSel[128] = {0};

	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s!\n", sqlite3_errmsg(pDB))	
		return -1;
	}

	ret = sqlite3_busy_timeout(pDB, 1000);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}


	while (1)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}

	sqlite3_finalize(pStmt);

	
	sprintf(pInsertTablesSel,"insert into UserDataErrorInfo values(%d)",(int)dataType);

	ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
       if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		return -1;
	}
	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		return -1;
	}

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	return 0;
}

static int _deteleErrorInfoFromDatabase()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteUserDataSel[128] = "delete from UserDataErrorInfo";
	
	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s!\n", sqlite3_errmsg(pDB))	
		return -1;
	}

	ret = sqlite3_busy_timeout(pDB, 1000);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_prepare(pDB, pDeleteUserDataSel, strlen(pDeleteUserDataSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);
		return -1;
	}
	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		return -1;
	}
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return 0;
}
DataManagerC::DataManagerC()
	: m_bReadWriteUserData(false),
	  m_pPluginManager(NULL)
{
	m_pPluginManager = new PluginManagerC();
	connect(m_pPluginManager, SIGNAL(sigBackupUserDataProgress(int)), this, SIGNAL(sigBackupUserDataProgress(int)), Qt::DirectConnection);
	connect(m_pPluginManager, SIGNAL(sigRevertUserDataProgress(int)), this, SIGNAL(sigRevertUserDataProgress(int)), Qt::DirectConnection);
	connect(m_pPluginManager, SIGNAL(sigRepairUserDataProgress(int)), this, SIGNAL(sigRepairUserDataProgress(int)), Qt::DirectConnection);
	connect(m_pPluginManager, SIGNAL(sigUserDataHandleResult(DM_ERROR_INFO,QString)), this, SIGNAL(sigUserDataHandleResult(DM_ERROR_INFO,QString)), Qt::DirectConnection);
}	

DataManagerC::~DataManagerC()
{
	if( m_pPluginManager )
	{
		delete m_pPluginManager;
		m_pPluginManager = NULL;
	}
}
bool DataManagerC::_isSubareaMountSuccess()
{
       char cmd[100] = "";
	sprintf(cmd,"df > %s",TMP_LOG_FILE);
	if(system(cmd) != 0)
	{
		printf("ERROR::system cmd[%s] error\n",cmd);
		return false;
	}
	FILE *fp=fopen(TMP_LOG_FILE,"r");
	if(NULL == fp)
	{
		printf("ERROR::open file[%s] error\n",TMP_LOG_FILE);
		return false;
	}
	while(!feof(fp))
	{
		char tmpbuffer[BUFFSIZE] = {};
		if(fgets(tmpbuffer,100,fp)!=NULL)
           	{           
			if(NULL != strstr(tmpbuffer,USERRUBAREA))
			{
				fclose(fp);
				remove(TMP_LOG_FILE);
				return true;
			}
		}
	}
    	fclose(fp);
	remove(TMP_LOG_FILE);
	return false;
}

QString DataManagerC::findPluginOfType(USER_DATA_SURPOR_TYPE dataType)
{
	return m_pPluginManager->findPluginOfType(dataType);
}
void DataManagerC::updateReadWriteUserDataFlag(bool bReadWriteUserData)
{
	m_bReadWriteUserData = bReadWriteUserData;
}

void DataManagerC::backupUserData(QString sAddress)
{

	m_bReadWriteUserData = false;
	emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);

	gs_mutexForThreadSafe.lock();
	
	DownloadTask myTask;
	myTask.backupAddress = sAddress;
	myTask.ThreadWorkMode= 0;
	m_TaskList.append(myTask);
	if(!isRunning() )
	{
		start();
	}
	gs_mutexForThreadSafe.unlock();


}

void DataManagerC::revertUserData(QString sAddress)
{

	m_bReadWriteUserData = false;
	emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
	gs_mutexForThreadSafe.lock();
	DownloadTask myTask;
	myTask.revertAddress = sAddress;
	myTask.ThreadWorkMode= 1;
	
	m_TaskList.append(myTask);
	if(!isRunning() )
	{
		start();
	}
	gs_mutexForThreadSafe.unlock();

}
void DataManagerC::repairUserData(QString sAddress)
{

	m_bReadWriteUserData = false;
	emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
	gs_mutexForThreadSafe.lock();
	DownloadTask myTask;
	myTask.ThreadWorkMode= 2;
	myTask.repairBackAddress = sAddress;
	m_TaskList.append(myTask);
	if(!isRunning() )
	{
		start();
	}
	gs_mutexForThreadSafe.unlock();
	
}

void DataManagerC::requestReadWriteUserData()
{
	 if( m_bReadWriteUserData )
		return;
	gs_mutexForThreadSafe.lock();	
	QTime  time;
	time.start();
	while(1)
	{
		if( m_bReadWriteUserData )
		{
			emit sigReadWriteUserDataChanged();
			emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
			break;
	 	}
		if(time.elapsed() == 10*1000)
		{
			 break;
		}
	}
	gs_mutexForThreadSafe.unlock();
	

}

void DataManagerC::reportError(USER_DATA_SURPOR_TYPE dataType)
{
	if(_saveErrorInfoToDatabase(dataType) < 0)
		IBD_PRINTF(DEBUG_INFOMATION,"ERROR:save errorInfo[%d] to base error \n",dataType);	
	return;
}
void DataManagerC::_repairUserSubarea()
{
	m_bReadWriteUserData = false;
	emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
	gs_mutexForThreadSafe.lock();
	DownloadTask myTask;
	myTask.ThreadWorkMode= 3;
	m_TaskList.append(myTask);
	if(!isRunning() )
	{
		start();
	}
	gs_mutexForThreadSafe.unlock();
	
}
int DataManagerC::startupCheck()
{
	if(_isSubareaMountSuccess())
	{
		User_Data_Error_List m_dataTypeList;
		if(_readErrorInfoFromDatabase(&m_dataTypeList) < 0)
			IBD_PRINTF(DEBUG_INFOMATION,"ERROR:read errorInfo from base error \n");	
		if(m_dataTypeList.size() == 0)
		{
			m_bReadWriteUserData = true;
			emit sigReadWriteUserDataChanged();
			emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
			return DM_E_OK;	
		}
		return USERDATA_ERROR;
			
	}
	else
	{
		_repairUserSubarea();
		return RUBAREA_UMOUNT;
	}
}


void DataManagerC::ignoreError()
{
	 if(_deteleErrorInfoFromDatabase() <0)
		IBD_PRINTF(DEBUG_INFOMATION,"ERROR:delete errorInfo to base error \n");
	m_bReadWriteUserData = true;
	emit sigReadWriteUserDataChanged();
	emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
}

void DataManagerC::ignoreErrorThisTime()
{
	 m_bReadWriteUserData = true;
	emit sigReadWriteUserDataChanged();
	emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
}

bool DataManagerC::isReadWriteUserData()
{
	 return m_bReadWriteUserData;
}

void DataManagerC::run()
{
	DownloadTask myTask;
	int status;
	while(1)
	{
		gs_mutexForThreadSafe.lock();
		if( !m_TaskList.isEmpty() )
		{
			myTask = m_TaskList.takeFirst();
		}
		else
		{
			gs_mutexForThreadSafe.unlock();
			break;
		}
		gs_mutexForThreadSafe.unlock();
		if(myTask.ThreadWorkMode == 0)
		{
			m_pPluginManager->backupUserData(myTask.backupAddress);
	
			m_bReadWriteUserData = true;
			emit sigReadWriteUserDataChanged();
			emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
		}
		else if(myTask.ThreadWorkMode == 1)
		{
			m_pPluginManager->revertUserData(myTask.revertAddress);
		
			m_bReadWriteUserData = true;
			emit sigReadWriteUserDataChanged();
			emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
		}
		else if(myTask.ThreadWorkMode == 2)
		{
			m_pPluginManager->repairUserData(myTask.repairBackAddress);
		
			m_bReadWriteUserData = true;
			emit sigReadWriteUserDataChanged();
			emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
		}
		else if(myTask.ThreadWorkMode == 3)
		{
			m_pPluginManager->repairUserSubarea();
			m_bReadWriteUserData = true;
			emit sigReadWriteUserDataChanged();
			emit sigSendReadWriteUserDataFlag(m_bReadWriteUserData);
		}
	}

	
}

}

