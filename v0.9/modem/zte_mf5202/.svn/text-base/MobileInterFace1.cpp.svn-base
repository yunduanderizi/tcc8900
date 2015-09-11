
#include "audioControl.h"
#include "MobileInterFace1.h"
#include "sendDTMF.h"
#include "phoneConnection.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sqlite3.h"
#include "bsGetTelephone.h"
#include "VDRPhoneTable.h"

#ifdef  SEND_MSG_TO_NAVI
#include "NaviDbusMessage.h"
#endif

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
#include "AudioManageMessage.h"
#endif

#define	ONE_KEY_SERVER_NUMBER	"4006761146"
#define	URGENT_HELP_NUMBER	"4006561210"
#define MAX_CLVL_VAL		(5)


static bool g_bFirstOpen3gAudio = true;


#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
PhoneMonitorVoiceInterface1::PhoneMonitorVoiceInterface1(PhoneMonitorVoiceReportBase *reportBase, BsLocationUploadC *pLocationUpload)
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
PhoneMonitorVoiceInterface1::PhoneMonitorVoiceInterface1(BsLocationUploadC *pLocationUpload)
#else
PhoneMonitorVoiceInterface1::PhoneMonitorVoiceInterface1(AudioCtrlClientC *pAudioCtrlClient, BsLocationUploadC *pLocationUpload)
#endif
#endif
	: m_pInterface1PhoneReport(NULL),
#ifndef WITH_NEW_AUDIO_CONTROL_SYSTEM
#ifndef NEW_FUNCTION_AUDIO_CONTROL_1	
	  m_pAudioCtrlClient(pAudioCtrlClient),
#endif
#endif
	  m_pPhoneAudioControl(NULL),
	  m_pSmsAudioControl(NULL),
	  m_pSendDTMF(NULL),
//	  m_pLocationUpload(pLocationUpload),
//	  m_pBsCommandProcessor(NULL),
	  m_iTimerForTimeout(-1),
	  m_CallNumber(""),
	  m_bDTMFAlwaysSend(false),
	  m_bPhoneAudioOpen(false)
{
#ifndef WITH_NEW_AUDIO_CONTROL_SYSTEM
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object();
#else
	if( NULL == pAudioCtrlClient )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "audio control object is NULL, so abort!\n")
		abort();
	}
#endif

#endif
	/*if( NULL == pLocationUpload )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "location upload object is NULL, so abort!\n")
		abort();
	}*/




#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
	m_pInterface1PhoneReport = new Interface1PhoneReport(this, reportBase);
        
	m_pPhoneAudioControl = new PhoneAudioControlC(reportBase);
	m_pSmsAudioControl = new SmsAudioControlC(reportBase);
#else
	m_pInterface1PhoneReport = new Interface1PhoneReport(this);
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	m_pPhoneAudioControl = new PhoneAudioControlC();
	m_pSmsAudioControl = new SmsAudioControlC();
#else
	m_pPhoneAudioControl = new PhoneAudioControlC(m_pAudioCtrlClient);
	m_pSmsAudioControl = new SmsAudioControlC(m_pAudioCtrlClient);
#endif

#endif
	m_pSendDTMF = new SendDTMFC(this);//PhoneMonitorVoiceInterface::rtDial);
	connect(m_pSendDTMF, SIGNAL(sigFinishedSend()), this, SLOT(slotFinishedSendDTMFMessage()), Qt::QueuedConnection);

/*	m_pBsCommandProcessor = new BsCommandProcessorC();
	qRegisterMetaType<Bs_Command_Type>("Bs_Command_Type");
	connect(m_pBsCommandProcessor, SIGNAL(sigBsCommandProcessResult(Bs_Command_Type, int, void*)), this, SLOT(slotBsCommandProcessResult(Bs_Command_Type, int, void*)), Qt::QueuedConnection);*/

	m_sServiceTelephone = BsGetTelephoneC::object()->getOEMTelephone(BsGetTelephoneC::SERVICETELEPHONE);
	m_sHelpingTelephone = BsGetTelephoneC::object()->getOEMTelephone(BsGetTelephoneC::HELPINGTELEPHONE);

	BsGetTelephoneC::object()->getTelephone(m_BsTelephoneList);
	
	connect(BsGetTelephoneC::object(), SIGNAL(sigTelephoneChanged()), this, SLOT(slotTelephoneChanged()), Qt::QueuedConnection);
}


PhoneMonitorVoiceInterface1::~PhoneMonitorVoiceInterface1()
{
	delete m_pInterface1PhoneReport;
	m_pInterface1PhoneReport = NULL;
	
	delete m_pPhoneAudioControl;
	m_pPhoneAudioControl = NULL;
	
	delete m_pSmsAudioControl;
	m_pSmsAudioControl = NULL;

	delete m_pSendDTMF;
	m_pSendDTMF = NULL;

/*	if( m_pBsCommandProcessor )
	{
		delete m_pBsCommandProcessor;
		m_pBsCommandProcessor = NULL;
	}*/
}

int PhoneMonitorVoiceInterface1::start( PhoneMonitorVoiceReportBase *reportHandle )
{
	IBD_PRINTF(DEBUG_INFOMATION, "PhoneMonitorVoiceInterface1::start [%p]\n", reportHandle)
	m_pInterface1PhoneReport->m_pCallback = reportHandle;

	PhoneMonitorVoiceInterface::start(m_pInterface1PhoneReport);

	return PHONE_MONITOR_SUCCESS;
}

