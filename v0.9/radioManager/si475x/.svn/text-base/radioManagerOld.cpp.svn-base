#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdlib.h>
#include "radioDefines.h"
#include <QSettings>
#include "radioManagerOld.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "typedefs.h"
#include <unistd.h>

void wait_us(u16 us);
void wait_ms(u16 ms);
//void insert_preset(u16 frequency, u8 rssi, u8 *seek_preset_rssi);
#define NUM_SEEK_PRESETS 15

#include "si47xxFMRX.h"
#include "si47xxAMRX.h"
//#include "FMRXtest.h"

#ifdef __cplusplus
}
#endif



//#define RADIO_TDA7541_QSD

#define RM_STATUS_INIT   	0
#define RM_STATUS_OPENED	1
#define RM_STATUS_LISTEN 	2
#define RM_STATUS_SEARCH 	3

/*
#define RM_FM_SEARCH_INTERVAL	40
#define RM_AM_SEARCH_INTERVAL	130
#define RM_SEARCHED_INTERVAL	0
*/
#define RM_FM_SEARCH_INTERVAL	500
#define RM_AM_SEARCH_INTERVAL	500
#define RM_SEARCHED_INTERVAL	500

#define RM_FM_METER_THRESHOLD	61
#define RM_AM_METER_THRESHOLD	61

#define RM_I2C_ADDR	 0X62
#define EEPROM_I2C_ADDR	0X50

#define RADIO_GET_SMETER  0x66666661

#define RADIO_CONFIG_FILE_PATHNAME		"/nand3/applicationdata/radio.ini"



RadioManagerOld::RadioManagerOld()
{
	status =RM_STATUS_INIT;
	volume =RM_VOLUME_MAX;

	listenedChannel =0;
	
	m_voiceSwithFunction =NULL;
	m_voiceSwithUserData =NULL;
	m_voiceStatus =RM_VOICE_SWITCH_OFF;


	timeId=-1;
}
RadioManagerOld::~RadioManagerOld()
{
	if(status !=RM_STATUS_INIT)
	{
		closeRadio();
	}
}

int RadioManagerOld::openRadio(int frequencySection,int channel,int volumeArg)
{

	int  searchFlag =0;
	if(channel ==0)
	{
		searchFlag =1;
		
		if(m_voiceSwithFunction !=NULL)
		{
			m_voiceSwithFunction(RM_VOICE_SWITCH_OFF,m_voiceSwithUserData);
			m_voiceStatus =RM_VOICE_SWITCH_OFF;
		}

		if(frequencySection==RM_FREQ_SECTION_FM)
		{
			channel =RM_FM_FREQ_MIN;
		}
		else
		{
			frequencySection =RM_FREQ_SECTION_AM;
			channel     	 =RM_AM_FREQ_MIN;
		}
	}
	else
	{
		if(frequencySection == RM_FREQ_SECTION_FM)
		{
			if(channel<RM_FM_FREQ_MIN || channel>RM_FM_FREQ_MAX)
				return RM_R_ERROR_RANGE;
		}
		else
		{
			if(channel<RM_AM_FREQ_MIN || channel>RM_AM_FREQ_MAX)
				return RM_R_ERROR_RANGE;

			frequencySection =RM_FREQ_SECTION_AM;
		}
			
	}
	

	int result =openRadioAtChannel(frequencySection,channel,searchFlag);

	if(result !=RM_R_OK)
		return result;

	adjustVolume(volumeArg,RM_VOLUME_SET);

	if(searchFlag!=0)
	{
		status =RM_STATUS_SEARCH;

		listenedChannel =0;
		lastSearchChannel     =0;
		currentSearchChannel =channel;

		channelMeter 	=0;
		searchedChannel =0;
		meterIsUping	=0;

		if(frequencySection == RM_FREQ_SECTION_FM)
			timeId =startTimer(RM_FM_SEARCH_INTERVAL);
		else
			timeId =startTimer(RM_AM_SEARCH_INTERVAL);

		emit searchChannelChanged(lastSearchChannel,currentSearchChannel);
	}
	else
	{
		listenedChannel =channel;
		status =RM_STATUS_LISTEN;
	}

	return RM_R_OK;
}

int RadioManagerOld::closeRadio()
{
	if(status == RM_STATUS_INIT)
		return RM_R_OK;


	if(status ==RM_STATUS_SEARCH)
	{
		killTimer(timeId);
		status =RM_STATUS_INIT;
        //printf("[radio][%4d] ~~~~~~~~~ close radio searchEnd cause[%d]   \n", __LINE__, RM_SEARCH_R_CANCEL);
		emit searchChannelChanged(currentSearchChannel,0);
		emit searchEnd(RM_SEARCH_R_CANCEL);
		
	}
	else
	{
		status =RM_STATUS_INIT;
	}
	return RM_R_OK;
}


