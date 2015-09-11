#include "diskManagerMessage.h"
#include "diskManagerClient.h"

map<string, DiskDevPartitionNode> DiskManagerClient::m_diskPartitionMap;
DiskManagerPlugInAndPullOutNotify *DiskManagerClient::m_pPlugInAndPullOutNoti;

DiskManagerClient::DiskManagerClient(DBusClientProxyC *pClient, DiskManagerPlugInAndPullOutNotify *pPlugInAndPullOutNoti)
{
	m_client = pClient;
    m_pPlugInAndPullOutNoti = pPlugInAndPullOutNoti;

	m_diskManager = new DiskManager(m_diskPartitionNodes);
	
//        m_client->registerReply(REP_ID_DISKMANAGER, DiskManagerReplyC::factory);
	m_client->registerNotify(NOTF_ID_DISKMANAGER, diskManagerNotifyHandler, DiskManagerNotifyC::factory);
	
	pthread_mutexattr_t     mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_lock,&mutexattr);	
}

DiskManagerClient::~DiskManagerClient()
{

}

int DiskManagerClient::initDiskManagerClient()
{
BEGINFUNC_DISKMANAGER_CLIENT
    memset(m_diskPartitionNodes, 0, sizeof(m_diskPartitionNodes));
    int diskPartitionNum = 0;
    if((diskPartitionNum = m_diskManager->initDiskManager(false)) < 0)
    {
        printf("initDiskManagerServer failed!\n");
        return -1;
    }
    if(diskPartitionNum == 0)
        printf("partitions file is empty!\n");
        
    m_diskPartitionMap.clear();
    string devNode;
    for (int i=0; i<diskPartitionNum; i++)
    {
        devNode = m_diskPartitionNodes[i].devPartitionNode;
        m_diskPartitionMap.insert(pair<string, DiskDevPartitionNode>(devNode, m_diskPartitionNodes[i]));
    }
#ifdef IBAC_DISKMANAGER_CLIENT_LOG    
    map<string, DiskDevPartitionNode>::iterator  iter;
    printf("*--------------Client Map begin-------------\n");
    printf("*Map size = %d\n", m_diskPartitionMap.size());
    for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
    {
        printf("*%s.devPartitionType:              %d\n", iter->first.c_str(), iter->second.devPartitionType);
        printf("*%s.name:              %s\n", iter->first.c_str(), iter->second.devPartitionName);
        printf("*%s.dev_node:          %s\n", iter->first.c_str(), iter->second.devPartitionNode);
        printf("*%s.label:             %s\n", iter->first.c_str(), iter->second.devPartitionLabel);
        printf("*%s.mount_path:        %s\n", iter->first.c_str(), iter->second.devPartitionMountPath);
        printf("*%s.totalSize:         %lu\n", iter->first.c_str(), iter->second.devPartitionTotalSize);
        printf("*%s.freeSize:          %lu\n", iter->first.c_str(), iter->second.devPartitionFreeSize);
        printf("*%s.partition_status:  %d\n", iter->first.c_str(), iter->second.devPartitionStatus);
        printf("*%s.partition_type_id: 0x%2x\n", iter->first.c_str(), iter->second.devPartitionFileSystemId);
        printf("*%s.partition_file:    %s\n", iter->first.c_str(), iter->second.devPartitionFileSystem);                
    }
    printf("*--------------Client Map end---------------\n");
#endif
    
ENDFUNC_DISKMANAGER_CLIENT        
	return 0;
}