//SMS短信回复信号，本来可以在此类中直接发送短信，为了保持处理短信命令一致，都调用一个类中逻辑 [zhenyubin 2015/09/09]
void PhoneMonitorVoiceInterface1::sendCommandSMSReply(SMS_CMD_STRUCT send)
{
    if(m_pInterface1PhoneReport)
    {
        m_pInterface1PhoneReport->sendCommandSMSReply(send);
    }
}

int PhoneMonitorVoiceInterface1::pickUp()
{
	int retval = PhoneMonitorVoiceInterface::pickUp();
	if( PHONE_MONITOR_SUCCESS == retval )
	{
		openPhoneAudio();
	}

        fprintf(stderr,"...%d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);
	m_pPhoneAudioControl->stopPlay();
        fprintf(stderr,"...%d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);

	return retval;
}

int PhoneMonitorVoiceInterface1::hangUp()
{
	m_pPhoneAudioControl->stopPlay();

	closePhoneAudio();
	
	m_pInterface1PhoneReport->m_bIsAutoCallbackMode = false;

	if( -1 != m_iTimerForTimeout )
	{
		killTimer(m_iTimerForTimeout);
		m_iTimerForTimeout = -1;
	}
	
	slotFinishedSendDTMFMessage();

	m_CallNumber = "";

	return PhoneMonitorVoiceInterface::hangUp();
}

bool PhoneMonitorVoiceInterface1::FaceTime_PickUp()
{
	m_pPhoneAudioControl->stopPlay();

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_pInterface1PhoneReport->m_pCallback->setPhoneMute(false, IBAC_DEV_ID_DAI_VIDEO);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_MPLAYER, false);
#else
	m_pAudioCtrlClient->openAudio(IBAC_DEV_ID_DAI_VIDEO, -1);//maybe use 10???
#endif

#endif
	return PhoneMonitorVoiceInterface::FaceTime_PickUp();
}

bool PhoneMonitorVoiceInterface1::FaceTime_HangUp()
{
	m_pPhoneAudioControl->stopPlay();

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_pInterface1PhoneReport->m_pCallback->setPhoneMute(true, IBAC_DEV_ID_DAI_VIDEO);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->close(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_MPLAYER, false);
#else
	m_pAudioCtrlClient->closeAudio(IBAC_DEV_ID_DAI_VIDEO);
#endif

#endif
	return PhoneMonitorVoiceInterface::FaceTime_HangUp();
}
	
int PhoneMonitorVoiceInterface1::baseHangUp()
{
	return PhoneMonitorVoiceInterface::hangUp();
}

int PhoneMonitorVoiceInterface1::mutePhoneAudio()
{
	m_pPhoneAudioControl->mute();

	return PHONE_MONITOR_SUCCESS;
}

int PhoneMonitorVoiceInterface1::unMutePhoneAudio()
{
	m_pPhoneAudioControl->unMute();

	return PHONE_MONITOR_SUCCESS;
}

int PhoneMonitorVoiceInterface1::dialCallback(const char* pNumber, const QStringList sCallbackNumber)
{
	if( m_pInterface1PhoneReport->m_bIsAutoCallbackMode )
		return PHONE_MONITOR_IN_AUTOCALLBACK_MODE;

	m_CallNumber = "";
	
	IBD_PRINTF(DEBUG_INFOMATION, "start auto call back [%s] !\n", pNumber)
	for(int i=0; i<sCallbackNumber.count(); i++)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "call back number %d     [%s]!\n", i, sCallbackNumber[i].toLocal8Bit().data())
	}
		
	int retval = PhoneMonitorVoiceInterface::dial(pNumber);
	if( retval >= 0 )
	{
		m_pInterface1PhoneReport->m_sCallbackNumber = sCallbackNumber;
		m_pInterface1PhoneReport->m_bIsAutoCallbackMode = true;
		if( -1 != m_iTimerForTimeout )
		{
			killTimer(m_iTimerForTimeout);
			m_iTimerForTimeout = -1;
		}
		m_iTimerForTimeout = startTimer(120*1000);
	}

	return retval;
}

void PhoneMonitorVoiceInterface1::timerEvent(QTimerEvent * event)
{
	if( m_iTimerForTimeout == event->timerId() )
	{
		hangUp();

		m_pInterface1PhoneReport->callHangUp(0);
	}
}

void PhoneMonitorVoiceInterface1::sendLocationMessage()
{
	float fX, fY;
	int iTime;
	int iOrientation;
	int iVelocity;

#if 0
	if( !m_bDTMFAlwaysSend )
	{
		if( m_pLocationUpload->preSendIsSuccess() )
			return;
	}

//send 22 byte data
/*
	if( BS_E_OK == m_pLocationUpload->getLocationInfo(fX, fY, iTime, iOrientation, iVelocity) )
	{
		m_pPhoneAudioControl->playDTMFAudio();

		char tmp[100];
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%010.6f%09.6f", fX, fY);
		char content[100];
		memset(content, 0, sizeof(content));
		strcpy(content, "*");
		unsigned char checknum = 0;
		for(int i=0, j=1; tmp[i]!=0; i++)
		{
			if( tmp[i] != '.' )
			{
				content[j++] = tmp[i];
				checknum += (unsigned char)(tmp[i]);
			}
		}

		char checkchar[4];
		sprintf(checkchar, "%d%d%d", checknum/100, (checknum%100)/10, checknum%10);

		strcat(content, checkchar);
		strcat(content, "#");

		IBD_PRINTF(DEBUG_INFOMATION, "DTMF upload content[%s]!\n", content)
		
		muteMic();
		m_pSendDTMF->send(content);
	}
*/

//send 13 byte data
	if( BS_E_OK == m_pLocationUpload->getLocationInfo(fX, fY, iTime, iOrientation, iVelocity) )
	{
		m_pPhoneAudioControl->playDTMFAudio();

		char tmp[100];
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%07.4f%07.4f", (fX-73), fY);
		char content[100];
		memset(content, 0, sizeof(content));
		unsigned char checknum = 0;
		for(int i=0, j=0; tmp[i]!=0; i++)
		{
			if( tmp[i] != '.' )
			{
				content[j++] = tmp[i];
				checknum += (unsigned char)(tmp[i]);
			}
		}

		char checkchar[4];
		sprintf(checkchar, "%d", checknum%10);

		strcat(content, checkchar);

		IBD_PRINTF(DEBUG_INFOMATION, "DTMF upload content[%s]!\n", content)
		
		muteMic();
		m_pSendDTMF->send(content);
	}
#endif

}

