#include <unistd.h>
#include "dbusClientProxy.h"


#define REQ_ID_PROCESS_MONITOR 1000
#define REP_ID_PROCESS_MONITOR 1000

class ProcessMonitorReqC:public DBusRequestC
{
        DBUS_REQUEST_OBJECT(ProcessMonitorReqC)

public:
        ProcessMonitorReqC():DBusRequestC(REQ_ID_PROCESS_MONITOR){}
        void print(FILE *fp)
        {
                fprintf(fp,"\tprocessName: %s\n",processName);
                fprintf(fp,"\top: %d\n\n",op);
        }

        char    processName[50];
	int  	op;
};

class ProcessMonitorRepC:public DBusReplyC
{
        DBUS_REPLY_OBJECT(ProcessMonitorRepC)

public:
        ProcessMonitorRepC():DBusReplyC(REP_ID_PROCESS_MONITOR){}
};


DBusClientProxyC::DBusClientProxyC(char *serverName,char *objectName)
:DBusClientC(serverName,objectName)
{
	char *p=strrchr(serverName,'.');

	//printf("processName=%s\n",p+1);

	strcpy(processName,p+1);

}
DBusClientProxyC::~DBusClientProxyC()
{
}

/*
int DBusClientProxyC::connect()
{
	int result;

	result=DBusClientC::connect();

	return result;
}
*/

int DBusClientProxyC::sendService(DBusRequestC  *request) //timeout unit is millisecond
{
	int result =DBUS_RESULT_ERROR_UNKNOWN;
	int count  =0;

	while(count < 10)
	{
		count++;

		result=DBusClientC::sendService(request);

		if(result==DBUS_RESULT_SUCCESS)
		{
			return result;
		}
		else if(result==DBUS_RESULT_ERROR_DISCONNECT)
		{
			result=DBusClientC::connect();

			if(result!=DBUS_RESULT_SUCCESS)
				return DBUS_RESULT_ERROR_DISCONNECT;

			continue;
		}
		else if(result==DBUS_RESULT_ERROR_NO_SERVER)
		{
			if(startServer(result)==DBUS_RESULT_SUCCESS)
			{	
				usleep(300*1000);
				continue;
			}
			else
			{
				return result;
			}
		}
		else
		{
			return result;
		}
	}

	return result;
}
int DBusClientProxyC::sendService(DBusRequestC  *request,DBusReplyC *&reply,int timeout) //timeout unit is millisecond
{
	int result =DBUS_RESULT_ERROR_UNKNOWN;
	int count  =0;

	while(count < 10)
	{
		count++;

		result=DBusClientC::sendService(request,reply,timeout);

		if(result==DBUS_RESULT_SUCCESS)
		{
			return result;
		}
		else if(result==DBUS_RESULT_ERROR_DISCONNECT)
		{
			result=DBusClientC::connect();

			if(result!=DBUS_RESULT_SUCCESS)
				return DBUS_RESULT_ERROR_DISCONNECT;

			continue;
		}
		else if(result==DBUS_RESULT_ERROR_NO_SERVER)
		{
			if(pmClient==NULL)
			{
				pmClient=new DBusClientC((char*)"com.innov.processMonitor", (char*)"/processMonitor");
				pmClient->registerReply(REP_ID_PROCESS_MONITOR,ProcessMonitorRepC::factory);

				int pmResult=pmClient->DBusClientC::connect();
				
				if(pmResult!=DBUS_RESULT_SUCCESS)
				{
					delete pmClient;
					pmClient=NULL;		

					return result;
				}
			}

			ProcessMonitorReqC	request;
        		strcpy(request.processName,processName);
			request.op =result;

        		DBusReplyC      *reply=NULL;
        		int pmResult=pmClient->sendService(&request,reply,3*1000);
			
			if(pmResult<0 || reply->result<0)
			{
				delete reply;
				return result;
			}
			else
			{
        			delete reply;
				usleep(300*1000);
				continue;
			}
		}
		else
		{
			return result;
		}
	}

	return result;
}

int DBusClientProxyC::sendServiceBlockMode(DBusRequestC  *request,DBusReplyC *&reply,int timeout) //timeout unit is millisecond
{
	int result =DBUS_RESULT_ERROR_UNKNOWN;
	int count  =0;

	while(count < 10)
	{
		count++;

		result=DBusClientC::sendServiceSubThread(request,reply,timeout);

		if(result==DBUS_RESULT_SUCCESS)
		{
			return result;
		}
		else if(result==DBUS_RESULT_ERROR_DISCONNECT)
		{
			result=DBusClientC::connect();

			if(result!=DBUS_RESULT_SUCCESS)
				return DBUS_RESULT_ERROR_DISCONNECT;

			continue;
		}
		else if(result==DBUS_RESULT_ERROR_NO_SERVER)
		{
			if(startServer(result)==DBUS_RESULT_SUCCESS)
			{	
				usleep(300*1000);
				continue;
			}
			else
			{
				return result;
			}
		}
		else
		{
			return result;
		}
	}

	return result;
}

int DBusClientProxyC::startServer(int result)
{
	if(pmClient==NULL)
	{
		pmClient=new DBusClientC((char*)"com.innov.processMonitor", (char*)"/processMonitor");
		pmClient->registerReply(REP_ID_PROCESS_MONITOR,ProcessMonitorRepC::factory);

		int pmResult=pmClient->DBusClientC::connect();
		
		if(pmResult!=DBUS_RESULT_SUCCESS)
		{
				delete pmClient;
				pmClient=NULL;		

				return result;
		}
	}

	ProcessMonitorReqC	request;
        strcpy(request.processName,processName);
	request.op =result;

        DBusReplyC      *reply=NULL;
        int pmResult=pmClient->sendServiceSubThread(&request,reply,3*1000);
			
	if(pmResult==0 && reply->result==0)
	{
		result =DBUS_RESULT_SUCCESS;
	}

	delete reply;

	return result;
}

DBusClientC	*DBusClientProxyC::pmClient       =NULL;
char  		 DBusClientProxyC::pmConnectStatus=0;



