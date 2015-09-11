#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "diskManager.h"

static int valid_part_table_flag(const unsigned char *mbuffer)
{
	return (mbuffer[510] == 0x55 && mbuffer[511] == 0xaa);
}

DiskManager::DiskManager(DiskDevPartitionNode *p)
{
    m_pDiskPartitions = p;
    m_partitionNum = 0;
//    memset(m_pDiskPartitions, 0, sizeof(m_pDiskPartitions));
    m_timerId = 0;
    m_timerId = startTimer(1000);// 1 second timer
    //m_ticker = new QBasicTimer(this);
    //m_ticker = new QTimer(this);
    m_umountList.clear();
//    m_timerId = startTimer(5000);// 5 second timer
    emmcFlag = true;
}

DiskManager::~DiskManager()
{

}

int DiskManager::initDiskManager(bool flag)
{
BEGINFUNC_DISKMANAGER
    //fdisk -l the device get the partitions of the device
	//initmouot
	//save the information to the list
	//pares partitions file get the device 	
    memset(m_pDiskPartitions, 0, sizeof(m_pDiskPartitions));
    m_partitionNum = 0;
	unsigned char typeId;
	PartitionsFileNode pfNode[20];
	memset(pfNode, 0, sizeof(pfNode));
	int devCount = 0;
	int partitionCount = 0;
    DiskDevPartitionNode diskPartition[8];
	devCount = parsePartitionsFile(pfNode);
//	printf("(~~~%d; %d)\n", devCount, m_partitionNum);
	if (devCount > 0)
	{
        for(int i=0; i < devCount; i++)
        {
            if (pfNode[i].devPartitonsNum > 1) 
            {
                partitionCount = parseFdiskFile(pfNode[i].devName, diskPartition);
                if (partitionCount > 0)
                {
                    for (int j=0; j<partitionCount; j++)
                    {
                        m_pDiskPartitions[m_partitionNum].devPartitionType = diskPartition[j].devPartitionType;//pfNode[i].devType;
                        strcpy((&m_pDiskPartitions[m_partitionNum])->devPartitionName, (&diskPartition[j])->devPartitionName);
                        strcpy((&m_pDiskPartitions[m_partitionNum])->devPartitionNode, (&diskPartition[j])->devPartitionNode);
                        //(&m_pDiskPartitions[m_partitionNum])->devPartitionTotalSize = (&diskPartition[j])->devPartitionTotalSize;
                        (&m_pDiskPartitions[m_partitionNum])->devPartitionFileSystemId = (&diskPartition[j])->devPartitionFileSystemId;
                        strcpy((&m_pDiskPartitions[m_partitionNum])->devPartitionFileSystem, (&diskPartition[j])->devPartitionFileSystem);

                        //printf("m_pDiskPartitions[%d].devPartitionName= %s\n", j, (&m_pDiskPartitions[m_partitionNum])->devPartitionName);
                        //printf("m_pDiskPartitions[%d].devPartitionNode= %s\n", j, (&m_pDiskPartitions[m_partitionNum])->devPartitionNode);
                        //printf("m_pDiskPartitions[%d].totalSize= %d\n", j, (&m_pDiskPartitions[m_partitionNum])->totalSize);
                        //printf("m_pDiskPartitions[%d].partition_type_id= 0x%2x\n", j, (&m_pDiskPartitions[m_partitionNum])->partition_type_id);
                        //printf("m_pDiskPartitions[%d].partition_file= %s\n", j, (&m_pDiskPartitions[m_partitionNum])->partition_file);
                        m_partitionNum++;
                    }
                }
                else
                {
                    printf("Err: partitionCount= %d; %s\n", partitionCount, pfNode[i].devName);
                }
            }
            else if(pfNode[i].devPartitonsNum == 1)
            {
                //printf("@@@devName= %s\n@@@devPartitionType= %d\n", pfNode[i].devName, pfNode[i].devType);
                (&m_pDiskPartitions[m_partitionNum])->devPartitionType = pfNode[i].devType;
                strcpy((&m_pDiskPartitions[m_partitionNum])->devPartitionName, pfNode[i].devName);
                strcpy((&m_pDiskPartitions[m_partitionNum])->devPartitionNode, pfNode[i].devName);
                //(&m_pDiskPartitions[m_partitionNum])->devPartitionTotalSize = pfNode[i].blocks;
                //get the filesystem type from the Partition table
                if ((typeId = getFilesystemTypeId((&m_pDiskPartitions[m_partitionNum])->devPartitionName)) != 0x00)
                {
                    (&m_pDiskPartitions[m_partitionNum])->devPartitionFileSystemId = typeId;
                    strcpy((&m_pDiskPartitions[m_partitionNum])->devPartitionFileSystem, "Unknown");
                }
                else 
                {
                    (&m_pDiskPartitions[m_partitionNum])->devPartitionFileSystemId = 0x0C;
                    strcpy((&m_pDiskPartitions[m_partitionNum])->devPartitionFileSystem, "FAT_32");
                }
                m_partitionNum ++;
            }
        }
        //if(true == flag)
        checkMountAndMount(flag);
	}
	//printf("~~~%d; %d\n", devCount, m_partitionNum);
ENDFUNC_DISKMANAGER	
	return m_partitionNum;
}

