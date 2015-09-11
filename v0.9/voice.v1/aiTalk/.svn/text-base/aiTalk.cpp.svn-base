#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>

#include <QMessageBox>
#include <QKeyEvent>
#include <QWidget>
#include <QtGui/QApplication>
#include <QFile>

#include "aiTalk.h"
#include "aiTalk_setting.h"
#include "parseConfig.h"
#include "error_types.h"

#include "audio_control_server.h"
#include "audio_control_client.h"

#include "audio_control.h"
#include "debugMonitor.h"

#define MCU_CHIPADDR 	0x50



#define USER_WORKBUFFER_ADDRESS		(0x01013d14)
	/* 工作内存尺寸（字节数）*/
#define USER_WORKBUFFER_BYTES		(640*1024)
	/* TODO:用户指定静态常驻内存地址*/
#define USER_RESIDENTBUFFER_ADDRESS	(0x01313d14)
	/* 常驻内存尺寸（字节数）*/
#define USER_RESIDENTBUFFER_BYTES	(32*1024)




/* 是否要控制填充数据*/
/* 如果定义这个宏，则填充数据，要受到识别线程的控制*/
/* 如果不定义这个宏，则一直填充数据，不受识别线程的控制*/
#define AITALK_CONTROL_APPEND_DATA		1

/* 录音采样率 */
#define AITALK_SAMPLERATE	16000

#define AITALK_CH		1


extern AudioCtrlServerC *audioServer;


/*用于设置词和遥控码对应关系的数组*/
Lexicon_Setting *g_aitalk_lexicon_setting = NULL;

/*数组长度*/
int g_aitalk_lexicon_setting_num = 0;

/*下面这个遥控码代表语音识别打开*/
char* g_aitalk_open_keycode = NULL;

/*下面这个遥控码代表语音识别关闭*/
char* g_aitalk_close_keycode = NULL;


/* 是否将收到的PCM数据存文件*/
//#define AITALK_SAVE_PCM_DATA_TO_FILE		1
#ifdef	AITALK_SAVE_PCM_DATA_TO_FILE
	static FILE *g_pPCMDataFile = NULL;
	static char *g_sPCMDataFileMotherName = PCM_DATA_FILE_MOTHER_NAME;
	static char g_sPCMDataFileName[256];
	static int    g_iPCMDataFileCount = 0;
#endif

static snd_pcm_uframes_t frames;
extern AudioCtrlClientC *g_pAudioClientCtl;

PCMDataLoader::PCMDataLoader(ivPointer hESRObj, QThread *parent)
    :QThread(parent),
     m_bExitRun(false),
     m_bAppendData(false),
     m_hESRObj(hESRObj)
{
	BEGINFUNC_USING_BY_VOICE
	IBD_PRINTF(4, "in PCMDataLoader...\n");
	ENDFUNC_USING_BY_VOICE
}

PCMDataLoader::~PCMDataLoader()
{
	BEGINFUNC_USING_BY_VOICE
	ReleasePcm();
	ENDFUNC_USING_BY_VOICE
}

void PCMDataLoader::exitFromRun()
{
	BEGINFUNC_USING_BY_VOICE

	m_bExitRun = true;

	ENDFUNC_USING_BY_VOICE
}

void PCMDataLoader::StartAppend()
{
	IBD_PRINTF(4, "StartAppend............\n");
	m_bAppendData = true;
}

void PCMDataLoader::StopAppend() 
{
	
	IBD_PRINTF(4, "StopAppend............\n");
	m_bAppendData = false;
}

