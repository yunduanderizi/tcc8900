#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <termios.h>

#include <linux/videodev2.h>

#include "interf_dec.h"
#include "interf_enc.h"
#include "tccfb_ioctl.h"
#include "alsa/asoundlib.h"
#include "vtlib.h"

#include "debugMonitor.h"
#include "videoPhoneMonitor.h"
#include "phoneConnection.h"
#include "phoneMonitorVoiceReportBase.h"
#include "MobileInterFace.h"

#include "i2ccontrol.h"	
#include "videoPhoneAudioPlayThread.h"

extern "C" {
//#include "Camapp.h" 
//int CamappStart();
//void CamappEnd();
int initCam();
void camStart();
void camStop();
void camQuit();
#ifdef CAM_2
int camGrab(char* buf);
int camPlay(char* buf);
#endif
}

#define VIDEO_PHONE_TEST_DEBUG

#define UINT32  unsigned int
#define MMSG_H32X_CLOSEBYREMOTE ((UINT32)0x00020010L)
#define MMSG_H32X_CLOSEBYPROTOCOL ((UINT32)0x00020012L)
#define MMSG_H32X_CLOSEBYSW ((UINT32)0x00020011L)
#define MMSG_H32X_CLOSEDCONNECTION       (0x00020013)
#define MMSG_H245_RTDSE_EXPIRY_INDICATION ((UINT32)0xC0030048L)
#define MMSG_H32X_CLOSEBYUSER            (0x0002000F)

#define SAMPLERATE 8000
#define CH 1

//#define WITHOUT_PPPD_FOR_INTERNAL_TEST
#define VIDEO_ADDRESS	0x45
#define VIDEO_SUBADDRESS 0x02
#define START_VIDEO	0x48


VideoPhoneMonitorC *VideoPhoneMonitorC::thisObject=NULL;

static void* amr_en;
static void* amr_de;
#ifdef CAM_2
#else
static unsigned long videobuff;
static unsigned long videoPlaybuff;
#endif
static snd_pcm_t* capture_handle;
static snd_pcm_t* videoCallPlayhandle;
static VideoPhoneAudioPlayThreadC* g_pVideoPhoneAudioPlay = NULL;

