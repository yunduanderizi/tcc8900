#ifndef PLUGIN_DATA_H_
#define PLUGIN_DATA_H_

#include <QString>
#include "dataManagerDefine.h"
namespace DataManager
{
class PluginDataC
{
public:
virtual int backupUserData(QString sAddress) = 0;
virtual int revertUserData(QString sAddress) = 0;
virtual int checkRervertUserData(int currentVersion,int backVersion) = 0;
virtual bool inquireSurporType(USER_DATA_SURPOR_TYPE surpor_type) = 0;
virtual bool isRepairUserSubarea() = 0;
};
}
#endif
