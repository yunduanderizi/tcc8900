
#include "myDbus.h"
#include "AiBsTalk.h"
#include "Record.h"
#include "AiTalk.h"
#include "AiPlay.h"
#include "KeyWordsMgr.h"

MyDbusC  *MyDbusC::thisObject = NULL;
KeyWordList MyDbusC::m_keywordList;
MyDbusC  *MyDbusC::object()
{
    MyDbusC *pTmp = NULL;

    //gs_mutexForThreadSafe.lock();

    if( !thisObject )
    {
        thisObject = new MyDbusC();
    }

    pTmp = thisObject;

    //gs_mutexForThreadSafe.unlock();

    return pTmp;
}

void MyDbusC::releaseObject()
{
    //gs_mutexForThreadSafe.lock();

    if( thisObject )
    {
        delete thisObject;
        thisObject = NULL;
    }

    //gs_mutexForThreadSafe.unlock();
}

void MyDbusC::_registerHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
    UiToVoiceRegisterReqC *req=(UiToVoiceRegisterReqC*)pRequest;

    if( pRequest )
    {
        if( thisObject )
        {
            thisObject->_registerUser(req);
        }
    }
}

void MyDbusC::_commonHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
    UiToVoiceCommonReqC *req=(UiToVoiceCommonReqC*)pRequest;

    printf("Receive cmd\n");
    if( pRequest )
    {
        if( thisObject )
        {
            printf("going to _commonCommand\n");
            thisObject->_commonCommand(req);
        }
    }
}

void MyDbusC::_playTextHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
    UiToVoicePlayTextReqC *req=(UiToVoicePlayTextReqC*)pRequest;

    if( pRequest )
    {
        if( thisObject )
        {
            //TODO
            thisObject->_playText(req);
        }
    }
}

void MyDbusC::_setKeywordHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
    UiToVoiceSetKeywordReqC *req = (UiToVoiceSetKeywordReqC *)pRequest;

    if (pRequest)
    {
        // 先拼接成一个list
        if (strcmp(req->m_strKeyword, "!") != 0)
        {
            printf("KKKKKey work is: %s\n", req->m_strKeyword);

            // 没有结束，填到list中
            m_keywordList.append(QString::fromUtf8(req->m_strKeyword));
        }
        else
        {
            if (thisObject)
            {
                thisObject->_setKeyword(req, m_keywordList);

                m_keywordList.clear();
            }
        }
    }
}

MyDbusC::MyDbusC()
    : m_pDbusServer(NULL),
      m_iCurrentUser(-1)
{
    m_DbusClientList.clear();
}

MyDbusC::~MyDbusC()
{
}

void MyDbusC::initDbus()
{
    m_pDbusServer = new DBusServerC(ServerVoice1, ObjectVoice1);

    m_pDbusServer->connect();

    m_pDbusServer->registerService(REQ_ID_UI_TO_VOICE1_REGISTER, _registerHandler, UiToVoiceRegisterReqC::factory);
    m_pDbusServer->registerService(REQ_ID_UI_TO_VOICE1_COMMON, _commonHandler, UiToVoiceCommonReqC::factory);
    m_pDbusServer->registerService(REQ_ID_UI_TO_VOICE1_PLAY_TEXT, _playTextHandler, UiToVoicePlayTextReqC::factory);
    m_pDbusServer->registerService(REQ_ID_VOICE1_TO_UI_SETKEYWORD, _setKeywordHandler, UiToVoiceSetKeywordReqC::factory);
}

int MyDbusC::_findUser(const char* pProcessName)
{
    printf("_findUser, pProcessName is %s\n", pProcessName);
    int iUser = -1;
    for(int i=0; i<m_DbusClientList.count(); i++)
    {
        printf("element is %s\n", m_DbusClientList[i].m_ProcessName);
        // 修改方法名[10/26/2011 yuzhenyang]
        if(0 == strcmp(m_DbusClientList[i].m_ProcessName, pProcessName))
        {
            iUser = i;
            break;
        }
    }

    return iUser;
}

DBusServerC* MyDbusC::getDbusServer()
{
    return m_pDbusServer;
}

void MyDbusC::_registerUser(UiToVoiceRegisterReqC *pRequest)
{
    DbusClientNode node;
    strcpy(node.m_ServerName, pRequest->m_ServerName);
    strcpy(node.m_ObjectName, pRequest->m_ObjectName);
    strcpy(node.m_ProcessName, pRequest->m_ProcessName);
    node.m_pDbusClient = new DBusClientProxyC(node.m_ServerName, node.m_ObjectName);

    node.m_pDbusClient->connect();

    m_DbusClientList.append(node);
}