static int pcmInit()
{
	  int err = 0;
	  
	  unsigned int val;

	  int dir = 0;
	  snd_pcm_uframes_t frames;
	  
	  snd_pcm_hw_params_t *hw_params = NULL;
  
	  snd_pcm_hw_params_alloca(&hw_params);
  
	  if ((err = snd_pcm_open (&capture_handle, "plug:tcc", SND_PCM_STREAM_CAPTURE, 0)) < 0) 
	  {
		  IBD_PRINTF(DEBUG_INFOMATION, "cannot open audio device (%s)", snd_strerror (err))
		  return -1;
	  }

	  if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) 
	  {
		 IBD_PRINTF(DEBUG_INFOMATION, "cannot initialize hardware parameter structure (%s)", snd_strerror (err))
		  return -1;
	  }

	  if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
	  {
		 IBD_PRINTF(DEBUG_INFOMATION, "cannot set access type (%s)", snd_strerror (err))
		  return -1;
	  }

	  if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0)
	  {
		  IBD_PRINTF(DEBUG_INFOMATION, "cannot set sample format (%s) ", snd_strerror (err))
		  return -1;
	  }
	  val = SAMPLERATE;
	  if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &val, 0)) < 0) 
	  {
		  IBD_PRINTF(DEBUG_INFOMATION, "cannot set sample rate (%s)", snd_strerror (err))
		  return -1;
	  }
		val = CH;  
	if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, val)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "cannot set channel count (%s)\n", snd_strerror (err))
		return -1;
	}
	frames = 10240;
	  
	if( (err = snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &frames, &dir)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_set_period_size_near failed!   err = %d\n", err)
		return -1;
	}
	  
	if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "cannot set parameters (%s)\n", snd_strerror (err))
		return -1;
	}

	if ((err = snd_pcm_prepare (capture_handle)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "cannot prepare audio interface for use (%s)\n", snd_strerror (err))
		return -1;
	}

	if( (err = snd_pcm_hw_params_get_period_size(hw_params, &frames, &dir)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_get_period_size failed!err = %d\n", err)
		return -1;
	}
	
	if( (err = snd_pcm_hw_params_get_period_time(hw_params, &val, &dir)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_get_period_time failed!	err = %d\n", err)
		return -1;
	}

 	return 0;
}

static int audioPlay_pcmInit()
{
	int err;
	int dir;
	int size;
	unsigned int val;
	snd_pcm_uframes_t frames;
	snd_pcm_hw_params_t* params;
	int channels = 0;
	if ( (err = snd_pcm_open(&videoCallPlayhandle, "plug:vphone", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "cannot open audio device (%s)", snd_strerror (err))
		goto OpenPcmFail;
	}

	snd_pcm_hw_params_alloca(&params);//ä¸ºå‚æ•°å˜é‡åˆ†é…ç©ºé—´
	if( (err = snd_pcm_hw_params_any(videoCallPlayhandle, params)) < 0)//å‚æ•°åˆå§‹åŒ–
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_any failed!	 err = %d\n", err)
		goto OpenPcmFail;
	}
	if( (err = snd_pcm_hw_params_set_access(videoCallPlayhandle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)//è®¾ç½®ä¸ºäº¤é”™æ¨¡å¼
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_set_access failed!	 err = %d\n", err)
		goto OpenPcmFail;
	}
	if( (err = snd_pcm_hw_params_set_format(videoCallPlayhandle, params, SND_PCM_FORMAT_S16_LE)) < 0)//ä½¿ç”¨16ä½æ ·æœ¬
	{
		 IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_set_format failed!	 err = %d\n", err)
		goto OpenPcmFail;
	}
	channels = val = 1;
	if( (err = snd_pcm_hw_params_set_channels(videoCallPlayhandle, params, val)) < 0)//è®¾ç½®ä¸ºç«‹ä½“å£°
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_set_channels failed!	 err = %d\n", err)
		goto OpenPcmFail;
	}
	val = 8000;
	if( (err = snd_pcm_hw_params_set_rate_near(videoCallPlayhandle, params, &val, &dir)) < 0)//è®¾ç½®é‡‡æ ·çŽ‡ä¸º44.1KHz
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_set_rate_near failed!   err = %d\n", err)
		goto OpenPcmFail;
	}

	frames = 4096;
	if( (err = snd_pcm_hw_params_set_period_size_near(videoCallPlayhandle, params, &frames, &dir)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_set_period_size_near failed!	err = %d\n", err)
		goto OpenPcmFail;
	}
	if ( (err = snd_pcm_hw_params(videoCallPlayhandle, params)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params-failed!	err = %d err: %s\n", err, snd_strerror(err))
		goto OpenPcmFail;
	}

	if( (err = snd_pcm_hw_params_get_period_size(params, &frames, &dir)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_get_period_size failed!	 err = %d\n", err)
		goto OpenPcmFail;
	}
	size = 160;

	if( (err = snd_pcm_hw_params_get_period_time(params, &val, &dir)) < 0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_hw_params_get_period_time failed!	err = %d\n", err)
		goto OpenPcmFail;
	}
	
	return 0;
	
OpenPcmFail:
	return -1;
}

static int audio_grab( void */*pUserArg*/,  void *pAudioBuffer,int pBufferSize)
{
	int err;
	err = snd_pcm_readi(capture_handle, (char* )pAudioBuffer, pBufferSize/2);
	if(err == -EPIPE)
	{
		if( (err = snd_pcm_prepare(capture_handle)) < 0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "snd_pcm_prepare_failed! err: %s\n", snd_strerror(err))
		}
	}

#if 0
static int c=0;
if(c++ % 50==0)
{
printf("audio_grab return size=%d\n",err * 2);
short *p=(short*)pAudioBuffer;

	for(int i=0;i<err;i++)
	{
		printf("%d ",p[i]);
	}
	printf("\n\n");
}
#endif
	return err * 2;
}

static bool audio_encode(void */*pUserArg*/,const short * pSrc, int pBufferSize,unsigned char * pDst,int pDstSize)
{
	pDstSize = Encoder_Interface_Encode(amr_en, MR122, pSrc, pDst, 0);

#if 0
static int c=0;
if(c++ % 50==0)
{
printf("audio_encode pDstSize=%d bufferlen=%d\n",pDstSize,pBufferSize);
short *p=(short*)pSrc;

	for(int i=0;i<pBufferSize;i++)
	{
		printf("%d ",p[i]);
	}
	printf("\n\n");
}
#endif

	return true;
}

static int audio_decode(void */*pUserArg*/,const unsigned char *  pSrc, int /*pBufferSize*/,short* pDst)
{
	//int AudioDecLen = 0;
	Decoder_Interface_Decode(amr_de, pSrc, pDst, 0);
	return 320;
}

static int audio_play(void */*pUserArg*/,  const void *pAudioDate, int pDataLength)
{

    g_pVideoPhoneAudioPlay->putAudioData((const unsigned char*)pAudioDate, pDataLength);
    
/*
	int err;
	err = snd_pcm_writei(videoCallPlayhandle, pAudioDate, pDataLength/2);
	if ( err == -EPIPE){
		if( (err = snd_pcm_prepare(videoCallPlayhandle)) < 0){
		}
	} 
*/
#if 0
static int c=0;
if(c++ % 50==0)
{
printf("audio_play size=%d\n",pDataLength);
short *p=(short*)pAudioDate;

	for(int i=0;i<pDataLength/2;i++)
	{
		printf("%d ",p[i]);
	}
	printf("\n\n");
}
#endif

	return pDataLength;
}

static bool video_grab(void */*pUserArg*/,	unsigned char *pVideoDate, /*video_format_type&pImageFormat,*/unsigned long /*waittime*/)
{
#ifdef CAM_2
	camGrab((char* ) pVideoDate);
#else
	memcpy(pVideoDate, (void* )videobuff, 38016); //(176 * 144 * 3) /2
#endif
	return true;
}

static bool video_play(void */*pUserArg*/,	unsigned char *pVideoDate)
{
#ifdef CAM_2
	camPlay((char* ) pVideoDate);
#else
 	memcpy((void* )videoPlaybuff, pVideoDate, 38016);
#endif
 	return true;
}


static int modem_write(unsigned char* pDate, int pDateLenth)
{
	int result =VideoPhoneMonitorC::thisObject->writeBuffer((char*)pDate,pDateLenth);

	return result;
}


static int modem_read(unsigned char* pBuffer, int pBufferSize)
{
	int result =VideoPhoneMonitorC::thisObject->readBuffer((char*)pBuffer,pBufferSize);

	if(result>0)
	{
	}
	else if(result==0)
	{
	}
	else
	{
	}

	return result;
}

static unsigned long sys_malloc_virtual_addr(unsigned long PAddr, unsigned long uiSize)
{
	//void *mem_ptr = MAP_FAILED;

	int fd= open("/dev/mem", O_RDWR | O_NDELAY);
	if (fd<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "mem dirver open fail\n")
		return NULL;
	}

	return (unsigned long )mmap(0, uiSize, PROT_READ | PROT_WRITE, MAP_SHARED,fd,PAddr);
}


static int FaceTimeCallBack(unsigned int pMsgId, unsigned char */*pData*/, void * /*pCallBackArg*/)
{
	int ret = 0;


	switch(pMsgId)
	{
		case MMSG_H32X_CLOSEBYREMOTE:  	//0x00020010L
		case MMSG_H32X_CLOSEBYPROTOCOL:	//0x00020012L 
		case MMSG_H32X_CLOSEBYSW:	//0x00020011L
		{
			printf("hangUp...1.....................by FaceTimeCallBack =%0X\n\n",pMsgId);
			VideoPhoneMonitorC::thisObject->hangUpInternal(1);
			break;
		}
		case MMSG_H32X_CLOSEDCONNECTION: //0x00020013
		{
			printf("hangUp...2.....................by FaceTimeCallBack =%0X\n\n",pMsgId);
			VideoPhoneMonitorC::thisObject->hangUpInternal(2);
			break;
		}
		case MMSG_H245_RTDSE_EXPIRY_INDICATION: //0xC0030048L
		{
			printf("hangUp...3.....................by FaceTimeCallBack =%0X\n\n",pMsgId);
			VideoPhoneMonitorC::thisObject->hangUpInternal(1);
			break;
		}
		case MMSG_H32X_CLOSEBYUSER: //0x0002000F
		{
			printf("hangUp...4.....................by FaceTimeCallBack =%0X\n\n",pMsgId);
			//VideoPhoneMonitorC::thisObject->hangUpInternal();
			break;
		}
		default:
		{
			//IBD_PRINTF(DEBUG_INFOMATION, "---------not recognized-----------------FaceTimeCallBack,pMsgId = [%x]\n", pMsgId);
		}
	}

	return ret;
}

#define	       CALL_NOT_INITED		  -1
#define        CALL_IDLE                   0
#define        CALL_STARTED                1
#define        CALL_WAIT_COMING_CALL	   2
#define        CALL_CALLING                3
#define        CALL_TALKING                4
#define        CALL_COMING_CALL            5

VideoPhoneMonitorC::VideoPhoneMonitorC()
{
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC ---\n")
#endif
	thisObject =this;
	m_phoneNumber[0]=0;
	m_phoneStatus =CALL_NOT_INITED;


	m_pPhoneConnection =new PhoneConnectionC();

	m_timerId =0;
}
VideoPhoneMonitorC::~VideoPhoneMonitorC()
{
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- ~VideoPhoneMonitorC ---\n")
#endif

	if(m_timerId>0)
		killTimer(m_timerId);

	delete m_pPhoneConnection;
}

int VideoPhoneMonitorC::init(PhoneMonitorVoiceReportBase *reportHandle)
{
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC::init ---\n")
#endif

	m_pCallback =reportHandle;

	if(pcmInit() == -1)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "InitPcm Error\n")
		return -1;
	}

	if(audioPlay_pcmInit() == -1)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "InitPcm Error\n")
		return -2;
	}

	amr_en = Encoder_Interface_init(0);
	amr_de = Decoder_Interface_init();
	
    int ret = initCam();
    if(ret < 0)
    {
        IBD_PRINTF(DEBUG_INFOMATION, "InitCam Error!\n")
    }
			
	VT_CALLBACK g_CallBack = {
    		video_grab, NULL,
		audio_grab, NULL,
		video_play, NULL,
		audio_play, NULL,
		modem_write,  modem_read,
		audio_encode, NULL,
		audio_decode, NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL,
    		};
	
	int err;
	err = H324M_Open(FaceTimeCallBack, NULL, g_CallBack, MF_YUV420, MF_YUV420, true, "/tmp/file/", false);
