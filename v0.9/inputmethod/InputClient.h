#ifndef YT_INPUT_CLIENT_H_20110802 
#define YT_INPUT_CLIENT_H_20110802

#include <QInputContext>

#include "InputMethod.h"

class YTInputClientDataC;

class YTIP_changeInputMethodEventC;
class YTIP_hideInputMethodEventC;

class YTInputClientC: public QInputContext
{
    Q_OBJECT
public:
    explicit YTInputClientC(QObject* parent = 0);
    ~YTInputClientC() {}


    QString identifierName() { return QString(); }
    QString language() { return QString(); }

    void reset();
    void update();
    void mouseHandler( int x, QMouseEvent *event);

    void setFocusWidget( QWidget *w );
    void widgetDestroyed(QWidget *w);

    bool isComposing() const;

public:
	void setCurrentInputMethod(YTInputMethodCategory category);
	void setInputMask(int mask);

public:
	static void installEditWidget(QWidget *w);

private:
	void changInputMethod(YTIP_changeInputMethodEventC *event);
	void hideInputMethod(YTIP_hideInputMethodEventC *event);

	bool filterEvent(const QEvent* event);

private:
	void adjustGeometry();

private:
	YTInputClientDataC  *m_data;

	friend class YTInputClientDataC;
};

#endif 
