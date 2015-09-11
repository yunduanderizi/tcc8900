 #include "Utility.h"
 #include "ConfigManager.h"
 #include "VDRRunParaTable.h"

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
    {"ICTcpPort",              VDR_PARA_IC_MAIN_SERVER_TCP_PORT,                  DEFAULT_TCP_PORT,     &ConfigManager::m_ictcpPort}
};

void ConfigManager::Refresh()
{
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