void DiskManagerClient::diskManagerNotifyHandler(DBusNotifyC *notify)
{
BEGINFUNC_DISKMANAGER_CLIENT
    DiskManagerNotifyC *diskManangerNotify = (DiskManagerNotifyC*)notify;
    if (NOTF_ID_DISKMANAGER == diskManangerNotify->getIdentifier())
    {    
#ifdef IBAC_DISKMANAGER_CLIENT_LOG

        printf("*--------------Client Notify begin-------------\n");    
        printf("*~~~%d\n", diskManangerNotify->opCode);
        printf("*%s.devPartitionType:              %d\n", diskManangerNotify->diskPartitionNotify.devPartitionNode, diskManangerNotify->diskPartitionNotify.devPartitionType);
        printf("*%s.name:              %s\n", diskManangerNotify->diskPartitionNotify.devPartitionNode, diskManangerNotify->diskPartitionNotify.devPartitionName);
        printf("*%s.dev_node:          %s\n", diskManangerNotify->diskPartitionNotify.devPartitionNode, diskManangerNotify->diskPartitionNotify.devPartitionNode);
        printf("*%s.label:             %s\n", diskManangerNotify->diskPartitionNotify.devPartitionNode, diskManangerNotify->diskPartitionNotify.devPartitionLabel);
        printf("*%s.mount_path:        %s\n", diskManangerNotify->diskPartitionNotify.devPartitionNode, diskManangerNotify->diskPartitionNotify.devPartitionMountPath);
        printf("*%s.totalSize:         %d\n", diskManangerNotify->diskPartitionNotify.devPartitionNode, diskManangerNotify->diskPartitionNotify.devPartitionTotalSize);
        printf("*%s.partition_status:  %d\n", diskManangerNotify->diskPartitionNotify.devPartitionNode, diskManangerNotify->diskPartitionNotify.devPartitionStatus);
        printf("*%s.partition_type_id: 0x%2x\n", diskManangerNotify->diskPartitionNotify.devPartitionNode, diskManangerNotify->diskPartitionNotify.devPartitionFileSystemId);
        printf("*%s.partition_file:    %s\n", diskManangerNotify->diskPartitionNotify.devPartitionNode, diskManangerNotify->diskPartitionNotify.devPartitionFileSystem);
        printf("*--------------Client Notify end-------------\n");

#endif    
        if(diskManangerNotify->opCode == 0)//add
        {
            pair<map<string, DiskDevPartitionNode>::iterator,bool> ret;
            string devNode = diskManangerNotify->diskPartitionNotify.devPartitionNode;

            map<string, DiskDevPartitionNode>::iterator  it;
            it = m_diskPartitionMap.find(devNode);
            if(it != m_diskPartitionMap.end())
            {
                m_diskPartitionMap.erase(it);
            }

            ret = m_diskPartitionMap.insert(pair<string, DiskDevPartitionNode>(devNode, diskManangerNotify->diskPartitionNotify));
            if(ret.second)
            {
                //printf("insert map success!\n");
                map<string, DiskDevPartitionNode>::iterator  iter;
                //printf("*--------------Map success begin-------------\n");
                //printf("dev_node: %s\n", diskManangerNotify->diskPartitionNotify.dev_node);
                for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
                {
                    //printf("\n*** dev_node: %s\n", iter->first.c_str());
                }
                //printf("*--------------Map success end---------------\n");                
            }
            else
            {
                printf("*Client insert map failed!\n");
                map<string, DiskDevPartitionNode>::iterator  iter;
                //printf("*--------------Map failed begin-------------\n");
                //printf("dev_node: %s\n", diskManangerNotify->diskPartitionNotify.dev_node);
                for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
                {
                    //printf("\n*** dev_node: %s\n", iter->first.c_str());
                }
                //printf("*--------------Map failed end---------------\n");
            }   
            m_pPlugInAndPullOutNoti->diskManagerPlugInNotify(&(diskManangerNotify->diskPartitionNotify));
        }
        else if(diskManangerNotify->opCode == 1)//remove
        {
            map<string, DiskDevPartitionNode>::iterator  iter;
            string devNode = diskManangerNotify->diskPartitionNotify.devPartitionNode;
            iter = m_diskPartitionMap.find(devNode);
            if( iter != m_diskPartitionMap.end())
            {
                  m_diskPartitionMap.erase(iter);
            }
            else
            {
                printf("*Client map Can not find the %s", diskManangerNotify->diskPartitionNotify.devPartitionNode);
            }
            m_pPlugInAndPullOutNoti->diskManagerPullOutNotify(&(diskManangerNotify->diskPartitionNotify));
        }
    }
#ifdef IBAC_DISKMANAGER_CLIENT_LOG    
    map<string, DiskDevPartitionNode>::iterator  iter;
    printf("*--------------Client Map begin-------------\n");
    printf("*Map size = %d\n", m_diskPartitionMap.size());
    for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
    {
        printf("*%s.devPartitionType:              %d\n", iter->first.c_str(), iter->second.devPartitionType);
        printf("*%s.name:              %s\n", iter->first.c_str(), iter->second.devPartitionName);
        printf("*%s.dev_node:          %s\n", iter->first.c_str(), iter->second.devPartitionNode);
        printf("*%s.label:             %s\n", iter->first.c_str(), iter->second.devPartitionLabel);
        printf("*%s.mount_path:        %s\n", iter->first.c_str(), iter->second.devPartitionMountPath);
        printf("*%s.totalSize:         %lu\n", iter->first.c_str(), iter->second.devPartitionTotalSize);
        printf("*%s.freeSize:          %lu\n", iter->first.c_str(), iter->second.devPartitionFreeSize);
        printf("*%s.partition_status:  %d\n", iter->first.c_str(), iter->second.devPartitionStatus);
        printf("*%s.partition_type_id: 0x%2x\n", iter->first.c_str(), iter->second.devPartitionFileSystemId);
        printf("*%s.partition_file:    %s\n", iter->first.c_str(), iter->second.devPartitionFileSystem);                
    }
    printf("*--------------Client Map end---------------\n");
#endif
ENDFUNC_DISKMANAGER_CLIENT    
}

