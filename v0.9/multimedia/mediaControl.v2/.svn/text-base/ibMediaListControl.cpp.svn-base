
#include <sqlite3.h>
#include "common_config.h"
#include "ibMediaControl.h"
#include "ibMediaListControl.h"
#include "trace.h"
#include "FileListUtil.h"

namespace MediaControl2
{

IBMediaListControlC::IBMediaListControlC(Media_Type type)
	: m_MediaType(type),
	  m_state(MEDIA_RUNNING_STATE_STOP),
	  m_iCursor(-1),
	  m_ListPlayMode(LIST_PLAY_MODE_SEQUENCE_LOOPED),
	  m_Property(SETTING_NO_INIT_FLAG),
	  m_iTimeFormat(TIME_FORMAT2_2_2),
  	  m_bFullScreen(false),
	  m_iFunctionState(0),
	  m_bPrePauseFromUser(false),
	  m_AudioEffect(MPLAYER_AUDIO_EFFCET_NULL),
	  m_bUserSetAudioEffect(false),
	  m_iTimerPlay(-1),
	  m_iTimerPlayDelay(666),
	  m_iTimerFreshState(-1),
	  m_iTimerFreshStateDelay(666)
{
	_loadData();
	if( _isSupport(PLAYER_PROPERTY_DELETE_ALL_NOOPEN_FILE) )
	{
		_deleteAllNoOpenFile();
	}
	else
	{
		_freshAllNoOpenFile();
	}
	
	qRegisterMetaType<MediaControl2::Media_Type>("MediaControl2::Media_Type");
	qRegisterMetaType<MediaControl2::Media_Running_State>("MediaControl2::Media_Running_State");
	qRegisterMetaType<MediaControl2::File_Status>("MediaControl2::File_Status");

	connect(IBMediaControlC::object(), SIGNAL(sigMediaTypeChanged(MediaControl2::Media_Type)), this, SLOT(slotMediaTypeChanged(MediaControl2::Media_Type)), Qt::DirectConnection);
	connect(IBMediaControlC::object(), SIGNAL(sigMediaStateChanged(MediaControl2::Media_Running_State)), this, SLOT(slotMediaStateChanged(MediaControl2::Media_Running_State)), Qt::DirectConnection);
	connect(IBMediaControlC::object(), SIGNAL(sigMediaPlayError(MediaControl2::File_Status)), SLOT(slotMediaPlayError(MediaControl2::File_Status)), Qt::DirectConnection);
	connect(IBMediaControlC::object(), SIGNAL(sigCurrentTimeChanged(int, QString, int, QString)), this, SLOT(slotCurrentTimeChanged(int, QString, int, QString)), Qt::DirectConnection);
}

IBMediaListControlC::~IBMediaListControlC()
{
	mediaTypeChangeToNone();
	
	_saveData();

	if( !_isSupport(PLAYER_PROPERTY_SAVE_POSITION) )
	{
		if( !m_slPlayList.isEmpty() )
		{
			m_slPlayList[m_iCursor].iPosition = 0;
		}
	}
}

void IBMediaListControlC::slotMediaTypeChanged(Media_Type type)
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return;

	if( m_MediaType != type )
	{
		if( m_iTimerPlay >= 0 )
		{
			killTimer(m_iTimerPlay);
			m_iTimerPlay = -1;
		}

		if( MEDIA_RUNNING_STATE_PLAYING == m_state ) 
		{
			m_state = MEDIA_RUNNING_STATE_PAUSE;
			m_bPrePauseFromUser = false;
			_freshState();
		}
	}
}

void IBMediaListControlC::slotMediaStateChanged(Media_Running_State state)
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return;

	if( m_MediaType == IBMediaControlC::object()->getMediaType() )
	{
             if( m_state != state )
		{
			m_state = state;
			if( MEDIA_RUNNING_STATE_STOP == m_state || MEDIA_RUNNING_STATE_FINISHED == m_state )
				m_slPlayList[m_iCursor].iPosition = 0;
			_freshState();

			if( MEDIA_RUNNING_STATE_FINISHED == m_state )
			{
				if( LIST_PLAY_MODE_SINGLE == m_ListPlayMode )
				{
					//do nothing
				}
				else if( LIST_PLAY_MODE_SINGLE_LOOPED == m_ListPlayMode )
				{
					m_slPlayList[m_iCursor].iPosition = 0;
					start();
				}
				else if( LIST_PLAY_MODE_SEQUENCE == m_ListPlayMode )
				{
					_playNext(false);
				}
				else if( LIST_PLAY_MODE_SEQUENCE_LOOPED == m_ListPlayMode )
				{
					if( MEDIA_E_OK != _playNext(true) )
					{
						m_slPlayList[m_iCursor].iPosition = 0;
						start();
					}
				}
				else if( LIST_PLAY_MODE_RANDOM == m_ListPlayMode )
				{
					if( MEDIA_E_OK != _playRand() )
					{
						m_slPlayList[m_iCursor].iPosition = 0;
						start();
					}
				}
				else
				{
					//use LIST_PLAY_MODE_SEQUENCE_LOOPED by default
					if( MEDIA_E_OK != _playNext(true) )
					{
						m_slPlayList[m_iCursor].iPosition = 0;
						start();
					}
				}
			}
		}
	}
}

void IBMediaListControlC::slotMediaPlayError(File_Status fileStatus)
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return;
	if( m_MediaType == IBMediaControlC::object()->getMediaType() )
	{
		m_slPlayList[m_iCursor].fileStatus = fileStatus;
		emit sigMediaPlayError(fileStatus);

		if( FILE_STATUS_NOT_OPEN == fileStatus )
		{
			IBMediaControlC::object()->stop();
			if( _isSupport(PLAYER_PROPERTY_DELETE_ALL_NOOPEN_FILE) )
			{
				_deleteAllNoOpenFile();
				if( LIST_PLAY_MODE_SEQUENCE_LOOPED == m_ListPlayMode || LIST_PLAY_MODE_RANDOM == m_ListPlayMode )
				{
					start();
				}
			}
			else
			{
				_freshAllNoOpenFile();
				slotMediaStateChanged(MEDIA_RUNNING_STATE_FINISHED);
			}
		}
		
		if( FILE_STATUS_NOT_PLAY == fileStatus )
		{
			IBMediaControlC::object()->stop();
			if( _isSupport(PLAYER_PROPERTY_DELETE_NOPLAY_FILE) )
			{
				playListDelete(m_slPlayList[m_iCursor].sFileName);
				if( LIST_PLAY_MODE_SEQUENCE_LOOPED == m_ListPlayMode || LIST_PLAY_MODE_RANDOM == m_ListPlayMode )
				{
					start();
				}
			}
			else
			{
				m_slPlayList[m_iCursor].bNormal = false;
				slotMediaStateChanged(MEDIA_RUNNING_STATE_FINISHED);
			}
		}
	}
}

