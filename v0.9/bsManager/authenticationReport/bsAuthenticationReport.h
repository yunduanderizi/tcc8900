/**
 * @file   bsAuthenticationReport.h
 * @author zhanghongbo <zhanghongbo@chinatsp.com>
 * @date  20110227
 * 
 * @brief  
 *      define for authentication information download 
 * 
 */
#ifndef BSAUTHENTICATIONREPORT_H_20110314
#define BSAUTHENTICATIONREPORT_H_20110314


#include <QString>
#include <QList>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"

class BsAuthenticationReportC : public QThread, public BsBaseC
{
	Q_OBJECT
		
public:
	//get the object of "BsAuthenticationReporttC", notice: you can not get the object from "new"
	static BsAuthenticationReportC *object();
	//release the object of "BsAuthenticationReportC"
	static void releaseObject();

	//get the object of "BsAuthenticationReportC", if it does not exist, not new it
	static BsAuthenticationReportC *getObject();
	//authentication the SMS  
	int startAuthenticate(char *pAuthentication);
	//get the new authentication information,if time has more than 12 hours will get new authentication.
	//or if flag == 1 it will to get the new authentication infomation;
	
	int refreshAuthenticationInfo(bool bForce=false);

	bool isFinish();

	void cancelOperation();

private slots:
	void slotFinished();	
	
private:
	static BsAuthenticationReportC *thisObject;
	BsAuthenticationReportC();
	~BsAuthenticationReportC();

	int _downloadAuthentication(const char* pTuid, char* pAuthenticationData);
	
	volatile bool m_bExit;
	bool m_bStartAgain;
	
	void run();
	QMutex m_mutexForThreadSafe;
	QMutex m_mutexWaitCondition;
	QWaitCondition m_condExit;
};

#endif


