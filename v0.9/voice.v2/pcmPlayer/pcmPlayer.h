
#ifndef PCMPLAYER_H
#define PCMPLAYER_H

#include <QThread>
#include <QMutex>
#include <QList>
#include "alsa/asoundlib.h"

//using namespace std;

//#define SAMPLERATE 16000
//#define CH 1
//#define VOICEDATALEN 50000

class PcmPlayerC : public QThread
{
public:
	PcmPlayerC();
	~PcmPlayerC();

	int playPcm(char *pData, int iLen, bool bBlock);
	void	stopPlay();
	
private slots:
	void slotFinished();
	
private:
	QMutex m_mutexForThreadSafe;
	volatile bool m_bExit;

	class PcmData
	{
	public:
		char *pData;
		int iLen;
	};
	typedef QList<PcmData> PcmDataList;
	PcmDataList m_list;
		
	snd_pcm_t *m_hHandle;

private:
	void run();

	int _initPcm(const char *pDeviceName);
	void _releasePcm();
	int _writePcm(char *pData, int iLen);
};

#endif
