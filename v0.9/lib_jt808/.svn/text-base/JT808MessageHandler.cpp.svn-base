#include "JT808MessageHelper.h"
#include "ConfigManager.h"
#include "LogUtility.h"
#include "ConnectionManager.h"
#include "VDRDeployParaTable.h"
#include "Utility.h"
//#include <dbusServer.h>
//#include <dbusApplication.h>
//#include "JT808DbusMessageHandler.h"
//#include "NaviDbusMessage.h"


extern ConnectStatus SendConnectionStatus;

static void JT808TimerHandler(string &name, void *data, void *data2)
{
    JT808MessageHandler *handler = (JT808MessageHandler *)data;
    if(handler != NULL)
    {
        handler->HandleTimer(name, data2);
    }
}

JT808MessageHandler::JT808MessageHandler()
{
    m_helloTimer = NULL;
    m_queueTimer = NULL;
    m_retryTimer = NULL;
    m_retryTimer_location = NULL;
    RefreshConfig();
    GetPhoneNumber();
    m_waitingQueue.set_capacity(JT808PacketFifo::capacity_type(JT808_MAX_WAITING_QUEUE));
    m_waitingQueue_reply.set_capacity(JT808PacketFifo::capacity_type(JT808_MAX_WAITING_QUEUE));
    m_waitingQueue_location.set_capacity(JT808PacketFifo::capacity_type(JT808_MAX_WAITING_QUEUE_LOCATION));
    m_rxPacket.set_capacity(JT808Packet::capacity_type(JT808_MAX_PACKET_SIZE, JT808_MIN_PACKET_SIZE));
    ClearRxPacket();
    m_seq = 0;
    ConnectionManager::Instance()->AddEventHandler(this);
    ConnectionManager::Instance()->SetPacketHandler(this);
    
    if(pthread_mutex_init(&m_queueMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler: mutex init failed.");
        return;
    }
    if(pthread_mutex_init(&m_packetMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler: mutex init failed.");
        return;
    }
    
    m_registered = false;
    m_authorized = false;
    s_multipacket= false;

    VDRDeployParaTable *table = new VDRDeployParaTable();
    string authToken = "";
    if(table->GetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, authToken) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler get authToken failed");
    }
    
    if(authToken.length() > 0)
    {
        m_registered = true;
    }

    delete table;
    
    
    JT808MessageHelper::Init(this);
}
void JT808MessageHandler::SetFifotodevice(CFifo *fifo)
{
	to_device=fifo;
}
JT808MessageHandler::~JT808MessageHandler()
{
    DestroyTimer(m_helloTimer);
    DestroyTimer(m_queueTimer);
    DestroyTimer(m_retryTimer);
    DestroyTimer(m_retryTimer_location);
    ClearQueue();
    ClearRxPacket();
    pthread_mutex_destroy(&m_queueMutex);
    pthread_mutex_destroy(&m_packetMutex);
}

void JT808MessageHandler::ClearQueue()
{
    pthread_mutex_lock(&m_queueMutex);
    for(JT808PacketFifo::iterator it = m_waitingQueue_reply.begin(); it != m_waitingQueue_reply.end(); it ++)
    {
        StruJT808Packet *packet = *it;
        DestroyPacket(packet);
    }
    m_waitingQueue_reply.clear();
    for(JT808PacketFifo::iterator it = m_waitingQueue.begin(); it != m_waitingQueue.end(); it ++)
    {
        StruJT808Packet *packet = *it;
        DestroyPacket(packet);
    }
    m_waitingQueue.clear();
    for(JT808PacketFifo::iterator it = m_waitingQueue_location.begin(); it != m_waitingQueue_location.end(); it ++)
    {
        StruJT808Packet *packet = *it;
	if(packet->retry!=0)packet->retry=0;
    }
    pthread_mutex_unlock(&m_queueMutex);
}

void JT808MessageHandler::RefreshConfig()
{
    ConfigManager::Refresh();
    m_helloTimeout = ConfigManager::GetHeartBeatInterval();
    m_retryTimeout = ConfigManager::GetTcpRetryTimeout();
    m_retryTimes = ConfigManager::GetTcpRetryTimes();
    LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::RefreshConfig Hello Timeout %d", m_helloTimeout);
    LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::RefreshConfig Retry Timeout %d", m_retryTimeout);
    LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::RefreshConfig Retry Times %d", m_retryTimes);
    if(m_helloTimer)
    {
        m_helloTimer->timeout = TIMER_SEC_TO_MSEC(m_helloTimeout);
    }
    else
    {
        m_helloTimer = CreateTimer(JT808_HELLO_TIMER_NAME, TIMER_SEC_TO_MSEC(m_helloTimeout), this, JT808TimerHandler, false);
    }
    if(m_queueTimer)
    {
        m_queueTimer->timeout = TIMER_SEC_TO_MSEC(JT808_QUEUE_TIME_OUT);
    }
    else
    {
        m_queueTimer = CreateTimer(JT808_QUEUE_TIMER_NAME, TIMER_SEC_TO_MSEC(JT808_QUEUE_TIME_OUT), this, JT808TimerHandler, false);
    }
    if(m_retryTimer)
    {
        m_retryTimer->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
    }
    else
    {
        m_retryTimer = CreateTimer(JT808_RETRY_TIMER_NAME, TIMER_SEC_TO_MSEC(m_retryTimeout), this, JT808TimerHandler, false);
    }
    if(m_retryTimer_location)
    {
        m_retryTimer_location->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
    }
    else
    {
        m_retryTimer_location = CreateTimer(JT808_RETRY_TIMER_NAME_LOCATION, TIMER_SEC_TO_MSEC(m_retryTimeout), this, JT808TimerHandler, false);
    }
}

void JT808MessageHandler::GetPhoneNumber()
{
    string phoneNumber;
    VDRDeployParaTable *table = new VDRDeployParaTable();
    int arraylen = sizeof(m_phoneNumber);
    int maxlen = arraylen * 2;
    
    for(int i = 0; i < (int)sizeof(m_phoneNumber); i ++)
    {
        m_phoneNumber[i] = 0x00;
    }
    
    if(table->GetStringValue((int)VDR_DEPLOY_PARA_PHONE_NUMBER, phoneNumber) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::GetPhoneNumber failed");
        delete table;
        return;
    }
    
    LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::GetPhoneNumber %s", phoneNumber.c_str());
    
    delete table;
    
    int len = phoneNumber.length();
    
    int offset = maxlen - len;
    if(offset < 0)
    {
        offset = 0;
    }
    
    for(int i = 0; i < maxlen; i ++)
    {
        unsigned char bcd1;
        unsigned char bcd2;
        if(offset > i)
        {
            bcd1 = 0x0;
        }
        else
        {
            bcd1 = CharToBCD(phoneNumber[i - offset]);
        }
        i ++;
        if((maxlen - i) > len)
        {
            bcd2 = 0x0;
        }
        else
        {
            bcd2 = CharToBCD(phoneNumber[i - offset]);
        }
        m_phoneNumber[i/2] = (unsigned char)((bcd1 << 4) |(bcd2));
    }

    return;
}

int JT808MessageHandler::ConnectionEventHandler(ENUM_CONNECTION_EVENT state, ENUM_CONNECTION_ID conn)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ConnectionEventHandler state %x, conn %x.", (int)state, (int)conn);
    if(state == ENUM_CONNECTION_STATE_CONNECTED)
    {
        if(conn == ENUM_CONNECTION_ID_MASTER)
        {
//            SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_CONNECTED, (int)CONN_STATUS_NTF_DISCONNECT);
//            此处应调用外部函数
	      SendConnectionStatus((int)CONN_STATUS_NTF_CONNECTED, (int)CONN_STATUS_NTF_DISCONNECT);
        }
        else
        {
  //          SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_CONNECTED);
	//  同上
	      SendConnectionStatus((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_CONNECTED);
	}

	if((conn == ENUM_CONNECTION_ID_MASTER) || (conn == ENUM_CONNECTION_ID_SECONDARY))
	{
		if(ConfigManager::m_control == 0){
			JT808MessageHelper::SendAuth();
		}else if(m_registered){
			if(ConfigManager::GetAuthFlag())
			{
				JT808MessageHelper::SendAuth();
			}
		}
	}
    }
    if(state == ENUM_CONNECTION_STATE_CONNECTTING)
    {
        if(conn == ENUM_CONNECTION_ID_MASTER)
        {
//            SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_CONNECTING, (int)CONN_STATUS_NTF_DISCONNECT);
  //          同上
              SendConnectionStatus((int)CONN_STATUS_NTF_CONNECTING, (int)CONN_STATUS_NTF_DISCONNECT);
  	}
        else
        {
//            SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_CONNECTING);
//            同上  
	      SendConnectionStatus((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_CONNECTING);
  	}
    }
    else if(state == ENUM_CONNECTION_STATE_DISCONNECTED)
    {
//        SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_DISCONNECT);
//        同上
          SendConnectionStatus((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_DISCONNECT); 
        StopTimer(m_helloTimer); 
        StopTimer(m_queueTimer); 
	StopTimer(m_retryTimer);
        StopTimer(m_retryTimer_location);
        ClearQueue();
        ClearRxPacket();
        m_authorized = false;
        s_multipacket= false;
    }
    
    return 0;
}

int JT808MessageHandler::PacketHandler(unsigned char *buf, int len)
{
    for(int i = 0; i < len; i ++)
    {
        unsigned char ch = buf[i];
        
        if(m_rxPacket.size() >= JT808_MAX_PACKET_SIZE)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PacketHandler buffer overflow.");
            ClearRxPacket();
            continue;
        }

        if(ch == JT808_PACKET_FLAG)
        {
            if(m_packet_start)
            {
                m_rxPacket.push_back(ch);
                if(m_rxPacket.size() < JT808_MIN_PACKET_SIZE)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PacketHandler too short.");
                    ClearRxPacket();
                    continue;
                }
                HandlePacket();
                ClearRxPacket();
            }
            else
            {
                m_rxPacket.push_back(ch);
                m_packet_start = true;
                m_packet_escape = false;
            }
            continue;
        }
        
        if(!m_packet_start)
        {
            continue;
        }
        
        if(ch == JT808_PACKET_ESCAPE)
        {
            if(m_packet_escape)
            {
                m_rxPacket.push_back(JT808_PACKET_ESCAPE);
            }
            m_packet_escape = true;
            continue;
        }
        
        if(m_packet_escape)
        {
            if(ch == 0x01)
            {
                m_rxPacket.push_back(JT808_PACKET_ESCAPE);
            }
            else if(ch == 0x02)
            {
                m_rxPacket.push_back(JT808_PACKET_FLAG);
            }
            else
            {
                m_rxPacket.push_back(JT808_PACKET_ESCAPE);
                m_rxPacket.push_back(ch);
            }
            m_packet_escape = false;
            continue;
        }
        
        m_rxPacket.push_back(ch);
    }
    
    return 0;
}

void JT808MessageHandler::HandlePacket()
{
    unsigned short id = JT808_GET_SHORT(m_rxPacket, 1);
    unsigned short attr = JT808_GET_SHORT(m_rxPacket, 3);
    unsigned short seq = JT808_GET_SHORT(m_rxPacket, 11);
    unsigned short packetNum;
    unsigned short sum;
    
    int len = attr & 0x3FF;
    bool segment = (attr >> 13) & 0x1;
    int crypto = (attr >> 10) & 0x7;
    int body;
    unsigned char checksum = 0;
    int packetsize = m_rxPacket.size();
    
    if(segment)
    {
        body = 17;

	sum = JT808_GET_SHORT(m_rxPacket,13);
	packetNum = JT808_GET_SHORT(m_rxPacket,15);
    }
    else
    {
        body = 13;
    }
    
    if(crypto != JT808_CRYTO_NONE)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::HandlePacket crypto not support(%d).", crypto);
        return;
    }
    
    if((body==13) && ((packetsize - 13 - 2) != len))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::HandlePacket length not matching(%d, %d).", packetsize, len);
        return;
    }
    if((body==17) && ((packetsize - 17 -2) != len))
    {
    	LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHandler::HandlePacket length not matching(%d, %d).", packetsize,len);
	return;
    }
    for(int i = 1; i < packetsize - 2; i ++)
    {
        checksum ^= m_rxPacket[i];
    }
    
    if(checksum != m_rxPacket[packetsize - 2])
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::HandlePacket checksum not matching(%02x, %02x).", checksum, m_rxPacket[packetsize - 2]);
        return;
    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandlePacket check pass(%04x, %d).", id, len);
    
    switch(id)
    {
        case JT808_PACKET_PLATFORM_COMMON_REP:
            ProcessCommonReply(body, len);
            break;
        case JT808_PACKET_ID_REGISTER_REP:
            ProcessRegisterReply(body, len);
            break;
        case JT808_PACKET_RESEND_REQUEST:
            JT808MessageHelper::HandlePacketResendRequest(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_SET_PARAMETERS:
            JT808MessageHelper::HandleSetParameters(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_QUERY_PARAMETERS:
            JT808MessageHelper::SendAck(seq,id,ENUM_JT808_COMMON_OK);
            JT808MessageHelper::SendParameters(seq);
            break;
        case JT808_PACKET_QUERY_LIST_PARAMETERS:
	    JT808MessageHelper::HandleRequryTermParameters(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_TERM_CONTROL:
            JT808MessageHelper::HandleTermControl(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_QUERY_ATTRIBUTE:
            JT808MessageHelper::SendAck(seq,id,ENUM_JT808_COMMON_OK);
            JT808MessageHelper::SendAttribute(seq);
            break;
        case JT808_PACKET_UPDATE_TERM:
            JT808MessageHelper::HandleTermUpdate(m_rxPacket, body, len, seq, sum, packetNum);
            break;
        case JT808_PACKET_LOC_QUERY:
            JT808MessageHelper::SendAck(seq,id,ENUM_JT808_COMMON_OK);
            JT808MessageHelper::SendLocationQueryResp(seq);
            break;
        case JT808_PACKET_TEMP_LOC_CONTROL:
            JT808MessageHelper::HandleTempLocControl(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MANUAL_CONFIRM:
            JT808MessageHelper::HandleAlarmConfirm(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_TEXT_MESSAGE:
            JT808MessageHelper::HandleTextMessage(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_EVENT:
            JT808MessageHelper::HandleEvent(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_QUESTION:
            JT808MessageHelper::HandleQuestion(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_CALL_BACK:
            JT808MessageHelper::HandleCallBack(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_PHONE_BOOK:
            JT808MessageHelper::HandlePhoneBook(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_CAR_CONTROL:
            JT808MessageHelper::HandleCarControl(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_SET_CIRCLE:
            JT808MessageHelper::HandleSetCircle(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_DELETE_CIRCLE:
            JT808MessageHelper::HandleDeleteCircle(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_SET_RECTANGLE:
            JT808MessageHelper::HandleSetRectangle(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_DELETE_RECTANGLE:
            JT808MessageHelper::HandleDeleteRectangle(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_SET_POLYGON:
            JT808MessageHelper::HandleSetPolygon(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_DELETE_POLYGON:
            JT808MessageHelper::HandleDeletePolygon(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_SET_LINE:
            JT808MessageHelper::HandleSetLine(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_DELETE_LINE:
            JT808MessageHelper::HandleDeleteLine(m_rxPacket, body, len, seq);
            break;
	case JT808_PACKET_DATA_ACQ:			    
            JT808MessageHelper::HandleDataAcq(m_rxPacket, body, len, seq);
            break;
	case JT808_PACKET_PARA_SET:			    
            JT808MessageHelper::HandleParaSet(m_rxPacket, body, len, seq);
	    break;
	case JT808_PACKET_QUERY_DRIVER_INFO:              
            //JT808MessageHelper::SendAck(seq,id,ENUM_JT808_COMMON_OK);
            JT808MessageHelper::SendDriverInfo(0);
	    break;
	case JT808_PACKET_CAMERA_START:
            JT808MessageHelper::HandleCameraStart(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_RECORD_START:
            JT808MessageHelper::HandleRecordStart(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_INDEX_QUERY:
            JT808MessageHelper::HandleQueryMediaIndex(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_SINGLE_INDEX_QUERY:
            JT808MessageHelper::HandleQueryMediaSingleIndex(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_DATA_RSP:
            JT808MessageHelper::HandlePacketMediaDataRsp(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_UPLOAD:
            JT808MessageHelper::HandlePacketMediaUpload(m_rxPacket, body, len, seq);
	    break;
	case JT808_PACKET_DOWN_TRANSPARENT_TRANSMISSION:  
            JT808MessageHelper::HandlePacketDownTransparentTransmission(m_rxPacket, body, len, seq);
	    break;
        default:
            LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandlePacket unknown message.");
            break;
    }
}

void JT808MessageHandler::ProcessCommonReply(int start, int len)
{
    if(len != 5)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::ProcessCommonReply len invalid.");
        return;
    }
    
    int seq = JT808_GET_SHORT(m_rxPacket, start);
    int id = JT808_GET_SHORT(m_rxPacket, start+2);
    int result = JT808_GET_BYTE(m_rxPacket, start+4);
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ProcessCommonReply %d, %04x, %d.", seq, id, result);
#if 1
    if(id != JT808_PACKET_LOC_REPORT)
	    PopQueue(id, seq);
#endif
   // PopQueue(id, seq);
    
    if(id==JT808_PACKET_ID_AUTH && m_seq>1)StartTimer(m_queueTimer);
    JT808MessageHelper::HandleCommonReply(id, result);
}

void JT808MessageHandler::ProcessRegisterReply(int start, int len)
{
    string auth = "";
    if(len < 3)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::ProcessRegisterReply len invalid.");
        return;
    }
    
    int seq = JT808_GET_SHORT(m_rxPacket, start);
    int result = JT808_GET_BYTE(m_rxPacket, start+2);

    for(int i = 3; i < len; i ++)
    {
        auth.push_back(m_rxPacket[i + start]);
    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ProcessRegisterReply %d, %d, %s.", seq, result, auth.c_str());
    
    PopQueue(JT808_PACKET_ID_REGISTER, seq);

    JT808MessageHelper::HandleRegisterResult(result, auth);
}

void JT808MessageHandler::PopQueue(int id, int seq)
{
    pthread_mutex_lock(&m_queueMutex);
    if((id == JT808_PACKET_LOC_REPORT) || (id == JT808_PACKET_LOC_MULTI_REPORT)){
    //if(id == JT808_PACKET_LOC_MULTI_REPORT){
	    if(m_waitingQueue_location.size() <= 0)
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PopQueue location waiting queue empty.");
		    pthread_mutex_unlock(&m_queueMutex);
		    return;
	    }

	    StruJT808Packet *packet = *(m_waitingQueue_location.begin());
	    if((packet->id == id) && (packet->seq == seq))
	    {
		    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PopQueue location correct resp.");
		    m_waitingQueue_location.pop_front();
		    DestroyPacket(packet);
		    StopTimer(m_retryTimer_location);
		    if(m_waitingQueue_location.size() > 0)
		    {
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PopQueue location start queue timer");
			    StartTimer(m_queueTimer);
		    }
	    }
	    else
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PopQueue location incorrect resp(%d, %04x).", packet->seq, packet->id);
	    }
    }else{
	    if(m_waitingQueue.size() <= 0)
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PopQueue waiting queue empty.");
		    pthread_mutex_unlock(&m_queueMutex);
		    return;
	    }

	    StruJT808Packet *packet = *(m_waitingQueue.begin());
LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::incorrect resp(%d, %d, %04x).", packet->retry, packet->seq, packet->id);
	    if((packet->retry >= 1) && (packet->id == id) && (packet->seq == seq))
	    {
		    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PopQueue correct resp.");
		    m_waitingQueue.pop_front();
		    DestroyPacket(packet);
		    StopTimer(m_retryTimer);
		    if(m_waitingQueue.size() > 0)
		    {
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PopQueue start queue timer");
			    StartTimer(m_queueTimer);
		    }
	    }
	    else
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PopQueue incorrect resp(%d, %04x).", packet->seq, packet->id);
	    }
    }
    
    pthread_mutex_unlock(&m_queueMutex);
}

void JT808MessageHandler::PopQueue(int index)
{
	pthread_mutex_lock(&m_queueMutex);
	if(m_waitingQueue.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "jt808messagehandler::popqueue waiting queue empty.");
		pthread_mutex_unlock(&m_queueMutex);
		return;
	}

	StruJT808Packet *packet = *(m_waitingQueue.begin());
	JT808Packet &jt_packet=*(packet->packetarray[0]);
	if(JT808_GET_LONG(jt_packet, 17)== index)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::popqueue correct resp.");
		m_waitingQueue.pop_front();
		DestroyPacket(packet);
		StopTimer(m_retryTimer);
		if(m_waitingQueue.size() > 0)
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::popqueue start queue timer");
			StartTimer(m_queueTimer);
		}
	}
	else
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "jt808messagehandler::popqueue incorrect resp(%d, %04x).", packet->seq, JT808_GET_LONG(jt_packet, 0));
	}
	pthread_mutex_unlock(&m_queueMutex);

}
void JT808MessageHandler::HandleTimer(string &name, void *data)
{
	if(name == JT808_HELLO_TIMER_NAME)
	{
		SendHello();
	}
	else if(name == JT808_QUEUE_TIMER_NAME)
	{
		SendQueue();
	}
	else if(name == JT808_RETRY_TIMER_NAME)
	{
		HandleRetryTimer(data);
	}
	else if(name == JT808_RETRY_TIMER_NAME_LOCATION)
	{
		HandleRetryTimerLocation(data);
	}
}

void JT808MessageHandler::HandleRetryTimer(void *data)
{
	pthread_mutex_lock(&m_queueMutex);
	StruJT808Packet *timer_packet = (StruJT808Packet *)data;
	if(m_waitingQueue.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandleRetryTimer empty");
		pthread_mutex_unlock(&m_queueMutex);
		return ;
	}

	StruJT808Packet *packet = *(m_waitingQueue.begin());
	if(timer_packet != packet)
	{
		LogUtility::Log(LOG_LEVEL_WARN, "JT808MessageHandler::HandleRetryTimer out of sync");
		pthread_mutex_unlock(&m_queueMutex);
		return ;
	}
	if(packet->retry > m_retryTimes)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandleRetryTimer %d reach limit", packet->id);
		m_waitingQueue.pop_front();
		DestroyPacket(packet);
		ConnectionManager::Instance()->Reconnect();
	}
	else
	{
		SendPacket(packet);
		packet->retry ++;
	}
	pthread_mutex_unlock(&m_queueMutex);
}
void JT808MessageHandler::HandleRetryTimerLocation(void *data)
{
	pthread_mutex_lock(&m_queueMutex);
	StruJT808Packet *timer_packet = (StruJT808Packet *)data;
	if(m_waitingQueue_location.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandleRetryTimer empty");
		pthread_mutex_unlock(&m_queueMutex);
		return ;
	}

	StruJT808Packet *packet = *(m_waitingQueue_location.begin());
	if(timer_packet != packet)
	{
		LogUtility::Log(LOG_LEVEL_WARN, "JT808MessageHandler::HandleRetryTimer out of sync");
		pthread_mutex_unlock(&m_queueMutex);
		return ;
	}
	if(packet->retry >= m_retryTimes)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandleRetryTimer %d reach limit", packet->id);
		m_waitingQueue_location.pop_front();
		DestroyPacket(packet);
		ConnectionManager::Instance()->Reconnect();
	}
	else
	{
		SendPacket(packet);
		packet->retry ++;
	}
	pthread_mutex_unlock(&m_queueMutex);
}

void JT808MessageHandler::SendHello()
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendHello entry");
    StruJT808Packet *packet = CreatePacket();
    
    SendBody(packet, JT808_PACKET_ID_HELLO, JT808_CRYTO_NONE);
}

void JT808MessageHandler::SendBody(StruJT808Packet *packet, unsigned short id, ENUM_JT808_CRYTO crypto)
{
	if(ConnectionManager::Instance()->m_connId == ENUM_CONNECTION_ID_IC)
	{
		if(id != JT808_PACKET_UP_TRANSPARENT_TRANSMISSION)
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::current state is connect to ic");
			DestroyPacket(packet);
			return;
		}

	}else if(!m_registered || !m_authorized)
	{
		if((id != JT808_PACKET_LOC_MULTI_REPORT ) && (id != JT808_PACKET_ID_QUICK_REGISTER) && (id != JT808_PACKET_ID_REGISTER) && (id != JT808_PACKET_ID_AUTH) && (id!=JT808_PACKET_LOC_REPORT))
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendBody not ready(%d, %d)", m_registered, m_authorized);
			DestroyPacket(packet);
			return;
		}
	}
    
    if(packet->packetarray.size())
    {
	    vector<JT808Packet*> &packetarray = packet->packetarray;
	    for(int i = 0; i < (int)packetarray.size(); i ++)
	    {
		    if(packetarray[i]->size() > JT808_SEGMENT_SIZE)
		    {
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendBody segment array find big packet(%d)", i);
			    DestroyPacket(packet);
			    return;
		    }
		    if(i == 0) packet->seq = m_seq;
		    PacketHead(packet,packetarray[i], (unsigned short)id, crypto, packet->segment_id[i]);
		    PacketCheckSumFlag(packetarray[i]);
	    }
	    packet->segment_id.clear();
	    packet->send_index=0;
	    PushQueue(packet);
    }
    else
    {
        JT808Packet &content = packet->packet;
        if(content.size() > JT808_SEGMENT_SIZE)
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendBody segment not support");
            DestroyPacket(packet);
            return;
        }
        PacketHead(packet, (unsigned short)id, crypto);
	PacketCheckSumFlag(packet);
	PushQueue(packet);
    }
}

int JT808MessageHandler::PushQueue(StruJT808Packet *packet)
{
	pthread_mutex_lock(&m_queueMutex);
	switch(packet->id){
		case JT808_PACKET_TERM_COMMON_REP:
		case JT808_PACKET_LOC_QUERY_REP:
		case JT808_PACKET_ANSWER:
		case JT808_PACKET_QUERY_PARAMETERS_REP:
		case JT808_PACKET_CAMERA_START_RSP:
		case JT808_PACKET_LOC_REPORT:
		case JT808_PACKET_MEDIA_INDEX_QUERY_RSP:
		case JT808_PACKET_DATA_SEND:
		case JT808_PACKET_UPDATE_TERM_REP:
		case JT808_PACKET_QUERY_ATTRIBUTE_REP:
		case JT808_PACKET_RESEND_REQUEST:
			{
				if(packet->packetarray.size() >0){
					if(m_waitingQueue.size() >= JT808_MAX_WAITING_QUEUE)
					{
						LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue full");
						pthread_mutex_unlock(&m_queueMutex);
						DestroyPacket(packet);
						return -1;
					}
					m_waitingQueue.push_back(packet);
					if(m_waitingQueue.size() == 1)
					{
						LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue empty");
						StartTimer(m_queueTimer);
					}
					pthread_mutex_unlock(&m_queueMutex);
					break;
				}
			}
#if 0
		case JT808_PACKET_LOC_REPORT:
		case JT808_PACKET_LOC_MULTI_REPORT: 
#endif
			if(m_waitingQueue_reply.size() >= JT808_MAX_WAITING_QUEUE)
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue full");
				pthread_mutex_unlock(&m_queueMutex);
				DestroyPacket(packet);
				return -1;
			}
			m_waitingQueue_reply.push_back(packet);
			if(m_waitingQueue_reply.size() == 1)
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue empty");
				StartTimer(m_queueTimer);
			}
			pthread_mutex_unlock(&m_queueMutex);
			break;
#if 1
		//case JT808_PACKET_LOC_REPORT:
		case JT808_PACKET_LOC_MULTI_REPORT:
			if(m_waitingQueue_location.size() >= JT808_MAX_WAITING_QUEUE_LOCATION)
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue full");
				pthread_mutex_unlock(&m_queueMutex);
				DestroyPacket(packet);
				return -1;
			}
			m_waitingQueue_location.push_back(packet);
			if(m_waitingQueue_location.size() == 1)
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue empty");
				StartTimer(m_queueTimer);
			}
			pthread_mutex_unlock(&m_queueMutex);
			break;
#endif
		default :
			if(m_waitingQueue.size() >= JT808_MAX_WAITING_QUEUE)
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue full");
				pthread_mutex_unlock(&m_queueMutex);
				DestroyPacket(packet);
				return -1;
			}
			m_waitingQueue.push_back(packet);
			if(m_waitingQueue.size() == 1)
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue empty");
				StartTimer(m_queueTimer);
			}
			pthread_mutex_unlock(&m_queueMutex);
			break;
	}
	return 0;
}

void JT808MessageHandler::SendQueue()
{
	pthread_mutex_lock(&m_queueMutex);
	if(m_authorized==true&&s_multipacket==false){
		if(m_waitingQueue_reply.size() <= 0)
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendQueue0 empty");
		}else{
			StruJT808Packet *packet = *(m_waitingQueue_reply.begin());
			SendPacket(packet);
			//packet->retry ++;
			pthread_mutex_unlock(&m_queueMutex);
			return;
		}
		if(m_waitingQueue_location.size() <= 0)
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendQueue1 empty");
		}else{
			StruJT808Packet *packet = *(m_waitingQueue_location.begin());
			if(packet->retry==0){
				SendPacket(packet);
				packet->retry ++;
				pthread_mutex_unlock(&m_queueMutex);
				return;
			}
		}
	}
	if(m_waitingQueue.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendQueue2 empty");
	}else{
		StruJT808Packet *packet = *(m_waitingQueue.begin());
		LogUtility::Log(LOG_LEVEL_DEBUG, "%d::*%d*****%d****%d",packet->retry, packet->seq, packet->id, packet->send_index);
		if(packet->retry == 0){
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::*******%d****%d  empty",packet->id,packet->send_index);
			SendPacket(packet);
			if((packet->send_index >= (int)packet->packetarray.size()) && (packet -> id == 0x0700))
			{
				packet->retry++;
				packet->send_index =0;
			//	m_waitingQueue.pop_front();
			//	DestroyPacket(packet);
			}else if(packet->id != JT808_PACKET_DATA_SEND){
				if(packet->packetarray.size()==0)packet->retry ++;
				else if(packet->send_index == packet->packetarray.size())packet->retry++;
			}
			pthread_mutex_unlock(&m_queueMutex);
			return;
		}else if(!(packet->segment_id.empty())){
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::reup ****%d****%d  empty",packet->id,packet->send_index);
			SendPacket(packet);
			pthread_mutex_unlock(&m_queueMutex);
			return;
		}else
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::cann't send **%d****%d  empty",packet->id,packet->retry);

	}
	pthread_mutex_unlock(&m_queueMutex);
	return;
}

