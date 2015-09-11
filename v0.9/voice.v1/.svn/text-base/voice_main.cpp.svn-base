#include <linux/input.h>
#include <QtGui/QApplication>
#include <pthread.h>
#include <errno.h>
#include "voice_setting.h"
#include "iv_TTS.h"
#include "aiTalk.h"
#include "dbusServer.h"
#include "dbusClient.h"
#include "dbusMessage.h"
#include "dbusApplication.h"

#include "audio_control_client.h"
#include "audio_control_server.h"
#include "audio_control.h"
#include "debugMonitor.h"

DBusServerC* pServer = NULL;
DBusClientC* pClient = NULL;

#define AUDIO_CONTROL_CENTRAL


#ifdef AUDIO_CONTROL_CENTRAL
//AudioControlC *pAudioCtl = NULL;
AudioCtrlServerC *audioServer = NULL;
#endif

int ControlSocket = -1;

/*下面指针指向本进程主要的3个对象*/
#if 1
IvTTS* gs_pIvTTS = NULL;									/*指向语音合成对象的指针*/
#endif
AiTalk* gs_pAiTalk = NULL;									/*指向语音识别对象的指针*/


static int AitalkFlag = 0;

static pthread_mutex_t AitalkLock;
static pthread_cond_t AitalkEvent;

AudioCtrlClientC *g_pAudioClientCtl = NULL;
DBusClientProxyC *pAudioDBusClient = NULL;

void DestroyObject()
{
	BEGINFUNC_USING_BY_VOICE

	/*销毁语音识别线程对象*/
	if( NULL != gs_pAiTalk )
	{
		delete gs_pAiTalk;
		gs_pAiTalk = NULL;
	}

	/*销毁语音合成线程对象*/
	if( NULL != gs_pIvTTS )
	{
		delete gs_pIvTTS;
		gs_pIvTTS = NULL;
	}
	ENDFUNC_USING_BY_VOICE
}

bool InitObject()
{
	BEGINFUNC_USING_BY_VOICE

	DestroyObject();

	gs_pAiTalk = new AiTalk();
	if(NULL == gs_pAiTalk || 0 == gs_pAiTalk)
	{
		dbug_out("create AiTalk object fails!");
		goto fail_init;
	}
	
	gs_pIvTTS = new IvTTS();

	if(NULL == gs_pIvTTS || 0 == gs_pIvTTS)
	{
		dbug_out("create IvTTS object fails!");
		goto fail_init;
	}

	if(!gs_pAiTalk->RegisterInterface((IvTTSInterface*)gs_pIvTTS))
	{
		dbug_out("RegisterInterface fail......");
		goto fail_init;
	}

	gs_pIvTTS->start();

	ENDFUNC_USING_BY_VOICE
	return true;

fail_init:
	DestroyObject();
	ENDFUNC_USING_BY_VOICE
	return false;
}


