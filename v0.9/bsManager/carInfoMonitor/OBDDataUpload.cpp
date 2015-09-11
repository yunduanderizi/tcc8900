#include <QXmlStreamReader>
#include <curl.h>
#include <QDir>
#include <QDebug>

#include "OBDDataUpload.h"


OBDDataUploadC::OBDDataUploadC()
{

}

OBDDataUploadC::~OBDDataUploadC()
{
	if( isRunning() )
	{
		cancelOperation();
		
		wait();
	}
}

void OBDDataUploadC::slotFinished()
{
	m_mutexForThreadSafe.lock();

	if( (!m_list.isEmpty()) && (!isRunning()) && (!m_bExit) )
	{
		m_bExit = false;
		start();
	}
		
	m_mutexForThreadSafe.unlock();
}

int OBDDataUploadC::uploadOBDData(OBDDataUploadTask* pTask)
{
	m_mutexForThreadSafe.lock();

	m_list.append(pTask);
	
	if( !isRunning() )
	{
		m_bExit = false;
		start();
	}
	
	m_mutexForThreadSafe.unlock();

	return 0;
}

void OBDDataUploadC::cancelOperation()
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

void OBDDataUploadC::run()
{
	OBDDataUploadTask* pTask = NULL;

	while(1)
	{
		m_mutexForThreadSafe.lock();
		if( !m_list.isEmpty() )
		{
			pTask = m_list.takeFirst();
		}
		else
		{
			m_mutexForThreadSafe.unlock();
			break;
		}
		m_mutexForThreadSafe.unlock();
		
		while(1)
		{
			//TODO upload OBD message pointed by pTask
			//if uploading success, set bSuccess of pTask to true, and break
			if( 1 )
			{
				pTask->result = OBD_DATA_UPLOAD_SUCCESS;
				break;
			}
			
			if( pTask->iCount == 0 )
			{
				pTask->result = OBD_DATA_UPLOAD_FAIL;
				break;
			}
			else
			{
				m_mutexWaitCondition.lock();
				m_condExit.wait(&m_mutexWaitCondition, 10000);
				m_mutexWaitCondition.unlock();

				pTask->iCount--;
			}
			
			if( m_bExit )
				break;
		}

		if( m_bExit )
			break;
	}

}

