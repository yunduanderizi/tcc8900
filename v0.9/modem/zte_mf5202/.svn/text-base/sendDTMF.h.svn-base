/**
 * @file   sendDTMF.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110303
 * 
 * @brief  
 *      Implement for DTMF send
 * 
 */
 
//#include "MobileInterFace1.h"

#ifndef SENDDTMF_H_20110303
#define SENDDTMF_H_20110303

#include <QObject>
#include <QTimerEvent>

//#include "audio_control_client.h"
//#include "play_sign_sound.h"





//typedef int (*FunRtDial)(char);	

class PhoneMonitorVoiceInterface1;
class SendDTMFC : public QObject
{
	Q_OBJECT		
public:
	SendDTMFC(PhoneMonitorVoiceInterface1 *pFather);
	~SendDTMFC();

	int send(char *pContent);
	void stopSend();

signals:
	void sigFinishedSend();

private:
	void timerEvent(QTimerEvent * event);
	int sendOneByte();
	
private:
	PhoneMonitorVoiceInterface1 *m_pFather;
	//FunRtDial	m_rtDial;
	char *m_pContent;
	int m_iCurSendByteNum;
	int m_iTimerForSend;
};


#endif


