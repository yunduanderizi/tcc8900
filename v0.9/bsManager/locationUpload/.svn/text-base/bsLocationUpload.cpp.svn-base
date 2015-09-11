/**
 * @file   bsLocationUpload.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date   20110124
 * 
 * @brief  
 *      Implement for traffic notices download 
 * 
 */

#include <QMutex>
#include <QWaitCondition>
#include <QXmlStreamReader>
#include <QTextCodec>
#include <curl.h>
#include "bsLocationUpload.h"
#include "bsCommunalLock.h"
#include <unistd.h>
#include <fcntl.h>

#define GPS_INIT_VALUE		-1
#define  TUID "/nand3/tuid"




BsLocationUploadC *BsLocationUploadC::thisObject = NULL;
BsLocationUploadC *BsLocationUploadC::object()
{
	BsLocationUploadC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	if( !thisObject )
	{
		thisObject = new BsLocationUploadC();
	}

	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}

void BsLocationUploadC::releaseObject()
{
	bsManagerObjectLock();
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();

}
BsLocationUploadC *BsLocationUploadC::getObject()
{
	BsLocationUploadC*pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}
void BsLocationUploadC::naviGPSInfoReportNotifyHandler(DBusNotifyC *pNotify)
{
	if( pNotify->getIdentifier() != NOTF_ID_NAVI_GPSINFO_REPORT )
		return;

	CNaviGPSInfoReportNotifyC *NaviGPSInfoReporttmp = (CNaviGPSInfoReportNotifyC*)pNotify;

	IBD_PRINTF(DEBUG_INFOMATION, "come to naviGPSInfoReportNotifyHandler! fX[%f] fY[%f]\n", NaviGPSInfoReporttmp->fX, NaviGPSInfoReporttmp->fY);

	if( 0==NaviGPSInfoReporttmp->fX || 0==NaviGPSInfoReporttmp->fY )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "fX[%d] or fY[%d] is 0\n", NaviGPSInfoReporttmp->fX, NaviGPSInfoReporttmp->fY);
		return;
	}
	if( thisObject != NULL )
		thisObject->updateInfo(NaviGPSInfoReporttmp->fX, NaviGPSInfoReporttmp->fY, NaviGPSInfoReporttmp->iTime, NaviGPSInfoReporttmp->iOrientation, NaviGPSInfoReporttmp->iVelocity);
}

void BsLocationUploadC::naviGPSLocationNotifyHandler(DBusNotifyC *pNotify)
{
       if(pNotify->getIdentifier()!=NOTF_ID_NAVI_CARLOCATION)
	   	return;
	   
	CNaviCarLocationNotifyC *NaviCarLocationtmp = (CNaviCarLocationNotifyC*)pNotify;

	IBD_PRINTF(DEBUG_INFOMATION, "come to naviGPSLocationNotifyHandler! roadName[%s] areaName[%s] cityName[%s]\n", NaviCarLocationtmp->cRoadName, NaviCarLocationtmp->cAreaName, NaviCarLocationtmp->cityName);

	if( thisObject != NULL )
		thisObject->updateLocation(NaviCarLocationtmp->cRoadName, NaviCarLocationtmp->cAreaName, NaviCarLocationtmp->cityName);
}

void BsLocationUploadC::naviGPSInfoNotifyHandler(DBusNotifyC *pNotify)
{
       if(pNotify->getIdentifier()!=NOTF_ID_NAVI_INFO)
	   	return;
	   
	CNaviNavigationInfoNotifyC *pCurNotify=(CNaviNavigationInfoNotifyC*)pNotify;


	IBD_PRINTF(DEBUG_INFOMATION, "come to naviGPSInfoNotifyHandler! roadName[%s]\n", pCurNotify->cRoadName);

	if( thisObject != NULL )
		thisObject->clearLocation();
}

