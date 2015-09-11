#include "carDataUpload.h"
#include <QXmlStreamReader>
#include <curl.h>
#include <QDir>
#include <QDebug>


CarDataUploadC::CarDataUploadC()
{
	connect(this, SIGNAL(finished()), this, SLOT(slotFinished()), Qt::QueuedConnection);
}

CarDataUploadC::~CarDataUploadC()
{
	if( isRunning() )
	{
		cancelOperation();

		wait();
	}
}

void	CarDataUploadC::cancelOperation()
{
	m_MutexForThreadSafe.lock();

	if( isRunning() )
	{
		BsBaseC::stopHttp();
		m_bExit = true;

		m_MutexWaitCondition.lock();
		m_CondExit.wakeAll();
		m_MutexWaitCondition.unlock();
	}

	m_MutexForThreadSafe.unlock();
}

void CarDataUploadC::slotFinished()
{
	m_MutexForThreadSafe.lock();

	if( (!(m_task.bUploading)) && (!isRunning()) && (!m_bExit) )
	{
		m_bExit = false;
		start();
	}
		
	m_MutexForThreadSafe.unlock();
}

int CarDataUploadC::uploadCarData(CarDataDownloadTask task)
{
	m_MutexForThreadSafe.lock();

	m_task = task;
	m_task.bUploading = false;

	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}
	else
	{
		m_MutexWaitCondition.lock();
		m_CondExit.wakeAll();
		m_MutexWaitCondition.unlock();
	}
	
	m_MutexForThreadSafe.unlock();
	return BS_E_OK;
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


int CarDataUploadC::_postCarInfoData(CarDataDownloadTask myTask)
{
	DataBlock dataBlock;
	int retval = -1;
	CURL *curl = curl_easy_init();
	if(curl)
	{
		char postData[200];
		strcpy(postData,myTask.postfield.toAscii().data());
		curl_easy_setopt(curl, CURLOPT_URL, myTask.url.toAscii().data());
		curl_easy_setopt(curl, CURLOPT_COOKIE, myTask.cookie.toAscii().data());
	//	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, myTask.postfield.toAscii().data());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	   	
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);	
	   	
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, m_bExit_func);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &m_bExit);
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *)&dataBlock);
     		
		CURLcode res = curl_easy_perform(curl);
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
	if( retval != 0 )	
	{
		IBD_PRINTF(DEBUG_INFOMATION, "xml parse error, xml[%s]!\n", dataBlock.buffer)
	}
	curl_easy_cleanup(curl);
	
	return retval;
}

void CarDataUploadC::run()
{
	CarDataDownloadTask		myTask;
	bool bReUpload = false;

	while(1)
	{
		if( m_bExit )
			break;
		
		m_MutexForThreadSafe.lock();
		
		if( !(m_task.bUploading) )
		{
			m_task.bUploading = true;
			myTask = m_task;

			m_MutexForThreadSafe.unlock();
		}
		else
		{
			m_MutexForThreadSafe.unlock();

			if( bReUpload )
			{
				m_MutexWaitCondition.lock();
				m_CondExit.wait(&m_MutexWaitCondition, 10000);
				m_MutexWaitCondition.unlock();
			}
			else
			{
				m_MutexForThreadSafe.unlock();
				break;
			}
		}

		//start to upload
		if(_postCarInfoData(myTask)<0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "Error: post  data  error\n");
			bReUpload = true;
		}
		else
		{
			bReUpload = false;
		}
	}
}
