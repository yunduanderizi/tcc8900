#ifndef IBMEDIA_DEFINE_H
#define IBMEDIA_DEFINE_H

#include <QString>
#include <QMetaType>  
#include "debugMonitor.h"


namespace MediaControl2
{

#define MEDIA_E_OK 								0
#define MEDIA_E_LIST_VOID						-1
#define MEDIA_E_NOT_TO_OTHER_FILE				-2


typedef enum _Media_Running_State
{
	MEDIA_RUNNING_STATE_STOP = 0,
	MEDIA_RUNNING_STATE_PLAYING,
	MEDIA_RUNNING_STATE_PAUSE,
	MEDIA_RUNNING_STATE_FINISHED,
	//MEDIA_RUNNING_STATE_CRASH,//commented for bug:can't play the next and previous when playing
}Media_Running_State;

typedef enum _Media_Type
{
	MEDIA_TYPE_MOVIE = 0,
	MEDIA_TYPE_MUSIC,
	MEDIA_TYPE_STORY,
	MEDIA_TYPE_NET_MUSIC,
	MEDIA_TYPE_NONE,
}Media_Type;

typedef enum _List_Play_Mode
{
	LIST_PLAY_MODE_NO_INIT_FLAG = 0,
	LIST_PLAY_MODE_SINGLE,
	LIST_PLAY_MODE_SINGLE_LOOPED,
	LIST_PLAY_MODE_SEQUENCE,
	LIST_PLAY_MODE_SEQUENCE_LOOPED,
	LIST_PLAY_MODE_RANDOM,
}List_Play_Mode;

typedef enum _File_Status
{
	FILE_STATUS_UNKNOWN = 0,
	FILE_STATUS_NORMAL,
	FILE_STATUS_NOT_OPEN,
	FILE_STATUS_NOT_PLAY,
}File_Status;

typedef enum _Player_Property
{
	SETTING_NO_INIT_FLAG = 0x01,
	PLAYER_PROPERTY_SAVE_POSITION = 0x02,
	PLAYER_PROPERTY_DELETE_ALL_NOOPEN_FILE	= 0x04,
	PLAYER_PROPERTY_DELETE_NOPLAY_FILE	= 0x08,
	PLAYER_PROPERTY_DELETE_SD_FILE = 0x10,
	PLAYER_PROPERTY_SKIP_ABNORMAL_FILE = 0x20,
}Player_Property;

typedef enum _Media_Time_Format
{
	TIME_FORMAT_1_2 = 0,//e.g:  1:59 123:29
	TIME_FORMAT_2_2,//e.g:   01:59 223:29
	TIME_FORMAT2_2_2,//e.g:00:01:02  00:32:32
	MAX_TIME_FORMAT,
}MEDIA_TIME_FORMAT;

typedef enum
{
	MPLAYER_AUDIO_EFFCET_NULL = 0,//无音效
	MPLAYER_POP,//流行音乐
	MPLAYER_CLASSIC,//古典
	MPLAYER_DANCE,//爵士
	MPLAYER_RB,//RB
	MPLAYER_BASS,//重低音
	MPLAYER_ROCK,//摇滚
	MPLAYER_USER_SET,//自定义
	MPLAYER_MAX_AUDIO_EFFCET,//
}M_AUDIO_EFFCET;

#define MAX_EQUALIZER       10

typedef struct _Audio_Equalizer_S
{
	_Audio_Equalizer_S(){for(int i=0; i<MAX_EQUALIZER; i++)iEqualizer[i]=0;}
	int iEqualizer[MAX_EQUALIZER];
}Audio_Equalizer_S;

#define MASK_FUNCTION_PLAY			0x0001
#define MASK_FUNCTION_STOP			0x0002
#define MASK_FUNCTION_PAUSE			0x0004
#define MASK_FUNCTION_RESUME			0x0008
#define MASK_FUNCTION_PLAYNEXT		0x0010
#define MASK_FUNCTION_PLAYPRE			0x0020

#define FUNCTION_IS_VALID(state, mask)		((state&mask)>0)?true:false)

class PlayListNode
{
public:
	PlayListNode(){sFileName=""; iTimeLength=-1; fileStatus=FILE_STATUS_UNKNOWN; iPosition=0; bNormal=true;}
 	QString sFileName; 
	int iTimeLength;//for second
	File_Status fileStatus;
	int iPosition;
	bool bNormal;
};


typedef QList<PlayListNode> PlayList ;






}
Q_DECLARE_METATYPE(MediaControl2::Media_Running_State);
Q_DECLARE_METATYPE(MediaControl2::File_Status);
Q_DECLARE_METATYPE(MediaControl2::Media_Type);
Q_DECLARE_METATYPE(MediaControl2::Audio_Equalizer_S);

Q_DECLARE_METATYPE(MediaControl2::PlayListNode);
Q_DECLARE_METATYPE(MediaControl2::PlayList);


#endif //IBMEDIA_DEFINE_H
