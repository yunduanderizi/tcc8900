#include <math.h>

#include <QMutexLocker> 

#include "sms.h"

#include "debugMonitor.h"

#include "voicePhoneWorkThread.h"
#include "MobileInterFace.h"
#include "phoneConnection.h"

VoicePhoneWorkThreadC::VoicePhoneWorkThreadC()
{
	recvHead=recvEnd=NULL;

	sendHead=sendEnd=NULL;

	m_signalStrength = Strength_0;
	m_networkType = No_Network;
	m_serviceType = No_Service;
}
VoicePhoneWorkThreadC::~VoicePhoneWorkThreadC()
{
}

void VoicePhoneWorkThreadC::init(PhoneMonitorVoiceInterface *voicePhone)
{
	m_voicePhone =voicePhone;
	start();
}

void VoicePhoneWorkThreadC::run()
{
    NetworkType net=No_Network;
    ServiceType service=No_Service;
	SignalStrength signal;
	int signalnum;
	
	while(1)
	{
		m_mutex.lock();
		bool waitResult = m_event.wait(&m_mutex,5*1000);
		m_mutex.unlock();

		if(waitResult==false)
		{
			m_voicePhone->getSignalStrength(signal,signalnum);
			sleep(1);
			m_voicePhone->getNetworkTypeAndServiceType(net);
                    sleep(1);
                    m_voicePhone->getNetworkTypeAndServiceType(service);

			int changeFlag=0;

			if( signal != m_signalStrength || m_networkType != net || m_serviceType != service )
			{
				m_signalStrength = signal;
				m_networkType = net;
#ifdef _8925_8D1
                                service = Phone_And_Net;
                                if(Strength_0 == signal && No_Network == net)
                                {
                                        service = No_Service;
                                }
                                else if(Strength_0 == signal)
                                {
                                        service = Net_Only;
                                }
                                else if(No_Network == net)
                                {
                                        service = Phone_Only;
                                }
#endif
				m_serviceType = service;
				changeFlag=1;
			}
				printf("time is %d m_signalStrength is %d, m_networkType is %d, m_serviceType is %d\n",lastReportTime,signal,net,service);

			if( changeFlag || ((time(NULL)-lastReportTime) >= (10*60))||(0==time(NULL)%3600))
			{
				IBD_PRINTF(DEBUG_INFOMATION, "update signal network type and service type\n")
				
				lastReportTime=time(NULL);

				m_voicePhone->m_pCallback->toSignalAndNetworkTypeAndServiceType(m_signalStrength, m_networkType, m_serviceType,signalnum);
			}
		}

		sendmsgqueue* msgToSend=getSendMsg();
		while(msgToSend)
		{
			int result=sendOneMsg(msgToSend->callNumber, msgToSend->msgContent, msgToSend->serialNo);

			if(result<0)
			{
			}

			free(msgToSend->callNumber);
			free(msgToSend->msgContent);
			free(msgToSend);
			msgToSend=getSendMsg();
		}

		int msgToRecv =getRecvMsg();
		while(msgToRecv>=0)
		{
			int result=recvOneMessage(msgToRecv);
			if(result>=0)
			{
			}
			else
			{
			}

			msgToRecv =getRecvMsg();
		}

	}	
}

void VoicePhoneWorkThreadC::readPreviousMsg()
{
	int MaxSMCount= 0;
	int MinSMCount = 0;
	int rcount=0;

        int ret = PreferredMessage(MinSMCount,MaxSMCount);

        if(ret < 0)
        {
               IBD_PRINTF(DEBUG_INFOMATION, "get sm msg ERROR\n")
        }

        if(MaxSMCount==0)
        {
                MaxSMCount = 100;
                MinSMCount = 50;
        }

        IBD_PRINTF(DEBUG_INFOMATION, "M=%d, min=%d\n",MaxSMCount, MinSMCount)

        while(rcount<MaxSMCount && MinSMCount>0)
        {
               ret = recvOneMessage(rcount);

               if(ret == 0)
               {
                       MinSMCount--;
               }

               rcount ++;
       }
}

