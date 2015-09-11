 #include "Utility.h"
 #include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 #include "ConfigManager.h"
 #include "VDRRunParaTable.h"
#include "JT808GnssManager.h"

string ConfigManager::m_mainServerAddress = DEFAULT_MAIN_SERVER_ADDRESS;
string ConfigManager::m_icmainServerAddress = DEFAULT_MAIN_SERVER_ADDRESS;
string ConfigManager::m_secondaryServerAddress  = DEFAULT_SECONDARY_SERVER_ADDRESS;
string ConfigManager::m_auth= "";
int ConfigManager::m_tcpPort = DEFAULT_TCP_PORT;
int ConfigManager::m_ictcpPort = DEFAULT_TCP_PORT;
int ConfigManager::m_tcpConnectTimeout = DEFAULT_TCP_CONNECT_TIMEOUT;
int ConfigManager::m_reconnectInterval = DEFAULT_RECONNECT_INTERVAL;
int ConfigManager::m_heartBeatInterval = DEFAULT_HEART_BEAT_INTERVAL;
int ConfigManager::m_tcpRetryTimeout = DEFAULT_TCP_RETRY_TIMEOUT;
int ConfigManager::m_tcpRetryTimes = DEFAULT_TCP_RETRY_TIMES;
//GNSS设置
int ConfigManager::m_iGNSSMode=1;
int ConfigManager::m_iGNSSBaud=1;
int ConfigManager::m_iGNSSOut=0;
int ConfigManager::m_iGNSSFre=0;
int ConfigManager::m_iGNSSUpLoadMode = 0;
int ConfigManager::m_iGNSSUpLoadSet = 0;
//--
int ConfigManager::m_control = 1;
string ConfigManager::m_controlServerAddress = DEFAULT_MAIN_SERVER_ADDRESS;
int ConfigManager::m_controlTcpPort = DEFAULT_TCP_PORT;
int ConfigManager::m_controlTime = 0;
bool ConfigManager::m_helloFlag = true;
bool ConfigManager::m_authFlag = true;

const StruStringConfigItem ConfigManager::m_stringItems[] = {
    {"MainServerAddress",       VDR_PARA_MAIN_SERVER_ADDRESS,       DEFAULT_MAIN_SERVER_ADDRESS,        &ConfigManager::m_mainServerAddress},
    {"SecondaryServerAddress",  VDR_PARA_SECOND_SERVER_ADDRESS,     DEFAULT_SECONDARY_SERVER_ADDRESS,   &ConfigManager::m_secondaryServerAddress},
    {"ICMainServerAddress",      VDR_PARA_IC_MAIN_SERVER_IP_ADDR,       DEFAULT_MAIN_SERVER_ADDRESS,        &ConfigManager::m_icmainServerAddress},
};

const StruIntConfigItem ConfigManager::m_intItems[] = {
    {"TcpPort",                 VDR_PARA_TCP_PORT,                  DEFAULT_TCP_PORT,                   &ConfigManager::m_tcpPort},
    {"HeartBeatInterval",       VDR_PARA_HEART_BEAT_INTERVAL,       DEFAULT_HEART_BEAT_INTERVAL,        &ConfigManager::m_heartBeatInterval},
    {"TcpRetryTimeout",         VDR_PARA_TCP_REPLY_TIMEOUT,         DEFAULT_TCP_RETRY_TIMEOUT,          &ConfigManager::m_tcpRetryTimeout},
    {"TcpRetryTimes",           VDR_PARA_TCP_RETRY_TIMES,           DEFAULT_TCP_RETRY_TIMES,            &ConfigManager::m_tcpRetryTimes},
    {"ICTcpPort",               VDR_PARA_IC_MAIN_SERVER_TCP_PORT,                  DEFAULT_TCP_PORT,    &ConfigManager::m_ictcpPort},
    {"GNSS_LOC_MODE",           VDR_PARA_GNSS_LOC_MODE,            1,                                   &ConfigManager::m_iGNSSMode},
    {"GNSS_BAUD_RATE",          VDR_PARA_GNSS_BAUD_RATE,           1,                                   &ConfigManager::m_iGNSSBaud},
    {"GNSS_DATA_OUT_RATE",      VDR_PARA_GNSS_DATA_OUT_RATE,       0,                                   &ConfigManager::m_iGNSSOut},
    {"GNSS_DATA_COLLECT_RATE",  VDR_PARA_GNSS_DATA_COLLECT_RATE,   0,                                   &ConfigManager::m_iGNSSFre},
    {"GNSS_DATA_UPLOAD_MODE",   VDR_PARA_GNSS_DATA_UPLOAD_MODE,    0,                                   &ConfigManager::m_iGNSSUpLoadMode},
    {"GNSS_DATA_UPLOAD_SET",    VDR_PARA_GNSS_DATA_UPLOAD_SET,     0,                                   &ConfigManager::m_iGNSSUpLoadSet},
};

