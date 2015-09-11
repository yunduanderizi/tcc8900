
#include "windowManagerApp.h"
#include "WmMessage.h"
#include "debugMonitor.h"

WindowManagerApp::WindowManagerApp(ManagedWindowApp *mainWindow,char *processName,DBusServerC *appServer,DBusClientProxyC *guiClient,char windowMode)
{
	strcpy(bottomWindow.windowName,"");

	bottomWindow.window=mainWindow;
	bottomWindow.windowMode=windowMode;
	bottomWindow.windowStatus =WMC_WINDOW_STATUS_HIDE;
	
	bottomWindow.next=NULL;
	topWindow=&bottomWindow;

	this->guiClient=guiClient;
	strcpy(this->processName,processName);

	thisWindowManagerApp=this;

	guiClient->registerReply(REP_ID_WMS_WINDOW_MANAGEMNT,WMSWindowManagementRepC::factory);
	guiClient->registerReply(REP_ID_WMS_GET_TOP_WINDOW,WMSGetTopWindowRepC::factory);

	appServer->registerService(REQ_ID_WMC_WINDOW_MANAGEMNT,windowManagerHandler,WMCWindowManagementReqC::factory);
	appServer->registerService(REQ_ID_WMC_SEND_KEY_TO_TOP,windowManagerKeyHandler,WMCSendKeyToTopReqC::factory);

	pthread_mutexattr_t     mutexattr;

        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init (&m_lock,&mutexattr);
}

void WindowManagerApp::applicationGoMainMenu()
{
	WMSWindowManagementReqC	req;

	strcpy(req.processName,processName);
	strcpy(req.windowName,"XX**");
	req.op =WMS_OP_MAIN;

	guiClient->sendService(&req);
}

void WindowManagerApp::applicationGoBack()
{
	WMSWindowManagementReqC req;

        strcpy(req.processName,processName);
        strcpy(req.windowName,"XX**");
        req.op =WMS_OP_BACK;

        guiClient->sendService(&req);
}

void WindowManagerApp::applicationShow(char *windowName,ManagedWindowApp *window,char windowMode)
{
	WMSWindowManagementReqC req;

	window->managerShow(0);

        strcpy(req.processName,processName);
        strcpy(req.windowName,windowName);

	if(windowMode==WMC_WINDOW_MODE_MIDDLE)
        	req.op =WMS_OP_SHOW_MIDDLE;
	else if(windowMode==WMC_WINDOW_MODE_HALF)
        	req.op =WMS_OP_SHOW_HALF;
	else
        	req.op =WMS_OP_SHOW;

        guiClient->sendService(&req);
	
	addToTop(windowName,window,windowMode);
}

void WindowManagerApp::applicationHide(char *windowName)
{
	WindowElementApp   winInfo;
        int     result=findWindow(windowName,winInfo);

        if(result>=0)
        {
                winInfo.window->managerHide(0);
		deleteWindow(windowName);
        }

	WMSWindowManagementReqC req;

        strcpy(req.processName,processName);
        strcpy(req.windowName,windowName);
        req.op =WMS_OP_HIDE;

        guiClient->sendService(&req);
}

int  WindowManagerApp::getTopWindow(char processName[30],char windowName[30])
{
	WMSWindowManagementReqC req;
        DBusReplyC              *rep=NULL;

        req.processName[0]=0;
        req.windowName [0]=0;
        req.op =WMS_OP_GET_TOP_WINDOW;

        int result=guiClient->sendService(&req,rep,5*1000);
	
	if(result<0)
	{
		//error
	}
	else if(rep->getIdentifier() !=REP_ID_WMS_GET_TOP_WINDOW)
	{
		result=-200;
	}
	else
	{
		WMSGetTopWindowRepC *topWindowRep=(WMSGetTopWindowRepC*)rep;
		strcpy(processName,topWindowRep->processName);
		strcpy(windowName,topWindowRep->windowName);

		result =0;	
	}
	delete rep;
	return result;
}