int RadioManagerOld::adjustVolume(int volumeArg,int mode)
{
	switch(mode)
	{
		case RM_VOLUME_SET:
			volume =volumeArg;
			break;
		case RM_VOLUME_INC:
			volume +=volumeArg;
			break;
		case RM_VOLUME_DEC:
			volume -=volumeArg;
			break;
		default:	
			return RM_R_ERROR_MODE;
	}

	if(volume<RM_VOLUME_MIN)
		volume =RM_VOLUME_MIN;
	else if(volume>RM_VOLUME_MAX)
		volume =RM_VOLUME_MAX;

	return RM_R_OK;
}

int RadioManagerOld::getVolume()
{
	return volume;
}


int RadioManagerOld::changeChannel(int channel)
{
	int	section;
	if(channel >=RM_AM_FREQ_MIN && channel<=RM_AM_FREQ_MAX)
		section=RM_FREQ_SECTION_AM;
	else if(channel >=RM_FM_FREQ_MIN && channel<=RM_FM_FREQ_MAX)
		section=RM_FREQ_SECTION_FM;
	else
		return RM_R_ERROR_RANGE;
			
	listenedChannel =channel;

	if(status !=RM_STATUS_LISTEN || freqSection!=section)
	{
		return openRadioAtChannel(section,channel,0);
	}

	if(freqSection==RM_FREQ_SECTION_FM)
		selectFMChannel(channel,0);
	else
		selectAMChannel(channel,0);

	return RM_R_OK;
}


int RadioManagerOld::beginSearch(int channel)
{


  
	int	section;
	if(channel >=RM_AM_FREQ_MIN && channel<=RM_AM_FREQ_MAX)
		section=RM_FREQ_SECTION_AM;
	else if(channel >=RM_FM_FREQ_MIN && channel<=RM_FM_FREQ_MAX)
		section=RM_FREQ_SECTION_FM;
	else
		return RM_R_ERROR_RANGE;
			
	if(m_voiceSwithFunction !=NULL)
	{
		m_voiceSwithFunction(RM_VOICE_SWITCH_OFF,m_voiceSwithUserData);
		m_voiceStatus =RM_VOICE_SWITCH_OFF;
	}

	if(status !=RM_STATUS_SEARCH || freqSection!=section)
	{
		int result =openRadioAtChannel(section,channel,1);

		if(result !=RM_R_OK)
                	return result;

                status =RM_STATUS_SEARCH;

                listenedChannel =0;
                lastSearchChannel     =0;
                currentSearchChannel =channel;

		channelMeter 	=0;
		searchedChannel =0;
		meterIsUping	=0;

		if(freqSection == RM_FREQ_SECTION_FM)
			timeId =startTimer(RM_FM_SEARCH_INTERVAL);
		else
			timeId =startTimer(RM_AM_SEARCH_INTERVAL);

                emit searchChannelChanged(lastSearchChannel,currentSearchChannel);

		return RM_R_OK;
    }

	killTimer(timeId);

	lastSearchChannel    =currentSearchChannel;
	currentSearchChannel =channel;

	emit searchChannelChanged(lastSearchChannel,currentSearchChannel);

	channelMeter 	=0;
	searchedChannel =0;
	meterIsUping	=0;

	if(freqSection == RM_FREQ_SECTION_FM)
		timeId =startTimer(RM_FM_SEARCH_INTERVAL);
	else
		timeId =startTimer(RM_AM_SEARCH_INTERVAL);

	return RM_R_OK;
}

int RadioManagerOld::stopSearch()
{
    //printf("[radio][%4d] ~~~~~~~~~~~~~~~~~stopSearch  \n", __LINE__, RM_SEARCH_R_CANCEL);
	if(status !=RM_STATUS_SEARCH)
		return RM_R_OK;

	killTimer(timeId);
	

	status = RM_STATUS_LISTEN;
	if(listenedChannel !=0)
	{
		if(freqSection ==RM_FREQ_SECTION_FM)
			selectFMChannel(listenedChannel,0);
		else
			selectAMChannel(listenedChannel,0);
		
	}
	else
	{
		closeRadio();
	}
	
	emit searchChannelChanged(currentSearchChannel,0);
    //printf("[radio][%4d] ~~~~~~~~~~~~~~~~~searchEnd cause[%d]   \n", __LINE__, RM_SEARCH_R_CANCEL);
	emit searchEnd(RM_SEARCH_R_CANCEL);

	return RM_R_OK;
}

void RadioManagerOld::setVoiceSwitchFunction(RMVoiceSwitchFunction /*voiceSwithFunction*/,void */*userData*/)
{
/*
	m_voiceSwithFunction =voiceSwithFunction;
	m_voiceSwithUserData =userData;
*/
}

