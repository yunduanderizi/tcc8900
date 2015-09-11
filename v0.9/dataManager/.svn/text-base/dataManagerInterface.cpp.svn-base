/**
 * @file   dataManagerInterface.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111017
 * 
 * @brief  
 *      cpp file of dataManager module's interface base class 
 * 
 */
 
#include "dataManager.h"
#include "dataManagerInterface.h"


namespace DataManager
{

DataManagerInterfaceC::DataManagerInterfaceC()
	: m_pDataManager(NULL),
	  m_bInitFlag(false)
{
	qRegisterMetaType<DM_ERROR_INFO>("DM_ERROR_INFO");
	qRegisterMetaType<USER_DATA_SURPOR_TYPE>("USER_DATA_SURPOR_TYPE");
	m_pDataManager = new DataManagerC();
	connect(m_pDataManager, SIGNAL(sigBackupUserDataProgress(int)), this, SIGNAL(sigBackupUserDataProgress(int)), Qt::DirectConnection);
	connect(m_pDataManager, SIGNAL(sigRevertUserDataProgress(int)), this, SIGNAL(sigRevertUserDataProgress(int)), Qt::DirectConnection);
	connect(m_pDataManager, SIGNAL(sigRepairUserDataProgress(int)), this, SIGNAL(sigRepairUserDataProgress(int)), Qt::DirectConnection);
	connect(m_pDataManager, SIGNAL(sigUserDataHandleResult(DM_ERROR_INFO,QString)), this, SIGNAL(sigUserDataHandleResult(DM_ERROR_INFO,QString)), Qt::DirectConnection);
	connect(m_pDataManager, SIGNAL(sigReadWriteUserDataChanged()), this, SIGNAL(sigReadWriteUserDataChanged()), Qt::DirectConnection);
	
}

DataManagerInterfaceC::~DataManagerInterfaceC()
{
	if( m_pDataManager )
	{
		delete m_pDataManager;
		m_pDataManager = NULL;
	}
}

void DataManagerInterfaceC::_updateReadWriteUserDataFlag(bool bReadWriteUserData)
{
	m_pDataManager->updateReadWriteUserDataFlag(bReadWriteUserData);
}
QString DataManagerInterfaceC::findPluginOfType(USER_DATA_SURPOR_TYPE dataType)
{
	return m_pDataManager->findPluginOfType(dataType);
}
int DataManagerInterfaceC::backupUserData(QString sAddress)
{
	if( !m_bInitFlag )
		return DATA_E_NOT_INIT;

	m_pDataManager->backupUserData(sAddress);

	return DATA_E_OK;
}

int DataManagerInterfaceC::revertUserData(QString sAddress)
{
	if( !m_bInitFlag )
		return DATA_E_NOT_INIT;

	m_pDataManager->revertUserData(sAddress);

	return DATA_E_OK;
}

int DataManagerInterfaceC::requestReadWriteUserData()
{
	if( !m_bInitFlag )
		return DATA_E_NOT_INIT;

	m_pDataManager->requestReadWriteUserData();

	return DATA_E_OK;
}

int DataManagerInterfaceC::reportError(USER_DATA_SURPOR_TYPE dataType)
{
	if( !m_bInitFlag )
		return DATA_E_NOT_INIT;

	m_pDataManager->reportError(dataType);

	return DATA_E_OK;
}

int DataManagerInterfaceC::startupCheck()
{
	if( !m_bInitFlag )
		return DATA_E_NOT_INIT;

	return m_pDataManager->startupCheck();

//	return DATA_E_OK;
}

int DataManagerInterfaceC::repairUserData(QString sAddress)
{
	if( !m_bInitFlag )
		return DATA_E_NOT_INIT;

	m_pDataManager->repairUserData(sAddress);

	return DATA_E_OK;
}

int DataManagerInterfaceC::ignoreError()
{
	if( !m_bInitFlag )
		return DATA_E_NOT_INIT;

	m_pDataManager->ignoreError();

	return DATA_E_OK;
}
	
int DataManagerInterfaceC::ignoreErrorThisTime()
{
	if( !m_bInitFlag )
		return DATA_E_NOT_INIT;

	m_pDataManager->ignoreErrorThisTime();

	return DATA_E_OK;
}

int DataManagerInterfaceC::getReadWriteUserDataFlag(bool* pbFlag)
{
	if( !m_bInitFlag )
		return DATA_E_NOT_INIT;

	*pbFlag = m_pDataManager->isReadWriteUserData();
	
	return DATA_E_OK;
}




}

