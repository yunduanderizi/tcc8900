#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include "diskManagerMessage.h"
#include "diskManagerServer.h"

DiskManager *DiskManagerServer::m_diskManager;
map<string, DiskDevPartitionNode> DiskManagerServer::m_diskPartitionMap;
DBusServerC *DiskManagerServer::m_server;
int DiskManagerServer::m_sock;

int strCheck(char *s, char t)
{
    int count = 0;
    while(*s != '\0')
    {
        if(*s == t)
        count ++;
        s ++;
    }
    //printf("&&&count= %d\n", count);
    return count;
}


DiskManagerServer::DiskManagerServer(DBusServerC *pServer)
{
    m_sock = -1;
	m_server = pServer;
	m_monitorThreadFlag = 0;
//	m_diskPartitionNum = 0;

	m_diskManager = new DiskManager(m_diskPartitionNodes);
//	m_server->registerService(REQ_ID_DISKMANAGER, diskManagerServiceHandler, DiskManagerRequestC::factory);

	pthread_mutexattr_t     mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_lock,&mutexattr);
}

DiskManagerServer::~DiskManagerServer()
{
	//delete;
}

int DiskManagerServer::initDiskManagerServer()
{
BEGINFUNC_DISKMANAGER_SERVER
    memset(m_diskPartitionNodes, 0, sizeof(m_diskPartitionNodes));

    int diskPartitionNum = 0;
    if((diskPartitionNum = m_diskManager->initDiskManager(true)) < 0)
    {
        printf("initDiskManagerServer failed!\n");
        return -1;
    }
    if(diskPartitionNum == 0)
        printf("partitions file is empty!\n");

//    printf("***diskPartitionNum= %d\n", diskPartitionNum);        
    m_diskPartitionMap.clear();
    string tmp;

    bool emmcFlag = true;
    for (int i=0; i<diskPartitionNum; i++)
    {
        if(strstr(m_diskPartitionNodes[i].devPartitionName, "ndda") != NULL)
        {
            emmcFlag = false;
        }
    }
    
    for (int i=0; i<diskPartitionNum; i++)
    {
        tmp = m_diskPartitionNodes[i].devPartitionNode;
        m_diskPartitionMap.insert(pair<string, DiskDevPartitionNode>(tmp, m_diskPartitionNodes[i]));
        if((true == emmcFlag) && (strstr(m_diskPartitionNodes[i].devPartitionName, "mmcblk0") != NULL))
            continue;
        diskManagerNotifyHandler(&m_diskPartitionNodes[i], 0);
    }

    if(0 == m_monitorThreadFlag)
    {
//        printf("&initDevMonitorThread\n");
        if(initDevMonitorThread() < 0)
        {
            printf("initDevMonitorThread failed!\n");
            return -1;
        }
    }


#ifdef IBAC_DISKMANAGER_SERVER_LOG    
    map<string, DiskDevPartitionNode>::iterator  iter;
    printf("&--------------Map begin-------------\n");
    printf("&Map size = %d\n", m_diskPartitionMap.size());
    for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
    {
        printf("\n&%s.devPartitionType:              %d\n", iter->first.c_str(), iter->second.devPartitionType);
        printf("&%s.name:              %s\n", iter->first.c_str(), iter->second.devPartitionName);
        printf("&%s.dev_node:          %s\n", iter->first.c_str(), iter->second.devPartitionNode);
        printf("&%s.label:             %s\n", iter->first.c_str(), iter->second.devPartitionLabel);
        printf("&%s.mount_path:        %s\n", iter->first.c_str(), iter->second.devPartitionMountPath);
        printf("&%s.totalSize:         %lu\n", iter->first.c_str(), iter->second.devPartitionTotalSize);
        printf("&%s.freeSize:          %lu\n", iter->first.c_str(), iter->second.devPartitionFreeSize);        
        printf("&%s.partition_status:  %d\n", iter->first.c_str(), iter->second.devPartitionStatus);
        printf("&%s.partition_type_id: 0x%2x\n", iter->first.c_str(), iter->second.devPartitionFileSystemId);
        printf("&%s.partition_file:    %s\n", iter->first.c_str(), iter->second.devPartitionFileSystem);
    }
    printf("&--------------Map end---------------\n");
#endif    
ENDFUNC_DISKMANAGER_SERVER
	return 0;
}

