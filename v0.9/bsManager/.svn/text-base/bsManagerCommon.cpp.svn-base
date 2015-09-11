/**
 * @file   bsManagerCommon.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110302
 * 
 * @brief  
 *      bsManager module's common base class 
 * 
 */

#include <sqlite3.h>
#include <sys/ioctl.h>
#include <fcntl.h>



#include <QXmlStreamReader>
#include <curl.h>

#include "common_config.h"
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"

#include "bsLogin.h"
#include "bsAuthenticationReport.h"
#include "bsGetTelephone.h"
#include "bsUcsInfoGather.h"
#include "bsLocationUpload.h"
#include "bsNews.h"
#include "bsPeccancyHint.h"
#include "bsStock.h"
#include "bsTrafficNotice.h"
#include "bsWeatherForecast.h"
#include "bsCarInfoMonitor.h"
#include "bsClient.h"
#include "bsServer.h"

#include "bsCommunalLock.h"


//#define DEFAULT_UCS_API_HOST		"ucs.chinatsp.com"
#define DEFAULT_UCS_API_HOST		"app.innov.21com.com"
//#define DEFAULT_UCS_API_HOST		"ap.chinatsp.com"
//#define DEFAULT_UCS_API_HOST		"ucs.sysdep.info"
#define DEFAULT_UUID				"0"
#define DEFAULT_TOKEN				"ChinatspGuest"

static QMutex	*gs_ObjectMutexForThreadSafe = NULL;
static QMutex	*gs_BsBaseCMutexForThreadSafe = NULL;







void initBsManager()
{
	if( NULL == gs_ObjectMutexForThreadSafe )
	{
		gs_ObjectMutexForThreadSafe = new QMutex(QMutex::Recursive);
	}

	if( NULL == gs_BsBaseCMutexForThreadSafe )
	{
		gs_BsBaseCMutexForThreadSafe = new QMutex(QMutex::Recursive);
	}
}

void  bsManagerObjectLock()
{
	if(NULL == gs_ObjectMutexForThreadSafe)
	{
		printf("ERROR:NO INIT BsManager\n");
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR:NO INIT BsManager\n")
		abort();
	}
	else
	{
		gs_ObjectMutexForThreadSafe->lock();
	}
	
}
void  bsManagerObjectUnLock()
{
	if(NULL == gs_ObjectMutexForThreadSafe)
	{
		printf("ERROR:NO INIT BsManager\n");
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR:NO INIT BsManager\n")
		abort();
	}
	else
	{
		gs_ObjectMutexForThreadSafe->unlock();
	}
}

