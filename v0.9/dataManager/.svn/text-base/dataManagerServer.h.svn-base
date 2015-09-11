/**
 * @file   dataManagerServer.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111017
 * 
 * @brief  
 *      header of dataManager module's interface class 
 * 
 */
#ifndef DATAMANAGERSERVER_H_20111017
#define DATAMANAGERSERVER_H_20111017

#include <QObject>
#include "debugMonitor.h"
#include "dataManagerDefine.h"
#include "dataManagerMessage.h"
#include "dataManagerInterface.h"

namespace DataManager
{

class DataManagerC;
class DataManagerServerC : public DataManagerInterfaceC
{
	Q_OBJECT
public:
	//get the object of "DataManagerServerC", notice: you can not get the object from "new"
	static DataManagerServerC *object();
	//release the object of "DataManagerServerC"
	static void releaseObject();		

	int Init(DBusServerC *pDbusServer);

private:
	static DataManagerServerC *thisObject;
	DataManagerServerC();
	~DataManagerServerC();

	static void _updateReadWriteUserDataFlagHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);

	DBusServerC*		m_pDbusServer;
private slots:
	void _sendReadWriteUserDataFlag(bool bReadWriteUserData);
private:
	//Overloading this function let this class to be instantiated
	void mustOverLoad(){}
};


}


#endif


