#include "handinput.h"
#include <stdlib.h>
#include <stdio.h>
//#include <QDebug>

#define Text_Xstart  109
#define Text_Ystart  12
#define HandWriteAere_width 	450
#define HandWriteAere_heigth 	260

void HWRE_FreeDict( unsigned char** hDic )
{	
	if( hDic != NULL && *hDic != NULL )	
	{
		free(*hDic);
		*hDic = NULL;	
	}
	
	return;
}


int HWRE_LoadDict(unsigned char** hDic)
{	
	FILE *in;
	long nLen;
	unsigned char* pDic;
	
	in = fopen("Dic_GB2312_littleEndian.bin", "rb" );
	if( in == NULL )
	{
		return -1;
	}
	
	fseek( in, 0, SEEK_END );
	nLen = ftell(in);
	fseek( in, 0, SEEK_SET );
	
	pDic = (unsigned char*)malloc(nLen);
	if( pDic == NULL )	
	{
		fclose(in);
		return -2;
	}
	
	fread( pDic, 1, nLen, in );	
	fclose(in);	
	*hDic = pDic;	
	
	return 0;
}

ButtonHand::ButtonHand(QWidget *parent) : QPushButton(parent)
{

}
void ButtonHand::setPixmap(QString sIconNameBase, QString sIconNamePushed)
{
	m_sIconBase		= sIconNameBase;
	m_sIconPushed	= sIconNamePushed;

	m_Icon = QPixmap(m_sIconBase);
       
}

void ButtonHand::paintEvent(QPaintEvent *)
{
	QPainter p (this);
	QStyleOptionButton option;
	option.initFrom (this);
	p.drawPixmap(QRect(0,0,this->width(),this->height()),m_Icon);
	p.setPen (this->palette().color (QPalette::ButtonText) );
	p.drawText (QRect(0,0,this->width(),this->height()), Qt::AlignHCenter, this->text() );

}

void ButtonHand::mousePressEvent ( QMouseEvent * event )
{
	m_Icon = QPixmap(m_sIconPushed);
	QPushButton::mousePressEvent( event );
	emit pressed();
}

void ButtonHand::mouseReleaseEvent ( QMouseEvent * event )
{
	m_Icon = QPixmap(m_sIconBase);
	update();
	QPushButton::mouseReleaseEvent( event );

    emit clicked();
}


void HandInputWidget::RefreshInfo()
{	
	m_nStopTime++;
	if( m_nStopTime > 1 && m_arrPoints.size() > 0 )
	{
		m_nStopTime = 0;
		m_arrPoints.clear();
		image.fill(qRgb(192, 196, 199));
		update();
	}
	if(m_pHandInput && true == m_pHandInput->AutomaticCandidate)
	{
		m_pHandInput->AutomaticCandidate = false;
		YTInputServerC::instance()->sendPreeditText(m_pHandInput->pbShow1->text());
		m_pHandInput->AutomaticIsSend = true;
	}
}

HandInputWidget::HandInputWidget(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	m_pHandInput=(handinput *)parent;
	this->setGeometry(QRect(Text_Xstart + 2 ,Text_Ystart + 2,HandWriteAere_width - 4,HandWriteAere_heigth - 4));

	m_nStopTime = 0;
	
	QImage newImage(QRect(Text_Xstart + 2 ,Text_Ystart + 2,HandWriteAere_width - 4,HandWriteAere_heigth - 4).size(), QImage::Format_RGB32);
	newImage.fill(qRgb(192, 196, 199));
	image = newImage;

	m_RefreshTimer = new QTimer(this);
	connect(m_RefreshTimer, SIGNAL(timeout()), this, SLOT(RefreshInfo()));
}

