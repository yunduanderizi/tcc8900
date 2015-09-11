#ifndef voicePhoneWorkThread_H_20101224
#define	voicePhoneWorkThread_H_20101224 

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "phoneMonitorVoiceReportBase.h"

typedef struct msgqueue
{
        int msgid;
        msgqueue *next;
}msgqueue;


typedef struct sendmsgqueue
{
        char* callNumber;
        char* msgContent;
        unsigned int serialNo;
        sendmsgqueue *next;
}sendmsgqueue;


class PhoneMonitorVoiceInterface;

class VoicePhoneWorkThreadC: public QThread
{
public:
	VoicePhoneWorkThreadC();
	~VoicePhoneWorkThreadC();

public:
	void init(PhoneMonitorVoiceInterface *voicePhone);

	void readPreviousMsg();
        int appendRecvMsg(int id);
	int appendSendMsg(const char* pcNumber,const char* pcContent, unsigned int serialNo);

private:
	void run();

        int RecvListMessage();    //¶ÁÒÅÁô¶ÌÏûÏ¢^M
        int PreferredMessage(int &MinSMCount,int &MaxSMCount);    //»ñÈ¡SM¿¨¶ÌÐÅÈÝÁ¿

	int sendOneMsg(char* cpNumber, char* cpTexting, unsigned int id);

	int recvOneMessage(int id);        //¶ÁÈ¡¶ÌÏûÏ¢^M
	int deleteOneMessage(int iId);     //É¾³ýSIM¿¨¶ÌÏûÏ¢       
        int analysisMessage(char* buf);

        int getRecvMsg();
	sendmsgqueue* getSendMsg();

	int sendMessageErrorRebuild();

private:
	PhoneMonitorVoiceInterface	*m_voicePhone;

private:
	QMutex  	m_mutex; 
	QWaitCondition  m_event;

	msgqueue  *recvHead;
        msgqueue  *recvEnd;	

	int lastReportTime;
	char networkType;

	SignalStrength m_signalStrength;
	NetworkType m_networkType;
	ServiceType m_serviceType;
	
        sendmsgqueue* sendHead;
        sendmsgqueue* sendEnd;
};

#endif

