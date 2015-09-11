#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <alsa/asoundlib.h>
#include "audio_main_volume.h"

#include "dbusServer.h"
#include "dbusApplication.h"
DBusServerC *pServer = NULL;

#include "i2ccontrol.h"
#include <sys/mman.h>

#ifdef _8925_VERSION
#define GPA_BASE                        0x74200000
#else
#define GPA_BASE                        0xF0102000
#endif
#define GPA_DAT                         0x00
#define GPA_EN                          0x04
#define OFFSET_FUN0                     0x24
#define OFFSET_FUN1                     0x28
#define OFFSET_FUN2                     0x2c
#define OFFSET_FUN3                     0x30


char *gpa_vbase;
int fd = -1;

int headinit();
int headclose();

int headclose()
{
        close(fd);
        munmap(gpa_vbase, OFFSET_FUN3);
        return 0;
}
int headinit()
{

        fd = open("/dev/mem",O_RDWR|O_SYNC);
        if(fd < 0){
                fprintf(stderr,"mem:open()\n");
                return -1;
        }
        gpa_vbase = (char *) mmap( NULL, OFFSET_FUN3,  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, fd, GPA_BASE);
        if(gpa_vbase == NULL || gpa_vbase ==  MAP_FAILED ){
                fprintf(stderr,"mmap:map file error \n");
                return -1;
        }
#ifdef _8925_VERSION
        gpa_vbase += 0x100;
        *(volatile unsigned int*)(gpa_vbase + OFFSET_FUN2) &= ~(0xf);
        *(volatile unsigned int*)(gpa_vbase + GPA_EN) &= ~(0x1 << 16);
#else
        *(volatile unsigned int*)(gpa_vbase + OFFSET_FUN1) &= ~(0xf << 12);
        *(volatile unsigned int*)(gpa_vbase + GPA_EN) &= ~(0x1 << 11);
#endif


        return 0;
}




#define check_range(val, min, max) \
        ((val < min) ? (min) : (val > max) ? (max) : (val)) 
#define convert_prange1(val, min, max) \
        ceil((val) * ((max) - (min)) / (IBAC_VOLUME_FULL_SCALE) + (min))

devIdStruct_t Main_Volume::devLogicId2CidSet[IBAC_DEV_ID_MAX]=
{
        // logic id, numid_volume, numid_switch,name
#ifdef _8925_VERSION
	{IBAC_DEV_ID_SPEAKER_PLAYBACK_VOLUME    ,30     ,0      ,"Speaker Playback Volume"},                    //0~31 (1)
	{IBAC_DEV_ID_SPEAKER_PLAYBACK_SWITCH    ,1      ,2      ,"Speaker Playback Switch"},                    //on (2)
	{IBAC_DEV_ID_SPEAKER_AMP                ,1      ,0      ,"Speaker Amp Type"},                           //   (3)
	{IBAC_DEV_ID_MAIN_VOLUME                ,0      ,0      ,"Headphone Playback Volume"},                  //0~31 (4)
	{IBAC_DEV_ID_MAIN_SWITCH                ,1      ,5      ,"Headphone Playback Switch"},                  //on   (5)
	{IBAC_DEV_ID_AXO_PLAYBACK_VOLUME        ,31     ,0      ,"AXO Playback Volume"},                        //0~31 (6)
	{IBAC_DEV_ID_AXO_PLAYBACK_SWITCH        ,0      ,7      ,"AXO Playback Switch"},                        //off  (7)
	{IBAC_DEV_ID_DAI                        ,30     ,0      ,"PCM Playback Volume"},                        //0~31 (8)
	{IBAC_DEV_ID_RADIO_VOLUME               ,30     ,0      ,"Line In Volume"},                             //0~31 (9)
	{IBAC_DEV_ID_MICIN1                     ,1      ,0      ,"Mic 1 Volume"},                               //0~31 (10)
	{IBAC_DEV_ID_MICIN1_BOOST               ,0      ,0      ,"Mic 1 Boost"},                                //0~3  (11)
	{IBAC_DEV_ID_3G_SPK_VOLUME              ,31     ,0      ,"Mic 2 Volume"},                               //0~31 (12)
	{IBAC_DEV_ID_MICIN2                     ,1      ,0      ,"MIC2 Boost"},                                 //0~3  (13)
	{IBAC_DEV_ID_AUX_VOLUME                 ,0      ,0      ,"AUX In Volume"},                              //0~31 (14)
	{IBAC_DEV_ID_CAPTURE_VOLUME             ,11     ,0      ,"Capture Volume"},                             //0~31 (15)
	{IBAC_DEV_ID_JFUNCTION                  ,1      ,0      ,"Jack Function"},                              // (16)
	{IBAC_DEV_ID_SFUNCTION                  ,1      ,0      ,"Speaker Function"},                           // (17)
	{IBAC_DEV_ID_AXO_VOL_IN_MUX             ,1      ,0      ,"AXO Vol Input Mux"},                          // (18)
	{IBAC_DEV_ID_HPR_VOL_IN_MUX             ,1      ,0      ,"HPR Vol Input Mux"},                          // (19)
	{IBAC_DEV_ID_HPL_VOL_IN_MUX             ,1      ,0      ,"HPL Vol Input Mux"},                          // (20)
	{IBAC_DEV_ID_SPK_VOL_IN_MUX             ,1      ,0      ,"SPK Vol Input Mux"},                          // (21)
	{IBAC_DEV_ID_SPK_RADIO_PLAYBACK_SWITCH  ,0      ,22     ,"SPK Mixer LineIn Playback Switch"},           //off (22)
	{IBAC_DEV_ID_SPK_AUXIN_PLAYBACK_SWITCH  ,0      ,23     ,"SPK Mixer AUXIN Playback Switch"},            //off (23)
	{IBAC_DEV_ID_SPK_MIC1_PLAYBACK_SWITCH   ,0      ,24     ,"SPK Mixer Mic1 Playback Switch"},             //off (24)
	{IBAC_DEV_ID_SPK_MIC2_PLAYBACK_SWITCH   ,0      ,25     ,"SPK Mixer Mic2 Playback Switch"},             //off (25)
	{IBAC_DEV_ID_SPK_DAI_PLAYBACK_SWITCH    ,1      ,26     ,"SPK Mixer PCM Playback Switch"},              //on (26)
	{IBAC_DEV_ID_RSTOHPMIXERRECORD_PB_SWIT  ,0      ,27     ,"Right STO HP Mixer RecordR Playback Switch"}, //off (27)
	{IBAC_DEV_ID_LSTOHPMIXERRECORD_PB_SWIT  ,0      ,28     ,"Left STO HP Mixer RecordL Playback Switch"},  //off (28)
	{IBAC_DEV_ID_RADIO_SWITCH               ,0      ,29     ,"Mono HP Mixer LineIn Playback Switch"},       //off (29)
	{IBAC_DEV_ID_HP_MIXER_AUXIN_PB_SWIT     ,0      ,30     ,"Mono HP Mixer AUXIN Playback Switch"},        //off (30)
	{IBAC_DEV_ID_HP_MIXER_MIC1_PB_SWIT      ,0      ,31     ,"Mono HP Mixer Mic1 Playback Switch"},         //off (31)
	{IBAC_DEV_ID_3G_SWITCH                  ,0      ,32     ,"Mono HP Mixer Mic2 Playback Switch"},         //off (32)
	{IBAC_DEV_ID_HP_MIXER_PCM_PB_SWIT       ,1      ,33     ,"Mono HP Mixer PCM Playback Switch"},          //on (33)
	{IBAC_DEV_ID_MIXER_LINEIN_PB_SWIT       ,0      ,34     ,"Mono Mixer LineIn Playback Switch"},          //off (34)
	{IBAC_DEV_ID_MIXER_MIC1_PB_SWIT         ,0      ,35     ,"Mono Mixer Mic1 Playback Switch"},            //off (35)
	{IBAC_DEV_ID_MIXER_MIC2_PB_SWIT         ,0      ,36     ,"Mono Mixer Mic2 Playback Switch"},            //off (36)
	{IBAC_DEV_ID_MIXER_AUXIN_PB_SWIT        ,0      ,37     ,"Mono Mixer AUXIN Playback Switch"},           //off (37)
	{IBAC_DEV_ID_MIXER_PCM_PB_SWIT          ,1      ,38     ,"Mono Mixer PCM Playback Switch"},             //on (38)
	{IBAC_DEV_ID_MIXER_RECORDL_PB_SWIT      ,0      ,39     ,"Mono Mixer RecordL Playback Switch"},         //off (39)
	{IBAC_DEV_ID_MIXER_RECORDR_PB_SWIT      ,0      ,40     ,"Mono Mixer RecordR Playback Switch"},         //off (40)
	{IBAC_DEV_ID_R_REC_M_MIC1_CAP_SWIT      ,1      ,41     ,"Right Record Mixer Mic1 Capture Switch"},     //on (41)
	{IBAC_DEV_ID_R_REC_M_MIC2_CAP_SWIT      ,1      ,42     ,"Right Record Mixer Mic2 Capture Switch"},     //on (42)
	{IBAC_DEV_ID_R_REC_M_LINEINR_CAP_SWIT   ,1      ,43     ,"Right Record Mixer LineInR Capture Switch"},  //on (43)
	{IBAC_DEV_ID_R_REC_M_AUXIN_CAP_SWIT     ,0      ,44     ,"Right Record Mixer AUXIN Capture Switch"},    //off (44)
	{IBAC_DEV_ID_R_REC_M_HPMR_CAP_SWIT      ,0      ,45     ,"Right Record Mixer HPMixerR Capture Switch"}, //off (45)
	{IBAC_DEV_ID_R_REC_M_SPKM_CAP_SWIT      ,0      ,46     ,"Right Record Mixer SPKMixer Capture Switch"}, //off (46)
	{IBAC_DEV_ID_R_REC_M_MM_CAP_SWIT        ,0      ,47     ,"Right Record Mixer MonoMixer Capture Switch"},//off (47)
	{IBAC_DEV_ID_L_REC_M_MIC1_CAP_SWIT      ,1      ,48     ,"Left Record Mixer Mic1 Capture Switch"},      //on (48)
	{IBAC_DEV_ID_L_REC_M_MIC2_CAP_SWIT      ,1      ,49     ,"Left Record Mixer Mic2 Capture Switch"},      //on (49)
	{IBAC_DEV_ID_L_REC_M_LINEINR_CAP_SWIT   ,1      ,50     ,"Left Record Mixer LineInL Capture Switch"},   //on (50)
	{IBAC_DEV_ID_L_REC_M_AUXIN_CAP_SWIT     ,0      ,51     ,"Left Record Mixer AUXIN Capture Switch"},     //off (51)
	{IBAC_DEV_ID_L_REC_M_HPML_CAP_SWIT      ,0      ,52     ,"Left Record Mixer HPMixerL Capture Switch"},  //off (52)
	{IBAC_DEV_ID_L_REC_M_SPKM_CAP_SWIT      ,0      ,53     ,"Left Record Mixer SPKMixer Capture Switch"},  //off (53)
	{IBAC_DEV_ID_L_REC_M_MM_CAP_SWIT        ,0      ,54     ,"Left Record Mixer MonoMixer Capture Switch"}, //off (54)
	{IBAC_DEV_ID_DAI_MP3                    ,0      ,0      ,"mp3vol"},                                     //0~255 (55)
	{IBAC_DEV_ID_DAI_VIDEO                  ,30     ,0      ,"videovol"},                                   //0~255 (56)
	{IBAC_DEV_ID_DAI_GPS                    ,30     ,0      ,"gpsvol"},                                     //0~255 (57) 
	{IBAC_DEV_ID_DAI_NEWS                   ,30     ,0      ,"newsvol"},                                    //0~255 (58)
	{IBAC_DEV_ID_DAI_SMS                    ,30     ,0      ,"smsvol"},                                     //0~255 (59)
	{IBAC_DEV_ID_DAI_RING                   ,30     ,0      ,"ringvol"},                                    //0~255 (60)
	{IBAC_DEV_ID_DAI_SMSRING                ,30     ,0      ,"smsringvol"},                                 //0~255 (61)
        {IBAC_DEV_ID_DAI_WARNING                ,30     ,0      ,"warningvol"},                                 //0~255 (62)

#else
        {IBAC_DEV_ID_MAIN_VOLUME, DEFAULT_MAIN_VOLUME, 0, "Headphone Playback Volume"},// 0~127
        {IBAC_DEV_ID_DAI, DEFAULT_MAIN_VOLUME, 0, "PCM"},                                                  // 0~127
        {IBAC_DEV_ID_DPB_VOLUME, 30, 0, "Digital Playback Volume"},        //255
        {IBAC_DEV_ID_DPB_SWITCH, 0, 4, "Digital Playback ZC Switch"},      //off
        {IBAC_DEV_ID_DEEP_SWITCH, 0, 5, "Deemphasis Switch"},              //off
        {IBAC_DEV_ID_MICIN, 26, 0, "Capture Volume"},                             //221
        {IBAC_DEV_ID_MICIN_SWITCH, 1, 7, "Capture Switch"},                  //on
        {IBAC_DEV_ID_CZC_SWITCH, 0, 8, "Capture ZC Switch"},               //off     
        {IBAC_DEV_ID_CHPF_SWITCH, 1, 9, "Capture HPF Switch"},             //on
        {IBAC_DEV_ID_COUTDAC_SWITCH, 1, 10, "Output Mixer DAC Switch"},//cpu out on~off
        {IBAC_DEV_ID_COUTAUX_SWITCH, 0, 11, "Output Mixer AUX Switch"},//off  
        {IBAC_DEV_ID_COUTBPS_SWITCH, 1, 12, "Output Mixer Bypass Switch"},//record-off  radio,3g-on
        {IBAC_DEV_ID_RADIO_SWITCH, 0, 13, "Input Mixer AIN1 Switch"},//radioin on~off
        {IBAC_DEV_ID_TV_SWITCH, 0, 14, "Input Mixer AIN2 Switch"},     //off
        {IBAC_DEV_ID_3G_SWITCH, 0, 15, "Input Mixer AIN3 Switch"},     // 3gin on~off
        {IBAC_DEV_ID_VOICE_SWITCH, 0, 16, "Input Mixer AIN4 Switch"},//off
        {IBAC_DEV_ID_BOLUETOOTH_SWITCH, 0, 17, "Input Mixer AIN5 Switch"},//off
        {IBAC_DEV_ID_JFUNCTION_SWITCH, 0, 18, "Jack Function"},// 0
        {IBAC_DEV_ID_SFUNCTION_SWITCH, 0, 19, "Speaker Function"},// 0
        {IBAC_DEV_ID_DAI_MP3, 30, 0, "mp3vol"},		//plug:mp3...
        {IBAC_DEV_ID_DAI_VIDEO, 30, 0, "videovol"},
        {IBAC_DEV_ID_DAI_GPS, 30, 0, "gpsvol"},
        {IBAC_DEV_ID_DAI_NEWS, 30, 0, "newsvol"},
        {IBAC_DEV_ID_DAI_SMS, 30, 0, "smsvol"},
        {IBAC_DEV_ID_DAI_RING, 30, 0, "ringvol"},
        {IBAC_DEV_ID_DAI_SMSRING, 30, 0, "smsringvol"},
        {IBAC_DEV_ID_DAI_WARNING, 30, 0, "warningvol"}
#endif
};

