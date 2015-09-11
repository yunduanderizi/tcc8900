#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QGraphicsWidget>
#include"scrollbar.h"


namespace chinatsp_common_gui_ui
{

    class KineticScroll;
    class ScrollAreaPrivate;
    class ScrollBar;
    class ScrollArea;
    class ScrollAreaPrivate
    {
    public:
        ScrollAreaPrivate(ScrollArea *s);

        int offset;
        int maximumOffset;
        QGraphicsWidget *widget;
        KineticScroll *kinetic;

        bool isDragging;
        int mouseDownPos;
        int moveConstant;
        int clickConstant;
        ScrollArea *q;

        QList<QGraphicsSceneMouseEvent *> ignoreList;

        void reconfigure();
        int smoothPos(int y);
        bool isClickPossible(int y);
        void sendClick(int x, int y);
        void updateMaximumOffset();
    };

    class ScrollArea : public QGraphicsWidget
    {
        Q_OBJECT
    public:
        ScrollArea(QGraphicsItem *parent = 0);
        ~ScrollArea();

        QGraphicsWidget *widget() const;
        void setWidget(QGraphicsWidget *widget);

        int offset() const;
        void setOffset(int offset);

        int maximumOffset() const;

        void stopKinetic();
        void setScrollBar(QPixmap &background,QPixmap &knob_bar,int left_x=5,int right_x=5,int factor_y=0);

    signals:
        void offsetChanged();
        void maximumOffsetChanged();

    public slots:
        bool kineticMove(int value);

    protected:
        bool eventFilter(QObject *watched, QEvent *event);
        void resizeEvent(QGraphicsSceneResizeEvent *event);

        void mousePressEvent(QGraphicsSceneMouseEvent *e);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *e);

    private:
        ScrollAreaPrivate *d;
        ScrollBar * m_scrollbar;
        friend class ScrollAreaPrivate;
        QGraphicsItem *m_parent;
    };
}
#endif
