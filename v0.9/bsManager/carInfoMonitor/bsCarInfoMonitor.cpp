#include "bsCarInfoMonitor.h"
#include <QXmlStreamReader>
#include <curl.h>
#include <QDir>
#include <QDebug>
#include <sqlite3.h>
#include <QDateTime>
#include "carDataUpload.h"
#include "carDataGather.h"
#include "OBDDataGather.h"
#include "common_config.h"
#include "bsCommunalLock.h"
#define CAR_INFO_IS_USED(carInfo)		(m_CarDataStatus[carInfo].type==carInfo ? true : false)

static int _readCarInfoFromDatabase(CarInfoList *ptypeList)
{
	int retval = 0; 
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char *pQueryTablesSel = (char*)"SELECT * FROM CarInfoList";//查表语句
			CarInfoList typeList;
	retval = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "can't open database : %s!\n", sqlite3_errmsg(pDB))
		sqlite3_close(pDB);
		return -1;
	}

	retval = sqlite3_busy_timeout(pDB, 1000);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	retval = sqlite3_prepare(pDB, pQueryTablesSel, strlen(pQueryTablesSel), &pStmt, (const char**)&pError);
	if (retval != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))

		sqlite3_close(pDB);
		return -1;
	}
	int i = 0;
	while(1)
	{
		retval = sqlite3_step(pStmt);
		if (retval != SQLITE_ROW)
		{
			sqlite3_finalize(pStmt);
			sqlite3_close(pDB);
			break;
		}
		else
		{
			int value = sqlite3_column_int(pStmt, 0);
			if( value < 0 )
			{
				sqlite3_finalize(pStmt);
				sqlite3_close(pDB);
				return -1;
			}
			
			ptypeList->append((CAR_INFO_TYPE)value);
			i++;
		}
			
	}

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	return 0;
}
static int _saveCarInfoToDatabase(CarInfoList *ptypeList)
{
	sqlite3 *pDB = NULL;
        sqlite3_stmt *pStmt = NULL;
        char *pError = NULL;
       char pDeleteCarInfoList[128] = "delete from CarInfoList";
        char pInsertCarInfoList[128] = {0};
	  int ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
        if (ret != 0) {
                fprintf(stderr, "Fail to Open the DB file!!\n");
	//	gs_mutexForThreadSafe.unlock();	
                return -1;
        }

	ret = sqlite3_busy_timeout(pDB, 1000);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_busy_timeout error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}
	
	ret = sqlite3_prepare(pDB,pDeleteCarInfoList, strlen(pDeleteCarInfoList), &pStmt, (const char**)&pError);

	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);
		return -1;
	}

	while(1)
	{
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_ROW)
			break;
	}
	//插入记录条
	
	for(int i = 0;i<ptypeList->size();i++)
	{
		
		sprintf(pInsertCarInfoList, "insert into CarInfoList values('%d')",(int) ptypeList->at(i));
	
	//准备插入数据
		ret = sqlite3_prepare(pDB, pInsertCarInfoList, strlen(pInsertCarInfoList), &pStmt, (const char**)&pError);
      		if (ret != SQLITE_OK)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
			break;
		}
		ret = sqlite3_step(pStmt);
		if (ret != SQLITE_DONE)
		{
		IBD_PRINTF(DEBUG_INFOMATION, "list table count error: %s!\n", sqlite3_errmsg(pDB))
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);	
		return -1;
		}
	}
	

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);
	return 0;

}

int BsCarInfoMonitorC::_ReadCarDataFromDB()
{
	int ret = 0;
	int retvalue = -1;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pSqlSel[200];
	sprintf(pSqlSel , "select * from CarDataList");
	
	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_open error: %s\n", sqlite3_errmsg(pDB));
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
	
	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt,( const char**)&pError );
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_prepare error: %s\n", sqlite3_errmsg(pDB));
		sqlite3_close(pDB);
		return -1;
	}

	ret = sqlite3_step(pStmt);
	while( ret==SQLITE_ROW )
	{
		int id = sqlite3_column_int(pStmt, 0);
		int value = sqlite3_column_int(pStmt, 1);
		if( id > 0 && id < CAR_INFO_TYPE_NUMBER )
		{
			if( CAR_INFO_DATA_INT == m_CarDataStatus[id].dataType )
			{
				m_CarDataStatus[id].intValue = value;
			}
			else if( CAR_INFO_DATA_BOOL == m_CarDataStatus[id].dataType )
			{
				m_CarDataStatus[id].boolValue = (bool)value;
			}

			m_CarDataStatus[id].bReadFromDatabase = true;
		}
		ret = sqlite3_step(pStmt);
	}

	sqlite3_finalize(pStmt);
	sqlite3_close(pDB);

	return 0;
}

