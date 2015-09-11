#ifndef SCROLLTEXTCDATA_H
#define SCROLLTEXTCDATA_H
#include<scrolltext.h>

namespace chinatsp_common_gui_ui
{
    class ScrollTextC_Data:public QGraphicsWidget
    {

    public:
        ScrollTextC_Data(QGraphicsItem *parent);
        void layoutText(void);
    protected:
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget = 0 );
        QStringList layoutString(QString sLine,int maxwitdh);
        int getLeftSpace(); //获得控件显示内容左边预留的空间大小
        int getRightSpace();//获得控件显示内容右边预留的空间大小


    private:
        QStringList     m_layout_text_list;
        QList<QRectF>   m_layout_rect_list;
        QString                     m_text;
        QColor                     m_color;
        ScrollArea           *m_scrollArea;
        ScrollTextC              *l_parent;

        int m_left;
        int m_right;
        friend class ScrollTextC;
    };

}
#endif // SCROLLTEXTCDATA_H