void IBMediaListControlC::slotCurrentTimeChanged(int current,QString s_current,int total,QString s_total)
{
	static int siCursor = -1;
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return;

	if( m_MediaType == IBMediaControlC::object()->getMediaType() )
	{

		if( m_slPlayList[m_iCursor].iPosition != current )
		{
			m_slPlayList[m_iCursor].iPosition = current;
			emit sigMediaPosition(current, s_current);
		}

		if( siCursor != m_iCursor || m_slPlayList[m_iCursor].iTimeLength != total )
		{
			siCursor = m_iCursor;
			
			if( m_slPlayList[m_iCursor].iTimeLength != total )
			{
				m_slPlayList[m_iCursor].iTimeLength = total;
			}
			
			emit sigMediaLength(total, s_total,m_iCursor);
		}
	}
}

void IBMediaListControlC::playListUpdate(const QStringList & fileList)
{
	PlayListNode node;
	if( MEDIA_RUNNING_STATE_PLAYING == m_state || MEDIA_RUNNING_STATE_PAUSE == m_state )
	{
#ifdef _8925_VERSION
                if(m_iCursor >= 0 &&m_iCursor <  m_slPlayList.count())
                {
#endif
		        node = m_slPlayList[m_iCursor];
#ifdef _8925_VERSION
                }
                else
                {
                }
#endif
	}
	m_slPlayList.clear();
	for(int i=0; i<fileList.count(); i++)
	{
		_playListAppend(fileList[i]);
	}
	_reportPlayListChanged();
	if( !node.sFileName.isEmpty() )
	{
		for(int i=0; i<m_slPlayList.count(); i++)
		{
			if( 0 == m_slPlayList[i].sFileName.compare(node.sFileName) )
			{
				m_slPlayList[i] = node;
				m_iCursor = i;
				emit sigPlayListCursorChanged(m_iCursor);
				emit sigMediaChanged(m_slPlayList[m_iCursor].sFileName);
				_freshState();
				return;
			}
		}
	}

	if( m_slPlayList.count() > 0 )
		m_iCursor = 0;
	else
		m_iCursor = -1;

	stop();
	_freshState();
	
	emit sigPlayListCursorChanged(m_iCursor);
	if(m_iCursor>=0)
		emit sigMediaChanged(m_slPlayList[m_iCursor].sFileName);
	else
		emit sigMediaChanged(QString(""));

	return;
}

void IBMediaListControlC::playListAppend(const QStringList & fileList)
{
	for(int i=0; i<fileList.count(); i++)
	{
		_playListAppend(fileList[i]);
	}
	_reportPlayListChanged();
	_freshState();
}

void IBMediaListControlC::_playListAppend(const QString & fileName)
{
	for(int i=0; i<m_slPlayList.count(); i++)
	{
		if( m_slPlayList[i].sFileName.compare(fileName) == 0 )
			return;
	}

	PlayListNode node;
	node.sFileName = fileName;
	m_slPlayList.append(node);
}

void IBMediaListControlC::playListDelete(const int index)
{
	if((index < 0) || (index >= m_slPlayList.count()) )
	{
		return;
	}
	else
	{
	}

	if( index == m_iCursor )
	{
		stop();
		m_iCursor = 0;
	}
	else if( m_iCursor > index )
	{
		m_iCursor--;
	}
	
	m_slPlayList.removeAt(index);
	if( m_slPlayList.count() <= 0 )
	{
		m_iCursor = -1;
	}
	
	_reportPlayListChanged();

	return;
}
void IBMediaListControlC::playListDelete(const QString fileName)
{
	for(int i=0; i<m_slPlayList.count(); i++)
	{
		if( m_slPlayList[i].sFileName.compare(fileName) == 0 )
		{
			if( i == m_iCursor )
			{
				stop();
				m_iCursor = 0;
			}
			else if( m_iCursor > i )
			{
				m_iCursor--;
			}
			
			m_slPlayList.removeAt(i);
			if( m_slPlayList.count() <= 0 )
			{
				m_iCursor = -1;
			}
			
			_reportPlayListChanged();
			return;
		}
	}

	_freshState();
}

void IBMediaListControlC::playListClear()
{
	if( m_slPlayList.count() > 0 )
	{
		stop();
		m_slPlayList.clear();
		m_iCursor = -1;
		_reportPlayListChanged();
		_freshState();
	}
}

void IBMediaListControlC::setDefaultListPlayMode(const List_Play_Mode mode)
{
	if( LIST_PLAY_MODE_NO_INIT_FLAG == m_ListPlayMode )
		m_ListPlayMode = mode;
}
	
void IBMediaListControlC::setListPlayMode(const List_Play_Mode mode)
{
	m_ListPlayMode = mode;
	_saveData();
}

List_Play_Mode IBMediaListControlC::getListPlayMode()
{
	return m_ListPlayMode;
}

PlayList IBMediaListControlC::getPlayList()
{
	return m_slPlayList;
}

int IBMediaListControlC::getPlayListCursor()
{
	return m_iCursor;
}

void IBMediaListControlC::defaultPropertySetting(const unsigned int property)
{
	if( m_Property & SETTING_NO_INIT_FLAG )
	{
		m_Property = (property & (~SETTING_NO_INIT_FLAG));
	}
}

void IBMediaListControlC::propertySetting(const unsigned int property)
{
	m_Property = (property & (~SETTING_NO_INIT_FLAG));
}