BsLocationUploadC::BsLocationUploadC()
	:m_bExit(false),
	m_bSendSuccess(false),
	m_iTimerAutoReportLocation(-1)
{
	m_dbusClient = NULL;
	m_fX= GPS_INIT_VALUE;
	m_fY = GPS_INIT_VALUE;
	m_iTime = GPS_INIT_VALUE;
	m_iOrientation = GPS_INIT_VALUE;
	m_iVelocity = GPS_INIT_VALUE;

	m_cRoadName[0] = 0;
	m_cAreaName[0] = 0;
	m_cCityName[0] = 0;
/*
	UcsInfoC		info;
	if( BS_E_OK == BsUcsInfoGatherC::object()->getUcsInfo(info) )
	{
		if( info.bAutoReportLocation )
		{
			int interval = info.iAutoReportInterval;
			if( interval < 60 )
				interval = 60;
			m_iTimerAutoReportLocation = startTimer(interval*1000);
		}
	}
	
	qRegisterMetaType<UcsInfoC>("UcsInfoC");
	
	connect(BsUcsInfoGatherC::object(), SIGNAL(sigGetUcsInfo(UcsInfoC)),this,SLOT(slotGetUcsInfo(UcsInfoC)),Qt::QueuedConnection);
	BsUcsInfoGatherC::object()->gatherUcsInfo();
*/
			//startTimer(60000);
}

BsLocationUploadC::~BsLocationUploadC()
{
	if( isRunning() )
	{
		cancelOperation();

		wait();
	}
}

void	BsLocationUploadC::cancelOperation()
{
	m_mutexForThreadSafe.lock();

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

void BsLocationUploadC::slotGetUcsInfo(UcsInfoC info)
{
/*
	printf("\n**************slotGetUcsInfo**************\n");
	printf("**************Ucs Info**************\n");
	printf("*******sUmId[%s]\n", info.sUmId.toAscii().data());
	printf("*******sUmHost[%s]\n", info.sUmHost.toAscii().data());
	printf("*******sUmPort[%s]\n", info.sUmPort.toAscii().data());
	printf("*******sUmUserJid[%s]\n", info.sUmUserJid.toAscii().data());
	printf("*******bAutoReportLocation[%d]\n", info.bAutoReportLocation);
	printf("*******iAutoReportInterval[%d]\n", info.iAutoReportInterval);
*/
	if( m_iTimerAutoReportLocation >= 0 )
	{
		killTimer(m_iTimerAutoReportLocation);
		m_iTimerAutoReportLocation = -1;
	}

	if( info.bAutoReportLocation )
	{
		int interval = info.iAutoReportInterval;
		if( interval < 60 )
			interval = 60;
		m_iTimerAutoReportLocation = startTimer(interval*1000);
	}
}

void BsLocationUploadC::timerEvent(QTimerEvent * event)
{
//	int iTimerId = event->timerId();

//	if( m_iTimerAutoReportLocation == iTimerId )
//	{
//		IBD_PRINTF(DEBUG_INFOMATION, "auto report location!\n");
		//uploadLocationInfo();
//	}
}

int BsLocationUploadC::setGpsClientDbusObject(DBusClientC *client)
{
	m_mutexForThreadSafe.lock();

	if( client == NULL )
	{
		m_mutexForThreadSafe.unlock();
		return BS_E_ARG_INVALID;
	}
	
	m_dbusClient = client;

	m_dbusClient->registerNotify(NOTF_ID_NAVI_GPSINFO_REPORT, naviGPSInfoReportNotifyHandler, CNaviGPSInfoReportNotifyC::factory);
	m_dbusClient->registerNotify(NOTF_ID_NAVI_CARLOCATION, naviGPSLocationNotifyHandler, CNaviCarLocationNotifyC::factory);
	m_dbusClient->registerNotify(NOTF_ID_NAVI_INFO, naviGPSInfoNotifyHandler, CNaviNavigationInfoNotifyC::factory);
	m_dbusClient->registerReply(REP_ID_NAVI_GPSINFO, CNaviGPSInfoRepC::factory);
	m_dbusClient->registerReply(REP_ID_NAVI_CARLOCATION, CNaviCarLocationRepC::factory);

	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}

void BsLocationUploadC::updateInfo(float fX, float fY, int iTime, int iOrientation, int iVelocity)
{
	m_mutexForThreadSafe.lock();
	m_fX= fX;
	m_fY = fY;
	m_iTime = iTime;
	m_iOrientation = iOrientation;
	m_iVelocity = iVelocity;
	m_mutexForThreadSafe.unlock();
}

void BsLocationUploadC::updateLocation(const char* pRoadName, const char* pAreaName, const char* pCityName)
{
	m_mutexForThreadSafe.lock();
	strcpy(m_cRoadName, pRoadName);
	trimStr(m_cRoadName);
	strcpy(m_cAreaName, pAreaName);
	trimStr(m_cAreaName);
	strcpy(m_cCityName, pCityName);
	trimStr(m_cCityName);
	m_mutexForThreadSafe.unlock();
}

void BsLocationUploadC::clearLocation()
{
	m_mutexForThreadSafe.lock();
	m_cRoadName[0] = 0;
	m_cAreaName[0] = 0;
	m_cCityName[0] = 0;
	m_mutexForThreadSafe.unlock();
}

int BsLocationUploadC::uploadLocationInfo()
{
	m_mutexForThreadSafe.lock();

	if( m_dbusClient == NULL )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "not gps dbus object!\n");
		m_mutexForThreadSafe.unlock();
		return BS_LOCATION_UPLOAD_E_NO_GPS_DBUS_OBJECT;
	}

	if( isRunning() )
	{
		m_mutexForThreadSafe.unlock();
		return BS_E_OK;
	}

	m_bExit = false;
	m_bSendSuccess = false;

	start();

	m_mutexForThreadSafe.unlock();
	
	return BS_E_OK;
}

