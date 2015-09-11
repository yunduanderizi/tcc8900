/**
 * @file   bsNews.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date   20110124
 * 
 * @brief  
 *      Implement for news download 
 * 
 */

#include "bsNews.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <curl.h>
#include <types.h>
#include <QXmlStreamReader>

#include <fstream>
#include <sstream>
#include <string.h>
#include "json/config.h"
#include "json/json.h"
#include <stdlib.h>
#include "bsLocationUpload.h"
#include "common_config.h"
#include "bsCommunalLock.h"
using namespace std;

#define BS_NEWS_E_CALLBACK			  	42

#define NEWSTYPE_LIST_XMLFEIL  "newsFile/newsType.xml"
#define NEWS_DEFAULT_TYPE_LIST_XMLFEIL  "newsFile/newsDefaultType.xml"
#define NEWSCONTENT_LIST_XMLFEIL  "newsFile/newsContent.xml"



int m_bExit_func(bool *m_bExit, double DlT, double DlN, double UlT, double UlN)
{		
	if ( *m_bExit == true)
	{
		return -1;
	}	
	return 0;
}

static size_t write_file(void *ptr, size_t size, size_t nmemb, void *stream)
{
	QIODevice *pFile = (QIODevice *)stream;
	qint64 writeen = pFile->write((char *)ptr, (qint64)(size*nmemb));
	if(writeen < nmemb*size)
	{
		return -1;
	}
	return writeen;
}
BsNewsC *BsNewsC::thisObject = NULL;
BsNewsC *BsNewsC::object()
{

	BsNewsC *pTmp = NULL;
	bsManagerObjectLock();
	if( !thisObject )
	{
		thisObject = new BsNewsC();
	}
	pTmp = thisObject;
	bsManagerObjectUnLock();
	return pTmp;
}

void BsNewsC::releaseObject()
{
	bsManagerObjectLock();
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();
}

BsNewsC *BsNewsC::getObject()
{
	BsNewsC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}
BsNewsC::BsNewsC()
	:m_bExit(false)
{
	connect(this, SIGNAL(finished()), this, SLOT(slotFinished()), Qt::QueuedConnection);
	QDir newsPath;
	char fileDir[512]= {""};
	sprintf(fileDir,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,"newsFile/");
	if(false == newsPath.mkpath(fileDir))
	{
		IBD_PRINTF(DEBUG_INFOMATION,"mkdir path[%s] fail\n", fileDir)
		return;
	}
}

BsNewsC::~BsNewsC()
{
	if( isRunning() )
	{
		cancelOperation();

		wait();
	}
}
int BsNewsC::startGetNewsDefaultTypeList()
{
	m_mutexForThreadSafe.lock();
	
	m_iThreadWorkMode = 0;
//	DownloadNewsTask myTask;
//	m_list_type.append(myTask);
	
	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}
	
	m_mutexForThreadSafe.unlock();
	return BS_E_OK;
}
int BsNewsC::startGetNewsCustomTypeList(QString newsId)
{
	m_mutexForThreadSafe.lock();
		
	DownloadNewsTask myTask;
	myTask.newsId = newsId;
	
	m_list_type.append(myTask);
	
	m_iThreadWorkMode = 1;
	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}
	
	m_mutexForThreadSafe.unlock();
	return BS_E_OK;
}

int BsNewsC::startGetNewsContentNode(NewsNoteicNode *pNewsNoteNode)
{
	m_mutexForThreadSafe.lock();
	DownloadNewsTask myTask;

	myTask.url = "";
	myTask.url += pNewsNoteNode->url;
	
	m_list_content.append(myTask);
	
	m_iThreadWorkMode = 2;
	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}
	m_mutexForThreadSafe.unlock();
	return BS_E_OK;
}

void BsNewsC::slotFinished()
{
	m_mutexForThreadSafe.lock();
	if( (!m_list_type.isEmpty()) && (!isRunning()) )
	{
		m_bExit = false;
		start();
	}
	else if( (!m_list_content.isEmpty()) && (!isRunning()) )
	{
		m_bExit = false;
		start();
	}
		
	m_mutexForThreadSafe.unlock();
}

void BsNewsC::cancelOperation()
{
	m_mutexForThreadSafe.lock();

	m_list_type.clear();
	m_list_content.clear();
	if( isRunning() )
	{
		BsBaseC::stopHttp();
		m_bExit = true;
	}

	m_mutexForThreadSafe.unlock();
}

