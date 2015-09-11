#ifndef phoneConnection_H_20101223
#define phoneConnection_H_20101223

#include <QMutex>
#include <QWaitCondition>


#define PC_RESULT_OK 		0
#define PC_RESULT_TIME_OUT 	-1
#define PC_RESULT_NOT_OPEN	-2
#define PC_RESULT_OPEN_ERROR	-3
#define PC_RESULT_RES_ERROR	-4
#define PC_RESULT_PARAM_ERROR	-5
#define PC_RESULT_OP_ERROR	-6

class PhoneConnectionC 
{
public:
	PhoneConnectionC();
	~PhoneConnectionC();

public:
	int openDev(const char *dev);
	int closeDev();

	void setAtCommandTable(const char *atCommandList[],int itemCount);

	int  sendAtCommand(const char *atCommand, char resultBuffer[], int bufferLen, int timeout=5000);
	int  sendSMSCommand(const char *smsPDU, char resultBuffer[], int bufferLen, int timeout=35000);

	int  getNextIndication(char buffer[], int bufferLen);

public:
	int writeBuffer(char buffer[], int bufferLen);
	int readBuffer(char buffer[], int bufferLen);
	int setBlockMode(int blockMode);


private:
	int  sendAtCommand_direct(const char *atCommand, char resultBuffer[], int bufferLen, int timeout=5000);
	int  writeBuffer_direct(char buffer[], int bufferLen);
	int  handleAtCommandResult(const char *commandResult,int len); 
	     
	int readLine(char *buf, int len, int* flag);
	int readCmdRespond(char buf[],int len);

private:
	int 	m_comFd;
	int 	m_pipe[2];
	char    m_devName[20];

	const char **m_atCommandList;
	int	     m_atCommandListCount;

	char 	*m_commandResult;
	int 	m_commandResultLen;
	const char 	*m_command;
	 
	QMutex 		m_mutex;
	QMutex 		m_readMutex;

	QMutex 		m_waitMutex;
	QWaitCondition 	m_event;
};

#endif