#ifdef CAM_2
#else
	videobuff = sys_malloc_virtual_addr(0x4bc00000, 38016);
    videoPlaybuff = sys_malloc_virtual_addr(0x4c200000, 38016);
    if(videobuff == 0 || videoPlaybuff == 0)
    {
		IBD_PRINTF(DEBUG_INFOMATION, "sys_malloc_virtual_addr error\nvideobuff = %d, videoPlaybuff = %d\n")
                //return -3;
	}
#endif

	IBD_PRINTF(DEBUG_INFOMATION, "H324M_Open = [%d]\n", err)
	if(err != 0)
		return -4;

	QMutexLocker locker(&(m_mutex));
	m_phoneStatus =CALL_IDLE;


#ifdef WITHOUT_PPPD_FOR_INTERNAL_TEST
	m_pPhoneConnection->openDev("/dev/ttyUSB4");
#endif

	start();

	return 0;
}

int VideoPhoneMonitorC::waitIncommingCall()
{
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC::waitIncommingCall ---\n")
#endif

	QMutexLocker locker(&(m_mutex));

	if(m_phoneStatus != CALL_IDLE)
		return -1;

	if(startVideoCall()<0)
		return -1;

	m_phoneStatus =CALL_WAIT_COMING_CALL;


	if(m_timerId>0)
		killTimer(m_timerId);

	m_timerId =startTimer(30*1000);

	return 0;
}