int DiskManager::closeDiskManager()
{
BEGINFUNC_DISKMANAGER
ENDFUNC_DISKMANAGER
    return 0;
}

int DiskManager::parsePartitionsFile(PartitionsFileNode *pPFNode)
{
BEGINFUNC_DISKMANAGER
    char nameBuf[30];
    char tmp[20];
    char stream[100];
    int dCount = 0;
    int dCountTmp;
    int blocks = 0;
    //bool emmcFlag = true;
    dCountTmp = dCount;
    FILE *fp = fopen("/proc/partitions", "r");
    if(fp == NULL)  
    {
        printf("Can not open partitions file!\n");
    	return -1;
    }
    
    memset(pPFNode, 0, sizeof(pPFNode));
     /* skip the first line */  
    fgets(tmp, sizeof(tmp), fp);  
    fgets(tmp, sizeof(tmp), fp);
    while (fgets(stream, sizeof(stream), fp))
    {
//    	printf("stream: %s\n", stream);
        sscanf(stream, "%s %s %d %s", tmp, tmp, &blocks, nameBuf);
        if(strstr(nameBuf, "ndda"))
        {            
            emmcFlag = false;
            if(strcmp(nameBuf, "ndda") == 0)
            {
                dCountTmp = dCount;
                pPFNode[dCountTmp].devType = NAND;
                strcpy(pPFNode[dCountTmp].devName, nameBuf);
                pPFNode[dCountTmp].devPartitonsNum ++;
                pPFNode[dCountTmp].blocks = blocks;
                dCount ++;
                continue;
            }
            else
            {
                pPFNode[dCountTmp].devPartitonsNum ++;
            }
        }
        else if (strstr(nameBuf, "mmcblk"))
        {
//            pPFNode[dCountTmp].devPartitonsNum ++;
            //if((true == emmcFlag) && (strstr(nameBuf, "mmcblk0p1") != NULL))
            //{
            //    continue;
            //}
            if(strstr(nameBuf, "mmcblk") && strlen(nameBuf) == 7)
            {
                dCountTmp = dCount;
                pPFNode[dCountTmp].devType = SD;
                strcpy(pPFNode[dCountTmp].devName, nameBuf);
                pPFNode[dCountTmp].devPartitonsNum ++;
                pPFNode[dCountTmp].blocks = blocks;
                dCount ++;
                continue;
            }
            else
            {
                pPFNode[dCountTmp].devPartitonsNum ++;
            }

        }
        else if(strstr(nameBuf, "sd"))
        {
            if(strstr(nameBuf, "sd") && strlen(nameBuf) == 3)
            {
                dCountTmp = dCount;
                pPFNode[dCountTmp].devType = USB;
                strcpy(pPFNode[dCountTmp].devName, nameBuf);
                pPFNode[dCountTmp].devPartitonsNum ++;
                pPFNode[dCountTmp].blocks = blocks;
                dCount ++;
                continue;
            }
            else
            {
                pPFNode[dCountTmp].devPartitonsNum ++;
            }
        }
    }
    fclose(fp);
//    printf("***dCount= %d\n", dCount);
/*    for(int i=0; i<dCount; i++)
    {
        printf("PartitionsFileNode[%d]: %10s; %d; %d\n", \
        i, pPFNode[i].devName, pPFNode[i].devPartitonsNum, pPFNode[i].blocks);
    }
*/
ENDFUNC_DISKMANAGER    
    return dCount;
}

