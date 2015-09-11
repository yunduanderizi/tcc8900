#ifndef AUDIO_CONTROL_H
#define AUDIO_CONTROL_H

#include <pthread.h>
#include <time.h>
#define IBAC_DEV_ID_MAIN_VOLUME		1
#define	IBAC_DEV_ID_DAI			2
#define	IBAC_DEV_ID_RADIO		3
#define IBAC_DEV_ID_TV			4
#define	IBAC_DEV_ID_3G_PHONE_SPEAKER	5
#define	IBAC_DEV_ID_MIC			6
#define	IBAC_DEV_ID_BOLUETOOTH		7
#define	IBAC_DEV_ID_CD_DVD		8
#define IBAC_DEV_ID_WALKI_TALKI		9
#define IBAC_DEV_ID_BYPASS_MX		10
#define	IBAC_DEV_ID_DAI_MP3		11
#define	IBAC_DEV_ID_DAI_VIDEO		12
#define	IBAC_DEV_ID_DAI_NETR		13
#define	IBAC_DEV_ID_DAI_GPS		14
#define	IBAC_DEV_ID_DAI_NEWS		15
#define	IBAC_DEV_ID_DAI_SMS		16
#define IBAC_DEV_ID_DAI_WARNING        IBAC_DEV_ID_DAI_SMS 
#define	IBAC_DEV_ID_DAI_CARINFO		17
#define	IBAC_DEV_ID_DAI_LIFEINFO	18
#define	IBAC_DEV_ID_DAI_COMMAND		19
#define	IBAC_DEV_ID_DAI_ANNOUNCE		20
#define	IBAC_DEV_ID_DAI_RING		21
#define	IBAC_DEV_ID_DAI_VPHONE		22
#define	IBAC_DEV_ID_DAI_KEYTONE		23
#define IBAC_DEV_ID_DAI_ILLEGALTIPS 24
#define IBAC_DEV_ID_DAI_SMSRING     25
#define IBAC_DEV_ID_DAI_TONE        26
#define IBAC_DEV_ID_DAI_VTALKI      27
#define IBAC_DEV_ID_DAI_MONLINE     28
#define IBAC_DEV_ID_DAI_VONLINE     29
#define IBAC_DEV_ID_DAI_LBOOK       30
#define IBAC_DEV_ID_DAI_MTRAFFIC    31
#ifdef IBAC_VOLUME_MAIN_OFFSET
#define IBAC_DEV_ID_MAIN_VOLUME_OFFSET 32
#endif
//#define	IBAC_DEV_ID_DAI_TCC		
#define	IBAC_DEV_ID_MIN			IBAC_DEV_ID_MAIN_VOLUME
#ifdef IBAC_VOLUME_MAIN_OFFSET
#define	IBAC_DEV_ID_MAX			IBAC_DEV_ID_MAIN_VOLUME_OFFSET
#else
#define	IBAC_DEV_ID_MAX			IBAC_DEV_ID_DAI_MTRAFFIC
#endif

#define	IBAC_AUDIO_OP_OPEN		1
#define	IBAC_AUDIO_OP_CLOSE		2
#define	IBAC_AUDIO_OP_VOLUME		3
#define	IBAC_AUDIO_OP_MUTE		4
#define	IBAC_AUDIO_OP_UNMUTE		5
#define	IBAC_AUDIO_OP_QUERY		6
#define	IBAC_AUDIO_OP_SAVE		7
#define	IBAC_AUDIO_OP_ALWAYS		8
#define	IBAC_AUDIO_OP_MIX			9
#define	IBAC_AUDIO_OP_MONO		10
#define	IBAC_AUDIO_OP_SETDEF		11
#define	IBAC_AUDIO_OP_OPENSR		12
#define	IBAC_AUDIO_OP_CLOSESR		13
#ifdef IBAC_VOLUME_MAIN_OFFSET
#define IBAC_AUDIO_OP_OFFSET	14
#define IBAC_AUDIO_OP_QUERYEXT		15
#define IBAC_AUDIO_OP_OPENSPDVOL	16
#define IBAC_AUDIO_OP_CLOSESPDVOL	17
#define	IBAC_AUDIO_OP_MAX		IBAC_AUDIO_OP_CLOSESPDVOL
#else
#define IBAC_AUDIO_OP_QUERYEXT		14
#define IBAC_AUDIO_OP_OPENSPDVOL	15
#define IBAC_AUDIO_OP_CLOSESPDVOL	16
#define	IBAC_AUDIO_OP_MAX		IBAC_AUDIO_OP_CLOSESPDVOL
#endif

