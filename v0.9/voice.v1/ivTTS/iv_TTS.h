#ifndef IVTTS_H
#define IVTTS_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "voiceInterface.h"
#include "voice_setting.h"
#include "AisoundTTS.h"

//语音合成请求
class TTSRequest
{
public:
	TTSRequest(TTSUser user, const char* str);
	~TTSRequest();

	void addNextRequest(TTSRequest* pNext);
	TTSRequest* getNextRequest();
	void wait();//等合成完成的信号
	void complete();//合成结束，发送合成完成的信号
	const char* getStr();//返回请求字符串
	TTSUser getUser();//返回发出请求的用户的身份


private:
	TTSRequest*		m_pNext;//下一个请求
	char*			m_pStr;//要合成的字符串
	QWaitCondition	m_condComplete;//合成完成的信号
	QMutex			m_mutexWaitCondition;//保护m_condComplete的互斥量
	TTSUser			m_TTSUser;//发出请求的用户身份
};

/*语音合成线程类*/
class IvTTS: public QThread, public IvTTSInterface
{
	Q_OBJECT
		
public:
	IvTTS(QThread *parent = 0);
	~IvTTS();

	/*实现IvTTSInterface接口*/
	void PlayString(TTSUser user, const char* str);//合成、播放字符串
			
private:
	volatile bool				m_bExitRun;//是否退出run函数
	
	/*下面变量用于实现多线程操作*/
	volatile bool				m_bClosing;//是否正在关闭自己
									      //这个变量用来在析构自身的时候设置，
									      //之后其他对象通过PlayString的调用都会立即返回
	TTSRequest*				m_pCurrentRequest;//当前请求
	TTSRequest*				m_pLastRequest;//最后一个请求
	QWaitCondition			m_condRequest;//通知新请求的信号
	QMutex					m_mutexRequest;//保护m_pCurrentRequest、m_pLastRequest、m_condRequest、m_bClosing
											 //的互斥量
											 //这个互斥量不管TTSRequest内部的数据，TTSRequest内部可能会有自己的互斥量	

	/*TODO下面的变量、函数用于合成播放的具体实现*/
	Aisound					m_Aisound;

private:
	void AppendRequest(TTSRequest* pRequest);//增加一个请求
	void exitFromRun();//通知线程从run函数中退出
	void run();
};

#endif // IVTTS_H

