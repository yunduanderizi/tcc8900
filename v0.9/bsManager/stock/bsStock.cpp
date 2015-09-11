/**
 * @file   bsTrafficNotice.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date   20110124
 * 
 * @brief  
 *      Implement for traffic notices download 
 * 
 */

#include "bsStock.h"
#include <QXmlStreamReader>
#include <curl.h>
#include <QDebug>
#include "bsCommunalLock.h"
#include "common_config.h"
#define BS_STOCK_E_CALLBACK			  	42

#define STORK_LIST_XMLFEIL  "stockDownload/stockInfo.xml"
#define STORK_TODAY_CHART_XMLFEIL  "stockDownload/todayChart.gif"
#define STORK_DAY_K_XMLFEIL  "stockDownload/dayK.gif"
#define STORK_WEEK_K_XMLFEIL  "stockDownload/weekK.gif"
#define STORK_MONTH_K_XMLFEIL  "stockDownload/monthK.gif"


static int m_bExit_func(bool *m_bExit, double DlT, double DlN, double UlT, double UlN)
{		
	if ( (*m_bExit) == true)
	{
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
		return -1;
	}
	return writeen;
}

BsStockInfoC *BsStockInfoC::thisObject = NULL;
BsStockInfoC *BsStockInfoC::object()
{
	BsStockInfoC* pTmp = NULL;

	bsManagerObjectLock();
	
	if( !thisObject )
	{
		thisObject = new BsStockInfoC();
	}

	pTmp = thisObject;
	bsManagerObjectUnLock();
		
	return pTmp;
}

void BsStockInfoC::releaseObject()
{

	bsManagerObjectLock();
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();
	
}

BsStockInfoC *BsStockInfoC::getObject()
{
	BsStockInfoC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}
BsStockInfoC::BsStockInfoC()
	:m_bExit(false)
{
	connect(this, SIGNAL(finished()), this, SLOT(slotFinished()), Qt::QueuedConnection);
	QDir stockPath;
	char storkDir[512] = {""};
	sprintf(storkDir,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,"stockDownload/");
	if(false == stockPath.mkpath(storkDir))
	{
		IBD_PRINTF(DEBUG_INFOMATION,"mkdir path[%s] fail\n", storkDir)
		return;
	}
}

BsStockInfoC::~BsStockInfoC()
{
	if( isRunning() )
	{
		cancelOperation();
		m_mutexForThreadSafe.unlock();
		wait();
		m_mutexForThreadSafe.lock();
	}
}

void BsStockInfoC::slotFinished()
{
	m_mutexForThreadSafe.lock();

	if( (!m_list.isEmpty()) && (!isRunning()) )
	{
		m_bExit = false;
		start();
	}
		
	m_mutexForThreadSafe.unlock();
}

void BsStockInfoC::cancelOperation()
{
	m_mutexForThreadSafe.lock();
	
	m_list.clear();
	if( isRunning() )
	{
		BsBaseC::stopHttp();
		m_bExit = true;
	}
	
	m_mutexForThreadSafe.unlock();
}

int BsStockInfoC::startGetStockInfo(const QString stockId, Bs_Stock_Download_Mode flag)
{
	m_mutexForThreadSafe.lock();

	DownloadTask myTask;
	myTask.stockId = stockId;
	myTask.iflag = flag;
	m_list.append(myTask);

	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}
		
	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}
int BsStockInfoC::_download_Xml(QFile *file,QString url)	
{
	CURL *curl = curl_easy_init();
	if(curl)
	{		
		printf("Connecting to %s->>>>\n", url.toAscii().data());
		curl_easy_setopt(curl,CURLOPT_URL, url.toAscii().data());
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	
   		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);	
   		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
   		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA,  &m_bExit);
   
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  	 	curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)file);
 			
		CURLcode res = curl_easy_perform(curl);
	
		if (res != CURLE_OK)
		{
			IBD_PRINTF(DEBUG_INFOMATION,"curl error:[%s]\n",curl_easy_strerror(res))
			if (res ==  BS_STOCK_E_CALLBACK)
			{
				IBD_PRINTF(DEBUG_INFOMATION,"curl by cancel\n")
			}
			curl_easy_cleanup(curl);
			return -1;
				
		}

	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error: curl_easy_init error\n");
		curl_easy_cleanup(curl);
		return -1;
	}
	curl_easy_cleanup(curl);	

	return 0;
}