void MyDbusC::_commonCommand(UiToVoiceCommonReqC* pRequest)
{
    int ret = AC_E_OK;
    AudioControlLibC::object();
    int iUser = _findUser(pRequest->m_ProcessName);

    if( iUser < 0 )
    {
        printf("_commonCommand, iUser < 0\n");
        return;
    }

    printf("_commonCommand, switch start\n");
    switch(pRequest->m_iCommandId)
    {
    case VOICE_COMMAND_OPEN_AITALK:
        if( m_iCurrentUser < 0 )
        {
            m_iCurrentUser = iUser;
            AiTalkC::object()->startVoiceRecorg();

           
        }
        break;

    case VOICE_COMMAND_CLOSE_AITALK:
        if( m_iCurrentUser == iUser )
        {
            // 关闭输入，关闭语音识别
            AiTalkC::object()->stopVoiceRecorg();
            RecordC::object()->stopVoiceRecord();

            m_iCurrentUser = -1;
        }
        break;

    case VOICE_COMMAND_OPEN_BSAITALK:
        if( m_iCurrentUser < 0 )
        {
            m_iCurrentUser = iUser;
            int iRet = 0;
            AiBsTalkC::object()->openVoiceRecog();
        }
        break;

    case VOICE_COMMAND_CLOSE_BSAITALK:
        if( m_iCurrentUser == iUser )
        {
            // 关闭语音输入并且关闭录音
            RecordC::object()->stopVoiceRecord();
            AiBsTalkC::object()->closeVoiceRecog();

            m_iCurrentUser = -1;
        }
        break;

    default:

        break;
    }
}

void MyDbusC::_playText(UiToVoicePlayTextReqC* pRequest)
{
    int iUser = _findUser(pRequest->m_ProcessName);

    if( iUser < 0 )
        return;

    if( m_iCurrentUser < 0 )
    {
        int iRet = AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP, true);
        if (iRet != AC_E_OK)
        {
            perror("open voiceInput error:");
        }
        // 语音合成
        m_iCurrentUser = iUser;

        AiPlayC::object()->voicePlay(pRequest->m_VoiceText);

        AudioControl1::AudioControlLibC::object()->close(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP, false);
        m_iCurrentUser = -1;
    }
}

void MyDbusC::_setKeyword(UiToVoiceSetKeywordReqC* pRequest, KeyWordList keywordList)
{
    int iUser = _findUser(pRequest->m_ProcessName);

    if (iUser < 0)
    {
        return;
    }

    if (m_iCurrentUser < 0)
    {
        // 设置关键词
        m_iCurrentUser = iUser;

        KeyWordsMgrC::object()->insertKeyWord(keywordList);

        m_iCurrentUser = -1;
    }
}

void MyDbusC::aiBsTalkStatusUpdate(Voice1::Ai_Bs_Talk_Status status)
{
    printf("[aiBsTalkStatusUpdate] m_iCurrentUser = %d status:%d\n", m_iCurrentUser, status);
    if( m_iCurrentUser >= 0 )
    {
        VoiceToUiAiBsTalkStatusReqC	request;
        request.m_iStatus = status;
        DBusReplyC      *pReply=NULL;

        m_DbusClientList[m_iCurrentUser].m_pDbusClient->sendService(&request, pReply, 1);

        if (status == Voice1::ABT_STATUS_STOP)
        {
            // 如果发送停止信号，将开关置为-1，这样外面程序就可以进来
            m_iCurrentUser = -1;
        }

        printf("[aiBsTalkStatusUpdate] m_iCurrentUser = %d status:%d\n", m_iCurrentUser, status);
    }
}

void MyDbusC::aiTalkStatusUpdate(Voice1::Ai_Talk_Status status)
{
    if( m_iCurrentUser >= 0 )
    {
        VoiceToUiAiTalkStatusReqC	request;
        request.m_iStatus = status;
        DBusReplyC      *pReply=NULL;
        m_DbusClientList[m_iCurrentUser].m_pDbusClient->sendService(&request, pReply, 1);

        if (status == Voice1::AT_STATUS_STOP)
        {
            m_iCurrentUser = -1;
        }
    }
}

void MyDbusC::playTextStatus(Voice1::Play_Text_Status status)
{
    if( m_iCurrentUser >= 0 )
    {
        VoiceToUiPlayTextStatusReqC	request;
        request.m_iStatus = status;
        DBusReplyC      *pReply=NULL;
        m_DbusClientList[m_iCurrentUser].m_pDbusClient->sendService(&request, pReply, 1);
    }
}

void MyDbusC::aiBsTalkResult(const char* pResultText)
{
    printf("MyDbus aiBsTalkResult start, m_iCurrentUser is %d\n", m_iCurrentUser);

    if (m_iCurrentUser >= 0)
    {
        VoiceToUiAiBsTalkResultReqC	request;
        memset(request.m_AiBsTalkResult, 0, AI_BS_TALK_RESULT_LENGTH_MAX+1);

        // 增加判断空情况[12/9/2011 yuzhenyang]
        if (pResultText != NULL)
        {
            strncpy(request.m_AiBsTalkResult, pResultText, AI_BS_TALK_RESULT_LENGTH_MAX);
        }

        DBusReplyC      *pReply=NULL;
        m_DbusClientList[m_iCurrentUser].m_pDbusClient->sendService(&request, pReply, 1);
    }
}

void MyDbusC::aiTalkResult(int iResultCode)
{
    if( m_iCurrentUser >= 0 )
    {
        VoiceToUiAiTalkResultReqC	request;
        request.iCode = iResultCode;
        DBusReplyC      *pReply=NULL;
        m_DbusClientList[m_iCurrentUser].m_pDbusClient->sendService(&request, pReply, 1);
    }
}