int VoicePhoneWorkThreadC::sendOneMsg(char* cpNumber, char* cpTexting, unsigned int id)
{
	char recvBuffer[1000];
	int ret = 0;
	int retval = 0;

	int len = 0;
	unsigned char buf[300];
	unsigned char* strHex;

	SmsSendStruct *pSmsSend = NULL;
	ComposeSubmitSms(pSmsSend, cpNumber, NULL, cpTexting);
	while( pSmsSend )
	{
		strHex = BinToHex((LPBYTE)(pSmsSend->pSendContent), pSmsSend->len);

		ret = m_voicePhone->m_pPhoneConnection->sendSMSCommand((char*)strHex,recvBuffer,1000,35000);
		free(strHex);

		if(ret>=0 && strstr(recvBuffer, "OK") != NULL)
		{

		}
		else
		{
			retval = -1;
		}
		
		SmsSendStruct *pTmp = pSmsSend;
		pSmsSend = pSmsSend->pNext;
		free(pTmp->pSendContent);
		free(pTmp);

	}

	if( retval >= 0 )
	{
		m_voicePhone->m_pCallback->toSmsSendResult(PHONE_MONITOR_SUCCESS);
		m_voicePhone->m_pCallback->toSmsSendWithIdResult(id, PHONE_MONITOR_SUCCESS);
	}
	else
	{
		m_voicePhone->m_pCallback->toSmsSendResult(PHONE_MONITOR_FAIL);
		m_voicePhone->m_pCallback->toSmsSendWithIdResult(id, PHONE_MONITOR_FAIL);
	}

	return retval;
}

int VoicePhoneWorkThreadC::RecvListMessage()    //¶ÁÒÅÁô¶ÌÏûÏ¢^M
{
	char sendBuffer[200];
        char recvBuffer[5000];
        int ret = 0;

	sprintf(sendBuffer, "AT+CMGL=4\r\n");
	ret =m_voicePhone->m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,5000,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		char* tmp, *tmp2;
		int iSmsId;

		tmp = strtok_r(recvBuffer, "\r\n", &tmp2);
		while(1)
		{
			tmp = strtok_r(NULL, "\r\n", &tmp2);
			if(strstr(tmp, "+CMGL:") != NULL)
				iSmsId = atoi(tmp+6);
			else
				break;
			
			IBD_PRINTF(DEBUG_INFOMATION, "iSmsid = %d \n", iSmsId)
			tmp = strtok_r(NULL, "\r\n", &tmp2);
			if(tmp == NULL)
			{
				break;
			}	
			
			analysisMessage(tmp);
			deleteOneMessage(iSmsId);
		}
	}
	else
	{
		return -2;
	}

	return 0;
}

int VoicePhoneWorkThreadC::PreferredMessage(int &MinSMCount,int &MaxSMCount)    //»ñÈ¡SM¿¨¶ÌÐÅÈÝÁ¿
{
	char sendBuffer[200];
        char recvBuffer[1000];
        int ret = 0;

	sprintf(sendBuffer, "AT+CPMS?\r\n");
	ret =m_voicePhone->m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,1000,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		char* tmp, *tmp2, *tmp1;

		if((tmp1 = strstr(recvBuffer, "SM\",")) != NULL)
		{		
			tmp = strtok_r(tmp1, ",", &tmp2);
			tmp = strtok_r(NULL, ",", &tmp2);
			if(tmp != NULL)
				MinSMCount = atoi(tmp);
			else
				MinSMCount = 50;
			
			tmp = strtok_r(NULL, ",", &tmp2);
			if(tmp != NULL)
				MaxSMCount = atoi(tmp);
			else
				MaxSMCount = 100;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -2;
	}

	return 0;
}

