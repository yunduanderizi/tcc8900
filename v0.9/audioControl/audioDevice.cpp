 // Files include
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdlib.h>
#include <QSettings>
#include <alsa/asoundlib.h>

#include "audioDevice.h"
#include "i2ccontrol.h"
#include "debugMonitor.h"


namespace AudioControl1
{





// Macro define
#define AUDIO_CHANNEL_EMPTY_NAME ""
#define AUDIO_MUTE_GF_TIME_DELAY	1000

// Some alsa chan number define
#define AUDIO_MIC_SWITCH_NUM		16
#define AUDIO_ANALOG_CHAN_NUM		6
#define AUDIO_BYPASS_SWITCH_NUM		12
#define AUDIO_DIGITAL_SWITCH_NUM	10
#define AUDIO_DIGITAL_CHAN_NUM		3

#define AUDIO_ALSA_SWITCH_ON		1
#define AUDIO_ALSA_SWITCH_OFF		0


#define AUDIO_CHECK_RANGE(val, min, max) \
	((val < min) ? (min) : (val > max) ? (max) : (val)) 
typedef AudioChannelIdE AudioChanName;


AudioDeviceC  *AudioDeviceC::m_thisObject = NULL;
AudioDeviceC  *AudioDeviceC::object()
{
	AudioDeviceC *pTmp = NULL;
		
	if( !m_thisObject )
	{
		m_thisObject = new AudioDeviceC();
	}

	pTmp = m_thisObject;
	return pTmp;
}

void  AudioDeviceC::releaseObject()
{
		
	if( NULL != m_thisObject )
	{
		delete m_thisObject;
	}
	
}

AudioDeviceC::AudioDeviceC()
{
	m_status = ACCR_MODE_STATUS_UNINIT;
	m_i2cHandler = NULL;
	m_gfOpen = false;
	
}

AudioDeviceC::~AudioDeviceC()
{
	if ( NULL != m_i2cHandler )
	{
		delete m_i2cHandler;
		m_i2cHandler = NULL;
	}
}

AudioChanCtlRet AudioDeviceC::init()
{
	AudioChanCtlRet ret;
	ret = initAlsa();

	if ( NULL == m_i2cHandler )
	{
		m_i2cHandler = new IBI2CControl();
		initChanCtlMap();
	}
	
	if ( 1 !=  m_i2cHandler->checkOK() )
	{
		return ACCR_ERR_I2C;
	}

	if ( ACCR_ERR_OK == ret )
	{
		m_status = ACCR_MODE_STATUS_INIT_OK;
	}
	else
	{
		return ret;
	}

	// init all Audio channel Volume
	QMap<AudioChannelIdE, AudioChanCtrlData>::const_iterator iter;
	for ( iter = m_auidioCtrlMap.begin(); iter != m_auidioCtrlMap.end(); iter++ )
	{
		setAudioVolume( iter.value(), (iter.value()).volume );		
	}

	// init Digital Audio total switch
	AudioChanCtrlData tmpCtl;
	memset(&tmpCtl, sizeof(tmpCtl), 0 );
	tmpCtl.alsaNumid = AUDIO_DIGITAL_CHAN_NUM;
	tmpCtl.alsaSwitchNumid = AUDIO_DIGITAL_SWITCH_NUM;
	setAudioVolume( tmpCtl, 255 );
	tmpCtl.alsaNumid = AUDIO_BYPASS_SWITCH_NUM;	// analog bypass switch
	tmpCtl.alsaSwitchNumid = AUDIO_BYPASS_SWITCH_NUM;
	setAudioVolume( tmpCtl, AUDIO_ALSA_SWITCH_ON );	//nonzero : switch on  zero: switch off


	// muteGF
	closeGF();
	
	return ACCR_ERR_OK;
	
}

AudioChanCtlRet AudioDeviceC::openGF()
{
	AudioChanCtlRet ret;
	ret = checkModeStatus();
	if ( ACCR_MODE_STATUS_INIT_OK != m_status )
	{
		return ret;
	}

	ret = setGFStatus(true);


	if ( ACCR_ERR_OK == ret )
	{
		printf("[Audio] Open GF OK !\n");
		m_gfOpen = true;
	}
	else
	{
		printf("[Audio] Open GF Failed !\n");

	}	
	return ret;
}

AudioChanCtlRet AudioDeviceC::closeGF()
{
	AudioChanCtlRet ret;
	ret = checkModeStatus();
	if ( ACCR_MODE_STATUS_INIT_OK != m_status )
	{
		return ret;
	}

	ret = setGFStatus(false);

	if ( ACCR_ERR_OK == ret )
	{
		printf("[Audio]  Close GF OK !\n");
		m_gfOpen = false;
	}
	else
	{
		printf("[Audio] close GF Failed !\n");

	}	
	return ret;
}

AudioChanCtlRet AudioDeviceC::setGFStatus( bool gfOpen )
{
	int err = -1;
	int nCount = 3;
	char i2cAddr = 0x6c;
	unsigned char nState;
	char oper = 0x1f;
	if ( gfOpen )
	{
		oper = 0x00;
	}
	printf("[Audio] setGFStatus [%d] !\n", gfOpen );

	if( m_i2cHandler->checkOK() == 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Check i2cHander err!\n");
		return ACCR_ERR_I2C; 
	}
	err = m_i2cHandler->writeByte( i2cAddr, 0x0C, oper );
	if(err < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "I2c write byte err: %d\n", err);
		return ACCR_ERR_I2C;
	}
	
