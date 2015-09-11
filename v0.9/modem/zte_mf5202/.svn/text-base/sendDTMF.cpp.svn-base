/**
 * @file   sendDTMF.cpp
 * @author chentao <chentao01@chinatsp.com>
 * @date   20110303
 * 
 * @brief  
 *      Implement for DTMF send
 * 
 */

#include "sendDTMF.h"
#include "MobileInterFace1.h"
#include "debugMonitor.h"



SendDTMFC::SendDTMFC(PhoneMonitorVoiceInterface1 *pFather)
	: m_pFather(pFather),
	  m_pContent(NULL),
	  m_iCurSendByteNum(0)
{

}

SendDTMFC::~SendDTMFC()
{
	stopSend();
}

int SendDTMFC::send(char *pContent)
{
	if( pContent == NULL )
		return -1;
	
	int len = strlen(pContent)+1;
	len = (len+3)/4*4;
	
	m_pContent = new char[len];
	strcpy(m_pContent, pContent);

	m_iCurSendByteNum = 0;

	m_iTimerForSend = startTimer(500);
	return 0;
}

void SendDTMFC::stopSend()
{
	if( m_iTimerForSend > 0 )
	{
		killTimer(m_iTimerForSend);
		delete m_pContent;
		m_pContent = NULL;
		m_iCurSendByteNum = 0;
		m_iTimerForSend = 0;
	}
}

int SendDTMFC::sendOneByte()
{
	int retval = 0;
	if( m_iCurSendByteNum == 0 )
	{
		killTimer(m_iTimerForSend);
		m_iTimerForSend = startTimer(200);
	}
	
	if( m_pContent[m_iCurSendByteNum] != 0 )
	{
		if( (retval=m_pFather->rtDial(m_pContent[m_iCurSendByteNum])) < 0 )
		{
			IBD_PRINTF(DEBUG_INFOMATION, "DTMF upload error     byte[%c][%d] retval[%d]!\n", m_pContent[m_iCurSendByteNum], m_pContent[m_iCurSendByteNum], retval)
			stopSend();
			emit sigFinishedSend();

			return retval;
		}
		
		m_iCurSendByteNum++;
	}

	if( m_pContent[m_iCurSendByteNum] == 0 )
	{
		stopSend();
		emit sigFinishedSend();
	}

	return 0;
}

void SendDTMFC::timerEvent(QTimerEvent * event)
{
	if( m_iTimerForSend == event->timerId() )
	{
		sendOneByte();
	}
}


