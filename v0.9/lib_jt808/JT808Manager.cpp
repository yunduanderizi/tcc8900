#ifndef _MANAGER_
#define _MANAGER_
#include <boost/program_options.hpp>
#include <iostream>

#include "LogUtility.h"
#include "ConfigManager.h"
#include "ConnectionManager.h"
#include "JT808MessageHandler.h"
#include "JT808MessageHelper.h"
#include "JT808ApplicationHandler.h"
#include "../deviceManager/devicemanager.h"
#include "JT808FifoMessageHandler.h"
#include "tool.h"
namespace po = boost::program_options;

using namespace std;


static int socket_recvfd;
static int socket_dp;
static int socket_gps;
static int begin_gnss =0;
struct sockaddr_in addr_recv;
struct sockaddr_in addr_gnss;
struct sockaddr_in addr_gps;
static int ProcessOptions(int argc, char *argv[])
{
    try {

        po::options_description options("Allowed options");
        options.add_options()
            ("help", "produce help message")
            ("nohello", "no hello send")
            ("noauth", "no auth send")
            ("debug", po::value<int>(), "set debug level")
            ("log", po::value<int>(), "set log level")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(argc, argv, options), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << options << "\n";
            return 1;
        }

        if (vm.count("nohello")) {
            ConfigManager::SetHelloFlag(false);
        }

        if (vm.count("noauth")) {
            ConfigManager::SetAuthFlag(false);
        }
        
        if (vm.count("debug")) {
            cout << "debug level was set to " 
                 << vm["debug"].as<int>() << ".\n";
            LogUtility::SetStdoutLevel(vm["debug"].as<int>());
        } else {
            cout << "debug level was not set.\n";
        }
        if (vm.count("log")) {
            cout << "log level was set to " 
                 << vm["log"].as<int>() << ".\n";
            LogUtility::SetLogLevel(vm["log"].as<int>());
        } else {
            cout << "log level was not set.\n";
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}


#if 1
#define MAX_PACKET 26 
struct LocStatusData locData[MAX_PACKET];
static int  mangqu=0;
void *process_device_thread(void *)
{
	unsigned char buff[512];
	int ret;
	fd_set rfd;
	CFifo *dfifo =new CFifo("fifo_to_808",0);
	while(1)
	{
		FD_ZERO(&rfd);
		FD_SET(dfifo->fd,&rfd);
		ret = select(dfifo->fd+1,&rfd,NULL,NULL,NULL);
		if(ret==-1)
		{
			error(0,1,"select return -1");
		}
		if(ret)
		{
			if(dfifo->Read(buff,PIPE_BUF - 1));
			analyze_data_to_808(buff);
				
		}
	}

}
void *gps_recv_thread(void *param)
{
#if 1
    int n, i=0;
    struct sockaddr client;
    int len = sizeof(struct sockaddr);
    while(1)
    {
	    n = recvfrom(socket_gps, &locData[i], sizeof(LocStatusData), 0, &client, (socklen_t *)&len);
	    printf("current n is %d , i is %d b_zone is %d\n", n, i, ConnectionManager::Instance()->b_zone);
	    if(n > 0){
		    if(ConnectionManager::Instance()->b_zone == true)
		    //if(1)
		    {
			    i++;
			    if(i >= MAX_PACKET-1)
			    {
				    JT808MessageHelper::SendLocationMulti(locData, i);
				    //if(mangqu == 0)
				//	    JT808MessageHelper::SendLocationMultiO();
				    mangqu = 1;
				    i=0;
			    }
		    }else if((ConnectionManager::Instance()->b_zone == false)&&(i > 1))
		    {
				    JT808MessageHelper::SendLocationMulti(locData, i);
				    mangqu = 1;
				    i=0;
		    }
		    else
		    {
			    if(mangqu == 0)
				    JT808MessageHelper::SendLocationbypass(locData[0]);
			    //locData[3]=locData[2];
			    //locData[2]=locData[1];
			    //locData[1]=locData[0];
		    }
	    }
    }
    return NULL;
#endif
#if 0
    int n;
    struct sockaddr client;
    struct LocStatusData locData;
    int len = sizeof(struct sockaddr);
    while(1)
    {
	    n = recvfrom(socket_gps, &locData, sizeof(LocStatusData), 0, &client, (socklen_t *)&len);
	    if(n > 0)
		    JT808MessageHelper::SendLocationbypass(locData);
    }
    return NULL;
#endif
}

int gps_socket_recv()
{
    int port =  8901;
    int ret;

    bzero(&addr_gps, sizeof(addr_gps));
    addr_gps.sin_family = AF_INET;
    addr_gps.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_gps.sin_port = htons(port);

    socket_gps = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_gps == -1)
    {
        perror("socket call failed");
        return -1;
    }

    bind(socket_gps, (struct sockaddr*)&addr_gps, sizeof(addr_gps));

    pthread_t sock_recv;
    ret = pthread_create(&sock_recv, NULL, gps_recv_thread, NULL);
    if(ret < 0)
    {
	    printf("create handle_image error!\n");
	    return ret;
    }
    pthread_detach(sock_recv);

    return 0;
}
#endif
void *gnss_recv_thread(void *param)
{
#if 0
	sleep(30);
	printf("}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}\n \
		}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}\n");
	    ConfigManager::m_control = 2;
	    ConnectionManager::Instance()->Close();
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n \
		}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}$\n");
