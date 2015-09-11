#include "DiskManageMessage.h"

void DiskManageReqC::print(FILE *fp)
{
	//fprintf(fp,"\tdevname=[%s]\n\n",devname);
}

void DiskManageRepC::print(FILE *fp)
{
	fprintf(fp,"\tdiskcount=[%d]\n\n",diskcount);
}

DiskManagerNotifyC::DiskManagerNotifyC():DBusNotifyC(NOTF_ID_DISK)
{
}
DiskManagerNotifyC::~DiskManagerNotifyC()
{
}
void DiskManagerNotifyC::print(FILE *fp)
{
	fprintf(fp,"\tdiskdevice=[%s %d %d %d %d]\n\n",diskdevice.disk_name, diskdevice.number, diskdevice.nr_partition,diskdevice.disk_type, diskdevice.disk_status);
}

