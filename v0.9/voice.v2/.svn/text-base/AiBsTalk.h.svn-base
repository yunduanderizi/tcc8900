/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： VoiceRecogImpl.h
* 文件标识： (见配置管理计划书)
* 内容摘要： 语音识别的实现
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

#ifndef VOICERECOGIMPL_H
#define VOICERECOGIMPL_H

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

/**
*<CLASS+>********************************************************************
* 类 名 称: VoiceRecogImplC
* 功能描述: 语音识别的实现
* 其它说明: 
* 修改记录: 
* 修改日期			版 本		修改人		修改摘要
* -------------------------------------------------------------------------
* [2011/10/17]	    V1.0		于震洋		创建类
*<CLASS->*********************************************************************
*/
class AiBsTalkC : public QThread
{
public:
    AiBsTalkC();
    virtual ~AiBsTalkC();

    static AiBsTalkC* object();
    static void releaseObject();

    int openVoiceRecog();
    int closeVoiceRecog();

private:
    const char *m_strSessionId;
    string m_strResult;

    static AiBsTalkC *thisObject;
    volatile bool m_bFlag;
    int m_iTimerId;
    //QTimer *m_stopThread;

    void run();
    int initVoiceRecog();
    int sendAudioData(char *audioData, size_t iWaveLen);
    string readAudioResult(int iRsltStatus, int &iRtnValue);
    void releaseSessionResourch();
    void closeSoundControl();
    void playVoiceFinishSound();
    void stopRecorze();
    void timerEvent(QTimerEvent *event);
};

#endif