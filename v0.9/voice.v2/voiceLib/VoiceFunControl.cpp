/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： VoiceFunControl.cpp
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
#include "VoiceFunControl.h"
#include "dbusMessage.h"
#include "voiceMessage.h"

namespace Voice1
{

void VoiceFunControlC::_aiBsTalkStatusUpdateHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
	VoiceToUiAiBsTalkStatusReqC *req=(VoiceToUiAiBsTalkStatusReqC*)pRequest;

	if( pRequest )
	{
		if( thisObject )
		{
			thisObject->_updateAiBsTalkStatus((Voice1::Ai_Bs_Talk_Status)(req->m_iStatus));
	 	}
	}
}

void VoiceFunControlC::_aiTalkStatusUpdateHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
	VoiceToUiAiTalkStatusReqC *req=(VoiceToUiAiTalkStatusReqC*)pRequest;

	if( pRequest )
	{
		if( thisObject )
		{
			thisObject->_updateAiTalkStatus((Voice1::Ai_Talk_Status)(req->m_iStatus));
	 	}
	}
}

void VoiceFunControlC::_playTextStatusHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
	VoiceToUiPlayTextStatusReqC *req=(VoiceToUiPlayTextStatusReqC*)pRequest;

	if( pRequest )
	{
		if( thisObject )
		{
			thisObject->_updatePlayTextStatus((Voice1::Play_Text_Status)(req->m_iStatus));
	 	}
	}
}

void VoiceFunControlC::_aiBsTalkResultHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
	VoiceToUiAiBsTalkResultReqC *req=(VoiceToUiAiBsTalkResultReqC*)pRequest;

	if( pRequest )
	{
		if( thisObject )
		{
			thisObject->_updateAiBsTalkResult(req->m_AiBsTalkResult);
	 	}
	}
}

void VoiceFunControlC::_aiTalkResultHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
	VoiceToUiAiTalkResultReqC *req=(VoiceToUiAiTalkResultReqC*)pRequest;

    printf("[VoiceFunControlC], recieve voice input \n");
	if( pRequest )
	{
       	if( thisObject )
		{
            printf("[VoiceFunControlC], req->iCode:%d \n", req->iCode);
			thisObject->_updateAiTalkResult(req->iCode);
	 	}
	}
}

VoiceFunControlC  *VoiceFunControlC::thisObject = NULL;
VoiceFunControlC  *VoiceFunControlC::object()
{
	VoiceFunControlC *pTmp = NULL;
		
	//gs_mutexForThreadSafe.lock();
	
	if(thisObject == NULL)
	{
		thisObject = new VoiceFunControlC();
        printf("[VoiceFunControlC] create Object[%p] \n", thisObject);
	}

    printf("[VoiceFunControlC]  old Object addr:[%p]\n", thisObject);
	pTmp = thisObject;

	//gs_mutexForThreadSafe.unlock();
	
	return pTmp;
}

VoiceFunControlC *VoiceFunControlC::getObject()
{
    VoiceFunControlC *pTmp = NULL;

    if (thisObject != NULL)
    {
        printf("[VoiceFunControlC] getObject %p m_bInitFlag:%d \n", thisObject, thisObject->m_bInitFlag);
        pTmp = thisObject;

    }

    return pTmp;
}
void VoiceFunControlC::releaseObject()
{
	//gs_mutexForThreadSafe.lock();

	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}

	//gs_mutexForThreadSafe.unlock();
}

VoiceFunControlC::VoiceFunControlC()
	: m_bInitFlag(false),
	  m_pDbusClientToVoice(NULL),
	  m_pDbusServer(NULL),
	  m_AiBsTalkStatus(Voice1::ABT_STATUS_STOP),
	  m_AiTalkStatus(Voice1::AT_STATUS_STOP),
	  m_PlayTextStatus(Voice1::PT_STATUS_STOP)
{
	printf("[VoiceFunControlC] new Object[%p] \n", this);
}

VoiceFunControlC::~VoiceFunControlC()
{
	if( m_bInitFlag )
	{
		UiToVoiceRegisterReqC	registerRequest;
		registerRequest.m_bRegister = false;
		registerRequest.importData(m_ServerName, m_ObjectName, m_ProcessName);
		DBusReplyC      *pReply=NULL;
		m_pDbusClientToVoice->sendService(&registerRequest, pReply, 1);
	}
}

