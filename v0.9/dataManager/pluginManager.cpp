/**
 * @file   pluginManager.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111017
 * 
 * @brief  
 *      cpp file of dataManager module's plugin manager class 
 * 
 */

#include "pluginManager.h"
#include <QSettings>
#include <sqlite3.h>
#include "common_config.h"
#include <sys/vfs.h>


#define PLUGIN_DATA_PATH 			 "./DataManagerPlugin/"
#define PLUGIN_LIB_LOG  				"./pluginDataList.log"
#define REPAIR_SUBAREA				 "./repair_subarea.sh"
#define REPAIR_DATA					"./repair_data.sh"
#define VERSION_INI					"/application/data/version.ini"


namespace DataManager
{


PluginManagerC::PluginManagerC()
{

}

PluginManagerC::~PluginManagerC()
{

}
static int _readVersionInfoFromDatabase(QString DataBasePath,int &versionInfo)
{
	int retval = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pQueryTablesSel = (char*)"SELECT * FROM Version";
	
	retval = sqlite3_open(DataBasePath.toAscii().data(), &pDB);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database: %s!\n", sqlite3_errmsg(pDB))
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
	retval = sqlite3_step(pStmt);
	if (retval != SQLITE_ROW)
	{
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);
		return 0;
	}
	versionInfo = sqlite3_column_int(pStmt, 0);	
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	return 0;
}

