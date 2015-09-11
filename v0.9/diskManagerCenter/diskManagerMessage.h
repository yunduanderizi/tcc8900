#ifndef DISKMANAGERMESSAGE_H
#define DISKMANAGERMESSAGE_H

#define REQ_ID_DISKMANAGER 152
#define REP_ID_DISKMANAGER 151
#define NOTF_ID_DISKMANAGER 150

#define ServerDiskManager		(char*)"com.innov.diskmanage"		//格式com.innov.<进程名>
#define ObjectDiskManager		(char*)"/diskmanageDBus"			//格式 /<模块名>

#include <dbusMessage.h>
#include "diskManager.h"

class DiskManagerRequestC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(DiskManagerRequestC)
public:
	DiskManagerRequestC():DBusRequestC(REQ_ID_DISKMANAGER){}

	void print(FILE *fp);
};
class DiskManagerReplyC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(DiskManagerReplyC)

public:
	DiskManagerReplyC():DBusReplyC(REP_ID_DISKMANAGER){}

        void print(FILE *fp);	
};
class DiskManagerNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(DiskManagerNotifyC)

public:
	DiskManagerNotifyC();//:DBusNotifyC(NOTF_ID_DISKMANAGER){}
	~DiskManagerNotifyC();//{}
	
	int opCode; // 0-add; 1-remove.
	DiskDevPartitionNode diskPartitionNotify;
        void print(FILE *fp);
}; 

#endif

