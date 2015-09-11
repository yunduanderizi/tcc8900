/**
 * @file   bsClient.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111111
 * 
 * @brief  
 *      Interface define for dbus client 
 * 
 */
#ifndef BSCLIENT_H_20111111
#define  BSCLIENT_H_20111111

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"
#include "bsManagerMessage.h"
#include <QMutex>



class BsClientC : public QThread,  public BsBaseC
{
	Q_OBJECT
public:
	//get the object of "BsClientC", notice: you can not get the object from "new"
	static BsClientC *object();
	//release the object of "BsClientC"
	static void releaseObject();
	//get the object of "BsClientC", if it does not exist, not new it
	static BsClientC *getObject();
	
	int Init(DBusServerC *pDbusServer);

public:
	void internalSendCommonCommand(BsManager_Command_Id id);
	
private:
	static BsClientC *thisObject;
	BsClientC();
	~BsClientC();

	static void _commonHandler(DBusNotifyC *notify);

	void _common(BsManager_Command_Id id);

private:
	bool		m_bInitFlag;
	DBusClientProxyC   *m_pDbusClientToBsManagerProcess;
	DBusServerC		*m_pDbusServer;
	QMutex m_mutexForThreadSafe;
};

#endif
