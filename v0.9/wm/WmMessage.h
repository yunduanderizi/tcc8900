#ifndef _WMMESSAGE_WM_H
#define _WMMESSAGE_WM_H

//消息定义
#include "dbusMessage.h"

#define REQ_ID_WMC_WINDOW_MANAGEMNT 51
#define REQ_ID_WMC_SEND_KEY_TO_TOP  52
#define REP_ID_WMC_WINDOW_MANAGEMNT 51

#define REQ_ID_WMS_WINDOW_MANAGEMNT 51
#define REP_ID_WMS_WINDOW_MANAGEMNT 51
#define REP_ID_WMS_GET_TOP_WINDOW   52

#define	WMC_OP_SHOW	1
#define	WMC_OP_HIDE	2
#define	WMC_OP_BACK	3
#define	WMC_OP_COVERED	4
#define	WMC_OP_ON_TOP	5

#define	WMS_OP_SHOW_MIDDLE	0
#define	WMS_OP_SHOW	1
#define	WMS_OP_HIDE	2
#define	WMS_OP_BACK	3
#define	WMS_OP_MAIN	4
#define WMS_OP_BACK_SUBWINDOW	5
#define	WMS_OP_GET_TOP_WINDOW	6
#define	WMS_OP_SHOW_HALF	7

////////////////////////////////////////////////////////////////////
class WMCWindowManagementReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(WMCWindowManagementReqC)

public:
	WMCWindowManagementReqC():DBusRequestC(REQ_ID_WMC_WINDOW_MANAGEMNT){
	processName[0]='\0';
	windowName[0]='\0';
	op=0;
	}
	void print(FILE *fp);

	char processName[30];
	char windowName[30];

	char op;
};

class WMCSendKeyToTopReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(WMCSendKeyToTopReqC)

public:
	WMCSendKeyToTopReqC():DBusRequestC(REQ_ID_WMC_SEND_KEY_TO_TOP){}

	void print(FILE *fp);
	
	char processName[30];
	char windowName[30];

	int	keyCode;
	int	modifier;
};

class WMCWindowManagementRepC:public DBusReplyC
{
	DBUS_REPLY_OBJECT(WMCWindowManagementRepC)

public:
	WMCWindowManagementRepC():DBusReplyC(REP_ID_WMC_WINDOW_MANAGEMNT){}

};


class WMSWindowManagementReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(WMSWindowManagementReqC)

public:
	WMSWindowManagementReqC():DBusRequestC(REQ_ID_WMS_WINDOW_MANAGEMNT){
	processName[0]='\0';
	windowName[0]='\0';
	op=0;
	}
	void print(FILE *fp);

	char processName[30];
	char windowName[30];
	char op;
};

class WMSWindowManagementRepC:public DBusReplyC
{
	DBUS_REPLY_OBJECT(WMSWindowManagementRepC)

public:
	WMSWindowManagementRepC():DBusReplyC(REP_ID_WMS_WINDOW_MANAGEMNT){}

};

class WMSGetTopWindowRepC:public DBusReplyC
{
	DBUS_REPLY_OBJECT(WMSGetTopWindowRepC)

public:
	WMSGetTopWindowRepC():DBusReplyC(REP_ID_WMS_GET_TOP_WINDOW)
	{
		processName[0]='\0';
		windowName[0]='\0';
	}
	void print(FILE *fp);

	char processName[30];
	char windowName[30];
};

#endif //_NAVIMESSAGE_H
