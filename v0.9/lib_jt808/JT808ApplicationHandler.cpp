#include "JT808MessageHelper.h"
#include "LogUtility.h"
#include "VDRRunParaTable.h"
#include "VDRDeployParaTable.h"
#include "ConnectionManager.h"
#include "JT808ApplicationHandler.h"
#include "ConfigManager.h"
#include <stdlib.h>
#include <map>
#define NHB
using namespace std;
extern JT808MessageHandler jt808;

text SendText;
question SendQuestion;
callback SendCallBack;
notify SendContactsUpdate,SendSettingUpdate;
regist SendLogoff,SendRegisterN,SendQuickRegister,SendAuthentication;
ConnectStatus SendConnectionStatus;
void JT808RegisterServiceHandler(int flag, unsigned char phone[])
{
	LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::JT808RegisterServiceHandler get register request");

	if(flag== 0)
	{
		JT808MessageHelper::SendLogout();

	}else if(flag == 1){
		JT808MessageHelper::SendRegister(1, phone);
	}else if(flag == 2){
		JT808MessageHelper::SendRegister(2, phone);
	}
    return;
}

void JT808AuthenticationServiceHandler(void)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::JT808AuthenticationServiceHandler get register request");
    JT808MessageHelper::SendAuth();
    return;
}

void JT808AnswerServiceHandler(unsigned short seq, unsigned char answer)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808AnswerServiceHandler get answer request");
    
    JT808MessageHelper::SendAnswer(seq, answer);
    return;
}

void JT808ModifySettingServiceHandler(char *main_server, char *second_server, char *auth, int port)
{
    VDRRunParaTable *runtable = new VDRRunParaTable();
    VDRDeployParaTable *deploytable = new VDRDeployParaTable();
    int flag1 = false, flag2 = false;
    int ret = 0;
    int err = 0;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808ModifySettingServiceHandler get modify setting request");

    if(main_server)
    {
        string m_main_server = main_server;
        string old_main_server;
        runtable->GetStringValue((int)VDR_PARA_MAIN_SERVER_ADDRESS, old_main_server);
        if(m_main_server != old_main_server)
        {
            runtable->SetStringValue((int)VDR_PARA_MAIN_SERVER_ADDRESS, m_main_server);
            flag1 = true;
        }
    }

    if(second_server)
    {
        string m_second_server = second_server;
        string old_second_server;
        runtable->GetStringValue((int)VDR_PARA_SECOND_SERVER_ADDRESS, old_second_server);
        if(m_second_server != old_second_server)
        {
            runtable->SetStringValue((int)VDR_PARA_SECOND_SERVER_ADDRESS, m_second_server);
            flag1 = true;
        }
    }

    if(port > 0)
    {
        int m_port = port;
        int old_port;
        runtable->GetIntValue((int)VDR_PARA_TCP_PORT, old_port);
        if(m_port != old_port)
        {
            runtable->SetIntValue((int)VDR_PARA_TCP_PORT, m_port);
            flag1 = true;
        }
    }
    
    if(flag1)
    {
        ret = runtable->Commit();
        if(ret < 0)
        {
            err = 1;
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808ModifySettingServiceHandler commit runtable failed");
        }
    }

    if(auth)
    {
        string m_auth = auth;
        string old_auth;
        deploytable->GetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, old_auth);
        if(m_auth != old_auth)
        {
            deploytable->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, m_auth);
            flag2 = true;
        }
    }

    if(flag2)
    {
	ret = deploytable->Commit();
        if(ret < 0)
        {
            err = 1;
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808ModifySettingServiceHandler commit deploytable failed");
        }
    }
    delete runtable;
    delete deploytable;
    
    if(flag1 || flag2)
    {
        ConnectionManager::Instance()->Reconnect();
    }
    
    return;
}
void JT808MediaHandler(int type, int index, int flag)
{
	if(type==0)
	{
		JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_PHOTO,index,flag);
	}
	else if(type==1)
	{
		JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_AUDIO,index,1);
	}
	else if(type==2)
	{
		JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_VIDEO,index,1);
	}
	return;
}
void JT808EventServiceHandler(unsigned char up_event[], unsigned char num)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808EventServiceHandler request");
    JT808MessageHelper::SendEvents(up_event,num);
    return;
}
//callback
void SendJT808TextNotify(void(*func)(int emergency,int lcd,int tts,int bigscreen,int naviorcan,string &text))
{

	if(NULL==func)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "pointer is null!");
	}
	SendText=func;
	return; 
}

void SendJT808QuestionNotify(void(*func)(int seq, int emergency,int lcd,int tts,int bigscreen,string &text, map<int,string>&answers))
{
	if(NULL==func)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "pointer is null!");
	}
	SendQuestion=func;
	return;
}

void SendJT808CallBackNotify(void(*func)(int monitor, string &phone))
{
	if(NULL==func)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "pointer is null!");
	}
	SendCallBack=func;
	return;
	
}

void SendJT808ContactsUpdateNotify(void(*func)(void))
{
	if(NULL==func)
	{
		LogUtility::Log(LOG_LEVEL_ERROR,"pointer is null!");
	}
	SendContactsUpdate=func;
	return;
}

void SendLogoffNotify(void(*func)(int result))
{
	if(NULL==func)
	{
		LogUtility::Log(LOG_LEVEL_ERROR,"pointer is null!");
	}
	SendLogoff=func;
	return;
}

void SendRegisterNotify(void(*func)(int result))
{
	if(NULL==func)
	{
		 LogUtility::Log(LOG_LEVEL_ERROR,"pointer is null!");
	}
	SendRegisterN=func;
	return;
}

void SendQuickRegisterNotify(void(*func)(int result))
{
	if(NULL==func)
	{
		LogUtility::Log(LOG_LEVEL_ERROR,"pointer is null!");
	}
	SendQuickRegister=func;
	return ;
}

void SendAuthenticationNotify(void(*func)(int result))
{
	if(NULL==func)
	{
		 LogUtility::Log(LOG_LEVEL_ERROR,"pointer is null!");
	}
	SendAuthentication=func;
	return;
}
void SendJT808SettingUpdateNotify(void(*func)(void))
{
	if(NULL==func)
	{
		LogUtility::Log(LOG_LEVEL_ERROR,"pointer is null!");
	}
	SendSettingUpdate=func;
	return;
}
void SendJT808ConnectionStatusNotify(void(*func)(int main_status, int second_status))
{
	if(NULL==func)
	{
		LogUtility::Log(LOG_LEVEL_ERROR,"pointer is null!");
	}
	SendConnectionStatus=func;
	return ;
}

