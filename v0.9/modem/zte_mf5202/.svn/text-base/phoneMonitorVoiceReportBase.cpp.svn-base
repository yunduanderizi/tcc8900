#include <QApplication>

#include "phoneMonitorVoiceReportBase.h"

void PhoneMonitorVoiceReportBase::newCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	printf("new call: %s\n",cPhoneNumber);
}
void PhoneMonitorVoiceReportBase::callConnect( void )     
{
	printf("call connected.\n");
}
void PhoneMonitorVoiceReportBase::callHangUp( int reason )
{
	printf("call hangup. reason=%d\n",reason);
}
void PhoneMonitorVoiceReportBase::missCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	printf("miss call: %s\n",cPhoneNumber);
}
void PhoneMonitorVoiceReportBase::newSMS( ReportSMSData  SMSData )
{
	printf("new SMS: phone=%s  time=%s contents=%s\n",SMSData.cNumber,SMSData.cSMSTime,SMSData.SMSContent);
}
void PhoneMonitorVoiceReportBase::sendMSGToNavi(char *SMSContent)
{
	printf("sendMSGTONavi: %d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);
}
void PhoneMonitorVoiceReportBase::newSMSToReg(char *SMSContent)
{
	printf("newSMSToReg: %d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);
}
bool PhoneMonitorVoiceReportBase::newSMSToSetup(ReportSMSData SMSData)
{
	printf("newSMSToSetup: %d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);

        return false;
}

int PhoneMonitorVoiceReportBase::setPhoneMute(bool val, int channelID)
{
	printf("... %d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);

        return 0;
}
int PhoneMonitorVoiceReportBase::freightageManager(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	printf("... %d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);

        return NOT_FREIGHTAGE_MANAGER_PHONE_NUMBER;
}

void PhoneMonitorVoiceReportBase::smsSendResult( int result )
{
	printf("sms send result: result=%d\n",result);
}
void PhoneMonitorVoiceReportBase::smsSendWithIdResult(unsigned int id, int result)
{
	printf("sms send with id result: id=%d result=%d\n", id, result);
}
void PhoneMonitorVoiceReportBase::signalAndNetworkType(int signal,int networkType)
{
	printf("signalAndNetworkType: signal=%d networkType=%d\n",signal,networkType);
}

void PhoneMonitorVoiceReportBase::signalAndNetworkTypeAndServiceType(SignalStrength signal, NetworkType network, ServiceType service,int signal_strength)
{
	printf("signalAndNetworkTypeAndServiceType: signal=%d networkType=%d serviceType=%d signal_strength = %d \n",signal,network,service,signal_strength);
}

void PhoneMonitorVoiceReportBase::ring(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	printf("Ring: phone=%s\n",cPhoneNumber);
}
void PhoneMonitorVoiceReportBase::newFaceTimeCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	printf("newFaceTimeCall: phone=%s\n",cPhoneNumber);
}
void PhoneMonitorVoiceReportBase::endFaceTimeCall(int reason)
{
	printf("endFaceTimeCall: reason=%d\n",reason);
}
void PhoneMonitorVoiceReportBase::connectFaceTimeCall( void )
{
	printf("connectFaceTimeCall.\n");
}
void PhoneMonitorVoiceReportBase::missFaceTimeCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	printf("missFaceTimeCall: phone=%s\n",cPhoneNumber);
}
void PhoneMonitorVoiceReportBase::hangupBySelf(void)
{
	printf(".....%d,%s,%s...\n",__LINE__,__func__,__FILE__);
}

//短信查询设置车机参数 [zhenyubin 2015/09/09]
bool PhoneMonitorVoiceReportBase::newCommandSMS(SMS_CMD_STRUCT recv)
{
    printf("newCommandSMS---------------\n");
}


/*void PhoneMonitorVoiceReportBase::bsCommandProcessResult(Bs_Command_Type type, int result, void *pRetData)
{
	printf("bsCommandProcessResult: type=%d result=%d retData=%p\n", type, result, pRetData);
}*/

#define NEW_CALL_EVENT		(QEvent::User+100)
#define CALL_CONNECTED_EVENT	(QEvent::User+101)
#define CALL_HANG_UP_EVENT	(QEvent::User+102)
#define MISS_CALL_EVENT		(QEvent::User+103)
#define NEW_SMS_EVENT		(QEvent::User+104)
#define SMS_SEND_RESULT_EVENT	(QEvent::User+105)
#define SIGNAL_NETWORK_TYPE_NOTIFY_EVENT	(QEvent::User+106)
#define RING_EVENT			(QEvent::User+107)
#define NEW_FACE_TIME_CALL_EVENT 	(QEvent::User+108)
#define END_FACE_TIME_CALL_EVENT 	(QEvent::User+109)
#define FACE_TIME_CALL_CONNECTED_EVENT 	(QEvent::User+110) 
#define MISS_FACE_TIME_CALL_EVENT      	(QEvent::User+111) 
#define SIGNAL_NETWORK_SERVICE_TYPE_NOTIFY_EVENT	(QEvent::User+112)
#define SMS_SEND_WITH_ID_RESULT_EVENT	(QEvent::User+113)
#define CALL_HANG_UP_BY_SELF_EVENT		(QEvent::User+114)


//¶¨ÒåÊÂ¼þ´¦ÀíÐÂÀ´µçÍ¨Öª
class PhoneMonitorNewCallEventC: public QEvent
{
public:
	PhoneMonitorNewCallEventC():QEvent((QEvent::Type)NEW_CALL_EVENT){}

public:
	char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN];	
};


//¶¨ÒåÊÂ¼þ´¦Àíµç»°½ÓÍ¨Í¨Öª
class PhoneMonitorCallConnectEventC: public QEvent
{
public:
	PhoneMonitorCallConnectEventC():QEvent((QEvent::Type)CALL_CONNECTED_EVENT){}
};


//¶¨ÒåÊÂ¼þ´¦Àíµç»°¹Ò¶ÏÍ¨Öª
class PhoneMonitorCallHangUpEventC: public QEvent
{
public:
	PhoneMonitorCallHangUpEventC():QEvent((QEvent::Type)CALL_HANG_UP_EVENT){}

public:
	int reason;
};

class PhoneMonitorHangupBySelfEventC:public QEvent
{
	public:
		PhoneMonitorHangupBySelfEventC():QEvent((QEvent::Type)CALL_HANG_UP_BY_SELF_EVENT){}
};


//¶¨ÒåÊÂ¼þ´¦ÀíÎ´½ÓÀ´µçÍ¨Öª
class PhoneMonitorMissCallEventC: public QEvent
{
public:
	PhoneMonitorMissCallEventC():QEvent((QEvent::Type)MISS_CALL_EVENT){}

public:
	char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN];
};

//¶¨ÒåÊÂ¼þ´¦ÀíÐÂ¶ÌÐÅÍ¨Öª
class PhoneMonitorNewSMSEventC: public QEvent
{
public:
	PhoneMonitorNewSMSEventC():QEvent((QEvent::Type)NEW_SMS_EVENT){}

public:
	ReportSMSData  SMSData;
};

//¶¨ÒåÊÂ¼þ´¦Àí¶ÌÐÅ½á¹ûÍ¨Öª
class PhoneMonitorSmsSendResultEventC: public QEvent
{
public:
	PhoneMonitorSmsSendResultEventC():QEvent((QEvent::Type)SMS_SEND_RESULT_EVENT){}

public:
	int result;
};

class PhoneMonitorSmsSendWithIdResultEventC: public QEvent
{
public:
	PhoneMonitorSmsSendWithIdResultEventC():QEvent((QEvent::Type)SMS_SEND_WITH_ID_RESULT_EVENT){}

public:
	int id;
	int result;
};

//¶¨ÒåÊÂ¼þ´¦ÀíÐÅºÅÇ¿¶ÈÍ¨Öª
class PhoneMonitorSignalAndNetworkTypeNotifyEventC: public QEvent
{
public:
	PhoneMonitorSignalAndNetworkTypeNotifyEventC():QEvent((QEvent::Type)SIGNAL_NETWORK_TYPE_NOTIFY_EVENT){}


public:
	int signal;
	int networkType;
};

class PhoneMonitorSignalAndNetworkTypeAndServiceTypeNotifyEventC: public QEvent
{
public:
	PhoneMonitorSignalAndNetworkTypeAndServiceTypeNotifyEventC():QEvent((QEvent::Type)SIGNAL_NETWORK_SERVICE_TYPE_NOTIFY_EVENT){}

public:
	SignalStrength signal;
	NetworkType network;
	ServiceType service;
	int signal_strength;
};

//¶¨ÒåÊÂ¼þ´¦ÀíÕñÁåÍ¨Öª
class PhoneMonitorRingEventC: public QEvent
{
public:
	PhoneMonitorRingEventC():QEvent((QEvent::Type)RING_EVENT){}

public:
	char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN];
};

