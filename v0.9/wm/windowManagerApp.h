#ifndef WINDOW_MANGER_APP_H
#define WINDOW_MANGER_APP_H

#include "dbusMessage.h"
#include "dbusClientProxy.h"
#include "dbusServer.h"


#define WMC_WINDOW_MODE_MIDDLE	0
#define WMC_WINDOW_MODE_HALF	1
#define WMC_WINDOW_MODE_FULL	2

#define WMC_WINDOW_STATUS_SHOW		0
#define WMC_WINDOW_STATUS_HIDE		1
#define WMC_WINDOW_STATUS_COVERD	2


class	ManagedWindowApp
{
public:
	virtual int managerShow(int /*flag*/){return 0;}
	virtual int managerHide(int /*flag*/){return 0;}
	virtual int managerBack(int /*flag*/){return 0;}
	virtual int hasSubWindow(){return 0;}

	virtual int managerKey(int /*keyCode*/,int /*modifier*/){return 0;}
};
	

typedef struct _WindowElementApp
{
	char 		  windowName[30];
	ManagedWindowApp *window;	
	char		  windowMode;
	char		  windowStatus;

	struct _WindowElementApp *next;
}WindowElementApp;


class WindowManagerApp
{
public:
	WindowManagerApp(ManagedWindowApp *mainWindow,char *processName,DBusServerC *appServer,DBusClientProxyC *guiClient,char windowMode=WMC_WINDOW_MODE_MIDDLE);

public:
	void applicationGoMainMenu();
	void applicationGoBack();
	void applicationShow(char *windowName,ManagedWindowApp *window,char windowMode=WMC_WINDOW_MODE_MIDDLE);
	void applicationHide(char *windowName);

	int  getTopWindow(char processName[30],char windowName[30]);

private:
	static WindowManagerApp *thisWindowManagerApp;
	static void windowManagerHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);
	static void windowManagerKeyHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);

	void managerRequestGoBack(char *windowName,DBusReplyC *&reply);
	void managerRequestShow(char *windowName,DBusReplyC *&reply);
	void managerRequestHide(char *windowName,DBusReplyC *&reply);
	void managerNotifyCovered(char *windowName,DBusReplyC *&reply);
	void managerNotifyOnTop(char *windowName,DBusReplyC *&reply);

private:
	void showTopWindow();

private:
	int addToTop(char *windowName,ManagedWindowApp *window,char windMode);

	int deleteWindow(char *windowName);

	int findWindow(char *windowName,WindowElementApp &winInfo);

	int changeWindowStatus(char *windowName,char windowStatus);

	int findTopWindow(WindowElementApp &winInfo);
	int deleteTopWindow(WindowElementApp &winInfo);
	int deleteTopWindow();

	void showWindowStack();

private:
	WindowElementApp    bottomWindow;
	WindowElementApp    *topWindow;
	
	DBusClientProxyC 	    *guiClient;
	char		    processName[30];

        pthread_mutex_t         m_lock;

        class   InternalLock
        {
        public:
                InternalLock(pthread_mutex_t* lock);
                ~InternalLock();
        private:
                pthread_mutex_t* m_lock;
        };
};



#endif