int VideoPhoneMonitorC::dial(const char* cpNumber)
{
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC::dial ---\n")
#endif

	QMutexLocker locker(&(m_mutex));

	switch(m_phoneStatus)
	{
		case CALL_NOT_INITED:
		case CALL_CALLING:
		case CALL_TALKING:
		case CALL_COMING_CALL:
			return -1;
		case CALL_IDLE:
			if(startVideoCall()<0)
				return -1;
			else
				break;
		case CALL_WAIT_COMING_CALL:
			if(m_timerId>0)
			{
                		killTimer(m_timerId);
				m_timerId=0;
			}
	}

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CRC=1\r\n");
        ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,3000);

        if(ret < 0 || (strstr(recvBuffer, "OK") == NULL) || strstr(recvBuffer,"END") != NULL)
        {

		endVideoCall();
                return PHONE_MONITOR_FAIL;
        }

        sprintf(sendBuffer,"ATD%s\r\n",cpNumber);

	IBD_PRINTF(DEBUG_INFOMATION,"Send AT(ttyUSB4):%s\n",sendBuffer);

        ret =m_pPhoneConnection->writeBuffer(sendBuffer,strlen(sendBuffer));
	//ret=strlen(sendBuffer);  //for test usb1 to usb2 data error

        if(ret != (int)strlen(sendBuffer))
        {

		endVideoCall();
                return PHONE_MONITOR_FAIL;
        }

	m_phoneStatus =CALL_CALLING;


	return PHONE_MONITOR_SUCCESS;
}
int VideoPhoneMonitorC::pickUp()
{
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC::pickUp ---\n")
#endif

        QMutexLocker locker(&m_mutex);
        if(m_phoneStatus !=CALL_COMING_CALL)
                return PHONE_MONITOR_FAIL;

        char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"ATA\r\n");

        ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

        if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
        {
                //m_phoneStatus =CALL_TALKING;
                return PHONE_MONITOR_SUCCESS;
        }
        else
        {
		//endVideoCall();
                //return PHONE_MONITOR_FAIL;
        }

	return PHONE_MONITOR_SUCCESS;
}
int VideoPhoneMonitorC::hangUp()
{
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC::hangUp ---\n")
#endif

        QMutexLocker locker(&m_mutex);
        if(m_phoneStatus <= CALL_IDLE)
                return PHONE_MONITOR_FAIL;

	if(m_phoneStatus==CALL_TALKING)
	{
		bool ret =H324M_HangUp();
		//CamappEnd();
		if(true == m_flagInit)
		{
		    m_flagInit = false;
            camStop();
            i2cEndPhoneVideo();
		}
		
		m_pPhoneConnection->setBlockMode(1);

		if(ret)
			IBD_PRINTF(DEBUG_INFOMATION, "\n---->H324M_HangUp =======<----SUCCESS\n\n")
		else
			IBD_PRINTF(DEBUG_INFOMATION, "\n---->H324M_HangUp =======<----Failure\n\n")
	}

	m_waitMutex.lock();
	m_phoneStatus =CALL_STARTED;
	m_event.wakeOne();
	m_waitMutex.unlock();
//printf("======================================1====\n");

        char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"ATH\r\n");
        ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,1000);