    usleep(AUDIO_MUTE_GF_TIME_DELAY);
	err = m_i2cHandler->readByte(i2cAddr, 0x06, nState);
	if(err < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "I2c read byte err: %d\n", err);
		return ACCR_ERR_GF;
	}
    while( /*(nState != 0x0F)  &&*/ (nCount-- > 0) )
	{
		usleep(AUDIO_MUTE_GF_TIME_DELAY);
		err = m_i2cHandler->readByte(i2cAddr, 0x06, nState);
		if(err < 0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "I2c read byte err: %d\n", err);
			return ACCR_ERR_GF;
		}
		else
		{
			if ( (nState == 0x0F &&  gfOpen )
				|| ( nState == 0xF0 &&  (!gfOpen) ))
			{
				break;
			}

		}
	}
	printf("[Audio] setGFStatus [%d] ret[%d] !\n", gfOpen, nState );
	if ( (nState == 0x0F &&  gfOpen )
		|| ( nState == 0xF0 &&  (!gfOpen) ))
	{
		return ACCR_ERR_OK;
	}
	else
	{
		return ACCR_ERR_GF;
	}
    
}

AudioChanCtlRet AudioDeviceC::setAudioChanVolume( AudioChanName chanName, int vol )
{
	AudioChanCtlRet ret;
	
	//IBD_PRINTF(DEBUG_INFOMATION, "setAudioChanVolume  ChanName:%d  Vol:%d !\n", chanName, vol);
	printf("\n[Audio] setAudioChanVolume  ChanName:%d  Vol:%d !\n", chanName, vol);

	// check Argument
	if ( vol < ACCR_CHAN_VOLUME_MIN ||  vol > ACCR_CHAN_VOLUME_MAX )
	{
		return ACCR_ERR_ARG_INVALID;
	}
	

	// check Mode status	
	ret = checkModeStatus();
	if ( ACCR_MODE_STATUS_INIT_OK != m_status )
	{
		printf("[Audio] setAudioChanVolume checkModeStatus Error  \n");
		return ret;
	}
	
	if ( vol == (m_auidioCtrlMap.value(chanName)).volume )
	{
		printf("[Audio] setAudioChanVolume Repeat operation Ignore \n");
		return ACCR_ERR_OK;
	}

	int alsaVolume = convertVolume( chanName, vol );
	printf("[Audio] 222 setAudioChanVolume  ChanName:%d  Vol:%d  Alsa:%d !\n", chanName, vol, alsaVolume);
	// set Audio channel volume
	if ( AUDIO_CHANNEL_ID_MAIN == chanName )
	{
		ret = setAudioVolume( m_auidioCtrlMap.value(chanName), alsaVolume );

		// sync the right and left Volume value
		if ( ACCR_ERR_OK == ret )
		{
			AudioChanCtrlData tmp = m_auidioCtrlMap.value(AUDIO_CHANNEL_ID_MAIN_LEFT);
			m_auidioCtrlMap.remove(AUDIO_CHANNEL_ID_MAIN_LEFT);
			tmp.volume = vol;
			m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_MAIN_LEFT, tmp);
			tmp = m_auidioCtrlMap.value(AUDIO_CHANNEL_ID_MAIN_RIGHT);
			m_auidioCtrlMap.remove(AUDIO_CHANNEL_ID_MAIN_RIGHT);
			tmp.volume = vol;
			m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_MAIN_RIGHT, tmp);
		}

	}else if ( AUDIO_CHANNEL_ID_MAIN_LEFT == chanName \
		|| AUDIO_CHANNEL_ID_MAIN_RIGHT == chanName )
	{
		int rlVol = (m_auidioCtrlMap.value(AUDIO_CHANNEL_ID_MAIN)).volume * vol \
			/( ACCR_CHAN_VOLUME_MAX - ACCR_CHAN_VOLUME_MIN );
		alsaVolume = convertVolume( chanName, rlVol );
		ret = setAudioVolume( m_auidioCtrlMap.value(chanName), alsaVolume );
	}
	else
	{
		ret = setAudioVolume( m_auidioCtrlMap.value(chanName), alsaVolume );
	}
	// save the volume setting
	if ( ACCR_ERR_OK == ret )
	{
		AudioChanCtrlData tmp = m_auidioCtrlMap.value(chanName);
		m_auidioCtrlMap.remove(chanName);
		tmp.volume = vol;
		m_auidioCtrlMap.insert(chanName, tmp);

	}
	else
	{
		return ret;
	}

	// refresh GF state
	bool gfNeedOpen = true;
	if ( AUDIO_CHANNEL_ID_MAIN == chanName \
		|| AUDIO_CHANNEL_ID_MAIN_LEFT == chanName	\
		|| AUDIO_CHANNEL_ID_MAIN_RIGHT == chanName )
	{
		if ( (m_auidioCtrlMap.value(AUDIO_CHANNEL_ID_MAIN)).volume == ACCR_CHAN_VOLUME_MIN	\
			&& (m_auidioCtrlMap.value(AUDIO_CHANNEL_ID_MAIN_LEFT)).volume == ACCR_CHAN_VOLUME_MIN	\
			&& (m_auidioCtrlMap.value(AUDIO_CHANNEL_ID_MAIN_RIGHT)).volume == ACCR_CHAN_VOLUME_MIN	)
		{
			gfNeedOpen = false;
		}
		else 
		{
			gfNeedOpen = true;
		}
		
		ret = checkGFStatus( gfNeedOpen );
		printf("========================= Open GF bool:%d  old[%d]=========================!\n", gfNeedOpen, m_gfOpen );
	}
	//checkGFStatus( true );
	printf("[Audio] Finish setAudioVolume  return Value:%d !\n\n", ret);
	return ret;
	
}