int BsCarInfoMonitorC::_WriteCarDataToDB()
{
	int ret = 0;
	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	char *pError = NULL;
	char pSqlSel[200];
	
	ret = sqlite3_open(INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY, &pDB);
	if (ret != SQLITE_OK)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "sqlite3_open error: %s\n", sqlite3_errmsg(pDB));
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
	
	sprintf( pSqlSel , "delete from CarDataList");

	ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt,( const char**)&pError );
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

	for(int i=0; i<CAR_INFO_TYPE_NUMBER; i++)
	{
		if( CAR_INFO_IS_USED(i) )
		{
			int value;
			if( CAR_INFO_DATA_INT == m_CarDataStatus[i].dataType )
			{
				value = m_CarDataStatus[i].intValue;
			}
			else if( CAR_INFO_DATA_BOOL == m_CarDataStatus[i].dataType )
			{
				value = (int)(m_CarDataStatus[i].boolValue);
			}
			sprintf(pSqlSel, "insert into CarDataList values('%d', '%d')", i, value);

			ret = sqlite3_prepare(pDB, pSqlSel, strlen(pSqlSel), &pStmt,( const char**)&pError );
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

	sqlite3_close(pDB);

	return 0;
}

BsCarInfoMonitorC  *BsCarInfoMonitorC::thisObject = NULL;
BsCarInfoMonitorC  *BsCarInfoMonitorC::object()
{
	BsCarInfoMonitorC *pTmp = NULL;
		
	bsManagerObjectLock();
	
	if( !thisObject )
	{
		thisObject = new BsCarInfoMonitorC();
	}

	pTmp = thisObject;
	bsManagerObjectUnLock();
	
	return pTmp;
}

void BsCarInfoMonitorC::releaseObject()
{
	bsManagerObjectLock();

	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
	bsManagerObjectUnLock();
}
BsCarInfoMonitorC *BsCarInfoMonitorC::getObject()
{
	BsCarInfoMonitorC *pTmp = NULL;
	
	bsManagerObjectLock();
	
	pTmp = thisObject;

	bsManagerObjectUnLock();

	return pTmp;
}

BsCarInfoMonitorC::BsCarInfoMonitorC()
	: m_pCarDataUpload(NULL),
	  m_pCarDataGather(NULL),
	  m_pOBDDataGather(NULL),
	  m_bCarDataChanage(false),
	  m_CarDataUploadMode(CAR_DATA_UPLOAD_MODE_ABNORMAL),
	  m_iTimerCarDataUpload(-1),
	  m_iTimerCarDataUploadElapseTime(0),
	  m_iTimerAllDoorOpenTimeout(-1),
	  m_iTimerCheckMaintenance(-1),
	  m_iTimerCheckMaintenanceElapseTime(600*1000),
	  m_iTimerCheckTipsOnCreate(-1),
	  m_iTimerCheckTipsOnCreateElapseTime(10*1000),
	  m_iTimerSaveCarData(-1),
	  m_iTimerSaveCarDataElapseTime(1800*1000),
	  m_bStartCarInfoMonitor(false)
{
	m_iTimerCheckTipsOnCreate = startTimer(m_iTimerCheckTipsOnCreateElapseTime);
	m_iTimerSaveCarData = startTimer(m_iTimerSaveCarDataElapseTime);
	m_iTimerCheckMaintenance = startTimer(m_iTimerCheckMaintenanceElapseTime);
	
	m_pCarDataUpload = new CarDataUploadC();
	m_pCarDataGather = new CarDataGatherC(this);
	m_pOBDDataGather = new OBDDataGatherC(this);

	m_pCarDataGather->getUploadMode(m_CarDataUploadMode, m_iTimerCarDataUploadElapseTime);
	if( m_CarDataUploadMode >= CAR_DATA_UPLOAD_MODE_NUMBER ||
		m_CarDataUploadMode < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"car data upload mode error, m_CarDataUploadMode[%d]\n", m_CarDataUploadMode)
		abort();
	}

	if( m_CarDataUploadMode == CAR_DATA_UPLOAD_MODE_TIMER ||
		m_CarDataUploadMode == CAR_DATA_UPLOAD_MODE_ABNORMAL_TIMER )
	{
		if( m_iTimerCarDataUploadElapseTime <= 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION,"car data upload time error, m_iTimerCarDataUploadElapseTime[%d]\n", m_iTimerCarDataUploadElapseTime)
			abort();
		}

		m_iTimerCarDataUpload = startTimer(m_iTimerCarDataUploadElapseTime);
	}

	m_CarInfoList.clear();
	
	if( _readCarInfoFromDatabase(&m_CarInfoList) < 0 || m_CarInfoList.size() == 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"read car info from database error\n")
		m_CarInfoList.clear();
		if( m_pCarDataGather->getValidCarInfo(m_CarInfoList) <0)
		{
			IBD_PRINTF(DEBUG_INFOMATION,"getValidCarInfo fail\n")
			abort();
		}
	}

	_initCarData();

}

void BsCarInfoMonitorC::_checkMaintenanceOnMileage()
{
	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_NEED_MAINTENANCE) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME) )
	{
		int mileageMaintenance = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST].intValue;
		int mileageInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE].intValue;
		mileageInterval *= m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE].intValue;
		int mileageCurrent = m_CarDataStatus[CAR_INFO_TYPE_MILEAGE].intValue;
		int miniInterval = m_CarDataStatus[CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL].intValue;
		bool bNotTipThisTime = m_CarDataStatus[CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME].boolValue;

		m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE].boolValue = false;
		if( (mileageCurrent-mileageMaintenance) > mileageInterval )
		{
			m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE].boolValue = true;
			struct timeval tv;
			struct timezone tz;
			gettimeofday(&tv, &tz);
			int iCurrentTime = tv.tv_sec;
			if( iCurrentTime > m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT].intValue )
			{
				if( bNotTipThisTime )
				{
					mileageInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE].intValue;
					int times = (mileageCurrent-mileageMaintenance+mileageInterval)/mileageInterval;
					m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE].intValue = times;
				}
									
				m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST].intValue = iCurrentTime;
				m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT].intValue = iCurrentTime+miniInterval;
				emit sigCarDataFeedback(m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE]);
			}
		}
	}
}

void BsCarInfoMonitorC::_checkMaintenanceOnMonth()
{
	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_NEED_MAINTENANCE) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME) )
	{
		int timeMaintenance = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST].intValue;
		int timeInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH].intValue;
		timeInterval = timeInterval * 60 * 60 * 24 * 30;
		int miniInterval = m_CarDataStatus[CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL].intValue;
		bool bNotTipThisTime = m_CarDataStatus[CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME].boolValue;
		timeInterval *= m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH].intValue;

		struct timeval tv;
		struct timezone tz;
		gettimeofday(&tv, &tz);
		int iCurrentTime = tv.tv_sec;

		m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE].boolValue = false;
		
		if( timeMaintenance>0 && iCurrentTime > (timeMaintenance+timeInterval) )
		{
			m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE].boolValue = true;
			if( iCurrentTime > m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT].intValue )
			{
				if( bNotTipThisTime )
				{
					timeInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH].intValue;
					int times = (iCurrentTime-timeMaintenance+timeInterval)/timeInterval;
					m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH].intValue = times;
				}
									
				m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST].intValue = iCurrentTime;
				m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT].intValue = iCurrentTime + miniInterval;
				emit sigCarDataFeedback(m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE]);
			}
		}
	}
}
		
