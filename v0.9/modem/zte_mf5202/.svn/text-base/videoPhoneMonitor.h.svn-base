#ifndef VIDEO_PHONE_MONITOR_20101227
#define VIDEO_PHONE_MONITOR_20101227

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "vtlib.h"
#include "alsa/asoundlib.h"


class PhoneConnectionC;
class PhoneMonitorVoiceReportBase;

class VideoPhoneMonitorC : public QThread
{
public:
	VideoPhoneMonitorC();
	~VideoPhoneMonitorC();

public:
	int init(PhoneMonitorVoiceReportBase *reportHandle);

	int waitIncommingCall();

	int dial(const char* cpNumber); 
        int pickUp(); 
        int hangUp();


public: //internal implementation need function
	int writeBuffer(char buffer[],int bufferLen);
	int readBuffer(char buffer[],int bufferLen);
	int hangUpInternal(int reason);


private:
	void run();
	void 	timerEvent(QTimerEvent * event);

	int startVideoCall();
	int endVideoCall();
	int i2cStartPhoneVideo();
	int i2cEndPhoneVideo();
	
public:
	static VideoPhoneMonitorC *thisObject;

private:
	PhoneConnectionC     *m_pPhoneConnection;
	PhoneMonitorVoiceReportBase     *m_pCallback;

	char            m_phoneNumber[30];
	int             m_phoneStatus;
        QMutex          m_mutex;

        QMutex          m_waitMutex;
	QWaitCondition  m_event;

	int		m_timerId;
	unsigned char m_i2cOldVal;
	bool m_flagInit;

private:
	friend class PhoneMonitorVoiceReportBase;
};

#endif
