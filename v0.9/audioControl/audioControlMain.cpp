
#include "debugMonitor.h"
#include "dbusApplication.h"
#include "audioControl.h"
#include "audioDevice.h"
#include "myDbus.h"






int mytest = 0;

int main(int argc, char *argv[])
{
	DBusApplicationC app(argc, argv);

	AudioControl1::AudioControlC::object();

	AudioControl1::AudioDeviceC::object();
	
	AudioControl1::MyDbusC::object();
	
	IBD_INIT("audioControl", AudioControl1::MyDbusC::object()->getDbusServer());

	int retval = app.loop();

	AudioControl1::AudioControlC::releaseObject();

	AudioControl1::AudioDeviceC::releaseObject();

	AudioControl1::MyDbusC::releaseObject();
	
	return retval; 
}


