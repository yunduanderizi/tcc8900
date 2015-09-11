
#include <math.h>
#include <sqlite3.h>
#include "common_config.h"
#include "debugMonitor.h"
#include "audioControlErrCode.h"
#include "audioControl.h"
#include "audioDevice.h"
#include "parseConfig.h"
#include "myDbus.h"

namespace AudioControl1
{





AudioControlC  *AudioControlC::thisObject = NULL;
AudioControlC  *AudioControlC::object()
{
	AudioControlC *pTmp = NULL;
		
	if( !thisObject )
	{
		thisObject = new AudioControlC();
	}

	pTmp = thisObject;
	
	return pTmp;
}

void AudioControlC::releaseObject()
{
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
}

AudioControlC::AudioControlC()
	: m_iCurrentOpenLevel(-1),
	  m_iSpeedStartForVolSpeed(-1),
	  m_iVolEndForVolSpeed(-1),
	  m_iSpeedIntervalForVolSpeed(-1),
	  m_iVolIncrementForVolSpeed(-1),
	  m_bOpenVolSpeed(false),
	  m_iCurrentSpeedForVolSpeed(0),
	  m_iTimerFreshData(-1),
	  m_iTimerFreshDataDelay(1000),
	  m_iTimerSaveData(-1),
	  m_iTimerSaveDataDelay(5000)
{
	m_AudioLevelList.clear();

	_loadDataFromConfig();
	_loadDataFromDB();

	m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN]->status = AUDIO_CHANNEL_OPEN;
	m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN_LEFT]->status = AUDIO_CHANNEL_OPEN;
	m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN_RIGHT]->status = AUDIO_CHANNEL_OPEN;

	if( WORK_MODE_SPECIAL == m_WorkMode )
	{
		m_pAudioChannel[AUDIO_CHANNEL_ID_RELATION]->status = AUDIO_CHANNEL_OPEN;

		for(int i=AUDIO_CHANNEL_ID_RELATION+1; i<AUDIO_CHANNEL_ID_MAX; i++)
		{
			AudioDeviceC::object()->setAudioChanVolume((AudioChannelIdE)i, VOL_MAX_VALUE);
		}
	}

	_updateAudioChannelDataToDevice(AUDIO_CHANNEL_ID_MAIN);
	_updateAudioChannelDataToDevice(AUDIO_CHANNEL_ID_MAIN_LEFT);
	_updateAudioChannelDataToDevice(AUDIO_CHANNEL_ID_MAIN_RIGHT);

	IBD_LOOP_BEGIN(DEBUG_INFOMATION, "\n")
	for(int i=0; i<AUDIO_CHANNEL_MAX_NUMBER; i++)
	{
		if( m_pAudioChannel[i] )
		{
			if( m_pAudioChannel[i]->bValid )
			{
				IBD_LOOP_PRINTF(DEBUG_INFOMATION, "channel id[%d] name[%s] level[%d] status[%d] virtual vol[%d] actual vol max[%d] mute[%d]\n", 
				m_pAudioChannel[i]->id, m_pAudioChannel[i]->name.toUtf8().data(), m_pAudioChannel[i]->idLevel,
				m_pAudioChannel[i]->status, m_pAudioChannel[i]->iVirtualVol, m_pAudioChannel[i]->iActualVolMax,
				m_pAudioChannel[i]->bMute)
			}
			else
			{
				IBD_LOOP_PRINTF(DEBUG_INFOMATION, "it is impossible to come here\n");
			}
		}
	}

	IBD_LOOP_PRINTF(DEBUG_INFOMATION, "m_iMixMainUpValue[%d] m_iMixOtherDownValue[%d]\n", m_iMixMainUpValue, m_iMixOtherDownValue)
	IBD_LOOP_PRINTF(DEBUG_INFOMATION, "m_iSpeedStartForVolSpeed[%d] m_iVolEndForVolSpeed[%d] m_iVolIncrementForVolSpeed[%d] m_iSpeedIntervalForVolSpeed[%d]\n", m_iSpeedStartForVolSpeed, m_iVolEndForVolSpeed, m_iVolIncrementForVolSpeed, m_iSpeedIntervalForVolSpeed)
	IBD_LOOP_PRINTF(DEBUG_INFOMATION, "m_bOpenVolSpeed[%d] m_iCurrentSpeedForVolSpeed[%d]\n", m_bOpenVolSpeed, m_iCurrentSpeedForVolSpeed)
	IBD_LOOP_PRINTF(DEBUG_INFOMATION, "\n")
	IBD_LOOP_END
	
}

AudioControlC::~AudioControlC()
{
	_saveDataToDB();

	for(int iLevel=0; iLevel<m_AudioLevelList.count(); iLevel++)
	{
		m_AudioLevelList[iLevel].audioChannelList.clear();
	}

	if( m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN] )
	{
		delete m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN];
		m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN] = NULL;
	}

	if( m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN_LEFT] )
	{
		delete m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN_LEFT];
		m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN_LEFT] = NULL;
	}

	if( m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN_RIGHT] )
	{
		delete m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN_RIGHT];
		m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN_RIGHT] = NULL;
	}

	if( m_pAudioChannel[AUDIO_CHANNEL_ID_RELATION] )
	{
		delete m_pAudioChannel[AUDIO_CHANNEL_ID_RELATION];
		m_pAudioChannel[AUDIO_CHANNEL_ID_RELATION] = NULL;
	}
}

int AudioControlC::open(AudioChannelIdE audioChannelId)
{
	int retval = AC_E_OK;
	bool	bDone = false;

	retval = _checkAudioChannelId(audioChannelId, ACC_OPEN);
	if( AC_E_OK != retval )
		return retval;

	retval = _openForSpecialLogic(audioChannelId, bDone);
	if( !bDone )
	{
		retval = _openForCommonLogic(audioChannelId);
	}

	return retval;
}

int AudioControlC::close(AudioChannelIdE audioChannelId)
{
	int retval = AC_E_OK;
	bool	bDone = false;

	retval = _checkAudioChannelId(audioChannelId, ACC_CLOSE);
	if( AC_E_OK != retval )
		return retval;
	
	retval = _closeForSpecialLogic(audioChannelId, bDone);
	if( !bDone )
	{
		retval = _closeForCommonLogic(audioChannelId);
	}

	return retval;
}

int AudioControlC::adjustVol(AudioChannelIdE audioChannelId, int vol)
{
	int retval = AC_E_OK;
	bool	bDone = false;

	retval = _checkAudioChannelId(audioChannelId, ACC_ADJUST_VOL);
	if( AC_E_OK != retval )
		return retval;

	if( vol < 0 || vol > VOL_MAX_VALUE )
		return AC_E_ARG_INVALID;
	
	retval = _adjustVolForSpecialLogic(audioChannelId, vol, bDone);
	if( !bDone )
	{
		retval = _adjustVolForCommonLogic(audioChannelId, vol);
	}

	if( AC_E_OK == retval )
	{
		if( m_iTimerSaveData >= 0 )
		{
			killTimer(m_iTimerSaveData);
			m_iTimerSaveData = -1;
		}
		m_iTimerSaveData = startTimer(m_iTimerSaveDataDelay);
	}
	
	return retval;
}

int AudioControlC::mute(AudioChannelIdE audioChannelId)
{
	int retval = AC_E_OK;
	bool	bDone = false;

	retval = _checkAudioChannelId(audioChannelId, ACC_MUTE);
	if( AC_E_OK != retval )
		return retval;
	
	retval = _muteForSpecialLogic(audioChannelId, bDone);
	if( !bDone )
	{
		retval = _muteForCommonLogic(audioChannelId);
	}

	if( AC_E_OK == retval )
	{
		if( m_iTimerSaveData >= 0 )
		{
			killTimer(m_iTimerSaveData);
			m_iTimerSaveData = -1;
		}
		m_iTimerSaveData = startTimer(m_iTimerSaveDataDelay);
	}

	return retval;
}

int AudioControlC::unMute(AudioChannelIdE audioChannelId)
{
	int retval = AC_E_OK;
	bool	bDone = false;

	retval = _checkAudioChannelId(audioChannelId, ACC_UNMUTE);
	if( AC_E_OK != retval )
		return retval;
	
	retval = _unMuteForSpecialLogic(audioChannelId, bDone);
	if( !bDone )
	{
		retval = _unMuteForCommonLogic(audioChannelId);
	}

	if( AC_E_OK == retval )
	{
		if( m_iTimerSaveData >= 0 )
		{
			killTimer(m_iTimerSaveData);
			m_iTimerSaveData = -1;
		}
		m_iTimerSaveData = startTimer(m_iTimerSaveDataDelay);
	}
	
	return retval;
}

