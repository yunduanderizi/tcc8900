#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <assert.h>
#include <pthread.h>
#include <alsa/asoundlib.h>
#include "i2ccontrol.h"
//#include "i2ctl.h"


#include "debugMonitor.h"
#include "audio_control.h"

#define IBAC_INIT_VOLUME_DAI_AMIXER 255

#define IBAC_AUDIO_PRIORITY_HI		100
#define IBAC_AUDIO_PRIORITY_LO		0
#ifdef  INCAR_9C1
#define IBAC_VOLUME_FULL_SCALE          40
#define IBAC_VOLUME_MAX_PERCENT		40
#define IBAC_INIT_MAIN_PROTECTION_VOL 15 //changeHardVolume   oldValue=0  newValue=107
#else
#define IBAC_VOLUME_FULL_SCALE          30
#define IBAC_VOLUME_MAX_PERCENT		30
#define IBAC_INIT_MAIN_PROTECTION_VOL 30 //amixer cget vol = 115
#endif

#define IBAC_INIT_VOLUME_MAIN		18
#define IBAC_INIT_VOLUME_DAI		28
#define IBAC_INIT_VOLUME_DAI_MIX        20
#ifdef _8925_VERSION
#define IBAC_INIT_VOLUME_ANALOG		30
#else
#define IBAC_INIT_VOLUME_ANALOG		20
#endif

//#define IBAC_AUDIO_INFO_FILE ((char*)"../data/audio.cfg")
#define IBAC_AUDIO_INFO_FILE ((char*)"/nand3/applicationdata/audio.cfg")
#define IBAC_AUDIO_DEF_INFO_FILE ((char*)"../data/audiodefault.cfg")
//#define IBAC_AUDIO_INFO_FILE ((char*)"audio.cfg")

#ifdef INCAR_9G1_SETVOL_H
#define IBAC_VOLUME_MAIN_MAX            112
#else
	#ifdef _8925_VERSION
	#define IBAC_VOLUME_MAIN_MAX            30
	#else
	#define IBAC_VOLUME_MAIN_MAX            127
	#endif
#endif

#ifdef  INCAR_9C1
#define IBAC_VOLUME_MAIN_MIN            50
#elif INCAR_9G1_SETVOL_H
//#define IBAC_VOLUME_MAIN_MIN            47
#define IBAC_VOLUME_MAIN_MIN            30
#else
//#define IBAC_VOLUME_MAIN_MIN            0x30
	#ifdef _8925_VERSION
	#define IBAC_VOLUME_MAIN_MIN            0
    #define IBAC_VOLUME_VIDEO_MAX           240
	#else
	#define IBAC_VOLUME_MAIN_MIN            80
	#endif
#endif

#define IBAC_VOLUME_MAIN_STEP_INV       1

#define IBAC_VOLUME_ANALOG_MAX          0xCF
#define IBAC_VOLUME_ANALOG_MIN          0xA5
#define IBAC_VOLUME_ANALOG_STEP_INV     2

#define IBAC_VOLUME_DIGITAL_MAX         0xFF
#define IBAC_VOLUME_DIGITAL_MIN         0x01
#define IBAC_VOLUME_DIGITAL_STEP_INV    2

#define IBAC_VOLUME_VOL_DIV_POINT       20
#define IBAC_VOLUME_VOL_DENOM           4
#define IBAC_VOLUME_VOL_NOM             1


// debug

//#define IBAC_AUDIO_CONFIG_LOG

#ifdef IBAC_AUDIO_CONFIG_LOG
#else
#endif

#define ALSA_PLUG_NAME_MAX (IBAC_DEV_ID_MAX -10)

// macro
#define convert_prange1(val, min, max) \
        ceil((val) * ((max) - (min)) / (IBAC_VOLUME_FULL_SCALE) + (min))

#define check_range(val, min, max) \
	((val < min) ? (min) : (val > max) ? (max) : (val)) 

#define CONVERT_APPVOL(vol) ((vol)*30/30)
#define CONVERT_VOLAPP(vol) ((vol)*30/30)


#define IBAC_SPEED_MIN 60
#define IBAC_SPEED_MAX 400
#define IBAC_SPEED_STEP 10
#define IBAC_SPEED_VOLMIN 0
#define IBAC_SPEED_VOLMAX 26

#ifdef IBAC_VOLUME_MAIN_OFFSET

#define IBAC_VOLUME_MAIN_OFFSET_MIN 0
#define IBAC_VOLUME_MAIN_OFFSET_MAX 127
#define IBAC_VOLUME_MAIN_OFFSET_PERCENT_MIN 0 
#define IBAC_VOLUME_MAIN_OFFSET_PERCENT_MAX 50
#endif
#ifdef INCAR_9G1_SETVOL_H
#define IBAC_VOLUME_RADIO_MAX 200
#define IBAC_VOLUME_RADIO_MIN 170

#define IBAC_VOLUME_MIC_MAX 200
#define IBAC_VOLUME_MIC_MIN 80

#define IBAC_VOLUME_DAI_MAX 255
#define IBAC_VOLUME_DAI_MIN 100

#define IBAC_VOLUME_DAI_PHONE_MAX 230
#define IBAC_VOLUME_DAI_PHONE_MIN 100
#else
#define IBAC_VOLUME_RADIO_MAX 210
#define IBAC_VOLUME_RADIO_MIN 170

#define IBAC_VOLUME_MIC_MAX 200
#define IBAC_VOLUME_MIC_MIN 80

#define IBAC_VOLUME_DAI_MAX 255
#define IBAC_VOLUME_DAI_MIN 100

#define IBAC_VOLUME_DAI_PHONE_MAX 212
#define IBAC_VOLUME_DAI_PHONE_MIN 100
#endif
// constuctor
AudioControlC::AudioControlC(AudioChangeListenerC *obj)
:m_autoSaveMonitor(this)
{
	strcpy(card, "default");
	
	// set the object pointer containing the callback function
	listener = obj;
	openSRFlag = 0;
	// initialize the audio device info array
	memset(audioControlArr, 0, sizeof(audioControlArr));

	// initalize the active audio device stack
	memset(audioDevActStk, 0, sizeof(audioDevActStk));	
	devActCnt = 0;
	// copy the id array to audioControlArr from devLogicId2CidSet
	for (int i = 0; i < IBAC_DEV_ID_MAX; i++)
	{
		memcpy(&audioControlArr[i], &devLogicId2CidSet[i], sizeof(devIdStruct_t));
		audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_OFF;
	}
    m_deltaVol = 0;
    m_carSpeed = 0;
	//thread lock
	pthread_mutexattr_t     mutexattr;

        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_lock,&mutexattr);	
}	

AudioControlC::~AudioControlC()
{
}

devIdStruct_t AudioControlC::devLogicId2CidSet[IBAC_DEV_ID_MAX+1]=
{
	// logic id, numid_volume, numid_switch
#ifdef _8925_VERSION
	{IBAC_DEV_ID_MAIN_VOLUME, 4, 10, ""},
#else
    {IBAC_DEV_ID_MAIN_VOLUME, 1, 10, ""},			//numid=1,iface=MIXER,name='Headphone Playback Volume'
#endif

	{IBAC_DEV_ID_DAI, 3, 10, ""},					//numid=10,iface=MIXER,name='Output Mixer DAC Switch'

#ifdef _8925_VERSION
	{IBAC_DEV_ID_RADIO, 6, 29, ""},	
#else
	{IBAC_DEV_ID_RADIO, 6, 13, ""},				//numid=13,iface=MIXER,name='Input Mixer AIN1 Switch'
#endif

	{IBAC_DEV_ID_TV, 6, 14, ""},					//numid=14,iface=MIXER,name='Input Mixer AIN2 Switch'

#ifdef _8925_VERSION
	{IBAC_DEV_ID_3G_PHONE_SPEAKER, 6, 32, ""},
#else
	{IBAC_DEV_ID_3G_PHONE_SPEAKER, 6, 15, ""},	//numid=15,iface=MIXER,name='Input Mixer AIN3 Switch'
#endif

	{IBAC_DEV_ID_MIC, 6, 16, ""},					//numid=16,iface=MIXER,name='Input Mixer AIN4 Switch'
	{IBAC_DEV_ID_BOLUETOOTH, 6, 17, ""},			//numid=17,iface=MIXER,name='Input Mixer AIN5 Switch'

	{IBAC_DEV_ID_CD_DVD, 0, 11, ""},				//numid=11,iface=MIXER,name='Output Mixer AUX Switch'
	{IBAC_DEV_ID_WALKI_TALKI, 0, 0, "wkitki"},		//plug:wkitki
	{IBAC_DEV_ID_BYPASS_MX, 0, 12, ""},			//numid=12,iface=MIXER,name='Output Mixer Bypass Switch'
				
	{IBAC_DEV_ID_DAI_MP3, 0, 0, "mp3vol"},		//plug:mp3...

#ifdef _8925_VERSION
	{IBAC_DEV_ID_DAI_VIDEO, 56, 0, "videovol"},
#else
	{IBAC_DEV_ID_DAI_VIDEO, 0, 0, "videovol"},	
#endif

	{IBAC_DEV_ID_DAI_NETR, 0, 0, "webradiovol"},

        {IBAC_DEV_ID_DAI_GPS, 0, 0, "gpsvol"},
	{IBAC_DEV_ID_DAI_NEWS, 0, 0, "newsvol"},
	{IBAC_DEV_ID_DAI_SMS, 0, 0, "smsvol"},
	{IBAC_DEV_ID_DAI_CARINFO, 0, 0, "carinfovol"},
	{IBAC_DEV_ID_DAI_LIFEINFO, 0, 0, "lifeinfovol"},
	{IBAC_DEV_ID_DAI_COMMAND, 0, 0, "cmdvol"},
	{IBAC_DEV_ID_DAI_ANNOUNCE, 0, 0, "anucvol"},
	{IBAC_DEV_ID_DAI_RING, 0, 0, "ringvol"},
	{IBAC_DEV_ID_DAI_VPHONE, 0, 0, "vphonevol"},
	{IBAC_DEV_ID_DAI_KEYTONE, 0, 0, "keytonevol"},
	{IBAC_DEV_ID_DAI_ILLEGALTIPS, 0, 0, "illegaltipsvol"},
	{IBAC_DEV_ID_DAI_SMSRING, 0, 0, "smsringvol"},
	{IBAC_DEV_ID_DAI_TONE, 0, 0, "tonevol"},
	{IBAC_DEV_ID_DAI_VTALKI, 0, 0, "vtalkivol"},
	{IBAC_DEV_ID_DAI_MONLINE, 0, 0, "monlinevol"},
	{IBAC_DEV_ID_DAI_VONLINE, 0, 0, "vonlinevol"},
	{IBAC_DEV_ID_DAI_LBOOK, 0, 0, "lbookvol"},
	{IBAC_DEV_ID_DAI_MTRAFFIC, 0, 0, "mtrafficvol"},
#ifdef IBAC_VOLUME_MAIN_OFFSET
    {IBAC_DEV_ID_MAIN_VOLUME_OFFSET, 16/*L-20 / R-21*/, 0, ""},
#endif
//	{IBAC_DEV_ID_DAI_TCC, 0, 0, "tccvol"},

	{0, 0, 0, ""}
};
/*
int setCodeRegs()
{
        int codec_reg = 0x1b;
        int fd = i2copen(0);
        //IBD_PRINTF(4, "\n======================= Set Audio Codec Register ========================\n");
        i2cset(fd, codec_reg, 0x10, 0xB, 'w');
        i2cset(fd, codec_reg, 0x11, 0x100, 'w');
        i2cset(fd, codec_reg, 0x12, 0x92, 'w');
        i2cset(fd, codec_reg, 0x14, 0x6, 'w');
        i2cclose(fd);
}
*/

