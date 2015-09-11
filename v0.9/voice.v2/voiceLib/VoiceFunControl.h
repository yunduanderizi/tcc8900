/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： VoiceFunControl.h
* 文件标识： (见配置管理计划书)
* 内容摘要： 语音控制动态库
* 其它说明： 
* 当前版本： V1.0
* 作    者： 于震洋
* 完成日期： [2011/10/20]
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容： 
*<FH+>***************************************************************************
*/ 

/**
*<CLASS+>********************************************************************
* 类 名 称: VoiceFunControlC
* 功能描述: 语音控制动态库
* 其它说明: 
* 修改记录: 
* 修改日期			版 本		修改人		修改摘要
* -------------------------------------------------------------------------
* [2011/10/20]	    V1.0		于震洋		创建类
*<CLASS->*********************************************************************
*/

#ifndef VOICE_FUN_CONTROL_H_20111020
#define VOICE_FUN_CONTROL_H_20111020 

#include "dbusClientProxy.h"
#include "voiceDefine.h"

namespace Voice1
{

class VoiceFunControlC : public QObject
{
	Q_OBJECT
public:
	//get the object of "VoiceFunControlC, notice: you can not get the object from "new"
	static VoiceFunControlC *object();
    static VoiceFunControlC *getObject();
	//release the object of "VoiceFunControlC"
	static void releaseObject();

	void Init(char* pServerName, char* pObjectName, char* pProcessName, DBusServerC *pDbusServer);
	void openAiTalk();
	void closeAiTalk();
	void openAiBsTalk();
	void closeAiBsTalk();
    void setKeyword(KeyWordList keywordList);
	void playText(char* pText);
    bool isInit();

signals:
	void sigAiBsTalkStatusChange(Voice1::Ai_Bs_Talk_Status status);
	void sigAiTalkStatusChange(Voice1::Ai_Talk_Status status);
	void sigPlayTextStatusChange(Voice1::Play_Text_Status status);
	void sigAiBsTalkResult(QString sResultText);
	void sigAiTalkResult(int iResultCode);
	
private:
	static VoiceFunControlC *thisObject;
	VoiceFunControlC();
	virtual ~VoiceFunControlC();

	static void _aiBsTalkStatusUpdateHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);
	static void _aiTalkStatusUpdateHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);
	static void _playTextStatusHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);
	static void _aiBsTalkResultHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);
	static void _aiTalkResultHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);

	void _updateAiBsTalkStatus(Voice1::Ai_Bs_Talk_Status status);
	void _updateAiTalkStatus(Voice1::Ai_Talk_Status status);
	void _updatePlayTextStatus(Voice1::Play_Text_Status status);
	void _updateAiBsTalkResult(const char* pResultText);
	void _updateAiTalkResult(int iResultCode);

	bool		m_bInitFlag;
	DBusClientProxyC   *m_pDbusClientToVoice;
	DBusServerC		*m_pDbusServer;
	char		m_ServerName[NAME_LENGTH_MAX+1];
	char		m_ObjectName[NAME_LENGTH_MAX+1];
	char		m_ProcessName[NAME_LENGTH_MAX+1];

	Ai_Bs_Talk_Status		m_AiBsTalkStatus;
	Ai_Talk_Status		m_AiTalkStatus;
	Play_Text_Status		m_PlayTextStatus;
};


}

#endif

