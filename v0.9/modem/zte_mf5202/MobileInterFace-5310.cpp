#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "MobileInterFace.h"
#include "phoneMonitorVoiceReportBase.h"

#include "phoneConnection.h"

#include "voicePhoneInitThread.h"
#include "voicePhoneWorkThread.h"
#include "videoPhoneMonitor.h"

#include "debugMonitor.h"
#include "md5.h"



PhoneMonitorVoiceInterface::PhoneMonitorVoiceInterface()
{
	m_phoneStatus =PHONE_STATUS_NOT_INIT;
	m_callStatus  =CALL_IDLE;
	m_phoneNumber[0]=0;

	m_pPhoneConnection =new PhoneConnectionC();
	m_pInitThread =new VoicePhoneInitThreadC();
	m_pWorkThread =new VoicePhoneWorkThreadC();
	m_videoPhone  =new VideoPhoneMonitorC();

	connect(m_pInitThread, SIGNAL(phoneInitedSignal()), this, SIGNAL(phoneInitedSignal()));//for bug:calling,auto-system reboot suddently because of unpredictable factor,calling is going on
}
PhoneMonitorVoiceInterface::~PhoneMonitorVoiceInterface()
{
}

#ifdef PHONE_MODULE_ZTEMF5200_AUTH
void PhoneMonitorVoiceInterface::getMd5Data(char *input, char *output)
{
	const char *data = input;
	unsigned char data_MD5[32] = {0};
	unsigned char data_MD5_char[33];
	memset(&data_MD5_char,0,sizeof(data_MD5_char));
	
	MD5_CTX context;
	MD5Init_UI(&context);
	MD5Update(&context, (unsigned char *)data, 16);
	MD5Final(data_MD5, &context);
	HextoASC((char *)data_MD5_char,data_MD5,sizeof(data_MD5));//将16进制的数转换为ASC
	
	strcpy(output, (char *)data_MD5_char);
}

int PhoneMonitorVoiceInterface::authentication()
{
	QMutexLocker locker(&m_mutex);
	char sendBuffer[200];
    char recvBuffer[200];
    int ret = 0;

    sprintf(sendBuffer, "AT+ZGKEY?\r\n");

	ret = m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);
	
	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL) && (strstr(recvBuffer, "+ZGKEY:") != NULL))
	{
		char cpTmp1[50];
		char cpTmp2[50];
		char *pTmp1 = cpTmp1;
		char *pTmp2 = NULL;
		char randStr[32];
		char outputStr[50];
		strcpy(cpTmp1, recvBuffer);

		if(strsep(&pTmp1, ":") != NULL)
		{
			strsep(&pTmp1, " ");
			pTmp2 = strsep(&pTmp1, "OK");
			strcpy(randStr, pTmp2);

			getMd5Data(randStr, outputStr);	

			memset(sendBuffer,0,sizeof(sendBuffer));
			sprintf(sendBuffer, "AT+ZVERIFY=\"%s\"\r\n", outputStr);

			ret = m_pPhoneConnection->sendAtCommand(sendBuffer, recvBuffer,200,2000);
			
			if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
			{
				IBD_PRINTF(DEBUG_EMERGENCY, "Ztemf5200 authentication success.");
				return PHONE_MONITOR_SUCCESS;
			}
		}
	}
	IBD_PRINTF(DEBUG_EMERGENCY, "Ztemf5200 authentication failed!");
	
	return PHONE_MONITOR_FAIL;
}
#endif

int PhoneMonitorVoiceInterface::start( PhoneMonitorVoiceReportBase *reportHandle )
{
	m_phoneStatus =PHONE_STATUS_NOT_INIT;
	m_callStatus  =CALL_IDLE;

	m_pCallback   =reportHandle;


	if(m_pPhoneConnection->openDev("/dev/ttyUSB2")<0)
		return -1;

	QThread::start();
#ifdef PHONE_MODULE_ZTEMF5200_AUTH	
	if(authentication() < 0)
	{
		printf("AUTH fail!!!\n");
		//m_pPhoneConnection->closeDev("/dev/ttyUSB2");
		return -1;
	}
#endif
	m_phoneStatus =PHONE_STATUS_INITTING;

	m_pInitThread->startInit(this);
	m_pWorkThread->init(this);

//	m_videoPhone->init(reportHandle);
	
	return PHONE_MONITOR_SUCCESS;
}

