#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <set>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

#define DEFAULT_MAIN_SERVER_ADDRESS             "127.0.0.1"
#define DEFAULT_SECONDARY_SERVER_ADDRESS        "127.0.0.1"
#define DEFAULT_TCP_PORT                        1024
#define DEFAULT_TCP_CONNECT_TIMEOUT             30
#define DEFAULT_RECONNECT_INTERVAL              30
#define DEFAULT_HEART_BEAT_INTERVAL             60
#define DEFAULT_TCP_RETRY_TIMEOUT               30
#define DEFAULT_TCP_RETRY_TIMES                 3

typedef struct
{
    string name;
    int col;
    string defaultValue;
    string *value;
} StruStringConfigItem;

typedef struct
{
    string name;
    int col;
    int defaultValue;
    int *value;
} StruIntConfigItem;

class ConfigManager
{
public:
    static void Refresh();
    static string GetMainServerAddress() { return m_mainServerAddress; }
    static string GetSecondaryServerAddress() { return m_secondaryServerAddress; }
    static int GetTcpPort() { return m_tcpPort; }
    static int GetTcpConnectTimout() { return m_tcpConnectTimeout; }
    static int GetReconnectInterval() { return m_reconnectInterval; }
    static int GetHeartBeatInterval() { return m_heartBeatInterval; }
    static int GetTcpRetryTimeout() { return m_tcpRetryTimeout; }
    static int GetTcpRetryTimes() { return m_tcpRetryTimes; }
    static bool GetHelloFlag() { return m_helloFlag; }
    static bool GetAuthFlag() { return m_authFlag; }
    
    static void SetHelloFlag(bool helloFlag) { m_helloFlag = helloFlag; }
    static void SetAuthFlag(bool authFlag) { m_authFlag = authFlag; }
    static int SetControl(string &auth, string &address, int tcpPort, int controlTime, int control)
    {
        m_control = control;
        if(m_control)
        {
            m_controlServerAddress = m_mainServerAddress;
            m_controlTcpPort = m_tcpPort;
        }
        else
        {
            m_controlServerAddress = address;
            m_controlTcpPort = tcpPort;
            m_controlTime = controlTime;
	    m_auth=auth;
        }
        
        return 0;
    }
    static int m_tcpPort;
    static int m_tcpConnectTimeout;
    static int m_reconnectInterval;
    static int m_heartBeatInterval;
    static int m_tcpRetryTimeout;
    static int m_tcpRetryTimes;
    static int m_control; 
    static string m_auth;
    static string m_controlServerAddress;
    static int m_controlTcpPort;

    static string m_icmainServerAddress;
    static int m_ictcpPort;
private:
    static string m_mainServerAddress;
    static string m_secondaryServerAddress;
    
    static int m_controlTime;
    
    static bool m_helloFlag;
    static bool m_authFlag;
    
    const static StruStringConfigItem m_stringItems[];
    const static StruIntConfigItem m_intItems[];
    
    friend ostream& operator <<(ostream& os, const ConfigManager& table);
};



#endif
