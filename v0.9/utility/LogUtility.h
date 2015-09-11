#ifndef _LOG_UTILITY_H_
#define _LOG_UTILITY_H_

#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>

typedef enum
{
    LOG_LEVEL_DEBUG = 0x00,
    LOG_LEVEL_INFO = 0x01,
    LOG_LEVEL_WARN = 0x02,
    LOG_LEVEL_ERROR = 0x03,
    LOG_LEVEL_OFF = 0x04
} ENUM_LOG_LEVEL;

class LogUtility
{
public:
    static void Log(ENUM_LOG_LEVEL level, const char *fmt, ...); 
    static void SetStdoutLevel(int level)
    {
        if(level > (int)LOG_LEVEL_OFF)
        {
            m_stdoutLevel = (int)LOG_LEVEL_OFF;
        }
        else if(level < (int)LOG_LEVEL_DEBUG)
        {
            m_stdoutLevel = (int)LOG_LEVEL_DEBUG;
        }
        else
        {
            m_stdoutLevel = level;
        }
    }

    static void SetLogLevel(int level)
    {
        if(level > (int)LOG_LEVEL_OFF)
        {
            m_syslogLevel = (int)LOG_LEVEL_OFF;
        }
        else if(level < (int)LOG_LEVEL_DEBUG)
        {
            m_syslogLevel = (int)LOG_LEVEL_DEBUG;
        }
        else
        {
            m_syslogLevel = level;
        }
    }
    
private:
    static int m_syslogLevel;
    static int m_stdoutLevel;
    static bool m_first;
};
#endif
