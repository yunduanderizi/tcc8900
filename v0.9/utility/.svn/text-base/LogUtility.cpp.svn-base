#include "LogUtility.h"

int LogUtility::m_syslogLevel = LOG_LEVEL_OFF;
int LogUtility::m_stdoutLevel = LOG_LEVEL_DEBUG;
bool LogUtility::m_first = true;
static int m_logMap[] = {LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERR};
static const char *m_logLevelName[] = {"DEBUG", "INFO", "WARN", "ERROR"}; 

void LogUtility::Log(ENUM_LOG_LEVEL level, const char *fmt, ...)
{
#ifdef DONT_PRINTF_LOG
    return ;
#endif 

    va_list args;
    
    if(m_first)
    {
        openlog("IVI", LOG_NDELAY|LOG_PID, LOG_USER);
        m_first = false;
    }
    
    va_start(args, fmt);
    
    if(level >= LOG_LEVEL_OFF)
    {
        return;
    }
    
    if(m_stdoutLevel <= (int)level)
    {
        fprintf(stderr, "[%s]", m_logLevelName[level]);
        vfprintf(stderr, fmt, args);
        fprintf(stderr, "\r\n");
    }
    
    if(m_syslogLevel <= (int)level)
    {
        vsyslog(m_logMap[level], fmt, args);
    }
    
    va_end(args);
}
