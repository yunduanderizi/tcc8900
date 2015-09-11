#include "pinyininputmethod.h"

#define Width 			41
#define keyheight 		42
#define FX 				5
#define FY				47
#define Hsbacing 		1
#define Vinterval		3
#define keyNumber		32

const LetterButMember letterParameter[keyNumber]=
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

	{FX+21,						FY+keyheight+Vinterval,	Width,keyheight,QString("a")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
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
	
	{434,	6,		41,	37,QString("right")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{392,	6,		41,	37,QString("left")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},

	{415,	137,	60,42,QString("change")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{5,		137,	62,42,QString("del")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{362,	137,	52,42,QString("space")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{425,	47,		50,42,QString("hide")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
};


ButtonPinyin::ButtonPinyin(QWidget *parent) : QPushButton(parent)
{
	//normalColor = color;
}
void ButtonPinyin::setPixmap(QString sIconNameBase, QString sIconNamePushed)
{
	//m_StateType		= TypeFocusInOutChange;
	m_sIconBase		= sIconNameBase;
	m_sIconPushed	= sIconNamePushed;

	m_Icon = QPixmap(m_sIconBase);
       
}

void ButtonPinyin::paintEvent(QPaintEvent *)
{
	QPainter p (this);
	QStyleOptionButton option;
	option.initFrom (this);
	//  QRect r = this->style() ->subElementRect (QStyle::SE_PushButtonContents, &option, this);
	QRect r = this->style()->subElementRect (QStyle::SE_PushButtonFocusRect, &option, this);
	//option.rect = r;
	//  option.palette = this->palette();
	//  this->style()->drawControl(QStyle::CE_PushButton, &option,&p,this);
	//  if (this->hasFocus() )
	//  {
	//p.setPen (this->palette().color (QPalette::HighlightedText) );
	//p.setPen (this->palette().color (QPalette::ButtonText) );
	//p.drawText (r, Qt::AlignHCenter, this->text() );
	p.drawPixmap(r,m_Icon);
	//}
	//else
	//{
	p.setPen (this->palette().color (QPalette::ButtonText) );
	p.drawText (r, Qt::AlignHCenter, this->text() );
	//  p.drawPixmap(r,m_Icon);
	//}

}
void ButtonPinyin::resizeEvent ( QResizeEvent * event ) 
{
	QPushButton::resizeEvent( event );
}

void ButtonPinyin::mousePressEvent ( QMouseEvent * event )
{
	m_Icon = QPixmap(m_sIconPushed);
	QPushButton::mousePressEvent( event );
}

void ButtonPinyin::mouseReleaseEvent ( QMouseEvent * event )
{
	m_Icon = QPixmap(m_sIconBase);
	update();
	QPushButton::mouseReleaseEvent( event );
}

void ButtonPinyin::keyPressEvent ( QKeyEvent * event )
{
	printf("ButtonPinyin::keyPressEvent\n");
}
static QLabel *BottonPressLarge1=NULL;
static QLabel *BottonPressPIN1=NULL;
PinyinInput::PinyinInput(QWidget *parent)
    :YTInputMethodC(parent),PinyinImagine(NULL),setIsso(false),lastKeyId(65535)
{
    QPixmap tmp_pixmap;
	this->setAutoFillBackground(true);
	
	QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/pinyinBackground.png")));
	palette.setColor(QPalette::Active, QPalette::WindowText, QColor(255, 255, 0));
	palette.setColor(QPalette::Active, QPalette::Window, QColor(174, 155,45,95)); //make the background 100% transparent
	palette.setColor(QPalette::Active, QPalette::ButtonText, QColor(20, 20,20)); 

	this->setPalette (palette);

	tmp_pixmap = QPixmap(":/images/pinyinBackground.png");
	this->setGeometry(0, 83, tmp_pixmap.width(), tmp_pixmap.height());
	this->setFocusPolicy(Qt::ClickFocus);	

	AssociationBottonX = 0;
	AssociationBottonY = 2;
	IsPage = false;
	currentpageNum = 1;
		
	QFont font ("fzhei", 25, QFont::Bold);
	font.setPixelSize(25);
	this->setFont (font);
	

	palette.setColor (QPalette::Active, static_cast<QPalette::ColorRole> (1), Qt::white);
    palette.setColor(QPalette::Active, QPalette::ButtonText, Qt::white);     
    palette.setColor(QPalette::Inactive, QPalette::ButtonText,Qt::white);     

	//ShowPinyinLabel= new QLabel(0,Qt::Tool);
	//ShowPinyinLabel->hide();
	//ShowPinyinLabel->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
	ShowPinyinLabel = BottonPressPIN1;
	ShowPinyinLabelText = new QLabel(ShowPinyinLabel);
	ShowPinyinLabelText->hide();
    if(PinyinImagine == NULL)
    {
    	PinyinImagine = new FcitxInputMethod();
    }
	if(PinyinImagine)
	{
		PinyinImagine->target = this;
		PinyinImagine->setDownlist(&downlist);
	}


	QFont fontbutton ("fzhei", 16, QFont::Bold);
	fontbutton.setPixelSize(16);
    AssociationBotton1 = new ButtonPinyin(this);
    //pbShow1->setObjectName(QString::fromUtf8("pbShow1"));
	//pbShow1->setText(tr("1"));
    AssociationBotton1->setGeometry(QRect(AssociationBottonX, AssociationBottonY, 60, 44));
    AssociationBotton1->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    AssociationBotton1->setPalette (palette);
	AssociationBotton1->setFont (fontbutton);
	
    AssociationBotton2 = new ButtonPinyin(this);
    //pbShow2->setObjectName(QString::fromUtf8("pbShow2"));
	//pbShow1->setText(tr("2"));
    AssociationBotton2->setGeometry(QRect(AssociationBottonX +60, AssociationBottonY, 60, 44));
    AssociationBotton2->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    AssociationBotton2->setPalette (palette);
	AssociationBotton2->setFont (fontbutton);
	
    AssociationBotton3 = new ButtonPinyin(this);
    //pbShow3->setObjectName(QString::fromUtf8("pbShow3"));
	//pbShow1->setText(tr("3"));
    AssociationBotton3->setGeometry(QRect(AssociationBottonX +120, AssociationBottonY, 60, 44));
    AssociationBotton3->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    AssociationBotton3->setPalette (palette);
	AssociationBotton3->setFont (fontbutton);
	
    AssociationBotton4 = new ButtonPinyin(this);
    //AssociationBotton4->setObjectName(QString::fromUtf8("pbShow4"));
	//AssociationBotton4->setText(tr("4"));
    AssociationBotton4->setGeometry(QRect(AssociationBottonX +180, AssociationBottonY, 60, 44));
    AssociationBotton4->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    AssociationBotton4->setPalette (palette);
	AssociationBotton4->setFont (fontbutton);
	
    AssociationBotton5 = new ButtonPinyin(this);
    //pbShow5->setObjectName(QString::fromUtf8("pbShow5"));
	//pbShow1->setText(tr("5"));
    AssociationBotton5->setGeometry(QRect(AssociationBottonX +240, AssociationBottonY, 60, 44));
    AssociationBotton5->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    AssociationBotton5->setPalette (palette);
	AssociationBotton5->setFont (fontbutton);
	
    AssociationBotton6 = new ButtonPinyin(this);
    //pbShow6->setObjectName(QString::fromUtf8("pbShow6"));
	//pbShow1->setText(tr("6"));
    AssociationBotton6->setGeometry(QRect(AssociationBottonX +300, AssociationBottonY, 60, 44));
    AssociationBotton6->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    AssociationBotton6->setPalette (palette);
	AssociationBotton6->setFont (fontbutton);
	
    AssociationBotton7 = new ButtonPinyin(this);
    //pbShow7->setObjectName(QString::fromUtf8("pbShow7"));
	//pbShow1->setText(tr("7"));
    AssociationBotton7->setGeometry(QRect(AssociationBottonX +360, AssociationBottonY, 60, 44));
    AssociationBotton7->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    AssociationBotton7->setPalette (palette);
	AssociationBotton7->setFont (fontbutton);
	
    AssociationBotton8 = new ButtonPinyin(this);
    //pbShow8->setObjectName(QString::fromUtf8("pbShow8"));
	//pbShow1->setText(tr("8"));
    AssociationBotton8->setGeometry(QRect(AssociationBottonX +420, AssociationBottonY, 60, 44));
    AssociationBotton8->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    AssociationBotton8->setPalette (palette);
	AssociationBotton8->setFont (fontbutton);

	
	showAssociationBotton();
#if 1
    connect(AssociationBotton1, SIGNAL(clicked()), this, SLOT(on_AssociationBotton1_clicked()));
    connect(AssociationBotton2, SIGNAL(clicked()), this, SLOT(on_AssociationBotton2_clicked()));
    connect(AssociationBotton3, SIGNAL(clicked()), this, SLOT(on_AssociationBotton3_clicked()));
    connect(AssociationBotton4, SIGNAL(clicked()), this, SLOT(on_AssociationBotton4_clicked()));
    connect(AssociationBotton5, SIGNAL(clicked()), this, SLOT(on_AssociationBotton5_clicked()));
    connect(AssociationBotton6, SIGNAL(clicked()), this, SLOT(on_AssociationBotton6_clicked()));
    connect(AssociationBotton7, SIGNAL(clicked()), this, SLOT(on_AssociationBotton7_clicked()));
    connect(AssociationBotton8, SIGNAL(clicked()), this, SLOT(on_AssociationBotton8_clicked()));
#endif	
  	PinyinImagine->ChangeIMState(IS_CHN);
	
	PressLabel = new QLabel(this);
	PressLabel->setPixmap(QPixmap(":/images/pinyinpressPix.png"));
 	PressLabel->hide();
	
	//PressLargeLabel = new QLabel(0);
	//PressLargeLabel->setPixmap(QPixmap(":/images/pinyinpressLargePix.png"));
 	//PressLargeLabel->hide();
	//PressLargeLabel->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);

	PressLargeLabel=BottonPressLarge1;

	PressLargeLabelText= new QLabel(PressLargeLabel);
 	PressLargeLabelText->hide();
	PressLargeLabelText->setAlignment(Qt::AlignHCenter);	

	DelTimeOutTime = 1;
	m_DelTimer = new QTimer(this);

	connect(m_DelTimer, SIGNAL(timeout()), this, SLOT(DelTimer()));
}

PinyinInput::~PinyinInput()
{
	if(PinyinImagine)
	{
		PinyinImagine->Reset();
	}

}

void PinyinInput::SetAssociationBottonText(QStringList showlist)
{
	int X= AssociationBottonX;
	int length = 0;
	int nCount = showlist.size();
	pageMember tmpPage;
	QString string;
		
	pageButtonlist.clear();
	IsPage = false;
	QFontMetrics temp = AssociationBotton1->fontMetrics();
	int pixsize;
	if(nCount>0)
	{
		tmpPage.startitem = 1;
		if(nCount>0)
		{
			string = static_cast<QString>(showlist.at(0));	
		}
		string = string.mid(0,string.length()-1);
		pixsize = temp.width(string) + 10;
		AssociationBotton1->setText((!string.isNull()) ? string:"");
		if(X + pixsize < 392)
		{
    		AssociationBotton1->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));
			X += pixsize + 6;
		}
		else if(X == AssociationBottonX)
		{
			tmpPage.startitem = 1;
			tmpPage.enditem = 1;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);

			IsPage = true;
    		AssociationBotton1->setGeometry(QRect(X, AssociationBottonY, 392, 50));

			tmpPage.startitem = 2;
			X= AssociationBottonX;
		}
		
		if(nCount>1)
		{
			string = static_cast<QString>(showlist.at(1));	
		}
		string = string.mid(0,string.length()-1);
		pixsize = temp.width(string) + 10;
		AssociationBotton2->setText((!string.isNull()) ? string:"");
		if(X + pixsize < 392)
		{
    		AssociationBotton2->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));
			X += pixsize + 6;
		}
		else if(X == AssociationBottonX)
		{
			tmpPage.startitem = 2;
			tmpPage.enditem = 2;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			
			IsPage = true;
    		AssociationBotton2->setGeometry(QRect(X, AssociationBottonY, 392, 50));

			tmpPage.startitem = 3;
			X= AssociationBottonX;
		}
		else
 		{
			tmpPage.enditem = 1;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;

			X= AssociationBottonX;
			tmpPage.startitem = 2;
    		AssociationBotton2->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));

			X += pixsize + 6;
		}
		
		if(nCount>2)
		{
			string = static_cast<QString>(showlist.at(2));	
		}
		string = string.mid(0,string.length()-1);
		pixsize = temp.width(string) + 10;
		AssociationBotton3->setText((!string.isNull()) ? string:"");
		if(X + pixsize < 392)
		{
    		AssociationBotton3->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));
			X += pixsize + 6;
		}
		else if(X == AssociationBottonX)
		{
			tmpPage.startitem = 3;
			tmpPage.enditem = 3;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;
    		AssociationBotton3->setGeometry(QRect(X, AssociationBottonY, 392, 50));

			tmpPage.startitem = 4;
			X= AssociationBottonX;
		}
		else
 		{
			tmpPage.enditem = 2;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;

			X= AssociationBottonX;
			tmpPage.startitem = 3;
    		AssociationBotton3->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));

			X += pixsize + 6;
		}
		
		if(nCount>3)
		{
			string = static_cast<QString>(showlist.at(3));	
		}
		string = string.mid(0,string.length()-1);
		pixsize = temp.width(string) + 10;
		AssociationBotton4->setText((!string.isNull()) ? string:"");
		if(X + pixsize < 392)
		{
    		AssociationBotton4->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));
			X += pixsize + 6;
		}
		else if(X == AssociationBottonX)
		{
			tmpPage.startitem = 4;
			tmpPage.enditem = 4;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;
    		AssociationBotton4->setGeometry(QRect(X, AssociationBottonY, 392, 50));

			tmpPage.startitem = 5;
			X= AssociationBottonX;
		}
		else
 		{
			tmpPage.enditem = 3;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;

			X= AssociationBottonX;
			tmpPage.startitem = 4;
    		AssociationBotton4->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));

			X += pixsize + 6;
		}
		
		if(nCount>4)
		{
			string = static_cast<QString>(showlist.at(4));	
		}
		string = string.mid(0,string.length()-1);
		pixsize = temp.width(string) + 10;
		AssociationBotton5->setText((!string.isNull()) ? string:"");
		if(X + pixsize < 392)
		{
    		AssociationBotton5->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));
			X += pixsize + 6;
		}
		else if(X == AssociationBottonX)
		{
			tmpPage.startitem = 5;
			tmpPage.enditem = 5;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;
    		AssociationBotton5->setGeometry(QRect(X, AssociationBottonY, 392, 50));

			tmpPage.startitem = 5;
			X= AssociationBottonX;
		}
		else
 		{
			tmpPage.enditem = 4;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;

			X= AssociationBottonX;
			tmpPage.startitem = 5;
    		AssociationBotton5->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));

			X += pixsize + 6;
		}
		
		
		if(nCount>5)
		{
			string = static_cast<QString>(showlist.at(5));	
		}
		string = string.mid(0,string.length()-1);
		pixsize = temp.width(string) + 10;
		AssociationBotton6->setText((!string.isNull()) ? string:"");
		if(X + pixsize < 392)
		{
    		AssociationBotton6->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));
			X += pixsize + 6;
		}
		else if(X == AssociationBottonX)
		{
			tmpPage.startitem = 6;
			tmpPage.enditem = 6;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;
    		AssociationBotton6->setGeometry(QRect(X, AssociationBottonY, 392, 50));

			tmpPage.startitem = 7;
			X= AssociationBottonX;
		}
		else
 		{
			tmpPage.enditem = 5;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;

			X= AssociationBottonX;
			tmpPage.startitem = 6;
    		AssociationBotton6->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));

			X += pixsize + 6;
		}
		   
		if(nCount>6)
		{
			string = static_cast<QString>(showlist.at(6));	
		}
		string = string.mid(0,string.length()-1);
		pixsize = temp.width(string) + 10;
		AssociationBotton7->setText((!string.isNull()) ? string:"");
		if(X + pixsize < 392)
		{
    		AssociationBotton7->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));
			X += pixsize + 6;
		}
		else if(X == AssociationBottonX)
		{
			tmpPage.startitem = 7;
			tmpPage.enditem = 7;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;
    		AssociationBotton7->setGeometry(QRect(X, AssociationBottonY, 392, 50));

			tmpPage.startitem = 8;
			X= AssociationBottonX;
		}
		else
 		{
			tmpPage.enditem = 6;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;

			X= AssociationBottonX;
			tmpPage.startitem = 7;
    		AssociationBotton7->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));

			X += pixsize + 6;
		}

		if(nCount>7)
		{
		string = static_cast<QString>(showlist.at(7));	
		}
		string = string.mid(0,string.length());
		pixsize = temp.width(string) + 10;
		AssociationBotton8->setText( (!string.isNull()) ? string:"");
		if(X + pixsize < 392)
		{
    		AssociationBotton8->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));
			X += pixsize + 6;

			tmpPage.enditem = 8;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
		}
		else if(X == AssociationBottonX)
		{
			tmpPage.startitem = 8;
			tmpPage.enditem = 8;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;
    		AssociationBotton8->setGeometry(QRect(X, AssociationBottonY, 392, 50));

			X= AssociationBottonX;
		}
		else
 		{
			tmpPage.enditem = 7;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
			IsPage = true;

			X= AssociationBottonX;
			tmpPage.startitem = 8;
			tmpPage.enditem = 8;
			tmpPage.pagenum = pageButtonlist.size()+1;
        	pageButtonlist.append(tmpPage);
    		AssociationBotton8->setGeometry(QRect(X, AssociationBottonY, pixsize, 50));

		}
		currentpageNum = 1;
	}
	else
	{
		ClearAssociationBottonText();
		currentpageNum = 1;
	}
	
	showAssociationBotton();
}

