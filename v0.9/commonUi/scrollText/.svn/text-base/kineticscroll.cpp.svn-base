#include <QTime>
#include <QTimer>

#include "kineticscroll.h"

//#define MAX_SPEED 2000
#define MAX_SPEED 200/*速度最大值*/


namespace chinatsp_common_gui_ui
{
class KineticScrollPrivate
{
public:
    QTimer *animation;
    qreal animationSpeed;
    QTime animationITime;
    qreal animationAccel;
    qreal accelConstant;
    QTime time;
    QTime lastTime;
    int value;
    int lastValue;
};
}
using namespace chinatsp_common_gui_ui;

KineticScroll::KineticScroll(QObject *parent)
    : QObject(parent), d(new KineticScrollPrivate)
{
    d->animation = 0;
    d->accelConstant = 0.95;/*减速度*/
    d->value = d->lastValue = -1;
    d->time = d->lastTime = QTime::currentTime();
}

KineticScroll::~KineticScroll()
{
    delete d;
}

void KineticScroll::mouseUp(int value)
{
    if (d->lastValue < 0 || d->animation)
        return;

    QTime t = QTime::currentTime();
    qreal dv = value - d->lastValue;
    qreal dt = (qreal)d->lastTime.msecsTo(t) / 500;

    if (dt == 0)
        return;

    kineticStart(dv / dt);
    mouseCancel();
}

bool KineticScroll::mouseDown(int value)
{
    bool r = true;
    if (d->animation) {
        d->animation->stop();
        d->animation->deleteLater();
        d->animation = 0;
        r = false;
    }
    d->lastValue = d->value = value;
    d->lastTime = d->time = QTime::currentTime();
    return r;
}

void KineticScroll::mouseMove(int value)
{
    if (d->lastValue < 0)
        return;

    int dv = value - d->value;
    QTime t = QTime::currentTime();

    d->lastValue = d->value;
    d->lastTime = d->time;
    d->value = value;
    d->time = t;

    emit signalMoveOffset(dv);
}

void KineticScroll::mouseCancel()
{
    d->value = -1;
    d->lastValue = -1;
}

void KineticScroll::kineticStop()
{
    if (d->animation) {
        d->animation->stop();
        d->animation->deleteLater();
    }

    d->animation = 0;

    // signal kinetic scroll stop
    emit signalKineticStop();
}

void KineticScroll::kineticStart(qreal speed)
{
    d->animationSpeed = qBound<qreal>(-MAX_SPEED, speed, MAX_SPEED);
    d->animationITime = QTime::currentTime();
    d->animationAccel = -d->animationSpeed * d->accelConstant;

    d->animation = new QTimer();
    connect(d->animation, SIGNAL(timeout()), this, SLOT(animator()));
    d->animation->start(10);
}

void KineticScroll::animator()
{
    QTime now = QTime::currentTime();
    qreal dt = (qreal)d->animationITime.msecsTo(now) / 1000;
    qreal speed = d->animationSpeed + d->animationAccel * dt;
    qreal value = d->animationSpeed * dt + d->animationAccel * dt * dt / 2;

    if (d->animationAccel * speed > 0) {
        // just to notify animation finish
        emit signalMoveOffset(0);
        kineticStop();
    } else {
        emit signalMoveOffset(qRound(value));
        d->animationSpeed = speed;
        d->animationITime = now;
    }
}
