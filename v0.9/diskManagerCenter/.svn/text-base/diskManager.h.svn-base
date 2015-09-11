#ifndef DISKMANAGER_H
#define DISKMANAGER_H
#include <sys/statvfs.h>
#include <string>
#include <vector>
#include <QObject>

//#include <QBasicTimer>
//#include <QTimer>
//#define IBAC_DISKMANAGER_LOG
#ifdef IBAC_DISKMANAGER_LOG
#define BEGINFUNC_DISKMANAGER	printf("\n[%s] %s begin \n", __FILE__, __FUNCTION__);
#define ENDFUNC_DISKMANAGER	printf("\n[%s] %s end \n", __FILE__, __FUNCTION__ );
#else
#define BEGINFUNC_DISKMANAGER	
#define ENDFUNC_DISKMANAGER	
#endif

#define MAP_KEY_LEN  30

#define MAX_PATH_LEN 256
#define FS_BLOCK_SIZE	 1024

using namespace std;

typedef enum _DiskDevPartitionStatus//_DiskDevPartitionStatus
{
        UNPLUGGED = -1,       //拔出 
        UMOUNTED= 0,      //插入但没有Mount /Mount 失败
        MOUNTED = 1,          //插入并Mount成功
        DONTMOUNT = 2,     //不需要Mount 
}DiskDevPartitionStatus;//DiskDevPartitionStatus;

typedef enum _IB_Partition_Type
{
        PRIMARY = 0,           //主分区  (1~4)
        EXTENDED ,             //扩展分区(1~4)
        LOGICAL,                //逻辑分区(5~n)
}IB_Partition_Type;

typedef enum _IB_Partition_FileSystem
{
        UNKNOWN = -1,
        FAT,
        NTFS,
        EXT2,
        EXT3,
        SWAP,
}IB_Partition_FileSystem;

typedef enum _DiskDevType //_DiskDevType
{
	NAND,
	SD,
	USB,
	EMMC,
}DiskDevType;//DiskDevType;

typedef struct _DiskDevPartitionNode// DiskDevPartitionNode
{
	DiskDevType devPartitionType;//devPartitionType;
    char devPartitionName[30];//char devPartitionName[30];//
	char devPartitionNode[30];      //char devPartitionNode[30];//
	char devPartitionLabel[30];//    char devPartitionLabel[64];//

    char  devPartitionMountPath[MAX_PATH_LEN];//char devPartitionMountPath[MAX_PATH_LEN];//
	unsigned long devPartitionTotalSize;//    int  devPartitionTotalSize;//
    unsigned long devPartitionFreeSize;
    DiskDevPartitionStatus devPartitionStatus;//devPartitionStatus;//
    unsigned char devPartitionFileSystemId;//devPartitionFileSystemId;//
	char devPartitionFileSystem[20];//    char devPartitionFileSystem;//
}DiskDevPartitionNode;

typedef struct _PartitionsFileNode//PartitionsFileNode
{
	DiskDevType devType;
	char devName[30];
	int devPartitonsNum;
	int blocks;
}PartitionsFileNode;

/*
struct StringLess
{
  bool operator()(const char* const& left,const char* const& right) const
  {
  return strcmp(left,right) < 0;
  }
};
*/

class DiskManager: public QObject
{
	Q_OBJECT
public:
	DiskManager(DiskDevPartitionNode *p);
	~DiskManager();
	int initDiskManager(bool flag);
	int closeDiskManager();
	int parsePartitionsFile(PartitionsFileNode *pPFNode);
	int parsePartitionsFile(char *devName, DiskDevPartitionNode *dp);
	int parseFdiskFile(char *dName, DiskDevPartitionNode dPartitions[]);

	int plugInPartitionMount(DiskDevPartitionNode *dPart);

	int doUmount(char *target);	
	int createMountDir(char *partitionName, char *dirPath);//mount table.
	int removeMountDir(char *dirName);
	unsigned char getFilesystemTypeId(char *devName);
	void getFilesystemType(unsigned char filesystemTypeId, char filesystemType[]);
	//int umountList();	
	int getFilesystemInfo(char *mountedPath, struct statvfs *sfs);
	
private:
	int checkMountAndMount(bool flag);
	int doMount(char *source, char *target, unsigned char filesystemTypeId);
	
private:
	void timerEvent(QTimerEvent *event);
	int m_timerId;
	//QBasicTimer *m_ticker;
	//QTimer *m_ticker;
	PartitionsFileNode partFNodes[10];
	char m_deviceName[5][15];
	DiskDevPartitionNode *m_pDiskPartitions;
	int m_partitionNum;
	vector<string> m_umountList;
    bool emmcFlag;
};

#endif