HandInputWidget::~HandInputWidget()
{
	if( m_RefreshTimer != NULL )
	{
		m_RefreshTimer->stop();
		delete m_RefreshTimer;
		m_RefreshTimer = NULL;
	}
}
void HandInputWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
	m_RefreshTimer->stop();
	scribbling = true;
    
	if(!m_pHandInput->AutomaticCandidate)
	{
		YTInputServerC::instance()->confirmPreeditText();
		m_pHandInput->AutomaticCandidate = true;
	}
	
	THWPoint pt;
	pt.x=mouseEvent->x();
	pt.y=mouseEvent->y();
	m_arrPoints.append(pt);	

	lastPoint = mouseEvent->pos();
}

void HandInputWidget::mouseMoveEvent(QMouseEvent *mouseEvent)
{		
	THWPoint pt;
	pt.x=mouseEvent->x();
	pt.y=mouseEvent->y();
	m_arrPoints.append(pt);	

	if (scribbling)
		drawLineTo(mouseEvent->pos());
}

void HandInputWidget::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	m_nStopTime = 0;
	m_RefreshTimer->start(700); 

	THWPoint pt;
	pt.x=mouseEvent->x();
	pt.y=mouseEvent->y();
	m_arrPoints.append(pt);	

	THWPoint pt1;
	pt1.x=STROKEENDMARK;
	pt1.y=0;
	m_arrPoints.append(pt1);

	if( m_arrPoints.size() > 0 )
	{
		m_pHandInput->m_arrPoints = m_arrPoints;
		m_pHandInput->WriteReturn = true;
		m_pHandInput->handWriteRecong();		
	}

	if (scribbling) {
		drawLineTo(mouseEvent->pos());
		scribbling = false;
	}
}

void HandInputWidget::paintEvent(QPaintEvent* e)
{  
	QPainter painter(this);
	painter.drawImage(e->rect(), image, e->rect());
}

