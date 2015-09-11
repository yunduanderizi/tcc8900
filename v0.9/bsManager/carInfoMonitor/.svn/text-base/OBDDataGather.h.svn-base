#ifndef  OBD_DATA_GATHER_H_20110527
#define  OBD_DATA_GATHER_H_20110527

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include "i2ccontrol.h"
#include "bsCarInfoMonitor.h"


class BsCarInfoMonitorC;
class OBDDataUploadC;
class OBDDataGatherC : public QThread
{
	Q_OBJECT
private:
	friend class BsCarInfoMonitorC;
	OBDDataGatherC(BsCarInfoMonitorC *pBsCarInfoMonitorC);
	~OBDDataGatherC();

	int stopGather();
	int startGather();

private slots:
	void slotFinished();
	
private:
	BsCarInfoMonitorC *m_pBsCarInfoMonitor;
	OBDDataUploadC *m_pOBDDataUpload;

	void run();
	volatile bool m_bExit;


};
#endif