int PCMDataLoader::InitPcm()
{
	int err = 0;
	
	unsigned int val;
	int dir=0;

	quiet_mode = 0;

	snd_pcm_hw_params_t *hw_params = NULL;

	snd_pcm_hw_params_alloca(&hw_params);

	stream = SND_PCM_STREAM_CAPTURE;

	if ((err = snd_pcm_open (&capture_handle, "plug:tcc", SND_PCM_STREAM_CAPTURE, 0)) < 0) 
	{
		dbug_out("cannot open audio device (%s)\n", snd_strerror (err));
		return -1;
	}
	dbug_out("After snd_pcm_open for SND_PCM_STREAM_CAPTURE");
	if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) 
	{
		IBD_PRINTF(4, "cannot initialize hardware parameter structure (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
	{
		IBD_PRINTF(4, "cannot set access type (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0)
	{
		IBD_PRINTF(4, "cannot set sample format (%s)\n",
			 snd_strerror (err));
		return -1;
	}
	val = AITALK_SAMPLERATE;
	if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &val, 0)) < 0) 
	{
		IBD_PRINTF(4, "cannot set sample rate (%s)\n",
			 snd_strerror (err));
		return -1;
	}
	 val = AITALK_CH;	
	if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, val)) < 0)
	{
		IBD_PRINTF(4, "cannot set channel count (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0)
	{
		IBD_PRINTF(4, "cannot set parameters (%s)\n",
			 snd_strerror (err));
		return -1;
	}
	
	if ((err = snd_pcm_prepare (capture_handle)) < 0)
	{
		IBD_PRINTF(4, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		return -1;
	}
	
	frames = 6144;
	
	if( (err = snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &frames, &dir)) < 0)
	{
		IBD_PRINTF(4,"snd_pcm_hw_params_set_period_size_near failed!   err = %d\n", err);
		exit(1);
	}

	snd_pcm_hw_params_get_period_size(hw_params, &frames, 0);

	return 0;
#if 0	
	size = frames * 1 * 2;
	PcmBuf = (char *)malloc(sizeof(char) * size);
#endif
}

void PCMDataLoader::ReleasePcm()
{
	snd_pcm_close(capture_handle);
}


void PCMDataLoader::run()
{
	BEGINFUNC_USING_BY_VOICE

	int iInitTimes = 0;
	snd_pcm_uframes_t err;
	ivESRStatus iStatus;
	char mono[PCMBUFFSIZE];

#ifdef	AITALK_SAVE_PCM_DATA_TO_FILE
	if( NULL == g_pPCMDataFile )
	{
		sprintf(g_sPCMDataFileName,"%s%d.wav", g_sPCMDataFileMotherName, g_iPCMDataFileCount);
		g_iPCMDataFileCount++;
		g_pPCMDataFile = fopen(g_sPCMDataFileName,"w+");
	}
#endif	

AgInit:
	if(InitPcm() == -1)
	{
		while(iInitTimes < 5)
		{
			iInitTimes++;
			dbug_out("InitPcm Error...will Init agin...");
			goto AgInit;
		}
		dbug_out("sInitPcm Error...");
	}
	
	EsrEngineReset(m_hESRObj); 
//	frames = 2048;
	frames = frames * 2;

#ifdef VOICE_LOG_OUT
	FILE* voiceLog;
	voiceLog = fopen(VOICE_LOG_PATH, "a");
	int logret = 0;
#endif

	while( !m_bExitRun )
	{	
		memset(mono, 0, sizeof(char) * PCMBUFFSIZE);
#if 1
		err = snd_pcm_readi(capture_handle, mono, frames);

#ifdef VOICE_LOG_OUT
		logret = fwrite(mono, sizeof(char), sizeof(char) * frames * 2, voiceLog);
		if(logret < 0)
			IBD_PRINTF(4, "fwrite voicelog file fail\n");
#endif

		if(err != frames)
		{
			IBD_PRINTF(4, "read from audio interface failed (%s)\n", snd_strerror(err));
			int myPrePareErr;
			if( (myPrePareErr = snd_pcm_prepare(capture_handle)) < 0)
			{
				IBD_PRINTF(4,"snd_pcm_prepare_failed! err: %s\n", snd_strerror(myPrePareErr));
			}
		}
#endif

#ifdef	AITALK_SAVE_PCM_DATA_TO_FILE
				if( NULL != g_pPCMDataFile )
				{
					fwrite(mono,1,frames*2,g_pPCMDataFile);
				}			
#endif
		if(m_bAppendData) 
		{
			iStatus = EsrAppendData(m_hESRObj, mono, frames);//因为要发的数据是16bit的，所以
															//长度应该是mono的长度(size/2)再
															//除以2,即size/4
			if( ivESR_OK == iStatus )
			{
//				IBD_PRINTF(4,"[aiTalk.cpp] ok!\n");
				usleep(30000);
			   /*成功输入语音数据*/
			}
			else if( ivESR_BUFFERFULL == iStatus )
			{
			/*Aitalk CE 2.2内部音频数据缓冲区已满，本次追加失败，数据已被Aitalk CE 2.2丢弃*/
				IBD_PRINTF(4,"[aiTalk.cpp] huan chong qu yi man , fa song shi bai  size[%d]!\n", (int)(frames/2));
			iStatus = EsrEngineReset(m_hESRObj);
			IBD_PRINTF(4,"[aiTalk.cpp]Engine buf full  Reset!!\n");
			}
			else if( ivESR_ASR_ENDED == iStatus )
			{
			/*识别引擎已停止，需要调用"EsrEngineReset" API重置ESR引擎*/
				iStatus = EsrEngineReset(m_hESRObj);
				IBD_PRINTF(4,"[aiTalk.cpp]Engine Stop Reset!!\n");
			}
			else if( ivESR_INVCAL == iStatus )
			{
			/*错误的调用，对象错误*/
				IBD_PRINTF(4,"[aiTalk.cpp] dui xiang cuo wu !!\n");
			}
			else if( ivESR_INVARG == iStatus )
			{
			/*错误的调用，对象错误*/
				IBD_PRINTF(4,"[aiTalk.cpp] can shu cuo wu !!! \n");
			}
			else
			{
			/*应该不会到这里*/
			}
		}
	}

//	system("caraction mutegf");
	ReleasePcm();
//	system("caraction unmutegf");

#ifdef	AITALK_SAVE_PCM_DATA_TO_FILE
	if( NULL != g_pPCMDataFile )
	{
		fclose(g_pPCMDataFile);
		g_pPCMDataFile = NULL;
	}
#endif

	m_bExitRun = false;
	ENDFUNC_USING_BY_VOICE
}






void RealseIdentifyInfo()
{
	if( NULL != g_aitalk_open_keycode )
	{
		free(g_aitalk_open_keycode);
		g_aitalk_open_keycode = NULL;
	}

	if( NULL != g_aitalk_close_keycode )
	{
		free(g_aitalk_close_keycode);
		g_aitalk_close_keycode = NULL;
	}

	if( NULL != g_aitalk_lexicon_setting )
	{
		for(int i=0; i<200; i++)
		{
			if( NULL != g_aitalk_lexicon_setting[i].item )
			{
				free(g_aitalk_lexicon_setting[i].item);
				g_aitalk_lexicon_setting[i].item = NULL;
			}

			if( NULL != g_aitalk_lexicon_setting[i].KeyCode)
			{
				free(g_aitalk_lexicon_setting[i].KeyCode);
				g_aitalk_lexicon_setting[i].KeyCode = NULL;
			}
		}

		free(g_aitalk_lexicon_setting);
		g_aitalk_lexicon_setting = NULL;
	}
	
	g_aitalk_lexicon_setting_num = 0;

	return;
}


bool LoadIdentifyInfo()
{
	int ret;
	char buffer[256];
	char key[64];

	//装载配置文件
	if(0 > IBParseConfigTool::read_config_load("./voice.ini"))
	{
		IBD_PRINTF(4,"[voice] cann't open voice.ini file!\n");
		return false;
	}

	g_aitalk_lexicon_setting = (Lexicon_Setting*)malloc(sizeof(Lexicon_Setting)*200);
	if(NULL == g_aitalk_lexicon_setting)
	{
		IBD_PRINTF(4,"[voice] malloc error!    size(%d)(%d)\n", sizeof(Lexicon_Setting), sizeof(Lexicon_Setting)*200);
		return false;
	}

	for(int i=0; i<200; i++)
	{
		g_aitalk_lexicon_setting[i].item = NULL;
		g_aitalk_lexicon_setting[i].KeyCode = NULL;
	}

	char item_name[16];
	g_aitalk_lexicon_setting_num=0;
	//读关键词及其对应的遥控码
	for(int i=0; i<200; i++)
	{
		sprintf(item_name, "word%d", i);
		if(R_INNOV_BOX_COMMON_ITEM_NOFOUND == (ret = IBParseConfigTool::read_config((char *)"identify word", (char*)item_name, key, 64)))
		{
			//IBD_PRINTF(4,"cann't find %s on config file\n", item_name);
		}
		else if( ret < 0 )
		{
			//IBD_PRINTF(4,"read config file error!\n");
		}
		else
		{
			sprintf(item_name, "KeyCode%d", i);
			if(R_INNOV_BOX_COMMON_ITEM_NOFOUND == (ret = IBParseConfigTool::read_config((char *)"identify word", (char*)item_name, buffer, 256)))
			{
				//IBD_PRINTF(4,"cann't find %s on config file\n", name);
			}
			else if( ret < 0 )
			{
				//IBD_PRINTF(4,"read config file error!\n");
			}
			else
			{
				//将读出的关键词和对应的遥控码加入数组中
				g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].item = (char*)malloc(strlen(key)+1);
				if(NULL == g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].item)
				{
					IBD_PRINTF(4,"[voice] malloc error!\n");
					return false;
				}

				strcpy(g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].item, key);

				g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].KeyCode = (char*)malloc(strlen(buffer)+1+1);
				if(NULL == g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].KeyCode)
				{
					IBD_PRINTF(4,"[voice] malloc error!\n");
					return false;
				}
				strcpy(g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].KeyCode, buffer);
				g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].KeyCode[strlen(buffer)] = 10;
				g_aitalk_lexicon_setting[g_aitalk_lexicon_setting_num].KeyCode[strlen(buffer)+1] = 0;

				g_aitalk_lexicon_setting_num++;
			}
		}
	}

	//卸载配置文件
	IBParseConfigTool::read_config_unload();
	
	return true;
}

