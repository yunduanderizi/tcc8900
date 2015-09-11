#include<QMessageBox>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<QDebug>
#include "FileListUtil.h"
#include <qfile.h>

FileListUtil::FileListUtil(QString fileName, QObject*parent, QString docType)
    :QObject(parent),fileListName(fileName), m_sDocType(docType)
{
    init();
}


FileListUtil::~FileListUtil()
{
    uninit();
}

void FileListUtil::init()
{
    reader=new QXmlStreamReader;
    writer=new QXmlStreamWriter;
    writer->setAutoFormatting(true);
}

void FileListUtil::uninit()
{
    if(reader){delete reader; reader=0;}
    if(writer){delete writer; writer=0;}
}

//Moreover,if file IO error occurs, FileIOError(QString errorPrompt) signal emits
//The call will remove the old items, doesn't support add items incrementally
//So you should maintain a complete list and save it at one time
void FileListUtil::saveFileList(const QStringList &list)
{
    QFile file(fileListName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug()<<"Save list failed!";
        return;
    }
    writer->setDevice(&file);

    writer->writeStartDocument();
    writer->writeDTD(m_sDocType);
    writer->writeStartElement("Playlist");
    writer->writeAttribute("version", "1.0");

    writer->writeStartElement("Infoslist");
    for (int i = 0; i < list.size(); ++i){
        writer->writeTextElement("Info",list.at(i));
    }
    writer->writeEndElement();

    writer->writeEndElement();
    writer->writeEndDocument();
}

//if file doesn't exist or empty, return a empty list
//Moreover,if file IO error occurs, the signal FileIOError(QString errorPrompt)  emits
QStringList FileListUtil::openFileList()
{
    QFile file(fileListName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<"Save list failed!";
        return QStringList();
    }

    reader->setDevice(&file);

    QStringList list,listCur;
    while (!reader->atEnd()) {
        reader->readNext();
        if (reader->isStartElement()) {
            if (reader->name() == "IC_Setting")
            {
                readFile3th( list,listCur );
            }
            else
                reader->raiseError(QObject::tr("The file is not a mediaplayer file list version 1.0 file."));
        }
    }

    return list;
}

void FileListUtil::readFiles(QStringList& list)
{
    QString file;
    while (!reader->atEnd()) {
        reader->readNext();
        if (reader->isStartElement()) {
            if (reader->name() == "Info") {
                file = reader->readElementText();
                list.append(file);
            }else //not a file element
            {
                readUnkownElement();
            }
        }//if is a start element

    }//while: not end
}

//if file doesn't exist or empty, return a empty list
//Moreover,if file IO error occurs, the signal FileIOError(QString errorPrompt)  emits
bool FileListUtil::openFileListOther(QStringList& list,QStringList& listCur)
{
    //TODO
    QFile file(fileListName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        //		QMessageBox::warning(0, tr("Open Meidia Player list failed!"),
        //			tr("Cannot read file %1:\n%2.")
        //			.arg(fileListName)
        //			.arg(file.errorString()));
        //		emit FileIOError(file.errorString());
        return false;
    }

    reader->setDevice(&file);

    while (!reader->atEnd())
    {
        reader->readNext();
        if (reader->isStartElement())
        {
            if (reader->name() == "Playlist" && reader->attributes().value("version") == "1.0")
            {
                readFile2th( list,listCur );
            }
            else
                reader->raiseError(QObject::tr("The file is not a mediaplayer file list version 1.0 file."));
        }
    }

    return true;
}

void FileListUtil::readFile2th(QStringList& list,QStringList& listCur)
{
    QString file;
    while (!reader->atEnd())
    {
        reader->readNext();
        if (reader->isStartElement())
        {
            if ((reader->name() == "Infoslist") || (reader->name() == "CurrentInfo"))
            {
                //read all items
                readFile3th(list,listCur);
            }
            else //not a file element
            {
                readUnkownElement();
            }
        }//if is a start element

    }//while: not end
}