#define IBAC_AUDIO_SWITCH_ON		1
#define IBAC_AUDIO_SWITCH_OFF		0

#define IBAC_VOL_INVAL			-1

#define IBAC_AUDIO_OP_RESULT_SUCCESS	0
#define IBAC_AUDIO_OP_RESULT_WRONG_DEVID	-1
#define IBAC_AUDIO_OP_RESULT_BLOCKED	-2
#define IBAC_AUDIO_OP_RESULT_SAVE_FAIL	-3
#define IBAC_AUDIO_OP_RESULT_LOAD_FAIL	-4


#define AUDIO_CONTROL_CENTRAL

#define IBAC_AUDIO_SERVICE_NAME ((char *)"com.innov.VoiceModule")
#define IBAC_AUDIO_SERVER_NAME ((char *)"/VoiceModule")

//dmix plug conf
static const char *IBACDmixDev[] = 
	{
		"",	// IBAC_DEV_ID_MAIN_VOLUME		1
		"",	// IBAC_DEV_ID_DAI			2
		"",	// IBAC_DEV_ID_RADIO		3
		"",	// IBAC_DEV_ID_TV			4
		"",	// IBAC_DEV_ID_3G_PHONE_SPEAKER	5
		"",	// IBAC_DEV_ID_MIC			6
		"",	// IBAC_DEV_ID_BOLUETOOTH		7
		"",	// IBAC_DEV_ID_CD_DVD		8
		"",	// IBAC_DEV_ID_WALKI_TALKI		9
		"",	// IBAC_DEV_ID_BYPASS_MX		10
/*		
		"plug:tcc",	// IBAC_DEV_ID_DAI_MP3		11
		"plug:tcc",	// IBAC_DEV_ID_DAI_VIDEO		12
		"plug:tcc",	// IBAC_DEV_ID_DAI_NETR		13
		"plug:gps",	// IBAC_DEV_ID_DAI_GPS		14
		"plug:tcc",	// IBAC_DEV_ID_DAI_NEWS		15
		"plug:tcc",	// IBAC_DEV_ID_DAI_SMS		16
		"plug:tcc",	// IBAC_DEV_ID_DAI_CARINFO		17
		"plug:tcc",	// IBAC_DEV_ID_DAI_LIFEINFO	18
		"plug:tcc"	// IBAC_DEV_ID_DAI_COMMAND		19
*/	
		"mp3vol",	// IBAC_DEV_ID_DAI_MP3		11
		"videovol",	// IBAC_DEV_ID_DAI_VIDEO		12
		"webradiovol",	// IBAC_DEV_ID_DAI_NETR		13
		"gpsvol",	// IBAC_DEV_ID_DAI_GPS		14
		"newsvol",	// IBAC_DEV_ID_DAI_NEWS		15
		"smsvol",	// IBAC_DEV_ID_DAI_SMS		16
		"carinfovol",	// IBAC_DEV_ID_DAI_CARINFO		17
		"lifeinfovol",	// IBAC_DEV_ID_DAI_LIFEINFO	18
		"cmdvol",	// IBAC_DEV_ID_DAI_COMMAND		19
		"anucvol",	// IBAC_DEV_ID_DAI_ANNOUNCE		20
		"ringvol",	// IBAC_DEV_ID_DAI_RING			21
		"vphonevol",  // IBAC_DEV_ID_DAI_VPHONE		22
		"keytonevol",  // IBAC_DEV_ID_DAI_KEYTONE		23
		"illegaltipsvol",	//IBAC_DEV_ID_DAI_ILLEGALTIPS 24
		"smsringvol",     //IBAC_DEV_ID_DAI_SMSRING
		"tonevol",        //IBAC_DEV_ID_DAI_TONE
    	"vtalkivol",      //IBAC_DEV_ID_DAI_VTALKI
	    "monlinevol",     //IBAC_DEV_ID_DAI_MONLINE
    	"vonlinevol",     //IBAC_DEV_ID_DAI_VONLINE
    	"lbookvol",       //IBAC_DEV_ID_DAI_LBOOK
    	"mtrafficvol"     //IBAC_DEV_ID_DAI_MTRAFFIC

	};

