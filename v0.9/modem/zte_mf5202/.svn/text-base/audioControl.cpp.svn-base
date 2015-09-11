/**
 * @file   audioControl.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date   20110301
 * 
 * @brief  
 *      Implement for phone audio control and sms audio control
 * 
 */

#include "audioControl.h"
#include "MobileInterFace1.h"
#include "sqlite3.h"
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
#include "AudioManageMessage.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
PhoneAudioControlC::PhoneAudioControlC(PhoneMonitorVoiceReportBase *reportBase)
	: m_reportBase(reportBase),
	  m_iTimerForSendDTMF(-1)
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
PhoneAudioControlC::PhoneAudioControlC()
	: m_iTimerForSendDTMF(-1)
#else
PhoneAudioControlC::PhoneAudioControlC(AudioCtrlClientC *pAudioCtrlClient)
	: m_pAudioCtrlClient(pAudioCtrlClient),
	  m_iTimerForSendDTMF(-1)
#endif
#endif
{

}

PhoneAudioControlC::~PhoneAudioControlC()
{
	m_Player.stop_sign_sound();
}

int PhoneAudioControlC::playRingAudio()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_reportBase->setPhoneMute(false, IBAC_DEV_ID_DAI_RING);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_RING, true);

	AudioControl1::AudioControlLibC::object()->unMute(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_RING, true);
#else
	m_pAudioCtrlClient->openAudio(IBAC_DEV_ID_DAI_RING,-1);

	m_pAudioCtrlClient->unmuteAudio(IBAC_DEV_ID_DAI_RING);
#endif
#endif

	m_Player.play_sign_sound((char *)"DTMF.wav", 1, PlaySignSound::Sound_Type_Ring);
	
	return 0;
}

int PhoneAudioControlC::playDefaultIncomingCallAudio()
{
	int fd;
	if ( (fd = open("home.wav", O_RDONLY)) == -1)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "open home.wav error\n")
		return -1;
	}
	close(fd);
	m_Player.play_sign_sound((char *)"home.wav", 1, PlaySignSound::Sound_Type_Ring);
	return 0;
}

int PhoneAudioControlC::playIncomingCallAudio()
{
	char musicFilePath[64] = {0};
	int retval = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pQueryTablesSel = (char*)"SELECT * FROM CallRing";//²é±íÓï¾ä

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_reportBase->setPhoneMute(false, IBAC_DEV_ID_DAI_RING);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_RING, true);

	//AudioControl1::AudioControlLibC::object()->unMute(AUDIO_CHANNEL_ID_DIGITAL_RING, true);
#else
	
	m_pAudioCtrlClient->openAudio(IBAC_DEV_ID_DAI_RING,-1);

	//m_pAudioCtrlClient->unmuteAudio(IBAC_DEV_ID_DAI_RING);
#endif
#endif

#if 0
	retval = sqlite3_open(FILE_NAME, &pDB);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_open error : %s!\n", sqlite3_errmsg(pDB))
		sqlite3_close(pDB);

		playDefaultIncomingCallAudio();

		return PHONE_MONITOR_GET_DATA_ERROR;
	}

	retval = sqlite3_prepare(pDB, pQueryTablesSel, strlen(pQueryTablesSel), &pStmt, (const char**)&pError);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error : %s!\n", sqlite3_errmsg(pDB))
		sqlite3_close(pDB);

		playDefaultIncomingCallAudio();

		return PHONE_MONITOR_GET_DATA_ERROR;
	}

	retval = sqlite3_step(pStmt);
	if (retval != SQLITE_ROW)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_step error : %s!\n", sqlite3_errmsg(pDB))
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);
		
		playDefaultIncomingCallAudio();

		return PHONE_MONITOR_GET_DATA_ERROR;
	}
	strcpy(musicFilePath, (char*)sqlite3_column_text(pStmt, 0));

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	
	IBD_PRINTF(DEBUG_INFOMATION, "read music file path is %s\n", musicFilePath)

	m_Player.play_sign_sound(musicFilePath, 1, PlaySignSound::Sound_Type_Ring);
#endif
        playDefaultIncomingCallAudio();
	return 0;
}

int PhoneAudioControlC::playDTMFAudio()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_reportBase->setPhoneMute(false, IBAC_DEV_ID_DAI_RING);
        m_reportBase->setPhoneMute(true, IBAC_DEV_ID_3G_SWITCH);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->close(AudioControl1::AUDIO_CHANNEL_ID_ANALOG_PHONE, false);

	AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_RING, true);

	AudioControl1::AudioControlLibC::object()->unMute(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_RING, true);
