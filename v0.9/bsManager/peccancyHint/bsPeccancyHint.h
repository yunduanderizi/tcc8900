#ifndef  PECCANCYHINT_H_20110307
#define  PECCANCYHINT_H_20110307

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include <QMutex>
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"


//data struct used by BsPeccancyHintC
class BsPeccancyHintInfo
{
public:
	QString  error;                                   //error of  PeccancyHint
	QString  tu_id;                                //tu_id of  PeccancyHint
	QString  pec_date;                             //pec_date of  PeccancyHint
	QString  pec_address;                          //pec_address of  PeccancyHint
	QString  pec_action;                           //pec_action of  PeccancyHint
	QString  pec_dispose;                          //pec_disposeof  PeccancyHint
	QString  create_date;                          //create_date of  PeccancyHint
	QString  is_return;                            //is_return of  PeccancyHint

	void clear()
		{
			tu_id = "";
			pec_date = "";
			pec_address = "";
			pec_action = "";
			pec_dispose = "";
			create_date = "";
			is_return = "";
		 }
};

Q_DECLARE_METATYPE(BsPeccancyHintInfo);
typedef QList<BsPeccancyHintInfo>  BsPeccancyHintInfoList;

class BsPeccancyHintC : public QThread,  public BsBaseC
{
	Q_OBJECT
	public:
	       //get the object of "BsPeccancyHintC", notice: you can not get the object from "new"
	       static BsPeccancyHintC *object();
	        //release the object of "BsPeccancyHintC"
	         static void releaseObject();
		static BsPeccancyHintC *getObject();
		  int startGetPeccancyHint(bool bAllHint=false);
		  void  cancelOperation();
	private:
		static BsPeccancyHintC *thisObject;
		BsPeccancyHintC();
		~BsPeccancyHintC();
		void run();
		volatile bool m_bExit;
		bool m_bAllHint;
		BsPeccancyHintInfo  m_PeccancyHint;
	signals:
		 void sigetPeccancyHintResult(int result,BsPeccancyHintInfoList peccancyHintinfo);
	private:
		   int _parseXmlPeccancyHint(QFile *file, BsPeccancyHintInfoList *p_PeccancyHint);
		   int _downloadPeccancyHintXml(QFile *file, char* url);
		   QMutex m_mutexForThreadSafe;
};

#endif