Main_Volume *Main_Volume::thisObject=NULL;
Main_Volume *Main_Volume::object()
{
        if(thisObject==NULL)
                thisObject = new Main_Volume();
        return thisObject;
}

Main_Volume::Main_Volume()
    :mutestate(true),
    volumenum(25),
    modestate(INIT_STATE)
{
        headinit();
        memset(channelMuteState, true, sizeof(channelMuteState));
        memset(channelVolumeValStored, 0 ,sizeof(channelVolumeValStored));
        resetAudioArr();
        if(checkInit() != IBAC_DEV_ID_MAX)
        {
                initAlsaPlug();
                initAudio();
        }
        else
        {
                getCurrentAlsaData();
        }
        //thread lock
        pthread_mutexattr_t     mutexattr;
        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_lock,&mutexattr);
        loadConf();
        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
               switch(audioControlArr[i].id.logicId)
               {
                        case IBAC_DEV_ID_MAIN_VOLUME:
                        case IBAC_DEV_ID_DAI:
#ifdef _8925_VERSION
                        case IBAC_DEV_ID_RADIO_VOLUME:
                        case IBAC_DEV_ID_3G_SPK_VOLUME:
#endif
                                channelVolumeValStored[i] = volumenum;
                                break;
                        case IBAC_DEV_ID_DAI_WARNING:
                                channelMuteState[i] = true;
                                break;
                        case IBAC_DEV_ID_DAI_RING:
                        case IBAC_DEV_ID_3G_SWITCH:
                                channelMuteState[i] = true;
                                break;
                        default:
                                break;
                }
        }
        if(mutestate == false)
                set_mute();
        else
        {
                for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
                {
                        switch(audioControlArr[i].id.logicId)
                        {
                                case IBAC_DEV_ID_MAIN_VOLUME:
                                case IBAC_DEV_ID_DAI:
#ifdef _8925_VERSION
                                case IBAC_DEV_ID_RADIO_VOLUME:
                                case IBAC_DEV_ID_3G_SPK_VOLUME:
#endif
                                        audioControlArr[i].volume = volumenum;
                                        channelVolumeValStored[i] = audioControlArr[i].volume;
                                        changeHardVolume(&audioControlArr[i],volumenum);
                                        break;
                                default:
                                        break;
                        }
                }
        }
}
Main_Volume::~Main_Volume()
{
        headclose();
}

int Main_Volume::checkInit()
{
        int err = -1;
        char card[20];
        snd_ctl_t *handle = NULL;
        snd_ctl_elem_list_t *clist = NULL;
        snd_ctl_elem_list_alloca(&clist);
        strcpy(card, "default");
        if (handle == NULL && (err = snd_ctl_open(&handle, card, 0)) < 0) // open the control 
        {		
                printf("[audioManager]: csetAudio() Set Audio --> Control %s open error: %s\n", card, snd_strerror(err));
                return err;
        }
        snd_ctl_elem_list(handle,clist);
        int count = snd_ctl_elem_list_get_count(clist);
        printf("[audioManager]: checkInit() count: %d \n", count);
        snd_ctl_close(handle); 
        return count;
}

int Main_Volume::initAlsaPlug()
{   
        const char *plugName[ALSA_PLUG_NAME_MAX] =  
        {       
                "plug:mp3",  //IBAC_DEV_ID_DAI_MP3      
                "plug:video",  //IBAC_DEV_ID_DAI_VIDEO             
                "plug:gps",  //IBAC_DEV_ID_DAI_GPS      
                "plug:news",  //IBAC_DEV_ID_DAI_NEWS        
                "plug:sms", //IBAC_DEV_ID_DAI_SMS              
                "plug:ring",  //IBAC_DEV_ID_DAI_RING             
                "plug:smsring",     //IBAC_DEV_ID_DAI_SMSRING      
                "plug:warning"        //IBAC_DEV_ID_DAI_WARNING       
        }; 
        int i = 0;  
        int err = 0;    
        for (i = 0; i<ALSA_PLUG_NAME_MAX; i++)  
        {       
                snd_pcm_t* handle;      
                if ( (err = snd_pcm_open(&handle, plugName[i], SND_PCM_STREAM_PLAYBACK, 0)) < 0)        
                {               
                        printf("[audioManager]: initAlsaPlug() snd_pcm_open %s failed!\n", plugName[i]);    
                }
                else
                {
                        snd_pcm_drain(handle);      
                        snd_pcm_close(handle);  
                }
        }   
        return 0;
}

void Main_Volume::getCurrentAlsaData()
{
        int ret;
        // get the current data of alsa
        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                switch(audioControlArr[i].id.logicId)
                {
                        case IBAC_DEV_ID_MAIN_VOLUME:
                                ret = cgetAudio(audioControlArr[i].id.logicId,audioControlArr[i].id.name,audioControlArr[i].id.numid_switch);
                                audioControlArr[i].volume = ret;
                                channelVolumeValStored[i] = audioControlArr[i].volume;
                                audioControlArr[i].on_off = -1;
#ifdef _8925_VERSION
                                volumenum = audioControlArr[i].volume;
                                for(int j = 0; j < IBAC_DEV_ID_MAX; j++)
                                {
                                        if(IBAC_DEV_ID_3G_SPK_VOLUME == audioControlArr[j].id.logicId)
                                        {
                                                channelVolumeValStored[j] = volumenum;
                                                break;
                                        }
                                }
#else
                                volumenum = mainvolFactToScale(audioControlArr[i].volume);
#endif
                                break;
                        case IBAC_DEV_ID_DAI:
#ifdef _8925_VERSION
                        case IBAC_DEV_ID_RADIO_VOLUME:
                        case IBAC_DEV_ID_MICIN1:
                        case IBAC_DEV_ID_MICIN1_BOOST:
                        case IBAC_DEV_ID_3G_SPK_VOLUME:
#else
                        case IBAC_DEV_ID_MICIN:
#endif
                        case IBAC_DEV_ID_DAI_MP3:
                        case IBAC_DEV_ID_DAI_VIDEO:
                        case IBAC_DEV_ID_DAI_GPS:
                        case IBAC_DEV_ID_DAI_NEWS:
                        case IBAC_DEV_ID_DAI_SMS:
                        case IBAC_DEV_ID_DAI_RING:
                        case IBAC_DEV_ID_DAI_SMSRING:
                        case IBAC_DEV_ID_DAI_WARNING:
                                ret = cgetAudio(audioControlArr[i].id.logicId,audioControlArr[i].id.name,audioControlArr[i].id.numid_switch);
                                audioControlArr[i].volume = ret;
                                channelVolumeValStored[i] = audioControlArr[i].volume;
                                audioControlArr[i].on_off = -1;
                                break;
                        //case IBAC_DEV_ID_COUTDAC_SWITCH:
#ifdef _8925_VERSION
                        case IBAC_DEV_ID_MAIN_SWITCH:
                        case IBAC_DEV_ID_HP_MIXER_PCM_PB_SWIT:
                        case IBAC_DEV_ID_HP_MIXER_AUXIN_PB_SWIT:
                        case IBAC_DEV_ID_HP_MIXER_MIC1_PB_SWIT:
                        case IBAC_DEV_ID_R_REC_M_MIC1_CAP_SWIT:
                        case IBAC_DEV_ID_R_REC_M_MIC2_CAP_SWIT:
                        case IBAC_DEV_ID_R_REC_M_LINEINR_CAP_SWIT:
                        case IBAC_DEV_ID_L_REC_M_MIC1_CAP_SWIT:
                        case IBAC_DEV_ID_L_REC_M_MIC2_CAP_SWIT:
                        case IBAC_DEV_ID_L_REC_M_LINEINR_CAP_SWIT:
#else
                        case IBAC_DEV_ID_COUTBPS_SWITCH:
                        case IBAC_DEV_ID_RADIO_SWITCH:
#endif
                        case IBAC_DEV_ID_3G_SWITCH:
                                ret = cgetAudio(audioControlArr[i].id.logicId,audioControlArr[i].id.name,audioControlArr[i].id.numid_switch);
                                audioControlArr[i].on_off = ret;
                                audioControlArr[i].volume = -1;
                                channelVolumeValStored[i] = audioControlArr[i].volume;
                                break;
                        default:
                                break;
                }
        } 
