#include <QPainter>
#include <QDebug>
#include <QTextCodec>
#include <QRegExp>
#include "fcitxMethod.h"

#ifndef WIN32
#include <QWSInputMethod>
#endif

#ifndef WIN32
FcitxInputMethod::FcitxInputMethod()
{
	Downlist = NULL;
	target = NULL;
    imeState = IS_ENG;
    isInputWidget = false;
}

FcitxInputMethod::~FcitxInputMethod()
{
}

void FcitxInputMethod::setDownlist(QStringList *list)
{
    Downlist= list;
}

void FcitxInputMethod::send_string(char* str)
{
    //FcitxIMEngine::send_string(str);
    
    if(Downlist)
	{
		QString txt; 
		QTextCodec *codec = QTextCodec::codecForName("GB2312");
		if(codec)
		{
		    txt = codec->toUnicode(str);
		}
	}
}

void FcitxInputMethod::DisplayInputWindow()
{
	
    FcitxIMEngine::DisplayInputWindow();
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    if(Downlist)
    {
        QString txt;
		QStringList temlist;
        if(codec)
        {
            txt = codec->toUnicode(m_downStr);
        }
		temlist = txt.split(QRegExp("[1-9]."),QString::SkipEmptyParts); 
		Downlist->clear();
		*Downlist = Downlist->operator+(temlist);
    }
    if(target)
    {
    	target->setIsso = false;
    	target->SetAssociationBottonText(target->downlist);
		
        QString txt;
        if(codec)
        {
            txt = codec->toUnicode(m_upStr);
			target->keyvalue = txt;
        }
    }
}

void FcitxInputMethod::ResetInputWindow()
{
    FcitxIMEngine::ResetInputWindow();
    if(target)
    {
        target->ClearAssociationBottonText(); 
    }
	if(Downlist)
	{
		Downlist->clear();
	}
	
}

bool  FcitxInputMethod::filter(int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat)
{
    return false;
}

void FcitxInputMethod::reset()
{
    FcitxIMEngine::reset();

}

enum UpdateType {Update, FocusIn, FocusOut, Reset, Destroyed};
void FcitxInputMethod::updateHandler(int type)
{
    if(type == FocusIn)
    {
        QObject *obj = qobject_cast<QObject *>(QApplication::focusWidget());
        if(obj)
        {
            //if you implement your own inputwidget, add it here.
            if(obj->inherits("QLineEdit") || obj->inherits("QTextEdit") || obj->inherits("QPlainTextEdit") )
            {
                if(obj->objectName() != "ignoreit")
                isInputWidget = true;
            }
        }
    }
    else if(type >= FocusOut)
    {
        isInputWidget = false;
    }
}

bool FcitxInputMethod::process_key_event (const IMKeyEvent& key)
{
    int ret = 0;
    if( (key.get_ascii_code() >= 'a' && key.get_ascii_code() <= 'z')
        || (key.get_ascii_code() >= '0' && key.get_ascii_code() <= '9' && uMessageUp > 0)
        || ((key.get_ascii_code() == ',') || (key.get_ascii_code() == '.') && uMessageUp > 0)
        || (key.code == SCIM_KEY_space) || (key.code == SCIM_KEY_Delete) || (key.code == SCIM_KEY_BackSpace)
    )
    {
        ret = FcitxIMEngine::process_key_event(key); 
    }
	
    return ret;
}

int FcitxInputMethod::upMsgLen()
{
    return uMessageUp;
}

void FcitxInputMethod::ChangeIMState(IME_STATE s)
{
    if(uMessageUp == 0)
    {
    
    }
    FcitxIMEngine::ChangeIMState(s);
}

void FcitxInputMethod::Reset()
{
    if(target)
    {
        target->ClearAssociationBottonText(); 
    }
	if(Downlist)
	{
		Downlist->clear();
	}
    FcitxIMEngine::reset();

	FcitxIMEngine::ResetInputWindow();
}
#endif