WindowManagerApp * WindowManagerApp::thisWindowManagerApp=NULL;
void WindowManagerApp::windowManagerHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
        if(pRequest->getIdentifier()!=REQ_ID_WMC_WINDOW_MANAGEMNT)
                return;

        WMCWindowManagementReqC *req=(WMCWindowManagementReqC*)pRequest;

	if(strcmp(req->processName,thisWindowManagerApp->processName)!=0)
	{
		WMCWindowManagementRepC *rep=new WMCWindowManagementRepC;
                rep->result=-1;
		pReply	   =rep;

		IBD_PRINTF(DEBUG_INFOMATION, "Process Name is not correct. this=%s,recv=%s\n",thisWindowManagerApp->processName,req->processName);
		
		return;
	}

        switch(req->op)
        {
                case WMC_OP_SHOW:
                {
			thisWindowManagerApp->managerRequestShow(req->windowName,pReply);
                        break;
                }
                case WMC_OP_HIDE:
                {
			thisWindowManagerApp->managerRequestHide(req->windowName,pReply);
                        break;
                }
                case WMC_OP_BACK:
                {
			thisWindowManagerApp->managerRequestGoBack(req->windowName,pReply);
                        break;
                }
                case WMC_OP_COVERED:
                {
			thisWindowManagerApp->managerNotifyCovered(req->windowName,pReply);
                        break;
		}
                case WMC_OP_ON_TOP:
                {
			thisWindowManagerApp->managerNotifyOnTop(req->windowName,pReply);
                        break;
		}
        }
}

void WindowManagerApp::windowManagerKeyHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
        if(pRequest->getIdentifier()!=REQ_ID_WMC_SEND_KEY_TO_TOP)
                return;

        WMCSendKeyToTopReqC *req=(WMCSendKeyToTopReqC*)pRequest;
	WMCWindowManagementRepC *rep=new WMCWindowManagementRepC;
        rep->result=-1;
	pReply	   =rep;

	if(strcmp(req->processName,thisWindowManagerApp->processName)!=0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Process Name is not correct. this=%s,recv=%s\n",thisWindowManagerApp->processName,req->processName);
		return;
	}

	WindowElementApp	winInfo;
	int result=thisWindowManagerApp->findWindow(req->windowName,winInfo);

	if(result<0)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Manager reuest window does not exist. windowName=%s\n",req->windowName);
		return;
	}

	winInfo.window->managerKey(req->keyCode,req->modifier);

        rep->result=0;
}

void WindowManagerApp::showTopWindow()
{
}

void WindowManagerApp::managerRequestGoBack(char *windowName,DBusReplyC *&reply)
{
	WMCWindowManagementRepC *rep=new WMCWindowManagementRepC;
        reply=rep;

	WindowElementApp	winInfo;
	int result=findWindow(windowName,winInfo);

	if(result<0)
	{
		rep->result=-1;

		IBD_PRINTF(DEBUG_INFOMATION, "Manager reuest window do not exit. windowName=%s\n",windowName);
		return;
	}

	int backResult;

	if(winInfo.window->hasSubWindow())
		backResult=winInfo.window->managerBack(1);
	else
		backResult=winInfo.window->managerBack(0);

	if(backResult ==1)
	{
		rep->result=1;
		deleteWindow(winInfo.windowName);
	}
	else if(backResult ==-2)
	{
		changeWindowStatus(windowName,WMC_WINDOW_STATUS_HIDE);
		rep->result=-2;
	}
	else
	{
		rep->result=0;
		deleteWindow(winInfo.windowName);
	}
}

