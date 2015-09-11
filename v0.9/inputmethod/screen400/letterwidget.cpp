#include <QApplication>
#include <QKeyEvent>
#include "letterwidget.h"


#define Width 			41
#define keyheight 		42
#define FX 				5
#define FY				5
#define Hsbacing 		1
#define Vinterval		3
#define keyEngNumber		31

const EnglishButMember EnglishParameter[keyEngNumber]=
{
	{FX,					FY,	Width,keyheight,QString("q")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+Width+Hsbacing,		FY,	Width,keyheight,QString("w")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*2,	FY,	Width,keyheight,QString("e")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*3,	FY,	Width,keyheight,QString("r")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*4,	FY,	Width,keyheight,QString("t")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*5,	FY,	Width,keyheight,QString("y")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*6,	FY,	Width,keyheight,QString("u")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*7,	FY,	Width,keyheight,QString("i")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*8,	FY,	Width,keyheight,QString("o")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*9,	FY,	Width,keyheight,QString("p")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{FX +21,					FY+keyheight+Vinterval,	Width,keyheight,QString("a")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+Width+Hsbacing,		FY+keyheight+Vinterval,	Width,keyheight,QString("s")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*2,	FY+keyheight+Vinterval,	Width,keyheight,QString("d")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*3,	FY+keyheight+Vinterval,	Width,keyheight,QString("f")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*4,	FY+keyheight+Vinterval,	Width,keyheight,QString("g")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*5,	FY+keyheight+Vinterval,	Width,keyheight,QString("h")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*6,	FY+keyheight+Vinterval,	Width,keyheight,QString("j")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*7,	FY+keyheight+Vinterval,	Width,keyheight,QString("k")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*8,	FY+keyheight+Vinterval,	Width,keyheight,QString("l")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{FX+21+Width+Hsbacing,		FY+(keyheight+Vinterval)*2,Width,keyheight,QString("z")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*2,	FY+(keyheight+Vinterval)*2,Width,keyheight,QString("x")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*3,	FY+(keyheight+Vinterval)*2,Width,keyheight,QString("c")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*4,	FY+(keyheight+Vinterval)*2,Width,keyheight,QString("v")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*5,	FY+(keyheight+Vinterval)*2,Width,keyheight,QString("b")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*6,	FY+(keyheight+Vinterval)*2,Width,keyheight,QString("n")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+21+(Width+Hsbacing)*7,	FY+(keyheight+Vinterval)*2,Width,keyheight,QString("m")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{415,	95,		60,42,QString("change")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{5,		95,		62,42,QString("del")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{362,	95,		52,42,QString("space")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{425,	5,		50,42,QString("hide")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{404,	50,		50,42,QString("abc")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
};
static QLabel *BottonPressLarge1=NULL;
LetterWidget::LetterWidget():m_bCapital(false),lastKeyId(65535)
{
	QPalette palette;
	if(!m_bCapital)
	{
    	palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/englishLowerbackgroud.png")));
	}
	else
	{
    	palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/englishCapitalbackgroud.png")));
	}

	this->setPalette (palette);

	QPixmap tmp_pixmap = QPixmap(":/images/englishLowerbackgroud.png");
	this->setGeometry(0, 125, tmp_pixmap.width(), tmp_pixmap.height());
	this->setFocusPolicy(Qt::ClickFocus);	
	
    BottonPress = new QLabel(this);
    //BottonPressLarge = new QLabel(0,Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
	BottonPress->hide();
	//BottonPressLarge->hide();
	//BottonPressLarge->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
	BottonPressLarge=BottonPressLarge1;
	
	PressLargeLabelText= new QLabel(BottonPressLarge);
 	PressLargeLabelText->hide();
	PressLargeLabelText->setAlignment(Qt::AlignHCenter);	

	DelTimeOutTime = 1;
	m_DelTimer = new QTimer(this);

	connect(m_DelTimer, SIGNAL(timeout()), this, SLOT(DelTimer()));
}

void LetterWidget::on_abc_clicked()
{
	m_bCapital = !m_bCapital;
	QPalette palette;
	
	if(!m_bCapital)
	{
	    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/englishLowerbackgroud.png")));
		this->setPalette (palette);

	}
	else
	{
	    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/englishCapitalbackgroud.png")));
		this->setPalette (palette);
	
	}

}

void LetterWidget::mousePressEvent( QMouseEvent * event )
{
	int i = 0;

	for(i=0;i<keyEngNumber;i++)
	{
		if((event->x() > EnglishParameter[i].x) && (event->x() < EnglishParameter[i].x + EnglishParameter[i].width) 
			&& (event->y() > EnglishParameter[i].y) && (event->y() < EnglishParameter[i].y + EnglishParameter[i].Vheigth))
		{
			if(lastKeyId != i)
			{
				lastKeyId = i;
				showPress(EnglishParameter[i].text,EnglishParameter[i].x,EnglishParameter[i].y,EnglishParameter[i].width,EnglishParameter[i].Vheigth,i);
			
				if(i<26)
				{
					showPressLarge(EnglishParameter[i].text,this->geometry().x()+EnglishParameter[i].x,this->geometry().y()+EnglishParameter[i].y - 70,56,68);
				}

				if(EnglishParameter[i].text == tr("del"))
				{
					m_DelTimer->start(600);
				}
			}
			break;
		}  
	}
}
void LetterWidget::mouseMoveEvent( QMouseEvent * event )
{
	int i = 0;

	for(i=0;i<keyEngNumber;i++)
	{
		if((event->x() > EnglishParameter[i].x) && (event->x() < EnglishParameter[i].x + EnglishParameter[i].width) 
			&& (event->y() > EnglishParameter[i].y) && (event->y() < EnglishParameter[i].y + EnglishParameter[i].Vheigth))
		{
			if(lastKeyId != i)
			{
				lastKeyId = i;
				showPress(EnglishParameter[i].text,EnglishParameter[i].x,EnglishParameter[i].y,EnglishParameter[i].width,EnglishParameter[i].Vheigth,i);
				if(i<26)
				{
					showPressLarge(EnglishParameter[i].text,this->geometry().x()+EnglishParameter[i].x,this->geometry().y()+EnglishParameter[i].y - 70,56,68);
				}

				if(EnglishParameter[i].text == tr("del"))
				{
					m_DelTimer->start(600);
				}
				else
				{
					m_DelTimer->stop();
					DelTimeOutTime = 1;
				}
			}
			break;
		}
		else
		{
		}
	}

	if(i >= keyEngNumber)
	{
		lastKeyId = 65535;
		m_DelTimer->stop();
		DelTimeOutTime = 1;
		BottonPress->hide();
		BottonPressLarge->hide();
	}
}
void LetterWidget::mouseReleaseEvent( QMouseEvent * event )
{
	int i = 0;

	BottonPress->hide();
	BottonPressLarge->hide();
	PressLargeLabelText->hide();
	
	for(i=0;i<keyEngNumber;i++)
	{
		if((event->x() > EnglishParameter[i].x) && (event->x() < EnglishParameter[i].x + EnglishParameter[i].width) 
			&& (event->y() > EnglishParameter[i].y) && (event->y() < EnglishParameter[i].y + EnglishParameter[i].Vheigth))
		{
			break;
		}
	}
	if((i >= 0) && (i < keyEngNumber))
	{
		if(EnglishParameter[i].text == tr("space"))
		{
			YTInputServerC::instance()->sendCommitText(QString(" "));
		}
		else if(EnglishParameter[i].text == tr("del"))
		{
			m_DelTimer->stop();
			DelTimeOutTime = 1;
			YTInputServerC::instance()->sendOneKey(Qt::Key_Backspace,0);
 		}
		else if(EnglishParameter[i].text == tr("change"))
		{
			YTInputServerC::instance()->showInputMethodSelect();
		}
		else if(EnglishParameter[i].text == tr("hide"))
		{
			emit hideInputMethod();
		}
		else if(EnglishParameter[i].text == tr("abc"))
		{
			on_abc_clicked();
		}
		else
		{
			if(m_bCapital)
			{			
				YTInputServerC::instance()->sendCommitText(EnglishParameter[i].text.toUpper());
			}
			else
			{
				YTInputServerC::instance()->sendCommitText(EnglishParameter[i].text);
			}
		}
	}	
	lastKeyId = 65535;
	update();
}

void LetterWidget::showPress(QString text,int x,int y,int width,int height,int ID)
{
	QPixmap tmppix;
	
	if(EnglishParameter[ID].text == tr("space"))
	{
		tmppix = QPixmap(":/images/pinyinspacepre.png");
	}
	else if(EnglishParameter[ID].text == tr("del"))
	{
		tmppix = QPixmap(":/images/pinyindelpre.png");
	}
	else if(EnglishParameter[ID].text == tr("change"))
	{
		tmppix = QPixmap(":/images/pinyinchangepre.png");
	}
	else if(EnglishParameter[ID].text == tr("hide"))
	{
		tmppix = QPixmap(":/images/pinyinhidepre.png");
	}
	else if(EnglishParameter[ID].text == tr("abc"))
	{
		tmppix = QPixmap(":/images/pinyinhidepre.png");
	}
	else
	{
		tmppix = QPixmap(":/images/pinyinpressPix.png");
	}

	BottonPress->setPixmap(tmppix);
	BottonPress->setGeometry(x,y,width,height);
	
	BottonPress->show();	
}

void LetterWidget::showPressLarge(QString text,int x,int y,int width,int height)
{

    BottonPressLarge->setStyleSheet("QLabel {background:rgba(0,0,0,0%);border-style:outset;font: bold 30px}");
    PressLargeLabelText->setStyleSheet("QLabel{background:rgba(0,0,0,0%);border-style:outset;font: bold 30px}");

	PressLargeLabelText->setGeometry(0,0,width,height);
	if(m_bCapital)
	{
		PressLargeLabelText->setText(text.toUpper());
	}
	else
	{
		PressLargeLabelText->setText(text);
	}
 	PressLargeLabelText->show();

	BottonPressLarge->setPixmap(QPixmap(":/images/pinyinpressLargePix.png"));
	BottonPressLarge->setGeometry(x,y,width,height);
	BottonPressLarge->show();	

}
void LetterWidget::setContextWords(QString textBeforeCursor,QStringList contextWords)
{

}
void LetterWidget::reset()
{
	m_bCapital = false;
	QPalette palette;
	
	if(!m_bCapital)
	{
	    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/englishLowerbackgroud.png")));
		this->setPalette (palette);
	}
	else
	{
	    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/englishCapitalbackgroud.png")));
		this->setPalette (palette);
	}
	BottonPress->hide();
 	PressLargeLabelText->hide();
	BottonPressLarge->hide();
}
void LetterWidget::DelTimer()
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

void LetterWidget::NewLargeLable()
{
    BottonPressLarge1 = new QLabel(0,Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
	BottonPressLarge1->setGeometry(-800,-800,10,10);
	BottonPressLarge1->show();
	BottonPressLarge1->hide();
	//BottonPressLarge->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
}