void VoiceFunControlC::Init(char* pServerName, char* pObjectName, char* pProcessName, DBusServerC *pDbusServer)
{

	if ( m_bInitFlag )
	{
		return;
	}
	
	m_pDbusServer = pDbusServer;

    printf("[VoiceFunControlC] init start pServerName:%s\t,pObjectName:%s, pProcessName:%s\n", pServerName, pObjectName, pProcessName);
	if( NULL == pServerName || strlen(pServerName) > NAME_LENGTH_MAX )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "server name is error!\n")
		abort();
	}

	if( NULL == pObjectName || strlen(pObjectName) > NAME_LENGTH_MAX )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "object name is error!\n")
		abort();
	}

	if( NULL == pProcessName || strlen(pProcessName) > NAME_LENGTH_MAX )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "process name is error!\n")
		abort();
	}

    //printf("名字校验完成，开始校验Server\n");
	if( NULL == m_pDbusServer )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "dbus server is NULL!\n")
		abort();
	}

	memset(m_ServerName, 0, NAME_LENGTH_MAX+1);
	memset(m_ObjectName, 0, NAME_LENGTH_MAX+1);
	memset(m_ProcessName, 0, NAME_LENGTH_MAX+1);

	strncpy(m_ServerName, pServerName, NAME_LENGTH_MAX);
	strncpy(m_ObjectName, pObjectName, NAME_LENGTH_MAX);
	strncpy(m_ProcessName, pProcessName, NAME_LENGTH_MAX);
	
	m_pDbusClientToVoice = new DBusClientProxyC(ServerVoice1, ObjectVoice1);
    m_pDbusClientToVoice->connect();

	UiToVoiceRegisterReqC	registerRequest;
	registerRequest.m_bRegister = true;
	registerRequest.importData(m_ServerName, m_ObjectName, m_ProcessName);
	DBusReplyC      *pReply=NULL;
	int iRet = m_pDbusClientToVoice->sendService(&registerRequest, pReply, 1);
    printf("[VoiceFunControlC] Init sendService return %d\n", iRet);

	m_pDbusServer->registerService(REQ_ID_VOICE1_TO_UI_AIBSTALK_STATUS, _aiBsTalkStatusUpdateHandler, VoiceToUiAiBsTalkStatusReqC::factory);
	m_pDbusServer->registerService(REQ_ID_VOICE1_TO_UI_AITALK_STATUS, _aiTalkStatusUpdateHandler, VoiceToUiAiTalkStatusReqC::factory);
	m_pDbusServer->registerService(REQ_ID_VOICE1_TO_UI_PLAYTEXT_STATUS, _playTextStatusHandler, VoiceToUiPlayTextStatusReqC::factory);
	m_pDbusServer->registerService(REQ_ID_VOICE1_TO_UI_AIBSTALK_RESULT, _aiBsTalkResultHandler, VoiceToUiAiBsTalkResultReqC::factory);
	m_pDbusServer->registerService(REQ_ID_VOICE1_TO_UI_AITALK_RESULT, _aiTalkResultHandler, VoiceToUiAiTalkResultReqC::factory);

    printf("[VoiceFunControlC] Init over \n");
    thisObject->m_bInitFlag = true;
}

void VoiceFunControlC::_updateAiBsTalkStatus(Voice1::Ai_Bs_Talk_Status status)
{
	if( m_AiBsTalkStatus != status )
	{
		m_AiBsTalkStatus = status;
		emit sigAiBsTalkStatusChange(m_AiBsTalkStatus);
	}
}

void VoiceFunControlC::_updateAiTalkStatus(Voice1::Ai_Talk_Status status)
{
	if( m_AiTalkStatus != status )
	{
		m_AiTalkStatus = status;
		emit sigAiTalkStatusChange(m_AiTalkStatus);
	}
}

void VoiceFunControlC::_updatePlayTextStatus(Voice1::Play_Text_Status status)
{
	if( m_PlayTextStatus != status )
	{
		m_PlayTextStatus = status;
		emit sigPlayTextStatusChange(m_PlayTextStatus);
	}
}

void VoiceFunControlC::_updateAiBsTalkResult(const char* pResultText)
{
    // 返回编码为UTF8，转化编码

	QString sResultText = QString::fromUtf8(pResultText);

    printf("[VoiceFunControlC] updateAiBsTalkResult: %s  QString = %s\n", pResultText, sResultText.toLatin1().data());
	emit sigAiBsTalkResult(sResultText);
}

void VoiceFunControlC::_updateAiTalkResult(int iResultCode)
{
	emit sigAiTalkResult(iResultCode);
}

void VoiceFunControlC::openAiTalk()
{
	if ( !m_bInitFlag )
	{
		printf("[VoiceFunControlC] line[%4d] Module not init !\n", __LINE__);
		return;
	}
	UiToVoiceCommonReqC	commandRequest;
	commandRequest.m_iCommandId = VOICE_COMMAND_OPEN_AITALK;
	strcpy(commandRequest.m_ProcessName, m_ProcessName);
	DBusReplyC      *pReply=NULL;
	int ret = m_pDbusClientToVoice->sendService(&commandRequest, pReply, 1);
	if ( ret < 0 )
	{
		printf("[VoiceFunControlC] line[%4d] sendService error[%d] !\n", __LINE__, ret);
	}
}

