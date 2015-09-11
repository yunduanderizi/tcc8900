#ifndef RECOVERYDEDAULTDATA_H
#define RECOVERYDEDAULTDATA_H

#include <QtGui/QWidget>
#include <QTimer>
class QLabel;
class recoveryDefaultDataC : public QWidget
{
    Q_OBJECT
public:
    QString getRecoveryKeyType(void);
    int getRecoveryKeyValue(void);
    recoveryDefaultDataC(QWidget *parent = 0);
    void writeLogFile(void);
    ~recoveryDefaultDataC();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

public slots:
    void on_timer_out(void);
private:
    QLabel *m_info;
    QLabel *m_keyinfo;
    QTimer  m_info_timer;
    int m_timer_count;
    int m_state;
};

#endif // RECOVERYDEDAULTDATA_H
