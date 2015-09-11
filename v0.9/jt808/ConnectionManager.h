#ifndef _CONNECTION_MANAGER_H_
#define _CONNECTION_MANAGER_H_

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <set>
#include <map>
#include <sstream>

using namespace std;

#define CONNECTION_BUFFER_LEN       4096

typedef enum
{
    ENUM_CONNECTION_STATE_CONNECTTING = 0x01,
    ENUM_CONNECTION_STATE_CONNECTED = 0x02,
    ENUM_CONNECTION_STATE_UNBLOCKED = 0x03,
    ENUM_CONNECTION_STATE_DISCONNECTED = 0x04
} ENUM_CONNECTION_EVENT;

typedef enum
{
    ENUM_CONNECTION_ID_MASTER = 0x01,
    ENUM_CONNECTION_ID_SECONDARY = 0x02,
    ENUM_CONNECTION_ID_ASSIGN= 0x03,
    ENUM_CONNECTION_ID_IC= 0x04,
    ENUM_CONNECTION_ID_QREGISTER= 0x05
} ENUM_CONNECTION_ID;

class ConnectionEventHandlerInterface
{
public:
    virtual int ConnectionEventHandler(ENUM_CONNECTION_EVENT state, ENUM_CONNECTION_ID conn) = 0;
};

class ConnectionPacketHandlerInterface
{
public:
    virtual int PacketHandler(unsigned char *buf, int len) = 0;
};

class ConnectionManager
{
public:
    ~ConnectionManager();
    void Start();
    void Close();
    void Stop();
    static ConnectionManager *Instance();
    static void Destroy();
    int Send(const unsigned char *buf, int len);
    void Reconnect();
    void AddEventHandler(ConnectionEventHandlerInterface * handler) { if(handler) { m_eventHandlers.insert(handler); } }
    void SetPacketHandler(ConnectionPacketHandlerInterface *handler) { m_packetHandler = handler; }

    static int connection_bei;
    bool m_connected;
    ENUM_CONNECTION_ID m_connId;
    bool b_zone;
    static int errornum_register;
protected:
    ConnectionManager();
    
private:
    static void *RxThreadLoop(void *arg);
    int Connect(int &m_socket, string serverAddress, unsigned int port);
    int ReadMessage();
    int Select();
    void WaitInterval();
    void Notify(ENUM_CONNECTION_EVENT event);
    ConnectionManager(const ConnectionManager &);
    ConnectionManager & operator=(ConnectionManager &);

    bool m_exit;
    pthread_t m_rx_tid;
    pthread_mutex_t m_sendingMutex;
    int m_socket;
    int m_socket_mine;
    bool m_blocked;
    bool m_reconnect;
    static ConnectionManager *m_instance;
    static int connection_num;
    set<ConnectionEventHandlerInterface *>m_eventHandlers;
    ConnectionPacketHandlerInterface *m_packetHandler;
};
#endif
