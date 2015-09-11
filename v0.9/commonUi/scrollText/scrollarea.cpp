#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "scrollarea.h"
#include "kineticscroll.h"


using namespace chinatsp_common_gui_ui;

ScrollAreaPrivate::ScrollAreaPrivate(ScrollArea *s)
    : offset(0),
    maximumOffset(0),
    widget(0),
    kinetic(0),
    isDragging(false),
    mouseDownPos(-1),
    moveConstant(15),
    clickConstant(25),
    q(s)
{
}

int ScrollAreaPrivate::smoothPos(int y)
{
    if (abs(mouseDownPos - y) <= moveConstant)
        return y;
    else if (mouseDownPos - y < 0)
        return y - moveConstant;
    else
        return y + moveConstant;
}

bool ScrollAreaPrivate::isClickPossible(int y)
{
    if (isDragging || mouseDownPos < 0)
        return false;
    else
        return abs(y - mouseDownPos) <= clickConstant;
}

void ScrollAreaPrivate::updateMaximumOffset()
{
    const int value = (!widget) ? 0 :
                      qMax<int>(0, widget->size().height() - q->size().height());
    if (value != maximumOffset)
    {
        maximumOffset = value;
        emit q->maximumOffsetChanged();
    }

    if(q->m_scrollbar)
        q->m_scrollbar->setMaximum(value);
}

void ScrollAreaPrivate::reconfigure()
{
    if (widget)
    {
        widget->resize(q->size().width(), widget->size().height());
        updateMaximumOffset();
        q->setOffset(offset);
    }
}

void ScrollAreaPrivate::sendClick(int x, int y)
{
    if (!q->scene())
        return;

    QGraphicsSceneMouseEvent *event;

    event = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    event->setButton(Qt::LeftButton);
    event->setScenePos(QPointF(x, y));
    ignoreList << event;
    QCoreApplication::postEvent(q->scene(), event);

    event = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease);
    event->setButton(Qt::LeftButton);
    event->setScenePos(QPointF(x, y));
    ignoreList << event;
    QCoreApplication::postEvent(q->scene(), event);
}

//-----------------------------------------------------------------------------------------------------------------------
//ScrollArea
//-----------------------------------------------------------------------------------------------------------------------
ScrollArea::ScrollArea(QGraphicsItem *parent)
    : QGraphicsWidget(parent),
    d(new ScrollAreaPrivate(this)),
    m_scrollbar(0)
{
    m_parent = parent;
    setFlag(QGraphicsItem::ItemHasNoContents);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape);//设置子窗口不超过父的区域

    d->kinetic = new KineticScroll(this);
    connect(d->kinetic, SIGNAL(signalMoveOffset(int)), SLOT(kineticMove(int)));
}

ScrollArea::~ScrollArea()
{
    delete d;
}

QGraphicsWidget *ScrollArea::widget() const
{
    return d->widget;
}

void ScrollArea::setWidget(QGraphicsWidget *widget)
{
    if (d->widget)
    {
        d->widget->setParentItem(0);
        d->widget->removeEventFilter(this);
        d->widget = 0;
    }

    if (widget)
    {
        d->widget = widget;
        d->widget->setParentItem(this);
        d->widget->installEventFilter(this);
        d->widget->setPos(0, 0);
        d->widget->setFlag(QGraphicsItem::ItemStacksBehindParent);

        d->reconfigure();
    }
}

int ScrollArea::offset() const
{
    return d->offset;
}

void ScrollArea::setOffset(int offset)
{
    if (d->widget)
    {
        const int value = qBound<int>(0, offset, d->maximumOffset);
        if(m_scrollbar)
        {
            m_scrollbar->setValue(value);
        }

        if (value != d->offset) {
            d->offset = value;
            d->widget->setY(-value);
            offsetChanged();
        }
    }
}



int ScrollArea::maximumOffset() const
{
    return d->maximumOffset;
}

void ScrollArea::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QGraphicsWidget::resizeEvent(event);
    d->reconfigure();
}

bool ScrollArea::eventFilter(QObject *object, QEvent *event)
{
    if (object == d->widget && event->type() == QEvent::GraphicsSceneResize)
        d->reconfigure();

    return false;
}

void ScrollArea::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if (d->ignoreList.contains(e))
    {
        d->ignoreList.removeOne(e);
        e->ignore();
        return;
    }

    int y = e->pos().y();
    d->mouseDownPos = y;
    d->isDragging = !d->kinetic->mouseDown(y);
}

void ScrollArea::stopKinetic()
{
    d->kinetic->kineticStop();
}

void ScrollArea::setScrollBar(QPixmap &background, QPixmap &knob_bar,int left_x,int right_x, int factor_y)
{
    if(NULL == m_scrollbar)
        m_scrollbar = new ScrollBar(background,knob_bar,this);
    m_scrollbar->setZValue(1);
    m_scrollbar->setMargins(left_x,right_x,factor_y);
    m_scrollbar->setMaximum(maximumOffset());
}

void ScrollArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    d->widget->setFlags( d->widget->flags()|QGraphicsItem::ItemStacksBehindParent );
    if (d->ignoreList.contains(e))
    {
        d->ignoreList.removeOne(e);
        e->ignore();
        return;
    }

    if (d->mouseDownPos >= 0)
    {
        int y = e->pos().y();
        if (d->isClickPossible(y))
        {
            d->sendClick(e->scenePos().x(), e->scenePos().y());
            d->kinetic->mouseCancel();
        }
        else
        {
            d->kinetic->mouseUp(d->smoothPos(y));
        }
    }

    d->mouseDownPos = -1;
}

void ScrollArea::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (d->mouseDownPos >= 0)
    {
        int y = e->pos().y();
        if (!d->isClickPossible(y))
            d->isDragging = true;

        if (abs(d->mouseDownPos - y) > d->moveConstant)
            d->kinetic->mouseMove(d->smoothPos(y));
    }
}

bool ScrollArea::kineticMove(int value)
{
    int finalOffset = offset() - value;

    setOffset(finalOffset);

    if (value == 0 || finalOffset != offset())
    {
        d->kinetic->kineticStop();
        return false;
    }

    return true;
}
