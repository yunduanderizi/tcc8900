#include "bsPeccancyHint.h"
#include <QXmlStreamReader>
#include <curl.h>
#include <QDir>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QMutex>
#include <sys/ioctl.h>
#include <QDebug>
#include "bsCommunalLock.h"
#include "common_config.h"


 
//#define INCAR_COMMON_MICRO_TMP_DIRECTORY "" 


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
	QIODevice *pFile = (QIODevice *)stream;

	qint64 writeen = pFile->write((char *)ptr, (qint64)(size*nmemb));
	if(writeen < nmemb*size)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error:write_data fail\n");
		return -1;
	}
	return writeen;
}
BsPeccancyHintC  *BsPeccancyHintC::thisObject = NULL;
BsPeccancyHintC  *BsPeccancyHintC::object()
{
	BsPeccancyHintC *Tmp;
	bsManagerObjectLock();
	if( !thisObject )
	{
		thisObject = new BsPeccancyHintC ();
	}
	Tmp = thisObject;
	bsManagerObjectUnLock();
	return Tmp;
	
}

void BsPeccancyHintC::releaseObject()
{
	bsManagerObjectLock();
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();
}

BsPeccancyHintC *BsPeccancyHintC::getObject()
{
	BsPeccancyHintC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}
BsPeccancyHintC ::BsPeccancyHintC ()
	:m_bExit(false)
{
	
}

BsPeccancyHintC ::~BsPeccancyHintC ()
{
	if( isRunning())
	{
		cancelOperation();
		
		wait();
	}
}

int BsPeccancyHintC::startGetPeccancyHint(bool bAllHint)
{
	if( isRunning())
	{
		return BS_E_BUSY;
	}

	m_mutexForThreadSafe.lock();

	m_bAllHint = bAllHint;
 
	m_bExit = false;
	start();
	
	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}

void	BsPeccancyHintC ::cancelOperation()
{
	m_mutexForThreadSafe.lock();
	
	if( isRunning())
	{
		BsBaseC::stopHttp();
		m_bExit = true;
	}
	
	m_mutexForThreadSafe.unlock();
}

int BsPeccancyHintC::_downloadPeccancyHintXml(QFile *file, char* url)
{
	CURL *curl = curl_easy_init();
	if(curl)
	{	
	 	printf("Connecting to %s -->\n", url);
		curl_easy_setopt(curl,CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	   	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);	
	   	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
	   	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &m_bExit);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
      		curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)file);
     		
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "Error: curl error: %s\n",curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			return -1;
		}
	}
	curl_easy_cleanup(curl);
	return 0;
}


int BsPeccancyHintC::_parseXmlPeccancyHint(QFile *file, BsPeccancyHintInfoList *p_PeccancyHintlist)
{
	int flag = -1;
	struct InFunction
		{
		 	int   rettu_id;
			int	  retpec_date;
			int   retpec_address;
			int   retpec_action;
			int	  retpec_dispose;
			int	  retcreate_date;
			int	  retis_return;
			int   reterror;
			void resetvalue()
			{
				rettu_id = -1;
				retpec_date = -1;
				retpec_address = -1;
				retpec_action = -1;
				retpec_dispose = -1;
				retcreate_date = -1;
				retis_return = -1;
				reterror = -1;
			}
		};
	 struct InFunction   revalue;
	revalue.resetvalue();
	BsPeccancyHintInfo  pb_PecHint;
	QIODevice *device = (QIODevice *)file;
	device->seek(0);
	QXmlStreamReader xml;
	xml.setDevice(device);

	while(!xml.atEnd())
	{
		xml.readNext();
		if(xml.isStartElement() && xml.name() == "item")
		{
			if(xml.attributes().value("key") == QString("tu_id"))
		    {
		    	pb_PecHint.clear();
				pb_PecHint.tu_id = xml.readElementText().toUtf8().data();
				revalue.rettu_id = 0;
		    }				
			else if ( xml.attributes().value("key") == QString("pec_date"))
			{
				pb_PecHint.pec_date = xml.readElementText().toUtf8().data();
				revalue.retpec_date = 0;
			}
			else if ( xml.attributes().value("key") == QString("pec_address"))
			{
				pb_PecHint.pec_address = xml.readElementText().toUtf8().data();
				revalue.retpec_address  = 0;
			}
			else if ( xml.attributes().value("key") == QString("pec_action"))
			{
				pb_PecHint.pec_action = xml.readElementText().toUtf8().data();
				revalue.retpec_action = 0;
			}
			else if ( xml.attributes().value("key") == QString("pec_dispose"))
			{
				pb_PecHint.pec_dispose = xml.readElementText().toUtf8().data();
				revalue.retpec_dispose = 0;
			}
			else if ( xml.attributes().value("key") == QString("create_date"))
			{
				pb_PecHint.create_date = xml.readElementText().toUtf8().data();
				revalue.retcreate_date = 0;
			}
			else if ( xml.attributes().value("key") == QString("is_return"))
			{
				pb_PecHint.is_return = xml.readElementText().toUtf8().data();
				revalue.retis_return = 0;
			}
	    }
	    else if(xml.isEndElement()&& xml.name() == "item")
	    {
	        if(revalue.rettu_id == 0 && revalue.retpec_date == 0 && revalue.retpec_address == 0 && revalue.retpec_action == 0 && revalue.retpec_dispose == 0 && revalue.retcreate_date == 0 && revalue.retis_return == 0 )
	     	{
	     		 p_PeccancyHintlist->append(pb_PecHint);
	     	}
			revalue.resetvalue();
			flag = 0;
	    }
	if(xml.isStartElement() && xml.name() == "item")
	{
		if(xml.attributes().value("key") == QString("error"))
		{
			if(xml.readElementText() != "")
			{
				flag =  BS_E_URL_HTTP_ERROR;
			}
 		}
	}
   }

	return flag;
}