unsigned int IBMediaListControlC::getProperty()
{
	return m_Property;
}
void IBMediaListControlC::_reportPlayListChanged(void)
{
    QStringList res;
    for(int i=0;i<m_slPlayList.count();i++)
    {
            res << m_slPlayList[i].sFileName;
    }
    emit sigPlayListChanged(res);
}

bool IBMediaListControlC::_isSupport(Player_Property property)
{
	if( (m_Property&property) >0 )
		return true;
	else
		return false;
}

int IBMediaListControlC::_playNext(bool bLoop)
{
	int iPreviousCursor = m_iCursor;
	
	if( m_slPlayList.isEmpty() )
		return MEDIA_E_LIST_VOID;

	while(1)
	{
		if( !bLoop && m_iCursor == m_slPlayList.count()-1 )
		{
			if( _isSupport(PLAYER_PROPERTY_SKIP_ABNORMAL_FILE) )
			{
				if( !(m_slPlayList[m_iCursor].bNormal) )
				{
					m_iCursor = iPreviousCursor;
				}
			}
			break;
		}

		if( m_iCursor < 0 )
		{
			m_iCursor = 0;
		}
		else
		{
			m_iCursor++;
			if( m_iCursor >= m_slPlayList.count() )
				m_iCursor = 0;
		}

		if( iPreviousCursor == m_iCursor )
			break;
		
		if( _isSupport(PLAYER_PROPERTY_SKIP_ABNORMAL_FILE) )
		{
			if( !(m_slPlayList[m_iCursor].bNormal) )
			{
				continue;
			}
		}
		break;
	}
	
	if( iPreviousCursor != m_iCursor )
	{
		stop();
		start();
		emit sigPlayListCursorChanged(m_iCursor);
		emit sigMediaChanged(m_slPlayList[m_iCursor].sFileName);

		return MEDIA_E_OK;
	}

	return MEDIA_E_NOT_TO_OTHER_FILE;
}

int IBMediaListControlC::_playPrevious(bool bLoop)
{
	int iPreviousCursor = m_iCursor;
	
	if( m_slPlayList.isEmpty() )
		return MEDIA_E_LIST_VOID;

	while(1)
	{
		if( m_iCursor < 0 )
		{
			m_iCursor = 0;
		}
		else
		{
			m_iCursor--;
			if( m_iCursor < 0 )
				m_iCursor = m_slPlayList.count()-1;
		}

		if( iPreviousCursor == m_iCursor )
			break;
		
		if( !bLoop && m_iCursor == 0 )
		{
			if( _isSupport(PLAYER_PROPERTY_SKIP_ABNORMAL_FILE) )
			{
				if( !(m_slPlayList[m_iCursor].bNormal) )
				{
					m_iCursor = iPreviousCursor;
				}
			}
			break;
		}

		if( _isSupport(PLAYER_PROPERTY_SKIP_ABNORMAL_FILE) )
		{
			if( !(m_slPlayList[m_iCursor].bNormal) )
			{
				continue;
			}
		}
		break;
	}
	
	if( iPreviousCursor != m_iCursor )
	{
		stop();
		start();
		emit sigPlayListCursorChanged(m_iCursor);
		emit sigMediaChanged(m_slPlayList[m_iCursor].sFileName);
		return MEDIA_E_OK;
	}

	return MEDIA_E_NOT_TO_OTHER_FILE;
}

int IBMediaListControlC::_playRand()
{
	int iPreviousCursor = m_iCursor;
	
	if( m_slPlayList.isEmpty() )
		return MEDIA_E_LIST_VOID;

	int iCount = m_slPlayList.count();
	if( _isSupport(PLAYER_PROPERTY_SKIP_ABNORMAL_FILE) )
	{
		iCount = 0;
		for(int i=0; i<m_slPlayList.count(); i++)
		{
			if( m_slPlayList[m_iCursor].bNormal )
			{
				iCount++;
			}
		}
	}

	srand((int)time(0));

	while( iCount > 1 && iPreviousCursor == m_iCursor )
	{
		int iIndex = rand()%iCount;
		for(int i=0; i<m_slPlayList.count(); i++)
		{
            if( 0 == iIndex )
            {
                m_iCursor = i;
                break;
            }

			if( _isSupport(PLAYER_PROPERTY_SKIP_ABNORMAL_FILE) )
			{
				if( m_slPlayList[i].bNormal )
				{
					iIndex--;
				}
			}
			else
			{
				iIndex--;
			}
		}
	}

	if( iPreviousCursor != m_iCursor )
	{
		stop();
		start();
		emit sigPlayListCursorChanged(m_iCursor);
		emit sigMediaChanged(m_slPlayList[m_iCursor].sFileName);
		return MEDIA_E_OK;
	}	

	return MEDIA_E_NOT_TO_OTHER_FILE;
}

void IBMediaListControlC::playNext()
{
	_freshState(true);
	
	if( LIST_PLAY_MODE_RANDOM == m_ListPlayMode )
	{
		_playRand();
	}
	else
	{
		_playNext();
	}
}

void IBMediaListControlC::playPrevious()
{
	_freshState(true);
	
	if( LIST_PLAY_MODE_RANDOM == m_ListPlayMode )
	{
		_playRand();
	}
	else
	{
		_playPrevious();
	}
}

void IBMediaListControlC::fastForward(int iSecond)
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return;

	int position = m_slPlayList[m_iCursor].iPosition + iSecond;
	changePlayPosition(position);
}

