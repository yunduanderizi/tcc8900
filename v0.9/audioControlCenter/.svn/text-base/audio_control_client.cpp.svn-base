#include <stdio.h>
#include "debugMonitor.h"
#include "audio_control_client.h"
#ifndef WITH_NEW_AUDIO_CONTROL_SYSTEM
#include "audio_control_message.h"
#endif
//#define IBAC_AUDIO_CONFIG_LOG

#ifdef IBAC_AUDIO_CONFIG_LOG
#else
#endif

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
#include "AudioManageMessage.h"
DBusClientProxyC *g_pAudioDBusClient = NULL;
#endif

// class AudioCtrlClientC
AudioCtrlClientC::AudioCtrlClientC(DBusClientProxyC *pClient)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        g_pAudioDBusClient = new DBusClientProxyC(ServerAudioManager, ObjectAudioManager);
        if(g_pAudioDBusClient->connect() < 0)
        {   
                const char* tmp = (char *)"g_pAudioDBusClient cann't connect to Audio server";
                IBD_BEGIN(DEBUG_WARNING)<<tmp<<IBD_ENDL<<IBD_END
                fprintf(stderr,"...%d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);
        }   
        else
        {   
        }   

#else
	//mvol = new Main_Volume();

	// client object pointer
	client = pClient;
	
	pThisAudioCtrlClient = this;

	// register reply
	client->registerReply(REP_ID_AUDIO_CTRL, AudioCtrlReplyC::factory);
	client->registerReply(REP_ID_AUDIO_GET, AudioGetReplyC::factory);	
	client->registerReply(REP_ID_AUDIO_SET, AudioSetReplyC::factory);	
	client->registerReply(REP_ID_AUDIO_SPEEDSETVOL, AudioSpeedSetVolReplyC::factory);
	
	// register notify handler
	client->registerNotify(NOTF_ID_AUDIO_CTRL, audioNotifyHandler, AudioCtrlNotifyC::factory);
	client->registerNotify(NOTF_ID_AUDIO_GET, audioNotifyHandler, AudioGetNotifyC::factory);
	client->registerNotify(NOTF_ID_AUDIO_SET, audioNotifyHandler, AudioSetNotifyC::factory);
//	client->registerNotify(NOTF_ID_AUDIO_SPEEDSETVOL, audioNotifyHandler, AudioSpeedSetVolNotifyC::factory);	

	//initialize the audio info
	devId = 0;
	volume = IBAC_VOL_INVAL;
	on_off = IBAC_VOL_INVAL;

	// callback object instance
	pCCallback = new AudioCtrlCallbackC();

	//thread lock
	pthread_mutexattr_t     mutexattr;

        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_lock,&mutexattr);
#endif
}

AudioCtrlClientC::~AudioCtrlClientC()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
#else
	delete pCCallback;
#endif
}

AudioCtrlClientC* AudioCtrlClientC::pThisAudioCtrlClient = NULL;

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
int AudioCtrlClientC::setChannelIDMute(bool flag, int channelID)
{
        AudioManagerChannelMuteStateReqC request;
        AudioManagerChannelMuteStateRepC *reply = NULL;
        DBusReplyC *tmpReply=NULL;

        request.channelID = channelID;
        request.mute_state = flag; 
        int result=g_pAudioDBusClient->sendService(&request, tmpReply, 3 * 1000);
        reply = (AudioManagerChannelMuteStateRepC *)tmpReply;

        if(result<0)
        {    
                char* tmp = (char *)"send to Audio Service  fail    ";
                char *result1 = (char *)"result is:  ";
                IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<result1<<result<<IBD_ENDL<<IBD_END
                return -1; 
        }    
        else if(reply->getIdentifier() != REP_ID_MANAGER_CHANNEL_MUTE)
        {    
                char* tmp = (char *)"send to Audio Service  fail    ";
                char *result1 = (char *)"result is:  ";
                IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<result1<<result<<IBD_ENDL<<IBD_END
                delete reply;
                return -1; 
        }    
        else if(reply->result < 0 || channelID != reply->channelID)
        {    
                char* tmp = (char *)"request Audio Service  fail    ";
                char *result1 = (char *)"result is:  ";
                IBD_BEGIN(DEBUG_EMERGENCY)<<tmp<<result1<<reply->result<<IBD_ENDL<<IBD_END
                delete reply;
                return -1; 
        }    
        else 
        {    
                char* tmp = (char *)"send to Audio Service  sucess   ";
                IBD_BEGIN(DEBUG_INFOMATION)<<tmp<<IBD_ENDL<<IBD_END
        }    
        delete reply;

        return 0;
}
#endif