// initialize the audio
// first, load the audio info array from file in disk if exist, else reset
// then, open default audio device
int AudioControlC::initAudio()
{
	InternalLock	lock(&m_lock);

	int ret_size, ret = IBAC_AUDIO_OP_RESULT_LOAD_FAIL;
	int flag = 0;

#ifdef _8925_VERSION
    system("amixer cset numid=8 23");
    system("amixer cset numid=9 30");
#endif

	//setCodeRegs();

	// reset the local audio array
	resetAudioArr();
	for (int i = 0; i < IBAC_DEV_ID_MAX; i++) 
	{
	    	audioControlArr[i].otherStatus &= 0x00; // close speed sound.
		audioControlArr[i].soundStatus &= 0xbf; // set to close.
		audioControlArr[i].soundStatus |= 0x20; // set to pause.
//		audioControlArr[i].soundStatus &= 0xef; // set to unmute.
		audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_OFF;
	}
	// first step, load audio info file
	FILE *fd = fopen(IBAC_AUDIO_DEF_INFO_FILE, "r+");
	if (!fd)	// audio info file not exist, reset the audio info array
	{
//		IBD_PRINTF(4, "\n [Audio Control]: Create the new audio info file, RESET!!!\n");

		/*fd = fopen(IBAC_AUDIO_INFO_FILE, "w+");
		IBD_PRINTF(4, "\n [Audio Control]: new audio info file created, begin Write!!!\n");
		if ((ret_size = fwrite(audioControlArr, sizeof(audioControlArray_t), IBAC_DEV_ID_MAX+1, fd)) != IBAC_DEV_ID_MAX+1)
		{
                        IBD_PRINTF(4, "\n [Audio Control]: Create the new audio info file, write failed!!!\n");
		}
#ifdef IBAC_AUDIO_CONFIG_LOG
                IBD_PRINTF(4, "\n [Audio Control]: new audio info file written!!!\n");
#endif
		fclose(fd);*/
#ifdef IBAC_AUDIO_CONFIG_LOG
                IBD_PRINTF(4, "\n [Audio Control]: create the audiodefault.cfg!\n");
#endif
//		system("cp -arf /application/data/audio.cfg /application/data/audiodefault.cfg");
	}
	else	// load the audio info array from the file
	{
		audioControlArray_t tempAudioArray[IBAC_DEV_ID_MAX+1];
		memset(tempAudioArray, 0, sizeof(tempAudioArray));

		// read data from file
//printf("KKKKKKDDDDDDDD:%d\n",fread(tempAudioArray, sizeof(audioControlArray_t), IBAC_DEV_ID_MAX+1, fd));
		if((ret_size = fread(tempAudioArray, sizeof(audioControlArray_t), IBAC_DEV_ID_MAX+1, fd)) != IBAC_DEV_ID_MAX+1)
		{
			flag = -1;
#ifdef IBAC_AUDIO_CONFIG_LOG
                        IBD_PRINTF(4, "\n [Audio Control]: read audio info from disk fail, RESET!!!\n");
                        IBD_PRINTF(4, "\nNumer of elements in audio info file = %3d\n", ret_size/sizeof(audioControlArray_t));
#endif
		}
		else
		{
#ifdef IBAC_AUDIO_CONFIG_LOG
                        IBD_PRINTF(4, "\n ----------------------------------------------------------------------------------\n");
                        IBD_PRINTF(4, "Dev #\t numid_vol\t numid_switch\t switch\t volume\t status\t\t\t\n");
#endif					

			// check the validity of the file
			for (int i = 0; i < IBAC_DEV_ID_MAX; i++)
			{
				tempAudioArray[i].soundStatus &= 0xbf; // set to close.
				tempAudioArray[i].soundStatus |= 0x20; // set to pause.
				tempAudioArray[i].on_off = IBAC_AUDIO_SWITCH_OFF;				

#ifdef _8925_VERSION
                switch(tempAudioArray[i].id.logicId)
                {
                    case IBAC_DEV_ID_MAIN_VOLUME:
                        tempAudioArray[i].volume = 8;
                        break;
                    case IBAC_DEV_ID_DAI:
                    case IBAC_DEV_ID_BYPASS_MX:
                        tempAudioArray[i].volume = 23;
                        break;
                    case IBAC_DEV_ID_RADIO:
                        tempAudioArray[i].volume = 255;
                        tempAudioArray[i].id.logicId = 6;
                        tempAudioArray[i].id.numid_switch = 29;
                        break;
                    case IBAC_DEV_ID_TV:
                    case IBAC_DEV_ID_3G_PHONE_SPEAKER:
                    case IBAC_DEV_ID_CD_DVD:
                    case IBAC_DEV_ID_MIC:
                        tempAudioArray[i].volume = 30;
                        break;
                    case IBAC_DEV_ID_DAI_MP3:
                    case IBAC_DEV_ID_DAI_VIDEO:
                        tempAudioArray[i].volume = IBAC_VOLUME_VIDEO_MAX;
                        break;
                    case IBAC_DEV_ID_MAIN_VOLUME_OFFSET:
                        //tempAudioArray[i].volume = 0;
                        break;
                    default:
                        tempAudioArray[i].volume = 30;
                        break;
                }
#endif

#ifdef IBAC_VOLUME_MAIN_OFFSET
                if(i == IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1)
				{
				    memcpy(&audioControlArr[i], &tempAudioArray[i], sizeof(audioControlArray_t));
                    continue;
                }
#endif				
				// printf the audio info array from file
				//printDevStatInfo(tempAudioArray[i].soundStatus);
				// check the id, they all should be equal
				if (tempAudioArray[i].id.logicId != devLogicId2CidSet[i].logicId || 
				    tempAudioArray[i].id.numid_vol != devLogicId2CidSet[i].numid_vol || 
				    tempAudioArray[i].id.numid_switch != devLogicId2CidSet[i].numid_switch ||
				    strcmp(tempAudioArray[i].id.name, devLogicId2CidSet[i].name) != 0 ||
				    tempAudioArray[i].volume < 0 ||
				    tempAudioArray[i].volume > IBAC_VOLUME_MAX_PERCENT )
				{
#ifdef IBAC_AUDIO_CONFIG_LOG
                                        IBD_PRINTF(4, "\n [Audio Control]: audio info of dev #%d invalid in FILE, RESET!!!\n", devLogicId2CidSet[i].logicId);
#endif					
					flag = -1;
				}
				else // load the info from file in disk if it is correct
				{
                    printf("-----copy tempAudioArray[i] to audioControlArr[i]. [i = %d]-----\n", i);
					memcpy(&audioControlArr[i], &tempAudioArray[i], sizeof(audioControlArray_t));
				}
				// reset the interrupted flag to false			
//				audioControlArr[i].interferred = false;
			}
            if(flag != -1)
            {
                if(audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume > IBAC_INIT_MAIN_PROTECTION_VOL)
                audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume = IBAC_INIT_MAIN_PROTECTION_VOL;
/*              FILE *fd_default = fopen(IBAC_AUDIO_DEF_INFO_FILE, "r");
               	if(fd_default)
               	{
                    audioControlArray_t tempDefaultAudioArray[IBAC_DEV_ID_MAX+1];
            		if((ret_size = fread(tempDefaultAudioArray, sizeof(audioControlArray_t), IBAC_DEV_ID_MAX+1, fd_default)) == IBAC_DEV_ID_MAX+1)
            		{
            			for(int i = 0; i < IBAC_DEV_ID_MAX; i++)
            			{
                            if(audioControlArr[i].volume > tempDefaultAudioArray[i].volume)
                            {
                                audioControlArr[i].volume = tempDefaultAudioArray[i].volume;
                            }
                        }
            		}
               	}
               	fclose(fd_default);
*/               	
            }
		}

		/*if (flag == -1)			
		{
			fclose(fd);
			fd = fopen(IBAC_AUDIO_INFO_FILE, "w");
			if (fwrite(audioControlArr, sizeof(audioControlArray_t), IBAC_DEV_ID_MAX+1, fd) != IBAC_DEV_ID_MAX+1)
				IBD_PRINTF(4, "\n [Audio Control]: Save file after load failed\n");
		}*/

		fclose(fd);
		
		ret = IBAC_AUDIO_OP_RESULT_SUCCESS;
	}

	muteGF();
	usleep(100*1000);
//	system("alsactl -f /usr/share/alsa/asound.state restore");
//	initAlsaPlug();
	for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
	{
#ifdef INCAR_9C1
    	audioControlArr[i].soundStatus &=(IBAC_STATUS_TYPE); 
#else
		audioControlArr[i].soundStatus &=(IBAC_STATUS_TYPE | IBAC_STATUS_MUTE); 
#endif

		switch(i+1)
                {
                        case IBAC_DEV_ID_MAIN_VOLUME:
                        case IBAC_DEV_ID_DAI:
#ifdef IBAC_VOLUME_MAIN_OFFSET
                        case IBAC_DEV_ID_MAIN_VOLUME_OFFSET:
#endif
			{
				channelSwitchOn(&audioControlArr[i],-1,1);
/*
				audioControlArr[i].soundStatus  &=(~(IBAC_STATUS_PAUSE | IBAC_STATUS_MUTE));
				audioControlArr[i].soundStatus  |=IBAC_STATUS_OPEN;
*/
				audioControlArr[i].soundStatus  &=(IBAC_STATUS_MUTE);
				audioControlArr[i].soundStatus  |=IBAC_STATUS_OPEN;
				break;
			}
                        case IBAC_DEV_ID_BYPASS_MX:
			case IBAC_DEV_ID_RADIO:
                        case IBAC_DEV_ID_3G_PHONE_SPEAKER:
			case IBAC_DEV_ID_MIC:
			{
				channelSwitchOff(&audioControlArr[i],-1,1);
				audioControlArr[i].soundStatus |=IBAC_STATUS_PAUSE; 
#ifdef INCAR_9G1_SETVOL_H
                audioControlArr[i].soundStatus &=(~IBAC_STATUS_MUTE);
#endif
				break;
			}
			default:
			{
			/*
				if((audioControlArr[i].soundStatus & IBAC_STATUS_ALWAYS) && ((audioControlArr[i].soundStatus & IBAC_STATUS_MUTE)==0))
				{
                                	channelSwitchOn(&audioControlArr[i],-1,1);
					audioControlArr[i].soundStatus  |=IBAC_STATUS_OPEN;
				}
				else*/
				{
					channelSwitchOff(&audioControlArr[i],-1,1);
					audioControlArr[i].soundStatus |=IBAC_STATUS_PAUSE; 
#ifdef INCAR_9G1_SETVOL_H
                    audioControlArr[i].soundStatus &=(~IBAC_STATUS_MUTE);
#endif					
				}
			}
		}
	}
	setGFStatus();

#ifdef _8925_VERSION
    system("amixer cset numid=58 240");
    system("amixer cset numid=60 255");
#endif

	printData();

	return ret;	
}
int AudioControlC::initAlsaPlug()
{
	const char *plugName[ALSA_PLUG_NAME_MAX] = 
	{
		"plug:mp3",  //IBAC_DEV_ID_DAI_MP3
		"plug:video",  //IBAC_DEV_ID_DAI_VIDEO
		"plug:webradio",  //IBAC_DEV_ID_DAI_NETR
		"plug:gps",  //IBAC_DEV_ID_DAI_GPS
		"plug:news",  //IBAC_DEV_ID_DAI_NEWS
		"plug:sms", //IBAC_DEV_ID_DAI_SMS
		"plug:carinfo",  //IBAC_DEV_ID_DAI_CARINFO
		"plug:lifeinfo",  //IBAC_DEV_ID_DAI_LIFEINFO
		"plug:cmd",  //IBAC_DEV_ID_DAI_COMMAND
		"plug:anuc",  //IBAC_DEV_ID_DAI_ANNOUNCE
		"plug:ring",  //IBAC_DEV_ID_DAI_RING
		"plug:vphone",  //IBAC_DEV_ID_DAI_VPHONE
		"plug:keytone",  //IBAC_DEV_ID_DAI_KEYTONE
		"plug:illegaltips", //IBAC_DEV_ID_DAI_ILLEGALTIPS
		"plug:smsring",     //IBAC_DEV_ID_DAI_SMSRING
		"plug:tone",        //IBAC_DEV_ID_DAI_TONE
    		"plug:vtalki",      //IBAC_DEV_ID_DAI_VTALKI
		"plug:monline",     //IBAC_DEV_ID_DAI_MONLINE
    		"plug:vonline",     //IBAC_DEV_ID_DAI_VONLINE
    		"plug:lbook",       //IBAC_DEV_ID_DAI_LBOOK
    		"plug:mtraffic"     //IBAC_DEV_ID_DAI_MTRAFFIC
	};
	
	int i = 0;
	int err = 0;
	for (i = 0; i<ALSA_PLUG_NAME_MAX; i++) 
	{
		snd_pcm_t* handle;
	 	if ( (err = snd_pcm_open(&handle, plugName[i], SND_PCM_STREAM_PLAYBACK, 0)) < 0)
		{
		        IBD_PRINTF(4, "*** snd_pcm_open %s failed!\n", plugName[i]);
//			continue;
		}
		snd_pcm_drain(handle);
		snd_pcm_close(handle);
	}
	return 0;
}

int AudioControlC::setDefaultParameters()//read conf from the default file?
{
	InternalLock	lock(&m_lock);


	int ret_size;
	FILE *defd = fopen(IBAC_AUDIO_DEF_INFO_FILE, "r");
	if(defd) 
	{
		audioControlArray_t tempAudioArrayDef[IBAC_DEV_ID_MAX+1];
		memset(tempAudioArrayDef, 0, sizeof(tempAudioArrayDef));
		// read data from file
		if((ret_size = fread(tempAudioArrayDef, sizeof(audioControlArray_t), IBAC_DEV_ID_MAX+1, defd)) != IBAC_DEV_ID_MAX+1) 
		{
			IBD_PRINTF(4, "\n [Audio Control]: read default audio info from disk fail, RESET!!!\n");
			IBD_PRINTF(4, "\nNumer of elements in audio info file = %3d\n", ret_size/sizeof(audioControlArray_t));

			fclose(defd);
			return -1;
		}

		
		int  volumeChangedFlag[IBAC_DEV_ID_MAX];
		int  typeChangedFlag[IBAC_DEV_ID_MAX];
		int  muteChangedFlag[IBAC_DEV_ID_MAX];
		int  otherChangedFlag[IBAC_DEV_ID_MAX];
		
		for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
		{
#ifdef IBAC_VOLUME_MAIN_OFFSET
            if(i == IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1)
            {
    			if(audioControlArr[i].volume !=check_range(tempAudioArrayDef[i].volume,-IBAC_VOLUME_MAIN_OFFSET_PERCENT_MAX,IBAC_VOLUME_MAIN_OFFSET_PERCENT_MAX))
    			{
    				audioControlArr[i].volume =check_range(tempAudioArrayDef[i].volume,-IBAC_VOLUME_MAIN_OFFSET_PERCENT_MAX,IBAC_VOLUME_MAIN_OFFSET_PERCENT_MAX);
    				volumeChangedFlag[i]=1;
    			}
    			else
    			{
    				volumeChangedFlag[i]=0;
    			}
    						
    			if((audioControlArr[i].soundStatus & IBAC_STATUS_TYPE) != (tempAudioArrayDef[i].soundStatus & IBAC_STATUS_TYPE))
    			{
    				typeChangedFlag[i]=1;
    				audioControlArr[i].soundStatus &=(~IBAC_STATUS_TYPE);
    				audioControlArr[i].soundStatus |=(tempAudioArrayDef[i].soundStatus & IBAC_STATUS_TYPE);
    			}
    			else
    			{
    				typeChangedFlag[i]=0;
    			}

    			if(audioControlArr[i].soundStatus & IBAC_STATUS_MUTE)
    			{
    			    muteChangedFlag[i]=1;
    				audioControlArr[i].soundStatus &=(~IBAC_STATUS_MUTE);
    			}
    			else
    			{
                    muteChangedFlag[i]=0;
    			}

    			if((audioControlArr[i].otherStatus & IBAC_OTHERSTATUS_SPEEDSOUND) != (tempAudioArrayDef[i].otherStatus & IBAC_OTHERSTATUS_SPEEDSOUND))
    			{
    				otherChangedFlag[i]=1;
    				audioControlArr[i].otherStatus &=(~IBAC_OTHERSTATUS_SPEEDSOUND);
    				audioControlArr[i].otherStatus |=(tempAudioArrayDef[i].otherStatus & IBAC_OTHERSTATUS_SPEEDSOUND);
    			}
    			else
    			{
                    otherChangedFlag[i]=0;
    			}
    			continue;
			}
#endif
			if(audioControlArr[i].volume !=check_range(tempAudioArrayDef[i].volume,0,IBAC_VOLUME_MAX_PERCENT))
			{
				audioControlArr[i].volume =check_range(tempAudioArrayDef[i].volume,0,IBAC_VOLUME_MAX_PERCENT);
				volumeChangedFlag[i]=1;
			}
			else
			{
				volumeChangedFlag[i]=0;
			}
	
			
			if((audioControlArr[i].soundStatus & IBAC_STATUS_TYPE) != (tempAudioArrayDef[i].soundStatus & IBAC_STATUS_TYPE))
			{
				typeChangedFlag[i]=1;
				audioControlArr[i].soundStatus &=(~IBAC_STATUS_TYPE);
				audioControlArr[i].soundStatus |=(tempAudioArrayDef[i].soundStatus & IBAC_STATUS_TYPE);
			}
			else
			{
				typeChangedFlag[i]=0;
			}

			if(audioControlArr[i].soundStatus & IBAC_STATUS_MUTE)
			{
			    muteChangedFlag[i]=1;
				audioControlArr[i].soundStatus &=(~IBAC_STATUS_MUTE);
			}
			else
			{
                muteChangedFlag[i]=0;
			}
			
			if((audioControlArr[i].otherStatus & IBAC_OTHERSTATUS_SPEEDSOUND) != (tempAudioArrayDef[i].otherStatus & IBAC_OTHERSTATUS_SPEEDSOUND))
			{
				otherChangedFlag[i]=1;
				audioControlArr[i].otherStatus &=(~IBAC_OTHERSTATUS_SPEEDSOUND);
				audioControlArr[i].otherStatus |=(tempAudioArrayDef[i].otherStatus & IBAC_OTHERSTATUS_SPEEDSOUND);
			}
			else
			{
                otherChangedFlag[i]=0;
			}			
		}

		setAudioStackRules();

		for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
		{
#ifdef IBAC_VOLUME_MAIN_OFFSET
            if(i==IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1)
            {
    			if(volumeChangedFlag[i]==1 && (audioControlArr[i].on_off ==1))
    			{
    				//changeHardVolume(&audioControlArr[i],audioControlArr[i].volume);
			        int absOffset = abs(audioControlArr[i].volume);
    				if(audioControlArr[i].volume>=0)
    				{
                        setMainVolRight(0);
                        setMainVolLeft(absOffset);
    				}
    				else //if(audioControlArr[i].volume<0)
    				{
                        setMainVolRight(absOffset);
                        setMainVolLeft(0);
    				}
    			}
    			/*if(otherChangedFlag[i]==1)
    			{
                    changeHardVolume(&audioControlArr[i],audioControlArr[i].volume);
    			}*/
    			if(volumeChangedFlag[i]==1  || typeChangedFlag[i]==1 || muteChangedFlag[i]==1)
    			{
    				listener->audioStatusUpdate(audioControlArr[i].id.logicId,
    				  	audioControlArr[i].volume,
    				  	audioControlArr[i].on_off,
    				  	audioControlArr[i].soundStatus);//?? otherStatus
    			}
                continue;
            }
#endif		
			if(volumeChangedFlag[i]==1 && (audioControlArr[i].on_off ==1))
			{
				changeHardVolume(&audioControlArr[i],audioControlArr[i].volume);
			}
			if(otherChangedFlag[i]==1)
			{
                changeHardVolume(&audioControlArr[i],audioControlArr[i].volume);
			}
			if(volumeChangedFlag[i]==1  || typeChangedFlag[i]==1 || muteChangedFlag[i]==1)
			{
				listener->audioStatusUpdate(audioControlArr[i].id.logicId,
				  	audioControlArr[i].volume,
				  	audioControlArr[i].on_off,
				  	audioControlArr[i].soundStatus);//?? otherStatus
			}
		}
		m_autoSaveMonitor.valueChanged();

		fclose(defd);
	}
	else 
	{
		IBD_PRINTF(4, "open file %s error\n",IBAC_AUDIO_DEF_INFO_FILE);		
		return -2;
	}

	return 0;
}

int AudioControlC::checkPhoneOpen()
{
	int topSoundLogicId =-1;
	int j=0;
	for(j = devActCnt-1; j >= 0; j--) 
	{ 
		int lgId =audioDevActStk[j];

		if(lgId==IBAC_DEV_ID_3G_PHONE_SPEAKER && (audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE)==0)
		{
			topSoundLogicId =lgId;
			break;
		}
	}

	if(topSoundLogicId<0)
		return 0;

	for(j=0;j<IBAC_DEV_ID_MAX;j++)
	{
		switch(j+1)
		{
			case IBAC_DEV_ID_MAIN_VOLUME:
			{
			    if((audioControlArr[j].soundStatus & IBAC_STATUS_MUTE)==0 && (audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE))
				{
					channelSwitchOn(&audioControlArr[j], -1);
                        		audioControlArr[j].soundStatus &=(~IBAC_STATUS_PAUSE);
                        		audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;
				}
				else if((audioControlArr[j].soundStatus & IBAC_STATUS_MUTE) && (audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)==0)
				{
					channelSwitchOff(&audioControlArr[j], -1);
                        		audioControlArr[j].soundStatus |=(IBAC_STATUS_PAUSE | IBAC_STATUS_CHANGED);
				}
			    break;
			}
			case IBAC_DEV_ID_BYPASS_MX:
			case IBAC_DEV_ID_3G_PHONE_SPEAKER:
			{
				if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE) && 
				        audioControlArr[IBAC_DEV_ID_3G_PHONE_SPEAKER-1].volume>0)
				{
					   channelSwitchOn(&audioControlArr[j], -1);
                       audioControlArr[j].soundStatus &=(~IBAC_STATUS_PAUSE);
                       audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;
				}
				else if(audioControlArr[IBAC_DEV_ID_3G_PHONE_SPEAKER-1].volume == 0)
				{
				    channelSwitchOff(&audioControlArr[j], -1);
				    audioControlArr[j].soundStatus |=(IBAC_STATUS_PAUSE);
                    audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;
				}			
				break;
			}	
#ifdef IBAC_VOLUME_MAIN_OFFSET
            case IBAC_DEV_ID_MAIN_VOLUME_OFFSET:
            break;
#endif			
			default:
			{
				if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)==0)
				{
					channelSwitchOff(&audioControlArr[j], -1);
                        		audioControlArr[j].soundStatus |=(IBAC_STATUS_PAUSE | IBAC_STATUS_CHANGED);
				}
			}	
		}
	}

	for(j=0;j<IBAC_DEV_ID_MAX;j++)
	{
		if((audioControlArr[j].soundStatus & IBAC_STATUS_CHANGED))
		{
			listener->audioStatusUpdate(audioControlArr[j].id.logicId,
				  audioControlArr[j].volume,
				  audioControlArr[j].on_off,
				  audioControlArr[j].soundStatus);

			audioControlArr[j].soundStatus &= (~IBAC_STATUS_CHANGED);
		}
	}

	setGFStatus();

	printData();

	return 1;
}