int DiskManager::parsePartitionsFile(char *devName, DiskDevPartitionNode *dp)//return the device partition num
{
	
BEGINFUNC_DISKMANAGER
    char tmp[20];
    char stream[100];
    int devPartitonsCount = 0 ;
    int blocks = 0;

    
    FILE *fp = fopen("/proc/partitions", "r");  
    if(fp == NULL)  
    {
    	return -1;  
    }
        /* skip the first line */  
    fgets(tmp, sizeof(tmp), fp);  
    fgets(tmp, sizeof(tmp), fp);
    while (fgets(stream, sizeof(stream), fp))
    {
        if(strstr(stream, devName))// ??? create a func
        {
            if(strstr(devName, "ndda"))
            {
                dp->devPartitionType = NAND;                
            }
            else if (strstr(devName, "mmcblk"))
            {
                dp->devPartitionType = SD;
            }
            else if (strstr(devName, "sd"))
            {
                dp->devPartitionType = USB;
            }
            //printf("@@@devName= %s\ndp.devPartitionType: %d\n", devName, dp->devPartitionType);
            sscanf(stream, "%s %s %d %s", tmp, tmp, &blocks, tmp);
            //dp->devPartitionTotalSize = blocks;
            devPartitonsCount ++;
        }
    }
    fclose(fp);
ENDFUNC_DISKMANAGER    
    return devPartitonsCount;
}

int DiskManager::parseFdiskFile(char *dName, DiskDevPartitionNode *dPartitions)
//get device all information from the fdisk -l /dev/name
{
BEGINFUNC_DISKMANAGER
    char cmd[50];
    char tmp[60];
    char stream[200];
    char deviceBootBuf[60];
    char deviceBoot[60];
    char blocks[60];
    int dPartitionsNum = 0;
    unsigned int Id;
    char systemBuf[60];
//    bool emmcFlag = true;
    sprintf(cmd, "fdisk -l /dev/%s > /tmp/fdisktmp.log", dName);
    if( system(cmd) < 0)
    {
        printf("system( %s ) failed!\n", cmd);
        return -1;
    }
    
    FILE *fp = fopen("/tmp/fdisktmp.log", "r");  
    if(fp == NULL)  
    {   
    	fprintf(stderr, " Read /tmp/fdisktmp.log failed!\n");
    	return -1;  
    }	
    memset(dPartitions, 0, sizeof(dPartitions));
    //skip 6 lines.
    for (int i=0; i<6; i++)
        fgets(stream, sizeof(stream), fp);
    while (fgets(stream, sizeof(stream), fp))
    {
        if(!strstr(stream, "/dev"))
            continue;
    	sscanf(stream, "%s %s %s %s %2x %s", deviceBootBuf, tmp, tmp, blocks, &Id, systemBuf);//blocks 189472+???
        printf("@@@@: stream = %s\n deviceBootBuf = %s; Id = %X; systemBuf = %s\n", stream, deviceBootBuf, Id, systemBuf);
    	if(strstr(systemBuf, "Extended") || strstr(systemBuf, "swap"))
    	    continue;

        if(strstr(dName, "ndda"))// ??? create a func
        {
            emmcFlag = false;
            (&dPartitions[dPartitionsNum])->devPartitionType = NAND;                
        }
        else if (strstr(dName, "mmcblk"))
        {
            if(true == emmcFlag)
            {
                if(strstr(deviceBootBuf, "mmcblk0p1") != NULL)
                {
                    continue;
                }
                if(strstr(deviceBootBuf, "mmcblk0p") != NULL)
                {
                    (&dPartitions[dPartitionsNum])->devPartitionType = EMMC;
                }
                else
                {
                    (&dPartitions[dPartitionsNum])->devPartitionType = SD;
                }
            }
            else
            {
                (&dPartitions[dPartitionsNum])->devPartitionType = SD;
            }
        }
        else if (strstr(dName, "sd"))
        {
            (&dPartitions[dPartitionsNum])->devPartitionType = USB;
        }

        strcpy(deviceBoot, (strrchr(deviceBootBuf, '/')+1));
        strcpy((&dPartitions[dPartitionsNum])->devPartitionName, dName);
        strcpy((&dPartitions[dPartitionsNum])->devPartitionNode, deviceBoot);
        //(&dPartitions[dPartitionsNum])->devPartitionTotalSize = atoi(blocks);                    //???  189472+
        (&dPartitions[dPartitionsNum])->devPartitionFileSystemId = Id;
        strcpy((&dPartitions[dPartitionsNum])->devPartitionFileSystem, systemBuf);
//        printf("dPartitions[%d])->dev_node: %s\n", dPartitionsNum, (&dPartitions[dPartitionsNum])->dev_node);

        dPartitionsNum ++;
    }
    fclose(fp);
ENDFUNC_DISKMANAGER    
    return dPartitionsNum;
}

