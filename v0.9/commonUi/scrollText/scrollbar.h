/****************************************************************************
* author :hufengkai@21com.com
* date:  2010-10-27
*
****************************************************************************/

#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QGraphicsPixmapItem>

namespace chinatsp_common_gui_ui
{
    class ScrollBar: public QObject, public QGraphicsPixmapItem
    {
    public:
        ScrollBar( QPixmap &background,QPixmap &knob_bar,QGraphicsItem *parent = 0);
        int value() const;
        void setValue(int value);
        int maximum() const;
        void setMaximum(int maximum);
        void setScrollHeight(int height);
        void setMargins(int left_x,int right_x,int factor_y);
        void setMargins(int left_x,int right_x);
        void setScrollPixmap(QPixmap &m_background,QPixmap &m_knob_bar);

    private:
        int m_maximum;
        int m_value;
        int m_height;
        int m_margins_l;
        int m_margins_r;
        int m_factor_x;
        int m_factor_y;
        QGraphicsPixmapItem m_knob;
        QSize m_size;
        int type;
    };
}
#endif
