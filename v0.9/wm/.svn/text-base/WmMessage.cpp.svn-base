#include "WmMessage.h"

void WMCWindowManagementReqC::print(FILE *fp)
{
	char msg[30] = "";
	if(op == 0)
		strcpy(msg,"wmc Request nothing");
	else if(op == WMC_OP_SHOW)
		strcpy(msg,"wmc Request show");
	else if(op == WMC_OP_HIDE)
		strcpy(msg,"wmc Request hide");
	else if(op == WMC_OP_BACK)
		strcpy(msg,"wmc Request back");
	else if(op == WMC_OP_COVERED)
		strcpy(msg,"wmc notify covered");
	else if(op == WMC_OP_ON_TOP)
		strcpy(msg,"wmc notify ON TOP");
	else
		sprintf(msg,"wmc Request opcode %d",op);
	
        fprintf(fp,"\t processName=%s\n",processName);
        fprintf(fp,"\t windowName=%s\n",windowName);
        fprintf(fp,"\t op=%s\n\n",msg);
}

void WMCSendKeyToTopReqC::print(FILE *fp)
{
        fprintf(fp,"\t processName=%s\n",processName);
        fprintf(fp,"\t windowName=%s\n",windowName);

        fprintf(fp,"\t keyCode=%d\n",keyCode);
        fprintf(fp,"\t modifier=%d\n\n",modifier);
}

void WMSWindowManagementReqC::print(FILE *fp)
{
	char msg[30] = "";
	if(op == WMS_OP_SHOW_HALF)
		strcpy(msg,"wms Request show half window");
	if(op == WMS_OP_SHOW_MIDDLE)
		strcpy(msg,"wms Request show middle window");
	else if(op == WMS_OP_SHOW)
		strcpy(msg,"wms Request show full window");
	else if(op == WMS_OP_HIDE)
		strcpy(msg,"wms Request hide");
	else if(op == WMS_OP_BACK)
		strcpy(msg,"wms Request back");
	else if(op == WMS_OP_BACK_SUBWINDOW)
		strcpy(msg,"wms Request back subwindow");
	else if(op == WMS_OP_MAIN)
		strcpy(msg,"wms Request main");
	else if(op == WMS_OP_GET_TOP_WINDOW)
		strcpy(msg,"wms get top window");
	else
		sprintf(msg,"wmc Request opcode %d",op);

        fprintf(fp,"\t processName=%s\n",processName);
        fprintf(fp,"\t windowName=%s\n",windowName);
        fprintf(fp,"\t op=%s\n\n",msg);
}

void WMSGetTopWindowRepC::print(FILE *fp)
{
        fprintf(fp,"\t processName=%s\n",processName);
        fprintf(fp,"\t windowName=%s\n",windowName);
}
