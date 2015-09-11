#ifndef HANDINPUT_H
#define HANDINPUT_H

#include <QtGui>
#include <QPushButton>
#include <QMouseEvent>
#include <QTimer>
#include <QPainter>
#include <stdio.h>
#include <QLabel>
#include "HWAPI.h"
#include "InputMethod.h"
#include "InputServer.h"

class handinput;

class HandInputWidget : public QWidget
{
	Q_OBJECT

public:
	HandInputWidget(QWidget *parent = 0);
	~HandInputWidget();

protected:
	void paintEvent(QPaintEvent *);    
	void mousePressEvent(QMouseEvent *mouseEvent);
	void mouseMoveEvent(QMouseEvent *mouseEvent);
	void mouseReleaseEvent(QMouseEvent *mouseEvent);

private slots:
	void RefreshInfo();
	
public:
	handinput 	*m_pHandInput;
	QList <THWPoint > m_arrPoints;
	QTimer		*m_RefreshTimer;
	int 		m_nStopTime;

	QList <THWPoint > m_newPoints;
	int ux,uy,uw,uh;
	
	QPixmap pix;

};

class ButtonHand : public QPushButton
{
	Q_OBJECT

public:
	ButtonHand(QWidget *parent = 0);
	~ButtonHand(){};
	void setPixmap(QString sIconNameBase, QString sIconNamePushed);    
protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent ( QMouseEvent * event );
	void mouseReleaseEvent ( QMouseEvent * event );

signals:
        void clicked();
        void pressed();
private:
	QPixmap m_Icon;
	QString m_sIconBase;
	QString m_sIconPushed;
};

class handinput : public YTInputMethodC
{
	Q_OBJECT

public:
	handinput(QWidget *parent = 0);
	~handinput();
	
	HandInputWidget *m_InputWidget;
        
    void setHandInputUi();
    void setContextWords(QString textBeforeCursor,QStringList contextWords);
    void reset();

	static YTInputMethodC *factory() { return new handinput(); }

    void handInputShow();	
	bool AutomaticCandidate;
	bool AutomaticIsSend;
	bool WriteReturn;

private slots:
	void buttonTextClear();
        
	void on_pbShow1_clicked();
	void on_pbShow2_clicked();
	void on_pbShow3_clicked();
	void on_pbShow4_clicked();
	void on_pbShow5_clicked();
	void on_pbShow6_clicked();
	void on_pbShow7_clicked();
	void on_pbShow8_clicked();
	void on_pbShow9_clicked();
	void on_pbShow10_clicked();
	void on_pbHide_clicked();
	void on_pbSpace_clicked();
	void on_pbChange_clicked();
	void on_pbDel_clicked();
	void on_pbDel_pressed();
	void DelTimer();
	void ShowTextTimer();
private:

	void sendText(QString string);
	void setUiText(int num, unsigned short getResult[], bool isAssociate );
	void setIssoText(QString text);//添加显示联想词
	void HandInputSignalAndSlot();
	
	bool 			m_bFull;
	bool 			isChinese;
	THWAttribute         attr;

#if  HANWANG_INTEGRATED_DICT==0	
	unsigned char* pRom;	
#endif
	unsigned short Result[MAXCANDNUM];

public:
	QList <THWPoint> m_arrPoints;
	void handWriteRecong();
        
    ButtonHand *pbShow1;
    ButtonHand *pbShow2;
    ButtonHand *pbShow3;
    ButtonHand *pbShow4;
    ButtonHand *pbShow5;
    ButtonHand *pbShow6;
    ButtonHand *pbShow7;
    ButtonHand *pbShow8;
    ButtonHand *pbShow9;
    ButtonHand *pbShow10;

    ButtonHand *pbHide;
    ButtonHand *pbDel;
    ButtonHand *pbSpace;
    ButtonHand *pbChange;   

	QTimer		*m_DelTimer;
	QTimer		*m_ShowTextTimer;
	int        DelTimeOutTime;
	QStringList ButtonTextList;
};        
#endif // HANDINPUT_H
