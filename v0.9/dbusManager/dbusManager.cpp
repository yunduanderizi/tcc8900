#include <QMutex>
#include "dbusManager.h"


static QMutex gs_mutexForThreadSafe;


DbusManagerC  *DbusManagerC::thisObject = NULL;
DbusManagerC  *DbusManagerC::object()
{
	DbusManagerC *pTmp = NULL;
		
	gs_mutexForThreadSafe.lock();
	
	if( !thisObject )
	{
		thisObject = new DbusManagerC();
	}

	pTmp = thisObject;

	gs_mutexForThreadSafe.unlock();
	
	return pTmp;
}

void DbusManagerC::releaseObject()
{
	gs_mutexForThreadSafe.lock();

	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	
	gs_mutexForThreadSafe.unlock();
}

DbusManagerC::DbusManagerC()
	: m_pDbusServer(NULL)
{
	m_DbusClientList.clear();
}

DbusManagerC::~DbusManagerC()
{
	
}

DBusServerC* DbusManagerC::getServer()
{
	DBusServerC *pTmp = NULL;

	gs_mutexForThreadSafe.lock();

	pTmp = m_pDbusServer;

	gs_mutexForThreadSafe.unlock();

	return pTmp;
}

void DbusManagerC::setServer(DBusServerC *pServer)
{
	gs_mutexForThreadSafe.lock();

	m_pDbusServer = pServer;
	
	gs_mutexForThreadSafe.unlock();

	return;
}

DBusClientProxyC* DbusManagerC::getClient(const char* pClientName)
{
	DBusClientProxyC *pTmp = NULL;
	
	gs_mutexForThreadSafe.lock();

	for(int i=0; i<m_DbusClientList.size(); i++)
	{
		if( 0 == m_DbusClientList.at(i).clientName.compare( QString(pClientName) ) )
		{
			pTmp = m_DbusClientList.at(i).pClient;
		}
	}
	
	gs_mutexForThreadSafe.unlock();

	return pTmp;
}

void DbusManagerC::setClient(DBusClientProxyC *pClient, const char* pClientName)
{
	DBusClientProxyC *pTmp = NULL;
	
	gs_mutexForThreadSafe.lock();

	for(int i=0; i<m_DbusClientList.size(); i++)
	{
		if( 0 == m_DbusClientList.at(i).clientName.compare( QString(pClientName) ) )
		{
			m_DbusClientList.removeAt(i);
		}
	}

	DbusClient client;
	client.clientName = pClientName;
	client.pClient = pClient;
	m_DbusClientList.append(client);
	
	gs_mutexForThreadSafe.unlock();

	return;
}