void BsCarInfoMonitorC::timerEvent(QTimerEvent * event)
{
	int iTimerId = event->timerId();

	if( m_iTimerCheckTipsOnCreate == iTimerId )
	{
		m_mutexForThreadSafe.lock();
	
		killTimer(m_iTimerCheckTipsOnCreate);
		m_iTimerCheckTipsOnCreate = -1;

		bool bTipsAfterCreate = false;
		
		if( CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_AFTER_CREATE) && m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_AFTER_CREATE].boolValue )
		{
			bTipsAfterCreate = true;
		}

		if( CAR_INFO_IS_USED(CAR_INFO_TYPE_NEED_MAINTENANCE) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME) )
		{
			//check whether tips
			int mileageMaintenance = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST].intValue;
			int mileageInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE].intValue;
			int mileageCurrent = m_CarDataStatus[CAR_INFO_TYPE_MILEAGE].intValue;
			int miniInterval = m_CarDataStatus[CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL].intValue;
			bool bNotTipThisTime = m_CarDataStatus[CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME].boolValue;
			if( bNotTipThisTime && bTipsAfterCreate )
			{

			}
			else
			{
				mileageInterval *= m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE].intValue;
			}
			
			m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE].boolValue = false;
			if( (mileageCurrent-mileageMaintenance) > mileageInterval )
			{
				m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE].boolValue = true;
				struct timeval tv;
				struct timezone tz;
				gettimeofday(&tv, &tz);
				int iCurrentTime = tv.tv_sec;
				if( iCurrentTime > m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT].intValue )
				{
					if( bNotTipThisTime )
					{
						mileageInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE].intValue;
						int times = (mileageCurrent-mileageMaintenance+mileageInterval)/mileageInterval;
						m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE].intValue = times;
					}
					
					m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST].intValue = iCurrentTime;
					m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT].intValue = iCurrentTime+miniInterval;
					emit sigCarDataFeedback(m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE]);
				}
			}
		}

		if( CAR_INFO_IS_USED(CAR_INFO_TYPE_NEED_MAINTENANCE) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME) )
		{
			int timeMaintenance = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST].intValue;
			int timeInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH].intValue;
			timeInterval = timeInterval * 60 * 60 * 24 * 30;
			int miniInterval = m_CarDataStatus[CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL].intValue;
			bool bNotTipThisTime = m_CarDataStatus[CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME].boolValue;
			if( bNotTipThisTime && bTipsAfterCreate )
			{

			}
			else
			{
				timeInterval *= m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH].intValue;
			}

			struct timeval tv;
			struct timezone tz;
			gettimeofday(&tv, &tz);
			int iCurrentTime = tv.tv_sec;

			m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE].boolValue = false;
			
			if( timeMaintenance>0 && iCurrentTime > (timeMaintenance+timeInterval) )
			{
				m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE].boolValue = true;
				if( iCurrentTime > m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT].intValue )
				{
					if( bNotTipThisTime )
					{
						timeInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH].intValue;
						int times = (iCurrentTime-timeMaintenance+timeInterval)/timeInterval;
						m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH].intValue = times;
					}
										
					m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST].intValue = iCurrentTime;
					m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT].intValue = iCurrentTime + miniInterval;
					emit sigCarDataFeedback(m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE]);
				}
			}
		}

		m_mutexForThreadSafe.unlock();
	}
	else if( m_iTimerCarDataUpload == iTimerId )
	{
		_uploadCarData(2);
	}
	else if( m_iTimerSaveCarData == iTimerId )
	{
		m_mutexForThreadSafe.lock();
		_WriteCarDataToDB();
		m_mutexForThreadSafe.unlock();
	}
	else if( m_iTimerAllDoorOpenTimeout == iTimerId )
	{
		m_mutexForThreadSafe.lock();

		killTimer(m_iTimerAllDoorOpenTimeout);
		m_iTimerAllDoorOpenTimeout = -1;
		
		if( CAR_INFO_IS_USED(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT) &&
			CAR_INFO_IS_USED(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT_TIME) )
		{
			if( m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN].boolValue )
			{
				m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT].boolValue = true;
				emit sigCarDataFeedback(m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT]);
			}
		}
		
		m_mutexForThreadSafe.unlock();
	}
      else if( m_iTimerCheckMaintenance == iTimerId )
	{
		m_mutexForThreadSafe.lock();

		if( -1 == m_iTimerCheckTipsOnCreate )
		{
			_checkMaintenanceOnMileage();
			_checkMaintenanceOnMonth();
		}
		
		m_mutexForThreadSafe.unlock();
	}
	
}

BsCarInfoMonitorC::~BsCarInfoMonitorC()
{
	m_mutexForThreadSafe.lock();

	cancelOperation();

	if( !m_pCarDataUpload )
	{
		delete m_pCarDataUpload;
		m_pCarDataUpload = NULL;
	}

	if( !m_pCarDataGather )
	{
		delete m_pCarDataGather;
		m_pCarDataGather = NULL;
	}

	if( !m_pOBDDataGather )
	{
		delete m_pOBDDataGather;
		m_pOBDDataGather = NULL;
	}

	if( -1 != m_iTimerCarDataUpload )
	{
		killTimer(m_iTimerCarDataUpload);
		m_iTimerCarDataUpload = -1;
	}

	if( -1 != m_iTimerAllDoorOpenTimeout )
	{
		killTimer(m_iTimerAllDoorOpenTimeout);
		m_iTimerAllDoorOpenTimeout = -1;
	}

	if( -1 != m_iTimerCheckMaintenance )
	{
		killTimer(m_iTimerCheckMaintenance);
		m_iTimerCheckMaintenance = -1;
	}

	if( -1 != m_iTimerCheckTipsOnCreate )
	{
		killTimer(m_iTimerCheckTipsOnCreate);
		m_iTimerCheckTipsOnCreate = -1;
	}

	if( -1 != m_iTimerSaveCarData )
	{
		killTimer(m_iTimerSaveCarData);
		m_iTimerSaveCarData = -1;
	}

	_WriteCarDataToDB();
		
	m_mutexForThreadSafe.unlock();
}

void	BsCarInfoMonitorC::cancelOperation()
{
	BsBaseC::stopHttp();
}

int BsCarInfoMonitorC::getCurrentCarInfo(CarInfoList &typeList)
{
	m_mutexForThreadSafe.lock();

	typeList = m_CarInfoList;
	
	m_mutexForThreadSafe.unlock();	

	return BS_E_OK;
}

int BsCarInfoMonitorC::cancelCarInfo(CAR_INFO_TYPE type)
{
	m_mutexForThreadSafe.lock();

      	m_CarInfoList.removeOne(type);

	_WriteCarDataToDB();;

	_initCarData();

	_saveCarInfoToDatabase(&m_CarInfoList);

	m_mutexForThreadSafe.unlock();	

	return BS_E_OK;
}

int BsCarInfoMonitorC::addCarInfo(CAR_INFO_TYPE type)
{

	m_mutexForThreadSafe.lock();

	CarInfoList carInfoList;
	if( m_pCarDataGather->getValidCarInfo(carInfoList) <0)
	{
		IBD_PRINTF(DEBUG_INFOMATION,"getValidCarInfo fail\n")
		abort();
	}

	bool bFound = false;
	for(int i=0; i<carInfoList.size(); i++)
	{
		if(carInfoList.at(i) == type)
		{
			bFound = true;
			break;
		}
	}
	
	if( !bFound )
	{
		m_mutexForThreadSafe.unlock();	
		return BS_CAR_INFO_MONITOR_E_CAR_INFO_NOT_SUPPORT;
	}

	for(int i =0;i<m_CarInfoList.size();i++)
	{
		if(m_CarInfoList.at(i)==type)
		{
			m_mutexForThreadSafe.unlock();	
			return BS_CAR_INFO_MONITOR_E_ADD_SAME_CAR_INFO;
		}
	}
	
	m_CarInfoList.append(type);

	_WriteCarDataToDB();

	_initCarData();

	_saveCarInfoToDatabase(&m_CarInfoList);

	m_mutexForThreadSafe.unlock();	

	return BS_E_OK;
}

int BsCarInfoMonitorC::resumeDefaultCarInfo()
{
	m_mutexForThreadSafe.lock();

	m_CarInfoList.clear();
	if( m_pCarDataGather->getValidCarInfo(m_CarInfoList) <0)
	{
		IBD_PRINTF(DEBUG_INFOMATION,"getValidCarInfo fail\n")
		abort();
	}

	_WriteCarDataToDB();

	_initCarData();

	_saveCarInfoToDatabase(&m_CarInfoList);

	m_mutexForThreadSafe.unlock();	

	return BS_E_OK;
}

