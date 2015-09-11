#ifndef AUDIO_CONTROL_MESSAGE_H_20111026
#define AUDIO_CONTROL_MESSAGE_H_20111026 

#include "dbusClientProxy.h"
#include "dbusMessage.h"
#include "audioData.h"

namespace AudioControl1
{




#define	ServerAudioControl 		(char*)"com.innov.audiocontrol"
#define	ObjectAudioControl		(char*)"/audioControl"


#define	REQ_ID_UI_TO_AUDIOCONTROL_COMMON								55
#define	REP_ID_AUDIOCONTROL_TO_UI_COMMON								55
#define	NOTF_ID_AUDIOCONTROL_TO_UI_CHANNEL_STATUS						55
#define	REQ_ID_UI_TO_AUDIOCONTROL_INQUIRE_CHANNEL_INFO				56
#define	REP_ID_AUDIOCONTROL_TO_UI_INQUIRE_CHANNEL_INFO				56
#define	REP_ID_AUDIOCONTROL_TO_UI_INQUIRE_ALL_CHANNEL_INFO			57

////////////////////////////////////////////////////////////////////

typedef enum _Audio_Control_Command_Id
{
	ACC_OPEN = 0,
	ACC_CLOSE,
	ACC_ADJUST_VOL,
	ACC_MUTE,
	ACC_UNMUTE,
	ACC_SET_SPEED,
	ACC_OPEN_VOL_SPEED,
	ACC_CLOSE_VOL_SPEED,
	ACC_GET_CURRENT_OPEN_AUDIO_CHANNEL,
	ACC_RESUME_DATA_TO_DEFAULT,
}Audio_Control_Command_Id;

class UiToAudioControlCommonReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(UiToAudioControlCommonReqC)

public:
	UiToAudioControlCommonReqC():DBusRequestC(REQ_ID_UI_TO_AUDIOCONTROL_COMMON){}
	void print(FILE *fp){fprintf(fp, "m_iCommandId[%d] m_AudioChannelId[%d] m_iVol[%d] m_iSpeed[%d] m_bNeedReply[%d]\n", m_iCommandId, m_AudioChannelId, m_iVol, m_iSpeed, m_bNeedReply);}

	int	m_iCommandId;
	AudioChannelIdE	m_AudioChannelId;
	int	m_iVol;
	int	m_iSpeed;
	bool m_bNeedReply;
};

class AudioControlToUiCommonRepC:public DBusReplyC
{
	DBUS_REPLY_OBJECT(AudioControlToUiCommonRepC)

public:
	AudioControlToUiCommonRepC():DBusReplyC(REP_ID_AUDIOCONTROL_TO_UI_COMMON){}
	void print(FILE *fp){fprintf(fp, "m_iRetval[%d] m_AudioChannelId[%d]\n", m_iRetval, m_AudioChannelId);}

	int	m_iRetval;
	AudioChannelIdE	m_AudioChannelId;

};

class AudioControlToUiChannelStatusNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(AudioControlToUiChannelStatusNotifyC)

public:
	AudioControlToUiChannelStatusNotifyC():DBusNotifyC(NOTF_ID_AUDIOCONTROL_TO_UI_CHANNEL_STATUS){}
	void print(FILE *fp){fprintf(fp, "m_AudioChannelId[%d] m_AudioChannelStatus[%d]\n", m_AudioChannelId, m_AudioChannelStatus);}

	AudioChannelIdE		m_AudioChannelId;
	AudioChannelStatusE	m_AudioChannelStatus;
}; 

typedef struct _AudioChannelInfoTmpS
{
	AudioChannelIdE			id;
	int						vol;
	bool						mute;
	AudioChannelStatusE		status;
}AudioChannelInfoTmpS;

class UiToAudioControlInquireChannelInfoReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(UiToAudioControlInquireChannelInfoReqC)

public:
	UiToAudioControlInquireChannelInfoReqC():DBusRequestC(REQ_ID_UI_TO_AUDIOCONTROL_INQUIRE_CHANNEL_INFO){}
	void print(FILE *fp){fprintf(fp, "m_bAllAudioChannel[%d] m_AudioChannelId[%d]\n", m_bAllAudioChannel, m_AudioChannelId);}

	bool	m_bAllAudioChannel;
	AudioChannelIdE	m_AudioChannelId;
};

class AudioControlToUiInquireChannelInfoRepC:public DBusReplyC
{
	DBUS_REPLY_OBJECT(AudioControlToUiInquireChannelInfoRepC)

public:
	AudioControlToUiInquireChannelInfoRepC():DBusReplyC(REP_ID_AUDIOCONTROL_TO_UI_INQUIRE_CHANNEL_INFO){}
	void print(FILE *fp){fprintf(fp, "m_iRetval[%d]\n", m_iRetval);}

	int	m_iRetval;
	AudioChannelInfoTmpS	m_info;
};

class AudioControlToUiInquireAllChannelInfoRepC:public DBusReplyC
{
	DBUS_REPLY_OBJECT(AudioControlToUiInquireAllChannelInfoRepC)

public:
	AudioControlToUiInquireAllChannelInfoRepC():DBusReplyC(REP_ID_AUDIOCONTROL_TO_UI_INQUIRE_ALL_CHANNEL_INFO){}
	void print(FILE *fp){fprintf(fp, "m_iInfoNum[%d]\n", m_iInfoNum);}

	int	m_iInfoNum;
	AudioChannelInfoTmpS	m_info[AUDIO_CHANNEL_ID_MAX];
};


}


#endif