int BsNewsC::_parseDefaultList(QFile * file, NewsTypeList * pNewsDefaultType)
{
	QIODevice *device = (QIODevice *)file;
	device->seek(0);
	QXmlStreamReader xml;
	xml.setDevice(device);
	NewsNoteicNode myNode;
 	int retval_url = -1;
	int retval_title = -1;
	while(!xml.atEnd())
	{
		xml.readNext();	
		if( xml.isStartElement() && xml.name()=="item" )
		{
			if(xml.attributes().value("key").toString().toInt()>= 0 )
			{
				xml.readNext();
				myNode.clear();
				if( xml.isStartElement() && xml.name()=="item" )
				{
					if( xml.attributes().value("key") == QString("0") )
					{
					
						myNode.url = xml.readElementText().toUtf8().data();
						//qDebug()<<"url:"<<myNode.url ;
						retval_url = 0;
				
					}	
					xml.readNext();
					 if( xml.isStartElement() && xml.name()=="item" )
					{
						if(xml.attributes().value("key") == QString("1") )
						{
							myNode.title= xml.readElementText().toUtf8().data();
							//qDebug()<<"title:"<<myNode.title;
							retval_title = 0;
											
						}
					}
					xml.readNext();
					if(xml.isEndElement() && xml.name()=="item")
					{
				
						if(retval_title == 0 && retval_url == 0)
						{
							pNewsDefaultType->append(myNode);	
						}
						else
							return -1;
					}
				
				}
				
			}
		}
		
	}
				
	return 0;				
}

int BsNewsC::_parseCustomList(QFile * file, NewsTypeList * pNewsCustomType)
{
	QIODevice *device = (QIODevice *)file;
	device->seek(0);
	QXmlStreamReader xml;
	xml.setDevice(device);
	NewsNoteicNode myNode;
 	int retval_status = -1;
	int retval_data = -1;
	int retval_id = -1;
	int retval_content = -1;
	int retval_time = -1;
	while(!xml.atEnd())
	{
		xml.readNext();	
		if( xml.isStartElement() && xml.name()=="item" )
		{
			if( xml.attributes().value("key") == QString("resp_status") )
			{
				if( xml.readElementText() == QString("OK") )
				{
					retval_status= 0;
					myNode.clear();
				}
			}
		
			else if(xml.attributes().value("key") == QString("news_id") )
			{
			
				myNode.id = xml.readElementText().toUtf8().data();
				retval_id = 0;

			}
			else if(xml.attributes().value("key") == QString("news_content") )
			{
				
				char *data_tmp = xml.readElementText().toUtf8().data();
				
				Json::Value object;
				Json::Reader reader;
				
				if(!reader.parse(data_tmp, object))
				{
					IBD_PRINTF(DEBUG_INFOMATION,"parse json [%s] fail\n", data_tmp)
				}
				string title_tmp = object["title"].asString();
				myNode.title = title_tmp.c_str();
				string url_tmp = object["url"].asString();
				myNode.url = url_tmp.c_str();
				retval_content = 0;
			
			}
			else if(xml.attributes().value("key") == QString("last_modify_timestamp") )
			{
				myNode.time = xml.readElementText().toUtf8().data();	
				retval_time = 0;
			}
			
		}
		else if(xml.isEndElement() && xml.name() == "item" )
		{
			if(retval_id == 0 && retval_content == 0 && retval_time == 0)
			{
				pNewsCustomType->append(myNode);
				retval_content = -1;
				retval_id = -1;
				retval_time = -1;
				
			}	
		}
		
	}		
	return 0;				
}




int BsNewsC::_parseXmlContent(QFile *file, NewsContentNodeList *pNewsContent)
{
	QIODevice *device = (QIODevice *)file;
	device->seek(0);
	QXmlStreamReader xml;
	xml.setDevice(device);
	NewsNoteicNode my_newsContent;
	int retval_title = -1;
	int retval_link = -1;
	int retval_description = -1;
	int retval_item = -1;
	while(!xml.atEnd())
	{
		xml.readNext();
		if( xml.isStartElement() && xml.name() == "item" )
		{
			my_newsContent.clear();
		}
		else if( xml.isStartElement() && xml.name() == "title" )
		{
			QString tmpTitle = xml.readElementText().toUtf8().data();
			my_newsContent.title = tmpTitle.trimmed();
			retval_title = 0;
		}
		else if( xml.isStartElement() && xml.name() == "link" )
		{
			my_newsContent.link= xml.readElementText().toUtf8().data();	
			retval_link = 0;
		}
		else if(xml.isStartElement() && xml.name() == "description")
		{
			QString tmpDescription = xml.readElementText().toUtf8().data();
			my_newsContent.description = tmpDescription.trimmed();
			retval_description = 0;
			
		}
		else if(xml.isEndElement() && xml.name() == "item" )
		{
			if(retval_title== 0 && retval_description== 0 && retval_link== 0)
			{
				pNewsContent->append(my_newsContent);
				retval_title= -1;
				retval_description= -1;
				retval_link= -1;
			}	
			
		}
		
	}
return 0;

}