int BsLocationUploadC::getLocationInfo(float &fX, float &fY, int &iTime, int &iOrientation, int &iVelocity)
{
	m_mutexForThreadSafe.lock();
	
	if( m_dbusClient == NULL )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "not gps dbus object!\n");
		m_mutexForThreadSafe.unlock();
		return BS_LOCATION_UPLOAD_E_NO_GPS_DBUS_OBJECT;
	}

	struct timeval tv;
	struct timezone tz;
	int iCurrentTime;
	gettimeofday(&tv, &tz);
	iCurrentTime = tv.tv_sec;
		
	//compare current time with m_iTime
	if(ABS(iCurrentTime, m_iTime) > 120 )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "gps info is too old!\n")
		m_mutexForThreadSafe.unlock();
		return BS_LOCATION_UPLOAD_E_GET_LOCATION_INFO_FAIL;
	}

	fX = m_fX;
	fY = m_fY;
	iTime = m_iTime;
	iOrientation = m_iOrientation;
	iVelocity = m_iVelocity;

	if( 0==fX && 0==fY )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "gps give me x=0 y=0!\n")
		m_mutexForThreadSafe.unlock();
		return BS_LOCATION_UPLOAD_E_GET_LOCATION_INFO_FAIL;
	}

	m_mutexForThreadSafe.unlock();
	return BS_E_OK;
}

int BsLocationUploadC::getLocationInfo2(float &fX, float &fY, int &iTime, int &iOrientation, int &iVelocity)
{
	m_mutexForThreadSafe.lock();
	
	if( m_dbusClient == NULL )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "not gps dbus object!\n");
		m_mutexForThreadSafe.unlock();
		return BS_LOCATION_UPLOAD_E_NO_GPS_DBUS_OBJECT;
	}

	struct timeval tv;
	struct timezone tz;
	int iCurrentTime;
	gettimeofday(&tv, &tz);
	iCurrentTime = tv.tv_sec;
		
	//compare current time with m_iTime
	if(ABS(iCurrentTime, m_iTime) > 120 )
	{
		m_mutexForThreadSafe.unlock();
		if( getGpsInfo(1000) < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "get gps info error!\n")
		}
		m_mutexForThreadSafe.lock();
	}

	if( m_iTime == GPS_INIT_VALUE )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "location info is void!\n")
		m_mutexForThreadSafe.unlock();
		return BS_LOCATION_UPLOAD_E_GET_LOCATION_INFO_FAIL;
	}

	fX = m_fX;
	fY = m_fY;
	iTime = m_iTime;
	iOrientation = m_iOrientation;
	iVelocity = m_iVelocity;

	if( 0==fX && 0==fY )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "gps give me x=0 y=0!\n")
		m_mutexForThreadSafe.unlock();
		return BS_LOCATION_UPLOAD_E_GET_LOCATION_INFO_FAIL;
	}

	m_mutexForThreadSafe.unlock();
	return BS_E_OK;
}