typedef struct
{
	unsigned int	logicId;
	int 	numid_vol;
	int 	numid_switch;		
	char	name[20];
}devIdStruct_t; 

typedef struct
{
	devIdStruct_t	id;		// logic to control id	
	int		on_off;			// 1: on; 0: off
	int		volume;			// current volume in percentage
	//int		on_off_pre;		// previous switch, used for restoring device, 0 or 1
	//int		volume_pre;		// previous volume, used for restoring device, in percentage
	//int		priority_back;	// priority in percentage as background audio	
	//bool	interferred;	// state indicating audio interferred by other source. 0: normal; 1: interferred
	 /*
	   * soundStatus:
	   * bit7			bit6			bit5			bit4			bit3			bit2			bit1			bit0
	   * closeSR=0		close=0		resume=0		unmute=0		unchanged=0	---			---			---
	   * openSR=1		open=1		pause=1		mute=1		changed=1	always=1		mix=1		mono=1
	   */
	unsigned char soundStatus;   
	unsigned char otherStatus;
} audioControlArray_t;

#define	IBAC_STATUS_OPEN	0x40
#define	IBAC_STATUS_PAUSE	0x20
#define	IBAC_STATUS_MUTE	0x10
#define	IBAC_STATUS_CHANGED	0x08

#define	IBAC_STATUS_TYPE	0x07
#define	IBAC_STATUS_ALWAYS	0x04
#define	IBAC_STATUS_MIX		0x02
#define	IBAC_STATUS_MONO	0x01

#define IBAC_OTHERSTATUS_SPEEDSOUND 0x01

enum sound_Status {	
	MONOTYPE,
	MIXTYPE,
	ALWAYSTYPE,
	MUTESTATUS,
	UNMUTESTATUS,
	PAUSESTATUS,
	RESUMESTATUS
};	


class AudioControlC;
class AutoSaveConfFile
{
public:
	AutoSaveConfFile(AudioControlC *pSAudio);
	~AutoSaveConfFile();
	int     valueChanged();
	static void *saveThread(void *);
private:
	pthread_mutex_t      m_lock;
	pthread_cond_t 	m_cond;
	pthread_t 			m_threadId;
	

	char				m_valueChangedFlag;
	char				m_quitFlag;
	AudioControlC *	m_pSAudio;

};

// class for updating the audio info to upper level
class AudioChangeListenerC
{
public:
	virtual void audioStatusUpdate(unsigned int, int, int, unsigned char){}

};