void HandInputWidget::drawLineTo(const QPoint &endPoint)
{
	QPainter painter(&image);
	int myPenWidth = 6;
	painter.setPen(QPen(Qt::blue, myPenWidth, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
	painter.drawLine(lastPoint, endPoint);

	int rad = (myPenWidth/ 2) + 2;
	update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
	lastPoint = endPoint;
}

void handinput::handWriteRecong()
{	
	THWPoint pt;
	pt.x=STROKEENDMARK;
	pt.y=STROKEENDMARK;
	m_arrPoints.append(pt);
	
	THWPoint *aPenData = new THWPoint[m_arrPoints.size()+1];
	for(int i = 0; i < m_arrPoints.size(); i++)
	{
		aPenData[i] = m_arrPoints[i];		
	}
	long pRam[HWRERAMSIZE/ 4]; // acquire RAM for recognition engine,in Four-byte alignment
	
	//unsigned short Result[MAXCANDNUM];

	int iCodeNum;
	
	attr.pRam = ( unsigned char* )pRam;
	attr.iCandidateNum = MAXCANDNUM;			//set the number of candidate you want to be returned	

	
	attr.dwRange =ALC_SC_COMMON| ALC_SC_RARE | ALC_GESTURE | ALC_NUMERIC | CHARSET_ENGLISH | CHARSET_SYMPUNC;;			//set range	
	
	iCodeNum = HWRecognize(aPenData, &attr, Result);
	m_arrPoints.clear();
	delete []aPenData;
	setUiText(iCodeNum,Result,false);
}

handinput::handinput(QWidget *parent)
	: YTInputMethodC(parent),AutomaticCandidate(false),AutomaticIsSend(false),WriteReturn(false)
{
	this->setAutoFillBackground(true);
	
	QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/handBackground.png")));

	this->setPalette (palette);

	QPixmap tmp_pixmap = QPixmap(":/images/handBackground.png");
	this->setGeometry(0, 197, tmp_pixmap.width(), tmp_pixmap.height());

	QFont font ("fzhei", 25, QFont::Bold);
	font.setPixelSize(25);
	this->setFont (font);

	setHandInputUi();
	m_InputWidget = new HandInputWidget(this);/*,QRect(x+80,y+55,width - x - 100,height - y-55)*/
	m_InputWidget->show();

#if  HANWANG_INTEGRATED_DICT==0

	int tmp=HWRE_LoadDict( &pRom );
	if( tmp == 0 )			//load dictionary	
	{
		attr.pRom = pRom;	
	}
	else		
	{
		printf("load dictionary fails\n");
	}
	
#endif
	attr.dwRange=CHARSET_CNDEFAULT| ALC_GESTURE|ALC_TC_TO_SC|ALC_SC_RADICAL|ALC_CS_CURSIVE;
	isChinese=true;

	DelTimeOutTime = 1;
	m_DelTimer = new QTimer(this);
	m_ShowTextTimer = new QTimer(this);

	connect(m_DelTimer, SIGNAL(timeout()), this, SLOT(DelTimer()));
	connect(m_ShowTextTimer, SIGNAL(timeout()), this, SLOT(ShowTextTimer()));
}

handinput::~handinput()
{
#if  HANWANG_INTEGRATED_DICT==0	
	HWRE_FreeDict( &pRom );
#endif
	
}

void handinput::setHandInputUi()
{	    
    QPalette palette;
    palette.setColor(QPalette::Active, QPalette::ButtonText, Qt::black);     
    palette.setColor(QPalette::Inactive, QPalette::ButtonText,Qt::black);   

	QString tmp_base = QString("");
	QString tmp_press = QString(":/images/handtextpre.png");
	int tmp_width= 0 ,tmp_height = 0;

	QPixmap bk_pixmap_press = QPixmap(":/images/handtextpre.png");
	tmp_width = bk_pixmap_press.width();
	tmp_height = bk_pixmap_press.height();

    pbShow1 = new ButtonHand(this);
    pbShow1->setObjectName(QString::fromUtf8("pbShow1"));
    pbShow1->setGeometry(QRect(568, 20, tmp_width, tmp_height));
    pbShow1->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 25px;min-width: 1em;padding: 6px;}");
    pbShow1->setPalette(palette);
	pbShow1->setPixmap(tmp_base,tmp_press);
	
    pbShow2 = new ButtonHand(this);
    pbShow2->setObjectName(QString::fromUtf8("pbShow2"));
    pbShow2->setGeometry(QRect(679, 20, tmp_width, tmp_height));
    pbShow2->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 25px;min-width: 1em;padding: 6px;}");
    pbShow2->setPalette (palette);
	pbShow2->setPixmap(tmp_base,tmp_press);
	
    pbShow3 = new ButtonHand(this);
    pbShow3->setObjectName(QString::fromUtf8("pbShow3"));
    pbShow3->setGeometry(QRect(568, 84, tmp_width, tmp_height));
    pbShow3->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 25px;min-width: 1em;padding: 6px;}");
    pbShow3->setPalette (palette);
	pbShow3->setPixmap(tmp_base,tmp_press);
	
    pbShow4 = new ButtonHand(this);
    pbShow4->setObjectName(QString::fromUtf8("pbShow4"));
    pbShow4->setGeometry(QRect(679, 84, tmp_width, tmp_height));
    pbShow4->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 25px;min-width: 1em;padding: 6px;}");
    pbShow4->setPalette (palette);
	pbShow4->setPixmap(tmp_base,tmp_press);
	
    pbShow5 = new ButtonHand(this);
    pbShow5->setObjectName(QString::fromUtf8("pbShow5"));
    pbShow5->setGeometry(QRect(568, 148, tmp_width, tmp_height));
    pbShow5->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 25px;min-width: 1em;padding: 6px;}");
    pbShow5->setPalette (palette);
	pbShow5->setPixmap(tmp_base,tmp_press);
	
    pbShow6 = new ButtonHand(this);
    pbShow6->setObjectName(QString::fromUtf8("pbShow6"));
    pbShow6->setGeometry(QRect(679, 148, tmp_width, tmp_height));
    pbShow6->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 25px;min-width: 1em;padding: 6px;}");
    pbShow6->setPalette (palette);
	pbShow6->setPixmap(tmp_base,tmp_press);
	
    pbShow7 = new ButtonHand(this);
    pbShow7->setObjectName(QString::fromUtf8("pbShow7"));
    pbShow7->setGeometry(QRect(568, 212, tmp_width, tmp_height));
    pbShow7->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 25px;min-width: 1em;padding: 6px;}");
    pbShow7->setPalette (palette);
	pbShow7->setPixmap(tmp_base,tmp_press);
	
    pbShow8 = new ButtonHand(this);
    pbShow8->setObjectName(QString::fromUtf8("pbShow8"));
    pbShow8->setGeometry(QRect(679, 212, tmp_width, tmp_height));
    pbShow8->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 25px;min-width: 1em;padding: 6px;}");
    pbShow8->setPalette (palette);
	pbShow8->setPixmap(tmp_base,tmp_press);
	
    pbShow9 = new ButtonHand(this);
    pbShow9->setObjectName(QString::fromUtf8("pbShow9"));
    pbShow9->setGeometry(QRect(568, Text_Ystart, tmp_width, tmp_height));
    pbShow9->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 25px;min-width: 1em;padding: 6px;}");
    pbShow9->setPalette (palette);
	pbShow9->setPixmap(tmp_base,tmp_press);
	pbShow9->hide();
	
    pbShow10 = new ButtonHand(this);
    pbShow10->setObjectName(QString::fromUtf8("pbShow10"));
    pbShow10->setGeometry(QRect(75, Text_Ystart, tmp_width, tmp_height));
    pbShow10->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 25px;min-width: 1em;padding: 6px;}");
    pbShow10->setPalette (palette);
	pbShow10->setPixmap(tmp_base,tmp_press);
	pbShow10->hide();

	tmp_press = QString(":/images/handpress.png");

	pbHide = new ButtonHand(this);
    pbHide->setGeometry(QRect(9, 10, 90, 62));
    pbHide->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    pbHide->setPalette (palette);
	pbHide->setPixmap(tmp_base,tmp_press);

	pbDel = new ButtonHand(this);
    pbDel->setGeometry(QRect(9, 78, 90, 62));
    pbDel->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    pbDel->setPalette (palette);
	pbDel->setPixmap(tmp_base,tmp_press);

	pbSpace = new ButtonHand(this);
    pbSpace->setGeometry(QRect(9, 146, 90, 62));
    pbSpace->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    pbSpace->setPalette (palette);
	pbSpace->setPixmap(tmp_base,tmp_press);

	pbChange = new ButtonHand(this);
    pbChange->setGeometry(QRect(9, 214, 90, 62));
    pbChange->setStyleSheet("QPushButton {border-style:outset;border-radius: 10px;border-color: beige;font: bold 30px;min-width: 1em;padding: 6px;}");
    pbChange->setPalette (palette);
	pbChange->setPixmap(tmp_base,tmp_press);

    HandInputSignalAndSlot();
}