int BsCarInfoMonitorC::resumeDefaultCarData(CAR_INFO_TYPE type)
{
	int retval = BS_E_OK;

	if( !CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE) )
	{
		retval = BS_CAR_INFO_MONITOR_E_CAR_DATA_UNUSED;
		return retval;
	}
	
	m_mutexForThreadSafe.lock();

	switch( type )
	{
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE:
			m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE].intValue = 5000000;
			m_pCarDataGather->getCarData(m_CarDataStatus[type]);
			break;
			
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH:
			m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH].intValue = 5;
                   m_pCarDataGather->getCarData(m_CarDataStatus[type]);
			break;

		case CAR_INFO_TYPE_MAINTENANCE_TIPS_AFTER_CREATE:
			m_CarDataStatus[type].boolValue = false;
                   m_pCarDataGather->getCarData(m_CarDataStatus[type]);
			break;
			
		default:
			retval = BS_CAR_INFO_MONITOR_E_CAR_DATA_UNSUPPORT_SET;
			break;
	}

	m_mutexForThreadSafe.unlock();

	return retval;
}

int BsCarInfoMonitorC::startOBDMonitor()
{
	m_mutexForThreadSafe.lock();

	m_pOBDDataGather->startGather();
	
	m_mutexForThreadSafe.unlock();	

	return BS_E_OK;
}

int BsCarInfoMonitorC::stopOBDMonitor()
{
	m_mutexForThreadSafe.lock();

	m_pOBDDataGather->stopGather();
	
	m_mutexForThreadSafe.unlock();	

	return BS_E_OK;
}

#define CHECK_CAR_DATA_TYPE(a,	b)	if(a!=b){IBD_PRINTF(DEBUG_INFOMATION,"car data type error!\n");abort();}

void BsCarInfoMonitorC::_checkReportedCarData(CAR_DATA data)
{
	switch(data.type)
	{
		case CAR_INFO_TYPE_AIRBAG_IS_OPEN:
		case CAR_INFO_TYPE_ALL_DOOR_IS_OPEN:
		case CAR_INFO_TYPE_DOOR_IS_ILLEGAL:
		case CAR_INFO_TYPE_LEFT_FRONT_DOOR_IS_OPEN:
		case CAR_INFO_TYPE_LEFT_BEHIND_DOOR_IS_OPEN:
		case CAR_INFO_TYPE_RIGHT_FRONT_DOOR_IS_OPEN:
		case CAR_INFO_TYPE_RIGHT_BEHIND_DOOR_IS_OPEN:
		case CAR_INFO_TYPE_TRUNK_DOOR_IS_OPEN:
		case CAR_INFO_TYPE_LEFT_FRONT_DOOR_IS_LOCK:
		case CAR_INFO_TYPE_LEFT_BEHIND_DOOR_IS_LOCK:
		case CAR_INFO_TYPE_RIGHT_FRONT_DOOR_IS_LOCK:
		case CAR_INFO_TYPE_RIGHT_BEHIND_DOOR_IS_LOCK:
		case CAR_INFO_TYPE_TRUNK_DOOR_IS_LOCK:
			CHECK_CAR_DATA_TYPE(data.dataType, CAR_INFO_DATA_BOOL)
			break;

		case CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT:
		case CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_ON_RUNNING:
		case CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH:
		case CAR_INFO_TYPE_WATER_TEMPERATURE_TOP_LIMITED:
		case CAR_INFO_TYPE_REMAINING_FUEL_IS_TOO_LOW:
		case CAR_INFO_TYPE_REMAINING_FUEL_BOTTOM_LIMITED:
		case CAR_INFO_TYPE_NEED_MAINTENANCE:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH:
		case CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST:
		case CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_AFTER_CREATE:
		case CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME:
		case CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL:
			IBD_PRINTF(DEBUG_INFOMATION,"this car data needn't be reported   type[%d]\n", data.type);
			abort();
			break;

		case CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT_TIME:
		case CAR_INFO_TYPE_SPEED:
		case CAR_INFO_TYPE_REMAINING_FUEL:
		case CAR_INFO_TYPE_MILEAGE:
		case  CAR_INFO_TYPE_MILEAGE_INCREMENT:
		case CAR_INFO_TYPE_WATER_TEMPERATURE:
		case CAR_INFO_TYPE_OUT_TEMPERATURE:
			CHECK_CAR_DATA_TYPE(data.dataType, CAR_INFO_DATA_INT)
			break;

		default:
			printf("can not come here!!!\n");
			abort();
	}
}