//printf("======================================2====\n");

	endVideoCall();

        if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
        {
                return PHONE_MONITOR_SUCCESS;
        }
        else
        {
                return PHONE_MONITOR_FAIL;
        }
}


int VideoPhoneMonitorC::writeBuffer(char buffer[],int bufferLen)
{
	QMutexLocker locker(&(m_mutex));
//printf("...............in writeBuffer.........................%d\n",bufferLen);
	int result =-1;
	if(m_phoneStatus==CALL_TALKING)
		result =m_pPhoneConnection->writeBuffer(buffer,bufferLen);

//printf("...............out writeBuffer.........................%d\n",result);
	return result;
}

int VideoPhoneMonitorC::readBuffer(char buffer[],int bufferLen)
{
	QMutexLocker locker(&(m_mutex));
//printf("...............in readBuffer.........................%d\n",bufferLen);

	int result =-1;
	if(m_phoneStatus==CALL_TALKING)
		result =m_pPhoneConnection->readBuffer(buffer,bufferLen);

//printf("...............out readBuffer.........................%d\n",result);
	return result;
}

int VideoPhoneMonitorC::hangUpInternal(int reason)
{
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC::hangUpInternal ---\n")
#endif

	QMutexLocker locker(&(m_mutex));
	if(m_phoneStatus==CALL_TALKING)
	{
		bool ret;

		m_phoneStatus =CALL_STARTED;

		ret =H324M_HangUp();
		//CamappEnd();
		if(true == m_flagInit)
		{
		    m_flagInit = false;
            camStop();
            i2cEndPhoneVideo();
		}		

		m_pPhoneConnection->setBlockMode(1);

		if(ret)
			IBD_PRINTF(DEBUG_INFOMATION, "\n---->H324M_HangUp =======<----SUCCESS\n\n")
		else
			IBD_PRINTF(DEBUG_INFOMATION, "\n---->H324M_HangUp =======<----Failure\n\n")
	}

	m_waitMutex.lock();
	m_phoneStatus =CALL_STARTED;
	m_event.wakeOne();
	m_waitMutex.unlock();

	endVideoCall();

	m_pCallback->toEndFaceTimeCall(reason);

        return PHONE_MONITOR_SUCCESS;
}


