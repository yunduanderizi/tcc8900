#ifndef  CAR_DATA_GATHER_H_20110516
#define  CAR_DATA_GATHER_H_20110516

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include "i2ccontrol.h"
#include "bsCarInfoMonitor.h"
#include <QMutex>

class BsCarInfoMonitorC;
class CarDataGatherC : public QThread
{
    Q_OBJECT
private:
    friend class BsCarInfoMonitorC;
    CarDataGatherC(BsCarInfoMonitorC *pBsCarInfoMonitorC);
    ~CarDataGatherC();

    int getValidCarInfo(CarInfoList& list);
    int getCarData(CAR_DATA& data);
    int getUploadMode(CAR_DATA_UPLOAD_MODE &mode, int &iTimer);
	
    int _InCar_getValidCarInfo(CarInfoList &list);
    void run();
    volatile bool m_bExit;

private:
    BsCarInfoMonitorC *m_pBsCarInfoMonitorC;
    void _InCar_Gather( CAR_DATA );
private:
    IBI2CControl *i2cControl;
    QMutex m_MutexForThreadSafe;
};
#endif
