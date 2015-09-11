#ifndef DISKMANAGERSERVER_H
#define DISKMANAGERSERVER_H

#include <map>
#include "dbusServer.h"
#include "diskManager.h"

//#define IBAC_DISKMANAGER_SERVER_LOG
#ifdef IBAC_DISKMANAGER_SERVER_LOG
#define BEGINFUNC_DISKMANAGER_SERVER	printf("\n[%s] %s begin \n", __FILE__, __FUNCTION__);
#define ENDFUNC_DISKMANAGER_SERVER	printf("\n[%s] %s end \n", __FILE__, __FUNCTION__ );
#else
#define BEGINFUNC_DISKMANAGER_SERVER	
#define ENDFUNC_DISKMANAGER_SERVER	
#endif

using namespace std;	

typedef map<string,DiskDevPartitionNode> DiskManagerDevServerMap;

class DiskManagerServer
{
public:
	DiskManagerServer(DBusServerC *pServer);
	~DiskManagerServer();
	int initDiskManagerServer();

	DiskManagerDevServerMap getDiskPartitonsList();
	DiskManagerDevServerMap getDiskDevNodeInfo(DiskDevType diskManagerDevType);
	int refreshDiskPartitonsList();

private:
	int initDevMonitorThread();
	int initMonitorSocket();
	static void *monitorThread(void *);
	static int plugInDiskDevice(char *devName);
	static int pullOutDiskDevice(string devPartitionName);
	static void diskManagerNotifyHandler(DiskDevPartitionNode *partName, int option);
	
private:
	static DiskManager *m_diskManager;
	static DiskManagerDevServerMap m_diskPartitionMap;//map<string, DiskDevPartitionNode>
	DiskDevPartitionNode m_diskPartitionNodes[20];
	int m_diskPartitionNum;
	static DBusServerC *m_server;
	static int m_sock;
	int m_monitorThreadFlag;
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