void PinyinInput::ClearAssociationBottonText()
{
	AssociationBotton1->setText("");
	AssociationBotton2->setText("");
	AssociationBotton3->setText("");
	AssociationBotton4->setText("");
	AssociationBotton5->setText("");
	AssociationBotton6->setText("");
	AssociationBotton7->setText("");
	AssociationBotton8->setText("");
 
	if(!PinyinImagine->upMsgLen())
	{
		YTInputServerC::instance()->sendCommitText(pinyin);

		pinyin.clear();
		keyvalue.clear();
		showpinyinlabel();
	}
}

void PinyinInput::showAssociationBotton()
{
	pageMember tmpPage;
	int i;

	AssociationBotton1->hide();
	AssociationBotton2->hide();
	AssociationBotton3->hide();
	AssociationBotton4->hide();
	AssociationBotton5->hide();
	AssociationBotton6->hide();
	AssociationBotton7->hide();
	AssociationBotton8->hide();
	
	if(IsPage)
	{
		if(pageButtonlist.size()>0 && currentpageNum<= pageButtonlist.size())
		{
			tmpPage = static_cast<pageMember>(pageButtonlist.at(currentpageNum - 1));	
		}
		
		if(1>=tmpPage.startitem && 1<=tmpPage.enditem)
		{
			AssociationBotton1->show();
		}

		if(2>=tmpPage.startitem && 2<=tmpPage.enditem)
		{
			AssociationBotton2->show();
		}
		
		if(3>=tmpPage.startitem && 3<=tmpPage.enditem)
		{
			AssociationBotton3->show();
		}
		
		if(4>=tmpPage.startitem && 4<=tmpPage.enditem)
		{
			AssociationBotton4->show();
		}

		if(5>=tmpPage.startitem && 5<=tmpPage.enditem)
		{
			AssociationBotton5->show();
		}
		
		if(6>=tmpPage.startitem && 6<=tmpPage.enditem)
		{
			AssociationBotton6->show();
		}
		
		if(7>=tmpPage.startitem && 7<=tmpPage.enditem)
		{
			AssociationBotton7->show();
		}
		
		if(8>=tmpPage.startitem && 8<=tmpPage.enditem)
		{
			AssociationBotton8->show();
		}
				
	}
	else
	{
		AssociationBotton1->show();
		AssociationBotton2->show();
		AssociationBotton3->show();
		AssociationBotton4->show();
		AssociationBotton5->show();
		AssociationBotton6->show();
		AssociationBotton7->show();
		AssociationBotton8->show();
	}
}


