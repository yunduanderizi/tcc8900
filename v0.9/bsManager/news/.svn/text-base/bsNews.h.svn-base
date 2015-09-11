/**
 * @file   bsNews.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110124
 * 
 * @brief  
 *      Interface define for news download 
 * 
 */
#ifndef BSNEWS_H_20110224
#define BSNEWS_H_20110224

#include <QString>
#include <QList>
#include <QMap>
#include <QThread>
#include <QMutex>
#include <QMetaType>  
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QIODevice>
#include "bsManagerCommon.h"
#include "bsManagerErrCode.h"



class NewsNoteicNode
{
	public:
		void clear(){id = ""; time = ""; newsItem = "";url = ""; title = ""; link = ""; description = "";};
		QString id;
		QString time;
		QString newsItem;//title of rss
		QString url;//news's url
		QString title;//title of news
		QString link;//url of content
		QString description;// content of news

};

typedef QList<NewsNoteicNode> NewsTypeList;

typedef QList<NewsNoteicNode> NewsContentNodeList;


class BsNewsC : public QThread, public BsBaseC
{
	Q_OBJECT
public:
	//get the object of "BsNewsC", notice: you can not get the object from "new"
	static BsNewsC *object();
	//release the object of "BsNewsC"
	static void releaseObject();

	static BsNewsC *getObject();
	
	int startGetNewsDefaultTypeList();

	//Start to download news type list, this is an asynchronous operation, result will be returned through signal "getNewsTypeListResult"
	int startGetNewsCustomTypeList(QString newsId);

	//Start to download news type content of the certain node, this is an asynchronous operation, 
	//result will be returned through signal "getNewsContentNodeResult".
	//The user should invoke "startGetNewsTypeList" at first, after getting the result from "getNewsTypeListResult",
	//then invoke this function to download the content.
	//Notice: pNewsNode will be modified, so before getNewsContentNodeResult, pls. don't modify it.
	int startGetNewsContentNode(NewsNoteicNode *pNewsNoteNode);

	//Cancel previous operation.
	void	cancelOperation();
	NewsNoteicNode *m_pNewsNoteNode;
signals:
	void	sigGetNewsDefaultTypeListResult(int result, NewsTypeList newsDefaultType);
	void	sigGetNewsCustomTypeListResult(int result, NewsTypeList newsCustomType);
	void	sigGetNewsContentNodeResult(int result,NewsContentNodeList newsContent);


private:
	static BsNewsC *thisObject;
	BsNewsC();
	~BsNewsC();
	class DownloadNewsTask
	{
	public:
		QString url;
		QString newsId;	
	};

	typedef QList<DownloadNewsTask> DownloadNewsTaskList;

	DownloadNewsTaskList m_list_type;
	DownloadNewsTaskList m_list_content;
	
	int m_iThreadWorkMode;
//	BSLoginStatusClientC *m_pLogin;

	volatile bool m_bExit;
	void run();

private slots:
		void slotFinished();

private:
	int _parseDefaultList(QFile *file, NewsTypeList *pNewsDefaultType);
	
	int _parseCustomList(QFile * file, NewsTypeList * pNewsCustomType);
	
	int _parseXmlContent(QFile *ContentFile, NewsContentNodeList *pNewsContent);
	QMutex m_mutexForThreadSafe;
};



#endif


