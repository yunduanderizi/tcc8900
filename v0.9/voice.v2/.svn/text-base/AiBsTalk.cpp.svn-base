/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： VoiceRecogImpl.cpp
* 文件标识： (见配置管理计划书)
* 内容摘要： 
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

#include "AiBsTalk.h"
#include "Record.h"
#include "voiceDefine.h"
#include "AiPlay.h"
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
#include <QTimerEvent>
#include <QObject>
#include <QTextCodec>
#include "ivTts.h"

AiBsTalkC *AiBsTalkC::thisObject = NULL;

/**
*<FUNC+>********************************************************************
* 函数名称： VoiceRecogImplC
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
AiBsTalkC::AiBsTalkC()
{
}

/**
*<FUNC+>********************************************************************
* 函数名称： VoiceRecogImplC
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
AiBsTalkC::~AiBsTalkC()
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
AiBsTalkC* AiBsTalkC::object()
{
    AiBsTalkC *pTmp = NULL;

    if (thisObject == NULL)
    {
        thisObject = new AiBsTalkC;
    }

    pTmp = thisObject;

    return pTmp;
}

/**
*<FUNC+>********************************************************************
* 函数名称： openVoiceRecog
* 功能描述： 打开语音识别
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/26]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiBsTalkC::openVoiceRecog()
{
    printf("[AiBsTalkC] openVoiceRecog start\n");
    int iRet = 0;
    m_strResult.clear();
    m_bFlag = false;

	// 发送语音输入开始信号
    MyDbusC::object()->aiBsTalkStatusUpdate(Voice1::ABT_STATUS_START);

    IvTtsC::object()->PlayText("语音输入开始");

	// 需要开启声音中控里语音输入通道
	iRet = AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_VOICE_INPUT, true);

	RecordC::object()->startVoiceRecord();



    // 开始计时
    m_iTimerId = startTimer(VOICETIMEOUT);

    // 初始化
    iRet = initVoiceRecog();
    if (iRet != 0)
    {
        // 关闭录音线程
        RecordC::object()->stopVoiceRecord();

        printf("Init return err %d\n", iRet);
		exit(-2);
		
        releaseSessionResourch();

        closeSoundControl();
        playVoiceFinishSound();

        // 发送语音输入结束信号
        MyDbusC::object()->aiBsTalkStatusUpdate(Voice1::ABT_STATUS_STOP);

        return iRet;
    }

    start();

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： closeVoiceRecog
* 功能描述： 关闭语音识别
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/7]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiBsTalkC::closeVoiceRecog()
{
    if (isRunning())
    {
        m_bFlag = true;
    }
	return 0;
}
/**
*<FUNC+>********************************************************************
* 函数名称： ReleaseObject
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
void AiBsTalkC::releaseObject()
{
    if (thisObject != NULL)
    {
        delete thisObject;
        thisObject = NULL;
    }
}

/**
*<FUNC+>********************************************************************
* 函数名称： InitVoiceRecog
* 功能描述： 初始化语音识别
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
int AiBsTalkC::initVoiceRecog()
{
    int iRet = 0;
    const char *strConfigs = "server_url=dev.voicecloud.cn, timeout=10000, vad_enable=true";
    const char *strParams = "ssm=1, sub=iat, aue=speex-wb, auf=audio/L16;rate=16000, ent=sms16k, rst=plain, rse=utf8";
    m_strSessionId = NULL;

    // 初始化
    iRet = QISRInit(strConfigs);
    if (0 != iRet)
    {
        printf("QISRInit failed, error code is %d\n", iRet);
        return iRet;
    }

    // 开启会话
    m_strSessionId = QISRSessionBegin("", strParams, &iRet);
    if (0 != iRet)
    {
        printf("QISRSessionBegin failed, error code is %d\n", iRet);
    }

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： SendAudioData
* 功能描述： 发送音频数据到服务器中
* 输入参数： audioData：数据缓存，iWaveLen：数据大小
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/18]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiBsTalkC::sendAudioData(char *audioData, size_t iWaveLen)
{
    if (audioData == NULL)
    {
        printf("SendAudioData, audioData is null\n");
        return -1;
    }

    int iRet = 0;
    int iAudioStatus = ISR_AUDIO_SAMPLE_CONTINUE;
    int iEqStatus = ISR_EP_LOOKING_FOR_SPEECH;
    int iRsltStatus = ISR_REC_STATUS_SUCCESS;
    const char *strResult = NULL;

    // 发送到服务器中
    iRet = QISRAudioWrite(m_strSessionId, audioData, iWaveLen, iAudioStatus, &iEqStatus, &iRsltStatus);
    if(0 != iRet)
    {
        printf("SendAudioData, QISRAudioWrite failed, error code is %d\n", iRet);
        iRsltStatus = ISR_REC_STATUS_SPEECH_COMPLETE;
        return iRet;
    }
    else if (ISR_EP_AFTER_SPEECH == iEqStatus)
    {
        // 检测到音频后端点，停止发送音频 
        printf("end point of speech has been detected!");
        return iEqStatus;
    }

    ///printf("QISRAudioWrite ok! len=%d\n", iWaveLen);

    // 一定要检查一次结果，也许已经识别成功有结果
    
    if (ISR_REC_STATUS_SUCCESS == iRsltStatus)
    {
        strResult = QISRGetResult(m_strSessionId, &iRsltStatus, 5000, &iRet);
        if (0 != iRet)
        {
            printf("QISRGetResult failed, error code is %d\n", iRet);
            iRsltStatus = ISR_REC_STATUS_SPEECH_COMPLETE;
            return iRet;
        }

        if (NULL != strResult)
        {
            printf("got a result: %s\n", strResult);
            m_strResult.append(strResult);
        }

        // 全部结果已经取完了
        if (ISR_REC_STATUS_SPEECH_COMPLETE == iRsltStatus)
        {
            printf("the result has been got completely!\n");
            return iRsltStatus;
        }
    }
	
	
    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： ReadAudioResult
* 功能描述： 获取字符串
* 输入参数： iRsltStatus：SendAudioData函数返回值
* 输出参数： iRtnValue：错误码
* 返 回	值： 有结果返回字符串，否则返回空
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/18]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
string AiBsTalkC::readAudioResult(int iRsltStatus, int &iRtnValue)
{
    const char *strResult = NULL;
    int iRet = 0;

    int iAudioStatus = ISR_AUDIO_SAMPLE_LAST;
    int iEqStatus;
    int iRecogStatus;
    iRet = QISRAudioWrite(m_strSessionId, NULL, 0, iAudioStatus, &iEqStatus, &iRecogStatus);

    while(ISR_REC_STATUS_SPEECH_COMPLETE != iRsltStatus)
    {
    	printf("[AiBsTalkC] wait for QISR Result\n");
        strResult = QISRGetResult(m_strSessionId, &iRsltStatus, 5000, &iRet);

        iRtnValue = iRet;
        if (0 != iRet)
        {
            // 有错误出现，返回空
            printf("QISRGetResult failed, error code is: %d\n", iRet);
            break;
        }

        if (NULL != strResult)
        {
            m_strResult.append(strResult);
            printf("got a result: %s\n", strResult);
        }

        // sleep一下很有必要，防止MSC端无缓存的识别结果时浪费CPU资源
        usleep(500000);

        
    }

    return m_strResult;
}

/**
*<FUNC+>********************************************************************
* 函数名称： ReleaseSessionResourch
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
void AiBsTalkC::releaseSessionResourch()
{
    if (m_strSessionId != NULL)
    {
        printf("[AiBsTalkC] m_strSessionId != NULL \n");
        QISRSessionEnd(m_strSessionId, "normal end");
        m_strSessionId = NULL;
    }

    printf("[AiBsTalkC] releaseSessionResourch \n");
    QISRFini();

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
void AiBsTalkC::run()
{
    int iRet = 0;


	// for test 
	int testCount= 0;
	
    while (1)
    {
        if (m_bFlag)
        {
            // 关闭录音线程
            RecordC::object()->stopVoiceRecord();

			if (m_strResult.empty())
			{
	            iRet = -1;
			}
            // 设定一个错误值，退出循环
            break;
        }
        bool bStopFlag = false;
        // 先获取到音频缓存队列
        list<Audio_Data> tempAudioData;

        RecordC::object()->getAudioData(tempAudioData);

        list<Audio_Data>::iterator tempAudioIter = tempAudioData.begin();
        for (; tempAudioIter != tempAudioData.end(); ++tempAudioIter)
        {
	
            iRet = sendAudioData(tempAudioIter->audioData, tempAudioIter->iLen);
            if (iRet != 0)
            {
                // 出现错误，要停止发送
                printf("SendAudioData failed, %d\n", iRet);
                bStopFlag = true;
                RecordC::object()->stopVoiceRecord();

                break;
            }

			testCount++;
			if ( testCount % 16 == 0 )
			{
				printf("[AiBsTalkC] sendAudioData ret[%d] len[%d]\n", iRet,tempAudioIter->iLen);
				
			}

            usleep(70000);
        }

        usleep(70000);

        if (bStopFlag)
        {
            // 需要停止
            break;
        }
    }

    if (iRet != 0 && iRet != 3 && iRet != 5)
    {
        // 出错，返回错误码
        // 关闭录音线程
        RecordC::object()->stopVoiceRecord();

        printf("[AiBsTalkC] ======== Get error %d ====== \n", iRet);
        releaseSessionResourch();

        // 关闭语音输入通道
        closeSoundControl();

        // 播放提示音
        playVoiceFinishSound();

        // 发送停止输入的信号
        MyDbusC::object()->aiBsTalkStatusUpdate(Voice1::ABT_STATUS_STOP);

        return;
    }

    // 获取数据
    int iTemp;
	
    string strRtnResult = "";//= readAudioResult(iRet, iTemp);
	
    if (iTemp != 0)
    {
        if (iTemp == MSP_ERROR_NO_DATA)
        {
            printf("[AiBsTalkC] readAudioResult No data\n");
        }
        else
        {
            printf("[AiBsTalkC] ======== readAudioResult error %d ======\n", iTemp);
        }
    }
    else
    {
    	printf("\n=====================================\n");
        printf("Get a result %s\n", strRtnResult.c_str());
		printf("=====================================\n\n");
    }
	
    if (m_strResult.empty())
    {
        MyDbusC::object()->aiBsTalkResult(NULL);
    }
    else
    {
        MyDbusC::object()->aiBsTalkResult(m_strResult.c_str());
    }

    // 关闭录音线程
    RecordC::object()->stopVoiceRecord();

    releaseSessionResourch();

    // 关闭语音输入通道
    closeSoundControl();

    // 播放提示音
    playVoiceFinishSound();

    // 发送停止输入的信号
    MyDbusC::object()->aiBsTalkStatusUpdate(Voice1::ABT_STATUS_STOP);


	// for test
	/*
	sleep(1);
	if (!strRtnResult.empty())
	{
	
		QString tmpString = strRtnResult.c_str();
		QTextCodec *codec = QTextCodec::codecForName("GB18030-0");
			QByteArray encodedString = codec->fromUnicode(tmpString);
		IvTtsC::object()->PlayText(encodedString.data());
	}*/
}