DiskManagerDevClientMap DiskManagerClient::getDiskPartitonsList()
{
BEGINFUNC_DISKMANAGER_CLIENT
    DiskManagerDevClientMap dPartitionMap;
    map<string, DiskDevPartitionNode>::iterator  iter;
#if 0
    printf("*--------------Map begin-------------\n");
    printf("*Map size = %d\n", m_diskPartitionMap.size());
    for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
    {
        printf("*%s.devPartitionType:              %d\n", iter->first.c_str(), iter->second.devPartitionType);
        printf("*%s.name:              %s\n", iter->first.c_str(), iter->second.devPartitionName);
        printf("*%s.dev_node:          %s\n", iter->first.c_str(), iter->second.devPartitionNode);
        printf("*%s.label:             %s\n", iter->first.c_str(), iter->second.devPartitionLabel);
        printf("*%s.mount_path:        %s\n", iter->first.c_str(), iter->second.devPartitionMountPath);
        printf("*%s.totalSize:         %lu\n", iter->first.c_str(), iter->second.devPartitionTotalSize);
        printf("*%s.freeSize:          %lu\n", iter->first.c_str(), iter->second.devPartitionFreeSize);
        printf("*%s.partition_status:  %d\n", iter->first.c_str(), iter->second.devPartitionStatus);
        printf("*%s.partition_type_id: 0x%2x\n", iter->first.c_str(), iter->second.devPartitionFileSystemId);
        printf("*%s.partition_file:    %s\n", iter->first.c_str(), iter->second.devPartitionFileSystem);                
    }
    printf("*--------------Map end---------------\n");  
#endif

    for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
    {
        dPartitionMap.insert(*iter);
    }
ENDFUNC_DISKMANAGER_CLIENT
    return dPartitionMap;
}

DiskManagerDevClientMap DiskManagerClient::getDiskDevNodeInfo(DiskDevType diskManagerDevType)
{
    DiskManagerDevClientMap dPartitionMap;
    map<string, DiskDevPartitionNode>::iterator  iter;
//    printf("(diskManagerDevType= %d)\n(m_diskPartitionMap.size= %d)\n", diskManagerDevType, m_diskPartitionMap.size());
    dPartitionMap.clear();
    switch(diskManagerDevType)
    {
        case SD:
            printf("---SD---\n");
            for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
            {
                if(iter->second.devPartitionType == SD)
                {
                    dPartitionMap.insert(*iter);
                }
            }
        break;

        case USB:
            printf("---USB---\n");
            for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
            {
                if(iter->second.devPartitionType == USB)
                {
                    dPartitionMap.insert(*iter);
                }
            }        
        break;

        case NAND:
            printf("---NAND---\n");        
            for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
            {
                if(iter->second.devPartitionType == NAND)
                {
                    dPartitionMap.insert(*iter);
                }
            }        
        break;

        case EMMC:
            printf("---EMMC---\n");        
            for(iter = m_diskPartitionMap.begin(); iter != m_diskPartitionMap.end(); iter++)
            {
                if(iter->second.devPartitionType == EMMC)
                {
                    dPartitionMap.insert(*iter);
                }
            }        
        break;
        
        default:
        printf("Do not find the dev node!\n");
    }
    if(dPartitionMap.size() == 0)
    {
        printf("Do not find the dev node!\n");
    }
    return dPartitionMap;
}

int DiskManagerClient::refreshDiskPartitonsList()
{
BEGINFUNC_DISKMANAGER_CLIENT
    initDiskManagerClient();
ENDFUNC_DISKMANAGER_CLIENT    
    return 0;
}

// internal lock
DiskManagerClient::InternalLock::InternalLock(pthread_mutex_t* lock)
{
	m_lock=lock;
	pthread_mutex_lock(m_lock);
}
DiskManagerClient::InternalLock::~InternalLock()
{
	pthread_mutex_unlock(m_lock);
}

