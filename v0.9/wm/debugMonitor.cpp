#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "debugMonitor.h"
#include "debugMonitorMessage.h"

#define DEFAULTDEBUGLEVEL DEBUG_DBUS_MSG 
#define DEFAULTDEBUGBLOCKS (10*1024*1024/512)



void IBDDebugMonitorC::debugMonitorHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
	if(pRequest->getIdentifier() != REQ_ID_DEBUG_MONITOR)
		return;
	DebugMonitorReqC* debugReqData = (DebugMonitorReqC*) pRequest; 

	if(debugReqData->oper == DEBUG_OPER_CHANGE_LEVEL) 
		IBD_debugMonitor->m_level = debugReqData->debugLevel;
	else if(debugReqData->oper == DEBUG_OPER_CHANGE_FILENAME){
		fprintf(stdout, "debugMonitorHandler in DEBUG_OPER_CHANGE_FILENAME\n");
	}else{
		fprintf(stdout, "debugMonitorHandler not oper to reply client request, oper id = %d\n", debugReqData->oper);
	}
}


IBDDebugMonitorC* IBDDebugMonitorC::IBD_debugMonitor = NULL;

IBDDebugMonitorC::IBDDebugMonitorC(const char* processName, DBusServerC *debugServer)
{
	char* env = NULL;

	m_serialNo = 0;

	filePathNamelen = 1;
	strcpy(filePathName,"./logs");

	fpConsole = NULL;
	fp = NULL;

	m_level   = DEFAULTDEBUGLEVEL;	//default debug message all
	maxBlocks = DEFAULTDEBUGBLOCKS;	//default block is 10;

	if(debugServer != NULL)
		debugServer->registerService(REQ_ID_DEBUG_MONITOR, debugMonitorHandler, DebugMonitorReqC::factory);
	

	pthread_mutex_init(&debugLock, NULL);

	dbusdebug.init(processName);
	
	env = getenv("DEBUG_LEVEL");
	if(env != NULL)
	{
		m_level = atoi(env);
	}

	env = getenv("DEBUG_BLOCKS");
	if(env != NULL)
	{
		maxBlocks = atoi(env);
	}

	
	env = getenv("DEBUG_PATH");
	if(env != NULL && strlen(env)<FILEPATHNAMESIZE-50)
	{
		strcpy(filePathName, env);
	}
	else if(env!=NULL)
	{
		::printf("DEBUG_PATH is too long: PATH=%s\n",env);
	}

	time_t dayTime;
	time(&dayTime);
	struct tm	timeStruct;
	gmtime_r(&dayTime, &timeStruct);

	sprintf(filePathName+strlen(filePathName), "/%s%04d%02d%02d%02d%02d%02d.log", processName,timeStruct.tm_year+1900, timeStruct.tm_mon+1, timeStruct.tm_mday, 
					timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec);
	filePathNamelen = strlen(filePathName);



	env = getenv("DEBUG_TYPE");

	if((m_level>0) && (env != NULL) && (strstr(env, "FILE") != NULL || strstr(env, "ALL") != NULL))
	{
		fp = fopen(filePathName, "a+");
		
		if(fp == NULL)
		{
			::printf("Open logfile error, logFile=%s,errorStr=%s\n",filePathName,strerror(errno));
		}
	}

	if(env==NULL || strstr(env, "FILE")==NULL)
	{		
		fpConsole = stdout;
	}


	::printf("log: processName=%s level=%d size=%dK file=%s dest=",processName,m_level,(int)maxBlocks/2,filePathName);
	if(m_level>0)
	{
		if(fp!=NULL && fpConsole!=NULL)
			::printf("ALL\n");
		else if(fp!=NULL)
			::printf("FILE\n");
		else if(fpConsole!=NULL)
			::printf("CONSOLE\n");
		else
			::printf("NONE\n");
	}
	else
	{
		::printf("NONE\n");
	}

	if(m_level>=DEBUG_DBUS_MSG)
	{
		if(fp!=NULL)
			dbusdebug.openDebug(fp);
		else if(fpConsole!=NULL)
			dbusdebug.openDebug(fpConsole);
	}
}


IBDDebugMonitorC::~IBDDebugMonitorC()
{
	if(fp != NULL)
	{
		dbusdebug.closeDebug();
		fclose(fp);
	}
}


