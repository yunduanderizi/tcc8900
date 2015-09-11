#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QtGui>
#include "InputMethod.h"
#include "InputServer.h"

typedef struct _EditButMember
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
}EditButMember;

class EditWidget : public YTInputMethodC
{
    Q_OBJECT

public:
        EditWidget();
        ~EditWidget(){}

		void mousePressEvent( QMouseEvent * event );
		void mouseMoveEvent( QMouseEvent * event );
		void mouseReleaseEvent( QMouseEvent * event );

    	void setContextWords(QString textBeforeCursor,QStringList contextWords);
    	void reset();
		static YTInputMethodC *factory() { return new EditWidget(); }

		QLabel *BottonPress;
		bool selectStart;
		
private:
		void showPress(QString text,int x,int y,int width,int height,int i);
		QTimer		*m_DelTimer;
		int        DelTimeOutTime;
		
public slots:
		void DelTimer();
		
} ;

#endif
