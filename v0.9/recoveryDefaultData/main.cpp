//#include <QtGui/QApplication>
//#include "recoveryDefaultData.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <QDateTime>
#include <QFile>

#define MOUNT_LOG  "/tmp/mount_log"
#define RECOVERY_LOG_FILE "/recovery_log"
#define RECOVERY_SH "./data_backup.sh"
#define MAX_FILE 1024
#define MAX_FILE_LENGTH 256
#define NO_DATA   0  //没有读到数据
char g_file_list[MAX_FILE][MAX_FILE_LENGTH];
int g_file_list_count;
char g_log_info[MAX_FILE_LENGTH];
char g_mount_path[MAX_FILE_LENGTH];
/*!
  检查程序数据分区是否正确挂载
  \param path 程序数据分区：如"/nand3"
*/
bool isDataMountSuccess(char * path)
{
       char cmd[512];
        memset(cmd,0,sizeof(cmd));
        //检查/nand3/是否挂上先用df将结果输出到MOUNT_LOG这个文件中
        strcpy(cmd,"df >");
        strcat(cmd,MOUNT_LOG);
        system(cmd);
        FILE *fp =NULL;
        fp = fopen(MOUNT_LOG,"rb");
        if(NULL ==fp)
        {
            printf("MOUNT_LOG not exist.\n");
            return false;
        }

        //读取MOUNT_LOG到内存进行分析
        struct stat fileStat;
        stat(MOUNT_LOG, &fileStat);
        char * l_buffer =(char*) malloc(fileStat.st_size+10);
        memset(l_buffer,0,fileStat.st_size+10);
        unsigned int readSize = fread(l_buffer, 1,  fileStat.st_size, fp);
        //对MOUNT_LOG的结果中查找程序数据分区名称如果找到说明已挂上
        bool res = false;
        int i=0;
        int i_res;
        int i_path_len = strlen(path);
        while(i<(int)readSize-i_path_len)
        {
            i_res = strncmp(l_buffer+i,path,i_path_len);
            i++;
            if(i_res==0)
            {
                    res =  true;
                    break;
            }
        }

        free(l_buffer);
        if(NULL != fp)
        {
            fclose(fp);
            //删除内存中的临时文件
            memset(cmd,0,sizeof(cmd));
            strcpy(cmd,"rm ");
            strcat(cmd,MOUNT_LOG);
            system(cmd);
        }
        return res;
}


/*!
  从指定的buffer中读取一行，并返回下一次读取的偏移位置,，滤去本次的0xd,0xa
  \param char * buffer 文件列表缓存,*buffer必须指定一个非0xd,0xa,0x0的数据才认为是有效的。
  \param int buffer_len 缓存的长度
  \param char *out_buffer 输出缓存
  \param int out_buffer_len 输出缓存最大长度
  \return int 0表示没有读到数据，>0表示读到的数长度加上0xd,0xa等换行字符的长度。
*/
int readLine(char * buffer ,int buffer_len,char *out_buffer,int out_buffer_len)
{
        int i,j;
        char *p =buffer;
        for( i = 0,j=0;i < buffer_len;i++)
        {
            p = buffer +i;
            if(*p != 0 && *p != 0xd && *p != 0xa)
            {
                if(j<out_buffer_len-1)
                {
                    *(out_buffer+j) = *p;
                    j++;
                }
                else
                {
                    printf("error file over length.\n");
                    break;
                }
            }
            else
                break;
        }
        if(i==0)//未读字符直接返回
            return NO_DATA;
        if(*p == 0xd)
        {
            i++;
            p = buffer+i;
        }
        if(*p == 0xa)
        {
            i++;
            p = buffer+i;
        }

        return i;
}