int AudioControlC::setSpeed(int carSpeed)
{
	int retval = AC_E_OK;
	bool	bDone = false;

	retval = _setSpeedForSpecialLogic(carSpeed, bDone);
	if( !bDone )
	{
		retval = _setSpeedForCommonLogic(carSpeed);
	}

	return retval;
}

int AudioControlC::openVolSpeed()
{
	int retval = AC_E_OK;
	bool	bDone = false;

	retval = _openVolSpeedForSpecialLogic(bDone);
	if( !bDone )
	{
		retval = _openVolSpeedForCommonLogic();
	}

	if( AC_E_OK == retval )
	{
		if( m_iTimerSaveData >= 0 )
		{
			killTimer(m_iTimerSaveData);
			m_iTimerSaveData = -1;
		}
		m_iTimerSaveData = startTimer(m_iTimerSaveDataDelay);
	}
	
	return retval;
}

int AudioControlC::closeVolSpeed()
{
	int retval = AC_E_OK;
	bool	bDone = false;
	
	retval = _closeVolSpeedForSpecialLogic(bDone);
	if( !bDone )
	{
		retval = _closeVolSpeedForCommonLogic();
	}

	if( AC_E_OK == retval )
	{
		if( m_iTimerSaveData >= 0 )
		{
			killTimer(m_iTimerSaveData);
			m_iTimerSaveData = -1;
		}
		m_iTimerSaveData = startTimer(m_iTimerSaveDataDelay);
	}

	return retval;
}

int AudioControlC::getCurrentOpenAudioChannel(AudioChannelIdE& audioChannelId)
{
	int retval = AC_E_OK;

	audioChannelId = AUDIO_CHANNEL_ID_INVALID;

	if( WORK_MODE_SPECIAL == m_WorkMode )
	{
		for(int i=AUDIO_CHANNEL_ID_RELATION+1; i<AUDIO_CHANNEL_ID_MAX; i++)
		{
			if( AUDIO_CHANNEL_OPEN == m_pAudioChannel[i]->status )
			{
				audioChannelId = (AudioChannelIdE)i;
				break;
			}
		}
	}
	else
	{
		retval = AC_E_COMMAND_NOT_SUPPORTED;
	}

	return retval;
}