int AudioCtrlClientC::openAudio(unsigned int id, int vol)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        if(14 == id)
        {
                setChannelIDMute(false, IBAC_DEV_ID_DAI_GPS);
        }
        else
        {
        }
        return 0;

#else
//	InternalLock	lock(&m_lock);

	return (audioChange(id, vol, IBAC_AUDIO_SWITCH_ON, IBAC_AUDIO_OP_OPEN));
#endif
}

int AudioCtrlClientC::closeAudio(unsigned int id)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(id, 0, IBAC_AUDIO_SWITCH_OFF, IBAC_AUDIO_OP_CLOSE));
#endif
}

int AudioCtrlClientC::adjustAudioVol(unsigned int id, int vol)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(id, vol, -1, IBAC_AUDIO_OP_VOLUME));
#endif
}

int AudioCtrlClientC::muteAudio(unsigned int id)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(id, -1, IBAC_AUDIO_SWITCH_OFF, IBAC_AUDIO_OP_MUTE));
#endif
}

int AudioCtrlClientC::unmuteAudio(unsigned int id)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(id, -1, IBAC_AUDIO_SWITCH_ON, IBAC_AUDIO_OP_UNMUTE));
#endif
}

int AudioCtrlClientC::saveAudioInfo()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(1, -1, -1, IBAC_AUDIO_OP_SAVE));
#endif
}
int AudioCtrlClientC::setDefaultParameters()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(1, -1, -1, IBAC_AUDIO_OP_SETDEF));
#endif
}
int AudioCtrlClientC::openWarning()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#endif

        Main_Volume *mvol = new Main_Volume(); 
        s_vol = mvol->get_main_volume();
        int mute_status = mvol->get_mute_status();
        openAudio(IBAC_DEV_ID_DAI_COMMAND, 25);
	adjustAudioVol(IBAC_DEV_ID_DAI_VIDEO, 0);
	adjustAudioVol(IBAC_DEV_ID_RADIO, 0);
	adjustAudioVol(IBAC_DEV_ID_DAI_GPS, 0);
#ifdef _8925_8D1
        mvol->set_main_volume(20);
	if(mute_status == 1)
	{
		mvol->set_unmute();
		mute_flag = true;
	}else{
		mute_flag = false;
	}
#else
	if(mute_status == 1)
	{
		mvol->set_unmute();
                mvol->set_main_volume(1);
		mute_flag = true;
	}else{
	        if(s_vol == 0)
                        mvol->set_main_volume(1);
		mute_flag = false;
	}
#endif
        delete mvol;
	return 1;
}

int  AudioCtrlClientC::openWarning_normal()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#endif

        Main_Volume *mvol = new Main_Volume(); 
        int vol =  mvol->get_main_volume();
        int mute_status = mvol->get_mute_status();

        if(mute_status == 1)
        {
                mvol->set_unmute();
                mute_flag = true;
        }else{
                mute_flag = false;
        }

#ifdef _8925_8D1
        mvol->set_main_volume(20);
        openAudio(IBAC_DEV_ID_DAI_COMMAND, 20);
#else
        openAudio(IBAC_DEV_ID_DAI_COMMAND, -1);
#endif
        delete mvol;	
        return 1;
}


int AudioCtrlClientC::recoveryMute()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#endif

	Main_Volume *mvol = new Main_Volume(); 

	adjustAudioVol(IBAC_DEV_ID_DAI_VIDEO, 25);
	adjustAudioVol(IBAC_DEV_ID_RADIO, 25);
	adjustAudioVol(IBAC_DEV_ID_DAI_GPS, 25);

	if( s_vol != 12345 ){
		 mvol->set_main_volume(s_vol);
		s_vol = 12345;
	}

	if( mute_flag == true )
	{
		mvol->set_mute();
	}

	closeAudio(IBAC_DEV_ID_DAI_COMMAND);

	delete mvol;
	return 1;
}

