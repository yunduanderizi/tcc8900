/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： VoiceRecord.h
* 文件标识： (见配置管理计划书)
* 内容摘要： 语音录音功能
* 其它说明： 
* 当前版本： V1.0
* 作    者： 于震洋
* 完成日期： [2011/10/17]
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容： 
*<FH+>***************************************************************************
*/ 
#ifndef VOICERECORD_H
#define VOICERECORD_H

#include <list>
#include <QThread>
#include <QMutex>
#include "alsa/asoundlib.h"

#define SAMPLERATE 16000
#define SAMPLERATEEIGHT 8000
#define CH 1

using namespace std;

typedef struct _S_AUDIO_DATA
{
    char audioData[1024];
    int iLen;
}Audio_Data;


/**
*<CLASS+>********************************************************************
* 类 名 称: VoiceRecordC
* 功能描述: 开启麦克风录音
* 其它说明: 
* 修改记录: 
* 修改日期			版 本		修改人		修改摘要
* -------------------------------------------------------------------------
* [2011/10/17]	    V1.0		于震洋		创建类
*<CLASS->*********************************************************************
*/
class RecordC : public QThread
{
public:
    RecordC();
    virtual ~RecordC();

    static RecordC* object();
    static void releaseObject();

    void getAudioData(list<Audio_Data> &audioData);

    int startVoiceRecord();
    void stopVoiceRecord();

private:
    static RecordC *thisObject;
    snd_pcm_stream_t m_stream;
    snd_pcm_t *m_capture_handle;
    snd_pcm_uframes_t m_frames;

    // 缓存队列，存储音频数据
    static list<Audio_Data> m_AudioData;

    // 停止标志
    volatile bool m_StopFlag;

    static QMutex m_MutexLock;

    void run();
    int initVoiceRecord();
};

#endif