int VoicePhoneWorkThreadC::recvOneMessage(int id)        //¶ÁÈ¡¶ÌÏûÏ¢^M
{
	char sendBuffer[200];
        char recvBuffer[1000];
        int ret = 0;

	sprintf(sendBuffer, "AT+CMGR=%d\r\n", id);
	ret =m_voicePhone->m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,1000,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
#ifdef IBD_DEBUG
		//printf("======for debug======>> phone receive a message %s \n",recvBuffer);
#endif
		char* tmp1= NULL;
		char* tmp = NULL;
#ifdef ADD_WIFI_TEST
		char* tmp2 = NULL;
		tmp1 = strstr(recvBuffer, "+CMGR");
		//printf("======for debug======0>> %s \n",tmp1);
		if(tmp1 != NULL)
			tmp = strtok_r(tmp1, "\r\n", &tmp2);
		else
			return -2;
		tmp = strtok_r(tmp2, "\r\n", &tmp1);
		//printf("======for debug======1>> %s \n",tmp);
#else
		tmp = strtok_r(recvBuffer, "\r\n", &tmp1);
		tmp = strtok_r(NULL, "\r\n", &tmp1);
		tmp = strtok_r(NULL, "\r\n", &tmp1);
#endif
		if(tmp == NULL)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "RecvMessage  is  NULL!!\n")
			return -2;
		}		

        analysisMessage(tmp);//解析短信内容，并存储到buf结构体中
		
		if((deleteOneMessage(id)) >= 0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "Delete Message Success ! ! !In SIM %d\n", id)
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "Delete Message Error ! ! !In SIM %d\n", id)
		}
	}
	else 
	{
		IBD_PRINTF(DEBUG_INFOMATION, "RecvMessage Error the resault is %d\n", ret)
		return -1;
	}

	return 0;
}
int VoicePhoneWorkThreadC::deleteOneMessage(int iId)     //É¾³ýSIM¿¨¶ÌÏûÏ¢
{
	char sendBuffer[200];
        char recvBuffer[1000];
        int ret = 0;

	sprintf(sendBuffer,"AT+CMGD=%d\r\n",iId);
	ret =m_voicePhone->m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,1000,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
int VoicePhoneWorkThreadC::analysisMessage(char* buf)
{
    char *tmp=NULL;
    unsigned char* smsc=NULL;
    unsigned char* bin=NULL;
    unsigned char* oa=NULL;
    unsigned char* text=NULL;
    unsigned char* date=NULL;

	tmp = buf;
	bin = HexToBin((unsigned char *)tmp, strlen(tmp));
	DecodeSMS(bin, &smsc, &oa, &text, &date);
	free(bin);

	if( text )
	{
		ReportSMSData	smsData;
		strncpy(smsData.cNumber,(char*)oa,PHONE_MONITOR_NUMBER_LEN);
		smsData.cNumber[PHONE_MONITOR_NUMBER_LEN-1]=0;

        //发现读取到没有日期的短信，段错误 如D760回复的 [zhenyubin 2015/09/10]
        if(date != NULL)
        {
            strncpy(smsData.cSMSTime,(char*)date,50);
        }
		smsData.cSMSTime[50-1]=0;

		smsData.SMSContent = (char*)text;
//strcpy(smsData.SMSContent, (char*)text);

        m_voicePhone->m_pCallback->toNewSMS(smsData);
    }

    return 0;
}

int VoicePhoneWorkThreadC::appendRecvMsg(int id)
{
	QMutexLocker locker(&m_mutex);

	msgqueue *p_buf;

	p_buf = (msgqueue*)malloc(sizeof(msgqueue));

	p_buf->msgid= id;
	p_buf->next = NULL;

	if(recvHead== NULL)
	{
		recvEnd = recvHead =p_buf;
	}
	else
	{
		recvEnd->next = p_buf;
		recvEnd = p_buf;
	}

	m_event.wakeOne();


	return 0;
}
int VoicePhoneWorkThreadC::getRecvMsg()
{
	QMutexLocker locker(&m_mutex);
	msgqueue* tmp =recvHead;

	if(recvHead != NULL)
	{
		if(recvHead == recvEnd)
		{
			recvHead = recvEnd = NULL;
		}
		else
		{
			recvHead = recvHead->next;
		}
	}

	if(tmp!=NULL)
	{
		int msgid=tmp->msgid;
		
		free(tmp);
		return msgid;
	}
	else
	{
		return -1;
	}
}

int VoicePhoneWorkThreadC::appendSendMsg(const char* pcNumber,const char* pcContent, unsigned int serialNo)
{
	QMutexLocker locker(&m_mutex);

	sendmsgqueue *p_buf;
	p_buf = (sendmsgqueue *)malloc(sizeof(sendmsgqueue));

	p_buf->callNumber = (char *)malloc(strlen(pcNumber)+1);
	p_buf->msgContent = (char *)malloc(strlen(pcContent)+1);

	strcpy(p_buf->callNumber, pcNumber);
	strcpy(p_buf->msgContent, pcContent);
	p_buf->serialNo = serialNo;

	p_buf->next = NULL;

	if(sendHead== NULL)
	{
		sendEnd = sendHead =p_buf;
	}
	else
	{
		sendEnd->next = p_buf;
		sendEnd = p_buf;	
	}

	m_event.wakeOne();

	return 0;
}
sendmsgqueue* VoicePhoneWorkThreadC::getSendMsg()
{
	QMutexLocker locker(&m_mutex);
	sendmsgqueue* tmp =sendHead;

	if(sendHead != NULL)
	{
		if(sendHead == sendEnd)
		{
			sendHead = sendEnd = NULL;
		}
		else
		{
			sendHead = sendHead->next;
		}
	}
	return tmp;
}

int VoicePhoneWorkThreadC::sendMessageErrorRebuild()
{
	char sendBuffer[200];
        char recvBuffer[100];
        int ret = 0;

	sprintf(sendBuffer,"\032");
	ret =m_voicePhone->m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,100,2000);
	
	return ret; 
}