//¶¨ÒåÊÂ¼þ´¦Àí¿ÉÊÓµç»°À´µç
class PhoneMonitorNewFaceTimeCallEventC: public QEvent
{
public:
	PhoneMonitorNewFaceTimeCallEventC():QEvent((QEvent::Type)NEW_FACE_TIME_CALL_EVENT){}


public:
	char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN];
};


//¶¨ÒåÊÂ¼þ´¦Àí¿ÉÊÓµç»°¹Ò¶Ï
class PhoneMonitorEndFaceTimeCallEventC: public QEvent
{
public:
	PhoneMonitorEndFaceTimeCallEventC():QEvent((QEvent::Type)END_FACE_TIME_CALL_EVENT){}

public:
	int reason;
};

//¶¨ÒåÊÂ¼þ´¦Àí¿ÉÊÓµç»°½ÓÍ¨
class PhoneMonitorFaceTimeCallConnectEventC: public QEvent
{
public:
	PhoneMonitorFaceTimeCallConnectEventC():QEvent((QEvent::Type)FACE_TIME_CALL_CONNECTED_EVENT){}
};


//¶¨ÒåÊÂ¼þ´¦Àí¿ÉÊÓµç»°Î´½ÓÀ´µçÍ¨Öª
class PhoneMonitorMissFaceTimeCallEventC: public QEvent
{
public:
	PhoneMonitorMissFaceTimeCallEventC():QEvent((QEvent::Type)MISS_FACE_TIME_CALL_EVENT){}


public:
	char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN];
};



