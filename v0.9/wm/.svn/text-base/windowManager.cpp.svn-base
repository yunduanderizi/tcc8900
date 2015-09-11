
#include "windowManager.h"
#include "WmMessage.h"
#include "debugMonitor.h"


WindowManager::WindowManager(ManagedWindow *mainWindow,DBusServerC *guiServer)
{
	strcpy(bottomWindow.processName,"gui");
	strcpy(bottomWindow.windowName,"");

	bottomWindow.client=NULL;
	bottomWindow.window=mainWindow;
	
	bottomWindow.next=NULL;
	bottomWindow.windowMode =WMS_WINDOW_MODE_FULL;
	topWindow=&bottomWindow;

	appClientHead.next=NULL;

	thisWindowManager=this;
	guiServer->registerService(REQ_ID_WMS_WINDOW_MANAGEMNT,windowManagerHandler,WMSWindowManagementReqC::factory);

	pthread_mutexattr_t     mutexattr;

        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init (&m_lock,&mutexattr);
}

void WindowManager::managerGoMainMenu()
{
	WindowElement	winInfo;

	bottomWindow.window->managerShow(0x83);

	if(topWindow==&bottomWindow)
		return;

	while(1)
	{
		int result=deleteTopWindow(winInfo);

		//printf("processName:%s windowName:%s\n",winInfo.processName,winInfo.windowName);

		if(result==1)
                {
                        bottomWindow.window->topWindowChanged(topWindow->processName,topWindow->windowName);
			return;
		}

		if(winInfo.window!=NULL)
		{
			winInfo.window->managerHide(1);
			continue;
		}	

		WMCWindowManagementReqC	req;

		strcpy(req.processName,winInfo.processName);
		strcpy(req.windowName,winInfo.windowName);
		req.op =WMC_OP_HIDE;

		winInfo.client->sendService(&req);
        }
}

void WindowManager::managerGoBack()
{
	managerGoBack(1);
}

void WindowManager::managerGoBack(int notifyTopWindow)
{
	WindowElement	winInfo;

	int result=deleteTopWindow(winInfo);

	if(result==1)
	{
		bottomWindow.window->managerShow(0x83);
		return;
	}

	if(winInfo.window!=NULL)
	{
		winInfo.window->managerBack(0);
	}	
	else
	{
		WMCWindowManagementReqC	req;

		strcpy(req.processName,winInfo.processName);
		strcpy(req.windowName,winInfo.windowName);
		req.op =WMC_OP_BACK;

		winInfo.client->sendService(&req);
	}

	if(notifyTopWindow)
	{
		showTopWindow();
		bottomWindow.window->topWindowChanged(topWindow->processName,topWindow->windowName);
	}
}

void WindowManager::managerShow(char *windowName,ManagedWindow *window,char windowMode)
{
	WindowElement	topWinInfo;
	findTopWindow(topWinInfo);

	window->managerShow(0x01);

	if(windowMode==WMS_WINDOW_MODE_MIDDLE)
		bottomWindow.window->managerShow(0x80);
	else if(windowMode==WMS_WINDOW_MODE_HALF)
		bottomWindow.window->managerShow(0x81);
	else if(windowMode==WMS_WINDOW_MODE_FULL)
		bottomWindow.window->managerShow(0x82);

	addToTop((char*)"gui",windowName,window,windowMode);

	if(strcmp(topWinInfo.processName,topWindow->processName)!=0 || strcmp(topWinInfo.windowName,topWindow->windowName)!=0)
	{
//printf("old top=%s:%s new=%s%s\n",topWinInfo.processName,topWinInfo.windowName,topWindow->processName,topWindow->windowName);

		bottomWindow.window->topWindowChanged(topWindow->processName,topWindow->windowName);

		if(topWinInfo.client!=NULL)
		{
			WMCWindowManagementReqC	req;

			strcpy(req.processName,topWinInfo.processName);
			strcpy(req.windowName,topWinInfo.windowName);
			req.op =WMC_OP_COVERED;

			topWinInfo.client->sendService(&req);
		}
		else 
		{
			topWinInfo.window->managerHide(2);
		}
	}
}

void WindowManager::managerHide(char *windowName)
{
	WindowElement	winInfo;
	int	result=findWindow((char*)"gui",windowName,winInfo);

	if(result==0)
	{
		int isTop=0;
		if(strcmp((char*)"gui",topWindow->processName)==0 && strcmp(windowName,topWindow->windowName)==0)
			isTop=1;	

		winInfo.window->managerHide(0);
		deleteWindow((char*)"gui",windowName);

		if(isTop)
		{
			showTopWindow();
			bottomWindow.window->topWindowChanged(topWindow->processName,topWindow->windowName);
		}
	}
}

