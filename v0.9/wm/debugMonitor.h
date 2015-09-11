#ifndef _DEBUGMONITOR_H_
#define _DEBUGMONITOR_H_

#include <qstring.h>
#include "dbusDebug.h"
#include "dbusServer.h"


#define IBD_DEBUG 


#define	DEBUG_NODBUGINFO		(0)
#define	DEBUG_EMERGENCY			(1)
#define	DEBUG_WARNING			(2)
#define	DEBUG_INFOMATION		(3)
#define	DEBUG_TRACEFUNC			(4)
#define	DEBUG_DBUS_MSG			(5)


class IBDDebugMonitorC;

typedef IBDDebugMonitorC*  (*IBDStreamOperator)(IBDDebugMonitorC *);


#define MVFILESIZE			500
#define FILEPATHNAMESIZE		200


class IBDDebugMonitorC
{
public:
	IBDDebugMonitorC(const char* processName, DBusServerC *debugServer);
	~IBDDebugMonitorC();

	int printf(char level,const char *fileName,const char* functionName,int lineNo,const char *fmt, ...);

	int loop_begin(char level,const char *fileName,const char* functionName,int lineNo,const char *fmt, ...);
	int loop_printf(int level, const char *fmt, ...);
	int loop_end();
	
	void	enterFunction(const char* fileName, const char* functionName, int lineNo);
	void	exitFunction(const char* fileName, const char* functionName, int lineNo);


	IBDDebugMonitorC& begin(unsigned char level,const char *fileName,const char* functionName,int lineNo);

	inline  IBDDebugMonitorC&  operator <<(IBDStreamOperator streamOP)
	{
		return *(streamOP(this));			
	}

	inline IBDDebugMonitorC& operator <<(int arg)
	{
		if(m_currentOutLevel > m_level || m_level == 0)
			return *this;
		
		if(fpConsole != NULL)
			fprintf(fpConsole, "%d", arg);
		
		if(fp != NULL)
			fprintf(fp, "%d", arg);

		return *this;
	}

	inline IBDDebugMonitorC& operator <<(double arg)
	{
		if(m_currentOutLevel > m_level || m_level == 0)
			return *this;
		
		if(fpConsole != NULL)
			fprintf(fpConsole, "%f", arg);
		
		if(fp != NULL)
			fprintf(fp, "%f", arg);
	

		return *this;
	}

	inline IBDDebugMonitorC& operator <<(char arg)
	{
		if(m_currentOutLevel > m_level || m_level == 0)
			return *this;
		
		if(fpConsole != NULL)
			fprintf(fpConsole, "%c", arg);
		
		if(fp != NULL)
			fprintf(fp, "%c", arg);

		return *this;	
	}

	inline IBDDebugMonitorC& operator <<(char* arg)
	{
		if(m_currentOutLevel > m_level || m_level == 0)
			return *this;
		
		if(fpConsole != NULL)
			fprintf(fpConsole, "%s", arg);
		
		if(fp != NULL)
			fprintf(fp, "%s", arg);
		
		return *this;

	}

	inline IBDDebugMonitorC& operator <<(QString arg)
	{
		if(m_currentOutLevel > m_level || m_level == 0)
			return *this;
		
		if(fpConsole != NULL)
			fprintf(fpConsole, "%s", arg.toUtf8().data());
		
		if(fp != NULL)
			fprintf(fp, "%s", arg.toUtf8().data());
		
		return *this;
	}

private:
	static void debugMonitorHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);

private:
	int mvFile();

public:
	static  IBDDebugMonitorC  *IBD_debugMonitor;

private:
	int filePathNamelen;
	unsigned int  m_serialNo;
	unsigned char m_level;
	unsigned char m_currentOutLevel;
	unsigned long  maxBlocks;
	pthread_mutex_t debugLock;
	char filePathName[FILEPATHNAMESIZE];
	DBusDebugC dbusdebug;

	FILE *fp;
	FILE *fpConsole;
	
	friend IBDDebugMonitorC*  IBDEndLineOperator(IBDDebugMonitorC *dbMonitor);
	friend IBDDebugMonitorC*  IBDEndOperator(IBDDebugMonitorC *dbMonitor);
};