int AudioControlC::setAudioStackRules()//every time the sound status is changed, do this function.
{

	if(checkPhoneOpen())
		return 0;

	int topSoundLogicId =-1;
	
	int j = 0;

	for(j = devActCnt-1; j >= 0; j--) 
	{ 
		int lgId = audioDevActStk[j];

		if((audioControlArr[lgId-1].soundStatus & IBAC_STATUS_ALWAYS) || (audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE)
		        || (audioControlArr[lgId-1].volume==0)) 
			continue;

		if(topSoundLogicId <0 || lgId==IBAC_DEV_ID_MIC)
		{
			topSoundLogicId =lgId;
		    printf("[setAudioStackRules] topSoundLogicId: %d\n", topSoundLogicId);
		}
	}

	for(j=0;j<IBAC_DEV_ID_MAX;j++)
	{
		if(j == topSoundLogicId-1)
		{
			if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE))
			{
				channelSwitchOn(&audioControlArr[j], -1);
                        	audioControlArr[j].soundStatus &=(~IBAC_STATUS_PAUSE);
                        	audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;
#ifdef INCAR_9G1_SETVOL_H
                if((audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].soundStatus&IBAC_STATUS_MUTE)==0)
                {
                    //usleep(400*1000);
                    if((j+1) != IBAC_DEV_ID_DAI_GPS)
                    {
           				audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume = audioControlArr[j].volume;
           				audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].soundStatus |= IBAC_STATUS_CHANGED;
                    }       				
       				channelSwitchOn(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1], audioControlArr[j].volume, -1);
   				}
#endif			                        	
			}
			continue;
		}

		switch(j+1)
		{
			case IBAC_DEV_ID_MAIN_VOLUME:
			case IBAC_DEV_ID_DAI:
			{
				if((audioControlArr[j].soundStatus & IBAC_STATUS_MUTE)==0 && (audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE))
				{
					channelSwitchOn(&audioControlArr[j], -1);
                        		audioControlArr[j].soundStatus &=(~IBAC_STATUS_PAUSE);
                        		audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;
				}
				else if((audioControlArr[j].soundStatus & IBAC_STATUS_MUTE) && (audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)==0)
				{
					channelSwitchOff(&audioControlArr[j], -1);
                        		audioControlArr[j].soundStatus |=(IBAC_STATUS_PAUSE | IBAC_STATUS_CHANGED);
				}
				break;
			}
			case IBAC_DEV_ID_BYPASS_MX:
			{
				unsigned char radioStatus =audioControlArr[IBAC_DEV_ID_RADIO-1].soundStatus;
				unsigned char phoneStatus =audioControlArr[IBAC_DEV_ID_3G_PHONE_SPEAKER-1].soundStatus;
				if(((radioStatus & IBAC_STATUS_OPEN) && (radioStatus & IBAC_STATUS_MUTE)==0) || 
					((phoneStatus & IBAC_STATUS_OPEN) && (phoneStatus & IBAC_STATUS_MUTE)==0)) 
				{
					if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE))
					{
						channelSwitchOn(&audioControlArr[j], -1);
                        			audioControlArr[j].soundStatus &=(~IBAC_STATUS_PAUSE);
                        			audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;
					}
				}
				else if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)==0)
				{
					channelSwitchOff(&audioControlArr[j], -1);
                        		audioControlArr[j].soundStatus |=(IBAC_STATUS_PAUSE | IBAC_STATUS_CHANGED);
				}
				break;
			}
			case IBAC_DEV_ID_RADIO:
                #ifdef _8925_VERSION
                audioControlArr[j].volume = 255;
                #endif
			case IBAC_DEV_ID_3G_PHONE_SPEAKER:
			{
				if((audioControlArr[j].soundStatus & IBAC_STATUS_OPEN) && (audioControlArr[j].soundStatus & IBAC_STATUS_MUTE)==0 
				        && (audioControlArr[j].volume!=0))
				{
					if(topSoundLogicId<0 || (audioControlArr[j].soundStatus & IBAC_STATUS_ALWAYS) 
							|| ((audioControlArr[j].soundStatus & IBAC_STATUS_MIX) && (audioControlArr[topSoundLogicId-1].soundStatus & IBAC_STATUS_MIX)))
					{
						if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE))
						{
							channelSwitchOn(&audioControlArr[j], -1);
                        				audioControlArr[j].soundStatus &=(~IBAC_STATUS_PAUSE);
                        				audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;
						}
					}
					else if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)==0)
					{
						channelSwitchOff(&audioControlArr[j], -1);
                        			audioControlArr[j].soundStatus |=(IBAC_STATUS_PAUSE | IBAC_STATUS_CHANGED);
#ifdef INCAR_9G1_SETVOL_H 
        usleep(400*1000);
#endif                        			
					}
				}
				else if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)==0)
				{
					channelSwitchOff(&audioControlArr[j], -1);
                        		audioControlArr[j].soundStatus |=(IBAC_STATUS_PAUSE | IBAC_STATUS_CHANGED);
				}
				break;
			}
			case IBAC_DEV_ID_MIC:
			{
				if((audioControlArr[j].soundStatus & IBAC_STATUS_OPEN) && (audioControlArr[j].soundStatus & IBAC_STATUS_MUTE)==0
					&& (audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE))
				{
					channelSwitchOn(&audioControlArr[j], -1);
                        		audioControlArr[j].soundStatus &=(~IBAC_STATUS_PAUSE);
                        		audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;
				}
				else if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)==0)
				{
					channelSwitchOff(&audioControlArr[j], -1);
                        		audioControlArr[j].soundStatus |=(IBAC_STATUS_PAUSE | IBAC_STATUS_CHANGED);
				}
				break;
			}
			case IBAC_DEV_ID_DAI_VPHONE:
			{
				if(topSoundLogicId==IBAC_DEV_ID_MIC &&
						(audioControlArr[j].soundStatus & IBAC_STATUS_OPEN) &&
						(audioControlArr[j].soundStatus & IBAC_STATUS_MUTE)==0)
				{
					if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)) 
					{
						channelSwitchOn(&audioControlArr[j], -1);
						audioControlArr[j].soundStatus &=(~IBAC_STATUS_PAUSE);
						audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;	//set changed flag=1,bit3=1.
					}	
					break;
				}
			}
#ifdef IBAC_VOLUME_MAIN_OFFSET
            case IBAC_DEV_ID_MAIN_VOLUME_OFFSET:
            break;
#endif
			default:
			{
				if((audioControlArr[j].soundStatus & IBAC_STATUS_ALWAYS)  && (audioControlArr[j].soundStatus & IBAC_STATUS_MUTE)==0 
							&& (audioControlArr[j].soundStatus & IBAC_STATUS_OPEN))
				{
					if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)) 
					{
						channelSwitchOn(&audioControlArr[j], -1);
						audioControlArr[j].soundStatus &=(~IBAC_STATUS_PAUSE);
						audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;	//set changed flag=1,bit3=1.
					}
				}
				else if((audioControlArr[j].soundStatus & IBAC_STATUS_MUTE)==0 && ((audioControlArr[j].soundStatus & IBAC_STATUS_OPEN) && (topSoundLogicId<0 || 
						((audioControlArr[j].soundStatus & IBAC_STATUS_MIX) && (audioControlArr[topSoundLogicId-1].soundStatus & IBAC_STATUS_MIX)))))
				{
					if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)) 
					{
						channelSwitchOn(&audioControlArr[j], -1);
						audioControlArr[j].soundStatus &=(~IBAC_STATUS_PAUSE);
						audioControlArr[j].soundStatus |= IBAC_STATUS_CHANGED;	//set changed flag=1,bit3=1.
					}
				}
				else if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)==0)
				{
					channelSwitchOff(&audioControlArr[j], -1);
                        		audioControlArr[j].soundStatus |=(IBAC_STATUS_PAUSE | IBAC_STATUS_CHANGED);
#ifdef INCAR_9G1_SETVOL_H 
        usleep(400*1000);
#endif                        		
				}
			}
		}
	} 

	for(j=0;j<IBAC_DEV_ID_MAX;j++)
	{
		if((audioControlArr[j].soundStatus & IBAC_STATUS_CHANGED))
		{
			listener->audioStatusUpdate(audioControlArr[j].id.logicId,
				  audioControlArr[j].volume,
				  audioControlArr[j].on_off,
				  audioControlArr[j].soundStatus);

			audioControlArr[j].soundStatus &= (~IBAC_STATUS_CHANGED);
		}
	}
//#ifdef INCAR_9A1 && AUDIO_AUTO_NAVI
    adjustGpsAudio();
//#endif

	setGFStatus();

	printData();
/*
	for(j = devActCnt-1; j >= 0; j--) 
	{ 
		lgId = audioDevActStk[j];

		if((audioControlArr[lgId-1].soundStatus & IBAC_STATUS_ALWAYS) || (audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE)) 
		{
			if((lgId==IBAC_DEV_ID_RADIO || lgId==IBAC_DEV_ID_3G_PHONE_SPEAKER) && (audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE)==0)
                        {
				channelSwitchOn(&audioControlArr[lgId-1], -1);
				audioControlArr[lgId-1].soundStatus &=(~IBAC_STATUS_PAUSE);
				audioControlArr[lgId-1].soundStatus |= IBAC_STATUS_CHANGED;	//set changed flag=1,bit3=1.
			}
			continue;
		}


		if(audioControlArr[lgId-1].soundStatus & IBAC_STATUS_PAUSE) 
		{		 
				audioControlArr[lgId-1].soundStatus &= (~IBAC_STATUS_PAUSE);	//set resume.
				audioControlArr[lgId-1].soundStatus |= IBAC_STATUS_CHANGED;	//set changed flag=1,bit3=1.

				ret = channelSwitchOn(&audioControlArr[lgId-1], -1);
				if(ret < 0) 
				{
					IBD_PRINTF(4, "channelSwitchOn --> %d error in switch set\n", audioControlArr[lgId-1].id.logicId);
				}
		}

		break;
	}

	for(j--; j >= 0; j--) 
	{
		int lgId2 = audioDevActStk[j];

		if((audioControlArr[lgId2-1].soundStatus & IBAC_STATUS_ALWAYS) || (audioControlArr[lgId2-1].soundStatus & IBAC_STATUS_MUTE)) 
		{
			if((lgId2==IBAC_DEV_ID_RADIO || lgId2==IBAC_DEV_ID_3G_PHONE_SPEAKER) && (audioControlArr[lgId2-1].soundStatus & IBAC_STATUS_MUTE)==0)
                        {
				channelSwitchOn(&audioControlArr[lgId2-1], -1);
				audioControlArr[lgId2-1].soundStatus &=(~IBAC_STATUS_PAUSE);
				audioControlArr[lgId2-1].soundStatus |= IBAC_STATUS_CHANGED;	//set changed flag=1,bit3=1.
			}
			continue;
		}

		if((audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MIX) && (audioControlArr[lgId2-1].soundStatus & IBAC_STATUS_MIX))
		{
			if(audioControlArr[lgId2-1].soundStatus & IBAC_STATUS_PAUSE)
			{
				channelSwitchOn(&audioControlArr[lgId2-1], -1);
				audioControlArr[lgId2-1].soundStatus &= (~IBAC_STATUS_PAUSE);	//set resume.
				audioControlArr[lgId2-1].soundStatus |= IBAC_STATUS_CHANGED;	//set changed flag=1,bit3=1.
			}
		}
		else
		{
			if((audioControlArr[lgId2-1].soundStatus & IBAC_STATUS_PAUSE)==0)
			{
				channelSwitchOff(&audioControlArr[lgId2-1],-1);
				audioControlArr[lgId2-1].soundStatus |= IBAC_STATUS_PAUSE;	//set resume.
				audioControlArr[lgId2-1].soundStatus |= IBAC_STATUS_CHANGED;	//set changed flag=1,bit3=1.
			}	
		}
	}
	

	for(j=0;j<IBAC_DEV_ID_MAX;j++)
	{
		if((audioControlArr[j].soundStatus & IBAC_STATUS_CHANGED))
		{
			listener->audioStatusUpdate(audioControlArr[j].id.logicId,
				  audioControlArr[j].volume,
				  audioControlArr[j].on_off,
				  audioControlArr[j].soundStatus);

			audioControlArr[j].soundStatus &= (~IBAC_STATUS_CHANGED);
		}
	}
*/
/*
	if (type == MONOTYPE) {
		for (j = location - 1; j >= 0; j--) {
			lgId = audioDevActStk[j];
			if ((audioControlArr[lgId-1].soundStatus & 0x10) == 0) {//if unmute:bit4=0,do
				if (checkSoundType(audioControlArr[lgId-1].soundStatus) != ALWAYSTYPE) {
					if ((audioControlArr[lgId-1].soundStatus & 0x20) == 0) {		 //if resume,do
						audioControlArr[lgId-1].soundStatus |= 0x20;		//set pause.
						audioControlArr[lgId-1].soundStatus |= 0x08;		//set changed flag=1,bit3=1.
					}
				}
			}
		}
	}
	else if (type == MIXTYPE) {
		for (j = location - 1; j >= 0; j--) {
			lgId = audioDevActStk[j];
			if ((audioControlArr[lgId-1].soundStatus & 0x10) == 0) {//if unmute:bit4=0,do
				if (checkSoundType(audioControlArr[lgId-1].soundStatus) == MONOTYPE) {
					if ((audioControlArr[lgId-1].soundStatus & 0x20) == 0) {		 //if resume,do
						audioControlArr[lgId-1].soundStatus |= 0x20;		//set pause.
						audioControlArr[lgId-1].soundStatus |= 0x08;		//set changed flag=1,bit3=1.
					}
				}
				else {
					if ((audioControlArr[lgId-1].soundStatus & 0x20) != 0) {		 //if pause,do					
						audioControlArr[lgId-1].soundStatus &= 0xdf;		//set resume.
						audioControlArr[lgId-1].soundStatus |= 0x08;		//set changed flag=1,bit3=1.
					}
				}
			}
		}
	}
	else if (type == ALWAYSTYPE) {
		//do nothing
	}
*/

	return 0;
}

int AudioControlC::adjustGpsAudio()
{   
    int j = 0;
    int volTmp;

    for(j=0;j<IBAC_DEV_ID_MAX;j++)
    {
   	    gpsAudioControlArr[j] = 0;           	    
    }

    if(!((audioControlArr[IBAC_DEV_ID_DAI_GPS-1].soundStatus & IBAC_STATUS_PAUSE)==0 && 
          (audioControlArr[IBAC_DEV_ID_DAI_GPS-1].soundStatus & IBAC_STATUS_ALWAYS)))
            return 0;

    for(j=0;j<IBAC_DEV_ID_MAX;j++)
	{

        switch(j+1)
        {
	        case IBAC_DEV_ID_MAIN_VOLUME:
			case IBAC_DEV_ID_DAI:
#ifndef _8925_VERSION
			case IBAC_DEV_ID_BYPASS_MX:
#else
                break;
			case IBAC_DEV_ID_BYPASS_MX:
                audioControlArr[j].volume = 23;
#endif
#ifdef IBAC_VOLUME_MAIN_OFFSET
			case IBAC_DEV_ID_MAIN_VOLUME_OFFSET:
#endif
			        break;
	        case IBAC_DEV_ID_DAI_GPS:
	        /*
	            gpsAudioControlArr[j]=1;
    	        volTmp = audioControlArr[j].volume + 5;
                if(volTmp > IBAC_VOLUME_FULL_SCALE)
                {
                    changeHardVolume(&audioControlArr[j], IBAC_VOLUME_FULL_SCALE);
                }
                else
                {
                    changeHardVolume(&audioControlArr[j], volTmp);
                }*/
    	             break;
	        default:
        	    if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)==0)
                {
                    gpsAudioControlArr[j]=1; 

                    if(audioControlArr[j].volume>20)
                        volTmp = audioControlArr[j].volume*50/100;
                    else
                        volTmp = audioControlArr[j].volume*30/100;
                    if(volTmp > 1)
                    {
                        changeHardVolume(&audioControlArr[j], volTmp);
                    }
                    else
                    {
                        changeHardVolume(&audioControlArr[j], 1);
                    }
                }
	    }
	 }   
        
    return 0;
}

int AudioControlC::restoreGpsAudio()
{
    int j = 0;
    for(j=0;j<IBAC_DEV_ID_MAX;j++)
	{
	    switch(j+1)
        {
	        case IBAC_DEV_ID_MAIN_VOLUME:
			case IBAC_DEV_ID_DAI:
			case IBAC_DEV_ID_BYPASS_MX:
#ifdef IBAC_VOLUME_MAIN_OFFSET
			case IBAC_DEV_ID_MAIN_VOLUME_OFFSET:
#endif			
			        continue;
            default:
                if((audioControlArr[j].soundStatus & IBAC_STATUS_PAUSE)==0
                    && gpsAudioControlArr[j]==1)
                {
                    gpsAudioControlArr[j]=0;
                    changeHardVolume(&audioControlArr[j], audioControlArr[j].volume);
                }
        }
	}
    return 0;
}