void WindowManager::managerHide(char *processName,char *windowName)
{
	WindowElement	winInfo;
	int	result=findWindow(processName,windowName,winInfo);

	if(result==0)
	{
		int isTop=0;
		if(strcmp(processName,topWindow->processName)==0 && strcmp(windowName,topWindow->windowName)==0)
			isTop=1;	

		if(winInfo.window!=NULL)
		{
			winInfo.window->managerHide(0);
		}
		else
		{
			WMCWindowManagementReqC	req;

			strcpy(req.processName,winInfo.processName);
			strcpy(req.windowName,winInfo.windowName);
			req.op =WMC_OP_HIDE;

			winInfo.client->sendService(&req);
		}
		deleteWindow(processName,windowName);

		if(isTop)
		{
			showTopWindow();
			bottomWindow.window->topWindowChanged(topWindow->processName,topWindow->windowName);
		}
	}
}
void WindowManager::managerGetTopWindow(char processName[30],char windowName[30])
{
        strcpy(processName,topWindow->processName);
        strcpy(windowName,topWindow->windowName);
}

int WindowManager::managerSendKeyToTopWindow(int keyCode,int modifier)
{
	WindowElement	winInfo;

	int result =findTopWindow(winInfo);

	if(winInfo.window!=NULL)
	{
		winInfo.window->managerKey(keyCode,modifier);
		result =0;
	}
	else
	{
		WMCSendKeyToTopReqC	req;

		strcpy(req.processName,winInfo.processName);
		strcpy(req.windowName, winInfo.windowName);

		req.keyCode =keyCode;
		req.modifier=modifier;

		result =winInfo.client->sendService(&req);

		if(result<0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "error here. result=%d\n",result);
		}

	}
	
	return result;
}


WindowManager * WindowManager::thisWindowManager=NULL;
void WindowManager::windowManagerHandler(DBusRequestC *pRequest, DBusReplyC *&pReply)
{
        if(pRequest->getIdentifier()!=REQ_ID_WMS_WINDOW_MANAGEMNT)
                return;

//printf("WindowManager::windowManagerH\n");

        WMSWindowManagementReqC *req=(WMSWindowManagementReqC*)pRequest;

        switch(req->op)
        {
                case WMS_OP_SHOW_MIDDLE:
                {
			thisWindowManager->applicationShow(req->processName,req->windowName,pReply,WMS_WINDOW_MODE_MIDDLE);
                        break;
                }
                case WMS_OP_SHOW_HALF:
                {
			thisWindowManager->applicationShow(req->processName,req->windowName,pReply,WMS_WINDOW_MODE_HALF);
                        break;
                }
                case WMS_OP_SHOW:
                {
			thisWindowManager->applicationShow(req->processName,req->windowName,pReply,WMS_WINDOW_MODE_FULL);
                        break;
                }
                case WMS_OP_HIDE:
                {
			thisWindowManager->applicationHide(req->processName,req->windowName,pReply);
                        break;
                }
                case WMS_OP_BACK:
                case WMS_OP_BACK_SUBWINDOW:
                {
			thisWindowManager->applicationGoBack(req->processName,req->windowName,req->op,pReply);
                        break;
                }
                case WMS_OP_MAIN:
                {
			thisWindowManager->applicationGoMainMenu(req->processName,req->windowName,pReply);
                        break;
                }
                case WMS_OP_GET_TOP_WINDOW:
                {
			thisWindowManager->applicationGetTopWindow(pReply);
                        break;
                }
        }
}

