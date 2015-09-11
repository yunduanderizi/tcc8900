#ifndef  IBMOVIE_CONTROL_H_20110824
#define  IBMOVIE_CONTROL_H_20110824

#include <QString>
#include <QObject>
#include <qfile.h>
#include <QMetaType>  
#include "ibMediaDefine.h"
#include "ibMediaListControl.h"

namespace MediaControl2
{

class IBMediaListControlC;
class IBMovieControlC : public IBMediaListControlC
{
	Q_OBJECT

public:
	//get the object of "IBMovieControlC", notice: you can not get the object from "new"
	static IBMovieControlC *object();
	//release the object of "IBMovieControlC"
	static void releaseObject();		


	void setFullScreenWindowMode(bool bFullScreen){IBMediaListControlC::setFullScreenWindowMode(bFullScreen);}
	bool isFullScreenWindowMode(void){return IBMediaListControlC::isFullScreenWindowMode();}
	void openVideoScreen(void){IBMediaListControlC::openVideoScreen();}
	void closeVideoScreen(void){IBMediaListControlC::closeVideoScreen();}

	

private:
	static IBMovieControlC *thisObject;
	
	IBMovieControlC();
	~IBMovieControlC();


	bool		m_bFullScreen;		
};





}

#endif
