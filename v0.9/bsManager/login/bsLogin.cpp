#include "bsLogin.h"
#include "bsAuthenticationReport.h"
#include "bsUcsInfoGather.h"
#include "bsGetTelephone.h"
#include <QXmlStreamReader>
#include <curl.h>
#include <QMutex>

#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "bsCommunalLock.h"
using namespace std;

BsLoginC  *BsLoginC::thisObject = NULL;
BsLoginC  *BsLoginC::object()
{
	BsLoginC *pTmp = NULL;

	bsManagerObjectLock();
	
	if( !thisObject )
	{
		thisObject = new BsLoginC();
	}

	pTmp = thisObject;
	
	bsManagerObjectUnLock();
	
	return pTmp;
}

void BsLoginC::releaseObject()
{
	bsManagerObjectLock();
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();
	
}
BsLoginC *BsLoginC::getObject()
{
	BsLoginC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}
BsLoginC::BsLoginC ()
	:m_bExit(false)
{
	BsAuthenticationReportC::object();
	BsUcsInfoGatherC::object();
	BsGetTelephoneC::object();
	connect(this, SIGNAL(finished()), this, SLOT(slotFinished()), Qt::QueuedConnection);
}

BsLoginC::~BsLoginC()
{
	if( isRunning() )
	{
		cancelOperation();
		wait();
	}
}

void BsLoginC::slotFinished()
{
	m_mutexForThreadSafe.lock();

	if( (!m_list.isEmpty()) && (!isRunning()) )
	{
		m_bExit = false;
		start();
	}
		
	m_mutexForThreadSafe.unlock();
}

int BsLoginC::getTuid(char *tuid)
{
	if( NULL == tuid )
	{
		return BS_E_ARG_INVALID;
	}

	m_mutexForThreadSafe.lock();

	if( BsBaseC::getTuid(tuid) < 0 )
	{
		m_mutexForThreadSafe.unlock();
		return BS_E_GET_TUID_ERROR;
	}

	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}

bool BsLoginC::isLogin()
{
	m_mutexForThreadSafe.lock();
	
	char tuid[BK_TUID_LENGTH_MAX+1];
	char uuid[BK_UUID_LENGTH_MAX+1];
	char password[BK_PASSWORD_LENGTH_MAX+1];
	char accessToken[BK_TOKEN_LENGTH_MAX+1];
	bool bLogin;
	bool bSavePassword;
		
	if( BsBaseC::getLoginResult(tuid, uuid, password, accessToken, bLogin, bSavePassword) < 0 )
	{
		m_mutexForThreadSafe.unlock();
		return false;
	}

	m_mutexForThreadSafe.unlock();
	
	return bLogin;
}

int BsLoginC::getLoginResult(char *tuid, char *uuid, char *password, char *accessToken, bool &bLogin, bool &bSavePassword)
{
	if( NULL == tuid || NULL==uuid || NULL==password || NULL==accessToken )
	{
		return BS_E_ARG_INVALID;
	}

	if( BsBaseC::getLoginResult(tuid, uuid, password, accessToken, bLogin, bSavePassword) < 0 )
	{
		return BS_E_DATABASE_ERROR;
	}
	
	return BS_E_OK;	
}

int BsLoginC::login(const char *uuid, const char *password, bool bSavePassword)
{
	if( NULL==uuid || NULL==password )
	{
		return BS_E_ARG_INVALID;
	}

	m_mutexForThreadSafe.lock();
	
	DownloadTask myTask;
	strncpy(myTask.uuid, uuid, BK_UUID_LENGTH_MAX);
	strncpy(myTask.password, password, BK_PASSWORD_LENGTH_MAX);
	myTask.bSavePassword = bSavePassword;
	myTask.mode = 0;
	m_list.append(myTask);

	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}

	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}

int BsLoginC::logout()
{
	m_mutexForThreadSafe.lock();

	DownloadTask myTask;
	myTask.mode = 1;
	m_list.append(myTask);
	
	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}

	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}