void handinput::setContextWords(QString textBeforeCursor,QStringList contextWords)
{
	
	QString string;
	if(!textBeforeCursor.isEmpty())
	{
		setIssoText(textBeforeCursor.right(1));
	}
	if(contextWords.size()>0)
	{
#if lxf
		if(contextWords.size()>=1)
		{
			string = static_cast<QString>(contextWords.at(0));	
			string = string.left(2);
			pbShow10->setText(pbShow9->text());
			pbShow9->setText(pbShow8->text());
			pbShow8->setText(pbShow7->text());
			pbShow7->setText(pbShow6->text());
			pbShow6->setText(pbShow5->text());
			pbShow5->setText(pbShow4->text());
			pbShow4->setText(pbShow3->text());
			pbShow3->setText(pbShow2->text());
			pbShow2->setText(pbShow1->text());
			pbShow1->setText((!string.isNull()) ? string:"");
		}

		if(contextWords.size()>=2)
		{
			string = static_cast<QString>(contextWords.at(1));	
			string = string.left(2);
			pbShow10->setText(pbShow9->text());
			pbShow9->setText(pbShow8->text());
			pbShow8->setText(pbShow7->text());
			pbShow7->setText(pbShow6->text());
			pbShow6->setText(pbShow5->text());
			pbShow5->setText(pbShow4->text());
			pbShow4->setText(pbShow3->text());
			pbShow3->setText(pbShow2->text());
			pbShow2->setText((!string.isNull()) ? string:"");
		}

		if(contextWords.size()>=3)
		{
			string = static_cast<QString>(contextWords.at(2));	
			string = string.left(2);
			pbShow10->setText(pbShow9->text());
			pbShow9->setText(pbShow8->text());
			pbShow8->setText(pbShow7->text());
			pbShow7->setText(pbShow6->text());
			pbShow6->setText(pbShow5->text());
			pbShow5->setText(pbShow4->text());
			pbShow4->setText(pbShow3->text());
			pbShow3->setText((!string.isNull()) ? string:"");
		}

		if(contextWords.size()>=4)
		{
			string = static_cast<QString>(contextWords.at(3));	
			string = string.left(2);
			pbShow10->setText(pbShow9->text());
			pbShow9->setText(pbShow8->text());
			pbShow8->setText(pbShow7->text());
			pbShow7->setText(pbShow6->text());
			pbShow6->setText(pbShow5->text());
			pbShow5->setText(pbShow4->text());
			pbShow4->setText((!string.isNull()) ? string:"");
		}
#if lxf
		if(contextWords.size()>=5)
		{
			string = static_cast<QString>(contextWords.at(4));	
			string = string.left(2);
			pbShow10->setText(pbShow9->text());
			pbShow9->setText(pbShow8->text());
			pbShow8->setText(pbShow7->text());
			pbShow7->setText(pbShow6->text());
			pbShow6->setText(pbShow5->text());
			pbShow5->setText((!string.isNull()) ? string:"");
		}

		if(contextWords.size()>=6)
		{
			string = static_cast<QString>(contextWords.at(5));	
			string = string.left(2);
			pbShow10->setText(pbShow9->text());
			pbShow9->setText(pbShow8->text());
			pbShow8->setText(pbShow7->text());
			pbShow7->setText(pbShow6->text());
			pbShow6->setText((!string.isNull()) ? string:"");
		}

		if(contextWords.size()>=7)
		{
			string = static_cast<QString>(contextWords.at(6));	
			string = string.left(2);
			pbShow10->setText(pbShow9->text());
			pbShow9->setText(pbShow8->text());
			pbShow8->setText(pbShow7->text());
			pbShow7->setText((!string.isNull()) ? string:"");
		}

		if(contextWords.size()>=8)
		{
			string = static_cast<QString>(contextWords.at(7));	
			string = string.left(2);
			pbShow10->setText(pbShow9->text());
			pbShow9->setText(pbShow8->text());
			pbShow8->setText((!string.isNull()) ? string:"");
		}

		if(contextWords.size()>=9)
		{
			string = static_cast<QString>(contextWords.at(8));	
			string = string.left(2);
			pbShow10->setText(pbShow9->text());
			pbShow9->setText((!string.isNull()) ? string:"");
		}

		if(contextWords.size()>=10)
		{
			string = static_cast<QString>(contextWords.at(9));	
			string = string.left(2);
			pbShow10->setText((!string.isNull()) ? string:"");
		}
#endif
#else
			for(int i = 3; i>=0;i--)
			{	
				string = static_cast<QString>(contextWords.at(i));	
				string = string.left(2);
				if(!string.isNull())
				{
					ButtonTextList.insert(0,string);
				}
			}
#endif
		//m_ShowTextTimer->stop();
	}
	handInputShow();
}