void destroyBsManager()
{
	if( NULL == gs_ObjectMutexForThreadSafe )
	{
		return;
	}
	
	//notify all object to exit from thread
	BsLoginC* pBsLoginC = BsLoginC::getObject();
	if( NULL != pBsLoginC )
	{
		pBsLoginC->cancelOperation();
	}

	BsAuthenticationReportC* pBsAuthenticationReportC = BsAuthenticationReportC::getObject();
	if( NULL != pBsAuthenticationReportC )
	{
		pBsAuthenticationReportC->cancelOperation();
	}

	BsGetTelephoneC* pBsGetTelephoneC = BsGetTelephoneC::getObject();
	if( NULL != pBsGetTelephoneC )
	{
		pBsGetTelephoneC->cancelOperation();
	}

	BsUcsInfoGatherC* pBsUcsInfoGatherC = BsUcsInfoGatherC::getObject();
	if( NULL != pBsUcsInfoGatherC )
	{
		pBsUcsInfoGatherC->cancelOperation();
	}

	BsLocationUploadC* pBsLocationUploadC = BsLocationUploadC::getObject();
	if( NULL != pBsLocationUploadC )
	{
		pBsLocationUploadC->cancelOperation();
	}

	BsNewsC* pBsNewsC = BsNewsC::getObject();
	if( NULL != pBsNewsC )
	{
		pBsNewsC->cancelOperation();
	}

	BsPeccancyHintC* pBsPeccancyHintC = BsPeccancyHintC::getObject();
	if( NULL != pBsPeccancyHintC )
	{
		pBsPeccancyHintC->cancelOperation();
	}

	BsStockInfoC* pBsStockInfoC = BsStockInfoC::getObject();
	if( NULL != pBsStockInfoC )
	{
		pBsStockInfoC->cancelOperation();
	}

	BsTrafficNoticeC* pBsTrafficNoticeC = BsTrafficNoticeC::getObject();
	if( NULL != pBsTrafficNoticeC )
	{
		pBsTrafficNoticeC->cancelOperation();
	}

	BsWeatherForecastC* pBsWeatherForecastC = BsWeatherForecastC::getObject();
	if( NULL != pBsWeatherForecastC )
	{
		pBsWeatherForecastC->cancelOperation();
	}

	BsCarInfoMonitorC* pBsCarInfoMonitorC = BsCarInfoMonitorC::getObject();
	if( NULL != pBsCarInfoMonitorC )
	{
		pBsCarInfoMonitorC->cancelOperation();
	}

	BsClientC* pBsClientC = BsClientC::getObject();
	if( NULL != pBsClientC )
	{
		//pBsClientC->cancelOperation();
	}

	BsServerC* pBsServerC = BsServerC::getObject();
	if( NULL != pBsServerC )
	{
		//pBsServerC->cancelOperation();
	}
	
	//delete all object
	if( NULL != pBsLoginC )
	{
		pBsLoginC->releaseObject();
	}

	if( NULL != pBsAuthenticationReportC )
	{
		pBsAuthenticationReportC->releaseObject();
	}

	if( NULL != pBsGetTelephoneC )
	{
		pBsGetTelephoneC->releaseObject();
	}

	if( NULL != pBsUcsInfoGatherC )
	{
		pBsUcsInfoGatherC->releaseObject();
	}

	if( NULL != pBsLocationUploadC )
	{
		pBsLocationUploadC->releaseObject();
	}

	if( NULL != pBsNewsC )
	{
		pBsNewsC->cancelOperation();
		pBsNewsC->releaseObject();
	}

	if( NULL != pBsPeccancyHintC )
	{
		pBsPeccancyHintC->releaseObject();
	}

	if( NULL != pBsStockInfoC )
	{
		pBsStockInfoC->releaseObject();
	}

	if( NULL != pBsTrafficNoticeC )
	{
		pBsTrafficNoticeC->releaseObject();
	}

	if( NULL != pBsWeatherForecastC )
	{
		pBsWeatherForecastC->releaseObject();
	}

	if( NULL != pBsCarInfoMonitorC )
	{
		pBsCarInfoMonitorC->releaseObject();
	}
	
	if( NULL != pBsClientC )
	{
		pBsClientC->releaseObject();
	}

	if( NULL != pBsServerC )
	{
		pBsServerC->releaseObject();
	}
			
	//delete gs_ObjectmutexForThreadSafe, after this, invoke bsManager will lead abort
	gs_ObjectMutexForThreadSafe->lock();
	delete gs_ObjectMutexForThreadSafe;
	gs_ObjectMutexForThreadSafe = NULL;

	if( NULL != gs_BsBaseCMutexForThreadSafe )
	{
		delete gs_BsBaseCMutexForThreadSafe;
		gs_BsBaseCMutexForThreadSafe = NULL;
	}
}













//parameters related with login
static char g_uuid[BK_UUID_LENGTH_MAX+1] = "-1";
static char g_tuid[BK_TUID_LENGTH_MAX+1] = "";
static char g_host[URL_MAX_LENGTH] = "";
static char *gp_apHost[Ap_Max] = {NULL, NULL, NULL, NULL,NULL};
static const char *gp_apIdStr[Ap_Max] = {"APINNOV004", "APINNOV010", "APINNOV011", "APINNOV007","APINNOV003","APINNOV005","APINNOV009","APINNOV006","APINNOV008","APINNOV017"};


static char radix32[32] ={ 
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
  'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
  'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
};