static int m_bExit_func(bool *m_bExit, double DlT, double DlN, double UlT, double UlN)
{		

	if ( (*m_bExit) == true)
	{

		printf("in  m_bExit_func  m_bExit = %d\n",*m_bExit);

		return -1;

	}	

	return 0;

}
void	BsLoginC::cancelOperation()
{
	m_mutexForThreadSafe.lock();

	m_list.clear();
	
	if( isRunning() )
	{
		BsBaseC::stopHttp();
		m_bExit = true;

		m_mutexWaitCondition.lock();
		m_condExit.wakeAll();
		m_mutexWaitCondition.unlock();
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


static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	DataBlock* pDataBlock =(DataBlock*)stream;

        //int written = fwrite(ptr, size, nmemb, (FILE*)stream);

	printf(" size=%d nmemb=%d  pDataBlock->len=%d\n",size,nmemb,pDataBlock->len);
	printf("contents=%s\n",(char*)ptr);

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

static int _parseLoginXml(const char *curlbuf, char* accessToken)
{
	int retval_ok = -1;
	int retval_access_token = -1;
	
	QXmlStreamReader xml;
	xml.addData(curlbuf);

	while(!xml.atEnd())
	{
		xml.readNext();
		if( xml.isStartElement() && xml.name()=="item" )
		{
			if( xml.attributes().value("key") == QString("resp_status") )
			{
				if( xml.readElementText() == QString("OK") )
					retval_ok = 0;
			}
			else if( xml.attributes().value("key") == QString("access_token") )
			{
				retval_access_token = 0;
				strcpy(accessToken, xml.readElementText().toAscii().data());
			}
		}
	}

/*
	if(xml.error())
	{
		//xml syntax error
		return -1;
	}
*/

	if( retval_ok==0 &&  retval_access_token==0 )
		return 0;
	else
		return -1;
}

int BsLoginC::_login(const char *uuid, const char *password, bool bSavePassword)
{
	int retval = BS_E_OK;
	
	char accessToken[BK_TOKEN_LENGTH_MAX+1];
	char tuid[BK_TUID_LENGTH_MAX+1];

	char pHost[URL_MAX_LENGTH];

	retval = BsBaseC::getUcsApiHost(pHost, uuid);
	if( retval < 0 )
		return retval;
	
	if( BsBaseC::getTuid(tuid) < 0 )
		return BS_E_GET_TUID_ERROR;
	
	CURL *curl = curl_easy_init();
	if(curl)
	{
		char address[URL_MAX_LENGTH];
		sprintf(address,"http://%s/api/1.0/tu/%s/login/",pHost, tuid);
		
		curl_easy_setopt(curl,CURLOPT_URL,(char *)address);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);

		string post_request = "uuid=";
		post_request += uuid;
		post_request += "&password=";
		post_request += password;

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)post_request.c_str());
		
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);	

   		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);

   		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA,  &m_bExit);
		DataBlock dataBlock;
		
        	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
        	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&dataBlock);


		CURLcode res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "Error: curl error: %s\n", curl_easy_strerror(res));
			retval = BS_E_URL_HTTP_ERROR;
		}
		else
		{
			if( _parseLoginXml(dataBlock.buffer, accessToken) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "Error: login ucs.chinatsp.com fail, parse xml errorl xml[%s]", dataBlock.buffer);
				retval = BS_E_URL_XML_PARSE_ERROR;
			}
			else
			{
				saveLoginResult(tuid, uuid, password, accessToken, true, bSavePassword);
				retval = BS_E_OK;
			}
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	return retval;
}

static int parseLogoutXml(const char *curlbuf)
{
	int retval_ok = -1;
	
	QXmlStreamReader xml;
	xml.addData(curlbuf);

	while(!xml.atEnd())
	{
		xml.readNext();
		if( xml.isStartElement() && xml.name()=="item" )
		{
			if( xml.attributes().value("key") == QString("resp_status") )
			{
				if( xml.readElementText() == QString("OK") )
					retval_ok = 0;
			}
		}
	}

/*
	if(xml.error())
	{
		//xml syntax error
		return -1;
	}
*/

	if( retval_ok==0 )
	{
		return 0;
	}
	else
		return -1;
}

