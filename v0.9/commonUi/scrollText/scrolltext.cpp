#include "scrolltext.h"
#include "scrolltextCdata.h"
#include <QPainter>
#include <QPalette>
#include <QString>
#include <QColor>
#include <QDebug>
#include <QFontMetrics>

using namespace chinatsp_common_gui_ui;

ScrollTextC_Data::ScrollTextC_Data(QGraphicsItem *parent):QGraphicsWidget(parent),m_left(0),m_right(0)
{

    l_parent = static_cast<ScrollTextC *>(parent);  //static_cast执行非多态的转换，用于代替C中通常的转换操作。因此，被做为隐式类型转换
    m_scrollArea = new ScrollArea(parent);
    m_scrollArea->setWidget(this);
}

/*!
   获得控件左边预留的的空间大小
*/
int ScrollTextC_Data::getLeftSpace()
{
   return m_left;
}

/*!
   获得控件右边预留的的空间大小
*/
int ScrollTextC_Data::getRightSpace()
{
   return m_right;
}

/*!
  把输入的字符串，按照指定的宽度，，拆分为若干个字符串，
    使每个字符串绘在屏幕上的宽度刚好小于指定的宽度.
    -----这个函数假设输入的字符串是不含换行符----.
   \param QString  sLine 指定的字符串
   \param int maxWidth 指定的显示宽度
   \return QStringList返回已拆分好的字符串列表.
*/
QStringList ScrollTextC_Data::layoutString(QString sLine,int maxwitdh)
{
    QStringList s_result_list;
    int i_average_count =0;
    bool b_up_break = false;
    bool b_down_break = false;
    int i= 0;
    int w =0;
    QString stemp;
    int i_count = sLine.count();
    int i_average;
    s_result_list.clear();

    //计算每个字的平均长度。
    if(i_count)
        i_average = QFontMetrics(l_parent->font()).width(sLine)/i_count;

    else
        return s_result_list;

    //计算一行平均能放几个字,如果平均字数小于1则按1计算。
    //将这个值做为搜索的平均点。这样可以使搜索次数最少
    //从平均点分别做向上和向下的搜索,来查找那些字可以被排成一行。
    if(i_average)
        i_average_count = maxwitdh /i_average;
    else
      i_average =1;

    //从平均点向字符串终点搜索，如果搜索到一个临界点，这个点之前的字符组合宽度刚好大于maxWidth,
    //1.--这个点又不是平均点,说明这个点的下一点之前的字符长度小于maxWidth.
    //----这意味着这个点之前的字符应该被排到一行。这个点之后字符应该被重新排版.
    //2.如果是平均点说明需要向下搜索
    if(i_count> i_average_count)
    {
        i = i_average_count;
        w =0;
        //搜索第一个宽度大于maxWidth的点
        while(i< i_count)
        {
            stemp = sLine.left(i);
            w = QFontMetrics(l_parent->font()).width(stemp);
            if(w>maxwitdh)
            {
                b_up_break =true;//设置临界点已找到
                break;
            }
            i++;
        }

        //根据搜索到的临界点来决定是否要做截断
        if(b_up_break)//找到这个点
        {
            if(i > i_average_count)//这个点不是平均点，说明是临界点,可以认为这个点前面的排成一行
            {
                  s_result_list<< sLine.left(i-1);
                  stemp = sLine.right(sLine.count()-i+1);
                  s_result_list<< layoutString(stemp,maxwitdh);
                  return s_result_list;
            }
            else
            {
                i= i_average_count;//该为平均点，则向字符串起点搜索
            }
        }
        else//未找到这个点说明整个字符串就是在一行。
        {
            s_result_list<<sLine;
            return s_result_list;
        }
    }
    else//总长度小于等于平均点,则向字符串起点搜索
        i = i_count;

    //从平均点向字符串起点算法
    //从平均点向字符串起点搜索，如果找到一个临界点，这个点之前的字符长度刚好等于maxWidth.说明这个点之前的字符应该被排成一行。
    //查找临界点
    while(i>0)
    {
        stemp = sLine.left(i);
        w = QFontMetrics(l_parent->font()).width(stemp);
        if(w<maxwitdh)
        {
            b_down_break =true;//设置临界点已找到
            break;
        }
        i--;
    }

    if(b_down_break)//如果找到这个临界点,则进行划分行处理。
    {
        s_result_list<< sLine.left(i);
        if(i!=sLine.count())
        {
            stemp =sLine.right(sLine.count()-i);
            s_result_list<< layoutString(stemp,maxwitdh);
        }
        return s_result_list;
    }
    else//未找到临界点,说明每个字可以要被排成一行。至少第一个字要被强制排成一行
    {
        s_result_list<<sLine.left(1);
        if(sLine.count()>1)
        {
            stemp = sLine.right(sLine.count()-1);
            s_result_list<< layoutString(stemp,maxwitdh);
        }
        return s_result_list;
    }

    return s_result_list;
}