/*!
  读取程序运行所需要的关键文件列表.关键文件是指如果缺少，部分进程将无法正常运行.
  \param file_list_path 列表文件全路径，该文件记录了程序数据分区有那些关键文件，列表文件格式要求
　　1.第一行表示一个文件全路径
      2.每一行以换行结尾。
    \return 返回读到文件数
*/
int    readKeyFileList(char * file_list_path,char l_file_list[MAX_FILE][MAX_FILE_LENGTH])
{
    int result = 0;
    FILE *fp =NULL;
    fp = fopen(file_list_path,"rb");
    if(NULL ==fp)
    {
        printf("file_list_path not exist.\n");
        return result;
    }
    struct stat fileStat;
    stat(file_list_path, &fileStat);
    char * l_buffer =(char*) malloc(fileStat.st_size+10);
    memset(l_buffer,0,fileStat.st_size+10);
    unsigned int readSize = fread(l_buffer, 1,  fileStat.st_size, fp);
    int offset = 0;
    int i_move_length = 0;
    result = 0;
    for(int i = 0;i < MAX_FILE;i++)
    {
            i_move_length = readLine(l_buffer+offset, readSize- offset,l_file_list[i],MAX_FILE_LENGTH);
            if(i_move_length == NO_DATA)
            {
                break;
            }
            result++;
            offset += i_move_length;
    }
    free(l_buffer);
    if(NULL != fp)
    {
        fclose(fp);
    }
    return result;
}

/*!
  检查关键列表中的文件是否存在。
*/
bool checkKeyFileExist(int count ,char l_file_list[MAX_FILE][MAX_FILE_LENGTH])
{
    bool res = true;
    struct stat fileStat;

    memset(g_log_info,0,sizeof(g_log_info));
    for(int i = 0;i<count;i++)
    {
        if(access(l_file_list[i], R_OK))
        {
            if(res)
                strcpy(g_log_info,l_file_list[i]);
            else
            {
                strncat(g_log_info,"; ",MAX_FILE_LENGTH-strlen(g_log_info)-1);
                strncat(g_log_info,l_file_list[i],MAX_FILE_LENGTH-strlen(g_log_info)-1);
            }
            printf("Failed to access file %s  \n",l_file_list[i]);
            res = false;
        }else//检测一下文件大小是否出现空文件
        {
            stat(l_file_list[i], &fileStat);
            if(fileStat.st_size ==0 )
            {
                if(res)
                    strcpy(g_log_info,l_file_list[i]);
                else
                {
                    strncat(g_log_info,"; ",MAX_FILE_LENGTH-strlen(g_log_info)-1);
                    strncat(g_log_info,l_file_list[i],MAX_FILE_LENGTH-strlen(g_log_info)-1);
                }
                printf(" file %s size is 0 \n",l_file_list[i]);
                res = false;
            }
        }
    }
    return res;
}

void writeLogFile(void)
{
    QString m_text = QDateTime::currentDateTime().toString();
    m_text = m_text + QString("\n") + QString(g_log_info);
    printf("log:\n %s \n", m_text.toLocal8Bit().data());
    QString m_path = QString(g_mount_path) + RECOVERY_LOG_FILE;
    QFile l_file(m_path);
    if(l_file.open(QFile::ReadWrite)){
        l_file.write(m_text.toLocal8Bit().data());
        l_file.close();
    }

    return ;
}


/*!

*/
int main(int argc, char *argv[])
{

    if(argc != 3)
    {
         printf("用法: \n\trecoveryDefaultData [列表文件路径] [程序数据分区]\n");
         printf("参数:\n");
         printf("\t[列表文件路径]:如:/application/work/file_list \n");
         printf("\t[程序数据分区]:如:/nand3 注意需要第一个/ \n");

         return 0;
    }
    memset(g_mount_path,0,sizeof(g_mount_path));
    strcpy(g_mount_path,argv[2]);
    if(true == isDataMountSuccess(argv[2]))
    {
        bool res ;
        g_file_list_count = readKeyFileList(argv[1],g_file_list);
        res = checkKeyFileExist(g_file_list_count,g_file_list);
        if(res)//所有关键文件都是存在的。
            return 1;
    }
    else
    {
        strcpy(g_log_info,"mount fail");
        printf("程序数据分区已挂载失败. \n");
    }
    //指定的分区未挂载或有重要文件损坏需要恢复默认数据

#if 0
    char *l_argv[] = {(char *)"recoverDefaultData",(char *)"-qws"};
    int  l_argc =2;
    //指定的分区未挂载或有重要文件损坏需要恢复默认数据
    QApplication a(l_argc,l_argv);
    recoveryDefaultDataC w;
    w.show();
#endif

    system(RECOVERY_SH);
    writeLogFile();
    return 0;
}