int AudioCtrlClientC::openSpeechRecognition()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(1, -1, -1, IBAC_AUDIO_OP_OPENSR));
#endif
}

int AudioCtrlClientC::closeSpeechRecognition()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(1, -1, -1, IBAC_AUDIO_OP_CLOSESR));
#endif
}
int AudioCtrlClientC::openSetAudioVolAccToSpeed()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(1, -1, -1, IBAC_AUDIO_OP_OPENSPDVOL));
#endif
}

int AudioCtrlClientC::closeSetAudioVolAccToSpeed()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(1, -1, -1, IBAC_AUDIO_OP_CLOSESPDVOL));
#endif
}

#ifdef IBAC_VOLUME_MAIN_OFFSET
int AudioCtrlClientC::setMainVolOffset(int offset)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	return (audioChange(1, offset, -1, IBAC_AUDIO_OP_OFFSET));
#endif
}
#endif
/*set up interface*/
//set up
int AudioCtrlClientC::setupAudioChannel(unsigned int id, int ctl)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	//if ((ctl&0x04) != 0) {
	if (ctl == ALWAYSTYPE) {//ALWAYSTYPE=2
		return (audioSetupChange(id, -1, -1, IBAC_AUDIO_OP_ALWAYS));
	}
	//else if ((ctl&0x02) != 0) {
	else if (ctl == MIXTYPE) {//MIXTYPE=1
		return (audioSetupChange(id, -1, -1, IBAC_AUDIO_OP_MIX));
	}
	//else if ((ctl&0x01) != 0) {
	else if (ctl == MONOTYPE) {//MONOTYPE=0
		return (audioSetupChange(id, -1, -1, IBAC_AUDIO_OP_MONO));
	}	
	return 0;
#endif
}

int AudioCtrlClientC::getAudioInfo(unsigned int id, int *vol, int* sw_on_off, unsigned char *sStatus)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	int ret = -1;
	if ((ret = audioChange(id, -1, -1, IBAC_AUDIO_OP_QUERY)) == -1)
	{
		IBD_PRINTF(4,"Audio Clinet: get audio info failed !!!\n");
		return -1;
	}
	*vol = volume;
	*sw_on_off = on_off;
	*sStatus = soundStatus;
	return 0;	
#endif
}
int AudioCtrlClientC::getAudioInfoExt(unsigned int id, int *vol, int* sw_on_off, unsigned char *sStatus, unsigned char *oStatus)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	int ret = -1;
	if ((ret = audioChange(id, -1, -1, IBAC_AUDIO_OP_QUERYEXT)) == -1)
	{
		IBD_PRINTF(4,"Audio Clinet: get audio info failed !!!\n");
		return -1;
	}
	*vol = volume;
	*sw_on_off = on_off;
	*sStatus = soundStatus;
	*oStatus = otherStatus;	
	return 0;	
#endif
}
//get all info.
int AudioCtrlClientC::getSetupAudioInfo(audioControlArray_t *info)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
//	InternalLock	lock(&m_lock);

	int tmp = -1;
	// create the request message
	AudioGetRequestC request;
	// initialize the reply
	DBusReplyC *reply = NULL;
	// send the message to server to request device open, close or volume adjust
	int result = client->sendService(&request, reply, 5*1000);
	if (result < 0) {
		IBD_PRINTF(4,"audio client-->server: error send message to server, \
				error code : %d. \n", result);
		return -1;
	}
	else if(reply->getIdentifier() == REP_ID_AUDIO_GET) {
		AudioGetReplyC *pReply = (AudioGetReplyC*)reply;	
		for (int i = 0; i < IBAC_DEV_ID_MAX; i++) {
			*(info+i) = pReply->allInfo[i];
		}
		tmp = reply->result;
//#ifdef IBAC_AUDIO_CONFIG_LOG
		IBD_PRINTF(4,"audio_client_test<--server got reply from server, result: %d. \n", tmp);			
//#endif
		delete reply;
	} 
#ifdef IBAC_AUDIO_CONFIG_LOG	
	IBD_PRINTF(4,"\nAudio Client --> audio change end !!!!!! \n");