void  VideoPhoneMonitorC::timerEvent(QTimerEvent * /*event*/)
{
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC::timerEvent ---\n")
#endif

	QMutexLocker locker(&(m_mutex));

	if(m_phoneStatus==CALL_TALKING)
	{
		m_phoneStatus =CALL_STARTED;

		bool ret=H324M_HangUp();
		//CamappEnd();	
		if(true == m_flagInit)
		{
		    m_flagInit = false;
            camStop();
            i2cEndPhoneVideo();
		}		

		m_pPhoneConnection->setBlockMode(1);

		if(ret)
			IBD_PRINTF(DEBUG_INFOMATION, "\n---->H324M_HangUp =======<----SUCCESS\n\n")
		else
			IBD_PRINTF(DEBUG_INFOMATION, "\n---->H324M_HangUp =======<----Failure\n\n")
	}

	if(m_timerId>0)
	{
		killTimer(m_timerId);
		m_timerId=0;
		endVideoCall();
	}
}

int VideoPhoneMonitorC::i2cStartPhoneVideo()
{
	int err = -1;
    IBI2CControl i2cHander;		
	if (i2cHander.checkOK() == 0)
	{
	    printf("StartPhoneVideo i2c is err!\n");
		return -1;
	}
	err = i2cHander.readByte(VIDEO_ADDRESS, VIDEO_SUBADDRESS, m_i2cOldVal);
	if(err < 0)
	{
	    printf("StartPhoneVideo i2c readByte is err!\n");
	    return -1;
    }	
    printf("StartPhoneVideo m_i2cOldVal= 0x%2X\n", m_i2cOldVal);
	err = i2cHander.writeByte(VIDEO_ADDRESS, VIDEO_SUBADDRESS, START_VIDEO);
	if(err < 0)
	{
	    printf("StartPhoneVideo i2c writeByte is err!\n");
        return -1;
    }
    return 0;

}
int VideoPhoneMonitorC::i2cEndPhoneVideo()
{
	int err = -1;
    IBI2CControl i2cHander;
	if (i2cHander.checkOK() == 0)
	{
	    printf("EndPhoneVideo i2c is err!\n");
		return -1;
	}
	err = i2cHander.writeByte(VIDEO_ADDRESS, VIDEO_SUBADDRESS, m_i2cOldVal);
	if(err < 0)
	{
	    printf("EndPhoneVideo i2c writeByte is err!\n");
        return -1;
    }
    printf("EndPhoneVideo m_i2cOldVal= 0x%2X\n", m_i2cOldVal);
    return 0;
}

