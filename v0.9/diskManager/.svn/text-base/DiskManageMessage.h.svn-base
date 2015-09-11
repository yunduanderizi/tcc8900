#ifndef __DISK__MANAGE_MESSAGE__
#define __DISK__MANAGE_MESSAGE__
#include "dbusMessage.h"
#include "innovbox_diskmanage.h"

#define ServerDiskManager 		(char*)"com.innov.diskmanage"		//格式com.innov.<进程名>
#define ObjectDiskManager 		(char*)"/diskmanageDBus"			//格式 /<模块名>

#define	REQ_ID_DISK	102
#define	REP_ID_DISK	103
#define	NOTF_ID_DISK	104

#define MAX_DISK_COUNT  10

class DiskManageReqC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(DiskManageReqC)

public:
	DiskManageReqC():DBusRequestC(REQ_ID_DISK){}
	void print(FILE *fp);
};

class DiskManageRepC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(DiskManageRepC)

public:
	DiskManageRepC():DBusReplyC(REP_ID_DISK){}
	
	void print(FILE *fp);

	int diskcount;

       struct InnovBox_Disk_Device diskdevices[MAX_DISK_COUNT];
};

class DiskManagerNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(DiskManagerNotifyC)

public:
	DiskManagerNotifyC();
	
	~DiskManagerNotifyC();
    
	void print(FILE *fp);   

        struct InnovBox_Disk_Device diskdevice;
    
};

#endif