void WindowManager::showTopWindow()
{
	WindowElement	winInfo;

	while(1)
	{
		int result=findTopWindow(winInfo);

		if(result==1)
		{
			bottomWindow.window->managerShow(0x83);
			return;
		}

		if(winInfo.window!=NULL)
		{
			winInfo.window->managerShow(0x01);

			if(winInfo.windowMode==WMS_WINDOW_MODE_MIDDLE)
				bottomWindow.window->managerShow(0x80);
			else if(winInfo.windowMode==WMS_WINDOW_MODE_HALF)
				bottomWindow.window->managerShow(0x81);
			else if(winInfo.windowMode==WMS_WINDOW_MODE_FULL)
				bottomWindow.window->managerShow(0x82);

			break;
		}	

		WMCWindowManagementReqC	req;

		strcpy(req.processName,winInfo.processName);

		if(strlen(winInfo.windowName)==0)
			strcpy(req.windowName,"1@2@3@4@5@");
		else
			strcpy(req.windowName,winInfo.windowName);

		req.op =WMC_OP_SHOW;

#ifndef WM_DDT_GPS
		result=winInfo.client->sendService(&req);

		if(result<0)
		{
			IBD_PRINTF(DEBUG_INFOMATION, "error here. result=%d\n",result);

			deleteWindow(winInfo.processName,winInfo.windowName);
		}
		else
		{
			break;
		}
#else
		if(strcmp(winInfo.processName,"Navi")!=0)
		{
			result=winInfo.client->sendService(&req);

			if(result<0)
			{
				IBD_PRINTF(DEBUG_INFOMATION, "error here. result=%d\n",result);
	
				deleteWindow(winInfo.processName,winInfo.windowName);
			}
			else
			{
				break;
			}
		}
		else
		{
			DBusReplyC              *rep=NULL;
			result=winInfo.client->sendService(&req,rep,5*1000);

                	if(result<0 || rep->getIdentifier()!=REP_ID_WMC_WINDOW_MANAGEMNT)
                	{
                        	IBD_PRINTF(DEBUG_INFOMATION, "error here. result=%d\n",result);

                        	deleteWindow(winInfo.processName,winInfo.windowName);
                	}
                	else
                	{
                        	WMCWindowManagementRepC *reply=(WMCWindowManagementRepC*)rep;
				if(strcmp(winInfo.processName,topWindow->processName)==0 && strcmp(winInfo.windowName,topWindow->windowName)==0 && reply->result>=0)
                        	{
                                	if(reply->result==0)
                                        	bottomWindow.window->managerShow(0x80);
                                	else if(reply->result==1)
                                        	bottomWindow.window->managerShow(0x81);
                                	else
                                        	bottomWindow.window->managerShow(0x82);

                                	delete rep;
                                	break;
                        	}

                        	deleteWindow(winInfo.processName,winInfo.windowName);
                        	delete rep;
                	}
		}
#endif
	}
}

void WindowManager::managerGoApplication(char *processName,DBusClientProxyC *client)
{
	WMCWindowManagementReqC	req;
	DBusReplyC		*rep=NULL;

	strcpy(req.processName,processName);
	strcpy(req.windowName,(char*)"");
	req.op =WMC_OP_SHOW;

	int result=client->sendService(&req,rep,5*1000);


	if(result<0 || rep->getIdentifier()!=REP_ID_WMC_WINDOW_MANAGEMNT)
	{
		IBD_PRINTF(DEBUG_INFOMATION, "error here. result=%d\n",result);
	}

#ifdef WM_DDT_GPS
	else if(strcmp(processName,"Navi")==0) 
	{
		WindowElement   topWinInfo;
        	findTopWindow(topWinInfo);

		WMCWindowManagementRepC *reply=(WMCWindowManagementRepC*)rep;

                if(reply->result<0)
                {
                       //deleteWindow(processName,(char*)"");
                }
                else
                {
			int windowFlag =0x80;

                        if(reply->result==0)
                        {
                                //changeWindowMode(processName,(char*)"",WMS_WINDOW_MODE_MIDDLE);
				addToTop(processName,(char*)"",client,WMS_WINDOW_MODE_MIDDLE);
                                windowFlag =0x80;
                        }
                        else if(reply->result==1)
                        {
                                //changeWindowMode(processName,(char*)"",WMS_WINDOW_MODE_HALF);
				addToTop(processName,(char*)"",client,WMS_WINDOW_MODE_HALF);
                                windowFlag =0x81;
                        }
                        else
                        {
                                //changeWindowMode(processName,(char*)"",WMS_WINDOW_MODE_FULL);
				addToTop(processName,(char*)"",client,WMS_WINDOW_MODE_FULL);
                                windowFlag =0x82;
                        }

                        bottomWindow.window->managerShow(windowFlag);

			if(strcmp(processName,topWinInfo.processName)!=0 || strcmp("",topWinInfo.windowName)!=0)
			{
				{
                        		bottomWindow.window->topWindowChanged(topWindow->processName,topWindow->windowName);

                        		if(topWinInfo.client!=NULL)
                        		{
                                		WMCWindowManagementReqC req;

                                		strcpy(req.processName,topWinInfo.processName);
                                		strcpy(req.windowName,topWinInfo.windowName);
                                		req.op =WMC_OP_COVERED;

                                		topWinInfo.client->sendService(&req);
                        		}
                        		else if(topWinInfo.window!=NULL)
                        		{
                                		topWinInfo.window->managerHide(2);
                        		}
				}
			}
                }
	}
#endif

	delete rep;
}

