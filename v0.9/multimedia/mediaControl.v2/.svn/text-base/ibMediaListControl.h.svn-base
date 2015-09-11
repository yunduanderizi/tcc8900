#ifndef  IBMEDIA_LIST_CONTROL_H_20110824
#define  IBMEDIA_LIST_CONTROL_H_20110824

#include <QString>
#include <QStringList>
#include <QObject>
#include <qfile.h>
#include <QMetaType>  
#include <QTimerEvent>
#include "ibMediaDefine.h"

namespace MediaControl2
{


class IBMediaControlC;
class IBMediaListControlC : public QObject
{
	Q_OBJECT
		
public slots:
	void playListUpdate(const QStringList & fileList);
	void playListAppend(const QStringList & fileList);
	void playListDelete(const QString fileName);
	void playListDelete(const int index);
	void playListClear();
	void setDefaultListPlayMode(const List_Play_Mode mode);
	void setListPlayMode(const List_Play_Mode mode);
	List_Play_Mode getListPlayMode();
	PlayList getPlayList();
	int getPlayListCursor();
	void defaultPropertySetting(const unsigned int property);
	void propertySetting(const unsigned int property);
	unsigned int getProperty();	

	void mediaTypeChangeToNone(bool bExceptMyself=false);
	void changePlayPosition(int position);
	int getCurrentMediaPlayPosition(void);
	Media_Running_State getCurrentMediaState(void);
	MEDIA_TIME_FORMAT getTimeFormat(void);
	int getCurrentMediaLength(void);
	void sdPop(QString sdDir);


	void start();
	void stop();
	void pause();
	void resume();
	void playNext();
	void playPrevious();
	void fastForward(int iSecond);
	void playListSetCursor(int cursor);
        void playListSetCursorNoPlay(int cursor);
	void saveData();
	void reportInitMessage();
	int	getFunctionState();
	void sendFunctionState();
	void resumeMediaStatus();
        void resumeFromPause(void);

	void setAudioEffect(const M_AUDIO_EFFCET effect);
	M_AUDIO_EFFCET getAudioEffect();
	void setAudioEqualizer(const Audio_Equalizer_S sEqualizer);
	Audio_Equalizer_S getAudioEqualizer();
	
signals:
	void sigMediaStateChanged(MediaControl2::Media_Running_State state,int index);
	void sigMediaPlayError(MediaControl2::File_Status fileStatus);
	void sigMediaPosition(int iPosition, QString sPosition);
	void sigMediaLength(int iMediaLength, QString sMediaLength,int index);
	void sigMediaChanged(QString mediaNameWithPath);
	void sigPlayListChanged(QStringList sNameList);
	void sigPlayListCursorChanged(int iCursor);
	void sigFunctionStateChanged(int state);
	void sigAudioEffect(MediaControl2::M_AUDIO_EFFCET effect);
	void sigEqualizer(MediaControl2::Audio_Equalizer_S equalizer);

protected:
	IBMediaListControlC(Media_Type type);
	~IBMediaListControlC();

	void setTimeFormat(MEDIA_TIME_FORMAT format);
	void setFullScreenWindowMode(bool bFullScreen);
	bool isFullScreenWindowMode(void);
	void openVideoScreen(void);
	void closeVideoScreen(void);

private:
	Media_Type	m_MediaType;
	PlayList		m_slPlayList;
	Media_Running_State m_state;
	int			m_iCursor;
	List_Play_Mode	m_ListPlayMode;
	unsigned int	m_Property;	
	MEDIA_TIME_FORMAT	m_iTimeFormat;
	bool			m_bFullScreen;
	int			m_iFunctionState;
	bool			m_bPrePauseFromUser;
	M_AUDIO_EFFCET	m_AudioEffect;
	Audio_Equalizer_S	m_AudioEqualizer;
	bool			m_bUserSetAudioEffect;
	
	int m_iTimerPlay;
	int m_iTimerPlayDelay;
	int m_iTimerFreshState;
	int m_iTimerFreshStateDelay;

private:
	void _reportPlayListChanged(void);
	bool _isSupport(Player_Property property);
	void _playListAppend(const QString & fileName);
	void _play(const QString & fileName, int seek=0);
	int _saveData();//save m_slPlayList   m_iCursor   m_ListPlayMode   m_Property   m_iTimeFormat  m_AudioEffect  m_AudioEqualizer m_bUserSetAudioEffect
	int _loadData();//load m_slPlayList   m_iCursor   m_ListPlayMode   m_Property   m_iTimeFormat  m_AudioEffect  m_AudioEqualizer m_bUserSetAudioEffect
	bool _isFileExist(const QString fileName);
	void _deleteAllNoOpenFile();
	void _freshAllNoOpenFile();
	int _playNext(bool bLoop=true);
	int _playPrevious(bool bLoop=true);
	int _playRand();
	void _sendState();
	void _freshState(bool bDelay=false);//fresh function state and playing state
	void timerEvent(QTimerEvent * event);

private slots:
	void slotMediaTypeChanged(MediaControl2::Media_Type type);
	void slotMediaStateChanged(MediaControl2::Media_Running_State state);
	void slotCurrentTimeChanged(int current,QString s_current,int total,QString s_total);
	void slotMediaPlayError(MediaControl2::File_Status fileStatus);
	
};








}

#endif