int AudioControlC::setAudioStack()
{
	int lgId = 0;
	int i = 0;
	int ret = -1;
	for (i = devActCnt-1; i >= 0; i--) {
		
		lgId = audioDevActStk[i];
		if ((audioControlArr[lgId-1].soundStatus & 0x10) == 0) {//if unmute:bit4=0,do
			if ((audioControlArr[lgId-1].soundStatus & 0x20) == 0 ) { // if the status is resume, sound out.
				if ((audioControlArr[lgId-1].soundStatus & 0x08) != 0) {	//if the change flag=1,bit3=1,do
					audioControlArr[lgId-1].soundStatus &= 0xf7;		//set the change flag=0,bit3=0.
					if ((ret = channelSwitchOn(&audioControlArr[lgId-1], -1)) < 0) {
						IBD_PRINTF(4, "channelSwitchOn --> %d error in switch set\n", audioControlArr[lgId-1].id.logicId);
						return -1;
					}
				}
			}
			else {
				if ((audioControlArr[lgId-1].soundStatus & 0x08) != 0) {	//if the change flag=1,bit3=1,do
					audioControlArr[lgId-1].soundStatus &= 0xf7;		//set the change flag=0,bit3=0.
					if ((ret = channelSwitchOff(&audioControlArr[lgId-1], -1)) < 0) {
						IBD_PRINTF(4, "channelSwitchOff --> %d error in switch set\n", audioControlArr[lgId-1].id.logicId);
						return -1;				
					}
				}
			}
		}
	}
	return 0;	
}

int AudioControlC::openAudio(unsigned int logId, int vol)
{
	InternalLock	lock(&m_lock);
    printf("[openAudio]logId: %d; vol: %d\n", logId, vol);
//	int n = 0, m = 0, ret = -1;
//	unsigned int tmp = 0;
#ifdef IBAC_VOLUME_MAIN_OFFSET
    if(logId==IBAC_DEV_ID_MAIN_VOLUME_OFFSET)
    {
        printf("Can not open IBAC_DEV_ID_MAIN_VOLUME_OFFSET\n");
        return 0;
    }
#endif
	if (logId <= 0 || logId > IBAC_DEV_ID_MAX) 
	{
		IBD_PRINTF(4, "Wrong control identifier: %d\n", logId);
		return -1;
	}

	
	if(openSRFlag  && logId==IBAC_DEV_ID_MIC && (audioControlArr[IBAC_DEV_ID_DAI_VPHONE-1].soundStatus & IBAC_STATUS_OPEN) ==0 
		&& (audioControlArr[IBAC_DEV_ID_3G_PHONE_SPEAKER-1].soundStatus & IBAC_STATUS_OPEN) ==0)
	{
		channelSwitchOff(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1],-1);
	}

	// set the volume and switch of the new device

/*
	if(vol>=0 || (audioControlArr[logId-1].soundStatus & IBAC_STATUS_PAUSE))
	{
		channelSwitchOn(&audioControlArr[logId-1], vol);
		
		audioControlArr[logId-1].soundStatus &=(~IBAC_STATUS_PAUSE);
		audioControlArr[logId-1].soundStatus &=(~IBAC_STATUS_MUTE);

		listener->audioStatusUpdate(audioControlArr[logId-1].id.logicId,
				  audioControlArr[logId-1].volume,
				  audioControlArr[logId-1].on_off,
				  audioControlArr[logId-1].soundStatus);
	}
*/
		
	audioControlArr[logId-1].soundStatus |= (IBAC_STATUS_OPEN);// set to open status,bit6=1.

	int oldVolume =audioControlArr[logId-1].volume;
	int oldOnOff  =audioControlArr[logId-1].on_off;

	if ((vol >= 0) && (vol != oldVolume)) 
	{
		audioControlArr[logId-1].volume =check_range(vol,0,IBAC_VOLUME_MAX_PERCENT);
	}

	
	if(logId!=IBAC_DEV_ID_MAIN_VOLUME && logId!=IBAC_DEV_ID_BYPASS_MX && logId != IBAC_DEV_ID_DAI)
	{
		addCellStak(logId);
	}

	if(audioControlArr[IBAC_DEV_ID_DAI_RING-1].on_off==0 || logId == IBAC_DEV_ID_3G_PHONE_SPEAKER )
		setAudioStackRules();

	if(oldVolume !=audioControlArr[logId-1].volume)
	{
	    if(oldOnOff==1 && audioControlArr[logId-1].on_off==1)
		{
			changeHardVolume(&audioControlArr[logId-1],audioControlArr[logId-1].volume);
		}

		listener->audioStatusUpdate(audioControlArr[logId-1].id.logicId,
				  audioControlArr[logId-1].volume,
				  audioControlArr[logId-1].on_off,
				  audioControlArr[logId-1].soundStatus);

		m_autoSaveMonitor.valueChanged();
	}
	
#ifdef INCAR_9G1_SETVOL_H
    if(logId != IBAC_DEV_ID_MAIN_VOLUME
    && audioControlArr[logId-1].volume != audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume
    && audioControlArr[logId-1].volume != 0
    && (audioControlArr[IBAC_DEV_ID_3G_PHONE_SPEAKER-1].soundStatus&IBAC_STATUS_OPEN) == 0
    )
    {
        audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume = audioControlArr[logId-1].volume;
        if(audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].on_off==1)
        {
   			changeHardVolume(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1],audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume);
        }    			
        listener->audioStatusUpdate(audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].id.logicId,
    			  audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume,
    			  audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].on_off,
    			  audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].soundStatus);
    }
    else if(logId == IBAC_DEV_ID_3G_PHONE_SPEAKER
    && audioControlArr[logId-1].soundStatus&IBAC_STATUS_OPEN
    && audioControlArr[logId-1].volume != audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume)
    {
        audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume = audioControlArr[logId-1].volume;
        if(audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].on_off==1)
        {
   			changeHardVolume(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1],audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume);
        }    			
        listener->audioStatusUpdate(audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].id.logicId,
    			  audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume,
    			  audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].on_off,
    			  audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].soundStatus);        
    }
#endif
    printf("[openAudio end]logId: %d; vol: %d\n", logId, vol);
	return 0;
}

int AudioControlC::closeAudio(unsigned int logId)// maybe to save
{
	InternalLock	lock(&m_lock);
    printf("[closeAudio]logId: %d\n", logId);	
#ifdef IBAC_VOLUME_MAIN_OFFSET
    if(logId==IBAC_DEV_ID_MAIN_VOLUME_OFFSET)
    {
        printf("Can not close IBAC_DEV_ID_MAIN_VOLUME_OFFSET\n");
        return 0;
    }
#endif
	if(logId==IBAC_DEV_ID_MAIN_VOLUME || logId==IBAC_DEV_ID_BYPASS_MX || logId == IBAC_DEV_ID_DAI)
		return 0;


	//int idx, ret = -1;
	if (logId <= 0 || logId > IBAC_DEV_ID_MAX) {
		IBD_PRINTF(4, "[Audio Control]: closeAudio --> Wrong device number: %d\n", logId);
		return -1;
	}
	
	audioControlArr[logId-1].soundStatus &= (~IBAC_STATUS_OPEN);		//set to close,bit6=0.

/*
	if((audioControlArr[logId-1].soundStatus & IBAC_STATUS_PAUSE)==0)
	{
		channelSwitchOff(&audioControlArr[logId-1],-1);

		audioControlArr[logId-1].soundStatus |=(IBAC_STATUS_PAUSE);

		listener->audioStatusUpdate(audioControlArr[logId-1].id.logicId,
				  audioControlArr[logId-1].volume,
				  audioControlArr[logId-1].on_off,
				  audioControlArr[logId-1].soundStatus);
	}
*/

	delCellStak(logId);

//#ifdef INCAR_9A1 && AUDIO_AUTO_NAVI
	if(IBAC_DEV_ID_DAI_GPS == logId)
    {
        restoreGpsAudio();
    }
//#endif

        if(logId == IBAC_DEV_ID_DAI_RING  || audioControlArr[IBAC_DEV_ID_DAI_RING-1].on_off==0)
                setAudioStackRules();

        if(openSRFlag  && logId==IBAC_DEV_ID_MIC)
	{
		channelSwitchOn(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1],-1);
	}
    printf("[closeAudio end]logId: %d\n", logId);
	return 0;
}

int AudioControlC::openSpeechRecognition()
{
	InternalLock	lock(&m_lock);

	openSRFlag =1;
/*
	int i = 0;
	unsigned char lgId;
	int ret = -1;
	if (openSRFlag == 0) {
		openSRFlag = 1;
		memset(tempAudioArraySR, 0, sizeof(tempAudioArraySR));
		for (int i = 0; i < IBAC_DEV_ID_MAX; i++) {
			memcpy(&tempAudioArraySR[i], &audioControlArr[i], sizeof(audioControlArray_t));
		}
		for (lgId = 1; lgId <= IBAC_DEV_ID_MAX; lgId++) {
			if (lgId == IBAC_DEV_ID_MIC || lgId == IBAC_DEV_ID_DAI_COMMAND) {
#ifdef IBAC_AUDIO_CONFIG_LOG
				IBD_PRINTF(4, "\n [Audio Control]: openSpeechRecognition !!!\n");
#endif
				audioControlArr[lgId-1].soundStatus &= 0x7f;//set to closeSR.
				continue;
			}
			if ((audioControlArr[lgId-1].soundStatus & 0x80) == 0) {//if closeSR,do
					audioControlArr[lgId-1].soundStatus |= 0x80;//set to openSR.
			}
		}
		setAudioStackRules();
		if ((ret = setAudioStack()) < 0) {
			IBD_PRINTF(4, "setAudioStack err!!!\n");
			return -1;
		}
	}	
*/
	return 0;
}

int AudioControlC::closeSpeechRecognition()
{
	InternalLock	lock(&m_lock);

	openSRFlag =0;

/*
	int i = 0;
	unsigned char lgId;
	int ret = -1;
	openSRFlag = 0;
	for (int i = 0; i < IBAC_DEV_ID_MAX; i++) {
		memcpy(&audioControlArr[i], &tempAudioArraySR[i], sizeof(audioControlArray_t));
	}
	for (lgId = 1; lgId <= IBAC_DEV_ID_MAX; lgId++) {
		if ((audioControlArr[lgId-1].soundStatus & 0x80) != 0) {//if openSR,do
			audioControlArr[lgId-1].soundStatus &= 0x7f;//set to closeSR.
		}
	}
	setAudioStackRules();
	if ((ret = setAudioStack()) < 0) {
		IBD_PRINTF(4, "setAudioStack err!!!\n");
		return -1;
	}
*/
	return 0;
}

int AudioControlC::channelSwitchOff(audioControlArray_t *aCArrOff, int /*volume*/,int initFlag)
{
    printf("[channelSwitchOff]: logId: %d; initFlag: %d\n", aCArrOff->id.logicId, initFlag);
#ifdef IBAC_VOLUME_MAIN_OFFSET
    if(aCArrOff->id.logicId==IBAC_DEV_ID_MAIN_VOLUME_OFFSET)
    {
        printf("Can not channelSwitchOff IBAC_DEV_ID_MAIN_VOLUME_OFFSET\n");
        return 0;
    }
#endif
	aCArrOff->on_off = IBAC_AUDIO_SWITCH_OFF;

	if(initFlag==0)	
		muteGF();

	if(aCArrOff->id.numid_switch > 0) 
	{
		if((csetAudio(aCArrOff->id.numid_switch, aCArrOff->id.name,IBAC_AUDIO_SWITCH_OFF)) < 0) 
		{
			IBD_PRINTF(4, "change device --> %d error in switch set\n", aCArrOff->id.logicId);
		}
	}
	else
	{
		changeHardVolume(aCArrOff,0);
	}

	if(initFlag==0)	
		setGFStatus();

	return 0;
}

int AudioControlC::channelSwitchOn(audioControlArray_t *aCArrOn, int volume,int initFlag)
{
    printf("[channelSwitchOn]: logId: %d; volume: %d; initFlag: %d\n", aCArrOn->id.logicId, volume, initFlag);
#ifdef IBAC_VOLUME_MAIN_OFFSET
    if(aCArrOn->id.logicId==IBAC_DEV_ID_MAIN_VOLUME_OFFSET)
    {
    	aCArrOn->on_off = IBAC_AUDIO_SWITCH_ON;
        printf("Can not channelSwitchOn IBAC_DEV_ID_MAIN_VOLUME_OFFSET\n");
        return 0;
    }
#endif
	aCArrOn->on_off = IBAC_AUDIO_SWITCH_ON;

	if(volume >= 0)
		aCArrOn->volume =volume;

	if(initFlag==0)	
		muteGF();

	if(aCArrOn->id.numid_switch > 0) 
	{		
		if((csetAudio(aCArrOn->id.numid_switch, NULL, IBAC_AUDIO_SWITCH_ON)) < 0)
		{
			IBD_PRINTF(4, "change device --> %d error in switch set\n", aCArrOn->id.logicId);
		}

		if(aCArrOn->id.numid_vol >0 || aCArrOn->id.name[0]!=0)
			changeHardVolume(aCArrOn,aCArrOn->volume);
	}
	else
	{
		changeHardVolume(aCArrOn,aCArrOn->volume);
	}

	if(initFlag==0)	
		setGFStatus();

	return 0;
}

#if 0
int AudioControlC::setVolume(audioControlArray_t *aCArrOn, int volume)
{

	int  oldVolume =aCArrOn->volume;

	aCArrOn->volume = volume >=0 ? check_range(volume, 0, IBAC_VOLUME_MAX_PERCENT) : aCArrOn->volume;;

	if(aCArrOn->on_off==0)
		return 0;

	if (aCArrOn->id.logicId == IBAC_DEV_ID_MAIN_VOLUME) 
	{
		return setMainVolume(oldVolume, aCArrOn->volume);
	}
	
	
	int  val = convertVol(aCArrOn->volume, aCArrOn->id.logicId);   // convert to the real vol value

IBD_PRINTF(4, ":setVolume ==%d\n",val);
	
	 if (( csetAudio(aCArrOn->id.numid_vol, aCArrOn->id.name, val)) < 0) 
	{
		IBD_PRINTF(4, "change device --> %d error in volume set\n", aCArrOn->id.logicId);
		return -1;	
	}

	
	return 0;
}

int AudioControlC::setMainVolume(int  oldVolume, int newVolume)
{
	int 	ret=0;
	int 	val_old = convertVol(oldVolume, IBAC_DEV_ID_MAIN_VOLUME);   // convert to the real vol value
	int	val_new = convertVol(newVolume, IBAC_DEV_ID_MAIN_VOLUME);   // convert to the real vol value

/*

 	int    radioOpenFlag=audioControlArr[IBAC_DEV_ID_RADIO-1].soundStatus & IBAC_STATUS_OPEN;
	int	radioOnFlag      =audioControlArr[IBAC_DEV_ID_RADIO-1].on_off ;

	int    phoneOpenFlag=audioControlArr[IBAC_DEV_ID_3G_PHONE_SPEAKER-1].soundStatus & IBAC_STATUS_OPEN;
	int	phoneOnFlag    =audioControlArr[IBAC_DEV_ID_3G_PHONE_SPEAKER-1].on_off ;
	
 	if(!((radioOpenFlag  &&  radioOnFlag)  || (phoneOpenFlag  &&  phoneOnFlag)))
 	{
		if((radioOnFlag) || (phoneOnFlag))
			channelSwitchOff(&audioControlArr[IBAC_DEV_ID_BYPASS_MX-1],-1);
 	}

	 if ((csetAudio(IBAC_DEV_ID_MAIN_VOLUME, NULL, val_new)) < 0) 
	{
			IBD_PRINTF(4, "change device --> %d error in volume set\n", IBAC_DEV_ID_MAIN_VOLUME);
			ret = -1;	
	}

	if(!((radioOpenFlag  &&  radioOnFlag)  || (phoneOpenFlag  &&  phoneOnFlag)))
 	{
		if((radioOnFlag) || (phoneOnFlag))
                        channelSwitchOn(&audioControlArr[IBAC_DEV_ID_BYPASS_MX-1],-1);
 	}
*/

	
	if(val_old==val_new)
	{
	          if ((csetAudio(IBAC_DEV_ID_MAIN_VOLUME, NULL, val_new)) < 0) 
		{
			IBD_PRINTF(4, "change device --> %d error in volume set\n", IBAC_DEV_ID_MAIN_VOLUME);
			return -1;	
		}

		return 0;
	}

	int  step =1;
	if (val_new<val_old)
		step=-1;
	
	for (int i =val_old;  i  != val_new; i+=step)
	{
                if ((csetAudio(IBAC_DEV_ID_MAIN_VOLUME, NULL,  i)) < 0) 
		{
			IBD_PRINTF(4, "change device --> %d error in volume set\n", IBAC_DEV_ID_MAIN_VOLUME);
			return -1;	
		}
	}
		
	return ret;	
}
#endif