int BsLoginC::_logout()
{
	int retval = BS_E_OK;

	char tuid[BK_TUID_LENGTH_MAX+1];
	char uuid[BK_UUID_LENGTH_MAX+1];
	char password[BK_PASSWORD_LENGTH_MAX+1];
	char accessToken[BK_TOKEN_LENGTH_MAX+1];
	bool bLogin;
	bool bSavePassword;
		
	if( BsBaseC::getLoginResult(tuid, uuid, password, accessToken, bLogin, bSavePassword) < 0 )
	{
		return BS_E_DATABASE_ERROR;
	}

	if( !bLogin )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "no need logout because not have logined");
		return BS_E_OK;
	}

	char pHost[URL_MAX_LENGTH];

	retval = BsBaseC::getUcsApiHost(pHost, uuid);
	if( retval < 0 )
		return retval;
	
	CURL *curl = curl_easy_init();
	if(curl)
	{
		string url = "http://";
		url += pHost;
		url += "/api/1.0/tu/";
		url += tuid;
		url += "/login/";

		curl_easy_setopt(curl,CURLOPT_URL,(char *)url.c_str());
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);

		string cookie = "access_token=";
		cookie += accessToken;

		curl_easy_setopt(curl, CURLOPT_COOKIE, (char *)cookie.c_str());
	       curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

		DataBlock dataBlock;
		
        	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
        	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&dataBlock);

		CURLcode res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			printf("Error: curl error: %s\n", curl_easy_strerror(res));
			retval = -1;
		}
		else
		{
			if( parseLogoutXml(dataBlock.buffer) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "Error: logout ucs.chinatsp.com fail, parse xml errorl xml[%s]\n", dataBlock.buffer);
				retval = BS_E_URL_XML_PARSE_ERROR;
			}
			else
			{
				strcpy(accessToken, "");
				saveLoginResult(tuid, uuid, password, accessToken, false, bSavePassword);
				retval = BS_E_OK;
			}
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	return retval;
}

void BsLoginC::_refreshRelatedInfo(bool bForLogin)
{
	bool bFinishForAuthentication_pre = BsAuthenticationReportC::object()->isFinish();
	bool bFinishForUcs_pre = BsUcsInfoGatherC::object()->isFinish();
	bool bFinishForTelephone_pre = BsGetTelephoneC::object()->isFinish();

	BsAuthenticationReportC::object()->refreshAuthenticationInfo(true);
	BsUcsInfoGatherC::object()->gatherUcsInfo();
	if( bForLogin )
	{
		BsGetTelephoneC::object()->startGettingTelephone();
	}
	else
	{
		BsGetTelephoneC::object()->emptyTelephone();
	}

	m_mutexWaitCondition.lock();
	m_condExit.wait(&m_mutexWaitCondition, 3000);
	m_mutexWaitCondition.unlock();
			
	int sleep_count = 0;//sleep 3 seconds per
	bool bFinishForAuthentication = false;
	bool bFinishForUcs = false;
	bool bFinishForTelephone = false;
	while( sleep_count < 10 )
	{
		if( m_bExit )
		{
			if( bFinishForAuthentication_pre )
			{
				BsAuthenticationReportC::object()->cancelOperation();
			}

			if( bFinishForUcs_pre )
			{
				BsUcsInfoGatherC::object()->cancelOperation();
			}

			if( bFinishForTelephone_pre )
			{
				BsGetTelephoneC::object()->cancelOperation();
			}

			break;
		}

		if( !bFinishForAuthentication )
		{
			if( BsAuthenticationReportC::object()->isFinish() )
			{
				bFinishForAuthentication = true;
			}
		}

		if( !bFinishForUcs )
		{
			if( BsUcsInfoGatherC::object()->isFinish() )
			{
				bFinishForUcs = true;
			}
		}

		if( !bFinishForTelephone )
		{
			if( BsGetTelephoneC::object()->isFinish() )
			{
				bFinishForTelephone = true;
			}
		}

		if( bFinishForAuthentication && bFinishForUcs && bFinishForTelephone )
		{
			break;
		}
		
		sleep_count++;

		m_mutexWaitCondition.lock();
		m_condExit.wait(&m_mutexWaitCondition, 3000);
		m_mutexWaitCondition.unlock();
	}

	if( sleep_count >= 10 )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "update user's info wait timeout after login\n");
	}

}

void BsLoginC::run()
{
	int retval = BS_E_OK;
	DownloadTask		myTask;
	
	while(1)
	{
		if( m_bExit )
			break;
			
		m_mutexForThreadSafe.lock();
		if( !m_list.isEmpty() )
		{
			myTask = m_list.takeFirst();
		}
		else
		{
			m_mutexForThreadSafe.unlock();
			break;
		}
		m_mutexForThreadSafe.unlock();

		if( 0 == myTask.mode )
		{
			retval = _login(myTask.uuid, myTask.password, myTask.bSavePassword);
			if( 0 == retval )
			{
				_refreshRelatedInfo(true);
			}
			
			emit sigLoginResult(retval);
		}
		else if( 1 == myTask.mode )
		{
			retval = _logout();
			if( 0 == retval )
			{
				_refreshRelatedInfo(false);
			}

			emit sigLogoutResult(retval);
		}
	}

}

