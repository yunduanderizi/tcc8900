/**
 * @file   musicDownInterface.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110124
 * 
 * @brief  
 *      Interface define for traffic notices download 
 * 
 */
#ifndef BSTRAFFICNOTICE_H_20110124
#define BSTRAFFICNOTICE_H_20110124


#include <QString>
#include <QList>
#include <QThread>
#include <QMetaType>  
#include <QFile>
#include <QDir>
#include <QMutex>
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"
#include "NaviDbusMessage.h"

#define TRAFFICNOTICE_LIST_XMLFEIL  "trafficNoticeDownload/trafficNoticeList.xml"
#define TRAFFICNOTICE_NODE_XMLFEIL  "trafficNoticeDownload/trafficNoticeNode.xml"

#define TRAFFICNOTICE_INFO_XMLFEIL "trafficNoticeDownload/trafficNoticeInfo.xml"
#define TRAFFICNOTICE_CONTENT_XMLFEIL "trafficNoticeDownload/trafficNoticeContent.xml"

//data struct used by BsTrafficNoticeC
class TrafficNoticeListNode
{
public:
	void clear(){from="";title="";time="";url="";content="";}
	QString from;//source of notice
	QString title;//title of notice
	QString time;//publish_time of notice
	QString url;//content's url
	QString content;//content of notice 
};

Q_DECLARE_METATYPE(TrafficNoticeListNode);
typedef QList<TrafficNoticeListNode> TrafficNoticeList;

class BsTrafficNoticeC :public QThread,  public BsBaseC
{
	Q_OBJECT
		
	public:
		//get the object of "BsTrafficNoticeC", notice: you can not get the object from "new"
		static BsTrafficNoticeC *object();
		//release the object of "BsTrafficNoticeC"
		static void releaseObject();
		static BsTrafficNoticeC *getObject();

		//Start to download traffic notice list, this is an asynchronous operation, result will be returned through signal "getTrafficNoticeListResult"
		int startGetTrafficNoticeList();

		//Start to download traffic notice content of the certain node, this is an asynchronous operation, 
		//result will be returned through signal "getTrafficNoticeNodeResult".
		//The user should invoke "startGetTrafficNoticeList" at first, after getting the result from "getTrafficNoticeListResult",
		//then invoke this function to download the content.
		//Notice: pTrafficNode will be modified, so before getTrafficNoticeNodeResult, pls. don't modify it.
		
		int startGetTrafficNoticeNode(TrafficNoticeListNode *pTrafficNode);

		//Start to download traffic notice all information, this is an asynchronous operation, result will be returned through signal "getTrafficNoticeInfoResult"
		int startGetTrafficNoticeInfo();
		
		//Cancel previous operation.
		void	cancelOperation();
		
	public:
		int setCityPinyin(const char *pcity);
		int setCityUtf8(char *pcity);
		
	signals:
		void getTrafficNoticeListResult(int result, TrafficNoticeList trafficNoticeList);
		void getTrafficNoticeNodeResult(int result);
		void getTrafficNoticeInfoResult(int resultList, int resultNode,TrafficNoticeList TrafficNoticeInfo);

	private:
		static BsTrafficNoticeC *thisObject;
		BsTrafficNoticeC();
		~BsTrafficNoticeC();

		//thread work mode: 0--get traffic notice list       1--get traffic notice list node   2--get all traffic info
		//int m_iThreadWorkMode;
		
		volatile bool m_bExit;
		QString m_city;
		
		void run();
private slots:
		void slotFinished();	 
private:
		class DownloadTask
		{
		public:
			QString city;
			QString TrafficNodeUrl;
			TrafficNoticeListNode* p_TrafficNode;
			int ThreadWorkMode;
		};
		typedef QList<DownloadTask> DownloadTaskList;
		DownloadTaskList m_list;
		//TrafficNoticeListNode* p_TrafficNode;
		int _parseXmlList(QFile *file, TrafficNoticeList *ptrafficNoticeList);
	
		int _downloadXml(QFile *file, char * url);

		int _parseXmlNode(QFile *file, TrafficNoticeListNode  *ptrafficNoticeNode);
		QMutex m_mutexForThreadSafe;
};


#endif