void handinput::reset()
{
	buttonTextClear();
	if(AutomaticCandidate)
	{
		AutomaticCandidate = false;
		YTInputServerC::instance()->cancelPreeditText();
	}
}

void handinput::HandInputSignalAndSlot()
{
    connect(pbShow1, SIGNAL(clicked()), this, SLOT(on_pbShow1_clicked()));
    connect(pbShow2, SIGNAL(clicked()), this, SLOT(on_pbShow2_clicked()));
    connect(pbShow3, SIGNAL(clicked()), this, SLOT(on_pbShow3_clicked()));
    connect(pbShow4, SIGNAL(clicked()), this, SLOT(on_pbShow4_clicked()));
    connect(pbShow5, SIGNAL(clicked()), this, SLOT(on_pbShow5_clicked()));
    connect(pbShow6, SIGNAL(clicked()), this, SLOT(on_pbShow6_clicked()));
    connect(pbShow7, SIGNAL(clicked()), this, SLOT(on_pbShow7_clicked()));
    connect(pbShow8, SIGNAL(clicked()), this, SLOT(on_pbShow8_clicked()));
    connect(pbShow9, SIGNAL(clicked()), this, SLOT(on_pbShow9_clicked()));
    connect(pbShow10, SIGNAL(clicked()), this, SLOT(on_pbShow10_clicked()));

    connect(pbHide, SIGNAL(clicked()), this, SLOT(on_pbHide_clicked()));
    connect(pbDel, SIGNAL(clicked()), this, SLOT(on_pbDel_clicked()));
    connect(pbDel, SIGNAL(pressed()), this, SLOT(on_pbDel_pressed()));
    connect(pbSpace, SIGNAL(clicked()), this, SLOT(on_pbSpace_clicked()));
    connect(pbChange, SIGNAL(clicked()), this, SLOT(on_pbChange_clicked()));
	
}

