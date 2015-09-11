/**
 * @file   dataManagerInterface.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111017
 * 
 * @brief  
 *      header of dataManager module's interface base class 
 * 
 */
#ifndef DATAMANAGERINTERFACE_H_20111017
#define DATAMANAGERINTERFACE_H_20111017

#include <QObject>
#include "debugMonitor.h"
#include "dataManagerDefine.h"





namespace DataManager
{


class DataManagerC;
class DataManagerInterfaceC : public QObject
{
	Q_OBJECT
public:
	DataManagerInterfaceC();
	~DataManagerInterfaceC();

	int backupUserData(QString sAddress);
	int revertUserData(QString sAddress);
	int repairUserData(QString sAddress);
	int requestReadWriteUserData();
	int reportError(USER_DATA_SURPOR_TYPE dataType);
	int startupCheck();
	int ignoreError();	
	int ignoreErrorThisTime();
	int getReadWriteUserDataFlag(bool* pbFlag);
	QString findPluginOfType(USER_DATA_SURPOR_TYPE dataType);
	

signals:
	void sigBackupUserDataProgress(int iProgress);
	void sigRevertUserDataProgress(int iProgress);
	void sigRepairUserDataProgress(int iProgress);
	void sigReadWriteUserDataChanged();
	void sigUserDataHandleResult(DM_ERROR_INFO statusNum, QString statusInfo);

protected:
	DataManagerC*	m_pDataManager;
	bool				m_bInitFlag;

	void _updateReadWriteUserDataFlag(bool bReadWriteUserData);

private:

private:
	//Following function protect this class not to be instantiated
	virtual void mustOverLoad()=0;
};


}


#endif


