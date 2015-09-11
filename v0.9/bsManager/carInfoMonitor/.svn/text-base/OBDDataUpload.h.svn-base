#ifndef  OBD_DATA_UPLOAD_H_20110527
#define  OBD_DATA_UPLOAD_H_20110527

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"
#include <QMutex>
#include <QWaitCondition>

typedef enum _OBD_DATA_UPLOAD_RESULT
{
	OBD_DATA_UPLOADING = 0,
	OBD_DATA_UPLOAD_SUCCESS,
	OBD_DATA_UPLOAD_FAIL,
}OBD_DATA_UPLOAD_RESULT;

class OBDDataUploadTask
{
public:
	OBDDataUploadTask(){ iCount = 0; result = OBD_DATA_UPLOADING; }
	QString url;
	QString postfield;
	QString cookie;
	volatile unsigned int iCount;	//the count of uploading when upload fail
	volatile OBD_DATA_UPLOAD_RESULT result;
};

typedef QList<OBDDataUploadTask*> OBDDataUploadTaskList;

class OBDDataUploadC : public QThread, public BsBaseC
{
	Q_OBJECT
	public:
		OBDDataUploadC();
		~OBDDataUploadC();

		int uploadOBDData(OBDDataUploadTask* pTask);
		void cancelOperation();
		void run();
		volatile bool m_bExit;

	private slots:
		void slotFinished();	 

	private:
		OBDDataUploadTaskList m_list; 
		QMutex m_mutexForThreadSafe;
		QMutex m_mutexWaitCondition;
		QWaitCondition m_condExit;
};

#endif