inline IBDDebugMonitorC*  IBDEndLineOperator(IBDDebugMonitorC *dbMonitor)
{
	if(dbMonitor->m_currentOutLevel > dbMonitor->m_level || dbMonitor->m_level == 0)
		return dbMonitor;
	
	if(dbMonitor->fpConsole != NULL)
		fprintf(dbMonitor->fpConsole, "\n");
	
	if(dbMonitor->fp != NULL)
		fprintf(dbMonitor->fp, "\n");
	
	return dbMonitor;
}

extern IBDDebugMonitorC*  IBDEndOperator(IBDDebugMonitorC *dbMonitor);


#ifdef IBD_DEBUG 
#define	IBD_INIT(processName, dbusServer)			IBDDebugMonitorC::IBD_debugMonitor = new IBDDebugMonitorC(processName, dbusServer);

#define	IBD_BEGIN(level)				if(IBDDebugMonitorC::IBD_debugMonitor==NULL) \
										IBDDebugMonitorC::IBD_debugMonitor=new IBDDebugMonitorC("default",NULL); \
									IBDDebugMonitorC::IBD_debugMonitor->begin(level, __FILE__, __FUNCTION__, __LINE__)
											
#define	IBD_ENDL					IBDEndLineOperator
#define	IBD_END						IBDEndOperator;

#define	IBD_ENTER_FUNC				do {if(IBDDebugMonitorC::IBD_debugMonitor==NULL) \
										IBDDebugMonitorC::IBD_debugMonitor=new IBDDebugMonitorC("default",NULL); \
									IBDDebugMonitorC::IBD_debugMonitor->enterFunction(__FILE__, __FUNCTION__, __LINE__); } while(0);

#define	IBD_EXIT_FUNC				do {if(IBDDebugMonitorC::IBD_debugMonitor==NULL) \
										IBDDebugMonitorC::IBD_debugMonitor=new IBDDebugMonitorC("default",NULL); \
									IBDDebugMonitorC::IBD_debugMonitor->exitFunction(__FILE__, __FUNCTION__, __LINE__); } while(0);

#define	IBD_PRINTF(level,fmt, arg...)		do {if(IBDDebugMonitorC::IBD_debugMonitor==NULL) \
										IBDDebugMonitorC::IBD_debugMonitor=new IBDDebugMonitorC("default",NULL); \
									IBDDebugMonitorC::IBD_debugMonitor->printf(level, __FILE__, __FUNCTION__, __LINE__, fmt, ##arg); } while(0);

#define	IBD_LOOP_BEGIN(level,fmt, arg...)	do {if(IBDDebugMonitorC::IBD_debugMonitor==NULL) \
											IBDDebugMonitorC::IBD_debugMonitor=new IBDDebugMonitorC("default",NULL); \
										IBDDebugMonitorC::IBD_debugMonitor->loop_begin(level, __FILE__, __FUNCTION__, __LINE__, fmt, ##arg); } while(0);

#define	IBD_LOOP_PRINTF(level, fmt, arg...)	do {if(IBDDebugMonitorC::IBD_debugMonitor==NULL) \
											IBDDebugMonitorC::IBD_debugMonitor=new IBDDebugMonitorC("default",NULL); \
										IBDDebugMonitorC::IBD_debugMonitor->loop_printf(level, fmt, ##arg); } while(0);

#define	IBD_LOOP_END		do {if(IBDDebugMonitorC::IBD_debugMonitor==NULL) \
											IBDDebugMonitorC::IBD_debugMonitor=new IBDDebugMonitorC("default",NULL); \
										IBDDebugMonitorC::IBD_debugMonitor->loop_end(); } while(0);

#else
#define IBD_INIT(a,b) ;

#define IBD_BEGIN(level) do{ if(0) { (*(IBDDebugMonitorC*)0)
#define IBD_ENDL		0
#define IBD_END		0;} }while(0);

#define IBD_ENTER_FUNC 	;
#define IBD_EXIT_FUNC ;

#define  IBD_PRINTF(level,fmt,arg...)  ;

#define IBD_LOOP_BEGIN(level,fmt, arg...) ;
#define IBD_LOOP_PRINTF(level, fmt, arg...) ;
#define IBD_LOOP_END ;

#endif


#endif
