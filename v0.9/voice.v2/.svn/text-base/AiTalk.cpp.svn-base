/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： AiTalk.cpp
* 文件标识： (见配置管理计划书)
* 内容摘要： 
* 其它说明： 
* 当前版本： V1.0
* 作    者： 于震洋
* 完成日期： [2011/10/20]
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容： 
*<FH+>***************************************************************************
*/
#include "AiTalk.h"
#include "Record.h"
#include "KeyWordsMgr.h"
#include "AiPlay.h"
#include "ivTts.h"

AiTalkC *AiTalkC::thisObject = NULL;

AiTalkC::AiTalkC()
{
}

AiTalkC::~AiTalkC()
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
AiTalkC* AiTalkC::object()
{
    AiTalkC *pTmp = NULL;

    if (thisObject == NULL)
    {
        thisObject = new AiTalkC;
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
void AiTalkC::releaseObject()
{
    if (thisObject != NULL)
    {
        delete thisObject;
        thisObject = NULL;
    }
}
/**
*<FUNC+>********************************************************************
* 函数名称： StartVoiceRecorg
* 功能描述： 开始识别
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/20]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiTalkC::startVoiceRecorg()
{
    int iRet = 0;
    m_strResult.clear();
    m_bFlag = false;
    AudioControlLibC::object();

    MyDbusC::object()->aiTalkStatusUpdate(Voice1::AT_STATUS_START);

	IvTtsC::object()->PlayText("语音识别功能开启");

	// 需要开启声音中控里语音输入通道
	iRet = AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_VOICE_INPUT, true);

	RecordC::object()->startVoiceRecord();


    // 初始化
    iRet = init();
    if (iRet != 0)
    {
        printf("[AiTalkC] Init return err %d\n", iRet);
        RecordC::object()->stopVoiceRecord();
        releaseSessionResourch();

        closeAudioControl();
        AiPlayC::object()->voicePlay("语音识别功能结束");

        MyDbusC::object()->aiTalkStatusUpdate(Voice1::AT_STATUS_STOP);
        return iRet;
    }

    start();

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： StopVoiceRecorg
* 功能描述： 停止语音识别
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/20]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiTalkC::stopVoiceRecorg()
{
    if (isRunning())
    {
        m_bFlag = true;
    }
	return 0;
}

/**
*<FUNC+>********************************************************************
* 函数名称： getGrammId
* 功能描述： 获取语法的ID
* 输入参数： strGrammId：语法ID
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/31]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void AiTalkC::getGrammId(string &strGrammId)
{
    // 从数据库中获取信息
    int iRet = 0;
    QString strKeyword;
    QString strKeywordId;
    KeyWordList keywordList;

	KeyWordsMgrC::object()->getKeyWordId(strKeywordId);
	printf("[AiTalkC] in getGrammId  len[%d]\n", strKeywordId.length() );
	
    if (strKeywordId.length() <= 0)
    {
        // 关键词ID为空，获取关键词上传
        KeyWordsMgrC::object()->getKeyWord(keywordList);
        if (keywordList.empty())
        {
            // 关键词也为空，直接返回
            printf("getGrammId, keywords empty\n");
            return;
        }

        // 上传关键词
        // 先拼接字符串为如下格式“词1,词2,词3”
        
        KeyWordList::iterator keywordIter = keywordList.begin();
        for (; keywordIter != keywordList.end(); ++keywordIter)
        {
        	printf("[AiTalkC] upload key word:[%s]\n", (*keywordIter).toUtf8().data() );
            strKeyword.append(*keywordIter);
            strKeyword.append(",");
        }

        // 去掉最后一个逗号
        strKeyword.remove(strKeyword.length() - 1, 1);

        m_strSessionId = QISRSessionBegin(NULL, "ssm=1,sub=asr", &iRet);
        if (iRet != 0)
        {
            printf("QISRSessionBegin with errorCode: %d \n", iRet);
            return;
        }

        // 获取的是关键词，上传到服务器中
        const char *strUploadId = QISRUploadData(m_strSessionId, "contact", 
            strKeyword.toUtf8().data(), strKeyword.length(), "dtt=keylist", &iRet);

        printf("[AiTalkC]  upload id is %s\n", strUploadId);
        if (iRet != 0)
        {
            printf("QISRUploadData with errorCode: %d \n", iRet);
            return;
        }

        strKeywordId = strUploadId;

        // 将获得的ID存到数据库中
        KeyWordsMgrC::object()->insertKeyWordId(strUploadId);

        QISRSessionEnd(m_strSessionId, "normal");
        m_strSessionId = NULL;
    }

    strGrammId = strKeywordId.toStdString();
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
* 	[2011/10/31]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiTalkC::init()
{
    int iRet = 0;
    const char *strConfigs = "server_url=dev.voicecloud.cn, timeout=10000, vad_enable=true";
    const char *strParams = "ssm=1, sub=asr, aue=speex-wb, auf=audio/L16;rate=16000, rst=plain";
    m_strSessionId = NULL;

    // 初始化
    iRet = QISRInit(strConfigs);
    if(iRet != 0)
    {
        printf("QISRInit with errorCode: %d \n", iRet);
        return iRet;
    }

    // 先获取语法ID
    printf("[AiTalkC]  start getGrammId \n");
    string strGrammId;
    getGrammId(strGrammId);

    printf("[AiTalkC]  getGrammId keywordId is %s\n", strGrammId.c_str());
    // 开启一路会话
    m_strSessionId = QISRSessionBegin(strGrammId.c_str(), strParams, &iRet);
    if (iRet != 0)
    {
        printf("QISRSessionBegin with errorCode: %d \n", iRet);
        return iRet;
    }

    return iRet;
}

/**
*<FUNC+>********************************************************************
* 函数名称： uploadKeyWord
* 功能描述： 上传关键字
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/31]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiTalkC::uploadKeyWord()
{
/*  No use Now !!!!!!!
    int iRet = 0;
    char *strKeyWord = NULL;
    int iFLen = 0;
    int iKeyWdLen = 0;
    FILE *fp = fopen("/application/work/keywords", "rb");
    if (fp == NULL)
    {
        printf("SendAudioData can not open the file\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    iFLen = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    strKeyWord = (char *)malloc(iFLen + 1);
    memset(strKeyWord, 0, iFLen + 1);

    iKeyWdLen = fread(strKeyWord, sizeof(char), iFLen, fp);
    fclose(fp);

    // 上传词库
    const char *strUploadId = QISRUploadData(m_strSessionId, "contact", 
        strKeyWord, iKeyWdLen, "dtt=keylist", &iRet);

    free(strKeyWord);
    strKeyWord = NULL;

    if (iRet != 0)
    {
        printf("QISRUploadData with errorCode: %d \n", iRet);
        return iRet;
    }

    m_strUploadId = (char *)malloc(strlen(strUploadId) + 1);
    memset(m_strUploadId, 0, strlen(strUploadId) + 1);

    strcpy(m_strUploadId, strUploadId);

    QISRSessionEnd(m_strSessionId, "normal");
    m_strSessionId = NULL;
    */
    return 0;
}

/**
*<FUNC+>********************************************************************
* 函数名称： sendAudioData
* 功能描述： 发送音频数据到服务器中
* 输入参数： audioData：音频数据，iWaveLen：数据长度
* 输出参数： 无
* 返 回	值： 成功返回0，否则返回错误码
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/31]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
int AiTalkC::sendAudioData(char *audioData, size_t iWaveLen)
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

//     printf("QISRAudioWrite ok! len=%d, audio status=%d, rsltStatus = %d\n", 
//         iWaveLen, iAudioStatus, iRsltStatus);

    // 一定要检查一次结果，也许已经识别成功有结果
    if(ISR_REC_STATUS_SUCCESS == iRsltStatus)
    {
        strResult = QISRGetResult(m_strSessionId, &iRsltStatus, 5000, &iRet);
        if(0 != iRet)
        {
            printf("QISRGetResult failed, error code is %d\n", iRet);
            iRsltStatus = ISR_REC_STATUS_SPEECH_COMPLETE;
            return iRet;
        }

        if(NULL != strResult)
        {
            printf("got a result: %s\n", strResult);
            m_strResult.append(strResult);
        }

        // 全部结果已经取完了
        if(ISR_REC_STATUS_SPEECH_COMPLETE == iRsltStatus)
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
* 	[2011/10/31]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
string AiTalkC::readAudioResult(int iRsltStatus, int &iRtnValue)
{
    const char *strResult = NULL;
    int iRet = 0;

    int iAudioStatus = ISR_AUDIO_SAMPLE_LAST;
    int iEqStatus;
    int iRecogStatus;
    iRet = QISRAudioWrite(m_strSessionId, NULL, 0, iAudioStatus, &iEqStatus, &iRecogStatus);

    while(ISR_REC_STATUS_SPEECH_COMPLETE != iRsltStatus)
    {
        strResult = QISRGetResult(m_strSessionId, &iRsltStatus, 5000, &iRet);
        
        iRtnValue = iRet;
        if (0 != iRet)
        {
            // 一直没有获取到数据并且有错误出现，返回空
            printf("QISRGetResult failed, error code is: %d\n", iRet);
            break;
        }

        if (NULL != strResult)
        {
            m_strResult.append(strResult);
            printf("got a result: %s\n", strResult);
        }

        // sleep一下很有必要，防止MSC端无缓存的识别结果时浪费CPU资源
        usleep(160000);
    }

    return m_strResult;
}

/**
*<FUNC+>********************************************************************
* 函数名称： releaseSessionResourch
* 功能描述： 释放资源
* 输入参数： 无
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/10/31]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void AiTalkC::releaseSessionResourch()
{
    printf("releaseSessionResourch,start \n");
    if (m_strSessionId != NULL)
    {
        printf("releaseSessionResourch, m_strSessionId [%s]\n", m_strSessionId);
        QISRSessionEnd(m_strSessionId, NULL);
    }

    printf("QISRFini start \n");
    QISRFini();

    printf("QISRFini over \n");
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
* 	[2011/10/31]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void AiTalkC::run()
{
    int iRet = 0;

    while (1)
    {
        if (m_bFlag)
        {
            // 关闭录音线程
            RecordC::object()->stopVoiceRecord();

            // 设定一个错误值，退出循环
            iRet = -1;
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
                printf("[AiTalkC] SendAudioData failed, %d\n", iRet);
                bStopFlag = true;
                RecordC::object()->stopVoiceRecord();

                break;
            }
        }

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

        printf("[AiTalkC] Get error %d\n", iRet);
        releaseSessionResourch();


        // 关闭语音输入通道
        closeAudioControl();

        // 播放提示音
        playCloseVoice("语音识别结束");

        MyDbusC::object()->aiTalkResult(-1);

        // 发送识别停止信号
        MyDbusC::object()->aiTalkStatusUpdate(Voice1::AT_STATUS_STOP);

        return;
    }

    // 获取数据
    int iTemp;
    string strRtnResult = readAudioResult(iRet, iTemp);

    if (iTemp != 0)
    {
        if (iTemp == MSP_ERROR_NO_DATA)
        {
            printf("[AiTalkC] No data\n");
        }
        else
        {
            printf("[AiTalkC] Get a error %d\n", iTemp);
        }
    }
    else
    {
    	printf("\n=====================================\n");
        printf("[AiTalkC] Get a result %s", strRtnResult.c_str());
		printf("\n=====================================\n");
    }

    string strTempResult = "";

    // 需要解析一下字符串
    strTempResult = parseResult(m_strResult);
    releaseSessionResourch();

    if (strTempResult.empty())
    {
        MyDbusC::object()->aiTalkResult(-1);
    }
    else
    {
        MyDbusC::object()->aiTalkResult(0);

        // 播放识别结果
        playCloseVoice(strTempResult.c_str());
    }

    // 关闭语音输入通道
    closeAudioControl();

    // 播放提示音
    playCloseVoice("语音识别结束");

    // 发送关闭语音识别信号
    MyDbusC::object()->aiTalkStatusUpdate(Voice1::AT_STATUS_STOP);

    printf("[AiTalkC] AiTalkC thread work over \n");
}

/**
*<FUNC+>********************************************************************
* 函数名称： parseResult
* 功能描述： 解析识别的结果
* 输入参数： strResult：需要解析的字符串
* 输出参数： 无
* 返 回	值： 返回解析的结果
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/11/8]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
string AiTalkC::parseResult(string strResult)
{
    printf("parseResult, strResult is %s\n", strResult.c_str());
    if (strResult.empty())
    {
        return "";
    }

    string strTempResult;

    // 字符串格式为 string strSource = "confidence=40 grammar=0 contact=邯郸钢铁  input=邯郸钢铁";
    int iPos = strResult.find("input=");
    if (iPos != (int)string::npos)
    {
        strTempResult = strResult.substr(iPos + 6, strResult.length());
    }

    printf("parseResult, 解析完成后%s\n", strTempResult.c_str());
    return strTempResult;
}

/**
*<FUNC+>********************************************************************
* 函数名称： closeAudioControl
* 功能描述： 关闭语音识别通道
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
void AiTalkC::closeAudioControl()
{
    // 需要关闭语音识别通道[12/6/2011 yuzhenyang]
    AudioControl1::AudioControlLibC::object()->close(AUDIO_CHANNEL_ID_DIGITAL_VOICE_IDENTIFY, false);

    printf("语音识别通道关闭\n");
}

/**
*<FUNC+>********************************************************************
* 函数名称： playCloseVoice
* 功能描述： 播放语音识别结束声音
* 输入参数： strText：要播放的字符串
* 输出参数： 无
* 返 回	值： 无
* 其它说明： 
* 修改记录： 
* 		修改日期        版本号     修改人	      修改内容
* --------------------------------------------------------------------------
* 	[2011/12/10]	    V1.0	   于震洋		  创建函数
*<FUNC+>********************************************************************
*/
void AiTalkC::playCloseVoice(const char *strText)
{
    printf("[AiTalkC] playCloseVoice [%s]\n", strText);

    if (strText == NULL)
    {
        printf("playCloseVoice, strText is null\n");
        return;
    }
    AiPlayC::object()->voicePlay(strText);
}
