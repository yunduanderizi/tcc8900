#include "voiceMessage.h"
#include "dbusMessage.h"

void UiToVoiceRegisterReqC::importData(const char* pServerName, const char *pObjectName, const char* pProcessName)
{
	memset(m_ServerName, 0, NAME_LENGTH_MAX+1);
	memset(m_ObjectName, 0, NAME_LENGTH_MAX+1);
	memset(m_ProcessName, 0, NAME_LENGTH_MAX+1);

	strncpy(m_ServerName, pServerName, NAME_LENGTH_MAX);
	strncpy(m_ObjectName, pObjectName, NAME_LENGTH_MAX);
	strncpy(m_ProcessName, pProcessName, NAME_LENGTH_MAX);
}