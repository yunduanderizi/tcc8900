
#ifndef MY_DBUS_H
#define MY_DBUS_H

#include "dbusServer.h"
#include "dbusClient.h"
#include "audioControlMessage.h"
#include "audioData.h"

namespace AudioControl1
{



class MyDbusC
{
public:
	//get the object of "MyDbusC", notice: you can not get the object from "new"
	static MyDbusC *object();
	//release the object of "MyDbusC"
	static void releaseObject();

	DBusServerC *getDbusServer();

	void audioChannelStatusChanged(AudioChannelIdE audioChannelId, AudioChannelStatusE audioChannelStatus);
	
private:
	static MyDbusC *thisObject;
	MyDbusC();
	virtual ~MyDbusC();

	static void _audioControlCommonHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);
	static void _audioControlInquireChannelInfoHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);
	
	void _initDbus();

	DBusServerC		*m_pDbusServer;
};


}

#endif

