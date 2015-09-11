/**************************************************************************
** Copyright (C) 2010 Niu FuYong http://
** All rights reserved.
**
** If you have questions regarding the use of this file, please contact
** at question@
***************************************************************************
** Author	: Niu FuYong , nfyuse@gmail.com
** File		: 
** Date(Y/M/D)	: 2013-01-20
** Description	:  
**
***************************************************************************
** History	: 
** 2011-02-10  NiuFuYong  fyuse@gmail.com  first version 
**
**************************************************************************/
#ifndef MAIN_VOLUME_H_
#define MAIN_VOLUME_H_
#include <pthread.h>
#include "AudioManageMessage.h"
#include <QTimer>

#define IBAC_AUDIO_INFO_FILE ((char*)"/nand3/applicationdata/audio.cfg")
#define IBAC_AUDIO_DEF_INFO_FILE ((char*)"../data/audiodefault.cfg")
#define MAIN_VOLUME_FILE ((char*)"/nand3/applicationdata/main_volume.cfg")
#define MUTE_VOLUME_FILE ((char*)"/nand3/applicationdata/mute_volume.cfg")
#define AUDIO_MANAGER_FILE ((char*)"/nand3/applicationdata/audiomanager.cfg")


#define    ALSA_PLUG_NAME_MAX                             8
	
#define	IBAC_DEV_ID_MIN			IBAC_DEV_ID_MAIN_VOLUME

#define IBAC_AUDIO_OP_RESULT_SUCCESS	0
#define IBAC_AUDIO_OP_RESULT_WRONG_DEVID	-1
#define IBAC_AUDIO_OP_RESULT_BLOCKED	-2
#define IBAC_AUDIO_OP_RESULT_SAVE_FAIL	-3
#define IBAC_AUDIO_OP_RESULT_LOAD_FAIL	-4

#define IBAC_VOLUME_MAIN_MIN            69  //50  80
#define IBAC_VOLUME_MAIN_MAX           127  //140 //127  //112
#define IBAC_VOLUME_MIN_SCALE           (MIN_VOLUME)
#define IBAC_VOLUME_FULL_SCALE          (MAX_VOLUME)
#define IBAC_INIT_VOLUME_NULL		       0
#define IBAC_INIT_VOLUME_MAX		       255
#define IBAC_INIT_VOLUME_MIC		       221

#define IBAC_AUDIO_SWITCH_ON          1
#define IBAC_AUDIO_SWITCH_OFF         0
#define IBAC_AUDIO_SWITCH		       1
#define IBAC_AUDIO_VOLUME		       0

#define IBAC_MUTE_GF_TIME	(1000)

typedef enum{
        INIT_STATE,
        RADIO_STATE,
        PHONE_STATE,
        VIDEO_STATE,
        NEWS_STATE,
}VOICE_MODE;

typedef struct
{
	unsigned int	logicId;
	int 	numid_vol;
	int 	numid_switch;		
#ifdef _8925_VERSION
	char	name[60];
#else
	char	name[30];
#endif
}devIdStruct_t; 

typedef struct
{
	devIdStruct_t	id;		// logic to control id	
	int		on_off;			// 1: on; 0: off
	int		volume;			// current volume in percentage
	int		status;
} audioControlArray_t;

class Main_Volume
{
public:
        Main_Volume();
        ~Main_Volume();
        int checkInit();
        int initAlsaPlug();
        void getCurrentAlsaData();
        void initAudio();
        //int initAudioConf();
        void resetAudioArr();

        void loadConf();
        int saveConf(int volume,bool mute);
        
        int muteGF();
        int unmuteGF();
        int channelSwitchOn(audioControlArray_t *aCArrOn);
        int channelSwitchOff(audioControlArray_t *aCArrOff);
        int changeHardVolume(audioControlArray_t *audioItem,int newVolume);
        int convertVol(int vol, unsigned int id);

        static Main_Volume *thisObject;
        static Main_Volume *object();

        int mainvolScaleToFact(int vol);//from 1~30 to 69~127
        int mainvolFactToScale(int vol);//from 69~127 to 1~30
        int mediavolScaleToFact(int vol);//from 0~30 to 0~255
        int mediavolFactToScale(int vol);//from 0~255 to 0~30
        long get_integer(long x, long min, long max);
        int csetAudio(int numid, char *name, int volume);
        int cgetAudio(int numid, char *name,int type);

        int set_main_volume(int volume);
        int get_main_volume();

        int set_mute();
        int set_unmute();
        int get_mute_status();

        VOICE_MODE get_mode();
        void set_mode(VOICE_MODE mode);
        int muteOneChannel(int channelID);
        int unmuteOneChannel(int channelID);
        int restoreDefault(DBusRequestC *request, DBusReplyC *&reply);
private:
        int turnSoundOffChannel(int channelID);
        int turnSoundOnChannel(int channelID);
        int getOneChannelMuteFlag(int channelID);

private:
        static devIdStruct_t devLogicId2CidSet[IBAC_DEV_ID_MAX];
        audioControlArray_t audioControlArr[IBAC_DEV_ID_MAX];
        bool mutestate;//ture--unmute  false--mute
        int volumenum;
        VOICE_MODE modestate;
        VOICE_MODE modestatebak;
        bool channelMuteState[IBAC_DEV_ID_MAX];
        int channelVolumeValStored[IBAC_DEV_ID_MAX];

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
