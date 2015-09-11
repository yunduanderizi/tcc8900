#ifndef  IBSTORY_CONTROL_H_20110829
#define  IBSTORY_CONTROL_H_20110829

#include <QString>
#include <QObject>
#include <qfile.h>
#include <QMetaType>  
#include "ibMediaDefine.h"
#include "ibMediaListControl.h"

namespace MediaControl2
{

class IBMediaControlC;
class IBStoryControlC : public IBMediaListControlC
{
	Q_OBJECT

public:
	//get the object of "IBStoryControlC", notice: you can not get the object from "new"
	static IBStoryControlC *object();
	//release the object of "IBStoryControlC"
	static void releaseObject();		

	void setTimeFormat(MEDIA_TIME_FORMAT format){IBMediaListControlC::setTimeFormat(format);}


private:
	static IBStoryControlC *thisObject;
	
	IBStoryControlC();
	~IBStoryControlC();

};





}

#endif
