#include "bsUcsInfoGather.h"
#include <sqlite3.h>
#include <QXmlStreamReader>
#include <curl.h>
#include <QDir>
#include <QDebug>
#include "common_config.h"
#include "bsClient.h"
#include "bsCommunalLock.h"


static int _ReadUcsInfoFromDB(UcsInfoC &info)
{
	int ret = 0;
	int retvalue = -1;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pSqlSel[200];
	char chSqlDBPath[128] = {0};
	sprintf( pSqlSel ,"%s", "select * from UcsInfo");
	sprintf(chSqlDBPath, "%s", INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY);
	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_open error : %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_busy_timeout(pDB, 1000);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt,( const char**)&pError );
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error : %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_step(pStmt);
	if( ret != SQLITE_ROW )
	{
		//UcsInfo is empty, because we first use UcsInfo
		IBD_PRINTF(DEBUG_INFOMATION, "UcsInfo is void\n");

		info.sUmHost = "";
		info.sUmId = "";
		info.sUmPort = "";
		info.sUmUserJid = "";
		info.bAutoReportLocation = false;
		info.iAutoReportInterval = 300;
	}
	else
	{
		info.sUmHost = (char*)sqlite3_column_text(pStmt, 0);
		info.sUmId = (char*)sqlite3_column_text(pStmt, 1);
		info.sUmPort = (char*)sqlite3_column_text(pStmt, 2);
		info.sUmUserJid = (char*)sqlite3_column_text(pStmt, 3);
		info.bAutoReportLocation = sqlite3_column_int(pStmt, 4);
		info.iAutoReportInterval = sqlite3_column_int(pStmt, 5);
	}
	
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	return 0;
}

static int _SaveUcsInfoToDB(const UcsInfoC &info)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = "delete from UcsInfo";
	char pInsertTablesSel[1024] = {0};
	
	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s\n", sqlite3_errmsg(pDB));
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
	
	sprintf(pInsertTablesSel, "insert into UcsInfo values('%s', '%s', '%s', '%s', '%d', '%d')", \
		info.sUmHost.toAscii().data(), info.sUmId.toAscii().data(), info.sUmPort.toAscii().data(), \
		info.sUmUserJid.toAscii().data(), info.bAutoReportLocation, info.iAutoReportInterval);

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

UcsInfoC::UcsInfoC()
	: sUmId(""),
	  sUmHost(""),
	  sUmPort(""),
	  sUmUserJid(""),
	  bAutoReportLocation(false),
	  iAutoReportInterval(300)
{
	
}


BsUcsInfoGatherC *BsUcsInfoGatherC::thisObject = NULL;
BsUcsInfoGatherC *BsUcsInfoGatherC::object()
{
	BsUcsInfoGatherC *pTmp = NULL;

	bsManagerObjectLock();
	
	if( !thisObject )
	{
		thisObject = new BsUcsInfoGatherC();
	}

	pTmp = thisObject;
	
	bsManagerObjectUnLock();
	
	return pTmp;
}

void BsUcsInfoGatherC::releaseObject()
{
	bsManagerObjectLock();

	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}

	bsManagerObjectUnLock();
}

BsUcsInfoGatherC *BsUcsInfoGatherC::getObject()
{
	BsUcsInfoGatherC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}
	
BsUcsInfoGatherC::BsUcsInfoGatherC()
	: m_bExit(false),
	  m_bStartAgain(false)
{
	connect(this, SIGNAL(finished()), this, SLOT(slotFinished()), Qt::QueuedConnection);
}

BsUcsInfoGatherC::~BsUcsInfoGatherC()
{
	if( isRunning() )
	{
		cancelOperation();
		
		wait();
	}
}

void	BsUcsInfoGatherC::cancelOperation()
{
	m_mutexForThreadSafe.lock();

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
	
int BsUcsInfoGatherC::gatherUcsInfo()
{
	m_mutexForThreadSafe.lock();

	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}
	else
	{
		m_bExit = true;
		m_bStartAgain = true;
	}

	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}

bool BsUcsInfoGatherC::isFinish()
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

int BsUcsInfoGatherC::getUcsInfo(UcsInfoC& info)
{
	int retval = BS_E_OK;

	m_mutexForThreadSafe.lock();

	if( _ReadUcsInfoFromDB(info) < 0 )
		retval =BS_UCS_INFO_GATHER_E_NOT_INFO;
	
	m_mutexForThreadSafe.unlock();

	return retval;
}

void BsUcsInfoGatherC::slotFinished()
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

static int m_bExit_func(bool *m_bExit, double DlT, double DlN, double UlT, double UlN)
{	
	if ( *m_bExit == true)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "in  m_bExit_func  m_bExit = %d\n",m_bExit);
		return -1;
	}	
	return 0;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	DataBlock* pDataBlock =(DataBlock*)stream;

      //int written = fwrite(ptr, size, nmemb, (FILE*)stream);

	//printf(" size=%d nmemb=%d  pDataBlock->len=%d\n",size,nmemb,pDataBlock->len);
	//printf("contents=%s\n",(char*)ptr);

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