int VideoPhoneMonitorC::startVideoCall()
{	
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC::startVideoCall ---\n")
#endif
#ifdef WITHOUT_PPPD_FOR_INTERNAL_TEST
	int result=0;
#else
	system("./pm k ppp-on");
	system("killall -9 pppd");

	int result=-1;
#endif
	


	int count=5;

	while(result<0 && count-->0)
	{
		usleep(50*1000);
		result =m_pPhoneConnection->openDev("/dev/ttyUSB4");
	}

printf("startVideoCall ------------------------------------count=%d\n",count);

	if(result<0)
	{
		printf("Cann't open dev: /dev/ttyUSB4\n");
		return result;
	}

	char buffer[1024];
	int  len=-1;
        m_pPhoneConnection->writeBuffer((char *)"AT+CRC=1\r\n",strlen("AT+CRC=1\r\n"));
	m_pPhoneConnection->setBlockMode(0);
	count =40;
	while(len<=0 && count-->0)
	{
		usleep(100*1000);
		len =m_pPhoneConnection->readBuffer(buffer,1024);
		if(len>0)
		{
			buffer[len]=0;
			IBD_PRINTF(DEBUG_INFOMATION, "Read /dev/ttyUSB4 redundant data:%s\n",buffer)
			
			if(strstr(buffer,"NO CARRIER")!=NULL || strstr(buffer,"OK")!=NULL || strstr(buffer,"ERROR")!=NULL)
				break;
			else
				len=-1;
		}
	}
	m_pPhoneConnection->setBlockMode(1);

printf("startVideoCall -----------read carrier-------------------------count=%d\n",count);

	m_waitMutex.lock();
	m_phoneStatus =CALL_STARTED;
	m_event.wakeOne();
	m_waitMutex.unlock();

//	g_pVideoPhoneAudioPlay = new VideoPhoneAudioPlayThreadC(videoCallPlayhandle);
    g_pVideoPhoneAudioPlay = NULL;
    
	return 0;
}

int VideoPhoneMonitorC::endVideoCall()
{
#ifdef VIDEO_PHONE_TEST_DEBUG
    IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC::endVideoCall ---\n")
    printf("\n==========VideoPhoneMonitorC::endVideoCall==========\n");
#endif

	if(m_phoneStatus>CALL_IDLE)
	{

        	char sendBuffer[200];
        	char recvBuffer[200];
        	int ret = 0;

        	sprintf(sendBuffer,"AT+CRC=1\r\n");
        	ret = m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,1000);

        	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
		{
		}
		else
		{
		}

		m_phoneStatus =CALL_IDLE;
        
#ifndef WITHOUT_PPPD_FOR_INTERNAL_TEST
		m_pPhoneConnection->closeDev();
		system("./pm s ppp-on");
#endif
	}

	if(g_pVideoPhoneAudioPlay != NULL)
	{
    	delete g_pVideoPhoneAudioPlay;
    	g_pVideoPhoneAudioPlay = NULL;
    }
	return 0;
}

