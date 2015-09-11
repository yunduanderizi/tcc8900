#ifndef FILELISTUTIL_H
#define FILELISTUTIL_H


#include<QObject>
#include <QXmlStreamReader>
#include<QString>
#include<QStringList>
#include<QFile>

class FileListUtil:public QObject
{
	Q_OBJECT
public:
	FileListUtil(QString fileName, QObject*parent=0, QString docType = "<!DOCTYPE Info>");
	~FileListUtil();

	virtual void  saveFileList(const QStringList &list);
	virtual QStringList openFileList();
	
	virtual bool openFileListNoListCur(QStringList& list);
	void readFile2thNoListCur(QStringList& list);
	void readFile3thNoListCur(QStringList& list);

	virtual bool openFileListOther(QStringList& list,QStringList& listCur);
        virtual void saveFileListOther(const QStringList &list,const QStringList &listCur,int type);
	void readFile2th(QStringList& list,QStringList& listCur);
	void readFile3th(QStringList& list,QStringList& listCur);

	void setFileListName(const QString& fileName);
	QString getFileListName() const;

private:
	void init();
	void uninit();
	void readFiles(QStringList& list);
	void readUnkownElement();

	public slots:
		//only for convenience, once changed, save the file list automatically if you want.
		//But you have to emit the "changed" signal and connect it to the slot
		void do_fileListChanged(const QStringList &list);

signals:
		void FileIOError(QString errorPrompt);

private:
	QXmlStreamReader *reader;
	QXmlStreamWriter *writer;
	//QFile *file;
	QString		fileListName;
	QString 	m_sDocType;
};

#endif // FILELISTUTIL_H