int DiskManager::checkMountAndMount(bool flag)//set the mount flag //init mount
{
BEGINFUNC_DISKMANAGER
    //for (int i=0; i<m_partitionNum; i++)
    //printf("m_pDiskPartitions: %s\n", (&m_pDiskPartitions[i])->dev_node);
    
    int ret;
    char mountPath[30];
    char mountsFilePath[30];
    char tmpBuff[1024];
    char buff[1024];
    char *pTmp;
    struct statvfs sBuf;

    FILE *fp = popen("cat /proc/mounts", "r");
    if(fp == NULL)  
    {   
        printf("fopen /proc/mounts failed!\n");
        return -1;
    }	
    
    memset(buff, 0, 1024); 
    fread(buff, 1024, 1, fp);
    
    for (int i=0; i<m_partitionNum; i++)
    {
        //if(true == flag)
        ret = createMountDir((&m_pDiskPartitions[i])->devPartitionNode, mountPath);
/*    printf("-----------\n");
    printf("%s\n", buff);
    printf("-----------\n");            
    */
        if( (pTmp = strstr(buff, (&m_pDiskPartitions[i])->devPartitionNode)) != NULL )
        //if mounted,get the mount path,and if get the mount path == the right path
        //get the mount path == the right path, set the partition_status = MOUNTED.???
        {                
            memset(tmpBuff, 0, sizeof(tmpBuff));
            strcpy(tmpBuff, pTmp);
            strtok(tmpBuff, " ");
            strcpy(mountsFilePath, strtok(NULL, " "));
            //printf("*** mountsFilePath: %s; mountPath: %s.\n", mountsFilePath, mountPath);
//            if(strcmp(mountsFilePath, mountPath) == 0)
//            {
//                printf("/proc/mounts: %s\n", mountsFilePath);
                strcpy((&m_pDiskPartitions[i])->devPartitionMountPath, mountsFilePath);
                (&m_pDiskPartitions[i])->devPartitionStatus = MOUNTED;
                if(getFilesystemInfo(m_pDiskPartitions[i].devPartitionMountPath, &sBuf) == 0)
                {
                    m_pDiskPartitions[i].devPartitionTotalSize = (sBuf.f_bsize/FS_BLOCK_SIZE)*sBuf.f_blocks;
                    m_pDiskPartitions[i].devPartitionFreeSize = (sBuf.f_bsize/FS_BLOCK_SIZE)*sBuf.f_bavail;
                }
                continue;
//            }
        }

        if(false == flag)
        {
            pclose(fp);            
            printf("\n ~~~isClient init!\n");
            return 0;
        }
        if ( ret >= 0)
        {
            if(doMount((&m_pDiskPartitions[i])->devPartitionNode, mountPath, (&m_pDiskPartitions[i])->devPartitionFileSystemId) == 0)
            {
                strcpy((&m_pDiskPartitions[i])->devPartitionMountPath, mountPath);
                (&m_pDiskPartitions[i])->devPartitionStatus = MOUNTED;
                if(getFilesystemInfo(m_pDiskPartitions[i].devPartitionMountPath, &sBuf) == 0)
                {
                    m_pDiskPartitions[i].devPartitionTotalSize = (sBuf.f_bsize/FS_BLOCK_SIZE)*sBuf.f_blocks;
                    m_pDiskPartitions[i].devPartitionFreeSize = (sBuf.f_bsize/FS_BLOCK_SIZE)*sBuf.f_bavail;
                }                
            }
            else
            {
                (&m_pDiskPartitions[i])->devPartitionStatus = UMOUNTED;
            }
        }
    }
    pclose(fp);
ENDFUNC_DISKMANAGER    
    return 0;
}