int RadioManagerOld::openRadioAtChannel(int frequencySection,int channel,char searchFlag)
{
	if(status ==RM_STATUS_LISTEN || status ==RM_STATUS_SEARCH)
	{
		
		if(status ==RM_STATUS_SEARCH)
		{
			killTimer(timeId);
            //printf("[radio][%4d] ~~~~~~~~~~~~~~~~~searchEnd cause[%d]   \n", __LINE__, RM_SEARCH_R_CANCEL);
			emit searchChannelChanged(currentSearchChannel,0);
			emit searchEnd(RM_SEARCH_R_CANCEL);
		}

		status =RM_STATUS_OPENED;
	}

    //printf("-++++++++++++++ open Radio start +++++++++++++++-\n");

	int rssi;
	int snr;
	
	if(frequencySection==RM_FREQ_SECTION_FM)
	{
		rssi = 20;
		snr = 3;
		readParameterConfig(RM_FREQ_SECTION_FM, &rssi, &snr);
        si47xxAMRX_powerdown();
        printf("------->[%s] FM rssi:%d snr:%d  +++++++++++++++-\n",__func__,rssi,snr);
	    si47xxFMRX_initialize( rssi, snr );
        si47xxFMRX_set_volume(30);     // full volume, turn off mute
	}
	else
	{
		rssi = 25;
		snr = 5;
		readParameterConfig(RM_FREQ_SECTION_AM, &rssi, &snr);
        si47xxFMRX_powerdown();
        printf("------->[%s] AM rssi:%d snr:%d  +++++++++++++++-\n",__func__,rssi,snr);
		si47xxAMRX_initialize( rssi, snr );
        si47xxAMRX_set_volume(30);     // full volume, turn off mute
    	
	}
	wait_ms(RM_SEARCHED_INTERVAL);



	if(frequencySection==RM_FREQ_SECTION_FM)
	{
		freqSection =RM_FREQ_SECTION_FM;
        //printf("~~~~~~~~~~~~~~~~~~~~~~~~ select one FM chanel  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		selectFMChannel(channel,searchFlag);

	}
	else
	{
		freqSection =RM_FREQ_SECTION_AM;
        //printf("~~~~~~~~~~~~~~~~~~~~~~~~ select one AM chanel start ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		selectAMChannel(channel,searchFlag );
        //printf("~~~~~~~~~~~~~~~~~~~~~~~~ select one AM chanel over ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	}

	status =RM_STATUS_LISTEN;

	return RM_R_OK;
}

void RadioManagerOld::searchFMChannel(int /*channel*/,char/* searchFlag*/)
{

}


void RadioManagerOld::selectFMChannel(int channel,char /*searchFlag*/)
{

	si47xxFMRX_tune(channel/10);		// abcdefg
}

void RadioManagerOld::searchAMChannel(int /*channel*/,char /*searchFlag*/)
{
		
}


void RadioManagerOld::selectAMChannel(int channel,char /*searchFlag*/)
{
	si47xxAMRX_tune(channel); 
}

void RadioManagerOld::timerEvent(QTimerEvent */*event*/)
{

    //printf("~~~~~~~~~~~~~~~~~~~~~~~~ timerEvent\n");
	killTimer(timeId);

	if(searchedChannel>0)
	{
		searchedChannel =0;

		searchNextChannel();
		return;
	}

	
	u8 seek_fail;
	u16 current_frequency;
	if ( RM_FREQ_SECTION_FM == freqSection )
	{


        seek_fail = si47xxFMRX_seek(1, 1); // seek up, 0 = stop at band limits
		current_frequency = si47xxFMRX_get_frequency();
        //u8 rssiMy = si47xxFMRX_get_rssi();

		currentSearchChannel = current_frequency*10;	// abcdefg
	}
	else
	{

        seek_fail = si47xxAMRX_seek(1, 0); // seek up, 0 = wrap at band limits
		current_frequency = si47xxAMRX_get_frequency();
        //u8 rssiMy = si47xxAMRX_get_rssi();

		currentSearchChannel = current_frequency;	// abcdefg
		if ( currentSearchChannel < lastSearchChannel)
		{
			seek_fail = 1;
		}


	}

	///判断是否有效台
    //printf("~~~~~~~~~~~~~~~~~~~~~~~~ searched result[%d] current_frequency[%d] last[%d]\n", seek_fail,current_frequency,lastSearchChannel);
	
	//printf("channel[%d]  sstop.ifc [%d] flag[0x%x]\n", currentSearchChannel, sstop.ifc, readvalue );

	if ( RM_FREQ_SECTION_FM == freqSection )
	{
		//if( result >= 0 && sstop.ifc == 1 && ( !(readvalue & 0x1)) )	
		if ( seek_fail ==  0 )
		{
			if(m_voiceSwithFunction!=NULL)
			{
				m_voiceSwithFunction(RM_VOICE_SWITCH_ON,m_voiceSwithUserData);
				m_voiceStatus =RM_VOICE_SWITCH_ON;
			}

			searchedChannel =currentSearchChannel;
			listenedChannel =currentSearchChannel;

			timeId =startTimer(RM_SEARCHED_INTERVAL);
			emit searchedOneChannel(searchedChannel);


		}
		else
		{
			currentSearchChannel = RM_FM_FREQ_MAX;
			searchNextChannel();
		}
	}
	else
	{
		if ( seek_fail ==  0  )
		{
			if(m_voiceSwithFunction!=NULL)
			{
				m_voiceSwithFunction(RM_VOICE_SWITCH_ON,m_voiceSwithUserData);
				m_voiceStatus =RM_VOICE_SWITCH_ON;
			}

			searchedChannel =currentSearchChannel;
			listenedChannel =currentSearchChannel;

			timeId =startTimer(RM_SEARCHED_INTERVAL);
			emit searchedOneChannel(searchedChannel);


		}
		else
		{	
			currentSearchChannel = RM_AM_FREQ_MAX + 10000;
			searchNextChannel();
		}

	}
}

void RadioManagerOld::searchNextChannel()
{
	int nextSearchInterval =RM_AM_SEARCH_INTERVAL;
	if(freqSection == RM_FREQ_SECTION_FM)
		nextSearchInterval =RM_FM_SEARCH_INTERVAL;

	lastSearchChannel =currentSearchChannel;

	if(freqSection ==RM_FREQ_SECTION_FM)
	{
		currentSearchChannel +=RM_FM_FREQ_STEP;
		if(currentSearchChannel>RM_FM_FREQ_MAX)
		{
			currentSearchChannel=0;
		}
	}
	else
	{
		currentSearchChannel +=RM_AM_FREQ_STEP;
		if(currentSearchChannel>RM_AM_FREQ_MAX)
		{
			currentSearchChannel=0;
		}
	}

    //printf("~~~~~~~~~~~~~~~~~lastSearchChannel[%d] currentSearchChannel[%d]\n", lastSearchChannel, currentSearchChannel);

	
	if ( currentSearchChannel > 107900 )
	{
		currentSearchChannel=0;
	}
	

	emit searchChannelChanged(lastSearchChannel,currentSearchChannel);

	if(currentSearchChannel>0)
	{
		if(m_voiceSwithFunction!=NULL &&  m_voiceStatus ==RM_VOICE_SWITCH_ON)
		{
			m_voiceSwithFunction(RM_VOICE_SWITCH_OFF,m_voiceSwithUserData);
			m_voiceStatus =RM_VOICE_SWITCH_OFF;
		}

		if(freqSection ==RM_FREQ_SECTION_FM)
			searchFMChannel(currentSearchChannel,1);
		else
			searchAMChannel(currentSearchChannel, 1);

		timeId =startTimer(nextSearchInterval);
	}
	else
	{
		if(listenedChannel!=0)
		{
			if(m_voiceSwithFunction!=NULL)
				m_voiceSwithFunction(RM_VOICE_SWITCH_ON,m_voiceSwithUserData);

			if(freqSection ==RM_FREQ_SECTION_FM)
				selectFMChannel(listenedChannel,0);
			else
				selectAMChannel(listenedChannel,0);

			status =RM_STATUS_LISTEN;
		}
		else
		{
			closeRadio();
		}
        //printf("~~~~~~~~~~~~~~~~~searchEnd Normally   \n");
		emit searchEnd(RM_SEARCH_R_OK);
	}

	//printf("~~~~~~~~~~~~~~~~~searchEnd   returnm \n", lastSearchChannel, currentSearchChannel);
        return;
}


void RadioManagerOld::readParameterConfig( int frequencySection, int * pRssi, int * pSnr )
{
	QSettings *ConfigIni = new QSettings(RADIO_CONFIG_FILE_PATHNAME,QSettings::IniFormat,0); 
	if ( NULL == ConfigIni )
	{
		return;
	}
	int size = 0;
	int ret = 0;

	if ( RM_FREQ_SECTION_FM == frequencySection )
	{
		size = ConfigIni->beginReadArray("fm_config");
	}
	else
	{
		size = ConfigIni->beginReadArray("am_config");
	}
	ret = ConfigIni->value("rssi", 0 ).toInt();
	if (  0 != ret )
	{
		*pRssi = ret;
	}
	
	ret = ConfigIni->value("snr", 0 ).toInt();
	if (  0 != ret )
	{
		*pSnr = ret;
	}
	
	ConfigIni->endArray();
}



