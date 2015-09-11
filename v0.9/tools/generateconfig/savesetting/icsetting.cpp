#include <stdlib.h>
#include <stdio.h>
#include <QDebug>
#include <cmath>

#include "icsetting.h"
#include "ui_icsetting.h"
#include "FileListUtil.h"

ICsetting::ICsetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICsetting)
{
    flag =1;
    isNOExit = true;
    this->setFixedSize(966,510);
    ui->setupUi(this);
    setupUI();
}

ICsetting::~ICsetting()
{
    delete ui;
}
void ICsetting::setupUI()
{
    QStringList listyear ;
    QStringList listMonth;
    QStringList listday;
    gropbox_one = new QGroupBox(this);
    gropbox_one->setGeometry(500,300,180,50);
    radio1 = new QRadioButton(tr("驾驶员"),gropbox_one);
    radio2 = new QRadioButton(tr("管理员"),gropbox_one);
    radio1->setChecked(true);
    s_IClist.driving_name=0;
    hbox = new QHBoxLayout;
    hbox->addWidget(radio1);
    hbox->addWidget(radio2);
    hbox->addStretch(1);
    connect(radio1,SIGNAL(clicked(bool)),this,SLOT(box_clickedstate(bool)));
    connect(radio2,SIGNAL(clicked(bool)),this,SLOT(box_clickedstate(bool)));
    gropbox_one->setLayout(hbox);
    ui->Edit_drivingnum->setMaxLength(18);
    ui->Edit_card->setMaxLength(18);
    ui->Edit_drivingnum->setText("123456789012345678");
    ui->Edit_card->setText("234567890123456789");
    ui->Edit_drivingphone->setText("133654798");
    ui->Edit_drivingphone->setMaxLength(11);
    ui->Edit_drivingpolicynum->setText("77293759");
    ui->Edit_carpolicynum->setText("7895232223");
    ui->Edit_emergencycontactperson->setText("张三");
    ui->Edit_contactphonenum->setText("13154964873");
    ui->Edit_contactphonenum->setMaxLength(11);
//    ui->EditTCB->hide();
//    ui->lbTCB->hide();
    //Year
    for(int i =2014;i <= 2050;i++)
    {
        listyear.append(QString::number(i));
    }
    ui->ComboBoxyear->addItems(listyear);
    ui->ComboBoxyear->setMaxVisibleItems(10);
    connect(ui->ComboBoxyear,SIGNAL(currentIndexChanged(int)),this,SLOT(changeDay(int)));

    //Month
    for(int i =1 ;i <= 12;i++)
    {
        listMonth.append(QString::number(i));
    }
    ui->ComboBoxmonth->addItems(listMonth);
    ui->ComboBoxmonth->setMaxVisibleItems(10);
    connect(ui->ComboBoxmonth,SIGNAL(currentIndexChanged(int)),this,SLOT(changeDay(int)));

    //day
    for(int i=1;i<=31;i++)
    {
        listday.append(QString::number(i));
    }
    ui->ComboBoxday->addItems(listday);
    ui->ComboBoxday->setMaxVisibleItems(10);
    QDir p_dir;
    filename = p_dir.currentPath();
    ui->Editpath->setText(filename);

}
void ICsetting::changeDay(int day)
{
    int year;
    int month;
    QStringList listDay;
    year = ui->ComboBoxyear->currentText().toInt();
    month = ui->ComboBoxmonth->currentText().toInt();
    ui->ComboBoxday->clear();
    if((year%4==0) && (year%100!=0) || (year%400 ==0))
    {
        if(month == 2)
        {
            listDay.clear();
            for(int i =1;i <= 29;i++)
            {
                listDay.append(QString::number(i));
            }
            ui->ComboBoxday->addItems(listDay);
        }
        else if(month ==4 || month ==6|| month== 9|| month == 11)
        {
            //4,6,9,11
            listDay.clear();
            for(int i =1;i <= 30;i++)
            {
                listDay.append(QString::number(i));
            }
            ui->ComboBoxday->addItems(listDay);
        }
        else
        {
            //1,3,5,7,8,10,12
            listDay.clear();
            for(int i =1;i <= 31;i++)
            {
                listDay.append(QString::number(i));
            }
            ui->ComboBoxday->addItems(listDay);
        }
    }
    else
    {
        if(month ==2)
        {
            listDay.clear();
            for(int i =1;i <= 28;i++)
            {
                listDay.append(QString::number(i));
            }
            ui->ComboBoxday->addItems(listDay);
        }
        else if(month==4 || month==6 || month==9 || month==11)
        {
            //4,6,9,11
            listDay.clear();
            for(int i =1;i <= 30;i++)
            {
                listDay.append(QString::number(i));
            }
            ui->ComboBoxday->addItems(listDay);
        }
        else
        {
            //1,3,5,7,8,10,12
            listDay.clear();
            for(int i =1;i <= 31;i++)
            {
                listDay.append(QString::number(i));
            }
            ui->ComboBoxday->addItems(listDay);
        }
    }

}