int PhoneMonitorVoiceInterface::stop()
{
	///xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	IBD_PRINTF(DEBUG_INFOMATION, " Function not supported Now ! \n")
	return PHONE_MONITOR_FAIL;
}




int PhoneMonitorVoiceInterface::dial(const char* cpNumber)
{
	if (strlen(cpNumber) > PHONE_MONITOR_NUMBER_LEN)
		return PHONE_MONITOR_PHONE_NUMBER_INVALID;

	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	if(m_callStatus!=CALL_IDLE)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer, "ATD%s;\r\n",cpNumber);

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);
        printf("ret is 000000000000000   %d;\r\n recvBuffer is %s\r\n",ret,recvBuffer);

	if(ret >= 0 && strstr(recvBuffer,"OK") != NULL && strstr(recvBuffer,"END") == NULL)
	{
		m_callStatus =CALL_CALLING;
		
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		
		return PHONE_MONITOR_FAIL;
	}
}

int PhoneMonitorVoiceInterface::pickUp()
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	if(m_callStatus!=CALL_COMING_CALL)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"ATA\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL  ||  strstr(recvBuffer, "BEGIN") != NULL))
	{
		m_callStatus =CALL_TALKING;
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}

int PhoneMonitorVoiceInterface::hangUp()
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	m_callStatus =CALL_IDLE;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CHUP\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL  ||  strstr(recvBuffer, "END") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}

int PhoneMonitorVoiceInterface::rtDial(char cbyte)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	if(m_callStatus!=CALL_TALKING)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+VTS=%c\r\n",cbyte);

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}



bool PhoneMonitorVoiceInterface::init3gfun()
{
	return true;
}

bool PhoneMonitorVoiceInterface::close3gfun()
{
	return true; 
}

int PhoneMonitorVoiceInterface::FaceTime_Dial(char* cpNumber)
{
	return m_videoPhone->dial(cpNumber);
}

bool PhoneMonitorVoiceInterface::FaceTime_PickUp()
{	
	return m_videoPhone->pickUp();
}

bool PhoneMonitorVoiceInterface::FaceTime_HangUp()
{
	return m_videoPhone->hangUp();
}

int PhoneMonitorVoiceInterface::sendSMS(const char* pcContent, const char* pcNumber)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned int iCurrentTime = tv.tv_sec;

	int contentlen = strlen(pcContent);
	if(contentlen == 0)
	{
		char content[2];
		content[0] = 32;
		content[1] = '\0';
		m_pWorkThread->appendSendMsg(pcNumber, content, iCurrentTime);

		return PHONE_MONITOR_SUCCESS;
	}

	m_pWorkThread->appendSendMsg(pcNumber, pcContent, iCurrentTime);
	
	return PHONE_MONITOR_SUCCESS;
}

int PhoneMonitorVoiceInterface::sendSMSWithId(const char* pcContent, const char* pcNumber, unsigned int* id)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned int iCurrentTime = tv.tv_sec;
	*id = iCurrentTime;
		
	int contentlen = strlen(pcContent);
	if(contentlen == 0)
	{
		char content[2];
		content[0] = 32;
		content[1] = '\0';
		m_pWorkThread->appendSendMsg(pcNumber, content, iCurrentTime);

		return PHONE_MONITOR_SUCCESS;
	}

	m_pWorkThread->appendSendMsg(pcNumber, pcContent, iCurrentTime);
	
	return PHONE_MONITOR_SUCCESS;
}

