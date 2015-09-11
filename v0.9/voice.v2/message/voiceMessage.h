#ifndef VOICE_MESSAGE_H_20111020
#define VOICE_MESSAGE_H_20111020 

#include "dbusClientProxy.h"
#include "dbusMessage.h"
#include "voiceDefine.h"

#define	ServerVoice1 		(char*)"com.innov.voice1"
#define	ObjectVoice1 		(char*)"/voice1"

#define	REQ_ID_UI_TO_VOICE1_REGISTER				86
#define	REQ_ID_UI_TO_VOICE1_COMMON				87
#define	REQ_ID_UI_TO_VOICE1_PLAY_TEXT			88
#define	REQ_ID_VOICE1_TO_UI_AIBSTALK_STATUS		89
#define	REQ_ID_VOICE1_TO_UI_AITALK_STATUS		90
#define	REQ_ID_VOICE1_TO_UI_PLAYTEXT_STATUS		91
#define	REQ_ID_VOICE1_TO_UI_AIBSTALK_RESULT		92
#define	REQ_ID_VOICE1_TO_UI_AITALK_RESULT		93
#define	REQ_ID_VOICE1_TO_UI_SETKEYWORD          	94


////////////////////////////////////////////////////////////////////
class UiToVoiceRegisterReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(UiToVoiceRegisterReqC)

public:
	UiToVoiceRegisterReqC():DBusRequestC(REQ_ID_UI_TO_VOICE1_REGISTER){}
	void importData(const char* pServerName, const char *pObjectName, const char* pProcessName);
	void print(FILE *fp){fprintf(fp, "[UiToVoiceRegisterReqC] m_ServerName[%s]m_ObjectName[%s]m_ProcessName[%s]\n", m_ServerName, m_ObjectName, m_ProcessName);}

	bool	m_bRegister;
	char m_ServerName[NAME_LENGTH_MAX+1];
	char m_ObjectName[NAME_LENGTH_MAX+1];
	char m_ProcessName[NAME_LENGTH_MAX+1];
};

typedef enum _Voice_Command
{
	VOICE_COMMAND_OPEN_AITALK = 0,
	VOICE_COMMAND_CLOSE_AITALK,
	VOICE_COMMAND_OPEN_BSAITALK,
	VOICE_COMMAND_CLOSE_BSAITALK,
}Voice_Command_Id;

class UiToVoiceSetKeywordReqC:public DBusRequestC
{
    DBUS_REQUEST_OBJECT(UiToVoiceSetKeywordReqC)

public:
    UiToVoiceSetKeywordReqC():DBusRequestC(REQ_ID_VOICE1_TO_UI_SETKEYWORD){}
    void print(FILE *fp){fprintf(fp, "UiToVoiceSetKeywordReqC keyword is %s\n", m_strKeyword);}

    char m_strKeyword[VOICE_TEXT_LENGTH_MAX+1];
    char m_ProcessName[NAME_LENGTH_MAX+1];
};

class UiToVoiceCommonReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(UiToVoiceCommonReqC)

public:
	UiToVoiceCommonReqC():DBusRequestC(REQ_ID_UI_TO_VOICE1_COMMON){}
	void print(FILE *fp){fprintf(fp, "[UiToVoiceCommonReqC] m_iCommandId[%d]\n", m_iCommandId);}

	int m_iCommandId;
	char m_ProcessName[NAME_LENGTH_MAX+1];
};

class UiToVoicePlayTextReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(UiToVoicePlayTextReqC)

public:
	UiToVoicePlayTextReqC():DBusRequestC(REQ_ID_UI_TO_VOICE1_PLAY_TEXT){}
	void print(FILE *fp){fprintf(fp, "[UiToVoicePlayTextReqC] m_VoiceText[%s]\n", m_VoiceText);}

	char m_VoiceText[VOICE_TEXT_LENGTH_MAX+1];
	char m_ProcessName[NAME_LENGTH_MAX+1];
};

class VoiceToUiAiBsTalkStatusReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(VoiceToUiAiBsTalkStatusReqC)

public:
	VoiceToUiAiBsTalkStatusReqC():DBusRequestC(REQ_ID_VOICE1_TO_UI_AIBSTALK_STATUS){}
	void print(FILE *fp){fprintf(fp, "[VoiceToUiAiBsTalkStatusReqC] m_iStatus[%d]\n", m_iStatus);}

	int m_iStatus;
};

class VoiceToUiAiTalkStatusReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(VoiceToUiAiTalkStatusReqC)

public:
	VoiceToUiAiTalkStatusReqC():DBusRequestC(REQ_ID_VOICE1_TO_UI_AITALK_STATUS){}
	void print(FILE *fp){fprintf(fp, "[VoiceToUiAiBsTalkStatusReqC] m_iStatus[%d]\n", m_iStatus);}

	int m_iStatus;
};

class VoiceToUiPlayTextStatusReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(VoiceToUiPlayTextStatusReqC)

public:
	VoiceToUiPlayTextStatusReqC():DBusRequestC(REQ_ID_VOICE1_TO_UI_PLAYTEXT_STATUS){}
	void print(FILE *fp){fprintf(fp, "[VoiceToUiPlayTextStatusReqC] m_iStatus[%d]\n", m_iStatus);}

	int m_iStatus;
};

class VoiceToUiAiBsTalkResultReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(VoiceToUiAiBsTalkResultReqC)

public:
	VoiceToUiAiBsTalkResultReqC():DBusRequestC(REQ_ID_VOICE1_TO_UI_AIBSTALK_RESULT){}
	void print(FILE *fp){fprintf(fp, "[VoiceToUiAiBsTalkResultReqC] m_AiBsTalkResult[%s]\n", m_AiBsTalkResult);}

	char m_AiBsTalkResult[AI_BS_TALK_RESULT_LENGTH_MAX+1];
};

class VoiceToUiAiTalkResultReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(VoiceToUiAiTalkResultReqC)

public:
	VoiceToUiAiTalkResultReqC():DBusRequestC(REQ_ID_VOICE1_TO_UI_AITALK_RESULT){}
	void print(FILE *fp){fprintf(fp, "[VoiceToUiAiTalkResultReqC] iCode[%d]\n", iCode);}

	int iCode;
};


#endif
 
