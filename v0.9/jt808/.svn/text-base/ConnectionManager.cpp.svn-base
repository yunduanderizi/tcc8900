#include <errno.h>
#include <signal.h>
#include <fstream>
#include <iostream>
#include <dbusClient.h>
#include "ConnectionManager.h"
#include "DeviceManagerDbusMessage.h"
#include "JT808DbusMessage.h"
#include "JT808DbusMessageHandler.h"
#include "LogUtility.h"
#include "ConfigManager.h"
//#define RECONNECT_TIME 55
#define RECONNECT_TIME 595
ConnectionManager *ConnectionManager::m_instance = NULL;
int ConnectionManager::connection_num= 3;
int ConnectionManager::errornum_register = 0;
int ConnectionManager::connection_bei= 0;
extern DBusClientC * g_pDbusClient;


using namespace std;
static unsigned char receiveBuffer[CONNECTION_BUFFER_LEN];

ConnectionManager *ConnectionManager::Instance()
{
    if(!m_instance)
    {
        m_instance = new ConnectionManager();
    }
    
    return m_instance;
}

void ConnectionManager::Destroy()
{
    if(m_instance != NULL)
    {
        delete m_instance;
    }
}

ConnectionManager::ConnectionManager()
{
    m_exit = true;
    b_zone = false;
    m_socket = -1;
    m_rx_tid = -1;
    m_connected = false;
    m_blocked = false;
    m_reconnect = false;
    m_connId = ENUM_CONNECTION_ID_MASTER;
    m_eventHandlers.clear();
    m_packetHandler = NULL;
    errornum_register = 0;

    if(pthread_mutex_init(&m_sendingMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "ConnectionManager::ConnectionManager: mutex init failed.");
        return;
    }
}

ConnectionManager::~ConnectionManager()
{
    printf("come in delete\n");
    Stop();
    pthread_mutex_destroy(&m_sendingMutex);
}

void ConnectionManager::Start()
{
    if(m_exit == true)
    {
        m_exit = false;
        pthread_create(&m_rx_tid, NULL, this->RxThreadLoop, this);
    }
    
    printf("come in exit, m_exit is %d\n", m_exit);
    return;
}

void ConnectionManager::Stop()
{
    if(m_exit != true)
    {
        printf("come in stop\n");
        ::close(m_socket);
        m_socket = -1;
        m_exit = true;
        pthread_kill(m_rx_tid, SIGUSR1);
        pthread_join(m_rx_tid, NULL);
    }

    return;
	
}

void ConnectionManager::Close()
{
    if(m_socket >= 0)
    {
//	shutdown(m_socket,SHUT_RD);
        ::close(m_socket);
        m_socket = -1;
//        m_exit = true;
    }
}

int ConnectionManager::Connect(int & m_socket, string serverAddress, unsigned int port)
{
    struct sockaddr_in to; 
    struct timeval timeout;
    unsigned int timeoutconfig;
    fd_set fds;
    int ret;
    int flags;
    int error;

    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Connect entry, server %s:%d.", serverAddress.c_str(), port);
    
    m_socket = socket(AF_INET,SOCK_STREAM,0);
    if( -1 == m_socket )
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "ConnectionManager::Connect create socket failed.");
        return -1;
    }
    
    flags=fcntl(m_socket,F_GETFL,0);
    fcntl(m_socket,F_SETFL,flags|O_NONBLOCK);

    to.sin_family=AF_INET;
    to.sin_addr.s_addr=inet_addr(serverAddress.c_str());
    to.sin_port = htons(port);
    
    ret = connect(m_socket, (struct sockaddr *)&to, sizeof(to));
    if(ret < 0)
    {
        if(errno != EINPROGRESS)
        {
            LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::Connect connect main server failed.");
            Close();
            return -1;
        }
        else
        {
            FD_ZERO(&fds);
            FD_SET(m_socket,&fds);

            timeoutconfig = ConfigManager::GetTcpConnectTimout();
            timeout.tv_sec = timeoutconfig;
            timeout.tv_usec = 0;
   
            switch (select(m_socket + 1, NULL, &fds, NULL, &timeout))
            {
                case -1:
                    if(errno == EINTR)
                    {
                        LogUtility::Log(LOG_LEVEL_INFO, "ConnectionManager::Connect got a signal.");
                        Close();
                        return 1;
                    }
                    else
                    {
                        LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::Connect select failed.");
                        Close();
                        return -1;
                    }
                case 0:
                    LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::Connect not ok.");
                    Close();
                    return -1;
                default:
                    error = 0;
                    socklen_t len = sizeof(int);
                    if (( 0 == getsockopt(m_socket,SOL_SOCKET,SO_ERROR,&error,&len) ))
                    {  
                        if( 0 == error )
                        {
                            LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Connect ok.");
                        }
                        else
                        {
                            LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::getsockopt get error.");
                            Close();
                            return -1;
                        }
                    }
                    else
                    {
                        LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::getsockopt failed.");
                        Close();
                        return -1;
                    }
                    break;
            }
        }
    }
    else
    {
        LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::Connect already.");
    }

    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Connect exit.");
    
    return 0;
}