#ifndef _8925_VERSION
        if((audioControlArr[IBAC_DEV_ID_COUTBPS_SWITCH].on_off == 0) && (audioControlArr[IBAC_DEV_ID_RADIO_SWITCH].on_off == 0) && (audioControlArr[IBAC_DEV_ID_3G_SWITCH].on_off == 0))
        {
                if((audioControlArr[IBAC_DEV_ID_DAI_MP3].volume == 0)  && (audioControlArr[IBAC_DEV_ID_DAI_NEWS].volume == 0) && (audioControlArr[IBAC_DEV_ID_DAI_RING].volume == 0))
                {
                        mutestate = false;
                        modestate = INIT_STATE;
                }
                else if((audioControlArr[IBAC_DEV_ID_DAI_MP3].volume == 255)  && (audioControlArr[IBAC_DEV_ID_DAI_NEWS].volume == 0) && (audioControlArr[IBAC_DEV_ID_DAI_RING].volume == 255))
                {
                        mutestate = true;
                        modestate = VIDEO_STATE;
                }
                else if((audioControlArr[IBAC_DEV_ID_DAI_MP3].volume == 0)  && (audioControlArr[IBAC_DEV_ID_DAI_NEWS].volume == 255) && (audioControlArr[IBAC_DEV_ID_DAI_RING].volume == 255))
                {
                        mutestate = true;
                        modestate = NEWS_STATE;
                }
        }
        else if((audioControlArr[IBAC_DEV_ID_COUTBPS_SWITCH].on_off == 1) && (audioControlArr[IBAC_DEV_ID_RADIO_SWITCH].on_off == 0) && (audioControlArr[IBAC_DEV_ID_3G_SWITCH].on_off == 0))
        {
                mutestate = true;
                modestate = INIT_STATE;
        }
        else if((audioControlArr[IBAC_DEV_ID_COUTBPS_SWITCH].on_off == 1) && (audioControlArr[IBAC_DEV_ID_RADIO_SWITCH].on_off == 1) && (audioControlArr[IBAC_DEV_ID_3G_SWITCH].on_off == 0))
        {
                mutestate = true;
                modestate = RADIO_STATE;
        }
        else if((audioControlArr[IBAC_DEV_ID_COUTBPS_SWITCH].on_off == 1) && (audioControlArr[IBAC_DEV_ID_RADIO_SWITCH].on_off == 0) && (audioControlArr[IBAC_DEV_ID_3G_SWITCH].on_off == 1))
        {
                mutestate = true;
                modestate = PHONE_STATE;
        }
#endif
}
    
void Main_Volume::initAudio()
{
        InternalLock	lock(&m_lock);

        //resetAudioArr();// reset the local audio array
        //initAudioConf();

        usleep(100*1000);
        //system("alsactl -f /usr/share/alsa/asound.state restore");
        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                switch(audioControlArr[i].id.logicId)
                {
                        case IBAC_DEV_ID_MAIN_VOLUME:
                        case IBAC_DEV_ID_DAI:
#ifdef _8925_VERSION
                        case IBAC_DEV_ID_RADIO_VOLUME:
                        case IBAC_DEV_ID_MICIN1:
                        case IBAC_DEV_ID_MICIN1_BOOST:
                        case IBAC_DEV_ID_3G_SPK_VOLUME:
#else
                        case IBAC_DEV_ID_DPB_VOLUME:
                        case IBAC_DEV_ID_MICIN:
#endif
                        case IBAC_DEV_ID_DAI_MP3:
                        case IBAC_DEV_ID_DAI_VIDEO:
                        case IBAC_DEV_ID_DAI_GPS:
                        case IBAC_DEV_ID_DAI_NEWS:
                        case IBAC_DEV_ID_DAI_SMS:
                        case IBAC_DEV_ID_DAI_RING:
                        case IBAC_DEV_ID_DAI_SMSRING:
                        case IBAC_DEV_ID_DAI_WARNING:
                                audioControlArr[i].status = IBAC_AUDIO_VOLUME;
                                changeHardVolume(&audioControlArr[i],audioControlArr[i].id.numid_vol);
                                break;
#ifdef _8925_VERSION
                        case IBAC_DEV_ID_MAIN_SWITCH:
                        case IBAC_DEV_ID_HP_MIXER_PCM_PB_SWIT:
#else
                        case IBAC_DEV_ID_MICIN_SWITCH:
                        case IBAC_DEV_ID_CHPF_SWITCH:
                        case IBAC_DEV_ID_COUTDAC_SWITCH:
                        case IBAC_DEV_ID_COUTBPS_SWITCH:
#endif
                                audioControlArr[i].status = IBAC_AUDIO_SWITCH;
                                if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_ON)
                                        audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_ON;
                                channelSwitchOn(&audioControlArr[i]);
                                break;
                        case IBAC_DEV_ID_RADIO_SWITCH:
#ifdef _8925_VERSION
                        case IBAC_DEV_ID_HP_MIXER_AUXIN_PB_SWIT:
                        case IBAC_DEV_ID_HP_MIXER_MIC1_PB_SWIT:
                        case IBAC_DEV_ID_R_REC_M_MIC1_CAP_SWIT:
                        case IBAC_DEV_ID_R_REC_M_MIC2_CAP_SWIT:
                        case IBAC_DEV_ID_R_REC_M_LINEINR_CAP_SWIT:
                        case IBAC_DEV_ID_L_REC_M_MIC1_CAP_SWIT:
                        case IBAC_DEV_ID_L_REC_M_MIC2_CAP_SWIT:
                        case IBAC_DEV_ID_L_REC_M_LINEINR_CAP_SWIT:
#else
                        case IBAC_DEV_ID_DPB_SWITCH:
                        case IBAC_DEV_ID_DEEP_SWITCH:
                        case IBAC_DEV_ID_CZC_SWITCH:
                        case IBAC_DEV_ID_COUTAUX_SWITCH:
                        case IBAC_DEV_ID_TV_SWITCH:
                        case IBAC_DEV_ID_VOICE_SWITCH: 
                        case IBAC_DEV_ID_BOLUETOOTH_SWITCH:
                        case IBAC_DEV_ID_JFUNCTION_SWITCH:
                        case IBAC_DEV_ID_SFUNCTION_SWITCH:
#endif
                        case IBAC_DEV_ID_3G_SWITCH: 
                                audioControlArr[i].status = IBAC_AUDIO_SWITCH;
                                if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_OFF)
                                        audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_OFF; 
                                channelSwitchOff(&audioControlArr[i]);
                                break;
                        default:
                                break;
                }
        }

        for (int i = 0; i < IBAC_DEV_ID_MAX; i++)
        {
                printf("[audioManager]: initAudio() logId=%3d numId=%3d switchId=%3d name=%s volume=%3d on=%d status=%02X \n",\
                audioControlArr[i].id.logicId,audioControlArr[i].id.numid_vol,audioControlArr[i].id.numid_switch,audioControlArr[i].id.name,audioControlArr[i].volume,audioControlArr[i].on_off,audioControlArr[i].status);
        }	
}

/*int Main_Volume::initAudioConf()
{
        int flag = 0;
        int ret_size = -1;
        int ret = IBAC_AUDIO_OP_RESULT_LOAD_FAIL;
        // first step, load audio info file
        FILE *fd = fopen(IBAC_AUDIO_INFO_FILE, "r+");
        if (!fd)	// audio info file not exist, reset the audio info array
        {
                fd = fopen(IBAC_AUDIO_INFO_FILE, "w+");
                printf("\n [audioManager]: initAudioConf() new audio info file created, begin Write!!!\n");
                if ((ret_size = fwrite(audioControlArr, sizeof(audioControlArray_t), IBAC_DEV_ID_MAX, fd)) != IBAC_DEV_ID_MAX)
                {
                        printf("\n [audioManager]: initAudioConf() Create the new audio info file, write failed!!!\n");
                        ret = IBAC_AUDIO_OP_RESULT_SAVE_FAIL;
                }
                fclose(fd);
                fd = NULL;
                //system("cp -arf /nand3/applicationdata/audio.cfg /nand3/applicationdata/audiodefault.cfg");
        }
        else	// load the audio info array from the file
        {
                audioControlArray_t tempAudioArray[IBAC_DEV_ID_MAX];
                memset(tempAudioArray, 0, sizeof(tempAudioArray));

                // read data from file
                if((ret_size = fread(tempAudioArray, sizeof(audioControlArray_t), IBAC_DEV_ID_MAX, fd)) != IBAC_DEV_ID_MAX)
                {
                        flag = -1;
                        printf("\n [audioManager]: initAudioConf() read audio info from disk fail, RESET!!!\n");
                        printf("\n[audioManager]: initAudioConf() Numer of elements in audio info file = %3d\n", ret_size/sizeof(audioControlArray_t));
                }
                else
                {				
                        // check the validity of the file
                        for (int i = 0; i < IBAC_DEV_ID_MAX; i++)
                        {
                                tempAudioArray[i].status = IBAC_AUDIO_VOLUME;
                                tempAudioArray[i].on_off = IBAC_AUDIO_SWITCH_OFF;				

//printf("---------------------------------------------------------------------------------------ss----------\r\n");
// printf the audio info array from file
//printf("---tempAudioArray--logicId = %d --numid_vol = %d --numid_switch = %d --name = %s volume = %d \r\n",tempAudioArray[i].id.logicId,tempAudioArray[i].id.numid_vol,tempAudioArray[i].id.numid_switch,tempAudioArray[i].id.name,tempAudioArray[i].volume);		
//printf("---devLogicId2CidSet--logicId = %d --numid_vol = %d --numid_switch = %d --name = %s \r\n",devLogicId2CidSet[i].logicId,devLogicId2CidSet[i].numid_vol,devLogicId2CidSet[i].numid_switch,devLogicId2CidSet[i].name);				
                                // check the id, they all should be equal
                                if (tempAudioArray[i].id.logicId != devLogicId2CidSet[i].logicId || tempAudioArray[i].id.numid_vol != devLogicId2CidSet[i].numid_vol || 
                                        tempAudioArray[i].id.numid_switch != devLogicId2CidSet[i].numid_switch ||strcmp(tempAudioArray[i].id.name, devLogicId2CidSet[i].name) != 0 ||
                                        tempAudioArray[i].volume < 0 ||tempAudioArray[i].volume > IBAC_VOLUME_FULL_SCALE )
                                {
                                        printf("\n [audioManager]: initAudioConf() audio info of dev #%d invalid in FILE, RESET!!!\n", devLogicId2CidSet[i].logicId);				
                                        flag = -1;
                                }
                                else // load the info from file in disk if it is correct
                                {
                                        //printf("-----copy tempAudioArray[i] to audioControlArr[i]. [i = %d]-----\n", i);
                                        memcpy(&audioControlArr[i], &tempAudioArray[i], sizeof(audioControlArray_t));
                                }
                        }
                }
                if (flag == -1)			
                {
                        fclose(fd);
                        fd = NULL;
                        fd = fopen(IBAC_AUDIO_INFO_FILE, "w+");
                        if ((ret_size = fwrite(audioControlArr, sizeof(audioControlArray_t), IBAC_DEV_ID_MAX, fd)) != IBAC_DEV_ID_MAX)
                        {
                                printf("\n [audioManager]: initAudioConf() Save file after load failed\n");
                                ret = IBAC_AUDIO_OP_RESULT_SAVE_FAIL;
                        }
                }
                fclose(fd);
                fd = NULL;
                ret = IBAC_AUDIO_OP_RESULT_SUCCESS;    
        }
        return ret;
}*/