int BsStockInfoC::_download_chart(QFile *file,QString url)
{

	CURL *curl = curl_easy_init();
	if(curl)
	{		
		printf("Connecting to %s->>>>\n",url.toAscii().data());
		curl_easy_setopt(curl,CURLOPT_URL,url.toAscii().data());
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
			
	   	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);	
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
	   	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &m_bExit);
	   
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
      	curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)file);
     		
		CURLcode res = curl_easy_perform(curl);
		
		if (res != CURLE_OK)
		{
			IBD_PRINTF(DEBUG_INFOMATION,"curl error:[%s]\n",curl_easy_strerror(res))
			if (res ==  BS_STOCK_E_CALLBACK)
			{
				IBD_PRINTF(DEBUG_INFOMATION,"curl by cancel\n")
			}
			curl_easy_cleanup(curl);
			return -1;		
		}
		
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Error: curl_easy_init error\n");
		curl_easy_cleanup(curl);
		return -1;
	}
	curl_easy_cleanup(curl);
	return 0;	
}

int BsStockInfoC::_parseStockInfoXml(QFile * file, StockInfoNodeC *pstockInfo)
{
	QIODevice *device = (QIODevice *)file;
	device->seek(0);
	QXmlStreamReader xml;
	xml.setDevice(device);
	bool bParseOk = false;	
	while(!xml.atEnd())
	{
		xml.readNext();
		if (xml.isStartElement() && xml.name() == "stockinfo"  )
		{
			pstockInfo->id= xml.attributes().value("id").toString().toUtf8().data();
			pstockInfo->name= xml.attributes().value("name").toString().toUtf8().data();
			bParseOk = true;	
		}
		else if(xml.isStartElement() && xml.name() == "from" )
		{
			pstockInfo->from = xml.readElementText().toUtf8().data();
	
		}
		else if(xml.isStartElement() && xml.name() == "today_open" )
		{
			pstockInfo->todayOpen = xml.readElementText().toUtf8().data();	
		}
		else if(xml.isStartElement() && xml.name() == "yesterday_close" )
		{
			pstockInfo->yesterdayClose =xml.readElementText().toUtf8().data();		
		}
		else if(xml.isStartElement() && xml.name() == "today_now" )
		{
			pstockInfo->todayNow = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "today_min" )
		{
			pstockInfo->todayMin = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "today_max" )
		{
			pstockInfo->todayMax = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "today_turnover" )
		{
			pstockInfo->todayTurnover = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "today_updown" )
		{
			pstockInfo->todayUpdown = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "today_volume")
		{
			pstockInfo->todayVolume = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "seller_one_price")
		{
			pstockInfo->sellerOnePrice = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "seller_one_volume")
		{
			pstockInfo->sellerOneVolume= xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "seller_two_price")
		{
			pstockInfo->sellerTwoPrice = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "seller_two_volume")
		{
			pstockInfo->sellerTwoVolume = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() =="seller_three_price")
		{
			pstockInfo->sellerThreePrice = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "seller_three_volume")
		{
			pstockInfo->sellerThreeVolume = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() =="seller_fourth_price")
		{
			pstockInfo->sellerFourthPrice = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "seller_fourth_volume")
		{
			pstockInfo->sellerFourthVolume = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() =="seller_fifth_price")
		{
			pstockInfo->sellerFifthPrice = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "seller_fifth_volume")
		{
			pstockInfo->sellerFifthVolume = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "buyer_one_price")
		{
			pstockInfo->buyerOnePrice = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "buyer_one_volume")
		{
			pstockInfo->buyerOneVolume = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "buyer_two_price")
		{
			pstockInfo->buyerTwoPrice = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "buyer_two_volume")
		{
			pstockInfo->buyerTwoVolume = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "buyer_three_price")
		{
			pstockInfo->buyerThreePrice = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "buyer_three_volume")
		{
			pstockInfo->buyerThreeVolume = xml.readElementText().toUtf8().data();
		}	
		else if(xml.isStartElement() && xml.name() == "buyer_fourth_price")
		{
			pstockInfo->buyerFourthPrice = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "buyer_fourth_volume")
		{
			pstockInfo->buyerFourthVolume = xml.readElementText().toUtf8().data();
		}	
		else if(xml.isStartElement() && xml.name() == "buyer_fifth_price")
		{
			pstockInfo->buyerFifthPrice = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "buyer_fifth_volume")
		{
			pstockInfo->buyerFifthVolume = xml.readElementText().toUtf8().data();
		}	
		else if(xml.isStartElement() && xml.name() == "commission_ratio")
		{
			pstockInfo->commissionRatio = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "commission_difference")
		{
			pstockInfo->commissionDifference = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "turnover")
		{
			pstockInfo->turnover = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "external_disk")
		{
			pstockInfo->externalDisk = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "plate")
		{
			pstockInfo->plate = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "updateTime" )
		{
			pstockInfo->updateTime = xml.readElementText().toUtf8().data();
		}	
		else if(xml.isStartElement() && xml.name() == "current_price" )
		{
			pstockInfo->current_price = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "limit_price" )
		{
			pstockInfo->limit_price = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "limit_down_price" )
		{
			pstockInfo->limit_down_price = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "inflation_rate_down" )
		{
			pstockInfo->inflation_rate_down = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "today_chart_uri" )
		{
			pstockInfo->todayChartUri = xml.readElementText().toUtf8().data();	
		}
		else if(xml.isStartElement() && xml.name() == "day_k_uri" )
		{
			pstockInfo->dayKUri = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "week_k_uri" )
		{
			pstockInfo->weekKUri = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "month_k_uri" )
		{
			pstockInfo->monthKUri = xml.readElementText().toUtf8().data();
		}	
	}
if(!bParseOk)
{
	return -1;
}
return 0;
}