int ConnectionManager::ReadMessage()
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::ReadMessage.");
    int ret = recv(m_socket, receiveBuffer, CONNECTION_BUFFER_LEN, 0);
   // send(m_socket_mine, receiveBuffer, ret, 0);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::ReadMessage recv failed.");
        return -1;
    }
    else if(ret == 0)
    {
        LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::ReadMessage get no data.");
        return -1;
    }
    
    if(m_packetHandler != NULL)
    {
        for(int i = 0; i < ret; i ++)
        {
            printf("%02x ", receiveBuffer[i]);
            if((i + 1) % 16 == 0)
            {
                printf("\r\n");
            }
	    fflush(stdout);
        }

        m_packetHandler->PacketHandler(receiveBuffer, ret);
    }
    return 0;
}

int ConnectionManager::Select()
{
    fd_set fds;
    fd_set wfds;
    int max_fd;
    int ret;

    while(!m_exit && ConfigManager::m_control&&(errornum_register < 3))
    {
	if(m_socket == -1)
		return -1;
        FD_ZERO(&fds);
        FD_SET(m_socket, &fds);
        max_fd = m_socket;
        if(m_blocked)
        {
            FD_ZERO(&wfds);
            FD_SET(m_socket, &wfds);
            ret = select(max_fd +1, &fds, &wfds, NULL, NULL);
        }
        else
        {
            ret = select(max_fd +1, &fds, NULL, NULL, NULL);
        }
                LogUtility::Log(LOG_LEVEL_ERROR, "ConnectionManager::Select select %d.",ret);
        if(ret > 0)
        {
            if(m_blocked && FD_ISSET(m_socket, &wfds))
            {
                Notify(ENUM_CONNECTION_STATE_UNBLOCKED);
                m_blocked = false;
            }
            if(FD_ISSET(m_socket, &fds))
            {
                ret = ReadMessage();
                if(ret < 0)
                {
                    return -1;
                }
            }
        }
        else if(ret < 0)
        {
            if(errno == EINTR)
            {
                LogUtility::Log(LOG_LEVEL_INFO, "ConnectionManager::Select got a signal.");
                if(m_reconnect)
                {
                    LogUtility::Log(LOG_LEVEL_INFO, "ConnectionManager::Select reconnect.");
                    m_reconnect = false;
                    return -1;
                }
                continue;
            }
            else
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "ConnectionManager::Select select failed.");
                return -1;
            }
        }else
                LogUtility::Log(LOG_LEVEL_ERROR, "ConnectionManager::Select select %d.",ret);
    }
    
    return 0;
}

void ConnectionManager::WaitInterval()
{
    struct timeval interval;
    interval.tv_sec = ConfigManager::GetReconnectInterval();
    interval.tv_usec = 0;
    
    select(0, NULL, NULL, NULL, &interval);
    return;
}