void WindowManager::applicationGoMainMenu(char */*processName*/,char */*windowName*/,DBusReplyC *&reply)
{
	managerGoMainMenu();

     	WMSWindowManagementRepC *rep=new WMSWindowManagementRepC;
     	rep->result=0;
     	reply=rep;
}

void WindowManager::applicationGoBack(char */*processName*/,char */*windowName*/,char /*op*/,DBusReplyC *&reply)
{
	WMSWindowManagementRepC *rep=new WMSWindowManagementRepC;
        reply=rep;
        rep->result=0;

	managerGoBack();
}

void WindowManager::applicationShow(char *processName,char *windowName,DBusReplyC *&reply,char windowMode)
{
	WMSWindowManagementRepC *rep=new WMSWindowManagementRepC;
	rep->result=0;
	reply=rep;

	WindowElement	topWinInfo;
	findTopWindow(topWinInfo);

	DBusClientProxyC *client=findDBusClient(processName);
	if(client)
	{
		if(windowMode == WMS_WINDOW_MODE_MIDDLE)
			bottomWindow.window->managerShow(0x80);
		else if(windowMode == WMS_WINDOW_MODE_HALF)
			bottomWindow.window->managerShow(0x81);
		else if(windowMode==WMS_WINDOW_MODE_FULL)
			bottomWindow.window->managerShow(0x82);

		addToTop(processName,windowName,client,windowMode);
	}
	else
	{
		IBD_PRINTF(DEBUG_INFOMATION, "Application show: Cann't find DBus client................\n");
		return;
	}


	bottomWindow.window->topWindowChanged(topWindow->processName,topWindow->windowName);

	if(strcmp(topWinInfo.processName,topWindow->processName)!=0 || strcmp(topWinInfo.windowName,topWindow->windowName)!=0)
	{
		if(topWinInfo.window!=NULL)
		{
			topWinInfo.window->managerHide(2);
		}
		else
		{
			WMCWindowManagementReqC	req;

			strcpy(req.processName,topWinInfo.processName);
			strcpy(req.windowName,topWinInfo.windowName);
			req.op =WMC_OP_COVERED;

			topWinInfo.client->sendService(&req);
		}

		if(topWindow->client!=NULL)
		{
                        WMCWindowManagementReqC req;

                        strcpy(req.processName,topWindow->processName);
                        strcpy(req.windowName,topWindow->windowName);
                        req.op =WMC_OP_ON_TOP;

#ifndef WM_DDT_GPS
                        topWindow->client->sendService(&req);
#else
			if(strcmp(processName,"Navi")==0)
				;//showTopWindow();
			else
                           	topWindow->client->sendService(&req);
#endif
		}
	}
}

void WindowManager::applicationHide(char *processName,char *windowName,DBusReplyC *&reply)
{
	int result =deleteWindow(processName,windowName);

        WMSWindowManagementRepC *rep=new WMSWindowManagementRepC;
        rep->result=0;
        reply=rep;

	if(result==0)
	{
		showTopWindow();

		bottomWindow.window->topWindowChanged(topWindow->processName,topWindow->windowName);
	}
}

void WindowManager::applicationGetTopWindow(DBusReplyC *&reply)
{
        WMSGetTopWindowRepC *rep=new WMSGetTopWindowRepC;
	strcpy(rep->processName,topWindow->processName);
	strcpy(rep->windowName,topWindow->windowName);
        rep->result=0;
        reply=rep;
}


int WindowManager::addToTop(char *processName,char *windowName,DBusClientProxyC  *client,char windowMode)
{
	InternalLock	lock(&m_lock);
	WindowElement	*pre=&bottomWindow;
	WindowElement	*p=bottomWindow.next;

	while(p)
	{
		if(strcmp(p->processName,processName)==0 && strcmp(p->windowName,windowName)==0)
		{
			if(p!=topWindow)
			{
				pre->next=p->next;

				p->next=NULL;

				topWindow->next=p;
				topWindow=p;
			}
			p->client=client;
			p->windowMode =windowMode;

			showWindowStack();

			return 1;
		}

		pre=p;
		p=p->next;
	}

	p=new WindowElement;
	strcpy(p->processName,processName);
	strcpy(p->windowName,windowName);

	p->client=client;
	p->window=NULL;
	p->windowMode =windowMode;
	
	p->next=NULL;
	topWindow->next=p;
	topWindow=p;
		
	showWindowStack();

	return 0;
}

