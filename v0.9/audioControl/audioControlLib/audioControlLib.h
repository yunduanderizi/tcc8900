
#ifndef AUDIO_CONTROL_LIB_H
#define AUDIO_CONTROL_LIB_H
#include <QObject>
#include "dbusClientProxy.h"
#include "dbusMessage.h"
#include "audioData.h"
#include "audioControlErrCode.h"

namespace AudioControl1
{

class AudioControlLibC : public QObject
{
	Q_OBJECT
public:
	//get the object of "AudioControlLibC", notice: you can not get the object from "new"
	static AudioControlLibC *object();
	//release the object of "AudioControlLibC"
	static void releaseObject();


	int open(AudioChannelIdE audioChannelId, bool bBlock=false);
	int close(AudioChannelIdE audioChannelId, bool bBlock=false);
	int adjustVol(AudioChannelIdE audioChannelId, int vol, bool bBlock=false);
	int mute(AudioChannelIdE audioChannelId, bool bBlock=false);
	int unMute(AudioChannelIdE audioChannelId, bool bBlock=false);
	int setSpeed(int carSpeed, bool bBlock=false);
	int openVolSpeed(bool bBlock=false);
	int closeVolSpeed(bool bBlock=false);
	int getCurrentOpenAudioChannel(AudioChannelIdE& audioChannelId);
	int inquireChannelInfo(AudioChannelC& channelInfo);
	int inquireAllChannelInfo(AudioChannelList& channelInfoList);
	int resumeDataToDefault(bool bBlock=false);
	
signals:
        void sigAudioChannelStatusChange(AudioChannelIdE audioChannelId, AudioChannelStatusE audioChannelStatus);

	
private:
	static AudioControlLibC *thisObject;
	AudioControlLibC();
	virtual ~AudioControlLibC();

	DBusClientProxyC   *m_pDbusClientToAudioControl;
	bool		m_bInitFlag;

	static void _audioChannelStatusChangedHandler(DBusNotifyC *notify);

	void _audioChannelStatusChanged(AudioChannelIdE audioChannelId, AudioChannelStatusE audioChannelStatus);


	
	void		_init();

};


}

#endif