#endif
	return tmp;	
#endif
}

int AudioCtrlClientC::registerNotiHandler(unsigned int id, AudioCtrlCBHandlerC* pCNotiHandle)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
	InternalLock	lock(&m_lock);

	pCCallback->registerHandler(id, pCNotiHandle);
	
	return 0;
#endif
}

int AudioCtrlClientC::deRegisterNotiHandler(unsigned int id, AudioCtrlCBHandlerC* pCNotiHandle)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
	InternalLock	lock(&m_lock);

	pCCallback->deRegisterHandler(id, pCNotiHandle);

	return 0;
#endif
}

void AudioCtrlClientC::audioNotifyHandler(DBusNotifyC *notify)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
#else

	AudioCtrlNotifyC *audioCtrlNotify = (AudioCtrlNotifyC*)notify;
	
    	if (NOTF_ID_AUDIO_CTRL == audioCtrlNotify->getIdentifier())
	{
#ifdef IBAC_AUDIO_CONFIG_LOG
		IBD_PRINTF(4,"audio client received CTRL_notify message ---> device id: %3d, volume: %3d, switch : %3d, status : 0x%2x\n", \
			audioCtrlNotify->devId, audioCtrlNotify->volume, audioCtrlNotify->on_off, audioCtrlNotify->soundStatus);
#endif
		pThisAudioCtrlClient->pCCallback->callHandlder(audioCtrlNotify->devId, audioCtrlNotify->volume, audioCtrlNotify->on_off, audioCtrlNotify->soundStatus);

	}
    	if (NOTF_ID_AUDIO_SET == audioCtrlNotify->getIdentifier())
    	{
#ifdef IBAC_AUDIO_CONFIG_LOG
		IBD_PRINTF(4,"audio client received SET_notify message ---> device id: %3d, volume: %3d, switch : %3d, status : 0x%2x\n", \
			audioCtrlNotify->devId, audioCtrlNotify->volume, audioCtrlNotify->on_off, audioCtrlNotify->soundStatus);
#endif
		pThisAudioCtrlClient->pCCallback->callHandlder(audioCtrlNotify->devId, audioCtrlNotify->volume, audioCtrlNotify->on_off, audioCtrlNotify->soundStatus);
    	}
    	if (NOTF_ID_AUDIO_GET == audioCtrlNotify->getIdentifier())
    	{
#ifdef IBAC_AUDIO_CONFIG_LOG
		IBD_PRINTF(4,"audio client received GET_notify message ---> device id: %3d, volume: %3d, switch : %3d, status : 0x%2x\n", \
			audioCtrlNotify->devId, audioCtrlNotify->volume, audioCtrlNotify->on_off, audioCtrlNotify->soundStatus);
#endif
		pThisAudioCtrlClient->pCCallback->callHandlder(audioCtrlNotify->devId, audioCtrlNotify->volume, audioCtrlNotify->on_off, audioCtrlNotify->soundStatus);
    	}		
#endif
}

int AudioCtrlClientC::audioSetupChange(unsigned int id, int vol, int sw_on_off, int op_code)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
	int tmp = -1;

	// create the request message
	AudioSetRequestC request;

	// fill the request message
	request.devId = id;
	request.operId = op_code;
	request.volume = vol;
	request.on_off = sw_on_off;

	// initialize the reply
	DBusReplyC *reply = NULL;

	int result = client->sendService(&request, reply, 5*1000);

	if (result < 0)
	{
		IBD_PRINTF(4,"audio client-->server: error send message to server, \
				op code: %d, error code : %d. \n", op_code, result);
		return -1;
	}
	else if(reply->getIdentifier() == REP_ID_AUDIO_SET)
	{
		AudioSetReplyC *pReply = (AudioSetReplyC*)reply;	
		// update the info from reply message
		if ((op_code==IBAC_AUDIO_OP_QUERY||op_code==IBAC_AUDIO_OP_QUERYEXT) && id == pReply->devId)
		{
			volume = pReply->volume;
			on_off = pReply->on_off;
			soundStatus = pReply->soundStatus;
			otherStatus = pReply->otherStatus;
		}
		
		tmp = reply->result;

#ifdef IBAC_AUDIO_CONFIG_LOG
		IBD_PRINTF(4,"audio client<--server got reply from server, result: %d. \n", tmp);			
#endif
		delete reply;

	} 
	