int DiskManager::plugInPartitionMount(DiskDevPartitionNode *dPart)
{
	
BEGINFUNC_DISKMANAGER
        struct statvfs sBuf;
        char mountPath[30];
        int ret;
        ret = createMountDir(dPart->devPartitionNode, mountPath);
        if (ret < 0)
        {
            printf("[plugInPartitionMount] createMountDir failed!\n");
            return -1;
        }

        ret = doMount(dPart->devPartitionNode, mountPath, dPart->devPartitionFileSystemId);
        //printf("doMount_ret= %d; mountPath: %s\n", ret, mountPath);
        if( ret == 0)
        {
            strcpy(dPart->devPartitionMountPath, mountPath);
            dPart->devPartitionStatus = MOUNTED;
            if(getFilesystemInfo(dPart->devPartitionMountPath, &sBuf) == 0)
            {
                dPart->devPartitionTotalSize = (sBuf.f_bsize/FS_BLOCK_SIZE)*sBuf.f_blocks;
                dPart->devPartitionFreeSize = (sBuf.f_bsize/FS_BLOCK_SIZE)*sBuf.f_bavail;
            }            
        }
        else 
        {   
             dPart->devPartitionStatus = UMOUNTED;
            return -1;
        }
ENDFUNC_DISKMANAGER        
        return 0;            
}

int DiskManager::doMount(char *source, char *target, unsigned char filesystemTypeId)
{
BEGINFUNC_DISKMANAGER
    char cmd[1024];
    char filesystemtype[10];
    char devNode[20];
    getFilesystemType(filesystemTypeId, filesystemtype);
    sprintf(devNode, "/dev/%s", source);
    printf("===== source: %s;  target: %s;  filesystemtypeID is %d;  filesystemtype is %s. =====\n", devNode, target,filesystemTypeId, filesystemtype);
    if(filesystemTypeId == 0x83)
    {
        if (mount(devNode, target, "ext2", 0, 0) < 0)
        {
            if (mount(devNode, target, "ext3", 0, 0) < 0)
            {
                printf("mount failed! 1Err: %s\n", strerror(errno));
                return -1;
            }
        }
    }
    else if(0x07 == filesystemTypeId||0x37==filesystemTypeId)
    {
        memset(cmd, 0, 1024);
        sprintf(cmd, "ntfs-3g %s %s", devNode, target);
     //   printf("$$$$$$$$$$$$$ %s", cmd);
        system(cmd);
    }
    else 
    {
        if (mount(devNode, target, filesystemtype, 0, "iocharset=utf8") < 0)
        {
            if (mount(devNode, target, filesystemtype, 0, "iocharset=utf8") < 0)
            {
                printf("mount failed! 2Err: %s\n", strerror(errno));
                return -1;
            }
        }
    }
/*
    char cmd[200];
    int ret;
    if(filesystemTypeId == 0x83)
    {
        sprintf(cmd, "mount -t ext2 %s %s", devNode, target);
        printf("%s\n", cmd);
        ret = system(cmd);
        printf("**ret = %d\n", ret);
        if(ret < 0)
        {
            sprintf(cmd, "mount -t ext3 %s %s", devNode, target);
            printf("%s\n", cmd);
            system(cmd);
        }
    }
    else
    {
        sprintf(cmd, "mount -t %s %s %s", filesystemtype, devNode, target);
        printf("%s\n", cmd);
        ret = system(cmd);
        printf("**ret = %d\n", ret);
    }
    */
ENDFUNC_DISKMANAGER    
    return 0;
}

