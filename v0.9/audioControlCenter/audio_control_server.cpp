#include <stdio.h>

#include "debugMonitor.h"
#include "audio_control_server.h"
#include "audio_control_message.h"

//#define IBAC_AUDIO_CONFIG_LOG

#ifdef IBAC_AUDIO_CONFIG_LOG
#else
#endif



// class AudioChangeNotifier
AudioChangeNotifier::AudioChangeNotifier(DBusServerC *pServer)
{
	server = pServer;
}

void AudioChangeNotifier::audioStatusUpdate(unsigned int devId, int volume, int on_off, unsigned char soundStatus)
{
	// call the callback registered in the server, if no handler in server, send notify to client
	if (callHandlder(devId, volume, on_off, soundStatus) == -1)	
	{
		// generate the notify message
		AudioCtrlNotifyC audioNotify;
	
		audioNotify.devId = devId;
		audioNotify.volume = volume;	
		audioNotify.on_off = on_off;
		audioNotify.soundStatus = soundStatus;

		// send the notify message
		server->sendNotify(&audioNotify);
	}

}

// class AudioCtrlServerC

AudioCtrlServerC::AudioCtrlServerC(DBusServerC *pServer)
{
	// server object pointer
	server = pServer;
	
	// notifier 
	pNotifier = new AudioChangeNotifier(pServer);
	
	// audio controller
	audioCtrl = new AudioControlC(pNotifier);

	pThisAudioServer = this;

	// register service handler
	server->registerService(REQ_ID_AUDIO_CTRL, AudioServiceHandler, AudioCtrlRequestC::factory);	
	server->registerService(REQ_ID_AUDIO_GET, AudioServiceGetHandler, AudioGetRequestC::factory);		
	server->registerService(REQ_ID_AUDIO_SET, AudioServiceSetHandler, AudioSetRequestC::factory);		
	server->registerService(REQ_ID_AUDIO_SPEEDSETVOL, AudioServiceSpeedSetVolHandler, AudioSpeedSetVolRequestC::factory);

	//thread lock
	pthread_mutexattr_t     mutexattr;

        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_lock,&mutexattr);	
}

AudioCtrlServerC::~AudioCtrlServerC()
{
	delete pNotifier;
	delete audioCtrl;	
}

AudioCtrlServerC*  AudioCtrlServerC::pThisAudioServer = NULL;

int AudioCtrlServerC::initServer()
{
	return audioCtrl->initAudio();	
}

int AudioCtrlServerC::openAudio(unsigned int id, int vol)
{
	return (audioCtrl->openAudio(id, vol));
}

int AudioCtrlServerC::closeAudio(unsigned int id)
{
	return (audioCtrl->closeAudio(id));
}

int AudioCtrlServerC::adjustAudioVol(unsigned int id, int vol)
{
	return (audioCtrl->adjustAudioVol(id, vol));
}

int AudioCtrlServerC::muteAudio(unsigned int id)
{
	return (audioCtrl->muteAudio(id));
}

int AudioCtrlServerC::unmuteAudio(unsigned int id)
{
	return (audioCtrl->unmuteAudio(id));
}

int AudioCtrlServerC::getAudioInfo(unsigned int id, int *vol, int* sw_on_off, unsigned char *sStatus)
{
	return audioCtrl->getAudio(id, vol, sw_on_off, sStatus);
}

int AudioCtrlServerC::getAudioInfoExt(unsigned int id, int *vol, int* sw_on_off, unsigned char *sStatus, unsigned char *oStatus)
{
	return audioCtrl->getAudioExt(id, vol, sw_on_off, sStatus, oStatus);
}

int AudioCtrlServerC::saveAudioInfo()
{
	return (audioCtrl->saveAudio());
}
int AudioCtrlServerC::openSpeechRecognition()
{
	return (audioCtrl->openSpeechRecognition());
}
int AudioCtrlServerC::closeSpeechRecognition()
{
	return (audioCtrl->closeSpeechRecognition());
}
#ifdef IBAC_VOLUME_MAIN_OFFSET
int AudioCtrlServerC::setMainVolOffset(int offset)
{
	return (audioCtrl->setMainVolOffset(offset));
}
#endif

int AudioCtrlServerC::openSetAudioVolAccToSpeed()
{
	return (audioCtrl->openSetAudioVolAccToSpeed());
}
int AudioCtrlServerC::closeSetAudioVolAccToSpeed()
{
	return (audioCtrl->closeSetAudioVolAccToSpeed());
}

