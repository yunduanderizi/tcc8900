#ifndef SAVESETTING_H
#define SAVESETTING_H

#include <QWidget>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QtGui>
#include <qfile.h>
#include <FileListUtil.h>


#define base_setting 1

struct Listinfo{
//    int province;
//    unsigned int city;
    QString province;
    QString city;
    QString maker;
    QString model;
    QString termed;
    QString VIN;
    int platecolor;
    QString plate;
    QString platetype;
    QString installtime;
    int mileage;
    unsigned int rate;
    int sensingelement;
    QString phone;
    QString auth;
    QString main;
    QString second;
    QString hadversion;
    int port;
    int speed;
    int drivedura;
    QString terminaltype;

};

namespace Ui {
class SaveSetting;
}

class SaveSetting : public QWidget
{
    Q_OBJECT

public:
    SaveSetting(QWidget *parent = 0);
    ~SaveSetting();
    void setupUI();
    int getText();
    QList <struct Listinfo>m_listinfo;
    int save_setting(QString fileName,QString fileTitle, int m_postion, int playState );
    void find_file();
    void findDiskPathName(QString findDiskFilePathName, int type);
    void Judge();
    bool read(QIODevice *device);
    void read_data();
    QString get_type();

private slots:
    void on_SaveButton_clicked();
    void on_pathButton_clicked();
    void changeDay(int count);
    void changeCity();
private:
    Ui::SaveSetting *ui;
    Listinfo s_listinfo;
    QString filename;
    int flag;
    char terminal_type;
    QStringList filename_list;
    QStringList findDiskFilePathNameList;
    QString findFilePathName;
    bool isNOExit;
    QXmlStreamReader *reader;
    QMap<QString,QString> proviceMap;
    QMap<QString,QString> cityMap;
public:
    QStringList cityList;
    QStringList proviceList;
    QStringList provicejList;
    QStringList proviceidList;
    QVector<QStringList> cityVector;

    QStringList beijingList;
    QStringList tianjingList;
    QStringList hebeiList;
    QStringList shanxiList;
    QStringList neimengguList;
    QStringList liaoningList;
    QStringList jilinList;
    QStringList heilongjiangList;
    QStringList shanghaiList;
    QStringList jiangsuList;

    QStringList anhuiList;
    QStringList fujianList;
    QStringList jiangxiList;
    QStringList shandongList;
    QStringList henanList;
    QStringList hubeiList;
    QStringList hunanList;
    QStringList guangdongList;
    QStringList hainanList;
    QStringList chongqingList;

    QStringList sichuanList;
    QStringList guizhouList;
    QStringList yunnanList;
    QStringList sanxiList;
    QStringList gansuList;
    QStringList qinghaiList;
    QStringList xizangList;
    QStringList zhejiangList;
    QStringList guangxiList;
    QStringList ningxiaList;

    QStringList xinjiangList;
    QStringList xianggangList;
    QStringList aomenList;
    QStringList taiwanList;
};

#endif // SAVESETTING_H