AiTalk::AiTalk(QThread *parent)
	:QThread(parent),
	m_bExitRun(false),
	m_bState(false),
	m_hESRObj(NULL),
	m_pResourceFile(NULL),
	m_pIvTTSInterface(NULL),
	m_bAllowEnterIdentifyMode(true)
{
	BEGINFUNC_USING_BY_VOICE

	ivBool bRet;
	
	audioServerBusy = false;

	/* 从voice.ini中载入识别信息*/
	if( !LoadIdentifyInfo() )
	{
		IBD_PRINTF(4, "[aiTalk.cpp] load identify info error from voice.ini !\n");
		AitalkDestryObject();
		throw;
	}

	bRet = AitalkCreateObject();
	if( !bRet )
	{
		IBD_PRINTF(4, "[Aitalk.cpp]--{Aitalk}: AitalkCreateObject Error........\n");
		throw;
	}
	
	/* 词典，句式，场景的设计和输入 */
	bRet = AitalkInputTextData();
	if( !bRet )
	{
		AitalkDestryObject();
		IBD_PRINTF(4, "[Aitalk.cpp]--{Aitalk}: AitalkInputTextData Error........\n");
		throw;
	}

	/* 创建装载音频PCM数据的线程*/
	m_pPCMDataLoader = new PCMDataLoader(m_hESRObj);
	if( NULL == m_pPCMDataLoader )
	{
		AitalkDestryObject();
		IBD_PRINTF(4, "[Aitalk.cpp]--{Aitalk}: New m_pPCMDataLoader Error........\n");
		throw;
	}

	ENDFUNC_USING_BY_VOICE
}


