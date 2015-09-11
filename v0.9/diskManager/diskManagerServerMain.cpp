#include <stdio.h>
#include "diskManagerMessage.h"
#include "dbusApplication.h"
#include "diskManagerServer.h"

int main(int argc,char *argv[])
{
	DBusApplicationC app(argc, argv);
	//QApplication a(argc, argv);
	// create the DBUS server and client
	DBusServerC server(ServerDiskManager, ObjectDiskManager);
	DiskManagerServer *pDiskManagerServer;
	pDiskManagerServer = new DiskManagerServer(&server);

	if (server.connect() < 0)
	{
		printf("DiskManager Server Cann't connect to DBUS server.\n");
		return -1;
	}

	pDiskManagerServer->initDiskManagerServer();
	return app.loop();
	//return a.exec();
}

