#ifndef ICSETTING_H
#define ICSETTING_H

#include <QWidget>
#include <QXmlStreamWriter>
#include <QtGui>
#define IC_Setting 2

struct ICList{
    //QString TCB;
    QString driving_num;
    int driving_name;
    QString seniority_card;//从业资格证
    QString valid_date;
    QString driving_phone;
    QString driving_policynum;//驾驶员保单号
    QString car_policynum;
    QString emergencycontact_person;//紧急联系人
    QString contact_phonenum;
};

namespace Ui {
class ICsetting;
}

class ICsetting : public QWidget
{
    Q_OBJECT

public:
    ICsetting(QWidget *parent = 0);
    ~ICsetting();
    void setupUI();
    void getText();
    QList <struct ICList>m_listinfo;
    int save_setting(QString fileName,QString fileTitle, int m_postion, int playState );
    void find_file();
    void findDiskPathName(QString findDiskFilePathName, int type);
private slots:
    void on_SaveButton_clicked();
    void on_pathButton_clicked();
    void box_clickedstate(bool flag);
    void changeDay(int count);



private:
    Ui::ICsetting *ui;
    QGroupBox * gropbox_one;
    QHBoxLayout *hbox;
    QRadioButton *radio1;
    QRadioButton *radio2;
    QString filename;
    ICList s_IClist;
    int flag;
    QStringList filename_list;
    QStringList findDiskFilePathNameList;
    QString findFilePathName;
    bool isNOExit;
};

#endif // ICSETTING_H