int BsCarInfoMonitorC::reportCarData(CAR_DATA data)
{
	bool bNeedSendSig = false;

	_checkReportedCarData(data);

	m_mutexForThreadSafe.lock();

	if( !m_bStartCarInfoMonitor )
	{
		m_mutexForThreadSafe.unlock();
		return 0;
	}
	
	for(int i=0; i<m_CarInfoList.size(); i++)
	{
		if(m_CarInfoList.at(i) == data.type)
		{//this car info can be supported
			if( !CAR_INFO_IS_USED(data.type) ) //haven't been initializated
			{
				bNeedSendSig = true;
				m_bCarDataChanage = true;
				m_CarDataStatus[data.type] = data;
			}
			else
			{
				if( CAR_INFO_DATA_BOOL == m_CarDataStatus[data.type].dataType )
				{
					if( m_CarDataStatus[data.type].boolValue != data.boolValue )
					{
						bNeedSendSig = true;
						m_bCarDataChanage = true;
						m_CarDataStatus[data.type] = data;
					}
				}
				else if( CAR_INFO_DATA_INT == m_CarDataStatus[data.type].dataType )
				{
					if( CAR_INFO_TYPE_MILEAGE_INCREMENT == data.type )
					{
						if( CAR_INFO_IS_USED(CAR_INFO_TYPE_MILEAGE) )
						{
							bNeedSendSig = true;
							m_bCarDataChanage = true;
							m_CarDataStatus[CAR_INFO_TYPE_MILEAGE].intValue += data.intValue;
                                             data.type = CAR_INFO_TYPE_MILEAGE;
						}
					}
					else
					{
						if( m_CarDataStatus[data.type].intValue != data.intValue )
						{
							bNeedSendSig = true;
							m_bCarDataChanage = true;
							m_CarDataStatus[data.type] = data;
						}					
					}
				}
				else
				{

				}
			}
		}		
	}

	if( bNeedSendSig )
            emit sigCarDataFeedback(data);


	if( bNeedSendSig )
	{
		//process car data of setting
		if( CAR_INFO_TYPE_ALL_DOOR_IS_OPEN == data.type ||
			CAR_INFO_TYPE_SPEED == data.type )
		{
			if( CAR_INFO_IS_USED(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_ON_RUNNING) )
			{
				bool bOpenOnRunning = false;
				if( m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN].boolValue &&
					m_CarDataStatus[CAR_INFO_TYPE_SPEED].intValue > 0 )
				{
					bOpenOnRunning = true;
				}

				if( m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_ON_RUNNING].boolValue != bOpenOnRunning )
				{
					m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_ON_RUNNING].boolValue = bOpenOnRunning;
					emit sigCarDataFeedback(m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_ON_RUNNING]);
				}

			}
		}

		if( CAR_INFO_TYPE_ALL_DOOR_IS_OPEN == data.type )
		{
			if( CAR_INFO_IS_USED(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT) &&
				CAR_INFO_IS_USED(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT_TIME) )
			{
				if( data.boolValue )
				{
					//all door from close to open
					m_iTimerAllDoorOpenTimeout = startTimer(m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT_TIME].intValue*1000);
				}
				else
				{
					//all door from open to close
					if( -1 != m_iTimerAllDoorOpenTimeout )
					{
						killTimer(m_iTimerAllDoorOpenTimeout);
					}
					m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT].boolValue = false;
				}
			}
		}

		if( CAR_INFO_TYPE_WATER_TEMPERATURE == data.type )
		{
			if( CAR_INFO_IS_USED(CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH) &&
				CAR_INFO_IS_USED(CAR_INFO_TYPE_WATER_TEMPERATURE_TOP_LIMITED) )
			{
				bool bTooHigh = false;
				
                                int top = m_CarDataStatus[CAR_INFO_TYPE_WATER_TEMPERATURE_TOP_LIMITED].intValue;
                                int temperature = m_CarDataStatus[CAR_INFO_TYPE_WATER_TEMPERATURE].intValue;
                                if( temperature > top )
					{
                                        bTooHigh = true;
                                }

                                if( m_CarDataStatus[CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH].boolValue != bTooHigh )
                                {
                                        m_CarDataStatus[CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH].boolValue = bTooHigh;
                                        emit sigCarDataFeedback(m_CarDataStatus[CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH]);
                                }
			}
		}

		if( CAR_INFO_TYPE_REMAINING_FUEL == data.type )
		{
			if( CAR_INFO_IS_USED(CAR_INFO_TYPE_REMAINING_FUEL_IS_TOO_LOW) &&
				CAR_INFO_IS_USED(CAR_INFO_TYPE_REMAINING_FUEL_BOTTOM_LIMITED) )
			{
				bool bTooLow = false;
				
				int bottom = m_CarDataStatus[CAR_INFO_TYPE_REMAINING_FUEL_BOTTOM_LIMITED].intValue;
				int fuel = m_CarDataStatus[CAR_INFO_TYPE_REMAINING_FUEL].intValue;

				if( fuel < bottom )
				{
					bTooLow = true;
				}

				if( m_CarDataStatus[CAR_INFO_TYPE_REMAINING_FUEL_IS_TOO_LOW].boolValue != bTooLow )
				{
					m_CarDataStatus[CAR_INFO_TYPE_REMAINING_FUEL_IS_TOO_LOW].boolValue = bTooLow;
					emit sigCarDataFeedback(m_CarDataStatus[CAR_INFO_TYPE_REMAINING_FUEL_IS_TOO_LOW]);
				}
			}
		}

		if( CAR_INFO_TYPE_MILEAGE == data.type )
		{
			if( -1 == m_iTimerCheckTipsOnCreate )
			{
				_checkMaintenanceOnMileage();
			}
		}

	}

	m_mutexForThreadSafe.unlock();	

	return 0;
}

int BsCarInfoMonitorC::_getEmergencyStatus()
{
	int iEmergencyStatus = 0;
	
	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_DOOR_IS_ILLEGAL) )
	{
		if( m_CarDataStatus[CAR_INFO_TYPE_DOOR_IS_ILLEGAL].boolValue )
		{
			iEmergencyStatus |= 0x04;
		}
	}
	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_AIRBAG_IS_OPEN) )
	{
		if( m_CarDataStatus[CAR_INFO_TYPE_AIRBAG_IS_OPEN].boolValue )
		{
			iEmergencyStatus |= 0x02;
		}
	}
	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH) )
	{
		if( m_CarDataStatus[CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH].boolValue )
		{
			iEmergencyStatus |= 0x10;
		}
	}
	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT) )
	{
		if( m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT].boolValue )
		{
			iEmergencyStatus |= 0x08;
		}
	}
	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_REMAINING_FUEL_IS_TOO_LOW) )
	{
		if( m_CarDataStatus[CAR_INFO_TYPE_REMAINING_FUEL_IS_TOO_LOW].boolValue )
		{
			iEmergencyStatus |= 0x40;
		}
	}
	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_ON_RUNNING) )
	{
		if( m_CarDataStatus[CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_ON_RUNNING].boolValue )
		{
			iEmergencyStatus |= 0x80;
		}
	}

	/*
		Notice: The emergency which door is unlock out of time, have not been realized,
			    because we don't see it.
	*/

	return iEmergencyStatus;
}

//0--all lock   1--not all lock   -1--unknown
int BsCarInfoMonitorC::_getAllDoorLockStatus()
{
	int retval;
	
	int iDoorNum = 0;
	int iLockDoorNum = 0;

	CAR_INFO_TYPE   doorLockInfo[] = {	CAR_INFO_TYPE_LEFT_FRONT_DOOR_IS_LOCK, \
										CAR_INFO_TYPE_LEFT_BEHIND_DOOR_IS_LOCK, \
										CAR_INFO_TYPE_RIGHT_FRONT_DOOR_IS_LOCK, \
										CAR_INFO_TYPE_RIGHT_BEHIND_DOOR_IS_LOCK, \
										CAR_INFO_TYPE_TRUNK_DOOR_IS_LOCK \
								     	};
	int iDoorLockInfoNum =sizeof (doorLockInfo)/sizeof(CAR_INFO_TYPE_LEFT_FRONT_DOOR_IS_LOCK);
	
	for(int i=0; i<iDoorLockInfoNum; i++)
	{
		CAR_INFO_TYPE type = doorLockInfo[i];
		if( m_CarDataStatus[type].type == type )
		{//this car info is supported
			iDoorNum++;
			if( m_CarDataStatus[type].boolValue )
			{
				iLockDoorNum++;
			}
		}
	}
	
	if( iDoorLockInfoNum == iDoorNum )
	{
		if( iDoorLockInfoNum == iLockDoorNum )
		{
			retval = 0;
		}
		else
		{
			retval = 1;
		}
	}
	else
	{
		retval = -1;
	}

	return retval;
}

int BsCarInfoMonitorC::uploadCarData()
{
	return _uploadCarData(0);
}
	
