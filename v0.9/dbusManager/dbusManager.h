#ifndef  DBUSMANAGER_H_20110615
#define  DBUSMANAGER_H_20110615

//#include <QtGui/QApplication>
#include <QString>
#include <QList>
#include "dbusServer.h"
#include "dbusClientProxy.h"
#include "dbusClient.h"
#include "dbusMessage.h"

class DbusManagerC
{
public:
       //get the object of "DbusManagerC", notice: you can not get the object from "new"
	static DbusManagerC *object();
       //release the object of "DbusManagerC"
       static void releaseObject();
	DBusServerC* getServer();
	void setServer(DBusServerC *pServer);
	DBusClientProxyC* getClient(const char* pClientName);
	void setClient(DBusClientProxyC *pClient, const char* pClientName);
	
private:
	static DbusManagerC *thisObject;
	DbusManagerC();
	~DbusManagerC();

private:
	class DbusClient
	{
	public:
		QString clientName;
		DBusClientProxyC *pClient;
	};
	typedef QList<DbusClient> DbusClientList;

	DbusClientList m_DbusClientList;
	
	DBusServerC *m_pDbusServer;

};

#endif
