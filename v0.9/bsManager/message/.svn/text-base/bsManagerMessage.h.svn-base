#ifndef BSMANAGER_MESSAGE_H_20111111
#define BSMANAGER_MESSAGE_H_20111111 

#include "dbusClientProxy.h"
#include "dbusMessage.h"


#define	ServerBsManagerProcess 		(char*)"com.innov.bsManagerProcess"
#define	ObjectBsManagerProcess		(char*)"/bsManagerProcess"

#define	NAME_LENGTH_MAX							256

#define	REQ_ID_CLIENT_TO_BSMANAGERPROCESS_COMMON				76
#define	NOTF_ID_CLIENT_TO_BSMANAGERPROCESS_COMMON			76

////////////////////////////////////////////////////////////////////

typedef enum _BsManager_Command_Id
{
	BM_COMMAND_TELEPHONE = 0,
	BM_COMMAND_UCSINFO,
}BsManager_Command_Id;

class ClientToBsManagerProcessCommonReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(ClientToBsManagerProcessCommonReqC)

public:
	ClientToBsManagerProcessCommonReqC():DBusRequestC(REQ_ID_CLIENT_TO_BSMANAGERPROCESS_COMMON){}
	void print(FILE *fp){fprintf(fp, "m_iCommandId[%d]\n", m_iCommandId);}

	int m_iCommandId;//BsManager_Command_Id
};

class BsManagerProcessToClientCommonNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(BsManagerProcessToClientCommonNotifyC)

public:
	BsManagerProcessToClientCommonNotifyC():DBusNotifyC(NOTF_ID_CLIENT_TO_BSMANAGERPROCESS_COMMON){}
	void print(FILE *fp){fprintf(fp, "m_iCommandId[%d]\n", m_iCommandId);}

	int m_iCommandId;//BsManager_Command_Id
}; 

#endif
 
