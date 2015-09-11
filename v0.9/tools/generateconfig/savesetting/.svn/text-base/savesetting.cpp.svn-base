#include <stdlib.h>
#include <stdio.h>
#include <QDebug>


#include "savesetting.h"
#include "FileListUtil.h"
#include "ui_savesetting.h"

SaveSetting::SaveSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveSetting)
{
    flag =1;
    terminal_type =0;
    isNOExit = true;
    this->setFixedSize(966,510);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    setupUI();

}

SaveSetting::~SaveSetting()
{
    delete ui;
}
void SaveSetting::setupUI()
{
    QStringList listyear ;
    QStringList listMonth;
    QStringList listday;


    QStringList carplatetypelist;
    carplatetypelist.append("重型载货汽车");
    carplatetypelist.append("中型载货汽车");
    carplatetypelist.append("轻型载货汽车");
    carplatetypelist.append("微型载货汽车");
    carplatetypelist.append("外资企业货车");
    QStringList carcolorlist;
    carcolorlist.append("1. 蓝色");
    carcolorlist.append("2. 黄色");
    carcolorlist.append("3. 黑色");
    carcolorlist.append("4. 白色");
    provicejList<<"京"<<"津"<<"冀"<<"晋"<<"蒙"<<"辽"<<"吉"<<"黑"<<"沪"<<"苏"<<"浙"<<"皖"
                  <<"闽"<<"赣"<<"鲁"<<"豫"<<"鄂"<<"湘"<<"粤"<<"桂"<<"琼"<<"渝"<<"川"
                 <<"贵"<<"云"<<"藏"<<"陕"<<"甘"<<"青"<<"宁"<<"新"<<"台"<<"港"
                <<"澳";


    reader=new QXmlStreamReader;
    //    QStringList proviceList;
    QFile file("provinces.xml");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"File has loaded...";
        read(&file);
        qDebug()<<proviceList.size();
//        qDebug()<<cityList.size();
    }
    else
    {
        qDebug()<<"No file to read...";
    }
    file.close();

    cityVector.append(beijingList);
    cityVector.append(tianjingList);
    cityVector.append(hebeiList);
    cityVector.append(shanxiList);
    cityVector.append(neimengguList);
    cityVector.append(liaoningList);
    cityVector.append(jilinList);
    cityVector.append(heilongjiangList);
    cityVector.append(shanghaiList);
    cityVector.append(jiangsuList);

    cityVector.append(zhejiangList);
    cityVector.append(anhuiList);
    cityVector.append(fujianList);
    cityVector.append(jiangxiList);
    cityVector.append(shandongList);
    cityVector.append(henanList);
    cityVector.append(hubeiList);
    cityVector.append(hunanList);
    cityVector.append(guangdongList);
    cityVector.append(guangxiList);

    cityVector.append(hainanList);
    cityVector.append(chongqingList);
    cityVector.append(sichuanList);
    cityVector.append(guizhouList);
    cityVector.append(yunnanList);
    cityVector.append(xizangList);
    cityVector.append(sanxiList);
    cityVector.append(gansuList);
    cityVector.append(qinghaiList);
    cityVector.append(ningxiaList);

    cityVector.append(xinjiangList);
    cityVector.append(taiwanList);
    cityVector.append(xianggangList);
    cityVector.append(aomenList);


//    qDebug()<<cityVector.count();

//add povicesid into  proviceidmap
    for(int i =0;i < 34;i++){

        proviceMap.insert(proviceList.at(i),proviceidList.at(i));
    }


    ui->comboBoxprovinceid->addItems(proviceList);
    connect(ui->comboBoxprovinceid,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCity()));

//    int cityid=100;
//    char strcity[20];
//    itoa(cityid, strcity, 10);
//    ui->EditcityID->setText(strcity);
    ui->comboBoxcityid->addItems(beijingList);

    ui->EditmakerID->setMaxLength(5);
    ui->EditmakerID->setText("70319");
    ui->EditmakerID->setReadOnly(true);

    //ui->Editversion->setMaxLength(20);

    ui->Editversion->setText("JL901");
    ui->Editterminal->setMaxLength(7);
    ui->Editterminal->setText("");
    ui->EditVIN->setMaxLength(17);
    ui->EditVIN->setText("12345678912345678");