AudioChanCtlRet AudioDeviceC::setAudioVolume( AudioChanCtrlData chanCtrlData, int vol )
{
	AudioChanCtlRet ret;
	ret = setAlsaVolume( chanCtrlData, vol );

	AudioChanCtrlData tmpCtl;
	memset(&tmpCtl, sizeof(tmpCtl), 0 );
	if ( AUDIO_ANALOG_CHAN_NUM == chanCtrlData.alsaNumid )
	{

		tmpCtl.alsaNumid = chanCtrlData.alsaSwitchNumid;
		tmpCtl.alsaSwitchNumid= chanCtrlData.alsaSwitchNumid;
		if (  vol > ACCR_CHAN_VOLUME_MIN /*&&  (m_auidioCtrlMap.value(chanCtrlData.logicId)).volume == 0*/ )
		{
			printf("[Audio] setAudioVolume Analog open! switch[%d]\n", chanCtrlData.alsaSwitchNumid);
			setAlsaVolume( tmpCtl, AUDIO_ALSA_SWITCH_ON );

		}
		else if ( vol == ACCR_CHAN_VOLUME_MIN /*&&  (m_auidioCtrlMap.value(chanCtrlData.logicId)).volume > 0*/ )
		{
			printf("[Audio] setAudioVolume Analog close! switch[%d]\n", chanCtrlData.alsaSwitchNumid);
			setAlsaVolume( tmpCtl, AUDIO_ALSA_SWITCH_OFF );

		}

		//
		if ( AUDIO_MIC_SWITCH_NUM == chanCtrlData.alsaSwitchNumid )
		{
			tmpCtl.alsaNumid = AUDIO_BYPASS_SWITCH_NUM;
			tmpCtl.alsaSwitchNumid= AUDIO_BYPASS_SWITCH_NUM;
			if (  vol > ACCR_CHAN_VOLUME_MIN )
			{
				printf("[Audio]  Bypass close! switch[%d]\n", chanCtrlData.alsaNumid);
				setAlsaVolume( tmpCtl, AUDIO_ALSA_SWITCH_OFF);

			}
			else if ( vol == ACCR_CHAN_VOLUME_MIN )
			{
				printf("[Audio]  Bypass open! switch[%d]\n", chanCtrlData.alsaNumid);
				setAlsaVolume( tmpCtl, AUDIO_ALSA_SWITCH_ON );

			}



		}
	}
	if ( AUDIO_ANALOG_CHAN_NUM == chanCtrlData.alsaNumid && AUDIO_MIC_SWITCH_NUM == chanCtrlData.alsaSwitchNumid )
	{
		tmpCtl.alsaNumid = 0;
		tmpCtl.alsaSwitchNumid= 0;
		strcpy(tmpCtl.chanName,"tccvol");
		if ( ACCR_CHAN_VOLUME_MIN == vol )
		{
			AudioChanCtrlData tmp = m_auidioCtrlMap.value(AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP);
			setAlsaVolume( tmpCtl, convertVolume(AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP, tmp.volume));
		}
		else
		{
			setAlsaVolume( tmpCtl,  220);
		}

	}
	return ret;
}