void PhoneMonitorVoiceInterface1::slotFinishedSendDTMFMessage()
{
	IBD_PRINTF(DEBUG_INFOMATION, "DTMF upload finish!\n")
	
	m_pSendDTMF->stopSend();

	unMuteMic();
}

/*void PhoneMonitorVoiceInterface1::slotBsCommandProcessResult(Bs_Command_Type type, int result, void* pRetData)
{
	int retval;
	Bs_Command_AGENT_RetData *pRetDataForAGENT = NULL;

	switch(type)
	{
		case BST_AGENT_SMS:
			pRetDataForAGENT = (Bs_Command_AGENT_RetData*)pRetData;
			if( result == 0 && pRetDataForAGENT )
			{
				retval = PhoneMonitorVoiceInterface::sendSMS(pRetDataForAGENT->pSmsContent, pRetDataForAGENT->pMobileNumber);
				if( retval < 0 )
					result = -1;
				delete pRetDataForAGENT;
			}

			m_pInterface1PhoneReport->bsCommandProcessResult(type, result, NULL);
			break;
			
		default:
			m_pInterface1PhoneReport->bsCommandProcessResult(type, result, pRetData);
			break;
	}
}*/

void PhoneMonitorVoiceInterface1::slotTelephoneChanged()
{
	m_sServiceTelephone = BsGetTelephoneC::object()->getOEMTelephone(BsGetTelephoneC::SERVICETELEPHONE);
	m_sHelpingTelephone = BsGetTelephoneC::object()->getOEMTelephone(BsGetTelephoneC::HELPINGTELEPHONE);
	BsGetTelephoneC::object()->getTelephone(m_BsTelephoneList);
}

int PhoneMonitorVoiceInterface1::dial(const char* cpNumber)
{
#ifdef	IBD_DEBUG 
	if( 0 == strcmp(cpNumber, "111222333100") )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "function number to set DTMF always send, number[%s]!\n", cpNumber)
		m_bDTMFAlwaysSend = true;
		return 0;
	}
	else if( 0 == strcmp(cpNumber, "111222333101") )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "function number to set DTMF not always send, number[%s]!\n", cpNumber)
		m_bDTMFAlwaysSend = false;
		return 0;
	}
	else if( 0 == strcmp(cpNumber, "111222333102") )
	{
		char sendBuffer[100];
		char recvBuffer[100];
		sprintf(sendBuffer, "AT+ZSNT=0,0,0\r\n");

		m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,0);
		return 0;
	}
	else if( 0 == strcmp(cpNumber, "111222333103") )
	{
		char sendBuffer[100];
		char recvBuffer[100];
		sprintf(sendBuffer, "AT+ZSNT=1,0,0\r\n");

		m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,0);
		return 0;
	}
	else
	{

	}
#endif

	for(int loop=0; loop<m_BsTelephoneList.count(); loop++)
	{
		if( 0 == m_BsTelephoneList[loop].phoneNumber.compare(cpNumber) )
		{
			if( TCT_PSTN_CALL_BACK == m_BsTelephoneList[loop].callType )
			{
				return dialCallback(m_BsTelephoneList[loop].phoneNumber.toLocal8Bit().data(), m_BsTelephoneList[loop].callBackNumber);
			}
		}
	}
	
	m_CallNumber = cpNumber;

#if defined(_8925_VERSION) && !defined(WITH_NEW_AUDIO_CONTROL_SYSTEM)
#ifdef _8925_8D1
    system("amixer cset numid=32 on");
#endif
    system("amixer cset numid=12 28");
#endif
	return PhoneMonitorVoiceInterface::dial(cpNumber);
}

#if 0
int PhoneMonitorVoiceInterface1::setNewCallRing(const char* cpNameWithPath)
{
	int fd = 0;
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = "delete from CallRing";
	char pInsertTablesSel[128] = {0};

	fprintf(stdout, "in setNewCallRing func\n");

	if (strcmp(cpNameWithPath, "") == 0)
	{
		printf("in strcmp func\n");
		return PHONE_MONITOR_INVALID_FILE; 
	}

	if ((fd = open(cpNameWithPath, O_RDONLY)) == -1)
	{
		printf("open music file error\n");
		return PHONE_MONITOR_INVALID_FILE; 
	}
	close(fd);

	ret = sqlite3_open(FILE_NAME, &pDB);
	if (ret != SQLITE_OK)
	{
		printf("open Service.db error\n");
		IBD_PRINTF(DEBUG_INFOMATION, "%s :can't open database : %s!\n", FILE_NAME, sqlite3_errmsg(pDB))	
		return PHONE_MONITOR_SAVE_DATA_ERROR;
	}

	//准备执行语句
	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		printf("sqlite3_prepare error\n");
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		return PHONE_MONITOR_SAVE_DATA_ERROR;
	}

	//执行记录集
	while (1)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}

	//释放记录集并关闭数据库
	sqlite3_finalize(pStmt);

	//插入记录条
	sprintf(pInsertTablesSel, "insert into CallRing values('%s')", cpNameWithPath);

	//准备插入数据
	ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
       if (ret != SQLITE_OK)
	{
		printf("insert data to Service.db error\n");
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))

		return PHONE_MONITOR_SAVE_DATA_ERROR;
	}

	//执行绑定以后的语句
	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))

		return PHONE_MONITOR_SAVE_DATA_ERROR;
	}

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	printf("\n---------- save music file ok! \n");
	return PHONE_MONITOR_SUCCESS;
}
#endif

