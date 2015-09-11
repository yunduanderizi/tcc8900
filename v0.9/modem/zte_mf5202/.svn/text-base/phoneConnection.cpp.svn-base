#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#include <QMutexLocker>

#include "phoneConnection.h"
#include "debugMonitor.h"

static const char *atCommandList[]=
{
#ifdef ADD_WIFI_TEST
	"CEND",
	"CONN",
	"CONF",
#endif
	"OK",
	"ERROR",
	"> ",
	"ANSWER",
//	"CARRIER",
	"END",
	"+CMTI",
	"+CLIP:",
	"MISSED",
//	"+CMGS:",
	"PB DONE",
//	"+CSQ:",
	"RINGBACK",
	"CONNECT",
	"STOPRING",
	"HANGUP",
    "MO RING",
    "MO CONNECTED",
	"NO CARRIER",

	"VOICE NO CARRIER",
	"+ZEND",
	"NO DIALTONE",
	"CSDRINGBACKLOCAL",
	"BUSY",
	"BEGIN"
};

static bool gsbDebugCsq = false;

PhoneConnectionC::PhoneConnectionC()
{
	m_comFd =-1;
	m_devName[0] =0;

	m_atCommandList	    =atCommandList;
	m_atCommandListCount=sizeof(atCommandList)/sizeof(const char*);

	m_command=NULL;
	m_commandResult=NULL;
	m_commandResultLen=0;

	char *pEnvContent = getenv("DEBUG_TEST_FOR_CSQ");
printf("------------------->come here         pEnvContent[%p]\n", pEnvContent);
	if( pEnvContent )
		gsbDebugCsq = true;
	else
		gsbDebugCsq = false;
	
	pipe(m_pipe);
}
PhoneConnectionC::~PhoneConnectionC()
{
	closeDev();
	close(m_pipe[0]);
	close(m_pipe[1]);
}

void PhoneConnectionC::setAtCommandTable(const char *atCommandList[],int itemCount)
{
	QMutexLocker locker(&m_mutex);

	m_atCommandList      =atCommandList;
	m_atCommandListCount =itemCount;
}

int PhoneConnectionC::openDev(const char *dev)
{
	QMutexLocker locker(&m_mutex);
	
	if(m_comFd>=0)
		close(m_comFd);

	strncpy(m_devName,dev,20);
	m_devName[19]=0;

	m_comFd =open(m_devName,O_RDWR);
	if(m_comFd<0)
	{
		char buffer[100];
		IBD_PRINTF(DEBUG_EMERGENCY,"open dev:%s error. %s\n",m_devName,strerror_r(errno,buffer,100));
		return PC_RESULT_OPEN_ERROR;
	}

	struct termios termAttr;
	bzero(&termAttr,sizeof(termAttr));
        cfmakeraw(&termAttr);
        termAttr.c_cflag = B115200;  /* set baudrate */
        termAttr.c_cflag |= CLOCAL | CREAD;      /* | CRTSCTS */

        termAttr.c_cflag |= CS8;
        termAttr.c_cflag &= ~CSTOPB;
        termAttr.c_cflag &= ~PARENB;

        termAttr.c_oflag = 0;
        termAttr.c_lflag |= 0;
        termAttr.c_oflag &= ~OPOST;
        termAttr.c_cc[VTIME] = 1;        /* unit: 1/10 second. */
        termAttr.c_cc[VMIN] = 1; /* minimal characters for reading */

        tcflush (m_comFd,TCIFLUSH);
        int result=tcsetattr(m_comFd,TCSANOW,&termAttr);

	if(result<0)
	{
		char buffer[100];
		IBD_PRINTF(DEBUG_WARNING,"set termio attribute error.dev=%s error=%s\n",m_devName,strerror_r(errno,buffer,100));
	}

	return PC_RESULT_OK;
}

