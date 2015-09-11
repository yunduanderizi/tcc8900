/**
 * @file   bsServer.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111111
 * 
 * @brief  
 *      Interface define for dbus server 
 * 
 */
#ifndef BSSERVER_H_20111111
#define  BSSERVER_H_20111111

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include <QMutex>
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"
#include "bsManagerMessage.h"




class BsServerC : public QThread,  public BsBaseC
{
	Q_OBJECT
public:
	//get the object of "BsServerC", notice: you can not get the object from "new"
	static BsServerC *object();
	//release the object of "BsServerC"
	static void releaseObject();
	//get the object of "BsServerC", if it does not exist, not new it
	static BsServerC *getObject();

	DBusServerC *getDbusServer();
	
private:
	static BsServerC *thisObject;
	BsServerC();
	~BsServerC();

	static void _commonHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);


	DBusServerC		*m_pDbusServer;
	QMutex m_mutexForThreadSafe;
};

#endif
