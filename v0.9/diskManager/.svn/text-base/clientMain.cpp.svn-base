
#include <stdio.h>

#include "dbusClient.h"
#include "dbusApplication.h"
#include "DiskManageMessage.h"

DBusClientC	*client;

void TestNotifyHandler(DBusNotifyC *notify)
{
	if(notify->getIdentifier()!=NOTF_ID_DISK)
	return;

	DiskManagerNotifyC  *testNotify=(DiskManagerNotifyC*)notify;

	printf("Receive InnovBox_Disk_Device: %s\n\n",testNotify->diskdevice.disk_name);	 
	printf("pdisk ->dev_node = %s\n", testNotify->diskdevice.dev_node);
	printf("pdisk ->number = %d\n", testNotify->diskdevice.number);
	printf( "pdisk ->nr_partition = %d\n", testNotify->diskdevice.nr_partition);
	printf( "pdisk ->disk_type = %d\n",  testNotify->diskdevice.disk_type);
	printf("pdisk ->disk_status = %d\n", testNotify->diskdevice.disk_status);
	for(int i=0;i < testNotify->diskdevice.nr_partition;i++)
	{		
		printf( "pdisk ->partition[0].dev_node = %s\n", testNotify->diskdevice.partition[i].dev_node);
		printf( "pdisk ->partition.number = %d\n", testNotify->diskdevice.partition[i].number);
		printf( "pdisk ->partition.mount_path = %s\n",  testNotify->diskdevice.partition[i].mount_path);
		printf("pdisk ->partition.partition_status = %d\n", testNotify->diskdevice.partition[i].partition_status);
	}
}


void* intractThread(void*)
{
	int i=0, j=0;
	char buffer[350];
 	DiskManageReqC	request;
	DBusReplyC	*reply=NULL;
	int result = 0;
	
       while(1)
       {
             printf("Command(r,n,l):----->");
             //char ch=get(stdin);
		fgets(buffer,250,stdin);

             if(buffer[0] == 'r')
             {
			result=client->sendService(&request,reply,5*1000);

			if(result<0 ||reply->getIdentifier()!=REP_ID_DISK)
			{
				printf("Send Service Error. Error Code: %d\n",result);
			}
			else
			{
				DiskManageRepC *pdiskReply  =(DiskManageRepC*)reply;
				printf("receive disk count = %d \n",pdiskReply->diskcount );
				for(i = 0; i < pdiskReply->diskcount; i++)
				{
					printf("--------------------------------------------------------------------\n");
				       printf("Receive InnovBox_Disk_Device: %s\n\n",pdiskReply->diskdevices[i].disk_name);
				       printf("pdisk number: %d\n\n",pdiskReply->diskdevices[i].number);
					printf( "pdisk ->nr_partition = %d\n", pdiskReply->diskdevices[i].nr_partition);
					printf( "pdisk ->disk_type = %d\n",  pdiskReply->diskdevices[i].disk_type);
					printf("pdisk ->disk_status = %d\n", pdiskReply->diskdevices[i].disk_status);
				 	for( j=0;j < pdiskReply->diskdevices[i].nr_partition;j++)
					{		
						printf(" partion===========================%d\n", j);
						printf( "pdisk ->partition[].name = %s\n", pdiskReply->diskdevices[i].partition[j].name);
						printf( "pdisk ->partition.mount_path = %s\n",  pdiskReply->diskdevices[i].partition[j].mount_path);
						printf( "pdisk ->partition.number = %d\n", pdiskReply->diskdevices[i].partition[j].number);
						printf("pdisk ->partition.partition_status = %d\n", pdiskReply->diskdevices[i].partition[j].partition_status);
	 				}				
				}
				
			}
			delete reply;
             }
        }

}

int main(int argc,char *argv[])
{
	DBusApplicationC app(argc, argv);

    	client	=new DBusClientC((char*)"com.innov.diskmanage", (char*)"/diskmanageDBus");


	client->registerReply(REP_ID_DISK,DiskManageRepC::factory);

	client->registerNotify(NOTF_ID_DISK,TestNotifyHandler,DiskManagerNotifyC::factory);

	if(client->connect()<0)
        {
                printf("Cann't connect to DBUS server.\n");
                return -1;
        }

	pthread_t	threadId;
	pthread_create(&threadId,NULL,intractThread,NULL);

	return app.loop();
}

