#ifndef RADIO_MANAGEROLD_H
#define RADIO_MANAGEROLD_H
#include "abstractRadio.h"

#define RM_VOLUME_MIN	0
#define RM_VOLUME_MAX	100

#define RM_VOLUME_SET		0
#define RM_VOLUME_INC		1
#define RM_VOLUME_DEC		2



#define RM_VOICE_SWITCH_ON	1
#define RM_VOICE_SWITCH_OFF	0
typedef void (*RMVoiceSwitchFunction)(int voiceSwitch,void* userData);


class RadioManagerOld:public AbstractRadio
{
	Q_OBJECT
public:
    RadioManagerOld();
    ~RadioManagerOld();

public:
	int openRadio(int frequencySection,int channel,int volume);
	int closeRadio();

	int adjustVolume(int volume,int mode);
	int getVolume();

	int changeChannel(int channel);

	int beginSearch(int channel);
	int stopSearch();

	void setVoiceSwitchFunction(RMVoiceSwitchFunction voiceSwithFunction,void *userData=NULL);

//public:
//signals:
//	void searchChannelChanged(int lastChannel,int currentChannel);
//	void searchedOneChannel(int channel);
//	void searchEnd(int status);

private:
	void timerEvent(QTimerEvent *event);
	void searchNextChannel();

	int openRadioAtChannel(int frequencySection,int channel,char searchFlag=0);
	void selectFMChannel(int channel,char searchFlag=0);
	void searchFMChannel(int channel,char searchFlag=1);
	void selectAMChannel(int channel,char searchFlag=0);
	void searchAMChannel(int channel,char searchFlag=1);
	int getTVValue(int frequency);
	//void readSStopConfig( struct tda7541_sstop * psstop, int freqSection, int freqence);
	//void readRegConfig( unsigned char * amRegArray, unsigned char * fmRegArray);
	void readParameterConfig( int frequencySection, int * pRssi, int * pSnr );
private:
	//int 	fd;
	int 	seekFd;
    char	status;
	char	freqSection;

	int	timeId;
	int	volume;

	int	lastSearchChannel;
	int	currentSearchChannel;
	int	searchedChannel;
	int	listenedChannel;

	unsigned int	channelMeter;
	int	meterIsUping;

	RMVoiceSwitchFunction  m_voiceSwithFunction;
	void		      *m_voiceSwithUserData;
	char		       m_voiceStatus;
};

#endif