void	AudioControlC::_loadDataFromConfig()
{
	char cBuf[1024];
	int iValue;
	char cItemName[1024];
	AudioChannelC  audioChannel[AUDIO_CHANNEL_ID_MAX];

	for(int i=0; i<AUDIO_CHANNEL_ID_MAX; i++)
	{
		audioChannel[i].id = (AudioChannelIdE)i;
		audioChannel[i].name = g_AudioChannelName[i];
	}

	if( 0 != read_config_load("/application/config/audioControl.conf") )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "open config file [%s] error! \n", "/application/config/audioControl.conf");
		abort();
	}

	if( 0 != read_config((char *)"audio_mode", (char *)"mode", cBuf, 1024) )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "audio_mode", "mode");
		abort();
	}

	if( 0 == strcmp(cBuf, "normal") )
	{
		m_WorkMode = WORK_MODE_NORMAL;
	}
	else if( 0 == strcmp(cBuf, "special") )
	{
		m_WorkMode = WORK_MODE_SPECIAL;
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "content is error at section[%s] item[%s]\n", "audio_mode", "mode");
		abort();
	}

	if( 0 != read_config((char *)"valid_channel", (char *)"channel", cBuf, 1024) )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "valid_channel", "channel");
		abort();
	}
	else
	{
		char* cChannel[AUDIO_CHANNEL_MAX_NUMBER];

		int num = ParseStr(cBuf, cChannel, AUDIO_CHANNEL_MAX_NUMBER);
		for(int i=0; i<num; i++)
		{
			bool bFind = false;
			for(int j=0; j<AUDIO_CHANNEL_ID_MAX; j++)
			{
				if( 0 == strcmp(cChannel[i], audioChannel[j].name.toUtf8().data()) )
				{
					if( audioChannel[j].bValid )
					{
						IBD_PRINTF(DEBUG_INFOMATION, "channel[%s] is repeated at section[%s] item[%s]\n", cChannel[i], "audio_mode", "mode");
						abort();
					}
					else
					{
						audioChannel[j].bValid = true;
					}
					bFind = true;
					break;
				}
			}
			if( !bFind )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "channel[%s] can not be found at section[%s] item[%s]\n", cChannel[i], "audio_mode", "mode");
				abort();
			}
		}
		audioChannel[AUDIO_CHANNEL_ID_MAIN].bValid = true;
		audioChannel[AUDIO_CHANNEL_ID_MAIN_LEFT].bValid = true;
		audioChannel[AUDIO_CHANNEL_ID_MAIN_RIGHT].bValid = true;
		if( WORK_MODE_SPECIAL == m_WorkMode )
			audioChannel[AUDIO_CHANNEL_ID_RELATION].bValid = true;
		
	}

	if( 0 != read_config((char *)"volume_correspondence", (char *)"correspondence", cBuf, 1024) )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "volume_correspondence", "correspondence");
		abort();
	}

	if( 0 == strcmp(cBuf, "liner") )
	{
		m_VolCorrespondence = VOL_CORRESPONDENCE_LINER;
	}
	else if( 0 == strcmp(cBuf, "log") )
	{
		m_VolCorrespondence = VOL_CORRESPONDENCE_LOG;
	}
	else if( 0 == strcmp(cBuf, "table") )
	{
		m_VolCorrespondence = VOL_CORRESPONDENCE_TABLE;
		if( 0 != read_config((char *)"volume_correspondence", (char *)"table_file", cBuf, 1024) )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "volume_correspondence", "table_file");
			abort();
		}

		read_config_unload();
		char table_file[1024];
		strcpy(table_file, "/application/config/");
		strcat(table_file, cBuf);
		if( 0 != read_config_load(table_file) )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "open config file [%s] error! \n", table_file);
			abort();
		}

		for(int i=0; i<=VOL_MAX_VALUE; i++)
		{
			sprintf(cItemName, "%d", i);
			if( 0 != read_config((char *)"table", cItemName, &iValue) )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "table", cItemName);
				abort();
			}

			if( iValue < 0 || iValue > VOL_MAX_VALUE )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error, value must be between %d and %d\n", "table", cItemName, 0, VOL_MAX_VALUE);
				abort();
			}
			
			m_iVolTable[i] = iValue;
		}

		read_config_unload();
		read_config_load("/application/config/audioControl.conf");
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "content is error at section[%s] item[%s]\n", "volume_correspondence", "correspondence");
		abort();
	}

	for(int i=AUDIO_CHANNEL_ID_MIN+1; i<AUDIO_CHANNEL_ID_MAX; i++)
	{
		if( audioChannel[i].bValid && AUDIO_CHANNEL_ID_MAIN_LEFT!=audioChannel[i].id 
			&& AUDIO_CHANNEL_ID_MAIN_RIGHT!=audioChannel[i].id )
		{
			strcpy(cItemName, audioChannel[i].name.toUtf8().data());
			if( 0 != read_config((char *)"actual_max_volume", cItemName, &iValue) )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "actual_max_volume", cItemName);
				abort();
			}
			else
			{
				audioChannel[i].iActualVolMax = iValue;
			}			
		}
	}

	for(int i = AUDIO_CHANNEL_ID_MIN+1; i<AUDIO_CHANNEL_ID_MAX; i++)
	{
		if( audioChannel[i].bValid )
		{
			strcpy(cItemName, audioChannel[i].name.toUtf8().data());
			if( 0 != read_config((char *)"virtual_volume_default", cItemName, &iValue) )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "virtual_volume_default", cItemName);
				abort();
			}
			else
			{
				audioChannel[i].iVirtualVol = iValue;
				audioChannel[i].iVirtualVolDefault = iValue;
			}			
		}
	}

	if( m_WorkMode == WORK_MODE_SPECIAL )
	{
		if( 0 != read_config((char *)"relation_channel",(char *)"channel", cBuf, 1024) )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "relation_channel", "channel");
			abort();
		}
		else
		{
			char* cChannel[AUDIO_CHANNEL_ID_MAX];

			int num = ParseStr(cBuf, cChannel, AUDIO_CHANNEL_ID_MAX);
			for(int j=0; j<num; j++)
			{
				bool bFind = false;
				int i=0;
				for(i=AUDIO_CHANNEL_ID_MIN+1; i<AUDIO_CHANNEL_ID_MAX; i++)
				{
					if( 0 == strcmp(audioChannel[i].name.toUtf8().data(), cChannel[j]) )
					{
						bFind = true;
						break;
					}
				}
				if( bFind )
				{
					audioChannel[i].bRelated = true;
				}
				else
				{
					IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error, %s can not be identified!\n", "main volume", "main_volume", cChannel[j]);
					abort();
				}
			}
		}
	}

	for(int iLevel=0; iLevel<LEVEL_MAX_NUMBER; iLevel++)
	{
		if( 0==iLevel )
		{
			AudioLevelS sLevel;
			sLevel.type = AUDIO_LEVEL_TYPE_ALWAYS_OPEN;
			sLevel.property = AUDIO_LEVEL_PROPERTY_NOTHING;
			if( 0 != read_config((char *)"level", (char *)"open", cBuf, 1024) )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "can not find section[%s] item[%s]\n", "level", "open");
			}
			else
			{
				if( WORK_MODE_SPECIAL == m_WorkMode )
				{
					IBD_PRINTF(DEBUG_INFOMATION, "this item[%s] at section[%s] can not be set in special work mode\n", "open", "level");
					abort();
				}
				
				char* cChannel[AUDIO_CHANNEL_MAX_NUMBER];

				int num = ParseStr(cBuf, cChannel, AUDIO_CHANNEL_MAX_NUMBER);
				for(int i=0; i<num; i++)
				{
					bool bFind = false;
					int j = 0;
					for(j=AUDIO_CHANNEL_ID_MIN+1; j<AUDIO_CHANNEL_ID_MAX; j++)
					{
						if( audioChannel[j].bValid && 0 == strcmp(audioChannel[j].name.toUtf8().data(), cChannel[i]) )
						{
							bFind = true;
							break;
						}
					}

					if( bFind )
					{
						if( AUDIO_CHANNEL_ID_MAIN==j || AUDIO_CHANNEL_ID_MAIN_LEFT==j || AUDIO_CHANNEL_ID_MAIN_RIGHT==j || AUDIO_CHANNEL_ID_RELATION==j )
						{
							IBD_PRINTF(DEBUG_INFOMATION, "audioChannel[%s] is a special channel, it can not been put into here, at section[%s] item[%s]!\n", cChannel[i], "open", "level");
							abort();
						}
						
						if( -1 != audioChannel[j].idLevel )
						{
							IBD_PRINTF(DEBUG_INFOMATION, "audioChannel[%s] have been put into some level, at section[%s] item[%s]!\n", cChannel[i], "open", "level");
							abort();
						}
						audioChannel[j].idLevel = 0;
						sLevel.audioChannelList.append(audioChannel[j]);
					}
					else
					{
						IBD_PRINTF(DEBUG_INFOMATION, "channel[%s] can not be found or is invalid, at section[%s] item[%s]!\n", cChannel[i], "open", "level");
						abort();
					}
				}
			}
			m_AudioLevelList.append(sLevel);
		}
		else
		{
			sprintf(cItemName, "%d", iLevel);
			if( 0 != read_config((char *)"level", cItemName, cBuf, 1024) )
			{
				//IBD_PRINTF(DEBUG_INFOMATION, "can not find section[%s] item[%s]\n", "level", cItemName);
				break;
			}
			else
			{
				AudioLevelS sLevel;
				sLevel.type = AUDIO_LEVEL_TYPE_COMMON;
				char* p1 = strstr(cBuf, "/");
				if( !p1 )
				{
					IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "level", cItemName);
					abort();
				}

				*p1 = 0;
				tools_trimStr(cBuf);
				if( 0 == strcmp(cBuf, "exclusive") )
				{
					sLevel.property = AUDIO_LEVEL_PROPERTY_EXCLUSIVE;
				}
				else if( 0 == strcmp(cBuf, "pause") )
				{
					sLevel.property = AUDIO_LEVEL_PROPERTY_PAUSE;
				}
				else if( 0 == strcmp(cBuf, "interrupt") )
				{
					sLevel.property = AUDIO_LEVEL_PROPERTY_INTERRUPT;
				}
				else if( 0 == strcmp(cBuf, "mixA") )
				{
					sLevel.property = AUDIO_LEVEL_PROPERTY_MIX_A;
				}
				else if( 0 == strcmp(cBuf, "mixB") )
				{
					sLevel.property = AUDIO_LEVEL_PROPERTY_MIX_B;
				}
				else
				{
					IBD_PRINTF(DEBUG_INFOMATION, "%s cann't be identified, at section[%s] item[%s]\n", cBuf, "level", cItemName);
					abort();
				}
				
				p1++;
				char* cChannel[AUDIO_CHANNEL_MAX_NUMBER];

				int num = ParseStr(p1, cChannel, AUDIO_CHANNEL_MAX_NUMBER);

				for(int i=0; i<num; i++)
				{
					bool bFind = false;
					int j = 0;
					for(j=AUDIO_CHANNEL_ID_MIN+1; j<AUDIO_CHANNEL_ID_MAX; j++)
					{
						if( audioChannel[j].bValid && 0 == strcmp(audioChannel[j].name.toUtf8().data(), cChannel[i]) )
						{
							bFind = true;
							break;
						}
					}
					if( bFind )
					{
						if( AUDIO_CHANNEL_ID_MAIN==j || AUDIO_CHANNEL_ID_MAIN_LEFT==j || AUDIO_CHANNEL_ID_MAIN_RIGHT==j || AUDIO_CHANNEL_ID_RELATION==j )
						{
							IBD_PRINTF(DEBUG_INFOMATION, "audioChannel[%s] is a special channel, it can not been put into here, at section[%s] item[%s]!\n", cChannel[i], "level", cItemName);
							abort();
						}

						if( -1 != audioChannel[j].idLevel )
						{
							IBD_PRINTF(DEBUG_INFOMATION, "audioChannel[%s] have been put into some level, at section[%s] item[%s]!\n", cChannel[i], "level", cItemName);
							abort();
						}
						audioChannel[j].idLevel = iLevel;
						sLevel.audioChannelList.append(audioChannel[j]);
					}
					else
					{
						IBD_PRINTF(DEBUG_INFOMATION, "channel[%s] can not be found or is invalid, at section[%s] item[%s]!\n", cChannel[i], "level", cItemName);
						abort();
					}
				}
				m_AudioLevelList.append(sLevel);
			}
		}
	}

	for(int i=0; i<AUDIO_CHANNEL_MAX_NUMBER; i++)
		m_pAudioChannel[i] = NULL;

	for(int i=0; i<m_AudioLevelList.count(); i++)
	{
		for(int j=0; j<m_AudioLevelList[i].audioChannelList.count(); j++)
		{
			int id = (int)(m_AudioLevelList[i].audioChannelList[j].id);
			m_pAudioChannel[id] = &(m_AudioLevelList[i].audioChannelList[j]);
		}
	}

	m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN] = new AudioChannelC(audioChannel[AUDIO_CHANNEL_ID_MAIN]);
	m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN_LEFT] = new AudioChannelC(audioChannel[AUDIO_CHANNEL_ID_MAIN_LEFT]);
	m_pAudioChannel[AUDIO_CHANNEL_ID_MAIN_RIGHT] = new AudioChannelC(audioChannel[AUDIO_CHANNEL_ID_MAIN_RIGHT]);
	if( WORK_MODE_SPECIAL == m_WorkMode )
		m_pAudioChannel[AUDIO_CHANNEL_ID_RELATION] = new AudioChannelC(audioChannel[AUDIO_CHANNEL_ID_RELATION]);
		
	if( 0 != read_config((char *)"mix", (char *)"main_mix", &iValue) )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "mix", "main_mix");
		abort();
	}
	else
	{
		if( iValue < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error, value can not < 0\n", "mix", "main_mix");
			abort();
		}
		m_iMixMainUpValue = iValue;
	}

	if( 0 != read_config((char *)"mix",(char *)"other_mix", &iValue) )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "mix", "other_mix");
		abort();
	}
	else
	{
		if( iValue < 0 || iValue > VOL_MAX_VALUE )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error, value can not < 0 and > %d\n", "mix", "other_mix", VOL_MAX_VALUE);
			abort();
		}
		m_iMixOtherDownValue = -iValue;
	}

	if( 0 != read_config((char *)"vol_speed", (char *)"speed_start", &iValue) )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "vol_speed", "speed_start");
		abort();
	}
	else
	{
		if( iValue < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error, value can not < 0\n", "vol_speed", "speed_start");
			abort();
		}
		m_iSpeedStartForVolSpeed = iValue;
	}

	if( 0 != read_config((char *)"vol_speed", (char *)"vol_end", &iValue) )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "vol_speed", "vol_end");
		abort();
	}
	else
	{
		if( iValue < 0 || iValue > 255 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error, value can be between 0 and 255\n", "vol_speed", "vol_end");
			abort();
		}
		m_iVolEndForVolSpeed = iValue;
	}

	if( 0 != read_config((char *)"vol_speed", (char *)"vol_to_speed", cBuf, 1024) )
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can not find section[%s] item[%s]\n", "vol_speed", "vol_to_speed");
		abort();
	}
	else
	{
		AudioLevelS sLevel;
		sLevel.type = AUDIO_LEVEL_TYPE_COMMON;
		char* p1 = strstr(cBuf, "/");
		if( !p1 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "vol_speed", "vol_to_speed");
			abort();
		}

		*p1 = 0;
		tools_trimStr(cBuf);
		if( 0 == strlen(cBuf) )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "vol_speed", "vol_to_speed");
			abort();
		}

		if( CheckStrNum(cBuf) )
		{
			int iTmp = atoi(cBuf);
			if( iTmp < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "vol_speed", "vol_to_speed");
				abort();
			}
			else
			{
				m_iVolIncrementForVolSpeed = iTmp;
			}
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "vol_speed", "vol_to_speed");
			abort();
		}

		p1++;
		tools_trimStr(p1);
		if( 0 == strlen(p1) )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "vol_speed", "vol_to_speed");
			abort();
		}

		if( CheckStrNum(p1) )
		{
			int iTmp = atoi(p1);
			if( iTmp < 0 )
			{
				IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "vol_speed", "vol_to_speed");
				abort();
			}
			else
			{
				m_iSpeedIntervalForVolSpeed = iTmp;
			}
		}
		else
		{
			IBD_PRINTF(DEBUG_INFOMATION, "read section[%s] item[%s] error\n", "vol_speed", "vol_to_speed");
			abort();
		}
	}




	read_config_unload();



	
	IBD_LOOP_BEGIN(DEBUG_INFOMATION, "\n/////////////////////////////////////////////////\n")
	IBD_LOOP_PRINTF(DEBUG_INFOMATION, "/////////audio control load config success///////")
	IBD_LOOP_PRINTF(DEBUG_INFOMATION, "\n/////////////////////////////////////////////////\n")
	IBD_LOOP_END