int PhoneMonitorVoiceInterface::getRSSI()
{
       static int recvErrorTimes=0;
	char *p=NULL;
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return -1;

	char sendBuffer[200];
        char recvBuffer[256];

	int iSigValue = -1;
        int ret = 0;
        static int iTmpSigValue = iSigValue;

#ifdef _8925_8D1
        sprintf(sendBuffer,"AT+CCALR?\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);
	if(ret >= 0 && ((p=strstr(recvBuffer, "+CCALR: 1")) != NULL))
        {
        }
        else
        {
                iTmpSigValue = iSigValue;

                return iSigValue;
        }
#endif
        sprintf(sendBuffer,"AT+CSQ\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);
	
	if(ret >= 0 && ((p=strstr(recvBuffer, "+CSQ:")) != NULL))
	{
		char cpTmp[300];
		char *cpSig =NULL;

		//strcpy(cpTmp,recvBuffer);
		strcpy(cpTmp,p);
		p=cpTmp;

		if(strsep(&p, ":") != NULL)
		{
			if((cpSig = strsep(&p,",")) != NULL)
			{
				iSigValue = atoi(cpSig);
			}
		}
              recvErrorTimes=0;
	}
        else if(ret >= 0)
        {   
                iSigValue = iTmpSigValue;
                recvErrorTimes=0;
        }   
        else
        {   
            recvErrorTimes++;
            if(recvErrorTimes<=2)
            {
                iSigValue = iTmpSigValue;
            }
            else
            {
                recvErrorTimes=0;
            }
        }   
        iTmpSigValue = iSigValue;

	return iSigValue;
}

int PhoneMonitorVoiceInterface::getSignalStrength(SignalStrength &signal,int &signal_strength)
{
	int newsignum=getRSSI();
	if( newsignum < 0 )
		newsignum = 0;
#ifdef ADD_WIFI_TEST
        newsignum = newsignum % 100;
	if( newsignum == 99 ||( newsignum >= 0 && newsignum < 10 ))
		signal = Strength_0;
	else if( newsignum >= 10 && newsignum < 20 )
		signal = Strength_1;
	else if( newsignum >= 20 && newsignum < 40 )
		signal = Strength_2;
	else if( newsignum >= 40 && newsignum < 60 )
		signal = Strength_3;
	else if( newsignum >= 60 && newsignum < 80 )
		signal = Strength_4;
	else if( newsignum >= 80 && newsignum <= 98 )
		signal = Strength_5;
#else      
	if( newsignum < 3 || newsignum > 31 )
		signal = Strength_0;
	else if( newsignum >= 3 && newsignum < 7 )
		signal = Strength_1;
	else if( newsignum >= 7 && newsignum < 10 )
		signal = Strength_2;
	else if( newsignum >= 10 && newsignum < 13 )
		signal = Strength_3;
	else if( newsignum >= 13 && newsignum < 16 )
		signal = Strength_4;
	else if( newsignum >= 16 && newsignum <= 31 )
		signal = Strength_5;
#endif
	signal_strength = newsignum;

	return 0;
}

int PhoneMonitorVoiceInterface::queryCallState(void)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	//m_callStatus =CALL_IDLE;

	char sendBuffer[200];
    char recvBuffer[200];
    int ret = 0;

    sprintf(sendBuffer,"AT+CPAS\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL  ||  strstr(recvBuffer, "END") != NULL))
	{
		//return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}

	if(strstr(recvBuffer, "3") != NULL || strstr(recvBuffer, "4") != NULL )
	{
		return PHONE_STATUS_CALLING;
	}
	else
    {
    }

	return 0;

	//return m_callStatus;
}

signed char PhoneMonitorVoiceInterface::getNetworkType()
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return -1;

	char sendBuffer[200];
        char recvBuffer[512];
        int ret = 0;

        sprintf(sendBuffer,"AT+ZPAS?\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,500,1000);

	if(ret<0)
		return -1;
	
	if((strstr(recvBuffer, "GPRS") != NULL))
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

//5310 2015-08-24
int PhoneMonitorVoiceInterface::getNetworkTypeAndServiceType(NetworkType &net)
{
       static int recvErrorTimes=0;
	static int recvErrorFails=0;
	NetworkType old_net=net;

	net = No_Network;
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return -1;

	char sendBuffer[200];
       char recvBuffer[512];
       int ret = 0;

       sprintf(sendBuffer,"AT+CNSMOD?\r\n");
	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,500,3000);
	//IBD_PRINTF(DEBUG_EMERGENCY, "111AT+CNSMOD Recv Ret [%d]-[%s]\n", ret,recvBuffer);
	if(ret<0)
	{
		recvErrorTimes++;
		if(recvErrorTimes<=2)
		{
			net=old_net;
		}
		else
		{
			recvErrorTimes=0;
		}
		return -1;
	}

	recvErrorTimes=0;


	if((strstr(recvBuffer, "+CNSMOD: 0,0") != NULL))
	{
		net = No_Network;
		recvErrorFails=0;
	}
	else if((strstr(recvBuffer, "+CNSMOD: 0,1") != NULL))
	{
		net = Network_2G;
		recvErrorFails=0;
	}
        else if((strstr(recvBuffer, "+CNSMOD: 0,2") != NULL))
        {
                net = Network_2G;
		recvErrorFails=0;
        }
        else if((strstr(recvBuffer, "+CNSMOD: 0,3") != NULL))
        {
                net = Network_2G;
		recvErrorFails=0;
        }
        else if((strstr(recvBuffer, "+CNSMOD: 0,4") != NULL))
        {
                net = Network_3G;
		recvErrorFails=0;
        }
	else if((strstr(recvBuffer, "+CNSMOD: 0,5") != NULL))
	{
		net = Network_3G;
		recvErrorFails=0;
	}
	else if((strstr(recvBuffer, "+CNSMOD: 0,6") != NULL))
	{
		net = Network_3G;
		recvErrorFails=0;
	}
	else if((strstr(recvBuffer, "+CNSMOD: 0,7") != NULL))
	{
		net = Network_3G;
		recvErrorFails=0;
	}
	else
	{
		recvErrorFails++;
		if(recvErrorFails>2)
		{
			recvErrorFails=0;
			net = No_Network;
		}
		else
		{
			net = old_net;
		}
		IBD_PRINTF(DEBUG_EMERGENCY, "CNSMOD error value [%s]\n", recvBuffer);
	}
    return 0;
}
int PhoneMonitorVoiceInterface::getNetworkTypeAndServiceType(ServiceType &service)
{
	static int recvServiceTimes=0;
       static int recvServiceErrors=0;
	ServiceType old_service=service;

	service = No_Service;
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return -1;

	char sendBuffer[200];
       char recvBuffer[512];
       int ret = 0;

        sprintf(sendBuffer,"AT+CNSDP?\r\n");
        ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,500,1000);
        //IBD_PRINTF(DEBUG_EMERGENCY, "222AT+CNSDP Recv Ret [%d]-[%s]\n", ret,recvBuffer);
        if(ret<0)
	{
		recvServiceTimes++;
		if(recvServiceTimes<=2)
		{
			service=old_service;
		}		
             else
             {
                    recvServiceTimes=0;
             }
        	return -1;
	}
       recvServiceTimes=0;

    	if((strstr(recvBuffer, "+CNSDP: 0") != NULL))
	{
		service = Phone_Only;
             recvServiceErrors=0;
	}
	else if((strstr(recvBuffer, "+CNSDP: 1") != NULL))
	{
		service = Net_Only;
             recvServiceErrors=0;
	}
	else if((strstr(recvBuffer, "+CNSDP: 2") != NULL))
	{
		service = Phone_And_Net;
             recvServiceErrors=0;
	}
       else
	{
		recvServiceErrors++;
		if(recvServiceErrors>2)
		{
			recvServiceErrors=0;
		}
		else
		{
			service=old_service;
		}
		IBD_PRINTF(DEBUG_EMERGENCY, "CNSDP error value [%s]\n", recvBuffer);
	}

    return 0;
}


int PhoneMonitorVoiceInterface::getNetworkTypeAndServiceType(NetworkType &net, ServiceType &service)
{
	static int recvErrorTimes=0;
	static int recvErrorFails=0;
	static int recvServiceTimes=0;
	NetworkType old_net=net;
	ServiceType old_service=service;


	net = No_Network;
#ifndef _8925_8D1
	service = No_Service;
#endif
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return -1;

	char sendBuffer[200];
       char recvBuffer[512];
       int ret = 0;

#if defined(_8925_8D1)
        sprintf(sendBuffer,"AT+CGATT?\r\n");
#elif defined(ADD_WIFI_TEST)
        sprintf(sendBuffer,"AT^SYSINFO\r\n");//sysinfo
#else
        //sprintf(sendBuffer,"AT+ZPAS?\r\n");
        sprintf(sendBuffer,"AT+CNSMOD?\r\n");
#endif

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,500,3000);

	//IBD_PRINTF(DEBUG_EMERGENCY, "AT+CNSMOD Recv Ret [%d]-[%s]\n", ret,recvBuffer);
	if(ret<0)
	{
		recvErrorTimes++;
		if(recvErrorTimes<=2)
		{
			service=old_service;
			net=old_net;
		}
		else
		{
			recvErrorTimes=0;
		}
		return -1;
	}

	recvErrorTimes=0;

#if defined(_8925_8D1)
	if((strstr(recvBuffer, "+CGATT: 1") != NULL))
	{
                net = Network_2G;
	}

	return 0;
#elif defined(ADD_WIFI_TEST)
        QString sContent;
        sContent.append(recvBuffer); 
	erviceType = 3
      if(listSplit.count() != 7)
        {
                net = Network_4G;
                service = Phone_And_Net;
                return 0;
        }
        if(listSplit[3].toInt() == 3)
                net = Network_2G;
        else if(listSplit[3].toInt() == 5)
            net = Network_3G;
        else if(listSplit[3].toInt() == 15)
            net = Network_3G;
        else if(listSplit[3].toInt() == 17)
            net = Network_4G;


        if(listSplit[1].toInt() == 0)
                service = No_Service;
        else if(listSplit[1].toInt() == 1)
                service = Phone_Only;
        else if(listSplit[1].toInt() == 2)
                service = Net_Only;
        else
                service = Phone_And_Net;
        return 0;
#else
        //do nothing...
#endif


	if((strstr(recvBuffer, "+CNSMOD: 0,0") != NULL))
	{
		net = No_Network;
		recvErrorFails=0;
	}
	else if((strstr(recvBuffer, "+CNSMOD: 0,1") != NULL))
	{
		net = Network_2G;
		recvErrorFails=0;
	}
        else if((strstr(recvBuffer, "+CNSMOD: 0,2") != NULL))
        {
                net = Network_2G;
		recvErrorFails=0;
        }
        else if((strstr(recvBuffer, "+CNSMOD: 0,3") != NULL))
        {
                net = Network_2G;
		recvErrorFails=0;
        }
        else if((strstr(recvBuffer, "+CNSMOD: 0,4") != NULL))
        {
                net = Network_3G;
		recvErrorFails=0;
        }
	else if((strstr(recvBuffer, "+CNSMOD: 0,5") != NULL))
	{
		net = Network_3G;
		recvErrorFails=0;
	}
	else if((strstr(recvBuffer, "+CNSMOD: 0,6") != NULL))
	{
		net = Network_3G;
		recvErrorFails=0;
	}
	else if((strstr(recvBuffer, "+CNSMOD: 0,7") != NULL))
	{
		net = Network_3G;
		recvErrorFails=0;
	}
	else
	{
		recvErrorFails++;
		if(recvErrorFails>2)
		{
			recvErrorFails=0;
			net = No_Network;
		}
		else
		{
			net = old_net;
		}
		IBD_PRINTF(DEBUG_EMERGENCY, "CNSMOD error value [%s]\n", recvBuffer);
	}
    
        sprintf(sendBuffer,"AT+CNSDP?\r\n");
        ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,500,1000);

        if(ret<0)
	{
		recvServiceTimes++;
		if(recvServiceTimes<=2)
		{
			service=old_service;
		}		
        	return -1;
	}

    	if((strstr(recvBuffer, "+CNSDP: 0") != NULL))
	{
		service = Phone_Only;

	}
	else if((strstr(recvBuffer, "+CNSDP: 1") != NULL))
	{
		service = Net_Only;
	}
	else if((strstr(recvBuffer, "+CNSDP: 2") != NULL))
	{
		service = Phone_And_Net;
	}

	/*if((strstr(recvBuffer, "GPRS") != NULL))
	{
		net = Network_2G;
	}
	else if((strstr(recvBuffer, "EDGE") != NULL))
	{
		net = Network_2G;
	}
	else if((strstr(recvBuffer, "GSM") != NULL))
	{
		net = Network_2G;
	}
	else if((strstr(recvBuffer, "No Service") != NULL))
	{
		net = No_Network;
	}
	else if((strstr(recvBuffer, "Limited Service") != NULL))
	{
		net = Limited_Network;
	}
	else if((strstr(recvBuffer, "HSDPA") != NULL))
	{
		net = Network_3G;
	}
	else if((strstr(recvBuffer, "HSUPA") != NULL))
	{
		net = Network_3G;
	}
	else if((strstr(recvBuffer, "UMTS") != NULL))
	{
		net = Network_3G;
	}
	else
	{
		net = No_Network;
		IBD_PRINTF(DEBUG_EMERGENCY, "ZPAS error value [%s]\n", recvBuffer);
	}
	
	if((strstr(recvBuffer, "CS_ONLY") != NULL))
	{
		service = Phone_Only;

	}
	else if((strstr(recvBuffer, "PS_ONLY") != NULL))
	{
		service = Net_Only;
	}
	else if((strstr(recvBuffer, "CS_PS") != NULL))
	{
		service = Phone_And_Net;
	}
	else if((strstr(recvBuffer, "CAMPED") != NULL))
	{
		service = No_Service;
	}
	else
	{
		service = No_Service;
		IBD_PRINTF(DEBUG_EMERGENCY, "ZPAS error value [%s]\n", recvBuffer);
	}*/

	return 0;
}

