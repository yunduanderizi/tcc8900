#include "LogUtility.h"
#include "ConfigManager.h"
#include "Utility.h"
#include "JT808DbusMessage.h"
#include "JT808DbusMessageHandler.h"
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <pthread.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <signal.h>

#define BUFFER_LEN  256


static bool exit_flag = false;
static unsigned char buffer[BUFFER_LEN];

static void SignalHandler(int sig)
{
    LogUtility::Log(LOG_LEVEL_INFO, "SignalHandler catch signal 0x%x.", sig);
}

void ReceiveMessage(int fd)
{
    int max_fd;
    fd_set fdset;
    int ret;
    int i;

    while(!exit_flag)
    {
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        max_fd = fd;
        ret = select(max_fd +1, &fdset, NULL, NULL, NULL);
        if(ret > 0)
        {
            if(FD_ISSET(fd, &fdset))
            {
                LogUtility::Log(LOG_LEVEL_INFO, "Got message.");
                int len = recv(fd, buffer, BUFFER_LEN, 0);
                if(len > 0)
                {
                    cout << "Buffer(" << len << "):" << endl;
                    for(i = 0; i < len; i ++)
                    {
                        cout << hexToString(buffer[i]).c_str() << " ";
                        if((i + 1) % 16 == 0)
                        {
                            cout << endl;
                        }
                    }
                    if((i % 16) != 0)
                    {
                        cout << endl;
                    }
                    
                    unsigned char buf[20] = {0x7e, 0x80, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x88, 0x7e};
                    send(fd, buf, 20, 0);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }
    
    return;
}

void *ThreadLoop(void *)
{
    int fd, realfd;
    int max_fd;
    int ret;
    int len, local_port;
    struct sockaddr_in my_addr;
    struct sockaddr client_addr;
    fd_set fdset;

    signal(SIGUSR1,SignalHandler);
    signal(SIGPIPE,SignalHandler);

    while(!exit_flag)
    {
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if(fd < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "Create socket failed.");
            break;
        }
        
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(ConfigManager::GetTcpPort());
        my_addr.sin_addr.s_addr = INADDR_ANY;

        bzero(&(my_addr.sin_zero),8);
        if (bind(fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "Bind failed.");
            close(fd);
            break;
        }
        
        if(listen(fd, 1) == -1)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "Listen failed.");
            close(fd);
            break;
        }
        
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        max_fd = fd;
        ret = select(max_fd +1, &fdset, NULL, NULL, NULL);
        if(ret > 0)
        {
            if(FD_ISSET(fd, &fdset))
            {
                LogUtility::Log(LOG_LEVEL_INFO, "Connected.");
                len = sizeof(client_addr);
                realfd=accept(fd,(struct sockaddr *)&client_addr,(socklen_t *)&len);
                if(realfd < 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "Accept failed.");
                    close(fd);
                    break;
                }

                len = sizeof(my_addr);
                if(getsockname(realfd, (sockaddr *)&my_addr, (socklen_t *)&len) < 0)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "Getsockname failed.");
                    close(fd);
                    close(realfd);
                    break;
                }
                
                local_port = (int)ntohs(my_addr.sin_port);
                LogUtility::Log(LOG_LEVEL_INFO, "Local port(%d).", local_port);
                ReceiveMessage(realfd);
                close(fd);
                close(realfd);
                continue;
            }
        }
        else
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "Select failed.");
            close(fd);
            break;
        }
    }
    
    return NULL;
}

DBusServerC    *g_pDbusServer;

int main(int , char **)
{
    pthread_t m_tid;
    
    ConfigManager::Refresh();

    pthread_create(&m_tid, NULL, ThreadLoop, NULL);
    
    getchar();
    
    exit_flag = true;
    pthread_kill(m_tid, SIGUSR1);
    pthread_join(m_tid, NULL);
    
    return 0;
}