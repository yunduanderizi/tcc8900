#include "carDataGather.h"
#include "i2ccontrol.h"
#include <QXmlStreamReader>
#include <curl.h>
#include <QDir>
#include <QTime>
#include <QMutex>
#include <QDebug>

static QMutex gs_mutexForThreadSafe;




CarDataGatherC::CarDataGatherC(BsCarInfoMonitorC *pBsCarInfoMonitorC)
	: m_pBsCarInfoMonitorC(pBsCarInfoMonitorC)
{
        i2cControl = NULL;
	start();//start gather car data
}

CarDataGatherC::~CarDataGatherC()
{
	m_MutexForThreadSafe.lock();

	if( isRunning() )
	{
		m_bExit = true;
		m_MutexForThreadSafe.unlock();
		wait();
		m_MutexForThreadSafe.lock();
	}

        if(i2cControl != NULL )
            delete i2cControl;
        i2cControl = NULL;
	m_MutexForThreadSafe.unlock();
}

int CarDataGatherC::getValidCarInfo(CarInfoList& list)
{
	m_MutexForThreadSafe.lock();
        list.clear();
    _InCar_getValidCarInfo( list );

	m_MutexForThreadSafe.unlock();

	return 0;
}

int CarDataGatherC::getCarData(CAR_DATA& data)
{
	m_MutexForThreadSafe.lock();

    int _tmp = data.type;
        switch(_tmp)
	{
		case CAR_INFO_TYPE_WATER_TEMPERATURE_TOP_LIMITED:
			data.intValue = 12000;
			break;
			
		case CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT_TIME:
			data.intValue = 15;
			break;

		case CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME:
			data.boolValue = true;
			break;

		case CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL:
			data.intValue = 0;
			break;

		default:
			break;
	}
	
	m_MutexForThreadSafe.unlock();

	return 0;
}

int CarDataGatherC::getUploadMode(CAR_DATA_UPLOAD_MODE &mode, int &iTimer)
{
	mode = CAR_DATA_UPLOAD_MODE_ABNORMAL;
	return 0;
}

int CarDataGatherC:: _InCar_getValidCarInfo(CarInfoList &list)
{
    list.append(CAR_INFO_TYPE_SPEED);
    list.append(CAR_INFO_TYPE_AIRBAG_IS_OPEN);
    list.append(CAR_INFO_TYPE_WATER_TEMPERATURE);
    list.append(CAR_INFO_TYPE_DOOR_IS_ILLEGAL);
    list.append(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN);
    list.append(CAR_INFO_TYPE_WATER_TEMPERATURE_TOP_LIMITED);
    list.append(CAR_INFO_TYPE_WATER_TEMPERATURE_IS_TOO_HIGH);
    list.append(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_ON_RUNNING);
    list.append(CAR_INFO_TYPE_ALL_DOOR_IS_OPEN_TIMEOUT_TIME);
    list.append(CAR_INFO_TYPE_MILEAGE);	
    list.append(CAR_INFO_TYPE_MILEAGE_INCREMENT);
    list.append(CAR_INFO_TYPE_NEED_MAINTENANCE);
    list.append(CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MILEAGE);
    list.append(CAR_INFO_TYPE_MAINTENANCE_TIPS_ON_MONTH);
    list.append(CAR_INFO_TYPE_MAINTENANCE_MILEAGE_AT_LAST);
    list.append(CAR_INFO_TYPE_MAINTENANCE_DATE_AT_LAST);
    list.append(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_LAST);
    list.append(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIME_AT_NEXT);
    list.append(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MILEAGE);
    list.append(CAR_INFO_TYPE_MAINTENANCE_TIPS_TIMES_ON_MONTH);
    list.append(CAR_INFO_TYPE_MAINTENANCE_TIPS_AFTER_CREATE);
    list.append(CAR_INFO_TYPE_MANTENANCE_NOT_TIPS_THIS_TIME);
    list.append(CAR_INFO_TYPE_MANTENANCE_TIPS_MINI_INTERVAL);

    return 0;
}

void CarDataGatherC::run()
{
    static CAR_DATA data;

    while(1)
    {
        _InCar_Gather( data );

        m_pBsCarInfoMonitorC->uploadCarData();
    }

}


