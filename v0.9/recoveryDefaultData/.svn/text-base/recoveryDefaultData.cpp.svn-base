#include "recoveryDefaultData.h"
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QKeyEvent>
#include <QTextCodec>
#include <QFile>
#include <QVBoxLayout>


#define DEFULAT_TEXT        ("程序设置数据已损坏，是否恢复为默认设置？")
#define RECOVER_TEXT        ("正在恢复默认设置请稍后.....")
#define NO_RECOVER_TEXT        ("用户已放弃恢复,正在启动主程序.")
#define RECOVER_OK        ("恢复已完成,正在启动主程序.")


#define LOG_FILE                    ("/recovery_log")
#define KEY_TEXT_HEAD       ("按")
#define KEY_TEXT_END          ("键确认恢复，其他键或超时则不恢复.")
#define RECOVERY_SH             ("./data_backup.sh")
#define KILLMYSELF_CMD              ("killall recoveryDefaultData")
#define TOTAL_TIME 5


#define KEY_HOME_TYPE     ("HOME")
#define KEY_NAVI_TYPE      ("NAVI")
#define KEY_RADIO_TYPE      ("RADIO")

extern char g_key_type[];
extern char g_key_value[];
extern char g_log_info[];
extern char g_mount_path[];



recoveryDefaultDataC::recoveryDefaultDataC(QWidget *parent)
    : QWidget(parent),m_timer_count(0),m_state(0)
{
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    Qt::WindowFlags flags = Qt::FramelessWindowHint;
    setWindowFlags (flags);
    QPalette palette;
    QBrush brush(QColor(0, 0, 2));
    palette.setBrush(QPalette::Active, QPalette::Window, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
    this->setPalette(palette);
    showFullScreen();
//    setGeometry(0, 0, 800, 480);
    m_info = new QLabel(this);
    m_info->setText(DEFULAT_TEXT);
    m_keyinfo = new QLabel(this);

    QFont l_font("fzhei");
    if(size().width()==800 && size().height()==480)//800x480
    {
         l_font.setPointSize(88);
    }
    else
          l_font.setPointSize(58);
    m_info->setFont( l_font );
    m_keyinfo->setFont( l_font );

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    m_info->setPalette(pa);
    m_keyinfo->setPalette(pa);
    m_info->setAlignment( Qt::AlignCenter);
    //m_info->setGeometry(0,120,800,50);
    m_keyinfo->setAlignment( Qt::AlignCenter);
    //m_keyinfo->setGeometry(0,200,800,50);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_info);
    mainLayout->addWidget(m_keyinfo);
    setLayout(mainLayout);


    QString s_key_type = getRecoveryKeyType();
     m_keyinfo->setText(s_key_type);

    m_info_timer.setInterval(1000);
    connect(&m_info_timer,SIGNAL(timeout()),this,SLOT(on_timer_out()));
    m_info_timer.start();
}


void recoveryDefaultDataC::writeLogFile(void)
{
        QString m_text = QDateTime::currentDateTime().toString();
        m_text = m_text +QString("\n")+QString(g_log_info);
        printf("log:\n %s \n",m_text.toLocal8Bit().data());
        QString m_path = QString(g_mount_path)+LOG_FILE;
        QFile l_file(m_path);
       if(l_file.open(QFile::ReadWrite))
       {
            l_file.write(m_text.toLocal8Bit().data());
            l_file.close();
       }
}

QString recoveryDefaultDataC::getRecoveryKeyType(void)
{
    QString m_keytype = g_key_type;
    QString res = QString(KEY_TEXT_HEAD) + m_keytype + QString(KEY_TEXT_END);


    return res;
}
int recoveryDefaultDataC::getRecoveryKeyValue(void)
{
    QString m_key_value = g_key_value;
     return m_key_value.toInt();
}
void recoveryDefaultDataC::keyReleaseEvent(QKeyEvent *e)
{
    int key_value = getRecoveryKeyValue();
    int i_key = e->key();
    printf(":____________keyReleaseEvent %d \n",i_key);
    if(key_value == e->key() )
    {
        m_info_timer.stop();
        m_info->setText(RECOVER_TEXT);
        m_keyinfo->hide();
        this->update();
        system(RECOVERY_SH);
        m_timer_count = TOTAL_TIME -1;
        m_info_timer.start(1000);
        m_state = 2;
        writeLogFile();

    }
    else
    {
        m_info->setText(NO_RECOVER_TEXT);
        m_keyinfo->hide();

        m_info_timer.stop();
        m_timer_count = TOTAL_TIME -1;
        m_info_timer.start(1000);
        m_state = 1;
    }
}

void recoveryDefaultDataC::keyPressEvent(QKeyEvent *e)
{
    int key_value = getRecoveryKeyValue();
    int i_key = e->key();
    printf(":_____keyPressEvent %d \n",i_key);
    if(key_value == e->key() )
    {
        m_info_timer.stop();
        m_info->setText(RECOVER_TEXT);
        m_keyinfo->hide();
        this->update();
        m_state = 2;

    }
    else
    {
        m_info->setText(NO_RECOVER_TEXT);
        m_keyinfo->hide();
        m_info_timer.stop();
        m_state = 1;
    }
}

void recoveryDefaultDataC::on_timer_out(void)
{
    QString s_text;
    switch(m_state)
    {
        case 0:
            s_text = QString(DEFULAT_TEXT) +QString(" ") + QString::number(TOTAL_TIME - m_timer_count) ;
            break;
        case 1:
           s_text = QString(NO_RECOVER_TEXT);
           break;
        case 2:
            s_text = QString(RECOVER_OK) ;
             break;
    }
    m_info->setText(s_text);
    m_timer_count ++;
    if(m_timer_count > TOTAL_TIME)
    {
        if(0 == m_state)
        {
            m_info->setText(NO_RECOVER_TEXT);
            m_keyinfo->hide();
            m_info_timer.stop();
            m_timer_count = TOTAL_TIME -1;
            m_info_timer.start(1000);
            m_state = 1;
        }
        else
        {
            m_info_timer.stop();
            system(KILLMYSELF_CMD);
        }
    }
}

recoveryDefaultDataC::~recoveryDefaultDataC()
{

}
