
#include "dbusMessage.h"
#include "debugMonitor.h"
#include "bsManagerMessage.h"
#include "dbusApplication.h"
#include "bsServer.h"

int main(int argc, char *argv[])
{
	DBusApplicationC app(argc, argv);
	initBsManager();
	IBD_INIT("bsManagerProcess", BsServerC::object()->getDbusServer());

	//apply for resources
	
	int retval = app.loop();

	//release for resources
	
	return retval;	
}