void IBMediaListControlC::start()
{
	if( m_slPlayList.isEmpty() )
		return;
	
	if( m_iCursor < 0 )
	{
		m_iCursor = 0;
	}
	if( _isSupport(PLAYER_PROPERTY_SKIP_ABNORMAL_FILE) )
	{
		if( !(m_slPlayList[m_iCursor].bNormal) )
		{
			return;
		}
	}

	if( m_MediaType == IBMediaControlC::object()->getMediaType() )
	{
		if( MEDIA_RUNNING_STATE_PLAYING == m_state )
		{
			return;
		}
		else if( MEDIA_RUNNING_STATE_STOP == m_state || MEDIA_RUNNING_STATE_FINISHED == m_state )
		{
            m_slPlayList[m_iCursor].iTimeLength = -1;
			m_state = MEDIA_RUNNING_STATE_PLAYING;
			m_slPlayList[m_iCursor].iPosition = 0;
			if( m_iTimerPlay >= 0 )
			{
				killTimer(m_iTimerPlay);
				m_iTimerPlay = -1;
			}
			m_iTimerPlay = startTimer(m_iTimerPlayDelay);
		}
		else if( MEDIA_RUNNING_STATE_PAUSE == m_state )
		{
	            resume();
		}
		else
		{
			//It is impossible to come here;
	            return;
		}
	}
	else
	{
		m_state = MEDIA_RUNNING_STATE_PLAYING;
		_play(m_slPlayList[m_iCursor].sFileName, m_slPlayList[m_iCursor].iPosition);
	}

    // 增加反馈信号[3/2/2012 yuzhenyang]
    emit sigPlayListCursorChanged(m_iCursor);
    emit sigMediaChanged(m_slPlayList[m_iCursor].sFileName);

	_freshState();

	return;
}

void IBMediaListControlC::_play(const QString & fileName, int seek)
{
	if( MPLAYER_USER_SET == m_AudioEffect )
	{
		IBMediaControlC::object()->setAudioEeffect(m_AudioEffect);
		IBMediaControlC::object()->setAudioEqualizer(m_AudioEqualizer);
	}
	else
	{
		IBMediaControlC::object()->setAudioEeffect(m_AudioEffect);
	}
	IBMediaControlC::object()->setFullScreenWindowMode(isFullScreenWindowMode());
	IBMediaControlC::object()->setTimeFormat(m_iTimeFormat);
	IBMediaControlC::object()->play(fileName, m_MediaType, seek);
}
void IBMediaListControlC::saveData()
{
	_saveData();
}

void IBMediaListControlC::playListSetCursor(int cursor)
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 || m_slPlayList.count() <= cursor )
		return;

	if( m_iCursor != cursor )
	{
		if( m_MediaType == IBMediaControlC::object()->getMediaType() )
		{
			IBMediaControlC::object()->stop();
		}
		m_iCursor = cursor;
		m_state = MEDIA_RUNNING_STATE_STOP;
		m_slPlayList[m_iCursor].iPosition = 0;
		start();
		emit sigPlayListCursorChanged(m_iCursor);
		emit sigMediaChanged(m_slPlayList[m_iCursor].sFileName);
	}
	else
	{
		if( MEDIA_RUNNING_STATE_PLAYING != m_state )
			start();
	}
}

void IBMediaListControlC::playListSetCursorNoPlay(int cursor)
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 || m_slPlayList.count() <= cursor )
		return;

	if( m_iCursor != cursor )
	{
		if( m_MediaType == IBMediaControlC::object()->getMediaType() )
		{
			IBMediaControlC::object()->stop();
		}
		m_iCursor = cursor;
		m_state = MEDIA_RUNNING_STATE_STOP;
		m_slPlayList[m_iCursor].iPosition = 0;
		//start();
		emit sigPlayListCursorChanged(m_iCursor);
		emit sigMediaChanged(m_slPlayList[m_iCursor].sFileName);
	}
	else
	{
		//if( MEDIA_RUNNING_STATE_PLAYING != m_state )
		//	start();
	}
}

void IBMediaListControlC::stop()
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return;
	
	if( MEDIA_RUNNING_STATE_STOP == m_state )
	{
		return;
	}

	m_state = MEDIA_RUNNING_STATE_STOP;
	m_slPlayList[m_iCursor].iPosition = 0;
	QString s_time;
	s_time = IBMediaControlC::object()->transferTimeFormat(0,m_MediaType);
	emit sigMediaPosition(0, s_time);

	_freshState();

	if( m_MediaType == IBMediaControlC::object()->getMediaType() )
	{
		if( m_iTimerPlay >= 0 )
		{
			killTimer(m_iTimerPlay);
			m_iTimerPlay = -1;
		}

		IBMediaControlC::object()->stop();
	}

}

void IBMediaListControlC::pause()
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return;
	
	if( MEDIA_RUNNING_STATE_PLAYING == m_state )
	{
		if( m_MediaType == IBMediaControlC::object()->getMediaType() )
		{
			if( m_iTimerPlay >= 0 )
			{
			
			}
			else
			{
				m_state = MEDIA_RUNNING_STATE_PAUSE;
				m_bPrePauseFromUser = true;
				IBMediaControlC::object()->pause();
			}

			_freshState();
		}
		else
		{
			
		}

	}
}

void IBMediaListControlC::resume()
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return;

	if( MEDIA_RUNNING_STATE_PAUSE == m_state )
	{
		m_state = MEDIA_RUNNING_STATE_PLAYING;
		if( m_MediaType == IBMediaControlC::object()->getMediaType() )
		{
			IBMediaControlC::object()->resume();
		}
		else
		{
			_play(m_slPlayList[m_iCursor].sFileName, m_slPlayList[m_iCursor].iPosition);
		}

		_freshState();
	}
}

void IBMediaListControlC::resumeFromPause(void)
{
        if( m_slPlayList.isEmpty() || m_iCursor < 0 )
                return;

        m_state = MEDIA_RUNNING_STATE_PLAYING;
        _play(m_slPlayList[m_iCursor].sFileName, m_slPlayList[m_iCursor].iPosition);
        _freshState();
        return;
}


void IBMediaListControlC::resumeMediaStatus()
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return;

	if( MEDIA_RUNNING_STATE_PAUSE == m_state )
	{
		if( !m_bPrePauseFromUser )
		{
			resume();
		}
	}
}

void IBMediaListControlC::mediaTypeChangeToNone(bool bExceptMyself)
{
	if( !bExceptMyself )
	{
		IBMediaControlC::object()->mediaTypeChangeToNone();
	}
	else
	{
		if( m_MediaType != IBMediaControlC::object()->getMediaType() )	
		{
			IBMediaControlC::object()->mediaTypeChangeToNone();
		}
	}
}