bool BsLocationUploadC::preSendIsSuccess()
{
	bool bSuccess = false;
	
	m_mutexForThreadSafe.lock();
	bSuccess = m_bSendSuccess;
	m_mutexForThreadSafe.unlock();

	return bSuccess;
}

int BsLocationUploadC::getGpsInfo(int iMsecond)
{
	CNaviGPSInfoReqC    pRequest;
	DBusReplyC      *pReply=NULL;

	int result = m_dbusClient->sendService(&pRequest, pReply, iMsecond);

	if(result<0 ||pReply->getIdentifier()!=REP_ID_NAVI_GPSINFO)
	{
		delete pReply;
		return -1;
	}
	else
	{
		CNaviGPSInfoRepC *pCNaviReply  = (CNaviGPSInfoRepC*)pReply;

		if( 0 == pCNaviReply->fX || 0 == pCNaviReply->fY )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "gps give me x=0 y=0!\n")
			return -1;
		}
		
		updateInfo(pCNaviReply->fX, pCNaviReply->fY, pCNaviReply->iTime, pCNaviReply->iOrientation, pCNaviReply->iVelocity);

		delete pReply;
		return 0;
	}
}

int BsLocationUploadC::getGpsLocation(int iMsecond)
{
	CNaviCarLocationReqC    pRequest;
	DBusReplyC      *pReply=NULL;

	int result = m_dbusClient->sendService(&pRequest, pReply, iMsecond);

	if(result<0 ||pReply->getIdentifier()!=REP_ID_NAVI_CARLOCATION)
	{
		delete pReply;
		return -1;
	}
	else
	{
		CNaviCarLocationRepC *pCNaviReply  = (CNaviCarLocationRepC*)pReply;

		updateLocation(pCNaviReply->cRoadName, pCNaviReply->cAreaName, pCNaviReply->cityName);

		delete pReply;
		return 0;
	}
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

int BsLocationUploadC::upload(char* uuid, char* tuid, char* token)
{
	int retval = -1;
	char url[512];
	char postFields[512];
	char cookie[512];
	char tmpbuffer[512];

	char ucsApiHost[URL_MAX_LENGTH];
	memset(ucsApiHost,0,sizeof(ucsApiHost));
//	if( (retval=BsBaseC::getUcsApiHost(ucsApiHost)) < 0 )
//		return retval;

	retval = -1;
	
	//sprintf(url, "Http://%s/api/1.0/user/%s/lbs/current/%s/", ucsApiHost, uuid, tuid);
	sprintf(url, "Http://udc.innov.21com.com/api/1.0/user/%s/lbs/current/%s/", uuid, tuid);

	m_mutexForThreadSafe.lock();

	char *pEnvContent = getenv("CONFIG_FOR_COOLD_TYPE");
	if( pEnvContent )
	{
		sprintf(postFields, "lng=%f&lat=%f&speed=%d&heading=%d&alti=0&gps_timestamp=%d&coord_type=%s", m_fX, m_fY, m_iVelocity, m_iOrientation, m_iTime, pEnvContent);
	}
	else
	{
		sprintf(postFields, "lng=%f&lat=%f&speed=%d&heading=%d&alti=0&gps_timestamp=%d", m_fX, m_fY, m_iVelocity, m_iOrientation, m_iTime);
	}	
	if( 0!=m_cRoadName[0] && 0!=m_cAreaName[0] && 0!=m_cCityName[0] )
	{
		QTextCodec *gbk = QTextCodec::codecForName("gb18030");
		QString locationMessage = gbk->toUnicode(m_cCityName);
		locationMessage += " ";
		locationMessage += gbk->toUnicode(m_cAreaName);
		locationMessage += " ";
		locationMessage += gbk->toUnicode(m_cRoadName);
		
		sprintf(tmpbuffer, "&raw_address=%s", locationMessage.toUtf8().data());
		strcat(postFields, tmpbuffer);
	}
	m_mutexForThreadSafe.unlock();
	sprintf(cookie, "customer_key=%s;access_token=%s", tuid, token);
	CURLcode res;
	CURL *curl = curl_easy_init();
	DataBlock dataBlock;

	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);

		curl_easy_setopt(curl,CURLOPT_URL, url);//
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields);
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);

        	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
        	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&dataBlock);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		if (res == CURLE_OK) 
		{
printf("****************************************************************************************\n");
printf("CURLE_OK\n");
printf("****************************************************************************************\n");
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
				}
			}
		}
		
		/* always cleanup */
		curl_easy_cleanup(curl);	
	}

	if( retval != 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "url[%s]!\n", url)
		IBD_PRINTF(DEBUG_INFOMATION, "postFields[%s]!\n", postFields)
		IBD_PRINTF(DEBUG_INFOMATION, "cookie[%s]!\n", cookie)

		if (res == CURLE_OK) 
		{
			IBD_PRINTF(DEBUG_INFOMATION, "xml[%s]!\n", dataBlock.buffer)
			return BS_E_URL_XML_PARSE_ERROR;
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "curl_easy_perform fail    res[%d]!\n", res)
			return BS_E_URL_HTTP_ERROR;
		}
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "success to send gps info    postFields[%s]!\n", postFields)
		return BS_E_OK;
	}
}