void BsNewsC::run()
{
	NewsTypeList newsType;	
	NewsContentNodeList newsContent;
	while(1)
	{
	
		if(m_bExit)
		{		
			IBD_PRINTF(DEBUG_INFOMATION,"thread by cancel\n")
			break;
		}

		if(m_iThreadWorkMode == 0 )
		{
			int retval = -1;
			char newsUrl[URL_MAX_LENGTH];
			char apApiHost[URL_MAX_LENGTH];
			retval = BsBaseC::getApApiHost(Ap_News,apApiHost);    
                    if(retval < 0)
                    {
                            IBD_PRINTF(DEBUG_INFOMATION,"getApApiHost fail\n")

		        	emit sigGetNewsDefaultTypeListResult(retval, newsType);
                            return;
                    }
			char filePath[512]= {""};
			sprintf(filePath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,NEWS_DEFAULT_TYPE_LIST_XMLFEIL);
					
			QFile defaultTypeFile(filePath);
			
			if (!defaultTypeFile.open(QIODevice::ReadWrite | QIODevice::Truncate| QIODevice::Text))
			{
				IBD_PRINTF(DEBUG_INFOMATION,"open file fail:[%s]\n",filePath)
				retval = BS_NEW_FILE_OPEN_FAIL;
				emit sigGetNewsDefaultTypeListResult(retval, newsType);
	       		return;
			}
		
			sprintf(newsUrl, "http://%s/ap003/api/1.0/tu/getrsslist/",apApiHost);
		
			CURL *curl = curl_easy_init();
		
			if(curl)
			{		
				curl_easy_setopt(curl,CURLOPT_URL,newsUrl);
				printf("news Connecting to ->>>>%s\n",newsUrl);
			
				curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
				
		   		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		   		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
		   		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &m_bExit);
				
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file);
	        		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&defaultTypeFile);
			
				CURLcode res = curl_easy_perform(curl);
			
				if (res != CURLE_OK)
				{
					IBD_PRINTF(DEBUG_INFOMATION,"curl error:[%s]\n",curl_easy_strerror(res))
					retval = BS_NEW_DOWNLOAD_ERROR;
					if (res ==  BS_NEWS_E_CALLBACK)
					{
						defaultTypeFile.close();
						curl_easy_cleanup(curl);
						IBD_PRINTF(DEBUG_INFOMATION,"curl by cancel\n")
						return;
					}
					
				}
				else
				{
	
					if(_parseDefaultList(&defaultTypeFile,&newsType) < 0 )
					{
						IBD_PRINTF(DEBUG_INFOMATION,"login ucs.chinatsp.com fail, parse xml errorl,NewsList[%s]\n",NEWS_DEFAULT_TYPE_LIST_XMLFEIL)
						retval = BS_NEW_PARSE_XML_ERROR;
					}
					else
						retval = BS_E_OK;	
				}  
			
			}
			defaultTypeFile.close();
			curl_easy_cleanup(curl);	

			emit sigGetNewsDefaultTypeListResult(retval, newsType);
			break;		
		}
		
		else if( m_iThreadWorkMode == 1 )
		{
			DownloadNewsTask myTask;
			m_mutexForThreadSafe.lock();
			if( !m_list_type.isEmpty() )
			{
				myTask = m_list_type.takeFirst();
			}
			else
			{
				m_mutexForThreadSafe.unlock();
				return;
			}
			m_mutexForThreadSafe.unlock();
			
			int retval = -1;
			char newsUrl[URL_MAX_LENGTH];
			char ucsApiHost[URL_MAX_LENGTH];
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
				m_mutexForThreadSafe.lock();
                		m_list_type.clear();
            		    	m_mutexForThreadSafe.unlock();
			 	emit sigGetNewsCustomTypeListResult(retval, newsType);				
				return;
			}

			if( 0 == strcmp(uuid, "") )
			{
				IBD_PRINTF(DEBUG_INFOMATION,"uuid is void\n")
				emit sigGetNewsCustomTypeListResult(BS_E_GET_UUID_ERROR, newsType);
				return;
			}
			retval = BsBaseC::getUcsApiHost(ucsApiHost,uuid);    
                        if(retval < 0)
                        {
                                IBD_PRINTF(DEBUG_INFOMATION,"getUcsApiHost fail\n")
                                m_mutexForThreadSafe.lock();
                                m_list_type.clear();
                                m_mutexForThreadSafe.unlock();
                                emit sigGetNewsCustomTypeListResult(retval, newsType);
                                return;
                        }
			char filePath[512]= {""};
			sprintf(filePath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,NEWSTYPE_LIST_XMLFEIL);
					
			QFile typeFile(filePath);
			if (!typeFile.open(QIODevice::ReadWrite | QIODevice::Truncate| QIODevice::Text))
			{
				IBD_PRINTF(DEBUG_INFOMATION,"open file fail:[%s]\n",filePath)
				retval = BS_NEW_FILE_OPEN_FAIL;
         			emit sigGetNewsCustomTypeListResult(retval, newsType);
	       		return;
			}
		
			sprintf(newsUrl, "http://%s/api/1.0/user/%s/info/news/%s/",ucsApiHost,uuid, myTask.newsId.toAscii().data());
		
			CURL *curl = curl_easy_init();
			
			if(curl)
			{		
				curl_easy_setopt(curl,CURLOPT_URL,newsUrl);
				printf("news Connecting to %s->>>>\n",newsUrl);

				curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
				
		   		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		   		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
		   		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &m_bExit);
				
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file);
	        		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&typeFile);
			
				CURLcode res = curl_easy_perform(curl);
			
				if (res != CURLE_OK)
				{
					IBD_PRINTF(DEBUG_INFOMATION,"curl error:[%s]\n",curl_easy_strerror(res))
					retval = BS_NEW_DOWNLOAD_ERROR;
					if (res ==  BS_NEWS_E_CALLBACK)
					{
						typeFile.close();
						curl_easy_cleanup(curl);
						IBD_PRINTF(DEBUG_INFOMATION,"curl by cancel\n")
						return;
					}
				}
				else
				{
					if(_parseCustomList(&typeFile,&newsType) < 0 )
					{
						IBD_PRINTF(DEBUG_INFOMATION,"login ucs.chinatsp.com fail, parse xml errorl\n")
						retval = BS_NEW_PARSE_XML_ERROR;
					}
					else
						retval = BS_E_OK;	
				}  
			
			}
			typeFile.close();
			curl_easy_cleanup(curl);	
			emit sigGetNewsCustomTypeListResult(retval, newsType);
			break;
		}

		else if(m_iThreadWorkMode == 2 )
		{
			int retval;
			DownloadNewsTask myTask;
			m_mutexForThreadSafe.lock();
			if( !m_list_content.isEmpty() )
			{
				myTask = m_list_content.takeFirst();
			}
			else
			{
				m_mutexForThreadSafe.unlock();
				return;
			}
			m_mutexForThreadSafe.unlock();
			
			char filePath[512]= {""};
			sprintf(filePath,"%s%s",INCAR_COMMON_MICRO_TMP_DIRECTORY,NEWSCONTENT_LIST_XMLFEIL);
					
			QFile contentFile(filePath); 
			if(!contentFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
			{
				IBD_PRINTF(DEBUG_INFOMATION,"open file fail:[%s]\n",filePath)
				retval = BS_NEW_FILE_OPEN_FAIL;
				emit sigGetNewsContentNodeResult(retval, newsContent);
				return;
			
			}
			CURL *curl = curl_easy_init();
			char *url = myTask.url.toAscii().data();
			if(curl)
			{		
				printf("Connecting to %s->>>>\n",url);
				curl_easy_setopt(curl,CURLOPT_URL,url);
				curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
				
		   		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);	
		   		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
		   		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &m_bExit);
		   
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file);
	      	 		curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)&contentFile);
	     			
				CURLcode res = curl_easy_perform(curl);
			
				if (res != CURLE_OK)
				{
					IBD_PRINTF(DEBUG_INFOMATION,"curl error:[%s]\n",curl_easy_strerror(res))
					retval = BS_NEW_DOWNLOAD_ERROR;
					if (res ==  BS_NEWS_E_CALLBACK)
					{
						curl_easy_cleanup(curl);
						contentFile.close();
						IBD_PRINTF(DEBUG_INFOMATION,"curl by cancel\n")
						return;
					}
				}
				else
				{		
					if(_parseXmlContent(&contentFile,&newsContent) < 0 )
					{
						IBD_PRINTF(DEBUG_INFOMATION,"parse xml error\n");
						retval = BS_NEW_PARSE_XML_ERROR;
					}
					else
						retval = BS_E_OK;	
				} 
			
			}
			contentFile.close();
			curl_easy_cleanup(curl);	
			emit sigGetNewsContentNodeResult(retval, newsContent);
			break;
		}
	}
}


