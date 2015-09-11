
#include "ibMediaControl.h"
#include "ibStoryControl.h"

namespace MediaControl2
{

IBStoryControlC *IBStoryControlC::thisObject = NULL;
IBStoryControlC *IBStoryControlC::object()
{
	IBStoryControlC *pTmp = NULL;

	if( !thisObject )
	{
		thisObject = new IBStoryControlC();
	}

	pTmp = thisObject;
	
	return pTmp;
}

void IBStoryControlC::releaseObject()
{
	if( thisObject )
	{
		delete thisObject;
		thisObject = NULL;
	}
}
	
IBStoryControlC::IBStoryControlC()
	: IBMediaListControlC(MEDIA_TYPE_STORY)
{

}

IBStoryControlC::~IBStoryControlC()
{

}















}