int PhoneMonitorVoiceInterface1::queryModuleVersion(QString& moduleVerion)
{
	char sendBuffer[200];
       char recvBuffer[200];

       sprintf(sendBuffer, "AT+CGMR\r\n");

	int ret = m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && strstr(recvBuffer,"OK") != NULL && strstr(recvBuffer,"END") == NULL)
	{
		char* pBegin;
		char* pEnd;
		pBegin = strstr(recvBuffer, "\n");
		pEnd = strstr(pBegin, "\r");
		if( NULL!=pBegin && NULL!=pEnd )
		{
			*pEnd = 0;
			pBegin++;
			moduleVerion = pBegin;
			return PHONE_MONITOR_SUCCESS;
		}		
	}

	return PHONE_MONITOR_FAIL;
}

void PhoneMonitorVoiceInterface1::reset3gModule()
{
	char sendBuffer[200];
       char recvBuffer[200];
	int ret = 0;
		
	sprintf(sendBuffer, "AT+ZRST\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,0);

	return;
}

int PhoneMonitorVoiceInterface1::set3GModuleAudioOutputVolume(int val)
{
	char sendBuffer[200];
	char recvBuffer[200];
	int ret = 0;
	int tmp = (val < 0) ? 0 : ((val > MAX_CLVL_VAL) ? MAX_CLVL_VAL : val );

	if(CALL_TALKING != getCallState())
	{
		return -1;
	}
	else
	{
	}

	sprintf(sendBuffer, "AT+CLVL=%d\r\n",tmp);
	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,0);

	if(ret < 0)
	{
		return -1;
	}
	else if(strstr(recvBuffer,"OK") == NULL)
	{
		return -1;
	}
	else
	{
	}

	return 0;
}

void PhoneMonitorVoiceInterface1::openPhoneAudio()
{
	if( !m_bPhoneAudioOpen )
	{
		if( g_bFirstOpen3gAudio )
		{
			usleep(1000000);
			g_bFirstOpen3gAudio = false;
		}
				
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
                m_pInterface1PhoneReport->m_pCallback->setPhoneMute(false, IBAC_DEV_ID_3G_SWITCH);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
		AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_ANALOG_PHONE, false);
#else
		m_pAudioCtrlClient->openAudio(IBAC_DEV_ID_3G_PHONE_SPEAKER, -1);
		m_pAudioCtrlClient->adjustAudioVol(IBAC_DEV_ID_3G_PHONE_SPEAKER,30);
#endif
#endif
		m_bPhoneAudioOpen = true;
	}
}

void PhoneMonitorVoiceInterface1::closePhoneAudio()
{
	if( m_bPhoneAudioOpen )
	{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
                m_pInterface1PhoneReport->m_pCallback->setPhoneMute(true, IBAC_DEV_ID_3G_SWITCH);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
		AudioControl1::AudioControlLibC::object()->close(AudioControl1::AUDIO_CHANNEL_ID_ANALOG_PHONE, false);
#else
		m_pAudioCtrlClient->closeAudio(IBAC_DEV_ID_3G_PHONE_SPEAKER);
#endif
#endif
		m_bPhoneAudioOpen = false;
	}
}

int PhoneMonitorVoiceInterface1::searchLocationOfFriend(const char* pFriendNumber)
{
	char smsContent[640];

	strcpy(smsContent, "[TU-CMD][1][102030][WLOCAL][E]A");
	
	return PhoneMonitorVoiceInterface::sendSMS(smsContent, pFriendNumber);
}
	
