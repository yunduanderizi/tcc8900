#ifndef	_DEBUG_MONITOR_MESSAGE_H_
#define	_DEBUG_MONITOR_MESSAGE_H_

#include "dbusMessage.h"		//dbusœ˚œ¢


#define	REQ_ID_DEBUG_MONITOR	70
#define	REP_ID_DEBUG_MONITOR	70

#define	DEBUG_OPER_CHANGE_FILENAME	1
#define	DEBUG_OPER_CHANGE_LEVEL		2
#define	DEBUG_OPER_CHANGE_ALL			3

class DebugMonitorReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(DebugMonitorReqC)

public:
	DebugMonitorReqC():DBusRequestC(REQ_ID_DEBUG_MONITOR){
		fileName[0]='\0';
		debugLevel = 0;
		oper = 0;
	}
	
	void print(FILE *fp);

	char fileName[256];
	int debugLevel;
	int oper;
};


class DebugMonitorRepC:public DBusReplyC
{
	DBUS_REPLY_OBJECT(DebugMonitorRepC)

public:
	DebugMonitorRepC():DBusReplyC(REP_ID_DEBUG_MONITOR){}
};

#endif