void ConfigManager::Refresh()
{
    struct hostent *h;
    VDRRunParaTable *table = new VDRRunParaTable();
    int itemNumber = sizeof(m_stringItems)/sizeof(m_stringItems[0]);
    for(int i = 0; i < itemNumber; i ++)
    {
        const StruStringConfigItem &item = m_stringItems[i];

        if(table->GetStringValue(item.col, *(item.value)) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "ConfigManager::Refresh fail to get %s.", item.name.c_str());
            *(item.value) = item.defaultValue;
        }
            LogUtility::Log(LOG_LEVEL_ERROR, "ConfigManager::%d-----------------fail to get %s.",__LINE__, (*(item.value)).c_str());
    }
    
    itemNumber = sizeof(m_intItems)/sizeof(m_intItems[0]);
    for(int i = 0; i < itemNumber; i ++)
    {
        const StruIntConfigItem &item = m_intItems[i];

        if(table->GetIntValue(item.col, *(item.value)) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "ConfigManager::Refresh fail to get %s.", item.name.c_str());
            *(item.value) = item.defaultValue;
        }
    }
    
    delete table;
    
    m_tcpConnectTimeout = DEFAULT_TCP_CONNECT_TIMEOUT;
    m_reconnectInterval = DEFAULT_RECONNECT_INTERVAL;
    //货运平台固定地址,货运平台需要打开
    printf( "fright_JT808   m_mainServerAddress=%s m_secondaryServerAddress=%s\n",m_mainServerAddress.c_str(),m_secondaryServerAddress.c_str() );
    if( !isdigit(m_mainServerAddress.at(0)) )
    {
        //char* ip=inet_ntoa(addr);
        if((h = gethostbyname( m_mainServerAddress.c_str())) == NULL)
        {
            printf("error: %s\n", m_mainServerAddress.c_str());
            m_mainServerAddress = "210.82.111.147";
        }
        else
        {
            //(*item.value).assign(inet_ntoa(*((struct in_addr *)h->h_addr)));
            //m_mainServerAddress = "210.82.111.147";
            m_mainServerAddress = inet_ntoa(*((struct in_addr *)h->h_addr));
            printf("m_mainServerAddress: %s\n", m_mainServerAddress.c_str());
        }
    }
    if( !isdigit(m_secondaryServerAddress.at(0)) )
    {
        if((h = gethostbyname( m_secondaryServerAddress.c_str())) == NULL)
        {
            printf("error: %s\n", m_secondaryServerAddress.c_str());
            m_secondaryServerAddress = "114.242.194.226";
        }else
        {
            //(*item.value).assign(inet_ntoa(*((struct in_addr *)h->h_addr)));
           m_secondaryServerAddress = "114.242.194.226";
           m_secondaryServerAddress = inet_ntoa(*((struct in_addr *)h->h_addr));
           printf("m_secondaryServerAddress: %s\n", m_secondaryServerAddress.c_str());
        }
    }
    //m_mainServerAddress = "210.82.111.147";
    //m_secondaryServerAddress = "114.242.194.226";
    g_JT808GnssManager.SetGnssUp( ConfigManager::m_iGNSSUpLoadMode, ConfigManager::m_iGNSSUpLoadSet );
    return;
}

ostream& operator <<(ostream& os, const ConfigManager& table)
{
    int itemNumber = sizeof(table.m_stringItems)/sizeof(table.m_stringItems[0]);

    os << "ConfigManager:" << endl;

    for(int i = 0; i < itemNumber; i ++)
    {
        const StruStringConfigItem &item = table.m_stringItems[i];
        os << item.name << "=" << (*item.value) << endl;
    }
    
    itemNumber = sizeof(table.m_intItems)/sizeof(table.m_intItems[0]);
    for(int i = 0; i < itemNumber; i ++)
    {
        const StruIntConfigItem &item = table.m_intItems[i];
        os << item.name << "=" << (*item.value) << endl;
    }
    return os;
}