int PhoneMonitorVoiceInterface1::sendLocationOfMineToFriend(const char* pFriendNumber)
{
	float fX, fY;
	char smsContent[640];
	int iTime;
	int iOrientation;
	int iVelocity;

#if 0
	if( BS_E_OK == m_pLocationUpload->getLocationInfo2(fX, fY, iTime, iOrientation, iVelocity) )
	{
		sprintf(smsContent, "[TU-CMD][1][102030][LOCAL]%f,%f[E]A", fX, fY);
		
		IBD_PRINTF(DEBUG_INFOMATION, "where am i [%s]\n", smsContent)
		return PhoneMonitorVoiceInterface::sendSMS(smsContent, pFriendNumber);
	}
	else
	{
		return PHONE_MONITOR_CANNOT_GET_MY_LOCATION;
	}
#else
		return PHONE_MONITOR_CANNOT_GET_MY_LOCATION;
#endif
}
#ifdef ENABLE_FLY_MODE
int PhoneMonitorVoiceInterface1::setFlyMode(FLY_MODE mode)
{
        if(MIN_FUN > mode || mode > RESERVE_FUN)
        {    
                return -1;
        }    
        else 
        {    
        }    

        int ret = 0; 
        int bufferLen = 100; 
        char sendBuffer[bufferLen];
        char recvBuffer[bufferLen];
        memset(sendBuffer, 0, sizeof(sendBuffer));
        memset(recvBuffer, 0, sizeof(recvBuffer));
        switch(mode)
        {    
                case MIN_FUN:
                {    
                        sprintf(sendBuffer, "AT+CFUN=0\r\n");
                        ret = m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,bufferLen,0);
                        if(ret < 0 || NULL != strstr(recvBuffer, "ERROR"))
                        {    
                                return -1;
                        }    
                        else 
                        {    
                                return 0;
                        }    
                        break;
                }    
                case FULL_FUN:
                {    
                        sprintf(sendBuffer, "AT+CFUN=1\r\n");
                        ret = m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,bufferLen,0);
                        if(ret < 0 || NULL != strstr(recvBuffer, "ERROR"))
                        {    
                                return -1;
                        }    
                        else 
                        {    
                                return 0;
                        }    
                            
                        break;
                }
                case DISABLE_RF_FUN:
                {
                        sprintf(sendBuffer, "AT+CFUN=4\r\n");
                        ret =  m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,bufferLen,0);
                        if(ret < 0 || NULL != strstr(recvBuffer, "ERROR"))
                        {
                                return -1;
                        }
                        else
                        {
                                return 0;
                        }

                        break;
                }
                case RESERVE_FUN:
                {
                        sprintf(sendBuffer, "AT+CFUN=5\r\n");
                        ret =  m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,bufferLen,0);
                        if(ret < 0 || NULL != strstr(recvBuffer, "ERROR"))
                        {
                                return -1;
                        }
                        else
                        {
                                return 0;
                        }

                        break;
                }
                default:
                {
                        sprintf(sendBuffer, "AT+CFUN=1\r\n");
                        ret =  m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,bufferLen,0);
                        if(ret < 0 || NULL != strstr(recvBuffer, "ERROR"))
                        {
                                return -1;
                        }
                        else
                        {
                                return 0;
                        }
                        break;
                }
        }
}
#endif









#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
Interface1PhoneReport::Interface1PhoneReport(PhoneMonitorVoiceInterface1 *pFather, PhoneMonitorVoiceReportBase *reportBase)
	: m_pCallback(reportBase),
#else
Interface1PhoneReport::Interface1PhoneReport(PhoneMonitorVoiceInterface1 *pFather)
	: m_pCallback(NULL),
#endif
	  m_sCallbackNumber(""),
	  m_bIsAutoCallbackMode(false),
	  m_pFather(pFather)
{
    sendCmdPhone="";
        closeAudio = false;
}

Interface1PhoneReport::~Interface1PhoneReport()
{

}

void Interface1PhoneReport::newCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
        if(m_pCallback && IS_FREIGHTAGE_MANAGER_PHONE_NUMBER == m_pCallback->freightageManager(cPhoneNumber))
        {
                return;
        }
        else if( m_bIsAutoCallbackMode )
	{
                fprintf(stderr,"...%d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);
		if( m_sCallbackNumber.contains(cPhoneNumber) )
		{
			int retval = m_pFather->pickUp();

			if( PHONE_MONITOR_SUCCESS == retval )
			{
				m_pFather->m_pPhoneAudioControl->stopPlay();

				m_pFather->openPhoneAudio();

				//m_pFather->sendLocationMessage();
					
				m_bIsAutoCallbackMode = false;

				if( -1 != m_pFather->m_iTimerForTimeout )
				{
					killTimer(m_pFather->m_iTimerForTimeout);
					m_pFather->m_iTimerForTimeout = -1;
				}
		
				IBD_PRINTF(DEBUG_INFOMATION, "auto call back success!\n")

				if( m_pCallback )
					m_pCallback->callConnect();
			}
			else
			{
				m_pFather->hangUp();

				if( m_pCallback )
					m_pCallback->callHangUp(0);
			}
		}
		else
		{
			m_pFather->baseHangUp();
		}
	}
	else
	{
        fprintf(stderr,"...%d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);
		m_pFather->m_pPhoneAudioControl->playIncomingCallAudio();
		if( m_pCallback )
			m_pCallback->newCall(cPhoneNumber);
	}
        fprintf(stderr,"...%d,%s,%s...\n",__LINE__,__FUNCTION__,__FILE__);
}

void Interface1PhoneReport::callConnect( void )
{
//	if( m_pFather->m_CallNumber.compare(ONE_KEY_SERVER_NUMBER) == 0 ||
//	    m_pFather->m_CallNumber.compare(URGENT_HELP_NUMBER) == 0 )
	if( m_pFather->m_CallNumber.compare(m_pFather->m_sServiceTelephone) == 0 ||
		m_pFather->m_CallNumber.compare(m_pFather->m_sHelpingTelephone) == 0 )
	{
		//m_pFather->sendLocationMessage();
		m_pFather->m_CallNumber = "";
	}

        if(false == closeAudio)
        {
	        m_pFather->openPhoneAudio();
        }
        else
        {
        }
	if( m_pCallback )
		m_pCallback->callConnect();

//#if 0//commented for bug:main volume of voice was 15,called,then warning came,call's volume would be lower
#if defined(_8925_VERSION) && !defined(WITH_NEW_AUDIO_CONTROL_SYSTEM)
#ifdef _8925_8D1
    system("amixer cset numid=32 on");
#endif
    system("amixer cset numid=12 28");
#endif
//#endif
}

void Interface1PhoneReport::callHangUp( int reason )
{
	if( m_bIsAutoCallbackMode )
	{
		m_pFather->m_CallNumber = "";
	}
	else
	{
		m_pFather->slotFinishedSendDTMFMessage();

		m_pFather->m_pPhoneAudioControl->stopPlay();

		m_pFather->closePhoneAudio();
	
		if( m_pCallback )
			m_pCallback->callHangUp(reason);
	}
}

void Interface1PhoneReport::missCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN] )
{
	if( m_bIsAutoCallbackMode )
	{
		if( m_sCallbackNumber.contains(cPhoneNumber) )
		{
			m_pFather->hangUp();

			if( m_pCallback )
				m_pCallback->callHangUp(0);
		}
		else
		{
			//do nothing
		}
	}
	else
	{
		if( m_pCallback )
			m_pCallback->missCall(cPhoneNumber);
	}
}