static int _saveVersionInfoToDatabase(int versionInfo)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = "delete from Version";
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

	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB)) 
		sqlite3_close(pDB);
		return -1;
	}

	sqlite3_finalize(pStmt);

	sprintf(pInsertTablesSel, "insert into Version values('%d')", versionInfo);
	ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
       if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB)) 
		sqlite3_close(pDB);
		return -1;
	}
	
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	return 0;
}
int PluginManagerC::_CheckPath(QString sAddress)
{
	QString cmd =  "cd ";
	cmd += sAddress;
	if(system(cmd.toUtf8().data()) != 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error:The path[%s] is not exist \n",sAddress.toUtf8().data());
		return DM_E_ADDRESS_NO_EXIST;

	}
	if(system("touch tmp.log") != 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION,"ERROR:The path [%s] can not write \n",sAddress.toUtf8().data());
		return DM_E_ADDRESS_READWRITE_ERROR ;
	}
	struct statfs statfs_buf;
	statfs(sAddress.toAscii().data(), &statfs_buf);
	int Pathsize = statfs_buf.f_bsize * statfs_buf.f_bavail /1024/1024;
	if(Pathsize <10)
	{
		IBD_PRINTF(DEBUG_INFOMATION,"ERROR:The path size  [%dM] not enough 10M \n",Pathsize);
		return DM_E_ADDRESS_NOT_ENOUGH;
	}	
	remove("tmp.log");
	return DM_E_OK;
}
int PluginManagerC::_readLib(PluginList *libList)
{
	if(NULL == libList)
	{
		return -1;
	}

	char cmd[BUFFSIZE] = {};
	char *plugin_dir = (char *)"";
	plugin_dir = getenv("DATA_MANAGER_PLUGIN");
	if(plugin_dir!= NULL)
		sprintf(cmd,"ls %s/ > %s",plugin_dir,PLUGIN_LIB_LOG);
	else
		sprintf(cmd,"ls %s/ > %s",PLUGIN_DATA_PATH,PLUGIN_LIB_LOG);
	if(system(cmd) != 0)
	{
		printf("ERROR::system cmd[%s] error\n",cmd);
		return -1;
	}
	FILE *fp=fopen(PLUGIN_LIB_LOG,"r");
	if(NULL == fp)
	{
		return -1;;
	}
	while(!feof(fp))
	{
		char tmpbuffer[BUFFSIZE] = {};
		if(fgets(tmpbuffer,100,fp)!=NULL)
           	{           
			QString libPath= "";
			QString strTmp = tmpbuffer;
			QString strLib = "";
			strLib = strTmp.trimmed();
			
			if(strLib.startsWith("lib") && strLib.endsWith(".so"))
			{
				libPath = PLUGIN_DATA_PATH;
				libPath += strLib;
				libList->append(libPath);
			}
		}
	}
    	fclose(fp);
	remove(PLUGIN_LIB_LOG);
	return 0;
}
void *PluginManagerC::_load(char *pluginName,PluginDataC *&GetObject, void (*&ReleaseObject)(void))
{
	if(NULL == pluginName)
		return NULL;
	
	void *libFp = dlopen(pluginName, RTLD_LAZY);
	if(NULL == libFp)
	{
		printf("ERROR::dlopen libname[%s] error(%s)!\n",pluginName,dlerror());
	 	return NULL;
	}
	PluginDataC *(*pGetObject)(void);
	
	pGetObject = (PluginDataC *(*)(void))dlsym(libFp, "GetObject");
	if (NULL == pGetObject)
	{
		printf("ERROR::lib[%s],dlsym fun(GetObject)error(%s)!\n",pluginName,dlerror());
		dlclose(libFp);
		return NULL;
	}
	GetObject = (*pGetObject)();
	ReleaseObject =  (void (*)(void))dlsym(libFp, "ReleaseObject");

	return libFp;

}
bool PluginManagerC::_isRepairUserSubarea()
{
	PluginList m_List;
	if(_readLib(&m_List)<0)
	{
		return false;
	}
	for(int i = 0;i<m_List.size();i++)
	{
		PluginDataC *m_pGetObject = NULL;
		void  (*m_pReleaseObject)(void);
		void *dl_fp = _load(m_List.at(i).toUtf8().data(),m_pGetObject,m_pReleaseObject);
		if(dl_fp != NULL)
		{
			
			if(m_pGetObject->isRepairUserSubarea())
			{			
				(*m_pReleaseObject)();
				dlclose(dl_fp);
				return true;
			}
			else
			{
				(*m_pReleaseObject)();
				dlclose(dl_fp);				
			}
			
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "ERROR::load pluginName[%s]plugin error(%s)\n",m_List.at(i).toUtf8().data(),dlerror());
			(*m_pReleaseObject)();
			dlclose(dl_fp);
			return false;
		}
	
	}

	return false;
}
int PluginManagerC::_inquireSurporType(TypeAndPlugin_List *typeAndPluginList)
{
	int status = DM_E_OK;
	PluginList m_List;
	if(_readLib(&m_List)<0)
	{
		return DM_E_LOAD_PLUGIN_ERROR;
	}
	for(int j = TYPE_FRIST; j<TYPE_MAX; j++)
	{
		int bSurpor = -1;
		TypeAndPlugin m_typeAndPlugin;
		m_typeAndPlugin.clear();
		for(int i = 0;i<m_List.size();i++)
		{
			PluginDataC *m_pGetObject = NULL;
			void  (*m_pReleaseObject)(void);
			void *dl_fp = _load(m_List.at(i).toUtf8().data(),m_pGetObject,m_pReleaseObject);

			if(dl_fp != NULL)
			{
				
				if(m_pGetObject->inquireSurporType((USER_DATA_SURPOR_TYPE)j))
				{
					m_typeAndPlugin.pluginName = m_List.at(i).toUtf8().data();
					m_typeAndPlugin.pluginDataPlugin = (USER_DATA_SURPOR_TYPE)j;
					typeAndPluginList->append(m_typeAndPlugin);
					bSurpor = 0;
					(*m_pReleaseObject)();
					dlclose(dl_fp);
					break;
				}
				else
				{
					(*m_pReleaseObject)();
					dlclose(dl_fp);				
				}
				
			}
			else
			{
				IBD_PRINTF(DEBUG_INFOMATION, "ERROR::load pluginName[%s]plugin error(%s)\n",m_List.at(i).toUtf8().data(),dlerror());
				(*m_pReleaseObject)();
				dlclose(dl_fp);
				return DM_E_CHECK_VERSION_ERROR;
			}
		
		}
		if(bSurpor == -1)
		{
			m_typeAndPlugin.pluginName = "";
			m_typeAndPlugin.pluginDataPlugin = (USER_DATA_SURPOR_TYPE)j;
			typeAndPluginList->append(m_typeAndPlugin);
		}
	}
	return status;
	

}