#else
	m_pAudioCtrlClient->openAudio(IBAC_DEV_ID_DAI_RING,-1);

	m_pAudioCtrlClient->closeAudio(IBAC_DEV_ID_3G_PHONE_SPEAKER);

	m_pAudioCtrlClient->unmuteAudio(IBAC_DEV_ID_DAI_RING);
#endif
#endif
	
	m_Player.play_sign_sound((char *)"DTMF.wav", 0, PlaySignSound::Sound_Type_Ring);

	if( m_iTimerForSendDTMF >= 0 )
	{
		killTimer(m_iTimerForSendDTMF);
		m_iTimerForSendDTMF = -1;
	}
	
	m_iTimerForSendDTMF = startTimer(7000);
	
	return 0;
}

void PhoneAudioControlC::timerEvent(QTimerEvent * event)
{
	if( m_iTimerForSendDTMF == event->timerId() )
	{
		killTimer(m_iTimerForSendDTMF);
		m_iTimerForSendDTMF = -1;

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_reportBase->setPhoneMute(false, IBAC_DEV_ID_3G_SWITCH);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
		AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_ANALOG_PHONE, false);
#else		
		m_pAudioCtrlClient->openAudio(IBAC_DEV_ID_3G_PHONE_SPEAKER, -1);
#endif
#endif
		stopPlay();
	}
}

void	PhoneAudioControlC::stopPlay()
{
	m_Player.stop_sign_sound();

	if( m_iTimerForSendDTMF >= 0 )
	{
		killTimer(m_iTimerForSendDTMF);
		m_iTimerForSendDTMF = -1;
	}
	
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_reportBase->setPhoneMute(true, IBAC_DEV_ID_DAI_RING);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->close(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_RING, false);
#else		
	m_pAudioCtrlClient->closeAudio(IBAC_DEV_ID_DAI_RING);
#endif
#endif
}

int PhoneAudioControlC::mute()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_reportBase->setPhoneMute(true, IBAC_DEV_ID_DAI_RING);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->mute(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_RING, false);
#else
	m_pAudioCtrlClient->muteAudio(IBAC_DEV_ID_DAI_RING);
#endif
#endif
return 0;
}

int PhoneAudioControlC::unMute()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_reportBase->setPhoneMute(false, IBAC_DEV_ID_DAI_RING);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->unMute(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_RING, false);
#else
	m_pAudioCtrlClient->unmuteAudio(IBAC_DEV_ID_DAI_RING);
#endif
#endif
        return 0;
}












#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
SmsAudioControlC::SmsAudioControlC(PhoneMonitorVoiceReportBase *reportBase)
        : m_reportBase(reportBase)
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
SmsAudioControlC::SmsAudioControlC()
#else
SmsAudioControlC::SmsAudioControlC(AudioCtrlClientC *pAudioCtrlClient)
	: m_pAudioCtrlClient(pAudioCtrlClient)
#endif
#endif
{
	connect(&m_Player, SIGNAL(finishedPlay()), this, SLOT(slotFinished()), Qt::QueuedConnection);
}

SmsAudioControlC::~SmsAudioControlC()
{
	m_Player.stop_sign_sound();
}

int SmsAudioControlC::playAudio()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_reportBase->setPhoneMute(false, IBAC_DEV_ID_DAI_SMSRING);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->open(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_SMSRING, false);
#else
	m_pAudioCtrlClient->openAudio(IBAC_DEV_ID_DAI_SMSRING,-1);
#endif
#endif
	m_Player.play_sign_sound("NewsSound.pcm",0, PlaySignSound::Sound_Type_Smsring);
	
	return 0;
}

void	SmsAudioControlC::stopPlay()
{
	m_Player.stop_sign_sound();

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_reportBase->setPhoneMute(true, IBAC_DEV_ID_DAI_SMSRING);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->close(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_SMSRING, false);
#else
	m_pAudioCtrlClient->closeAudio(IBAC_DEV_ID_DAI_SMSRING);
#endif
#endif
}

void SmsAudioControlC::slotFinished()
{
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        m_reportBase->setPhoneMute(true, IBAC_DEV_ID_DAI_SMSRING);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioControl1::AudioControlLibC::object()->close(AudioControl1::AUDIO_CHANNEL_ID_DIGITAL_SMSRING, false);
#else
	m_pAudioCtrlClient->closeAudio(IBAC_DEV_ID_DAI_SMSRING);
#endif
#endif
}

