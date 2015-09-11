/**
 * @file   bsLogin.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110411
 * 
 * @brief  
 *      Interface define for login 
 * 
 */
#ifndef BSLOGIN_H_20110411
#define  BSLOGIN_H_20110411

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include <QMutex>
#include <QWaitCondition>
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"




class BsLoginC : public QThread,  public BsBaseC
{
	Q_OBJECT
	public:
		//get the object of "BsLoginC, notice: you can not get the object from "new"
		static BsLoginC *object();
		//release the object of "BsLoginC"
		static void releaseObject();

		static BsLoginC *getObject();
		
		int getTuid(char *tuid);
		bool isLogin();
		int getLoginResult(char *tuid, char *uuid, char *password, char *accessToken, bool &bLogin, bool &bSavePassword);
		int login(const char *uuid, const char *password, bool bSavePassword);
		int logout();
		void cancelOperation();
			
	signals:
		 void sigLoginResult(int result);
		 void sigLogoutResult(int result);

	private:
		static BsLoginC *thisObject;
		BsLoginC();
		~BsLoginC();
		void run();
		int _login(const char *uuid, const char *password, bool bSavePassword);
		int _logout();
		void _refreshRelatedInfo(bool bForLogin);

		volatile bool m_bExit;

	private slots:
		void slotFinished();	 
		
	private:
		class DownloadTask
		{
		public:
			char uuid[BK_UUID_LENGTH_MAX+1];
			char password[BK_PASSWORD_LENGTH_MAX+1];
			bool bSavePassword;
			int mode;//0--login   1--logout
		};
		typedef QList<DownloadTask> DownloadTaskList;
		DownloadTaskList m_list;
		QMutex m_mutexForThreadSafe;
		QMutex m_mutexWaitCondition;
		QWaitCondition m_condExit;
};

#endif
