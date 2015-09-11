#include "editwidget.h"
#include <QDebug>
 
#define keyNumber		11

const EditButMember EditParameter[keyNumber]=
{
	{124,	61,	53,42,QString("left")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{178,	61,	53,42,QString("right")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{16,	61,	53,42,QString("up")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{70,	61,	53,42,QString("down")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{70,	16,	53,42,QString("cut")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{124,	16,	53,42,QString("copy")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
		
	{178,	16,	53,42,QString("select")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
//	{178,	16,	53,42,QString("cancel")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{16,	16,	53,42,QString("paste")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	
	{232,	16,	60,42,QString("del")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{232,	61,	121,42,QString("space")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{293,	16,	60,42,QString("close")	/*,QPixmap(""),QPixmap(""),QPixmap("")*/}
};

EditWidget::EditWidget()
{	
	QPalette palette;
	selectStart = false;
	if(!selectStart)
	{
    	palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/editbackground.png")));
	}
	else
	{
    	palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/editbackgroundend.png")));
	}
	this->setPalette (palette);

	QPixmap tmp_pixmap = QPixmap(":/images/editbackground.png");
	this->setGeometry(56, 136, tmp_pixmap.width(), tmp_pixmap.height());
	this->setFocusPolicy(Qt::ClickFocus);	
	
    BottonPress = new QLabel(this);
	BottonPress->hide();

	DelTimeOutTime = 1;
	m_DelTimer = new QTimer(this);

	connect(m_DelTimer, SIGNAL(timeout()), this, SLOT(DelTimer()));
	
}         

void EditWidget::mousePressEvent( QMouseEvent * event )
{
	int i = 0;

	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > EditParameter[i].x) && (event->x() < EditParameter[i].x + EditParameter[i].width) 
			&& (event->y() > EditParameter[i].y) && (event->y() < EditParameter[i].y + EditParameter[i].Vheigth))
		{
			showPress(EditParameter[i].text,EditParameter[i].x,EditParameter[i].y,EditParameter[i].width,EditParameter[i].Vheigth,i);

			if(EditParameter[i].text == tr("del"))
			{
				m_DelTimer->start(600);
			}
			break;
		}  
	}
}
void EditWidget::mouseMoveEvent( QMouseEvent * event )
{
	int i = 0;

	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > EditParameter[i].x) && (event->x() < EditParameter[i].x + EditParameter[i].width) 
			&& (event->y() > EditParameter[i].y) && (event->y() < EditParameter[i].y + EditParameter[i].Vheigth))
		{
			showPress(EditParameter[i].text,EditParameter[i].x,EditParameter[i].y,EditParameter[i].width,EditParameter[i].Vheigth,i);

			if(EditParameter[i].text == tr("del"))
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
void EditWidget::mouseReleaseEvent( QMouseEvent * event )
{
	int i = 0;

	BottonPress->hide();
	
	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > EditParameter[i].x) && (event->x() < EditParameter[i].x + EditParameter[i].width) 
			&& (event->y() > EditParameter[i].y) && (event->y() < EditParameter[i].y + EditParameter[i].Vheigth))
		{
			break;
		}
	}
	if((i >= 0) && (i < keyNumber))
	{
		if(EditParameter[i].text == tr("space"))
		{
			YTInputServerC::instance()->sendCommitText(QString(" "));
		}
		else if(EditParameter[i].text == tr("del"))
		{
			m_DelTimer->stop();
			DelTimeOutTime = 1;
			YTInputServerC::instance()->sendOneKey(Qt::Key_Backspace,0);
 		}
		else if(EditParameter[i].text == tr("close"))
		{
			emit hideInputEdit();
		}
		else if(EditParameter[i].text == tr("left"))
		{
			if(selectStart)
			{	
				YTInputServerC::instance()->sendOneKey(Qt::Key_Left,Qt::ShiftModifier);
			}
			else
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_Left,0);
			}
		}
		else if(EditParameter[i].text == tr("right"))
		{
			if(selectStart)
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_Right,Qt::ShiftModifier);
			}
			else
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_Right,0);
			}
		}
		else if(EditParameter[i].text == tr("up"))
		{
			if(selectStart)
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_Up,Qt::ShiftModifier);
			}
			else
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_Up,0);
			}
		}
		else if(EditParameter[i].text == tr("down"))
		{
			if(selectStart)
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_Down,Qt::ShiftModifier);
			}
			else
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_Down,0);
			}
		}
		else if(EditParameter[i].text == tr("cut"))
		{
			YTInputServerC::instance()->sendOneKey(Qt::Key_unknown,Qt::ControlModifier);
			YTInputServerC::instance()->sendOneKey(Qt::Key_X,Qt::ControlModifier);
			
			YTInputServerC::instance()->sendOneKey(Qt::Key_unknown,0);
			selectStart = false;
			QPalette palette;
			if(!selectStart)
			{
				palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/editbackground.png")));
				this->setPalette (palette);
			}
			else
			{
				palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/editbackgroundend.png")));
				this->setPalette (palette);
			}
		}
		else if(EditParameter[i].text == tr("copy"))
		{
			YTInputServerC::instance()->sendOneKey(Qt::Key_unknown,Qt::ControlModifier);
			YTInputServerC::instance()->sendOneKey(Qt::Key_C,Qt::ControlModifier);

			YTInputServerC::instance()->sendOneKey(Qt::Key_unknown,0);
			selectStart = false;
			QPalette palette;
			if(!selectStart)
			{
				palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/editbackground.png")));
				this->setPalette (palette);
			}
			else
			{
				palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/editbackgroundend.png")));
				this->setPalette (palette);
			}
			
		}
		else if(EditParameter[i].text == tr("select"))
		{
			QPalette palette;
			selectStart = !selectStart;
			if(!selectStart)
			{
				palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/editbackground.png")));
				this->setPalette (palette);
			}
			else
			{
				palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/editbackgroundend.png")));
				this->setPalette (palette);
			}
			

			if(!selectStart)
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_unknown,0);
				//YTInputServerC::instance()->sendOneKey(Qt::Key_End,0);
			}
			else
			{
				YTInputServerC::instance()->sendOneKey(Qt::Key_unknown,Qt::ShiftModifier);
			}
		}
		else if(EditParameter[i].text == tr("cancel"))
		{
			YTInputServerC::instance()->sendOneKey(Qt::Key_Cancel,0);
		}
		else if(EditParameter[i].text == tr("paste")) 
		{
			YTInputServerC::instance()->sendOneKey(Qt::Key_unknown,Qt::ControlModifier);
			YTInputServerC::instance()->sendOneKey(Qt::Key_V,Qt::ControlModifier);

			YTInputServerC::instance()->sendOneKey(Qt::Key_unknown,0);
			selectStart = false;
			QPalette palette;
			if(!selectStart)
			{
				palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/editbackground.png")));
				this->setPalette (palette);
			}
			else
			{
				palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/editbackgroundend.png")));
				this->setPalette (palette);
			}
		}
	}
	
	update();
}

void EditWidget::showPress(QString text,int x,int y,int width,int height,int i)
{
	if(i<8)
	{
		BottonPress->setPixmap(QPixmap(":/images/editpresspix.png"));
	}
	else if(i==9)
	{
		BottonPress->setPixmap(QPixmap(":/images/editspacepre.png"));
	}
	else
	{
		BottonPress->setPixmap(QPixmap(":/images/editotherpre.png"));
	}
	BottonPress->setGeometry(x,y,width,height);
	
	BottonPress->show();	
}

void EditWidget::setContextWords(QString textBeforeCursor,QStringList contextWords)
{

}
void EditWidget::reset()
{
	selectStart = false;

}

void EditWidget::DelTimer()
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
