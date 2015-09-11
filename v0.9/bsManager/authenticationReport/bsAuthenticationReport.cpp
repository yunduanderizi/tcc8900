/**
 * @file   BsAuthenticationReportC.cpp
 * @author zhanghongbo <zhanghongbo@chinatsp.com>
 * @date   20110314
 * 
 * @brief  
 *      Implement for authentication information download 
 * 
 */

#include <QXmlStreamReader>
#include <QXmlStreamReader>
#include <curl.h>
#include "bsAuthenticationReport.h"
#include <sqlite3.h>
#include <QSettings>
#include "common_config.h"
#include "bsCommunalLock.h"

#define UCS_API_VERSION  1



static int getAuthenticationInfo(int& time, char password[8], char newPassword[8])
{

	int retval = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pQueryTablesSel = (char*)"SELECT * FROM AuthenticationInfo";//查表语句

	retval = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s!\n", sqlite3_errmsg(pDB))
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

	retval = sqlite3_step(pStmt);
	if (retval != SQLITE_ROW)
	{
		//AuthenticationInfo is empty, because we first use AuthenticationInfo
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);

		time = 0;
		strcpy(password, "");
		strcpy(newPassword, "");

		return 0;
	}

	time =sqlite3_column_int(pStmt, 0);
	strcpy(password, (char*)sqlite3_column_text(pStmt, 1));
	strcpy(newPassword, (char*)sqlite3_column_text(pStmt, 2));

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	return 0;
}

static int removeAuthenticationInfo()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = "delete from AuthenticationInfo";

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
	
	//准备执行语句
	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
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

	return 0;
}

static int updateAuthenticationInfo(int time, char password[8], char newPassword[8])
{
	if( NULL == password || NULL == newPassword )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "argument error!\n")
		return -1;
	}

	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = "delete from AuthenticationInfo";
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
	
	//准备执行语句
	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
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

	//插入记录条
	//(0!=strcmp(password, ""))?password:"", (0!=strcmp(newPassword, ""))?newPassword:""
	sprintf(pInsertTablesSel, "insert into AuthenticationInfo values('%d', '%s', '%s')", time, (0!=strcmp(password, ""))?password:"", (0!=strcmp(newPassword, ""))?newPassword:"");

	//IBD_PRINTF(DEBUG_INFOMATION, "%s!\n", pInsertTablesSel)

	//准备插入数据
	ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
       if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))

		return -1;
	}

	//执行绑定以后的语句
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

BsAuthenticationReportC *BsAuthenticationReportC::thisObject = NULL;
BsAuthenticationReportC *BsAuthenticationReportC::object()

{
	BsAuthenticationReportC *pTmp = NULL;

	bsManagerObjectLock();
		
	if( !thisObject )
	{
		thisObject = new BsAuthenticationReportC();
	}

	pTmp = thisObject;
	bsManagerObjectUnLock();


	return pTmp;
}

void BsAuthenticationReportC::releaseObject()
{
	
	bsManagerObjectLock();
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();

}

BsAuthenticationReportC *BsAuthenticationReportC::getObject()
{
	BsAuthenticationReportC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}
	
BsAuthenticationReportC::BsAuthenticationReportC()
	:m_bExit(false),
	 m_bStartAgain(false)
{
	connect(this, SIGNAL(finished()), this, SLOT(slotFinished()), Qt::QueuedConnection);
}

BsAuthenticationReportC::~BsAuthenticationReportC()
{
	if( isRunning() )
	{
		cancelOperation();

		wait();
	}
}

void	BsAuthenticationReportC::cancelOperation()
{
	m_mutexForThreadSafe.unlock();

	if( isRunning() )
	{
		BsBaseC::stopHttp();
		m_bExit = true;
		m_bStartAgain = false;

		m_mutexWaitCondition.lock();
		m_condExit.wakeAll();
		m_mutexWaitCondition.unlock();
	}

	m_mutexForThreadSafe.unlock();
}

void BsAuthenticationReportC::slotFinished()
{
	m_mutexForThreadSafe.lock();

	if( m_bStartAgain && (!isRunning()) )
	{
		m_bStartAgain = false;
		m_bExit = false;
		start();
	}
		
	m_mutexForThreadSafe.unlock();
}

