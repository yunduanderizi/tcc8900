#ifndef videoPhoneAudioPlayThread_H_20110328
#define videoPhoneAudioPlayThread_H_20110328

#include <QThread>
#include <stdlib.h>
#include "alsa/asoundlib.h"

class VideoPhoneAudioPlayThreadC: public QThread
{
	Q_OBJECT
public:
	VideoPhoneAudioPlayThreadC(snd_pcm_t* p);
	~VideoPhoneAudioPlayThreadC();
	
	int putAudioData(const unsigned char* pData, int iLen);

private:
	void run();

private:
	class AudioData
	{
	public:
		AudioData(const unsigned char* pData, int iLen)
		{
			m_pAudioData = (unsigned char*)malloc(iLen); 
			memcpy(m_pAudioData, pData, iLen); 
			m_iLen=iLen;
		}
		~AudioData(){delete m_pAudioData;}
		
		unsigned char *m_pAudioData;
		int m_iLen;
	};
	typedef QList<AudioData*> AudioDataList;
	AudioDataList m_list;
	
	volatile bool m_bExit;
	snd_pcm_t* m_videoPhoneAudioPlayhandle;
};

#endif


