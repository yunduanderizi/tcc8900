
#ifndef AUDIO_CONTROL_H
#define AUDIO_CONTROL_H

#include <QObject>
#include <QTimerEvent>
#include "audioControlMessage.h"
#include "audioData.h"


namespace AudioControl1
{

class AudioControlC : public QObject
{
	Q_OBJECT
		
public:
	//get the object of "AudioControlC", notice: you can not get the object from "new"
	static AudioControlC *object();
	//release the object of "AudioControlC"
	static void releaseObject();

	int open(AudioChannelIdE id);
	int close(AudioChannelIdE id);
	int adjustVol(AudioChannelIdE id, int vol);
	int mute(AudioChannelIdE id);
	int unMute(AudioChannelIdE id);
	int setSpeed(int carSpeed);
	int openVolSpeed();
	int closeVolSpeed();
	int getCurrentOpenAudioChannel(AudioChannelIdE& id);
	int inquireChannelInfo(AudioChannelC& channelInfo);
	int inquireAllChannelInfo(AudioChannelList& channelInfoList);
	int resumeDataToDefault();
	
private:
	static AudioControlC *thisObject;
	AudioControlC();
	virtual ~AudioControlC();

	void timerEvent(QTimerEvent * event);
	
	void	_loadDataFromConfig();
	int _loadDataFromDB();
	int _saveDataToDB();

	int _openForCommonLogic(AudioChannelIdE audioChannelId);
	int _closeForCommonLogic(AudioChannelIdE audioChannelId);
	int _adjustVolForCommonLogic(AudioChannelIdE audioChannelId, int vol);
	int _muteForCommonLogic(AudioChannelIdE audioChannelId);
	int _unMuteForCommonLogic(AudioChannelIdE audioChannelId);
	int _setSpeedForCommonLogic(int carSpeed);
	int _openVolSpeedForCommonLogic();
	int _closeVolSpeedForCommonLogic();

	int _openForSpecialLogic(AudioChannelIdE audioChannelId, bool& bDone);
	int _closeForSpecialLogic(AudioChannelIdE audioChannelId, bool& bDone);
	int _adjustVolForSpecialLogic(AudioChannelIdE audioChannelId, int vol, bool& bDone);
	int _muteForSpecialLogic(AudioChannelIdE audioChannelId, bool& bDone);
	int _unMuteForSpecialLogic(AudioChannelIdE audioChannelId, bool& bDone);
	int _setSpeedForSpecialLogic(int carSpeed, bool& bDone);
	int _openVolSpeedForSpecialLogic(bool& bDone);
	int _closeVolSpeedForSpecialLogic(bool& bDone);

	int _checkAudioChannelId(AudioChannelIdE audioChannelId, Audio_Control_Command_Id idCommand);
	void _pauseLevel(int idLevel);
	void _closeLevel(int idLevel);
	void _openAudioChannel(AudioChannelIdE audioChannelId);
	void _pauseAudioChannel(AudioChannelIdE audioChannelId);
	void _closeAudioChannel(AudioChannelIdE audioChannelId);
	bool _isAudioChannelOpen(AudioChannelIdE audioChannelId);
	void _updateAudioChannelDataToDevice(AudioChannelIdE audioChannelId);
	void _notifyAudioChannelStatusChanged(AudioChannelIdE audioChannelId);
	void _adjustVolForMixA(int idLevel);
	void _freshDataStatus(bool bOnlyFresh=false);
	int _getIndexInLevel(AudioChannelIdE audioChannelId);
	int _calculateActualVol(AudioChannelIdE audioChannelId);

	WorkModeE			m_WorkMode;
	VolCorrespondenceE	m_VolCorrespondence;
	//声音级别列表
	AudioLevelList			m_AudioLevelList;
	//当前打开声道所在声音级别的序号(如果没有打开声道，则为-1)
	int					m_iCurrentOpenLevel;
	//声道指针数组
	AudioChannelC*		m_pAudioChannel[AUDIO_CHANNEL_MAX_NUMBER];
	//混音A主混音音量升高的百分比
	int					m_iMixMainUpValue;
	//混音A中其他混音音量降低的百分比
	int					m_iMixOtherDownValue;
	//声音随速的起始速度
	int					m_iSpeedStartForVolSpeed;
	//声音随速的终止音量
	int					m_iVolEndForVolSpeed;
	//声音随速中速度调节的间隔值
	int					m_iSpeedIntervalForVolSpeed;
	//声音随速中音量每次调节的大小
	int					m_iVolIncrementForVolSpeed;
	//是否打开声音随速(会存入数据库)
	bool					m_bOpenVolSpeed;
	//当前车速(起始值是0)
	int					m_iCurrentSpeedForVolSpeed;
	int					m_iVolTable[VOL_MAX_VALUE+1];

	//用于更新声道状态的定时器变量
	int					m_iTimerFreshData;
	int					m_iTimerFreshDataDelay;

	//用于存储数据的定时器变量
	int					m_iTimerSaveData;
	int					m_iTimerSaveDataDelay;
};


}

#endif

