/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： VoiceRecord.cpp
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
#include "Record.h"

RecordC *RecordC::thisObject = NULL;
list<Audio_Data> RecordC::m_AudioData;
QMutex RecordC::m_MutexLock;
/**
*<FUNC+>********************************************************************
* 函数名称： VoiceRecordC
* 功能描述： 构造函数
* 输入参数： 
* 输出参数： 
* 返 回	值： 
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/17]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
RecordC::RecordC()
{
    m_StopFlag = false;
    m_AudioData.clear();
}

/**
*<FUNC+>********************************************************************
* 函数名称： VoiceRecordC
* 功能描述： 析构函数
* 输入参数： 
* 输出参数： 
* 返 回	值： 
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/17]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
RecordC::~RecordC()
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
* 	[2011/10/18]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
RecordC* RecordC::object()
{
    RecordC *pTmp = NULL;

    if(thisObject == NULL)
    {
        thisObject = new RecordC;
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
* 	[2011/10/18]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void RecordC::releaseObject()
{
    if (thisObject != NULL)
    {
        delete thisObject;
        thisObject = NULL;
    }
}

/**
*<FUNC+>********************************************************************
* 函数名称： InitVoiceRecord
* 功能描述： 初始化语音录音
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/17]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int RecordC::initVoiceRecord()
{
    int err = 0;
    unsigned int val;
    int dir = 0;

    snd_pcm_hw_params_t *hw_params = NULL;

    snd_pcm_hw_params_alloca(&hw_params);

    m_stream = SND_PCM_STREAM_CAPTURE;

    if ((err = snd_pcm_open (&m_capture_handle, "plug:tcc", SND_PCM_STREAM_CAPTURE, 0)) < 0) 
    {
        printf("cannot open audio device (%d)\n", err);
        return -1;
    }

    if ((err = snd_pcm_hw_params_any (m_capture_handle, hw_params)) < 0) 
    {
        printf("cannot initialize hardware parameter structure (%d)\n", err);
        return -1;
    }

    if ((err = snd_pcm_hw_params_set_access (m_capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        printf("cannot set access type (%d)\n", err);
        return -1;
    }

    if ((err = snd_pcm_hw_params_set_format (m_capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0)
    {
        printf("cannot set sample format (%d)\n", err);
        return -1;
    }
    val = SAMPLERATE;
    if ((err = snd_pcm_hw_params_set_rate_near (m_capture_handle, hw_params, &val, 0)) < 0) 
    {
        printf("cannot set sample rate (%d)\n", err);
        return -1;
    }
    val = CH;	
    if ((err = snd_pcm_hw_params_set_channels (m_capture_handle, hw_params, val)) < 0)
    {
        printf("cannot set channel count (%d)\n", err);
        return -1;
    }

    if ((err = snd_pcm_hw_params (m_capture_handle, hw_params)) < 0)
    {
        printf ("cannot set parameters (%d)\n", err);
        return -1;
    }

    if ((err = snd_pcm_prepare (m_capture_handle)) < 0)
    {
        printf("cannot prepare audio interface for use (%d)\n", err);
        return -1;
    }

    m_frames = 6144;

    if( (err = snd_pcm_hw_params_set_period_size_near(m_capture_handle, hw_params, &m_frames, &dir)) < 0)
    {
        printf("snd_pcm_hw_params_set_period_size_near failed!   err = %d\n", err);
        return -1;
    }

    snd_pcm_hw_params_get_period_size(hw_params, &m_frames, 0);

    return 0;
}

/**
*<FUNC+>********************************************************************
* 函数名称： StartVoiceRecord
* 功能描述： 开始录音
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/19]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int RecordC::startVoiceRecord()
{
    int iRet = 0;

    // 清空缓存
    m_AudioData.clear();
    m_StopFlag = false;

    // 初始化录音设备
    iRet = initVoiceRecord();
    if (iRet != 0)
    {
        printf("Init pcm fail, please try again...\n");
        return -1;
    }

    printf("[RecordC] startVoiceRecord\n");

    start();

    return iRet;
}
/**
*<FUNC+>********************************************************************
* 函数名称： StopVoiceRecord
* 功能描述： 结束语音录音
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/17]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void RecordC::stopVoiceRecord()
{
    printf("[RecordC] stopVoiceRecord\n");
    if (isRunning())
    {
        printf("[RecordC] stopVoiceRecord when last running \n");
        m_StopFlag = true;
    }
}

/**
*<FUNC+>********************************************************************
* 函数名称： GetAudioData
* 功能描述： 获取音频队列
* 输入参数： 无
* 输出参数： audioData：音频队列
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/18]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void RecordC::getAudioData(list<Audio_Data> &audioData)
{
    m_MutexLock.lock();

    list<Audio_Data>::iterator audioIter = m_AudioData.begin();
    for (; audioIter != m_AudioData.end(); ++audioIter)
    {
        audioData.push_back(*audioIter);
    }

    m_AudioData.clear();
    m_MutexLock.unlock();
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
* 	[2011/10/19]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void RecordC::run()
{
    printf("[RecordC]  ===== run =======  \n");
    int iRet = 0;

	// for test printf
	int testCont = 0;
	//system("amixer sset tccvol 220");

	
    while (1)
    {
    	if (m_StopFlag)
        {
            // 需要停止
            printf("[RecordC]  stop testCont:%d\n", testCont);
            break;
        }

        Audio_Data tempAudioData;
        memset(tempAudioData.audioData, 0, sizeof(tempAudioData.audioData));

        // 录制
        int iLen = snd_pcm_readi(m_capture_handle, tempAudioData.audioData, m_frames);

        if (iLen != (int)m_frames)
        {
            printf("read from audio interface failed (%d)\n", iLen);
            if ((iRet = snd_pcm_prepare(m_capture_handle)) < 0)
            {
                printf("snd_pcm_prepare_failed! err: %d\n", iRet);
                break;
            }
        }

		testCont++;	
		if ( testCont % 128 == 0 )
		{
			printf("[TRecordC] read from audio interface len [%d]\n", iLen);
			for (int i=0; i < 256; i++ )
			{
				printf("%2x ", tempAudioData.audioData[i] );
				if ( i % 16 == 15 )
				{
					printf("\n");
				}

			}
		}

        tempAudioData.iLen = m_frames * 2;

        // 录制成功，存在音频队列中，保证数据连续，先清空原有数据
        m_MutexLock.lock();

        m_AudioData.push_back(tempAudioData);

        m_MutexLock.unlock();

        //printf("录音中\n");
    }
}
