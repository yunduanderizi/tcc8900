#ifndef VOICEINPUTWIDGET_H
#define VOICEINPUTWIDGET_H

#include <QtGui>
#include "ytInputMethod.h"
#include "ytInputServer.h"
#ifdef NEW_FUNCTION_VOICE1
#include "VoiceFunControl.h"

using namespace Voice1;
#endif

typedef struct _VoiceButMember
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
}VoiceButMember;

class VoiceInputWidget : public YTInputMethodC
{
    Q_OBJECT

public:
	VoiceInputWidget();
	~VoiceInputWidget(){}

	void mousePressEvent( QMouseEvent * event );
	void mouseMoveEvent( QMouseEvent * event );
	void mouseReleaseEvent( QMouseEvent * event );

	void setContextWords(QString textBeforeCursor,QStringList contextWords);
	void reset();
	static YTInputMethodC *factory() { return new VoiceInputWidget(); }

	QLabel *BottonPress;
		
private:
	void showPress(QString text,int x,int y,int width,int height,int i);
	void showPrompts(QString text);
	QTimer		*m_DelTimer;
	int        DelTimeOutTime;
	QLabel  *PromptsLabel;
		
public slots:
	void inputStatusChange(Voice1::Ai_Bs_Talk_Status status);
	void DelTimer();
	void GetVoiceInputResult(QString resultText);
		
} ;

#endif