void FileListUtil::readFile3th(QStringList& list,QStringList& listCur)
{
    QString file;
    while (!reader->atEnd()) {
        reader->readNext();
        if (reader->isStartElement()) {
            if (reader->name() == "driving_num")
            {
                file = reader->readElementText();
                list.append(file);
            }
            else //not a file element
            {
                readUnkownElement();
            }
        }//if is a start element

    }//while: not end
}


//Moreover,if file IO error occurs, FileIOError(QString errorPrompt) signal emits
//The call will remove the old items, doesn't support add items incrementally
//So you should maintain a complete list and save it at one time
void FileListUtil::saveFileListOther(const QStringList &list, const QStringList &listCur, int type)
{
    QFile file(fileListName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        //		QMessageBox::warning(0, tr("Save Meidia Player list failed!"),
        //			tr("Cannot write file %1:\n%2.")
        //			.arg(fileListName)
        //			.arg(file.errorString()));
        //		emit FileIOError(file.errorString());
        return;
    }
    writer->setDevice(&file);

    writer->writeStartDocument();
    writer->writeDTD(m_sDocType);
    if(1 == type)
    {
        writer->writeStartElement("setting");
        // writer->writeAttribute("version", "1.0");
        for (int i = 0; i < list.size(); ++i)
        {
            if(0 ==i)
            {
                writer->writeTextElement("province",list.at(i));
            }
            if(1 ==i)
            {
                writer->writeTextElement("city",list.at(i));
            }
            if(2 == i&& NULL!= list.at(i))
            {
                writer->writeTextElement("maker",list.at(i));
            }
            if(3 == i &&NULL!=list.at(i))
            {
                writer->writeTextElement("model",list.at(i));
            }
            if(4 == i&& NULL!= list.at(i))
            {
                writer->writeTextElement("termid",list.at(i));
            }
            if(5 == i &&NULL!=list.at(i))
            {
                writer->writeTextElement("vin",list.at(i));
            }
            if(6 ==i)
            {
                writer->writeTextElement("platecolor",list.at(i));
            }
            if(7 == i&& NULL!= list.at(i))
            {
                writer->writeTextElement("plate",list.at(i));
            }
            if(8 == i &&NULL!=list.at(i))
            {
                writer->writeTextElement("platetype",list.at(i));
            }
            if(9 == i&& NULL!= list.at(i))
            {
                writer->writeTextElement("time",list.at(i));
            }
            if(10 == i)
            {

                writer->writeTextElement("mileage",list.at(i));
            }
            if(11 == i && 0!=list.at(i).toInt())
            {
                writer->writeTextElement("rate",list.at(i));
            }
            if(12 == i && 0!=list.at(i).toInt())
            {
                writer->writeTextElement("sensingelement",list.at(i));
            }
            if(13 ==i && NULL!=list.at(i))
            {
                writer->writeTextElement("phone",list.at(i));
            }
            if(14 == i&& NULL!= list.at(i))
            {
                writer->writeTextElement("auth",list.at(i));
            }
            if(15 == i &&NULL!=list.at(i))
            {
                writer->writeTextElement("main",list.at(i));
            }
            if(16 == i&& NULL!= list.at(i))
            {
                writer->writeTextElement("second",list.at(i));
            }
            if(17 == i&& 0!=list.at(i).toInt() )
            {
                writer->writeTextElement("port",list.at(i));
            }

            if(18 == i&& 0!=list.at(i).toInt() )
            {
                writer->writeTextElement("speed",list.at(i));
            }
            if(19 == i&& 0!=list.at(i).toInt() )
            {
                writer->writeTextElement("drivedura",list.at(i));
            }
            if(20 == i&& 0!=list.at(i) )
            {
                writer->writeTextElement("termtype",list.at(i));
            }
            if(21 == i&& NULL!=list.at(i) )
            {
                writer->writeTextElement("hadversion",list.at(i));
            }
        }
        writer->writeEndElement();
    }
    else
        if(2 == type)
        {
            writer->writeStartElement("IC_Setting");
            // writer->writeAttribute("version", "1.0");
            for (int i = 0; i < list.size(); ++i)
            {
                /*if(0 ==i )
                {
                    writer->writeTextElement("TCB",list.at(i));
                }*/
                if(0 ==i&& NULL!= list.at(i))
                {
                    writer->writeTextElement("driving_num",list.at(i));
                }
                if(1 ==i)
                {
                    writer->writeTextElement("driving_name",list.at(i));
                }
                if(2 == i&& NULL!= list.at(i))
                {
                    writer->writeTextElement("Practitioner_card",list.at(i));
                }
                if(3 == i &&NULL!=list.at(i))
                {
                    writer->writeTextElement("valid_date",list.at(i));
                }
                if(4 == i && NULL!= list.at(i))
                {
                     writer->writeTextElement("driving_phone",list.at(i));
                }
                if(5 == i && NULL!= list.at(i))
                {
                     writer->writeTextElement("driving_policynum",list.at(i));
                }
                if(6 == i && NULL!= list.at(i))
                {
                     writer->writeTextElement("car_policynum",list.at(i));
                }
                if(7 == i && NULL!= list.at(i))
                {
                     writer->writeTextElement("emergencycontact_person",list.at(i));
                }
                if(8 == i && NULL!= list.at(i))
                {
                     writer->writeTextElement("contact_phonenum",list.at(i));
                }
            }
            writer->writeEndElement();

        }

    //writer->writeStartElement("CurrentInfo");
    //writer->writeTextElement("Current",listCur.at(0));
    //writer->writeEndElement();

    //writer->writeEndElement();
    //writer->writeEndDocument();


}
//**************************************************************************************************************
void FileListUtil::readUnkownElement()
{
    while (!reader->atEnd()) {
        reader->readNext();

        if (reader->isEndElement())
            break;

        if (reader->isStartElement())
            readUnkownElement();
    }
}