int PhoneConnectionC::closeDev()
{
	QMutexLocker locker(&m_mutex);

	int fd=m_comFd;
	m_comFd =-1;

	if(fd>=0)
	{
		write(m_pipe[1],"1",1);
		close(fd);
	}
return PC_RESULT_OK;
}

int  PhoneConnectionC::sendAtCommand(const char *atCommand, char resultBuffer[], int bufferLen, int timeout)
{
#ifdef ADD_WIFI_TEST
	usleep(1000*500);
#endif
	QMutexLocker locker(&m_mutex);

	if(m_comFd<0)
		return PC_RESULT_NOT_OPEN;

	return sendAtCommand_direct(atCommand, resultBuffer, bufferLen, timeout);
}
int  PhoneConnectionC::sendSMSCommand(const char *smsPDU, char resultBuffer[], int bufferLen, int timeout)
{
	QMutexLocker locker(&m_mutex);

	if(m_comFd<0)
		return PC_RESULT_NOT_OPEN;

	int pdu_len = strlen(smsPDU)/2-1;
	char cmd[100];

	sprintf(cmd,"AT+CMGS=%d\r\n",pdu_len);
	int result=sendAtCommand_direct(cmd,resultBuffer,bufferLen,timeout);
	if(result<0)
	{
		return result;
	}

	if(strstr(resultBuffer,"> ") == NULL)
	{
		return PC_RESULT_RES_ERROR;
	}

	char *smsContent=(char*)malloc(strlen(smsPDU)+10);
	sprintf(smsContent,"%s\032",smsPDU);

	result=sendAtCommand_direct(smsContent,resultBuffer,bufferLen,timeout);

	free(smsContent);

	return result;
}

/*
return: <0: error(which need recover)
        >=0: return AT notification
*/
int  PhoneConnectionC::getNextIndication(char buffer[], int bufferLen)
{
	char recvBuffer[1024];
	while(1)
	{
		if(m_comFd<0)
			return PC_RESULT_NOT_OPEN;

		int ret =readCmdRespond(recvBuffer,1024);
		if(ret <= 0)
		{
			if(ret==-2)
				continue;
			else
				return PC_RESULT_OP_ERROR;
		}

	if( gsbDebugCsq )
	{
		if(strncmp(recvBuffer,"AT+CSQ",strlen("AT+CSQ"))==0)
		{
			printf("Recv AT(%s):%s\n",m_devName,recvBuffer);
		}
		else if(strncmp(recvBuffer,"AT+ZPAS",strlen("AT+ZPAS"))==0)
		{
			printf("Recv AT(%s):%s\n",m_devName,recvBuffer);
		}
		else
		{

		}
	}
	
#ifdef IBD_DEBUG
	static unsigned int csqCount=0;
	static unsigned int zpasCount=0;

	if(strncmp(recvBuffer,"AT+CSQ",strlen("AT+CSQ"))==0)
	{
		if((csqCount++)%(10*60)==0)
			IBD_PRINTF(DEBUG_INFOMATION,"Recv AT(%s):%s\n",m_devName,recvBuffer);
	}
	else if(strncmp(recvBuffer,"AT+ZPAS",strlen("AT+ZPAS"))==0)
	{
		if((zpasCount++)%(10*60)==0)
			IBD_PRINTF(DEBUG_INFOMATION,"Recv AT(%s):%s\n",m_devName,recvBuffer);
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION,"Recv AT(%s):%s,line:%d....",m_devName,recvBuffer,__LINE__);
	}
#endif

		if(handleAtCommandResult(recvBuffer,strlen(recvBuffer)) == PC_RESULT_OK)
                {
			continue;
		}
		strncpy(buffer,recvBuffer,bufferLen-1);
		buffer[bufferLen-1]=0;
		break;
	}

	return strlen(buffer);
}

