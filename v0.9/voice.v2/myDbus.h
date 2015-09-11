
#ifndef MY_DBUS_H_20111020
#define MY_DBUS_H_20111020 

#include <stdio.h>
#include <errno.h>
#include "dbusMessage.h"
#include "voiceDefine.h"
#include "message/voiceMessage.h"
#include <QList>
#include "audioControlLib.h"

using namespace AudioControl1;

class DbusClientNode
{
public:
	DBusClientProxyC*	m_pDbusClient;
	char m_ServerName[NAME_LENGTH_MAX+1];
	char m_ObjectName[NAME_LENGTH_MAX+1];
	char m_ProcessName[NAME_LENGTH_MAX+1];
};

typedef QList<DbusClientNode> DbusClientList;

class MyDbusC
{
public:
	//get the object of "MyDbusC", notice: you can not get the object from "new"
	static MyDbusC *object();
	//release the object of "MyDbusC"
	static void releaseObject();

	void initDbus();
	DBusServerC *getDbusServer();
	void aiBsTalkStatusUpdate(Voice1::Ai_Bs_Talk_Status status);
	void aiTalkStatusUpdate(Voice1::Ai_Talk_Status status);
	void playTextStatus(Voice1::Play_Text_Status status);
	void aiBsTalkResult(const char* pResultText);
	void aiTalkResult(int iResultCode);
	
private:
	static MyDbusC *thisObject;
	MyDbusC();
	virtual ~MyDbusC();

	static void _registerHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);
	static void _commonHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);
	static void _playTextHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);
    static void _setKeywordHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);

	void _registerUser(UiToVoiceRegisterReqC* pRequest);
	void _commonCommand(UiToVoiceCommonReqC* pRequest);
	void _playText(UiToVoicePlayTextReqC* pRequest);
    void _setKeyword(UiToVoiceSetKeywordReqC* pRequest, KeyWordList keywordList);

	int _findUser(const char* pProcessName);

	DBusServerC		*m_pDbusServer;
	int				m_iCurrentUser;
	DbusClientList	m_DbusClientList;
    static KeyWordList     m_keywordList;
};

#endif