/*!
     根据内容的大小和长度解析字符串
*/
void ScrollTextC_Data::layoutText(void)
{

    QRectF l_rect;
    int w =geometry().width();
    int left = getLeftSpace();
    int right = getRightSpace();
    int h ;
    h = QFontMetrics(l_parent->font()).height();
    m_layout_rect_list.clear();
    m_layout_text_list.clear();

    QString s_layout_test = m_text;
    if(s_layout_test != NULL)
    {
        QStringList l_split_n;
        l_split_n = s_layout_test.split("\n");
        for(int i = 0; i< l_split_n.count();i++)
        {
            m_layout_text_list<<layoutString(l_split_n.at(i),w-right-left);
        }

        for(int i =0;i<m_layout_text_list.count();i++)
        {
            l_rect.setRect(0+left,h*(i),w-right-left,h*(i+1));
            m_layout_rect_list<<l_rect;
        }

        setGeometry(0,0,w-right-left,h*m_layout_text_list.count());  //全部内容显示的实际宽，高    
    }
    else
    {   
        return ;
    }
}

/*!
    绘制显示的内容，颜色和字体
*/
void ScrollTextC_Data::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget )
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //set font and color
    painter->setFont(l_parent->font());
    painter->setPen(m_color);

    //draw text;
    if(m_layout_text_list.count() == m_layout_rect_list.count())
    {
        for(int i=0;i<m_layout_text_list.count();i++)
        {
            painter->drawText(m_layout_rect_list.at(i),Qt::AlignLeft|Qt::TextSingleLine,m_layout_text_list.at(i));
        }
    }
}

//------------------------------------------------------
//-ScrollTextC
ScrollTextC::ScrollTextC(QGraphicsItem *parent ):QGraphicsWidget(parent)
{
    m_content = new ScrollTextC_Data(this);
}

/*!
    设置显示的内容
*/
void  ScrollTextC::setText(const QString inputText)
{
        m_content->m_text = inputText;
        m_content->layoutText();
        update();
}

/*!
    获得显示的内容
*/
QString ScrollTextC::text()
{
        return m_content->m_text;
}

/*!
    设置显示内容的颜色
*/
void ScrollTextC::setColor(const QColor  fontColorText)
{
    m_content->m_color = fontColorText;
    update();
}

/*!
    获得显示内容的颜色
*/
QColor ScrollTextC::color()
{
    return m_content->m_color;
}

/*!
   设置滚动条
*/
void ScrollTextC::setTextScrollBar(QPixmap &background,QPixmap &knob_bar,int x,int y)
{
    m_content->m_scrollArea->setScrollBar(background,knob_bar,x,y);

}

/*!

*/
//void ScrollTextC::slotOnGerometryChanged()
//{
//    QRectF l_rect =geometry();
//    m_content->m_scrollArea->setGeometry(l_rect);
//    m_content->layoutText();
//}

/*!
   设置显示内容的的范围大小
*/
 void ScrollTextC::setRectGeometry(const QRectF geo)
 {
     m_content->m_scrollArea->setGeometry(geo);
     m_content->layoutText();
 }
/*!
   设置控件左右预留的空间大小，默认情况下是0,0
*/
void ScrollTextC::setLeftRightSpace(int left, int right)
{
    m_content->m_left = left;
    m_content->m_right = right;
    m_content->layoutText();
    update();
}

