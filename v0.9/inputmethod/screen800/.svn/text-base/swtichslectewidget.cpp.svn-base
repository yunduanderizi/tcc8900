#include <QApplication>
#include <QKeyEvent>
#include "swtichslectewidget.h"

ButtonChange::ButtonChange(YTInputMethodCategory id,QWidget *parent) : QPushButton(parent)
{
	M_Id = id;
}
void ButtonChange::setPixmap(QString sIconNameBase, QString sIconNamePushed)
{
	m_sIconBase		= sIconNameBase;
	m_sIconPushed	= sIconNamePushed;

	m_Icon = QPixmap(m_sIconBase);
       
}

void ButtonChange::paintEvent(QPaintEvent *)
{
	QPainter p (this);
	QStyleOptionButton option;
	option.initFrom (this);
	p.drawPixmap(QRect(0,0,this->width(),this->height()),m_Icon);
	p.setPen (this->palette().color (QPalette::ButtonText) );
	p.drawText (QRect(0,0,this->width(),this->height()), Qt::AlignHCenter, this->text() );

}

void ButtonChange::mousePressEvent ( QMouseEvent * event )
{
	m_Icon = QPixmap(m_sIconPushed);
	QPushButton::mousePressEvent( event );
	
}

void ButtonChange::mouseReleaseEvent ( QMouseEvent * event )
{
	m_Icon = QPixmap(m_sIconBase);
	update();
	QPushButton::mouseReleaseEvent( event );

    emit clicked(M_Id);
}

SwitchWidget::SwitchWidget()
:YTInputMethodSelectC(0,Qt::FramelessWindowHint)
{
	categoriesMask = YT_INPUT_METHOD_ALL;
	hideButton = NULL;

}

