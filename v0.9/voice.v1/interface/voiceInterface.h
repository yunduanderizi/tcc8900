#ifndef MOVIEINTERFACE_H
#define MOVIEINTERFACE_H

#define MOVIE_MSG_BUF_SIZE 30

/*语音识别接口*/
class AiTalkInterface
{
public:
	virtual ~AiTalkInterface(){}
	virtual bool GetIdentifyState() = 0;//得到语音识别功能的状态，true表示语音识别功能打开，false表示语音识别功能关闭
};

/*提供给主线程调用的语音识别接口*/
class AiTalkInterfaceForMainthread
{
public:
	virtual ~AiTalkInterfaceForMainthread(){}
	
	virtual bool GetIdentifyState() = 0;//查询现在语音识别所处的状态，是打开，还是关闭，查询结果会作为遥控码发送
#if 0
	virtual void ProhibitEnterIdentifyMode() = 0;	//禁止进入语音识别
	virtual void AllowEnterIdentifyMode() = 0;		//允许进入语音识别
#endif	
	virtual bool StartIdentify() = 0;//开始语音识别功能
	virtual bool StopIdentify() = 0;//停止语音识别功能
};

/*用户身份*/
typedef enum _TTSUser
{
	User_MainThread,
	User_AiTalk
}TTSUser;

/*声音输出模式*/
typedef enum _SoundOutputMode
{
	GPRS_3G_Output_Mode,					//GPRS/3G 音频输出模式
	FM_Output_Mode,						//FM 音频输出模式
	DVD_Output_Mode,						//DVD输出模式
	DSP_Output_Mode						//DSP(cpu)输出模式
}SoundOutputMode;

/*语音合成接口*/
class IvTTSInterface
{
public:
	virtual ~IvTTSInterface(){}
	virtual void PlayString(TTSUser user, const char* str) = 0;		//合成、播放字符串
};

#endif // MOVIEINTERFACE_H