#if 0
	IBD_LOOP_BEGIN(DEBUG_INFOMATION, "\n")
	for(int i=0; i<AUDIO_CHANNEL_MAX_NUMBER; i++)
	{
		if( m_pAudioChannel[i] )
		{
			if( m_pAudioChannel[i]->bValid )
			{
				IBD_LOOP_PRINTF(DEBUG_INFOMATION, "channel id[%d] name[%s] level[%d] virtual vol[%d]\n", m_pAudioChannel[i]->id, 
				m_pAudioChannel[i]->name.toUtf8().data(), m_pAudioChannel[i]->idLevel,
				m_pAudioChannel[i]->iVirtualVol)
			}
			else
			{
				IBD_LOOP_PRINTF(DEBUG_INFOMATION, "it is impossible to come here\n");
			}
		}
	}

	IBD_LOOP_PRINTF(DEBUG_INFOMATION, "\n")
	IBD_LOOP_END
#endif
}

int AudioControlC::_loadDataFromDB()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pSqlSel[200];
	char chSqlDBPath[128] = {0};
	sprintf(chSqlDBPath, "%s", INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY);


	ret = sqlite3_open(chSqlDBPath, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_open error : %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_busy_timeout(pDB, 1000);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}



	/////////////////////////////////////////////
	//read content from table about audio channel
	/////////////////////////////////////////////
	strcpy(pSqlSel , "select * from AudioChannelSetting");

	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt,( const char**)&pError );
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	while(1)
	{
		ret = sqlite3_step(pStmt);
		if( ret != SQLITE_ROW )
		{
			//It is completed to read table
			break;
		}
		else
		{
			int iChannelId = (int)sqlite3_column_int(pStmt, 0);
			int iVirtualVol = (int)sqlite3_column_int(pStmt, 1);
			bool bMute = (bool)sqlite3_column_int(pStmt, 2);
			for(int i=0; i<AUDIO_CHANNEL_MAX_NUMBER; i++)
			{
				if( i == iChannelId && NULL != m_pAudioChannel[i] )
				{
					m_pAudioChannel[i]->iVirtualVol = iVirtualVol;
					m_pAudioChannel[i]->bMute = bMute;
				}
			}
		}
	}
	
	sqlite3_finalize(pStmt);

	/////////////////////////////////////////////
	//read content from table about audioControl setting
	/////////////////////////////////////////////
	strcpy(pSqlSel , "select * from AudioControlSetting");

	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt,( const char**)&pError );
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_step(pStmt);
	if( ret != SQLITE_ROW )
	{
		//Table is empty
	}
	else
	{
		m_bOpenVolSpeed = (bool)sqlite3_column_int(pStmt, 0);
	}
	
	sqlite3_finalize(pStmt);
	
	sqlite3_close(pDB);

	printf("\n//////////////////////////////////////////\n");
	printf("////////audio control load data(DB) success//////");
	printf("\n//////////////////////////////////////////\n");

	IBD_PRINTF(DEBUG_INFOMATION, "audio control load data(DB) success\n");
	
	return 0;
}

int AudioControlC::_saveDataToDB()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pDeleteAllInfoSel[128] = {0};
	char pInsertTablesSel[1024] = {0};
	
	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s\n", sqlite3_errmsg(pDB));
		return -1;
	}	

	ret = sqlite3_busy_timeout(pDB, 1000);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}


	/////////////////////////////////////////////	
	//delete content of table about audio channel
	/////////////////////////////////////////////	
	strcpy(pDeleteAllInfoSel , "delete from AudioChannelSetting");

	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_step error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	sqlite3_finalize(pStmt);


	/////////////////////////////////////////////	
	//insert content to table about audio channel
	/////////////////////////////////////////////	
	for(int i=0; i<AUDIO_CHANNEL_MAX_NUMBER; i++)
	{
		if( NULL != m_pAudioChannel[i] )
		{
			sprintf(pInsertTablesSel, "insert into AudioChannelSetting values('%d', '%d', '%d')", \
					i, m_pAudioChannel[i]->iVirtualVol, m_pAudioChannel[i]->bMute);

			ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
			if (ret != SQLITE_OK)
			{
		                IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error: %s\n", sqlite3_errmsg(pDB));
				sqlite3_close(pDB);
				return -1;
			}

			ret = sqlite3_step(pStmt);
			if (ret != SQLITE_DONE)
			{
				IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_step error: %s\n", sqlite3_errmsg(pDB));
				sqlite3_close(pDB);
				return -1;
			}

			sqlite3_finalize(pStmt);
		}
	}

	/////////////////////////////////////////////	
	//delete content of table about audioControl setting
	/////////////////////////////////////////////	
	strcpy(pDeleteAllInfoSel , "delete from AudioControlSetting");

	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_step error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	sqlite3_finalize(pStmt);


	/////////////////////////////////////////////	
	//insert content to table about audioControl setting
	/////////////////////////////////////////////	
	sprintf(pInsertTablesSel, "insert into AudioControlSetting values('%d')", \
			m_bOpenVolSpeed);

	ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_step(pStmt);
	if (ret != SQLITE_DONE)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_step error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	sqlite3_finalize(pStmt);

	sqlite3_close(pDB);

	printf("\n//////////////////////////////////////////\n");
	printf("////////audio control save data(DB) success//////");
	printf("\n//////////////////////////////////////////\n");

	IBD_PRINTF(DEBUG_INFOMATION, "audio control save data(DB) success\n");
	
	return 0;
}

int AudioControlC::resumeDataToDefault()
{
	int retval = AC_E_OK;

	for(int i=0; i<AUDIO_CHANNEL_MAX_NUMBER; i++)
	{
		if( NULL != m_pAudioChannel[i] )
		{
			m_pAudioChannel[i]->iVirtualVol = m_pAudioChannel[i]->iVirtualVolDefault;
			m_pAudioChannel[i]->bMute = false;
		}
	}

	if( m_iTimerSaveData >= 0 )
	{
		killTimer(m_iTimerSaveData);
		m_iTimerSaveData = -1;
	}
	m_iTimerSaveData = startTimer(m_iTimerSaveDataDelay);

	return retval;		
}

