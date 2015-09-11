#include "mainwidget.h"
#include "ui_mainwidget.h"

mainwidget::mainwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainwidget)
{
    this->setFixedSize(966,510);
    ui->setupUi(this);
    setupUI();
}

mainwidget::~mainwidget()
{
    delete ui;
}
void mainwidget::setupUI()
{
    setting = new SaveSetting(ui->tab_one);
    setting->setGeometry(0,0,966,510);

    IC_setting = new ICsetting(ui->tab_two);
    IC_setting->setGeometry(0,0,966,510);

}
