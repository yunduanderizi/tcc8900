#ifndef  IBMUSIC_CONTROL_H_20110829
#define  IBMUSIC_CONTROL_H_20110829

#include <QString>
#include <QObject>
#include <qfile.h>
#include <QMetaType>  
#include "ibMediaDefine.h"
#include "ibMediaListControl.h"

namespace MediaControl2
{

class IBMediaControlC;
class IBMusicControlC : public IBMediaListControlC
{
	Q_OBJECT

public:
	IBMusicControlC();
	~IBMusicControlC();
	//get the object of "IBMusicControlC", notice: you can not get the object from "new"
	static IBMusicControlC *object();
	//release the object of "IBMusicControlC"
	static void releaseObject();		

	void setTimeFormat(MEDIA_TIME_FORMAT format){IBMediaListControlC::setTimeFormat(format);}

private:
	static IBMusicControlC *thisObject;
	

};





}

#endif