static int is_alnum(int ch)
{
  return ((ch >= '0' && ch <= '9') ||
    (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

BsBaseC::BsBaseC()
	: m_bExitHttp(false)
{

}

BsBaseC::~BsBaseC()
{
	m_bExitHttp = true;
}

void BsBaseC::urlEncode(unsigned char *src, unsigned char *dst)
{
	for (; *src; src++)
	{
		if (*src == ' ')
			*dst++ = '+';
		else if (is_alnum(*src))
			*dst++ = *src;
		else
		{
                     unsigned int cc = *src;

                    *dst++ = '%';
                    *dst++ = radix32[cc >> 4];
                    *dst++ = radix32[cc & 0xf];
		}
	}
	*dst = '\0';
}


#define AT88SC_IOCTL_BASE  'C'
#define AT88SC_DATA       _IOW(AT88SC_IOCTL_BASE,0,struct at88sc_ioctl_data)
#define AT88SC_PRODUCT_ID _IOW(AT88SC_IOCTL_BASE,1,struct at88sc_ioctl_data)

struct at88sc_ioctl_data { 
    unsigned char rw;
    unsigned char userzone;
    unsigned char password[13];
};

int BsBaseC::getTuid(char *tuid)
{
	if(NULL == gs_BsBaseCMutexForThreadSafe)
	{
		printf("ERROR:NO INIT BsManager\n");
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR:NO INIT BsManager\n")
		abort();
	}
	
	gs_BsBaseCMutexForThreadSafe->lock();

	if( 0!=strcmp(g_tuid, "") )
	{
		strcpy(tuid, g_tuid);
		gs_BsBaseCMutexForThreadSafe->unlock();
		return 0;
	}

	FILE *pf_fd;
	pf_fd = fopen("/application/data/tuid","r");
	if(!pf_fd)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "open %s error\n", "/application/data/tuid");
		gs_BsBaseCMutexForThreadSafe->unlock();
		return -1;
	}

	memset(tuid, 0, 33);
	int retval = 0;
	if( 32 != (retval=fread(tuid, 1, 32, pf_fd)) )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read %s error [%d]\n", "/application/data/tuid", retval);
		fclose(pf_fd);
		gs_BsBaseCMutexForThreadSafe->unlock();
		return -1;
	}

	fclose(pf_fd);

	strcpy(g_tuid, tuid);

	gs_BsBaseCMutexForThreadSafe->unlock();

	return 0;
}


#if 0
//warning: this function can not be invoked at the same time
int BsBaseC::getTuid(char *tuid)
{
	 m_BsMutexForTuid.lock();

	static char current_tuid[BK_TUID_LENGTH_MAX+1]="";

	if( 0!=strcmp(current_tuid, "") )
	{
		strcpy(tuid, current_tuid);
                //printf("-------------------------->tuid have been getted ever!!!\n");
		 m_BsMutexForTuid.unlock();
		return 0;
	}
	
	int dev = -1;
	struct at88sc_ioctl_data ioctl_data;
	unsigned char passbuf[13] = {'c','H','i','N','a','T','s','P',9,2,8,7,5};

	dev = open("/dev/at88sc",O_RDWR);
	if(dev < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Failed to open device \n");
		goto err_out;
	}
	memcpy(ioctl_data.password,passbuf,13);
	if(ioctl(dev,AT88SC_PRODUCT_ID,&ioctl_data) != 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Failed to control device\n");
		goto err_out;
	}

	ioctl_data.rw = 1;
	ioctl_data.userzone = 1;
	if(ioctl(dev,AT88SC_DATA,&ioctl_data) != 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Failed to read from device\n");
		goto err_out;
	}
	if(read(dev, tuid, 32) != 32)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "tuid len is not 32\n");
		goto err_out;
	}
	tuid[32] = '\0';
	close(dev);
	 m_BsMutexForTuid.unlock();

	strcpy(current_tuid, tuid);
	
	return 0;

err_out:
	if(dev >= 0)
	{
		close(dev);
	}

	 m_BsMutexForTuid.unlock();
	return -1;
}
#endif

//0 -- success to get data from database
//-1 -- fail
// 1 -- not login never
int BsBaseC::getLoginResult(char *tuid, char *uuid, char *password, char *accessToken, bool &bLogin, bool &bSavePassword)
{
	int retval = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pQueryTablesSel = (char*)"SELECT * FROM BSLoginInfo";//查表语句

	if( 0 != getTuid(tuid) )
	{
		strcpy(tuid, "");
		return -1;
	}

	retval = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s\n", sqlite3_errmsg(pDB));
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
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	retval = sqlite3_step(pStmt);
	if (retval != SQLITE_ROW)
	{
		//BSLoginInfo is empty, because we first use BSLoginInfo
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);

		strcpy(uuid, "");
		strcpy(password, "");
		strcpy(accessToken, "");
		bLogin = false;
		bSavePassword = false;
		saveLoginResult(tuid, uuid, password, accessToken, bLogin, bSavePassword);
		return 0;
	}
	
	strcpy(uuid, (char*)sqlite3_column_text(pStmt, 1));
	strcpy(password, (char*)sqlite3_column_text(pStmt, 2));
	strcpy(accessToken, (char*)sqlite3_column_text(pStmt, 3));
	bSavePassword = sqlite3_column_int(pStmt, 4);
	bLogin = sqlite3_column_int(pStmt, 5);
	