AiTalk::~AiTalk()
{
	BEGINFUNC_USING_BY_VOICE

	ivBool bRet;

	/* 销毁装载音频PCM数据的线程*/
	if( NULL != m_pPCMDataLoader )
	{
		m_pPCMDataLoader->exitFromRun();
		m_pPCMDataLoader->wait();
		delete m_pPCMDataLoader;
		m_pPCMDataLoader = NULL;
	}

	/*停止语音识别线程*/
	exitFromRun();
	wait();
	
	/* Aitalk CE 2.2对象销毁 */
	bRet = AitalkDestryObject();

	/* 关闭资源文件*/
	if( NULL != m_pResourceFile )
	{
		fclose(m_pResourceFile);
		m_pResourceFile = NULL;
	}

	RealseIdentifyInfo();
	
	ENDFUNC_USING_BY_VOICE
}

void AiTalk::exitFromRun()
{
	BEGINFUNC_USING_BY_VOICE

	m_bExitRun = true;

	ENDFUNC_USING_BY_VOICE
}

void AiTalk::run()
{
	BEGINFUNC_USING_BY_VOICE

	int fd;
	
	ivESRStatus    iStatus;
	
	fd = open("/dev/key", O_RDWR);

#if( VOICE_OPEN_CLOSE_MODE == 0 )
	usleep(2000000);//播放"开始语音识别"需要大约2秒多，所以没必要那么早打开语音识别
#elif( VOICE_OPEN_CLOSE_MODE == 1 )
	usleep(500000);//播放短暂的提示音需要大约0.5秒多，所以没必要那么早打开语音识别
#elif( VOICE_OPEN_CLOSE_MODE == 2 )
	//什么提示也没有，不用等待
#else

#endif
	/* 开始识别及录音数据的处理 */
	iStatus = EsrStartA(m_hESRObj, (ivCStrA)"141场景");
	EsrEngineReset(m_hESRObj);	
#if 1
	if( NULL != m_pPCMDataLoader )
	{
		/* 设置开始填充数据*/
		/* 注意: 这个函数一定要在前面调用，这样线程开始运行
		                  后，就会马上填充数据*/
		m_pPCMDataLoader->StartAppend();

		/* 开始载入音频PCM数据的线程*/
		m_pPCMDataLoader->start();
	}
#endif
	while(!m_bExitRun)
	{	
		ivUInt32		iSyntax;
		ivPUInt32	pCmdID;
		ivUInt32		nSame;
		
		iStatus = EsrRunStep(m_hESRObj);
		
		if( ivESR_RESULT == iStatus )
		{
			m_pPCMDataLoader->StopAppend();
			IBD_PRINTF(4, "\n[aiTalk.cpp] get a result\n");

			/* 获取识别出的句式*/
			if( ivESR_OK != (iStatus = EsrGetResultSyntax(m_hESRObj,&iSyntax)) )
			{
				/* 得到句式错误*/
				IBD_PRINTF(4, "[aiTalk.cpp] EsrGetResultSyntax error! iStatus[%ld]\n", iStatus);
			}
			else if( 1 != iSyntax )
			{
				/* 得到了错误的句式ID，我们只有ID = 1 的句式*/
				IBD_PRINTF(4, "[aiTalk.cpp] iSyntax isn't 1, but we only have iSyntax whose id is 1!!!\n");
			}
			/* 获取识别出的词条(141词典) */
			else if( ivESR_OK != (iStatus = EsrGetResultParameterA(m_hESRObj, &pCmdID, &nSame, (ivCStrA)"141词典")) )
			{
				/* 得到了错误的句式ID，我们只有ID = 1 的句式*/
				IBD_PRINTF(4, "[aiTalk.cpp] EsrGetResultParameterA error! iStatus[%ld]\n", iStatus);
			}
			else if( 1 != nSame )
			{
				/* 我们的句式每次只能获得一条词条，不等于1 肯定错了*/
				IBD_PRINTF(4, "[aiTalk.cpp] nSame isn't 1, but we only can get one item everytime!!!\n");
			}
			else
			{
				while(audioServerBusy){
					dbug_out("Get a resault Aitalk wait audioServerBusy");
					usleep(10000);
				}
				m_mutexAiTalkAudioServerBusy.lock();
				audioServerBusy = true;
				m_mutexAiTalkAudioServerBusy.unlock();

#if 1
				audioServer->closeAudio(IBAC_DEV_ID_MIC);
#else
				if(g_pAudioClientCtl == NULL){
					dbug_out("g_pAudioClientCtl is NULL");
					continue;
				}

				int resault = 0;
				resault = g_pAudioClientCtl->closeAudio(IBAC_DEV_ID_MIC);
				if(resault != 0)
				{
					dbug_out("Close mic error");			
				}
#endif
				/* 终于，我们获得了正确的词条*/
				ioctl(fd, atoi(g_aitalk_lexicon_setting[pCmdID[0]-1].KeyCode), NULL);

				/*合成、播放识别出的词*/
				if( NULL != m_pIvTTSInterface )
				{
					m_pIvTTSInterface->PlayString(User_AiTalk, g_aitalk_lexicon_setting[pCmdID[0]-1].item);
				}
				sleep(2);
#if 1
				audioServer->openAudio(IBAC_DEV_ID_MIC, -1);
#else
				if(g_pAudioClientCtl == NULL){
					dbug_out("g_pAudioClientCtl is NULL");
					continue;
				}

				resault = g_pAudioClientCtl->openAudio(IBAC_DEV_ID_MIC, -1);
				if(resault != 0)
				{
					dbug_out("Open mic error");			
				}
#endif
				m_mutexAiTalkAudioServerBusy.lock();
				audioServerBusy = false;
				m_mutexAiTalkAudioServerBusy.unlock();
			}

			EsrEngineReset(m_hESRObj);
			usleep(300000);
			m_pPCMDataLoader->StartAppend();
		}
		else if(ivESR_FORCERESULT == iStatus)
		{
			/* 强制获取识别结果，识别过程非正常结束，语音超时或者用户手工结束了识别过程才会返回ivESR_FORCERESULT */
			dbug_out("[aiTalk.cpp] get a force result! \n");
			EsrEngineReset(m_hESRObj);
		}
		else if(ivESR_RESPONSETIMEOUT == iStatus)
		{
			/* 用户反应超时，即用户开启识别后，长时间不说话，超过了设定了反应超时时间 */
			dbug_out("[aiTalk.cpp] time out !\n");
			EsrEngineReset(m_hESRObj);	
		}
		else if(ivESR_ASR_ENDED == iStatus)
		{
			/* 识别过程已经结束，需要调用EsrReset重启识别，手工结束了识别过程后，Aitalk CE 2.2 已经处理完所有缓存的录音数据并返回了ivESR_FORCERESULT后继续调用EsrRunStep会返回ivESR_ASR_ENDED */
			dbug_out("[aiTalk.cpp] identify is ended\n");
			EsrEngineReset(m_hESRObj);	
		}
		else if(ivESR_SPEECHSTART == iStatus)
		{
			/* 检测到语音开始通知，Aitalk CE 2.2可以检测到用户是否开始说话 */
			dbug_out("[aiTalk.cpp] aitalk start !\n");
		}
		else if(ivESR_BUFFEREMPTY == iStatus)
		{
			/* Aitalk CE 2.2缓存的录音数据已经处理完了，可以让处理器处理其他任务或休息20毫秒 */
			usleep(20000);
		}
		else if(ivESR_OK == iStatus)
		{
			/* 正常处理完一帧录音数据，可以继续调用EsrRunStep继续处理Aitalk CE 2.2缓存的录音数据 */
			//VOICE_PRINT("[aiTalk.cpp] do one PCM frame!");
		}
		else
		{
			/* 有错误发生，请根据返回值尝试解决问题，若无法解决请联系技术支持 */
			dbug_out("EsrRunStep error, we will reset the engine!\n");
			EsrEngineReset(m_hESRObj);
		}
	}

	iStatus = EsrStop(m_hESRObj);
	dbug_out("EsrStop iStatus[%ld]\n", iStatus);
#if 1
	/* 关闭载入音频PCM数据的线程, 并且设置停止填充数据*/	
	if( NULL != m_pPCMDataLoader )
	{
		m_pPCMDataLoader->exitFromRun();
		m_pPCMDataLoader->wait();
		m_pPCMDataLoader->StopAppend();
	}
#endif
	m_bExitRun = false;
#if 1

#endif

	close(fd);
	ENDFUNC_USING_BY_VOICE
}

