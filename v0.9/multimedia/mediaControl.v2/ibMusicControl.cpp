
#include "ibMediaControl.h"
#include "ibMusicControl.h"

namespace MediaControl2
{

IBMusicControlC *IBMusicControlC::thisObject = NULL;
IBMusicControlC *IBMusicControlC::object()
{
	IBMusicControlC *pTmp = NULL;

	if( !thisObject )
	{
		thisObject = new IBMusicControlC();
	}

	pTmp = thisObject;
	
	return pTmp;
}

void IBMusicControlC::releaseObject()
{
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
}
	
IBMusicControlC::IBMusicControlC()
	: IBMediaListControlC(MEDIA_TYPE_MUSIC)
{

}

IBMusicControlC::~IBMusicControlC()
{

}















}