//if the file doesn't exist, create it.
//If create fail,FileIOError() signal emits
void FileListUtil::setFileListName(const QString& fileName)
{
    if(fileListName==fileName)
        return;
    fileListName=fileName;
}

QString FileListUtil::getFileListName() const
{
    return fileListName;
}

//only for convenience, once changed, save the file list automatically if you want.
//But you have to emit the "changed" signal and connect it to the slot
void  FileListUtil::do_fileListChanged(const QStringList &list)
{
    saveFileList(list);
}

bool FileListUtil::openFileListNoListCur(QStringList& list)
{
    QFile file(fileListName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        //FILE *fpOut=fopen(fileListName.toAscii().data(),"a+");
        return false;
    }

    reader->setDevice(&file);

    while (!reader->atEnd())
    {
        reader->readNext();
        if (reader->isStartElement())
        {
            if (reader->name() == "Playlist" && reader->attributes().value("version") == "1.0")
            {
                readFile2thNoListCur( list);
            }
            else
                reader->raiseError(QObject::tr("The file is not a mediaplayer file list version 1.0 file."));
        }
    }

    return true;
}

void FileListUtil::readFile2thNoListCur(QStringList& list)
{
    QString file;
    while (!reader->atEnd())
    {
        reader->readNext();
        if (reader->isStartElement())
        {
            if ((reader->name() == "Infoslist") )
            {
                //read all items
                readFile3thNoListCur(list );
            }
            else //not a file element
            {
                readUnkownElement();
            }
        }//if is a start element

    }//while: not end
}
void FileListUtil::readFile3thNoListCur(QStringList& list)
{
    QString file;
    while (!reader->atEnd())
    {
        reader->readNext();
        if (reader->isStartElement())
        {
            if (reader->name() == "Info")
            {
                file = reader->readElementText();
                list.append(file);
            }
            else //not a file element
            {
                readUnkownElement();
            }
        }//if is a start element

    }//while: not end
}