int PluginManagerC::_checkRervertUserData(int currentVersion,int backVersion)
{
	int status = DM_E_OK;
	PluginList m_List;
	if(_readLib(&m_List)<0)
	{
		return DM_E_LOAD_PLUGIN_ERROR;
	}
	for(int i = 0;i<m_List.size();i++)
	{
		PluginDataC *m_pGetObject = NULL;
		void  (*m_pReleaseObject)(void);
		void *dl_fp = _load(m_List.at(i).toUtf8().data(),m_pGetObject,m_pReleaseObject);

		if(dl_fp != NULL)
		{
			int status = m_pGetObject->checkRervertUserData(currentVersion,backVersion);
			if(status < 0 )
			{
				(*m_pReleaseObject)();
				dlclose(dl_fp);
				return status;
			}
			(*m_pReleaseObject)();
		
			dlclose(dl_fp);
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "ERROR::load pluginName[%s]plugin error(%s)\n",m_List.at(i).toUtf8().data(),dlerror());
			(*m_pReleaseObject)();
			dlclose(dl_fp);
			return DM_E_CHECK_VERSION_ERROR;
		}
		
	}
	return status;
	
}
void PluginManagerC::_getCurrentVersionInfo(int &currentVersion)
{
	QSettings *ConfigIni = new QSettings(VERSION_INI,QSettings::IniFormat,0);
	currentVersion = ConfigIni->value("Internal-Version").toInt();
}
QString PluginManagerC::findPluginOfType(USER_DATA_SURPOR_TYPE dataType)
{
	QString TmpPlugin = "";
    	TypeAndPlugin_List m_typeAndPluginList;

	if(_inquireSurporType(&m_typeAndPluginList)<0)
	 	return TmpPlugin;
	else
	{
			for(int i = 0;i < m_typeAndPluginList.size();i++)
			{
			if(m_typeAndPluginList.at(i).pluginDataPlugin == dataType)
			
			TmpPlugin = m_typeAndPluginList.at(i).pluginName;
			}
	}
	return TmpPlugin;
}

