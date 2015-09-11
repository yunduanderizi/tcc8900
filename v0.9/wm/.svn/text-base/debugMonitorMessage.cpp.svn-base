#include "debugMonitorMessage.h"

void DebugMonitorReqC::print(FILE *fp)
{
	if(fileName != NULL)
		fprintf(fp,"\t debugMonitor get Req the Req-fileName=%s\n", fileName);
	else
		fprintf(fp,"\t debugMonitor get Req the Req-fileName is NULL\n");
	
	fprintf(fp, "\t debugMonitor get Req the debugLevel = %d\n", debugLevel);
	
	fprintf(fp, "\t debugMonitor get Req the oper = %d\n", oper);
}