// reset the audio control array
void Main_Volume::resetAudioArr()
{
        // initialize the audio info array
        for (int i = 0; i < IBAC_DEV_ID_MAX; i++)
        {
                memcpy(&audioControlArr[i], &devLogicId2CidSet[i], sizeof(devIdStruct_t));
                audioControlArr[i].volume = devLogicId2CidSet[i].numid_vol;
                audioControlArr[i].status = IBAC_AUDIO_VOLUME;
                audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_OFF;
                channelVolumeValStored[i] = audioControlArr[i].volume;
        }
}

void Main_Volume::loadConf()
{
        int tmp = 0;
        FILE *fb = NULL;

        fb = fopen(AUDIO_MANAGER_FILE, "r+");
        if (!fb) 
        {      
                printf("[audioManager]: loadConf() fopen file failed !!!\n");
                saveConf(volumenum,mutestate);
                return;
        }

        if(fscanf(fb, "%02d:%d:",&volumenum,&tmp) <= 0)
        {
                printf("[audioManager]: loadConf() fscanf file failed !!!\n");
                fclose(fb);
                saveConf(volumenum,mutestate);
                return;
        }

        if(tmp != mutestate)
                mutestate = tmp;
        else
        {
                if(mutestate == false)
                {
                        volumenum = 20;
                        saveConf(volumenum,mutestate); 
                }
                else
                {
                        mutestate = true;
                }
        }
                

        if(volumenum < IBAC_VOLUME_MIN_SCALE || volumenum > IBAC_VOLUME_FULL_SCALE)
        {
                volumenum = 25;
                saveConf(volumenum,mutestate);
        }

        fclose(fb);
        fb = NULL;
}

int Main_Volume::saveConf(int volume,bool mute)
{
        FILE *fb = NULL;
        int ret = -1;
        volumenum = volume;
        fb = fopen(AUDIO_MANAGER_FILE, "w+");
        if (!fb) 
        {      
                printf("[audioManager]: saveConf() fopen file failed !!!\n");
                return IBAC_AUDIO_OP_RESULT_LOAD_FAIL;
        }
        if(fprintf(fb, "%02d:%d:",volumenum,(int)mutestate) < 0)
        {
                printf("[audioManager]: saveConf() fprintf file failed !!!\n");
                fclose(fb);
                return IBAC_AUDIO_OP_RESULT_LOAD_FAIL;
        }
        system("sync");
        fclose(fb);
        fb = NULL;
        return IBAC_AUDIO_OP_RESULT_SUCCESS;
}

int Main_Volume::muteGF()
{		
        int ret = -1;
        IBI2CControl i2cHander;
        unsigned char nState;
        int nCount = 3;
        if (i2cHander.checkOK() == 0)
        {
                printf("[audioManager]: muteGF() Check i2cHander err!\n");
                return -1; 
        }
        ret = i2cHander.writeByte(0x6C, 0x0C, 0x1F);
        if(ret < 0)
        {
                printf("[audioManager]: muteGF() I2c write byte err: %d\n", ret);
                return -1;
        }
        usleep(IBAC_MUTE_GF_TIME);

        ret = i2cHander.readByte(0x6C, 0x06, nState);
        if(ret < 0)
        {
                printf("[audioManager]: muteGF() I2c read byte err: %d\n", ret);
                return -1;
        }
        while( (nState != 0x0F)  && (nCount-- > 0) )
        {
                usleep(IBAC_MUTE_GF_TIME);
                ret = i2cHander.readByte(0x6C, 0x06, nState);
                if(ret < 0)
                {
                        printf("[audioManager]: muteGF() I2c read byte err: %d\n", ret);
                        return -1;
                }		
        }
        return 0;
}

int Main_Volume::unmuteGF()
{
        int ret = -1;
        IBI2CControl i2cHander;
        unsigned char nState;
        int nCount = 3;
        if (i2cHander.checkOK() == 0)
        {
                printf("[audioManager]: unmuteGF() Check i2cHander err!\n");
                return -1; 
        }
        ret = i2cHander.writeByte(0x6C, 0x0C, 0x00);
        if(ret < 0)
        {
                printf("[audioManager]: unmuteGF() I2c write byte err: %d\n", ret);
                return -1;
        }
        usleep(IBAC_MUTE_GF_TIME);

        ret = i2cHander.readByte(0x6C, 0x06, nState);
        if(ret < 0)
        {
                printf("[audioManager]: unmuteGF() I2c read byte err: %d\n", ret);
                return -1;
        }
        while( (nState != 0x0F)  && (nCount-- > 0) )
        {
                usleep(IBAC_MUTE_GF_TIME);
                ret = i2cHander.readByte(0x6C, 0x06, nState);
                if(ret < 0)
                {
                        printf("[audioManager]: unmuteGF() I2c read byte err: %d\n", ret);
                        return -1;
                }		
        }
        return 0;
}    

int Main_Volume::channelSwitchOn(audioControlArray_t *aCArrOn)
{
        //printf("[audioManager]: logId: %d\n", aCArrOn->id.logicId);
#ifndef _8925_VERSION
        if(aCArrOn->id.numid_switch == IBAC_DEV_ID_COUTBPS_SWITCH)// test 13 15 
                muteGF();
#endif

        if(aCArrOn->id.numid_switch > 0) 
        {		
                if((csetAudio(aCArrOn->id.numid_switch, NULL, IBAC_AUDIO_SWITCH_ON)) < 0)
                {
                        printf("[audioManager]: channelSwitchOn() change device --> %d error in switch set\n", aCArrOn->id.logicId);
                }
        }
#ifndef _8925_VERSION
        if(aCArrOn->id.numid_switch == IBAC_DEV_ID_COUTBPS_SWITCH)
                unmuteGF();
#elif defined(_8925_VERSION)
        unmuteGF();
#endif
        return 0;
}

int Main_Volume::channelSwitchOff(audioControlArray_t *aCArrOff)
{
        //printf("[audioManager]: logId: %d\n", aCArrOff->id.logicId);
#ifndef _8925_VERSION
        if(aCArrOff->id.numid_switch == IBAC_DEV_ID_COUTBPS_SWITCH)
                muteGF();
#endif

        if(aCArrOff->id.numid_switch > 0) 
        {
                if((csetAudio(aCArrOff->id.numid_switch, aCArrOff->id.name,IBAC_AUDIO_SWITCH_OFF)) < 0) 
                {
                        printf("[audioManager]: channelSwitchOff() change device --> %d error in switch set\n", aCArrOff->id.logicId);
                }
        }
#ifndef _8925_VERSION
        if(aCArrOff->id.numid_switch == IBAC_DEV_ID_COUTBPS_SWITCH)
                unmuteGF();
#elif defined(_8925_VERSION)
        unmuteGF();
#endif
        return 0;
}

int Main_Volume::changeHardVolume(audioControlArray_t *audioItem,int newVolume)
{
        int  newValue = convertVol(newVolume,audioItem->id.logicId);   // convert to the real vol value
        printf("[audioManager]: changeHardVolume() logicId: %d;newVolume: %d;newValue: %d\n",audioItem->id.logicId, newVolume,newValue);
        if((csetAudio(audioItem->id.logicId,NULL,newValue)) < 0) 
        {
                printf("[audioManager]: changeHardVolume() change device --> %d error in volume set\n",audioItem->id.logicId);
                return -1;	
        }
        return 0;
}

int Main_Volume::convertVol(int vol, unsigned int id)
{
        int ret = -1;

        if (vol < 0 )
                return -1;

        switch (id)
        {
                case IBAC_DEV_ID_MAIN_VOLUME:
                case IBAC_DEV_ID_DAI:
#ifdef _8925_VERSION
                case IBAC_DEV_ID_RADIO_VOLUME:
                case IBAC_DEV_ID_3G_SPK_VOLUME:
                        if(vol < IBAC_VOLUME_MIN_SCALE)
                        {
                                ret = IBAC_VOLUME_MIN_SCALE;
                        }
                        else if(vol > IBAC_VOLUME_FULL_SCALE)
                        {
                                ret = IBAC_VOLUME_FULL_SCALE;
                        }
                        else	
                        {
                                ret = vol;
                        }
#else
                        ret = mainvolScaleToFact(vol);
#endif
                        break;
#ifndef _8925_VERSION
                case IBAC_DEV_ID_DPB_VOLUME:
                        ret = IBAC_INIT_VOLUME_MAX;
                        break;
                case IBAC_DEV_ID_MICIN:
#else
                case IBAC_DEV_ID_MICIN1:
#endif
                        ret = IBAC_INIT_VOLUME_MIC;
                        break;       
                case IBAC_DEV_ID_DAI_MP3:
                case IBAC_DEV_ID_DAI_VIDEO:
                case IBAC_DEV_ID_DAI_GPS:
                case IBAC_DEV_ID_DAI_NEWS:
                case IBAC_DEV_ID_DAI_SMS:
                case IBAC_DEV_ID_DAI_SMSRING:
                case IBAC_DEV_ID_DAI_RING:
                case IBAC_DEV_ID_DAI_WARNING:
                        ret = mediavolScaleToFact(vol);
                        break;                 
                default:
                        ret = IBAC_INIT_VOLUME_MAX;
                        break;
        }
        return ret;
}

int Main_Volume::mainvolScaleToFact(int vol)//from 1~30 to 69~127
{
        int ret = -1;
        if(vol < IBAC_VOLUME_MIN_SCALE ||vol > IBAC_VOLUME_FULL_SCALE)
                return ret;
        ret = vol*2 + IBAC_VOLUME_MAIN_MIN - 2;
        printf("[audioManager]: mainvolScaleToFact() ret = %d; vol = %d\n", ret, vol);
        return ret;
}

