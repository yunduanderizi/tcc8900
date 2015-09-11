#ifndef VOICE_DEFINE_H
#define VOICE_DEFINE_H

#include <QString>
#include <QMetaType>  
#include "debugMonitor.h"


#define	NAME_LENGTH_MAX							256
#define	VOICE_TEXT_LENGTH_MAX					512
#define	AI_BS_TALK_RESULT_LENGTH_MAX			128
#define	AI_TALK_RESULT_LENGTH_MAX				32

// 识别超过10000MS(10秒)退出
#define VOICETIMEOUT                            20000

namespace Voice1
{


typedef enum _Ai_Bs_Talk_Status
{
	ABT_STATUS_START = 0,
	ABT_STATUS_STOP,
}Ai_Bs_Talk_Status;

typedef enum _Ai_Talk_Status
{
	AT_STATUS_START = 0,
	AT_STATUS_STOP,
}Ai_Talk_Status;

typedef enum _Play_Text_Status
{
	PT_STATUS_START = 0,
	PT_STATUS_STOP,
}Play_Text_Status;






}

// 增加关键词列表[11/23/2011 yuzhenyang]
typedef QList<QString> KeyWordList;
Q_DECLARE_METATYPE(Voice1::Ai_Bs_Talk_Status);
Q_DECLARE_METATYPE(Voice1::Ai_Talk_Status);
Q_DECLARE_METATYPE(Voice1::Play_Text_Status);




#endif

