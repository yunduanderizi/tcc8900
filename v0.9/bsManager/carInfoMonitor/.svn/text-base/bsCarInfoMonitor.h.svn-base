#ifndef  BS_CARINFO_MONITOR_H_20110509
#define  BS_CARINFO_MONITOR_H_20110509

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"
#include <QTimerEvent>
#include <QMutex>

typedef enum _CAR_INFO_TYPE
{
	CAR_INFO_TYPE_AIRBAG_IS_OPEN = 0,					//bool	data
		
	CAR_INFO_TYPE_ALL_DOOR_IS_OPEN ,					//bool	data
	CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT,			//bool	setting	need CAR_INFO_TYPE_ALL_DOOR_IS_OPEN  CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT_TIME
	CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT_TIME,		//int		setting	need CAR_INFO_TYPE_ALL_DOOR_IS_OPEN  CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT
	CAR_INFO_TYPE_DOOR_IS_ILLEGAL,						//bool	data

	CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_ON_RUNNING,		//bool	setting	need CAR_INFO_TYPE_ALL_DOOR_IS_OPEN  CAR_INFO_TYPE_SPEED
        
	CAR_INFO_TYPE_LEFT_FRONT_DOOR_IS_OPEN,				//bool	data
	CAR_INFO_TYPE_LEFT_BEHIND_DOOR_IS_OPEN,			//bool	data
	CAR_INFO_TYPE_RIGHT_FRONT_DOOR_IS_OPEN,			//bool	data
	CAR_INFO_TYPE_RIGHT_BEHIND_DOOR_IS_OPEN,			//bool	data
	CAR_INFO_TYPE_TRUNK_DOOR_IS_OPEN,					//bool	data

	CAR_INFO_TYPE_LEFT_FRONT_DOOR_IS_LOCK,				//bool	data
	CAR_INFO_TYPE_LEFT_BEHIND_DOOR_IS_LOCK,			//bool	data
	CAR_INFO_TYPE_RIGHT_FRONT_DOOR_IS_LOCK,			//bool	data
	CAR_INFO_TYPE_RIGHT_BEHIND_DOOR_IS_LOCK,			//bool	data
	CAR_INFO_TYPE_TRUNK_DOOR_IS_LOCK,					//bool	data

	CAR_INFO_TYPE_SPEED,									//int	data
	CAR_INFO_TYPE_WATER_TEMPERATURE,					//int	data
	CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH,		//bool	setting	need CAR_INFO_TYPE_WATER_TEMPERATURE  CAR_INFO_TYPE_WATER_TEMPERATURE_TOP_LIMITED
	CAR_INFO_TYPE_WATER_TEMPERATURE_TOP_LIMITED,		//int	setting	need CAR_INFO_TYPE_WATER_TEMPERATURE  CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH
	CAR_INFO_TYPE_OUT_TEMPERATURE,						//int	data

	CAR_INFO_TYPE_REMAINING_FUEL,						//int		data
	CAR_INFO_TYPE_REMAINING_FUEL_IS_TOO_LOW,			//bool	setting	need CAR_INFO_TYPE_REMAINING_FUEL  CAR_INFO_TYPE_REMAINING_FUEL_BOTTOM_LIMITED
	CAR_INFO_TYPE_REMAINING_FUEL_BOTTOM_LIMITED,		//int		setting	need CAR_INFO_TYPE_REMAINING_FUEL  CAR_INFO_TYPE_REMAINING_FUEL_IS_TOO_LOW

	CAR_INFO_TYPE_MILEAGE,								//int		date		should be saved to database
	CAR_INFO_TYPE_MILEAGE_INCREMENT,					//int		date
	CAR_INFO_TYPE_NEED_MAINTENANCE,						//bool	setting	
	CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE,			//int		setting	should be saved to database
															//can be modified by user of UI
	CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE,	//int		setting	should be saved to database
	CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH,			//int		setting	should be saved to database
															//can be modified by user of UI
	CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH,	//int		setting	should be saved to database
	CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST,			//int		date		should be saved to database
	CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST,			//int		date		should be saved to database
	CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST,		//int		setting	should be saved to database
															//									     
	CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT,		//int		setting	should be saved to database  
															//									     
															//can be modified by user of UI
	CAR_INFO_TYPE_MAINTENANCE_TIPS_AFTER_CREATE,		//bool		setting	should be saved to database  need
															//can be modified by user of UI
	CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME,		//bool		setting
	CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL,		//int		setting
	
	CAR_INFO_TYPE_NUMBER,
	CAR_INFO_TYPE_INVALID
}CAR_INFO_TYPE;

typedef QList<CAR_INFO_TYPE> CarInfoList;

typedef enum _CAR_DATA_TYPE
{
	CAR_INFO_DATA_BOOL=0,
	CAR_INFO_DATA_INT
}CAR_DATA_TYPE;

