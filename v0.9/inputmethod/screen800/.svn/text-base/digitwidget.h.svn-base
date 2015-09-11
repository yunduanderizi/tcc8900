#ifndef DIGITWIDGET_H
#define DIGITWIDGET_H

#include <QtGui>
#include "InputMethod.h"
#include "InputServer.h"

typedef struct _DigitalButMember
{
	int			x;
	int			y;
	int			width;
	int			Vheigth;
	QString 	text;
#if 0
	QPixmap		pressed;
	QPixmap		release;
	QPixmap		large;

#endif
}DigitalButMember;


class DigitWidget : public YTInputMethodC
{
    Q_OBJECT

public:
        DigitWidget();
        ~DigitWidget(){}

		void mousePressEvent( QMouseEvent * event );
		void mouseMoveEvent( QMouseEvent * event );
		void mouseReleaseEvent( QMouseEvent * event );

    	void setContextWords(QString textBeforeCursor,QStringList contextWords);
    	void reset();
		static YTInputMethodC *factory() { return new DigitWidget(); }

		QLabel *BottonPress;
		
private:
	
		void showPress(QString text,int x,int y,int width,int height);		

		QTimer		*m_DelTimer;
		int        DelTimeOutTime;
		
		
public slots:
		void DelTimer();
} ;



#endif
