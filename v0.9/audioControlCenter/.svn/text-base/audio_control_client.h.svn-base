/////////////////////////////////////////////////////////////////////////
// Header file for client for audio control
//
/////////////////////////////////////////////////////////////////////////

#ifndef AUDIO_CONTROL_CLIENT_H
#define AUDIO_CONTROL_CLIENT_H

#include <dbusMessage.h>

#include "dbusClientProxy.h"
#ifndef WITH_NEW_AUDIO_CONTROL_SYSTEM
#include "audio_control.h"
#include "audio_main_volume.h"
#else
typedef struct
{
        unsigned int    logicId;
        int     numid_vol;
        int     numid_switch;
        char    name[20];
}devIdStruct_t;

typedef struct
{
        devIdStruct_t   id;             // logic to control id  
        int             on_off;                 // 1: on; 0: off
        int             volume;                 // current volume in percentage
        //int           on_off_pre;             // previous switch, used for restoring device, 0 or 1
        //int           volume_pre;             // previous volume, used for restoring device, in percentage
        //int           priority_back;  // priority in percentage as background audio   
        //bool  interferred;    // state indicating audio interferred by other source. 0: normal; 1: interferred
         /*  
           * soundStatus:
           * bit7                       bit6                    bit5                    bit4                    bit3                    bit2                    bit1                    bit0
           * closeSR=0          close=0         resume=0                unmute=0                unchanged=0     ---                     ---                     ---
           * openSR=1           open=1          pause=1         mute=1          changed=1       always=1                mix=1           mono=1
           */
        unsigned char soundStatus;   
        unsigned char otherStatus;
} audioControlArray_t;

class AudioCtrlCBHandlerC
{
public:
        virtual void notifyHandle(int, int, unsigned char){}
};

class AudioCtrlCallbackC
{
};

#endif

class AudioCtrlClientC
{

public:
	AudioCtrlClientC(DBusClientProxyC *pClient);
	~AudioCtrlClientC();

	int openAudio(unsigned int id, int vol);
	int closeAudio(unsigned int id);
	int adjustAudioVol(unsigned int id, int vol);
	int muteAudio(unsigned int id);
	int unmuteAudio(unsigned int id);
	int saveAudioInfo();
	int openSpeechRecognition();
	int closeSpeechRecognition();		
	int setDefaultParameters();

	int openWarning();
	int openWarning_normal();
	int recoveryMute();
/*set up*/
	int setupAudioChannel(unsigned int id, int ctl);
	int getAudioInfo(unsigned int id, int *vol, int* sw_on_off, unsigned char *sStatus);
	int getAudioInfoExt(unsigned int id, int *vol, int* sw_on_off, unsigned char *sStatus, unsigned char *oStatus);	
/*get all info*/
	int getSetupAudioInfo(audioControlArray_t *info);
	
	int registerNotiHandler(unsigned int id, AudioCtrlCBHandlerC* pCNotiHandle);
	int deRegisterNotiHandler(unsigned int id, AudioCtrlCBHandlerC* pCNotiHandle);

	static AudioCtrlClientC* getAudioCtrlClientInstance();
	void openAudioDevInit();		// at initialization, open the audio devices as default

	int setAudioVolAccToSpeed(int carSpeed);
	int openSetAudioVolAccToSpeed();
	int closeSetAudioVolAccToSpeed();
#ifdef IBAC_VOLUME_MAIN_OFFSET
	int setMainVolOffset(int offset);
#endif	
	// Main_Volume *mvol;

private:
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        int setChannelIDMute(bool flag, int channelID);
#endif
	int audioSetupChange(unsigned int id, int vol, int sw_on_off, int op_code);
	int audioChange(unsigned int id, int vol, int sw_on_off, int op_code);	// change the volume, 1 - open; 2 - close; 3 - change volume or switch
	static void audioNotifyHandler(DBusNotifyC *notify);
	static AudioCtrlClientC *pThisAudioCtrlClient;

private:
	DBusClientProxyC *client;
	AudioCtrlCallbackC *pCCallback;	

	// audio info
	unsigned int devId;		// logic id of audio device
	int volume; // volume in percentage, 0 - min; 100 - max 100%
	int on_off; //	 switch of the audio device, 0 - off; 1 - on	
	unsigned char soundStatus;
	unsigned char otherStatus;
	//audioControlArray_t info[IBAC_DEV_ID_MAX];

	bool mute_flag;
	int s_vol; 

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
