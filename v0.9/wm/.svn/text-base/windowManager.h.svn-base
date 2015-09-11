#ifndef WINDOW_MANGER_H
#define WINDOW_MANGER_H

#include "dbusMessage.h"
#include "dbusClientProxy.h"
#include "dbusServer.h"

#define WMS_WINDOW_MODE_MIDDLE  0
#define WMS_WINDOW_MODE_HALF    1
#define WMS_WINDOW_MODE_FULL    2

//#define WM_DDT_GPS	1

class	ManagedWindow
{
public:
	virtual int managerShow(int /*flag*/){return 0;}
	virtual int managerHide(int /*flag*/){return 0;}
	virtual int managerBack(int /*flag*/){return 0;}

	virtual int managerKey(int /*keyCode*/,int /*modifier*/){return 0;}

	virtual void topWindowChanged(const char /*processName*/[30],const char /*windowName*/[30]){return;}
};
	

typedef struct _WindowElement
{
	char processName[30];
	char windowName[30];

	DBusClientProxyC	*client;

	ManagedWindow	*window;	
	char             windowMode;

	struct _WindowElement *next;
}WindowElement;


typedef struct _AppDBusClient
{
	char 		processName[30];
	DBusClientProxyC	*client;
	struct _AppDBusClient	*next;
}AppDBusClient;


class WindowManager
{
public:
	WindowManager(ManagedWindow *mainWindow,DBusServerC *guiServer);

public:
	void managerGoMainMenu();
	void managerGoBack();
	void managerGoBack(int notifyTopWindow);
	void managerGoApplication(char *processName,DBusClientProxyC *client);
	void managerShow(char *windowName,ManagedWindow *window,char windowMode=WMS_WINDOW_MODE_MIDDLE);
	void managerHide(char *windowName);
	void managerHide(char *processName,char *windowName);
	void managerGetTopWindow(char processName[30],char windowName[30]);

	int managerSendKeyToTopWindow(int keyCode,int modifier);

	void 	      addDBusClient(char *processName,DBusClientProxyC *client);

private:
	static WindowManager *thisWindowManager;
	static void windowManagerHandler(DBusRequestC *pRequest, DBusReplyC *&pReply);

	void applicationGoMainMenu(char *processName,char *windowName,DBusReplyC *&reply);
	void applicationGoBack(char *processName,char *windowName,char op,DBusReplyC *&reply);
	void applicationShow(char *processName,char *windowName,DBusReplyC *&reply,char windowMode);
	void applicationHide(char *processName,char *windowName,DBusReplyC *&reply);
	void applicationGetTopWindow(DBusReplyC *&reply);

public:
	void showTopWindow();

private:
	int addToTop(char *processName,char *windowName,DBusClientProxyC  *client,char windowMode);
	int addToTop(char *processName,char *windowName,ManagedWindow *window,char windowMode);

	int changeWindowMode(char *processName,char *windowName,char windowMode);

	int deleteWindow(char *processName,char *windowName);

	int findWindow(char *processName,char *windowName,WindowElement &winInfo);
	int findWindowBefore(char *processName,char *windowName,WindowElement &winInfo);

	int findTopWindow(WindowElement &winInfo);
	int deleteTopWindow(WindowElement &winInfo);
	int deleteTopWindow();

	void showWindowStack();

	DBusClientProxyC  *findDBusClient(char *processName);

private:
	WindowElement    bottomWindow;
	WindowElement    *topWindow;

	AppDBusClient	 appClientHead;

	pthread_mutex_t         m_lock;

	class	InternalLock
	{
	public:
		InternalLock(pthread_mutex_t* lock);
		~InternalLock();
	private:
		pthread_mutex_t* m_lock;
	};
};



#endif

