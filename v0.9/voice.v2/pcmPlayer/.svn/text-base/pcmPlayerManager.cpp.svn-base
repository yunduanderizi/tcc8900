
#include "pcmPlayerManager.h"

PcmPlayerManagerC *PcmPlayerManagerC::thisObject = NULL;

PcmPlayerManagerC* PcmPlayerManagerC::object()
{
	PcmPlayerManagerC *pTmp = NULL;

	if (thisObject == NULL)
	{
		thisObject = new PcmPlayerManagerC();
	}

	pTmp = thisObject;

	return pTmp;
}

void PcmPlayerManagerC::releaseObject()
{
	if (thisObject != NULL)
	{
		delete thisObject;
		thisObject = NULL;
	}
}

PcmPlayerManagerC::PcmPlayerManagerC()
{
	m_mutexForThreadSafe.lock();

	for(int i=0; i<AudioControl1::AUDIO_CHANNEL_ID_MAX; i++)
	{
		m_pPcmPlayer[i] = NULL;
	}

	m_mutexForThreadSafe.unlock();
}

PcmPlayerManagerC::~PcmPlayerManagerC()
{
	m_mutexForThreadSafe.lock();

	for(int i=0; i<AudioControl1::AUDIO_CHANNEL_ID_MAX; i++)
	{
		if( NULL != m_pPcmPlayer[i] )
		{
			delete m_pPcmPlayer[i];
			m_pPcmPlayer[i] = NULL;
		}
	}

	m_mutexForThreadSafe.unlock();
}

int PcmPlayerManagerC::playPcm(char *pData, int iLen, AudioControl1::AudioChannelIdE id, bool bBlock)
{
	PcmPlayerC *pPcmPlayer = NULL;
	
	m_mutexForThreadSafe.lock();

	if( NULL == m_pPcmPlayer[id] )
	{
		m_pPcmPlayer[id] = new PcmPlayerC();
		pPcmPlayer = m_pPcmPlayer[id];
	}
	else
	{
		pPcmPlayer = m_pPcmPlayer[id];
	}
		
	m_mutexForThreadSafe.unlock();

	if( NULL != pPcmPlayer )
	{
		pPcmPlayer->playPcm(pData, iLen, bBlock);
	}
	return 0;
}

void	PcmPlayerManagerC::stopPlay(AudioControl1::AudioChannelIdE id)
{
	PcmPlayerC *pPcmPlayer = NULL;

	m_mutexForThreadSafe.lock();

	pPcmPlayer = m_pPcmPlayer[id];
		
	m_mutexForThreadSafe.unlock();

	if( NULL != pPcmPlayer )
	{
		pPcmPlayer->stopPlay();
	}
}