//    int carcolor=2;
//    char strcolor[20];
//    itoa(carcolor, strcolor, 10);
    ui->comboBoxCarcolor->addItems(carcolorlist);

    ui->EditcarNUM->setMaxLength(6);
    ui->comboBoxprovince->addItems(provicejList);
    ui->comboBoxCarpalteType->addItems(carplatetypelist);
    QString plate="NB1234";
    ui->EditcarNUM->setText(plate);


//    ui->EditpalteType->setMaxLength(20);
//    QString platetype="大型汽车";
//    ui->EditpalteType->setText(platetype);
    //installation time
//    QDateTime Dtime =  QDateTime::currentDateTime();
//    QString text = Dtime.toString("yy-MM-dd-HH-mm-ss");
//    ui->EditinstallTime->setText(text);

    int mileage=123;
    char strmileage[20];
    itoa(mileage, strmileage, 10);
    ui->Editinstallmeliage->setText(strmileage);
    int rate = 90;
    char strrate[25];
    itoa(rate, strrate, 10);
    ui->EditSpeedratio->setText(strrate);
    int Ele =8;
    char strele[20];
    itoa(Ele,strele,10);
    ui->Editsensingelement->setText(strele);
    ui->EdittelNUM->setMaxLength(11);
    ui->EdittelNUM->setText("12345678912");
    ui->EditauthCode->setMaxLength(32);
    ui->EditauthCode->setText("");
    ui->EditmainIP->setText("111.75.223.134");
    ui->EditsubIP->setText("111.75.223.134");
    int portNnm = 9094;
    char strportnum[25];
    itoa(portNnm, strportnum, 10);
    ui->EditportNUM->setText(strportnum);
    //add 12-12-13 speed alarm
    int speed = 80;
    char strspeed[25];
    itoa(speed, strspeed, 10);
    ui->Speed->setText(strspeed);
    //add 13-01-22 drivedura 超时驾驶连续时间
    int drivedura = 180;
    char strdrivedura[40];
    itoa(drivedura,strdrivedura,10);
    ui->drivedura->setText(strdrivedura);

    QDir p_dir;
    filename = p_dir.currentPath();
    ui->Editpath->setText(filename);

    //Year
    for(int i =2012;i <= 2050;i++)
    {
        listyear.append(QString::number(i));
    }
    ui->ComboBoxyear->addItems(listyear);
    ui->ComboBoxyear->setMaxVisibleItems(10);
    connect(ui->ComboBoxyear,SIGNAL(currentIndexChanged(int)),this,SLOT(changeDay(int)));

    //Month
    for(int i = 1;i <= 12;i++)
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
//add 2013-4-2
  ui->radioButton_1->setChecked(true);
  ui->radioButton_4->setChecked(true);
  ui->radioButton_5->setChecked(true);
  ui->radioButton_8->setChecked(true);
  ui->radioButton_10->setChecked(true);
  ui->radioButton_12->setChecked(true);
}
//---------------------------------------------------->
void SaveSetting::changeCity()
{
    ui->comboBoxcityid->clear();
   int i =  ui->comboBoxprovinceid->currentIndex();
   ui->comboBoxcityid->addItems(cityVector[i]);
}
//---------------------------------------------------->
void SaveSetting::changeDay(int day)
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

