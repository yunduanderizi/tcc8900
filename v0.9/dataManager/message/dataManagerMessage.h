#ifndef DATAMANAGER_MESSAGE_H_20111020
#define DATAMANAGER_MESSAGE_H_20111020 

#include "dbusClientProxy.h"
#include "dbusMessage.h"


#define	REQ_ID_UPDATE_READWRITE_USERDATA_FLAG		0x10000000
#define	NOTF_ID_UPDATE_READWRITE_USERDATA_FLAG		0x10000000


////////////////////////////////////////////////////////////////////
class UpdateReadWriteUserDataFlagReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(UpdateReadWriteUserDataFlagReqC)

public:
	UpdateReadWriteUserDataFlagReqC():DBusRequestC(REQ_ID_UPDATE_READWRITE_USERDATA_FLAG){}
	void print(FILE *fp){fprintf(fp, "[UpdateReadWriteUserDataFlagReqC] m_bReadWriteUserData[%d]\n", m_bReadWriteUserData);}

	bool m_bReadWriteUserData;
};

class UpdateReadWriteUserDataFlagNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(UpdateReadWriteUserDataFlagNotifyC)

public:
	UpdateReadWriteUserDataFlagNotifyC():DBusNotifyC(NOTF_ID_UPDATE_READWRITE_USERDATA_FLAG){}
	void print(FILE *fp){fprintf(fp, "[UpdateReadWriteUserDataFlagNotifyC] m_bReadWriteUserData[%d]\n", m_bReadWriteUserData);}

	bool m_bReadWriteUserData;
}; 



#endif
 