static void SignalHandler(int sig)
{
    pthread_t pth = pthread_self();
    LogUtility::Log(LOG_LEVEL_INFO, "+++%d::SignalHandler catch signal 0x%x.", pth, sig);
}

void *ConnectionManager::RxThreadLoop(void *arg)
{
    int ret;
    string serverAddress;
    int port;
    int num;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::RxThreadLoop entry.");
    //ConnectionManager *manager = (ConnectionManager *)arg;
    ConnectionManager *manager = reinterpret_cast<ConnectionManager *> (arg);
    
    signal(SIGUSR1,SignalHandler);
    signal(SIGPIPE,SignalHandler);
    

    while((!manager -> m_exit) && (ConnectionManager::Instance()->b_zone == false))
    {
   // manager->Connect(manager->m_socket_mine, "42.96.146.34", 6000);
	if(ConfigManager::m_control == 2 || ConfigManager::m_control == 3){
			manager->m_connId = ENUM_CONNECTION_ID_IC;
			if(ConfigManager::m_control == 2){
				serverAddress = ConfigManager::m_icmainServerAddress;
				port = ConfigManager::m_ictcpPort;
			}else{
			//	serverAddress = "jr1.gghypt.net";
				serverAddress = "114.242.194.226";
				port = 8899;
			}

			manager->Notify(ENUM_CONNECTION_STATE_CONNECTTING);
			ret = manager->Connect(manager->m_socket, serverAddress, port);
			if(ret < 0)
			{
				LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect to IC failed.");
				manager->WaitInterval();
				LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop try again.");
				continue;
			}
			else if(ret > 0)
			{
				LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect return by signal.");
				continue;
			}

	}else if(ConfigManager::m_control == 1){
		if(connection_bei <= 1)
		{
			if(connection_bei == 1) connection_bei++; 
			manager->m_connId = ENUM_CONNECTION_ID_MASTER;
			serverAddress = ConfigManager::GetMainServerAddress();
			port = ConfigManager::GetTcpPort();
			manager->Notify(ENUM_CONNECTION_STATE_CONNECTTING);
			for(num=0; num< connection_num; num++)
			{
				ret = manager->Connect(manager->m_socket, serverAddress, port);
				if( ret>= 0)
					break;
				else{
					LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect to secondary failed.");
					manager->WaitInterval();
					LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop try again.");
					continue;
				}
			}
			if(ret < 0)
			{
				LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect to main failed.");

				manager->m_connId = ENUM_CONNECTION_ID_SECONDARY;
				serverAddress = ConfigManager::GetSecondaryServerAddress();
				port = ConfigManager::GetTcpPort();
				manager->Notify(ENUM_CONNECTION_STATE_CONNECTTING);
				for(num=0; num< connection_num; num++)
				{
					ret = manager->Connect(manager->m_socket, serverAddress, port);
					if(ret < 0)
					{
						LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect to secondary failed.");
						manager->WaitInterval();
						LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop try again.");
						continue;
					}
					else if(ret > 0)
					{
						LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect return by signal.");
						continue;
					}
				}
#if 0
				if(ret != 0)
				{
					RebootMachineReqC   rrequest;
					DBusReplyC  *reply=NULL;
					ret = g_pDbusClient->sendService(&rrequest,reply,5*1000);
					if(ret < 0)
					{
						printf("Send Service Error. Error Code: %d\n",ret);
					}
				}
#endif
			}
		}else if( connection_bei > 1)
		{
			LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect to main failed.");

			manager->m_connId = ENUM_CONNECTION_ID_SECONDARY;
			serverAddress = ConfigManager::GetSecondaryServerAddress();
			port = ConfigManager::GetTcpPort();
			manager->Notify(ENUM_CONNECTION_STATE_CONNECTTING);
			ret = manager->Connect(manager->m_socket, serverAddress, port);
			connection_bei = 0; 
			if(ret < 0)
			{
				LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect to secondary failed.");
				manager->WaitInterval();
				LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop try again.");
				continue;
			}
			else if(ret > 0)
			{
				LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect return by signal.");
				continue;
			}
		}
	}else{
		manager->m_connId = ENUM_CONNECTION_ID_ASSIGN;
		serverAddress = ConfigManager::m_controlServerAddress;
		port = ConfigManager::m_controlTcpPort;
		manager->Notify(ENUM_CONNECTION_STATE_CONNECTTING);
		ret = manager->Connect(manager->m_socket, serverAddress, port);
		if(ret < 0)
		{
			LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect to Assign failed.");
			manager->WaitInterval();
			LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop try again.");
			continue;
		}
		else if(ret > 0)
		{
			LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect return by signal.");
			continue;
		}
	}
        
        manager->m_connected = true;
        manager->Notify(ENUM_CONNECTION_STATE_CONNECTED);
        
        manager->Select();

       // ::close(manager->m_socket_mine);
       // manager->m_socket_mine = -1;
        manager->Notify(ENUM_CONNECTION_STATE_DISCONNECTED);
        manager->m_connected = false;

        manager->Close();
	if(errornum_register > 2){
		errornum_register = 0;
		sleep(RECONNECT_TIME);
	}
    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::RxThreadLoop exit.");
    
    return NULL;
}

int ConnectionManager::Send(const unsigned char *buf, int len)
{
    int ret;
    int totalSent=0;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Send %d.", len);
if(!( ConnectionManager::Instance()-> m_connId == ENUM_CONNECTION_ID_IC ))
{
    if(m_exit)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Send already exit.");
        return -1;
    }

    if(m_blocked)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Send blocked.");
        return -1;
    }

    if(!m_connected)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Send not connected.");
        return -1;
    }
}
    
    for(int i = 0; i < len; i ++)
    {
        printf("%02x ", buf[i]);
        if((i + 1) % 16 == 0)
        {
            printf("\r\n");
        }
    }
    
    pthread_mutex_lock(&m_sendingMutex);
    do{
	   // send(m_socket_mine, buf+totalSent, len-totalSent, 0);
	    ret = send(m_socket, buf+totalSent, len-totalSent, 0);
	    if(ret < 0)
	    {
		    if((errno == EAGAIN) || (errno == EWOULDBLOCK))
		    {
			    m_blocked = true;
			    pthread_kill(m_rx_tid, SIGUSR1);
			    pthread_mutex_unlock(&m_sendingMutex);
			    return 0;
		    }
		    else
		    {
			    pthread_mutex_unlock(&m_sendingMutex);
			    return -1;
		    }
	    }
	    else if(ret == 0)
	    {
		    m_blocked = true;
		    pthread_kill(m_rx_tid, SIGUSR1);
		    pthread_mutex_unlock(&m_sendingMutex);
		    return 0;
	    }
	    else
	    {
		    printf("\n%d:::Current Packet need translate %d; infact translate %d packets\n",__LINE__,len,ret);
		    totalSent += ret;
	    }
    }while(totalSent < len);
    pthread_mutex_unlock(&m_sendingMutex);
    return totalSent;
}

void ConnectionManager::Reconnect()
{
    m_reconnect = true;
    ConfigManager::Refresh();
    ::close(m_socket);
	m_socket = -1;
    int kill_rc=pthread_kill(m_rx_tid, SIGUSR1);
    if(kill_rc==ESRCH)
    {
	    LogUtility::Log(LOG_LEVEL_DEBUG, "this pthread not exit or already quit.");
    }
    else if(kill_rc==EINVAL)
    {
    	    LogUtility::Log(LOG_LEVEL_DEBUG,"signal is invalid.");
    }
    else
	    LogUtility::Log(LOG_LEVEL_DEBUG,"this pthread is still alive.");

}

void ConnectionManager::Notify(ENUM_CONNECTION_EVENT event)
{
    for(set<ConnectionEventHandlerInterface *>::iterator it = m_eventHandlers.begin(); it != m_eventHandlers.end(); it ++)
    {
        ConnectionEventHandlerInterface *handler = *it;
        handler->ConnectionEventHandler(event, m_connId);
    }
    return;
}
