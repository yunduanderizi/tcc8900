#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

// Files include
#include <QObject>
#include <QMap>

#include "audioData.h"
#include "i2ccontrol.h"


namespace AudioControl1
{




// Data structures define
typedef enum _AudioChanCtlRet
{
	ACCR_ERR_OK = 0,	// sucess; other value reflact the fail reason
	ACCR_ERR_ALSA = 0x1,
	ACCR_ERR_ARG_INVALID = 0x2,
	ACCR_ERR_I2C = 0x4,
	ACCR_ERR_GF = 0x8,
	ACCR_ERR_INTER = 0x16,
}AudioChanCtlRet;

#define ACCR_CHAN_VOLUME_MIN 0
#define ACCR_CHAN_VOLUME_MAX 255

// for Inter Usage
typedef enum _AudioChanModeStatus
{
	ACCR_MODE_STATUS_UNINIT = 0,
	ACCR_MODE_STATUS_INIT_FAIL,
	ACCR_MODE_STATUS_INIT_OK,
}AudioChanModeStatus;

typedef struct _AudioChanCtrlData
{
	AudioChannelIdE	logicId;
	int 	alsaNumid;
	int 	alsaSwitchNumid;		
	char	chanName[24];
	int 	volume;
}AudioChanCtrlData; 


// AudioDeviceC
class AudioDeviceC:public QObject
{
	Q_OBJECT
public:
	AudioDeviceC();
	~AudioDeviceC();

public:
	static AudioDeviceC *object();
	static void releaseObject();
	
	AudioChanCtlRet init();

	// vol range[0-100]
	AudioChanCtlRet setAudioChanVolume( AudioChannelIdE chanName, int vol );


private:
	AudioChanCtlRet openGF();
	AudioChanCtlRet closeGF();
	AudioChanCtlRet initAlsa();
	AudioChanCtlRet setGFStatus( bool gfOpen );
	AudioChanCtlRet checkModeStatus();
	AudioChanCtlRet checkGFStatus( bool gfOpen );
	AudioChanCtlRet setAudioVolume( AudioChanCtrlData chanCtrlData, int vol );
	AudioChanCtlRet setAlsaVolume( AudioChanCtrlData chanCtrlData, int vol );
	void initChanCtlMap();
	int convertVolume( AudioChannelIdE chanName, int volume );

private:
	static AudioDeviceC *				m_thisObject;
	IBI2CControl * 				m_i2cHandler;	// I2C handle to control Speeker
	AudioChanModeStatus			m_status;		// Interface inter status
	bool 						m_gfOpen;		// GF status, true means gf is open 
	QMap<AudioChannelIdE, AudioChanCtrlData> m_auidioCtrlMap;
};


}



#endif

