#ifndef  CHARACTERWIDGET_H
#define CHARACTERWIDGET_H
#include <QtGui>
#include "InputMethod.h"
#include "InputServer.h"

typedef struct _SymbolButMember
{
	int			x;
	int			y;
	int			width;
	int			Vheigth;
	char	 	*text;
#if 0
	QPixmap		pressed;
	QPixmap		release;
	QPixmap		large;

#endif
}SymbolButMember;

class CharacterWidget : public YTInputMethodC
{
    Q_OBJECT

public:
        CharacterWidget();
        ~CharacterWidget(){}

		void mousePressEvent( QMouseEvent * event );
		void mouseMoveEvent( QMouseEvent * event );
		void mouseReleaseEvent( QMouseEvent * event );

    	void setContextWords(QString textBeforeCursor,QStringList contextWords);
    	void reset();
		static YTInputMethodC *factory() { return new CharacterWidget(); }

		QLabel *BottonPress;
		QLabel *BottonPressLarge;
		QLabel *PressLargeLabelText;
		static void NewLargeLable();
private:
        bool m_bSecond;

		void showPress(QString text,int x,int y,int width,int height,int ID);
		void showPressLarge(QString text,int x,int y,int width,int height);
		QTimer		*m_DelTimer;
		int        DelTimeOutTime;
		int 	lastKeyId;
		
public slots:
		void DelTimer();
        void on_second_clicked();
} ;



#endif
