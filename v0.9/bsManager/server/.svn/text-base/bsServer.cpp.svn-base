#include "bsServer.h"

#include <QMutex>

#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "bsCommunalLock.h"




BsServerC  *BsServerC::thisObject = NULL;
BsServerC  *BsServerC::object()
{
	BsServerC *pTmp = NULL;
		
	bsManagerObjectLock();
	
	if( !thisObject )
	{
		thisObject = new BsServerC();
	}

	pTmp = thisObject;
	bsManagerObjectUnLock();
	
	return pTmp;
}

void BsServerC::releaseObject()
{
	bsManagerObjectLock();

	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();
}

BsServerC *BsServerC::getObject()
{
	BsServerC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}

BsServerC::BsServerC ()
	: m_pDbusServer(NULL)
{
	m_pDbusServer = new DBusServerC(ServerBsManagerProcess, ObjectBsManagerProcess);

	m_pDbusServer->connect();

	m_pDbusServer->registerService(REQ_ID_CLIENT_TO_BSMANAGERPROCESS_COMMON, _commonHandler, ClientToBsManagerProcessCommonReqC::factory);
}

BsServerC::~BsServerC()
{

}

DBusServerC* BsServerC::getDbusServer()
{
	return m_pDbusServer;
}

void BsServerC::_commonHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
	ClientToBsManagerProcessCommonReqC *pCommonRequest=(ClientToBsManagerProcessCommonReqC*)pRequest;

	if( pRequest )
	{
		if( thisObject )
		{
			BsManagerProcessToClientCommonNotifyC notify;
			notify.m_iCommandId = pCommonRequest->m_iCommandId;

			thisObject->m_pDbusServer->sendNotify(&notify);
	 	}
	}
}