void IBMediaListControlC::changePlayPosition(int position)
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return;

	if( MEDIA_RUNNING_STATE_STOP != m_state && MEDIA_RUNNING_STATE_FINISHED != m_state )
	{
		if( position < 0 || position > m_slPlayList[m_iCursor].iTimeLength )
		{
			stop();
		}
		else
		{
			if( m_MediaType == IBMediaControlC::object()->getMediaType() )
			{
				IBMediaControlC::object()->changePosition(position);
                                m_slPlayList[m_iCursor].iPosition = position;//for bug:play music,then pause,move slider,goto VIDEO-UI  and back to play music,PLAY-TIME would be the PAUSE-TIME
			}
			else
			{
				m_slPlayList[m_iCursor].iPosition = position;
				_play(m_slPlayList[m_iCursor].sFileName, m_slPlayList[m_iCursor].iPosition);
				IBMediaControlC::object()->pause();
	 		}
		}
	}
}

int IBMediaListControlC::getCurrentMediaPlayPosition(void)
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return -1;

	if( MEDIA_RUNNING_STATE_STOP != m_state )
	{
		return m_slPlayList[m_iCursor].iPosition;
	}
	else
	{
		return -1;
	}
}

Media_Running_State IBMediaListControlC::getCurrentMediaState(void)
{
	return m_state;
}

void IBMediaListControlC::setTimeFormat(MEDIA_TIME_FORMAT format)
{
	m_iTimeFormat = format;

	if( MEDIA_TYPE_MOVIE == IBMediaControlC::object()->getMediaType() )
	{
		IBMediaControlC::object()->setTimeFormat(m_iTimeFormat);
	}
}

MEDIA_TIME_FORMAT IBMediaListControlC::getTimeFormat(void)
{
	return m_iTimeFormat;
}

int IBMediaListControlC::getCurrentMediaLength(void)
{
	if( m_slPlayList.isEmpty() || m_iCursor < 0 )
		return -1;

	return m_slPlayList[m_iCursor].iTimeLength;
}

int IBMediaListControlC::_saveData()
{
	/*int ret = 0;
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
	//delete content of table about media setting
	/////////////////////////////////////////////	
	if( MEDIA_TYPE_MOVIE == m_MediaType )
	{
		strcat(pDeleteAllInfoSel , "delete from MovieControlSetting");
	}
	else if( MEDIA_TYPE_MUSIC == m_MediaType )
	{
		strcat(pDeleteAllInfoSel , "delete from MusicControlSetting");
	}
	else if( MEDIA_TYPE_STORY == m_MediaType )
	{
		strcat(pDeleteAllInfoSel , "delete from StoryControlSetting");
	}
	else if( MEDIA_TYPE_NET_MUSIC == m_MediaType )
	{
		strcat(pDeleteAllInfoSel , "delete from NetMusicControlSetting");
	}
	else
	{
		//It is impossible to come here
	}	

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
	//insert content to table about media setting
	/////////////////////////////////////////////	
	if( MEDIA_TYPE_MOVIE == m_MediaType )
	{
		sprintf(pInsertTablesSel, "insert into MovieControlSetting \
			values('%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d')", \
			m_iCursor, m_ListPlayMode, m_Property, m_iTimeFormat, m_AudioEffect, m_bUserSetAudioEffect, \
			m_AudioEqualizer.iEqualizer[0], m_AudioEqualizer.iEqualizer[1], m_AudioEqualizer.iEqualizer[2], m_AudioEqualizer.iEqualizer[3], m_AudioEqualizer.iEqualizer[4], \
			m_AudioEqualizer.iEqualizer[5], m_AudioEqualizer.iEqualizer[6], m_AudioEqualizer.iEqualizer[7], m_AudioEqualizer.iEqualizer[8], m_AudioEqualizer.iEqualizer[9]);
	}
	else if( MEDIA_TYPE_MUSIC == m_MediaType )
	{
		sprintf(pInsertTablesSel, "insert into MusicControlSetting \
			values('%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d')", \
			m_iCursor, m_ListPlayMode, m_Property, m_iTimeFormat, m_AudioEffect, m_bUserSetAudioEffect, \
			m_AudioEqualizer.iEqualizer[0], m_AudioEqualizer.iEqualizer[1], m_AudioEqualizer.iEqualizer[2], m_AudioEqualizer.iEqualizer[3], m_AudioEqualizer.iEqualizer[4], \
			m_AudioEqualizer.iEqualizer[5], m_AudioEqualizer.iEqualizer[6], m_AudioEqualizer.iEqualizer[7], m_AudioEqualizer.iEqualizer[8], m_AudioEqualizer.iEqualizer[9]);
	}
	else if( MEDIA_TYPE_STORY == m_MediaType )
	{
		sprintf(pInsertTablesSel, "insert into StoryControlSetting \
			values('%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d')", \
			m_iCursor, m_ListPlayMode, m_Property, m_iTimeFormat, m_AudioEffect, m_bUserSetAudioEffect, \
			m_AudioEqualizer.iEqualizer[0], m_AudioEqualizer.iEqualizer[1], m_AudioEqualizer.iEqualizer[2], m_AudioEqualizer.iEqualizer[3], m_AudioEqualizer.iEqualizer[4], \
			m_AudioEqualizer.iEqualizer[5], m_AudioEqualizer.iEqualizer[6], m_AudioEqualizer.iEqualizer[7], m_AudioEqualizer.iEqualizer[8], m_AudioEqualizer.iEqualizer[9]);
	}
	else if( MEDIA_TYPE_NET_MUSIC == m_MediaType )
	{
		sprintf(pInsertTablesSel, "insert into NetMusicControlSetting \
			values('%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d')", \
			m_iCursor, m_ListPlayMode, m_Property, m_iTimeFormat, m_AudioEffect, m_bUserSetAudioEffect, \
			m_AudioEqualizer.iEqualizer[0], m_AudioEqualizer.iEqualizer[1], m_AudioEqualizer.iEqualizer[2], m_AudioEqualizer.iEqualizer[3], m_AudioEqualizer.iEqualizer[4], \
			m_AudioEqualizer.iEqualizer[5], m_AudioEqualizer.iEqualizer[6], m_AudioEqualizer.iEqualizer[7], m_AudioEqualizer.iEqualizer[8], m_AudioEqualizer.iEqualizer[9]);
	}
	else
	{
		//It is impossible to come here
	}

	ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
                IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error 1: %s\n", sqlite3_errmsg(pDB));
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
	//delete content of table about media list
	/////////////////////////////////////////////	
        memset(pDeleteAllInfoSel,0,sizeof(pDeleteAllInfoSel));
	if( MEDIA_TYPE_MOVIE == m_MediaType )
	{
		strcat(pDeleteAllInfoSel , "delete from MovieControlList");
	}
	else if( MEDIA_TYPE_MUSIC == m_MediaType )
	{
		strcat(pDeleteAllInfoSel , "delete from MusicControlList");
	}
	else if( MEDIA_TYPE_STORY == m_MediaType )
	{
		strcat(pDeleteAllInfoSel , "delete from StoryControlList");
	}
	else if( MEDIA_TYPE_NET_MUSIC == m_MediaType )
	{
		strcat(pDeleteAllInfoSel , "delete from NetMusicControlList");
	}
	else
	{
		//It is impossible to come here
	}

	ret = sqlite3_prepare(pDB, pDeleteAllInfoSel, strlen(pDeleteAllInfoSel), &pStmt, (const char**)&pError);
	if (ret != SQLITE_OK)
	{
                IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error 2: %s\n", sqlite3_errmsg(pDB));
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
	//insert content to table about media list
	/////////////////////////////////////////////	
	for(int loop=0; loop<m_slPlayList.count(); loop++)
	{
		if( MEDIA_TYPE_MOVIE == m_MediaType )
		{
			sprintf(pInsertTablesSel, "insert into MovieControlList values('%s', '%d', '%d', '%d')", \
				m_slPlayList[loop].sFileName.toLocal8Bit().data(), m_slPlayList[loop].iTimeLength, \
				m_slPlayList[loop].fileStatus, m_slPlayList[loop].iPosition);
		}
		else if( MEDIA_TYPE_MUSIC == m_MediaType )
		{
			sprintf(pInsertTablesSel, "insert into MusicControlList values('%s', '%d', '%d', '%d')", \
				m_slPlayList[loop].sFileName.toLocal8Bit().data(), m_slPlayList[loop].iTimeLength, \
				m_slPlayList[loop].fileStatus, m_slPlayList[loop].iPosition);
		}
		else if( MEDIA_TYPE_STORY == m_MediaType )
		{
			sprintf(pInsertTablesSel, "insert into StoryControlList values('%s', '%d', '%d', '%d')", \
				m_slPlayList[loop].sFileName.toLocal8Bit().data(), m_slPlayList[loop].iTimeLength, \
				m_slPlayList[loop].fileStatus, m_slPlayList[loop].iPosition);
		}
		else if( MEDIA_TYPE_NET_MUSIC == m_MediaType )
		{
			sprintf(pInsertTablesSel, "insert into NetMusicControlList values('%s', '%d', '%d', '%d')", \
				m_slPlayList[loop].sFileName.toLocal8Bit().data(), m_slPlayList[loop].iTimeLength, \
				m_slPlayList[loop].fileStatus, m_slPlayList[loop].iPosition);
		}
		else
		{
			//It is impossible to come here
		}

		ret = sqlite3_prepare(pDB, pInsertTablesSel, strlen(pInsertTablesSel), &pStmt, (const char**)&pError);
		if (ret != SQLITE_OK)
		{
                        IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error 3: %s\n", sqlite3_errmsg(pDB));
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
	
	sqlite3_close(pDB);*/

        QString fileName="/nand3/applicationdata/SettingMusic.xml";
        FileListUtil util(fileName, 0, "<!DOCTYPE SettingMusic>");
        QStringList List;
        List.append( QString("%1|0").arg(m_ListPlayMode));
        util.saveFileList(List);

	return 0;
}

