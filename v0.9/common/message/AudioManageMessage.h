#ifndef __AUDIO__MANAGER_MESSAGE__
#define __AUDIO__MANAGER_MESSAGE__
#include "dbusMessage.h"

#define ServerAudioManager 		(char*)"com.innov.audiomanager"		//格式com.innov.<进程名>
#define ObjectAudioManager 		(char*)"/audiomanager"			//格式 /<模块名>

#ifdef _8925_VERSION

#define IBAC_DEV_ID_SPEAKER_PLAYBACK_VOLUME     (1)	//UNUSED
#define IBAC_DEV_ID_SPEAKER_PLAYBACK_SWITCH     (2)	//UNUSED
#define IBAC_DEV_ID_SPEAKER_AMP                 (3)	//UNUSED
#define IBAC_DEV_ID_MAIN_VOLUME                 (4)
#define IBAC_DEV_ID_MAIN_SWITCH                 (5)
#define IBAC_DEV_ID_AXO_PLAYBACK_VOLUME         (6)	//UNUSED
#define IBAC_DEV_ID_AXO_PLAYBACK_SWITCH         (7)	//UNUSED
#define IBAC_DEV_ID_DAI                         (8)
#define IBAC_DEV_ID_RADIO_VOLUME                (9)
#define IBAC_DEV_ID_MICIN1                      (10)
#define IBAC_DEV_ID_MICIN1_BOOST                (11)
#define IBAC_DEV_ID_3G_SPK_VOLUME               (12)
#define IBAC_DEV_ID_MICIN2                      (13)	//UNUSED
#define IBAC_DEV_ID_AUX_VOLUME                  (14)	//UNUSED
#define IBAC_DEV_ID_CAPTURE_VOLUME              (15)	//UNUSED
#define IBAC_DEV_ID_JFUNCTION                   (16)	//UNUSED
#define IBAC_DEV_ID_SFUNCTION                   (17)	//UNUSED
#define IBAC_DEV_ID_AXO_VOL_IN_MUX              (18)	//UNUSED
#define IBAC_DEV_ID_HPR_VOL_IN_MUX              (19)	//UNUSED
#define IBAC_DEV_ID_HPL_VOL_IN_MUX              (20)	//UNUSED
#define IBAC_DEV_ID_SPK_VOL_IN_MUX              (21)	//UNUSED
#define IBAC_DEV_ID_SPK_RADIO_PLAYBACK_SWITCH   (22)	//UNUSED
#define IBAC_DEV_ID_SPK_AUXIN_PLAYBACK_SWITCH   (23)	//UNUSED
#define IBAC_DEV_ID_SPK_MIC1_PLAYBACK_SWITCH    (24)	//UNUSED
#define IBAC_DEV_ID_SPK_MIC2_PLAYBACK_SWITCH    (25)	//UNUSED
#define IBAC_DEV_ID_SPK_DAI_PLAYBACK_SWITCH     (26)	//UNUSED
#define IBAC_DEV_ID_RSTOHPMIXERRECORD_PB_SWIT   (27)	//UNUSED
#define IBAC_DEV_ID_LSTOHPMIXERRECORD_PB_SWIT   (28)	//UNUSED
#define IBAC_DEV_ID_RADIO_SWITCH                (29)
#define IBAC_DEV_ID_HP_MIXER_AUXIN_PB_SWIT      (30)
#define IBAC_DEV_ID_HP_MIXER_MIC1_PB_SWIT       (31)
#define IBAC_DEV_ID_3G_SWITCH                   (32)
#define IBAC_DEV_ID_HP_MIXER_PCM_PB_SWIT        (33)
#define IBAC_DEV_ID_MIXER_LINEIN_PB_SWIT        (34)	//UNUSED
#define IBAC_DEV_ID_MIXER_MIC1_PB_SWIT          (35)	//UNUSED
#define IBAC_DEV_ID_MIXER_MIC2_PB_SWIT          (36)	//UNUSED
#define IBAC_DEV_ID_MIXER_AUXIN_PB_SWIT         (37)	//UNUSED
#define IBAC_DEV_ID_MIXER_PCM_PB_SWIT           (38)	//UNUSED
#define IBAC_DEV_ID_MIXER_RECORDL_PB_SWIT       (39)	//UNUSED
#define IBAC_DEV_ID_MIXER_RECORDR_PB_SWIT       (40)	//UNUSED
#define IBAC_DEV_ID_R_REC_M_MIC1_CAP_SWIT       (41)
#define IBAC_DEV_ID_R_REC_M_MIC2_CAP_SWIT       (42)
#define IBAC_DEV_ID_R_REC_M_LINEINR_CAP_SWIT    (43)
#define IBAC_DEV_ID_R_REC_M_AUXIN_CAP_SWIT      (44)	//UNUSED
#define IBAC_DEV_ID_R_REC_M_HPMR_CAP_SWIT       (45)	//UNUSED
#define IBAC_DEV_ID_R_REC_M_SPKM_CAP_SWIT       (46)	//UNUSED
#define IBAC_DEV_ID_R_REC_M_MM_CAP_SWIT         (47)	//UNUSED
#define IBAC_DEV_ID_L_REC_M_MIC1_CAP_SWIT       (48)
#define IBAC_DEV_ID_L_REC_M_MIC2_CAP_SWIT       (49)
#define IBAC_DEV_ID_L_REC_M_LINEINR_CAP_SWIT    (50)
#define IBAC_DEV_ID_L_REC_M_AUXIN_CAP_SWIT      (51)	//UNUSED
#define IBAC_DEV_ID_L_REC_M_HPML_CAP_SWIT       (52)	//UNUSED
#define IBAC_DEV_ID_L_REC_M_SPKM_CAP_SWIT       (53)	//UNUSED
#define IBAC_DEV_ID_L_REC_M_MM_CAP_SWIT         (54)	//UNUSED
#define IBAC_DEV_ID_DAI_MP3                     (55)
#define IBAC_DEV_ID_DAI_VIDEO                   (56)
#define IBAC_DEV_ID_DAI_GPS                     (57) 
#define IBAC_DEV_ID_DAI_NEWS                    (58)
#define IBAC_DEV_ID_DAI_SMS                     (59)
#define IBAC_DEV_ID_DAI_RING                    (60)
#define IBAC_DEV_ID_DAI_SMSRING                 (61)
#define IBAC_DEV_ID_DAI_WARNING                 (62)

