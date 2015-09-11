#include <QApplication>
#include <QKeyEvent>
#include "characterwidget.h"
/*
 static const char characterKeyText[]={
     ','     ,    '.'       ,    '\''    ,    '\"'    ,   ':'   ,
     ';'     ,     '('      ,    ')'     ,    '@'   ,   '*'   ,
     '#'    ,     '/'     ,    '\\'   ,     '{'     ,  '}'    ,
     '-'    ,      '_'     ,   '+'    ,     '='    ,  '|'    ,
     '<'   ,      '>'    ,  '&'    ,  '!'  , '$'  ,  '%'  ,
     '^'   ,    '?'  ,  '~'  ,  '['     ,  ']'  ,  'a'   ,  '()'  ,
     '[]'   ,     '{}' , 'b'
 };
*/

#define Width 			62
#define keyheight 		62
#define FX 				99
#define FY				8
#define Hsbacing 		8
#define Vinterval		8
#define keyNumber		57

const SymbolButMember symbolParameter[keyNumber]=    
{
	{FX,					FY,	Width,keyheight,(char *)("“")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+Width+Hsbacing,		FY,	Width,keyheight,(char *)("”")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*2,	FY,	Width,keyheight,(char *)("、")/*,QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*3,	FY,	Width,keyheight,(char *)(";")/*,QPixmap(""),QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*4,	FY,	Width,keyheight,(char *)(":")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*5,	FY,	Width,keyheight,(char *)("？")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*6,	FY,	Width,keyheight,(char *)("!")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*7,	FY,	Width,keyheight,(char *)("。")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*8,	FY,	Width,keyheight,(char *)("，")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*9,	FY,	Width,keyheight,(char *)(".")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{FX+35,						FY+keyheight+Vinterval,	Width,keyheight,(char *)("《")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+Width+Hsbacing,		FY+keyheight+Vinterval,	Width,keyheight,(char *)("》")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*2,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("\\")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*3,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("(")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*4,	FY+keyheight+Vinterval,	Width,keyheight,(char *)(")")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*5,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("…")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*6,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("/")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*7,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("~")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*8,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("@")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{FX+35+(Width+Hsbacing)*1,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("_")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*2,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("√")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*3,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("^")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*4,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("<")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*5,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)(">")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*6,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("【")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*7,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("】")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{FX,					FY,	Width,keyheight,(char *)("&")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+Width+Hsbacing,		FY,	Width,keyheight,(char *)("#")/*,QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*2,	FY,	Width,keyheight,(char *)("*")/*,QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*3,	FY,	Width,keyheight,(char *)("%")/*,QPixmap(""),QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*4,	FY,	Width,keyheight,(char *)("\\")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*5,	FY,	Width,keyheight,(char *)("+")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*6,	FY,	Width,keyheight,(char *)("-")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*7,	FY,	Width,keyheight,(char *)("×")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*8,	FY,	Width,keyheight,(char *)("÷")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+(Width+Hsbacing)*9,	FY,	Width,keyheight,(char *)("=")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{FX+35,					FY+keyheight+Vinterval,	Width,keyheight,("$")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+Width+Hsbacing,		FY+keyheight+Vinterval,	Width,keyheight,(char *)("¥")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*2,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("※")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*3,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("「")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*4,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("」")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*5,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("[")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*6,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("]")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*7,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("{")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*8,	FY+keyheight+Vinterval,	Width,keyheight,(char *)("}")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{FX+35+(Width+Hsbacing)*1,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("々")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*2,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("∮")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*3,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("±")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*4,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("№")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*5,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("∠")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*6,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("℃")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{FX+35+(Width+Hsbacing)*7,	FY+(keyheight+Vinterval)*2,Width,keyheight,(char *)("⊙")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{9,		148,	90,62,(char *)("change")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{694,	148,	97,62,(char *)("del")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{107,	148,	89,62,(char *)("space")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{9,		8,		82,62,(char *)("hide")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{44,	78,		82,62,(char *)("next")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
};
static QLabel *BottonPressLarge1=NULL;

CharacterWidget::CharacterWidget():m_bSecond(false),lastKeyId(65535)
{
	QPalette palette;
	if(m_bSecond)
	{
    	palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/charactersubBackground.png")));
	}
	else
	{
    	palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/characterBackground.png")));
	}

	this->setPalette (palette);

	QPixmap tmp_pixmap = QPixmap(":/images/characterBackground.png");
	this->setGeometry(0,262, tmp_pixmap.width(), tmp_pixmap.height());
	this->setFocusPolicy(Qt::ClickFocus);	
	
    BottonPress = new QLabel(this);
    //BtttonPressLarge = new QLabel(0);
	BottonPress->hide();
	//BttonPressLarge->hide();
	//BottonPressLarge->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);

	BottonPressLarge=BottonPressLarge1;
	
	PressLargeLabelText= new QLabel(BottonPressLarge);
 	PressLargeLabelText->hide();
	PressLargeLabelText->setAlignment(Qt::AlignHCenter);	

	DelTimeOutTime = 1;
	m_DelTimer = new QTimer(this);

	connect(m_DelTimer, SIGNAL(timeout()), this, SLOT(DelTimer()));
}

void CharacterWidget::mousePressEvent( QMouseEvent * event )
{
	int i = 0;

	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > symbolParameter[i].x) && (event->x() < symbolParameter[i].x + symbolParameter[i].width) 
			&& (event->y() > symbolParameter[i].y) && (event->y() < symbolParameter[i].y + symbolParameter[i].Vheigth))
		{
			if(lastKeyId != i)
			{
				lastKeyId = i;
				QString tmp;
				QTextCodec *codec = QTextCodec::codecForName("UTF-8");
				if(!m_bSecond)
				{
					tmp = codec->toUnicode(symbolParameter[i].text);
				}
				else
				{
					tmp = codec->toUnicode(symbolParameter[i+26].text);
				}

				showPress(tmp,symbolParameter[i].x,symbolParameter[i].y,symbolParameter[i].width,symbolParameter[i].Vheigth,i);
				if(i<26)
				{
					showPressLarge(tmp,this->geometry().x()+symbolParameter[i].x,this->geometry().y()+symbolParameter[i].y - 100,94,109);
				}

				if(symbolParameter[i].text == tr("del"))
				{
					m_DelTimer->start(600);
				}
			}
			break;
		}  
	}
}
void CharacterWidget::mouseMoveEvent( QMouseEvent * event )
{

	int i = 0;

	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > symbolParameter[i].x) && (event->x() < symbolParameter[i].x + symbolParameter[i].width) 
			&& (event->y() > symbolParameter[i].y) && (event->y() < symbolParameter[i].y + symbolParameter[i].Vheigth))
		{
			if(lastKeyId != i)
			{
				lastKeyId = i;
				QString tmp;
				QTextCodec *codec = QTextCodec::codecForName("UTF-8");
				if(!m_bSecond)
				{
					tmp = codec->toUnicode(symbolParameter[i].text);
				}
				else
				{
					tmp = codec->toUnicode(symbolParameter[i+26].text);
				}
				
				showPress(tmp,symbolParameter[i].x,symbolParameter[i].y,symbolParameter[i].width,symbolParameter[i].Vheigth,i);
				if(i<26)
				{
					showPressLarge(tmp,this->geometry().x()+symbolParameter[i].x,this->geometry().y()+symbolParameter[i].y - 100,94,109);
				}

				if(symbolParameter[i].text == tr("del"))
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

	if(i >= keyNumber)
	{
		lastKeyId = 65535;
		m_DelTimer->stop();
		DelTimeOutTime = 1;
		
		BottonPress->hide();
		BottonPressLarge->hide();
		PressLargeLabelText->hide();
	}
}
void CharacterWidget::mouseReleaseEvent( QMouseEvent * event )
{
	int i = 0;
	

	BottonPress->hide();
	BottonPressLarge->hide();
	PressLargeLabelText->hide();
	
	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > symbolParameter[i].x) && (event->x() < symbolParameter[i].x + symbolParameter[i].width) 
			&& (event->y() > symbolParameter[i].y) && (event->y() < symbolParameter[i].y + symbolParameter[i].Vheigth))
		{
			break;
		}
	}
	if((i >= 0) && (i < keyNumber))
	{
		if(symbolParameter[i].text == tr("next"))
		{
			on_second_clicked();
		}
		else if(symbolParameter[i].text == tr("space"))
		{
			YTInputServerC::instance()->sendCommitText(QString(" "));		
		}
		else if(symbolParameter[i].text == tr("del"))
		{
			m_DelTimer->stop();
			DelTimeOutTime = 1;
			YTInputServerC::instance()->sendOneKey(Qt::Key_Backspace,0); 
 		}
		else if(symbolParameter[i].text == tr("change"))
		{
			YTInputServerC::instance()->showInputMethodSelect();
		}
		else if(symbolParameter[i].text == tr("hide"))
		{
			emit hideInputMethod();
		}
		else
		{
			if(!m_bSecond)
			{
				QString tmp;
    			QTextCodec *codec = QTextCodec::codecForName("UTF-8");
				tmp = codec->toUnicode(symbolParameter[i].text);

				YTInputServerC::instance()->sendCommitText(tmp);
			}
			else
			{
				QString tmp;
    			QTextCodec *codec = QTextCodec::codecForName("UTF-8");
				tmp = codec->toUnicode(symbolParameter[i+26].text);
				YTInputServerC::instance()->sendCommitText(tmp);
			}

		}
		
	}
	lastKeyId = 65535;
	update();
}

void CharacterWidget::showPress(QString text,int x,int y,int width,int height,int ID)
{
	QPixmap tmppix;
	
	if(symbolParameter[ID].text == tr("space"))
	{
		tmppix = QPixmap(":/images/pinyinspacepre.png");
	}
	else if(symbolParameter[ID].text == tr("del"))
	{
		tmppix = QPixmap(":/images/pinyindelpre.png");
	}
	else if(symbolParameter[ID].text == tr("change"))
	{
		tmppix = QPixmap(":/images/pinyinchangepre.png");
	}
	else if(symbolParameter[ID].text == tr("hide"))
	{
		tmppix = QPixmap(":/images/pinyinhidepre.png");
	}
	else if(symbolParameter[ID].text == tr("next"))
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

void CharacterWidget::showPressLarge(QString text,int x,int y,int width,int height)
{
    QFont font = QFont(QString::fromUtf8("fzhei"));
	
    font.setPixelSize(35);

	BottonPressLarge->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
    BottonPressLarge->setStyleSheet("QLabel {background:rgba(0,0,0,0%);border-style:outset;font: bold 30px}");
    PressLargeLabelText->setStyleSheet("QLabel{background:rgba(0,0,0,0%);border-style:outset;font: bold 50px}");

	PressLargeLabelText->setGeometry(0,0,width,height);
	PressLargeLabelText->setText(text);
 	PressLargeLabelText->show();

	BottonPressLarge->setPixmap(QPixmap(":/images/pinyinpressLargePix.png"));
	BottonPressLarge->setGeometry(x,y,width,height);
	BottonPressLarge->show();	

}

void CharacterWidget::on_second_clicked()
{
	m_bSecond = !m_bSecond;
	QPalette palette;

	if(!m_bSecond)
	{
	    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/characterBackground.png")));
		this->setPalette (palette);

	}
	else
	{
	    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/charactersubBackground.png")));
		this->setPalette (palette);
	
	}

}

void CharacterWidget::setContextWords(QString textBeforeCursor,QStringList contextWords)
{

}
void CharacterWidget::reset()
{
	BottonPress->hide();
 	PressLargeLabelText->hide();
	BottonPressLarge->hide();
}
void CharacterWidget::DelTimer()
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

void CharacterWidget::NewLargeLable()
{
    BottonPressLarge1 = new QLabel(0,Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
	BottonPressLarge1->setGeometry(-800,-800,10,10);
	BottonPressLarge1->show();
	BottonPressLarge1->hide();
	//BottonPressLarge->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
}