int DiskManagerServer::initDevMonitorThread()
{
BEGINFUNC_DISKMANAGER_SERVER
	if(initMonitorSocket() < 0)
	{
	    printf("initMonitorSocket failed!\n");
            return -1;
	}
	m_monitorThreadFlag = 1;
	pthread_t Nett;
    pthread_create(&Nett, NULL, DiskManagerServer::monitorThread, NULL);
ENDFUNC_DISKMANAGER_SERVER    
    return 0;
}

int DiskManagerServer::initMonitorSocket()
{
BEGINFUNC_DISKMANAGER_SERVER
	int error = 0;
	struct sockaddr_nl snl;
	memset (&snl, 0x00, sizeof (struct sockaddr_nl));
	snl.nl_family = AF_NETLINK;
	snl.nl_pid = getpid ();
	snl.nl_groups = 1;
	if (m_sock < 0)
    	m_sock = socket (PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (m_sock == -1)
	{
		error = -1;
	}
	else
	{
		if ( bind (m_sock, (struct sockaddr *) & snl, sizeof (struct sockaddr_nl)) < 0)
		{
			error = -1;
		}
	}
	if( error == -1)
		close( m_sock );
ENDFUNC_DISKMANAGER_SERVER		
	return error;
}
/*
void *DiskManagerServer::monitorThread(void *)//???
{
BEGINFUNC_DISKMANAGER
    int ret;
    while(1)
    {
        ret = monitorPlugInAndPullOutDevice();
        if(ret < 0)
        {
            printf("monitorPlugInAndPullOutDevice failed!!!\n");
        }
    }
    m_diskManager->closeDiskManager();
    //free
ENDFUNC_DISKMANAGER    
}
*/
void *DiskManagerServer::monitorThread(void *)
{
BEGINFUNC_DISKMANAGER_SERVER
    char buffer[128];
    char *p, *mount_tmp, *umount_tmp;
    int ret;
//    int pulgInFlag = 0;
    if(m_sock < 0)
    {
        printf("the socket m_sock < 0\n");
//        return -1;
    }
    while(1)
    {
        //printf("=== monitorThread ===\n");
        if(recv (m_sock, &buffer, sizeof (buffer), 0) <= 0)
            continue;
//            return -1;
        printf("### begin recv_buffer ###\n");
        printf("%s\n", buffer);
        printf("### end recv_buffer ###\n");
        if(strstr(buffer, "add@")&&(mount_tmp = strstr(buffer, "/block/"))&&(strCheck(mount_tmp, '/') == 3))
        {
            p = strrchr(mount_tmp,'/');
            if(strstr(p, "sd") || strstr(p, "mmcblk") || strstr(p, "ndda"))
            {
                ret = plugInDiskDevice(p+1);
#ifdef IBAC_DISKMANAGER_SERVER_LOG            
                map<string, DiskDevPartitionNode>::iterator  iter;
                printf("&--------------Map begin-------------\n");
                printf("&Map size = %d\n", m_diskPartitionMap.size());
                for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
                {
                    printf("\n&%s.devPartitionType:              %d\n", iter->first.c_str(), iter->second.devPartitionType);
                    printf("&%s.name:              %s\n", iter->first.c_str(), iter->second.devPartitionName);
                    printf("&%s.dev_node:          %s\n", iter->first.c_str(), iter->second.devPartitionNode);
                    printf("&%s.label:             %s\n", iter->first.c_str(), iter->second.devPartitionLabel);
                    printf("&%s.mount_path:        %s\n", iter->first.c_str(), iter->second.devPartitionMountPath);
                    printf("&%s.totalSize:         %lu\n", iter->first.c_str(), iter->second.devPartitionTotalSize);
                    printf("&%s.freeSize:          %lu\n", iter->first.c_str(), iter->second.devPartitionFreeSize);
                    printf("&%s.partition_status:  %d\n", iter->first.c_str(), iter->second.devPartitionStatus);
                    printf("&%s.partition_type_id: 0x%2x\n", iter->first.c_str(), iter->second.devPartitionFileSystemId);
                    printf("&%s.partition_file:    %s\n", iter->first.c_str(), iter->second.devPartitionFileSystem);                
                }
                printf("&--------------Map end---------------\n");    
#endif
            }
//            return ret;
        }
        
        if(strstr(buffer, "remove@") && (umount_tmp = strstr(buffer, "/block/")))
        {
            p = strrchr(umount_tmp,'/');
            if(strstr(p, "sd") || strstr(p, "mmcblk") || strstr(p, "ndda"))
            {
                string devName(p+1);
                ret = pullOutDiskDevice(devName);
#ifdef IBAC_DISKMANAGER_SERVER_LOG                
                    map<string, DiskDevPartitionNode>::iterator  iter;
                    printf("&--------------Map begin-------------\n");
                    printf("&Map size = %d\n", m_diskPartitionMap.size());
                    for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
                    {
                        printf("\n&%s.devPartitionType:              %d\n", iter->first.c_str(), iter->second.devPartitionType);                
                        printf("&%s.name:              %s\n", iter->first.c_str(), iter->second.devPartitionName);
                        printf("&%s.dev_node:          %s\n", iter->first.c_str(), iter->second.devPartitionNode);
                        printf("&%s.label:             %s\n", iter->first.c_str(), iter->second.devPartitionLabel);
                        printf("&%s.mount_path:        %s\n", iter->first.c_str(), iter->second.devPartitionMountPath);
                        printf("&%s.totalSize:         %lu\n", iter->first.c_str(), iter->second.devPartitionTotalSize);
                        printf("&%s.freeSize:          %lu\n", iter->first.c_str(), iter->second.devPartitionFreeSize);
                        printf("&%s.partition_status:  %d\n", iter->first.c_str(), iter->second.devPartitionStatus);
                        printf("&%s.partition_type_id: 0x%2x\n", iter->first.c_str(), iter->second.devPartitionFileSystemId);
                        printf("&%s.partition_file:    %s\n", iter->first.c_str(), iter->second.devPartitionFileSystem);
                    }
                    printf("&--------------Map end---------------\n");    
#endif
            }
        }        
    }
ENDFUNC_DISKMANAGER_SERVER    
    return 0;
}

int DiskManagerServer::plugInDiskDevice(char *devName)
//check the mounts. Reference checkMountAndMount()???
{
    printf("=== plugInDiskDevice ===\n");
BEGINFUNC_DISKMANAGER_SERVER
    DiskDevPartitionNode plugInPartiton[10];
    int plugInPartitonNum = 0;
    int devPartitonsCount = 0;
    char typeId = 0x00;
    int mount_fail_num = 0;
    memset(plugInPartiton, 0, sizeof(plugInPartiton));
    if((devPartitonsCount = m_diskManager->parsePartitionsFile(devName, &plugInPartiton[0])) <= 0)
    {
        printf("parsePartitionsFile failed; devName is: %s\n", devName);
        return -1;
    }
    //printf("!!!plugInPartiton.devPartitionType: %d, devName = %s\n", plugInPartiton[0].devPartitionType, devName);
    if (devPartitonsCount > 1) //have more than two partitions in the partition file.
    {
        plugInPartitonNum = m_diskManager->parseFdiskFile(devName, plugInPartiton);
        if(plugInPartitonNum > 0)
        {
            string devNode;
            for(int i=0; i<plugInPartitonNum; i++)
            {   
                //printf("***plugInPartiton.devPartitionType: %d\n", plugInPartiton[0].devPartitionType);
                plugInPartiton[i].devPartitionType = plugInPartiton[0].devPartitionType;
                devNode = plugInPartiton[i].devPartitionNode;
                if(m_diskManager->plugInPartitionMount(&plugInPartiton[i]) < 0)
                {
                    printf("plugInPartitionMount failed! partitionNum: %d\n", i);
                    mount_fail_num++;
                }
                m_diskPartitionMap.insert(pair<string, DiskDevPartitionNode>(devNode, plugInPartiton[i]));
                //m_diskPartitionNum ++;//map.size.???
                diskManagerNotifyHandler(&plugInPartiton[i], 0);
            }
            if(mount_fail_num == plugInPartitonNum)
                return -1;
        }
        else 
        {
            printf("plugInPartitonNum = %d\n", plugInPartitonNum);
            return -1;
        }
    }
    else if (devPartitonsCount == 1)
    {
        
        strcpy(plugInPartiton[0].devPartitionName, devName);
        strcpy(plugInPartiton[0].devPartitionNode, devName);
        //plugInPartiton[0].devPartitionTotalSize = plugInPartiton[0].devPartitionTotalSize;
        if ((typeId = m_diskManager->getFilesystemTypeId(plugInPartiton[0].devPartitionName)) != 0x00)
        {
            plugInPartiton[0].devPartitionFileSystemId = typeId;
            strcpy(plugInPartiton[0].devPartitionFileSystem, "Unknown");
        }
        else 
        {
            plugInPartiton[0].devPartitionFileSystemId = 0x0C;
            strcpy(plugInPartiton[0].devPartitionFileSystem, "FAT_32");
        }
        string devNode;
        if(m_diskManager->plugInPartitionMount(&plugInPartiton[0]) < 0)
        {
            printf("plugInPartitionMount Err!\n");
            mount_fail_num = 1;
        }
        devNode = plugInPartiton[0].devPartitionNode;
        m_diskPartitionMap.insert(pair<string, DiskDevPartitionNode>(devNode, plugInPartiton[0]));
//            m_diskPartitionNum ++;  //map.size();???
        if(mount_fail_num == 1)
            return -1;
        diskManagerNotifyHandler(&plugInPartiton[0], 0);
    }
ENDFUNC_DISKMANAGER_SERVER    
    return 0;
}

int DiskManagerServer::pullOutDiskDevice(string devPartitionName )
{
BEGINFUNC_DISKMANAGER_SERVER
    //check the mounts && map???
    //printf("FFFFFFFFFFFFFF= %s\n", devPartitionName.c_str());
    printf("=== pullOutDiskDevice ===\n");

    map<string, DiskDevPartitionNode>::iterator  iter;
    iter = m_diskPartitionMap.find(devPartitionName);
    if( iter != m_diskPartitionMap.end())
    {
        diskManagerNotifyHandler(&iter->second, 1);
        //umount iter->second.mountPath
//        printf("iter->second.mount_path= %s\n", iter->second.devPartitionMountPath);
        if (m_diskManager->doUmount(iter->second.devPartitionMountPath) == 0)
        {
            //rm dir of mountPath.
            m_diskManager->removeMountDir(iter->second.devPartitionMountPath);
        }
        m_diskPartitionMap.erase(iter);
    }
    else 
        printf("Can not find the %s in the map!\n", devPartitionName.c_str());

ENDFUNC_DISKMANAGER_SERVER        
    return 0;
}
/*
void DiskManagerServer::diskManagerServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{

}
*/
void DiskManagerServer::diskManagerNotifyHandler(DiskDevPartitionNode *partName, int option)
{
BEGINFUNC_DISKMANAGER_SERVER
    DiskManagerNotifyC dmNotify;
    
    memset(&dmNotify.diskPartitionNotify, 0, sizeof(DiskDevPartitionNode));
    
    dmNotify.opCode = option;
    dmNotify.diskPartitionNotify.devPartitionType = partName->devPartitionType;
    strcpy(dmNotify.diskPartitionNotify.devPartitionName, partName->devPartitionName);
    strcpy(dmNotify.diskPartitionNotify.devPartitionNode, partName->devPartitionNode);
    strcpy(dmNotify.diskPartitionNotify.devPartitionLabel, partName->devPartitionLabel);    
    strcpy(dmNotify.diskPartitionNotify.devPartitionMountPath, partName->devPartitionMountPath);
    dmNotify.diskPartitionNotify.devPartitionTotalSize = partName->devPartitionTotalSize;
    dmNotify.diskPartitionNotify.devPartitionFreeSize = partName->devPartitionFreeSize;
    dmNotify.diskPartitionNotify.devPartitionStatus = partName->devPartitionStatus;
    dmNotify.diskPartitionNotify.devPartitionFileSystemId = partName->devPartitionFileSystemId;
    strcpy(dmNotify.diskPartitionNotify.devPartitionFileSystem, partName->devPartitionFileSystem);
#if 1    
    printf("&--------------Server Notify begin-------------\n");    
    printf("&~~~%d\n", dmNotify.opCode);
    printf("&%s.devPartitionType:              %d\n", dmNotify.diskPartitionNotify.devPartitionNode, dmNotify.diskPartitionNotify.devPartitionType);    
    printf("&%s.name:              %s\n", dmNotify.diskPartitionNotify.devPartitionNode, dmNotify.diskPartitionNotify.devPartitionName);
    printf("&%s.dev_node:          %s\n", dmNotify.diskPartitionNotify.devPartitionNode, dmNotify.diskPartitionNotify.devPartitionNode);
    printf("&%s.label:             %s\n", dmNotify.diskPartitionNotify.devPartitionNode, dmNotify.diskPartitionNotify.devPartitionLabel);
    printf("&%s.mount_path:        %s\n", dmNotify.diskPartitionNotify.devPartitionNode, dmNotify.diskPartitionNotify.devPartitionMountPath);
    printf("&%s.totalSize:         %d\n", dmNotify.diskPartitionNotify.devPartitionNode, dmNotify.diskPartitionNotify.devPartitionTotalSize);
    printf("&%s.partition_status:  %d\n", dmNotify.diskPartitionNotify.devPartitionNode, dmNotify.diskPartitionNotify.devPartitionStatus);
    printf("&%s.partition_type_id: 0x%2x\n", dmNotify.diskPartitionNotify.devPartitionNode, dmNotify.diskPartitionNotify.devPartitionFileSystemId);
    printf("&%s.partition_file:    %s\n", dmNotify.diskPartitionNotify.devPartitionNode, dmNotify.diskPartitionNotify.devPartitionFileSystem);
    printf("&--------------Server Notify end-------------\n");
#endif    
    m_server->sendNotify(&dmNotify);
ENDFUNC_DISKMANAGER_SERVER    
}

DiskManagerDevServerMap DiskManagerServer::getDiskPartitonsList()
{
BEGINFUNC_DISKMANAGER_SERVER
    DiskManagerDevServerMap dPartitionMap;
#if 0
    map<string, DiskDevPartitionNode>::iterator  iter;
    printf("&--------------Map begin-------------\n");
    printf("&Map size = %d\n", m_diskPartitionMap.size());
    for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
    {
        printf("\n&%s.devPartitionType:              %d\n", iter->first.c_str(), iter->second.devPartitionType);    
        printf("&%s.name:              %s\n", iter->first.c_str(), iter->second.name);
        printf("&%s.dev_node:          %s\n", iter->first.c_str(), iter->second.dev_node);
        printf("&%s.label:             %s\n", iter->first.c_str(), iter->second.label);
        printf("&%s.mount_path:        %s\n", iter->first.c_str(), iter->second.mount_path);
        printf("&%s.totalSize:         %d\n", iter->first.c_str(), iter->second.totalSize);
        printf("&%s.partition_status:  %d\n", iter->first.c_str(), iter->second.partition_status);
        printf("&%s.partition_type_id: 0x%2x\n", iter->first.c_str(), iter->second.partition_type_id);
        printf("&%s.partition_file:    %s\n", iter->first.c_str(), iter->second.partition_file);                
    }
    printf("&--------------Map end---------------\n");    
#endif
ENDFUNC_DISKMANAGER_SERVER    
    return dPartitionMap;
}

DiskManagerDevServerMap DiskManagerServer::getDiskDevNodeInfo(DiskDevType diskManagerDevType)
{
BEGINFUNC_DISKMANAGER_SERVER
    DiskManagerDevServerMap dPartitionMap;
ENDFUNC_DISKMANAGER_SERVER
    return dPartitionMap;
}

int DiskManagerServer::refreshDiskPartitonsList()
{
BEGINFUNC_DISKMANAGER_SERVER

    initDiskManagerServer();
    
ENDFUNC_DISKMANAGER_SERVER    
    return 0;
}

// internal lock
DiskManagerServer::InternalLock::InternalLock(pthread_mutex_t* lock)
{
	m_lock=lock;
	pthread_mutex_lock(m_lock);
}
DiskManagerServer::InternalLock::~InternalLock()
{
	pthread_mutex_unlock(m_lock);
}

