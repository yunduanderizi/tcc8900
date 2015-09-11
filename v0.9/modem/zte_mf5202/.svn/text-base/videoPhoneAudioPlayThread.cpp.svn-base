#include <sys/time.h>
#include <QMutex>
#include "videoPhoneAudioPlayThread.h"
#include "debugMonitor.h"

static QMutex gs_mutexForThreadSafe;

VideoPhoneAudioPlayThreadC::VideoPhoneAudioPlayThreadC(snd_pcm_t* p)
	:m_bExit(false)
{
    m_videoPhoneAudioPlayhandle = p;
    start();
}

VideoPhoneAudioPlayThreadC::~VideoPhoneAudioPlayThreadC()
{
	//gs_mutexForThreadSafe.lock();
    
	if( isRunning() )
	{
		m_bExit = true;
		//terminate();
		wait();
	}
	AudioData* pAudioData = NULL;

	while(!m_list.isEmpty())
	{
		pAudioData = m_list.takeFirst();
        delete pAudioData;
    }
   	IBD_PRINTF(DEBUG_INFOMATION, "\n##### Out VideoPhoneAudioPlayThreadC #####\n\n")
    printf("\n##### Out VideoPhoneAudioPlayThreadC #####\n\n");
	//gs_mutexForThreadSafe.unlock();
}

int VideoPhoneAudioPlayThreadC::putAudioData(const unsigned char* pData, int iLen)
{
	if( NULL == pData )
		return -1;

	if( iLen <= 0 )
		return -1;

	AudioData*  pAudioData = new AudioData(pData, iLen);
	if( !pAudioData )
	{
		printf("new error, so abort, in %s\n", __FILE__);
		abort();
	}
    gs_mutexForThreadSafe.lock();
	m_list.append(pAudioData);
	gs_mutexForThreadSafe.unlock();
		
	return 0;
}

void VideoPhoneAudioPlayThreadC::run()
{
	AudioData* pAudioData = NULL;
	int err;
    bool sFlag = false;
    int mListSize = 50;
    struct timeval tvStart, tvEnd;
    gettimeofday(&tvStart, NULL);
    
	while(1)
	{
		if( m_bExit )
			break;

		gs_mutexForThreadSafe.lock();
		if((m_list.size() < mListSize) && (false == sFlag))
		{
    		gs_mutexForThreadSafe.unlock();
			continue;		    
		}
		sFlag = true;
		if( !m_list.isEmpty() )
		{
            pAudioData = m_list.takeFirst();
		}
		else
		{
		    sFlag = false;
    		printf("\n$$$ ------------------------------------------isEmpty--- $$$\n");
            gettimeofday(&tvEnd, NULL);
            float timeUse = 1000000*(tvEnd.tv_sec-tvStart.tv_sec) + (tvEnd.tv_usec-tvStart.tv_usec);
            timeUse /= 1000*1000;
            printf("                                              *Used time = %fs\n", timeUse);
            tvStart = tvEnd;
    		printf("$$$ ------------------------------------------isEmpty--- $$$\n");    		
    		gs_mutexForThreadSafe.unlock();
    		continue;
		}
/*		if( !m_list.isEmpty() )
		{
			pAudioData = m_list.takeFirst();
		}
		else
		{
    		gs_mutexForThreadSafe.unlock();
    		printf("\n$$$ isEmpty\n");
			usleep(1*1000*1000);
			continue;
		}
*/
		gs_mutexForThreadSafe.unlock();

		//play audio  , todo
    	err = snd_pcm_writei(m_videoPhoneAudioPlayhandle, pAudioData->m_pAudioData, pAudioData->m_iLen/2);
    	if ( err == -EPIPE){
    		if( (err = snd_pcm_prepare(m_videoPhoneAudioPlayhandle)) < 0){
    		    printf("snd_pcm_prepare err!\n");
    		}
    	} 		
		delete pAudioData;
	}
}


