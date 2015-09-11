/**
 * @file   musicDownInterface.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110124
 * 
 * @brief  
 *      Interface define for stock information download 
 * 
 */
#ifndef BSSTOCK_H_20110308
#define BSSTOCK_H_20110308


#include <QString>
#include <QList>
#include <QThread>
#include <QMetaType>  
#include <QFile>
#include <QDir>
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"
#include "NaviDbusMessage.h"
#include <QMutex>


//data struct used by BsStockInfoC
class StockInfoNodeC
{
public:
	
	QString id;
	QString name;
	QString from;
	QString yesterdayClose;
	QString todayOpen;
	QString todayNow;
	QString todayMin;
	QString todayMax;
	
	QString todayUpdown;
	QString todayTurnover;
	QString todayVolume;
	
	QString sellerOnePrice;
	QString sellerOneVolume;
	QString sellerTwoPrice;
	QString sellerTwoVolume;
	QString sellerThreePrice;
	QString sellerThreeVolume;
	QString sellerFourthPrice;
	QString sellerFourthVolume;
	QString sellerFifthPrice;
	QString sellerFifthVolume;
		
	QString buyerOnePrice;
	QString buyerOneVolume;
	QString buyerTwoPrice;
	QString buyerTwoVolume;
	QString buyerThreePrice;
	QString buyerThreeVolume;
	QString buyerFourthPrice;
	QString buyerFourthVolume;
	QString buyerFifthPrice;
	QString buyerFifthVolume;
	
	
	QString commissionRatio;
	QString commissionDifference;
	QString turnover;
	QString externalDisk;
	QString plate;
	
	QString todayChartUri;
	QString dayKUri;
	QString weekKUri;
	QString monthKUri;
	QString updateTime; 

	QString current_price;
	QString limit_price;
	QString limit_down_price;
	QString inflation_rate_down;

	QString todayChartPath;
	QString dayKPath;
	QString weekKPath;
	QString monthKPath;

};

typedef enum _Bs_Stock_Download_Mode
{
	Bs_Stock_No_Chat = 0,
	Bs_Stock_Today_Chat,
	Bs_Stock_Day_K_Chat,
	Bs_Stock_Week_K_Chat,
	Bs_Stock_Month_K_Chat,
	Bs_Stock_All_Chat
}Bs_Stock_Download_Mode;

class BsStockInfoC :public QThread,  public BsBaseC
{
	Q_OBJECT
		
	public:
		//get the object of "BsStockInfoC", notice: you can not get the object from "new"
		static BsStockInfoC *object();
		//release the object of "BsTrafficNoticeC"
		static void releaseObject();

		static BsStockInfoC *getObject();
		//Start to download Stock information , this is an asynchronous operation, result will be returned through signal "getStockInfoListResult"
		//flag is 0 --  no need down load chart, flag is 1 need down load chart;
		int startGetStockInfo(const QString stockId, Bs_Stock_Download_Mode flag);

		//Cancel previous operation.
		void	cancelOperation();
		
	signals:
		void sigGetStockInfoResult(int result, StockInfoNodeC stockInfo);

		

	private:
		class DownloadTask
		{
		public:
			QString stockId;
			Bs_Stock_Download_Mode iflag;
		};
		//Q_DECLARE_METATYPE(TrafficNoticeListNode);
		typedef QList<DownloadTask> DownloadTaskList;
		DownloadTaskList m_list;

	private slots:
		void slotFinished();
				
	private:
		static BsStockInfoC *thisObject;
		BsStockInfoC();
		~BsStockInfoC();

		volatile bool m_bExit;

		void run();
	private:
		int _parseStockInfoXml(QFile *file,StockInfoNodeC *pstockInfo);
		int _download_Xml(QFile *file,QString url);
		int _download_chart(QFile *file,QString url);
		
		QMutex m_mutexForThreadSafe;
};


#endif