void BsLocationUploadC::run()
{
	int fd, len;
	int retval = -1;
	int loop = 0;
	char tuid[20];
	char uuid[50];
	char filebuf[50];
	char password[BK_PASSWORD_LENGTH_MAX+1];
	char accessToken[BK_TOKEN_LENGTH_MAX+1];
	bool bLogin;
	bool bSavePassword;
		
	struct timeval tv;
	struct timezone tz;
	
	memset(filebuf, 0, sizeof(filebuf));
	memset(uuid, 0, sizeof(uuid));
	memset(tuid, 0, sizeof(tuid));

	while(1)
	{
		if( m_bExit )
			break;
/*
		if( BsBaseC::getLoginResult(tuid, uuid, password, accessToken, bLogin, bSavePassword) < 0 )
		{
			break;
		}

		if( !bLogin )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "user not login to back server!\n")
			break;
		}
*/
		//when upload fail, need wait some time
		if( loop > 0 )
		{
			m_mutexWaitCondition.lock();
			m_condExit.wait(&m_mutexWaitCondition, 10000);
			m_mutexWaitCondition.unlock();
		}
	
		int iCurrentTime;
		gettimeofday(&tv, &tz);
		iCurrentTime = tv.tv_sec;
		
		//compare current time with m_iTime
		if(ABS(iCurrentTime, m_iTime) > 120 )
		{
			if( getGpsInfo(1000) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "get gps info error!\n")
			}
		}

		if( m_iTime == GPS_INIT_VALUE )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "location info is void!\n")
			loop++;
			continue;
		}

		if( 0==m_cRoadName[0] && 0==m_cAreaName[0] && 0==m_cCityName[0] )
		{
			if( getGpsLocation(1000) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "get gps location error!\n")
			}
		}
		
		IBD_PRINTF(DEBUG_INFOMATION, "roadName[%s] areaName[%s] cityName[%s]\n", m_cRoadName, m_cAreaName, m_cCityName);

		//upload gps info
	//	if( (retval=upload(uuid, tuid, accessToken)) < 0 )

		if((fd = open(TUID,O_RDONLY)) < 0){
			perror("open FIFO"); exit(1);
		}

		if((len = read(fd,filebuf,50)) > 0){
			sscanf(filebuf, "%d,%s", uuid, tuid);
			printf("uuid:%d\ntuid:%s\n", uuid, tuid);
		}else{
			printf("read tuid file fail\n");
		}

		if( (retval=upload(uuid, tuid, "123")) < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "upload gps info error!\n")
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
			m_bSendSuccess = true;
			m_mutexForThreadSafe.unlock();
		
			IBD_PRINTF(DEBUG_INFOMATION, "upload gps info success!\n")
		}

		break;
	}
}

