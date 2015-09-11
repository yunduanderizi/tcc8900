
#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include <QList>
#include <QString>
//#include <QMetaType>

namespace AudioControl1
{




#define AUDIO_CHANNEL_MAX_NUMBER		100
#define LEVEL_MAX_NUMBER					10
#define VOL_MAX_VALUE						255

typedef enum _WorkModeE
{
	WORK_MODE_NORMAL = 0,
	WORK_MODE_SPECIAL,
}WorkModeE;

typedef enum _AudioChannelIdE
{
	AUDIO_CHANNEL_ID_MIN = 0,
//特殊通道
	AUDIO_CHANNEL_ID_MAIN,							//主声道(功放)
	AUDIO_CHANNEL_ID_MAIN_LEFT,						//左声道
	AUDIO_CHANNEL_ID_MAIN_RIGHT,					//右声道
	AUDIO_CHANNEL_ID_RELATION,						//关联声道
		//实际操纵: AUDIO_CHANNEL_ID_MAIN

//模拟通道
	AUDIO_CHANNEL_ID_ANALOG_RADIO,					//收音机
	AUDIO_CHANNEL_ID_ANALOG_TV,						//电视
	AUDIO_CHANNEL_ID_ANALOG_PHONE,					//语音电话
	AUDIO_CHANNEL_ID_ANALOG_MIC,					//麦克风(用户不用，内部使用)
	AUDIO_CHANNEL_ID_ANALOG_BLUETOOTH,				//蓝牙

//数字通道
	AUDIO_CHANNEL_ID_DIGITAL_MPLAYER,				//mplayer占用的声道
	AUDIO_CHANNEL_ID_DIGITAL_GPS,					//GPS语音
	AUDIO_CHANNEL_ID_DIGITAL_GPS_RING,				//GPS提示音
	AUDIO_CHANNEL_ID_DIGITAL_NEWS,					//新闻播报
	AUDIO_CHANNEL_ID_DIGITAL_SMS,					//短信播报
	AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP,				//语音模块提示 15
	AUDIO_CHANNEL_ID_DIGITAL_VOICE_IDENTIFY,		//语音识别	16
		//实际操纵: AUDIO_CHANNEL_ID_ANALOG_MIC
	AUDIO_CHANNEL_ID_DIGITAL_VOICE_INPUT,			//语音输入  17
		//实际操纵: AUDIO_CHANNEL_ID_ANALOG_MIC
	AUDIO_CHANNEL_ID_DIGITAL_ANNOUNCE,				//交通播报
	AUDIO_CHANNEL_ID_DIGITAL_RING,					//来电铃声
	AUDIO_CHANNEL_ID_DIGITAL_VPHONE,				//视频电话
	AUDIO_CHANNEL_ID_DIGITAL_KEYTONE,				//按键音
	AUDIO_CHANNEL_ID_DIGITAL_ILLEGALTIPS,			//违章提示
	AUDIO_CHANNEL_ID_DIGITAL_SMSRING,				//短信提示音
//	AUDIO_CHANNEL_ID_DIGITAL_TONE,
//	AUDIO_CHANNEL_ID_DAI_DAI_VTALKI,
//	AUDIO_CHANNEL_ID_DAI_DAI_MONLINE,
//	AUDIO_CHANNEL_ID_DAI_DAI_VONLINE,
//	AUDIO_CHANNEL_ID_DAI_DAI_LBOOK,
//	AUDIO_CHANNEL_ID_DAI_DAI_MTRAFFIC,

	AUDIO_CHANNEL_ID_MAX,
	AUDIO_CHANNEL_ID_INVALID,
}AudioChannelIdE;

static const char*  g_AudioChannelName[] = 
{
	"",								//AUDIO_CHANNEL_ID_MIN = 0,
	"main",							//AUDIO_CHANNEL_ID_MAIN,							//主声道(功放)
	"main_left",						//AUDIO_CHANNEL_ID_MAIN_LEFT,					//左声道
	"main_right",						//AUDIO_CHANNEL_ID_MAIN_RIGHT,					//右声道
	"relation",						//AUDIO_CHANNEL_ID_RELATION,						//关联声道

	"radio",							//AUDIO_CHANNEL_ID_ANALOG_RADIO,				//收音机
	"tv",								//AUDIO_CHANNEL_ID_ANALOG_TV,					//电视
	"phone",							//AUDIO_CHANNEL_ID_ANALOG_PHONE,				//语音电话
	"",								//AUDIO_CHANNEL_ID_ANALOG_MIC,					//麦克风(用户不用，内部使用)
	"bluetooth",						//AUDIO_CHANNEL_ID_ANALOG_BLUETOOTH,			//蓝牙

	"mplayer",						//AUDIO_CHANNEL_ID_DIGITAL_MPLAYER,				//mplayer占用的声道
	"gps",							//AUDIO_CHANNEL_ID_DIGITAL_GPS,					//GPS语音
	"gps_ring",						//AUDIO_CHANNEL_ID_DIGITAL_GPS_RING,				//GPS提示音
	"news",							//AUDIO_CHANNEL_ID_DIGITAL_NEWS,					//新闻播报
	"sms",							//AUDIO_CHANNEL_ID_DIGITAL_SMS,					//短信播报
	"voice_tip",						//AUDIO_CHANNEL_ID_DIGITAL_VOICE_TIP,				//语音模块提示
	"voice_identify",					//AUDIO_CHANNEL_ID_DIGITAL_VOICE_IDENTIFY,		//语音识别
	"voice_input",						//AUDIO_CHANNEL_ID_DIGITAL_VOICE_INPUT,			//语音输入
	"announce",						//AUDIO_CHANNEL_ID_DIGITAL_ANNOUNCE,			//交通播报
	"ring",							//AUDIO_CHANNEL_ID_DIGITAL_RING,					//来电铃声
	"vphone",						//AUDIO_CHANNEL_ID_DIGITAL_VPHONE,				//视频电话
	"keytone",						//AUDIO_CHANNEL_ID_DIGITAL_KEYTONE,				//按键音
	"illegaltips",						//AUDIO_CHANNEL_ID_DIGITAL_ILLEGALTIPS,			//违章提示
	"smsring",						//AUDIO_CHANNEL_ID_DIGITAL_SMSRING,				//短信提示音
	
	"",								//AUDIO_CHANNEL_ID_MAX,
};

typedef enum _VolCorrespondenceE
{
	VOL_CORRESPONDENCE_LINER = 0,
	VOL_CORRESPONDENCE_LOG,
	VOL_CORRESPONDENCE_TABLE,
}VolCorrespondenceE;

typedef enum _AudioChannelStatusE
{
	AUDIO_CHANNEL_OPEN = 0,
	AUDIO_CHANNEL_CLOSE,
	AUDIO_CHANNEL_PAUSE,
}AudioChannelStatusE;

class AudioChannelC
{
public:
	AudioChannelC(){id=AUDIO_CHANNEL_ID_INVALID; idLevel=-1; status=AUDIO_CHANNEL_CLOSE;
		bMute=false; iVirtualVol=VOL_MAX_VALUE; iVirtualVolDefault=VOL_MAX_VALUE; iActualVol=VOL_MAX_VALUE; iActualVolMax=VOL_MAX_VALUE; iVolAdjustRange=0;
		bChanged=false; name=""; bRelated=false; bValid=false;}
	AudioChannelC(const AudioChannelC &other){*this=other;}
	
