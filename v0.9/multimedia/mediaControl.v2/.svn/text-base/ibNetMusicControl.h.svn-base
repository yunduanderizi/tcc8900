#ifndef  IBNETMUSIC_CONTROL_H_20110829
#define  IBNETMUSIC_CONTROL_H_20110829

#include <QString>
#include <QObject>
#include <qfile.h>
#include <QMetaType>  
#include "ibMediaDefine.h"
#include "ibMediaListControl.h"

namespace MediaControl2
{

class IBMediaControlC;
class IBNetMusicControlC : public IBMediaListControlC
{
	Q_OBJECT

public:
	//get the object of "IBNetMusicControlC", notice: you can not get the object from "new"
	static IBNetMusicControlC *object();
	//release the object of "IBNetMusicControlC"
	static void releaseObject();		

	void setTimeFormat(MEDIA_TIME_FORMAT format){IBMediaListControlC::setTimeFormat(format);}


private:
	static IBNetMusicControlC *thisObject;
	
	IBNetMusicControlC();
	~IBNetMusicControlC();

};





}

#endif