void VideoPhoneMonitorC::run()
{
#ifdef VIDEO_PHONE_TEST_DEBUG
	IBD_PRINTF(DEBUG_INFOMATION, "\n--- VideoPhoneMonitorC::run ---\n")
#endif

	char recvBuffer[1024];

	while(1)
	{
printf("in wait.......\n\n");
		m_waitMutex.lock();
		while(m_phoneStatus==CALL_IDLE || m_phoneStatus==CALL_TALKING)
			m_event.wait(&m_waitMutex);	
		m_waitMutex.unlock();
printf("out wait.......\n\n");

		if(m_pPhoneConnection->getNextIndication(recvBuffer,1024)<0)
		{
			sleep(1);
                        continue;
		}

		if(strstr(recvBuffer, "SYNC"))
		{
			if(strstr(recvBuffer, "CLIP"))//ÓÐµç»°ºôÈëµÃµ½À´µçºÅÂë
			{
				char* cpTmp=recvBuffer;

				if(strsep(&cpTmp, "\"") != NULL)
				{
					char *cpPhoneNumber;
					if((cpPhoneNumber = strsep(&cpTmp, "\"")) != NULL)
					{
						QMutexLocker locker(&(m_mutex));

						if(m_timerId>0)
						{
							killTimer(m_timerId);
							m_timerId=0;
						}

						if(m_phoneStatus!=CALL_COMING_CALL)
						{
							strncpy(m_phoneNumber,cpPhoneNumber,30);
							m_phoneNumber[29]=0;

							m_phoneStatus =CALL_COMING_CALL;
							m_pCallback->toNewFaceTimeCall(m_phoneNumber);
						}
					}
				}
			}
		}
		else if(strstr(recvBuffer, "CARRIER") != NULL)
		{
			hangUpInternal(0);
		}
		else if(strstr(recvBuffer, "NO ANSWER") != NULL)
		{
			hangUpInternal(0);
		}
#ifdef ADD_WIFI_TEST
		else if(strstr(recvBuffer, "CEND") != NULL)  
		{
			hangUpInternal(0);
		}
#else
		else if(strstr(recvBuffer, "VOICE CALL: END") != NULL)  
		{
			hangUpInternal(0);
		}
#endif
		else if(strstr(recvBuffer, "HANGUP") != NULL) ////¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿HANGUP
		{	
			hangUpInternal(0);
		}
		else if(strstr(recvBuffer, "BUSY") != NULL) ////¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿HANGUP
		{	
			hangUpInternal(0);
		}
		else if(strstr(recvBuffer, "STOPRING") != NULL)
		{
			hangUpInternal(0);
			m_pCallback->toFaceTimeMissCall(m_phoneNumber);
		}
		else if(strstr(recvBuffer, "MISSED") != NULL)
		{
			hangUpInternal(0);
			m_pCallback->toFaceTimeMissCall(m_phoneNumber);
		}
		else if(strstr(recvBuffer, "NO DIALTONE") != NULL)
		{	
			hangUpInternal(0);
		}
		else if(strstr(recvBuffer, "CSDRINGBACKLOCAL") != NULL)
		{		
			//IBD_PRINTF(DEBUG_INFOMATION, "[CSDRINGBACKLOCAL]: [%s]...\n", recvBuffer)
		}
#ifdef ADD_WIFI_TEST
		else if(strstr(recvBuffer, "CONN") != NULL)
		{
			QMutexLocker locker(&(m_mutex));

			int  ret324;
			int  oldStatus =m_phoneStatus;

            if(i2cStartPhoneVideo() < 0)
            {
                printf("i2cStartPhoneVideo is err!\n");
                continue;
            }
			camStart();
			m_flagInit = true;
			
			m_phoneStatus =CALL_TALKING;
			m_pPhoneConnection->setBlockMode(0);

			if(oldStatus==CALL_CALLING)
				ret324 =H324M_MakeCall(NULL,"60000",ZVT_BEARER_MODEM);
			else
				ret324 =H324M_MakeCall(NULL,"60000",ZVT_BEARER_MODEM);
				//ret324 =H324M_AnswerCall(ZVT_BEARER_MODEM);

			if(ret324)
				IBD_PRINTF(DEBUG_INFOMATION, "\n---->H324M_MakeCall=======<----SUCCESS oldStatus=%d\n\n",oldStatus)
			else
				IBD_PRINTF(DEBUG_INFOMATION, "\n---->H324M_MakeCall=======<----Failure oldStatus=%d\n\n",oldStatus)

			m_pCallback->toFaceTimeCallConnect();
        	g_pVideoPhoneAudioPlay = new VideoPhoneAudioPlayThreadC(videoCallPlayhandle);			
		}
#else
		else if(strstr(recvBuffer, "CONNECT") != NULL)
		{
			QMutexLocker locker(&(m_mutex));

			int  ret324;
			int  oldStatus =m_phoneStatus;

            if(i2cStartPhoneVideo() < 0)
            {
                printf("i2cStartPhoneVideo is err!\n");
                continue;
            }
			camStart();
			m_flagInit = true;
			
			m_phoneStatus =CALL_TALKING;
			m_pPhoneConnection->setBlockMode(0);

			if(oldStatus==CALL_CALLING)
				ret324 =H324M_MakeCall(NULL,"60000",ZVT_BEARER_MODEM);
			else
				ret324 =H324M_MakeCall(NULL,"60000",ZVT_BEARER_MODEM);
				//ret324 =H324M_AnswerCall(ZVT_BEARER_MODEM);

			if(ret324)
				IBD_PRINTF(DEBUG_INFOMATION, "\n---->H324M_MakeCall=======<----SUCCESS oldStatus=%d\n\n",oldStatus)
			else
				IBD_PRINTF(DEBUG_INFOMATION, "\n---->H324M_MakeCall=======<----Failure oldStatus=%d\n\n",oldStatus)

			m_pCallback->toFaceTimeCallConnect();
        	g_pVideoPhoneAudioPlay = new VideoPhoneAudioPlayThreadC(videoCallPlayhandle);			
		}
#endif
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "USB4 Cmd Type not recongized\n")
		}
	}
}

