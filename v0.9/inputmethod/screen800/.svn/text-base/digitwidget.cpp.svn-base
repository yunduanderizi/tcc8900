#include "digitwidget.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
 
#define Width 			132
#define keyheight 		62
#define FX 				99
#define FY				8
#define Hsbacing 		8
#define Vinterval		8
#define keyNumber		18

const DigitalButMember DigitalParameter[keyNumber]=
{
	{FX,					FY,	Width,keyheight,QString("1")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+Width+Hsbacing,		FY,	Width,keyheight,QString("2")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*2,	FY,	Width,keyheight,QString("3")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*3,	FY,	Width,keyheight,QString("4")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*4,	FY,	Width,keyheight,QString("5")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	
	{FX,					FY+keyheight+Vinterval,	Width,keyheight,QString("6")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*1,	FY+keyheight+Vinterval,	Width,keyheight,QString("7")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*2,	FY+keyheight+Vinterval,	Width,keyheight,QString("8")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*3,	FY+keyheight+Vinterval,	Width,keyheight,QString("9")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*4,	FY+keyheight+Vinterval,	Width,keyheight,QString("0")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{FX+(Width+Hsbacing)*1,	FY+(keyheight+Vinterval)*2,	Width,keyheight,QString("#")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*2,	FY+(keyheight+Vinterval)*2,	Width,keyheight,QString("*")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*3,	FY+(keyheight+Vinterval)*2,	Width,keyheight,QString(".")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*4,	FY+(keyheight+Vinterval)*2,	Width,keyheight,QString(",")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	
	{9,		148,	105,62,QString("change")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{9,		78,		82,	62,QString("del")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{122,	148,	109,62,QString("space")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{9,		8,		82,	62,QString("hide")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
};

DigitWidget::DigitWidget()
{
	QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/digitalbackground.png")));

	this->setPalette (palette);

	QPixmap tmp_pixmap = QPixmap(":/images/digitalbackground.png");
	this->setGeometry(0, 262, tmp_pixmap.width(), tmp_pixmap.height());
	this->setFocusPolicy(Qt::ClickFocus);	
	
    BottonPress = new QLabel(this);
	BottonPress->hide();

	DelTimeOutTime = 1;
	m_DelTimer = new QTimer(this);

	connect(m_DelTimer, SIGNAL(timeout()), this, SLOT(DelTimer()));
}         


void DigitWidget::mousePressEvent( QMouseEvent * event )
{
	int i = 0;

	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > DigitalParameter[i].x) && (event->x() < DigitalParameter[i].x + DigitalParameter[i].width) 
			&& (event->y() > DigitalParameter[i].y) && (event->y() < DigitalParameter[i].y + DigitalParameter[i].Vheigth))
		{
			showPress(DigitalParameter[i].text,DigitalParameter[i].x,DigitalParameter[i].y,DigitalParameter[i].width,DigitalParameter[i].Vheigth);

			if(DigitalParameter[i].text == tr("del"))
			{
				m_DelTimer->start(600);
			}
			break;
		}  
	}
}
void DigitWidget::mouseMoveEvent( QMouseEvent * event )
{
	int i = 0;

	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > DigitalParameter[i].x) && (event->x() < DigitalParameter[i].x + DigitalParameter[i].width) 
			&& (event->y() > DigitalParameter[i].y) && (event->y() < DigitalParameter[i].y + DigitalParameter[i].Vheigth))
		{
			showPress(DigitalParameter[i].text,DigitalParameter[i].x,DigitalParameter[i].y,DigitalParameter[i].width,DigitalParameter[i].Vheigth);

			if(DigitalParameter[i].text == tr("del"))
			{
				m_DelTimer->start(600);
			}
			else
			{
				m_DelTimer->stop();
				DelTimeOutTime = 1;
			}
			break;
		}
		else
		{
			m_DelTimer->stop();
			DelTimeOutTime = 1;
			BottonPress->hide();
		}
	}
}
void DigitWidget::mouseReleaseEvent( QMouseEvent * event )
{
	int i = 0;

	BottonPress->hide();
	
	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > DigitalParameter[i].x) && (event->x() < DigitalParameter[i].x + DigitalParameter[i].width) 
			&& (event->y() > DigitalParameter[i].y) && (event->y() < DigitalParameter[i].y + DigitalParameter[i].Vheigth))
		{
			break;
		}
	}
	if((i >= 0) && (i < keyNumber))
	{
		if(DigitalParameter[i].text == tr("space"))
		{
			YTInputServerC::instance()->sendCommitText(QString(" "));
		}
		else if(DigitalParameter[i].text == tr("del"))
		{
			m_DelTimer->stop();
			DelTimeOutTime = 1;
			YTInputServerC::instance()->sendOneKey(Qt::Key_Backspace,0);
 		}
		else if(DigitalParameter[i].text == tr("change"))
		{
			YTInputServerC::instance()->showInputMethodSelect();
		}
		else if(DigitalParameter[i].text == tr("hide"))
		{
			emit hideInputMethod();
		}
		else
		{
			YTInputServerC::instance()->sendCommitText(DigitalParameter[i].text);
		}
	}
	
	update();
}

void DigitWidget::showPress(QString text,int x,int y,int width,int height)
{
	BottonPress->setPixmap(QPixmap(":/images/digitalpressPix.png"));
	BottonPress->setGeometry(x,y,width,height);
	
	BottonPress->show();	
}

void DigitWidget::setContextWords(QString textBeforeCursor,QStringList contextWords)
{

}
void DigitWidget::reset()
{

}

void DigitWidget::DelTimer()
{
	YTInputServerC::instance()->sendOneKey(Qt::Key_Backspace,0);

	DelTimeOutTime++;

	if(DelTimeOutTime == 2)
	{
		m_DelTimer->setInterval(300);
	}
	else if(DelTimeOutTime > 5)
	{
		m_DelTimer->setInterval(100);
	}
}

