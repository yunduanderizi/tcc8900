
//#include <stdlib.h>
#include "pcmPlayer.h"
//#include <stdio.h>
//#include <string.h>
//#include "qtts.h"

PcmPlayerC::PcmPlayerC()
	: m_bExit(false),
	  m_hHandle(NULL)
{
	_initPcm("plug:tcc");
}

PcmPlayerC::~PcmPlayerC()
{
	_releasePcm();
}

int PcmPlayerC::playPcm(char *pData, int iLen, bool bBlock)
{
	m_mutexForThreadSafe.lock();

	PcmData myData;
	myData.pData = (char*)malloc(iLen);
	memcpy(myData.pData, pData, iLen);
	myData.iLen = iLen;
	m_list.append(myData);
	
	if( !isRunning() )
	{
		m_bExit = false;
		
		start();
	}

	m_mutexForThreadSafe.unlock();

	if(bBlock)
	{
		wait();
	}
	return 0;
}

void	PcmPlayerC::stopPlay()
{
	m_mutexForThreadSafe.lock();

	m_list.clear();
	
	if( isRunning() )
	{
		m_bExit = true;
	}

	m_mutexForThreadSafe.unlock();
}

void PcmPlayerC::slotFinished()
{
	m_mutexForThreadSafe.lock();

	if( (!m_list.isEmpty()) && (!isRunning()) )
	{
		m_bExit = false;
		start();
	}
		
	m_mutexForThreadSafe.unlock();
}

int PcmPlayerC::_initPcm(const char *pDeviceName)
{
	int retval;
	int dir;
	unsigned int val;
	snd_pcm_hw_params_t* params;
	snd_pcm_uframes_t frames;
	int channels = 0;

	//if( (retval = snd_pcm_open(&capture_handle, "plug:tcc", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
	if( (retval = snd_pcm_open(&m_hHandle, pDeviceName, SND_PCM_STREAM_PLAYBACK, 0)) < 0)
	{
		fprintf(stderr, "cannot open audio device (%s) reason(%s)", pDeviceName, snd_strerror (retval));
		return -1;
	}

	snd_pcm_hw_params_alloca(&params);

	if( (retval = snd_pcm_hw_params_any(m_hHandle, params)) < 0)
	{
		fprintf(stderr, "snd_pcm_hw_params_any failed!  err = %d\n", retval);
		return -1;
	}

	if( (retval = snd_pcm_hw_params_set_access(m_hHandle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
	{
		fprintf(stderr, "snd_pcm_hw_params_set_access failed! err = %d\n", retval);
		return -1;
	}
	if( (retval = snd_pcm_hw_params_set_format(m_hHandle, params, SND_PCM_FORMAT_S16_LE)) < 0)
	{
		fprintf(stderr, "snd_pcm_hw_params_set_format failed!  err = %d\n", retval);
		return -1;
	}

	channels = 1;
	if( (retval = snd_pcm_hw_params_set_channels(m_hHandle, params, channels)) < 0)
	{
		fprintf(stderr, "snd_pcm_hw_params_set_channels failed!	 err = %d\n", retval);
		return -1;
	}

	val = 16000;
	if( (retval = snd_pcm_hw_params_set_rate_near(m_hHandle, params, &val, &dir)) < 0)
	{
		fprintf(stderr, "snd_pcm_hw_params_set_rate_near failed!	err = %d\n", retval);
		return -1;
	}

	frames = 4096;

	if( (retval = snd_pcm_hw_params_set_period_size_near(m_hHandle, params, &frames, &dir)) < 0)
	{
		fprintf(stderr, "snd_pcm_hw_params_set_period_size_near failed!	err = %d\n", retval);
		return -1;
	}
	if( (retval = snd_pcm_hw_params(m_hHandle, params)) < 0)
	{
		fprintf(stderr, "snd_pcm_hw_params-failed! err = %d err: %s\n", retval, snd_strerror(retval));
		return -1;
	}

	if( (retval = snd_pcm_hw_params_get_period_size(params, &frames, &dir)) < 0)
	{
		fprintf(stderr, "snd_pcm_hw_params_get_period_size failed!  err = %d\n", retval);
		return -1;
	}

	if( (retval = snd_pcm_hw_params_get_period_time(params, &val, &dir)) < 0)
	{
		fprintf(stderr, "snd_pcm_hw_params_get_period_time failed! err = %d\n", retval);
		return -1;
	}

	return 0;
}

void PcmPlayerC::_releasePcm()
{
	if(m_hHandle)
	{
		snd_pcm_drain(m_hHandle);
		snd_pcm_close(m_hHandle);
		m_hHandle = NULL;
	}
}

int PcmPlayerC::_writePcm(char *pData, int iLen)
{
	int retval = 0;

	retval = snd_pcm_writei(m_hHandle, pData, iLen/2);
	if( retval == -EPIPE )
	{
		fprintf(stderr,"snd_pcm_writei error! err: %s\n", snd_strerror(retval));
		if( (retval = snd_pcm_prepare(m_hHandle)) < 0)
		{
			fprintf(stderr,"snd_pcm_prepare_failed! err: %s\n", snd_strerror(retval));
		}
		return 0;
	}
	else if( retval < 0 )
	{
		fprintf(stderr,"snd_pcm_writei error retval[%d]!\n", retval);
		return -1;
	}
	else
	{
		return 0;
	}
	
}

void PcmPlayerC::run()
{
	PcmData myData;

	if( NULL == m_hHandle )
	{
		printf("pcm have not been initialized!\n");

		m_mutexForThreadSafe.lock();
		m_list.clear();
		m_mutexForThreadSafe.unlock();
		return;
	}
	
	while(1)
	{
		if( m_bExit )
			break;
			
		m_mutexForThreadSafe.lock();
		if( !m_list.isEmpty() )
		{
			myData = m_list.takeFirst();
		}
		else
		{
			m_mutexForThreadSafe.unlock();
			break;
		}
		m_mutexForThreadSafe.unlock();
		
		_writePcm(myData.pData, myData.iLen);
	}
}