#define IBAC_DEV_ID_MAX                         (IBAC_DEV_ID_DAI_WARNING)
#else

#define    IBAC_DEV_ID_MAIN_VOLUME                   1
#define    IBAC_DEV_ID_DAI                                     2
#define    IBAC_DEV_ID_DPB_VOLUME                     3
#define    IBAC_DEV_ID_DPB_SWITCH                     4
#define    IBAC_DEV_ID_DEEP_SWITCH                   5
#define    IBAC_DEV_ID_MICIN                                6
#define    IBAC_DEV_ID_MICIN_SWITCH                 7
#define    IBAC_DEV_ID_CZC_SWITCH                     8
#define    IBAC_DEV_ID_CHPF_SWITCH                   9
#define    IBAC_DEV_ID_COUTDAC_SWITCH           10
#define    IBAC_DEV_ID_COUTAUX_SWITCH            11
#define    IBAC_DEV_ID_COUTBPS_SWITCH            12
#define    IBAC_DEV_ID_RADIO_SWITCH                 13
#define    IBAC_DEV_ID_TV_SWITCH                       14
#define    IBAC_DEV_ID_3G_SWITCH                       15
#define    IBAC_DEV_ID_VOICE_SWITCH                 16 
#define    IBAC_DEV_ID_BOLUETOOTH_SWITCH      17
#define    IBAC_DEV_ID_JFUNCTION_SWITCH         18
#define    IBAC_DEV_ID_SFUNCTION_SWITCH         19
#define    IBAC_DEV_ID_DAI_MP3                            20
#define    IBAC_DEV_ID_DAI_VIDEO                        21
#define    IBAC_DEV_ID_DAI_GPS                            22
#define    IBAC_DEV_ID_DAI_NEWS                         23
#define    IBAC_DEV_ID_DAI_SMS                           24
//#define    IBAC_DEV_ID_DAI_SMSRING                   25
//#define    IBAC_DEV_ID_DAI_RING                          26
#define    IBAC_DEV_ID_DAI_RING                          25
#define    IBAC_DEV_ID_DAI_SMSRING                   26
#define    IBAC_DEV_ID_DAI_WARNING                  27

