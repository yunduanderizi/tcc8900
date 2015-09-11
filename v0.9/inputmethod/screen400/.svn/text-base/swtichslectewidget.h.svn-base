#ifndef  HANDINPUTWIDGET_H
#define HANDINPUTWIDGET_H

#include <QtGui>
#include "InputMethod.h"

class TranslucentWidget ;

class ButtonChange : public QPushButton
{
	Q_OBJECT

public:
	ButtonChange(YTInputMethodCategory id,QWidget *parent = 0);
	~ButtonChange(){};
	void setPixmap(QString sIconNameBase, QString sIconNamePushed);    
protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent ( QMouseEvent * event );
	void mouseReleaseEvent ( QMouseEvent * event );

signals:
        void clicked(YTInputMethodCategory);
private:
	QPixmap m_Icon;
	QString m_sIconBase;
	QString m_sIconPushed;
	
	YTInputMethodCategory M_Id;
};
typedef QList<ButtonChange*> KEYLIST ;

class SwitchWidget : public YTInputMethodSelectC
{
    Q_OBJECT

public:
        SwitchWidget();
        ~SwitchWidget(){};

		void registerInputMethod(YTInputMethodCategory category,QString inputMethodPicName[3]);
		void init();	
		void setMask(int categories);
		
private:
		ButtonChange *PB_Hand;
		ButtonChange *PB_English;
		ButtonChange *PB_Pinyin;
		ButtonChange *PB_Symbol;
		ButtonChange *PB_Digital;
		ButtonChange *PB_Edit;
#ifdef NEW_FUNCTION_VOICE1
		ButtonChange *PB_Voice;
#endif		
		int categoriesMask;
		ButtonChange *hideButton;
		void setButDisable();

public slots:
        void on_item_clicked(YTInputMethodCategory);
};


#endif
