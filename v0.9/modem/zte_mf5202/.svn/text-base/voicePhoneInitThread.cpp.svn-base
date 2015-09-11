#include "phoneConnection.h"
#include "voicePhoneInitThread.h"
#include "voicePhoneWorkThread.h"
#include "MobileInterFace.h"
#include "videoPhoneMonitor.h"


static const char *phoneInitCommandList[]=
{
#ifdef ADD_WIFI_TEST
	"AT\r\n",
	"AT\r\n",
	"ATE0\r\n",
	"AT\r\n",
#else
	"ATE1\r\n",
	"\032",           //SetREBUILD
//	"AT+CHUP\r\n",	  //SetHANGUP
	"AT\r\n",
	"AT+CCWA=1,0\r\n",	//SetCCWA
#endif
	"AT+CSCS=\"UCS2\"\r\n",  //SetCSCS
	"AT+CMGF=0\r\n",    //
//	"AT+CNMI=2,1\r\n",
#ifdef _8925_8D1
        "AT+MORING=1\r\n"
	"AT+CNMI=2,1,0,1,0\r\n",
#else
	"AT+CNMI=2,1,0,2,0\r\n",
#endif
	"AT+CLIP=1\r\n",
	"AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n",
#ifndef ADD_WIFI_TEST
	"AT+CLVL=5\r\n",
#endif
//	"AT+WAKEUPSMS=UTSCMDWAKE\r\n",
#ifdef _8925_8D1
	"AT+CBST=4,0,0\r\n",
#else
	"AT+CBST=134,1,0\r\n",
#endif
#ifdef ADD_WIFI_TEST
	"AT+CRC=1\r\n"
#else
	"AT+ZSNT=0,0,0\r\n",
	"AT+CRC=1\r\n",
	"AT+ZGINIT\r\n",
	"AT+ZGMODE=3\r\n",
	"AT+ZGQOS=50,255\r\n",
	"AT+ZGFIXRATE=2147483647,0\r\n",
	"AT+ZGRUN=2\r\n",
	"AT+ZAUDIO_HUNGUP_DELAY=100\r\n" //增加延迟100ms挂断电话
#endif
};


void VoicePhoneInitThreadC::startInit(PhoneMonitorVoiceInterface *voicePhone)
{
	m_voicePhone =voicePhone;
	start();
}

void VoicePhoneInitThreadC::run()
{
	char resultBuffer[100];
	for(int i=0;i<(int)(sizeof(phoneInitCommandList)/sizeof(const char *));i++)
	{
		int result =m_voicePhone->m_pPhoneConnection->sendAtCommand(phoneInitCommandList[i],resultBuffer,100,3000);

		if(result<0)
		{
			;//printf("Send AT cmd:[%s] error\n",phoneInitCommandList[i]);
		}
		else if(strstr(resultBuffer,"OK")==NULL)
		{
			;//printf("Send AT cmd:[%s] respose error. res=[%s]\n",resultBuffer);
		}
		else
		{
			;//printf("Send AT cmd:[%s] success\n",phoneInitCommandList[i]);
		}
	}

	m_voicePhone->m_mutex.lock();
	m_voicePhone->m_phoneStatus =PHONE_STATUS_INITED;
	emit phoneInitedSignal();//for bug:calling,auto-system reboot suddently because of unpredictable factor,calling is going on
	//fprintf(stderr,"\n...line:%d,%s,%s...\n\n",__LINE__,__FUNCTION__,__FILE__);
	m_voicePhone->m_mutex.unlock();

#ifndef ADD_WIFI_TEST
	m_voicePhone->m_videoPhone->init(m_voicePhone->m_pCallback);
#endif
	m_voicePhone->m_pWorkThread->readPreviousMsg();
}