void PinyinInput::DelTimer()
{
	if(PinyinImagine->imeState == IS_CHN && PinyinImagine->upMsgLen())
	{
	  	PinyinImagine->process_key_event(IMKeyEvent(SCIM_KEY_BackSpace));
		showpinyinlabel();	
	}
	else if(keyvalue.isEmpty())
	{
		YTInputServerC::instance()->sendOneKey(Qt::Key_Backspace,0);
	}

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

void PinyinInput::on_AssociationBotton1_clicked()
{
	QString tep="1";
    int c = tep.toAscii().at(0); 

	if(setIsso)
	{
		setIsso = false;
		pinyin.append(AssociationBotton1->text());		
		ClearAssociationBottonText();
	}
	else
	{
		pinyin.append(AssociationBotton1->text());
		PinyinImagine->process_key_event(IMKeyEvent(c));
		showpinyinlabel();
	}
	
}

void PinyinInput::on_AssociationBotton2_clicked()
{
	QString tep="2";
    int c = tep.toAscii().at(0); 

	if(setIsso)
	{
		setIsso = false;
		pinyin.append(AssociationBotton2->text());		
		ClearAssociationBottonText();
	}
	else
	{
		pinyin.append(AssociationBotton2->text());
		PinyinImagine->process_key_event(IMKeyEvent(c));
		showpinyinlabel();
	}
	
}

void PinyinInput::on_AssociationBotton3_clicked()
{
	QString tep="3";
    int c = tep.toAscii().at(0); 

	if(setIsso)
	{
		setIsso = false;
		pinyin.append(AssociationBotton3->text());		
		ClearAssociationBottonText();
	}
	else
	{
		pinyin.append(AssociationBotton3->text());
		PinyinImagine->process_key_event(IMKeyEvent(c));
		showpinyinlabel();
	}
	
}

void PinyinInput::on_AssociationBotton4_clicked()
{
	QString tep="4";
    int c = tep.toAscii().at(0); 

	if(setIsso)
	{
		setIsso = false;
		pinyin.append(AssociationBotton4->text());		
		ClearAssociationBottonText();
	}
	else
	{
		pinyin.append(AssociationBotton4->text());
		PinyinImagine->process_key_event(IMKeyEvent(c));
		showpinyinlabel();
	}
}

void PinyinInput::on_AssociationBotton5_clicked()
{
	QString tep="5";
    int c = tep.toAscii().at(0); 

	if(setIsso)
	{
		setIsso = false;
		pinyin.append(AssociationBotton5->text());		
		ClearAssociationBottonText();
	}
	else
	{
		pinyin.append(AssociationBotton5->text());
		PinyinImagine->process_key_event(IMKeyEvent(c));
		showpinyinlabel();
	}
	
}

void PinyinInput::on_AssociationBotton6_clicked()
{
	QString tep="6";
    int c = tep.toAscii().at(0); 

	if(setIsso)
	{
		setIsso = false;
		pinyin.append(AssociationBotton6->text());		
		ClearAssociationBottonText();
	}
	else
	{
		pinyin.append(AssociationBotton6->text());
		PinyinImagine->process_key_event(IMKeyEvent(c));
		showpinyinlabel();
	}
	
}

void PinyinInput::on_AssociationBotton7_clicked()
{
	QString tep="7";
    int c = tep.toAscii().at(0); 

	if(setIsso)
	{
		setIsso = false;
		pinyin.append(AssociationBotton7->text());		
		ClearAssociationBottonText();
	}
	else
	{
		pinyin.append(AssociationBotton7->text());
		PinyinImagine->process_key_event(IMKeyEvent(c));
		showpinyinlabel();
	}
}

void PinyinInput::on_AssociationBotton8_clicked()
{
	QString tep="8";
    int c = tep.toAscii().at(0); 

	if(setIsso)
	{
		setIsso = false;
		pinyin.append(AssociationBotton8->text());		
		ClearAssociationBottonText();
	}
	else
	{
		pinyin.append(AssociationBotton8->text());
		PinyinImagine->process_key_event(IMKeyEvent(c));
		showpinyinlabel();
	}
	
}


void PinyinInput::mouseReleaseEvent(QMouseEvent * event )
{
	int i = 0;

	PressLabel->hide();
	PressLargeLabel->hide();
	
	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > letterParameter[i].x) && (event->x() < letterParameter[i].x + letterParameter[i].width) 
			&& (event->y() > letterParameter[i].y) && (event->y() < letterParameter[i].y + letterParameter[i].Vheigth))
		{
			break;
		}
	}
	if((i >= 0) && (i < keyNumber))
	{
		if(letterParameter[i].text == tr("right"))
		{
			if(IsPage)
			{
				if(currentpageNum == pageButtonlist.size())
				{
					if(setIsso)
					{
						setIsso = false;
						SetAssociationBottonText(downlist);
					}
					else
					{
						PinyinImagine->process_key_event(IMKeyEvent('.'));
					}
				}
				else
				{
					currentpageNum++;
					showAssociationBotton();
				}
			}
			else if(!setIsso)
			{
				PinyinImagine->process_key_event(IMKeyEvent('.'));
			}
		}
		else if(letterParameter[i].text == tr("left"))
		{
			if(IsPage)
			{
				if(currentpageNum == 1)
				{
					if(setIsso)
					{
						//showAssociationBotton();
						 
					}
					else
					{
      					PinyinImagine->process_key_event(IMKeyEvent(','));
					}
				}
				else
				{
					currentpageNum--;
					showAssociationBotton();
				}
			}
			else if(!setIsso)
			{
      			PinyinImagine->process_key_event(IMKeyEvent(','));
			}
		}
		else if(letterParameter[i].text == tr("space"))
		{
			YTInputServerC::instance()->sendCommitText(QString(" "));
		
			PinyinImagine->process_key_event(IMKeyEvent(SCIM_KEY_space));
		}
		else if(letterParameter[i].text == tr("del"))
		{
			m_DelTimer->stop();
			DelTimeOutTime = 1;
			if(PinyinImagine->imeState == IS_CHN && PinyinImagine->upMsgLen())
			{
			  	PinyinImagine->process_key_event(IMKeyEvent(SCIM_KEY_BackSpace));
				showpinyinlabel();	
				
			}
			else if(keyvalue.isEmpty())
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_Backspace,0);
			}
 
 		}
		else if(letterParameter[i].text == tr("change"))
		{
			YTInputServerC::instance()->showInputMethodSelect();
		}
		else if(letterParameter[i].text == tr("hide"))
		{
			ShowPinyinLabel->hide();
			emit hideInputMethod();
		}
		else
		{
			int c = letterParameter[i].text.toAscii().at(0); 
			PinyinImagine->process_key_event(IMKeyEvent(c));
			showpinyinlabel();			

		}
	}
	
	lastKeyId = 65535;
	update();
}