void handinput::sendText(QString string)
{
	
	YTInputServerC::instance()->sendCommitText(string);
}
void handinput::handInputShow()
{
	//printf("--------------------->handinput::handInputShow  in");
	QString tmp;
	m_InputWidget->show();

	if(ButtonTextList.size()>0)
	{
		tmp = ButtonTextList.at(0);
		pbShow1->setText((!tmp.isNull()) ? tmp:"");
	}
	if(ButtonTextList.size()>1)
	{
		pbShow2->setText((!ButtonTextList.at(1).isNull()) ? ButtonTextList.at(1):"");				
	}
	if(ButtonTextList.size()>2)
	{
		pbShow3->setText((!ButtonTextList.at(2).isNull()) ? ButtonTextList.at(2):"");
	}
	if(ButtonTextList.size()>3)
	{
		pbShow4->setText((!ButtonTextList.at(3).isNull()) ? ButtonTextList.at(3):"");
	}
	if(ButtonTextList.size()>4)
	{
		pbShow5->setText((!ButtonTextList.at(4).isNull()) ? ButtonTextList.at(4):"");
	}
	if(ButtonTextList.size()>5)
	{
		pbShow6->setText((!ButtonTextList.at(5).isNull()) ? ButtonTextList.at(5):"");
	}
	if(ButtonTextList.size()>6)
	{
		pbShow7->setText((!ButtonTextList.at(6).isNull()) ? ButtonTextList.at(6):"");
	}
	if(ButtonTextList.size()>7)
	{
		pbShow8->setText( (!ButtonTextList.at(7).isNull()) ? ButtonTextList.at(7):"");
	}
	if(ButtonTextList.size()>8)
	{
		pbShow9->setText( (!ButtonTextList.at(8).isNull()) ? ButtonTextList.at(8):"");
	}
	if(ButtonTextList.size()>9)
	{
		pbShow10->setText( (!ButtonTextList.at(9).isNull())?ButtonTextList.at(9):"");		
	}
	
    pbShow1->show();
    pbShow2->show();
    pbShow3->show();
    pbShow4->show();
    pbShow5->show();
    pbShow6->show();
    pbShow7->show();
    pbShow8->show();
    //pbShow9->show();
    //pbShow10->show();
}
void handinput::setUiText(int num, unsigned short getResult[], bool isAssociate)
{
	if( num <0 )
	{		
		if(isAssociate)
		{
			qDebug()<< "error argument or invalid data  in Association!";
		}
		else
		{
			qDebug()<< "error argument or invalid data!";	
		}
	}	
	else if(num =0)
	{
		if(isAssociate)
		{
			qDebug()<< "Do not think the word";
		}
		else
		{
			qDebug()<< "Refused to identify";
		}
	}	
	else
	{	
		QString sResult = QString::fromUtf16(getResult);
		
		QString sResult1 = sResult.left(1);
		QString sResult2 = sResult.mid(1,1);
		QString sResult3 = sResult.mid(2,1);
		QString sResult4 = sResult.mid(3,1);
		QString sResult5 = sResult.mid(4,1);
		QString sResult6 = sResult.mid(5,1);
		QString sResult7 = sResult.mid(6,1);
		QString sResult8 = sResult.mid(7,1);
		QString sResult9 = sResult.mid(8,1);
		QString sResult10 = sResult.mid(9,1);
#if lxf
		pbShow1->setText((!sResult1.isNull()) ? sResult1:"");
		pbShow2->setText((!sResult2.isNull()) ? sResult2:"");				
		pbShow3->setText((!sResult3.isNull()) ? sResult3:"");
		pbShow4->setText((!sResult4.isNull()) ? sResult4:"");
		pbShow5->setText((!sResult5.isNull()) ? sResult5:"");
		pbShow6->setText((!sResult6.isNull()) ? sResult6:"");
		pbShow7->setText((!sResult7.isNull()) ? sResult7:"");
		pbShow8->setText( (!sResult8.isNull()) ? sResult8:"");
		pbShow9->setText( (!sResult9.isNull()) ? sResult9:"");
		pbShow10->setText( (!sResult10.isNull())?sResult10:"");		
#else
		ButtonTextList.clear();
		ButtonTextList.append(sResult1);
		ButtonTextList.append(sResult2);
		ButtonTextList.append(sResult3);
		ButtonTextList.append(sResult4);
		ButtonTextList.append(sResult5);
		ButtonTextList.append(sResult6);
		ButtonTextList.append(sResult7);
		ButtonTextList.append(sResult8);
		ButtonTextList.append(sResult9);
		ButtonTextList.append(sResult10);

		//m_ShowTextTimer->start(100);
#endif
	}
	
	if(WriteReturn)
	{
		WriteReturn = false;
		handInputShow();
	}

}