AudioChanCtlRet AudioDeviceC::setAlsaVolume( AudioChanCtrlData chanCtrlData, int vol )
{
	printf("[Audio] setAudioVolume  ChanName:%s Alsa id:%d Vol:%d !\n", chanCtrlData.chanName, chanCtrlData.alsaNumid, vol);
	long volume = (long)vol;
	int numid = chanCtrlData.alsaNumid;
	int err;
	unsigned int idx, count;
	long tmp = 0;
	char card[24] = "default";
	
	snd_ctl_t *handle = NULL;
	snd_ctl_elem_info_t *info;
	snd_ctl_elem_id_t *id;
	snd_ctl_elem_value_t *control;
	snd_ctl_elem_type_t type;
	snd_ctl_elem_info_alloca(&info);
	snd_ctl_elem_id_alloca(&id);
	snd_ctl_elem_value_alloca(&control);
	

	// if numid is 0 or name is NULL, device is not available, return
	if (numid <= 0 && !strcmp(chanCtrlData.chanName, AUDIO_CHANNEL_EMPTY_NAME))
	{		
		IBD_PRINTF(4, "[Audio Control]: Set Audio --> Wrong control identifier: %d\n", numid);
		return ACCR_ERR_INTER;
	}
	snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);	// default

	if (numid > 0 && numid <= AUDIO_CHANNEL_ID_MAX)
	{
		snd_ctl_elem_id_set_numid(id, numid);		// set the numid to id->numid
	}
	else if (strcmp(chanCtrlData.chanName, AUDIO_CHANNEL_EMPTY_NAME) > 0)
	{
		snd_ctl_elem_id_set_name(id, chanCtrlData.chanName);
	}

	err = snd_ctl_open(&handle, card, 0);
	if (handle == NULL || err < 0 )
	{
		IBD_PRINTF(4, "[Audio Control]: Set Audio --> Control %s open error: %s\n", card, snd_strerror(err));
		return ACCR_ERR_ALSA;
	}
	
	snd_ctl_elem_info_set_id(info, id);		// info->id = id

	// get info details from drivers
	err = snd_ctl_elem_info(handle, info);
	if ( err < 0 ) 		
	{	// handle->ops->element_info(handle, info);
		IBD_PRINTF(4, "[Audio Control]: Set Audio --> Cannot find the given element from control %s, name = %s, id = %d\n", card, chanCtrlData.chanName, numid);
		IBD_PRINTF(4, "*** pb_error: %s\n", snd_strerror(err));		
		snd_ctl_close(handle);
		handle = NULL;
		return ACCR_ERR_ALSA;
	}
	
	snd_ctl_elem_info_get_id(info, id); // FIXME: Remove it when hctl find works ok !!!

	type = snd_ctl_elem_info_get_type(info);	// type = info->type
	count = snd_ctl_elem_info_get_count(info);	// count = info->count
	snd_ctl_elem_value_set_id(control, id);		// control->id = id
	
	//IBD_PRINTF(4, "\n [Audio Control]: Set Audio --> numid %d --> # of controls %d\n", numid, count);


	for (idx = 0; idx < count && idx < 128; idx++) 
	{
		switch (type) 
		{
			case SND_CTL_ELEM_TYPE_BOOLEAN:
				tmp = volume > 0 ? 1 : 0;
				snd_ctl_elem_value_set_boolean(control, idx, tmp);		// control->value.integer.value[idx] = tmp;
				break;
			case SND_CTL_ELEM_TYPE_INTEGER:
                                tmp = volume;
				snd_ctl_elem_value_set_integer(control, idx, tmp);	// control->value.integer.value[idx] = tmp;
				break;
			case SND_CTL_ELEM_TYPE_INTEGER64:
                                tmp = volume;
				snd_ctl_elem_value_set_integer64(control, idx, tmp);	// control->value.integer64.value[idx] = tmp;
				break;
			case SND_CTL_ELEM_TYPE_ENUMERATED:
				volume = (unsigned)(volume < 0  ? 0 : volume);
				tmp = snd_ctl_elem_info_get_items(info) - 1;
				tmp = AUDIO_CHECK_RANGE(volume, 0, tmp);
				snd_ctl_elem_value_set_enumerated(control, idx, tmp);	// control->value.enumerated.item[idx] = val;
				break;
			case SND_CTL_ELEM_TYPE_BYTES:
				//tmp = get_integer(volume, 0, 255);
				tmp = vol;
				snd_ctl_elem_value_set_byte(control, idx, tmp);	// control->value.bytes.data[idx] = val;
				break;
			default:
				break;
		}
	}

	// handle->ops->element_write(handle, control)
	err = snd_ctl_elem_write(handle, control);
	if ( err < 0 ) 	
	{		
		IBD_PRINTF(4, "[Audio Control]: Set Audio --> Control %s element write error: %s\n", card, snd_strerror(err));		
		snd_ctl_close(handle);
		handle = NULL;
		return ACCR_ERR_ALSA;
	}

	snd_ctl_close(handle);
	handle = NULL;
	return ACCR_ERR_OK;
}