void PhoneMonitorVoiceReportBase::toNewCall( char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN] )
{
	PhoneMonitorNewCallEventC *phoneEvent =new PhoneMonitorNewCallEventC();
	strcpy(phoneEvent->cPhoneNumber, cPhoneNumber);
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toCallConnect(  )
{
	PhoneMonitorCallConnectEventC *phoneEvent =new PhoneMonitorCallConnectEventC();
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toCallHangUp( int reason )
{
	PhoneMonitorCallHangUpEventC *phoneEvent = new PhoneMonitorCallHangUpEventC();
	phoneEvent->reason = reason;
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toCallHangUpBySelf(void)//for bug:1058,HANGUP-BUTTON may appear after hanguping by self
{
	PhoneMonitorHangupBySelfEventC *phoneEvent =new PhoneMonitorHangupBySelfEventC();
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toMissCall( char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN] )
{
	PhoneMonitorMissCallEventC *phoneEvent =new PhoneMonitorMissCallEventC();
	strcpy(phoneEvent->cPhoneNumber, cPhoneNumber);
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toNewSMS( ReportSMSData  SMSData )
{
	PhoneMonitorNewSMSEventC *phoneEvent =new PhoneMonitorNewSMSEventC();
	phoneEvent->SMSData = SMSData;
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toSmsSendResult( int result )
{
	PhoneMonitorSmsSendResultEventC *phoneEvent =new PhoneMonitorSmsSendResultEventC();
	phoneEvent->result = result;
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toSmsSendWithIdResult(unsigned int id, int result )
{
	PhoneMonitorSmsSendWithIdResultEventC *phoneEvent =new PhoneMonitorSmsSendWithIdResultEventC();
	phoneEvent->result = result;
	phoneEvent->id = id;
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toSignalAndNetworkType(int signal,int networkType)
{
	PhoneMonitorSignalAndNetworkTypeNotifyEventC *phoneEvent =new PhoneMonitorSignalAndNetworkTypeNotifyEventC();

	phoneEvent->signal = signal;
	phoneEvent->networkType= networkType;
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toSignalAndNetworkTypeAndServiceType(SignalStrength signal, NetworkType network, ServiceType service,int signal_strength)
{
	PhoneMonitorSignalAndNetworkTypeAndServiceTypeNotifyEventC *phoneEvent =new PhoneMonitorSignalAndNetworkTypeAndServiceTypeNotifyEventC();

	phoneEvent->signal = signal;
	phoneEvent->network = network;
	phoneEvent->service = service;
	phoneEvent->signal_strength = signal_strength;
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toRing(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	PhoneMonitorRingEventC *phoneEvent =new PhoneMonitorRingEventC();
	strcpy(phoneEvent->cPhoneNumber, cPhoneNumber);
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toNewFaceTimeCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	PhoneMonitorNewFaceTimeCallEventC *phoneEvent =new PhoneMonitorNewFaceTimeCallEventC();
	strcpy(phoneEvent->cPhoneNumber, cPhoneNumber);
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toEndFaceTimeCall(int reason)
{
	PhoneMonitorEndFaceTimeCallEventC *phoneEvent =new PhoneMonitorEndFaceTimeCallEventC();
	phoneEvent->reason = reason;
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toFaceTimeCallConnect(  )
{
	PhoneMonitorFaceTimeCallConnectEventC *phoneEvent =new PhoneMonitorFaceTimeCallConnectEventC();	
	QApplication::postEvent(this,phoneEvent);
}

void PhoneMonitorVoiceReportBase::toFaceTimeMissCall( char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN] )
{
	PhoneMonitorMissFaceTimeCallEventC *phoneEvent =new PhoneMonitorMissFaceTimeCallEventC();
	strcpy(phoneEvent->cPhoneNumber, cPhoneNumber);
	
	//IBD_PRINTF(DEBUG_INFOMATION, "post PhoneMonitorMissFaceTimeCallEventC type[%d]" , phoneEvent->type())

	QApplication::postEvent(this,phoneEvent);
}


bool PhoneMonitorVoiceReportBase::event(QEvent *e)
{
	//printf("PhoneReport::event===%d\n",e->type());

	switch((int)(e->type()))
	{
		case NEW_CALL_EVENT:
		{
			PhoneMonitorNewCallEventC *phoneEvent =(PhoneMonitorNewCallEventC*)e;
			newCall(phoneEvent->cPhoneNumber);
			return TRUE;
		}
		case CALL_CONNECTED_EVENT:
		{
			PhoneMonitorCallConnectEventC *phoneEvent =(PhoneMonitorCallConnectEventC*)e;
			callConnect();
			return TRUE;
		}
		case CALL_HANG_UP_EVENT:
		{
			PhoneMonitorCallHangUpEventC *phoneEvent =(PhoneMonitorCallHangUpEventC*)e;
			callHangUp(phoneEvent->reason);
			return TRUE;
		}
		case CALL_HANG_UP_BY_SELF_EVENT:
		{
			PhoneMonitorHangupBySelfEventC *phoneEvent = (PhoneMonitorHangupBySelfEventC *)e;
			hangupBySelf();
			return TRUE;
		}
		case MISS_CALL_EVENT:
		{
			PhoneMonitorMissCallEventC *phoneEvent =(PhoneMonitorMissCallEventC*)e;
			missCall(phoneEvent->cPhoneNumber);
			return TRUE;
		}
		case NEW_SMS_EVENT:
		{
			PhoneMonitorNewSMSEventC *phoneEvent =(PhoneMonitorNewSMSEventC*)e;
			newSMS(phoneEvent->SMSData);
			return TRUE;
		}
		case SMS_SEND_RESULT_EVENT:
		{
			PhoneMonitorSmsSendResultEventC *phoneEvent =(PhoneMonitorSmsSendResultEventC*)e;
			smsSendResult(phoneEvent->result);
			return TRUE;
		}
		case SMS_SEND_WITH_ID_RESULT_EVENT:
		{
			PhoneMonitorSmsSendWithIdResultEventC *phoneEvent =(PhoneMonitorSmsSendWithIdResultEventC*)e;
			smsSendWithIdResult(phoneEvent->id, phoneEvent->result);
			return TRUE;
		}
		case SIGNAL_NETWORK_TYPE_NOTIFY_EVENT:
		{
			PhoneMonitorSignalAndNetworkTypeNotifyEventC *phoneEvent =(PhoneMonitorSignalAndNetworkTypeNotifyEventC*)e;
			signalAndNetworkType(phoneEvent->signal,phoneEvent->networkType);
			return TRUE;
		}
		case SIGNAL_NETWORK_SERVICE_TYPE_NOTIFY_EVENT:
		{
			PhoneMonitorSignalAndNetworkTypeAndServiceTypeNotifyEventC *phoneEvent =(PhoneMonitorSignalAndNetworkTypeAndServiceTypeNotifyEventC*)e;

			IBD_PRINTF(DEBUG_INFOMATION, "update signal network type and service type (main thread)\n")

			signalAndNetworkTypeAndServiceType(phoneEvent->signal,phoneEvent->network,phoneEvent->service,phoneEvent->signal_strength);
			int signal;
			if( Strength_0 == phoneEvent->signal )
				signal = 0;
			else if( Strength_1 == phoneEvent->signal )
				signal = 1;
			else if( Strength_2 == phoneEvent->signal )
				signal = 2;
			else if( Strength_3 == phoneEvent->signal )
				signal = 3;
			else if( Strength_4 == phoneEvent->signal )
				signal = 4;
			else if( Strength_5 == phoneEvent->signal )
				signal = 5;
			else
				signal = -1;
			int networkType;
			if( Network_3G == phoneEvent->network )
				networkType = 3;
			else if( Network_2G == phoneEvent->network )
				networkType = 2;
			else
				networkType = 0;
			signalAndNetworkType(signal,networkType);
			return TRUE;
		}
		case RING_EVENT:
		{
			PhoneMonitorRingEventC *phoneEvent =(PhoneMonitorRingEventC*)e;
			ring(phoneEvent->cPhoneNumber);
			return TRUE;
		}
		case NEW_FACE_TIME_CALL_EVENT:
		{
			PhoneMonitorNewFaceTimeCallEventC *phoneEvent =(PhoneMonitorNewFaceTimeCallEventC*)e;
			newFaceTimeCall(phoneEvent->cPhoneNumber);
			return TRUE;
		}
		case END_FACE_TIME_CALL_EVENT:
		{
			PhoneMonitorEndFaceTimeCallEventC *phoneEvent =(PhoneMonitorEndFaceTimeCallEventC*)e;
			endFaceTimeCall(phoneEvent->reason);
			return TRUE;
		}
		case FACE_TIME_CALL_CONNECTED_EVENT:
		{
			PhoneMonitorFaceTimeCallConnectEventC *phoneEvent =(PhoneMonitorFaceTimeCallConnectEventC*)e;
			connectFaceTimeCall();
			return TRUE;
		}
		case MISS_FACE_TIME_CALL_EVENT:
		{
			PhoneMonitorMissFaceTimeCallEventC *phoneEvent =(PhoneMonitorMissFaceTimeCallEventC*)e;
			missFaceTimeCall(phoneEvent->cPhoneNumber);
			return TRUE;
		}
		default:
			return QObject::event(e);
	}
}