int IBMediaListControlC::_loadData()
{
	/*int ret = 0;
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
	//read content from table about media setting
	/////////////////////////////////////////////	
	if( MEDIA_TYPE_MOVIE == m_MediaType )
	{
		strcpy(pSqlSel , "select * from MovieControlSetting");
	}
	else if( MEDIA_TYPE_MUSIC == m_MediaType )
	{
		strcpy(pSqlSel , "select * from MusicControlSetting");
	}
	else if( MEDIA_TYPE_STORY == m_MediaType )
	{
		strcpy(pSqlSel , "select * from StoryControlSetting");
	}
	else if( MEDIA_TYPE_NET_MUSIC == m_MediaType )
	{
		strcpy(pSqlSel , "select * from NetMusicControlSetting");
	}
	else
	{
		//It is impossible to come here
	}

	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt,( const char**)&pError );
	if (ret != SQLITE_OK)
	{
                IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error 4: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_step(pStmt);
	if( ret != SQLITE_ROW )
	{
		//table is empty, because we first use them
		IBD_PRINTF(DEBUG_INFOMATION, "table of media controling is void\n");
	}
	else
	{
		m_iCursor = (int)sqlite3_column_int(pStmt, 0);
		m_ListPlayMode = (List_Play_Mode)sqlite3_column_int(pStmt, 1);
		m_Property = (unsigned int)sqlite3_column_int(pStmt, 2);
		m_iTimeFormat = (MEDIA_TIME_FORMAT)sqlite3_column_int(pStmt, 3);
		m_AudioEffect = (M_AUDIO_EFFCET)sqlite3_column_int(pStmt, 4);
		m_bUserSetAudioEffect = (bool)sqlite3_column_int(pStmt, 5);
		for(int i=0; i<MAX_EQUALIZER; i++)
		{
			m_AudioEqualizer.iEqualizer[i] = (int)sqlite3_column_int(pStmt, (6+i));
		}
	}
	
	sqlite3_finalize(pStmt);



	/////////////////////////////////////////////	
	//read content from table about media list
	/////////////////////////////////////////////	
	if( MEDIA_TYPE_MOVIE == m_MediaType )
	{
		strcpy(pSqlSel , "select * from MovieControlList");
	}
	else if( MEDIA_TYPE_MUSIC == m_MediaType )
	{
		strcpy(pSqlSel , "select * from MusicControlList");
	}
	else if( MEDIA_TYPE_STORY == m_MediaType )
	{
		strcpy(pSqlSel , "select * from StoryControlList");
	}
	else if( MEDIA_TYPE_NET_MUSIC == m_MediaType )
	{
		strcpy(pSqlSel , "select * from NetMusicControlList");
	}
	else
	{
		//It is impossible to come here
	}

	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt,( const char**)&pError );
	if (ret != SQLITE_OK)
	{
                IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error 5: %s\n", sqlite3_errmsg(pDB));
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
			PlayListNode node;
			node.sFileName = (char*)sqlite3_column_text(pStmt, 0);
			node.iTimeLength = (int)sqlite3_column_int(pStmt, 1);
			node.fileStatus = (File_Status)sqlite3_column_int(pStmt, 2);
			node.iPosition = (int)sqlite3_column_int(pStmt, 3);
			m_slPlayList.append(node);
		}
	}
	
	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);*/
	
        QString fileName="/nand3/applicationdata/SettingMusic.xml";
        FileListUtil util(fileName, 0, "<!DOCTYPE SettingMusic>");
        QStringList List=util.openFileList();
        if( List.count() > 0 )
        {
                QStringList listSplit = List[0].split("|");
                m_ListPlayMode = (MediaControl2::List_Play_Mode)listSplit[0].toInt();
        }
        else
        {
                _saveData();
        }

	return 0;
}