void PinyinInput::mouseMoveEvent(QMouseEvent *event)
{
	int i = 0;

	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > letterParameter[i].x) && (event->x() < letterParameter[i].x + letterParameter[i].width) 
			&& (event->y() > letterParameter[i].y) && (event->y() < letterParameter[i].y + letterParameter[i].Vheigth))
		{
			if(lastKeyId != i)
			{
				lastKeyId = i;
				showPress(letterParameter[i].text,letterParameter[i].x,letterParameter[i].y,letterParameter[i].width,letterParameter[i].Vheigth,i);
				
				if(i<26)
				{
					showPressLarge(letterParameter[i].text,this->geometry().x()+letterParameter[i].x,this->geometry().y()+letterParameter[i].y - 70,56,68);
				}

				if(letterParameter[i].text == tr("del"))
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
		PressLabel->hide();
		PressLargeLabel->hide();
	}
}

void PinyinInput::mousePressEvent(QMouseEvent *event)
{	
	int i = 0;

	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > letterParameter[i].x) && (event->x() < letterParameter[i].x + letterParameter[i].width) 
			&& (event->y() > letterParameter[i].y) && (event->y() < letterParameter[i].y + letterParameter[i].Vheigth))
		{
			if(lastKeyId != i)
			{
				lastKeyId = i;
				showPress(letterParameter[i].text,letterParameter[i].x,letterParameter[i].y,letterParameter[i].width,letterParameter[i].Vheigth,i);
				if(i<26)
				{
					showPressLarge(letterParameter[i].text,this->geometry().x()+letterParameter[i].x,this->geometry().y()+letterParameter[i].y - 70,56,68);
				}

				if(letterParameter[i].text == tr("del"))
				{
					m_DelTimer->start(600);
				}
			}
			break;
		}  
	}
	
}