void ICsetting::getText()
{
    QString monthstr,daystr;
    QDateTime Dtime = QDateTime::currentDateTime();
    QString time = Dtime.toString("HH-mm-ss");
    //s_IClist.TCB = ui->EditTCB->text();
    s_IClist.driving_num = ui->Edit_drivingnum->text();
    s_IClist.seniority_card = ui->Edit_card->text();
    if(ui->ComboBoxmonth->currentText().toInt() <10)
        monthstr = "0" + ui->ComboBoxmonth->currentText();
    else
        monthstr = ui->ComboBoxmonth->currentText();
    if(ui->ComboBoxday->currentText().toInt() < 10)
        daystr = "0" + ui->ComboBoxday->currentText();
    else
        daystr = ui->ComboBoxday->currentText();
    s_IClist.valid_date=ui->ComboBoxyear->currentText().right(2)+'-'+monthstr+'-'+daystr +'-'+ time;

    s_IClist.driving_phone = ui->Edit_drivingphone->text();
    s_IClist.driving_policynum = ui->Edit_drivingpolicynum->text();
    s_IClist.car_policynum = ui->Edit_carpolicynum->text();
    s_IClist.emergencycontact_person = ui->Edit_emergencycontactperson->text();
    s_IClist.contact_phonenum = ui->Edit_contactphonenum->text();
}

int ICsetting::save_setting(QString fileName, QString fileTitle, int m_postion, int playState)
{
    //qDebug()<<"save setting................";
    FileListUtil util(fileName, this, fileTitle);
    QStringList List, ListPlayInfo;
    // List.append(QString("%1").arg(s_IClist.TCB));
    List.append(QString("%1").arg(s_IClist.driving_num));
    List.append(QString("%1").arg(s_IClist.driving_name));
    List.append(QString("%1").arg(s_IClist.seniority_card));
    List.append(QString("%1").arg(s_IClist.valid_date));
    List.append(QString("%1").arg(s_IClist.driving_phone));
    List.append(QString("%1").arg(s_IClist.driving_policynum));
    List.append(QString("%1").arg(s_IClist.car_policynum));
    List.append(QString("%1").arg(s_IClist.emergencycontact_person));
    List.append(QString("%1").arg(s_IClist.contact_phonenum));

    ListPlayInfo.append(QString("%1").arg(s_IClist.driving_num));
    util.saveFileListOther(List, ListPlayInfo,IC_Setting);
    return 1;
}
void ICsetting::box_clickedstate(bool flag)
{
    if(radio1->isChecked())
    {
        s_IClist.driving_name=0;//驾驶员
    }
    else
        if(radio2->isChecked())
        {
            s_IClist.driving_name=1;
        }
}

