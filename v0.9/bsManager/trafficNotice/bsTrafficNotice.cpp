/*
 * @file   bsTrafficNotice.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date   20110124
 * 
 * @brief  
 *      Implement for traffic notices download 
 * 
 */

#include "bsTrafficNotice.h"
#include <QXmlStreamReader>
#include <curl.h>
#include "common_config.h"
#include "bsCommunalLock.h"
#define BS_TRAFFIC_NOTICE_E_CALLBACK			  	42

static int m_bExit_func(bool *m_bExit, double DlT, double DlN, double UlT, double UlN)
{		
	if ( (*m_bExit) == true)
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

BsTrafficNoticeC *BsTrafficNoticeC::thisObject = NULL;
BsTrafficNoticeC *BsTrafficNoticeC::object()
{
	BsTrafficNoticeC *pTmp = NULL;
	bsManagerObjectLock();
		
	if( !thisObject )
	{
		thisObject = new BsTrafficNoticeC();
	}
	pTmp = thisObject;
	bsManagerObjectUnLock();
		
	return pTmp;
}

void BsTrafficNoticeC::releaseObject()
{
	bsManagerObjectLock();
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();
}

BsTrafficNoticeC *BsTrafficNoticeC::getObject()
{
	BsTrafficNoticeC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}
BsTrafficNoticeC::BsTrafficNoticeC()
	:m_bExit(false)
{
	connect(this, SIGNAL(finished()), this, SLOT(slotFinished()), Qt::QueuedConnection);
	m_city = "北京";
	IBD_PRINTF(DEBUG_INFOMATION,"set city name is :[%s]\n",m_city.toUtf8().data())
}

BsTrafficNoticeC::~BsTrafficNoticeC()
{
	if( isRunning() )
	{
		cancelOperation();
		wait();
	}
}
void BsTrafficNoticeC::slotFinished()
{
	m_mutexForThreadSafe.lock();

	if( (!m_list.isEmpty()) && (!isRunning()) )
	{
		m_bExit = false;
		start();
	}
		
	m_mutexForThreadSafe.unlock();
}
int BsTrafficNoticeC::setCityPinyin(const char *pCity)
{
	m_mutexForThreadSafe.lock();
	if(NULL == pCity)
	{
		m_mutexForThreadSafe.unlock();
		return BS_E_ARG_INVALID;
	}
	
	IBD_PRINTF(DEBUG_INFOMATION,"set city name is :[%s]\n",pCity)
	m_city = pCity;
	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}
int BsTrafficNoticeC::setCityUtf8(char *pCity)
{
	m_mutexForThreadSafe.lock();
	if(NULL == pCity)
	{
		m_mutexForThreadSafe.unlock();
		return BS_E_ARG_INVALID;
	}
	
	if(strlen(pCity)*3>500)
	{
		IBD_PRINTF(DEBUG_INFOMATION,"set city name [%s] is  too long\n",pCity)
		return BS_TRAFFIC_CITY_NAME_LONGER;
	}
	unsigned char utf8ToUrl[500];

	IBD_PRINTF(DEBUG_INFOMATION,"set city name is :[%s]\n",pCity)
	urlEncode((unsigned char *)pCity, utf8ToUrl);
	
	m_city = (char *)utf8ToUrl;
	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}
int BsTrafficNoticeC::startGetTrafficNoticeList()
{
	m_mutexForThreadSafe.lock();
	
	DownloadTask myTask;
	myTask.city = m_city;
	myTask.ThreadWorkMode = 0;
	m_list.append(myTask);
	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}

	m_mutexForThreadSafe.unlock();
		
	return BS_E_OK;
	

}
int BsTrafficNoticeC::startGetTrafficNoticeNode(TrafficNoticeListNode *TrafficNode)
{
	if(NULL == TrafficNode)
		return BS_E_ARG_INVALID;
	m_mutexForThreadSafe.lock();
	
	DownloadTask myTask;
	myTask.city = m_city;
	myTask.TrafficNodeUrl = TrafficNode->url;
	myTask.ThreadWorkMode = 1;
	myTask.p_TrafficNode = TrafficNode;
	m_list.append(myTask);
	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}

	m_mutexForThreadSafe.unlock();

	return BS_E_OK;
}
int BsTrafficNoticeC::startGetTrafficNoticeInfo()
{
	m_mutexForThreadSafe.lock();
	DownloadTask myTask;
	myTask.city = m_city;
	myTask.ThreadWorkMode = 2;
	m_list.append(myTask);
	
	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}

	m_mutexForThreadSafe.unlock();
		
	return BS_E_OK;
	
}
void	BsTrafficNoticeC ::cancelOperation()
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
int BsTrafficNoticeC::_downloadXml(QFile *file, char *url)
{

	if(NULL == url )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR:the url is NUll\n")
		return BS_E_ARG_INVALID;
	}
	CURL *curl = curl_easy_init();
	if(curl)
	{		
		printf("Connecting to trafficNotice %s->>>>\n", url);
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
			IBD_PRINTF(DEBUG_INFOMATION,"curl error:[%s]\n",curl_easy_strerror(res))
			if (res ==  BS_TRAFFIC_NOTICE_E_CALLBACK)
			{
				curl_easy_cleanup(curl);
				IBD_PRINTF(DEBUG_INFOMATION,"curl by cancel\n")
				return -1;
			}
		}

	}
	curl_easy_cleanup(curl);
	return 0;
}


