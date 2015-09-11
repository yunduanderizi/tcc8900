/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： AiTalk.h
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

#ifndef AITALK_H
#define AITAKL_H

#include "qisr.h"
#include <stdio.h>
#include <string>
#include <list>
#include <QThread>
#include <QMutex>
#include "msp_errors.h"
#include "myDbus.h"

using namespace std;
using namespace Voice1;
using namespace AudioControl1;

/**
*<CLASS+>********************************************************************
* 类 名 称: AiTalkC
* 功能描述: 本地识别语音
* 其它说明: 
* 修改记录: 
* 修改日期			版 本		修改人		修改摘要
* -------------------------------------------------------------------------
* [2011/10/20]	    V1.0		于震洋		创建类
*<CLASS->*********************************************************************
*/
class AiTalkC : public QThread
{
public:
    AiTalkC();
    virtual ~AiTalkC();

    static AiTalkC* object();
    static void releaseObject();

    int startVoiceRecorg();
    int stopVoiceRecorg();

private:
    static AiTalkC *thisObject;

    const char *m_strSessionId;
    char *m_strUploadId;
    string m_strResult;
    volatile bool m_bFlag;

    void run();
    void getGrammId(string &strGrammId);
    int init();
    int uploadKeyWord();
    int sendAudioData(char *audioData, size_t iWaveLen);
    string readAudioResult(int iRsltStatus, int &iRtnValue);
    void releaseSessionResourch();
    string parseResult(string strResult);
    void closeAudioControl();
    void playCloseVoice(const char *strText);
};

#endif