int Main_Volume::mainvolFactToScale(int vol)//from 69~127 to 1~30
{
        int ret = -1;
        if(vol < IBAC_VOLUME_MAIN_MIN ||vol > IBAC_VOLUME_MAIN_MAX)
                return ret;
        ret = (vol - IBAC_VOLUME_MAIN_MIN - 2)/2;
        printf("[audioManager]: mainvolFactToScale() ret = %d; val = %d\n", ret, vol);
        return ret;
}

int Main_Volume::mediavolScaleToFact(int vol)//from 0~30 to 0~255
{
        int ret = -1;
        if(vol < IBAC_INIT_VOLUME_NULL ||vol > IBAC_VOLUME_FULL_SCALE)
                return ret;
        ret = IBAC_INIT_VOLUME_MAX - (IBAC_INIT_VOLUME_MAX*(IBAC_VOLUME_FULL_SCALE - vol))/IBAC_VOLUME_FULL_SCALE;
        printf("[audioManager]: mediavolScaleToFact() ret = %d; val = %d\n", ret, vol);
        return ret;
}

int Main_Volume::mediavolFactToScale(int vol)//from 0~255 to 0~30
{
        int ret = -1;
        if(vol < IBAC_INIT_VOLUME_NULL ||vol > IBAC_INIT_VOLUME_MAX)
                return ret;
        ret = IBAC_VOLUME_FULL_SCALE - (IBAC_VOLUME_FULL_SCALE*(IBAC_INIT_VOLUME_MAX - vol))/IBAC_INIT_VOLUME_MAX;
        printf("[audioManager]: mediavolFactToScale() ret = %d; val = %d\n", ret, vol);
        return ret;
}

long Main_Volume::get_integer(long x, long min, long max)
{
        long val = min;
        val = (long)convert_prange1(x, min, max);
        val = check_range(val, min, max);
        return val;
}

int Main_Volume::csetAudio(int numid, char *name, int volume)
{
        //printf("[audioManager]: csetAudio() *** %d; %s; %d\n", numid, name, volume);
        long vol = (long)volume;
        int err;
        char card[20];
        unsigned int idx, count;
        long tmp = 0;
        const char *names;
        snd_ctl_t *handle = NULL;
        snd_ctl_elem_type_t type;
        snd_ctl_elem_info_t *info;
        snd_ctl_elem_id_t *id;
        snd_ctl_elem_value_t *control;
        snd_ctl_elem_info_alloca(&info);
        snd_ctl_elem_id_alloca(&id);
        snd_ctl_elem_value_alloca(&control);

        strcpy(card, "default");
        if(name == NULL)
                name = "";

        // if numid is 0 or name is NULL, device is not available, return
        if (numid <= 0 && !strcmp(name, "")) 
        {		
                printf("[audioManager]: csetAudio() Set Audio --> Wrong control identifier: %d\n", numid);
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

        if (handle == NULL && (err = snd_ctl_open(&handle, card, 0)) < 0) /* open the control */
        {		
                printf("[audioManager]: csetAudio() Set Audio --> Control %s open error: %s\n", card, snd_strerror(err));
                return err;
        }

        snd_ctl_elem_info_set_id(info, id);				/* info->id = id */

        if ((err = snd_ctl_elem_info(handle, info)) < 0) 		//handle->ops->element_info(handle, info); get info details from drivers
        {
                printf("[audioManager]: csetAudio() Set Audio --> Cannot find the given element from control %s, name = %s, id = %d\n", card, name, numid);
                printf("[audioManager]: csetAudio() *** pb_error: %s\n", snd_strerror(err));		
                snd_ctl_close(handle);
                handle = NULL;
                return err;
        }
	
        snd_ctl_elem_info_get_id(info, id); /* FIXME: Remove it when hctl find works ok !!! */ 

        type = snd_ctl_elem_info_get_type(info);	/* type = info->type */
        count = snd_ctl_elem_info_get_count(info);	/* count = info->count*/
        names = snd_ctl_elem_info_get_name(info);
        snd_ctl_elem_value_set_id(control, id);		/* control->id = id */

        printf("[audioManager]: csetAudio() ************** for name %s count : %d \n", names,count);
        for (idx = 0; idx < count && idx < 128; idx++) 
        {
                switch (type) 
                {
                        case SND_CTL_ELEM_TYPE_BOOLEAN:
                                tmp = vol > 0 ? 1 : 0;
                                printf("[audioManager]: csetAudio() ************** for SND_CTL_ELEM_TYPE_BOOLEAN : %ld\n", tmp);
                                snd_ctl_elem_value_set_boolean(control, idx, tmp);		/* control->value.integer.value[idx] = tmp; */
                                break;
                        case SND_CTL_ELEM_TYPE_INTEGER:
                                tmp = vol;
                                printf("[audioManager]: csetAudio() ************** for SND_CTL_ELEM_TYPE_INTEGER : %ld\n", tmp);
                                snd_ctl_elem_value_set_integer(control, idx, tmp);	/* control->value.integer.value[idx] = tmp; */
                                break;
                        case SND_CTL_ELEM_TYPE_INTEGER64:
                                tmp = vol;
                                printf("[audioManager]: csetAudio() ************** for SND_CTL_ELEM_TYPE_INTEGER64 : %ld\n", tmp);
                                snd_ctl_elem_value_set_integer64(control, idx, tmp);	/* control->value.integer64.value[idx] = tmp; */
                                break;
                        case SND_CTL_ELEM_TYPE_ENUMERATED:
                                vol = (unsigned)(vol < 0  ? 0 : vol);				
                                tmp =  check_range(vol, 0, (int)snd_ctl_elem_info_get_items(info) - 1);
                                printf("[audioManager]: csetAudio() ************** for SND_CTL_ELEM_TYPE_ENUMERATED : %ld\n", tmp);
                                snd_ctl_elem_value_set_enumerated(control, idx, tmp);	/* control->value.enumerated.item[idx] = val; */
                                break;
                        case SND_CTL_ELEM_TYPE_BYTES:
                                tmp = get_integer(vol, 0, 255);
                                printf("[audioManager]: csetAudio() ************** for SND_CTL_ELEM_TYPE_BYTES : %ld\n", tmp);
                                snd_ctl_elem_value_set_byte(control, idx, tmp);	/* control->value.bytes.data[idx] = val; */
                                break;
                        default:
                                break;
                }
        }

        if ((err = snd_ctl_elem_write(handle, control)) < 0) 	//handle->ops->element_write(handle, control) 
        {		
                printf("[audioManager]: csetAudio() Set Audio --> Control %s element write error: %s\n", card, snd_strerror(err));		
                snd_ctl_close(handle);
                handle = NULL;
                return err;
        }
        snd_ctl_close(handle);
        handle = NULL;

        return 0;
}

int Main_Volume::cgetAudio(int numid, char *name, int type)
{
        int err;
        char card[20];
        long tmp = 0;
        snd_hctl_t *hctl;
        snd_hctl_elem_t *elem;
        snd_ctl_elem_id_t *mixer_id;
        snd_ctl_elem_value_t *control;
        snd_ctl_elem_id_alloca(&mixer_id);
        snd_ctl_elem_value_alloca(&control);

        strcpy(card, "default");

        snd_ctl_elem_id_set_interface(mixer_id, SND_CTL_ELEM_IFACE_MIXER);
        snd_ctl_elem_id_set_name(mixer_id, name);
        snd_ctl_elem_value_set_id(control, mixer_id);

        if ((err = snd_hctl_open(&hctl, card, 0)) < 0) 
        {
                printf("[audioManager]: cgetAudio() Can't load mixer control '%s'\n", (char *)control);
                return err;
        }

        if ((err = snd_hctl_load(hctl)) < 0) 
        {
                fprintf(stderr, "mixer: Can't load mixer control '%s'\n", (char *)control);
                snd_hctl_close(hctl);
                hctl = NULL;
                return err;
        }
        
        elem = snd_hctl_find_elem(hctl, mixer_id);
        if(elem)
        {
                snd_hctl_elem_read(elem, control);//left/right balance will be set together 
                if(type == 0)
                        tmp = snd_ctl_elem_value_get_integer(control, 0);
                else
                        tmp = snd_ctl_elem_value_get_boolean(control, 0);
        }       

        printf("[audioManager]: cgetAudio() -------------------------count: %d name: %s\n", tmp,name);
        snd_hctl_close(hctl);
        hctl = NULL;
        return tmp;
}

int Main_Volume::set_main_volume(int volume)
{
        int vol;
        if(volume < IBAC_VOLUME_MIN_SCALE)
                volumenum = IBAC_VOLUME_MIN_SCALE;
        else if(volume > IBAC_VOLUME_FULL_SCALE)
                volumenum = IBAC_VOLUME_FULL_SCALE;
        else
                volumenum = volume;
        printf("[audioManager]: set_main_volume() vol = %d volumenum = %d \n", vol,volumenum);
        mutestate = true;
        saveConf(volumenum,mutestate);
        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                switch(audioControlArr[i].id.logicId)
                {
                        case IBAC_DEV_ID_MAIN_VOLUME:
                        case IBAC_DEV_ID_DAI:
#ifdef _8925_VERSION
                        case IBAC_DEV_ID_RADIO_VOLUME:
                        case IBAC_DEV_ID_3G_SPK_VOLUME:
#endif
                                changeHardVolume(&audioControlArr[i],volumenum);
                                channelVolumeValStored[i] = volumenum;

                                break;
                        default:
                                break;
                }
        }
        
        return 0;
}

int Main_Volume::get_main_volume()
{
        int tmpvol = volumenum;
        loadConf();
        printf("----------------------------------------------------tmp%d read%d \n",tmpvol,volumenum); 
        if(tmpvol != volumenum)
        {
                if(volumenum >= IBAC_VOLUME_MIN_SCALE && volumenum <= IBAC_VOLUME_FULL_SCALE)
                {
                        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_MAIN_VOLUME:
                                        case IBAC_DEV_ID_DAI:
                                                audioControlArr[i].volume = volumenum;
                                                channelVolumeValStored[i] = audioControlArr[i].volume;
                                                changeHardVolume(&audioControlArr[i],audioControlArr[i].volume);
#ifdef _8925_VERSION
                                                for(int j = 0; j < IBAC_DEV_ID_MAX; j++)
                                                {
                                                        if(IBAC_DEV_ID_3G_SPK_VOLUME == audioControlArr[j].id.logicId)
                                                        {
                                                                audioControlArr[j].volume = volumenum;
                                                                channelVolumeValStored[j] = volumenum;
                                                                changeHardVolume(&audioControlArr[j],volumenum);
                                                                break;
                                                        }
                                                }
#endif
                                                break;
                                        default:
                                                break;
                                }
                        }
                }
                else
                {
                        system("rm -rf /nand3/applicationdata/audiomanager.cfg;sync;");
                        system("sync;");
                        volumenum = 25;
                        saveConf(volumenum, mutestate);
                }
        }

        return volumenum;
}

