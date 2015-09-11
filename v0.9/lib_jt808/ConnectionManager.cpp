#include <errno.h>
#include <signal.h>
#include "ConnectionManager.h"
#include "LogUtility.h"
#include "ConfigManager.h"

ConnectionManager *ConnectionManager::m_instance = NULL;
int ConnectionManager::connection_num= 1;
int ConnectionManager::connection_bei= 0;

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

    if(pthread_mutex_init(&m_sendingMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "ConnectionManager::ConnectionManager: mutex init failed.");
        return;
    }
}

ConnectionManager::~ConnectionManager()
{
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
    
    return;
}

void ConnectionManager::Stop()
{
    if(m_exit != true)
    {
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
    }
}

int ConnectionManager::Connect(string serverAddress, unsigned int port)
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

    while(!m_exit && ConfigManager::m_control)
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
        }
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
    LogUtility::Log(LOG_LEVEL_INFO, "SignalHandler catch signal 0x%x.", sig);
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
	if(ConfigManager::m_control == 2){
			manager->m_connId = ENUM_CONNECTION_ID_IC;
			serverAddress = ConfigManager::m_icmainServerAddress;
			port = ConfigManager::m_ictcpPort;
			manager->Notify(ENUM_CONNECTION_STATE_CONNECTTING);
			ret = manager->Connect(serverAddress, port);
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
				ret = manager->Connect(serverAddress, port);
				if( ret>= 0)
					break;
			}
			if(ret < 0)
			{
				LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect to main failed.");

				manager->m_connId = ENUM_CONNECTION_ID_SECONDARY;
				serverAddress = ConfigManager::GetSecondaryServerAddress();
				port = ConfigManager::GetTcpPort();
				manager->Notify(ENUM_CONNECTION_STATE_CONNECTTING);
				ret = manager->Connect(serverAddress, port);
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
		}else if( connection_bei > 1)
		{
			LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect to main failed.");

			manager->m_connId = ENUM_CONNECTION_ID_SECONDARY;
			serverAddress = ConfigManager::GetSecondaryServerAddress();
			port = ConfigManager::GetTcpPort();
			manager->Notify(ENUM_CONNECTION_STATE_CONNECTTING);
			ret = manager->Connect(serverAddress, port);
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
		ret = manager->Connect(serverAddress, port);
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
        
        manager->Notify(ENUM_CONNECTION_STATE_CONNECTED);
        manager->m_connected = true;
        
        manager->Select();

        manager->Notify(ENUM_CONNECTION_STATE_DISCONNECTED);
        manager->m_connected = false;

        manager->Close();
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
    pthread_kill(m_rx_tid, SIGUSR1);
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