void JT808MessageHandler::SendPacket(StruJT808Packet *packet)
{
	m_send_len = 0;

	vector<JT808Packet*> &packetarray = packet->packetarray;
	if(packetarray.size())
	{
		if(packet->retry == 0){
			int i = 0;
			if(s_multipacket==false)s_multipacket=true;
			while((i < (int)JT808_SEND_SEGMENT_NUM_ONCE) && (packet->send_index < (int)packetarray.size()))
			{
					if(SendContent(*packetarray[packet->send_index]) < 0)
					{
						LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::SendPacket failed to send %d packet", packet->send_index);
						ConnectionManager::Instance()->Reconnect();
						return;
					}
					i ++;
					packet->send_index ++;
					printf("current index is %d \n",packet->send_index);
			}

			if(packet->send_index >= (int)packetarray.size())
			{
#if 0
				if(packet -> id == 0x0700)
				{
					m_waitingQueue.pop_front();
					DestroyPacket(packet);
				}
#endif
				if(s_multipacket== true) s_multipacket= false;
				//m_retryTimer->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
				//m_retryTimer->data2 = packet;
				//StartTimer(m_retryTimer);
				if(m_waitingQueue_reply.size() > 0||m_waitingQueue_location.size() > 0)
				{
					LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket not empty");
					StartTimer(m_queueTimer);
				}
			}
			else
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket array not over");
				StartTimer(m_queueTimer);
			}
		}else if(!(packet->segment_id.empty())){
			if(s_multipacket==false)s_multipacket=true;
			if(SendContent(*packetarray[packet->segment_id[packet->send_index]]) < 0)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::SendPacket failed to send %d packet", packet->send_index);
				ConnectionManager::Instance()->Reconnect();
				return;
			}
			packet->send_index ++;
			printf("reup current index is %d \n",packet->send_index);
			if(packet->send_index >= (int)packet->segment_id.size())
			{
				packet->send_index = 0;
				packet->segment_id.clear();
				if(s_multipacket== true) s_multipacket= false;
				if(m_waitingQueue_reply.size() > 0||m_waitingQueue_location.size() > 0)
				{
					LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket not empty");
					StartTimer(m_queueTimer);
				}
			}
			else
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket array not over");
				StartTimer(m_queueTimer);
			}

		}else {
			if(SendContent(*packetarray[(int)packetarray.size()-1]) < 0)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::SendPacket failed to send %d packet", packet->send_index);
				ConnectionManager::Instance()->Reconnect();
				return;
			}
			m_retryTimer->timeout = m_retryTimer->timeout * (packet->retry + 1);
			m_retryTimer->data2 = packet;
			StartTimer(m_retryTimer);
			if(m_waitingQueue_reply.size() > 0||m_waitingQueue_location.size() > 0)
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket not empty");
				StartTimer(m_queueTimer);
			}
		}
	}
	else
	{
		if(SendContent(packet->packet) < 0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::SendPacket failed to send packet");
			ConnectionManager::Instance()->Reconnect();
			return;
		}

		if(packet->reply)
		{
			//if((packet->id == JT808_PACKET_LOC_REPORT) || (packet->id == JT808_PACKET_LOC_MULTI_REPORT))
			if(packet->id == JT808_PACKET_LOC_MULTI_REPORT)
			{
				m_waitingQueue_location.pop_front();
			}else 
				m_waitingQueue_reply.pop_front();
			if(m_waitingQueue_reply.size() > 0)
			{
				DestroyPacket(packet);
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket11 not empty");
				StartTimer(m_queueTimer);
				return ;
			}
			if(m_waitingQueue_location.size() > 0 && ((*m_waitingQueue_location.begin())->id!=packet->id))
			{
				DestroyPacket(packet);
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket21 not empty");
				StartTimer(m_queueTimer);
				return ;
			}
			if(m_waitingQueue.size() > 0 && ((*m_waitingQueue.begin())->id!=packet->id))
			{
				DestroyPacket(packet);
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket31 not empty");
				StartTimer(m_queueTimer);
				return ;
			}
			DestroyPacket(packet);
		}
		else
		{
			if(packet->id==JT808_PACKET_LOC_REPORT)
			{
				if(packet->retry == 0)
				{
					m_retryTimer_location->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
				}
				else
				{
					m_retryTimer_location->timeout = m_retryTimer_location->timeout * (packet->retry + 1);
				}

				m_retryTimer_location->data2 = packet;
				StartTimer(m_retryTimer_location);
			}
			else 
			{
				if(packet->retry == 0)
				{
					m_retryTimer->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
				}
				else
				{
					m_retryTimer->timeout = m_retryTimer->timeout * (packet->retry + 1);
				}

				m_retryTimer->data2 = packet;
				StartTimer(m_retryTimer);
			}

			if(m_waitingQueue_reply.size() > 0)
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket1 not empty");
				StartTimer(m_queueTimer);
				return ;
			}
			if(m_waitingQueue_location.size() > 0 && ((*m_waitingQueue_location.begin())->id!=packet->id)&&((*m_waitingQueue.begin())->retry==0))
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket2 not empty");
				StartTimer(m_queueTimer);
				return ;
			}
			if(m_waitingQueue.size() > 0 && ((*m_waitingQueue.begin())->id!=packet->id)&&((*m_waitingQueue.begin())->retry==0))
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket3 not empty %d",(*m_waitingQueue.begin())->id);
				StartTimer(m_queueTimer);
				return ;
			}

		}
    }
    
    return;
}

