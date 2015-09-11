/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： AiPlay.h
* 文件标识： (见配置管理计划书)
* 内容摘要： 
* 其它说明： 
* 当前版本： V1.0
* 作    者： 于震洋
* 完成日期： [2011/10/19]
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容： 
*<FH+>***************************************************************************
*/ 
#ifndef AIPLAY_H
#define AIPLAY_H

#include <QThread>
#include <QMutex>
#include <list>
#include "alsa/asoundlib.h"

using namespace std;

#define SAMPLERATE 16000
#define CH 1
#define VOICEDATALEN 50000

typedef struct _S_VOICE_DATA
{
    char voiceData[VOICEDATALEN];
    int iLen;
}Voice_Data;


/**
*<CLASS+>********************************************************************
* 类 名 称: AiPlayC
* 功能描述: 语音合成播放
* 其它说明: 
* 修改记录: 
* 修改日期			版 本		修改人		修改摘要
* -------------------------------------------------------------------------
* [2011/10/19]	    V1.0		于震洋		创建类
*<CLASS->*********************************************************************
*/
class AiPlayC : public QThread
{
public:
    AiPlayC();
    virtual ~AiPlayC();

    static AiPlayC* object();
    static void releaseObject();

    int voicePlay(const char *strText);
	int writePcm(char *pData, int iLen);
	
private:
    static AiPlayC *thisObject;
    const char *m_strSessionId;
    list<Voice_Data> m_VoiceList;
    static QMutex m_MutexLock;
    volatile bool m_bStopFlag;

    snd_pcm_t *capture_handle;
    snd_pcm_stream_t stream;
    snd_pcm_uframes_t frames;


    int init();
    int sendText(const char *strText);
    int getVoice();
    int playText();
    void releaseSessionResourch();
    void run();

    int InitPcm();
    int pcmDataPlay();
};

#endif