int IBDDebugMonitorC::mvFile()
{
	char bufcmd[500];
	struct stat buf;

	if(fp==NULL)
		return 0;

	if(stat(filePathName, &buf) == -1)
	{
		fprintf(stderr, "get the stat is faild!\n");
		//return -1;
	}
	else
	{
		if(buf.st_blocks <= (int)maxBlocks)
			return 0;
	}

	dbusdebug.closeDebug();
	fclose(fp);
	fp=NULL;
	
	sprintf(bufcmd, "mv %s %s.bak", filePathName, filePathName);
	system(bufcmd);
	system("sync");

	if(m_level>0)
	{
		fp = fopen(filePathName, "a+");
		if(fp == NULL)
		{
			::printf("in mv File Reopen logfile error, %s\n", strerror(errno));	
		}

		if(m_level>=DEBUG_DBUS_MSG)
		{
			if(fp!=NULL)
				dbusdebug.openDebug(fp);
			else if(fpConsole!=NULL)
				dbusdebug.openDebug(fpConsole);
		}
	}
		
	return 0;
}


int IBDDebugMonitorC::printf(char level,const char *fileName,const char* functionName,int lineNo,const char *fmt, ...)
{
	pthread_mutex_lock(&debugLock);
	
	if(level <= m_level  && m_level != 0)
	{
		if(fp == NULL && fpConsole == NULL)
		{
			pthread_mutex_unlock(&debugLock);
			return -1;
		}

		va_list ap;
		va_start(ap, fmt);

		time_t dayTime;
		time(&dayTime);
		struct tm	timeStruct;

		gmtime_r(&dayTime, &timeStruct);

		m_serialNo++;

		if(fpConsole != NULL)
		{
			fprintf(fpConsole, "%5d. %02d:%02d:%02d %s:%s:%d\n", m_serialNo, timeStruct.tm_hour, 
				timeStruct.tm_min, timeStruct.tm_sec, 
				fileName, functionName, lineNo);

			vfprintf(fpConsole, fmt, ap);
			fflush(fpConsole);
		}

		if(fp != NULL)
		{
			fprintf(fp, "%5d. %02d:%02d:%02d %s:%s:%d\n", m_serialNo, timeStruct.tm_hour, 
				timeStruct.tm_min, timeStruct.tm_sec, 
				fileName, functionName, lineNo);
			vfprintf(fp, fmt, ap);
			fflush(fp);
	
			if(mvFile() < 0)
				fprintf(stdout, "mv File error\n");
		}
		
		va_end(ap);
	}

	pthread_mutex_unlock(&debugLock);
	return 0;
}


int IBDDebugMonitorC::loop_begin(char level, const char *fileName, const char* functionName, int lineNo, const char *fmt, ...)
{
	pthread_mutex_lock(&debugLock);
	
	if(level > m_level || m_level == 0)
		return -1;

	m_currentOutLevel = level;

	va_list ap;
	va_start(ap, fmt);

	time_t dayTime;
	time(&dayTime);
	struct tm	timeStruct;

	gmtime_r(&dayTime, &timeStruct);

	if(fp == NULL && fpConsole == NULL)
		goto failPrint;
	
	m_serialNo++;

	if(fpConsole != NULL)
	{
		fprintf(fpConsole,"%5d. %02d:%02d:%02d %s:%s:%d\n", m_serialNo, timeStruct.tm_hour, 
				timeStruct.tm_min, timeStruct.tm_sec, 
				fileName, functionName, lineNo);
		vfprintf(fpConsole, fmt, ap);
		fflush(fpConsole);
	}

	if(fp != NULL)
	{
		fprintf(fp, "%5d. %02d:%02d:%02d %s:%s:%d\n", m_serialNo, timeStruct.tm_hour, 
			timeStruct.tm_min, timeStruct.tm_sec, 
			fileName, functionName, lineNo);
		vfprintf(fp, fmt, ap);
		fflush(fp);
	}
	
	va_end(ap);

	return 0;

failPrint:
	return -1;
}

int IBDDebugMonitorC::loop_printf(int level, const char *fmt, ...)
{
	if(level > m_level || m_level == 0)
		return -1;

	va_list ap;
	va_start(ap, fmt);

	if(fp == NULL && fpConsole == NULL)
		goto failPrint;

	if(fpConsole != NULL)
	{
		vfprintf(fpConsole, fmt, ap);
		fflush(fpConsole);
	}

	if(fp != NULL)
	{
		vfprintf(fp, fmt, ap);
		fflush(fp);
	}
	
	va_end(ap);

	return 0;

failPrint:
	return -1;
}


int IBDDebugMonitorC::loop_end()
{
	if(m_currentOutLevel > m_level || m_level == 0)
		goto failPrint;	

	if(fp == NULL && fpConsole == NULL)
		goto failPrint;

	if(fpConsole != NULL)
	{
		fprintf(fpConsole, "\n");
		fflush(fpConsole);
	}

	if(fp != NULL)
	{
		fprintf(fp, "\n");
		fflush(fp);

		if(mvFile() < 0)
			fprintf(stdout, "mv File error\n");
	}
	
	pthread_mutex_unlock(&debugLock);
	return 0;

failPrint:	
	pthread_mutex_unlock(&debugLock);
	return -1;
}