int AudioControlC::changeHardVolume(audioControlArray_t *audioItem,int newVolume)
{
#ifdef IBAC_VOLUME_MAIN_OFFSET
    if(audioItem->id.logicId == IBAC_DEV_ID_MAIN_VOLUME_OFFSET)
    {
        printf("[changeHardVolume] id.logicId = IBAC_DEV_ID_MAIN_VOLUME_OFFSET\n");
        int absOffset;
        int oldOffset = audioControlArr[IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1].volume;

        audioControlArr[IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1].volume = newVolume;
        absOffset = abs(newVolume);

        if(oldOffset<0)
        {
            if(newVolume>0)
            {
                setMainVolRight(0);
                setMainVolLeft(absOffset);
            }
            else //if(newVolume<=0)
            {
                setMainVolRight(absOffset);
            }
        }
        else if(oldOffset>0)
        {
            if(newVolume<0)
            {
                setMainVolLeft(0);
                setMainVolRight(absOffset);
            }
            else //if(newVolume>=0)
            {
                setMainVolLeft(absOffset);
            }            
        }
        else //if(oldOffset==0)
        {
            if(newVolume<0)
            {
                setMainVolRight(absOffset);
            }
            else //if(newVolume>=0)
            {
                setMainVolLeft(absOffset);
            }      
        }
        return 0;
    }
#endif

#ifdef INCAR_9G1_SETVOL_H
	if(audioItem->id.logicId != IBAC_DEV_ID_MAIN_VOLUME)
	{
   		if(newVolume != 0)
   		{
            newVolume = IBAC_VOLUME_FULL_SCALE;//IBAC_VOLUME_FULL_SCALE;
   		}
	}
#endif
	int  oldValue = 0;//getHardVolume(audioItem->id.numid_vol,audioItem->id.name);
	int  newValue = convertVol(newVolume,audioItem->id.logicId);   // convert to the real vol value
    printf("[changeHardVolume]logicId: %d; newValue: %d\n", audioItem->id.logicId, newValue);
	if(oldValue<0)
		oldValue=0;

IBD_PRINTF(4, "changeHardVolume   oldValue=%d  newValue=%d\n",oldValue,newValue);
	
	oldValue =newValue;
	
	if(oldValue > newValue)
	{
		for(int value=oldValue;value >= newValue; value--)
		{
                	if((csetAudio(audioItem->id.numid_vol,audioItem->id.name,value)) < 0) 
			{
				IBD_PRINTF(4, "change device --> %d error in volume set\n",audioItem->id.logicId);
				return -1;	
			}
		}
	}
	else
	{
		for(int value=oldValue;value <= newValue; value++)
		{
                	if((csetAudio(audioItem->id.numid_vol,audioItem->id.name,value)) < 0) 
			{
				IBD_PRINTF(4, "change device --> %d error in volume set\n",audioItem->id.logicId);
				return -1;	
			}
		}
	}
#ifdef IBAC_VOLUME_MAIN_OFFSET
    if(audioItem->id.logicId == IBAC_DEV_ID_MAIN_VOLUME)
    {
        if((audioItem->soundStatus & IBAC_STATUS_MUTE) == 0)
        initMainVolOffset();
    }
#endif
/*    system("amixer cget numid=1");
    system("amixer cget numid=6");
    system("amixer sget videovol");*/
	return 0;
}

int AudioControlC::getHardVolume(int numid, char *name)
{
        snd_hctl_t *hctl;
        snd_hctl_elem_t *elem;

	//int ret;
	int err;
	snd_ctl_t *handle = NULL;
	snd_ctl_elem_info_t *info;
	snd_ctl_elem_id_t *id;
	snd_ctl_elem_value_t *control;
	unsigned int idx, count;
	int tmp = 0;
	snd_ctl_elem_type_t type;
	snd_ctl_elem_info_alloca(&info);
	snd_ctl_elem_id_alloca(&id);
	snd_ctl_elem_value_alloca(&control);

        if ((err = snd_hctl_open(&hctl, card, 0)) < 0) {
                IBD_PRINTF(4, "Control %s open error: %s\n", card, snd_strerror(err));
                return err;
        }

        if ((err = snd_hctl_load(hctl)) < 0) {
                IBD_PRINTF(4, "Control %s load error: %s\n", card, snd_strerror(err));
                return err;
        }

	// if numid is 0 or name is NULL, device is not available, return
        if (numid == 0 && !strcmp(name, ""))
	{		
		IBD_PRINTF(4, "[Audio Control]: Get Audio --> Wrong control identifier: %d\n", numid);
		return -1;
	}
	snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);	/* default */

	if (numid > 0)
	{
		snd_ctl_elem_id_set_numid(id, numid);		/* set the numid to id->numid */
	}
	else if (strcmp(name, "") > 0)
	{
                snd_ctl_elem_id_set_name(id,name);
	}
	
	if (handle == NULL &&
	    (err = snd_ctl_open(&handle, card, 0)) < 0) 
	{		/* open the control */
		IBD_PRINTF(4, "[Audio Control]: Set Audio --> Control %s open error: %s\n", card, snd_strerror(err));
		return err;
	}
	
	snd_ctl_elem_info_set_id(info, id);				/* info->id = id */
	
	if ((err = snd_ctl_elem_info(handle, info)) < 0) 		// get info details from drivers
	{		/* handle->ops->element_info(handle, info); */
                IBD_PRINTF(4, "[Audio Control]: Set Audio --> Cannot find the given element from control %s, name = %s, id = %d\n", card, name, numid);
		IBD_PRINTF(4, "*** error: %s\n", snd_strerror(err));		
		snd_ctl_close(handle);
		handle = NULL;
		return err;
	}

	snd_ctl_elem_info_get_id(info, id); /* FIXME: Remove it when hctl find works ok !!! */ 

	type = snd_ctl_elem_info_get_type(info);	/* type = info->type */
	count = snd_ctl_elem_info_get_count(info);	/* count = info->count*/
	snd_ctl_elem_value_set_id(control, id);		/* control->id = id */

        elem = snd_hctl_find_elem(hctl, id);

        if (!snd_ctl_elem_info_is_readable(info))
        {
                IBD_PRINTF(4, "[Audio Control]: Get Audio --> cannot readable\n");
                return err;
        }
        if ((err = snd_hctl_elem_read(elem, control)) < 0)
        {
                IBD_PRINTF(4, "Control %s element read error: %s\n", card, snd_strerror(err));
                          return err;
        }

        IBD_PRINTF(4, "\ndev id #, control--> type = %d, count = %d\n", type, count);

	idx = 0;
	switch (type) 
	{
		case SND_CTL_ELEM_TYPE_BOOLEAN:
			tmp = snd_ctl_elem_value_get_boolean(control, idx) ? 1 : 0;
			break;
		case SND_CTL_ELEM_TYPE_INTEGER:
                        tmp = (int) snd_ctl_elem_value_get_integer(control, idx);
                        IBD_PRINTF(4, "\ndev id #, type = %d, value = %d\n", SND_CTL_ELEM_TYPE_INTEGER, tmp);
			break;
		case SND_CTL_ELEM_TYPE_INTEGER64:
			tmp = (int) snd_ctl_elem_value_get_integer64(control, idx);
			break;
		case SND_CTL_ELEM_TYPE_ENUMERATED:
			tmp = (int) snd_ctl_elem_value_get_enumerated(control, idx);
			break;
		case SND_CTL_ELEM_TYPE_BYTES:
			tmp = (int) snd_ctl_elem_value_get_byte(control, idx);
			break;
		default:
			break;
	}

        snd_ctl_close(handle);
        snd_hctl_close(hctl);

        return tmp;
}

int AudioControlC::checkSoundType(unsigned char sStatus)
{
//	IBD_PRINTF(4, "###checkSoundType sStatus = 0x%2x\n", sStatus);
	int ret = -1;
	if ((sStatus&0x04) != 0) {
		ret = ALWAYSTYPE;
	}
	else if ((sStatus&0x02) != 0) {
		ret = MIXTYPE;
	}
	else if ((sStatus&0x01) != 0) {
		ret = MONOTYPE;
	}	
	return ret;
}

/*
  *  setup Audio UI
  */

int AudioControlC::setupAudio(unsigned int id, int ctl) 
{
	InternalLock	lock(&m_lock);

	
	if(id==IBAC_DEV_ID_MAIN_VOLUME)
		ctl =ALWAYSTYPE;
	else if(id==IBAC_DEV_ID_MIC)
		ctl =MONOTYPE;
#ifdef IBAC_VOLUME_MAIN_OFFSET
	else if(id==IBAC_DEV_ID_MAIN_VOLUME_OFFSET)
		ctl =ALWAYSTYPE;	
#endif

	unsigned char oldStatus=audioControlArr[id-1].soundStatus;

	switch(ctl)
	{
		case MONOTYPE:
			audioControlArr[id-1].soundStatus &=(~IBAC_STATUS_TYPE);
			audioControlArr[id-1].soundStatus |=IBAC_STATUS_MONO;
			break;
		case MIXTYPE:
			audioControlArr[id-1].soundStatus &=(~IBAC_STATUS_TYPE);
			audioControlArr[id-1].soundStatus |=IBAC_STATUS_MIX;
			break;
		case ALWAYSTYPE:
			audioControlArr[id-1].soundStatus &=(~IBAC_STATUS_TYPE);
			audioControlArr[id-1].soundStatus |=IBAC_STATUS_ALWAYS;
			break;
	}

	if(audioControlArr[id-1].soundStatus != oldStatus)
	{
/*
		if((oldStatus & IBAC_STATUS_ALWAYS) &&  ((audioControlArr[id-1].soundStatus & IBAC_STATUS_ALWAYS)==0) && 
				((audioControlArr[id-1].soundStatus & IBAC_STATUS_OPEN)==0))
		{
			channelSwitchOff(&audioControlArr[id-1],-1);			

			audioControlArr[id-1].soundStatus |=(IBAC_STATUS_PAUSE);

			listener->audioStatusUpdate(audioControlArr[id-1].id.logicId,
				  audioControlArr[id-1].volume,
				  audioControlArr[id-1].on_off,
				  audioControlArr[id-1].soundStatus);
		}
*/
		audioControlArr[id-1].soundStatus |= IBAC_STATUS_CHANGED;
		setAudioStackRules();
		m_autoSaveMonitor.valueChanged();
	}

/*
	int ret = -1;
	if (ctl != -1) {
		if (ctl == MONOTYPE) {			//mono	soundStatus: bit2=0; bit1=0; bit0=1;
			audioControlArr[id-1].soundStatus &= 0xf8;
			audioControlArr[id-1].soundStatus |= 0x01;
		}
		else if (ctl == MIXTYPE) {			//mix	soundStatus: bit2=0; bit1=1; bit0=0;
			audioControlArr[id-1].soundStatus &= 0xf8;
			audioControlArr[id-1].soundStatus |= 0x02;
		}
		else if (ctl == ALWAYSTYPE) {		//always	soundStatus: bit2=1; bit1=0; bit0=0;
			audioControlArr[id-1].soundStatus &= 0xf8;
			audioControlArr[id-1].soundStatus |= 0x04;
		}
		setAudioStackRules();
		if ((ret = setAudioStack()) < 0) {
			IBD_PRINTF(4, "setAudioStack err!!!\n");
			return -1;
		}
	}
*/
	return 0;
}

int AudioControlC::getAllAudio(audioControlArray_t *allInfo)
{
	InternalLock	lock(&m_lock);

        audioControlArray_t temp_Array[IBAC_DEV_ID_MAX];
        memcpy(&temp_Array[0], &audioControlArr[0], sizeof(temp_Array));

        for (int i =0; i < IBAC_DEV_ID_MAX; i++ )
        {
                temp_Array[i].volume = CONVERT_VOLAPP(temp_Array[i].volume);
        }

        for (int i =0; i < IBAC_DEV_ID_MAX; i++ ) {
                *(allInfo+i) = temp_Array[i];
	}
	return 0;
}

