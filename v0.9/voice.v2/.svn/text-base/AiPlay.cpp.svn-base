/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： AiPlay.cpp
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
#include <stdlib.h>
#include "AiPlay.h"
#include <stdio.h>
#include <string.h>
#include "qtts.h"

AiPlayC *AiPlayC::thisObject = NULL;
QMutex AiPlayC::m_MutexLock;

AiPlayC::AiPlayC()
{

	InitPcm();
}

AiPlayC::~AiPlayC()
{
}

/**
*<FUNC+>********************************************************************
* 函数名称： object
* 功能描述： 获取对象
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回对象，否则返回null
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/25]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
AiPlayC* AiPlayC::object()
{
    AiPlayC *pTmp = NULL;

    if (thisObject == NULL)
    {
        thisObject = new AiPlayC;
    }

    pTmp = thisObject;

    return pTmp;
}

/**
*<FUNC+>********************************************************************
* 函数名称： releaseObject
* 功能描述： 释放资源
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/25]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void AiPlayC::releaseObject()
{
    if (thisObject != NULL)
    {
        delete thisObject;
        thisObject = NULL;
    }
}
/**
*<FUNC+>********************************************************************
* 函数名称： VoicePlay
* 功能描述： 合成播放文本
* 输入参数： strText：要合成的文本
* 输出参数： 无
* 返 回	值： 
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/19]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiPlayC::voicePlay(const char *strText)
{
    if (strText == NULL)
    {
        printf("voicePlay, strText is null");
        return -1;
    }

    int iRet = 0;
    m_VoiceList.clear();
    m_bStopFlag = false;

    // 初始化
    iRet = init();
    if (iRet != 0)
    {
        printf("voicePlay, init failed %d\n", iRet);
        releaseSessionResourch();
        return iRet;
    }

    // 发送字符到服务器中
    iRet = sendText(strText);
    if (iRet != 0)
    {
        printf("voicePlay, sendText failed %d\n", iRet);
        releaseSessionResourch();
        return iRet;
    }

    // 获取音频到缓存中
    iRet = getVoice();
    if (iRet != 0)
    {
        printf("voicePlay, getVoice failed %d\n", iRet);
        releaseSessionResourch();
        return iRet;
    }


    wait();

    // 释放资源
    releaseSessionResourch();

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： init
* 功能描述： 初始化
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiPlayC::init()
{
    int iRet = 0;
    const char *strParams = "ssm=1, auf=audio/L16;rate=16000, aue=speex-wb, ent=intp65";

    iRet = QTTSInit("");
    if (0 != iRet)
    {
        printf("QTTSInit failed, error code is %d\n", iRet);
        return iRet;
    }

    // 开启一路会话
    m_strSessionId = QTTSSessionBegin(strParams, &iRet);
    if (0 != iRet)
    {
        printf("QTTSSessionBegin failed, error code is %d\n", iRet);
        return iRet;
    }

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： sendText
* 功能描述： 发送要合成的文本到服务器中
* 输入参数： strText：要合成文本
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiPlayC::sendText(const char *strText)
{
    if (strText == NULL)
    {
        printf("sendText, strText is null\n");
        return -1;
    }

    int iRet = 0;

    iRet = QTTSTextPut(m_strSessionId, strText, strlen(strText), NULL);
    if (iRet != 0)
    {
        printf("QTTSTextPut failed, error code is %d\n", iRet);
        return iRet;
    }
	return 0;
}

/**
*<FUNC+>********************************************************************
* 函数名称： getVoice
* 功能描述： 获取语音数据
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiPlayC::getVoice()
{
    int iRet = 0;
    unsigned int iVoiceDataLen = 0;
    int iVoiceStatus = 0;
    const char *voiceData = NULL;
    int iStartFlag = 0;

    while (TTS_FLAG_DATA_END != iVoiceStatus)
    {
        voiceData = (char *)QTTSAudioGet(m_strSessionId, &iVoiceDataLen, &iVoiceStatus, &iRet);
        if (0 != iRet)
        {
            printf("QTTSAudioGet failed, error code is %d\n", iRet);
            m_bStopFlag = true;
            break;
        }

        if (voiceData != NULL && iVoiceDataLen != 0)
        {
            Voice_Data tempVoiceData;

            if (iVoiceDataLen <= VOICEDATALEN)
            {
                tempVoiceData.iLen = iVoiceDataLen;

                printf("iVoiceLen = %d\n", iVoiceDataLen);
                memset(tempVoiceData.voiceData, 0, VOICEDATALEN);
                memcpy(tempVoiceData.voiceData, voiceData, iVoiceDataLen);
            }
            else
            {
                tempVoiceData.iLen = VOICEDATALEN;

                printf("iVoiceLen = %d\n", iVoiceDataLen);
                memset(tempVoiceData.voiceData, 0, VOICEDATALEN);
                memcpy(tempVoiceData.voiceData, voiceData, VOICEDATALEN);
            }

            m_MutexLock.lock();
            m_VoiceList.push_back(tempVoiceData);
            m_MutexLock.unlock();

            // 第一次获取到数据，开始线程播放
            if (iStartFlag == 0)
            {
                playText();
            }

            iStartFlag++;
        }
    }

    // 停止播放线程
    m_bStopFlag = true;

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： playText
* 功能描述： 播放缓存中的数据
* 输入参数： 无
* 输出参数： 无 
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiPlayC::playText()
{
    int iRet = 0;

    iRet = InitPcm();
    if (iRet < 0)
    {
        // 初始化
        printf("playText, InitPcm failed, %d\n", iRet);
        return iRet;
    }

    start();

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： InitPcm
* 功能描述： 初始化播放器
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiPlayC::InitPcm()
{
    int err;
    int dir;
    int size;
    unsigned int val;
    snd_pcm_hw_params_t* params;
    int channels = 0;

    if ( (err = snd_pcm_open(&capture_handle, "plug:tcc", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    {
        fprintf(stderr, "cannot open audio device (%s)", snd_strerror (err));
        goto OpenPcmFail;
    }

    snd_pcm_hw_params_alloca(&params);

    if( (err = snd_pcm_hw_params_any(capture_handle, params)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_any failed!  err = %d\n", err);
        goto OpenPcmFail;
    }

    if( (err = snd_pcm_hw_params_set_access(capture_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_access failed! err = %d\n", err);
        goto OpenPcmFail;
    }
    if( (err = snd_pcm_hw_params_set_format(capture_handle, params, SND_PCM_FORMAT_S16_LE)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_format failed!  err = %d\n", err);
        goto OpenPcmFail;
    }

    channels = CH;
    if( (err = snd_pcm_hw_params_set_channels(capture_handle, params, channels)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_channels failed!	 err = %d\n", err);
        goto OpenPcmFail;
    }

    val = SAMPLERATE;
    if( (err = snd_pcm_hw_params_set_rate_near(capture_handle, params, &val, &dir)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_rate_near failed!	err = %d\n", err);
        goto OpenPcmFail;
    }

    frames = 4096;

    if( (err = snd_pcm_hw_params_set_period_size_near(capture_handle, params, &frames, &dir)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_period_size_near failed!	err = %d\n", err);
        goto OpenPcmFail;
    }
    if ( (err = snd_pcm_hw_params(capture_handle, params)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params-failed! err = %d err: %s\n", err, snd_strerror(err));
        goto OpenPcmFail;
    }

    if( (err = snd_pcm_hw_params_get_period_size(params, &frames, &dir)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_get_period_size failed!  err = %d\n", err);
        goto OpenPcmFail;
    }
    size = 160;

    if( (err = snd_pcm_hw_params_get_period_time(params, &val, &dir)) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_get_period_time failed! err = %d\n", err);
        goto OpenPcmFail;
    }

    return 0;

OpenPcmFail:
    return -1;
}

/**
*<FUNC+>********************************************************************
* 函数名称： pcmDataPlay
* 功能描述： 播放声音
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，错误返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiPlayC::pcmDataPlay()
{
    int iRet = 0;

    m_MutexLock.lock();
    list<Voice_Data>::iterator voiceIter = m_VoiceList.begin();
    for (; voiceIter != m_VoiceList.end(); ++voiceIter)
    {
        // 长度不除2会有杂音
        iRet = snd_pcm_writei(capture_handle, voiceIter->voiceData, voiceIter->iLen / 2);
    }

    m_VoiceList.clear();
    m_MutexLock.unlock();

    return iRet;
}

int AiPlayC::writePcm(char *pData, int iLen)
{
    int err = 0;

    //m_MutexLock.lock();

    err = snd_pcm_writei(capture_handle, pData, iLen/2);
        if( err == -EPIPE)
        {
            fprintf(stderr,"snd_pcm_writei error! err: %s\n", snd_strerror(err));
            if( (err = snd_pcm_prepare(capture_handle)) < 0)
            {
                    fprintf(stderr,"snd_pcm_prepare_failed! err: %s\n", snd_strerror(err));
            }
        }
        else if (err < 0)
        {
            fprintf(stderr,"err writei!\n");
            //break;
        }
        else
        {
           //position+=playLen;
        }
		
    //m_MutexLock.unlock();

    return err;
}

/**
*<FUNC+>********************************************************************
* 函数名称： releaseSessionResourch
* 功能描述： 释放会话资源
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/1]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void AiPlayC::releaseSessionResourch()
{
    if (m_strSessionId != NULL)
    {
        QTTSSessionEnd(m_strSessionId, NULL);
        m_strSessionId = NULL;
    }

    QTTSFini();
}

/**
*<FUNC+>********************************************************************
* 函数名称： run
* 功能描述： 线程执行函数
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/11]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
// 播放声音
void AiPlayC::run()
{
    while (1)
    {
        // 检查当前缓存中是否还有数据，无数据并且需要停止就停止，否则播放完最后的缓存再停止
        if (m_bStopFlag && m_VoiceList.empty())
        {
            break;
        }

        pcmDataPlay();
    }
}
