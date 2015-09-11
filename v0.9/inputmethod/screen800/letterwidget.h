#ifndef  LETTERWIDGET_H
#define LETTERWIDGET_H
#include <QtGui>
#include "InputMethod.h"
#include "InputServer.h"

typedef struct _EnglishButMember
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
}EnglishButMember;

class LetterWidget : public YTInputMethodC
{
    Q_OBJECT

public:
        LetterWidget();
        ~LetterWidget(){}

		void mousePressEvent ( QMouseEvent * event );
		void mouseReleaseEvent ( QMouseEvent * event );
		void mouseMoveEvent(QMouseEvent *event);

    	void setContextWords(QString textBeforeCursor,QStringList contextWords);
    	void reset();
		static YTInputMethodC *factory() { return new LetterWidget(); }
		
		QLabel *BottonPress;
		QLabel *BottonPressLarge;
		QLabel *PressLargeLabelText;

		static void NewLargeLable();
private:

        bool m_bCapital ;
		
		void showPress(QString text,int x,int y,int width,int height,int ID);
		void showPressLarge(QString text,int x,int y,int width,int height);
		QTimer		*m_DelTimer;
		int        DelTimeOutTime;
		int 	lastKeyId;
		
public slots:
        void on_abc_clicked();
		void DelTimer();
} ;



#endif
