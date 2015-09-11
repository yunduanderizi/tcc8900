/**
 * @file   bsGetTelephone.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110704
 * 
 * @brief  
 *      define for getting telephone of sevice or helping
 * 
 */
#ifndef BSGETTELEPHONE_H_20110704
#define BSGETTELEPHONE_H_20110704


#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QThread>
#include <QMetaType>  
#include <qfile.h>
#include <QMutex>
#include <QWaitCondition>
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"
#include "NaviDbusMessage.h"
#include "dbusClientProxy.h"
#include "dbusClient.h"
#include "dbusMessage.h"
#include "bsManagerMessage.h"


typedef enum _Telephone_Call_Type_E
{
	TCT_PSTN = 0,
	TCT_PSTN_CALL_BACK,
	TCT_SKYPE,
}Telephone_Call_Type_E;

class BsTelephoneC
{
public:
	BsTelephoneC(){id=-1; phoneNumber="", callType=TCT_PSTN; callBackNumber.clear();};
	void clear(){id=-1; phoneNumber="", callType=TCT_PSTN; callBackNumber.clear();};

	int		id;
	QString	phoneNumber;
	Telephone_Call_Type_E callType;
	QStringList	callBackNumber;
};

typedef QList<BsTelephoneC> BsTelephoneList;

class BsGetTelephoneC : public QThread, public BsBaseC
{
	Q_OBJECT
		
public:
	
	//get the object of "BsGetTelephoneC", notice: you can not get the object from "new"
	static BsGetTelephoneC *object();
	//release the object of "BsGetTelephoneC"
	static void releaseObject();
	//get the object of "BsGetTelephoneC", if it does not exist, not new it
	static BsGetTelephoneC *getObject();

	typedef enum  _TELEPHONE_TYPE
	{
		SERVICETELEPHONE = 0,
		HELPINGTELEPHONE,
		SUPPORTTELEPHONE,
		SHOPTELEPHONE,
		OEM_4,
		OEM_5,
		OEM_6,
		OEM_7,
		OEM_8,
		OEM_9,
		OEM_MAX
	}TELEPHONE_TYPE;
	void startGettingTelephone();
	bool isFinish();

	QString getOEMTelephone(TELEPHONE_TYPE typeNum);  //get OEM telephone 

	int getTelephone(BsTelephoneList& phoneList);

	void cancelOperation();

	void emptyTelephone();
	
public:
	void internalNotify(BsManager_Command_Id id);

signals:
	void sigTelephoneChanged();
	
private slots:
	void slotFinished();
	
private:
	static BsGetTelephoneC *thisObject;
	BsGetTelephoneC();
	~BsGetTelephoneC();

	volatile bool m_bExit;
	bool	m_bStartAgain;


	int _getTelephoneFromBS(QFile *file, BsTelephoneList& honeList);
	int _comparePhoneList(BsTelephoneList& list1, BsTelephoneList& list2);
	void run();
	QMutex m_mutexForThreadSafe;
	QMutex m_mutexWaitCondition;
	QWaitCondition m_condExit;

};



#endif


