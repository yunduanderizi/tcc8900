
#ifndef PCMPLAYER_MANAGER_H
#define PCMPLAYER_MANAGER_H

#include <QThread>
#include <QMutex>
#include <QList>
#include "audioData.h"
#include "pcmPlayer.h"

class PcmPlayerManagerC : public QThread
{
public:
	static PcmPlayerManagerC* object();
	static void releaseObject();
	
	int playPcm(char *pData, int iLen, AudioControl1::AudioChannelIdE id, bool bBlock);
	void	stopPlay(AudioControl1::AudioChannelIdE id);
	
private:
	static PcmPlayerManagerC *thisObject;
	PcmPlayerManagerC();
	~PcmPlayerManagerC();

	QMutex m_mutexForThreadSafe;
		
	PcmPlayerC *m_pPcmPlayer[AudioControl1::AUDIO_CHANNEL_ID_MAX];
};

#endif