void WindowManagerApp::managerRequestShow(char *windowName,DBusReplyC *&reply)
{
	WMCWindowManagementRepC *rep=new WMCWindowManagementRepC;
        reply=rep;
	reply->result=0;

	if(strcmp(windowName,"")==0)
	{
		applicationShow(bottomWindow.windowName,bottomWindow.window,bottomWindow.windowMode);

		WindowElementApp	*p;
		{
			InternalLock    lock(&m_lock);
			p=bottomWindow.next;
			bottomWindow.next=NULL;
			topWindow	 =&bottomWindow;
		}

		while(p)
		{
			applicationShow(p->windowName,p->window,p->windowMode);

			WindowElementApp  *temp=p;
			p=p->next;
			delete temp;
		}

		return;
	}
	else if(strcmp(windowName,"1@2@3@4@5@")==0)
	{
//printf("windowName 1@2@3@4@5@================\n");
		applicationShow(bottomWindow.windowName,bottomWindow.window,bottomWindow.windowMode);

		return;
	}

	WindowElementApp	winInfo;
	int result=findWindow(windowName,winInfo);

	if(result<0)
	{
		rep->result=-1;

		IBD_PRINTF(DEBUG_INFOMATION, "Manager reuest window do not exit. windowName=%s\n",windowName);
		return;
	}

	applicationShow(winInfo.windowName,winInfo.window,winInfo.windowMode);
}

void WindowManagerApp::managerRequestHide(char *windowName,DBusReplyC *&reply)
{
        WMSWindowManagementRepC *rep=new WMSWindowManagementRepC;
        rep->result=0;
        reply=rep;

	WindowElementApp	winInfo;
	int result=findWindow(windowName,winInfo);

	if(result<0)
	{
		rep->result=-1;

		IBD_PRINTF(DEBUG_INFOMATION, "Manager reuest window does not exist. windowName=%s\n",windowName);
		return;
	}

	winInfo.window->managerHide(1);

	//int hideResult=winInfo.window->managerHide(1);
	//if(hideResult !=1)
		deleteWindow(winInfo.windowName);
}

void WindowManagerApp::managerNotifyCovered(char *windowName,DBusReplyC *&reply)
{
        WMSWindowManagementRepC *rep=new WMSWindowManagementRepC;
        rep->result=0;
        reply=rep;

	WindowElementApp	winInfo;
	int result=findWindow(windowName,winInfo);

	if(result<0)
	{
		rep->result=-1;

		IBD_PRINTF(DEBUG_INFOMATION, "Manager reuest window do not exit. windowName=%s\n",windowName);
		return;
	}

	if(changeWindowStatus(windowName,WMC_WINDOW_STATUS_COVERD)==WMC_WINDOW_STATUS_SHOW)
		winInfo.window->managerHide(2);
}

void WindowManagerApp::managerNotifyOnTop(char *windowName,DBusReplyC *&reply)
{
        WMSWindowManagementRepC *rep=new WMSWindowManagementRepC;
        rep->result=0;
        reply=rep;

#ifndef WM_DDT_GPS	
        if(0==strcmp(processName,"Navi"))//gps out
            return;
#endif
	
	WindowElementApp	winInfo;
	int result=findWindow(windowName,winInfo);

	if(result>=0)
	{
		if(winInfo.windowMode==WMC_WINDOW_MODE_FULL)
		{
			changeWindowStatus(windowName,WMC_WINDOW_STATUS_SHOW);
			winInfo.window->managerShow(1);
		}
		else if(winInfo.windowStatus!=WMC_WINDOW_STATUS_SHOW)
		{
			applicationShow(winInfo.windowName,winInfo.window,winInfo.windowMode);
		}
	}
}

int WindowManagerApp::addToTop(char *windowName,ManagedWindowApp *window,char windowMode)
{
	InternalLock    lock(&m_lock);
	WindowElementApp	*pre=&bottomWindow;
	WindowElementApp	*p=bottomWindow.next;

	if(strlen(windowName)==0)
	{
		bottomWindow.window=window;
		bottomWindow.windowMode=windowMode;
		bottomWindow.windowStatus=WMC_WINDOW_STATUS_SHOW;
		return 1;
	}

	while(p)
	{
		if(strcmp(p->windowName,windowName)==0)
		{
			if(p!=topWindow)
			{
				pre->next=p->next;

				p->next=NULL;
				topWindow->next=p;
				topWindow=p;
			}
			p->window=window;
			p->windowMode=windowMode;
			p->windowStatus=WMC_WINDOW_STATUS_SHOW;

			showWindowStack();

			return 1;
		}

		pre=p;
		p=p->next;
	}

	p=new WindowElementApp;
	strcpy(p->windowName,windowName);

	p->window=window;
	p->windowMode=windowMode;
	p->windowStatus=WMC_WINDOW_STATUS_SHOW;
	
	p->next=NULL;
	topWindow->next=p;
	topWindow=p;
		
	showWindowStack();

	return 0;
}