void IBMediaListControlC::timerEvent(QTimerEvent * event)
{
	int iTimerId = event->timerId();

	if( m_iTimerPlay == iTimerId )
	{
		killTimer(m_iTimerPlay);
		m_iTimerPlay = -1;
#ifdef _8925_VERSION
                if(m_iCursor >= 0 &&m_iCursor <  m_slPlayList.count())
                {
#endif
		        _play(m_slPlayList[m_iCursor].sFileName, m_slPlayList[m_iCursor].iPosition);
#ifdef _8925_VERSION
                }
                else
                {
                }
#endif
	}
	else if( m_iTimerFreshState == iTimerId )
	{
		killTimer(m_iTimerFreshState);
		m_iTimerFreshState = -1;

		_sendState();
	}
	else
	{

	}
}

void IBMediaListControlC::sdPop(QString sdDir)
{
	if( _isSupport(PLAYER_PROPERTY_DELETE_SD_FILE) )
	{
		bool bMaybeNeedStart = false;
		bool bChanged = false;
		
		for(int i=0; i<m_slPlayList.count();)
		{
			if( m_slPlayList[i].sFileName.startsWith(sdDir) )
			{
				bChanged = true;
				if( i == m_iCursor )
				{
					if( MEDIA_RUNNING_STATE_PLAYING == m_state )
						bMaybeNeedStart = true;
					stop();
					m_iCursor = 0;
				}
				else if( m_iCursor > i )
				{
					m_iCursor--;
				}
				
				m_slPlayList.removeAt(i);
				if( m_slPlayList.count() <= 0 )
				{
					m_iCursor = -1;
				}
				i = 0;
				continue;
			}
			i++;
		}	

		if( bChanged )
		
{
			_reportPlayListChanged();
		}

		if( bMaybeNeedStart )
		{
			if( LIST_PLAY_MODE_SEQUENCE_LOOPED == m_ListPlayMode || LIST_PLAY_MODE_RANDOM == m_ListPlayMode)
			{
				start();
			}
		}
	}
	else
	{
		bool bFinished = false;
		
		for(int i=0; i<m_slPlayList.count();i++)
		{
			if( m_slPlayList[i].sFileName.startsWith(sdDir) )
			{
				if( i == m_iCursor )
				{
					if( MEDIA_RUNNING_STATE_PLAYING == m_state )
						bFinished = true;
					stop();
				}
				
				m_slPlayList[i].bNormal = false;
			}
		}	

		if( bFinished )
		{
			slotMediaStateChanged(MEDIA_RUNNING_STATE_FINISHED);
		}
	}
}

bool IBMediaListControlC::_isFileExist(const QString fileName)
{
	return QFile::exists(fileName);
}

void IBMediaListControlC::_deleteAllNoOpenFile()
{
	bool bChanged = false;
	
	for(int i=0; i<m_slPlayList.count();)
	{
		if( !_isFileExist(m_slPlayList[i].sFileName) )
		{
			bChanged = true;
			if( i == m_iCursor )
			{
				stop();
				m_iCursor = 0;
			}
			else if( m_iCursor > i )
			{
				m_iCursor--;
			}
			
			m_slPlayList.removeAt(i);
			if( m_slPlayList.count() <= 0 )
			{
				m_iCursor = -1;
			}
			i = 0;
			continue;
		}
		i++;
	}	

	if( bChanged )
		_reportPlayListChanged();
}

void IBMediaListControlC::_freshAllNoOpenFile()
{
	for(int i=0; i<m_slPlayList.count(); i++)
	{
		if( !_isFileExist(m_slPlayList[i].sFileName) )
		{
			m_slPlayList[i].bNormal = false;
		}
	}	
}

void IBMediaListControlC::reportInitMessage()
{
	_reportPlayListChanged();
	if( !m_slPlayList.isEmpty() && m_iCursor >= 0)
	{
		emit sigPlayListCursorChanged(m_iCursor);
		emit sigMediaChanged(m_slPlayList[m_iCursor].sFileName);
	}
}

void IBMediaListControlC::setFullScreenWindowMode(bool bFullScreen)
{
	if( m_bFullScreen != bFullScreen )
	{
		m_bFullScreen = bFullScreen;

		if( m_slPlayList.isEmpty() )
			return;

		if( MEDIA_RUNNING_STATE_STOP != m_state )
		{
			if( MEDIA_TYPE_MOVIE == IBMediaControlC::object()->getMediaType() )
			{
				IBMediaControlC::object()->setFullScreenWindowMode(m_bFullScreen);
			}
			else
			{
				start();
				pause();
	 		}
		}
	}
}

bool IBMediaListControlC::isFullScreenWindowMode(void)
{
	return m_bFullScreen;
}

void IBMediaListControlC::openVideoScreen(void)
{
	IBMediaControlC::object()->openVideoScreen();
}

void IBMediaListControlC::closeVideoScreen(void)
{
	IBMediaControlC::object()->closeVideoScreen();
}

int IBMediaListControlC::getFunctionState()
{
	return m_iFunctionState;
}