void Interface1PhoneReport::newFaceTimeCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	m_pFather->m_pPhoneAudioControl->playIncomingCallAudio();

	if( m_pCallback )
		m_pCallback->newFaceTimeCall(cPhoneNumber);
}

void Interface1PhoneReport::connectFaceTimeCall(void)  
{
	if( m_pCallback )
		m_pCallback->connectFaceTimeCall();
}
void Interface1PhoneReport::hangupBySelf(void)
{
	if( m_pCallback )
		m_pCallback->hangupBySelf();
}

void Interface1PhoneReport::endFaceTimeCall(int reason)   
{
	m_pFather->m_pPhoneAudioControl->stopPlay();

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_pCallback->setPhoneMute(true, IBAC_DEV_ID_DAI_VIDEO);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->close(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_MPLAYER, false);
#else
	m_pFather->m_pAudioCtrlClient->closeAudio(IBAC_DEV_ID_DAI_VIDEO);
#endif
#endif
	if( m_pCallback )
		m_pCallback->endFaceTimeCall(reason);
	//printf("\n*** abort phone ***\n");	
    //abort();
}

void Interface1PhoneReport::missFaceTimeCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	if( m_pCallback )
		m_pCallback->missFaceTimeCall(cPhoneNumber);
}

static int _insertMsgToDatabase(const ReportSMSData *pData)
{
	int ret = 0; 
    int id = -1;
    //teleBookInfo teleBook;
    VDRPhoneRecieveSMSTable phoneReceiveSMSTable;

	if(NULL == pData)
	{
		return -1;
	}
	else
	{
	}

	ret = phoneReceiveSMSTable.GetRowCount();
	if(ret < 0)
	{
		return ret;
	}
	else if(0 == ret)
	{
		id = 1;
	}
	else
	{
		ret = phoneReceiveSMSTable.RefreshFirstByRowId(false);
		if(ret < 0)
		{
			return ret;
		}
		else
		{
		}
		ret = phoneReceiveSMSTable.GetIntValue(VDR_RECIEVE_SMS_ID,id);
		if(ret < 0)
		{
			return ret;
		}
		else
		{
			id++;
		}
		
	}

	ret = phoneReceiveSMSTable.SetIntValue(VDR_RECIEVE_SMS_ID, id);
	if(ret < 0)
	{
		return ret;
	}
	else
	{
	}
	ret = phoneReceiveSMSTable.SetStringValue( VDR_FROM_PHONE, pData->cNumber);
	if(ret < 0)
	{
		return ret;
	}
	else	
	{
	}
	ret = phoneReceiveSMSTable.SetStringValue( VDR_TIME, pData->cSMSTime);
	if(ret < 0)
	{
		return ret;
	}
	else	
	{
	}
	ret = phoneReceiveSMSTable.SetStringValue( VDR_CONTENT, pData->SMSContent);
	if(ret < 0)
	{
		return ret;
	}
	else	
	{
	}
	ret = phoneReceiveSMSTable.SetStringValue( VDR_POI_NAME, "");
	if(ret < 0)
	{
		return ret;
	}
	else	
	{
	}
	ret = phoneReceiveSMSTable.SetFloatValue(VDR_DLON, 0.0);
	if(ret < 0)
	{
		return ret;
	}
	else	
	{
	}
	ret = phoneReceiveSMSTable.SetFloatValue(VDR_DLAT, 0.0);
	if(ret < 0)
	{
		return ret;
	}
	else	
	{
	}
	ret = phoneReceiveSMSTable.SetIntValue(VDR_EVENT_TYPE, 0);
	if(ret < 0)
	{
		return ret;
	}
	else
	{
	}
	ret = phoneReceiveSMSTable.SetIntValue(VDR_STATUS, 0);
	if(ret < 0)
	{
		return ret;
	}
	else
	{
	}
	ret = phoneReceiveSMSTable.SetIntValue(VDR_BOOK_ID, 0);
	if(ret < 0)
	{
		return ret;
	}
	else
	{
	}
	 
	ret = phoneReceiveSMSTable.Commit(false);
	if(ret < 0)
	{
		return ret;
	}
	else
	{
	}
	
	
	return 0;

#if 0
	char* ErrMsg = NULL;
	sqlite3 *pDB = NULL;
	int ret;

	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Fail to Open the DB file!\n")
		return -1;
	}

	QString Sentence;
	Sentence.sprintf("INSERT INTO PoiMsg(FromPhone, Status, Time, PoiName, Desc, Longitude, Latitude, EventType) VALUES('%s', '%d', '%s', '%s', '%s', '%f', '%f', '%d')",
			pData->cNumber, 0, pData->cSMSTime,
			"", pData->SMSContent, 0.0,
			0.0, 0);

	ret = sqlite3_exec(pDB, Sentence.toUtf8().data(), 0, 0, &ErrMsg);//
	if (ret == SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "insert Msg successful!\n")
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "insert Msg error: %s\n",sqlite3_errmsg(pDB));
		sqlite3_close(pDB);

		return -1;
	}

	sqlite3_close(pDB);

	return 0;
#endif

}