	AudioChannelIdE		id;
	int					idLevel;
	AudioChannelStatusE	status;
	bool					bMute;
	int					iVirtualVol;
	int					iVirtualVolDefault;
	int					iActualVol;
	int					iActualVolMax;
	int					iVolAdjustRange;//用于mixA，标识iVirtualVol需要调节的范围
	int					bChanged;//用于mixA，标识已经改变了的声道
	QString				name;
	bool					bRelated;
	bool					bValid;
};

//Q_DECLARE_METATYPE(AudioChannelC)
	
typedef QList<AudioChannelC> AudioChannelList;

typedef enum _AudioLevelTypeE
{
	AUDIO_LEVEL_TYPE_COMMON = 0,
	AUDIO_LEVEL_TYPE_ALWAYS_OPEN,
}AudioLevelTypeE;

typedef enum _AudioLevelPropertyE
{
	AUDIO_LEVEL_PROPERTY_EXCLUSIVE = 0,
	AUDIO_LEVEL_PROPERTY_PAUSE,
	AUDIO_LEVEL_PROPERTY_INTERRUPT,
	AUDIO_LEVEL_PROPERTY_MIX_A,
	AUDIO_LEVEL_PROPERTY_MIX_B,
	AUDIO_LEVEL_PROPERTY_NOTHING,		//for level of type AUDIO_LEVEL_TYPE_ALWAYS_OPEN
}AudioLevelPropertyE;

typedef struct _AudioLevelS
{
	AudioLevelTypeE	type;
	AudioLevelPropertyE	property;
	AudioChannelList	audioChannelList;
}AudioLevelS;

typedef QList<AudioLevelS> AudioLevelList;


}


#endif