void AudioCtrlServerC::AudioServiceGetHandler(DBusRequestC *request, DBusReplyC *&reply)
{

	unsigned int devId;
	int volume, on_off, opcode;
	int ret = -1;
	unsigned char sStatus;
	if(REQ_ID_AUDIO_GET != request->getIdentifier())
		return;
		
	// get the message
	AudioGetRequestC *pReq = (AudioGetRequestC *)request;
	
#ifdef IBAC_AUDIO_CONFIG_LOG

//	IBD_PRINTF(4,"audio server received message,op code: %d, devId: %d, volume: %d, switch: %d. \n", opcode, devId, volume, on_off);

#endif	

	audioControlArray_t info[IBAC_DEV_ID_MAX];

	ret = pThisAudioServer->audioCtrl->getAllAudio(info);
	// check the return value
	if (ret < 0)
	{
		IBD_PRINTF(4,"[Audio Control Server]: audio operation error !!! \n");
	}
	AudioGetReplyC *pReply = new AudioGetReplyC();
	for (int i=0; i<IBAC_DEV_ID_MAX; i++) {
		pReply->allInfo[i] = info[i];
	}
	pReply->result = ret;
	reply = pReply;
#ifdef IBAC_AUDIO_CONFIG_LOG
	IBD_PRINTF(4,"\naudio server ---> create reply message end! \n");
#endif

}
void AudioCtrlServerC::AudioServiceSetHandler(DBusRequestC *request, DBusReplyC *&reply)
{

	unsigned int devId;
	int volume, on_off, opcode;
	int ret = -1;
	unsigned char sStatus;
	if(REQ_ID_AUDIO_SET != request->getIdentifier())
		return;
		
	// get the message
	AudioSetRequestC *pReq = (AudioSetRequestC *)request;
	devId = pReq->devId;
	volume = pReq->volume;
	on_off = pReq->on_off;	
	opcode = pReq->operId;
	
#ifdef IBAC_AUDIO_CONFIG_LOG
	IBD_PRINTF(4,"audio server received message, \
             op code: %d, devId: %d, volume: %d, switch: %d. \n", opcode, devId, volume, on_off);
#endif	
	// validate the message
	if (devId > IBAC_DEV_ID_MAX || opcode > IBAC_AUDIO_OP_MAX)
	{
		ret = -1;
#ifdef IBAC_AUDIO_CONFIG_LOG
		IBD_PRINTF(4,"audio server received message error field, devId: %d\n", devId);
#endif	
	}
	switch (opcode)
	{
		case IBAC_AUDIO_OP_ALWAYS:
			ret = pThisAudioServer->audioCtrl->setupAudio(devId, ALWAYSTYPE);
			break;
		case IBAC_AUDIO_OP_MIX:
			ret = pThisAudioServer->audioCtrl->setupAudio(devId, MIXTYPE);
			break;
		case IBAC_AUDIO_OP_MONO:
			ret = pThisAudioServer->audioCtrl->setupAudio(devId, MONOTYPE);
			break;

		default:
			break; 

	}
	// check the return value
	if (ret < 0)
	{
		IBD_PRINTF(4,"[Audio Control Server]: audio operation error !!! \n");
	}

	AudioSetReplyC *pReply = new AudioSetReplyC();
	pReply->devId = devId;
	pReply->volume = volume;
	pReply->on_off = on_off;	
	pReply->soundStatus = sStatus;
	pReply->result = ret;
	reply = pReply;

#ifdef IBAC_AUDIO_CONFIG_LOG
	IBD_PRINTF(4,"\naudio server ---> create reply message end! \n");
#endif

}
void AudioCtrlServerC::AudioServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
	unsigned int devId;
	int volume, on_off, opcode;
	int ret = -1;
	unsigned char sStatus;
	unsigned char oStatus;
	if(REQ_ID_AUDIO_CTRL != request->getIdentifier())
		return;
		
	// get the message
	AudioCtrlRequestC *pReq = (AudioCtrlRequestC *)request;
	devId = pReq->devId;
	volume = pReq->volume;
	on_off = pReq->on_off;	
	opcode = pReq->operId;
	
#ifdef IBAC_AUDIO_CONFIG_LOG
	IBD_PRINTF(4,"audio server received message, \
             op code: %d, devId: %d, volume: %d, switch: %d. \n", opcode, devId, volume, on_off);