#ifdef IBAC_AUDIO_CONFIG_LOG	
	IBD_PRINTF(4,"\nAudio Client --> audio change end !!!!!! \n");
#endif
	return tmp;
#endif
}

int AudioCtrlClientC::audioChange(unsigned int id, int vol, int sw_on_off, int op_code)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
	int tmp = -1;

	// create the request message
	AudioCtrlRequestC request;

	// fill the request message
	request.devId = id;
	request.operId = op_code;
	request.volume = vol;
	request.on_off = sw_on_off;

	// initialize the reply
	DBusReplyC *reply = NULL;

	// send the message to server to request device open, close or volume adjust
	int result = client->sendService(&request, reply, 5*1000);

	if (result < 0)
	{
		IBD_PRINTF(4,"audio client-->server: error send message to server, \
				op code: %d, error code : %d. \n", op_code, result);
		return -1;
	}
	else if(reply->getIdentifier() == REP_ID_AUDIO_CTRL)
	{
		AudioCtrlReplyC *pReply = (AudioCtrlReplyC*)reply;	
		// update the info from reply message
		if ((op_code==IBAC_AUDIO_OP_QUERY||op_code==IBAC_AUDIO_OP_QUERYEXT)&& id == pReply->devId)
		{
			volume = pReply->volume;
			on_off = pReply->on_off;
			soundStatus = pReply->soundState;
			otherStatus = pReply->otherState;
		}
		
		tmp = reply->result;

#ifdef IBAC_AUDIO_CONFIG_LOG
		IBD_PRINTF(4,"audio client<--server got reply from server, result: %d. \n", tmp);			
#endif
		delete reply;

	} 
	
#ifdef IBAC_AUDIO_CONFIG_LOG	
	IBD_PRINTF(4,"\nAudio Client --> audio change end !!!!!! \n");
#endif
	return tmp;
#endif
}

void AudioCtrlClientC::openAudioDevInit()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
#else
//	InternalLock	lock(&m_lock);

	audioChange(IBAC_DEV_ID_MAIN_VOLUME, (int)100*100/127, IBAC_AUDIO_SWITCH_ON, IBAC_AUDIO_OP_OPEN);	// main volume
#endif
}

int AudioCtrlClientC::setAudioVolAccToSpeed(int carSpeed)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        return 0;
#else
    int tmp = -1;
   	// create the request message
	AudioSpeedSetVolRequestC request;
	
	// fill the request message
	request.carSpeed = carSpeed;
	
	// initialize the reply
	DBusReplyC *reply = NULL;
	
	int result = client->sendService(&request, reply, 5*1000);
	if (result < 0)
	{
		IBD_PRINTF(DEBUG_TRACEFUNC,"audio client-->server: error send message to server, \
				carSpeed: %d, error code: %d. \n", carSpeed, result);
		return -1;
	}
	else if(reply->getIdentifier() == REP_ID_AUDIO_SPEEDSETVOL)
	{
		//AudioSpeedSetVolReplyC* pReply = (AudioSpeedSetVolReplyC*)reply;

		// update the info from reply message	
		tmp = reply->result;

#ifdef IBAC_AUDIO_CONFIG_LOG
		IBD_PRINTF(DEBUG_TRACEFUNC,"audio client<--server got reply from server, result: %d. \n", tmp);			
#endif
		delete reply;
	} 
	
#ifdef IBAC_AUDIO_CONFIG_LOG	
	IBD_PRINTF(DEBUG_TRACEFUNC,"\nAudio Client --> audio change end !!!!!! \n");
#endif
	return tmp;
#endif
}


AudioCtrlClientC* AudioCtrlClientC::getAudioCtrlClientInstance()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
#else
	return pThisAudioCtrlClient;
#endif
}


// internal lock
AudioCtrlClientC::InternalLock::InternalLock(pthread_mutex_t* lock)
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
#else
	m_lock=lock;
	pthread_mutex_lock(m_lock);
#endif
}
AudioCtrlClientC::InternalLock::~InternalLock()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
#else
	pthread_mutex_unlock(m_lock);
#endif
}