int AudioControlC::adjustAudioVol(unsigned int id, int vol)
{
	InternalLock	lock(&m_lock);
	
//	IBD_PRINTF(4, "[adjustAudioVol] id: %d; newVol: %d\n", id, vol);
	unsigned int lgId = id;

#ifdef IBAC_VOLUME_MAIN_OFFSET
        if (lgId <= 0 || lgId > IBAC_DEV_ID_MAX || vol<0 || lgId == IBAC_DEV_ID_MAIN_VOLUME_OFFSET)
#else
        if (lgId <= 0 || lgId > IBAC_DEV_ID_MAX || vol<0)
#endif        
	{
		IBD_PRINTF(4, "[Audio Control]: adjustAudioVol --> Wrong control identifier: %d\n", lgId);
		return -1;
	}

#ifdef INCAR_9G1_SETVOL_H
    int oldVolume;
    int topAudioId = -1;    
    if(id==IBAC_DEV_ID_MAIN_VOLUME)
    {
        topAudioId = getTopSoundAudio();
        if(topAudioId<0)
        topAudioId=IBAC_DEV_ID_DAI_VIDEO;
    	oldVolume=audioControlArr[topAudioId-1].volume;
	    IBD_PRINTF(4, "[adjustAudioVol] topId: %d; newVol: %d; oldVol: %d\n", topAudioId, vol, oldVolume);
    }
    else
    {
       	oldVolume=audioControlArr[lgId-1].volume;
        IBD_PRINTF(4, "[adjustAudioVol] lgId: %d; newVol: %d;  oldVol: %d\n", lgId, vol, oldVolume);       	
	}
#else
	int oldVolume=audioControlArr[lgId-1].volume;
#endif
	if ((vol >= 0) && (vol != oldVolume)) 
	{	
        	audioControlArr[lgId-1].volume =check_range(vol,0,IBAC_VOLUME_MAX_PERCENT);
#ifdef INCAR_9G1_SETVOL_H
            if(topAudioId>0)
            syncMainAudioVol(topAudioId, audioControlArr[lgId-1].volume);
#endif
		if(oldVolume !=audioControlArr[lgId-1].volume)
		{
		    if(oldVolume==0 || audioControlArr[lgId-1].volume==0 || 
			        (audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE))
			{
			    if(audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE)
				    audioControlArr[lgId-1].soundStatus &=(~IBAC_STATUS_MUTE);
#ifdef INCAR_9G1_SETVOL_H
                if(lgId==IBAC_DEV_ID_MAIN_VOLUME)
                {
            		for (int i = 1; i < IBAC_DEV_ID_MAX; i++)
                    {
        			    if(audioControlArr[i].soundStatus & IBAC_STATUS_MUTE)
    	    			    audioControlArr[i].soundStatus &=(~IBAC_STATUS_MUTE);
    			    }
   			    }
#endif
				setAudioStackRules();
			}

			if((audioControlArr[lgId-1].soundStatus & IBAC_STATUS_PAUSE)==0)
			{          
                if(audioControlArr[lgId-1].on_off == 1)
                {
                    int tmpVol = audioControlArr[lgId-1].volume + m_deltaVol;
                    if((IBAC_DEV_ID_MAIN_VOLUME == lgId) && (audioControlArr[lgId-1].volume < IBAC_SPEED_VOLMAX))
                    {
                        IBD_PRINTF(4, "[adjustAudioVol] if: %d\n", lgId);
                        if(tmpVol > IBAC_SPEED_VOLMAX)
                            tmpVol = IBAC_SPEED_VOLMAX;
                        if( 0 == audioControlArr[lgId-1].volume )
                        {
                            tmpVol = 0;
                        }
#ifdef INCAR_9G1_SETVOL_H
                        //if((lgId==IBAC_DEV_ID_MAIN_VOLUME))
                        //{
                            if((audioControlArr[IBAC_DEV_ID_DAI_GPS-1].soundStatus&IBAC_STATUS_OPEN)==0)
                            {
                                if(oldVolume == 0)
                                {
                                    changeHardVolume(&audioControlArr[topAudioId-1],audioControlArr[lgId-1].volume);
                                }
                                changeHardVolume(&audioControlArr[lgId-1],audioControlArr[lgId-1].volume);
                            }
                        //}
#else
        				changeHardVolume(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1], tmpVol);
                    	printf("*** [adjustAudioVol] carspeed = %d; MAIN_VOLUME = %d; action vol = %d ***\n", \
                			m_carSpeed, audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume, tmpVol);
#endif
        			}
        			else
        			{
#ifdef INCAR_9G1_SETVOL_H
                        IBD_PRINTF(4, "[adjustAudioVol] else: %d\n", lgId);
                        if(lgId == IBAC_DEV_ID_MAIN_VOLUME)
                        {
                            if((audioControlArr[IBAC_DEV_ID_DAI_GPS-1].soundStatus&IBAC_STATUS_OPEN)==0)
           				    changeHardVolume(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1],audioControlArr[lgId-1].volume);
                        }
                        else
                        {
                            changeHardVolume(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1],audioControlArr[lgId-1].volume);
                        }
#else
        				changeHardVolume(&audioControlArr[lgId-1],audioControlArr[lgId-1].volume);
#endif
        			}
				}
            }
    		IBD_PRINTF(4, "[adjustAudioVol] listener: %d; %d; %d; 0x%2X\n", 
lgId, audioControlArr[lgId-1].volume, audioControlArr[lgId-1].on_off, audioControlArr[lgId-1].soundStatus);
			listener->audioStatusUpdate(lgId, 
				audioControlArr[lgId-1].volume, 
		        	audioControlArr[lgId-1].on_off,
		        	audioControlArr[lgId-1].soundStatus);

			m_autoSaveMonitor.valueChanged();
		}
	}
	return 0;
}
#ifdef INCAR_9G1_SETVOL_H
int AudioControlC::getTopSoundAudio()
{
   	int topSoundLogicId = -1;
   	for(int j = devActCnt-1; j >= 0; j--)
	{ 
		int lgId = audioDevActStk[j];

		if(lgId==IBAC_DEV_ID_3G_PHONE_SPEAKER && (audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE)==0)
		{
			topSoundLogicId =lgId;
			break;
		}

		if(lgId==IBAC_DEV_ID_DAI_RING && (audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE)==0)
		{
			topSoundLogicId =lgId;
			break;
		}

		if((audioControlArr[lgId-1].soundStatus & IBAC_STATUS_ALWAYS) || (audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE)
		        /*|| (audioControlArr[lgId-1].volume==0)*/) 
			continue;

		if(topSoundLogicId <0 || lgId==IBAC_DEV_ID_MIC)
		{
		    if(lgId != IBAC_DEV_ID_DAI_GPS)
		    {
    			topSoundLogicId =lgId;
		    }
		}
		/*for(int n = devActCnt-1; n >= 0; n--)
		{
		    int id = audioDevActStk[n];
            if(id==IBAC_DEV_ID_DAI_RING)
            topSoundLogicId = IBAC_DEV_ID_DAI_RING;
		}*/
	}
	printf("[getTopSoundAudio]*** topSoundLogicId: %d\n", topSoundLogicId);
    return topSoundLogicId;
}
int AudioControlC::syncMainAudioVol(int id, int vol)
{
    if(id<0)
    return -1;
    
    if(id == IBAC_DEV_ID_DAI_RING)
    {
        if(audioControlArr[id-1].volume != vol)
        {
            audioControlArr[id-1].volume = vol;
            listener->audioStatusUpdate(IBAC_DEV_ID_DAI_RING, 
                				audioControlArr[IBAC_DEV_ID_DAI_RING-1].volume, 
                	        	audioControlArr[IBAC_DEV_ID_DAI_RING-1].on_off,
                	        	audioControlArr[IBAC_DEV_ID_DAI_RING-1].soundStatus);
            IBD_PRINTF(4, "[syncMainAudioVol] lgid: %d; vol: %d; on_off: %d; st: 0x%2X\n", 
            id, audioControlArr[id-1].volume, audioControlArr[id-1].on_off, audioControlArr[id-1].soundStatus);
        }    
       	return 0;
    }
    if(id == IBAC_DEV_ID_3G_PHONE_SPEAKER)
    {
        if(audioControlArr[id-1].volume != vol)
        {
            audioControlArr[id-1].volume = vol;
            listener->audioStatusUpdate(IBAC_DEV_ID_3G_PHONE_SPEAKER, 
                				audioControlArr[IBAC_DEV_ID_3G_PHONE_SPEAKER-1].volume, 
                	        	audioControlArr[IBAC_DEV_ID_3G_PHONE_SPEAKER-1].on_off,
                	        	audioControlArr[IBAC_DEV_ID_3G_PHONE_SPEAKER-1].soundStatus);            
            IBD_PRINTF(4, "[syncMainAudioVol] lgid: %d; vol: %d; on_off: %d; st: 0x%2X\n", 
            id, audioControlArr[id-1].volume, audioControlArr[id-1].on_off, audioControlArr[id-1].soundStatus);                	        	
        }    
       	return 0;
    }
    if(id == IBAC_DEV_ID_DAI_GPS)
    {
        if(audioControlArr[id-1].volume != vol)
        {
            audioControlArr[id-1].volume = vol;
            listener->audioStatusUpdate(IBAC_DEV_ID_DAI_GPS, 
                				audioControlArr[IBAC_DEV_ID_DAI_GPS-1].volume, 
                	        	audioControlArr[IBAC_DEV_ID_DAI_GPS-1].on_off,
                	        	audioControlArr[IBAC_DEV_ID_DAI_GPS-1].soundStatus);            
            IBD_PRINTF(4, "[syncMainAudioVol] lgid: %d; vol: %d; on_off: %d; st: 0x%2X\n", 
            id, audioControlArr[id-1].volume, audioControlArr[id-1].on_off, audioControlArr[id-1].soundStatus);
        }    
       	return 0;
    }
    else //if(id == IBAC_DEV_ID_DAI_VIDEO)
    {
        for(int i=0; i<IBAC_DEV_ID_MAX; i++)
        {
            switch(i+1)
            {
                case IBAC_DEV_ID_MAIN_VOLUME:
            	case IBAC_DEV_ID_DAI:
               	case IBAC_DEV_ID_TV:
            	case IBAC_DEV_ID_3G_PHONE_SPEAKER:
            	case IBAC_DEV_ID_MIC:
            	case IBAC_DEV_ID_BOLUETOOTH:
            	case IBAC_DEV_ID_CD_DVD:
            	case IBAC_DEV_ID_WALKI_TALKI:
            	case IBAC_DEV_ID_BYPASS_MX:
                case IBAC_DEV_ID_DAI_GPS:
            	case IBAC_DEV_ID_DAI_RING:            
#ifdef IBAC_VOLUME_MAIN_OFFSET
                case IBAC_DEV_ID_MAIN_VOLUME_OFFSET:
#endif
                    break;
            	case IBAC_DEV_ID_RADIO:        	
            	case IBAC_DEV_ID_DAI_MP3:
            	case IBAC_DEV_ID_DAI_VIDEO:
            	case IBAC_DEV_ID_DAI_NETR:
            	case IBAC_DEV_ID_DAI_NEWS:
            	case IBAC_DEV_ID_DAI_SMS:
            	case IBAC_DEV_ID_DAI_CARINFO:
            	case IBAC_DEV_ID_DAI_LIFEINFO:
            	case IBAC_DEV_ID_DAI_COMMAND:
            	case IBAC_DEV_ID_DAI_ANNOUNCE:
            	case IBAC_DEV_ID_DAI_VPHONE:
            	case IBAC_DEV_ID_DAI_KEYTONE:
            	case IBAC_DEV_ID_DAI_ILLEGALTIPS:
            	case IBAC_DEV_ID_DAI_SMSRING:
            	case IBAC_DEV_ID_DAI_TONE:
            	case IBAC_DEV_ID_DAI_VTALKI:
            	case IBAC_DEV_ID_DAI_MONLINE:
            	case IBAC_DEV_ID_DAI_VONLINE:
            	case IBAC_DEV_ID_DAI_LBOOK:
            	case IBAC_DEV_ID_DAI_MTRAFFIC:
                {
                    int oldVol = audioControlArr[i].volume;
                    int newVolMain = vol;
                    if(oldVol != newVolMain)
                    {
                        audioControlArr[i].volume = newVolMain;
                    	/*listener->audioStatusUpdate(i+1, 
                				audioControlArr[i].volume, 
                	        	audioControlArr[i].on_off,
                	        	audioControlArr[i].soundStatus);*/
                        //IBD_PRINTF(4, "[syncMainAudioVol] lgid: %d; vol: %d; on_off: %d; st: 0x%2X\n", 
                        //i+1, audioControlArr[i].volume, audioControlArr[i].on_off, audioControlArr[i].soundStatus);
                	}
       	        	break;
               	}
                default:
                    break;
        	}
        }
    }
    return 0;    	
}
#endif
int AudioControlC::muteAudio(unsigned int id)
{
	InternalLock	lock(&m_lock);
#ifdef IBAC_VOLUME_MAIN_OFFSET
    if(id==IBAC_DEV_ID_MAIN_VOLUME_OFFSET)
    {
        return 0;
    }
#endif

IBD_PRINTF(4, "===================muteAudio %d %02X\n",id,audioControlArr[id-1].soundStatus);

	//int ret = -1;
	unsigned int lgId = id;
	if (lgId <= 0 || lgId > IBAC_DEV_ID_MAX) {
		IBD_PRINTF(4, "[Audio Control]: Mute --> Wrong control identifier: %d\n", lgId);
		return -1;
	}

	if((audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE))
		return 0;

	audioControlArr[lgId-1].soundStatus |=IBAC_STATUS_MUTE;
	audioControlArr[lgId-1].soundStatus |=IBAC_STATUS_CHANGED;
	
//#ifdef INCAR_9G1_SETVOL_H
    if(id == IBAC_DEV_ID_MAIN_VOLUME)
    {
		for (int i = 1; i < IBAC_DEV_ID_MAX; i++)
        {
        	if((audioControlArr[i].soundStatus & IBAC_STATUS_MUTE))
            	continue;
        	audioControlArr[i].soundStatus |=IBAC_STATUS_MUTE;
        	audioControlArr[i].soundStatus |=IBAC_STATUS_CHANGED;
    	}
   	}
//#endif

//
	if((audioControlArr[lgId-1].soundStatus & IBAC_STATUS_PAUSE)==0)
	{
		
		channelSwitchOff(&audioControlArr[lgId-1],-1);
				
		audioControlArr[lgId-1].soundStatus |=IBAC_STATUS_PAUSE;
	}

	m_autoSaveMonitor.valueChanged();
//
    printf("[muteAudio]soundStatus: 0x%x\n", audioControlArr[lgId-1].soundStatus);
       	setAudioStackRules();

	return 0;
}

int AudioControlC::unmuteAudio(unsigned int id)
{
	InternalLock	lock(&m_lock);
#ifdef IBAC_VOLUME_MAIN_OFFSET
    if(id==IBAC_DEV_ID_MAIN_VOLUME_OFFSET)
    {
        return 0;
    }
#endif

IBD_PRINTF(4, "===============un====muteAudio %d %02X\n",id,audioControlArr[id-1].soundStatus);
	//int ret = -1;
	unsigned int lgId = id;
	if (lgId <= 0 || lgId > IBAC_DEV_ID_MAX) {
		IBD_PRINTF(4, "[Audio Control]: UnMute --> Wrong control identifier: %d\n", lgId);
		return -1;
	}

	if((audioControlArr[lgId-1].soundStatus & IBAC_STATUS_MUTE)==0)
		return 0;

	audioControlArr[lgId-1].soundStatus &=(~IBAC_STATUS_MUTE);
	audioControlArr[lgId-1].soundStatus |=IBAC_STATUS_CHANGED;
	
//#ifdef INCAR_9G1_SETVOL_H
    if(id == IBAC_DEV_ID_MAIN_VOLUME)
    {
		for (int i = 1; i < IBAC_DEV_ID_MAX; i++)
        {
        	if((audioControlArr[i].soundStatus & IBAC_STATUS_MUTE)==0)
            	continue;
    	    audioControlArr[i].soundStatus &=(~IBAC_STATUS_MUTE);
        	audioControlArr[i].soundStatus |=IBAC_STATUS_CHANGED;
    	}
   	}
//#endif

///*
	if((audioControlArr[lgId-1].soundStatus &  IBAC_STATUS_OPEN) && (audioControlArr[lgId-1].soundStatus & IBAC_STATUS_PAUSE))
	{
		channelSwitchOn(&audioControlArr[lgId-1],-1);
		
		audioControlArr[lgId-1].soundStatus &=(~IBAC_STATUS_PAUSE);
	}

	m_autoSaveMonitor.valueChanged();
//*/

    printf("[unmuteAudio]soundStatus: 0x%x\n", audioControlArr[lgId-1].soundStatus);
	setAudioStackRules();

	return 0;
}

int AudioControlC::getAudio(unsigned int id, int *vol, int* on_off, unsigned char *soundStatus)
{
	InternalLock	lock(&m_lock);
	if (id <= 0 || id > IBAC_DEV_ID_MAX)
	{
		IBD_PRINTF(4, "\n[Audio Control]: Get Audio --> Wrong control identifier: %d\n", id);
		return -1;
	}
#ifdef INCAR_9G1_SETVOL_H
    if(id == IBAC_DEV_ID_MAIN_VOLUME)
    {
        int topSoundLogicId;
        topSoundLogicId = getTopSoundAudio();
        if(topSoundLogicId<0)
        topSoundLogicId=IBAC_DEV_ID_DAI_VIDEO;
	
        *vol = CONVERT_VOLAPP(audioControlArr[topSoundLogicId-1].volume);
    	*on_off = audioControlArr[topSoundLogicId-1].on_off;
//    	*soundStatus = audioControlArr[topSoundLogicId-1].soundStatus;	
    	*soundStatus = audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].soundStatus;	    	
    	return 0;
   	}
#endif
        *vol = CONVERT_VOLAPP(audioControlArr[id-1].volume);
	*on_off = audioControlArr[id-1].on_off;
	*soundStatus = audioControlArr[id-1].soundStatus;
	printDevStatInfo(*soundStatus);
	return 0;	
}

int AudioControlC::saveAudio()
{
	InternalLock	lock(&m_lock);

	FILE *fd = fopen(IBAC_AUDIO_INFO_FILE, "w");
	if (!fd) {	// open file error
		IBD_PRINTF(4, "\n[Audio Control]: Save Audio --> open file failed !!!\n");
		fclose(fd);
		return -1;
	}
	else if(fwrite(audioControlArr, sizeof(audioControlArray_t), IBAC_DEV_ID_MAX+1, fd) != IBAC_DEV_ID_MAX+1) {
		IBD_PRINTF(4, "\n[Audio Control]: Save Audio --> save file failed !!!\n");
		fclose(fd);
		return -1;		
	}

	fclose(fd);
	return 0;	
}

/*!
 *  y = (127-60) * log2(x+1) / log2(31) + 60
 *
 *  ret = (max-min) * (log(val+1)/log(2)) / (log(31)/log(2)) + min;
 *  Simplified:
 *  ret = (max-min) * (log(val+1)/log31) + min;
 *  explain:
 *  log():By e the natural logarithm base
 */

#define IBAC_E_BASE_31 3.43  //loge(31) = ln31
static double getLogBaics(void)
{
    static double res = 0.0;
    if(res <0.001)
    {
        res =log(IBAC_VOLUME_FULL_SCALE+1);
    }
    return res;
}

int AudioControlC::percent2val_2(int val, int min, int max)
{
    int ret = -1;
    if(val < 0)
        return -1;
        
    //ret = (max-min) * (log(val+1)/log(2)) / (log(31)/log(2)) + min;
   // ret = (max-min) * log(val+1) / IBAC_E_BASE_31 + min;
    ret = (max-min) * (log(val+1)) / (getLogBaics()) + min;

    
    printf("*[percent2val_2]*percent2val_vol = %d; val = %d\n", ret, val);

    return ret;
}

int AudioControlC::percent2val_3(int val, int min, int max) 
{
    int retval = -1;
    if(val < 0) 
        return -1;
    if(val == 0)    
        return 0; 

    retval = ((IBAC_VOLUME_FULL_SCALE-val)*min + (val-1)*max)/(IBAC_VOLUME_FULL_SCALE - 1); 

    printf("------------>after_map_vol = %d\n",retval);
    return retval;  
}


int AudioControlC::percent2val(int val, int min, int max)
{
        int ret = -1;
        if (val < 0)
                return -1;

        // val from [0, IBAC_VOLUME_VOL_DIV_POINT] convert to [min, min+(IBAC_VOLUME_VOL_DENOM - IBAC_VOLUME_VOL_NOM)*(max-min)/IBAC_VOLUME_VOL_DENOM] range
        // val from [IBAC_VOLUME_VOL_DIV_POINT+1, IBAC_VOLUME_FULL_SCALE] to [min+(IBAC_VOLUME_VOL_DENOM - IBAC_VOLUME_VOL_NOM)*(max-min)/IBAC_VOLUME_VOL_DENOM+1, max] range

        int mid_point = max - (max - min)*IBAC_VOLUME_VOL_NOM/IBAC_VOLUME_VOL_DENOM;

        if (val<= IBAC_VOLUME_VOL_DIV_POINT)
        {
                ret = (mid_point - min)*val/IBAC_VOLUME_VOL_DIV_POINT + min;
        }
        else
        {
                //ret = (max - (mid_point+1))*(val - (IBAC_VOLUME_VOL_DIV_POINT+1))/(IBAC_VOLUME_FULL_SCALE -(IBAC_VOLUME_VOL_DIV_POINT+1)) + mid_point+1;
                ret = (max - (mid_point))*(val - (IBAC_VOLUME_VOL_DIV_POINT))/(IBAC_VOLUME_FULL_SCALE -(IBAC_VOLUME_VOL_DIV_POINT)) + mid_point;                
        }

        return ret;
}

int AudioControlC::convertVol(int vol, unsigned int id)
{
        int ret = -1;
        //int tmp = -1;

        bool offset_flag = false;

        if (vol < 0 )
                return -1;

        switch (id)
        {
        case IBAC_DEV_ID_MAIN_VOLUME:
                ret = percent2val_2(vol, IBAC_VOLUME_MAIN_MIN, IBAC_VOLUME_MAIN_MAX);
                break;
        case IBAC_DEV_ID_DAI:
                ret = IBAC_INIT_VOLUME_DAI_AMIXER;
                break;
        case IBAC_DEV_ID_DAI_MP3:
        case IBAC_DEV_ID_DAI_VIDEO:
#ifdef _8925_VERSION
                ret = IBAC_VOLUME_VIDEO_MAX;
                break;
#endif
        case IBAC_DEV_ID_DAI_NETR:
        case IBAC_DEV_ID_DAI_GPS:
        case IBAC_DEV_ID_DAI_NEWS:
        case IBAC_DEV_ID_DAI_SMS:
        case IBAC_DEV_ID_DAI_CARINFO:
        case IBAC_DEV_ID_DAI_LIFEINFO:
        case IBAC_DEV_ID_DAI_COMMAND:
        case IBAC_DEV_ID_DAI_ANNOUNCE:
        case IBAC_DEV_ID_DAI_RING:
        case IBAC_DEV_ID_DAI_ILLEGALTIPS:        
        case IBAC_DEV_ID_DAI_SMSRING:
        case IBAC_DEV_ID_DAI_TONE:    
        case IBAC_DEV_ID_DAI_VTALKI:  
        case IBAC_DEV_ID_DAI_MONLINE: 
        case IBAC_DEV_ID_DAI_VONLINE: 
        case IBAC_DEV_ID_DAI_LBOOK:   
        case IBAC_DEV_ID_DAI_MTRAFFIC:

	case IBAC_DEV_ID_DAI_VPHONE:
	case IBAC_DEV_ID_DAI_KEYTONE:
        case IBAC_DEV_ID_WALKI_TALKI:
				//ret = percent2val(vol, IBAC_VOLUME_DIGITAL_MIN, IBAC_VOLUME_DIGITAL_MAX);
                ret = percent2val_3(vol, IBAC_VOLUME_DAI_MIN, IBAC_VOLUME_DAI_MAX);
                break;

		 //--------phone-------
		 case IBAC_DEV_ID_3G_PHONE_SPEAKER:
				 ret = percent2val_3(vol, IBAC_VOLUME_DAI_PHONE_MIN, IBAC_VOLUME_DAI_PHONE_MAX);
				 break;
		//------


        case IBAC_DEV_ID_RADIO:
#ifndef _8925_VERSION
				ret = percent2val_3(vol, IBAC_VOLUME_RADIO_MIN, IBAC_VOLUME_RADIO_MAX);
#else
				ret = 255;
#endif
				break;
        case IBAC_DEV_ID_TV:
//        case IBAC_DEV_ID_3G_PHONE_SPEAKER:
        case IBAC_DEV_ID_MIC:
        case IBAC_DEV_ID_BOLUETOOTH:
                ret = percent2val_3(vol, IBAC_VOLUME_MIC_MIN, IBAC_VOLUME_MIC_MAX);
                break;
#ifdef IBAC_VOLUME_MAIN_OFFSET                
        case IBAC_DEV_ID_MAIN_VOLUME_OFFSET:
                break;
#endif                
        default:
                ret = vol;
                break;
        }
		
        if ((vol == 0) && (offset_flag == false))
                ret = 0;
		
        if (ret >= 0)
        {
                //ret = ret*IBAC_VOLUME_MAX_PERCENT/IBAC_VOLUME_FULL_SCALE;
	}


	return ret;
}
long AudioControlC::get_integer(long x, long min, long max)
{
	long val = min;
	val = (long)convert_prange1(x, min, max);
	val = check_range(val, min, max);
	return val;
}