//whereFrom   0--from uploadCarData   1--from forceUploadCarInfo   2--from timerEvent
int BsCarInfoMonitorC::_uploadCarData(int whereFrom)
{
        m_mutexForThreadSafe.lock();

	if( !m_bStartCarInfoMonitor )
	{
		m_mutexForThreadSafe.unlock();
		return 0;
	}

	if( 1 == whereFrom )
	{
		if( !m_bCarDataChanage )
		{
			m_mutexForThreadSafe.unlock();
			return 0;
		}
	}
	
	CarDataDownloadTask m_task;
	char tuid[BK_TUID_LENGTH_MAX+1] = {0};
	char uuid[BK_UUID_LENGTH_MAX+1] = {0};
	char password[BK_PASSWORD_LENGTH_MAX+1] = {0};
	char accessToken[BK_TOKEN_LENGTH_MAX+1] = {0};
	bool bLogin;
	bool bSavePassword;
	char ucsApiHost[URL_MAX_LENGTH] ={0};

	int retval = BsBaseC::getLoginResult(tuid, uuid, password, accessToken, bLogin, bSavePassword);
	if( retval < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"getLoginResult fail\n")
		m_mutexForThreadSafe.unlock();	
		return retval;
	}

	if( 0 == strcmp(tuid, "") )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"tuid is void\n")
		m_mutexForThreadSafe.unlock();	
		return  BS_E_GET_TUID_ERROR;
	}

	if( 0 == strcmp(uuid, "") )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"uuid is void\n")
		m_mutexForThreadSafe.unlock();	
		return  BS_E_GET_UUID_ERROR;
	}

	if( !bLogin )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"user not login\n")
		m_mutexForThreadSafe.unlock();	
		return  BS_E_USER_NOT_LOGIN;
	}

	retval = BsBaseC::getUcsApiHost(ucsApiHost,uuid);
	if( retval < 0 )
	{
		IBD_PRINTF(DEBUG_INFOMATION,"getUcsApiHost error\n")
		m_mutexForThreadSafe.unlock();	
		return retval;
	}
	
	char upLoadUrl[URL_MAX_LENGTH] = {""};
	char m_upPostfield[URL_MAX_LENGTH] = {""};
      	char m_upCookie[URL_MAX_LENGTH] = {""};
	char buffer[200];
	int tmp;

	int iEmergencyStatus = _getEmergencyStatus();
	sprintf(buffer, "emergency_status=%d&", iEmergencyStatus);
	strcat(m_upPostfield, buffer);

	if( 0 == whereFrom )
	{
		if( m_CarDataUploadMode == CAR_DATA_UPLOAD_MODE_ABNORMAL_TIMER ||
			m_CarDataUploadMode == CAR_DATA_UPLOAD_MODE_ABNORMAL )
		{
			if( m_iEmergencyStatus == iEmergencyStatus )
			{
				m_mutexForThreadSafe.unlock();			
				return 0;
			}
			else
			{
				IBD_PRINTF(DEBUG_INFOMATION,"car data abnormal status have changed, iEmergencyStatus[%x]\n", iEmergencyStatus)
			}
		}
	}

	m_iEmergencyStatus = iEmergencyStatus;
	
	for(int i=0; i<CAR_INFO_TYPE_NUMBER; i++)
	{
		switch(m_CarDataStatus[i].type)
		{
			case CAR_INFO_TYPE_AIRBAG_IS_OPEN:
				if( CAR_INFO_DATA_BOOL == m_CarDataStatus[i].dataType )
				{
					if(m_CarDataStatus[i].boolValue)
						strcat(m_upPostfield, "airbag_status=1&");
					else
						strcat(m_upPostfield, "airbag_status=0&");
				}
				break;
			case CAR_INFO_TYPE_ALL_DOOR_IS_OPEN:
				if(m_CarDataStatus[i].boolValue)
				{
					strcat(m_upPostfield, "door_status=2&");
				}
				else
				{
					tmp = _getAllDoorLockStatus();
					if( 0 == tmp )
					{
						strcat(m_upPostfield, "door_status=0&");
					}
					else if( 1 == tmp )
					{
						strcat(m_upPostfield, "door_status=1&");
					}
					else if( -1 == tmp )
					{
						strcat(m_upPostfield, "door_status=3&");
					}
					else
					{
						//impossible come here
					}
					
				}
				break;

			case CAR_INFO_TYPE_LEFT_FRONT_DOOR_IS_OPEN:
				if(m_CarDataStatus[i].boolValue)
				{
					strcat(m_upPostfield, "driver_door_status=2&");
				}
				else
				{
					if(m_CarDataStatus[CAR_INFO_TYPE_LEFT_FRONT_DOOR_IS_LOCK].type==CAR_INFO_TYPE_LEFT_FRONT_DOOR_IS_LOCK)
					{
                                                if( m_CarDataStatus[CAR_INFO_TYPE_LEFT_FRONT_DOOR_IS_LOCK].boolValue )
						{
							strcat(m_upPostfield, "driver_door_status=0&");
						}
						else
						{
							strcat(m_upPostfield, "driver_door_status=1&");
						}
					}
					else
					{
						strcat(m_upPostfield, "driver_door_status=3&");
					}
				}	
				break;

			case CAR_INFO_TYPE_LEFT_BEHIND_DOOR_IS_OPEN:
				if(m_CarDataStatus[i].boolValue)
				{
					strcat(m_upPostfield, "rear_left_door_status=2&");
				}
				else
				{
					if(m_CarDataStatus[CAR_INFO_TYPE_LEFT_BEHIND_DOOR_IS_LOCK].type==CAR_INFO_TYPE_LEFT_BEHIND_DOOR_IS_LOCK)
					{
						if( m_CarDataStatus[CAR_INFO_TYPE_LEFT_BEHIND_DOOR_IS_LOCK].boolValue )
						{
							strcat(m_upPostfield, "rear_left_door_status=0&");
						}
						else
						{
							strcat(m_upPostfield, "rear_left_door_status=1&");
						}
					}
					else
					{
						strcat(m_upPostfield, "rear_left_door_status=3&");
					}
				}	
				break;

			case CAR_INFO_TYPE_RIGHT_FRONT_DOOR_IS_OPEN:
				if(m_CarDataStatus[i].boolValue)
				{
					strcat(m_upPostfield, "copilot_door_status=2&");
				}
				else
				{
					if(m_CarDataStatus[CAR_INFO_TYPE_RIGHT_FRONT_DOOR_IS_LOCK].type==CAR_INFO_TYPE_RIGHT_FRONT_DOOR_IS_LOCK)
					{
						if( m_CarDataStatus[CAR_INFO_TYPE_RIGHT_FRONT_DOOR_IS_LOCK].boolValue )
						{
							strcat(m_upPostfield, "copilot_door_status=0&");
						}
						else
						{
							strcat(m_upPostfield, "copilot_door_status=1&");
						}
					}
					else
					{
						strcat(m_upPostfield, "copilot_door_status=3&");
					}
				}	
				break;

			case CAR_INFO_TYPE_RIGHT_BEHIND_DOOR_IS_OPEN:
				if(m_CarDataStatus[i].boolValue)
				{
					strcat(m_upPostfield, "rear_right_door_status=2&");
				}
				else
				{
					if(m_CarDataStatus[CAR_INFO_TYPE_RIGHT_BEHIND_DOOR_IS_LOCK].type==CAR_INFO_TYPE_RIGHT_BEHIND_DOOR_IS_LOCK)
					{
						if( m_CarDataStatus[CAR_INFO_TYPE_RIGHT_BEHIND_DOOR_IS_LOCK].boolValue )
						{
							strcat(m_upPostfield, "rear_right_door_status=0&");
						}
						else
						{
							strcat(m_upPostfield, "rear_right_door_status=1&");
						}
					}
					else
					{
						strcat(m_upPostfield, "rear_right_door_status=3&");
					}
				}	
				break;

			case CAR_INFO_TYPE_TRUNK_DOOR_IS_OPEN:
				if(m_CarDataStatus[i].boolValue)
				{
					strcat(m_upPostfield, "driver_door_status=2&");
				}
				else
				{
					if(m_CarDataStatus[CAR_INFO_TYPE_TRUNK_DOOR_IS_LOCK].type==CAR_INFO_TYPE_TRUNK_DOOR_IS_LOCK)
					{
						if( m_CarDataStatus[CAR_INFO_TYPE_TRUNK_DOOR_IS_LOCK].boolValue )
						{
							strcat(m_upPostfield, "trunk_status=0&");
						}
						else
						{
							strcat(m_upPostfield, "trunk_status=1&");
						}
					}
					else
					{
						strcat(m_upPostfield, "trunk_status=3&");
					}
				}	
				break;

			case CAR_INFO_TYPE_SPEED:
				sprintf(buffer, "speed=%d&",m_CarDataStatus[i].intValue);	
			 	strcat(m_upPostfield, buffer);
				break;

			case CAR_INFO_TYPE_WATER_TEMPERATURE:
				sprintf(buffer, "engine_water_temp=%.1f&",((float)(m_CarDataStatus[i].intValue))/100.0);
				strcat(m_upPostfield, buffer);
				break;

			case CAR_INFO_TYPE_MILEAGE:
				sprintf(buffer, "mileage=%d&",m_CarDataStatus[i].intValue ); 
				strcat(m_upPostfield, buffer); 
				break;

			case CAR_INFO_TYPE_REMAINING_FUEL:
				sprintf(buffer, "remaining_fuel=%d&",m_CarDataStatus[i].intValue);
				strcat(m_upPostfield, buffer);
				break;

			case CAR_INFO_TYPE_OUT_TEMPERATURE:
				break;
				
			default:
				break;
		}
	}

	if( '&' == m_upPostfield[strlen(m_upPostfield)-1] )
	{
		m_upPostfield[strlen(m_upPostfield)-1] = 0;
	}
	
	printf("*****get m_upPostfield**** [%s]\n",m_upPostfield);
	sprintf(upLoadUrl, "http://%s/api/1.0/user/%s/veh/condition/%s/", ucsApiHost, uuid, tuid);
	sprintf(m_upCookie, "customer_key=%s;access_token=%s", tuid, accessToken);
	m_task.url = upLoadUrl;
	m_task.postfield = m_upPostfield;
	m_task.cookie = m_upCookie;
	if(m_pCarDataUpload->uploadCarData(m_task)<0)
	{	
		IBD_PRINTF(DEBUG_INFOMATION,"uploadCarData fail!\n")
	}	
	
	m_bCarDataChanage = false;

	m_mutexForThreadSafe.unlock();	

	return 0;
}

