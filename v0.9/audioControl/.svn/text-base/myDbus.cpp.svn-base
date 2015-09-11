
#include "debugMonitor.h"
#include "myDbus.h"
#include "audioControl.h"
#include "audioControlErrCode.h"

namespace AudioControl1
{


MyDbusC  *MyDbusC::thisObject = NULL;
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

void MyDbusC::_audioControlCommonHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
	int retval = AC_E_OK;
	AudioChannelIdE audioChannelId = AUDIO_CHANNEL_ID_INVALID;

	UiToAudioControlCommonReqC *req=(UiToAudioControlCommonReqC*)pRequest;

	if( pRequest )
	{
		if( thisObject )
		{
			if( req->m_bNeedReply )
			{
				pReply = new AudioControlToUiCommonRepC();
			}

			switch(req->m_iCommandId)
			{
				case ACC_OPEN:
					retval = AudioControlC::object()->open(req->m_AudioChannelId);
					break;
					
				case ACC_CLOSE:
					retval = AudioControlC::object()->close(req->m_AudioChannelId);
					break;

				case ACC_ADJUST_VOL:
					retval = AudioControlC::object()->adjustVol(req->m_AudioChannelId, req->m_iVol);
					break;

				case ACC_MUTE:
					retval = AudioControlC::object()->mute(req->m_AudioChannelId);
					break;

				case ACC_UNMUTE:
					retval = AudioControlC::object()->unMute(req->m_AudioChannelId);
					break;

				case ACC_SET_SPEED:
					retval = AudioControlC::object()->setSpeed(req->m_iSpeed);
					break;

				case ACC_OPEN_VOL_SPEED:
					retval = AudioControlC::object()->openVolSpeed();
					break;

				case ACC_CLOSE_VOL_SPEED:
					retval = AudioControlC::object()->closeVolSpeed();
					break;

				case ACC_GET_CURRENT_OPEN_AUDIO_CHANNEL:
					retval = AudioControlC::object()->getCurrentOpenAudioChannel(audioChannelId);
					break;

				case ACC_RESUME_DATA_TO_DEFAULT:
					retval = AudioControlC::object()->resumeDataToDefault();
					break;

				default:
					IBD_PRINTF(DEBUG_INFOMATION, "It's impossible to come here\n");
					break;
			}

			if( req->m_bNeedReply )
			{
				pReply = new AudioControlToUiCommonRepC();
				((AudioControlToUiCommonRepC*)pReply)->m_iRetval = retval;
				((AudioControlToUiCommonRepC*)pReply)->m_AudioChannelId = audioChannelId;
			}
	 	}
	}
}

void MyDbusC::_audioControlInquireChannelInfoHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
	int retval = AC_E_OK;

	UiToAudioControlInquireChannelInfoReqC *req=(UiToAudioControlInquireChannelInfoReqC*)pRequest;

	if( pRequest )
	{
		if( thisObject )
		{
			if( req->m_bAllAudioChannel )
			{
				AudioChannelList audioInfoList;
				retval = AudioControlC::object()->inquireAllChannelInfo(audioInfoList);

				AudioControlToUiInquireAllChannelInfoRepC* pMyReply = new AudioControlToUiInquireAllChannelInfoRepC();
				for(int i=0; i<audioInfoList.count(); i++)
				{
					pMyReply->m_info[i].id = audioInfoList[i].id;
					pMyReply->m_info[i].vol = audioInfoList[i].iVirtualVol;
					pMyReply->m_info[i].mute = audioInfoList[i].bMute;
					pMyReply->m_info[i].status = audioInfoList[i].status;
				}
				pMyReply->m_iInfoNum = audioInfoList.count();
				pReply = pMyReply;
			}
			else
			{
				AudioChannelC audioInfo;
				audioInfo.id = req->m_AudioChannelId;
				retval = AudioControlC::object()->inquireChannelInfo(audioInfo);

				AudioControlToUiInquireChannelInfoRepC* pMyReply = new AudioControlToUiInquireChannelInfoRepC();
				pMyReply->m_iRetval = retval;
				pMyReply->m_info.id = audioInfo.id;
				pMyReply->m_info.vol = audioInfo.iVirtualVol;
				pMyReply->m_info.mute = audioInfo.bMute;
				pMyReply->m_info.status = audioInfo.status;
				pReply = pMyReply;
			}
	 	}
	}
}

MyDbusC::MyDbusC()
	: m_pDbusServer(NULL)
{
	_initDbus();
}

MyDbusC::~MyDbusC()
{

}

void MyDbusC::_initDbus()
{
	m_pDbusServer = new DBusServerC(ServerAudioControl, ObjectAudioControl);

	m_pDbusServer->connect();

	m_pDbusServer->registerService(REQ_ID_UI_TO_AUDIOCONTROL_COMMON, _audioControlCommonHandler, UiToAudioControlCommonReqC::factory);
	m_pDbusServer->registerService(REQ_ID_UI_TO_AUDIOCONTROL_INQUIRE_CHANNEL_INFO, _audioControlInquireChannelInfoHandler, UiToAudioControlInquireChannelInfoReqC::factory);
}

DBusServerC* MyDbusC::getDbusServer()
{
	return m_pDbusServer;
}

void MyDbusC::audioChannelStatusChanged(AudioChannelIdE audioChannelId, AudioChannelStatusE audioChannelStatus)
{
	AudioControlToUiChannelStatusNotifyC notify;
	notify.m_AudioChannelId = audioChannelId;
	notify.m_AudioChannelStatus = audioChannelStatus;

	m_pDbusServer->sendNotify(&notify);
}


}


