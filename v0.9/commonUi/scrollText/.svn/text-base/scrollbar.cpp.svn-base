/****************************************************************************
* author :hufengkai@21com.com
* date:  2010-10-27
*
****************************************************************************/
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "scrollbar.h"

using namespace chinatsp_common_gui_ui;

ScrollBar::ScrollBar(QPixmap &background,QPixmap &knob_bar,QGraphicsItem* parent): QObject(),
QGraphicsPixmapItem(parent),
m_maximum(0),
m_value(0),
m_margins_l(0),
m_margins_r(0),
m_factor_y(0),
m_knob(this),
type(0)
{
    setScrollPixmap(background, knob_bar);
}


void ScrollBar::setMargins(int left_x,int right_x,int factor_y)
{
    m_margins_l   =  left_x;
    m_margins_r  =  right_x;
    m_factor_y     = factor_y;
}

void ScrollBar::setMargins(int left_x,int right_x)
{
    m_margins_l = left_x;
    m_margins_r = right_x;
}

int ScrollBar::value()const
{
    return m_value;
}

void ScrollBar::setValue(int value)
{
    int min,max;
    qreal y =0;
    qreal x = 0;

    if (value < 0 || value > m_maximum)
        return;

    m_value = value;
    if(0 == type )
    {
        min = boundingRect().topLeft().y();
        max = boundingRect().bottomLeft().y() - m_knob.boundingRect().height();
        x = (boundingRect().width() - m_knob.boundingRect().width());
        if(m_maximum)
            y = (qreal)(value * (max - min)) /(qreal) m_maximum;
        m_knob.setPos(x, y);
    }
    else if(1==type)
    {
        min = m_factor_y/2;
        max = parentItem()->boundingRect().height()-m_factor_y/2-m_size.height();
        x = parentItem()->boundingRect().width() -m_size.width()-m_margins_l - m_margins_r;
        if(m_maximum)
            y = (qreal)(value * (max - min)) /(qreal) m_maximum;

        setPos(x,y);
    }
}

int ScrollBar::maximum()const
{
    return m_maximum;
}

void  ScrollBar::setMaximum(int maximum)
{
    this->m_maximum = maximum;
    //qWarning("maximum %d\n",maximum);
    if(maximum>0)
    {
        this->show();
        qreal y =0;
        qreal x = 0;
        if(this->parentItem())
        {

            x = parentItem()->boundingRect().width() -m_size.width()-m_margins_l - m_margins_r;
            if(0 == type )
                y = (parentItem()->boundingRect().height()-m_size.height()-m_factor_y)/2;
            else
                y  = m_factor_y/2;
            // y =  m_factor_y;
            setPos(x,y);
        }
        else
        {
            qWarning("no scence \n");

        }
    }
    else
        this->hide();
}


void ScrollBar::setScrollPixmap(QPixmap &background, QPixmap &knob_bar)
{
    if(background.isNull())
    {
        if(!knob_bar.isNull())
        {
            m_size = knob_bar.size();
            setPixmap(knob_bar);
            type = 1;
        }
        else
        {
            qWarning("error ScrollBar::setScrollPixmapbackground-knob_bar------------->>>>>>>> \n");
            type = -1;
        }
    }
    else
    {
        m_size = background.size();
        setPixmap(background);
        m_knob.setPixmap(knob_bar);
        type = 0;
    }
}