//SMS短信回复 [zhenyubin 2015/09/09]
void Interface1PhoneReport::sendCommandSMSReply(SMS_CMD_STRUCT send)
{
    if(send.type == QUERY_CMD_TYPE)
    {
        QString reply=cmdProtocol.packageSmsCommand(send,false);
        sendCmdPhone.sprintf("%s",send.phone);
        m_pFather->sendSMS(reply.toUtf8().data(),send.phone);
    }
    else
    {
        QString reply=cmdProtocol.packageSmsCommand(send,true);
        sendCmdPhone.sprintf("%s",send.phone);
        m_pFather->sendSMS(reply.toUtf8().data(),send.phone);
    }
}

void Interface1PhoneReport::newSMS( ReportSMSData  SMSData )
{
#ifdef IBD_DEBUG
		printf("======for debug======>> new SMS, content[%s] number[%s]\n", SMSData.SMSContent, SMSData.cNumber);
#endif

	char smsTime[50];
	strcpy(smsTime, SMSData.cSMSTime);
	strcpy(SMSData.cSMSTime, "20");
	strcat(SMSData.cSMSTime, smsTime);

	if( strncmp(SMSData.cNumber, "+86", 3) == 0 )
	{
		strcpy(SMSData.cNumber, SMSData.cNumber+3);
	}
	/*if( m_pFather->m_pBsCommandProcessor )
	{
		bool isBsCommand = m_pFather->m_pBsCommandProcessor->FilterBsSms(SMSData.SMSContent, SMSData.cNumber);
		if( isBsCommand )
		{
			return;
		}
	}*/
#ifdef  SEND_MSG_TO_NAVI
                if(true == isSMSToNavi(SMSData.SMSContent))
                {
                        if( m_pCallback )
                        {
                                m_pCallback->sendMSGToNavi(SMSData.SMSContent);
                        }
                        else
                        {
                        }
                        //parseSMS(SMSData.SMSContent);
                        free(SMSData.SMSContent);
                        return;
                        
                }
                else
#endif
                if(true == isSMSToReg(SMSData.SMSContent))
                {
                        if( m_pCallback )
                        {
                                m_pCallback->newSMSToReg(SMSData.SMSContent);
                        }
                        else
                        {
                        }

                        free(SMSData.SMSContent);
                        return;
                }
                else if(m_pCallback && true == m_pCallback->newSMSToSetup(SMSData))
                {
                        free(SMSData.SMSContent);
                        return;
                }
                else
                {
                    //处理远程短信指令查询设置 [zhenyubin 2015/09/09]
                    if(m_pCallback)
                    {
                        char sendContent[100]={0};
                        SMS_CMD_STRUCT recvStruct;
                        SMS_CMD_PARSE_RESULT result;

                        //电话号码
                        memset(recvStruct.phone,0,PHONE_MONITOR_NUMBER_LEN);
                        strcpy(recvStruct.phone,SMSData.cNumber);

                        result=cmdProtocol.parseSmsCommand(SMSData,recvStruct);
                        if(result == OTHER_ERROR_PARSE) //不是命令短信
                        {
                             ;
                        }
                        else if(result == MODIFY_PHONE_PARSE) //超级密码修改短信指令号码 [zhenyubin 2015/09/02]
                        {
                            QString reply=cmdProtocol.packageSmsCommand(recvStruct,true);
                            m_pFather->sendSMS(reply.toUtf8().data(),recvStruct.phone);
                            sendCmdPhone.sprintf("%s",recvStruct.phone);
                            free(SMSData.SMSContent);
                            return;
                        }
                        else if(result == PASSWD_ERROR_PARSE)
                        {
                            sprintf(sendContent,"Password error");
                            m_pFather->sendSMS(sendContent,SMSData.cNumber);
                            sendCmdPhone.sprintf("%s",SMSData.cNumber);
                            free(SMSData.SMSContent);
                            return;
                        }
                        else if(result == UNKNOW_CMD_PARSE)
                        {
                            sprintf(sendContent,"Unknow Cmd");
                            m_pFather->sendSMS(sendContent,SMSData.cNumber);
                            sendCmdPhone.sprintf("%s",SMSData.cNumber);
                            free(SMSData.SMSContent);
                            return;
                        }
                        else if(result == FORMAT_ERROR_PARSE)
                        {
                            sprintf(sendContent,"Format error");
                            m_pFather->sendSMS(sendContent,SMSData.cNumber);
                            sendCmdPhone.sprintf("%s",SMSData.cNumber);
                            free(SMSData.SMSContent);
                            return;
                        }
                        else if(result == SUCCESS_PARSE) //成功收到ST或RD指令
                        {
                            m_pCallback->newCommandSMS(recvStruct);
                            free(SMSData.SMSContent);
                            return;
                        }
                    }
                }

		_insertMsgToDatabase(&SMSData);
	
	m_pFather->m_pSmsAudioControl->playAudio();
	
	if( m_pCallback )
		m_pCallback->newSMS(SMSData);
	else
		free(SMSData.SMSContent);
}

void Interface1PhoneReport::smsSendResult( int result )
{
#ifdef IBD_DEBUG
    printf("======for debug======>>[%s] sms send result [%d]\n",sendCmdPhone.toLatin1().data(), result);
#endif

    //当是查询设置参数命令反馈回复的信息时，不向应用层汇报发送结果 [zhenyubin 2015/09/09]
    if((sendCmdPhone != "") && (sendCmdPhone.length()>10))
    {
        sendCmdPhone="";
        return;
    }
	if( m_pCallback )
		m_pCallback->smsSendResult(result);
}

void Interface1PhoneReport::smsSendWithIdResult(unsigned int id, int result)
{
	if( m_pCallback )
		m_pCallback->smsSendWithIdResult(id, result);
}