int DiskManager::doUmount(char *target)
{
BEGINFUNC_DISKMANAGER
	/*    char cmd[100];
    int ret;
    sprintf(cmd, "umount %s", target);
    printf("%s\n", cmd);
    ret = system(cmd);
    if(ret != 0)
    {
        //check the umountList,if do not have target, insert to the umountList
        string umountNode(target);
        m_umountList.push_back(umountNode);
        //if timer is not started ,start timer
        if(m_timerId == 0)
        {
            m_timerId = startTimer(5000);// 5 second timer
        }
    }
*/
    if (umount(target) < 0)
    {
        printf("umount first failed!  Err: %s\n", strerror(errno));
        if(umount(target) < 0)
        {
            printf("umount second failed!  Err: %s\n", strerror(errno));
            //check the umountList,if do not have target, insert to the umountList
            string umountNode(target);
            m_umountList.push_back(umountNode);
            //if timer is not started ,start timer
            //if(m_timerId == 0)
            /*if(!m_ticker->isActive())
            {
                //m_timerId = startTimer(1000);// 1 second timer
                //m_ticker->start(1000, this);
                m_ticker->start(1000);
                //printf("@@@ m_timerId: %d.\n", m_timerId);
                printf("@@@ m_ticker.\n");
            }*/            
            return -1;
        }
    }
	return 0;
ENDFUNC_DISKMANAGER    
    
}

/*!
  *  mount table
  *  
  *  mmcblk0p1->/tmp/sd1/sd1_1
  *  mmcblk0p2->/tmp/sd1/sd1_2
  *  mmcblk0p3->/tmp/sd1/sd1_3
  *  mmcblk0p5->/tmp/sd1/sd1_5
  *  mmcblk0p6->/tmp/sd1/sd1_6
  *
  *  mmcblk1p1->/tmp/sd2/sd2_1 ...
  */