typedef enum _OBD_CHECK_RESULT
{
	OBD_CHECK_SUCCESS = 0,
	OBD_CHECK_UPLOAD_FAIL,
}OBD_CHECK_RESULT;

typedef struct _CAR_DATA
{
	CAR_INFO_TYPE type;
	CAR_DATA_TYPE dataType;
	union
	{
		bool		boolValue;
		int		intValue;
	};
	bool	bReadFromDatabase;
}CAR_DATA;

typedef enum _CAR_DATA_UPLOAD_MODE
{
	CAR_DATA_UPLOAD_MODE_CHANGE = 0,
	CAR_DATA_UPLOAD_MODE_TIMER,
	CAR_DATA_UPLOAD_MODE_ABNORMAL,
	CAR_DATA_UPLOAD_MODE_ABNORMAL_TIMER,
	CAR_DATA_UPLOAD_MODE_NUMBER
}CAR_DATA_UPLOAD_MODE;

class CarDataUploadC;
class CarDataGatherC;
class OBDDataGatherC;
class BsCarInfoMonitorC : public QObject, public BsBaseC
{
	Q_OBJECT
	public:
		friend class CarDataUploadC;
		friend class CarDataGatherC;
		friend class OBDDataGatherC;
		
	       //get the object of "BsCarInfoMonitorC" "
		static BsCarInfoMonitorC *object();
	       //release the object of "BsCarInfoMonitorC"
	       static void releaseObject();
		//get the object of "BsCarInfoMonitorC", if it does not exist, not new it
		static BsCarInfoMonitorC *getObject();
		int getCurrentCarInfo(CarInfoList &typeList);
		int cancelCarInfo(CAR_INFO_TYPE type);
		int addCarInfo(CAR_INFO_TYPE type);
		int resumeDefaultCarInfo();
		int resumeDefaultCarData(CAR_INFO_TYPE type);
		int forceUploadCarInfo();
		int retrieveCarData();
		int setCarData(CAR_DATA data);
		int getCarData(CAR_DATA &data);

		int startCarInfoMonitor();
		int stopCarInfoMonitor();

		int nowToMaintain();
		int setNextMaintenanceTipsTime(int IntervalTime);
		int thisTimeNotTips();
		
		int startOBDMonitor();
		int stopOBDMonitor();

		void cancelOperation();
		
		
	signals:
		void sigCarDataFeedback(CAR_DATA data);
		void sigOBDCheckResult(OBD_CHECK_RESULT result);

	private:
		static BsCarInfoMonitorC *thisObject;
		BsCarInfoMonitorC();
		~BsCarInfoMonitorC();
		void timerEvent(QTimerEvent * event);
		int reportCarData(CAR_DATA data);
		int uploadCarData();

		int feedbackOBDCheckResult(OBD_CHECK_RESULT result);

	private:
		int _uploadCarData(int whereFrom);
		int _getEmergencyStatus();
		int _getAllDoorLockStatus();
		void _checkReportedCarData(CAR_DATA data);
		void _initCarData();
		int _ReadCarDataFromDB();
		int _WriteCarDataToDB();
		void _checkMaintenanceOnMileage();
		void _checkMaintenanceOnMonth();

		CAR_DATA m_CarDataStatus[CAR_INFO_TYPE_NUMBER];	//save all car data
		CarInfoList m_CarInfoList;								//save car infos supported currently

		CarDataUploadC *m_pCarDataUpload;
		CarDataGatherC *m_pCarDataGather;
		OBDDataGatherC *m_pOBDDataGather;
		
	
		bool m_bCarDataChanage;							//if change in m_CarDataStatus
		int m_iAllDoorOpenTime;								//the time that car door is openned
		CAR_DATA_UPLOAD_MODE m_CarDataUploadMode;		//
		int m_iTimerCarDataUpload;							//timer of uploading car data
		int m_iTimerCarDataUploadElapseTime;				//times elapse by m_iTimerCarDataUpload
		int m_iTimerAllDoorOpenTimeout;						//timer of timeout of all door openning
		int m_iTimerCheckMaintenance;						//timer for maintenance checking, decide whether to give out the tips
		int m_iTimerCheckMaintenanceElapseTime;				//times elapse by m_iTimerCheckMaintenance
		int m_iEmergencyStatus;								//emergency status currently
		int m_iTimerCheckTipsOnCreate;						//timer for checking whether to tips when creating
		int m_iTimerCheckTipsOnCreateElapseTime;			//times elapse by m_iTimerCheckTipsOnCreate
		int m_iTimerSaveCarData;								//timer for saving car data
		int m_iTimerSaveCarDataElapseTime;					//times elapse by m_iTimerSaveCarData
		
		bool m_bStartCarInfoMonitor;
		QMutex m_mutexForThreadSafe;
};

#endif