static bool judgeAuthenticationInfoIsVoid(int iTime, char* pAuthenticationData, char* pNewAuthenticationData)
{
	bool bDataVoid = (iTime==0) ? true : false;
	
	int ret1 = strcmp(pAuthenticationData, "");
	int ret2 = strcmp(pNewAuthenticationData, "");
	bDataVoid = (ret1==0 && ret2==0) ? true : bDataVoid;

	return bDataVoid;
}

int BsAuthenticationReportC::refreshAuthenticationInfo(bool bForce)
{
	int retval = BS_E_OK;
	
	m_mutexForThreadSafe.lock();

	do
	{
		if( !bForce )
		{
			int iTime;
			char authenticationData[20];
			char newAuthenticationData[20];
			
			if( getAuthenticationInfo(iTime, authenticationData, newAuthenticationData) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "get authentication data error from database!\n")
				retval = BS_AUTHENTICATION_GET_DATA_ERROR;
				break;
			}
			
			if( !judgeAuthenticationInfoIsVoid(iTime, authenticationData, newAuthenticationData) )
			{
				int iCurrentTime;
				struct timeval tv;
				struct timezone tz;
				gettimeofday(&tv, &tz);
				iCurrentTime = tv.tv_sec;
			
				//compare current time with iTime
				if( ABS(iCurrentTime, iTime) < (12*60*60) )
				{
					IBD_PRINTF(DEBUG_INFOMATION, "only refresh 1 in 12 hours!\n")
					retval = BS_AUTHENTICATION_TIME_NO_OUT;
					break;
				}
			}
			else
			{
				IBD_PRINTF(DEBUG_INFOMATION, "authentication data is void in database!\n")
			}
		}
		else
		{
			removeAuthenticationInfo();
		}
		
		if( !isRunning() )
		{
			m_bExit = false;
				
			start();
		}
	}while(0);

	m_mutexForThreadSafe.unlock();
	
	return retval;
}

bool BsAuthenticationReportC::isFinish()
{
	bool bFinished = false;
	
	m_mutexForThreadSafe.lock();

	if( !isRunning() )
	{
		bFinished = true;
	}

	m_mutexForThreadSafe.unlock();

	return bFinished;
}

int BsAuthenticationReportC::startAuthenticate(char *pAuthentication)
{
	int retval = BS_E_OK;
	
	m_mutexForThreadSafe.lock();

	do
	{
		if( NULL == pAuthentication )
		{
			retval = BS_E_ARG_INVALID;
			break;
		}
		
		int iTime;
		char authenticationData[20];
		char newAuthenticationData[20];		
		if( getAuthenticationInfo(iTime, authenticationData, newAuthenticationData) < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "get authentication data error from database!\n")
			retval = BS_AUTHENTICATION_GET_DATA_ERROR;
		}
		IBD_PRINTF(DEBUG_INFOMATION,"--------->iTime[%d] authenticationData[%s] newAuthenticationData[%s]\n", iTime, authenticationData, newAuthenticationData);

		if( judgeAuthenticationInfoIsVoid(iTime, authenticationData, newAuthenticationData) )
		{
			if( !isRunning() )
			{
				m_bExit = false;
				m_bStartAgain = true;
				
				start();
			}

			retval = BS_AUTHENTICATION_NO_MATCHING;
		}
		else
		{
			if( 0 == strcmp(pAuthentication, newAuthenticationData) )
			{
				strcpy(authenticationData, newAuthenticationData);
				newAuthenticationData[0] = 0;
				if(updateAuthenticationInfo(iTime, authenticationData, newAuthenticationData)<0)
				{
					IBD_PRINTF(DEBUG_INFOMATION, "write authentication data to database error!\n")
				}
				else
				{
					IBD_PRINTF(DEBUG_INFOMATION, "authentication success, match to new authentication!\n")
				}
			}
			else if( 0 == strcmp(pAuthentication, authenticationData) )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "authentication success, match to authentication!\n")
			}
			else
			{
				IBD_PRINTF(DEBUG_INFOMATION, "authentication fail!\n")
				retval = BS_AUTHENTICATION_NO_MATCHING;
			}
		}
	}while(0);
	
	m_mutexForThreadSafe.unlock();
		
	return retval;
}

