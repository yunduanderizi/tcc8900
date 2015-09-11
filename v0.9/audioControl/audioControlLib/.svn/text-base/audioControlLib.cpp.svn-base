#include <QDebug>
#include "audioControlLib.h"
#include "audioControlMessage.h"

namespace AudioControl1
{

/*
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
*/


AudioControlLibC  *AudioControlLibC::thisObject = NULL;
AudioControlLibC  *AudioControlLibC::object()
{
    AudioControlLibC *pTmp = NULL;

    //	gs_mutexForThreadSafe.lock();

    if( !thisObject )
    {
        thisObject = new AudioControlLibC();
    }

    pTmp = thisObject;

    //gs_mutexForThreadSafe.unlock();

    return pTmp;
}

void AudioControlLibC::releaseObject()
{
    //gs_mutexForThreadSafe.lock();

    if( thisObject )
    {
        delete thisObject;
        thisObject = NULL;
    }

    //gs_mutexForThreadSafe.unlock();
}

AudioControlLibC::AudioControlLibC()
    : m_pDbusClientToAudioControl(NULL),
      m_bInitFlag(false)
{
    _init();
}

AudioControlLibC::~AudioControlLibC()
{
    if( m_pDbusClientToAudioControl )
    {
        m_pDbusClientToAudioControl->disconnect();
        delete m_pDbusClientToAudioControl;
    }
}

void AudioControlLibC::_init()
{
    m_pDbusClientToAudioControl = new DBusClientProxyC(ServerAudioControl, ObjectAudioControl);
    m_pDbusClientToAudioControl->connect();

    m_pDbusClientToAudioControl->registerNotify(NOTF_ID_AUDIOCONTROL_TO_UI_CHANNEL_STATUS, _audioChannelStatusChangedHandler, AudioControlToUiChannelStatusNotifyC::factory);
    m_pDbusClientToAudioControl->registerReply(REP_ID_AUDIOCONTROL_TO_UI_COMMON, AudioControlToUiCommonRepC::factory);
    m_pDbusClientToAudioControl->registerReply(REP_ID_AUDIOCONTROL_TO_UI_INQUIRE_CHANNEL_INFO, AudioControlToUiInquireChannelInfoRepC::factory);
    m_pDbusClientToAudioControl->registerReply(REP_ID_AUDIOCONTROL_TO_UI_INQUIRE_ALL_CHANNEL_INFO, AudioControlToUiInquireAllChannelInfoRepC::factory);

    m_bInitFlag = true;
}

void AudioControlLibC::_audioChannelStatusChangedHandler(DBusNotifyC *notify)
{

    AudioControlToUiChannelStatusNotifyC *pNotify=(AudioControlToUiChannelStatusNotifyC*)notify;

    if( notify )
    {
        if( thisObject )
        {
            thisObject->_audioChannelStatusChanged(pNotify->m_AudioChannelId, pNotify->m_AudioChannelStatus);
        }
    }
}

void AudioControlLibC::_audioChannelStatusChanged(AudioChannelIdE audioChannelId, AudioChannelStatusE audioChannelStatus)
{
    emit sigAudioChannelStatusChange(audioChannelId, audioChannelStatus);
}

int AudioControlLibC::open(AudioChannelIdE audioChannelId, bool bBlock)
{
	int retval = AC_E_OK;


	UiToAudioControlCommonReqC	commandRequest;
	commandRequest.m_iCommandId = ACC_OPEN;
	commandRequest.m_AudioChannelId = audioChannelId;
	DBusReplyC      *pReply=NULL;
	if( bBlock )
	{
		commandRequest.m_bNeedReply = true;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);
	}
	else
	{
		commandRequest.m_bNeedReply = false;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 1);
	}

	if( pReply )
	{
		if( bBlock )
		{
			AudioControlToUiCommonRepC* pMyReply = (AudioControlToUiCommonRepC*)pReply;
			retval = pMyReply->m_iRetval;
		}

		delete pReply;
		pReply = NULL;
	}
	else
	{
		if( bBlock )
		{
			retval = AC_E_DBUS_TIMEOUT;
		}
	}

	return retval;
}

int AudioControlLibC::close(AudioChannelIdE audioChannelId, bool bBlock)
{
	int retval = AC_E_OK;

	UiToAudioControlCommonReqC	commandRequest;
	commandRequest.m_iCommandId = ACC_CLOSE;
	commandRequest.m_AudioChannelId = audioChannelId;
	DBusReplyC      *pReply=NULL;
	if( bBlock )
	{
		commandRequest.m_bNeedReply = true;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);
	}
	else
	{
		commandRequest.m_bNeedReply = false;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 1);
	}

	if( pReply )
	{
		if( bBlock )
		{
			AudioControlToUiCommonRepC* pMyReply = (AudioControlToUiCommonRepC*)pReply;
			retval = pMyReply->m_iRetval;
		}
		delete pReply;
		pReply = NULL;
	}
	else
	{
		if( bBlock )
		{
			retval = AC_E_DBUS_TIMEOUT;
		}
	}

	return retval;
}

