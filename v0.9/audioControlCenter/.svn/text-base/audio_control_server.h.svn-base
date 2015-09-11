/////////////////////////////////////////////////////////////////////////
// Header file for client for audio control
//
/////////////////////////////////////////////////////////////////////////

#ifndef AUDIO_CONTROL_SERVER_H
#define AUDIO_CONTROL_SERVER_H

#include <dbusMessage.h>
#include <dbusServer.h>

#include "audio_control.h"

class AudioChangeNotifier:
	public AudioChangeListenerC,
	public AudioCtrlCallbackC	
{
public:
	AudioChangeNotifier(DBusServerC *serverPtr);
	void audioStatusUpdate(unsigned int devId, int volume, int on_off, unsigned char soundStatus);

private:
	DBusServerC *server;

};
   
class AudioCtrlServerC
{

public:
	AudioCtrlServerC(DBusServerC *pServer);
	~AudioCtrlServerC();
	
	int initServer();

	int openAudio(unsigned int id, int vol);
	int closeAudio(unsigned int id);
	int adjustAudioVol(unsigned int id, int vol);
	int muteAudio(unsigned int id);
	int unmuteAudio(unsigned int id);
	int getAudioInfo(unsigned int id, int *vol, int *sw_on_off, unsigned char *sStatus);
	int getAudioInfoExt(unsigned int id, int *vol, int* sw_on_off, unsigned char *sStatus, unsigned char *oStatus);
	int saveAudioInfo();
	int openSpeechRecognition();
	int closeSpeechRecognition();
	int openSetAudioVolAccToSpeed();
	int closeSetAudioVolAccToSpeed();
#ifdef IBAC_VOLUME_MAIN_OFFSET
	int setMainVolOffset(int offset);
#endif
public:
	int registerNotiHandler(unsigned int id, AudioCtrlCBHandlerC* pCNotiHandle);
	int deRegisterNotiHandler(unsigned int id, AudioCtrlCBHandlerC* pCNotiHandle);

private:
	static void AudioServiceHandler(DBusRequestC *request, DBusReplyC *&reply);
	static void AudioServiceGetHandler(DBusRequestC *request, DBusReplyC *&reply);
	static void AudioServiceSetHandler(DBusRequestC *request, DBusReplyC *&reply);
	static void AudioServiceSpeedSetVolHandler(DBusRequestC *request, DBusReplyC *&reply);	
	
	static AudioCtrlServerC *pThisAudioServer;
	DBusServerC *server;
	AudioChangeNotifier *pNotifier;
	AudioControlC *audioCtrl;

private:
	pthread_mutex_t         m_lock;

	class	InternalLock
	{
	public:
		InternalLock(pthread_mutex_t* lock);
		~InternalLock();
	private:
		pthread_mutex_t* m_lock;
	};
};


#endif