long long AudioControlC::get_integer64(long long x, long long min, long long max)
{
	long long val = min;
	val = (long long)convert_prange1(x, min, max);
	val = check_range(val, min, max);
	return val;
}	


/* set through cid*/
int AudioControlC::csetAudio(int numid, char *name, int volume)
{
//	name = "tccvol";// tccvol
	IBD_PRINTF(4, "[csetAudio]*** %d; %s; %d\n", numid, name, volume);
	long vol = (long)volume;
	int err;
	snd_ctl_t *handle = NULL;
	snd_ctl_elem_info_t *info;
	snd_ctl_elem_id_t *id;
	snd_ctl_elem_value_t *control;
	unsigned int idx, count;
	long tmp = 0;
	snd_ctl_elem_type_t type;
	snd_ctl_elem_info_alloca(&info);
	snd_ctl_elem_id_alloca(&id);
	snd_ctl_elem_value_alloca(&control);
	

	// if numid is 0 or name is NULL, device is not available, return
	if (numid <= 0 && !strcmp(name, ""))
	{		
		IBD_PRINTF(4, "[Audio Control]: Set Audio --> Wrong control identifier: %d\n", numid);
		return -1;
	}
	snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);	/* default */

	if (numid > 0 && numid <= IBAC_DEV_ID_MAX)
	{
		snd_ctl_elem_id_set_numid(id, numid);		/* set the numid to id->numid */
	}
	else if (strcmp(name, "") > 0)
	{
		snd_ctl_elem_id_set_name(id, name);
	}
	
	if (handle == NULL &&
	    (err = snd_ctl_open(&handle, card, 0)) < 0) 
	{		/* open the control */
		IBD_PRINTF(4, "[Audio Control]: Set Audio --> Control %s open error: %s\n", card, snd_strerror(err));
		return err;
	}
	
	snd_ctl_elem_info_set_id(info, id);				/* info->id = id */
	
	if ((err = snd_ctl_elem_info(handle, info)) < 0) 		// get info details from drivers
	{		/* handle->ops->element_info(handle, info); */
		IBD_PRINTF(4, "[Audio Control]: Set Audio --> Cannot find the given element from control %s, name = %s, id = %d\n", card, name, numid);
		IBD_PRINTF(4, "*** pb_error: %s\n", snd_strerror(err));		
		snd_ctl_close(handle);
		handle = NULL;
		return err;
	}
	
	snd_ctl_elem_info_get_id(info, id); /* FIXME: Remove it when hctl find works ok !!! */ 

	type = snd_ctl_elem_info_get_type(info);	/* type = info->type */
	count = snd_ctl_elem_info_get_count(info);	/* count = info->count*/
	snd_ctl_elem_value_set_id(control, id);		/* control->id = id */
	

#ifdef IBAC_AUDIO_CONFIG_LOG
	IBD_PRINTF(4, "\n [Audio Control]: Set Audio --> numid %d --> # of controls %d\n", numid, count);
#endif

	for (idx = 0; idx < count && idx < 128; idx++) 
	{
		switch (type) 
		{
			case SND_CTL_ELEM_TYPE_BOOLEAN:
				tmp = vol > 0 ? 1 : 0;
				snd_ctl_elem_value_set_boolean(control, idx, tmp);		/* control->value.integer.value[idx] = tmp; */
				break;
			case SND_CTL_ELEM_TYPE_INTEGER:
                                tmp = vol;
				snd_ctl_elem_value_set_integer(control, idx, tmp);	/* control->value.integer.value[idx] = tmp; */
				break;
			case SND_CTL_ELEM_TYPE_INTEGER64:
                                tmp = vol;
				snd_ctl_elem_value_set_integer64(control, idx, tmp);	/* control->value.integer64.value[idx] = tmp; */
				break;
			case SND_CTL_ELEM_TYPE_ENUMERATED:
				vol = (unsigned)(vol < 0  ? 0 : vol);				
				tmp =  check_range(vol, 0, (int)snd_ctl_elem_info_get_items(info) - 1);
				snd_ctl_elem_value_set_enumerated(control, idx, tmp);	/* control->value.enumerated.item[idx] = val; */
				break;
			case SND_CTL_ELEM_TYPE_BYTES:
				tmp = get_integer(vol, 0, 255);
				snd_ctl_elem_value_set_byte(control, idx, tmp);	/* control->value.bytes.data[idx] = val; */
				break;
			default:
				break;
		}
	}

	if ((err = snd_ctl_elem_write(handle, control)) < 0) 	/* handle->ops->element_write(handle, control) */
	{		
		IBD_PRINTF(4, "[Audio Control]: Set Audio --> Control %s element write error: %s\n", card, snd_strerror(err));		
		snd_ctl_close(handle);
		handle = NULL;
		return err;
	}

	snd_ctl_close(handle);
	handle = NULL;

	return 0;
}


// delete cell from stack
int AudioControlC::delCellStak(unsigned int logId)
{
	//int idInStack=-1;
	for(unsigned i=0; i < devActCnt; i++)
	{
		if(audioDevActStk[i]==logId)
		{
			for(;i<devActCnt-1;i++)
				audioDevActStk[i]=audioDevActStk[i+1];

			devActCnt--;

			return 0;
		}
	}
		
	return -1;
}
int AudioControlC::addCellStak(unsigned int logId)
{
	for(unsigned int i=0; i < devActCnt; i++)
	{
		if(audioDevActStk[i]==logId)
		{
			for(;i<devActCnt-1;i++)
				audioDevActStk[i]=audioDevActStk[i+1];

			audioDevActStk[i]=logId;
			
			return 0;
		}
	}

	audioDevActStk[devActCnt++]=logId;
	
	return 0;
}

void AudioControlC::printData()
{
	IBD_LOOP_BEGIN(4, "Begin printData\n");

	for (int i = 0; i < IBAC_DEV_ID_MAX; i++)
	{
		IBD_LOOP_PRINTF(4, "logId=%3d numId=%3d switchId=%3d volume=%3d on=%d status=%02X otherS=%02X\n",\
		audioControlArr[i].id.logicId,audioControlArr[i].id.numid_vol,audioControlArr[i].id.numid_switch,
		audioControlArr[i].volume,audioControlArr[i].on_off,audioControlArr[i].soundStatus,audioControlArr[i].otherStatus);
	}

	for(unsigned int i=0; i < devActCnt; i++)
	{
		IBD_LOOP_PRINTF(4, "No.%d  %d\n",i,audioDevActStk[i]);
	}

	IBD_LOOP_PRINTF(4, "openSRFlag=%d \n",openSRFlag);
	
	IBD_LOOP_END
}

#define IBAC_MUTE_GF_TIME	(1000)

int AudioControlC::muteGF()
{
/*        int file;
        file = i2copen(0);
        if( file == 0 )
                return -1;

        i2cset(file,0x6c,0x0c,0x1f,'b');

        usleep(IBAC_MUTE_GF_TIME);//µÈ 35 ms
        int nState = i2cget(file,0x6c,0x06,'b');
        int nCount = 3;
        while( (nState != 0x0f)  && (nCount-- > 0) )
        {
                usleep(IBAC_MUTE_GF_TIME);//µÈ 35 ms
                nState = i2cget(file,0x6c,0x06,'b');
        }

        i2cclose(file);
*/		
		int ret = -1;
		IBI2CControl i2cHander;
		unsigned char nState;
		int nCount = 3;
		if (i2cHander.checkOK() == 0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "Check i2cHander err!\n")
			return -1; 
		}
		ret = i2cHander.writeByte(0x6C, 0x0C, 0x1F);
		if(ret < 0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "I2c write byte err: %d\n", ret)
			return -1;
		}
        usleep(IBAC_MUTE_GF_TIME);

		ret = i2cHander.readByte(0x6C, 0x06, nState);
		if(ret < 0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "I2c read byte err: %d\n", ret)
			return -1;
		}
        while( (nState != 0x0F)  && (nCount-- > 0) )
		{
			usleep(IBAC_MUTE_GF_TIME);
			ret = i2cHander.readByte(0x6C, 0x06, nState);
			if(ret < 0)
			{
				IBD_PRINTF(DEBUG_INFOMATION, "I2c read byte err: %d\n", ret)
				return -1;
			}		
		}
        return 0;
}

int AudioControlC::unmuteGF()
{
/*      int file;
        file = i2copen(0);
        if( file == 0 )
                return -1;

        i2cset(file,0x6c,0x0c,0x00,'b');
        usleep(IBAC_MUTE_GF_TIME);//µÈ 35 ms

        int nState = i2cget(file,0x6c,0x06,'b');
        int nCount = 3;
        while( (nState != 0x0f)  && (nCount-- > 0) )
        {
                usleep(IBAC_MUTE_GF_TIME);//µÈ 35 ms
                nState = i2cget(file,0x6c,0x06,'b');
        }

        i2cclose(file);
*/
		int ret = -1;
		IBI2CControl i2cHander;
		unsigned char nState;
		int nCount = 3;
		if (i2cHander.checkOK() == 0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "Check i2cHander err!\n")
			return -1; 
		}
		ret = i2cHander.writeByte(0x6C, 0x0C, 0x00);
		if(ret < 0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "I2c write byte err: %d\n", ret)
			return -1;
		}
        usleep(IBAC_MUTE_GF_TIME);

		ret = i2cHander.readByte(0x6C, 0x06, nState);
		if(ret < 0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "I2c read byte err: %d\n", ret)
			return -1;
		}
        while( (nState != 0x0F)  && (nCount-- > 0) )
		{
			usleep(IBAC_MUTE_GF_TIME);
			ret = i2cHander.readByte(0x6C, 0x06, nState);
			if(ret < 0)
			{
				IBD_PRINTF(DEBUG_INFOMATION, "I2c read byte err: %d\n", ret)
				return -1;
			}		
		}
        return 0;
}      

int AudioControlC::setGFStatus()
{
	int needGF=0;
	for(int i = 0; (i < IBAC_DEV_ID_MAX) && (needGF==0); i++) 
	{
		switch(i+1)
		{
   			case IBAC_DEV_ID_MAIN_VOLUME:
                	case IBAC_DEV_ID_DAI:
                	case IBAC_DEV_ID_BYPASS_MX:
			case IBAC_DEV_ID_MIC:
#ifdef IBAC_VOLUME_MAIN_OFFSET
			case IBAC_DEV_ID_MAIN_VOLUME_OFFSET:
#endif			
				break;
			default:
				if(((audioControlArr[i].soundStatus & IBAC_STATUS_OPEN)) 
						&& (audioControlArr[i].soundStatus & IBAC_STATUS_MUTE)==0)
				{
					needGF =1;
					break;
				}
		}
	}

	if(needGF)
		unmuteGF();
	else
		muteGF();

	return 0;
}

// reset the audio control array
void AudioControlC::resetAudioArr()
{
	// initialize the audio info array
	for (int i = 0; i < IBAC_DEV_ID_MAX; i++)
	{
		switch (audioControlArr[i].id.logicId)
		{
			case IBAC_DEV_ID_MAIN_VOLUME:
			case IBAC_DEV_ID_BYPASS_MX:
			    audioControlArr[i].otherStatus &= 0x00; // close speed sound.
#ifndef _8925_VERSION
				audioControlArr[i].volume = IBAC_INIT_VOLUME_MAIN;
#else
                audioControlArr[i].volume = 23;
#endif
				audioControlArr[i].soundStatus &= 0xf8;
				audioControlArr[i].soundStatus |= 0x04;//set to always.
				audioControlArr[i].soundStatus &= 0xef;//set to unmute.
				break;
			case IBAC_DEV_ID_DAI:
    			audioControlArr[i].otherStatus &= 0x00; // close speed sound.
				audioControlArr[i].volume = IBAC_INIT_VOLUME_DAI;
				audioControlArr[i].soundStatus &= 0xf8;				
				audioControlArr[i].soundStatus |= 0x04;//set to always.
				audioControlArr[i].soundStatus &= 0xef;//set to unmute.				
				break;
			case IBAC_DEV_ID_CD_DVD:
				audioControlArr[i].otherStatus &= 0x00; // close speed sound.
#ifndef _8925_VERSION
				audioControlArr[i].volume = IBAC_INIT_VOLUME_MAIN;
#else
				audioControlArr[i].volume = IBAC_VOLUME_FULL_SCALE;
#endif
				audioControlArr[i].soundStatus &= 0xf8;				
				audioControlArr[i].soundStatus |= 0x01;//set to mono.
				audioControlArr[i].soundStatus &= 0xef;//set to unmute.
				break;
			case IBAC_DEV_ID_DAI_MP3:
			case IBAC_DEV_ID_DAI_VIDEO:
#ifdef _8925_VERSION
                audioControlArr[i].volume = IBAC_VOLUME_VIDEO_MAX;
                break;
#endif
			case IBAC_DEV_ID_DAI_NETR:
			case IBAC_DEV_ID_DAI_GPS:
			case IBAC_DEV_ID_DAI_NEWS:
			case IBAC_DEV_ID_DAI_SMS:
			case IBAC_DEV_ID_DAI_CARINFO:
			case IBAC_DEV_ID_DAI_LIFEINFO:
			case IBAC_DEV_ID_DAI_COMMAND:	
		        case IBAC_DEV_ID_DAI_ANNOUNCE:
		        case IBAC_DEV_ID_DAI_RING:
		        case IBAC_DEV_ID_DAI_ILLEGALTIPS:
                case IBAC_DEV_ID_DAI_SMSRING:
                case IBAC_DEV_ID_DAI_TONE:    
                case IBAC_DEV_ID_DAI_VTALKI:  
                case IBAC_DEV_ID_DAI_MONLINE: 
                case IBAC_DEV_ID_DAI_VONLINE: 
                case IBAC_DEV_ID_DAI_LBOOK:   
                case IBAC_DEV_ID_DAI_MTRAFFIC:
		        
			case IBAC_DEV_ID_DAI_VPHONE:
			case IBAC_DEV_ID_DAI_KEYTONE:					
                        case IBAC_DEV_ID_WALKI_TALKI:
            	audioControlArr[i].otherStatus &= 0x00; // close speed sound.
#ifdef _8925_VERSION
                                audioControlArr[i].volume = 30;
#else
                                audioControlArr[i].volume = IBAC_INIT_VOLUME_DAI_MIX;
#endif
				audioControlArr[i].soundStatus &= 0xf8;				
				audioControlArr[i].soundStatus |= 0x01;//set to mono.
				audioControlArr[i].soundStatus &= 0xef;//set to unmute.
				break;

			case IBAC_DEV_ID_RADIO:
#ifdef _8925_VERSION
                audioControlArr[i].otherStatus &= 0x00; // close speed sound.
				audioControlArr[i].volume = 255;
				audioControlArr[i].soundStatus &= 0xf8;				
				audioControlArr[i].soundStatus |= 0x01;//set to mono.
				audioControlArr[i].soundStatus &= 0xef;//set to unmute.	
				break;
#endif
			case IBAC_DEV_ID_TV:
			case IBAC_DEV_ID_3G_PHONE_SPEAKER:
			case IBAC_DEV_ID_MIC:
			case IBAC_DEV_ID_BOLUETOOTH:
				audioControlArr[i].otherStatus &= 0x00; // close speed sound.
				audioControlArr[i].volume = IBAC_INIT_VOLUME_ANALOG;
				audioControlArr[i].soundStatus &= 0xf8;				
				audioControlArr[i].soundStatus |= 0x01;//set to mono.
				audioControlArr[i].soundStatus &= 0xef;//set to unmute.				
				break;
#ifdef IBAC_VOLUME_MAIN_OFFSET
            case IBAC_DEV_ID_MAIN_VOLUME_OFFSET:
            	audioControlArr[i].otherStatus &= 0x00; // close speed sound.
				audioControlArr[i].volume = 0;
				audioControlArr[i].soundStatus &= 0xf8;
				audioControlArr[i].soundStatus |= 0x04;//set to always.
				audioControlArr[i].soundStatus &= 0xef;//set to unmute.
            break;
#endif				
			default:
#ifdef _8925_VERSION
                audioControlArr[i].volume = 30;
#endif
				break;
		}
	}
}
void AudioControlC::printDevStatInfo(unsigned char stat)
{
	if ((stat & 0x80) == 0) { 		//bit7=0, status is closeSR.
//		IBD_PRINTF(4, " CloseSR ");
	} else {	 				//bit7=1, status is openSR.
//		IBD_PRINTF(4, " OpenSR ");
	}						
	if ((stat & 0x40) == 0) { 		//bit6=0, status is close.
//		IBD_PRINTF(4, " close ");
	} else {	 				//bit6=1, status is open.
//		IBD_PRINTF(4, " open ");
	}
	if ((stat & 0x20) == 0) { 		//bit5=0, status is resume.
//		IBD_PRINTF(4, "resume ");
	} else { 					//bit5=1, status is pause.
//		IBD_PRINTF(4, "pause ");
	}
	if ((stat & 0x10) == 0) { 		//bit4=0, status is unmute.
//		IBD_PRINTF(4, "unmute ");
	} else { 					//bit4=1, status is mute.
//		IBD_PRINTF(4, "mute ");
	}
	if ((stat & 0x04) != 0) { 		//bit2=1, status is always.
//		IBD_PRINTF(4, "always ");
	}
	if ((stat & 0x02) != 0) { 		//bit1=1, status is mix.
//		IBD_PRINTF(4, "mix ");
	}
	if ((stat & 0x01) != 0) { 		//bit0=1, status is mono.
//		IBD_PRINTF(4, "mono ");
	}
	IBD_PRINTF(4, "\n");
}