bool AiTalk::StartIdentify()
{
	BEGINFUNC_USING_BY_VOICE

	m_mutexAiTalk.lock();

	if(!m_bAllowEnterIdentifyMode)
	{
		m_mutexAiTalk.unlock();
		return false;
	}
	if(m_bState)
	{
		m_mutexAiTalk.unlock();
		return true;	
	}

	/*保存语音识别状态*/
	/*注意:	m_bState的改变，必须放在StartIdentify的前部，因为m_bState为true标志着voice要占用一些设备(dsp、i2sin)，
				而占用这些设备正是本函数执行下去的条件*/
	m_bState = true;

	/*发送特定遥控码通知其他进程，语音识别功能已经打开*/
	NotifyState();
	
	/*开始语音识别线程*/
	start();


	m_mutexAiTalk.unlock();

	ENDFUNC_USING_BY_VOICE

	return true;
}

bool AiTalk::StopIdentify()
{
	BEGINFUNC_USING_BY_VOICE

	m_mutexAiTalk.lock();

	if( !m_bState )
	{
		m_mutexAiTalk.unlock();
		return true;	
	}
	
	/*停止语音识别线程*/
	exitFromRun();

	wait();

	/*保存语音识别状态*/
	/*注意:	m_bState的改变，必须放在StartIdentify的前部，因为m_bState为true标志着voice要占用一些设备(dsp、i2sin)，
				而占用这些设备正是本函数执行下去的条件*/
	m_bState = false;

	/*发送特定遥控码通知其他进程，语音识别功能已经关闭*/
	NotifyState();

	m_mutexAiTalk.unlock();

	ENDFUNC_USING_BY_VOICE

	return true;
}