void PluginManagerC::backupUserData(QString sAddress)
{
	DM_ERROR_INFO status = DM_E_OK;
	status= (DM_ERROR_INFO)_CheckPath(sAddress);
	if(status != DM_E_OK)
	{
		emit sigUserDataHandleResult(status,dataErrorInfo[status]);
		return;
	}
	PluginList m_List;
	if(_readLib(&m_List)<0)
	{
		emit sigUserDataHandleResult(DM_E_LOAD_PLUGIN_ERROR, dataErrorInfo[DM_E_LOAD_PLUGIN_ERROR]);
		return;
	}
	int m_currentVersion = 0;
	_getCurrentVersionInfo(m_currentVersion);
	if(0 == m_currentVersion)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::ger version number error \n");
		emit sigUserDataHandleResult(DM_E_GET_VERSION_ERROR, dataErrorInfo[DM_E_GET_VERSION_ERROR]);
		return;
	}
	if(_saveVersionInfoToDatabase(m_currentVersion)<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::saveVersionToDatabase error \n");
		emit sigUserDataHandleResult(DM_E_DATA_BASE_ERROR, dataErrorInfo[DM_E_DATA_BASE_ERROR]);
		return;
	}
	for(int i = 0;i<m_List.size();i++)
	{
		PluginDataC *m_pGetObject = NULL;
		void  (*m_pReleaseObject)(void);
		void *dl_fp = _load(m_List.at(i).toUtf8().data(),m_pGetObject,m_pReleaseObject);

		if(dl_fp != NULL)
		{
			int result = m_pGetObject->backupUserData(sAddress);
			if(result < 0 )
			{
				(*m_pReleaseObject)();
				dlclose(dl_fp);
				dataErrorInfo[DM_E_BACKUP_ERROR] += "[%s]";
				dataErrorInfo[DM_E_BACKUP_ERROR] += dataErrorInfo[DM_E_BACKUP_ERROR].number(result);
				emit sigUserDataHandleResult(DM_E_BACKUP_ERROR, dataErrorInfo[DM_E_BACKUP_ERROR]);
				return;
			}
			(*m_pReleaseObject)();	
			dlclose(dl_fp);
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "ERROR::load pluginName[%s]plugin error(%s)\n",m_List.at(i).toUtf8().data(),dlerror());
			(*m_pReleaseObject)();
			dlclose(dl_fp);
			emit sigUserDataHandleResult(DM_E_LOAD_PLUGIN_ERROR, dataErrorInfo[DM_E_LOAD_PLUGIN_ERROR]);
			return ;
		}
	
		int progress = ((float) (i+1)/(float) m_List.size())* 100;
		emit sigBackupUserDataProgress(progress);
	
	}
	emit sigUserDataHandleResult(status, dataErrorInfo[status]);
}