void IBDDebugMonitorC::enterFunction(const char* fileName, const char* functionName, int lineNo)
{
	pthread_mutex_lock(&debugLock);

	if(m_level < DEBUG_TRACEFUNC)
		goto failEnter;
	
	time_t dayTime;
	time(&dayTime);
	struct tm	timeStruct;

	gmtime_r(&dayTime, &timeStruct);

	if(fp == NULL && fpConsole == NULL)
		goto failEnter;

	m_serialNo++;
	
	if(fpConsole != NULL)
	{	
		fprintf(fpConsole,"%5d. %02d:%02d:%02d %s:%s:%d Enter\n\n", m_serialNo, timeStruct.tm_hour, 
			timeStruct.tm_min, timeStruct.tm_sec, 
			fileName, functionName, lineNo);		
		fflush(fpConsole);
	}
	if(fp != NULL)
	{
		fprintf(fp,"%5d. %02d:%02d:%02d %s:%s:%d Enter\n\n", m_serialNo, timeStruct.tm_hour, 
			timeStruct.tm_min, timeStruct.tm_sec, 
			fileName, functionName, lineNo);
		fflush(fp);
		
		if(mvFile() < 0)
			fprintf(stdout, "mv File error\n");
	}

	pthread_mutex_unlock(&debugLock);
	return;

failEnter:
	
	pthread_mutex_unlock(&debugLock);
	return;
	
}

void IBDDebugMonitorC::exitFunction(const char* fileName, const char* functionName, int lineNo)
{
	pthread_mutex_lock(&debugLock);

	if(m_level < DEBUG_TRACEFUNC)
		goto failExit;
	
	time_t dayTime;
	time(&dayTime);
	struct tm	timeStruct;

	gmtime_r(&dayTime, &timeStruct);

	if(fp == NULL && fpConsole == NULL)
		goto failExit;

	m_serialNo++;

	if(fpConsole != NULL)
	{
		fprintf(fpConsole,"%5d. %02d:%02d:%02d %s:%s:%d Exit\n\n", m_serialNo, timeStruct.tm_hour, 
			timeStruct.tm_min, timeStruct.tm_sec, 
			fileName, functionName, lineNo);
		
		fflush(fpConsole);
	}
	
	if(fp != NULL)
	{
		fprintf(fp, "%5d. %02d:%02d:%02d %s:%s:%d Exit\n\n", m_serialNo, timeStruct.tm_hour, 
			timeStruct.tm_min, timeStruct.tm_sec, 
			fileName, functionName, lineNo);
		fflush(fp);
		if(mvFile() < 0)
			fprintf(stdout, "mv File error\n");
	}

	pthread_mutex_unlock(&debugLock);
	return;

failExit:
	pthread_mutex_unlock(&debugLock);
	return;
	
}

IBDDebugMonitorC& IBDDebugMonitorC::begin(unsigned char level,const char *fileName,const char* functionName,int lineNo)
{
	pthread_mutex_lock(&debugLock);

	m_currentOutLevel = level;

	if(level > m_level || m_level == 0)
		return *this;
	
	time_t dayTime;
	time(&dayTime);
	struct tm	timeStruct;
	gmtime_r(&dayTime, &timeStruct);

	m_serialNo++;
	
	if(fp != NULL)
		fprintf(fp, "%5d. %02d:%02d:%02d %s:%s:%d\n",m_serialNo, timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec, fileName, functionName, lineNo);
	if(fpConsole != NULL)
		fprintf(fpConsole, "%5d. %02d:%02d:%02d %s:%s:%d\n",m_serialNo, timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec, fileName, functionName, lineNo);

	return *this;
}

IBDDebugMonitorC*  IBDEndOperator(IBDDebugMonitorC *dbMonitor)
{
	if(dbMonitor->m_currentOutLevel > dbMonitor->m_level || dbMonitor->m_level == 0)
	{
		pthread_mutex_unlock(&dbMonitor->debugLock);
		return dbMonitor;
	}

	if(dbMonitor->fp != NULL)
	{
		fprintf(dbMonitor->fp, "\n");
		fflush(dbMonitor->fp);
		if(dbMonitor->mvFile() < 0)
			fprintf(stdout, "mv File error\n");
	}
	if(dbMonitor->fpConsole != NULL)
	{
		fprintf(dbMonitor->fpConsole, "\n");
		fflush(dbMonitor->fpConsole);
	}

	pthread_mutex_unlock(&dbMonitor->debugLock);
	return dbMonitor;
}
