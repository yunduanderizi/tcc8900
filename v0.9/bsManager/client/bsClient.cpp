#include "bsClient.h"
#include "bsGetTelephone.h"
#include "bsUcsInfoGather.h"
#include <QMutex>

#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "bsCommunalLock.h"




BsClientC  *BsClientC::thisObject = NULL;
BsClientC  *BsClientC::object()
{
	BsClientC *pTmp = NULL;
		
	
	bsManagerObjectLock();
	if( !thisObject )
	{
		thisObject = new BsClientC();
	}

	pTmp = thisObject;
	bsManagerObjectUnLock();

	
	return pTmp;
}

void BsClientC::releaseObject()
{
	bsManagerObjectLock();

	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();
	
}

BsClientC *BsClientC::getObject()
{
	BsClientC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}

BsClientC::BsClientC ()
	: m_bInitFlag(false),
	  m_pDbusClientToBsManagerProcess(NULL),
	  m_pDbusServer(NULL)
{

}

BsClientC::~BsClientC()
{

}

void BsClientC::_commonHandler(DBusNotifyC *notify)
{
	BsManagerProcessToClientCommonNotifyC *pCommonNotify=(BsManagerProcessToClientCommonNotifyC*)notify;

	if( notify )
	{
		if( thisObject )
		{
			thisObject->_common((BsManager_Command_Id)(pCommonNotify->m_iCommandId));
	 	}
	}
}

void BsClientC::_common(BsManager_Command_Id id)
{
	BsGetTelephoneC	*pTelephone = BsGetTelephoneC::getObject();
	BsUcsInfoGatherC	*UcsInfoGather = BsUcsInfoGatherC::getObject();
	
	switch(id)
	{
		case BM_COMMAND_TELEPHONE:
			if( pTelephone )
			{
				pTelephone->internalNotify(id);
			}
			break;

		case BM_COMMAND_UCSINFO:
			if( UcsInfoGather )
			{
				UcsInfoGather->internalNotify(id);
			}
			break;

		default:
			IBD_PRINTF(DEBUG_INFOMATION, "It's impossible to come here\n");
			break;
	}
}

int BsClientC::Init(DBusServerC *pDbusServer)
{
	if( m_bInitFlag )
	{
		return BS_E_OK;
	}
	
	m_pDbusServer = pDbusServer;
	
	if( NULL == m_pDbusServer )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "dbus server is NULL!\n")
		abort();
	}

	m_pDbusClientToBsManagerProcess = new DBusClientProxyC(ServerBsManagerProcess, ObjectBsManagerProcess);
	m_pDbusClientToBsManagerProcess->connect();

	m_pDbusClientToBsManagerProcess->registerNotify(NOTF_ID_CLIENT_TO_BSMANAGERPROCESS_COMMON, _commonHandler, BsManagerProcessToClientCommonNotifyC::factory);

	m_bInitFlag = true;
	
	return BS_E_OK;
}

void BsClientC::internalSendCommonCommand(BsManager_Command_Id id)
{
	if( !m_bInitFlag )
		return;
	
	ClientToBsManagerProcessCommonReqC commandRequest;
	commandRequest.m_iCommandId = id;

	((DBusClientC*)m_pDbusClientToBsManagerProcess)->sendService(&commandRequest);

	return;	
}