void BsStockInfoC::run()
{
	DownloadTask		myTask;
	int retval;
	StockInfoNodeC stockInfo;

	char stockUrl[URL_MAX_LENGTH];
	char stockUrlUsed[URL_MAX_LENGTH];
	char apHost[URL_MAX_LENGTH];
	retval = BsBaseC::getApApiHost(Ap_Stock, apHost);
	if( BS_E_OK != retval )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"ERROR:getApApiInfo fail\n")
		m_mutexForThreadSafe.lock();
		m_list.clear();
		m_mutexForThreadSafe.unlock();
		emit sigGetStockInfoResult(retval, stockInfo);
		return;
	}

	sprintf(stockUrl, "http://%s/ap010/api/1.0/tu/getstockdata?stockid=", apHost);
	
	while(1)
	{
		if(m_bExit)
		{
			IBD_PRINTF(DEBUG_INFOMATION,"thread by cancel\n")
			break;
		}
	
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
		
		if( myTask.iflag == 0 )
		{ 
			char storkPath[512] = {""};
	 		sprintf(storkPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_LIST_XMLFEIL);
			QFile stockFile(storkPath); 
			
			if (!stockFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
				retval = BS_E_URL_FILE_ERROR;
			}

			strcpy(stockUrlUsed, stockUrl);
			strcat(stockUrlUsed, myTask.stockId.toAscii().data());
			
			if(_download_Xml(&stockFile, stockUrlUsed)<0)
			{
				retval = BS_E_URL_HTTP_ERROR;
			}
			else
			{
				if(_parseStockInfoXml(&stockFile,&stockInfo) < 0 )
				{
					IBD_PRINTF(DEBUG_INFOMATION,"parse xml errorl,stockInfo[%s]\n",STORK_LIST_XMLFEIL)
					retval = BS_E_URL_XML_PARSE_ERROR;
						
				}
				else
					retval = BS_E_OK;	
			}  
			stockFile.close();
			emit sigGetStockInfoResult(retval,stockInfo);
		}
		else if(myTask.iflag == 1)
		{
			char storkPath[512] = {""};
	 		sprintf(storkPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_LIST_XMLFEIL);
			QFile stockFile(storkPath); 
			
			if (!stockFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
				retval = BS_E_URL_FILE_ERROR;
			}

			strcpy(stockUrlUsed, stockUrl);
			strcat(stockUrlUsed, myTask.stockId.toAscii().data());

			if(_download_Xml(&stockFile, stockUrlUsed)<0)
			{
				retval = BS_E_URL_HTTP_ERROR;
			}
			else
			{
				if(_parseStockInfoXml(&stockFile,&stockInfo) < 0 )
				{
					IBD_PRINTF(DEBUG_INFOMATION,"parse xml errorl,stockInfo[%s]\n",&stockInfo)
					retval = BS_E_URL_XML_PARSE_ERROR;
						
				}
				else
					retval = BS_E_OK;	
			}  
			stockFile.close();
			
			char charkPath[512] = {""};
			sprintf(charkPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_TODAY_CHART_XMLFEIL);
			QFile charkFile(charkPath);
			if (!charkFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
		       retval = BS_E_URL_FILE_ERROR;
			}
			if(_download_chart(&charkFile,stockInfo.todayChartUri) < 0)
			{
				IBD_PRINTF(DEBUG_INFOMATION,"no have today_chart\n")
				retval = BS_E_URL_HTTP_ERROR;
		
			}
			else 
			{
				retval = BS_E_OK;
				stockInfo.todayChartPath = STORK_TODAY_CHART_XMLFEIL;
			}
			charkFile.close();
			emit sigGetStockInfoResult(retval,stockInfo);
			
		}

		else if(myTask.iflag == 2)	
		{
			char storkPath[512] = {""};
	 		sprintf(storkPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_LIST_XMLFEIL);
			QFile stockFile(storkPath); 
				
			if (!stockFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
				retval = BS_E_URL_FILE_ERROR;
			}
		
			strcpy(stockUrlUsed, stockUrl);
			strcat(stockUrlUsed, myTask.stockId.toAscii().data());
			if(_download_Xml(&stockFile, stockUrlUsed)<0)
			{
				retval = BS_E_URL_HTTP_ERROR;
			}
			else
			{
				if(_parseStockInfoXml(&stockFile,&stockInfo) < 0 )
				{
					IBD_PRINTF(DEBUG_INFOMATION,"parse xml errorl,stockInfo[%s]\n",&stockInfo)
					retval = BS_E_URL_XML_PARSE_ERROR;
						
				}
				else
					retval = BS_E_OK;	
			}  
			stockFile.close();

			char dayKPath[512] = {""};
	 		sprintf(dayKPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_DAY_K_XMLFEIL);
			QFile dayKFile(dayKPath); 
			if (!dayKFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
		        retval = BS_E_URL_FILE_ERROR;
			}
			if(_download_chart(&dayKFile,stockInfo.dayKUri) < 0)
			{
				IBD_PRINTF(DEBUG_INFOMATION,"no have day_k\n")
				retval = BS_E_URL_HTTP_ERROR;
			
			}
			else 
			{
				retval = BS_E_OK;
				stockInfo.dayKPath = STORK_DAY_K_XMLFEIL;
			}
			dayKFile.close();
			emit sigGetStockInfoResult(retval,stockInfo);
		}
		
		else if(myTask.iflag == 3)
		{
			char storkPath[512] = {""};
			sprintf(storkPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_LIST_XMLFEIL);
			QFile stockFile(storkPath); 
			
			if (!stockFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
				retval = BS_E_URL_FILE_ERROR;
			}
		
			strcpy(stockUrlUsed, stockUrl);
			strcat(stockUrlUsed, myTask.stockId.toAscii().data());
			if(_download_Xml(&stockFile, stockUrlUsed)<0)
			{
				retval = BS_E_URL_HTTP_ERROR;
			}
			else
			{
				if(_parseStockInfoXml(&stockFile,&stockInfo) < 0 )
				{
					IBD_PRINTF(DEBUG_INFOMATION,"login ucs.chinatsp.com fail, parse xml errorl,stockInfoL[%s]\n",&stockInfo)
					retval = BS_E_URL_XML_PARSE_ERROR;
						
				}
				else
					retval = BS_E_OK;	
			}  
			stockFile.close();

			char weekKPath[512] = {""};
			sprintf(weekKPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_WEEK_K_XMLFEIL);
			QFile weekKFile(weekKPath); 
			if (!weekKFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
		        retval = BS_E_URL_FILE_ERROR;
			}
			if(_download_chart(&weekKFile,stockInfo.weekKUri) < 0)
			{
				IBD_PRINTF(DEBUG_INFOMATION,"no have week_k\n")
				retval = BS_E_URL_HTTP_ERROR;
				
			}
			else 
			{
				retval = BS_E_OK;
				stockInfo.weekKPath = STORK_WEEK_K_XMLFEIL;
			}
			weekKFile.close();
			emit sigGetStockInfoResult(retval,stockInfo);
		}
		
		else if(myTask.iflag == 4)
		{
			char storkPath[512] = {""};
			sprintf(storkPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_LIST_XMLFEIL);
			QFile stockFile(storkPath); 
			
			if (!stockFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
				retval = BS_E_URL_FILE_ERROR;
			}
			strcpy(stockUrlUsed, stockUrl);
			strcat(stockUrlUsed, myTask.stockId.toAscii().data());
			if(_download_Xml(&stockFile, stockUrlUsed)<0)
			{
				retval = BS_E_URL_HTTP_ERROR;
			}
			else
			{
				if(_parseStockInfoXml(&stockFile,&stockInfo) < 0 )
				{
					IBD_PRINTF(DEBUG_INFOMATION,"parse xml errorl,stockInfo[%s]\n",&stockInfo)
					retval = BS_E_URL_XML_PARSE_ERROR;
						
				}
				else
					retval = BS_E_OK;	
			}  
			stockFile.close();

			char mothKPath[512] = {""};
			sprintf(mothKPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_MONTH_K_XMLFEIL);
			QFile mothKFile(mothKPath); 
			if (!mothKFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
		       	 retval = BS_E_URL_FILE_ERROR;
			}
			if(_download_chart(&mothKFile,stockInfo.monthKUri) < 0)
			{
				IBD_PRINTF(DEBUG_INFOMATION,"no have month_k\n")
				retval = BS_E_URL_HTTP_ERROR;
				return;
			}
			else 
			{
				retval = BS_E_OK;
				stockInfo.monthKPath = STORK_MONTH_K_XMLFEIL;
			}
			mothKFile.close();
			emit sigGetStockInfoResult(retval,stockInfo);
		}
		else if( myTask.iflag == 5 )
		{
			char storkPath[512] = {""};
			 sprintf(storkPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_LIST_XMLFEIL);
			QFile stockFile(storkPath); 
			
			if (!stockFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
				retval = BS_E_URL_FILE_ERROR;
			}
		
			strcpy(stockUrlUsed, stockUrl);
			strcat(stockUrlUsed, myTask.stockId.toAscii().data());
			if(_download_Xml(&stockFile, stockUrlUsed)<0)
			{
				retval = BS_E_URL_HTTP_ERROR;
			}
			else
			{
				if(_parseStockInfoXml(&stockFile,&stockInfo) < 0 )
				{
					IBD_PRINTF(DEBUG_INFOMATION,"parse xml errorl,stockInfo[%s]\n",&stockInfo)
					retval = BS_E_URL_XML_PARSE_ERROR;	
				}
				else
				{	
					int downToday_Chart;
					int downDay_K;
					int downWeek_K;
					int downMonth_K;
					char charkPath[512] = {""};
					sprintf(charkPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_TODAY_CHART_XMLFEIL);
					QFile charkFile(charkPath); 
					if (!charkFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
					{
		       	  		retval = BS_E_URL_FILE_ERROR;
					}
					downToday_Chart = _download_chart(&charkFile,stockInfo.todayChartUri);
					char dayKPath[512] = {""};
					sprintf(dayKPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_DAY_K_XMLFEIL);
					QFile dayKFile(dayKPath); 
					if (!dayKFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
					{
		       	  		retval = BS_E_URL_FILE_ERROR;
					}
				
					downDay_K = _download_chart(&dayKFile,stockInfo.dayKUri);
					char weekKPath[512] = {""};
					sprintf(weekKPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_WEEK_K_XMLFEIL);
					QFile weekKFile(weekKPath); 
					if (!weekKFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
					{
		       	  		retval = BS_E_URL_FILE_ERROR;
					}
					downWeek_K = _download_chart(&weekKFile,stockInfo.weekKUri);
					char monthKPath[512] = {""};
					sprintf(monthKPath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,STORK_MONTH_K_XMLFEIL);
					QFile monthKFile(monthKPath); 
					if (!monthKFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
					{
		       	  		retval = BS_E_URL_FILE_ERROR;
					}
					downMonth_K = _download_chart(&monthKFile,stockInfo.monthKUri);
					
					if(downToday_Chart == 0 && downDay_K == 0 && downWeek_K==0 && downMonth_K==0)
					{
						retval = BS_E_OK;
						stockInfo.todayChartPath = STORK_TODAY_CHART_XMLFEIL;
						stockInfo.dayKPath = STORK_DAY_K_XMLFEIL;
						stockInfo.weekKPath = STORK_WEEK_K_XMLFEIL;
						stockInfo.monthKPath = STORK_MONTH_K_XMLFEIL;
				
					}
				}  
				stockFile.close();
				emit sigGetStockInfoResult(retval, stockInfo);
			}
		}
	}
}