void BsPeccancyHintC ::run()
{
	int retval = -1;
	QDir peccancyHintPath;
	BsPeccancyHintInfoList   pb_peccancyhintlist; 
	char peccancyDir[512];

	char peccancyHintUrl[URL_MAX_LENGTH];
	char apHost[URL_MAX_LENGTH];
	retval = BsBaseC::getApApiHost(Ap_PeccancyHint, apHost);
	if( BS_E_OK != retval )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"getApApiInfo fail\n")
		emit sigetPeccancyHintResult(retval, pb_peccancyhintlist);
		return;
	}

	char tuid[BK_TUID_LENGTH_MAX+1];
	char uuid[BK_UUID_LENGTH_MAX+1];
	char password[BK_PASSWORD_LENGTH_MAX+1];
	char accessToken[BK_TOKEN_LENGTH_MAX+1];
	bool bLogin;
	bool bSavePassword;
	retval = BsBaseC::getLoginResult(tuid, uuid, password, accessToken, bLogin, bSavePassword);
	if( retval < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"getLoginResult fail\n")
		emit sigetPeccancyHintResult(retval, pb_peccancyhintlist);
		return;
	}

	if( 0 == strcmp(tuid, "") )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"tuid is void\n")
		emit sigetPeccancyHintResult(BS_E_GET_TUID_ERROR, pb_peccancyhintlist);
		return;
	}

	if( 0 == strcmp(uuid, "") )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"uuid is void\n")
		emit sigetPeccancyHintResult(BS_E_GET_UUID_ERROR, pb_peccancyhintlist);
		return;
	}

        sprintf(peccancyHintUrl, "http://%s/ap007/api/1.0/interface/index?uuid=%s&tu_id=%s&is_all=%s", apHost, uuid, tuid, m_bAllHint?"1":"0");
	
	strcpy(peccancyDir, INCAR_COMMON_MICRO_TMP_DIRECTORY);
	strcat(peccancyDir, "peccancyHint");
		
	if(false == peccancyHintPath.mkpath(peccancyDir))
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error:mkdir path fail [%s]\n", peccancyDir);
		emit sigetPeccancyHintResult(BS_PECCANCY_HINT_E_MKDIR_FAILURE,pb_peccancyhintlist);
		return;
	}

	strcat(peccancyDir, "/peccancyhint.xml");
	QFile peccancyHintFile(peccancyDir); 
	if (!peccancyHintFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
	{
		emit sigetPeccancyHintResult(BS_E_URL_FILE_ERROR,pb_peccancyhintlist);
        	return;
	}

	if(_downloadPeccancyHintXml(&peccancyHintFile, peccancyHintUrl) < 0)
	{
		retval = BS_E_URL_HTTP_ERROR;
	}
	else
	{
		if(_parseXmlPeccancyHint(&peccancyHintFile,&pb_peccancyhintlist) < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION,"login ucs.chinatsp.com fail, parse xml errorl\n");
			retval = _parseXmlPeccancyHint(&peccancyHintFile,&pb_peccancyhintlist);
		}
		else
			retval = 0;	
	}  
	peccancyHintFile.close();

	emit sigetPeccancyHintResult(retval,pb_peccancyhintlist);
}