int BsUcsInfoGatherC::_downloadUcsInfo()
{
	int retval = -1;	
	char ucsInfoUrl[URL_MAX_LENGTH];
	char cookie[512];
	char ucsApiHost[URL_MAX_LENGTH];
	char tuid[BK_TUID_LENGTH_MAX+1];
	char uuid[BK_UUID_LENGTH_MAX+1];
	char password[BK_PASSWORD_LENGTH_MAX+1];
	char accessToken[BK_TOKEN_LENGTH_MAX+1];
	bool bLogin;
	bool bSavePassword;
	UcsInfoC		ucsInfo;
	
	if( BsBaseC::getLoginResult(tuid, uuid, password, accessToken, bLogin, bSavePassword) < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"BsBaseC::getLoginResult fail\n")
		return -1;
	}

	if( !bLogin )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "user not login to back server!\n")
		return -1;
	}

	if( ( BS_E_OK != BsBaseC::getUcsApiHost(ucsApiHost)) < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"BsBaseC::getUcsApiHost fail\n")
		return -1;
	}
	
	sprintf(ucsInfoUrl, "http://%s/api/1.0/user/%s/inf/setting/", ucsApiHost, uuid);
	sprintf(cookie, "customer_key=%s;access_token=%s", tuid, accessToken);

	DataBlock dataBlock;
	CURL *curl = curl_easy_init();
	CURLcode res;
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, ucsInfoUrl);
		
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	   	
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);	
	   	
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &m_bExit);
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)&dataBlock);
     		
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
							retval = 0;
					}
					else if( xml.attributes().value("key") == QString("um_id") )
					{
						ucsInfo.sUmId = xml.readElementText();
					}
					else if( xml.attributes().value("key") == QString("um_host") )
					{
						ucsInfo.sUmHost = xml.readElementText();
					}
					else if( xml.attributes().value("key") == QString("um_port") )
					{
						ucsInfo.sUmPort = xml.readElementText();
					}
					else if( xml.attributes().value("key") == QString("um_user_jid") )
					{
						ucsInfo.sUmUserJid = xml.readElementText();
					}
					else if( xml.attributes().value("key") == QString("auto_report_switch") )
					{
						QString tmp = xml.readElementText();
						if( xml.readElementText().compare("1") )
							ucsInfo.bAutoReportLocation = true;
						else
							ucsInfo.bAutoReportLocation = false;
					}
					else if( xml.attributes().value("key") == QString("auto_report_interval") )
					{
						QString tmp = xml.readElementText();
						ucsInfo.iAutoReportInterval = tmp.toInt();
					}
					else
					{
						
					}
				}
			}
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "Error: curl_easy_perform error\n");			
		}
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error: curl_easy_init error\n");
	}

	curl_easy_cleanup(curl);

	if( retval == 0 )	
	{
		m_mutexForThreadSafe.lock();

		if( m_bExit )
		{
			m_mutexForThreadSafe.unlock();
			return -1;
		}

		UcsInfoC tmpUcsInfo;
		_ReadUcsInfoFromDB(tmpUcsInfo);
		if( tmpUcsInfo.sUmHost.compare(ucsInfo.sUmHost) != 0 ||
			tmpUcsInfo.sUmId.compare(ucsInfo.sUmId) != 0 ||
			tmpUcsInfo.sUmPort.compare(ucsInfo.sUmPort) != 0 ||
			tmpUcsInfo.sUmUserJid.compare(ucsInfo.sUmUserJid) != 0 ||
			tmpUcsInfo.bAutoReportLocation != ucsInfo.bAutoReportLocation ||
			tmpUcsInfo.iAutoReportInterval != ucsInfo.iAutoReportInterval )
		{
			BsClientC *pClient = BsClientC::getObject();
			if( pClient )
			{
				pClient->internalSendCommonCommand(BM_COMMAND_UCSINFO);
			}
			emit sigGetUcsInfo(ucsInfo);
			_SaveUcsInfoToDB(ucsInfo);
		}
		
		m_mutexForThreadSafe.unlock();
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ucsInfoUrl[%s]\n", ucsInfoUrl);
		IBD_PRINTF(DEBUG_INFOMATION, "cookie[%s]\n", cookie);
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

	return retval;
}

void BsUcsInfoGatherC::run()
{
	bool bSleep = false;
	int retval = -1;

	while(1)
	{
		if( m_bExit )
			break;

		if( bSleep )
		{
			m_mutexWaitCondition.lock();
			m_condExit.wait(&m_mutexWaitCondition, 10000);
			m_mutexWaitCondition.unlock();
		}

		if( m_bExit )
			break;

		//start to download
		if( (retval=_downloadUcsInfo())<0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "Error: _downloadUcsInfo fail\n");
			if( BS_E_URL_HTTP_ERROR == retval )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "because http error, so try it again!\n")
				bSleep = true;
				continue;
			}
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "get ucs info success!\n");
		}

		break;
	}
}

void BsUcsInfoGatherC::internalNotify(BsManager_Command_Id id)
{
	if( BM_COMMAND_UCSINFO == id )
	{
		UcsInfoC info;
		if( BS_E_OK == getUcsInfo(info) )
		{
			emit sigGetUcsInfo(info);
		}
	}
}

