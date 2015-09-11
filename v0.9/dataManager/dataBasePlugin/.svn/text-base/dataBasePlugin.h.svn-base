#ifndef DATABASE_PLUGIN_H_20111104
#define DATABASE_PLUGIN_H_20111104

#include "pluginData.h"
#include <QString>
#include <QList>
#include <sqlite3.h>
#include <stdlib.h>
#include <string>
#include <QSettings>
using namespace DataManager;
using namespace std;
#define DATA_SIZE  100*1000
extern "C" PluginDataC *GetObject(void);
extern "C" void ReleaseObject(void);

typedef enum  _DATA_BASE_PLUGIN_ERROR
	{
		DATA_BASE_BACK_ERROR = -5,
		DATA_BASE_READ_ERROR,
		DATA_BASE_OPEN_ERROR,
		DATA_BASE_OUT_TIME,
		DATA_BASE_PREPARE_ERROR,
		DATA_BASE_PLUGIN_OK = 0
	}DATA_BASE_PLUGIN_ERROR;
class DataBasePluginC: public PluginDataC
{
public:
	DataBasePluginC();
	~DataBasePluginC();
public:
	virtual int backupUserData(QString sAddress);
	virtual int revertUserData(QString sAddress);
	virtual int checkRervertUserData(int currentVersion,int backVersion);
	virtual bool inquireSurporType(USER_DATA_SURPOR_TYPE surpor_type);
	virtual bool isRepairUserSubarea();
		 
private:
	typedef enum _FIELDS_DATA_TYPE
	{
		FIELDS_DATA_TYPE_INT=1,
		FIELDS_DATA_TYPE_DOUBLE,
		FIELDS_DATA_TYPE_TEXT,
		FIELDS_DATA_TYPE_BLOB,
		FIELDS_DATA_TYPE_NULL
	}FIELDS_DATA_TYPE;

	class ColumnInfo
	{
	public:
		int lineID;
		string name;
		FIELDS_DATA_TYPE dataType;
		string declType;
		union
		{
			char Data_Text[DATA_SIZE];
			char Data_Blob[DATA_SIZE];
			double Data_Float;
			int Data_Int;
		};
	};
	typedef QList<ColumnInfo> ColumnInfoList;

	int _readBaseToStruct(sqlite3 *db,char *tableName,ColumnInfoList *columnInfoList,int &maxLine);
	bool _findDataFromStruct(int lineID,string name,string declTpye,ColumnInfoList *columnInfoList,ColumnInfo *columnInfo);

};
#endif