int Main_Volume::set_mute()
{  
        mutestate = false;
        printf("[audioManager]: set_mute() vol = %d state = %d \n", volumenum,(int)mutestate);
        saveConf(volumenum,mutestate);

        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                switch(audioControlArr[i].id.logicId)
                {
#ifndef _8925_VERSION
                        case IBAC_DEV_ID_COUTBPS_SWITCH:
#endif
                        case IBAC_DEV_ID_3G_SWITCH:
                        case IBAC_DEV_ID_RADIO_SWITCH:
                                channelSwitchOff(&audioControlArr[i]);
                                break;
                        case IBAC_DEV_ID_DAI_MP3:
                        case IBAC_DEV_ID_DAI_VIDEO:
                        case IBAC_DEV_ID_DAI_GPS:
                        case IBAC_DEV_ID_DAI_NEWS:
                        case IBAC_DEV_ID_DAI_SMS:
                        case IBAC_DEV_ID_DAI_RING:
                        case IBAC_DEV_ID_DAI_SMSRING:
                                changeHardVolume(&audioControlArr[i],0);
                        default:
                                break;
                }
        }
        system("sleep 1");
        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                switch(audioControlArr[i].id.logicId)
                {
                        case IBAC_DEV_ID_MAIN_VOLUME:
                        case IBAC_DEV_ID_DAI:
                                changeHardVolume(&audioControlArr[i],20);
                                channelVolumeValStored[i] = 20;
#ifdef _8925_VERSION
                                for(int j = 0; j < IBAC_DEV_ID_MAX; j++)
                                {
                                        if(IBAC_DEV_ID_3G_SPK_VOLUME == audioControlArr[j].id.logicId)
                                        {
                                                channelVolumeValStored[j] = 20;
                                                changeHardVolume(&audioControlArr[j],20);
                                                break;
                                        }
                                }
#endif
                                break;
                        default:
                                break;
                }
        }
        if(0 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_WARNING))
        {
                turnSoundOnChannel(IBAC_DEV_ID_DAI_WARNING);
        }
        else
        {
                turnSoundOffChannel(IBAC_DEV_ID_DAI_WARNING);
        }

        return 0;
}

int Main_Volume::set_unmute()
{
        int ret;
        mutestate = true;
        printf("[audioManager]: set_unmute() vol = %d state = %d \n", volumenum,(int)mutestate);
        saveConf(volumenum,mutestate);
        bool hasTurnOn = false;
        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                switch(audioControlArr[i].id.logicId)
                {
                        case IBAC_DEV_ID_MAIN_VOLUME:
                        case IBAC_DEV_ID_DAI:
                                //audioControlArr[i].volume = volumenum;
                                changeHardVolume(&audioControlArr[i],volumenum);
                                channelVolumeValStored[i] = volumenum;
#ifdef _8925_VERSION
                                for(int j = 0; j < IBAC_DEV_ID_MAX; j++)
                                {
                                        if(IBAC_DEV_ID_3G_SPK_VOLUME == audioControlArr[j].id.logicId)
                                        {
                                                channelVolumeValStored[j] = volumenum;
                                                changeHardVolume(&audioControlArr[j],volumenum);
                                                break;
                                        }
                                }
#endif
                                break;
#ifndef _8925_VERSION
                        case IBAC_DEV_ID_COUTBPS_SWITCH:
#endif
                                if(audioControlArr[i].on_off == IBAC_AUDIO_SWITCH_ON)
                                        channelSwitchOn(&audioControlArr[i]);
                                break;
                        case IBAC_DEV_ID_RADIO_SWITCH:
                        case IBAC_DEV_ID_DAI_MP3:
                        case IBAC_DEV_ID_DAI_VIDEO:
                        case IBAC_DEV_ID_DAI_GPS:
                        case IBAC_DEV_ID_DAI_NEWS:
                        case IBAC_DEV_ID_DAI_SMS:
                        case IBAC_DEV_ID_DAI_SMSRING:
                                if(0 == getOneChannelMuteFlag(audioControlArr[i].id.logicId) \
                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_3G_SWITCH)
                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_WARNING)
                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_RING))
                                {
                                        if(false == hasTurnOn)
                                        {
                                                hasTurnOn = true;
                                                turnSoundOnChannel(audioControlArr[i].id.logicId);
                                        }
                                }
                                else
                                {
                                        turnSoundOffChannel(audioControlArr[i].id.logicId);
                                }
                                break;
                        case IBAC_DEV_ID_DAI_WARNING:
                        {
                                if(0 == getOneChannelMuteFlag(audioControlArr[i].id.logicId) \
                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_3G_SWITCH) \
                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_RING))
                                {
                                        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
                                        {
                                                switch(audioControlArr[i].id.logicId)
                                                {
                                                        case IBAC_DEV_ID_MAIN_VOLUME:
                                                        case IBAC_DEV_ID_DAI:
                                                                changeHardVolume(&audioControlArr[i],20);
#ifdef _8925_VERSION
                                                        for(int j = 0; j < IBAC_DEV_ID_MAX; j++)
                                                        {
                                                                if(IBAC_DEV_ID_3G_SPK_VOLUME == audioControlArr[j].id.logicId)
                                                                {
                                                                        changeHardVolume(&audioControlArr[j],20);
                                                                        break;
                                                                }
                                                        }
#endif
                                                                break;
                                                        default:
                                                                break;
                                                }
                                        }
                                        
                                        turnSoundOnChannel(audioControlArr[i].id.logicId);
                                }
                                else
                                {
                                        turnSoundOffChannel(audioControlArr[i].id.logicId);
                                }
                                break;

                                if(audioControlArr[i].on_off == IBAC_AUDIO_SWITCH_ON)
                                        channelSwitchOn(&audioControlArr[i]);
                                break;
                        }
                        case IBAC_DEV_ID_DAI_RING:
                        case IBAC_DEV_ID_3G_SWITCH:
                        {
                                if(1 == getOneChannelMuteFlag(audioControlArr[i].id.logicId))
                                {
                                       turnSoundOffChannel(audioControlArr[i].id.logicId);
                                }
                                else
                                {
                                       turnSoundOnChannel(audioControlArr[i].id.logicId);
                                }
                                break;


                                if(true == channelMuteState[i])
                                {
                                        channelSwitchOff(&audioControlArr[i]);
                                }
                                else
                                {
                                        if(audioControlArr[i].on_off == IBAC_AUDIO_SWITCH_ON)
                                                channelSwitchOn(&audioControlArr[i]);
                                }
                                break;
                        }
                        default:
                                break;
                }
        } 
         return 0;
}      

int Main_Volume::get_mute_status()
{
        bool tmpvol = mutestate;
        loadConf();
        if(tmpvol != mutestate)
        {
                if(mutestate == false)
                        set_mute();
                else
                        set_unmute();
        }
        return (int)mutestate;
}

VOICE_MODE Main_Volume::get_mode()
{
        return modestate;
}

#ifndef _8925_VERSION
void Main_Volume::set_mode(VOICE_MODE mode)
{
        if(mode == PHONE_STATE)
        {
                if(modestate != PHONE_STATE && modestate != INIT_STATE)
                {
                        modestatebak = modestate;
                        modestate = mode;
                }
        }
        else if(mode == INIT_STATE)
        {
                if(modestate == PHONE_STATE && modestatebak != INIT_STATE)
                {
                        modestate = modestatebak;
                        modestatebak = INIT_STATE;
                }        
        }
        else
        {
                modestatebak = INIT_STATE;
                modestate = mode;
        }
        switch(modestate)
        {
                case INIT_STATE:
                        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_DAI_MP3:
                                        case IBAC_DEV_ID_DAI_VIDEO:
                                        case IBAC_DEV_ID_DAI_GPS:
                                        case IBAC_DEV_ID_DAI_NEWS:
                                        case IBAC_DEV_ID_DAI_SMS:
                                        case IBAC_DEV_ID_DAI_RING:
                                        case IBAC_DEV_ID_DAI_SMSRING:
                                        case IBAC_DEV_ID_DAI_WARNING:
                                                changeHardVolume(&audioControlArr[i],audioControlArr[i].id.numid_vol);
                                                break;
                                        case IBAC_DEV_ID_COUTBPS_SWITCH:
                                                if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_ON)
                                                        audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_ON;
                                                channelSwitchOn(&audioControlArr[i]);
                                                break;
                                        case IBAC_DEV_ID_RADIO_SWITCH:
                                        case IBAC_DEV_ID_3G_SWITCH:
                                                if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_OFF)
                                                        audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_OFF; 
                                                channelSwitchOff(&audioControlArr[i]);
                                                break;
                                        default:
                                                break;
                                }
                        } 
                        break;
                case RADIO_STATE:
                        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_DAI_MP3:
                                        case IBAC_DEV_ID_DAI_VIDEO:
                                        case IBAC_DEV_ID_DAI_GPS:
                                        case IBAC_DEV_ID_DAI_NEWS:
                                        case IBAC_DEV_ID_DAI_SMS:
                                        case IBAC_DEV_ID_DAI_RING:
                                        case IBAC_DEV_ID_DAI_SMSRING:
                                                changeHardVolume(&audioControlArr[i],IBAC_INIT_VOLUME_NULL);
                                                break;                                            
                                        case IBAC_DEV_ID_DAI_WARNING:
                                                changeHardVolume(&audioControlArr[i],audioControlArr[i].id.numid_vol);
                                                break;
                                        case IBAC_DEV_ID_COUTBPS_SWITCH:
                                        case IBAC_DEV_ID_RADIO_SWITCH:
                                                if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_ON)
                                                        audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_ON;
                                                channelSwitchOn(&audioControlArr[i]);
                                                break;
                                        case IBAC_DEV_ID_3G_SWITCH:
                                                if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_OFF)
                                                        audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_OFF; 
                                                channelSwitchOff(&audioControlArr[i]);
                                                break;
                                        default:
                                                break;
                                }
                        } 
                        break;
                case PHONE_STATE:
                        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_DAI_MP3:
                                        case IBAC_DEV_ID_DAI_VIDEO:
                                        case IBAC_DEV_ID_DAI_GPS:
                                        case IBAC_DEV_ID_DAI_NEWS:
                                        case IBAC_DEV_ID_DAI_SMS:
                                        case IBAC_DEV_ID_DAI_RING:
                                        case IBAC_DEV_ID_DAI_SMSRING:
                                                changeHardVolume(&audioControlArr[i],IBAC_INIT_VOLUME_NULL);
                                                break;                                            
                                        case IBAC_DEV_ID_DAI_WARNING:
                                                changeHardVolume(&audioControlArr[i],audioControlArr[i].id.numid_vol);
                                                break;
                                        case IBAC_DEV_ID_COUTBPS_SWITCH:
                                                if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_ON)
                                                        audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_ON;
                                                channelSwitchOn(&audioControlArr[i]);
                                                break;
                                        case IBAC_DEV_ID_3G_SWITCH:
                                                if(true == channelMuteState[i])
                                                {
                                                        channelSwitchOff(&audioControlArr[i]);
                                                }
                                                else
                                                {
                                                        if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_ON)
                                                                audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_ON;
                                                        channelSwitchOn(&audioControlArr[i]);
                                                }
                                                break;
                                        case IBAC_DEV_ID_RADIO_SWITCH:
                                                if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_OFF)
                                                        audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_OFF; 
                                                channelSwitchOff(&audioControlArr[i]);
                                                break;
                                        default:
                                                break;
                                }
                        } 
                        break;
                case VIDEO_STATE:
                        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_DAI_NEWS:
                                                changeHardVolume(&audioControlArr[i],IBAC_INIT_VOLUME_NULL);
                                                break;
                                        case IBAC_DEV_ID_DAI_MP3:
                                        case IBAC_DEV_ID_DAI_VIDEO:
                                        case IBAC_DEV_ID_DAI_GPS:
                                        case IBAC_DEV_ID_DAI_SMS:
                                        case IBAC_DEV_ID_DAI_RING:
                                        case IBAC_DEV_ID_DAI_SMSRING:
                                        case IBAC_DEV_ID_DAI_WARNING:
                                                changeHardVolume(&audioControlArr[i],audioControlArr[i].id.numid_vol);
                                                break;
                                        case IBAC_DEV_ID_COUTBPS_SWITCH:
                                        case IBAC_DEV_ID_3G_SWITCH:
                                        case IBAC_DEV_ID_RADIO_SWITCH:
                                                if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_OFF)
                                                        audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_OFF; 
                                                channelSwitchOff(&audioControlArr[i]);
                                                break;
                                        default:
                                                break;
                                }
                        } 
                        break;
                case NEWS_STATE:
                        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_DAI_MP3:
                                        case IBAC_DEV_ID_DAI_VIDEO:
                                                changeHardVolume(&audioControlArr[i],IBAC_INIT_VOLUME_NULL);
                                                break;
                                        case IBAC_DEV_ID_DAI_NEWS:
                                        case IBAC_DEV_ID_DAI_GPS:
                                        case IBAC_DEV_ID_DAI_SMS:
                                        case IBAC_DEV_ID_DAI_RING:
                                        case IBAC_DEV_ID_DAI_SMSRING:
                                        case IBAC_DEV_ID_DAI_WARNING:
                                                changeHardVolume(&audioControlArr[i],audioControlArr[i].id.numid_vol);
                                                break;
                                        case IBAC_DEV_ID_COUTBPS_SWITCH:
                                        case IBAC_DEV_ID_3G_SWITCH:
                                        case IBAC_DEV_ID_RADIO_SWITCH:
                                                if(audioControlArr[i].on_off != IBAC_AUDIO_SWITCH_OFF)
                                                        audioControlArr[i].on_off = IBAC_AUDIO_SWITCH_OFF; 
                                                channelSwitchOff(&audioControlArr[i]);
                                                break;
                                        default:
                                                break;
                                }
                        } 
                        break;
        }
}
#endif