#define IBAC_DEV_ID_MAX                 IBAC_DEV_ID_DAI_WARNING

#endif

#define	REQ_ID_AUDIO	7000
#define	REP_ID_AUDIO	7001
#define	NOTF_ID_AUDIO	7002
#define	REQ_ID_AUDIOMODE	7003
#define REQ_ID_MANAGER_CHANNEL_MUTE     (7004)
#define REP_ID_MANAGER_CHANNEL_MUTE     (7005)
#define REQ_ID_AUDIO_RESTORE_DEFAULT    (7006)
#define REP_ID_AUDIO_RESTORE_DEFAULT    (7007)
#define REQ_ID_AUDIO_MANAGER_SYNC_AUDIO (7008)

#define SET_AUDIO       (1)
#define GET_AUDIO       (0)
#define MAX_VOLUME      (30)
#define MIN_VOLUME      (1)

#define DEFAULT_MAIN_VOLUME     (25)


class AudioManagerSyncAudioReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(AudioManagerSyncAudioReqC)
public:
	AudioManagerSyncAudioReqC():DBusRequestC(REQ_ID_AUDIO_MANAGER_SYNC_AUDIO){}
};

class AudioManagerRestoreDefaultReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(AudioManagerRestoreDefaultReqC)
public:
	AudioManagerRestoreDefaultReqC():DBusRequestC(REQ_ID_AUDIO_RESTORE_DEFAULT){}
};

class AudioManagerRestoreDefaultRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(AudioManagerRestoreDefaultRepC)
public:
	AudioManagerRestoreDefaultRepC():DBusReplyC(REP_ID_AUDIO_RESTORE_DEFAULT){mute_state = false,volume = 0,result = 1;}

	int volume;
	int result;//0-success,-1-fail
	bool mute_state;//true-mute false-unmute
};

class AudioManagerChannelMuteStateReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(AudioManagerChannelMuteStateReqC)
public:
	AudioManagerChannelMuteStateReqC():DBusRequestC(REQ_ID_MANAGER_CHANNEL_MUTE){channelID = -1;mute_state = false;}

	int channelID;
	bool mute_state;//true-mute false-unmute
};

class AudioManagerChannelMuteStateRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(AudioManagerChannelMuteStateRepC)
public:
	AudioManagerChannelMuteStateRepC():DBusReplyC(REP_ID_MANAGER_CHANNEL_MUTE){channelID = -1,result = 1;}

	int channelID;
	int result;//0-success,-1-fail
};



class AudioManagerReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(AudioManagerReqC)
public:
	AudioManagerReqC():DBusRequestC(REQ_ID_AUDIO){}
	void print(FILE *fp){fprintf(fp,"\tctlstate = %d (0--get(reply) 1--set(notify))\r\nvolume = %d (0~30  0=mute)\r\nmute_state = %d (0-mute 1-unmute)\r\n",ctl_state,volume,mute_state);}

	int ctl_state;//0--get(reply) 1--set(notify)
	int volume;//0~30  0=mute
	int mute_state;//0-mute 1-unmute
};

class AudioManagerRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(AudioManagerRepC)
public:
	AudioManagerRepC():DBusReplyC(REP_ID_AUDIO){}
	void print(FILE *fp){fprintf(fp,"\tvolume = %d mute_state = %d\r\n",volume,mute_state);}

	int volume;//0~30  0=mute
	int mute_state;//0-mute 1-unmute
};

class AudioManagerNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(AudioManagerNotifyC)
public:
	AudioManagerNotifyC(): DBusNotifyC(NOTF_ID_AUDIO){}
    
	void print(FILE *fp){fprintf(fp,"\tvolume = %d mute_state = %d\r\n",volume,mute_state);}  

	int volume;//0~30  0=mute
	int mute_state;//0-mute 1-unmute    
};


class AudioManagerModeReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(AudioManagerModeReqC)
public:
	AudioManagerModeReqC():DBusRequestC(REQ_ID_AUDIOMODE){}
	void print(FILE *fp){fprintf(fp,"\t mode = %d (0--init  1--radio  2--phone  3--video  4--news)\r\n",mode);}

	int mode;//0--init  1--radio  2--phone  3--video  4--news
};
#endif
