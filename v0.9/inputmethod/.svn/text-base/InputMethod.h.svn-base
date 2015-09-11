#ifndef YT_INPUT_METHOD_H_20110801
#define YT_INPUT_METHOD_H_20110801


#include <QWidget>


typedef enum _YTInputMethodCategory
{
	YT_INPUT_METHOD_NONE	=0x0000,
	YT_INPUT_METHOD_DIGITAL	=0x0001,
	YT_INPUT_METHOD_ALPHA	=0x0002,
	YT_INPUT_METHOD_SYMBOL	=0x0004,
	YT_INPUT_METHOD_SPELL	=0x0008,
	YT_INPUT_METHOD_HANDWRITE=0x0010,
	YT_INPUT_METHOD_EDIT	=0x0020,
#ifdef NEW_FUNCTION_VOICE1
	YT_INPUT_METHOD_VOICE	=0x0040,
#endif
	YT_INPUT_METHOD_ALL	=0xFFFF
} YTInputMethodCategory;


class YTInputMethodC: public QWidget
{
	 Q_OBJECT
public:
	YTInputMethodC(QWidget * parent = 0, Qt::WindowFlags f = 0);
	virtual ~YTInputMethodC();

	virtual void setContextWords(QString textBeforeCursor,QStringList contextWords);
	virtual void reset();

protected:
signals:
	void hideInputMethod();
	void hideInputEdit();

public:
	static YTInputMethodC *factory() { return new YTInputMethodC(); }
};


class YTInputMethodSelectC: public QWidget
{
	 Q_OBJECT
public:
	YTInputMethodSelectC(QWidget * parent = 0, Qt::WindowFlags f = 0);
	virtual ~YTInputMethodSelectC();

	virtual void registerInputMethod(YTInputMethodCategory category,QString inputMethodPicName[3])=0;
	virtual void init()=0;
	virtual void setMask(int categories)=0;

public:
	void setAlign(char align);

protected:
signals:
	void changInputMethod(YTInputMethodCategory category);

private:
	char	m_initFlag;
private:
	char	m_align;

	friend class YTInputServerC;
};


#endif
