#ifndef YT_INPUT_SERVER_H_20110802
#define YT_INPUT_SERVER_H_20110802

#include <QWidget>
#include <QWSWindow>


#include "InputMethod.h"

typedef YTInputMethodC *(*YTInputMethodFactory)();

class  YTInputServerDataC;

class YTIP_showInputMethodC;
class YTIP_changeCursorPositionC;
class YTIP_setContextWordsC;
class YTIP_hideInputMethodC;

class YTInputServerC: public QObject
{
	Q_OBJECT
private:
	YTInputServerC();
	~YTInputServerC();

	static YTInputServerC *thisObject;

public:
	void	registerInputMethodSelect(YTInputMethodSelectC *inputMethodSelect);
	void 	registerInputMethod(YTInputMethodCategory category,YTInputMethodFactory factory,const QString picFile[3]);
	void	setMaskPic(const QString& picFile);
	

public:
	static YTInputServerC *instance();

	void 	showInputMethodSelect();

public:
	void sendPreeditText(const QString &preeditText);
	void cancelPreeditText();
	void confirmPreeditText();

    	void sendCommitText(const QString &commitText);

	void sendOneKey(int keycode,Qt::KeyboardModifiers modifiers,int unicode=0xFFFF);
	void sendKey(int keycode,Qt::KeyboardModifiers modifiers,
                             bool isPress=TRUE,int unicode=0xFFFF);

public slots:
	void changInputMethod(YTInputMethodCategory category);	

private slots:
	void mouseFiltered();	
	void hideInputMethod();
	void hideInputEdit();

	void 	windowEvent(QWSWindow * window, QWSServer::WindowEvent eventType);

private:
	void 	timerEvent(QTimerEvent * event);
	void	showInputMethod(YTIP_showInputMethodC *cmd);
	void 	changeCursor(YTIP_changeCursorPositionC *cmd);
	void 	setContextWords(YTIP_setContextWordsC  *cmd);
	void	hideInputMethod(YTIP_hideInputMethodC *cmd);

private:
	void	showMaskWidget(QRect origin,int newY);
	void  	hideMaskWidget();

	void	showMaskMethod(int y=0);
	void  	hideMaskMethod();

	void  	hideInputMethod_internal(int notifyClient);
	void 	updateWidgetGeometry();

private:
	int 	m_timerId;

	YTInputMethodSelectC	*m_inputMethodSelect;
	YTInputServerDataC	*m_data;

	QString			m_maskPicFile;
	QWidget			*m_maskWidget;
	QWidget			*m_maskMethod;

	friend class YTInputServerDataC;
};


#endif
