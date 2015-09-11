#ifndef  SYSTEMSETTING_H_20110408
#define  SYSTEMSETTING_H_20110408

#include <QString>



#define SYSSETTING_E_OK 						0
#define SYSSETTING_E_INVALID_FILE              -1
#define SYSSETTING_E_SAVE_DATA_ERROR        -2
#define SYSSETTING_E_GET_DATA_ERROR       -3



class SystemSettingC
{
	public:
	       //get the object of "SystemSettingC", notice: you can not get the object from "new"
		static SystemSettingC *object();
	       //release the object of "SystemSettingC"
	       static void releaseObject();
		int setNewCallRing(const QString ringNameWithPath);
		int getNewCallRing(QString& ringNameWithPath);
		
	private:
		static SystemSettingC *thisObject;
		SystemSettingC();
		~SystemSettingC();
};

#endif