int DiskManager::createMountDir(char *partitionName, char *dirPath)//mount table.
{
BEGINFUNC_DISKMANAGER

    char cmd[100];
    memset(cmd, 0, sizeof(cmd));
    memset(dirPath, 0, sizeof(dirPath));
//    printf("partitionName: %s\n", partitionName);
	if(strstr(partitionName, "mmcblk"))
	{
	    char tmpa, tmpb;
	    if(strlen(partitionName) >= 9)
	    {
            tmpa = *(partitionName+6);
            tmpb = *(partitionName+8);
            if(tmpa == '0' || tmpa == '1' || tmpa == '2')
            {
                sprintf(dirPath, "/tmp/sd%c/sd%c", tmpa, tmpb);
                sprintf(cmd, "mkdir -p %s", dirPath);
                system(cmd);                
            }
	    }
	    else
	    {
            tmpa = *(partitionName+6);
            if(tmpa == '0' || tmpa == '1' || tmpa == '2')
            {
                sprintf(dirPath, "/tmp/sd%c/sd0", tmpa);
                sprintf(cmd, "mkdir -p %s", dirPath);
                system(cmd);                
            }
	    }
	}
	else if(strstr(partitionName, "sd"))  // usb
	{
	    char tmpa, tmpb;
        if(strlen(partitionName) >= 4)
        {
            tmpa = *(partitionName+2);
            tmpb = *(partitionName+3);
            if(tmpa == 'a')
            {
                sprintf(dirPath, "/tmp/usb1/usb%c", tmpb);
                sprintf(cmd, "mkdir -p %s", dirPath);
                system(cmd);                
            }
            if(tmpa == 'b')
            {
                sprintf(dirPath, "/tmp/usb2/usb%c", tmpb);
                sprintf(cmd, "mkdir -p %s", dirPath);
                system(cmd);                
            }
            if(tmpa == 'c')
            {
                sprintf(dirPath, "/tmp/usb3/usb%c", tmpb);
                sprintf(cmd, "mkdir -p %s", dirPath);
                system(cmd);                
            }
            if(tmpa == 'd')
            {
                sprintf(dirPath, "/tmp/usb4/usb%c", tmpb);
                sprintf(cmd, "mkdir -p %s", dirPath);
                system(cmd);                
            }            
        }
        else
        {
            tmpa = *(partitionName+2);
            if(tmpa == 'a')
            {
                strcpy(dirPath, "/tmp/usb1/usb0");
                sprintf(cmd, "mkdir -p %s", dirPath);
                system(cmd);           
            }
            if(tmpa == 'b')
            {
                strcpy(dirPath, "/tmp/usb2/usb0");
                sprintf(cmd, "mkdir -p %s", dirPath);
                system(cmd);                
            }
            if(tmpa == 'c')
            {
                strcpy(dirPath, "/tmp/usb3/usb0");
                sprintf(cmd, "mkdir -p %s", dirPath);
                system(cmd);                
            }            
            if(tmpa == 'd')
            {
                strcpy(dirPath, "/tmp/usb4/usb0");
                sprintf(cmd, "mkdir -p %s", dirPath);
                system(cmd);                
            }            
        }
    }
	else if(strstr(partitionName, "ndda"))
	{
        if(strstr(partitionName, "ndda1"))
        {
            strcpy(dirPath, "/nand1");
            sprintf(cmd, "mkdir -p %s", dirPath);
            system(cmd);            
/*            if((ret = mkdir(dirPath, 0777)) < 0)
			{
			    printf("mkdir %s err: %s; ret=%d\n", dirPath, strerror(errno), ret);
			    if(errno != EEXIST)
        		return -1;
        	}*/
        }
        else if(strstr(partitionName, "ndda2"))
        {
            strcpy(dirPath, "/nand2");
            sprintf(cmd, "mkdir -p %s", dirPath);
            system(cmd);            
/*            if((ret = mkdir(dirPath, 0777)) < 0)
			{
			    printf("mkdir %s err: %s; ret=%d\n", dirPath, strerror(errno), ret);
			    if(errno != EEXIST)
        		return -1;
        	}*/
        }
        else if(strstr(partitionName, "ndda"))
        {
            strcpy(dirPath, "/nand0");
            sprintf(cmd, "mkdir -p %s", dirPath);
            system(cmd);            
/*            if((ret = mkdir(dirPath, 0777)) < 0)
			{
			    printf("mkdir %s err: %s; ret=%d\n", dirPath, strerror(errno), ret);
			    if(errno != EEXIST)
        		return -1;
        	}*/
        }
	}
  
ENDFUNC_DISKMANAGER	
    return 0;
}

int DiskManager::removeMountDir(char *dirName)
{
BEGINFUNC_DISKMANAGER
    char cmd[100];
    sprintf(cmd, "rmdir %s -p", dirName);
    system(cmd);
/*    if(rmdir(dirName) < 0)
    {
        if(rmdir(dirName) < 0)
        {
            
            printf("rmdir failed!Err: %s\n", strerror(errno));
            return -1;
        }
    }
    printf("----------------here removeMountDir () cmd [%s]-------------------- \n",cmd);
*/
ENDFUNC_DISKMANAGER    
    return 0;
}