int BsTrafficNoticeC::_parseXmlList(QFile *file, TrafficNoticeList *pTrafficNoticeList)
{	
	
	QIODevice *device = (QIODevice *)file;
	device->seek(0);
	QXmlStreamReader xml;
	xml.setDevice(device);
	TrafficNoticeListNode myNode;

	while(!xml.atEnd())
	{
		xml.readNext();
		
		if(xml.isStartElement() && xml.name() == "notif" )
		{		
			myNode.clear();
		}
		else if(xml.isStartElement() && xml.name() == "from" )
		{
			myNode.from = xml.readElementText().toUtf8().data();
		}
		else if(xml.isStartElement() && xml.name() == "title" )
		{
			myNode.title = xml.readElementText().toUtf8().data();	
		}
		else if(xml.isStartElement() && xml.name() == "publish_time" )
		{
			myNode.time =xml.readElementText().toUtf8().data();		
		}
		else if(xml.isStartElement() && xml.name() == "url" )
		{
			myNode.url = xml.readElementText().toUtf8().data();
		}
		else if(xml.isEndElement() && xml.name() == "notif" )
		{
			pTrafficNoticeList->append(myNode);
		}
	
	}

return 0;
}


int BsTrafficNoticeC::_parseXmlNode(QFile *file, TrafficNoticeListNode *pTrafficNoticeNode)
{	
	int flag = 0;
   	QIODevice *device = (QIODevice *)file;
   	device->seek(0);
   	QXmlStreamReader xml;
	xml.setDevice(device);

	while(!xml.atEnd())
	{
		xml.readNext();
		if(xml.isStartElement() && xml.name()=="content" )
		{	
			flag = 1;
			pTrafficNoticeNode->content = xml.readElementText().toUtf8().data();
		}
		
	}
	if(flag == 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "ERROR:The xml no has content\n")
		return BS_E_URL_XML_PARSE_ERROR;
	}
return 0;
}

