
#include "ibMediaControl.h"
#include "ibNetMusicControl.h"

namespace MediaControl2
{

IBNetMusicControlC *IBNetMusicControlC::thisObject = NULL;
IBNetMusicControlC *IBNetMusicControlC::object()
{
	IBNetMusicControlC *pTmp = NULL;

	if( !thisObject )
	{
		thisObject = new IBNetMusicControlC();
	}

	pTmp = thisObject;
	
	return pTmp;
}

void IBNetMusicControlC::releaseObject()
{
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
}
	
IBNetMusicControlC::IBNetMusicControlC()
	: IBMediaListControlC(MEDIA_TYPE_NET_MUSIC)
{

}

IBNetMusicControlC::~IBNetMusicControlC()
{

}















}

