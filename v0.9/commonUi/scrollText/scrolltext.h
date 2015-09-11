#ifndef SCROLLTEXT_H
#define SCROLLTEXT_H
#include<QGraphicsWidget>
#include"scrollarea.h"

namespace chinatsp_common_gui_ui
{
    class ScrollTextC_Data;
    class ScrollTextC: public QGraphicsWidget
    {
        Q_OBJECT
    public:
        ScrollTextC(QGraphicsItem *parent = 0);
        void  setText(const QString inputText);
        QString text();
        void setColor(const QColor  fontColorText);
        QColor color();

        void setRectGeometry(const QRectF geo);
        void setLeftRightSpace(int left , int right);
        void setTextScrollBar(QPixmap &m_background,QPixmap &m_knob_bar,int x,int y);
//    public slots:
//        void slotOnGerometryChanged();  由于交叉编译时QT信号不识别，故不用信号槽实现
    private:
        ScrollTextC_Data * m_content;

    };



}
#endif // SCROLLTEXT_H