int BsCarInfoMonitorC::feedbackOBDCheckResult(OBD_CHECK_RESULT result)
{
	emit sigOBDCheckResult(result);
	return BS_E_OK;
}

int BsCarInfoMonitorC::forceUploadCarInfo()
{
	return _uploadCarData(1);
}

int BsCarInfoMonitorC::startCarInfoMonitor()
{
	m_mutexForThreadSafe.lock();

	m_bStartCarInfoMonitor = true;
	
	m_mutexForThreadSafe.unlock();
	
	return BS_E_OK;	
}

int BsCarInfoMonitorC::stopCarInfoMonitor()
{
	m_mutexForThreadSafe.lock();

	m_bStartCarInfoMonitor = false;
	
	m_mutexForThreadSafe.unlock();
	
	return BS_E_OK;	
}

int BsCarInfoMonitorC::retrieveCarData()
{
	CAR_DATA data;
	
	m_mutexForThreadSafe.lock();

	if( !m_bStartCarInfoMonitor )
	{
		m_mutexForThreadSafe.unlock();
		return BS_CAR_INFO_MONITOR_E_CAR_MONITOR_NOT_START;
	}
	
	for(int i=0; i<m_CarInfoList.size(); i++)
	{
		data = m_CarDataStatus[m_CarInfoList.at(i)];
		if(data.type==m_CarInfoList.at(i))
		{
			emit sigCarDataFeedback(data);
		}
	}

	m_mutexForThreadSafe.unlock();
	
	return BS_E_OK;	
}

void BsCarInfoMonitorC::_initCarData()
{
	struct timeval tv;
	struct timezone tz;
	int iCurrentTime;


	for(int i=0; i<CAR_INFO_TYPE_NUMBER; i++)
	{
		m_CarDataStatus[i].type = CAR_INFO_TYPE_INVALID;
		m_CarDataStatus[i].bReadFromDatabase = false;
	}

	_ReadCarDataFromDB();

    for(int i=0; i<m_CarInfoList.size(); i++)
    {
            CAR_INFO_TYPE type = m_CarInfoList.at(i);
            switch( (int)type )
            {
                    case CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT:
                    case CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_ON_RUNNING:
                    case CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH:
                    case CAR_INFO_TYPE_REMAINING_FUEL_IS_TOO_LOW:
                    case CAR_INFO_TYPE_NEED_MAINTENANCE:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_BOOL;
                            m_CarDataStatus[type].boolValue = false;
                            break;
							
                    case CAR_INFO_TYPE_MAINTENANCE_TIPS_AFTER_CREATE:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_BOOL;
                            if( !(m_CarDataStatus[i].bReadFromDatabase) )
                            {
					m_CarDataStatus[type].boolValue = false;
					m_pCarDataGather->getCarData(m_CarDataStatus[type]);
                            }
                            break;

                    case CAR_INFO_TYPE_MILEAGE:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
                            if( !(m_CarDataStatus[i].bReadFromDatabase) )
					m_CarDataStatus[type].intValue = 0;
                            break;

                    case CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
                            if( !(m_CarDataStatus[i].bReadFromDatabase) )
                            {
					m_CarDataStatus[type].intValue = 5000000;
					m_pCarDataGather->getCarData(m_CarDataStatus[type]);
                            }
                            break;
							
			case CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE:			
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
                            if( !(m_CarDataStatus[i].bReadFromDatabase) )
					m_CarDataStatus[type].intValue = 1;
                            break;

			case CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH:			
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
                            if( !(m_CarDataStatus[i].bReadFromDatabase) )
                            {
					m_CarDataStatus[type].intValue = 1;
					m_pCarDataGather->getCarData(m_CarDataStatus[type]);
                            }
                            break;
							
                    case CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
                            if( !(m_CarDataStatus[i].bReadFromDatabase) )
                            {
					m_CarDataStatus[type].intValue = 5;
                            }
                            break;

                    case CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
                            if( !(m_CarDataStatus[i].bReadFromDatabase) )
                            {
					m_CarDataStatus[type].intValue = 0;
                            }
                            break;

                    case CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
                            if( !(m_CarDataStatus[i].bReadFromDatabase) )
                            {
					m_CarDataStatus[type].intValue = 0;
                            }
                            break;

                    case CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
                            if( !(m_CarDataStatus[i].bReadFromDatabase) )
                            {
				      m_CarDataStatus[type].intValue = 0;
                            }
                            break;

                    case CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
                            if( !(m_CarDataStatus[i].bReadFromDatabase) )
                            {
				      m_CarDataStatus[type].intValue = 0;
                            }
                            break;

                    case CAR_INFO_TYPE_WATER_TEMPERATURE_TOP_LIMITED:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
				  m_CarDataStatus[type].intValue = 99999;
                            m_pCarDataGather->getCarData(m_CarDataStatus[type]);
                            break;

                    case CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT_TIME:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
				  m_CarDataStatus[type].intValue = 15;			
                            m_pCarDataGather->getCarData(m_CarDataStatus[type]);
                            break;

                    case CAR_INFO_TYPE_REMAINING_FUEL_BOTTOM_LIMITED:
                            m_CarDataStatus[type].type = type;
                            m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
				  m_CarDataStatus[type].intValue = 0;
                            m_pCarDataGather->getCarData(m_CarDataStatus[type]);
                            break;
			
			case CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME:
				  m_CarDataStatus[type].type = type;
				  m_CarDataStatus[type].dataType = CAR_INFO_DATA_BOOL;
				  m_CarDataStatus[type].boolValue = false;
				  m_pCarDataGather->getCarData(m_CarDataStatus[type]);
				  break;
				  
			case CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL:
				  m_CarDataStatus[type].type = type;
				  m_CarDataStatus[type].dataType = CAR_INFO_DATA_INT;
				  m_CarDataStatus[type].intValue = 30*60;
				  m_pCarDataGather->getCarData(m_CarDataStatus[type]);
				  break;
            }
    }

}