AudioChanCtlRet AudioDeviceC::checkModeStatus()
{
	AudioChanCtlRet ret ;
	if ( ACCR_MODE_STATUS_INIT_OK != m_status )
	{
		ret = init();
	}
	else
	{
		ret = ACCR_ERR_OK;
	}
	return ret;
}

AudioChanCtlRet AudioDeviceC::initAlsa()
{

	const unsigned int plugChanNum = 22;
	const char *plugName[plugChanNum] = 
	{
		"plug:mp3",  //IBAC_DEV_ID_DAI_MP3
		"plug:video",  //IBAC_DEV_ID_DAI_VIDEO
		"plug:webradio",  //IBAC_DEV_ID_DAI_NETR
		"plug:gps",  //IBAC_DEV_ID_DAI_GPS
		"plug:gpsringvol",  //IBAC_DEV_ID_DAI_GPS_RING
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
	for (i = 0; i< (int)plugChanNum; i++) 
	{
		snd_pcm_t* handle;
	 	if ( (err = snd_pcm_open(&handle, plugName[i], SND_PCM_STREAM_PLAYBACK, 0)) < 0)
		{
		        IBD_PRINTF(4, "*** snd_pcm_open %s failed!\n", plugName[i]);
				return ACCR_ERR_ALSA;
				//continue;
		}
		snd_pcm_drain(handle);
		snd_pcm_close(handle);
	}
	return ACCR_ERR_OK;;
	
}

AudioChanCtlRet AudioDeviceC::checkGFStatus( bool gfOpen )
{
	AudioChanCtlRet ret;

	if ( m_gfOpen == gfOpen )
	{
		return ACCR_ERR_OK;
	}

	if ( gfOpen )
	{
		ret = openGF();
	}
	else
	{
		ret = closeGF();
	}
	return ret; 
}

// assert volume range is valid
int AudioDeviceC::convertVolume( AudioChanName chanName, int volume )
{
	int ret = ACCR_CHAN_VOLUME_MIN + 1;
	int maxVol = 255;
	int minVol = 0;

	switch ( chanName )
	{
	case AUDIO_CHANNEL_ID_MAIN:
	{
		maxVol = 127;
		minVol = 0;
		break;
	}
	default:
	{
		break;
	}
	}

	double tmpVol = ( (volume - ACCR_CHAN_VOLUME_MIN) \
		* (double)(maxVol - minVol) )	\
		/ (ACCR_CHAN_VOLUME_MAX - ACCR_CHAN_VOLUME_MIN) + minVol;
	ret = round(tmpVol);
	printf("[Audio] Volume Convert [%d] vol[%d] ---> [%d]  \n", chanName,volume,ret);
	return ret;
	

}

void AudioDeviceC::initChanCtlMap()
{
	AudioChanCtrlData tmp={AUDIO_CHANNEL_ID_MAIN,1,0,"",0};
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_MAIN, tmp);

	// init volume for all channels
	tmp.volume = ACCR_CHAN_VOLUME_MIN;
	/*
	
	tmp.logicId= AUDIO_CHANNEL_ID_DAI;
	tmp.alsaNumid = 3;
	tmp.alsaSwitchNumid = 10;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI, tmp);
	*/

	tmp.logicId= AUDIO_CHANNEL_ID_ANALOG_RADIO;
	tmp.alsaNumid = 6;
	tmp.alsaSwitchNumid = 13;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_ANALOG_RADIO, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_ANALOG_TV;
	tmp.alsaNumid = 6;
	tmp.alsaSwitchNumid = 14;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_ANALOG_TV, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_ANALOG_PHONE;
	tmp.alsaNumid = 6;
	tmp.alsaSwitchNumid = 15;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_ANALOG_PHONE, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_ANALOG_MIC;
	tmp.alsaNumid = 6;
	tmp.alsaSwitchNumid = 16;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_ANALOG_MIC, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_ANALOG_BLUETOOTH;
	tmp.alsaNumid = 6;
	tmp.alsaSwitchNumid = 17;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_ANALOG_BLUETOOTH, tmp);

	/*
	tmp.logicId= AUDIO_CHANNEL_ID_CD_DVD;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 11;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_CD_DVD, tmp);

	
	tmp.logicId= AUDIO_CHANNEL_ID_WALKI_TALKI;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"wkitki");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_WALKI_TALKI, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_BYPASS_MX;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 12;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_BYPASS_MX, tmp);
	*/

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_MPLAYER;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"videovol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_MPLAYER, tmp);

	/*
	tmp.logicId= AUDIO_CHANNEL_ID_DAI_NETR;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"webradiovol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI_NETR, tmp);
	*/

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_GPS;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"gpsvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_GPS, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_GPS_RING;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"gpsringvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_GPS_RING, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_NEWS;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"newsvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_NEWS, tmp);

	/*
	tmp.logicId= AUDIO_CHANNEL_ID_DAI_CARINFO;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"carinfovol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI_CARINFO, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DAI_LIFEINFO;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"lifeinfovol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI_LIFEINFO, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DAI_COMMAND;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"cmdvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI_COMMAND, tmp);
	*/
	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"tccvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_VOICE_IDENTIFY;
	tmp.alsaNumid = 6;
	tmp.alsaSwitchNumid = 16;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_VOICE_IDENTIFY, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_VOICE_INPUT;
	tmp.alsaNumid = 6;
	tmp.alsaSwitchNumid = 16;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_VOICE_INPUT, tmp);
	

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_ANNOUNCE;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"anucvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_ANNOUNCE, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_RING;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"ringvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_RING, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_VPHONE;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"vphonevol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_VPHONE, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_KEYTONE;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"keytonevol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_KEYTONE, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_ILLEGALTIPS;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"illegaltipsvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_ILLEGALTIPS, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_SMS;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"smsvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_SMS, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DIGITAL_SMSRING;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"smsringvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DIGITAL_SMSRING, tmp);

	/*
	tmp.logicId= AUDIO_CHANNEL_ID_DAI_TONE;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"tonevol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI_TONE, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DAI_DAI_VTALKI;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"vtalkivol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI_DAI_VTALKI, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DAI_DAI_MONLINE;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"monlinevol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI_DAI_MONLINE, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DAI_DAI_VONLINE;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"vonlinevol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI_DAI_VONLINE, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DAI_DAI_LBOOK;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"lbookvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI_DAI_LBOOK, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_DAI_DAI_MTRAFFIC;
	tmp.alsaNumid = 0;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,"mtrafficvol");
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_DAI_DAI_MTRAFFIC, tmp);
	*/
	
	

	tmp.logicId= AUDIO_CHANNEL_ID_MAIN_LEFT;
	tmp.alsaNumid = 20;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_MAIN_LEFT, tmp);

	tmp.logicId= AUDIO_CHANNEL_ID_MAIN_RIGHT;
	tmp.alsaNumid = 21;
	tmp.alsaSwitchNumid = 0;
	strcpy(tmp.chanName,AUDIO_CHANNEL_EMPTY_NAME);
	m_auidioCtrlMap.insert(AUDIO_CHANNEL_ID_MAIN_RIGHT, tmp);

}



}