typedef struct _DataBlock
{
	_DataBlock()
	{
		buffer[0]=0;
		max=2000;
		len=0;
	}
	
	char buffer[2000];
	int  len;
	
	int  max;
}DataBlock;
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	DataBlock* pDataBlock =(DataBlock*)stream;

	int len=size*nmemb;

	if(len > pDataBlock->max-pDataBlock->len-1)
	{
		len =pDataBlock->max-pDataBlock->len-1;
	}

	if(len>0)
	{
		memcpy(pDataBlock->buffer+pDataBlock->len,ptr,len);

		pDataBlock->len +=len;
		pDataBlock->buffer[pDataBlock->len] =0;
	}

        return size*nmemb;
}
int BsAuthenticationReportC::_downloadAuthentication(const char* pTuid, char* pAuthenticationData)
{
	int retval = BS_E_OK;
	int retval_data = -1;
	char url[512];
	char postFields[512];
	char* pSoftware;


	char host_name[256];
	memset(host_name,0,sizeof(host_name));
	if( (retval=BsBaseC::getUcsApiHost(host_name)) < 0 )
		return retval;

	retval = -1;
	
 	QSettings *ConfigIni = new QSettings("/application/data/version.ini",QSettings::IniFormat,0);
	pSoftware = ConfigIni->value("Version",20).toString().toAscii().data();
	sprintf(url, "Http://%s/api/1.0/tu/%s/data/", host_name,pTuid);

	char *pEnvContent = getenv("CONFIG_FOR_COOLD_TYPE");
	if( pEnvContent )
	{
		sprintf(postFields,"software_version=%s&ucs_api_version=%d&coord_type=%s",pSoftware,UCS_API_VERSION,pEnvContent);
	}
	else
	{
		sprintf(postFields,"software_version=%s&ucs_api_version=%d",pSoftware,UCS_API_VERSION);
	}	
	
	CURLcode res;
	CURL *curl = curl_easy_init();
	DataBlock dataBlock;
	
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_easy_setopt(curl,CURLOPT_URL, url);
		printf("Connecting to %s->>>>\n",url);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields);

       	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
       	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&dataBlock);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		if (res == CURLE_OK) 
		{
			QXmlStreamReader xml;
			xml.addData(dataBlock.buffer);

			while(!xml.atEnd())
			{
				xml.readNext();
				if( xml.isStartElement() && xml.name()=="item" )
				{
					if( xml.attributes().value("key") == QString("resp_status") )
					{
						if( xml.readElementText() == QString("OK") )
						{
							retval = 0;
						}
					}
					else if( xml.attributes().value("key") == QString("token") )
					{
						retval_data = 0;
						strcpy(pAuthenticationData, xml.readElementText().toUtf8().data());
						
					}
				}
						
			}
		}

		/* always cleanup */
		curl_easy_cleanup(curl);	

	}
	
	if( retval != 0 || retval_data != 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "url[%s]!\n", url)

		if (res == CURLE_OK) 
		{
			IBD_PRINTF(DEBUG_INFOMATION, "xml[%s]!\n", dataBlock.buffer)
			return BS_E_URL_XML_PARSE_ERROR;
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "curl_easy_perform fail res[%d]!\n", res)
			return BS_E_URL_HTTP_ERROR;
		}
	}
	else
	{
		return BS_E_OK;
	}
}

void BsAuthenticationReportC::run()
{
	int retval = -1;
	int loop = 0;
	char tuid[BK_TUID_LENGTH_MAX+1];
	struct timeval tv;
	struct timezone tz;
	int iTime;
	char authenticationData[20];
	char newAuthenticationData[20];
	char tmp[20];
	
	if( BsBaseC::getTuid(tuid) < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "get tuid fail!\n")
		return;
	}

	while(1)
	{
		if( m_bExit )
			break;
		
		if( loop > 0 )
		{
			m_mutexWaitCondition.lock();
			m_condExit.wait(&m_mutexWaitCondition, (10*1000));
			m_mutexWaitCondition.unlock();
		}
		
		if( (retval=_downloadAuthentication(tuid, tmp)) < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "download authentication info error!\n")
			if( BS_E_URL_HTTP_ERROR == retval )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "because http error, so try it again!\n")
				loop++;
				continue;
			}
		}		
		else
		{
			m_mutexForThreadSafe.lock();
			
			if( getAuthenticationInfo(iTime, authenticationData, newAuthenticationData) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "get authentication data error from database!\n")
				break;
			}

			gettimeofday(&tv, &tz);
			iTime = tv.tv_sec;
					
			if( updateAuthenticationInfo(iTime, authenticationData, tmp) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "write authentication data to database error!\n")
				break;
			}

			m_mutexForThreadSafe.unlock();

			IBD_PRINTF(DEBUG_INFOMATION, "refresh authentication info success!\n")
		} 
	
		break;
	}
}