void BsTrafficNoticeC::run()
{

	DownloadTask		myTask;	
	char trafficUrl[URL_MAX_LENGTH];
	char apHost[URL_MAX_LENGTH];
	if( BS_E_OK != BsBaseC::getApApiHost(Ap_TrafficNotice, apHost) )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"ERROR:getApApiInfo fail\n")
		m_mutexForThreadSafe.lock();
		m_list.clear();
		m_mutexForThreadSafe.unlock();
		return;
	}

	QDir trafficNoticePath;
	char trafficDir[512] ={""};
	sprintf(trafficDir,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,"trafficNoticeDownload/");
	if(false == trafficNoticePath.mkpath(trafficDir))
	{
		IBD_PRINTF(DEBUG_INFOMATION,"ERROR:mkdir path[%s] fail\n", trafficDir)
		return;
	}
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
		int retval = BS_E_OK;
		if( myTask.ThreadWorkMode == 0 )
		{ 
			sprintf(trafficUrl, "http://%s/ap011/api/1.0/tu/getroadnotice?city=%s", apHost,myTask.city.toAscii().data());
			TrafficNoticeList  trafficNoticeList;
			char listFilePath[512] = {""};
			sprintf(listFilePath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,TRAFFICNOTICE_LIST_XMLFEIL);
			QFile listFile(listFilePath); 

			if (!listFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
				emit getTrafficNoticeListResult(BS_E_URL_FILE_ERROR, trafficNoticeList);
				return;
			}
			if(_downloadXml(&listFile,trafficUrl)<0)
			{
				IBD_PRINTF(DEBUG_INFOMATION,"ERROR :download xml errorl,TarfficList[%d]\n",BS_TRAFFIC_DOWNLOAD_ERROR)
				listFile.close();
				emit getTrafficNoticeListResult(BS_TRAFFIC_DOWNLOAD_ERROR, trafficNoticeList);
				return;
			}		
			if(_parseXmlList(&listFile,&trafficNoticeList) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION,"ERROR :parse xml errorl,TarfficList[%d]\n",BS_E_URL_XML_PARSE_ERROR)
				retval = BS_E_URL_XML_PARSE_ERROR;
			}
			else
			{
				IBD_PRINTF(DEBUG_INFOMATION,"GET LIST 0k ,TarfficList[%d]\n",BS_E_OK)
				retval = BS_E_OK;	
			}	
			listFile.close();
			emit getTrafficNoticeListResult(retval, trafficNoticeList);
		}
		else if( myTask.ThreadWorkMode == 1 )
		{
			char nodeFilePath[512] ={""};
			sprintf(nodeFilePath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,TRAFFICNOTICE_NODE_XMLFEIL);
			QFile nodeFile(nodeFilePath);
			if (!nodeFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
				emit getTrafficNoticeNodeResult(BS_E_URL_FILE_ERROR);	
			 	return;
			}
		
			if(_downloadXml(&nodeFile,myTask.TrafficNodeUrl.toUtf8().data())<0)
			{
				IBD_PRINTF(DEBUG_INFOMATION,"ERROR :download xml errorl,TarfficList[%d]\n",BS_TRAFFIC_DOWNLOAD_ERROR)
				nodeFile.close();
				emit getTrafficNoticeNodeResult(BS_TRAFFIC_DOWNLOAD_ERROR);
				return;
			}
		
			if(_parseXmlNode(&nodeFile,myTask.p_TrafficNode) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION,"parse xml errorl TrafficNode[%d]\n",BS_E_URL_XML_PARSE_ERROR)
				retval = BS_E_URL_XML_PARSE_ERROR;
			}
			else
			{
				retval = BS_E_OK;
			}
			nodeFile.close();
			emit getTrafficNoticeNodeResult(retval); 
		}

		else if( myTask.ThreadWorkMode == 2 )
		{
			TrafficNoticeList  m_trafficNoticeInfo;
			TrafficNoticeList  trafficNoticeInfo;
			int retvalList = BS_E_OK;
			int retvalNode = BS_E_OK;
			sprintf(trafficUrl, "http://%s/ap011/api/1.0/tu/getroadnotice?city=%s", apHost,myTask.city.toAscii().data());
			char infoFilePath[512] ={""};
			sprintf(infoFilePath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,TRAFFICNOTICE_INFO_XMLFEIL);
			QFile infoFile(infoFilePath); 

			if (!infoFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
				emit getTrafficNoticeInfoResult(BS_E_URL_FILE_ERROR, 0,trafficNoticeInfo);
				return;
			}

			if(_downloadXml(&infoFile,trafficUrl)<0)
			{
				IBD_PRINTF(DEBUG_INFOMATION,"ERROR :download xml errorl,TarfficList[%d]\n",BS_TRAFFIC_DOWNLOAD_ERROR)
				infoFile.close();
				emit getTrafficNoticeInfoResult(BS_TRAFFIC_DOWNLOAD_ERROR,0, trafficNoticeInfo);
				return;
			}
		
			if(_parseXmlList(&infoFile,&m_trafficNoticeInfo) < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION,"parse xml errorl TrafficNode[%d]\n",BS_E_URL_XML_PARSE_ERROR)
				retvalList = BS_E_URL_XML_PARSE_ERROR;
				infoFile.close();
			}
			else
			{
				infoFile.close();
				for(int i = 0;i <m_trafficNoticeInfo.size();i++ )
				{
					char contentFilePath[512] ={""};
					sprintf(contentFilePath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,TRAFFICNOTICE_CONTENT_XMLFEIL);
					QFile contentFile(contentFilePath); 

					if (!contentFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
					{
						emit getTrafficNoticeInfoResult(retvalList,BS_E_URL_FILE_ERROR, trafficNoticeInfo);
						return;
					}
					TrafficNoticeListNode m_content;
					if(_downloadXml(&contentFile, m_trafficNoticeInfo.at(i).url.toUtf8().data())<0)
					{
						IBD_PRINTF(DEBUG_INFOMATION,"ERROR :download xml errorl,TarfficList[%d]\n",BS_TRAFFIC_DOWNLOAD_ERROR)
						contentFile.close();
						emit getTrafficNoticeInfoResult(retvalList,BS_TRAFFIC_DOWNLOAD_ERROR, trafficNoticeInfo);
						return;
					}
					if(_parseXmlNode(&contentFile,&m_content)<0)
					{
						IBD_PRINTF(DEBUG_INFOMATION,"parse xml errorl TrafficNode[%d]\n",BS_E_URL_XML_PARSE_ERROR)
						retvalNode = BS_E_URL_XML_PARSE_ERROR;
					}
					else
					{
						m_content.from =  m_trafficNoticeInfo.at(i).from;
						m_content.time=  m_trafficNoticeInfo.at(i).time;
						m_content.title=  m_trafficNoticeInfo.at(i).title;
						trafficNoticeInfo.append(m_content);
						retvalNode = BS_E_OK;
					}
					contentFile.close();
				}
					
			}
			emit getTrafficNoticeInfoResult(retvalList, retvalNode,trafficNoticeInfo);
		}
	}
}

