//*********************************************************************************************/
//*********************************************************************************************/
//**  模块功能:语音识别                                                                                                             **/
//**                                                                                                                                                            **/
//**  文件:aitalk.h  (主头文件，定义语音识别类(AiTalk)被用户使用)                     **/
//**              aitalk.cpp  (主cpp，通过语音识别类(AiTalk)对象，创建、控制两个    **/
//**                               线程，一个进行语音识别，另一个输入语音数据)        **/
//**              aitalk_setting.h  (词典遥控器设置结构)                                                                  **/
//**              aitalk_setting.c  (词典遥控器设置数组)                                                                  **/
//**                                                                                                                                                           **/
//**  目录: Inc  (科大讯飞嵌入式语音识别引擎头文件)                                           **/
//**               Lib  (科大讯飞嵌入式语音识别引擎库文件)                                            **/
//**               Resource  (科大讯飞嵌入式语音识别引擎资源文件)                             **/
//**                                                                                                                                                           **/
//**  对外接口:  StartIdentify()  (用来开始语音识别功能，调用这个函数，     **/
//**                                                   将开始语音识别线程类的两个工作线程)         **/
//**                           StopIdentify()  (用来停止语音识别功能，调用这个函数，      **/
//**                                                   将停止语音识别线程类的两个工作线程)          **/
//**                                                                                                                                                           **/
//**  作者: 陈涛、张赛君、何博乾                                                                                     **/
//**  时间: 2009/11/20                        																		**/
//*********************************************************************************************/
//**  语音识别内容修改
//**  将原来的遥控码通讯改为模拟真实按键
//**  将键值通过ioctl送给按键驱动，按键驱动识别到键值后，会上报应用，应用从设备节点抓取
//**  修改时间:2010/7/15
//**  修改者: 王东晟
//*********************************************************************************************/
//*********************************************************************************************/
#ifndef AITALK_H
#define AITALK_H

#include <QThread>
#include <QMutex>
#include "voiceInterface.h"
#include "voice_setting.h"
#include "ivTTS.h"
#include "ivESR.h"
#include "alsa/asoundlib.h"

#define AIWORKBUFFSIZE		504400		//每个词占用0.06K空间。即0.06 * 1024 = 61.44个字节，200 * 61.44 = 
#define AIRESIBUFFSIZE	32768			


#define PCMBUFFSIZE 163840

#if 0
#define STATUS 								"STATUS\n"
#define GPS									"GPS\n"
#define MUSIC								"MUSIC\n"
#define MOVIE								"MOVIE\n"
#define PICTURE								"PICTURE\n"
#define TEXT								"TEXT\n"
#define FM									"FM\n"
#define TVOUT								"TVOUT\n"
#define PLAY								"PLAY\n"
#define PAUSE								"PAUSE\n"
#define STOP								"STOP\n"
#define FULLSCREEN							"FULLSCREEN\n"
#define VPREV								"VPREV\n"
#define NEXT								"NEXT\n"
#define VOL_UP							"VOL_UP\n"
#define VOL_DOWN						"VOL_DOWN\n"
#define ONE_KEY						"ONE_KEY\n"
#endif

#define SEND_AITALK_AISTART				87
#define SEND_AITALK_AISTOP				88


#define SEND_AITALK_STATUS 				59
#define SEND_AITALK_GPS					60
#define SEND_AITALK_MUSIC				61
#define SEND_AITALK_MOVIE				62
#define SEND_AITALK_PICTURE				63
#define SEND_AITALK_TEXT				64
#define SEND_AITALK_FM					65
#define SEND_AITALK_TVOUT				66
#define SEND_AITALK_FULLSCREEN			67
#define SEND_AITALK_VPREV				68
#define SEND_AITALK_NEXT				71

#define SEND_AITALK_PLAY				207
#define SEND_AITALK_PAUSE				164
#define SEND_AITALK_STOP				128


#define SEND_AITALK_VOL_UP				4
#define SEND_AITALK_VOL_DOWN			3
#define SEND_AITALK_ONE_KEY			2





typedef void* HiREC;

#if 1
/*载入音频PCM数据的线程*/
class PCMDataLoader : public QThread
{
	Q_OBJECT
public:
	PCMDataLoader(ivPointer hESRObj, QThread *parent = 0);
	~PCMDataLoader();
	void exitFromRun();//通知线程从run函数中退出
	void StartAppend();//开始填充数据
	void StopAppend();//停止填充数据
 
protected:
	void run();
	volatile bool m_bExitRun;//是否退出run函数
	volatile bool m_bAppendData;//是否填充数据
	int ReadPcmBuf(char* Repbuf);
	void ReleasePcm();
	int InitPcm();

private:
	/* Aitalk CE 2.2 对象句柄 */
	ivPointer m_hESRObj;
	snd_pcm_t *capture_handle;
	char *PcmBuf;
	snd_pcm_stream_t stream;
	int quiet_mode;
};
#endif


/*语音识别线程类*/
class AiTalk: public QThread, public AiTalkInterface, public AiTalkInterfaceForMainthread
{
	Q_OBJECT
		
public:
	AiTalk(QThread *parent = 0);
	~AiTalk();

	bool audioServerBusy;
	QMutex m_mutexAiTalkAudioServerBusy;
	/*实现AiTalkInterface接口*/
	bool StartIdentify();//开始语音识别功能
	bool StopIdentify();//停止语音识别功能
	bool GetIdentifyState();//得到语音识别功能的状态，true表示语音识别功能打开，false表示语音识别功能关闭


	/*实现AiTalkInterfaceForMainthread接口*/
	void NotifyState();//查询现在语音识别所处的状态，是打开，还是关闭，查询结果会作为遥控码发送
#if 0
	void ProhibitEnterIdentifyMode();
	void AllowEnterIdentifyMode();
#endif

    	/*用来获取IvTTSInterface接口*/
	bool RegisterInterface(IvTTSInterface* pInterface);

private:
	volatile bool		m_bExitRun;//是否退出run函数
	volatile bool		m_bState;//语音识别所处状态，true是打开状态，false是关闭状态
	PCMDataLoader*	m_pPCMDataLoader;//载入音频PCM数据的线程
	ivPointer			m_hESRObj;//Aitalk CE 2.2 对象句柄
	char			pAiWorkBuffer[AIWORKBUFFSIZE];
	char			pAiResidentBuffer[AIRESIBUFFSIZE];
	FILE*			m_pResourceFile;//指向资源文件的指针
	QMutex			m_mutexAiTalk;//保护StartIdentify、StopIdentify同一时间，只能有一个被调用
#if 1
	IvTTSInterface*			m_pIvTTSInterface;//指向IvTTSInterface接口的指针
#endif
	volatile bool		m_bAllowEnterIdentifyMode;//是否允许进入语音识别模式	
	
private:
	void exitFromRun();//通知线程从run函数中退出
	void run();
	ivBool AitalkCreateObject();//Aitalk CE 2.2对象创建 
	ivBool AitalkDestryObject();//Aitalk CE 2.2对象销毁 
	ivBool AitalkInputTextData();//词典，句式，场景的设计和输入

	
};

#endif // AITALK_H