void handinput::buttonTextClear()
{
	pbShow1->setText("");
	pbShow2->setText("");
	pbShow3->setText("");
	pbShow4->setText("");
	pbShow5->setText("");
	pbShow6->setText("");
	pbShow7->setText("");
	pbShow8->setText("");
	pbShow9->setText("");
	pbShow10->setText("");
}

void handinput::setIssoText(QString btnText)
{
	unsigned short usSuggest[6*1024] ;// 预测结果缓冲区
	unsigned short tmp[10]={0}; //防止出现乱码

	const  unsigned short * mpwszWord =btnText.utf16();
	int iNum=HWRE_PredictText(mpwszWord, NULL, NULL,  usSuggest, 6*1024*2, 0, 0); //添加联想词
	for(int i=0; i<((iNum>=30) ? 30:iNum); i+=3)
	{
		if((usSuggest[i]== *mpwszWord)&&(usSuggest[i+2]==0x000A))
		{
			tmp[i/3]=usSuggest[i+1];
		}
	}
	setUiText(iNum,tmp,true);  
}


void handinput::on_pbShow1_clicked()
{	
	YTInputServerC::instance()->cancelPreeditText();
	AutomaticIsSend = false;
	if( pbShow1->text().length() > 0 )
	{
		sendText(pbShow1->text());
		WriteReturn = false;
		if(isChinese)
			setIssoText(pbShow1->text());
	}
}

void handinput::on_pbShow2_clicked()
{
	YTInputServerC::instance()->cancelPreeditText();
	AutomaticIsSend = false;
	if( pbShow2->text().length() > 0 )
	{
		sendText(pbShow2->text());
		WriteReturn = false;
		if(isChinese)
			setIssoText(pbShow2->text());
	}
}