void VoiceFunControlC::closeAiTalk()
{
	if ( !m_bInitFlag )
	{
		printf("[VoiceFunControlC] line[%4d] Module not init !\n", __LINE__);
		return;
	}
	UiToVoiceCommonReqC	commandRequest;
	commandRequest.m_iCommandId = VOICE_COMMAND_CLOSE_AITALK;
	strcpy(commandRequest.m_ProcessName, m_ProcessName);
	DBusReplyC      *pReply=NULL;
	int ret = m_pDbusClientToVoice->sendService(&commandRequest, pReply, 1);
	if ( ret < 0 )
	{
		printf("[VoiceFunControlC] line[%4d] sendService error[%d] !\n", __LINE__, ret);
	}
}

void VoiceFunControlC::openAiBsTalk()
{
	if ( !m_bInitFlag )
	{
		printf("[VoiceFunControlC] line[%4d] Module not init !\n", __LINE__);
		return;
	}
	UiToVoiceCommonReqC	commandRequest;
	commandRequest.m_iCommandId = VOICE_COMMAND_OPEN_BSAITALK;
	strcpy(commandRequest.m_ProcessName, m_ProcessName);
	DBusReplyC      *pReply=NULL;

	int ret = m_pDbusClientToVoice->sendService(&commandRequest, pReply, 1);
	if ( ret < 0 )
	{
		printf("[VoiceFunControlC] line[%4d] sendService error[%d] !\n", __LINE__, ret);
	}
    printf("[VoiceFunControlC] openAiBsTalk finish\n");
}

void VoiceFunControlC::closeAiBsTalk()
{
	if ( !m_bInitFlag )
	{
		printf("[VoiceFunControlC] line[%4d] Module not init !\n", __LINE__);
		return;
	}
	UiToVoiceCommonReqC	commandRequest;
	commandRequest.m_iCommandId = VOICE_COMMAND_CLOSE_BSAITALK;
	strcpy(commandRequest.m_ProcessName, m_ProcessName);
	DBusReplyC      *pReply=NULL;
	int ret = m_pDbusClientToVoice->sendService(&commandRequest, pReply, 1);
	if ( ret < 0 )
	{
		printf("[VoiceFunControlC] line[%4d] sendService error[%d] !\n", __LINE__, ret);
	}
}

void VoiceFunControlC::setKeyword(KeyWordList keywordList)
{
	if ( !m_bInitFlag )
	{
		printf("[VoiceFunControlC] line[%4d] Module not init !\n", __LINE__);
		return;
	}
    UiToVoiceSetKeywordReqC	commandRequest;
    strcpy(commandRequest.m_ProcessName, m_ProcessName);
    DBusReplyC      *pReply=NULL;

    // 拷贝keywordList内容
    KeyWordList::iterator keywordIter = keywordList.begin();
    for (; keywordIter != keywordList.end(); ++keywordIter)
    {
        // 循环发送到进程中
        memset(commandRequest.m_strKeyword, 0, VOICE_TEXT_LENGTH_MAX+1);
        strncpy(commandRequest.m_strKeyword, keywordIter->toUtf8().data(), VOICE_TEXT_LENGTH_MAX);
        int ret = m_pDbusClientToVoice->sendService(&commandRequest, pReply, 1);
		if ( ret < 0 )
		{
			printf("[VoiceFunControlC] line[%4d] sendService error[%d] !\n", __LINE__, ret);
		}
    }

    // 结束时发送一个"!"，表示发送完毕
    memset(commandRequest.m_strKeyword, 0, VOICE_TEXT_LENGTH_MAX+1);
    strcpy(commandRequest.m_strKeyword, "!");
    int ret = m_pDbusClientToVoice->sendService(&commandRequest, pReply, 1);
	if ( ret < 0 )
	{
		printf("[VoiceFunControlC] line[%4d] sendService error[%d] !\n", __LINE__, ret);
	}
}

void VoiceFunControlC::playText(char* pText)
{
	if ( !m_bInitFlag )
	{
		printf("[VoiceFunControlC] line[%4d] Module not init !\n", __LINE__);
		return;
	}
	UiToVoicePlayTextReqC	commandRequest;
	memset(commandRequest.m_VoiceText, 0, VOICE_TEXT_LENGTH_MAX+1);
	strncpy(commandRequest.m_VoiceText, pText, VOICE_TEXT_LENGTH_MAX);
	strcpy(commandRequest.m_ProcessName, m_ProcessName);
	DBusReplyC      *pReply=NULL;
	int ret = m_pDbusClientToVoice->sendService(&commandRequest, pReply, 1);
	if ( ret < 0 )
	{
		printf("[VoiceFunControlC] line[%4d] sendService error[%d] !\n", __LINE__, ret);
	}
}

bool VoiceFunControlC::isInit()
{
    return m_bInitFlag;
}






}