//	IBD_PRINTF(DEBUG_INFOMATION, "readfromdatabase: tuid = %s, uuid = %s, password = %s, access_token = , bLogin = %d, bSavePassword = %d \n", tuid, uuid, password, bLogin, bSavePassword);
	
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
		
	return 0;	
}

int BsBaseC::saveLoginResult(const char *tuid, const char *uuid, const char *password, const char *accessToken, bool bLogin, bool bSavePassword)
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = "delete from BSLoginInfo";
	char pInsertTablesSel[128] = {0};
	
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
	
	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);
	
	//插入记录条
	sprintf(pInsertTablesSel, "insert into BSLoginInfo values('%s', '%s', '%s', '%s', '%d', '%d')", tuid, uuid, password, accessToken, bSavePassword, bLogin);

	//准备插入数据
	ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	//执行绑定以后的语句
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

void BsBaseC::trimStr(char* p_str)
{
	char *pFirst = NULL;//第一个非空字符
	char *pLast = NULL;//最后一个非空字符

	if( NULL == p_str )
	{
		return;
	}
	
	int len = strlen(p_str);
	if( 0 == len )
	{//空字符串没必要去空格
		return;
	}
	
	//首先找到第一个非空字符
	pFirst = p_str;
	while(1)
	{
		if( ' ' != *pFirst && 9 != *pFirst && 10 != *pFirst && 13 != *pFirst && 8 != *pFirst )
		{
			break;
		}

		pFirst++;
	}

	//找到最后一个非空字符
	pLast = p_str+len-1;
	while( pLast >= p_str )
	{
		if( ' ' != *pLast && 9 != *pLast && 10 != *pLast && 13 != *pLast )
		{
			break;
		}

		pLast--;
	}

	//拷贝非空字符串
	if( pLast >= pFirst )
	{
		*(pLast+1) = 0;
		strcpy(p_str, pFirst);
	}
	else
	{
		strcpy(p_str, "");
	}
	
	return;
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
//	printf("authentication size=%d nmemb=%d  pDataBlock->len=%d\n",size,nmemb,pDataBlock->len);
//	printf("authentication contents=%s\n",(char*)ptr);

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

static int m_bExit_func(bool *m_bExit, double DlT, double DlN, double UlT, double UlN)
{		
	if ( (*m_bExit) == true)
	{
		printf("in  m_bExit_func  m_bExit = %d\n",*m_bExit);
		return -1;
	}	

	return 0;
}

int BsBaseC::_getUcsApiHostInside(const char *uuid, char *host)
{
	int retval = -1;
	int retval_data = -1;
	char url[512];

	strcpy(host, DEFAULT_UCS_API_HOST);

	if( 0 == strcmp(uuid, DEFAULT_UUID) )
		return BS_E_OK;

	m_bExitHttp = false;
	
 	sprintf(url, "Http://%s/api/1.0/vlr/%s/", DEFAULT_UCS_API_HOST, uuid);

	CURLcode res;

	CURL *curl = curl_easy_init();

	DataBlock dataBlock;

	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_easy_setopt(curl,CURLOPT_URL, url);
		printf("Connecting to get uscHost %s->>>>\n",url);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);	
   		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
   		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA,  &m_bExitHttp);

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
					else if( xml.attributes().value("key") == QString("ucs_host") )
					{
						retval_data = 0;
						strcpy(host, xml.readElementText().toUtf8().data());
						IBD_PRINTF(DEBUG_INFOMATION, "host[%s]!\n", host)
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

int BsBaseC::_getUuid(char *uuid)
{
	char tuid[BK_TUID_LENGTH_MAX+1];
	char password[BK_PASSWORD_LENGTH_MAX+1];
	char token[BK_TOKEN_LENGTH_MAX+1];
	bool bLogin;
	bool bSavePassword;
	if( getLoginResult(tuid, uuid, password, token, bLogin, bSavePassword) < 0 )
	{
	       IBD_PRINTF(DEBUG_INFOMATION, "ERRROR:get login result error!\n")
		return -1;
	}
	else
	{
		if( !bLogin )
		{
		       IBD_PRINTF(DEBUG_INFOMATION, "user not login!\n")
			strcpy(uuid, DEFAULT_UUID);
		}
	}

	return 0;
}

int BsBaseC::getUcsApiHost(char *pHost, const char *pUuid)
{
	int retval = BS_E_OK;
	
	if( NULL == pHost )
		return BS_E_ARG_INVALID;

	char uuid[BK_UUID_LENGTH_MAX+1];
	if( NULL == pUuid )
	{
		if( _getUuid(uuid) < 0 )
			return BS_E_DATABASE_ERROR;
	}
	else
	{
		strcpy(uuid, pUuid);
	}

	if(NULL == gs_BsBaseCMutexForThreadSafe)
	{
		printf("ERROR:NO INIT BsManager\n");
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR:NO INIT BsManager\n")
		abort();
	}
		
	gs_BsBaseCMutexForThreadSafe->lock();

	if( 0 == strcmp(uuid, g_uuid) )
	{
		strcpy(pHost, g_host);
		 gs_BsBaseCMutexForThreadSafe->unlock();
		return BS_E_OK;
	}

	gs_BsBaseCMutexForThreadSafe->unlock();

	retval=_getUcsApiHostInside(uuid, pHost);

	if( BS_E_OK == retval )
	{
		gs_BsBaseCMutexForThreadSafe->lock();
		strcpy(g_uuid, uuid);
		strcpy(g_host, pHost);
		for(int i=0; i<Ap_Max; i++)
		{
			if( NULL != gp_apHost[i] )
			{
				free(gp_apHost[i]);
				gp_apHost[i] = NULL;
			}
		}
		gs_BsBaseCMutexForThreadSafe->unlock();
	}
	

	return retval;
}

int BsBaseC::_getApApiHostInside(const char *uuid, const char *pApIdStr, const char *tuid, const char *token, const char* pHost, char* pApHost)
{
	int retval;
	int retval_ok = -1;
	int retval_host = -1;
	int retval_status = -1;

	char url[URL_MAX_LENGTH];
	char cookie[512];

	DataBlock dataBlock;

	//sprintf(url, "Http://%s/api/1.0/user/%s/ap/%s/", pHost, uuid, pApIdStr);
	strcpy(url, "http://ap.chinatsp.com/ap004/api/1.0/tu/getweather?city=%E5%8C%97%E4%BA%AC");
	sprintf(cookie,"customer_key=%s;access_token=%s",tuid,token);

	m_bExitHttp = false;

	CURLcode res;
	CURL *curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_easy_setopt(curl,CURLOPT_URL, url);
		printf("Connecting to get apHost--%s->>>>\n",url);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);	
   		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
   		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA,  &m_bExitHttp);
		
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);
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
							retval_ok = 0;
						}

					}
					else if( xml.attributes().value("key") == QString("token_data") )
					{
						
					}
					else if( xml.attributes().value("key") == QString("ap_id") )
					{
						//strcpy(apInfo->ap_id, xml.readElementText().toUtf8().data());
					}
					else if( xml.attributes().value("key") == QString("ap_name") )
                                   {
                                   	//strcpy(apInfo->ap_name, xml.readElementText().toUtf8().data());
                                   }
					else if( xml.attributes().value("key") == QString("ap_host") )
                                   {
                                   	strcpy(pApHost, xml.readElementText().toUtf8().data());
						retval_host = 0;
                                   	//strcpy(apInfo->ap_host, xml.readElementText().toUtf8().data());
                                   }
					else if( xml.attributes().value("key") == QString("state") )
                                   {
                                   	//strcpy(apInfo->state, xml.readElementText().toUtf8().data());
                                   	char mystatus[128];
						strcpy(mystatus, xml.readElementText().toUtf8().data());
						if( 0 == strcmp(mystatus, "0") )
						{
							retval_status = 0;
						}
                                   }

				}

			}

		}

		/* always cleanup */

		curl_easy_cleanup(curl);	

	}

	if (res == CURLE_OK) 
	{
		if( retval_ok == 0 )
		{
			if( retval_status == 0 )
			{
				if( retval_host == 0 )
				{
					retval = BS_E_OK;
				}
				else
				{
					retval = BS_E_URL_XML_PARSE_ERROR;
					IBD_PRINTF(DEBUG_INFOMATION, "ERROR:xml parse error! xml[%s]\n", dataBlock.buffer)
					IBD_PRINTF(DEBUG_INFOMATION, "url[%s] cookie[%s]\n", url, cookie);
				}
			}
			else
			{
				retval = BS_E_USER_NOT_PERMISSION;
				IBD_PRINTF(DEBUG_INFOMATION, "ERROR:user not permission! xml[%s]\n", dataBlock.buffer)
				IBD_PRINTF(DEBUG_INFOMATION, "url[%s] cookie[%s]\n", url, cookie);
			}
		}
		else
		{
			retval = BS_E_INVALID_AP;
			IBD_PRINTF(DEBUG_INFOMATION, "ERROR:invalid ap! xml[%s]\n", dataBlock.buffer)
			IBD_PRINTF(DEBUG_INFOMATION, "url[%s] cookie[%s]\n", url, cookie);
		}
	}
	else
	{
		retval = BS_E_URL_HTTP_ERROR;
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR:curl_easy_perform fail res[%d]!\n", res)
		IBD_PRINTF(DEBUG_INFOMATION, "url[%s] cookie[%s]\n", url, cookie);
	}
	
	return retval;
}

