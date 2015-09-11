#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "dbusServer.h"
#include "dbusApplication.h"
#include "DiskManageMessage.h"
#include "innovbox_diskmanage.h"
#include "innov_box_api.h"

DBusServerC	*pServer;

void DiskManageServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{

	if(request->getIdentifier()!=REQ_ID_DISK)
		return;

	int index = 0, i=0;
	struct InnovBox_Disk_Device *pdisk=NULL;

	DiskManageReqC *pdiskRequest=(DiskManageReqC*)request;
	DiskManageRepC  *pdiskReply =new DiskManageRepC();

	pdisk = IBGetDiskDeviceList();      	

	if(pdisk == NULL)
	{
		printf(" no disk device \n");
		pdiskReply->diskcount = 0;
		reply = pdiskReply;
		return;
	}

	while(pdisk!=NULL)
	{
		memset(&pdiskReply->diskdevices[index], 0, sizeof(struct InnovBox_Disk_Device));

		strcpy(pdiskReply->diskdevices[index].disk_name, pdisk->disk_name);
		strcpy(pdiskReply->diskdevices[index].dev_node, pdisk->dev_node);
		pdiskReply->diskdevices[index].number = pdisk->number;
		pdiskReply->diskdevices[index].nr_partition = pdisk->nr_partition;		
		pdiskReply->diskdevices[index].disk_status = pdisk->disk_status;
		pdiskReply->diskdevices[index].disk_type = pdisk->disk_type;

		for(i=0; i < pdisk->nr_partition; i++)
		{					
			strcpy(pdiskReply->diskdevices[index].partition[i].mount_path, pdisk->partition[i].mount_path);
			strcpy(pdiskReply->diskdevices[index].partition[i].name, pdisk->partition[i].name);
			strcpy(pdiskReply->diskdevices[index].partition[i].dev_node, pdisk->partition[i].dev_node);
			pdiskReply->diskdevices[index].partition[i].number = pdisk->partition[i].number;
			pdiskReply->diskdevices[index].partition[i].partition_type = pdisk->partition[i].partition_type;				
			pdiskReply->diskdevices[index].partition[i].partition_status = pdisk->partition[i].partition_status;
		}	

		pdiskReply->diskcount = ++index;

		pdisk = pdisk->next;
	}

	reply = pdiskReply;

}

void *DiskManage_Thread(void *)
{
	int ret = 0, i=0;
	char buf[128];
	struct InnovBox_Disk_Device *pdisk= NULL;
	DiskManagerNotifyC notify;

	IBDebugInit("innovbox.ini");

	ret = IBDiskManageInit("innovbox.ini");
	if(ret != R_INNOV_BOX_OK)
		return NULL;

	while( 1 )
	{
		memset(buf, 0, sizeof(buf));
		ret =  IBDiskAutoMount(buf);
		printf("buf =%s =========ret=%d========\n",buf, ret);
		if(ret == R_INNOV_BOX_OK)
		{
			pdisk = IBGetDiskDevice(buf);
			if(pdisk == NULL)
				continue;
			
			memset(&notify.diskdevice,0,sizeof(struct InnovBox_Disk_Device));

			strcpy(notify.diskdevice.disk_name, pdisk->disk_name);
			strcpy(notify.diskdevice.dev_node, pdisk->dev_node);
			notify.diskdevice.number = pdisk->number;
			notify.diskdevice.nr_partition = pdisk->nr_partition;
			notify.diskdevice.disk_status = pdisk->disk_status;
			notify.diskdevice.disk_type = pdisk->disk_type;
			
			for( i=0; i <  pdisk->nr_partition; i++)
			{
				strcpy(notify.diskdevice.partition[i].name, pdisk->partition[i].name);
				strcpy(notify.diskdevice.partition[i].dev_node, pdisk->partition[i].dev_node);
				strcpy(notify.diskdevice.partition[i].mount_path, pdisk->partition[i].mount_path);
				notify.diskdevice.partition[i].number = pdisk->partition[i].number;
				notify.diskdevice.partition[i].partition_type = pdisk->partition[i].partition_type;				
				notify.diskdevice.partition[i].partition_status = pdisk->partition[i].partition_status;
			}
			
			pServer->sendNotify(&notify);
		}      
	}

	IBDiskManageClose();    
	return 0;

}


int main(int argc,char *argv[])
{
	DBusApplicationC app(argc, argv);

	DBusServerC	server(ServerDiskManager,ObjectDiskManager);	

	pServer	=&server;

	server.registerService(REQ_ID_DISK,DiskManageServiceHandler,DiskManageReqC::factory);

	if(server.connect()<0)
	{
		printf("Cann't connect to DBUS server.\n");
		return -1;
	}

	pthread_t ptd;
	pthread_create(&ptd, NULL, DiskManage_Thread, NULL);

	return app.loop();
}

