#ifndef  CAR_DATA_UPLOAD_H_20110516
#define  CAR_DATA_UPLOAD_H_20110516

#include <QString>
#include <QThread>
#include <qfile.h>
#include <QMetaType>  
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"
#include <QMutex>
#include <QWaitCondition>


class CarDataDownloadTask
{
public:
	CarDataDownloadTask(){bUploading = true;}
	QString url;
	QString postfield;
	QString cookie;
	bool bUploading;
};

class CarDataUploadC : public QThread, public BsBaseC
{
	Q_OBJECT
	public:
		CarDataUploadC();
		~CarDataUploadC();

		int uploadCarData(CarDataDownloadTask task);
		void run();
		volatile bool m_bExit;

		void cancelOperation();
	
	private slots:
		void slotFinished();	 

	private:
		int _postCarInfoData(CarDataDownloadTask myTask);
		CarDataDownloadTask m_task;
		QMutex m_MutexForThreadSafe;
		QMutex m_MutexWaitCondition;
		QWaitCondition m_CondExit;
};

#endif
