#ifndef __PININPUTMETHOD_H_
#define __PININPUTMETHOD_H_

#include <QtGui>
#include <unistd.h>
#include <QString>
#include "fcitxMethod.h"
#include "InputMethod.h"
#include "InputServer.h"

class FcitxInputMethod;

#if 1
class ButtonPinyin : public QPushButton
{
	Q_OBJECT

public:
	ButtonPinyin(QWidget *parent = 0);
	~ButtonPinyin(){};
	void setPixmap(QString sIconNameBase, QString sIconNamePushed);    
	
protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent ( QResizeEvent * event );    
	void keyPressEvent ( QKeyEvent * event );
	void mousePressEvent ( QMouseEvent * event );
	void mouseReleaseEvent ( QMouseEvent * event );
	
private:
	QPixmap m_Icon;
	QString m_sIconBase;
	QString m_sIconPushed;
};
#endif

typedef struct _LetterButMember
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
}LetterButMember;

class pageMember
{
public:	
	int			pagenum;
	int			startitem;
	int			enditem;
};


typedef QList<pageMember> PAGELIST ;

class PinyinInput: public YTInputMethodC
{
	Q_OBJECT
		
public:
	PinyinInput(QWidget *parent = 0);
	~PinyinInput();

	FcitxInputMethod *PinyinImagine;
	QStringList downlist;
	QString keyvalue;
	bool IsPage;
	bool setIsso;
	int currentpageNum;

	PAGELIST pageButtonlist;
	
	void SetAssociationBottonText(QStringList showlist);
	void showAssociationBotton();
	void ClearAssociationBottonText();
	void keyPressEvent(QKeyEvent * event);

    void setContextWords(QString textBeforeCursor,QStringList contextWords);
    void reset();
	static YTInputMethodC *factory() { return new PinyinInput(); }
	static void NewLargeLable();
	static void NewPINLable();
	
protected:
	void mousePressEvent ( QMouseEvent * event );
	void mouseReleaseEvent ( QMouseEvent * event );
	void mouseMoveEvent(QMouseEvent *event);
	
private:
    ButtonPinyin *AssociationBotton1;
    ButtonPinyin *AssociationBotton2;
    ButtonPinyin *AssociationBotton3;
    ButtonPinyin *AssociationBotton4;
    ButtonPinyin *AssociationBotton5;
    ButtonPinyin *AssociationBotton6;
    ButtonPinyin *AssociationBotton7;
    ButtonPinyin *AssociationBotton8;
	
	QLabel *PressLabel;
	QLabel *PressLargeLabel;
	QLabel *PressLargeLabelText;
	QLabel *ShowPinyinLabel;
	QLabel *ShowPinyinLabelText;
		
	int backgroundX;
	int backgroundY;
	int backgroundWidth;
	int backgroundHeight;
	int lastKeyId;

	QString pinyin;
	QStringList AssociationText;

	int AssociationBottonX;
	int AssociationBottonY;
	void showPress(QString text,int x,int y,int width,int height,int id);
	void showPressLarge(QString text,int x,int y,int width,int height);
	void showpinyinlabel();

	QTimer		*m_DelTimer;
	int        DelTimeOutTime;
		
public slots:
	void DelTimer();
	
private slots:
	void on_AssociationBotton1_clicked();
	void on_AssociationBotton2_clicked();
	void on_AssociationBotton3_clicked();
	void on_AssociationBotton4_clicked();
	void on_AssociationBotton5_clicked();
	void on_AssociationBotton6_clicked();
	void on_AssociationBotton7_clicked();
	void on_AssociationBotton8_clicked();
};

#endif