bool AiTalk::GetIdentifyState()
{
	BEGINFUNC_USING_BY_VOICE

	ENDFUNC_USING_BY_VOICE

	return m_bState;
	
}

void AiTalk::NotifyState()
{
	BEGINFUNC_USING_BY_VOICE
	int fd;
	
	fd = open("/dev/key", O_RDWR);

	if(m_bState)
	{
		/*语音识别功能已经打开*/
		ioctl(fd, SEND_AITALK_AISTART, NULL);
	}
	else
	{
		/*语音识别功能已经关闭*/
		ioctl(fd, SEND_AITALK_AISTOP, NULL);
	}
	close(fd);
	ENDFUNC_USING_BY_VOICE

}

#if 0
void AiTalk::ProhibitEnterIdentifyMode()
{
	BEGINFUNC_USING_BY_VOICE
	
	m_bAllowEnterIdentifyMode = false;
	usleep(300);	//下面，我们要调用m_pAiTalkInterface->StopIdentify()来退出语音识别模式
				//这个usleep用来确保遥控器线程中如果调用m_pAiTalkInterface->StartIdentify()
				//一定在我们调用m_pAiTalkInterface->StopIdentify()之前来进行，避免出现
				//我们停止语音识别，遥控器又打开语音识别的巧合情况。
				//呵呵，其实这种可能性应该很微小吧
				
	StopIdentify();
	
	ENDFUNC_USING_BY_VOICE
}

