#ifndef  BS_UCS_INFO_GATHER_20110824
#define  BS_UCS_INFO_GATHER_20110824

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"
#include "bsManagerMessage.h"
#include <QMutex>
#include <QWaitCondition>

class UcsInfoC
{
public:
	UcsInfoC();

	QString	sUmId;
	QString	sUmHost;
	QString	sUmPort;
	QString	sUmUserJid;
	bool bAutoReportLocation;
	int iAutoReportInterval;
};

Q_DECLARE_METATYPE(UcsInfoC);

class BsUcsInfoGatherC : public QThread, public BsBaseC
{
	Q_OBJECT
		
public:
	//get the object of "BsUcsInfoGatherC", notice: you can not get the object from "new"
	static BsUcsInfoGatherC *object();
	//release the object of "BsUcsInfoGatherC"
	static void releaseObject();		
	//get the object of "BsUcsInfoGatherC", if it does not exist, not new it
	static BsUcsInfoGatherC *getObject();
	
	int gatherUcsInfo();

	bool isFinish();

	int getUcsInfo(UcsInfoC& info);

	void cancelOperation();

public:
	void internalNotify(BsManager_Command_Id id);
	
signals:
	void sigGetUcsInfo(UcsInfoC info);

private:
	static BsUcsInfoGatherC *thisObject;
	
	BsUcsInfoGatherC();
	~BsUcsInfoGatherC();

	void run();
	int _downloadUcsInfo();
	volatile bool m_bExit;
	bool m_bStartAgain;

private slots:
	void slotFinished();		
private:
	QMutex m_mutexForThreadSafe;
	QMutex m_mutexWaitCondition;
	QWaitCondition m_condExit;
};

#endif
