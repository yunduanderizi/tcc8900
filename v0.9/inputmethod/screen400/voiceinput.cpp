#include "voiceinput.h"
#include <QDebug>
#ifdef NEW_FUNCTION_VOICE1
#include "VoiceFunControl.h"

using namespace Voice1;
#endif

#define keyNumber		5
const VoiceButMember VoiceParameter[keyNumber]=
{
	{134,	36,	104,18,QString("start")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{292,	36,	104,18,QString("stop")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{134,	83, 104,18,QString("del")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{292,	83, 104,18,QString("close")/*,QPixmap(""),QPixmap(""),QPixmap("")*/},
	{17,		120, 104,18,QString("change") /*,QPixmap(""),QPixmap(""),QPixmap("")*/}
};

VoiceInputWidget::VoiceInputWidget()
{	
	QPalette palette;
    	palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/voiceinputbackground.png")));
	

	this->setPalette (palette);

	QPixmap tmp_pixmap = QPixmap(":/images/voiceinputbackground.png");
	this->setGeometry(56, 136, tmp_pixmap.width(), tmp_pixmap.height());
	this->setFocusPolicy(Qt::ClickFocus);	
	
	BottonPress = new QLabel(this);
	BottonPress->hide();

	PromptsLabel = new QLabel(this) ;
	PromptsLabel->setGeometry(56,136,tmp_pixmap.width(),tmp_pixmap.height());
	PromptsLabel->hide();
	
	DelTimeOutTime = 1;
	m_DelTimer = new QTimer(this);

	connect(m_DelTimer, SIGNAL(timeout()), this, SLOT(DelTimer()));
	VoiceFunControlC  *VoiceControl = Voice1::VoiceFunControlC::object();
	if(VoiceControl)
	{
		connect(VoiceControl, SIGNAL(sigAiBsTalkResult(QString)), this, SLOT(GetVoiceInputResult(QString )));
		connect(VoiceControl, SIGNAL(sigAiBsTalkStatusChange(Voice1::Ai_Bs_Talk_Status )), this, SLOT(inputStatusChange(Voice1::Ai_Bs_Talk_Status)));
	}
}         

void VoiceInputWidget::mousePressEvent( QMouseEvent * event )
{
	int i = 0;

	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > VoiceParameter[i].x) && (event->x() < VoiceParameter[i].x + VoiceParameter[i].width) 
			&& (event->y() > VoiceParameter[i].y) && (event->y() < VoiceParameter[i].y + VoiceParameter[i].Vheigth))
		{
			showPress(VoiceParameter[i].text,VoiceParameter[i].x,VoiceParameter[i].y,VoiceParameter[i].width,VoiceParameter[i].Vheigth,i);

			if(VoiceParameter[i].text == tr("del"))
			{
				m_DelTimer->start(600);
			}
			break;
		}  
	}
}
void VoiceInputWidget::mouseMoveEvent( QMouseEvent * event )
{
	int i = 0;

	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > VoiceParameter[i].x) && (event->x() < VoiceParameter[i].x + VoiceParameter[i].width) 
			&& (event->y() > VoiceParameter[i].y) && (event->y() < VoiceParameter[i].y + VoiceParameter[i].Vheigth))
		{
			showPress(VoiceParameter[i].text,VoiceParameter[i].x,VoiceParameter[i].y,VoiceParameter[i].width,VoiceParameter[i].Vheigth,i);

			if(VoiceParameter[i].text == tr("del"))
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
void VoiceInputWidget::mouseReleaseEvent( QMouseEvent * event )
{
	int i = 0;
	VoiceFunControlC  *VoiceControl = Voice1::VoiceFunControlC::getObject();

	BottonPress->hide();
	
	for(i=0;i<keyNumber;i++)
	{
		if((event->x() > VoiceParameter[i].x) && (event->x() < VoiceParameter[i].x + VoiceParameter[i].width) 
			&& (event->y() > VoiceParameter[i].y) && (event->y() < VoiceParameter[i].y + VoiceParameter[i].Vheigth))
		{
			break;
		}
	}
	if((i >= 0) && (i < keyNumber))
	{
		if(VoiceParameter[i].text == tr("start"))
		{
			if(VoiceControl)
			{
			VoiceControl->openAiBsTalk();
		}
			else
			{	
				showPrompts(QString("\uFEFF\u6B63\u5728\u6253\u5F00\u8BED\u97F3\u8F93\u5165"));
			}
		}
		else if(VoiceParameter[i].text == tr("del"))
		{
			m_DelTimer->stop();
			DelTimeOutTime = 1;
			YTInputServerC::instance()->sendOneKey(Qt::Key_Backspace,0);
 		}
		else if(VoiceParameter[i].text == tr("close"))
		{
			emit hideInputEdit();
		}
		else if(VoiceParameter[i].text == tr("stop"))
		{
			VoiceControl->closeAiBsTalk();
		}
		else if(VoiceParameter[i].text == tr("change"))
		{
			YTInputServerC::instance()->showInputMethodSelect();
		}
	}
	
	update();
}

void VoiceInputWidget::showPress(QString text,int x,int y,int width,int height,int i)
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
void VoiceInputWidget::showPrompts(QString text)
{
	BottonPress->setText(text);
	BottonPress->show();
}
void VoiceInputWidget::setContextWords(QString textBeforeCursor,QStringList contextWords)
{

}
void VoiceInputWidget::reset()
{
	
}

void VoiceInputWidget::DelTimer()
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

void VoiceInputWidget::GetVoiceInputResult(QString resultText)
{
	YTInputServerC::instance()->sendCommitText(resultText);
}

void VoiceInputWidget::inputStatusChange(Voice1::Ai_Bs_Talk_Status status)
{
	switch(status)
	{
		case ABT_STATUS_START:
		{
			PromptsLabel->hide();
			break;
		}

		case ABT_STATUS_STOP:
		{
			//showPrompts(QString(""));
			break;
		}
	}
}