void CarDataGatherC::_InCar_Gather( CAR_DATA data )
{
    static QTime OldTime = QTime::currentTime();
    unsigned char value;
    usleep(9*100000);
    if( NULL == i2cControl )
    {
        i2cControl = new IBI2CControl(0);
    }

    int i2cOpen = i2cControl->checkOK();
    if( 0 == i2cOpen )
    {
        IBD_BEGIN(DEBUG_EMERGENCY)<<QString("i2c open error !")<<IBD_ENDL<<IBD_END
    }
//===========================================================================>气囊  start

    if( 0 != i2cControl->readByte( 0x54 ,0x07,value) )//0x07汽车状态 bit3为 气囊
    {
        IBD_BEGIN(DEBUG_EMERGENCY)<<QString("i2c readByte( 0x54,0x07 ) error !")<<IBD_ENDL<<IBD_END
    }

    data.type = CAR_INFO_TYPE_AIRBAG_IS_OPEN;
    data.dataType = CAR_INFO_DATA_BOOL;
    (((value >> 3) & 0x01) == 1 )?(data.boolValue = true):(data.boolValue = false);

    m_pBsCarInfoMonitorC->reportCarData(data);
 //===========================================================================>气囊  end






 //===========================================================================>车门总开关 start
    if( 0 != i2cControl->readByte( 0x54 ,0x08,value) )
    {
        IBD_BEGIN(DEBUG_EMERGENCY)<<QString("i2c readByte( 0x54,0x08 ) error !")<<IBD_ENDL<<IBD_END
    }

    data.type = CAR_INFO_TYPE_ALL_DOOR_IS_OPEN;
    data.dataType = CAR_INFO_DATA_BOOL;
    ((((value >> 4) & 0x01)== 1)?(data.boolValue = true):(data.boolValue = false));

    m_pBsCarInfoMonitorC->reportCarData(data);
 //===========================================================================>车门总开关 end






//===========================================================================>车门异常开启 start
    data.type = CAR_INFO_TYPE_DOOR_IS_ILLEGAL;
    data.dataType = CAR_INFO_DATA_BOOL;
    ((((value >> 6) & 0x03)== 0x03)?(data.boolValue = true):(data.boolValue = false));

    m_pBsCarInfoMonitorC->reportCarData(data);
//===========================================================================>车门异常开启 end






//===========================================================================>冷却液温度 start
    if( 0 != i2cControl->readByte( 0x54 ,0x0B,value) ) //0x0B
    {
        IBD_BEGIN(DEBUG_EMERGENCY)<<QString("i2c readByte( 0x54,0x0B ) error !")<<IBD_ENDL<<IBD_END
    }

    int Temper = 0;
    Temper = value & 0xff;

    data.type = CAR_INFO_TYPE_WATER_TEMPERATURE;
    data.dataType = CAR_INFO_DATA_INT;
    data.intValue = (int)((((float)Temper * 0.75) - 48) * 100);

    m_pBsCarInfoMonitorC->reportCarData(data);
//===========================================================================>冷却液温度 end






//============================================================================>车速  start
    if( 0 != i2cControl->readByte( 0x54 ,0x0C,value) ) //0x0C车速
    {
        IBD_BEGIN(DEBUG_EMERGENCY)<<QString("i2c readByte( 0x54,0x0C ) error !")<<IBD_ENDL<<IBD_END
    }

    data.type = CAR_INFO_TYPE_SPEED;
    data.dataType = CAR_INFO_DATA_INT;
    data.intValue = value;
    m_pBsCarInfoMonitorC->reportCarData(data);
//============================================================================>车速  end



//============================================================================>里程  start
    QTime TimeNow = QTime::currentTime();
    int nPasedMilliSeconds =OldTime.msecsTo(TimeNow);
    static float atLastMileage = 0;
	/*get time < 5s mileage */
    float tempMileage = (nPasedMilliSeconds < 5000)? (data.intValue*nPasedMilliSeconds)/(3.6*1000.0):0;
    tempMileage += atLastMileage;
    atLastMileage = (tempMileage - int(tempMileage));
    OldTime = TimeNow;

    data.type = CAR_INFO_TYPE_MILEAGE_INCREMENT;
    data.dataType = CAR_INFO_DATA_INT;
    data.intValue = (int)tempMileage;
	//qDebug()<<"data.intValue============================================="<<data.intValue;
    m_pBsCarInfoMonitorC->reportCarData(data);
//============================================================================>里程  end

}


