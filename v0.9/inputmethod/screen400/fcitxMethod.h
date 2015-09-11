#ifndef FCITXMETHOD_H
#define FCITXMETHOD_H

#define INPUTMETHOD_INPUTANYWHERE //QLineEdit,QTextEdit,QPlainTextEdit, or it's subclass
/*
  To enable the INPUTMETHOD_SUPPORT:
  1. goto sw_c2apps/3rdParty/fcitxIMEngine(if not exist, checkout it), type make; make install; cd tools; make;cd data;make
  2. uncomment the above macro
  3. uncomment the line #LIBS += -lfcitxIMEngine in filemanager.pro
  4. make
  run on the target:
  1. scp QtopiaCore-4.1.4-generic/lib/libfcitxIMEngine.so.1 to root@<yourtarget>:/home/work/Trolltech/QtopiaCore-4.1.4-generic/lib
  2. mkdir /home/work/qtcim on target
  3. scp sw_c2apps/3rdParty/fcitxIMEngine/data/py*.mb root@<yourtarget>:/home/work/qtcim/
  4. run.sh
*/
#include <QWidget>
#include <QLineEdit>
#include <QString>
#include <QPushButton>
#include <QGraphicsWidget>
#include <fcitxIMEngine.h>
#include "pinyininputmethod.h"

#define QlistTest 0

class PinyinInput;

#ifndef WIN32
class FcitxInputMethod : public FcitxIMEngine// , public QWSInputMethod
{
public:
	FcitxInputMethod();
	~FcitxInputMethod();
	
	void setDownlist(QStringList *list);
	bool process_key_event (const IMKeyEvent& key);
	void Reset();
	void ChangeIMState(IME_STATE s);
	int  upMsgLen();
	bool commitStrinNull;
	PinyinInput *target;
	QStringList *Downlist;
	
protected:
	virtual void reset(); //QWSInputMethod
	virtual void updateHandler(int type);
	virtual bool filter(int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat);

	virtual void send_string(char* str);
	virtual void DisplayInputWindow();
	virtual void ResetInputWindow();
	private:
		
	bool isInputWidget;
};
#endif


#endif