void handinput::on_pbShow3_clicked()
{
	YTInputServerC::instance()->cancelPreeditText();
	AutomaticIsSend = false;
	if( pbShow3->text().length() > 0 )
	{	
		sendText(pbShow3->text());
		WriteReturn = false;
		if(isChinese)
			setIssoText(pbShow3->text());
	}
}

void handinput::on_pbShow4_clicked()
{
	YTInputServerC::instance()->cancelPreeditText();
	AutomaticIsSend = false;
	if( pbShow4->text().length() > 0 )
	{	
		sendText(pbShow4->text());
		WriteReturn = false;
		if(isChinese)
			setIssoText(pbShow4->text());
	}
}

void handinput::on_pbShow5_clicked()
{
	YTInputServerC::instance()->cancelPreeditText();
	AutomaticIsSend = false;
	if( pbShow5->text().length() > 0 )
	{	
		sendText(pbShow5->text());
		WriteReturn = false;
		if(isChinese)
			setIssoText(pbShow5->text());
	}
}

void handinput::on_pbShow6_clicked()
{
	YTInputServerC::instance()->cancelPreeditText();
	AutomaticIsSend = false;
	if( pbShow6->text().length() > 0 )
	{	
		sendText(pbShow6->text());
		WriteReturn = false;
		if(isChinese)
			setIssoText(pbShow6->text());
	}
}

void handinput::on_pbShow7_clicked()
{
	YTInputServerC::instance()->cancelPreeditText();
	AutomaticIsSend = false;
	if( pbShow7->text().length() > 0 )
	{	
		sendText(pbShow7->text());
		WriteReturn = false;
		if(isChinese)
			setIssoText(pbShow7->text());
	}
}

void handinput::on_pbShow8_clicked()
{
	YTInputServerC::instance()->cancelPreeditText();
	AutomaticIsSend = false;
	if( pbShow8->text().length() > 0 )
	{	
		sendText(pbShow8->text());
		WriteReturn = false;
		if(isChinese)
			setIssoText(pbShow8->text());
	}
}

void handinput::on_pbShow9_clicked()
{
	YTInputServerC::instance()->cancelPreeditText();
	AutomaticIsSend = false;
	if( pbShow9->text().length() > 0 )
	{	
		sendText(pbShow9->text());
		WriteReturn = false;
		if(isChinese)
			setIssoText(pbShow9->text());
	}
}

void handinput::on_pbShow10_clicked()
{
	YTInputServerC::instance()->cancelPreeditText();
	AutomaticIsSend = false;
	if( pbShow10->text().length() > 0 )
	{	
		sendText(pbShow10->text());
		WriteReturn = false;
		if(isChinese)
			setIssoText(pbShow10->text());
	}
}

void handinput::on_pbHide_clicked()
{	
	emit hideInputMethod();
}
void handinput::on_pbSpace_clicked()
{
	if(AutomaticIsSend)
	{
		AutomaticIsSend = false;
		YTInputServerC::instance()->confirmPreeditText();
	}
	else
	{
		YTInputServerC::instance()->sendCommitText(QString(" "));
	}
}
void handinput::on_pbChange_clicked()
{
	YTInputServerC::instance()->showInputMethodSelect();
}
void handinput::on_pbDel_clicked()
{
	if(AutomaticIsSend)
	{
		AutomaticIsSend = false;
		YTInputServerC::instance()->cancelPreeditText();
	}
	else
	{
		YTInputServerC::instance()->sendOneKey(Qt::Key_Backspace,0);
	}

	m_DelTimer->stop();
	DelTimeOutTime = 1;
}
void handinput::on_pbDel_pressed()
{
	m_DelTimer->start(600);
}

void handinput::DelTimer()
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
void handinput::ShowTextTimer()
{
	//printf("\n+ID = +++ -------------------->handinput::ShowTextTimer:  \n");
	m_ShowTextTimer->stop();
	handInputShow();
}