void PinyinInput::showPress(QString text,int x,int y,int width,int height,int id)
{ 
	QPixmap tmppix;
	
	if(letterParameter[id].text == tr("right"))
	{
		tmppix = QPixmap(":/images/pinyinrightpre.png");
	}
	else if(letterParameter[id].text == tr("left"))
	{
		tmppix = QPixmap(":/images/pinyinleftpre.png");
	}
	else if(letterParameter[id].text == tr("space"))
	{
		tmppix = QPixmap(":/images/pinyinspacepre.png");
	}
	else if(letterParameter[id].text == tr("del"))
	{
		tmppix = QPixmap(":/images/pinyindelpre.png");
	}
	else if(letterParameter[id].text == tr("change"))
	{
		tmppix = QPixmap(":/images/pinyinchangepre.png");
	}
	else if(letterParameter[id].text == tr("hide"))
	{
		tmppix = QPixmap(":/images/pinyinhidepre.png");
	}
	else
	{
		tmppix = QPixmap(":/images/pinyinpressPix.png");
	}
	PressLabel->setGeometry(x,y,width,height);
	//PressLabel->setPixmap(QPixmap(":/images/pinyinpressPix.png"));
	PressLabel->setPixmap(tmppix);
 	PressLabel->show();
}

void PinyinInput::showPressLarge(QString text,int x,int y,int width,int height)
{
	//PressLargeLabel->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
    PressLargeLabel->setStyleSheet("QLabel {background:rgba(0,0,0,0%);border-style:outset;font: bold 30px}");
    PressLargeLabelText->setStyleSheet("QLabel{background:rgba(0,0,0,0%);border-style:outset;font: bold 30px}");

	PressLargeLabelText->setGeometry(0,0,width,height);
	PressLargeLabelText->setText(text.toUpper());
 	PressLargeLabelText->show();


	PressLargeLabel->setGeometry(x,y,width,height);
	PressLargeLabel->setPixmap(QPixmap(":/images/pinyinpressLargePix.png"));
 	PressLargeLabel->show();

}
void PinyinInput::showpinyinlabel()
{
	QFontMetrics temp = ShowPinyinLabel->fontMetrics();
	QPixmap tmp_PinyinBackground_Pix;
	QPixmap tmp_PinyinBackground_Scaled;
	int tmp_scaled_width = 0;
	int tmp_scaled_height = 0;
	QString tmp_text;
	QPalette palette;
	
	palette.setColor (QPalette::Active, static_cast<QPalette::ColorRole> (1), Qt::white);
	palette.setColor(QPalette::Active, QPalette::WindowText, Qt::white);     
	palette.setColor(QPalette::Inactive, QPalette::WindowText,Qt::white);     

	ShowPinyinLabel->setPalette(palette);	
	ShowPinyinLabelText->setPalette(palette);	
	
	int length = temp.width(keyvalue) + 10;
	if(length > 450)
	{
		printf("-----------+++++++++++++++++++++++++++++++++------keyvalue.length() >500------%d--length %d---",keyvalue.length(),length);
		tmp_text = keyvalue.right(50);
		length = 450;
	}
	else
	{
		tmp_text = keyvalue;
	}
	//ShowPinyinLabel->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
	ShowPinyinLabel->setStyleSheet("QLabel{background:rgba(0,0,0,0%);border-style:outset;font: bold 30px}");
	ShowPinyinLabelText->setStyleSheet("QLabel{background:rgba(0,0,0,0%);border-style:outset;font: bold 20px}");

	ShowPinyinLabelText->setGeometry(0,0,length,23);
	ShowPinyinLabelText->setText(tmp_text);
	ShowPinyinLabelText->show();

	tmp_PinyinBackground_Pix = QPixmap(":/images/pinyinLable.png");
	tmp_scaled_height = tmp_PinyinBackground_Pix.height();
	tmp_scaled_width = length;
	tmp_PinyinBackground_Scaled = tmp_PinyinBackground_Pix.scaled(tmp_scaled_width,tmp_scaled_height);
	ShowPinyinLabel->setGeometry(this->geometry().x()+28,this->geometry().y()-13,tmp_scaled_width,23);
	ShowPinyinLabel->setPixmap(tmp_PinyinBackground_Scaled);
	if(keyvalue.isEmpty())
	{
		ShowPinyinLabel->hide();
	}
	else
	{
		ShowPinyinLabel->show();
	}
}

