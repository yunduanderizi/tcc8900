#include <QXmlStreamReader>
#include <curl.h>
#include <QDir>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>

#include "OBDDataGather.h"
#include "OBDDataUpload.h"
#include "i2ccontrol.h"


static QMutex gs_mutexForThreadSafe;
static QMutex gs_mutexWaitCondition;
static QWaitCondition gs_condExit;

OBDDataGatherC::OBDDataGatherC(BsCarInfoMonitorC *pBsCarInfoMonitor)
	: m_pBsCarInfoMonitor(pBsCarInfoMonitor),
	  m_pOBDDataUpload(NULL)
{
	gs_mutexForThreadSafe.lock();

	m_pOBDDataUpload = new OBDDataUploadC();
	
	connect(this, SIGNAL(finished()), this, SLOT(slotFinished()), Qt::QueuedConnection);

	gs_mutexForThreadSafe.unlock();
}

OBDDataGatherC::~OBDDataGatherC()
{
	gs_mutexForThreadSafe.lock();

	if( isRunning() )
	{
		m_bExit = true;

		gs_mutexWaitCondition.lock();
		gs_condExit.wakeAll();
		gs_mutexWaitCondition.unlock();

		gs_mutexForThreadSafe.unlock();
		wait();
		gs_mutexForThreadSafe.lock();
	}

	if( !m_pOBDDataUpload )
	{
		delete m_pOBDDataUpload;
		m_pOBDDataUpload = NULL;
	}
	
	gs_mutexForThreadSafe.unlock();
}

void OBDDataGatherC::slotFinished()
{
	gs_mutexForThreadSafe.lock();

	if( !m_bExit )
	{
		start();
	}
		
	gs_mutexForThreadSafe.unlock();
}

int OBDDataGatherC::stopGather()
{
	gs_mutexForThreadSafe.lock();
	
	if( isRunning() )
	{
		gs_mutexWaitCondition.lock();
		gs_condExit.wakeAll();
		gs_mutexWaitCondition.unlock();
		
		m_bExit = true;
	}

	gs_mutexForThreadSafe.unlock();
	return BS_E_OK;
}

int OBDDataGatherC::startGather()
{
	gs_mutexForThreadSafe.lock();

	m_bExit = false;

	if( !isRunning() )
	{
		start();
	}

	gs_mutexForThreadSafe.unlock();
	return BS_E_OK;
}

#define	WAIT_IN_SECOND(secondCount)			gs_mutexWaitCondition.lock(); \
												gs_condExit.wait(&gs_mutexWaitCondition, secondCount*1000); \
												gs_mutexWaitCondition.unlock();

#define	CHECK_EXIT								if( m_bExit)break;
		
void OBDDataGatherC::run()
{
	while(1)
	{
		CHECK_EXIT

		//TODO          gather OBD data

/*
		//upload OBD data
		OBDDataUploadTask task;
		m_pOBDDataUpload->uploadOBDData(&task);

		CHECK_EXIT

		//wait for uploading
		while(1)
		{
			WAIT_IN_SECOND(5)

			if( task.result == OBD_DATA_UPLOAD_SUCCESS )
				break;
			else( task.result == OBD_DATA_UPLOAD_FAIL )
				break;
			else
			{

			}

			CHECK_EXIT
		}
*/
		CHECK_EXIT

		m_pBsCarInfoMonitor->feedbackOBDCheckResult(OBD_CHECK_SUCCESS);//TODO feedback OBD check result

		//wait for next OBD check
		WAIT_IN_SECOND(3600)
	}

}


