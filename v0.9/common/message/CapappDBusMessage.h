#ifndef _CAPAPP_DBUS_MESSAGE_H_
#define _CAPAPP_DBUS_MESSAGE_H_

#include <stdio.h>

#include "dbusMessage.h"
#if 0 
#include <dbusServer.h>
#include <dbusApplication.h>
#endif 
#define CAPAPP_DBUS_SERVER_ID              (char*)"com.innov.capapp"
#define CAPAPP_DBUS_SERVER_OBJ             (char*)"/capapp"

#define CAPAPP_REQ_ID_DO_CAP				9600
#define CAPAPP_REQ_ID_REVERSE				9601
#define CAPAPP_REP_ID_REVERSE				9602
#define CAPAPP_NOTF_ID_REVERSE              9603  
#define CAPAPP_REQ_ID_REVERSEIN              9604
#define CAPAPP_NOTF_ID_REVERSEOUT               9605

class CapappDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(CapappDbusReqC)

public:
    CapappDbusReqC():DBusRequestC(CAPAPP_REQ_ID_REVERSE){}
    void print(FILE *fp) 
	{
		fprintf(fp,"\ttableIndex=[%d]\r\n", display_flag);
		fprintf(fp,"\ttableIndex=[%d]\r\n", switch_flag);
	}
    
	int display_flag;//显示
	int switch_flag;//按键切换
};

class CapappDbusReqC_Do_Capture : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(CapappDbusReqC_Do_Capture)

public:
    CapappDbusReqC_Do_Capture():DBusRequestC(CAPAPP_REQ_ID_DO_CAP){}
    void print(FILE *fp) 
	{
		fprintf(fp,"\ttableIndex=[%d]\r\n", capture_type);
	}
    
	int capture_type;//0正常拍照，1报警拍照
	void *pri;
};


class CapappDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(CapappDbusRepC)

public:
    CapappDbusRepC():DBusReplyC(CAPAPP_REP_ID_REVERSE){}
    
    void print(FILE *) {}
};

class CapappDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(CapappDbusNotifyC)

public:
    CapappDbusNotifyC():DBusNotifyC(CAPAPP_NOTF_ID_REVERSE)
    {
    }

    void print(FILE *fp)
    {
    }

	int capture_status;//1正在拍照，0拍照完成
	int capture_type;//0正常拍照，1报警拍照
	void *pri;
};

class CapappReverseDbusReqC: public DBusRequestC
{
    DBUS_REQUEST_OBJECT(CapappReverseDbusReqC)

public:
    CapappReverseDbusReqC():DBusRequestC(CAPAPP_REQ_ID_REVERSEIN)
    {
    }

    void print(FILE *fp)
    {
    }

	bool reverse;//true--进倒车  false--退出倒车
	int icount;
};

class CapappReverseDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(CapappReverseDbusNotifyC)

public:
    CapappReverseDbusNotifyC():DBusNotifyC(CAPAPP_NOTF_ID_REVERSEOUT)
    {
    }

    void print(FILE *fp)
    {
    }

	bool reverse;//true--进倒车  false--退出倒车
	int icount;
};

#endif
