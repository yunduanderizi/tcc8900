#include <boost/program_options.hpp>
#include <iostream>
#include <dbusClient.h>
#include <signal.h>
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
#include "JT808GnssManager.h"
#include "phoneMessage.h"
//
namespace po = boost::program_options;

using namespace std;


static int socket_recvfd;
static int socket_gps;
static int gnss_flag = 0;
//int begin_gnss =0;
//int inter_gnss =0;
int  wire_state =0;
int  mangqu_flag = 0; 
bool mqic_flag = false;
struct sockaddr_in addr_recv;
struct sockaddr_in addr_gps;

JT808MessageHandler jt808;
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
		if (vm.count("log")) { cout << "log level was set to " 
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

#if 1
#define MAX_PACKET 10240
struct LocStatusData locData[MAX_PACKET];
static int  mangqu=0;
void *gps_recv_thread(void *param)
{
	int n, i=0, m=0;
	struct sockaddr client;
	int len = sizeof(struct sockaddr);
	while(1)
	{
		n = recvfrom(socket_gps, &locData[i], sizeof(LocStatusData), 0, &client, (socklen_t *)&len);
      		printf("fei ------------------------------n = %d\n", n);
		if(n > 0)
		{    
			if((ConnectionManager::Instance()->b_zone == false)&& i>0)     // Communication start
			{
              
                              mangqu = 0;
				JT808MessageHelper::SendLocationbypass(locData[i]);  //0200
#ifdef GZTESTPLAT
				JT808MessageHelper::SendLocationMultiGZ(locData, i);	    
#else	
				JT808MessageHelper::SendLocationMulti(locData, i);  //0704
#endif
				i=0;
			}
			else if((ConnectionManager::Instance()->b_zone == true) &&mangqu_flag == 1)  //Communication close
			{
				printf("fei----------------------0 mangqu== %d\n",mangqu);
              
				if(mangqu == 0)
				{
                    
                    			JT808MessageHelper::SendLocationbypass(locData[0]);
                   			 sleep(1);
					ConnectionManager::Instance()->Stop();
					mangqu = 1;
					printf("fei----------------------1  mangqu== %d\n",mangqu);
                    			continue;
				}
				i++;
               		printf("i------------------------- %d\n",i);
				if(i >= MAX_PACKET-1)
				{
#ifdef GuangZhou
		   			  JT808MessageHelper::SendLocationMultiGZ(locData, i);
					  i = 0;
 #else		 
                    			JT808MessageHelper::SendLocationMulti(locData, i);
					i = 0;
#endif
				}	
			}
			else
			{
				printf("fei----------------------2  mangqu== %d\n",mangqu);
                 
               		 if(mangqu == 0)
				{

                   			 JT808MessageHelper::SendLocationbypass(locData[0]);

					if(gnss_flag == 1)
						g_JT808GnssManager.SetGnssUp( ConfigManager::m_iGNSSUpLoadMode, ConfigManager::m_iGNSSUpLoadSet ); 
				}  
			}
		}
	}
	return NULL;
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
void PhoneSMSToSetupNotifyHandler(DBusNotifyC *pNotify)
{
	if(pNotify->getIdentifier()!=NOTF_PHONE_SMS_TO_SETUP)
		return ;
	PhoneSMSToSetupNotifyC *Notify= (PhoneSMSToSetupNotifyC *)pNotify;
	VDRRunParaTable *table = new VDRRunParaTable();
	int timeout;
	if(Notify->success)
	{
		table->GetIntValue(VDR_PARA_HEART_BEAT_INTERVAL,timeout);
		jt808.ModifyHelloTimer(timeout);
	}
	delete table;
	return;
}
void CNaviElectronicFenceInorOutNotifyHandler(DBusNotifyC *pNotify)
{
	int gnss_tmp ;
	if(pNotify->getIdentifier() != DEVICE_MANAGER_NOTF_ID_FENCE_INOUT)
		return;
	DeviceFenceInOutDbusNotifyC *Notify= (DeviceFenceInOutDbusNotifyC *)pNotify;
	printf("CNaviElectronicFenceInorOutNotifyHandler Notify->gnss_flag=%d",Notify->gnss_flag );
	if(Notify->flag == 1) // 进区域
	{
		if(Notify->gnss_flag == 1)    // gnss  采集
		{
			//gnss_tmp = 2;
			g_JT808GnssManager.SetGnssUp( ConfigManager::m_iGNSSUpLoadMode, ConfigManager::m_iGNSSUpLoadSet );
		}
		else    //gnss  不采集 
		{
			gnss_tmp = 0;
			gnss_flag = 0;
			g_JT808GnssManager.SetGnssUp( gnss_tmp, ConfigManager::m_iGNSSUpLoadSet );

		}		
		if((Notify->cm_flag == 0)&&(ConnectionManager::Instance()->b_zone == false))
		{
			ConnectionManager::Instance()->b_zone = true;
			mangqu_flag = 1;
			mqic_flag = true;
		}
	}
	else
	{     
		gnss_flag = 1;
		mangqu_flag = 0;
		mqic_flag = false;
		if((Notify->cm_flag == 1)&&(ConnectionManager::Instance()->b_zone == true))
		{
			ConnectionManager::Instance()->b_zone = false;
			ConnectionManager::Instance()->Start();	
		}	   
	}

	return;
}
#endif
static void SignalHandler(int sig)
{
	pthread_t pth = pthread_self();
	LogUtility::Log(LOG_LEVEL_INFO, " ---%d::SignalHandler catch signal 0x%x.", pth, sig);
}
DBusClientC    *g_pDbusClient;
DBusServerC    *g_pDbusServer;
DBusClientProxyC *sp_ClientGps;
DBusClientProxyC *sp_ClientPhone;

//JT808MessageHandler jt808;
int main(int argc, char *argv[])
{
	if(ProcessOptions(argc, argv) != 0)
	{
		return 1;
	}

	DBusApplicationC app(argc, argv);

	signal(SIGUSR1,SignalHandler);
	signal(SIGPIPE,SignalHandler);
	sp_ClientGps = new DBusClientProxyC((char*)"com.YunTu.Navi", (char*)"/Navi");
	sp_ClientGps->registerReply(REP_ID_NAVI_COMMON,CNaviCommonRepC::factory);//通用应答
	if(sp_ClientGps->connect()<0)
	{
		printf("Failed to connect to DBUS server.\r\n");
		//return -1;
	}
	sp_ClientPhone = new DBusClientProxyC(ServerPhone, ObjectPhone);
	sp_ClientPhone->registerNotify(NOTF_PHONE_SMS_TO_SETUP,PhoneSMSToSetupNotifyHandler,PhoneSMSToSetupNotifyC::factory);
	if(sp_ClientPhone->connect()<0)
	{
		printf("Failed to connect to DBUS server.\r\n");
	}

	printf("---------------------------------------------------------------\r\n");

	DBusServerC server(JT808_DBUS_SERVER_ID,JT808_DBUS_SERVER_OBJ);

	g_pDbusServer =&server;

	server.registerService(JT808_DBUS_REQ_ID_REGISTER,JT808RegisterServiceHandler,JT808RegisterDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_AUTHENTICATION,JT808AuthenticationServiceHandler,JT808AuthenticationDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_LOC_REPORT,JT808LocReportServiceHandler,JT808LocReportDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_ANSWER,JT808AnswerServiceHandler,JT808AnswerDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_SETTING_MODIFY,JT808ModifySettingServiceHandler,JT808ModifySettingReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_DRIVER_INFO,JT808DriverInfoServiceHandler,JT808DriverInfoReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_CAMERA,JT808CameraServiceHandler,JT808CameraDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_EVENTR,JT808EventServiceHandler,JT808EventReportDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_VIDEO,JT808VideoServiceHandler,JT808VideoDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_RECORD,JT808RecordServiceHandler,JT808RecordDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_DATA_TRANSMIT,JT808TransmissionDataServiceHandler,JT808DataTransmitReqC::factory); 

	if(server.connect()<0)
	{
		cerr << "Failed to connect to DBUS server.\r\n";
		return -1;
	}

	DBusClientC client(DEVICE_MANAGER_DBUS_SERVER_ID, DEVICE_MANAGER_DBUS_SERVER_OBJ);
	g_pDbusClient= &client;
	g_pDbusClient->registerNotify(DEVICE_MANAGER_NOTF_ID_FENCE_INOUT,CNaviElectronicFenceInorOutNotifyHandler,DeviceFenceInOutDbusNotifyC ::factory);

	if(g_pDbusClient->connect()<0)
	{
		cerr << "Failed to connect to DBUS client.\r\n";
		return -1;
	}

	can_socket_recv();
	//gnss_socket_recv();
	if( !g_JT808GnssManager.Init() )
	{
		cerr << "UpLoadManager::Init Failed\r\n";
		return -1;
	}
	gps_socket_recv();


	ConnectionManager::Instance()->Start();

	return app.loop();
}