int AudioControlLibC::adjustVol(AudioChannelIdE audioChannelId, int vol, bool bBlock)
{
	int retval = AC_E_OK;

	UiToAudioControlCommonReqC	commandRequest;
	commandRequest.m_iCommandId = ACC_ADJUST_VOL;
	commandRequest.m_AudioChannelId = audioChannelId;
	commandRequest.m_iVol = vol;
	DBusReplyC      *pReply=NULL;
	if( bBlock )
	{
		commandRequest.m_bNeedReply = true;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);
	}
	else
	{
		commandRequest.m_bNeedReply = false;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 1);
	}

	if( pReply )
	{
		if( bBlock )
		{
			AudioControlToUiCommonRepC* pMyReply = (AudioControlToUiCommonRepC*)pReply;
			retval = pMyReply->m_iRetval;
		}
		delete pReply;
		pReply = NULL;
	}
	else
	{
		if( bBlock )
		{
			retval = AC_E_DBUS_TIMEOUT;
		}
	}

	return retval;
}

int AudioControlLibC::mute(AudioChannelIdE audioChannelId, bool bBlock)
{
	int retval = AC_E_OK;

	UiToAudioControlCommonReqC	commandRequest;
	commandRequest.m_iCommandId = ACC_MUTE;
	commandRequest.m_AudioChannelId = audioChannelId;
	DBusReplyC      *pReply=NULL;
	if( bBlock )
	{
		commandRequest.m_bNeedReply = true;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);
	}
	else
	{
		commandRequest.m_bNeedReply = false;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 1);
	}

	if( pReply )
	{
		if( bBlock )
		{
			AudioControlToUiCommonRepC* pMyReply = (AudioControlToUiCommonRepC*)pReply;
			retval = pMyReply->m_iRetval;
		}
		delete pReply;
		pReply = NULL;
	}
	else
	{
		if( bBlock )
		{
			retval = AC_E_DBUS_TIMEOUT;
		}
	}

	return retval;
}

int AudioControlLibC::unMute(AudioChannelIdE audioChannelId, bool bBlock)
{
	int retval = AC_E_OK;

	UiToAudioControlCommonReqC	commandRequest;
	commandRequest.m_iCommandId = ACC_UNMUTE;
	commandRequest.m_AudioChannelId = audioChannelId;
	DBusReplyC      *pReply=NULL;
	if( bBlock )
	{
		commandRequest.m_bNeedReply = true;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);
	}
	else
	{
		commandRequest.m_bNeedReply = false;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 1);
	}

	if( pReply )
	{
		if( bBlock )
		{
			AudioControlToUiCommonRepC* pMyReply = (AudioControlToUiCommonRepC*)pReply;
			retval = pMyReply->m_iRetval;
		}
		delete pReply;
		pReply = NULL;
	}
	else
	{
		if( bBlock )
		{
			retval = AC_E_DBUS_TIMEOUT;
		}
	}

    return retval;
}

int AudioControlLibC::setSpeed(int iSpeed, bool bBlock)
{
	int retval = AC_E_OK;

	UiToAudioControlCommonReqC	commandRequest;
	commandRequest.m_iCommandId = ACC_SET_SPEED;
	commandRequest.m_iSpeed = iSpeed;
	DBusReplyC      *pReply=NULL;
	if( bBlock )
	{
		commandRequest.m_bNeedReply = true;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);
	}
	else
	{
		commandRequest.m_bNeedReply = false;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 1);
	}

	if( pReply )
	{
		if( bBlock )
		{
			AudioControlToUiCommonRepC* pMyReply = (AudioControlToUiCommonRepC*)pReply;
			retval = pMyReply->m_iRetval;
		}
		delete pReply;
		pReply = NULL;
	}
	else
	{
		if( bBlock )
		{
			retval = AC_E_DBUS_TIMEOUT;
		}
	}

	return retval;
}

int AudioControlLibC::openVolSpeed(bool bBlock)
{
	int retval = AC_E_OK;

	UiToAudioControlCommonReqC	commandRequest;
	commandRequest.m_iCommandId = ACC_OPEN_VOL_SPEED;
	DBusReplyC      *pReply=NULL;
	if( bBlock )
	{
		commandRequest.m_bNeedReply = true;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);
	}
	else
	{
		commandRequest.m_bNeedReply = false;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 1);
	}

	if( pReply )
	{
		if( bBlock )
		{
			AudioControlToUiCommonRepC* pMyReply = (AudioControlToUiCommonRepC*)pReply;
			retval = pMyReply->m_iRetval;
		}
		delete pReply;
		pReply = NULL;
	}
	else
	{
		if( bBlock )
		{
			retval = AC_E_DBUS_TIMEOUT;
		}
	}

	return retval;
}