int SaveSetting::getText()
{
    QDateTime Dtime =  QDateTime::currentDateTime();
    QString time = Dtime.toString("HH-mm-ss");
//    s_listinfo.province =ui->EditProvinceID->text().toInt();
//    s_listinfo.city = ui->EditcityID->text().toInt();
    s_listinfo.province = proviceMap.value(ui->comboBoxprovinceid->currentText());
    s_listinfo.city = cityMap.value(ui->comboBoxcityid->currentText());
    s_listinfo.maker = ui->EditmakerID->text();
    s_listinfo.model = ui->Editversion->text();
    s_listinfo.termed = ui->Editterminal->text();
    s_listinfo.VIN = ui->EditVIN->text();
    s_listinfo.platecolor = ui->comboBoxCarcolor->currentIndex()+1;
    s_listinfo.plate = ui->comboBoxprovince->currentText()+ui->EditcarNUM->text();
    s_listinfo.platetype = ui->comboBoxCarpalteType->currentText();
//    s_listinfo.time = ui->EditinstallTime->text();
//get install time
    QString year_str,month_str,day_str;
    if(ui->ComboBoxmonth->currentText().toInt()<10)
            month_str = QString("0"+ ui->ComboBoxmonth->currentText());
    else
            month_str = ui->ComboBoxmonth->currentText();
    if(ui->ComboBoxday->currentText().toInt()<10)
            day_str = QString("0"+ ui->ComboBoxday->currentText());
    else
            day_str = ui->ComboBoxday->currentText();
    year_str = ui->ComboBoxyear->currentText().right(2);
    s_listinfo.installtime = year_str+'-'+month_str+ '-' + day_str + '-'+ time;

    s_listinfo.mileage =ui->Editinstallmeliage->text().toInt()*10;
    s_listinfo.rate = ui->EditSpeedratio->text().toInt();
    s_listinfo.sensingelement = ui->Editsensingelement->text().toInt();
    s_listinfo.phone =ui->EdittelNUM->text();
    s_listinfo.auth= ui->EditauthCode->text();
    s_listinfo.main =ui->EditmainIP->text();
    s_listinfo.second = ui->EditsubIP->text();
    s_listinfo.port = ui->EditportNUM->text().toInt();
    s_listinfo.speed = ui->Speed->text().toInt();
    s_listinfo.drivedura = ui->drivedura->text().toInt()*60;
    s_listinfo.terminaltype = get_type();
    s_listinfo.hadversion = ui->hadversion->text();
    if( ui->EditmakerID->text() == NULL
            || ui->Editversion->text() == NULL ||ui->EditVIN->text() == NULL || ui->EditcarNUM->text() == NULL
           || ui->Editinstallmeliage->text() == NULL|| ui->EdittelNUM->text() == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

QString SaveSetting::get_type()
{
    char tmp = 0x1;
    QString type;
    //int tmp =1;
    char str[10] ="";

        terminal_type = ui->radioButton_1->isChecked()? terminal_type |= tmp:terminal_type;

        terminal_type = ui->radioButton_3->isChecked()? terminal_type |= tmp<<1:terminal_type;

        terminal_type = ui->radioButton_5->isChecked()? terminal_type |= tmp<<2:terminal_type;

        terminal_type = ui->radioButton_7->isChecked()? terminal_type |= tmp<<3:terminal_type;

        terminal_type = ui->radioButton_9->isChecked()? terminal_type |= tmp<<6:terminal_type;

        terminal_type = ui->radioButton_11->isChecked()? terminal_type |= tmp<<7:terminal_type;
        terminal_type &=0xff;
        sprintf(str,"%x",terminal_type);
        //itoa(terminal_type,str,16);
        type = "0x" + QString(str).right(2);
        return type;
}

int SaveSetting::save_setting(QString fileName, QString fileTitle, int m_postion, int playState)
{
    //qDebug()<<"save setting................";
    FileListUtil util(fileName, this, fileTitle);
    QStringList List, ListPlayInfo;
    List.append(QString("%1").arg(s_listinfo.province));
    List.append(QString("%1").arg(s_listinfo.city));
    List.append(QString("%1").arg(s_listinfo.maker));
    List.append(QString("%1").arg(s_listinfo.model));
    List.append(QString("%1").arg(s_listinfo.termed));
    List.append(QString("%1").arg(s_listinfo.VIN));
    List.append(QString("%1").arg(s_listinfo.platecolor));
    List.append(QString("%1").arg(s_listinfo.plate));
    List.append(QString("%1").arg(s_listinfo.platetype));
    List.append(QString("%1").arg(s_listinfo.installtime));
    List.append(QString("%1").arg(s_listinfo.mileage));
    List.append(QString("%1").arg(s_listinfo.rate));
    List.append(QString("%1").arg(s_listinfo.sensingelement));
    List.append(QString("%1").arg(s_listinfo.phone));
    List.append(QString("%1").arg(s_listinfo.auth));
    List.append(QString("%1").arg(s_listinfo.main));
    List.append(QString("%1").arg(s_listinfo.second));
    List.append(QString("%1").arg(s_listinfo.port));
    List.append(QString("%1").arg(s_listinfo.speed));
    List.append(QString("%1").arg(s_listinfo.drivedura));
    List.append(QString("%1").arg(s_listinfo.terminaltype));
    List.append(QString("%1").arg(s_listinfo.hadversion));

    ListPlayInfo.append(QString("%1").arg(s_listinfo.province));
    util.saveFileListOther(List, ListPlayInfo,base_setting);
    return 1;
}
void SaveSetting::on_pathButton_clicked()
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
void SaveSetting::findDiskPathName(QString findDiskFilePathName, int type)
{
    findDiskFilePathNameList<<findDiskFilePathName;
    //qDebug()<<"findDiskFilePathName ="<<findDiskFilePathName;
    if(findDiskFilePathName.contains("Basesetting",Qt::CaseInsensitive))
    {
        this->filename_list << findDiskFilePathName ;
    }
}
void SaveSetting::find_file()
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

void SaveSetting::on_SaveButton_clicked()
{
    bool flag;
    find_file();
    if(NULL != ui->Editpath->text())
    {
        flag = getText();
        if(flag)
        {
            Judge();
        }
        else
        {
            QMessageBox::warning(this,tr("警告"),tr("带*的为必填项!"),QMessageBox::Yes);
        }
    }
    else
    {
        QMessageBox::warning(this,tr("警告"),tr("请选择保存的路径!"),QMessageBox::Yes);
    }
    this->filename_list.clear();
    findDiskFilePathNameList.clear();
}
//-------read provices from file---------------------------------------->>
bool SaveSetting::read(QIODevice *device)
{
    reader->setDevice(device);
    while(!reader->atEnd())
    {
        reader->readNext();
        if(reader->isStartElement())
        {
            if(reader->name() == "Provinces")
            {
                read_data();
            }
        }
    }
    return !reader->error();
}
void SaveSetting::read_data()
{
    qDebug()<<"Start to read data...";
    while(!reader->atEnd())
                  {
                      reader->readNext();
                      if(reader->isStartElement())
                      {
                          QStringList list;
                          if(reader->name() == "Province"){
                              proviceList.append(reader->readElementText());
                          }
                          if(reader->name() == "beijing"){

                               list = reader->readElementText().split(" ");
                               beijingList<<list[0];
                               cityMap.insert(list[0],list[1]);
                               list.clear();
                          }
                          //-------------------------------------------------------->
                          if(reader->name() == "tianjing"){
//                              tianjingList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              tianjingList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "hebei"){
//                              hebeiList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              hebeiList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "shanxi"){
//                              shanxiList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              shanxiList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "liaoning"){
//                              liaoningList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              liaoningList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "jilin"){
//                              jilinList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              jilinList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "heilongjiang"){
//                              heilongjiangList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              heilongjiangList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "shanghai"){
//                              shanghaiList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              shanghaiList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "jiangsu"){
//                              jiangsuList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              jiangsuList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "zhejiang"){
//                              zhejiangList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              zhejiangList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "anhui"){
//                              anhuiList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              anhuiList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          //------------------------------------------------------------->

                          if(reader->name() == "fujian"){
//                              fujianList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              fujianList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "jiangxi"){
//                              jiangxiList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              jiangxiList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "shandong"){
//                              shandongList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              shandongList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "henan"){
//                              henanList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              henanList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "hubei"){
//                              hubeiList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              hubeiList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "hunan"){
//                              hunanList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              hunanList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "guangdong"){
//                              guangdongList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              guangdongList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "xinjiang"){
//                              xinjiangList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              xinjiangList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "hainan"){
//                              hainanList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              hainanList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "chongqing"){
//                              chongqingList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              chongqingList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                  //------------------------------------------------------------->
                          if(reader->name() == "sichuan"){
//                              sichuanList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              sichuanList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "guizhou"){
//                              guizhouList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              guizhouList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }


                          if(reader->name() == "sanxi"){
//                              sanxiList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              sanxiList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }

                        if(reader->name() == "yunnan"){
//                              yunnanList<<reader->readElementText();
                            list = reader->readElementText().split(" ");
                            yunnanList<<list[0];
                            cityMap.insert(list[0],list[1]);
                            list.clear();
                        }

                          if(reader->name() == "gansu"){
//                              gansuList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              gansuList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "qinghai"){
//                              qinghaiList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              qinghaiList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }

                          if(reader->name() == "xizang"){
//                              xizangList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              xizangList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "guangxi"){
//                              guangxiList<<reader->readElementText();

                              list = reader->readElementText().split(" ");
                              guangxiList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }

                          if(reader->name() == "neimenggu"){
//                              neimengguList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              neimengguList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "ningxia"){
//                              ningxiaList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              ningxiaList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }

                          //---------------------------------------------->
                          if(reader->name() == "aomen"){
//                              aomenList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              aomenList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "xianggang"){
//                              xianggangList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              xianggangList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "taiwan"){
//                              taiwanList<<reader->readElementText();
                              list = reader->readElementText().split(" ");
                              taiwanList<<list[0];
                              cityMap.insert(list[0],list[1]);
                              list.clear();
                          }
                          if(reader->name() == "proviceid"){
                              proviceidList<<reader->readElementText();
                          }
                      }
                      if(reader->isEndElement())
                      {
                          if(reader->name() == "Provinces")
                          {
                              qDebug()<<"Now at the end of data element...";
                              break;
                          }
                      }
                  }
//    qDebug()<< "list size is "<< list.at(3);
}
//---------------------------------------------------------------------------------->>
void SaveSetting::Judge()
{
//    if(s_listinfo.province >=0 && s_listinfo.province <=99)
//    {
//        if(s_listinfo.city >=0 && s_listinfo.city <=9999)
//        {
            if(s_listinfo.maker.length() == 5)
            {
                if(s_listinfo.VIN.length() == 17)

                {
                    if(s_listinfo.plate.length() == 7)
                    {
//                        if(s_listinfo.platetype.length() <10)
//                        {
                            if(s_listinfo.mileage >0)
                            {
                                if(s_listinfo.rate >=0 && s_listinfo.rate <= 65535)
                                {
                                    if(s_listinfo.sensingelement >=0 && s_listinfo.sensingelement <=255)
                                    {
                                        if(s_listinfo.phone.length() == 11)
                                        {
                                            int count = filename_list.count();
                                            QString m_name;
                                            if( 0 == count)
                                            {
                                                m_name = "Basesetting.xml";
                                            }
                                            else
                                            {
                                                m_name = QString("Basesetting"+QString::number(count)+".xml");
                                            }
                                            QString f_name = QString(filename+"/"+ m_name);
                                            bool flag =save_setting(f_name/*"setting.xml"*/,"<!DOCTYPE SettingInfo>",0,0);
                                            if(flag)
                                            {
                                                QMessageBox::information(this,tr("提示"),tr("文件生成成功，生成的文件名为")+m_name,QMessageBox::Yes);
                                            }
                                            else
                                            {
                                                QMessageBox::information(this,tr("提示"),tr("文件生成错误！"),QMessageBox::Yes);
                                            }
                                        }
                                        else
                                        {
                                            QMessageBox::warning(this,tr("警告"),tr("输入的电话号码不正确!"),QMessageBox::Yes);
                                        }
                                    }
                                    else
                                    {
                                        QMessageBox::warning(this,tr("警告"),tr("输入的传感器单圈脉冲数不在范围内!"),QMessageBox::Yes);
                                    }
                                }
                                else
                                {
                                    QMessageBox::warning(this,tr("警告"),tr("输入的速度系数不在范围内!"),QMessageBox::Yes);
                                }
                            }
                            else
                            {
                                QMessageBox::warning(this,tr("警告"),tr("输入的安装里程值不在范围内!"),QMessageBox::Yes);
                            }
                        }
//                        else
//                        {
//                            QMessageBox::warning(this,tr("警告"),tr("输入的车牌类型不正确!"),QMessageBox::Yes);
//                        }
//                    }
                    else
                    {
                        QMessageBox::warning(this,tr("警告"),tr("输入的车牌号位数不正确!"),QMessageBox::Yes);
                    }
                }
                else
                {
                    QMessageBox::warning(this,tr("警告"),tr("输入的VIN位数不正确!"),QMessageBox::Yes);
                }
            }
            else
            {
                QMessageBox::warning(this,tr("警告"),tr("输入的制造商ID位数不正确!"),QMessageBox::Yes);
            }
        }
//        else
//        {
//            QMessageBox::warning(this,tr("警告"),tr("输入的城市ID不在范围内!"),QMessageBox::Yes);
//        }
//    }
//    else
//    {
//        QMessageBox::warning(this,tr("警告"),tr("输入的省ID不在范围内!"),QMessageBox::Yes);
//    }
//}