Main_Volume::InternalLock::InternalLock(pthread_mutex_t* lock)
{
        m_lock=lock;
        pthread_mutex_lock(m_lock);
}

Main_Volume::InternalLock::~InternalLock()
{
        pthread_mutex_unlock(m_lock);
}

int Main_Volume::muteOneChannel(int channelID)
{
        int i = 0;
        for(i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                if(channelID == audioControlArr[i].id.logicId)
                {
                        channelMuteState[i] = true;
                        break;
                }
                else
                {
                }
        }
        if(i >= IBAC_DEV_ID_MAX)
        {
                return -1;
        }
        else
        {
        }
        turnSoundOffChannel(channelID);
        switch(channelID)
        {
                case IBAC_DEV_ID_DAI_MP3:
                case IBAC_DEV_ID_DAI_VIDEO:
                case IBAC_DEV_ID_DAI_GPS:
                case IBAC_DEV_ID_DAI_NEWS:
                case IBAC_DEV_ID_DAI_SMS:
                case IBAC_DEV_ID_DAI_SMSRING:
                case IBAC_DEV_ID_RADIO_SWITCH:
                {
                        int hasTurnOn = false;
                        for(i = 0; i < IBAC_DEV_ID_MAX; i++)
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_DAI_MP3:
                                        case IBAC_DEV_ID_DAI_VIDEO:
                                        case IBAC_DEV_ID_DAI_GPS:
                                        case IBAC_DEV_ID_DAI_NEWS:
                                        case IBAC_DEV_ID_DAI_SMS:
                                        case IBAC_DEV_ID_DAI_SMSRING:
                                        case IBAC_DEV_ID_RADIO_SWITCH:
                                        {
                                                if(0 == getOneChannelMuteFlag(audioControlArr[i].id.logicId) \
                                                        && true == mutestate \
                                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_WARNING) \
                                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_3G_SWITCH) \
                                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_RING))
                                                {
                                                        if(false == hasTurnOn)
                                                        {
                                                                hasTurnOn = true;
                                                                turnSoundOnChannel(audioControlArr[i].id.logicId);
                                                        }
                                                }
                                                else
                                                {
                                                        turnSoundOffChannel(audioControlArr[i].id.logicId);
                                                }
                                                break;
                                        }
                                        default:
                                                break;
                                }
                        } 
                }
                        break;
                case IBAC_DEV_ID_DAI_RING:
                case IBAC_DEV_ID_3G_SWITCH:
                {
                        for(i = 0; i < IBAC_DEV_ID_MAX; i++)
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_DAI_MP3:
                                        case IBAC_DEV_ID_DAI_VIDEO:
                                        case IBAC_DEV_ID_DAI_GPS:
                                        case IBAC_DEV_ID_DAI_NEWS:
                                        case IBAC_DEV_ID_DAI_SMS:
                                        case IBAC_DEV_ID_DAI_SMSRING:
                                        case IBAC_DEV_ID_RADIO_SWITCH:
                                        {
                                                if(0 == getOneChannelMuteFlag(audioControlArr[i].id.logicId) \
                                                        && true == mutestate \
                                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_WARNING))
                                                {
                                                        turnSoundOnChannel(audioControlArr[i].id.logicId);
                                                }
                                                else
                                                {
                                                        turnSoundOffChannel(audioControlArr[i].id.logicId);
                                                }
                                                break;
                                        }
                                        case IBAC_DEV_ID_DAI_WARNING:
                                        {
                                                if(0 == getOneChannelMuteFlag(audioControlArr[i].id.logicId))
                                                {
                                                        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
                                                        {
                                                                switch(audioControlArr[i].id.logicId)
                                                                {
                                                                        case IBAC_DEV_ID_MAIN_VOLUME:
                                                                        case IBAC_DEV_ID_DAI:
                                                                                changeHardVolume(&audioControlArr[i],20);
#ifdef _8925_VERSION
										for(int j = 0; j < IBAC_DEV_ID_MAX; j++)
										{
											if(IBAC_DEV_ID_3G_SPK_VOLUME == audioControlArr[j].id.logicId)
											{
												changeHardVolume(&audioControlArr[j],20);
												break;
											}
										}
#endif
                                                                                break;
                                                                        default:
                                                                                break;
                                                                }
                                                        }
                                                        turnSoundOnChannel(audioControlArr[i].id.logicId);
                                                }
                                                else
                                                {
                                                        turnSoundOffChannel(audioControlArr[i].id.logicId);
                                                }
                                                break;
                                        }
                                        default:
                                                break;
                                }
                        } 
                        break;
                }
                case IBAC_DEV_ID_DAI_WARNING:
                {
                        for(i = 0; i < IBAC_DEV_ID_MAX; i++)
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_MAIN_VOLUME:
                                        case IBAC_DEV_ID_DAI:
                                                turnSoundOnChannel(audioControlArr[i].id.logicId);
                                                break;
                                        case IBAC_DEV_ID_DAI_MP3:
                                        case IBAC_DEV_ID_DAI_VIDEO:
                                        case IBAC_DEV_ID_DAI_GPS:
                                        case IBAC_DEV_ID_DAI_NEWS:
                                        case IBAC_DEV_ID_DAI_SMS:
                                        case IBAC_DEV_ID_DAI_SMSRING:
                                        case IBAC_DEV_ID_RADIO_SWITCH:
                                        {
                                                if(0 == getOneChannelMuteFlag(audioControlArr[i].id.logicId) \
                                                        && true == mutestate \
                                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_3G_SWITCH) \
                                                        && 1 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_RING))
                                                {
                                                        turnSoundOnChannel(audioControlArr[i].id.logicId);
                                                }
                                                else
                                                {
                                                        turnSoundOffChannel(audioControlArr[i].id.logicId);
                                                }
                                                break;
                                        }
                                        default:
                                                break;
                                }
                        } 
                        break;
                }
                default:
                {
                        break;
                }
        }

        return -1;
}

int Main_Volume::unmuteOneChannel(int channelID)
{
        int i = 0;
        for(i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                if(channelID == audioControlArr[i].id.logicId)
                {
                        channelMuteState[i] = false;
                        break;
                }
                else
                {
                }
        }
        if(i >= IBAC_DEV_ID_MAX)
        {
                return -1;
        }
        else
        {
        }
        switch(channelID)
        {
                case IBAC_DEV_ID_DAI_MP3:
                case IBAC_DEV_ID_DAI_VIDEO:
                case IBAC_DEV_ID_DAI_GPS:
                case IBAC_DEV_ID_DAI_NEWS:
                case IBAC_DEV_ID_DAI_SMS:
                case IBAC_DEV_ID_DAI_SMSRING:
                case IBAC_DEV_ID_RADIO_SWITCH:
                {
                        for(i = 0; i < IBAC_DEV_ID_MAX; i++)
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_DAI_MP3:
                                        case IBAC_DEV_ID_DAI_VIDEO:
                                        case IBAC_DEV_ID_DAI_GPS:
                                        case IBAC_DEV_ID_DAI_NEWS:
                                        case IBAC_DEV_ID_DAI_SMS:
                                        case IBAC_DEV_ID_DAI_SMSRING:
                                        case IBAC_DEV_ID_RADIO_SWITCH:
                                        {
                                                if(audioControlArr[i].id.logicId != channelID)
                                                {
                                                        turnSoundOffChannel(audioControlArr[i].id.logicId);
                                                }
                                                break;
                                        }
                                        default:
                                                break;
                                }
                        } 
                        if(0 == getOneChannelMuteFlag(IBAC_DEV_ID_3G_SWITCH) \
                                || 0 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_RING) \
                                || 0 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_WARNING) \
                                || false == mutestate)
                        {
                                turnSoundOffChannel(channelID);
                        }
                        else
                        {
                                turnSoundOnChannel(channelID);
                        }
                        break;
                }
                case IBAC_DEV_ID_DAI_WARNING:
                {
                        if(     (0 == getOneChannelMuteFlag(IBAC_DEV_ID_3G_SWITCH) \
                                || 0 == getOneChannelMuteFlag(IBAC_DEV_ID_DAI_RING)) && true == mutestate)
                        {
                                turnSoundOffChannel(channelID);
                        }
                        else
                        {
#if 0
#ifdef _8925_VERSION
                                if(0 >= *(volatile unsigned int*)(gpa_vbase + GPA_DAT) & (0x1 << 16))
#else
                                if(0 >= *(volatile unsigned int*)(gpa_vbase + GPA_DAT) & (0x1 << 11))
#endif
                                { 
                                       muteGF();
                                }
                                else
                                {
                                        unmuteGF();
                                }
#endif
                                for(i = 0; i < IBAC_DEV_ID_MAX; i++)
                                {
                                        switch(audioControlArr[i].id.logicId)
                                        {
                                                case IBAC_DEV_ID_DAI_MP3:
                                                case IBAC_DEV_ID_DAI_VIDEO:
                                                case IBAC_DEV_ID_DAI_GPS:
                                                case IBAC_DEV_ID_DAI_NEWS:
                                                case IBAC_DEV_ID_DAI_SMS:
                                                case IBAC_DEV_ID_DAI_SMSRING:
                                                case IBAC_DEV_ID_RADIO_SWITCH:
                                                {
                                                        turnSoundOffChannel(audioControlArr[i].id.logicId);
                                                        break;
                                                }
                                                default:
                                                        break;
                                        }
                                } 
#ifdef _8925_VERSION
                                usleep(500000);//for bug:playing music,while volume-value was 1,music's volume would be louder for a little time if play-warn-sound
#else
                                usleep(50);//for bug:playing music,while volume-value was 1,music's volume would be louder for a little time if play-warn-sound
#endif
                                for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
                                {
                                        switch(audioControlArr[i].id.logicId)
                                        {
                                                case IBAC_DEV_ID_MAIN_VOLUME:
                                                case IBAC_DEV_ID_DAI:
                                                        changeHardVolume(&audioControlArr[i],20);
#ifdef _8925_VERSION
                                                        for(int j = 0; j < IBAC_DEV_ID_MAX; j++)
                                                        {
                                                                if(IBAC_DEV_ID_3G_SPK_VOLUME == audioControlArr[j].id.logicId)
                                                                {
                                                                        changeHardVolume(&audioControlArr[j],20);
                                                                        break;
                                                                }
                                                        }
#endif
                                                        break;
                                                default:
                                                        break;
                                        }
                                }
                                turnSoundOnChannel(channelID);
                        }
                        break;
                }
                case IBAC_DEV_ID_DAI_RING:
                case IBAC_DEV_ID_3G_SWITCH:
                {
                        for(i = 0; i < IBAC_DEV_ID_MAX; i++)
                        {
                                switch(audioControlArr[i].id.logicId)
                                {
                                        case IBAC_DEV_ID_DAI_MP3:
                                        case IBAC_DEV_ID_DAI_VIDEO:
                                        case IBAC_DEV_ID_DAI_GPS:
                                        case IBAC_DEV_ID_DAI_NEWS:
                                        case IBAC_DEV_ID_DAI_SMS:
                                        case IBAC_DEV_ID_DAI_RING:
                                        case IBAC_DEV_ID_DAI_SMSRING:
                                        case IBAC_DEV_ID_RADIO_SWITCH:
                                        case IBAC_DEV_ID_DAI_WARNING:
                                        {
                                                turnSoundOffChannel(audioControlArr[i].id.logicId);
                                                break;
                                        }
                                        default:
                                                break;
                                }
                        } 
                        turnSoundOnChannel(channelID);
                        break;
                }
                default:
                {
                        turnSoundOnChannel(channelID);
                        break;
                }
        }

        return -1;
}
int Main_Volume::getOneChannelMuteFlag(int channelID)
{
        int i = 0;
        for(i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                if(channelID == audioControlArr[i].id.logicId)
                {
                        if(true == channelMuteState[i])
                        {
                                return 1;
                        }
                        else
                        {
                                return 0;
                        }
                }
                else
                {
                }
        }

        return -1;
}