int JT808MessageHandler::SendContent(JT808Packet &content)
{
    int ret = 0;
    
    m_send_len = 0;

    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendContent entry(%d)", content.size());
    JT808Packet::const_array_range ar = content.array_one();
    if(ar.second > 0)
    {
        memcpy(m_send_buffer, ar.first, ar.second);
        m_send_len += ar.second;
    }
    
    ar = content.array_two();
    if(ar.second > 0)
    {
        memcpy(m_send_buffer + m_send_len, ar.first, ar.second);
        m_send_len += ar.second;
    }
    
    ret = ConnectionManager::Instance()->Send(m_send_buffer, m_send_len);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::SendContent failed");
        return -1;
    }
    
    return m_send_len;
}

void JT808MessageHandler::PacketHead(StruJT808Packet *thepacket, unsigned short id, ENUM_JT808_CRYTO crypto)
{
    unsigned short len;
    unsigned short attr = 0;
    
    JT808Packet &packet = thepacket->packet;
    
    len = (unsigned short)packet.size();
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PacketHead Count len %d", len);

    
    packet.push_front((unsigned char)((unsigned char)(m_seq & 0xFF)));
    packet.push_front((unsigned char)((unsigned char)((m_seq >> 8) & 0xFF)));

    pthread_mutex_lock(&m_packetMutex);

    thepacket->seq = m_seq;
    thepacket->id = id;

    m_seq ++;
    pthread_mutex_unlock(&m_packetMutex);

    for(int i = sizeof(m_phoneNumber); i > 0; i --)
    {
        packet.push_front(m_phoneNumber[i-1]);
    }

    if(thepacket->total_segment)
    {
        attr |= (1 << 13);
    }
    
    if(crypto == JT808_CRYTO_RSA)
    {
        attr |= (1 << 10);
    }
    attr |= (len & 0x3FF);
    packet.push_front((unsigned char)((unsigned char)(attr & 0xFF)));
    packet.push_front((unsigned char)((unsigned char)((attr >> 8) & 0xFF)));

    packet.push_front((unsigned char)((unsigned char)(id & 0xFF)));
    packet.push_front((unsigned char)((unsigned char)((id >> 8) & 0xFF)));
}
void JT808MessageHandler::PacketHead(StruJT808Packet *packet , JT808Packet *thepacket, unsigned short id, ENUM_JT808_CRYTO crypto, int index)
{
    unsigned short len;
    unsigned short attr = 0;
    
    
    len = (unsigned short)thepacket->size();
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PacketHead len %d", len);

    if(packet->total_segment)
    {
        thepacket->push_front((unsigned char)((unsigned char)(index & 0xFF)));
        thepacket->push_front((unsigned char)((unsigned char)((index >> 8) & 0xFF)));
        thepacket->push_front((unsigned char)((unsigned char)(packet->total_segment & 0xFF)));
        thepacket->push_front((unsigned char)((unsigned char)((packet->total_segment >> 8) & 0xFF)));
    }
    
    thepacket->push_front((unsigned char)((unsigned char)(m_seq & 0xFF)));
    thepacket->push_front((unsigned char)((unsigned char)((m_seq >> 8) & 0xFF)));

    pthread_mutex_lock(&m_packetMutex);

    //packet->seq = m_seq;
    packet->id = id;

    m_seq ++;
    pthread_mutex_unlock(&m_packetMutex);

    for(int i = sizeof(m_phoneNumber); i > 0; i --)
    {
        thepacket->push_front(m_phoneNumber[i-1]);
    }

    if(packet->total_segment)
    {
        attr |= (1 << 13);
    }
    
    if(crypto == JT808_CRYTO_RSA)
    {
        attr |= (1 << 10);
    }
    attr |= (len & 0x3FF);
    thepacket->push_front((unsigned char)((unsigned char)(attr & 0xFF)));
    thepacket->push_front((unsigned char)((unsigned char)((attr >> 8) & 0xFF)));

    thepacket->push_front((unsigned char)((unsigned char)(id & 0xFF)));
    thepacket->push_front((unsigned char)((unsigned char)((id >> 8) & 0xFF)));
}

