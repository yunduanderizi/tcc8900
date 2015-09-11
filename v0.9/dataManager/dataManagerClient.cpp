/**
 * @file   dataManagerClient.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111017
 * 
 * @brief  
 *      cpp file of dataManager module's interface class 
 * 
 */

#include "dataManager.h"
#include "dataManagerClient.h"

namespace DataManager
{

DataManagerClientC *DataManagerClientC::thisObject = NULL;
DataManagerClientC *DataManagerClientC::object()
{
	DataManagerClientC *pTmp = NULL;

	if( !thisObject )
	{
		thisObject = new DataManagerClientC();
	}

	pTmp = thisObject;
	
	return pTmp;
}

void DataManagerClientC::releaseObject()
{
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
}

void DataManagerClientC::_updateReadWriteUserDataFlagHandler(DBusNotifyC *notify)
{
	UpdateReadWriteUserDataFlagNotifyC *pNotify=(UpdateReadWriteUserDataFlagNotifyC*)notify;
	if( pNotify )
	{
		if( thisObject )
			thisObject->_updateReadWriteUserDataFlag(pNotify->m_bReadWriteUserData);
	}
}

DataManagerClientC::DataManagerClientC()
	: m_pDbusClient(NULL)
{
	connect(m_pDataManager, SIGNAL(sigSendReadWriteUserDataFlag(bool)), this, SLOT(_sendReadWriteUserDataFlag(bool)), Qt::DirectConnection);
}

DataManagerClientC::~DataManagerClientC()
{

}
void DataManagerClientC::Init(DBusClientC *pDbusClient)
{
	m_pDbusClient = pDbusClient;
	
	m_pDbusClient->registerNotify(NOTF_ID_UPDATE_READWRITE_USERDATA_FLAG, _updateReadWriteUserDataFlagHandler, UpdateReadWriteUserDataFlagNotifyC::factory);
	m_bInitFlag = true;
}

void DataManagerClientC::_sendReadWriteUserDataFlag(bool bReadWriteUserData)
{
	UpdateReadWriteUserDataFlagReqC request;
	request.m_bReadWriteUserData = bReadWriteUserData;

//	DBusReplyC *reply = NULL;

//	m_pDbusClient->sendService(&request, reply, 0);
	((DBusClientC*)m_pDbusClient)->sendService(&request);
}

}