int WindowManager::addToTop(char *processName,char *windowName,ManagedWindow *window,char windowMode)
{
	InternalLock	lock(&m_lock);
	WindowElement	*pre=&bottomWindow;
	WindowElement	*p=bottomWindow.next;

	while(p)
	{
		if(strcmp(p->processName,processName)==0 && strcmp(p->windowName,windowName)==0)
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

			showWindowStack();

			return 1;
		}

		pre=p;
		p=p->next;
	}

	p=new WindowElement;
	strcpy(p->processName,processName);
	strcpy(p->windowName,windowName);

	p->client=NULL;
	p->window=window;
	p->windowMode=windowMode;
	
	p->next=NULL;
	topWindow->next=p;
	topWindow=p;
		
	showWindowStack();

	return 0;
}

int WindowManager::changeWindowMode(char *processName,char *windowName,char windowMode)
{
	InternalLock	lock(&m_lock);
	WindowElement	*p=bottomWindow.next;

	while(p)
	{
		if(strcmp(p->processName,processName)==0 && strcmp(p->windowName,windowName)==0)
		{
			p->windowMode=windowMode;

			return 0;
		}
		p=p->next;
	}

	return -1;
}

int WindowManager::deleteWindow(char *processName,char *windowName)
{
	InternalLock	lock(&m_lock);
	WindowElement	*pre=&bottomWindow;
	WindowElement	*p=bottomWindow.next;

	while(p)
	{
		if(strcmp(p->processName,processName)==0 && strcmp(p->windowName,windowName)==0)
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
		return 1;
	}
}

int WindowManager::findWindow(char *processName,char *windowName,WindowElement &winInfo)
{
	InternalLock	lock(&m_lock);
	WindowElement	*p=&bottomWindow;

	while(p)
	{
		if(strcmp(p->processName,processName)==0 && strcmp(p->windowName,windowName)==0)
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

int WindowManager::findWindowBefore(char *processName,char *windowName,WindowElement &winInfo)
{
	InternalLock	lock(&m_lock);
	WindowElement	*pre=&bottomWindow;
	WindowElement	*p=bottomWindow.next;

	while(p)
	{
		if(strcmp(p->processName,processName)==0 && strcmp(p->windowName,windowName)==0)
		{
			winInfo=*pre;
			if(pre==&bottomWindow)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		pre=p;
		p=p->next;
	}
	return -1;
}

int WindowManager::findTopWindow(WindowElement &winInfo)
{
	InternalLock	lock(&m_lock);
	winInfo=*topWindow;

	if(topWindow==&bottomWindow)
		return 1;
	else
		return 0;
}

int WindowManager::deleteTopWindow(WindowElement &winInfo)
{
	InternalLock	lock(&m_lock);
	WindowElement	*pre=&bottomWindow;
	WindowElement	*p=bottomWindow.next;

	if(topWindow==&bottomWindow)
	{
		winInfo=bottomWindow;
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

int WindowManager::deleteTopWindow()
{
	InternalLock	lock(&m_lock);
	WindowElement	*pre=&bottomWindow;
	WindowElement	*p=bottomWindow.next;

	if(topWindow==&bottomWindow)
	{
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

	return 0;
}

void WindowManager::showWindowStack()
{
	WindowElement	*p=&bottomWindow;
	int		level=0;

	IBD_LOOP_BEGIN(DEBUG_INFOMATION, "Current window stack:\n");
	while(p)
	{
		IBD_LOOP_PRINTF(DEBUG_INFOMATION, "\t%d\t %s:%s %d\n",level++,p->processName,p->windowName,p->windowMode);
		p=p->next;
	}
	IBD_LOOP_END
}

void  WindowManager::addDBusClient(char *processName,DBusClientProxyC *client)
{
	InternalLock	lock(&m_lock);
	AppDBusClient   *appClient=new AppDBusClient;

	strcpy(appClient->processName,processName);
	appClient->client =client;

	appClient->next   =appClientHead.next;
	appClientHead.next=appClient;

	client->registerReply(REP_ID_WMS_WINDOW_MANAGEMNT,WMSWindowManagementRepC::factory);
}

DBusClientProxyC* WindowManager::findDBusClient(char *processName)
{
	InternalLock	lock(&m_lock);
	AppDBusClient	*appClient=appClientHead.next;

	while(appClient!=NULL)
	{
		if(strcmp(processName,appClient->processName)==0)
			return appClient->client;

		appClient=appClient->next;
	}

	return NULL;
}

WindowManager::InternalLock::InternalLock(pthread_mutex_t* lock)
{
	m_lock=lock;
	pthread_mutex_lock(m_lock);
}
WindowManager::InternalLock::~InternalLock()
{
	pthread_mutex_unlock(m_lock);
}
