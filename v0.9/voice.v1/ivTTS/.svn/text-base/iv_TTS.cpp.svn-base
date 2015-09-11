#include <QMessageBox>
#include <QKeyEvent>
#include <QWidget>
#include <QtGui/QApplication>
#include <QFile>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include <string.h>
#include "debugMonitor.h"

#include "iv_TTS.h"

TTSRequest::TTSRequest(TTSUser user, const char* str)
	:m_pNext(NULL),
	m_pStr(NULL)
{
	BEGINFUNC_USING_BY_VOICE
			
	m_TTSUser = user;

	m_pStr = (char*)malloc(strlen(str)+1);
	
	if(NULL == m_pStr)
	{
		IBD_PRINTF(4, "malloc error\n");
		throw;
	}

	strcpy(m_pStr, str);

	ENDFUNC_USING_BY_VOICE
}

TTSRequest::~TTSRequest()
{
	BEGINFUNC_USING_BY_VOICE

	if( NULL != m_pStr )
	{
		delete m_pStr;
		m_pStr = NULL;
	}

	ENDFUNC_USING_BY_VOICE
}

void TTSRequest::addNextRequest(TTSRequest* pNext)
{
	BEGINFUNC_USING_BY_VOICE

	m_pNext = pNext;
	
	ENDFUNC_USING_BY_VOICE
}

TTSRequest* TTSRequest::getNextRequest()
{
	BEGINFUNC_USING_BY_VOICE

	ENDFUNC_USING_BY_VOICE

	return m_pNext;
}
		
void TTSRequest::wait()
{
	BEGINFUNC_USING_BY_VOICE

	m_mutexWaitCondition.lock();
	m_condComplete.wait(&m_mutexWaitCondition);
	m_mutexWaitCondition.unlock();
	
	ENDFUNC_USING_BY_VOICE
}

void TTSRequest::complete()
{
	BEGINFUNC_USING_BY_VOICE

	m_mutexWaitCondition.lock();
	m_condComplete.wakeAll();
	m_mutexWaitCondition.unlock();
	
	ENDFUNC_USING_BY_VOICE
}

const char* TTSRequest::getStr()
{
	return m_pStr;
}

TTSUser TTSRequest::getUser()
{
	return m_TTSUser;
}





IvTTS::IvTTS(QThread *parent)
	:QThread(parent),
	m_bExitRun(false),
	m_bClosing(false),
	m_pCurrentRequest(NULL),
	m_pLastRequest(NULL)
{
	BEGINFUNC_USING_BY_VOICE

	IBD_PRINTF(4, "in IvTTS.............\n");
	
	ENDFUNC_USING_BY_VOICE
}

IvTTS::~IvTTS()
{
	BEGINFUNC_USING_BY_VOICE

	/*停止语音合成线程*/
	exitFromRun();
	wait();

	/*删除请求队列中的所有请求*/
	m_mutexRequest.lock();
	while( NULL != m_pCurrentRequest )
	{
		if( User_MainThread == m_pCurrentRequest->getUser() )
		{
			/*主线程发来的请求，立即删除*/
			delete m_pCurrentRequest;
		}
		else if( User_MainThread == m_pCurrentRequest->getUser() )
		{
			/*语音识别线程发来的请求，通知语音识别线程
			   请求已经"完成"，语音识别线程会自己删除它*/
			m_pCurrentRequest->complete();
		}
		
		m_pCurrentRequest = m_pCurrentRequest->getNextRequest();
	}

	m_pLastRequest = NULL;
		
	m_bClosing = true;

	m_mutexRequest.unlock();

	ENDFUNC_USING_BY_VOICE
}

void IvTTS::exitFromRun()
{
	BEGINFUNC_USING_BY_VOICE

	m_bExitRun = true;

	ENDFUNC_USING_BY_VOICE
}

void IvTTS::run()
{
	BEGINFUNC_USING_BY_VOICE

	TTSRequest* pRequest = NULL;
	const char* pStr = NULL;

	while(!m_bExitRun)
	{
		/*从请求队列中取请求*/
		m_mutexRequest.lock();
		while(NULL != m_pCurrentRequest)
		{
			pRequest = m_pCurrentRequest;
			m_pCurrentRequest = m_pCurrentRequest->getNextRequest();
			if( NULL == m_pCurrentRequest )
			{
				/*请求队列已经为空*/
				m_pLastRequest = NULL;
				break;
			}

			/*如果是语音识别线程发来的请求，立即处理*/
			if( User_AiTalk == pRequest->getUser() )
			{
				break;
			}
			
			/*后面还有请求，就扔掉前面的请求*/
			delete pRequest;//其实，这些被扔掉的请求都是主线程发来的请求
			pRequest = NULL;
		}
		m_mutexRequest.unlock();

		if( NULL != pRequest )
		{
			/*处理请求*/
			bool		bIsPlayMode = true;
			pStr = pRequest->getStr();//得到请求合成的字符串

			if(bIsPlayMode)
			{
				/*合成、播放pStr所指的字符串,pStr不用delete*/
				if(m_Aisound.PlayText(pStr) < 0)
				{
					IBD_PRINTF(4, "[iv_TTS]--{run}: Play Str Fail...\n");
				}
			}

			/*通知其他等待的线程，这个请求已经处理完成*/
			pRequest->complete();

			/*从主线程发来的请求是非阻塞的，所以在这里删除它*/
			if( User_MainThread == pRequest->getUser() )
			{
				delete pRequest;
			}

			pRequest = NULL;
		}
		else
		{
			/*没有请求，等待新请求*/
			m_mutexRequest.lock();
			m_condRequest.wait(&m_mutexRequest, 500);//为了防止死锁，也为了提供退出机制，设置0.5s的超时
			m_mutexRequest.unlock();
		}
		
	}

	m_bExitRun = false;

	ENDFUNC_USING_BY_VOICE
}

void IvTTS::AppendRequest(TTSRequest* pRequest)
{
	BEGINFUNC_USING_BY_VOICE

	m_mutexRequest.lock();
	
	if(NULL != m_pLastRequest)
	{
		m_pLastRequest->addNextRequest(pRequest);
		m_pLastRequest = pRequest;
	}
	else
	{/*请求对列为空*/
		m_pCurrentRequest = m_pLastRequest = pRequest;
	}

	/*发送信号，通知线程处理请求*/
	m_condRequest.wakeAll();
	
	m_mutexRequest.unlock();
	
	ENDFUNC_USING_BY_VOICE
}

void IvTTS::PlayString(TTSUser user, const char* str)
{
	BEGINFUNC_USING_BY_VOICE

	m_mutexRequest.lock();
	if(m_bClosing)
	{
		m_mutexRequest.unlock();
		return;
	}
	m_mutexRequest.unlock();
	
	if(User_MainThread == user)
	{
		/*主线程传来的字符串， 非阻塞处理*/
		TTSRequest*	pRequest = new TTSRequest(user, str);

		AppendRequest(pRequest);
		
		pRequest->wait();//等待请求被完成
		
		delete pRequest;
	}
	else if(User_AiTalk == user)
	{
		/*语音识别线程对象传来的字符串，阻塞处理*/
		TTSRequest*	pRequest = new TTSRequest(user, str);
		AppendRequest(pRequest);
		pRequest->wait();//等待请求被完成
		delete pRequest;
	}

	ENDFUNC_USING_BY_VOICE
}