int PhoneConnectionC::writeBuffer(char buffer[], int bufferLen)
{
	QMutexLocker locker(&m_mutex);

	if(m_comFd<0)
		return PC_RESULT_NOT_OPEN;

        int writeLen = 0;

        writeLen =write(m_comFd,buffer,bufferLen);
	if(writeLen<0 && (errno==EAGAIN || errno==EINTR))
		writeLen =0;

/*
        do
	{
                int len = write(m_comFd,buffer+writeLen, bufferLen- writeLen);
                if(len > 0)
		{
                        writeLen += len;
		}
                else if(len==0)
		{
                        break;
                }
		else
		{
			if(errno==EINTR)
				continue;
			else
				return PC_RESULT_OP_ERROR;
		}
        }while(writeLen < bufferLen);
*/

	return writeLen;
}

int PhoneConnectionC::readBuffer(char buffer[], int bufferLen)
{
	QMutexLocker locker(&m_mutex);

	if(m_comFd<0)
		return PC_RESULT_NOT_OPEN;

	//usleep(200*1000);

	int readLen=0;
        readLen =read(m_comFd,buffer,bufferLen);
	if(readLen<0 && (errno==EAGAIN || errno==EINTR))
		readLen=0;
/*
	while(readLen<bufferLen)
	{
		fd_set fds;
		struct timeval t;
		t.tv_sec = 1;
		t.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(m_comFd, &fds);
	
		int selectResult=select(m_comFd+1, &fds, NULL, NULL, &t);

		if(selectResult==0)
		{
			//break;
		}
		else if(selectResult<0)
		{
			if(errno == EAGAIN)	
				break;
			else
				return selectResult;
		}
		else
		{
        		int len = read(m_comFd,buffer,1);
			if(len>0)
				readLen+=len;
			else if(len==0)
				break;
			else if(errno==EINTR)
				continue;
			else
				return len;
		}
	}

*/
	
/*
	int len=0;
	while(len<=0)
	{
        	len = read(m_comFd,buffer,bufferLen);
	
		if(len<0 && errno==EINTR)
			continue;
		else 
			return len;
	}
	return len;
*/
	return readLen;
}

int PhoneConnectionC::setBlockMode(int blockMode)
{
	QMutexLocker locker(&m_mutex);

	if(m_comFd<0)
		return PC_RESULT_NOT_OPEN;

	long	fileFlag=0;
	int     ret;

	ret =fcntl(m_comFd,F_GETFL,&fileFlag);

	if(ret<0)
	{
		IBD_PRINTF(DEBUG_EMERGENCY,"Cann't get file status flag(%s).\n",m_devName);
		return ret;
	}

	if(blockMode)
		fileFlag &=(~O_NONBLOCK);	
	else
		fileFlag |=O_NONBLOCK;
	
	ret =fcntl(m_comFd,F_SETFL,fileFlag);

	if(ret<0)
	{
		IBD_PRINTF(DEBUG_EMERGENCY,"Cann't set file status flag(%s).\n",m_devName);
	}

	return ret;
}

//SUCCESS:  return the length 0f the command result,the length may great than the bufferLen
//TIME OUT: return RESULT_TIME_OUT
//OTHER ERROR: return <0
int  PhoneConnectionC::sendAtCommand_direct(const char *atCommand, char resultBuffer[], int bufferLen, int timeout)
{
	QMutexLocker locker(&m_waitMutex);

	if(atCommand == NULL || resultBuffer == NULL || bufferLen <= 0)
		return PC_RESULT_PARAM_ERROR;

#ifdef IBD_DEBUG
	static unsigned int csqCount=0;
	static unsigned int zpasCount=0;

	if(strncmp(atCommand,"AT+CSQ\r\n",strlen("AT+CSQ\r\n"))==0) 
	{
		if((csqCount++)%(10*60)==0)
			IBD_PRINTF(DEBUG_INFOMATION,"Send AT(%s):%s\n",m_devName,atCommand);
	}
	else if(strncmp(atCommand,"AT+ZPAS?\r\n",strlen("AT+ZPAS?\r\n"))==0)
	{
		if((zpasCount++)%(10*60)==0)
			IBD_PRINTF(DEBUG_INFOMATION,"Send AT(%s):%s\n",m_devName,atCommand);
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION,"Send AT(%s):%s\n",m_devName,atCommand);
	}