void AiTalk::AllowEnterIdentifyMode()
{
	BEGINFUNC_USING_BY_VOICE
	
	m_bAllowEnterIdentifyMode = true;

	ENDFUNC_USING_BY_VOICE
}
#endif

/* 记录日志回调函数 */
static void ivCall UserLogCB(ivPointer pUserParam, ivCPointer pLogData, ivSize nBytes)
{
	/* TODO: 客户平台读取资源操作，下面实现是以Windows平台为例，供参考 */

	FILE *fp = (FILE *)pUserParam;
	fwrite(pLogData, nBytes, 1, fp);
	fflush(fp);
}

/* 读取资源回调函数 */
static void ivCall UserReadResCB(ivPointer pParameter, ivPointer pBuffer, ivResAddress iPos, ivResSize nSize)
{
	/* TODO: 客户平台读取资源操作，下面实现是以Windows平台为例，供参考 */

	FILE* pFile = (FILE*)pParameter;
	fseek(pFile, iPos, SEEK_SET);
	fread(pBuffer, nSize, 1, pFile);
}

/* 内存分配回调函数 */
static ivPointer ivCall UserReallocCB(ivPointer pUserParam, ivPointer pMemBlock, ivSize nBytes)
{
	pUserParam = NULL;
	return realloc(pMemBlock, nBytes);
}

/* 内存释放回调函数 */
static void ivCall UserFreeCB(ivPointer pUserParam, ivPointer pMemBlock)
{
	pUserParam = NULL;
	return free(pMemBlock);
}

/* 保存数据回调函数 */
static ivBool ivCall UserSaveDataCB(ivPointer pUserParam, ivPointer pSrcBuffer, ivSize nBufferBytes)
{
     FILE* fp;
	  pUserParam = NULL;
     fp = fopen(VOICE_IDENTIFY_ADAPT_FILE, "wb+");
     fwrite(pSrcBuffer, nBufferBytes, 1, fp);
     fclose(fp);

     return ivTrue;
}

/* 装载数据回调函数 */
static ivBool ivCall UserLoadDataCB(ivPointer pUserParam, ivPointer pDstBuffer, ivSize ivPtr pnBufferBytes)
{
        FILE* fp;
		  pUserParam = NULL;
        ivUInt32 nSize;
        fp = fopen(VOICE_IDENTIFY_ADAPT_FILE, "rb");
        if(ivNull == fp){
                return ivFalse;
        }
        fseek(fp, 0, SEEK_END);
        nSize = ftell(fp);
        if(nSize > *pnBufferBytes){
                fclose(fp);
                return ivFalse;
        }
        fseek(fp, 0, SEEK_SET);
        fread(pDstBuffer, nSize, 1, fp);
        fclose(fp);

        *pnBufferBytes = nSize;

        return ivTrue;
}

ivBool AiTalk::AitalkDestryObject()
{
	BEGINFUNC_USING_BY_VOICE

	ivESRStatus		iStatus;

	if(m_hESRObj)
	{
		iStatus = EsrDestroy(m_hESRObj);
		if(ivESR_OK != iStatus)
		{
			return ivFalse;
		}
	}

	ENDFUNC_USING_BY_VOICE

	return ivTrue;
}

