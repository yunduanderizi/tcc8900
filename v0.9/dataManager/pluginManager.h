/**
 * @file   pluginManager.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20111017
 * 
 * @brief  
 *      header of dataManager module's plugin manager class 
 * 
 */
#ifndef PLUGINMANAGER_H_20111017
#define PLUGINMANAGER_H_20111017

#include <QObject>
#include "debugMonitor.h"
#include "pluginData.h"
 #include <dlfcn.h>

#define BUFFSIZE 					1024*10
namespace DataManager
{


static QString dataErrorInfo[DM_E_ERROR_MAX] = {"succeed","check version error","dataBase error","load plugin error","address is no exist","address can not read and write","address not enough","back userData error","rever userData error","repair subarea start","repair subarea succeed","repair subarea error","repair data error","get version info error"};
	
class PluginManagerC : public QObject
{
	Q_OBJECT
public:
	PluginManagerC();
	~PluginManagerC();

	void backupUserData(QString sAddress);
	void revertUserData(QString sAddress);	
	void repairUserData(QString sAddress);
	void repairUserSubarea();
	QString findPluginOfType(USER_DATA_SURPOR_TYPE dataType);
signals:
	void sigBackupUserDataProgress(int iProgress);
	void sigRevertUserDataProgress(int iProgress);
	void sigRepairUserDataProgress(int iProgress);
	void sigUserDataHandleResult(DM_ERROR_INFO statusNum, QString statusInfo);

private:
	typedef QList<QString> PluginList;
	class TypeAndPlugin
	{
	public:
		void clear(){pluginDataPlugin = TYPE_FRIST;pluginName = "";};
		USER_DATA_SURPOR_TYPE  pluginDataPlugin;
		QString  pluginName;
	};
	typedef QList<TypeAndPlugin> TypeAndPlugin_List;
	void _getCurrentVersionInfo(int &currentVersion);
	int _CheckPath(QString sAddress);
	int _readLib(PluginList *libList);
	void *_load(char *pluginName,PluginDataC *&GetObject, void (*&ReleaseObject)(void));
	int _checkRervertUserData(int currentVersion,int backVersion);
	int _inquireSurporType(TypeAndPlugin_List *typeAndPluginList);
	bool _isRepairUserSubarea();
};

}


#endif