void PluginManagerC::revertUserData(QString sAddress)
{
	DM_ERROR_INFO status = DM_E_OK;
	status= (DM_ERROR_INFO)_CheckPath(sAddress);
	if(status != DM_E_OK)
	{
		emit sigUserDataHandleResult(status,dataErrorInfo[status]);
		return;
	}
	PluginList m_List;
	if(_readLib(&m_List)<0)
	{
		emit sigUserDataHandleResult(DM_E_LOAD_PLUGIN_ERROR, dataErrorInfo[DM_E_LOAD_PLUGIN_ERROR]);
		return;
	}
	int m_backVersion = 0;
	char path_db[200] = "";
	sprintf(path_db,"%s%s",sAddress.toAscii().data(),SERVICE_BACKNAME);
	if(_readVersionInfoFromDatabase(path_db,m_backVersion)<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::readVersionInfoFromDatabase error \n");
		emit sigUserDataHandleResult(DM_E_DATA_BASE_ERROR, dataErrorInfo[DM_E_DATA_BASE_ERROR]);
		return;
	}
	int m_currentVersion = 0;
	if(_readVersionInfoFromDatabase(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY,m_currentVersion)<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::readVersionInfoFromDatabase error \n");
		emit sigUserDataHandleResult(DM_E_DATA_BASE_ERROR, dataErrorInfo[DM_E_DATA_BASE_ERROR]);
		return;
	}
	if(_checkRervertUserData(m_currentVersion,m_backVersion)<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::can not revertUserData \n");
		emit sigUserDataHandleResult(DM_E_CHECK_VERSION_ERROR, dataErrorInfo[DM_E_CHECK_VERSION_ERROR]);
		return;
	}
	for(int i = 0;i<m_List.size();i++)
	{
		PluginDataC *m_pGetObject = NULL;
		void  (*m_pReleaseObject)(void);
		void *dl_fp = _load(m_List.at(i).toUtf8().data(),m_pGetObject,m_pReleaseObject);

		if(dl_fp != NULL)
		{		
			int result  = m_pGetObject->revertUserData(sAddress);

			if(result < 0 )
			{
				(*m_pReleaseObject)();
				dlclose(dl_fp);
				dataErrorInfo[DM_E_BACKUP_ERROR] += "[%s]";
				dataErrorInfo[DM_E_BACKUP_ERROR] += dataErrorInfo[DM_E_BACKUP_ERROR].number(result);
				emit sigUserDataHandleResult(DM_E_REVER_ERROR, dataErrorInfo[DM_E_REVER_ERROR]);
				return;
			}
			(*m_pReleaseObject)();
		
			dlclose(dl_fp);
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "ERROR::load pluginName[%s]plugin error(%s)\n",m_List.at(i).toUtf8().data(),dlerror());
			(*m_pReleaseObject)();
			dlclose(dl_fp);
			emit sigUserDataHandleResult(DM_E_LOAD_PLUGIN_ERROR, dataErrorInfo[DM_E_LOAD_PLUGIN_ERROR]);
			return ;
		}
		int progress = ((float) (i+1)/(float) m_List.size())* 100;
		emit sigRevertUserDataProgress(progress);
	}
	if(_saveVersionInfoToDatabase(m_currentVersion)<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::saveVersionToDatabase error \n");
		emit sigUserDataHandleResult(DM_E_DATA_BASE_ERROR, dataErrorInfo[DM_E_DATA_BASE_ERROR]);
		return;
	}
	emit sigUserDataHandleResult(status, dataErrorInfo[status]);
}
void PluginManagerC::repairUserSubarea()
{
	emit sigUserDataHandleResult(DM_E_REPAIR_SUBAREA_START, dataErrorInfo[DM_E_REPAIR_SUBAREA_START]);

	if(0!= system(REPAIR_SUBAREA))
		emit sigUserDataHandleResult(DM_E_REPAIR_SUBAREA_ERROR, dataErrorInfo[DM_E_REPAIR_SUBAREA_ERROR]);
	else
		emit sigUserDataHandleResult(DM_E_REPAIR_SUBAREA_OK, dataErrorInfo[DM_E_REPAIR_SUBAREA_OK]);
}
void PluginManagerC::repairUserData(QString sAddress)
{
	DM_ERROR_INFO status = DM_E_OK;
	status= (DM_ERROR_INFO)_CheckPath(sAddress);
	if(status != DM_E_OK)
	{
		emit sigUserDataHandleResult(status,dataErrorInfo[status]);
		return;
	}
	PluginList m_List;
	if(_readLib(&m_List)<0)
	{
		emit sigUserDataHandleResult(DM_E_LOAD_PLUGIN_ERROR, dataErrorInfo[DM_E_LOAD_PLUGIN_ERROR]);
		return;
	}
	int m_currentVersion= 0;
	_getCurrentVersionInfo(m_currentVersion);
	if(0 == m_currentVersion)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::ger version number error \n");
		emit sigUserDataHandleResult(DM_E_GET_VERSION_ERROR, dataErrorInfo[DM_E_GET_VERSION_ERROR]);
		return;
	}
	if(_saveVersionInfoToDatabase(m_currentVersion)<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::saveVersionToDatabase error \n");
		emit sigUserDataHandleResult(DM_E_DATA_BASE_ERROR, dataErrorInfo[DM_E_DATA_BASE_ERROR]);
		return;
	}
	for(int i = 0;i<m_List.size();i++)
	{
		PluginDataC *m_pGetObject = NULL;
		void  (*m_pReleaseObject)(void);
		void *dl_fp = _load(m_List.at(i).toUtf8().data(),m_pGetObject,m_pReleaseObject);

		if(dl_fp != NULL)
		{
			int result = m_pGetObject->backupUserData(sAddress);
			if(result < 0 )
			{
				(*m_pReleaseObject)();
				dlclose(dl_fp);
				dataErrorInfo[DM_E_BACKUP_ERROR] += "[%s]";
				dataErrorInfo[DM_E_BACKUP_ERROR] += dataErrorInfo[DM_E_BACKUP_ERROR].number(result);
				emit sigUserDataHandleResult(DM_E_BACKUP_ERROR, dataErrorInfo[DM_E_BACKUP_ERROR]);
				return;
			}
			(*m_pReleaseObject)();	
			dlclose(dl_fp);
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "ERROR::load pluginName[%s]plugin error(%s)\n",m_List.at(i).toUtf8().data(),dlerror());
			(*m_pReleaseObject)();
			dlclose(dl_fp);
			emit sigUserDataHandleResult(DM_E_LOAD_PLUGIN_ERROR, dataErrorInfo[DM_E_LOAD_PLUGIN_ERROR]);
			return ;
		}
	
	}

	if(_isRepairUserSubarea())
	{
		emit sigUserDataHandleResult(DM_E_REPAIR_SUBAREA_START, dataErrorInfo[DM_E_REPAIR_SUBAREA_START]);
		
		if(0!= system(REPAIR_SUBAREA))
		{
			emit sigUserDataHandleResult(DM_E_REPAIR_SUBAREA_ERROR, dataErrorInfo[DM_E_REPAIR_SUBAREA_ERROR]);
			return;
		}
		else
			emit sigUserDataHandleResult(DM_E_REPAIR_SUBAREA_OK, dataErrorInfo[DM_E_REPAIR_SUBAREA_OK]);
	}
	else
	{
		if(0!= system(REPAIR_DATA))
		{
			emit sigUserDataHandleResult(DM_E_REPAIR_DATA_ERROR, dataErrorInfo[DM_E_REPAIR_DATA_ERROR]);
			return;
		}
	}
	int m_backVersion = 0;
	char path_db[200] = "";
	sprintf(path_db,"%s%s",sAddress.toAscii().data(),SERVICE_BACKNAME);
	if(_readVersionInfoFromDatabase(path_db,m_backVersion)<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::readVersionInfoFromDatabase error \n");
		emit sigUserDataHandleResult(DM_E_DATA_BASE_ERROR, dataErrorInfo[DM_E_DATA_BASE_ERROR]);
		return;
	}
	if(_checkRervertUserData(m_currentVersion,m_backVersion)<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::can not revertUserData \n");
		emit sigUserDataHandleResult(DM_E_CHECK_VERSION_ERROR, dataErrorInfo[DM_E_CHECK_VERSION_ERROR]);
		return;
	}
	for(int i = 0;i<m_List.size();i++)
	{
		PluginDataC *m_pGetObject = NULL;
		void  (*m_pReleaseObject)(void);
		void *dl_fp = _load(m_List.at(i).toUtf8().data(),m_pGetObject,m_pReleaseObject);

		if(dl_fp != NULL)
		{		
			int result  = m_pGetObject->revertUserData(sAddress);

			if(result < 0 )
			{
				(*m_pReleaseObject)();
				dlclose(dl_fp);
				dataErrorInfo[DM_E_BACKUP_ERROR] += "[%s]";
				dataErrorInfo[DM_E_BACKUP_ERROR] += dataErrorInfo[DM_E_BACKUP_ERROR].number(result);
				emit sigUserDataHandleResult(DM_E_REVER_ERROR, dataErrorInfo[DM_E_REVER_ERROR]);
				return;
			}
			(*m_pReleaseObject)();
		
			dlclose(dl_fp);
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "ERROR::load pluginName[%s]plugin error(%s)\n",m_List.at(i).toUtf8().data(),dlerror());
			(*m_pReleaseObject)();
			dlclose(dl_fp);
			emit sigUserDataHandleResult(DM_E_LOAD_PLUGIN_ERROR, dataErrorInfo[DM_E_LOAD_PLUGIN_ERROR]);
			return ;
		}
		int progress = ((float) (i+1)/(float) m_List.size())* 100;
		emit sigRepairUserDataProgress(progress);
		
	}
	if(_saveVersionInfoToDatabase(m_currentVersion)<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR::saveVersionToDatabase error \n");
		emit sigUserDataHandleResult(DM_E_DATA_BASE_ERROR, dataErrorInfo[DM_E_DATA_BASE_ERROR]);
		return;
	}
	emit sigUserDataHandleResult(status, dataErrorInfo[status]);
	
}
}