static void* AitalkThread(void *arg)
{
	arg = NULL;
	AiTalkInterfaceForMainthread* pAiTalkInterfaceForMainthread = (AiTalkInterfaceForMainthread*)gs_pAiTalk;
	int bytes, flush;
	int fd;

	struct input_event ev;
	
	fd = open("/dev/input/event1", O_RDONLY);	//阻塞方式打开
	if(fd < 0){
		 dbug_out("Open failed with err %d: %s\n", fd, strerror(errno));
		 pthread_exit(NULL);
	}

	while(1){
		bytes = read(fd, &ev, sizeof(ev));
		if(bytes != sizeof(ev))
			continue;

		if (!(ev.type == EV_KEY && ev.value == 1 && ev.code == 6 && AitalkFlag == 0)){
			dbug_out("read 1 Key Press Code = [%d]", ev.code);
		}else{
			while(1){
				bytes = read(fd, &ev, sizeof(ev));

				if(ev.type == EV_KEY && ev.value == 1 && ev.code == 6){
					dbug_out("read 2 Key Press Code = [%d]", ev.code);
					continue;
				}
				pthread_mutex_lock(&AitalkLock);
				if(AitalkFlag != 0){
					dbug_out("Has entered Aitalk status");
					pthread_mutex_unlock(&AitalkLock);
					break;
				}else{				
					printf("Event1..... Key release Code = [%d]\n", ev.code);
					AitalkFlag = 1;
					dbug_out("\n\n\n\n\n*******************Will openSpeechRecognition**************\n\n\n\n");


#if 1
					audioServer->openSpeechRecognition();
#else
					if(g_pAudioClientCtl == NULL){
						dbug_out("g_pAudioClientCtl is NULL");
						break;
					}
					int resault = 0;
					resault = g_pAudioClientCtl->openSpeechRecognition();
					if(resault != 0)
					{
						dbug_out("openSpeechRecognition error");			
					}
#endif
					dbug_out("\n\n\n\n\n*******************Will gs_pAiTalk->audioServerBusy**************\n\n\n\n");

					while(gs_pAiTalk->audioServerBusy){
						dbug_out("Start Aitalk wait audioServerBusy");
						usleep(10000);
					}
					gs_pAiTalk->m_mutexAiTalkAudioServerBusy.lock();
					gs_pAiTalk->audioServerBusy = true;
					gs_pAiTalk->m_mutexAiTalkAudioServerBusy.unlock();
					
					dbug_out("\n\n\n\n\n*******************Will audioServer->openAudio**************\n\n\n\n");
#if 1
					audioServer->openAudio(IBAC_DEV_ID_DAI_COMMAND, -1);
#else
					if(g_pAudioClientCtl == NULL){
						dbug_out("g_pAudioClientCtl is NULL");
						break;
					}

					resault = g_pAudioClientCtl->openAudio(IBAC_DEV_ID_DAI_COMMAND, -1);
					if(resault != 0)
					{
						dbug_out("openAudio cmd error");			
					}
#endif
					if(NULL != gs_pIvTTS)
					{
						gs_pIvTTS->PlayString(User_AiTalk, "打开语音识别");
					}
					sleep(1);
#if 1
					audioServer->openAudio(IBAC_DEV_ID_MIC, -1);
#else
					if(g_pAudioClientCtl == NULL){
						dbug_out("g_pAudioClientCtl is NULL");
						break;
					}

					resault = g_pAudioClientCtl->openAudio(IBAC_DEV_ID_MIC, -1);
					if(resault != 0)
					{
						dbug_out("openAudio IBAC_DEV_ID_MIC error");			
					}
#endif
					dbug_out("\n\n\n\n\n*******************audioServer->openAudio**************\n\n\n\n");
					
					gs_pAiTalk->m_mutexAiTalkAudioServerBusy.lock();
					gs_pAiTalk->audioServerBusy = false;
					gs_pAiTalk->m_mutexAiTalkAudioServerBusy.unlock();
					dbug_out("\n\n\n\n\n*******************StartIdentify**************\n\n\n\n");
					
					pAiTalkInterfaceForMainthread->StartIdentify();
					
					dbug_out("\n\n\n\n\n******************* end StartIdentify  will wakeup*************\n\n\n\n");
					pthread_cond_signal(&AitalkEvent);
					pthread_mutex_unlock(&AitalkLock);
					break;
				}
			}
		}
	}
	
	flush = close(fd);
	if(flush < 0) 
	{
		dbug_out("close fd event1 error");
	}
}

static void* mainThread(void *arg)
{
	arg = NULL;	
	int i = 0;

beginInit:
	bool ret = InitObject();

	if(!ret)
	{
		dbug_out("InitObject Error");
		DestroyObject();
		if(i < 3)
		{
			sleep(5);
			i++;
			goto beginInit;
		}
		else
		{
			dbug_out("InitObject Error exit Init Aitalk");
		}
	}
	
	pthread_t threadId;
	ret = pthread_create(&threadId, NULL, AitalkThread, NULL);//创建一个线程，用于跟用户交互，以便发送请求消息
	if(ret < 0)
	{
		dbug_out("pthread_create AitalkThread error");
		pthread_exit(NULL);
	}
	while(1)
	{
		pthread_cond_wait(&AitalkEvent, &AitalkLock);
		
		dbug_out("\n\n\n\n\n*******************Stop Aitalk**************\n\n\n\n");
		if(AitalkFlag != 0){
			sleep(8);
			AiTalkInterfaceForMainthread* pAiTalkInterfaceForMainthread = (AiTalkInterfaceForMainthread*)gs_pAiTalk;

			while(gs_pAiTalk->audioServerBusy){
				dbug_out("Stop Aitalk wait audioServerBusy");
				usleep(10000);
			}
			gs_pAiTalk->m_mutexAiTalkAudioServerBusy.lock();
			gs_pAiTalk->audioServerBusy = true;
			gs_pAiTalk->m_mutexAiTalkAudioServerBusy.unlock();

#if 1
			audioServer->closeAudio(IBAC_DEV_ID_MIC);
#else
			int resault = 0;
			if(g_pAudioClientCtl == NULL){
				dbug_out("g_pAudioClientCtl is NULL");
				continue;
			}

			
			resault = g_pAudioClientCtl->closeAudio(IBAC_DEV_ID_MIC);
			if(resault != 0)
			{
				dbug_out("Close mic error");			
			}
#endif
			pAiTalkInterfaceForMainthread->StopIdentify();
			if( NULL != gs_pIvTTS )
			{
				gs_pIvTTS->PlayString(User_AiTalk, "关闭语音识别");
			}
			sleep(1); //to play stop aitalk voice#if 1

#if 1
			audioServer->closeAudio(IBAC_DEV_ID_DAI_COMMAND);
#else
			if(g_pAudioClientCtl == NULL){
				dbug_out("g_pAudioClientCtl is NULL");
				continue;
			}


			resault = g_pAudioClientCtl->closeAudio(IBAC_DEV_ID_DAI_COMMAND);
			if(resault != 0)
			{
				dbug_out("Close cmd error");			
			}
#endif

			gs_pAiTalk->m_mutexAiTalkAudioServerBusy.lock();
			gs_pAiTalk->audioServerBusy = false;
			gs_pAiTalk->m_mutexAiTalkAudioServerBusy.unlock();
#if 1
			audioServer->closeSpeechRecognition();
#else
			if(g_pAudioClientCtl == NULL){
				dbug_out("g_pAudioClientCtl is NULL");
				continue;
			}

			resault = g_pAudioClientCtl->closeSpeechRecognition();
			if(resault != 0)
			{
				dbug_out("Close cmd error");			
			}
#endif
		}
		AitalkFlag = 0;
	}
}
/*
class PhoneAudioHandleC:public AudioCtrlCBHandlerC
{
public:
    void notifyHandle(int vol, int resume_pause, unsigned char sStat);

};

void PhoneAudioHandleC::notifyHandle(int vol, int resume_pause, unsigned char sStat)
{
    printf("\nPhone callback for audio control notify hanldling: vol = %d, play_flag = %s, status = 0x%2x\n", \
        vol, resume_pause == 0 ? "PAUSE" : "RESUME", sStat);

}
*/