int AudioControlC::_openForCommonLogic(AudioChannelIdE audioChannelId)
{
	int retval = AC_E_OK;
	int index = 0;
	int idLevel = m_pAudioChannel[audioChannelId]->idLevel;
	bool bNeedFreshData = false;

	if( m_iTimerFreshData >= 0 )
	{
		killTimer(m_iTimerFreshData);
		m_iTimerFreshData = -1;
		bNeedFreshData = true;
		//只刷新m_iCurrentOpenLevel，不打开声道
		_freshDataStatus(true);
	}

	if( 0 == idLevel )
	{
		//如果声道在常开级，直接打开该声道
		_openAudioChannel(audioChannelId);
		return AC_E_OK;
	}
	
	if( m_iCurrentOpenLevel >= 0 )
	{
		if( idLevel < m_iCurrentOpenLevel )
		{
			//如果声道所在级小于当前打开的级，暂停当前打开的级
			_pauseLevel(m_iCurrentOpenLevel);
			_openAudioChannel(audioChannelId);
			m_iCurrentOpenLevel = idLevel;
			bNeedFreshData = false;
		}
		else if( idLevel > m_iCurrentOpenLevel )
		{
			//如果声道所在级大于当前打开的级，暂停当前声道所在级
			_pauseAudioChannel(audioChannelId);
		}
		else
		{
			//如果当前声道所在级与当前打开的级相同
			switch( m_AudioLevelList[idLevel].property )
			{
				case AUDIO_LEVEL_PROPERTY_EXCLUSIVE:
					//当前级是互斥级，拒绝打开该声道
					retval = AC_E_REQUEST_REFUSED;
					break;
					
				case AUDIO_LEVEL_PROPERTY_PAUSE:
					//当前级是暂停级，暂停当前级，将该声道移到级顶，并打开该声道
					_pauseLevel(idLevel);
					index = _getIndexInLevel(audioChannelId);
					m_AudioLevelList[idLevel].audioChannelList.move(index, 0);
					_openAudioChannel(audioChannelId);
					break;
					
				case AUDIO_LEVEL_PROPERTY_INTERRUPT:
					//当前级是中断级，关闭当前级，打开该声道
					_closeLevel(idLevel);
					_openAudioChannel(audioChannelId);
					break;

				case AUDIO_LEVEL_PROPERTY_MIX_A:
					//当前级是混音A级
					if( AUDIO_CHANNEL_OPEN != m_pAudioChannel[audioChannelId]->status )
					{
						//改变声道状态，方便_adjustVolForMixA进行调节
						m_pAudioChannel[audioChannelId]->status = AUDIO_CHANNEL_OPEN;

						//告诉_adjustVolForMixA该声道需要设置到audio device
						m_pAudioChannel[audioChannelId]->bChanged = true;
						
						_adjustVolForMixA(idLevel);
						
						_notifyAudioChannelStatusChanged(audioChannelId);
					}
					break;

				case AUDIO_LEVEL_PROPERTY_MIX_B:
					//当前级是混音B级，直接打开声道即可
					_openAudioChannel(audioChannelId);
					break;
					
				default:
					IBD_PRINTF(DEBUG_INFOMATION, "It's impossible to come here\n");
					break;
			}
		}
	}
	else
	{
		//没有打开的级，那就很简单了，直接打开该声道即可
		_openAudioChannel(audioChannelId);
		m_iCurrentOpenLevel = idLevel;
		if( AUDIO_LEVEL_PROPERTY_PAUSE == m_AudioLevelList[idLevel].property )
		{
			index = _getIndexInLevel(audioChannelId);
			m_AudioLevelList[idLevel].audioChannelList.move(index, 0);
		}
	}

	if( bNeedFreshData )
	{
		//有刷新动作，说明前面有关闭声道的操作，关闭后，还没有刷新所有声道
		//这是又有打开声道的动作，那么我们在打开声道后，延时1秒再进行刷新
		m_iTimerFreshData = startTimer(m_iTimerFreshDataDelay);
	}
	
	return retval;
}

int AudioControlC::_closeForCommonLogic(AudioChannelIdE audioChannelId)
{
	int retval = AC_E_OK;
	int index = 0;
	int count = 0;
	int idLevel = m_pAudioChannel[audioChannelId]->idLevel;
	bool bNeedFreshData = false;

	if( m_iTimerFreshData >= 0 )
	{
		killTimer(m_iTimerFreshData);
		m_iTimerFreshData = -1;
		bNeedFreshData = true;
	}

	if( 0 == idLevel )
	{
		//如果声道在常开级，直接关闭该声道
		_closeAudioChannel(audioChannelId);
		return AC_E_OK;
	}
	
	if( m_iCurrentOpenLevel >= 0 )
	{
		if( idLevel < m_iCurrentOpenLevel )
		{
			//要关闭的声道所在的级小于当前打开级，这根本不可能发生
			IBD_PRINTF(DEBUG_INFOMATION, "It's impossible to come here\n");
			retval = AC_E_REQUEST_REFUSED;
		}
		else if( idLevel > m_iCurrentOpenLevel )
		{
			//要关闭的声道所在的级大于当前打开级，直接关闭即可
			_closeAudioChannel(audioChannelId);
		}
		else
		{
			//要关闭的声道所在的级等于当前打开级
			switch( m_AudioLevelList[idLevel].property )
			{
				case AUDIO_LEVEL_PROPERTY_EXCLUSIVE:
					//当前级是互斥级，直接关闭即可
					_closeAudioChannel(audioChannelId);
					break;
					
				case AUDIO_LEVEL_PROPERTY_PAUSE:
					//当前级是暂停级，关闭该声道，将该声道移到级尾
					index = _getIndexInLevel(audioChannelId);
					count = m_AudioLevelList[idLevel].audioChannelList.count();
					_closeAudioChannel(audioChannelId);
					m_AudioLevelList[idLevel].audioChannelList.move(index, count-1);
					break;
					
				case AUDIO_LEVEL_PROPERTY_INTERRUPT:
					//当前级是中断级，直接关闭即可
					_closeAudioChannel(audioChannelId);
					break;

				case AUDIO_LEVEL_PROPERTY_MIX_A:
					//当前级是混音A级
					if( AUDIO_CHANNEL_OPEN == m_pAudioChannel[audioChannelId]->status )
					{
						//改变声道状态，方便_adjustVolForMixA进行调节
						m_pAudioChannel[audioChannelId]->status = AUDIO_CHANNEL_CLOSE;

						//告诉_adjustVolForMixA该声道需要设置到audio device
						m_pAudioChannel[audioChannelId]->bChanged = true;
						
						_adjustVolForMixA(idLevel);
						
						_notifyAudioChannelStatusChanged(audioChannelId);
					}
					else if( AUDIO_CHANNEL_PAUSE == m_pAudioChannel[audioChannelId]->status )
					{
						m_pAudioChannel[audioChannelId]->status = AUDIO_CHANNEL_CLOSE;
						
						_notifyAudioChannelStatusChanged(audioChannelId);
					}
					else
					{

					}
		
					break;

				case AUDIO_LEVEL_PROPERTY_MIX_B:
					//当前级是混音B级，直接关闭即可
					_closeAudioChannel(audioChannelId);
					break;
					
				default:
					IBD_PRINTF(DEBUG_INFOMATION, "It's impossible to come here\n");
					break;
			}

			//关闭当前级的声道，需要刷新所有声道数据
			bNeedFreshData = true;
		}
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "It's impossible to come here\n");
	}

	if( bNeedFreshData )
	{
		//只刷新m_iCurrentOpenLevel，不打开声道
		_freshDataStatus(true);
		
		//关闭声道后，需要刷新所有声道数据，打开某些暂停的声道
		//刷新动作要在1秒钟后进行，这样避免了快速关闭、打开声道造成的漏音现象
		m_iTimerFreshData = startTimer(m_iTimerFreshDataDelay);
	}
	
	return retval;
}

