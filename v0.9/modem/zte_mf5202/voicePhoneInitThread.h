#ifndef voicePhoneInitThread_H_20101223
#define voicePhoneInitThread_H_20101223

#include <QObject>
#include <QThread>

class PhoneMonitorVoiceInterface;

class VoicePhoneInitThreadC: public QThread
{
	Q_OBJECT
public:
	void startInit(PhoneMonitorVoiceInterface *voicePhone);

private:
	void run();

private:
	PhoneMonitorVoiceInterface	*m_voicePhone;

signals:
	void phoneInitedSignal(void);//for bug:calling,auto-system reboot suddently because of unpredictable factor,calling is going on
};

#endif

