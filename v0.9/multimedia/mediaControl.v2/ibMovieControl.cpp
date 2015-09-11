
#include "ibMediaControl.h"
#include "ibMovieControl.h"
//#include <sqlite3.h>

namespace MediaControl2
{

IBMovieControlC *IBMovieControlC::thisObject = NULL;
IBMovieControlC *IBMovieControlC::object()
{
	IBMovieControlC *pTmp = NULL;

	if( !thisObject )
	{
		thisObject = new IBMovieControlC();
	}

	pTmp = thisObject;
	
	return pTmp;
}

void IBMovieControlC::releaseObject()
{
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
}
	
IBMovieControlC::IBMovieControlC()
	: IBMediaListControlC(MEDIA_TYPE_MOVIE)
{

}

IBMovieControlC::~IBMovieControlC()
{

}












}