#endif

	int writeLen=0;
	while(writeLen<(int)strlen(atCommand))
	{
		int len=write(m_comFd, atCommand+writeLen, strlen(atCommand)-writeLen);
		if(len<=0)
		{
			if(errno==EINTR)
			{
				continue;
			}
			else
			{
				char buffer[100];
				IBD_PRINTF(DEBUG_EMERGENCY,"Send AT(%s):%s error=%s\n",m_devName,atCommand,strerror_r(errno,buffer,100));
				return PC_RESULT_OP_ERROR;
			}
		}

		writeLen+=len;
	}

	m_command = atCommand;
	m_commandResult = resultBuffer;
	m_commandResultLen = bufferLen;

	bool waitResult = m_event.wait(&m_waitMutex, timeout);

	m_command = NULL;
	m_commandResult = NULL;

	if(waitResult == false)
	{
		IBD_PRINTF(DEBUG_WARNING,"Send AT(%s):%s timeout\n",m_devName,atCommand);
		return PC_RESULT_TIME_OUT;
	}

	return m_commandResultLen;
}

int PhoneConnectionC::writeBuffer_direct(char buffer[], int bufferLen)
{
/*
        fd_set fds;
        int    selectResult=0;

        FD_ZERO(&fds);
        FD_SET(m_comFd, &fds);

        selectResult = select(m_comFd+1, NULL, &fds, NULL, NULL);
*/

        int writeLen = 0;
        do
	{
                int len = write(m_comFd,buffer+writeLen, bufferLen- writeLen);
                if(len > 0)
		{
                        writeLen += len;
		}
                else if(len==0)
		{
                        break;
                }
		else
		{
			if(errno==EINTR)
				continue;
			else
				return PC_RESULT_OP_ERROR;
		}
        }while(writeLen < bufferLen);

	return writeLen;
}

//if find the match command,return RESULT_OK
//else: return <0
int  PhoneConnectionC::handleAtCommandResult(const char *commandResult,int len)
{
	QMutexLocker locker(&m_waitMutex);

	if(m_command == NULL)
		return -2;

	/*´Ë´¦¶Ô±ÈÊ±ºòÊÇÖ÷¶¯·¢ËÍµÄÃüÁî*/
	//if(strncmp(m_command, commandResult, strlen(m_command)-2) == 0)
	if(strstr(commandResult,"OK")!=NULL || strstr(commandResult,"ERROR")!=NULL || strstr(commandResult,"> "))
	//if(!strncmp(commandResult,"OK",2) || !strncmp(commandResult,"ERROR",5))
	{
		//fprintf(stderr, "Active AT cmd Response ok....\n");
		strncpy(m_commandResult, commandResult, m_commandResultLen-1);
		m_commandResult[m_commandResultLen-1] = 0;

		m_commandResultLen = strlen(commandResult);

		m_event.wakeOne();

		return PC_RESULT_OK;
	}
	else
	{
		return -3;
	}
}