void JT808MessageHandler::PacketCheckSumFlag(StruJT808Packet *thepacket)
{
    unsigned char checksum = 0;
    
    JT808Packet &packet = thepacket->packet;
    
    for(JT808Packet::iterator it = packet.begin(); it != packet.end(); it ++)
    {
        checksum ^= (*it);
    }
    
    packet.push_back(checksum);

    for(JT808Packet::iterator it = packet.begin(); it != packet.end(); it ++)
    {
	    if(*it==JT808_PACKET_FLAG)
	    {
		    *it=JT808_PACKET_ESCAPE;
		    if(++it==packet.end())
		    {
			    packet.push_back(JT808Value(0x02));
			    break;
		    }
		    else
			    it=packet.insert(it,JT808Value(0x02));
		    continue;
	    }
	    if(*it==JT808_PACKET_ESCAPE)
	    {
		    if(++it==packet.end())
		    {
			    packet.push_back(JT808Value(0x01));
			    break;
		    }
		    else
			    it=packet.insert(it,JT808Value(0x01));
	    }
    }

    packet.push_front(JT808_PACKET_FLAG);
    packet.push_back(JT808_PACKET_FLAG);
}
void JT808MessageHandler::PacketCheckSumFlag(JT808Packet *thepacket)
{
    unsigned char checksum = 0;
    
    
    for(JT808Packet::iterator it = thepacket->begin(); it != thepacket->end(); it ++)
    {
        checksum ^= (*it);
    }
    
    thepacket->push_back(checksum);

    for(JT808Packet::iterator it = thepacket->begin(); it != thepacket->end(); it ++)
    {
	    if(*it==JT808_PACKET_FLAG)
	    {
		    *it=JT808_PACKET_ESCAPE;
		    if(++it==thepacket->end())
		    {
			    thepacket->push_back(JT808Value(0x02));
			    break;
		    }
		    else
			    it=thepacket->insert(it,JT808Value(0x02));
		    continue;
	    }
	    if(*it==JT808_PACKET_ESCAPE)
	    {
		    if(++it==thepacket->end())
		    {
			    thepacket->push_back(JT808Value(0x01));
			    break;
		    }
		    else
			    it=thepacket->insert(it,JT808Value(0x01));
	    }
    }

    thepacket->push_front(JT808_PACKET_FLAG);
    thepacket->push_back(JT808_PACKET_FLAG);
}
void JT808MessageHandler::RSendMessage(unsigned short req, map<int, int> &idlist)
{
	pthread_mutex_lock(&m_queueMutex);
	if(m_waitingQueue.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::ReUp empty.");
		pthread_mutex_unlock(&m_queueMutex);
		return;
	}

	StruJT808Packet *packet = *(m_waitingQueue.begin());
	if((packet->retry >= 1) && (packet->seq == req))
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ReUp correct resp.");
		map<int, int>::const_iterator map_it= idlist.begin();
		while(map_it != idlist.end())
		{
			packet->segment_id.push_back( map_it->first - 1);
			++map_it;
		}
		if(m_waitingQueue.size() > 0)
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ReUp start queue timer");
			StartTimer(m_queueTimer);
		}
	}
	else
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PopQueue incorrect resp(%d, %04x).", packet->seq, packet->id);
	}
    	pthread_mutex_unlock(&m_queueMutex);

}