// class for audio control
class AudioControlC
{
public: 
	AudioControlC(AudioChangeListenerC *obj);
	~AudioControlC();
	
//	int audioChange(unsigned int devId, int volume, int on_off, int open_close);
	int initAudio();
	int openAudio(unsigned int id, int vol);
	int closeAudio(unsigned int id);
	int openSpeechRecognition();
	int closeSpeechRecognition();
	int adjustAudioVol(unsigned int id, int vol);
	int setupAudio(unsigned int id, int ctl) ;
	int muteAudio(unsigned int id);
	int unmuteAudio(unsigned int id);
	int getAllAudio(audioControlArray_t *allInfo);
	int getAudio(unsigned int id, int *vol, int* on_off, unsigned char *soundStatus);
	int getAudioExt(unsigned int id, int *vol, int* on_off, unsigned char *soundStatus, unsigned char *otherStatus);
	int saveAudio();
	int setDefaultParameters();
	int setAudioVolAccToSpeed(int carSpeed);
	int openSetAudioVolAccToSpeed();
	int closeSetAudioVolAccToSpeed();
#ifdef IBAC_VOLUME_MAIN_OFFSET
	int setMainVolOffset(int offset);
#endif
private:
#ifdef INCAR_9G1_SETVOL_H
	int syncMainAudioVol(int id, int vol);
	int getTopSoundAudio();
#endif
	long get_integer(long x, long min, long max);
	long long get_integer64(long long x, long long min, long long max);
	int csetAudio(int numid, char *name, int volume);
	void printDevCtlInfo(unsigned int id);
        void printDevStatInfo(unsigned char stat);

	int delCellStak(unsigned int logId);
	int addCellStak(unsigned int logId);

	void printData();

	void resetAudioArr();

	int channelSwitchOff(audioControlArray_t *aCArrOff, int volume,int initFlag=0);
	int channelSwitchOn(audioControlArray_t *aCArrOn, int volume,int initFlag=0);  //open and save volume if volume>0

	int changeHardVolume(audioControlArray_t *audioItem,int newVolume);  //don't store newVolume
	int getHardVolume(int numid, char *name);  //get current used volume

	int checkSoundType(unsigned char sStatus);
	int checkPhoneOpen();
	int setAudioStackRules();
	int setAudioStack();
        int convertVol(int vol, unsigned int id);
        int percent2val(int val, int min, int max);
	int percent2val_2(int val, int min, int max);		
	int percent2val_3(int val, int min, int max);		
#ifdef IBAC_VOLUME_MAIN_OFFSET
	int initMainVolOffset();
	int setMainVolLeft(int offset);
	int setMainVolRight(int offset);
#endif
	int muteGF();
	int unmuteGF();
	int setGFStatus();
	int initAlsaPlug();	
	int restoreGpsAudio();
	int adjustGpsAudio();

private:
	char card[20];
	static devIdStruct_t devLogicId2CidSet[IBAC_DEV_ID_MAX+1];
	audioControlArray_t audioControlArr[IBAC_DEV_ID_MAX];
	unsigned int audioDevActStk[IBAC_DEV_ID_MAX];
	unsigned int devActCnt;
	int openSRFlag;
	AudioChangeListenerC *listener;

	AutoSaveConfFile 	m_autoSaveMonitor;
	int m_deltaVol;
	int m_carSpeed;
	char gpsAudioControlArr[IBAC_DEV_ID_MAX];
	
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


class AudioCtrlCBHandlerC
{
public:
	virtual void notifyHandle(int, int, unsigned char){}
};


typedef struct _AudioCtrlCBHandler{
	unsigned int id;
	AudioCtrlCBHandlerC *pHandle;
	struct _AudioCtrlCBHandler *pNext;
}AudioCtrlCBHandler_t;

class AudioCtrlCallbackC
{

public:
	AudioCtrlCallbackC();
	~AudioCtrlCallbackC();

	int registerHandler(unsigned int id, AudioCtrlCBHandlerC* pCBHandler);
	int deRegisterHandler(unsigned int id, AudioCtrlCBHandlerC* pCBHandler);
	int callHandlder(unsigned int id, int vol, int sw_on_off, unsigned char sStat);

private:
	AudioCtrlCBHandler_t *serarchHandlerLink(unsigned int id, AudioCtrlCBHandlerC* pCBHandler);

private:
	AudioCtrlCBHandler_t *pHandlerLinkHeader;
	AudioCtrlCBHandler_t *pHandlerLinkTail;
	unsigned int nHandler;

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