ivBool AiTalk::AitalkCreateObject()
{
	BEGINFUNC_USING_BY_VOICE

	TUserSys		tUserSys;			/* 用户系统结构体 */
	ivTResPackDesc	tResPackDesc;		/* 资源访问描述结构体 */
	ivESRStatus		iStatus;			/* API调用返回值 */


/* 让Aitalk CE 2.2使用静态内存 */
#if 0
	memset(pAiWorkBuffer, 0, sizeof(char) * AIWORKBUFFSIZE);
	memset(pAiResidentBuffer, 0, sizeof(char) * AIRESIBUFFSIZE);

	tUserSys.pWorkBuffer = pAiWorkBuffer;
	tUserSys.nWorkBufferBytes = AIWORKBUFFSIZE;
	tUserSys.pResidentBuffer = pAiResidentBuffer;
	tUserSys.nResidentBufferBytes = AIRESIBUFFSIZE;
	tUserSys.pfnRealloc = ivNull;
	tUserSys.pfnFree = ivNull;
	
#else

	/* 让Aitalk CE 2.2使用动态内存分配 */
	tUserSys.pWorkBuffer = ivNull;
	tUserSys.nWorkBufferBytes = 0;
	tUserSys.pResidentBuffer = ivNull;
	tUserSys.nResidentBufferBytes = 0;
	tUserSys.pfnRealloc = UserReallocCB;
	tUserSys.pfnFree = UserFreeCB;
#endif

	tUserSys.pfnSaveData = UserSaveDataCB;
	tUserSys.pfnLoadData = UserLoadDataCB;


	tUserSys.pfnStartRecord = ivNull;
	tUserSys.pfnStopRecord = ivNull;

	tUserSys.bCheckResource = ivTrue;

#if 0
	tUserSys.pCBParam = fopen(AITALK_LOG_FILE, "wb+");
	tUserSys.pfnLog = UserLogCB;
#else
	tUserSys.pCBParam = fopen(AITALK_LOG_FILE, "wb+");
	tUserSys.pfnLog = UserLogCB;
#endif
	tResPackDesc.nCacheBlockCount = 0;
	tResPackDesc.nCacheBlockExt = 0;
	tResPackDesc.nCacheBlockSize = 0;
	tResPackDesc.nSize = 0;
	tResPackDesc.pCacheBlockIndex = ivNull;
	tResPackDesc.pCacheBuffer = ivNull;
	tResPackDesc.pfnMap = ivNull;
	tResPackDesc.pCBParam = m_pResourceFile = fopen(IVESR_FILE, "rb");
	tResPackDesc.pfnRead = UserReadResCB;

	if( NULL == tResPackDesc.pCBParam )
	{
		IBD_PRINTF(4, "[aiTalk.cpp] open ivESR.irf file error\n");
		return ivFalse;
	}

	if(m_hESRObj)
	{
		iStatus = EsrDestroy(m_hESRObj);
		if(ivESR_OK != iStatus)
		{
			/* TODO: 销毁Aitalk CE 2.2对象失败后操作 */
			return ivFalse;
		}
	}

	m_hESRObj = ivNull;

	/* 建立识别引擎 */
	iStatus = EsrCreate(&m_hESRObj, &tUserSys, &tResPackDesc, (ivSize)1);

	if(ivESR_OK != iStatus)
	{
		/* TODO: 创建Aitalk CE 2.2对象失败后操作，请检查以上参数设置 */
		return ivFalse;
	}

	ENDFUNC_USING_BY_VOICE

	return ivTrue;
}

ivBool AiTalk::AitalkInputTextData()
{
	BEGINFUNC_USING_BY_VOICE

	ivESRStatus iStatus;

	/* 输入词典 */
	iStatus = EsrSetACP(m_hESRObj, ivESR_CP_GBK);//开始
	
	iStatus = EsrBeginLexiconA(m_hESRObj, (ivCStrA)"141词典");//词典名称

	for( int i=0; i<g_aitalk_lexicon_setting_num; i++)
	{
		iStatus = EsrAddLexiconItemA(m_hESRObj, (ivCStrA)(g_aitalk_lexicon_setting[i].item), i+1);
	}

	iStatus = EsrEndLexicon(m_hESRObj);//结束

	/* 输入场景 */
	iStatus = EsrBeginSceneA(m_hESRObj, (ivCStrA)"141场景");

	/* 输入句式-- 我们只有一个句式*/
	iStatus = EsrAddSyntaxA(m_hESRObj, (ivCStrA)"{141词典}", 1);

	iStatus = EsrEndScene(m_hESRObj);//结束

	ENDFUNC_USING_BY_VOICE

	return ivTrue;
}

bool AiTalk::RegisterInterface(IvTTSInterface* pInterface)
{
	bool retval = false;

	if(NULL != pInterface)
	{
		m_pIvTTSInterface = pInterface;
		retval = true;
	}

	return retval;
}