#ifdef IBAC_VOLUME_MAIN_OFFSET

#define IBAC_HEADPHONE_VOLUME_LEFT 20
#define IBAC_HEADPHONE_VOLUME_RIGHT 21

int AudioControlC::setMainVolLeft(int offset)
{
    int mainVol, leftVol, conLeftVol;
    mainVol = audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume;
    leftVol = mainVol-offset*mainVol/50;
	conLeftVol = convertVol(leftVol, IBAC_DEV_ID_MAIN_VOLUME);// convert to the real vol value
	
    if((csetAudio(IBAC_HEADPHONE_VOLUME_LEFT, NULL, conLeftVol)) < 0) 
    {
        IBD_PRINTF(4, "change device --> %d error in volume set\n",IBAC_HEADPHONE_VOLUME_LEFT);
        return -1;
    }
    printf("[setMainVolLeft] mainVol= %d; leftVol= %d; conLeftVol= %d\n", \
                                mainVol, leftVol, conLeftVol);
    /*printf("--------------------\n");
    system("amixer cget numid=1");
    system("amixer cget numid=20");
    system("amixer cget numid=21");
    system("amixer cget numid=22");
    printf("--------------------\n");*/
    return 0;
}
int AudioControlC::setMainVolRight(int offset)
{
    int mainVol, rightVol, conRightVol;
    mainVol = audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume;
    rightVol = mainVol-offset*mainVol/50;
	conRightVol = convertVol(rightVol, IBAC_DEV_ID_MAIN_VOLUME);// convert to the real vol value
	
    if((csetAudio(IBAC_HEADPHONE_VOLUME_RIGHT, NULL, conRightVol)) < 0) 
    {
        IBD_PRINTF(4, "change device --> %d error in volume set\n",IBAC_HEADPHONE_VOLUME_RIGHT);
        return -1;
    }
    printf("[setMainVolRight] mainVol= %d; rightVol= %d; conRightVol= %d\n", \
                                mainVol, rightVol, conRightVol);
/*    printf("--------------------\n");
    system("amixer cget numid=1");
    system("amixer cget numid=20");
    system("amixer cget numid=21");
    system("amixer cget numid=22");
    printf("--------------------\n");*/
    return 0;
}
int AudioControlC::initMainVolOffset()
{
    int absOffset;
    int offset = audioControlArr[IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1].volume;
    if(offset == 0)
    {
        printf("[initMainVolOffset] audioControlArr[IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1].volume=0\n");
        return 0;
    }
    absOffset = abs(offset);
    printf("[initMainVolOffset] offset= %d; absOffset= %d\n", offset, absOffset);
    if(offset < 0)
    {
        setMainVolRight(absOffset);
    }
    else
    {
        setMainVolLeft(absOffset);
    }
    return 0;
}
int AudioControlC::setMainVolOffset(int offset)
{
    InternalLock	lock(&m_lock);
    
    int oldOffset = audioControlArr[IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1].volume;
    printf("[setMainVolOffset] oldOffset = %d; offset = %d\n", oldOffset, offset);
    if(offset<-IBAC_VOLUME_MAIN_OFFSET_PERCENT_MAX || offset>IBAC_VOLUME_MAIN_OFFSET_PERCENT_MAX)
    {
        printf("offset is invalid: %d\n", offset);
        return -1;
    }
    if(oldOffset == offset)
    {
        printf("oldOffset = offset\n");
        return 0;
    }
    changeHardVolume(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1], offset);

    listener->audioStatusUpdate(IBAC_DEV_ID_MAIN_VOLUME_OFFSET, 
	    audioControlArr[IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1].volume, 
		audioControlArr[IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1].on_off,
		audioControlArr[IBAC_DEV_ID_MAIN_VOLUME_OFFSET-1].soundStatus);
	m_autoSaveMonitor.valueChanged();
	
    return 0;
}
#endif

int AudioControlC::getAudioExt(unsigned int id, int *vol, int* on_off, unsigned char *soundStatus, unsigned char *otherStatus)
{
	InternalLock	lock(&m_lock);

	if (id <= 0 || id > IBAC_DEV_ID_MAX)
	{
		IBD_PRINTF(4, "\n[Audio Control]: Get Audio --> Wrong control identifier: %d\n", id);
		return -1;
	}
    *vol = CONVERT_VOLAPP(audioControlArr[id-1].volume);
	*on_off = audioControlArr[id-1].on_off;
	*soundStatus = audioControlArr[id-1].soundStatus;
	*otherStatus = audioControlArr[id-1].otherStatus;

	return 0;	
}
int AudioControlC::openSetAudioVolAccToSpeed()
{
    unsigned char tmpOtherStatus;
    tmpOtherStatus = (audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].otherStatus & IBAC_OTHERSTATUS_SPEEDSOUND);
    if(IBAC_OTHERSTATUS_SPEEDSOUND == tmpOtherStatus)
    {
        printf("openSetAudioVolAccToSpeed is opened!\n");
        return 0;
    }
    audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].otherStatus |= IBAC_OTHERSTATUS_SPEEDSOUND;
	m_autoSaveMonitor.valueChanged();
    return 0;
}
int AudioControlC::closeSetAudioVolAccToSpeed()
{
    unsigned char tmpOtherStatus;

    m_deltaVol = 0;
    m_carSpeed = 0;

    tmpOtherStatus = (audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].otherStatus & IBAC_OTHERSTATUS_SPEEDSOUND);
    if(IBAC_OTHERSTATUS_SPEEDSOUND != tmpOtherStatus)
    {
        printf("closeSetAudioVolAccToSpeed is closed!\n");
        return 0;
    }
    audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].otherStatus &= ~IBAC_OTHERSTATUS_SPEEDSOUND;
    
    if(((audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].soundStatus & IBAC_STATUS_MUTE) == 0) 
    && (audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume < IBAC_SPEED_VOLMAX))
    {
        changeHardVolume(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1], audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume);
        //printf("set the speed vol is %d\n", vol);
    }
	m_autoSaveMonitor.valueChanged();    
    return 0;
}

int AudioControlC::setAudioVolAccToSpeed(int carSpeed)
{
    if((audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].otherStatus & IBAC_OTHERSTATUS_SPEEDSOUND) == 0)
    {
        m_deltaVol = 0;
        m_carSpeed = 0;
        return 0;
    }
    
    int val = 0;
    int vol = 0;
    if(carSpeed < (IBAC_SPEED_MIN + IBAC_SPEED_STEP))
    {
        vol = audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume + 0;
        m_deltaVol = 0;
    }
    else
    {
        val = (carSpeed-IBAC_SPEED_MIN)/IBAC_SPEED_STEP;
        vol = audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume + val;
        m_deltaVol = val;
        if(vol >= IBAC_SPEED_VOLMAX)
        {
            vol = IBAC_SPEED_VOLMAX;
        }
    }
    if ( 0 == audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume )
    {
        vol = 0;
    }

    if(((audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].soundStatus & IBAC_STATUS_MUTE) == 0) 
        && (audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume < IBAC_SPEED_VOLMAX))
    {
        
        changeHardVolume(&audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1], vol);        
        //printf("set the speed vol is %d\n", vol);
    }
    m_carSpeed = carSpeed;
	printf("*** [setAudioVolAccToSpeed] carspeed = %d; MAIN_VOLUME = %d; action vol = %d ***\n", \
			carSpeed, audioControlArr[IBAC_DEV_ID_MAIN_VOLUME-1].volume, vol);
    return 0;
}
#if 0
void AudioControlC::printDevCtlInfo(unsigned int /*id*/)
{
#ifdef IBAC_AUDIO_CONFIG_LOG	
	IBD_PRINTF(4,"\n ------------------ audio device #%d info in audio controller array -----------------\n", id);
	IBD_PRINTF(4," logic id = %d, numid_vol = %d, numid_swich = %d \n", \
		audioControlArr[id-1].id.logicId, audioControlArr[id-1].id.numid_vol, audioControlArr[id-1].id.numid_switch);
	IBD_PRINTF(4," switch = %d, volume = %d, soundStatus = 0x%2x  ", \
			audioControlArr[id-1].on_off, audioControlArr[id-1].volume, audioControlArr[id-1].soundStatus);
	printDevStatInfo(audioControlArr[id-1].soundStatus);
	IBD_PRINTF(4,"----------------------------------- END --------------------------------------------\n");
#endif
}
#endif
// internal lock
AudioControlC::InternalLock::InternalLock(pthread_mutex_t* lock)
{
	m_lock=lock;
	pthread_mutex_lock(m_lock);
}
AudioControlC::InternalLock::~InternalLock()
{
	pthread_mutex_unlock(m_lock);
}

AutoSaveConfFile::AutoSaveConfFile(AudioControlC *pSAudio)
:m_pSAudio(pSAudio)
{
	m_valueChangedFlag =0;
	m_quitFlag		    =0;

	pthread_cond_init(&m_cond, NULL);
	
	//thread lock
	pthread_mutexattr_t     mutexattr;
        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_lock,&mutexattr);

	pthread_create(&m_threadId, NULL, saveThread, this);

}

AutoSaveConfFile::~AutoSaveConfFile()
{
	pthread_mutex_lock(&m_lock);

	m_quitFlag   =1;
	pthread_cond_signal(&m_cond);

	pthread_mutex_unlock(&m_lock);

	pthread_join(m_threadId, NULL);

	pthread_cond_destroy(&m_cond);
	
        pthread_mutex_destroy(&m_lock);
}

int AutoSaveConfFile::valueChanged()
{
	pthread_mutex_lock(&m_lock);
	
	m_valueChangedFlag = 1;
	pthread_cond_signal(&m_cond);

	pthread_mutex_unlock(&m_lock);
	return 0;
}

void *AutoSaveConfFile::saveThread(void *arg)
{
	AutoSaveConfFile* monitor = (AutoSaveConfFile*)arg;
	int ret = 0;
	while (1) 
	{
		pthread_mutex_lock(&monitor->m_lock);
		
		struct timespec  to;
		to.tv_sec = time(NULL) + 30;
		to.tv_nsec = 0;	

		if(monitor->m_valueChangedFlag)
			ret = pthread_cond_timedwait(&monitor->m_cond, &monitor->m_lock, &to);
		else
			ret = pthread_cond_wait(&monitor->m_cond, &monitor->m_lock);
		
		if (monitor->m_valueChangedFlag  && ((ret == ETIMEDOUT)  || monitor->m_quitFlag))
		{
			monitor->m_pSAudio->saveAudio();
			monitor->m_valueChangedFlag = 0;
		}

		if(monitor->m_quitFlag)
		{
			pthread_mutex_unlock(&monitor->m_lock);
			break;
		}

		pthread_mutex_unlock(&monitor->m_lock);
	}
	pthread_exit(NULL);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Callback Class										  //	
////////////////////////////////////////////////////////////////////////////////////////////////////
// class AudioCtrlCallbackC
AudioCtrlCallbackC::AudioCtrlCallbackC()
{
	// initialize the audio client notify handler array
	pHandlerLinkHeader = NULL;
	pHandlerLinkTail = NULL;
	nHandler = 0;

	//thread lock
	pthread_mutexattr_t     mutexattr;

        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_lock,&mutexattr);

}

AudioCtrlCallbackC::~AudioCtrlCallbackC()
{
	AudioCtrlCBHandler_t *pC = pHandlerLinkHeader;
	AudioCtrlCBHandler_t *pN = pC ? pC->pNext : NULL;
	while (pC)
	{
		free(pC);
		pC = pN;
		pN = pC->pNext;
	}
	pHandlerLinkHeader = NULL;
	pHandlerLinkTail = NULL;
	nHandler = 0;
}

int AudioCtrlCallbackC::registerHandler(unsigned int id, AudioCtrlCBHandlerC* pCBHandler)
{
	InternalLock	lock(&m_lock);

	AudioCtrlCBHandler_t *pC = pHandlerLinkHeader;
	
	if (pHandlerLinkHeader == NULL)			// null link list, create the header
	{
		pHandlerLinkHeader = (AudioCtrlCBHandler_t*)malloc(sizeof(AudioCtrlCBHandler_t));
		pHandlerLinkHeader->id = id;
		pHandlerLinkHeader->pHandle = pCBHandler;
		pHandlerLinkHeader->pNext = NULL;
		pHandlerLinkTail = pHandlerLinkHeader;
		nHandler++;
		return 0;
	}
	else
	{
		// first check if this handler is already registered
		if ((pC = serarchHandlerLink(id, pCBHandler)) != NULL)	// there is, do nothing, & return
			return 0;
		else						// none, create new node
		{
			if ((pHandlerLinkTail->pNext = (AudioCtrlCBHandler_t*)malloc(sizeof(AudioCtrlCBHandler_t))) == NULL)
				return -1;
			pHandlerLinkTail = pHandlerLinkTail->pNext;
			pHandlerLinkTail->id = id;
			pHandlerLinkTail->pHandle = pCBHandler;
			pHandlerLinkTail->pNext = NULL;
			nHandler++;
		}
	}
	
	return 0;
}

int AudioCtrlCallbackC::deRegisterHandler(unsigned int id, AudioCtrlCBHandlerC* pCBHandler)
{
	InternalLock	lock(&m_lock);

	AudioCtrlCBHandler_t *pC = pHandlerLinkHeader;
	AudioCtrlCBHandler_t *pN = pC ? pC->pNext : NULL;

	if(pHandlerLinkHeader == NULL)
			return 0;
	
	if(pC->id == id && pC->pHandle == pCBHandler)
	{
		pHandlerLinkHeader = pHandlerLinkHeader->pNext;
		if(pHandlerLinkHeader == NULL)
		{
			pHandlerLinkTail = NULL;	
		}
	}
	else
	{
		while(pN)
		{
			if(pN->id == id && pN->pHandle == pCBHandler)
			{
				pC->pNext = pN->pNext;

				if(pN == pHandlerLinkTail)
					pHandlerLinkTail = pC; 
				break;
			}

			pC = pN;
			pN = pN->pNext;
		}
		pC = pN;
		
	}
	if(pC != NULL)
	{
		free(pC);
	
		nHandler--;
	}

	return 0;
}

int AudioCtrlCallbackC::callHandlder(unsigned int id, int vol, int sw_on_off, unsigned char sStat)
{
	int ret = -1;

	AudioCtrlCBHandler_t* pHandler = pHandlerLinkHeader;

	if (!pHandler)	// link head is NULL, no notify handler object registered
	{
#ifdef IBAC_AUDIO_CONFIG_LOG
		IBD_PRINTF(4, "No Notify handler callback object in client !!!\n");
#endif
	}
	else
	{
		while (pHandler)
		{
			if (pHandler->id == id)	// handler for this device
			{
				pHandler->pHandle->notifyHandle(vol, sw_on_off, sStat);
				ret = 0;
#ifdef IBAC_AUDIO_CONFIG_LOG
				IBD_PRINTF(4, "Dev #%3d Notify handler callback object called !!!\n", pHandler->id);
#endif
			}
			pHandler = pHandler->pNext;
		}

	}
	return ret;

}

// search the link to locate the handler along with the device id
// return the pointer pointing to preceding the node if number of nodes larger than 1, else return the head
AudioCtrlCBHandler_t* AudioCtrlCallbackC::serarchHandlerLink(unsigned int id, AudioCtrlCBHandlerC* pCBHandler)
{
	InternalLock	lock(&m_lock);

	AudioCtrlCBHandler_t *pC = pHandlerLinkHeader;
	AudioCtrlCBHandler_t *pN = pC ? pC->pNext : NULL;
	
	if (!pC)	// empty link, return NULL
		return NULL;
		
	// if only header exist, return the pointer to head if it is just the handler
	if (pHandlerLinkHeader->id == id && pHandlerLinkHeader->pHandle	== pCBHandler)
		return pHandlerLinkHeader;

	// search the other nodes
	while(pN)
	{
		if (pN->id == id && pN->pHandle	== pCBHandler)
			return pN;
		else		
		{
			pC = pN;
			pN = pC->pNext;
		}
	}
	return NULL;
}

// internal lock
AudioCtrlCallbackC::InternalLock::InternalLock(pthread_mutex_t* lock)
{
	m_lock=lock;
	pthread_mutex_lock(m_lock);
}
AudioCtrlCallbackC::InternalLock::~InternalLock()
{
	pthread_mutex_unlock(m_lock);
}