void IBMediaListControlC::sendFunctionState()
{
	emit sigFunctionStateChanged(m_iFunctionState);
}

void IBMediaListControlC::_sendState()
{
	int iFunctionState = 0;
	static Media_Running_State sState = m_state;

	if( m_slPlayList.count() > 0 )
	{
		if( MEDIA_RUNNING_STATE_PLAYING == m_state )
		{
			iFunctionState |= MASK_FUNCTION_STOP;
			iFunctionState |= MASK_FUNCTION_PAUSE;
		}
		else if( MEDIA_RUNNING_STATE_STOP == m_state )
		{
			iFunctionState |= MASK_FUNCTION_PLAY;
		}
		else if( MEDIA_RUNNING_STATE_PAUSE == m_state )
		{
			iFunctionState |= MASK_FUNCTION_PLAY;
			iFunctionState |= MASK_FUNCTION_STOP;
			iFunctionState |= MASK_FUNCTION_RESUME;
		}
		else if( MEDIA_RUNNING_STATE_FINISHED == m_state )
		{
			iFunctionState |= MASK_FUNCTION_PLAY;
		}
		else
		{
			//It is impossible to come here
		}

		if( LIST_PLAY_MODE_SINGLE == m_ListPlayMode || LIST_PLAY_MODE_SINGLE_LOOPED == m_ListPlayMode || LIST_PLAY_MODE_SEQUENCE == m_ListPlayMode )
		{
			int iNext = -1;
			for(int i=m_iCursor+1; i<m_slPlayList.count(); i++)
			{
				if( _isSupport(PLAYER_PROPERTY_SKIP_ABNORMAL_FILE) )
				{
					if( m_slPlayList[i].bNormal )
					{
						iNext = i;
						break;
					}
				}
				else
				{
					iNext = i;
					break;
				}
			}
			if( iNext != -1 )
			{
				iFunctionState |= MASK_FUNCTION_PLAYNEXT;
			}
			
			int iPre = -1;
			for(int i=m_iCursor-1; i>=0; i++)
			{
				if( _isSupport(PLAYER_PROPERTY_SKIP_ABNORMAL_FILE) )
				{
					if( m_slPlayList[i].bNormal )
					{
						iPre = i;
						break;
					}
				}
				else
				{
					iPre = i;
					break;
				}
			}
			if( iPre != -1 )
			{
				iFunctionState |= MASK_FUNCTION_PLAYPRE;
			}
		}
		else
		{
			int iNext = -1;
			for(int i=0; i<m_slPlayList.count(); i++)
			{
				if( i != m_iCursor )
				{
					if( _isSupport(PLAYER_PROPERTY_SKIP_ABNORMAL_FILE) )
					{
						if( m_slPlayList[i].bNormal )
						{
							iNext = i;
							break;
						}
					}
					else
					{
						iNext = i;
						break;
					}
				}
			}
			if( iNext != -1 )
			{
				iFunctionState |= MASK_FUNCTION_PLAYNEXT;
				iFunctionState |= MASK_FUNCTION_PLAYPRE;
			}
		}
	}

	if( sState != m_state )
	{
		sState = m_state;
		emit sigMediaStateChanged(sState,m_iCursor);
	}
	
	if( m_iFunctionState != iFunctionState )
	{
		m_iFunctionState = iFunctionState;
		emit sigFunctionStateChanged(m_iFunctionState);
	}
}

void IBMediaListControlC::_freshState(bool bDelay)
{
	if( bDelay )
	{
		if( m_iTimerFreshState >= 0 )
		{
			killTimer(m_iTimerFreshState);
			m_iTimerFreshState = -1;
		}
		m_iTimerFreshState = startTimer(m_iTimerFreshStateDelay);
	}
	else
	{
		if( m_iTimerFreshState < 0 )
		{
			_sendState();
		}
	}
}

void IBMediaListControlC::setAudioEffect(const M_AUDIO_EFFCET effect)
{
	if( effect < MPLAYER_AUDIO_EFFCET_NULL || effect >= MPLAYER_MAX_AUDIO_EFFCET )
		return;

	m_AudioEffect = effect;

	if( m_MediaType == IBMediaControlC::object()->getMediaType() )
	{
		if( MEDIA_RUNNING_STATE_PLAYING == m_state || MEDIA_RUNNING_STATE_PAUSE == m_state )
		{
			IBMediaControlC::object()->setAudioEeffect(m_AudioEffect);
		}
	}
	
	emit sigAudioEffect(m_AudioEffect);

	if( MPLAYER_USER_SET == m_AudioEffect )
	{
		emit sigEqualizer(m_AudioEqualizer);
	}
	else
	{
		Audio_Equalizer_S curEqualizer = IBMediaControlC::object()->getAudioEqualize(m_AudioEffect);
		if( !m_bUserSetAudioEffect )
		{
			m_AudioEqualizer = curEqualizer;
		}
		emit sigEqualizer(curEqualizer);
	}
}

M_AUDIO_EFFCET IBMediaListControlC::getAudioEffect()
{
	return m_AudioEffect;
}

void IBMediaListControlC::setAudioEqualizer(const Audio_Equalizer_S sEqualizer)
{
	m_AudioEqualizer = sEqualizer;

	if( m_MediaType == IBMediaControlC::object()->getMediaType() )
	{
		if( MEDIA_RUNNING_STATE_PLAYING == m_state || MEDIA_RUNNING_STATE_PAUSE == m_state )
		{
			IBMediaControlC::object()->setAudioEqualizer(m_AudioEqualizer);
		}
	}

	m_bUserSetAudioEffect = true;

	if( MPLAYER_USER_SET != m_AudioEffect )
	{
		m_AudioEffect = MPLAYER_USER_SET;
		emit sigAudioEffect(m_AudioEffect);
	}
	
	emit sigEqualizer(m_AudioEqualizer);
}

Audio_Equalizer_S IBMediaListControlC::getAudioEqualizer()
{
	if( MPLAYER_USER_SET == m_AudioEffect )
	{
		return m_AudioEqualizer;
	}
	else
	{
		Audio_Equalizer_S curEqualizer = IBMediaControlC::object()->getAudioEqualize(m_AudioEffect);
		return curEqualizer;
	}
}



	




}