void Interface1PhoneReport::signalAndNetworkType( int signal,int networkType )	
{
	IBD_PRINTF(DEBUG_INFOMATION, "signalAndNetworkType m_pCallback[%p]\n", m_pCallback)

	if( m_pCallback )
		m_pCallback->signalAndNetworkType(signal, networkType);
}

void Interface1PhoneReport::signalAndNetworkTypeAndServiceType(SignalStrength signal, NetworkType network, ServiceType service,int signal_strength)
{
	IBD_PRINTF(DEBUG_INFOMATION, "signalAndNetworkTypeAndServiceType m_pCallback[%p]\n", m_pCallback)
	
	if( m_pCallback )
		m_pCallback->signalAndNetworkTypeAndServiceType(signal, network, service,signal_strength);
}

void Interface1PhoneReport::ring(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN])
{
	if( m_bIsAutoCallbackMode )
	{
                if(false == closeAudio)
		{
                        m_pFather->m_pPhoneAudioControl->playRingAudio();
                }
                else
                {
                }
		m_pFather->baseHangUp();
	}
	else
	{


                if(false == closeAudio)
                {
	                m_pFather->openPhoneAudio();
                }
                else
                {
                }
	}
	
	if( m_pCallback )
		m_pCallback->ring(cPhoneNumber);

//#if 0//commented for bug:main volume of voice was 15,called,then warning came,call's volume would be lower
#if defined(_8925_VERSION) && !defined(WITH_NEW_AUDIO_CONTROL_SYSTEM)
#ifdef _8925_8D1
    system("amixer cset numid=32 on");
#endif
    system("amixer cset numid=12 28");
#endif
//#endif
    
}

/*void Interface1PhoneReport::bsCommandProcessResult(Bs_Command_Type type, int result, void *pRetData)
{
	if( m_pCallback )
		m_pCallback->bsCommandProcessResult(type, result, pRetData);
}*/

int Interface1PhoneReport::closePhoneAudio(bool flag)
{

        closeAudio = flag;
        return 0;
}

bool Interface1PhoneReport::isSMSToReg(char *SMSContent)
{
        if(NULL == SMSContent)
        {
                return false;
        }
        else if('*' == *SMSContent++ && 'I' == *SMSContent++ && 'N' == *SMSContent++ && 'N' == *SMSContent++ && 'O' == *SMSContent++ && 'V' == *SMSContent++ && '0' == *SMSContent++ && '1' == *SMSContent++ && '*' == *SMSContent)
        {
                return true;
        }
        else
        {       
        }

        return false;
}
#ifdef  SEND_MSG_TO_NAVI
bool Interface1PhoneReport::isSMSToNavi(char *SMSContent)
{
        if(NULL == SMSContent)
        {
                return false;
        }
        else if('\0' == *SMSContent)//length < 1
        {
                return false;
        }
        else if('#' == *SMSContent && 'N' == *(SMSContent + 1))
        {
                return true;
        }
        else
        {
                return false;
        }

        return false;
}

#define FX_FY_LEN       (8)
#define LOCATE_POINT    (5)
int Interface1PhoneReport::parseSMS(char *SMSContent)
{
        CNaviRoadBookReqC req;
        char *pSMSContent = NULL;
        char *tmp = NULL;
        char *pSMSEnd = NULL;
        int countName = 0;
        float fx = 0.0f;
        float fy = 0.0f;
        char *name = NULL;
        int sum = 0;
        
        long ixy = 0;
        int i = 0;
        int j = 0;
        

        if(NULL == SMSContent)
        {
                return -1;
        }
        else if(false == isSMSToNavi(SMSContent))
        {
                return -1;
        }
        else
        {
        }

        pSMSContent = SMSContent + 2;
        pSMSEnd = pSMSContent;
        while('\0' != *pSMSEnd && '#' != *pSMSEnd && 'N' != *(pSMSEnd + 1))
        {
                pSMSEnd++;
        }
        for(j = 0;pSMSContent < pSMSEnd && j < LOCATE_POINT; j++)
        {
                ixy = 0;
                if(',' != *pSMSContent && *pSMSContent < '7')
                {
                        ixy = ixy * 10 + 1;
                }
                else
                {
                }
                for(i = 0;i < FX_FY_LEN;i++)
                {
                        if(pSMSEnd <= pSMSContent || ',' == *pSMSContent)
                        {
                                break;
                        }
                        else
                        {
                        }
                        ixy = ixy * 10 + *pSMSContent - '0';
                        pSMSContent++;
                }
                fx = ixy / 1000000.f;
                ixy = 0;
                for(i = 0;i < FX_FY_LEN;i++)
                {
                        if(pSMSEnd <= pSMSContent || ',' == *pSMSContent)
                        {
                                break;
                        }
                        else
                        {
                        }
                        ixy = ixy * 10 + *pSMSContent - '0';
                        pSMSContent++;
                }
                fy = ixy / 1000000.f;
                if(',' == *pSMSContent)
                {
                       pSMSContent++;
                }
                else
                {
                }
                countName = 0;
                tmp = pSMSContent;
                while( tmp < pSMSEnd && ';' != *tmp)
                {
                        tmp++;
                        countName++;
                }
                name = (char *)malloc(sizeof(char) * (countName + 1));
                memset(name, '\0', sizeof(char) * (countName + 1));
                memcpy(name,pSMSContent,sizeof(char) * countName);
                pSMSContent += countName;
                if(';' == *pSMSContent)
                {
                        pSMSContent++;
                }
                else
                {
                }
                req.AddBookItem(name,fx,fy);
                free(name);

                sum++;
        }
        if(sum && m_pCallback)
        {
                //m_pCallback->sendMSGToNavi(req);
        }
        else
        {
        }

        return 0;
}
#endif








