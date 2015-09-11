#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLCDNumber>
#include <QSlider>
#include "ibmediacontrolc.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QPushButton m_play;
    QPushButton m_stop;
    QPushButton m_pause;
    QPushButton m_fullscreen;
    QPushButton m_normalscreen;
    QPushButton m_close;

    QLabel   m_media_filenme;

    QLCDNumber m_totaltime;
    QLCDNumber m_currenttime;
    QSlider m_progress;
    int m_i_totaltime;
    int m_seek;

    QString m_filename;
    MediaControl::IBMediaControlC * m_mplayer;
  public slots:
    void on_fullscreen_click();
    void on_normalscreen_click();
    void on_play_click();
    void on_stop_click();
    void on_pause_click();
    void on_current_time_changed(int,QString,int,QString);
    void on_mplayer_state_changed(int,int);
    void on_mediaChanged(QString);
    void on_slider_move(int);
    void on_slider_release(void);
    void on_close_click(void);
};

#endif // WIDGET_H