class MessageAudioHandleC : public AudioCtrlCBHandlerC
{
public:
	void notifyHandle(int vol, int resume_pause, unsigned char sStat);
};

void MessageAudioHandleC::notifyHandle(int vol, int resume_pause, unsigned char sStat)
{
	printf("Message callback for audio control notify hanldling: vol = %d, play_flag = %s, sStat = 0x%2x\n", \
		vol, resume_pause == 0 ? "PAUSE" : "RESUME", sStat);
}

int  InitDBusAudiolient()
{	
	MessageAudioHandleC messageHandle;
	
	pAudioDBusClient = new DBusClientProxyC(IBAC_AUDIO_SERVICE_NAME, IBAC_AUDIO_SERVER_NAME);

	g_pAudioClientCtl = new AudioCtrlClientC(pAudioDBusClient);

	if(pAudioDBusClient->connect()<0)
	{
	        printf("Cann't connect to DBUS server.\n");
	        return -1;
	}

	return 0;
}



int main(int argc, char *argv[])
{
	BEGINFUNC_USING_BY_VOICE
	int ret = 0;

	DBusApplicationC app(argc, argv);
	

//	DBusServerC server((char*)"com.innov.VoiceModule",(char*)"/VoiceModule");
	DBusServerC server(IBAC_AUDIO_SERVICE_NAME, IBAC_AUDIO_SERVER_NAME);
	pServer = &server;
	if(pServer == NULL || pServer == 0)
		dbug_out("pServer is NULL %x", pServer);
	
	IBD_INIT("voice",pServer);

	//server.registerService(REQ_ID_VOICE, VoiceRequestServiceHandler, VoiceRequestC::factory);

#ifdef AUDIO_CONTROL_CENTRAL
	// create DBus server for audio control
//	DBusServerC dbusServerAudio(IBAC_AUDIO_SERVICE_NAME, IBAC_AUDIO_SERVER_NAME);	
	// create the audio control server
	audioServer = new AudioCtrlServerC(&server);	
	if(audioServer == NULL || audioServer == 0)
		dbug_out("audioServer is NULL %x\n", audioServer);
	
	//PhoneAudioHandleC phoneHandle;
	//audioServer->registerNotiHandler(IBAC_DEV_ID_3G_PHONE_SPEAKER, &phoneHandle);
	// DBus server connect
	audioServer->initServer();
#endif

	if(server.connect()<0)
	{
		dbug_out("Cann't connect to DBUS server.");
	}

#if 0
	ret = InitDBusAudiolient();
	if(ret < 0){
		g_pAudioClientCtl = NULL;
		dbug_out("InitDBusAudiolient error");
	}
#endif
#if 0
	pClient  = new DBusClientC((char*)"com.innov.PhoneModule", (char*)"/PhoneModule");
	if(pClient == NULL || pClient == 0)
		dbug_out("pClient is NULL %x", pClient);
	
//	pClient->registerReply(REP_ID_PHONED,PhonedReplyC::factory);
	pClient->registerNotify(NOTF_ID_PHONED, PhoneNotifyHandler, PhonedNotifyC::factory);
	if(pClient->connect() < 0)
	{		
		dbug_out("Cann't connect to DBUS server.");
	}
#endif

	pthread_t threadId;
	ret = pthread_create(&threadId, NULL, mainThread, NULL);//创建一个线程，用于跟用户交互，以便发送请求消息
	if(ret < 0)
	{
		dbug_out("Create pthread mainThread error");
	}

	ENDFUNC_USING_BY_VOICE
	return app.loop();
}