#endif	
	// validate the message
	if (devId > IBAC_DEV_ID_MAX || opcode > IBAC_AUDIO_OP_MAX)
	{
		ret = -1;
#ifdef IBAC_AUDIO_CONFIG_LOG
		IBD_PRINTF(4,"audio server received message error field, devId: %d\n", devId);
#endif	
	}

	switch (opcode)
	{
		case IBAC_AUDIO_OP_OPEN:
			ret = pThisAudioServer->audioCtrl->openAudio(devId, volume);
			break;

		case IBAC_AUDIO_OP_CLOSE:
			ret = pThisAudioServer->audioCtrl->closeAudio(devId);
			break;

		case IBAC_AUDIO_OP_VOLUME:
			ret = pThisAudioServer->audioCtrl->adjustAudioVol(devId, volume);
			break;

		case IBAC_AUDIO_OP_MUTE:
			ret = pThisAudioServer->audioCtrl->muteAudio(devId);
			break;

		case IBAC_AUDIO_OP_UNMUTE:
			ret = pThisAudioServer->audioCtrl->unmuteAudio(devId);
			break;

		case IBAC_AUDIO_OP_QUERY:
			ret = pThisAudioServer->audioCtrl->getAudio(devId, &volume, &on_off, &sStatus);
			break;

		case IBAC_AUDIO_OP_QUERYEXT:
			ret = pThisAudioServer->audioCtrl->getAudioExt(devId, &volume, &on_off, &sStatus, &oStatus);
			break;
		
		case IBAC_AUDIO_OP_SAVE:
			ret = pThisAudioServer->audioCtrl->saveAudio();
			break;
			
		case IBAC_AUDIO_OP_SETDEF:
			ret = pThisAudioServer->audioCtrl->setDefaultParameters();
			break;

		case IBAC_AUDIO_OP_OPENSR:
			ret = pThisAudioServer->audioCtrl->openSpeechRecognition();
			break;

		case IBAC_AUDIO_OP_CLOSESR:
			ret = pThisAudioServer->audioCtrl->closeSpeechRecognition();
			break;
			
		case IBAC_AUDIO_OP_OPENSPDVOL:
			ret = pThisAudioServer->audioCtrl->openSetAudioVolAccToSpeed();
			break;
			
		case IBAC_AUDIO_OP_CLOSESPDVOL:
			ret = pThisAudioServer->audioCtrl->closeSetAudioVolAccToSpeed();
			break;
			
#ifdef IBAC_VOLUME_MAIN_OFFSET
        case IBAC_AUDIO_OP_OFFSET:
			ret = pThisAudioServer->audioCtrl->setMainVolOffset(volume);
			break;
#endif			
		default:
			break; 

	}


	// check the return value
	if (ret < 0)
	{
		IBD_PRINTF(4,"[Audio Control Server]: audio operation error !!! \n");
	}

	AudioCtrlReplyC *pReply = new AudioCtrlReplyC();
	pReply->devId = devId;
	pReply->volume = volume;
	pReply->on_off = on_off;	
	pReply->result = ret;
	pReply->soundState = sStatus;
	pReply->otherState = oStatus;
	reply = pReply;

#ifdef IBAC_AUDIO_CONFIG_LOG
	IBD_PRINTF(4,"\naudio server ---> create reply message end! \n");
#endif

}
void AudioCtrlServerC::AudioServiceSpeedSetVolHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    int carSpeed = 0;
    int ret = -1;
    
	if(REQ_ID_AUDIO_SPEEDSETVOL != request->getIdentifier())
		return;
		
	// get the message
	AudioSpeedSetVolRequestC *pReq = (AudioSpeedSetVolRequestC *)request;
	carSpeed = pReq->carSpeed;
	
	ret = pThisAudioServer->audioCtrl->setAudioVolAccToSpeed(carSpeed);
    if(ret < 0)
    {
		IBD_PRINTF(DEBUG_TRACEFUNC,"[Audio Control Server]: audio operation error !!! \n");
    }
    
	AudioSpeedSetVolReplyC *pReply = new AudioSpeedSetVolReplyC();
	pReply->carSpeed = carSpeed;
	pReply->result = ret;
	reply = pReply;
}

int AudioCtrlServerC::registerNotiHandler(unsigned int id, AudioCtrlCBHandlerC* pCNotiHandle)
{
	InternalLock	lock(&m_lock);

	pNotifier->registerHandler(id, pCNotiHandle);
	
	return 0;
}

int AudioCtrlServerC::deRegisterNotiHandler(unsigned int id, AudioCtrlCBHandlerC* pCNotiHandle)
{
	InternalLock	lock(&m_lock);

	pNotifier->deRegisterHandler(id, pCNotiHandle);

	return 0;
}


// internal lock
AudioCtrlServerC::InternalLock::InternalLock(pthread_mutex_t* lock)
{
	m_lock=lock;
	pthread_mutex_lock(m_lock);
}
AudioCtrlServerC::InternalLock::~InternalLock()
{
	pthread_mutex_unlock(m_lock);
}