int BsCarInfoMonitorC::nowToMaintain()
{
	bool bSucceed = false;
	
	m_mutexForThreadSafe.lock();
	
	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_MILEAGE) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST) )
	{
		m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST].intValue = m_CarDataStatus[CAR_INFO_TYPE_MILEAGE].intValue;
		bSucceed = true;
	}

	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST) )
	{
		struct timeval tv;
		struct timezone tz;
		gettimeofday(&tv, &tz);
		int iCurrentTime = tv.tv_sec;
		m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST].intValue = iCurrentTime;
		bSucceed = true;
	}

	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE) )
	{
		m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE].intValue = 1;
	}

	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH) )
	{
		m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH].intValue = 1;
	}
	
	m_mutexForThreadSafe.unlock();

	if( bSucceed )
		return BS_E_OK;
	else
		return BS_E_UNSUPPORTED;
}

int BsCarInfoMonitorC::setNextMaintenanceTipsTime(int IntervalTime)
{
	bool bSucceed = false;

	if( IntervalTime <= 0 )
		return BS_E_ARG_INVALID;
	
	m_mutexForThreadSafe.lock();
	
	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT) )
	{
		m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT].intValue = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST].intValue + IntervalTime;
		bSucceed = true;
	}
	
	m_mutexForThreadSafe.unlock();
	
	if( bSucceed )
		return BS_E_OK;
	else
		return BS_E_UNSUPPORTED;
}

int BsCarInfoMonitorC::thisTimeNotTips()
{
	m_mutexForThreadSafe.lock();

	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME) )
	{
		if( m_CarDataStatus[CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME].boolValue )
		{
			m_mutexForThreadSafe.unlock();
	
			return BS_E_OK;			
		}
	}

	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_NEED_MAINTENANCE) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT) && 
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME) )
	{
		int mileageMaintenance = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST].intValue;
		int mileageInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE].intValue;
		mileageInterval *= m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE].intValue;
		int mileageCurrent = m_CarDataStatus[CAR_INFO_TYPE_MILEAGE].intValue;
		
		m_CarDataStatus[CAR_INFO_TYPE_NEED_MAINTENANCE].boolValue = false;
		if( (mileageCurrent-mileageMaintenance) > mileageInterval )
		{
			mileageInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE].intValue;
			int times = (mileageCurrent-mileageMaintenance+mileageInterval)/mileageInterval;
			m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE].intValue = times;
		}
	}

	if( CAR_INFO_IS_USED(CAR_INFO_TYPE_NEED_MAINTENANCE) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL) &&
		CAR_INFO_IS_USED(CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME) )
	{
		int timeMaintenance = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST].intValue;
		int timeInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH].intValue;
		timeInterval *= m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH].intValue;
		timeInterval = timeInterval * 60 * 60 * 24 * 30;
			
		struct timeval tv;
		struct timezone tz;
		gettimeofday(&tv, &tz);
		int iCurrentTime = tv.tv_sec;

		if( iCurrentTime > (timeMaintenance+timeInterval) )
		{
			timeInterval = m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH].intValue;
			int times = (iCurrentTime-timeMaintenance+timeInterval)/timeInterval;
			m_CarDataStatus[CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH].intValue = times;
		}
	}
		
	m_mutexForThreadSafe.unlock();
	
	return BS_E_OK;
}

int BsCarInfoMonitorC::setCarData(CAR_DATA data)
{
	int retval = BS_E_OK;

	if( data.type >= CAR_INFO_TYPE_NUMBER || data.dataType != CAR_INFO_DATA_INT )
	{
		return BS_E_ARG_INVALID;
	}
	if( !CAR_INFO_IS_USED(data.type) )
	{
		return BS_CAR_INFO_MONITOR_E_CAR_DATA_UNUSED;
	}
	
	m_mutexForThreadSafe.lock();

	switch( data.type )
	{
		case CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH:
		case CAR_INFO_TYPE_MILEAGE:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_AFTER_CREATE:
		case CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT:
			if( CAR_INFO_DATA_INT == data.dataType )
			{
				m_CarDataStatus[data.type].intValue = data.intValue;
			}
			else
			{
				m_CarDataStatus[data.type].boolValue = data.boolValue;
			}

			if( -1 != m_iTimerSaveCarData )
			{
				killTimer(m_iTimerSaveCarData);
				_WriteCarDataToDB();
				m_iTimerSaveCarData = startTimer(m_iTimerSaveCarDataElapseTime);
			}
			break;
			
		default:
			retval = BS_CAR_INFO_MONITOR_E_CAR_DATA_UNSUPPORT_SET;
			break;
	}

	m_mutexForThreadSafe.unlock();

	return retval;
}

int BsCarInfoMonitorC::getCarData(CAR_DATA &data)
{
	int retval = BS_E_OK;
	if( data.type >= CAR_INFO_TYPE_NUMBER )
		return BS_E_ARG_INVALID;
	if( !CAR_INFO_IS_USED(data.type) )
		return BS_CAR_INFO_MONITOR_E_CAR_DATA_UNUSED;
	
	m_mutexForThreadSafe.lock();

	data = m_CarDataStatus[data.type];

	m_mutexForThreadSafe.unlock();

	return retval;
}