int PhoneMonitorVoiceInterface::getSKVol()
{
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	return PHONE_MONITOR_FAIL;
}

int PhoneMonitorVoiceInterface::setSKVol(int val)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CLVL=%d\r\n", val);

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}


bool PhoneMonitorVoiceInterface::setMicVol(int val)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED){
		return PHONE_MONITOR_FAIL;
	}
	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CMVL=%d\r\n", val);

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}


int PhoneMonitorVoiceInterface::muteMic()
{
	QMutexLocker locker(&m_mutex);

	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CMUT=1\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);
	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}

int PhoneMonitorVoiceInterface::unMuteMic()
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CMUT=0\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}

int PhoneMonitorVoiceInterface::getMicSatus()
{
	return PHONE_MONITOR_FAIL;
}

void PhoneMonitorVoiceInterface::run()
{
	char recvBuffer[1024];

	while(1)
	{
		if(m_pPhoneConnection->getNextIndication(recvBuffer,1024)<0)
		{
			IBD_PRINTF(DEBUG_EMERGENCY,"voice phone device error.");
			sleep(1);
			continue;
		}

		if(strstr(recvBuffer, "+CMTI") != NULL)//得到一条短消息
		{
			char* tmp;
			int iSmsId;

			if((tmp = strstr(recvBuffer, "\",")) == NULL)//未找到","
			{
				IBD_PRINTF(DEBUG_INFOMATION, "not found '\",' error...\n")
				continue;	
			}	
			else
			{
				if((iSmsId = atoi(tmp+2)) < 0)//解析出短消息的ID
				{
					IBD_PRINTF(DEBUG_INFOMATION, "Get Sms Id error...\n")
					continue;
				}
			}
			m_pWorkThread->appendRecvMsg(iSmsId);//将短消息ID放入队列，等待处理		
		}
		if(strstr(recvBuffer, "CLIP"))
		{
			if(strstr(recvBuffer, "SYNC"))
			{
				char* cpTmp=recvBuffer;

                                if(strsep(&cpTmp, "\"") != NULL)
                                {
                                        char *cpPhoneNumber;
                                        if((cpPhoneNumber = strsep(&cpTmp, "\"")) != NULL)
                                        {
						strncpy(m_phoneNumber,cpPhoneNumber,30);
                                                m_phoneNumber[29]=0;

						m_videoPhone->waitIncommingCall();
                                        }
                                }

			}
			else
			{
				char* cpTmp =recvBuffer;

				if(strsep(&cpTmp, "\"") != NULL)
				{
					char *number=strsep(&cpTmp, "\"");
					if(number != NULL)
					{
						QMutexLocker locker(&m_mutex);
						if(m_callStatus!=CALL_COMING_CALL)
						{
							m_callStatus =CALL_COMING_CALL;

							strncpy(m_phoneNumber,number,30);
							m_phoneNumber[30-1] =0;
							m_pCallback->toNewCall(m_phoneNumber);
						}
					}
				}
			}
		}
        else if(strstr(recvBuffer, "NO CARRIER") != NULL)
        {
            msleep(1500);
            QMutexLocker locker(&m_mutex);
            m_pCallback->toCallHangUp(PHONE_MONITOR_TIMEOUT_HANG);
            printf("*** m_videoPhone->hangUp ***\n");
            m_videoPhone->hangUp();
            m_callStatus =CALL_IDLE;
        }
        else if(strstr(recvBuffer, "VOICE NO CARRIER") != NULL)
		{
#ifndef _8925_8D1
                        //without codes below,when hung up,over_init()(exists in PHONE-UI-CODES) will run twice
                        if(strstr(recvBuffer, ":") == NULL)//for bug : run APP "ppp-on", calling ,the opposition hung up,run AT+CPAS,will get result: 4(state:talking)
                        {                                                                       //VOICE NO CARRIER 
                                                                                                //                      : hung up by self
                                m_pCallback->toCallHangUpBySelf();
                                continue;                                               //VOICE NO CARRIER : ERROR-CODE
                        }                                                                       //                      : hung up because of errors
                        else
                        {   
                        }
#endif

			QMutexLocker locker(&m_mutex);

			m_callStatus =CALL_IDLE;
            m_pCallback->toCallHangUp(PHONE_MONITOR_TIMEOUT_HANG);
			printf("*** m_videoPhone->hangUp ***\n");
			m_videoPhone->hangUp();
		}
#ifdef ADD_WIFI_TEST
		else if(strstr(recvBuffer, "CEND") != NULL)  
		{
			QMutexLocker locker(&m_mutex);
			m_callStatus =CALL_IDLE;
			m_pCallback->toCallHangUp(PHONE_MONITOR_OPPOSITE_HANG);
		}
#else
		else if(strstr(recvBuffer, "VOICE CALL: END") != NULL)  
		{
            msleep(1500);
            QMutexLocker locker(&m_mutex);
            m_callStatus =CALL_IDLE;
            m_pCallback->toCallHangUp(PHONE_MONITOR_OPPOSITE_HANG);
		}
#endif
		else if(strstr(recvBuffer, "HANGUP") != NULL) ////靠靠靠靠靠靠靠靠靠HANGUP
		{	
			QMutexLocker locker(&m_mutex);
            m_pCallback->toCallHangUp(PHONE_MONITOR_OPPOSITE_HANG);
            m_callStatus =CALL_IDLE;
        }
		
		else if(strstr(recvBuffer, "CSDSTOPRING") != NULL)
		{
			continue;
		}
		else if(strstr(recvBuffer, "STOPRING") != NULL)
		{
			QMutexLocker locker(&(m_mutex));
			m_callStatus= CALL_IDLE;
			m_pCallback->toMissCall(m_phoneNumber);
			m_phoneNumber[0]=0;
		}
		else if(strstr(recvBuffer, "MISSED") != NULL)
		{
			QMutexLocker locker(&(m_mutex));
			m_callStatus= CALL_IDLE;
			m_pCallback->toMissCall(m_phoneNumber);
			m_phoneNumber[0]=0;
		}
		else if(strstr(recvBuffer, "NO DIALTONE") != NULL)
		{
			QMutexLocker locker(&m_mutex);
			m_callStatus =CALL_IDLE;
            m_pCallback->toCallHangUp(PHONE_MONITOR_TIMEOUT_HANG);
		}
		else if(strstr(recvBuffer, "RINGBACK") != NULL)
		{
			char *p=strstr(recvBuffer, "RINGBACK");

			if(strncmp(p,"RINGBACKLOCAL",strlen("RINGBACKLOCAL"))!=0)
			{
				QMutexLocker locker(&m_mutex);
				m_pCallback->toRing(m_phoneNumber);
			}
			continue;
		}
#if 1
                else if(strstr(recvBuffer, "MO RING") != NULL)
                {
                        QMutexLocker locker(&m_mutex);
                        m_pCallback->toRing(m_phoneNumber);
                }
                else if(strstr(recvBuffer, "MO CONNECTED") != NULL)
                {
			QMutexLocker locker(&(m_mutex));
			m_callStatus= CALL_TALKING;
			m_pCallback->toCallConnect();
                }
#endif
		else if(strstr(recvBuffer, "BEGIN") != NULL)
		{
			QMutexLocker locker(&(m_mutex));
            m_callStatus= CALL_TALKING;
            m_pCallback->toCallConnect();
		}
		else if(strstr(recvBuffer, "VOICE CALL: BEGIN") != NULL)
		{
			QMutexLocker locker(&(m_mutex));
            m_callStatus= CALL_TALKING;
            m_pCallback->toCallConnect();
		}

#ifndef _8925_8D1
		else if(strstr(recvBuffer, "CONNECT") != NULL)
		{
			QMutexLocker locker(&(m_mutex));
			m_callStatus= CALL_TALKING;
			m_pCallback->toCallConnect();
		}
#endif
#ifdef ADD_WIFI_TEST
		else if(strstr(recvBuffer, "CONN") != NULL)
		{
			QMutexLocker locker(&(m_mutex));
			m_callStatus= CALL_TALKING;
			m_pCallback->toCallConnect();
		}
#endif
		else if(strstr(recvBuffer, "ANSWER") != NULL)
		{
			QMutexLocker locker(&(m_mutex));
			m_callStatus= CALL_TALKING;
			m_pCallback->toCallConnect();
		}
		else if(strstr(recvBuffer, "+ZEND") != NULL)//出现此字段说明模块不能再使用，原因目前不清楚，ZTE怀疑是SIM卡报出的错误
		{
		}
	}
}