int AudioControlC::_calculateActualVol(AudioChannelIdE audioChannelId)
{
	int vol = 0;
	bool	bAdjustVolForVolSpeed = false;

	IBD_PRINTF(DEBUG_INFOMATION, "_calculateActualVol 0 audioChannelId[%d] virtualVol[%d]\n", audioChannelId, m_pAudioChannel[audioChannelId]->iVirtualVol);

	//首先根据声音随速功能，计算音量vol
	if( m_bOpenVolSpeed )
	{
		if( WORK_MODE_SPECIAL == m_WorkMode )
		{
			bAdjustVolForVolSpeed = true;
		}
		else
		{
			if( AUDIO_CHANNEL_ID_MAIN == audioChannelId )
			{
				bAdjustVolForVolSpeed = true;
			}
		}
	}

	if( bAdjustVolForVolSpeed )
	{
		vol = m_pAudioChannel[audioChannelId]->iVirtualVol;
		int iSpeed = m_iCurrentSpeedForVolSpeed - m_iSpeedStartForVolSpeed;
		if( iSpeed > 0 )
		{
			int iInterval = iSpeed/m_iSpeedIntervalForVolSpeed;
			int iVol = iInterval*m_iVolIncrementForVolSpeed;
			if( vol < m_iVolEndForVolSpeed )
			{
				if( iVol > (m_iVolEndForVolSpeed-vol) )
				{
					vol =  m_iVolEndForVolSpeed;
				}
				else
				{
					vol += iVol;
				}
			}
		}
	}
	else
	{
		vol = m_pAudioChannel[audioChannelId]->iVirtualVol;
	}

	IBD_PRINTF(DEBUG_INFOMATION, "_calculateActualVol 1 vol[%d]\n", vol);

	//根据iVolAdjustRange 调节vol
	if( m_pAudioChannel[audioChannelId]->iVolAdjustRange != 0 )
	{
		vol = vol*(100+m_pAudioChannel[audioChannelId]->iVolAdjustRange)/100;
		if( vol > VOL_MAX_VALUE )
			vol = VOL_MAX_VALUE;
	}

	IBD_PRINTF(DEBUG_INFOMATION, "_calculateActualVol 2 vol[%d]\n", vol);
	
	if( VOL_CORRESPONDENCE_LINER == m_VolCorrespondence )
	{
		m_pAudioChannel[audioChannelId]->iActualVol = (vol*m_pAudioChannel[audioChannelId]->iActualVolMax)/VOL_MAX_VALUE;
		IBD_PRINTF(DEBUG_INFOMATION, "_calculateActualVol 3 iActualVol[%d]\n", m_pAudioChannel[audioChannelId]->iActualVol);
	}
	else if( VOL_CORRESPONDENCE_LOG == m_VolCorrespondence )
	{
		m_pAudioChannel[audioChannelId]->iActualVol = (m_pAudioChannel[audioChannelId]->iActualVolMax) * (log(vol+1)) / (log(VOL_MAX_VALUE+1));
		IBD_PRINTF(DEBUG_INFOMATION, "_calculateActualVol 4 iActualVol[%d]\n", m_pAudioChannel[audioChannelId]->iActualVol);
	}
	else if( VOL_CORRESPONDENCE_TABLE == m_VolCorrespondence )
	{
		vol = m_iVolTable[vol];
		m_pAudioChannel[audioChannelId]->iActualVol = (vol*m_pAudioChannel[audioChannelId]->iActualVolMax)/VOL_MAX_VALUE;
		IBD_PRINTF(DEBUG_INFOMATION, "_calculateActualVol 5 iActualVol[%d] voltable[%d] iActualVolMax[%d]\n", m_pAudioChannel[audioChannelId]->iActualVol, m_iVolTable[vol], m_pAudioChannel[audioChannelId]->iActualVolMax);
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "It's impossible to come here\n");
	}
	return 0;
}

int AudioControlC::_adjustVolForCommonLogic(AudioChannelIdE audioChannelId, int vol)
{
	int idLevel = m_pAudioChannel[audioChannelId]->idLevel;

	if( AUDIO_CHANNEL_ID_MAIN_LEFT == audioChannelId ||
		AUDIO_CHANNEL_ID_MAIN_RIGHT == audioChannelId )
	{
		m_pAudioChannel[audioChannelId]->iVirtualVol = vol;
		AudioDeviceC::object()->setAudioChanVolume(audioChannelId, vol);
		return AC_E_OK;
	}

	if( m_pAudioChannel[audioChannelId]->iVirtualVol == vol )
	{
		return AC_E_DONE;
	}
	
	m_pAudioChannel[audioChannelId]->iVirtualVol = vol;

	//如果该声道所在级的属性是混音A，按照其特殊逻辑来处理
	if( idLevel > 0 && AUDIO_LEVEL_PROPERTY_MIX_A == m_AudioLevelList[idLevel].property )
	{
		//告诉_adjustVolForMixA该声道需要设置到audio device
		m_pAudioChannel[audioChannelId]->bChanged = true;

		_adjustVolForMixA(m_pAudioChannel[audioChannelId]->idLevel);	
	}
	else
	{
		//将该声道的实际音量设置到AudioDevice
		_updateAudioChannelDataToDevice(audioChannelId);
	}
	
	return AC_E_OK;
}

int AudioControlC::_muteForCommonLogic(AudioChannelIdE audioChannelId)
{
	int idLevel = m_pAudioChannel[audioChannelId]->idLevel;

	m_pAudioChannel[audioChannelId]->bMute = true;

	//如果当前级是混音A级，调用_adjustVolForMixA来处理音量
	if( idLevel > 0 && AUDIO_LEVEL_PROPERTY_MIX_A == m_AudioLevelList[idLevel].property )
	{
		//告诉_adjustVolForMixA该声道需要设置到audio device
		m_pAudioChannel[audioChannelId]->bChanged = true;
		
		_adjustVolForMixA(idLevel);
	}
	else
	{
		_updateAudioChannelDataToDevice(audioChannelId);
	}
	
	return AC_E_OK;
}

int AudioControlC::_unMuteForCommonLogic(AudioChannelIdE audioChannelId)
{
	int idLevel = m_pAudioChannel[audioChannelId]->idLevel;

	m_pAudioChannel[audioChannelId]->bMute = false;

	//如果当前级是混音A级，调用_adjustVolForMixA来处理音量
	if( idLevel > 0 && AUDIO_LEVEL_PROPERTY_MIX_A == m_AudioLevelList[idLevel].property )
	{
		//告诉_adjustVolForMixA该声道需要设置到audio device
		m_pAudioChannel[audioChannelId]->bChanged = true;
		
		_adjustVolForMixA(idLevel);
	}
	else
	{
		_updateAudioChannelDataToDevice(audioChannelId);
	}

	return AC_E_OK;
}

int AudioControlC::_setSpeedForCommonLogic(int carSpeed)
{
	m_iCurrentSpeedForVolSpeed = carSpeed;
	
	return AC_E_OK;
}

int AudioControlC::_openVolSpeedForCommonLogic()
{
	int retval = AC_E_OK;
	
	if( m_bOpenVolSpeed )
	{
		retval = AC_E_DONE;
	}
	else
	{
		m_bOpenVolSpeed = true;
		_updateAudioChannelDataToDevice(AUDIO_CHANNEL_ID_MAIN);
	}
	
	return retval;
}

int AudioControlC::_closeVolSpeedForCommonLogic()
{
	int retval = AC_E_OK;
	
	if( !m_bOpenVolSpeed )
	{
		retval = AC_E_DONE;
	}
	else
	{
		m_bOpenVolSpeed = false;
		_updateAudioChannelDataToDevice(AUDIO_CHANNEL_ID_MAIN);
	}
	
	return retval;
}

int AudioControlC::_openForSpecialLogic(AudioChannelIdE audioChannelId, bool& bDone)
{
	bDone = false;
	return AC_E_OK;
}

int AudioControlC::_closeForSpecialLogic(AudioChannelIdE audioChannelId, bool& bDone)
{
	bDone = false;
	return AC_E_OK;
}

int AudioControlC::_adjustVolForSpecialLogic(AudioChannelIdE audioChannelId, int vol, bool& bDone)
{
	bDone = false;
	return AC_E_OK;
}

int AudioControlC::_muteForSpecialLogic(AudioChannelIdE audioChannelId, bool& bDone)
{
	bDone = false;
	return AC_E_OK;
}

int AudioControlC::_unMuteForSpecialLogic(AudioChannelIdE audioChannelId, bool& bDone)
{
	bDone = false;
	return AC_E_OK;
}

int AudioControlC::_setSpeedForSpecialLogic(int carSpeed, bool& bDone)
{
	bDone = false;
	return AC_E_OK;
}

int AudioControlC::_openVolSpeedForSpecialLogic(bool& bDone)
{
	bDone = false;
	return AC_E_OK;
}

int AudioControlC::_closeVolSpeedForSpecialLogic(bool& bDone)
{
	bDone = false;
	return AC_E_OK;
}

int AudioControlC::_checkAudioChannelId(AudioChannelIdE audioChannelId, Audio_Control_Command_Id idCommand)
{
	if( audioChannelId <= 0 || audioChannelId >= AUDIO_CHANNEL_ID_MAX || NULL == m_pAudioChannel[audioChannelId] )
		return AC_E_CHANNEL_NOT_SUPPORTED;

	if( WORK_MODE_SPECIAL != m_WorkMode && AUDIO_CHANNEL_ID_RELATION==audioChannelId )
		return AC_E_CHANNEL_NOT_SUPPORTED;

	switch( idCommand )
	{
		case ACC_OPEN:
			if( AUDIO_CHANNEL_ID_MAIN==audioChannelId ||
				AUDIO_CHANNEL_ID_MAIN_LEFT==audioChannelId ||
				AUDIO_CHANNEL_ID_MAIN_RIGHT==audioChannelId ||
				AUDIO_CHANNEL_ID_RELATION==audioChannelId )
				return AC_E_COMMAND_NOT_SUPPORTED;
			
			if( AUDIO_CHANNEL_OPEN == m_pAudioChannel[audioChannelId]->status )
				return AC_E_DONE;
			break;
			
		case ACC_CLOSE:
			if( AUDIO_CHANNEL_ID_MAIN==audioChannelId ||
				AUDIO_CHANNEL_ID_MAIN_LEFT==audioChannelId ||
				AUDIO_CHANNEL_ID_MAIN_RIGHT==audioChannelId ||
				AUDIO_CHANNEL_ID_RELATION==audioChannelId )
				return AC_E_COMMAND_NOT_SUPPORTED;

			if( AUDIO_CHANNEL_CLOSE == m_pAudioChannel[audioChannelId]->status )
				return AC_E_DONE;
			break;

		case ACC_ADJUST_VOL:
			if( WORK_MODE_SPECIAL == m_WorkMode && AUDIO_CHANNEL_ID_MAIN==audioChannelId )
				return AC_E_COMMAND_NOT_SUPPORTED;
			break;

		case ACC_MUTE:
			if( m_pAudioChannel[audioChannelId]->bMute )
				return AC_E_DONE;
			break;

		case ACC_UNMUTE:
			if( !(m_pAudioChannel[audioChannelId]->bMute) )
				return AC_E_DONE;
			break;

		default:
			break;
	}

	return AC_E_OK;
}