void SwitchWidget::registerInputMethod(YTInputMethodCategory category,QString inputMethodPicName[3])
{
	
}
void SwitchWidget::init()
{
	
	QPixmap tmp_pixmap ,bk_pixmap_press;
	QString tmp_base,tmp_press;

	setWindowTitle("Bai Wenyan");
	setFocusPolicy(Qt::NoFocus);
		
	this->setAutoFillBackground(true);
	
	QPalette palette;
	palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/translucentbackground.png")));
	this->setPalette (palette);


	tmp_pixmap = QPixmap(":/images/translucentbackground.png");
	
	this->setGeometry(0,236,tmp_pixmap.width(),tmp_pixmap.height());

	ButtonChange  *temp_button = NULL;

	bk_pixmap_press = QPixmap(":/images/switchbutP.png");

	tmp_base = QString(":/images/switchbuthand.png");
	tmp_press = QString(":/images/switchbutP.png");

	PB_Hand = new ButtonChange(YT_INPUT_METHOD_HANDWRITE , this);     
	PB_Hand->setPixmap(tmp_base,tmp_press);
	PB_Hand->setGeometry(76,25,bk_pixmap_press.width(),bk_pixmap_press.height());
	connect(PB_Hand,SIGNAL(clicked(YTInputMethodCategory)),this,SLOT(on_item_clicked(YTInputMethodCategory)));

	tmp_base = QString(":/images/switchbutenglish.png");
	PB_English = new ButtonChange(YT_INPUT_METHOD_ALPHA , this);
	PB_English->setPixmap(tmp_base,tmp_press);
	PB_English->setGeometry(181,25,bk_pixmap_press.width(),bk_pixmap_press.height());
	connect(PB_English,SIGNAL(clicked(YTInputMethodCategory)),this,SLOT(on_item_clicked(YTInputMethodCategory)));

	tmp_base = QString(":/images/switchbutpinyin.png");
	PB_Pinyin = new ButtonChange(YT_INPUT_METHOD_SPELL , this);
	PB_Pinyin->setPixmap(tmp_base,tmp_press);
	PB_Pinyin->setGeometry(76,95,bk_pixmap_press.width(),bk_pixmap_press.height());
	connect(PB_Pinyin,SIGNAL(clicked(YTInputMethodCategory)),this,SLOT(on_item_clicked(YTInputMethodCategory)));

	tmp_base = QString(":/images/switchbutsymbol.png");
	PB_Symbol = new ButtonChange(YT_INPUT_METHOD_SYMBOL , this);
	PB_Symbol->setPixmap(tmp_base,tmp_press);
	PB_Symbol->setGeometry(286,25,bk_pixmap_press.width(),bk_pixmap_press.height());
	connect(PB_Symbol,SIGNAL(clicked(YTInputMethodCategory)),this,SLOT(on_item_clicked(YTInputMethodCategory)));

	tmp_base = QString(":/images/switchbutdigital.png");
	PB_Digital = new ButtonChange(YT_INPUT_METHOD_DIGITAL , this);
	PB_Digital->setPixmap(tmp_base,tmp_press);
	PB_Digital->setGeometry(181,95,bk_pixmap_press.width(),bk_pixmap_press.height());
	connect(PB_Digital,SIGNAL(clicked(YTInputMethodCategory)),this,SLOT(on_item_clicked(YTInputMethodCategory)));

	tmp_base = QString(":/images/switchbutedit.png");
	PB_Edit = new ButtonChange(YT_INPUT_METHOD_EDIT, this);
	PB_Edit->setPixmap(tmp_base,tmp_press);
	PB_Edit->setGeometry(286,95,bk_pixmap_press.width(),bk_pixmap_press.height());
	connect(PB_Edit,SIGNAL(clicked(YTInputMethodCategory)),this,SLOT(on_item_clicked(YTInputMethodCategory)));
	
#ifdef NEW_FUNCTION_VOICE1
	tmp_base = QString(":/images/switchbutedit.png");
	PB_Voice = new ButtonChange(YT_INPUT_METHOD_VOICE, this);
	PB_Voice->setPixmap(tmp_base,tmp_press);
	PB_Voice->setGeometry(286,95,bk_pixmap_press.width(),bk_pixmap_press.height());
	connect(PB_Voice,SIGNAL(clicked(YTInputMethodCategory)),this,SLOT(on_item_clicked(YTInputMethodCategory)));
#endif

	tmp_base = QString("");
	hideButton = new ButtonChange(YT_INPUT_METHOD_NONE , this);
	hideButton->setPixmap(tmp_base,tmp_press);
	hideButton->setGeometry(2,175,bk_pixmap_press.width(),bk_pixmap_press.height());

	connect(hideButton,SIGNAL(clicked(YTInputMethodCategory)),this,SLOT(on_item_clicked(YTInputMethodCategory)));

	setButDisable();
}
void SwitchWidget::setMask(int categories)
{
	categoriesMask = categories;
	setButDisable();
}

void SwitchWidget::on_item_clicked(YTInputMethodCategory id)
{
		
	emit changInputMethod(id);
	
	this->hide();
}

void SwitchWidget::setButDisable()
{
	
	if(categoriesMask & YT_INPUT_METHOD_DIGITAL)
	{
        PB_Digital->show();
	}
	else
	{
        PB_Digital->hide();
	}

	if(categoriesMask & YT_INPUT_METHOD_ALPHA)
	{
        PB_English->show();
	}
	else
	{
        PB_English->hide();
	}

	if(categoriesMask & YT_INPUT_METHOD_SYMBOL)
	{
		PB_Symbol->show();
	}
	else
	{
		PB_Symbol->hide();
	}

	if(categoriesMask & YT_INPUT_METHOD_SPELL)
	{
		PB_Pinyin->show();
	}
	else
	{
		PB_Pinyin->hide();
	}

	if(categoriesMask & YT_INPUT_METHOD_HANDWRITE)
	{
		PB_Hand->show();
	}
	else
	{
		PB_Hand->hide();
	}

	if(categoriesMask & YT_INPUT_METHOD_EDIT)
	{
		PB_Edit->show();
	}
	else
	{
		PB_Edit->hide();
	}
#ifdef NEW_FUNCTION_VOICE1
	if(categoriesMask & YT_INPUT_METHOD_VOICE)
	{
		PB_Voice->show();
	}
	else
	{
		PB_Voice->hide();
	}
#endif
}