#endif
    int n;
    struct sockaddr client;
    unsigned char buff[1024];
    int len = sizeof(struct sockaddr);
    while(1)
    {
	    n = recvfrom(socket_dp, buff, 1024, 0, &client, (socklen_t *)&len);
	    if(begin_gnss == 1){
		    printf("buf length is %d \n", n);
		    JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
	    }
	    else
	    {
		    printf("gnss is sleep---------%d\n",begin_gnss);
		    usleep(800000);
	    }
    }
    return NULL;
}

int gnss_socket_recv()
{
    int port =  1012;
    int ret;

    bzero(&addr_gnss, sizeof(addr_gnss));
    addr_gnss.sin_family = AF_INET;
    addr_gnss.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_gnss.sin_port = htons(port);

    socket_dp = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_dp == -1)
    {
        perror("socket call failed");
        return -1;
    }

    bind(socket_dp, (struct sockaddr*)&addr_gnss, sizeof(addr_gnss));

    pthread_t sock_recv;
    ret = pthread_create(&sock_recv, NULL, gnss_recv_thread, NULL);
    if(ret < 0)
    {
    printf("create handle_image error!\n");
    return ret;
    }
    pthread_detach(sock_recv);

    return 0;
}

void *can_recv_thread(void *param)
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    struct sockaddr client;
    int n;
    unsigned char buff[1024];

    int len = sizeof(struct sockaddr);
    while(1)
    {
		    FD_ZERO(&rfds);
		    FD_SET(socket_recvfd, &rfds);

		    tv.tv_sec = 0;
		    tv.tv_usec = 50*1000;

		    retval = select(socket_recvfd + 1, &rfds, NULL, NULL, &tv);

		    if (retval == -1)
		    {
			    perror("select()");
			    continue;
		    }  
		    else if (retval)
		    {
			    n = recvfrom(socket_recvfd, buff, 1024, 0, &client, (socklen_t *)&len);
			    printf("buf: %s \n", buff);
			    JT808MessageHelper::SendCanData(0x00 , n, buff);
		    }
		    else
			    sleep(1);
    }
    return NULL;
}

int can_socket_recv()
{
    socket_recvfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_recvfd == -1)
    {
        perror("socket call failed");
        return -1;
    }

    int port_recv = 2010;
    bzero(&addr_recv, sizeof(addr_recv));
    addr_recv.sin_family = AF_INET;
    addr_recv.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_recv.sin_port = htons(port_recv);

    bind(socket_recvfd, (struct sockaddr*)&addr_recv, sizeof(addr_recv));

    int ret = -1;
    pthread_t sock_recv;
    ret = pthread_create(&sock_recv, NULL, can_recv_thread, (void *)&addr_recv);
    if(ret < 0)
    {
    printf("create handle_image error!\n");
    return ret;
    }
    pthread_detach(sock_recv);

    return 0;
}
#if 0
void CNaviElectronicFenceInorOutNotifyHandler(DBusNotifyC *pNotify)
{
    if(pNotify->getIdentifier() != DEVICE_MANAGER_NOTF_ID_FENCE_INOUT)
        return;
    DeviceFenceInOutDbusNotifyC *Notify= (DeviceFenceInOutDbusNotifyC *)pNotify;
    if(Notify->gnss_flag == 1)
	    begin_gnss = 1;
    else
	    begin_gnss = 0;
#if 1
    if((Notify->cm_flag == 1)&&(ConnectionManager::Instance()->b_zone == true))
    {
    		ConnectionManager::Instance()->b_zone = false;
    		ConnectionManager::Instance()->Start();
    }else if((Notify->cm_flag == 0)&&(ConnectionManager::Instance()->b_zone == false))
    {
    	    ConnectionManager::Instance()->b_zone = true;
	    ConnectionManager::Instance()->Stop();
    }
#endif
    return;
}
#endif

JT808MessageHandler jt808;
void JT808_Run(void)
{
	pthread_t device_tid;

	int argc;
	char **argv;
	if(ProcessOptions(argc, argv) != 0)
	{
		return ;
	}

	CFifo *jfifo = new CFifo("fifo_to_device",1);
	jt808.SetFifotodevice(jfifo);
	

	pthread_create(&device_tid,NULL,process_device_thread,NULL);

	can_socket_recv();
	gnss_socket_recv();
	gps_socket_recv();
	ConnectionManager::Instance()->Start();
	while(1)
	{
	}
}
#endif
