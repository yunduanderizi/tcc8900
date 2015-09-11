/**
 * @file   dataManagerClient.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111017
 * 
 * @brief  
 *      header of dataManager module's interface class 
 * 
 */
#ifndef DATAMANAGERCLIENT_H_20111017
#define DATAMANAGERCLIENT_H_20111017

#include <QObject>
#include "debugMonitor.h"
#include "dataManagerDefine.h"
#include "dataManagerMessage.h"
#include "dataManagerInterface.h"

namespace DataManager
{

class DataManagerC;
class DataManagerClientC : public DataManagerInterfaceC
{
	Q_OBJECT
public:
	//get the object of "DataManagerClientC", notice: you can not get the object from "new"
	static DataManagerClientC *object();
	//release the object of "DataManagerClientC"
	static void releaseObject();		

	void Init(DBusClientC *pDbusClient);

private:
	static DataManagerClientC *thisObject;
	DataManagerClientC();
	~DataManagerClientC();

	static void _updateReadWriteUserDataFlagHandler(DBusNotifyC *notify);

	DBusClientC*		m_pDbusClient;
private slots:
	void _sendReadWriteUserDataFlag(bool bReadWriteUserData);
private:
	//Overloading this function let this class to be instantiated
	void mustOverLoad(){}
};


}


#endif