void AudioControlC::_pauseLevel(int idLevel)
{
	for(int i=0; i<m_AudioLevelList[idLevel].audioChannelList.count(); i++)
	{
		_pauseAudioChannel(m_AudioLevelList[idLevel].audioChannelList[i].id);
	}
}

void AudioControlC::_closeLevel(int idLevel)
{
	for(int i=0; i<m_AudioLevelList[idLevel].audioChannelList.count(); i++)
	{
		_closeAudioChannel(m_AudioLevelList[idLevel].audioChannelList[i].id);
	}
}

void AudioControlC::_openAudioChannel(AudioChannelIdE audioChannelId)
{
	if( AUDIO_CHANNEL_OPEN != m_pAudioChannel[audioChannelId]->status )
	{
		m_pAudioChannel[audioChannelId]->status = AUDIO_CHANNEL_OPEN;

		_updateAudioChannelDataToDevice(audioChannelId);

		_notifyAudioChannelStatusChanged(audioChannelId);
	}
}

void AudioControlC::_pauseAudioChannel(AudioChannelIdE audioChannelId)
{
	if( AUDIO_CHANNEL_OPEN == m_pAudioChannel[audioChannelId]->status )
	{
		m_pAudioChannel[audioChannelId]->status = AUDIO_CHANNEL_PAUSE;

		_updateAudioChannelDataToDevice(audioChannelId);

		_notifyAudioChannelStatusChanged(audioChannelId);
	}
}

void AudioControlC::_closeAudioChannel(AudioChannelIdE audioChannelId)
{
	if( AUDIO_CHANNEL_OPEN == m_pAudioChannel[audioChannelId]->status )
	{
		m_pAudioChannel[audioChannelId]->status = AUDIO_CHANNEL_CLOSE;
		
		_updateAudioChannelDataToDevice(audioChannelId);
		
		_notifyAudioChannelStatusChanged(audioChannelId);
	}
	else if( AUDIO_CHANNEL_PAUSE == m_pAudioChannel[audioChannelId]->status )
	{
		m_pAudioChannel[audioChannelId]->status = AUDIO_CHANNEL_CLOSE;
		
		_notifyAudioChannelStatusChanged(audioChannelId);
	}
	else
	{

	}
}

bool AudioControlC::_isAudioChannelOpen(AudioChannelIdE audioChannelId)
{
	if( AUDIO_CHANNEL_OPEN == m_pAudioChannel[audioChannelId]->status && !(m_pAudioChannel[audioChannelId]->bMute)  && m_pAudioChannel[audioChannelId]->iVirtualVol>0 )
		return true;
	else
		return false;
}

//根据某个声道的状态和实际音量，设置AudioDevice
//AudioDeviceC只实现了一个设置音量的函数，该函数可以满足所有对声道的控制
void AudioControlC::_updateAudioChannelDataToDevice(AudioChannelIdE audioChannelId)
{
	int iActualVol;

	_calculateActualVol(audioChannelId);

	if( !_isAudioChannelOpen(audioChannelId) )
	{
		if( WORK_MODE_SPECIAL == m_WorkMode )
		{
			AudioDeviceC::object()->setAudioChanVolume(AUDIO_CHANNEL_ID_MAIN, 0);
		}
		else
		{
			AudioDeviceC::object()->setAudioChanVolume(audioChannelId, 0);
		}
		return;
	}
		
	if( WORK_MODE_SPECIAL == m_WorkMode )
	{
		if( AUDIO_CHANNEL_ID_MAIN == audioChannelId )
		{
			for(int i=AUDIO_CHANNEL_ID_RELATION+1; i<AUDIO_CHANNEL_ID_MAX; i++)
			{
				if( m_pAudioChannel[i]->bRelated && _isAudioChannelOpen((AudioChannelIdE)i) && _isAudioChannelOpen(AUDIO_CHANNEL_ID_RELATION) )
				{
					iActualVol = m_pAudioChannel[i]->iActualVol*m_pAudioChannel[AUDIO_CHANNEL_ID_RELATION]->iVirtualVol/VOL_MAX_VALUE;
					if( _isAudioChannelOpen(AUDIO_CHANNEL_ID_MAIN) )
					{
						AudioDeviceC::object()->setAudioChanVolume(AUDIO_CHANNEL_ID_MAIN, iActualVol);
					}
					break;
				}
				else if( !(m_pAudioChannel[i]->bRelated) && _isAudioChannelOpen((AudioChannelIdE)i) )
				{
					iActualVol = m_pAudioChannel[i]->iActualVol;
					if( _isAudioChannelOpen(AUDIO_CHANNEL_ID_MAIN) )
					{
						AudioDeviceC::object()->setAudioChanVolume(AUDIO_CHANNEL_ID_MAIN, iActualVol);
					}
					break;
				}
				else
				{

				}
			}			
		}
		else if( AUDIO_CHANNEL_ID_RELATION == audioChannelId )
		{
			for(int i=AUDIO_CHANNEL_ID_RELATION+1; i<AUDIO_CHANNEL_ID_MAX; i++)
			{
				if( m_pAudioChannel[i]->bRelated && _isAudioChannelOpen((AudioChannelIdE)i) )
				{
					iActualVol = m_pAudioChannel[i]->iActualVol*m_pAudioChannel[AUDIO_CHANNEL_ID_RELATION]->iVirtualVol/VOL_MAX_VALUE;
					if( _isAudioChannelOpen(AUDIO_CHANNEL_ID_MAIN) )
					{
						AudioDeviceC::object()->setAudioChanVolume(AUDIO_CHANNEL_ID_MAIN, iActualVol);
					}
					break;
				}
			}
		}
		else
		{
			if( m_pAudioChannel[audioChannelId]->bRelated && _isAudioChannelOpen(AUDIO_CHANNEL_ID_RELATION) )
			{
				iActualVol = m_pAudioChannel[audioChannelId]->iActualVol*m_pAudioChannel[AUDIO_CHANNEL_ID_RELATION]->iVirtualVol/VOL_MAX_VALUE;
			}
			else
			{
				iActualVol = m_pAudioChannel[audioChannelId]->iActualVol;
			}

			if( _isAudioChannelOpen(AUDIO_CHANNEL_ID_MAIN) )
			{
				AudioDeviceC::object()->setAudioChanVolume(AUDIO_CHANNEL_ID_MAIN, iActualVol);
			}
		}
	}
	else
	{
		if( AUDIO_CHANNEL_ID_MAIN_LEFT == audioChannelId || AUDIO_CHANNEL_ID_MAIN_RIGHT == audioChannelId )
			AudioDeviceC::object()->setAudioChanVolume(audioChannelId, m_pAudioChannel[audioChannelId]->iVirtualVol);
		else
			AudioDeviceC::object()->setAudioChanVolume(audioChannelId, m_pAudioChannel[audioChannelId]->iActualVol);
	}
}

void AudioControlC::_notifyAudioChannelStatusChanged(AudioChannelIdE audioChannelId)
{
	MyDbusC::object()->audioChannelStatusChanged(audioChannelId, m_pAudioChannel[audioChannelId]->status);
}