/******************************************
º¯Êý¹¦ÄÜ:		´Ó´®¿Ú¶ÁÈ¡Ò»ÐÐÐÅÏ¢
º¯Êý·µ»ØÖµ:	   i+1	/ i-1±íÊ¾¶ÁÈ¡µÄ³¤¶È
					-2		±íÊ¾¶ÁÈ¡´®¿Ú³ö´í
					-1 		±íÊ¾¶ÁÈ¡´®¿Ú³¬Ê±
return: 0: file is closed
        -1: time out
	-2: buffer is too short
	-3: system error

	buf: return the string already read (null terminated)
******************************************/
int PhoneConnectionC::readLine(char *buf, int len, int* flag)
{	
	int i = 0;
	struct timeval t;
	fd_set fds;
	int    selectResult=0;

	while(i<len-1)
	{
		int tempFd=m_comFd;
		if(tempFd<0)
			return 0;

		t.tv_sec = 2;
		t.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(tempFd, &fds);
		FD_SET(m_pipe[0], &fds);

		int maxFd =tempFd>m_pipe[0] ? tempFd : m_pipe[0];

		if(*flag == 0)
		{
			selectResult=select(maxFd+1, &fds, NULL, NULL,NULL);
		}
		else
		{
			selectResult=select(maxFd+1, &fds, NULL, NULL, &t);
		}
		if(selectResult > 0)
		{
			if(FD_ISSET(m_pipe[0], &fds))
			{
				char buf[10];
				read(m_pipe[0],buf,1);
				continue;
			}

			int ret=read(tempFd,buf+i,1);
			if(ret>0)
			{
				*flag=1;

				if(*(buf+i) == '\n' || (i>0 && *(buf+i-1) =='>' && *(buf+i)==' '))
				{
					buf[i+1] =0;
					return i+1;
				}
				i++;
			}
			else  if(ret==0)
			{
				buf[i]=0;
				return 0;
			}
			else
			{
				if(errno==EINTR)
				{
					continue;
				}
				else
				{
					buf[i]=0;
					return -3;
				}
			}
		}
		else if(selectResult==0) //time out
		{
			buf[i] = 0;
			return -1;
		}
		else
		{
			if(errno==EINTR)
			{
				continue;
			}
			else
			{
				buf[i]=0;
				return -3;
			}
		}
	}

	buf[i] = 0;
	return -2;
}


/*************************************
º¯Êý¹¦ÄÜ:		¶ÁÈ¡ÍêÕûµÄÃüÁî
º¯Êý·µ»ØÖµ:	currLen	±íÊ¾¶Á³öÃüÁîµÄ³¤¶È
					-2		±íÊ¾¶ÁÈ¡´®¿ÚÊ±³ö´í
					-1 		±íÊ¾¶ÁÈ¡ÃüÁîµÄ¹ý³ÌÖÐ³¬Ê±
return: 0: file is closed
	-2: buffer is too short
	-3: system error

	buf: return the string already read (null terminated)
*************************************/
int PhoneConnectionC::readCmdRespond(char buf[],int len)
{
	int currLen=0;
	int currLine=0;
	int flag    =0;

	buf[0]=0;

	while(1)
	{
		int result = 0;

		result = readLine(buf+ currLen, len - currLen, &flag);

		if(result == -1)
		{
			IBD_PRINTF(DEBUG_WARNING,"Read AT(%s)timeout:%s\n",m_devName,buf);
#ifdef _8925_8D1
			result = strlen(buf) - currLen;
#else
			currLen = 0;
			flag = 0;
			continue;
#endif
		}
		else if(result == -2)
		{
			IBD_PRINTF(DEBUG_WARNING,"Read AT(%s)buffer is too short:%s\n",m_devName,buf);
			return -2;
		}
		else if(result==0)
		{
			IBD_PRINTF(DEBUG_WARNING,"Read AT(%s)dev is closed:%s\n",m_devName,buf);
			return result;
		}
		else if(result<0)
		{
			IBD_PRINTF(DEBUG_WARNING,"Read AT(%s)system error:%s\n",m_devName,buf);
			return result;
		}
		
		if(strcmp(buf + currLen,"\r\n")==0)
			continue;

		int i=0;
		for(i=0;i<m_atCommandListCount;i++)
		{
			if(strstr(buf + currLen,m_atCommandList[i]) != NULL)
			{
				break;
			}
		}
		currLine++;
		currLen+=result;
		if(i<m_atCommandListCount)
			break;
	}

	return currLen;
}