void PinyinInput::keyPressEvent(QKeyEvent * event)
{
	switch(event->key())
	{
		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Left:
		case Qt::Key_Right:
		{
			break;
		}
		case Qt::Key_Space:
		{
			YTInputServerC::instance()->sendCommitText(QString(" "));
			PinyinImagine->process_key_event(IMKeyEvent(SCIM_KEY_space));

			event->accept();
			break;
		}
		case Qt::Key_Backspace:
		{
			if(PinyinImagine->imeState == IS_CHN && PinyinImagine->upMsgLen())
			{
			  	PinyinImagine->process_key_event(IMKeyEvent(SCIM_KEY_BackSpace));
				showpinyinlabel();	
				
			}
			else if(keyvalue.isEmpty())
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_Backspace,0);
			}
			
			event->accept();
			break;
		}

		case Qt::Key_Enter:
		{
			event->accept();
			break;
		}

		default:
			event->accept();
			break;
	}
}
void PinyinInput::setContextWords(QString textBeforeCursor,QStringList contextWords)
{
	if(contextWords.size()>0)
	{
		pinyin.clear();
		keyvalue.clear();
		showpinyinlabel();
	
		setIsso = true;
		AssociationText = contextWords;
		SetAssociationBottonText(AssociationText);
	}
	
	if(!textBeforeCursor.isEmpty())
	{
	
	}
}
void PinyinInput::reset()
{
	ClearAssociationBottonText();
	pageButtonlist.clear();
	IsPage = false;
	setIsso = false;
	currentpageNum = 1;
	PinyinImagine->Reset();
	PinyinImagine->commitStrinNull = false;
	ShowPinyinLabel->setText("");
	ShowPinyinLabel->hide();
	PressLabel->hide();
	PressLargeLabel->hide();
}

void PinyinInput::NewLargeLable()
{
	BottonPressLarge1 = new QLabel(0,Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
	BottonPressLarge1->setGeometry(-800,-800,10,10);
	BottonPressLarge1->show();
	BottonPressLarge1->hide();
	//BottonPressLarge->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
}
void PinyinInput::NewPINLable()
{
	 BottonPressPIN1 = new QLabel(0,Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
	BottonPressPIN1->setGeometry(-800,-800,10,10);
	BottonPressPIN1->show();
	BottonPressPIN1->hide();
	//BottonPressLarge->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
}