unsigned char DiskManager::getFilesystemTypeId(char *devName)
{
BEGINFUNC_DISKMANAGER
   int fd;
   int idLocation = 0x1c2;
   char devNode[20];
   unsigned char buf[512];
   unsigned char ret = 0x00;
   sprintf(devNode, "/dev/%s", devName);
    fd = open(devNode, O_RDONLY);
    if(fd < 0)
    {
        printf("open %s failed!\n", devNode);
        return 0;
    }
    memset(buf, 0, 512);
    read(fd, buf, 512);
    close(fd);
    if(!(valid_part_table_flag(buf)))
        return 0;
    ret = buf[idLocation];
ENDFUNC_DISKMANAGER    
    return ret;
}

void DiskManager::getFilesystemType(unsigned char filesystemTypeId, char filesystemType[])
{
BEGINFUNC_DISKMANAGER
    switch(filesystemTypeId)
    {
        case 0x04:
        case 0x0B:
        case 0x0C:
        case 0x0E:
            strcpy(filesystemType, "vfat");
        break;
        case 0x83:
            strcpy(filesystemType, "ext2");
//            strcpy(filesystemType, "ext3");            
        break;
/*        
        case :
            strcpy(filesystemType, "ext3");
        break;

        case :
            strcpy(filesystemType, "yaffs");
        break;
        case :
            strcpy(filesystemType, "yaffs2");
        break;

        case :
            strcpy(filesystemType, "iso9660");
        break;
*/
        default:
        strcpy(filesystemType, "vfat");
    }
    //printf("filesystemTypeId=0x%2x; filesystemtype= %s\n", filesystemTypeId, filesystemType);    
ENDFUNC_DISKMANAGER    
}

int DiskManager::getFilesystemInfo(char *mountedPath, struct statvfs *sfs)
{
    sync();
    if( statvfs(mountedPath, sfs)!=0 )
    {   
        fprintf(stderr, "Cannot read super block !\n");
        return -1;
    }  
    return 0;
}

void DiskManager::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    //printf("---timerEvent list size[%d] ---\n", m_umountList.size());
    if(m_umountList.size() != 0)
    {      
        char tmpBuf[1025];
        FILE *pp = popen("cat /proc/mounts", "r");
        if(pp == NULL)  
        {   
            printf("fopen /proc/mounts failed!\n");
        }        
        memset(tmpBuf, 0, 1025); 
        fread(tmpBuf, 1024, 1, pp);
        pclose(pp);   

        vector<string>::iterator iter;
        //for(unsigned int i=0; i<m_umountList.size(); i++)
        for(iter = m_umountList.begin(); iter != m_umountList.end(); )
        {
            
            const char *target;
            target = (*iter).c_str();
//            printf("iter_target: %s, list.size= %d\n", target, m_umountList.size());
            
            //check the mountPath in the mounts file, 
            //if the target is not in the mounts file, do erase and continue.
            if(strstr(tmpBuf, target) == NULL)
            {
                printf("- %s - is in the /proc/mounts file!\n", target);
                iter = m_umountList.erase(iter);
                continue;
            }
            if(umount(target) < 0)
            {
            	  printf("Timer umount %s failed!  Err: %s\n", target, strerror(errno));
                if(errno == ENOENT)//No such file or directory
                {
                    iter = m_umountList.erase(iter);
                    continue;
                }
            }
            else
            {
                printf("Timer umount succeed\n");
                iter = m_umountList.erase(iter);
                continue;
            }
            ++ iter;
        }
    }
    /*else
    {
        //if ( 0 != m_timerId )
        if(m_ticker->isActive())
        {
            //killTimer(m_timerId);
            //m_timerId = 0;
            m_ticker->stop();
        }
        
    }*/
}