void ICsetting::on_pathButton_clicked()
{
    QString dir;
    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                            "",
                                            QFileDialog::ShowDirsOnly
                                            | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
        filename = dir;
    ui->Editpath->setText(filename);


}
void ICsetting::findDiskPathName(QString findDiskFilePathName, int type)
{
    findDiskFilePathNameList<<findDiskFilePathName;
    if(findDiskFilePathName.contains("ICsetting",Qt::CaseInsensitive))
    {
        this->filename_list << findDiskFilePathName ;
    }
}
void ICsetting::find_file()
{
    QDir dir(filename);
    //qDebug()<<"***********findfilepathname="<<filename;
    QStringList currentDirs = dir.entryList(QDir::Dirs, QDir::Name);
    QStringList filters;
    filters <<"*.xml";
    foreach (QString sContentDir, currentDirs)
    {
        int addCount = 0;//璁板綍宸茬粡妫�绱㈠埌鐨勬枃浠朵釜鏁?
        if(isNOExit)
        {
            QDir findDir(filename+QString(QDir::separator())+ sContentDir);//鏂囦欢鐨勭粷瀵硅矾寰?
            findDir.setNameFilters(filters);
            findDir.setSorting(QDir::DirsFirst|QDir::Name);
            QFileInfoList fl = findDir.entryInfoList( QDir::Files );
            while(!fl.isEmpty())
            {
                if(isNOExit)
                {
                    if(addCount >= 1000)
                    {
                        isNOExit = true;
                        //emit findDiskPathFinished();
                    }
                    QString tmp = fl.takeFirst().absoluteFilePath();
                    //qDebug()<<"find -------------------------------------------- "<<tmp<<addCount;
                    findDiskPathName(tmp, addCount);
                    addCount++;
                }
            }
        }
    }
}


void ICsetting::on_SaveButton_clicked()
{
    find_file();
    if(NULL != ui->Editpath->text())
    {
        getText();
        if(ui->Edit_drivingnum->text()==NULL ||ui->Edit_card->text() == NULL|| ui->Edit_drivingphone->text() == NULL ||
                ui->Edit_drivingpolicynum->text() == NULL || ui->Edit_carpolicynum->text() == NULL ||
                ui->Edit_emergencycontactperson->text() == NULL || ui->Edit_contactphonenum->text()== NULL)
        {
            QMessageBox::warning(this,tr("警告"),tr("带*的为必填项!"),QMessageBox::Yes);
        }
        else
            if(s_IClist.driving_num.count()!=ui->Edit_drivingnum->maxLength())
            {
                QMessageBox::warning(this,tr("警告"),tr("输入的驾证号位数不正确!"),QMessageBox::Yes);
                //return;
            }
            else
                if(s_IClist.seniority_card.count()!=ui->Edit_card->maxLength())
                {
                    QMessageBox::warning(this,tr("警告"),tr("输入的从业资格证位数不正确!"),QMessageBox::Yes);
                    //return;
                }
                else
                {
                    int count = filename_list.count();
                    QString m_name;
                    if( 0 == count)
                    {
                        m_name = "ICsetting.xml";
                    }
                    else
                    {
                        m_name = QString("ICsetting"+QString::number(count)+".xml");
                    }
                    QString f_name = QString(filename+"/"+ m_name);
                    bool flag =save_setting(f_name,"<!DOCTYPE ICInfo>",0,0);
                    if(flag)
                    {
                        QMessageBox::information(this,tr("提示"),tr("文件生成成功，生成的文件名为")+m_name,QMessageBox::Yes);
                    }
                    else
                    {
                        QMessageBox::information(this,tr("提示"),tr("文件生成错误！"),QMessageBox::Yes);
                    }
                }
    }
    else
    {
        QMessageBox::warning(this,tr("警告"),tr("请选择保存的路径!"),QMessageBox::Yes);
    }
    this->filename_list.clear();
    findDiskFilePathNameList.clear();


}