int BsBaseC::getApApiHost(Ucs_Ap_Id apId, char* pApHost)
{
	int retval = BS_E_OK;
	
	if( NULL == pApHost )
		return BS_E_ARG_INVALID;
	if( apId < 0 || apId >= Ap_Max )
		return BS_E_ARG_INVALID;

	char uuid[BK_UUID_LENGTH_MAX+1];
	char tuid[BK_TUID_LENGTH_MAX+1];
	char password[BK_PASSWORD_LENGTH_MAX+1];
	char token[BK_TOKEN_LENGTH_MAX+1];
	bool bLogin;
	bool bSavePassword;
	if( getLoginResult(tuid, uuid, password, token, bLogin, bSavePassword) < 0 )
	{
	       IBD_PRINTF(DEBUG_INFOMATION, "ERROR:get login result error!\n")
		return BS_E_DATABASE_ERROR;
	}
	else
	{
	printf("----Line is %d && FUNCTION is %s==========\n", __LINE__,__FUNCTION__);
		if( !bLogin )
		{
		       IBD_PRINTF(DEBUG_INFOMATION, "user not login!\n")
			strcpy(uuid, DEFAULT_UUID);
			strcpy(token, DEFAULT_TOKEN);
		}
	}

	if(NULL == gs_BsBaseCMutexForThreadSafe)
	{
	printf("----Line is %d && FUNCTION is %s==========\n", __LINE__,__FUNCTION__);
		printf("ERROR:NO INIT BsManager\n");
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR:NO INIT BsManager\n")
		abort();
	}
	
	gs_BsBaseCMutexForThreadSafe->lock();

	if( 0 == strcmp(uuid, g_uuid) )
	{
		if( NULL != gp_apHost[apId] )
		{
			strcpy(pApHost, gp_apHost[apId]);
			gs_BsBaseCMutexForThreadSafe->unlock();
			return BS_E_OK;
		}
	}
	else
	{
		gs_BsBaseCMutexForThreadSafe->unlock();

		char pHost[URL_MAX_LENGTH];

	printf("----Line is %d && FUNCTION is %s==========\n", __LINE__,__FUNCTION__);
		retval = _getUcsApiHostInside(uuid, pHost);
		if( BS_E_OK != retval )
		{
	printf("----Line is %d && FUNCTION is %s==========\n", __LINE__,__FUNCTION__);
			return retval;
		}
		
	printf("----Line is %d && FUNCTION is %s==========\n", __LINE__,__FUNCTION__);
		gs_BsBaseCMutexForThreadSafe->lock();

		strcpy(g_uuid, uuid);
		strcpy(g_host, pHost);
		for(int i=0; i<Ap_Max; i++)
		{
			if( NULL != gp_apHost[i] )
			{
				free(gp_apHost[i]);
				gp_apHost[i] = NULL;
			}
		}
	}

	gs_BsBaseCMutexForThreadSafe->unlock();

	printf("----Line is %d && FUNCTION is %s==========\n", __LINE__,__FUNCTION__);
	retval = _getApApiHostInside(uuid, gp_apIdStr[apId], tuid, token, g_host, pApHost);
	printf("----Line is %d && FUNCTION is %s======%d====\n", __LINE__,__FUNCTION__,retval);
	if( BS_E_OK == retval )
	{
		gs_BsBaseCMutexForThreadSafe->lock();

		if( NULL == gp_apHost[apId] )
		{
			gp_apHost[apId] = (char*)malloc(URL_MAX_LENGTH);
		}
		strcpy(gp_apHost[apId], pApHost);

		gs_BsBaseCMutexForThreadSafe->unlock();
	}

	return retval;
}

void	BsBaseC::stopHttp()
{
	m_bExitHttp = true;
}