//调整混音A级中各声道的iVolAdjustRange，并将需要设置的声道设到AudioDevice中
void AudioControlC::_adjustVolForMixA(int idLevel)
{
	int iOpenChannelNum = 0;
	
	if( AUDIO_LEVEL_PROPERTY_MIX_A != m_AudioLevelList[idLevel].property )
		return;

	//统计级中出声的声道数量
	for(int i=0; i<m_AudioLevelList[idLevel].audioChannelList.count(); i++)
	{
		if( _isAudioChannelOpen(m_AudioLevelList[idLevel].audioChannelList[i].id) )
			iOpenChannelNum++;
	}

	//如果出声的声道数量大于1，并且主混音声道出声
	//则设置主混音声道的iVolAdjustRange的值为m_iMixMainUpValue
	//设置其他出声的声道的iVolAdjustRange值为m_iMixOtherDownValue
	if( iOpenChannelNum > 1 && _isAudioChannelOpen(m_AudioLevelList[idLevel].audioChannelList[0].id) )
	{
		if( m_AudioLevelList[idLevel].audioChannelList[0].iVolAdjustRange != m_iMixMainUpValue )
		{
			m_AudioLevelList[idLevel].audioChannelList[0].iVolAdjustRange = m_iMixMainUpValue;
			m_AudioLevelList[idLevel].audioChannelList[0].bChanged = true;
		}
		
		for(int i=1; i<m_AudioLevelList[idLevel].audioChannelList.count(); i++)
		{
			if( _isAudioChannelOpen(m_AudioLevelList[idLevel].audioChannelList[i].id) )
			{
				if( m_AudioLevelList[idLevel].audioChannelList[i].iVolAdjustRange != m_iMixOtherDownValue )
				{
					m_AudioLevelList[idLevel].audioChannelList[i].iVolAdjustRange = m_iMixOtherDownValue;
					m_AudioLevelList[idLevel].audioChannelList[i].bChanged = true;
				}
			}
			else
			{
				if( m_AudioLevelList[idLevel].audioChannelList[i].iVolAdjustRange != 0 )
				{
					m_AudioLevelList[idLevel].audioChannelList[i].iVolAdjustRange = 0;
					m_AudioLevelList[idLevel].audioChannelList[i].bChanged = true;
				}
			}
		}
	}
	else
	{
		//将iVolAdjustRange 清零
		for(int i=0; i<m_AudioLevelList[idLevel].audioChannelList.count(); i++)
		{
			if( m_AudioLevelList[idLevel].audioChannelList[i].iVolAdjustRange != 0 )
			{
				m_AudioLevelList[idLevel].audioChannelList[i].iVolAdjustRange = 0;
				m_AudioLevelList[idLevel].audioChannelList[i].bChanged = true;
			}
		}		
	}

	//将改变的声道设到audio device
	for(int i=0; i<m_AudioLevelList[idLevel].audioChannelList.count(); i++)
	{
		if( m_AudioLevelList[idLevel].audioChannelList[i].bChanged )
		{
			m_AudioLevelList[idLevel].audioChannelList[i].bChanged = false;
			_updateAudioChannelDataToDevice(m_AudioLevelList[idLevel].audioChannelList[i].id);
		}
	}		

}

//关闭m_iCurrentOpenLevel 级的声道后，需要对所有声道进行刷新，将那些需要打开的声道打开
//bOnlyFresh为true，表示只刷新m_iCurrentOpenLevel，不打开声道
void AudioControlC::_freshDataStatus(bool bOnlyFresh)
{
	if( m_iCurrentOpenLevel < 0 )
		return;

	//查询当前级，看当前级是否所有声道已经关闭
	bool bAllChannelCloseAtCurrentLevel = true;
	for(int iChannel=0; iChannel<m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList.count(); iChannel++)
	{
		int status = m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[iChannel].status;
		if( AUDIO_CHANNEL_OPEN == status || AUDIO_CHANNEL_PAUSE == status )
		{
			bAllChannelCloseAtCurrentLevel = false;
			break;
		}
	}

	//如果当前级中所有声道都关闭了，则到下面的级中去要找要打开的声道
	//如果找到，更新当前级变量m_iCurrentOpenLevel
	if( bAllChannelCloseAtCurrentLevel )
	{
		bool bFind = false;
		for(int iLevel=m_iCurrentOpenLevel+1; iLevel<m_AudioLevelList.count(); iLevel++)
		{
			for(int iChannel=0; iChannel<m_AudioLevelList[iLevel].audioChannelList.count(); iChannel++)
			{
				int status = m_AudioLevelList[iLevel].audioChannelList[iChannel].status;
				if( AUDIO_CHANNEL_PAUSE == status )
				{
					m_iCurrentOpenLevel = iLevel;
					bFind = true;
					break;
				}
			}

			if( bFind )
			{
				break;
			}
		}

		if( !bFind )
		{
			m_iCurrentOpenLevel = -1;
		}
	}

	if( bOnlyFresh )
		return;
	
	//当前级为-1，说明没有要打开的声道
	if( m_iCurrentOpenLevel < 0 )
		return;
	
	switch( m_AudioLevelList[m_iCurrentOpenLevel].property )
	{
		case AUDIO_LEVEL_PROPERTY_EXCLUSIVE:
		case AUDIO_LEVEL_PROPERTY_INTERRUPT:
			//如果当前级是互斥级或中断级，则直接打开暂停声道即可
			for(int iChannel=0; iChannel<m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList.count(); iChannel++)
			{
				int status = m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[iChannel].status;
				if( AUDIO_CHANNEL_PAUSE == status )
				{
					_openAudioChannel(m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[iChannel].id);
					break;
				}
			}
			break;
					
		case AUDIO_LEVEL_PROPERTY_PAUSE:
			//如果当前级是暂停级，则打开级顶的声道即可
			{
				int status = m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[0].status;
				if( AUDIO_CHANNEL_PAUSE == status )
				{
					_openAudioChannel(m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[0].id);
				}
				else
				{
					IBD_PRINTF(DEBUG_INFOMATION, "It's impossible to come here\n");
				}
			}
			break;

		case AUDIO_LEVEL_PROPERTY_MIX_A:
			//如果当前级是混音A级，将声道状态由暂停变为打开，再调用
			//_adjustVolForMixA调整级中各声道音量即可
			for(int iChannel=0; iChannel<m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList.count(); iChannel++)
			{
				//改变声道状态，方便_adjustVolForMixA进行调节
				if( AUDIO_CHANNEL_PAUSE == m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[iChannel].status )
				{
					m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[iChannel].status = AUDIO_CHANNEL_OPEN;

					//告诉_adjustVolForMixA该声道需要设置到audio device
					m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[iChannel].bChanged = true;

					_notifyAudioChannelStatusChanged(m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[iChannel].id);
				}
			}

			_adjustVolForMixA(m_iCurrentOpenLevel);
			
			break;

		case AUDIO_LEVEL_PROPERTY_MIX_B:
			//如果当前级是混音B级，则直接将所有暂停声道打开
			for(int iChannel=0; iChannel<m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList.count(); iChannel++)
			{
				int status = m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[iChannel].status;
				if( AUDIO_CHANNEL_PAUSE == status )
				{
					_openAudioChannel(m_AudioLevelList[m_iCurrentOpenLevel].audioChannelList[iChannel].id);
				}
			}
			break;
					
		default:
			IBD_PRINTF(DEBUG_INFOMATION, "It's impossible to come here\n");
			break;
	}	
}

int AudioControlC::_getIndexInLevel(AudioChannelIdE audioChannelId)
{
	int idLevel = m_pAudioChannel[audioChannelId]->idLevel;
	int index = 0;
	
	for(int i=0; i<m_AudioLevelList[idLevel].audioChannelList.count(); i++)
	{
		if( audioChannelId == m_AudioLevelList[idLevel].audioChannelList[i].id )
		{
			break;
		}
		else
		{
			index++;
		}
	}

	return index;
}

void AudioControlC::timerEvent(QTimerEvent * event)
{
	int iTimerId = event->timerId();

	if( m_iTimerFreshData == iTimerId )
	{
		killTimer(m_iTimerFreshData);
		m_iTimerFreshData = -1;

		_freshDataStatus();
	}
	else if( m_iTimerSaveData == iTimerId )
	{
		killTimer(m_iTimerSaveData);
		m_iTimerSaveData = -1;

		_saveDataToDB();
	}
}

int AudioControlC::inquireChannelInfo(AudioChannelC& channelInfo)
{
	AudioChannelIdE audioChannelId = channelInfo.id;
	if( audioChannelId <= 0 || audioChannelId >= AUDIO_CHANNEL_ID_MAX || NULL == m_pAudioChannel[audioChannelId] )
		return AC_E_CHANNEL_NOT_SUPPORTED;

	channelInfo.iVirtualVol = m_pAudioChannel[audioChannelId]->iVirtualVol;
	channelInfo.bMute = m_pAudioChannel[audioChannelId]->bMute;
	channelInfo.status = m_pAudioChannel[audioChannelId]->status;

	return AC_E_OK;
}

int AudioControlC::inquireAllChannelInfo(AudioChannelList& channelInfoList)
{
	channelInfoList.clear();

	for(int i=0; i<AUDIO_CHANNEL_ID_MAX; i++)
	{
		if( NULL != m_pAudioChannel[i] )
		{
			AudioChannelC channelInfo;
			channelInfo.id = m_pAudioChannel[i]->id;
			channelInfo.iVirtualVol = m_pAudioChannel[i]->iVirtualVol;
			channelInfo.bMute = m_pAudioChannel[i]->bMute;
			channelInfo.status = m_pAudioChannel[i]->status;
			channelInfoList.append(channelInfo);
		}
	}

	return AC_E_OK;
}




}

