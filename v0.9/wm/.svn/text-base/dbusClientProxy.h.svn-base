#ifndef DBUS_CLIENT_PROXY_H_20100815
#define DBUS_CLIENT_PROXY_H_20100815

#include "dbusClient.h"


class DBusClientProxyC: public DBusClientC
{
public:
        DBusClientProxyC(char *serverName,char *objectName);
        ~DBusClientProxyC();

//        int     connect();

	int sendService(DBusRequestC  *request);
        int sendService(DBusRequestC  *request,DBusReplyC *&reply,int timeout); //timeout unit is millisecond
	int sendServiceBlockMode(DBusRequestC  *request,DBusReplyC *&reply,int timeout); //timeout unit is millisecond

private:
	int startServer(int result);

private:
	char processName[50];
	char connectStatus;

	static DBusClientC	*pmClient;
	static char  		 pmConnectStatus;
};

#endif

