/**
 * @file   bsLocationUpload.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110227
 * 
 * @brief  
 *      define for info of location uploading 
 * 
 */
#ifndef BSLOCATIONUPLOAD_H_20110227
#define BSLOCATIONUPLOAD_H_20110227


#include <QString>
#include <QList>
#include <QMap>
#include <QThread>
#include <QMetaType>  
#include <qfile.h>
#include <QTimerEvent>
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"
#include "NaviDbusMessage.h"
#include "dbusClientProxy.h"
#include "dbusClient.h"
#include "dbusMessage.h"
#include "bsUcsInfoGather.h"
#include <QMutex>
#include <QWaitCondition>

//data struct used by BsLocationUploadC


class BsLocationUploadC : public QThread, public BsBaseC
{
	Q_OBJECT
		
public:
	//get the object of "BsLocationUploadC", notice: you can not get the object from "new"
	static BsLocationUploadC *object();
	//release the object of "BsLocationUploadC"
	static void releaseObject();

	static BsLocationUploadC *getObject();
	
	static void naviGPSInfoReportNotifyHandler(DBusNotifyC *pNotify);

	static void naviGPSLocationNotifyHandler(DBusNotifyC *pNotify);

	static void naviGPSInfoNotifyHandler(DBusNotifyC *pNotify);

	int setGpsClientDbusObject(DBusClientC *client);
	
	int uploadLocationInfo();

	//if location info is void or too old, we will return at once, not take any time
	int getLocationInfo(float &fX, float &fY, int &iTime, int &iOrientation, int &iVelocity);

	//if location info is void or too old, we will get them from gsp, this will take some time.
	int getLocationInfo2(float &fX, float &fY, int &iTime, int &iOrientation, int &iVelocity);

	bool preSendIsSuccess();

	void cancelOperation();
private:
	static BsLocationUploadC *thisObject;
	BsLocationUploadC();
	~BsLocationUploadC();

	void updateInfo(float fX, float fY, int iTime, int iOrientation, int iVelocity);
	void updateLocation(const char* pRoadName, const char* pAreaName, const char* pCityName);
	void clearLocation();
	
	//following two function only be invoked by run()
	int getGpsInfo(int iMsecond);
	int getGpsLocation(int iMsecond);
	int upload(char* uuid, char* tuid, char* token);
	
	DBusClientC *m_dbusClient;
	
	//following is gps info
	float		m_fX;
	float		m_fY;
	int		m_iTime;
	int		m_iOrientation;
	int		m_iVelocity;
	char		m_cRoadName[50];
	char		m_cAreaName[50];
	char		m_cCityName[50];
	
	volatile bool m_bExit;
	bool m_bSendSuccess;
	
	void run();

private slots:
	void slotGetUcsInfo(UcsInfoC info);
	
private:
	void timerEvent(QTimerEvent * event);
	
	int m_iTimerAutoReportLocation;							//timer of auto report location	
	QMutex m_mutexForThreadSafe;
	QMutex m_mutexWaitCondition;
	QWaitCondition m_condExit;
};



#endif


