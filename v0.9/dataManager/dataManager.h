/**
 * @file   dataManager.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111017
 * 
 * @brief  
 *      header of dataManager module's function class 
 * 
 */
#ifndef DATAMANAGER_H_20111017
#define DATAMANAGER_H_20111017
#include <QDir>
#include <QObject>
#include <QThread>
#include <QMetaType>  
#include <QMutex>  
#include "debugMonitor.h"
#include "pluginManager.h"
#define USERDATA_REPORT_ERROR  1
namespace DataManager
{

class PluginManagerC;

class DataManagerC :public QThread
{
	Q_OBJECT
public:
	DataManagerC();
	~DataManagerC();

	void backupUserData(QString sAddress);
	void revertUserData(QString sAddress);
	void repairUserData(QString sAddress);
	void requestReadWriteUserData();
	void reportError(USER_DATA_SURPOR_TYPE dataType);
	int startupCheck();
	void ignoreError();
	void ignoreErrorThisTime();
	bool isReadWriteUserData();
	void updateReadWriteUserDataFlag(bool bReadWriteUserData);
	QString findPluginOfType(USER_DATA_SURPOR_TYPE dataType);
	
signals:
	void sigBackupUserDataProgress(int iProgress);
	void sigRevertUserDataProgress(int iProgress);
	void sigRepairUserDataProgress(int iProgress);
	void sigReadWriteUserDataChanged();
	void sigSendReadWriteUserDataFlag(bool bReadWriteUserData);
	void sigUserDataHandleResult(DM_ERROR_INFO statusNum, QString statusInfo);

private:
	void run();
	class DownloadTask
	{
	public:
		QString backupAddress;
		QString revertAddress;
		QString repairBackAddress;
		int ThreadWorkMode;
	};
	typedef QList<DownloadTask> DownloadTaskList;
	DownloadTaskList m_TaskList;
	bool m_bReadWriteUserData;
	PluginManagerC*	m_pPluginManager;
	bool _isSubareaMountSuccess();
	void _repairUserSubarea();
};


}


#endif


