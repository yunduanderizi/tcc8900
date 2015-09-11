#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include <QWidget>
#include "savesetting.h"
#include "icsetting.h"


namespace Ui {
    class mainwidget;
}

class mainwidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainwidget(QWidget *parent = 0);
    ~mainwidget();
    void setupUI();

private:
    Ui::mainwidget *ui;
    SaveSetting *setting;
    ICsetting *IC_setting;
};

#endif // MAINWIDGET_H
