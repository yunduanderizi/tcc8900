/**
 * @file   dataManagerServer.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111017
 * 
 * @brief  
 *      cpp file of dataManager module's interface class 
 * 
 */
 
#include "dataManager.h"
#include "dataManagerServer.h"


namespace DataManager
{

DataManagerServerC* DataManagerServerC::thisObject = NULL;
DataManagerServerC* DataManagerServerC::object()
{
	DataManagerServerC *pTmp = NULL;

	if( !thisObject )
	{
		thisObject = new DataManagerServerC();
	}

	pTmp = thisObject;
	
	return pTmp;
}

void DataManagerServerC::releaseObject()
{
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
}

void DataManagerServerC::_updateReadWriteUserDataFlagHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
	UpdateReadWriteUserDataFlagReqC *req=(UpdateReadWriteUserDataFlagReqC*)pRequest;

	if( pRequest )
	{
		if( thisObject )
		{
			
			thisObject->_updateReadWriteUserDataFlag(req->m_bReadWriteUserData);
			thisObject->_sendReadWriteUserDataFlag(req->m_bReadWriteUserData);
	 	}
	}
}

DataManagerServerC::DataManagerServerC()
	: m_pDbusServer(NULL)
{
	connect(m_pDataManager, SIGNAL(sigSendReadWriteUserDataFlag(bool)), this, SLOT(_sendReadWriteUserDataFlag(bool)), Qt::DirectConnection);
}

DataManagerServerC::~DataManagerServerC()
{

}

int DataManagerServerC::Init(DBusServerC *pDbusServer)
{
	m_pDbusServer = pDbusServer;
	
	m_pDbusServer->registerService(REQ_ID_UPDATE_READWRITE_USERDATA_FLAG, _updateReadWriteUserDataFlagHandler, UpdateReadWriteUserDataFlagReqC::factory);

	m_bInitFlag = true;
	return 0;
}

void DataManagerServerC::_sendReadWriteUserDataFlag(bool bReadWriteUserData)
{
	UpdateReadWriteUserDataFlagNotifyC notify;
	notify.m_bReadWriteUserData = bReadWriteUserData;
	m_pDbusServer->sendNotify(&notify);
	
}

}

