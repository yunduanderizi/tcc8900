#include "JT808GnssManager.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <dbusClient.h>
#include <signal.h>
#include <queue>
#include "LogUtility.h"
#include "ConfigManager.h"
#include "ConnectionManager.h"
#include "JT808MessageHandler.h"
#include "JT808MessageHelper.h"
#include "JT808DbusMessage.h"
#include "JT808DbusMessageHandler.h"
#include "DeviceManagerDbusMessage.h"
#include "../deviceManager/devicemanager.h"
#include "../wm/dbusClientProxy.h"
#include "NaviDbusMessage.h"

using namespace std;
extern int mangqu;
struct SendDate
{
    int type;
    int total;
    unsigned char *buff;
};

SendDate senddate;
queue <SendDate> GnssSendQueue;

JT808GnssManager g_JT808GnssManager;
pthread_mutex_t GnssDateMutex;
unsigned long GetTickCount()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
void *GnssPorc(void *param)
{
    g_JT808GnssManager.GnssProc();
    return NULL;
}

void *GnssSendDate(void *param)
{
    g_JT808GnssManager.GnssSend();
    return NULL;
}

JT808GnssManager::JT808GnssManager()
{
    m_gnssupmode = 0;
    m_gnssupset = 0;
}
JT808GnssManager::~JT808GnssManager()
{

}
bool JT808GnssManager::Init()
{
    JT808GnssManager::SetGnssUp(ConfigManager::m_iGNSSUpLoadMode,ConfigManager::m_iGNSSUpLoadSet);
    int port =  1012;
    bzero(&m_addr_gnss, sizeof(m_addr_gnss));
    m_addr_gnss.sin_family = AF_INET;
    m_addr_gnss.sin_addr.s_addr = inet_addr("127.0.0.1");
    m_addr_gnss.sin_port = htons(port);

    m_socket_dp = socket(AF_INET, SOCK_DGRAM, 0);
    if(m_socket_dp == -1)
    {
        perror("socket call failed");
        return -1;
    }
    bind(m_socket_dp, (struct sockaddr*)&m_addr_gnss, sizeof(m_addr_gnss));

    pthread_t sock_recv;
    int ret = pthread_create(&sock_recv, NULL, GnssPorc, NULL);
    if(ret < 0)
    {
        printf("create handle_image error!\n");
        return ret;
    }
    pthread_detach(sock_recv);

    pthread_t sock_send;
    int ret1 = pthread_create(&sock_send , NULL , GnssSendDate , NULL);
    if(ret1 < 0)
    {
        printf("create handle_image2 error!\n");
        return ret;
    }
    pthread_detach(sock_send);
    return true;
}

void JT808GnssManager::StopGnssUp()
{
    printf("JT808GnssManager::StopGnssUp\n");
    m_gnssupmode = 0;
    m_distace = 0;
    VDRRunParaTable *vdrtable = new  VDRRunParaTable();
    vdrtable->SetIntValue(VDR_PARA_GNSS_DATA_UPLOAD_MODE,0);
    vdrtable->Commit();
    delete vdrtable;
    ConfigManager::m_iGNSSUpLoadMode=0;
}
void JT808GnssManager::SetGnssUp()
{
    m_gnssupmode = 0;
    m_gnssupset = 0;
    //	m_times = 0;
    m_distace = 0;
    m_gnsslasttime = GetTickCount();
    m_gnssbegintime = m_gnsslasttime;
}

void JT808GnssManager::SetGnssUp( int gnssupmode, int gnssupset )
{
    printf("JT808GnssManager::SetGnssUp gnssupmode=%d,gnssupset=%d\r\n",
           gnssupmode,gnssupset);
    m_gnssupmode = gnssupmode;
    m_gnssupset = gnssupset;
    //m_times = 0;
    //m_distace = 0;
    m_gnsslasttime = GetTickCount();
    m_gnssbegintime = m_gnsslasttime;
}

void JT808GnssManager::GnssProc()
{

    int n;
    struct sockaddr client;
    unsigned char buff[1024];
    int len = sizeof(struct sockaddr);
    m_times = 0;
    while(1)
    {
        n = recvfrom(m_socket_dp, buff, 1024, 0, &client, (socklen_t *)&len);
        m_gnsslasttime = GetTickCount();
        //printf("gnss m_gnssupmode=%d,m_gnssupset=%d m_gnsslasttime=%d buf length is %d\r\n",
        //       m_gnssupmode,m_gnssupset,m_gnsslasttime,n);
        //printf("%s",buff);
        //printf("gnss %d,%d,%d\r\n",n,m_gnsslasttime,m_gnssupmode,m_gnssupset);
        if( m_times < 0 )
            m_times = 0;
        m_times++;
        printf("gnss %d %d %d %d \r\n",n,m_gnssupmode,m_gnssupset,m_times);
        switch( m_gnssupmode )
        {
            //	m_gnssupset  shijian jiange
        case 0x01:
            if( m_gnssupset > 0 )
            {
                if( m_times%m_gnssupset == 0 )
                {

                    senddate.type = 0x00;
                    senddate.total = n;
                    senddate.buff = buff;
                    pthread_mutex_lock(&GnssDateMutex);
                    GnssSendQueue.push(senddate);
                    pthread_mutex_unlock(&GnssDateMutex);
                    //JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
                    printf("********************* gnsss  date  buff = %s\n", buff);
                }
            }
            else
            {
                JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
                printf("********************* gnsss  date  buff = %s\n", buff);
            }
            break;
        case 0x02:
            //按距离上??
            //printf("distance up load . no up ,default 10 mi!");
            m_distace += 3;
            if( m_distace > m_gnssupset )
            {
                printf("distance up load . no up ,default 10 mi");
                JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
                m_distace = 0;
            }
            break;
        case 0x0B:
            if( m_gnssupset > 0 )
            {
                if( m_times%m_gnssupset == 0 )
                {
                    StopGnssUp();
                }
                else
                {
                    JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
                    printf("********************* gnsss  date  buff = %s\n", buff);
                    m_times = 0;
                }
            }
            break;
        case 0x0C:
            //按距离上传，
            m_gnssupset -= 3;
            printf("distance up load . no up ,default 3 mi!");
            if( m_gnssupset > 0 )
            {
                JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
            }
            else
            {
                StopGnssUp();
            }
            break;
        case 0x0D:
            if( m_gnssupset > 0 )
            {
                m_gnssupset--;
                JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
            }
            else
            {
                StopGnssUp();
            }
            break;
        default:
            //printf("gnss gnssmode=%d,gnssmodeset=%d buf length is %d\n", m_gnssupmode,m_gnssupset,n );
            usleep(800000);
        }
        usleep(50000);
    }
    return;
}

void JT808GnssManager::GnssSend()
{
    SendDate tmp;
    while(1)
    {
        if(!GnssSendQueue.empty())
        {
            if(mangqu == 0)
            {
            	  printf("GnssSendQueue size = %d\n",GnssSendQueue.size());
                pthread_mutex_lock(&GnssDateMutex);
                tmp = GnssSendQueue.back();
                GnssSendQueue.pop();
                pthread_mutex_unlock(&GnssDateMutex);
                JT808MessageHelper::SendTransmissionData(tmp.type , tmp.total, tmp.buff);
            }
        }
        usleep(50000);
    }
    return;
}