int WindowManagerApp::deleteWindow(char *windowName)
{
	InternalLock    lock(&m_lock);
	WindowElementApp	*pre=&bottomWindow;
	WindowElementApp	*p=bottomWindow.next;

	while(p)
	{
		if(strcmp(p->windowName,windowName)==0)
		{
			pre->next=p->next;
			if(p==topWindow)
			{
				topWindow=pre;
				topWindow->next=NULL;
			}
			break;
		}

		pre=p;
		p=p->next;
	}

	showWindowStack();

	if(p)
	{
		delete p;
		return 0;
	}
	else
	{
		if(windowName[0]==0)
			bottomWindow.windowStatus =WMC_WINDOW_STATUS_HIDE;

		return -1;
	}
}

int WindowManagerApp::findWindow(char *windowName,WindowElementApp &winInfo)
{
	InternalLock    lock(&m_lock);
	WindowElementApp	*p=&bottomWindow;

	while(p)
	{
		if(strcmp(p->windowName,windowName)==0)
		{
			winInfo=*p;
			if(p==&bottomWindow)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		p=p->next;
	}
	return -1;
}
int WindowManagerApp::changeWindowStatus(char *windowName,char windowStatus)
{
	InternalLock    lock(&m_lock);
        WindowElementApp        *p=&bottomWindow;
	int		oldWindowStatus=-1;		

        while(p)
        {
                if(strcmp(p->windowName,windowName)==0)
                {
			oldWindowStatus =p->windowStatus;
			p->windowStatus =windowStatus;
			break;
                }

                p=p->next;
        }
        return oldWindowStatus;
}

int WindowManagerApp::findTopWindow(WindowElementApp &winInfo)
{
	InternalLock    lock(&m_lock);
	winInfo=*topWindow;

	if(topWindow==&bottomWindow)
		return 1;
	else
		return 0;
}

int WindowManagerApp::deleteTopWindow(WindowElementApp &winInfo)
{
	InternalLock    lock(&m_lock);
	WindowElementApp	*pre=&bottomWindow;
	WindowElementApp	*p=bottomWindow.next;

	if(topWindow==&bottomWindow)
	{
		winInfo=bottomWindow;

		showWindowStack();

		return 1;
	}

	while(p!=topWindow)
	{
		pre=p;
		p=p->next;
	}

	winInfo=*p;
	delete p;

	pre->next=NULL;
	topWindow=pre;

	showWindowStack();

	return 0;
}

int WindowManagerApp::deleteTopWindow()
{
	InternalLock    lock(&m_lock);
	WindowElementApp	*pre=&bottomWindow;
	WindowElementApp	*p=bottomWindow.next;

	if(topWindow==&bottomWindow)
	{
		showWindowStack();

		return 1;
	}

	while(p!=topWindow)
	{
		pre=p;
		p=p->next;
	}

	delete p;

	pre->next=NULL;
	topWindow=pre;

	showWindowStack();

	return 0;
}

void WindowManagerApp::showWindowStack()
{
	InternalLock    lock(&m_lock);
	WindowElementApp	*p=&bottomWindow;
	int		level=0;

	IBD_LOOP_BEGIN(DEBUG_INFOMATION, "Current APP window stack:\n");
	
	while(p)
	{
		IBD_LOOP_PRINTF(DEBUG_INFOMATION, "\t%d\t %s:%s %d %d\n",level++,processName,p->windowName,p->windowMode,p->windowStatus);
		p=p->next;
	}
	IBD_LOOP_END
}

WindowManagerApp::InternalLock::InternalLock(pthread_mutex_t* lock)
{
        m_lock=lock;
        pthread_mutex_lock(m_lock);
}
WindowManagerApp::InternalLock::~InternalLock()
{
        pthread_mutex_unlock(m_lock);
}