int AudioControlLibC::closeVolSpeed(bool bBlock)
{
	int retval = AC_E_OK;

	UiToAudioControlCommonReqC	commandRequest;
	commandRequest.m_iCommandId = ACC_CLOSE_VOL_SPEED;
	DBusReplyC      *pReply=NULL;
	if( bBlock )
	{
		commandRequest.m_bNeedReply = true;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);
	}
	else
	{
		commandRequest.m_bNeedReply = false;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 1);
	}

	if( pReply )
	{
		if( bBlock )
		{
			AudioControlToUiCommonRepC* pMyReply = (AudioControlToUiCommonRepC*)pReply;
			retval = pMyReply->m_iRetval;
		}
		delete pReply;
		pReply = NULL;
	}
	else
	{
		if( bBlock )
		{
			retval = AC_E_DBUS_TIMEOUT;
		}
	}

	return retval;
}

int AudioControlLibC::getCurrentOpenAudioChannel(AudioChannelIdE& audioChannelId)
{
	int retval = AC_E_OK;

	UiToAudioControlCommonReqC	commandRequest;
	commandRequest.m_iCommandId = ACC_GET_CURRENT_OPEN_AUDIO_CHANNEL;
	DBusReplyC      *pReply=NULL;
	commandRequest.m_bNeedReply = true;
	m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);

	if( pReply )
	{
		AudioControlToUiCommonRepC* pMyReply = (AudioControlToUiCommonRepC*)pReply;
		audioChannelId = pMyReply->m_AudioChannelId;
		retval = pMyReply->m_iRetval;
		delete pReply;
		pReply = NULL;
	}
	else
	{
		retval = AC_E_DBUS_TIMEOUT;
	}

	return retval;
}

int AudioControlLibC::inquireChannelInfo(AudioChannelC& channelInfo)
{
	int retval = AC_E_OK;

	UiToAudioControlInquireChannelInfoReqC commandRequest;
	commandRequest.m_bAllAudioChannel = false;
	commandRequest.m_AudioChannelId = channelInfo.id;
		
	DBusReplyC      *pReply=NULL;
	m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);

	if( pReply )
	{
		AudioControlToUiInquireChannelInfoRepC* pMyReply = (AudioControlToUiInquireChannelInfoRepC*)pReply;
		channelInfo.iVirtualVol = pMyReply->m_info.vol;
		channelInfo.bMute = (bool)(pMyReply->m_info.mute);
		channelInfo.status = pMyReply->m_info.status;
		retval = pMyReply->m_iRetval;
		delete pReply;
		pReply = NULL;
	}
	else
	{
		retval = AC_E_DBUS_TIMEOUT;
	}

	return retval;
}

int AudioControlLibC::inquireAllChannelInfo(AudioChannelList& channelInfoList)
{
	int retval = AC_E_OK;

	UiToAudioControlInquireChannelInfoReqC commandRequest;
	commandRequest.m_bAllAudioChannel = true;
		
	DBusReplyC      *pReply=NULL;
	m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);
	if( pReply )
	{
		AudioControlToUiInquireAllChannelInfoRepC* pMyReply = (AudioControlToUiInquireAllChannelInfoRepC*)pReply;
		channelInfoList.clear();
		AudioChannelC audioChannel;
		for(int i=0; i<pMyReply->m_iInfoNum; i++)
		{
			audioChannel.id = pMyReply->m_info[i].id;
			audioChannel.iVirtualVol = pMyReply->m_info[i].vol;
			audioChannel.bMute = (bool)(pMyReply->m_info[i].mute);
			audioChannel.status = pMyReply->m_info[i].status;
			channelInfoList.append(audioChannel);
		}
		delete pReply;
		pReply = NULL;
	}
	else
	{
		retval = AC_E_DBUS_TIMEOUT;
	}

	return retval;
}

int AudioControlLibC::resumeDataToDefault(bool bBlock)
{
	int retval = AC_E_OK;

	UiToAudioControlCommonReqC	commandRequest;
	commandRequest.m_iCommandId = ACC_RESUME_DATA_TO_DEFAULT;
	DBusReplyC      *pReply=NULL;
	if( bBlock )
	{
		commandRequest.m_bNeedReply = true;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 5000);
	}
	else
	{
		commandRequest.m_bNeedReply = false;
		m_pDbusClientToAudioControl->sendService(&commandRequest, pReply, 1);
	}

	if( pReply )
	{
		if( bBlock )
		{
			AudioControlToUiCommonRepC* pMyReply = (AudioControlToUiCommonRepC*)pReply;
			retval = pMyReply->m_iRetval;
		}
		delete pReply;
		pReply = NULL;
	}
	else
	{
		if( bBlock )
		{
			retval = AC_E_DBUS_TIMEOUT;
		}
	}

	return retval;
}






}


