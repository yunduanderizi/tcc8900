#ifndef _SCHOOL_ID_DBUS_MESSAGE_H_
#define _SCHOOL_ID_DBUS_MESSAGE_H_

#include <stdio.h>

#include "dbusMessage.h"
#if 0 
#include <dbusServer.h>
#include <dbusApplication.h>
#endif 
#define SCHOOL_ID_DBUS_SERVER_ID              (char*)"com.innov.schoolid"
#define SCHOOL_ID_DBUS_SERVER_OBJ             (char*)"/schoolid"

#define SCHOOL_ID_REQ_ID_REVERSE			  18887
#define SCHOOL_ID_REP_ID_REVERSE			  18888
#define SCHOOL_ID_NOTF_ID_REVERSE             18889    

#define SCHOOL_ID_MAX_LEN					  128

class SchoolIDDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(SchoolIDDbusReqC)

public:
    SchoolIDDbusReqC():DBusRequestC(SCHOOL_ID_REQ_ID_REVERSE){}
    void print(FILE *fp) 
	{
		fprintf(fp,"\ttableIndex=[%d]\r\n", time);
		fprintf(fp,"\ttableIndex=[%d]\r\n", value);
	}
    
	unsigned long time;//时间
	int value;//上午或者下午
};

class SchoolIDDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(SchoolIDDbusRepC)

public:
    SchoolIDDbusRepC():DBusReplyC(SCHOOL_ID_REP_ID_REVERSE){}
    
    void print(FILE *) {}
};

class SchoolIDDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(SchoolIDDbusNotifyC)

public:
    SchoolIDDbusNotifyC():DBusNotifyC(SCHOOL_ID_NOTF_ID_REVERSE){}

    void print(FILE *fp)
	{
		fprintf(fp,"\tid_num=[%d]\r\n", id_num);
		fprintf(fp,"\tcount_all=[%d]\r\n", count_all);
	}
	unsigned char id_num[SCHOOL_ID_MAX_LEN];//id号
	int count_all;//人数统计
	int flag_done;//应刷卡
	int flag_ampm;//上午下午
};



#endif