/**
*<FUNC+>********************************************************************
* 函数名称： closeSoundControl
* 功能描述： 关闭语音输入通道
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/12/10]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void AiBsTalkC::closeSoundControl()
{
    // 需要关闭语音输入通道[12/6/2011 yuzhenyang]
    AudioControl1::AudioControlLibC::object()->close(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_VOICE_INPUT, false);
    printf("[AiBsTalkC] closeSoundControl[VOICE_INPUT] \n");
}

/**
*<FUNC+>********************************************************************
* 函数名称： playVoiceFinishSound
* 功能描述： 播放关闭语音输入提示音
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/12/10]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void AiBsTalkC::playVoiceFinishSound()
{
    IvTtsC::object()->PlayText("语音输入结束");
    printf("[AiBsTalkC] playVoiceFinishSound \n");
}

/**
*<FUNC+>********************************************************************
* 函数名称： timerEvent
* 功能描述： 开始计时，录音线程不能超过10秒
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/12/16]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void AiBsTalkC::timerEvent(QTimerEvent *event)
{
    int iTimerId = event->timerId();

    if(m_iTimerId == iTimerId)
    {
        stopRecorze();
    }
}
/**
*<FUNC+>********************************************************************
* 函数名称： stopRecorze
* 功能描述： 停止语音识别槽函数
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/12/16]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void AiBsTalkC::stopRecorze()
{
    killTimer(m_iTimerId);
    closeVoiceRecog();
}