int Main_Volume::turnSoundOnChannel(int channelID)
{
        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                if(channelID == audioControlArr[i].id.logicId)
                {
                        if(audioControlArr[i].id.numid_switch > 0)
                        {
                                return channelSwitchOn(&audioControlArr[i]);
                        }
                        else if(0 == audioControlArr[i].id.numid_switch)
                        {

#ifdef _8925_VERSION
                        if(IBAC_DEV_ID_MAIN_VOLUME == channelID || IBAC_DEV_ID_DAI == channelID)
                        {
                                for(int j = 0; j < IBAC_DEV_ID_MAX; j++)
                                {
                                        if(IBAC_DEV_ID_3G_SPK_VOLUME == audioControlArr[j].id.logicId)
                                        {
                                                channelVolumeValStored[j] = channelVolumeValStored[i];
                                                changeHardVolume(&audioControlArr[j], channelVolumeValStored[i]);
                                                break;
                                        }
                                }
                        }
#endif
                                return changeHardVolume(&audioControlArr[i], channelVolumeValStored[i]);
                        }
                        else
                        {
                        }
                }
                else
                {
                }
        }


        return -1;
}

int Main_Volume::turnSoundOffChannel(int channelID)
{
        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                if(channelID == audioControlArr[i].id.logicId)
                {
                        if(audioControlArr[i].id.numid_switch > 0)
                        {
                                return channelSwitchOff(&audioControlArr[i]);
                        }
                        else if(0 == audioControlArr[i].id.numid_switch)
                        {
                                return changeHardVolume(&audioControlArr[i], 0);
                        }
                        else
                        {
                        }
                }
                else
                {
                }
        }


        return -1;
}

int Main_Volume::restoreDefault(DBusRequestC *request, DBusReplyC *&reply)
{
        int ret = -1;
        int tmpVolume = 0;

        if(request->getIdentifier() != REQ_ID_AUDIO_RESTORE_DEFAULT)
                return -1;
        printf("[audioManager]: %s----------------------------\n",__FUNCTION__);

        reply = new AudioManagerRestoreDefaultRepC();
        ((AudioManagerRestoreDefaultRepC *)reply)->volume = volumenum;
        ((AudioManagerRestoreDefaultRepC *)reply)->mute_state = !mutestate;
        for(int i = 0; i < IBAC_DEV_ID_MAX; i++) 
        {
                switch(audioControlArr[i].id.logicId)
                {
                        case IBAC_DEV_ID_MAIN_VOLUME:
                        case IBAC_DEV_ID_DAI:
                                tmpVolume = audioControlArr[i].volume = devLogicId2CidSet[i].numid_vol;
                                channelVolumeValStored[i] = audioControlArr[i].volume;
                                ret = changeHardVolume(&audioControlArr[i],audioControlArr[i].volume);
#ifdef _8925_VERSION
				for(int j = 0; j < IBAC_DEV_ID_MAX; j++)
				{
					if(IBAC_DEV_ID_3G_SPK_VOLUME == audioControlArr[j].id.logicId)
					{
                                		channelVolumeValStored[j] = audioControlArr[i].volume;
						changeHardVolume(&audioControlArr[j], audioControlArr[i].volume);
						break;
					}
				}
#endif
                                break;
                        default:
                                break;
                }
        }
        if(ret >= 0)
        {
                ((AudioManagerRestoreDefaultRepC *)reply)->volume = tmpVolume;
                mutestate = true;
                ((AudioManagerRestoreDefaultRepC *)reply)->mute_state = false;
                ret = saveConf(tmpVolume,mutestate);
        }
        else
        {
        }
        ((AudioManagerRestoreDefaultRepC *)reply)->result = ret;

        return ret;
}

void AudioManagerServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
        if(request->getIdentifier()!=REQ_ID_AUDIO)
                return;
        AudioManagerReqC *pRequest = (AudioManagerReqC*)request;
        if(pRequest->ctl_state == 0)//get
        {
                printf("[audioManager]: AudioManagerServiceHandler,get volume.----------------------------\n");			
                AudioManagerRepC *creply = new AudioManagerRepC();
                creply->volume = Main_Volume::object()->get_main_volume();
                creply->mute_state = Main_Volume::object()->get_mute_status();
                reply = creply;
        }
        else//set
        {
                if(pRequest->volume != -1)
                {
                        Main_Volume::object()->set_main_volume(pRequest->volume);
                }
                else
                {
                        if(pRequest->mute_state == 0)
                                Main_Volume::object()->set_mute();
                        else
                                Main_Volume::object()->set_unmute();
                }
                printf("[audioManager]: AudioManagerServiceHandler,set volume.----------------------------\n");
        }
}

void AudioManagerModeServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
        return;
        if(request->getIdentifier()!=REQ_ID_AUDIOMODE)
                return;
        AudioManagerModeReqC *pRequest = (AudioManagerModeReqC*)request;
        printf("[audioManager]: AudioManagerModeServiceHandler,set mode %d .----------------------------\n",pRequest->mode);		
        Main_Volume::object()->set_mode((VOICE_MODE)pRequest->mode);
}

void AudioManagerChannelMuteStateServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
        if(request->getIdentifier() != REQ_ID_MANAGER_CHANNEL_MUTE)
                return;
        AudioManagerChannelMuteStateReqC *pRequest = (AudioManagerChannelMuteStateReqC *)request;
        printf("[audioManager]: %s----------------------------\n",__FUNCTION__);

        reply = new AudioManagerChannelMuteStateRepC();
        ((AudioManagerChannelMuteStateRepC *)reply)->channelID = pRequest->channelID;
        if(true == pRequest->mute_state)
        {
                ((AudioManagerChannelMuteStateRepC *)reply)->result = Main_Volume::object()->muteOneChannel(pRequest->channelID);
        }
        else
        {
                ((AudioManagerChannelMuteStateRepC *)reply)->result = Main_Volume::object()->unmuteOneChannel(pRequest->channelID);
        }
}
void AudioManagerRestoreDefaultServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
        Main_Volume::object()->restoreDefault(request, reply);
}

/*void *AudioManager_Thread(void *)
{
	int ret = 0, i=0;
	char buf[128];
	
	AudioManagerNotifyC notify;

	while( 1 )
	{
			pServer->sendNotify(&notify);    
	}
 
	return 0;
}*/

int sendNotifyToSyncVolume(void)
{
        AudioManagerNotifyC notify;
        int state = Main_Volume::object()->get_mute_status();
        int vol = Main_Volume::object()->get_main_volume();
        printf("[audioManager]: --------------------------------audiomanager get state %d %d .\r\n",state,vol);
        notify.volume = vol;
        notify.mute_state = state;
        pServer->sendNotify(&notify);

        return 0;
}

void AudioManagerSyncAudioServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
        if(request->getIdentifier() != REQ_ID_AUDIO_MANAGER_SYNC_AUDIO)
                return;
        printf("[audioManager]: %s----------------------------\n",__FUNCTION__);

        reply = NULL;

        sendNotifyToSyncVolume();
}

int main(int argc,char *argv[])
{
	
	DBusApplicationC app(argc, argv);


	pServer	= new DBusServerC(ServerAudioManager,ObjectAudioManager);

	pServer->registerService(REQ_ID_AUDIO,AudioManagerServiceHandler,AudioManagerReqC::factory);
	//server.registerService(REQ_ID_AUDIOMODE,AudioManagerModeServiceHandler,AudioManagerModeReqC::factory);
	pServer->registerService(REQ_ID_MANAGER_CHANNEL_MUTE,AudioManagerChannelMuteStateServiceHandler,AudioManagerChannelMuteStateReqC::factory);
	pServer->registerService(REQ_ID_AUDIO_RESTORE_DEFAULT,AudioManagerRestoreDefaultServiceHandler,AudioManagerRestoreDefaultReqC::factory);
	pServer->registerService(REQ_ID_AUDIO_MANAGER_SYNC_AUDIO,AudioManagerSyncAudioServiceHandler,AudioManagerSyncAudioReqC::factory);
    

	if(pServer->connect()<0)
	{
		printf("[audioManager]: Cann't connect to DBUS server.\n");
		return -1;
	}

        //pthread_t ptd;
        //pthread_create(&ptd, NULL, AudioManager_Thread, NULL);
        sendNotifyToSyncVolume();

        return app.loop();	
}
