#ifndef DISKMANAGERCLIENT_H
#define DISKMANAGERCLIENT_H
#include <map>
#include "dbusClientProxy.h"
#include "diskManager.h"

//#define IBAC_DISKMANAGER_CLIENT_LOG
#ifdef IBAC_DISKMANAGER_CLIENT_LOG
#define BEGINFUNC_DISKMANAGER_CLIENT	printf("\n[%s] %s begin \n", __FILE__, __FUNCTION__);
#define ENDFUNC_DISKMANAGER_CLIENT	printf("\n[%s] %s end \n", __FILE__, __FUNCTION__ );
#else
#define BEGINFUNC_DISKMANAGER_CLIENT	
#define ENDFUNC_DISKMANAGER_CLIENT	
#endif

using namespace std;

typedef map<string,DiskDevPartitionNode> DiskManagerDevClientMap;

class DiskManagerPlugInAndPullOutNotify
{
public:
	virtual void diskManagerPlugInNotify(DiskDevPartitionNode *) {}
	virtual void diskManagerPullOutNotify(DiskDevPartitionNode *) {}
};

class DiskManagerClient
{
public:
	DiskManagerClient(DBusClientProxyC *pClient, DiskManagerPlugInAndPullOutNotify *pPlugInAndPullOutNoti);
	~DiskManagerClient();
	int initDiskManagerClient();
	int refreshDiskPartitonsList();	
	DiskManagerDevClientMap getDiskPartitonsList();
	DiskManagerDevClientMap getDiskDevNodeInfo(DiskDevType diskManagerDevType);
	
private:
	static void diskManagerNotifyHandler(DBusNotifyC *notify);	
	
private:
	DiskManager *m_diskManager;
	static DiskManagerDevClientMap m_diskPartitionMap;
	DiskDevPartitionNode m_diskPartitionNodes[20];
	int m_diskPartitionNum;
	DBusClientProxyC *m_client;
	static DiskManagerPlugInAndPullOutNotify *m_pPlugInAndPullOutNoti;
	
private:
	pthread_mutex_t         m_lock;

	class	InternalLock
	{
	public:
		InternalLock(pthread_mutex_t* lock);
		~InternalLock();
	private:
		pthread_mutex_t* m_lock;
	};
};

#endif
