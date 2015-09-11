#ifndef _JT808_MESSAGE_HANLDER_H_
#define _JT808_MESSAGE_HANLDER_H_

#include "ConnectionManager.h"
#include "GeneralTimer.h"
#include <pthread.h>
#include <boost/circular_buffer.hpp>
#include <vector>

#define JT808_MAX_WAITING_QUEUE             20000
#define JT808_MAX_WAITING_QUEUE_LOCATION    20000
#define JT808_QUEUE_TIME_OUT                2
#define JT808_REGISTER_TIME_OUT             600 
#define JT808_RETRY_REGISTER_TIME	    30 
#define JT808_MAX_PACKET_SIZE               1038
#define JT808_MIN_PACKET_SIZE               15
#define JT808_SEGMENT_SIZE                  1023

//#define JT808_FILE_SEGMENT_SIZE             512      
#define JT808_FILE_SEGMENT_SIZE             950

#define JT808_HELLO_TIMER_NAME              "JT808Hello"
#define JT808_QUEUE_TIMER_NAME              "JT808Queue"
#define JT808_RETRY_TIMER_NAME              "JT808Retry"
#define JT808_RETRY_REGISTER_NAME           "JT808Register"
#define JT808_RETRY_TIMER_NAME_LOCATION     "JT808RetryLocation"
#define JT808_DEAD_ZONE_TIMER		    "JT808DeadZone"

using namespace std;

typedef boost::circular_buffer_space_optimized<unsigned char> JT808Packet;
typedef boost::circular_buffer_space_optimized<unsigned char>::value_type JT808Value;
typedef vector<JT808Packet *>::iterator JT808_Vector_Iterator ;
typedef struct
{
    unsigned short seq;
    unsigned short id;
    int retry;
    GeneralTimerItem *timer;
    JT808Packet packet_bak;
    JT808Packet packet;
    vector<JT808Packet *> packetarray;
    vector<int> segment_id;
    int send_index;
    int total_segment;
    bool reply;
}StruJT808Packet;

typedef boost::circular_buffer<StruJT808Packet *> JT808PacketFifo;

typedef enum
{
    JT808_CRYTO_NONE = 0x00,
    JT808_CRYTO_RSA = 0x01
}ENUM_JT808_CRYTO;

#define JT808_PACKET_FLAG              ((unsigned char)0x7E)
#define JT808_PACKET_ESCAPE            ((unsigned char)0x7D)

#define JT808_PACKET_TERM_COMMON_REP                0x0001
#define JT808_PACKET_ID_HELLO                       0x0002
#define JT808_PACKET_ID_REGISTER                    0x0100
#define JT808_PACKET_ID_QUICK_REGISTER              0x0110
#define JT808_PACKET_ID_REGISTER_REP                0x8100
#define JT808_PACKET_ID_LOGOUT                      0x0003
#define JT808_PACKET_RESEND_REQUEST  		    0x8003
#define JT808_PACKET_ID_AUTH                        0x0102
#define JT808_PACKET_PLATFORM_COMMON_REP            0x8001
#define JT808_PACKET_SET_PARAMETERS                 0x8103
#define JT808_PACKET_QUERY_PARAMETERS               0x8104
#define JT808_PACKET_QUERY_PARAMETERS_REP           0x0104
#define JT808_PACKET_TERM_CONTROL                   0x8105
#define JT808_PACKET_QUERY_LIST_PARAMETERS          0x8106
#define JT808_PACKET_QUERY_ATTRIBUTE                0x8107
#define JT808_PACKET_QUERY_ATTRIBUTE_REP            0x0107
#define JT808_PACKET_UPDATE_TERM 		    0x8108
#define JT808_PACKET_UPDATE_TERM_REP                0x0108
#define JT808_PACKET_LOC_REPORT                     0x0200
#define JT808_PACKET_LOC_QUERY                      0x8201
#define JT808_PACKET_LOC_QUERY_REP                  0x0201
#define JT808_PACKET_TEMP_LOC_CONTROL               0x8202
#define JT808_PACKET_MANUAL_CONFIRM		    0x8203
#define JT808_PACKET_TEXT_MESSAGE                   0x8300
#define JT808_PACKET_EVENT			    0x8301
#define JT808_PACKET_EVENT_REPORT		    0x0301
#define JT808_PACKET_QUESTION                       0x8302
#define JT808_PACKET_ANSWER                         0x0302
#define JT808_PACKET_CALL_BACK                      0x8400
#define JT808_PACKET_PHONE_BOOK                     0x8401
#define JT808_PACKET_CAR_CONTROL                    0x8500
#define JT808_PACKET_CAR_CONTROL_REP                0x0500
#define JT808_PACKET_SET_CIRCLE                     0x8600
#define JT808_PACKET_DELETE_CIRCLE                  0x8601
#define JT808_PACKET_SET_RECTANGLE                  0x8602
#define JT808_PACKET_DELETE_RECTANGLE               0x8603
#define JT808_PACKET_SET_POLYGON                    0x8604
#define JT808_PACKET_DELETE_POLYGON                 0x8605
#define JT808_PACKET_SET_LINE                       0x8606
#define JT808_PACKET_DELETE_LINE                    0x8607
#define JT808_PACKET_DATA_ACQ			    0x8700
#define JT808_PACKET_DATA_SEND			    0x0700
#define JT808_PACKET_PARA_SET			    0x8701
#define JT808_PACKET_ELEC_BILL			    0x0701
#define JT808_PACKET_QUERY_DRIVER_INFO              0x8702
#define JT808_PACKET_DRIVER_INFO                    0x0702
#define JT808_PACKET_LOC_MULTI_REPORT		    0x0704
#define JT808_PACKET_CAN_INFO                       0x0705
#define JT808_PACKET_MEDIA_INFO                     0x0800
#define JT808_PACKET_MEDIA_DATA                     0x0801
#define JT808_PACKET_MEDIA_DATA_RSP                 0x8800
#define JT808_PACKET_CAMERA_START                   0x8801
#define JT808_PACKET_CAMERA_START_RSP               0x0805
#define JT808_PACKET_MEDIA_INDEX_QUERY              0x8802
#define JT808_PACKET_MEDIA_INDEX_QUERY_RSP          0x0802
#define JT808_PACKET_MEDIA_UPLOAD                   0x8803
#define JT808_PACKET_RECORD_START                   0x8804
#define JT808_PACKET_MEDIA_SINGLE_INDEX_QUERY       0x8805
#define JT808_PACKET_DOWN_TRANSPARENT_TRANSMISSION  0x8900
#define JT808_PACKET_UP_TRANSPARENT_TRANSMISSION    0x0900
#define JT808_PACKET_UP_GZIP_TRANSMISSION    	    0x0901

#define JT808_GET_BYTE(buf, idx)       (buf[idx])
#define JT808_GET_SHORT(buf, idx)      (((buf[idx]) << 8) | (buf[(idx) + 1]))
#define JT808_GET_LONG(buf, idx)       (((buf[idx]) << 24) |  ((buf[(idx) + 1]) << 16) | ((buf[(idx) + 2]) << 8) | (buf[(idx) + 3]))

#define JT808_NEW_PACKET               (new JT808Packet(JT808Packet::capacity_type(JT808_MAX_PACKET_SIZE, JT808_MIN_PACKET_SIZE))) 
#define JT808_SEND_SEGMENT_NUM_ONCE                 1

class JT808MessageHelper;

class JT808MessageHandler : public ConnectionPacketHandlerInterface, public ConnectionEventHandlerInterface
{
public:
    JT808MessageHandler();
    ~JT808MessageHandler();
    virtual int ConnectionEventHandler(ENUM_CONNECTION_EVENT state, ENUM_CONNECTION_ID conn);
    virtual int PacketHandler(unsigned char *buf, int len);
    void HandleTimer(string &name, void *data);

//    void RefreshConfig();   
    void ModifyHelloTimer(int timeout);
    void ResetFactory();
    friend class JT808MessageHelper;
    int m_authorized;
    int m_authfailure;
    //int errornum_register; //注册失败的次数
    int quick_register; //注册失败的次数

private:
    inline StruJT808Packet *CreatePacket()
    {
        StruJT808Packet *packet = new StruJT808Packet();
        packet->packet.set_capacity(JT808Packet::capacity_type(JT808_MAX_PACKET_SIZE, JT808_MIN_PACKET_SIZE));
        packet->retry = 0;
        packet->timer = NULL;
        packet->reply = false;
        packet->packetarray.clear();
        packet->send_index = 0;
        packet->total_segment = 0;
        
        return packet;
    }
    
    inline void DestroyPacket(StruJT808Packet *packet)
    {
	    if(packet->timer != NULL)
	    {
		    DestroyTimer(packet->timer);
	    }

	    vector<JT808Packet*> &packetarray = packet->packetarray;
	    if(packetarray.size())
	    {
		    for(vector<JT808Packet *>::iterator it=packetarray.begin();it!=packetarray.end();)
		    {
			    delete packetarray[0];
			    it=packetarray.erase(it);
		    }
	    }
	    delete packet;
    }
    
    inline void PushByteFront(JT808Packet &packet, unsigned char byte)
    {
        packet.push_front(byte);
    }

    inline void PushWordFront(JT808Packet &packet, unsigned short word)
    {
        PushByteFront(packet, (unsigned char)(word & 0xFF));
        PushByteFront(packet, (unsigned char)((word >> 8) & 0xFF));
    }

    inline void PushLongFront(JT808Packet &packet, unsigned long word)
    {
        PushByteFront(packet, (unsigned char)(word & 0xFF));
        PushByteFront(packet, (unsigned char)((word >> 8) & 0xFF));
        PushByteFront(packet, (unsigned char)((word >> 16) & 0xFF));
        PushByteFront(packet, (unsigned char)((word >> 24) & 0xFF));
    }
    
    inline void PushByte(JT808Packet &packet, unsigned char byte)
    {
        packet.push_back(byte);
#if 0
        if(byte == JT808_PACKET_FLAG)
        {
            packet.push_back(JT808_PACKET_ESCAPE);
            packet.push_back(0x02);
            return;
        }
        packet.push_back(byte);
        if(byte == JT808_PACKET_ESCAPE)
        {
            packet.push_back(0x01);
        }
#endif
    }

    inline void PushWord(JT808Packet &packet, unsigned short word)
    {
        PushByte(packet, (unsigned char)((word >> 8) & 0xFF));
        PushByte(packet, (unsigned char)(word & 0xFF));
    }

    inline void PushLong(JT808Packet &packet, unsigned long word)
    {
        PushByte(packet, (unsigned char)((word >> 24) & 0xFF));
        PushByte(packet, (unsigned char)((word >> 16) & 0xFF));
        PushByte(packet, (unsigned char)((word >> 8) & 0xFF));
        PushByte(packet, (unsigned char)(word & 0xFF));
    }

    inline void PushString(JT808Packet &packet, string str)
    {
        for(int i = 0; i < (int)str.length(); i ++)
        {
            PushByte(packet, (unsigned char)(str[i]));
        }
        //packet.push_back(0x00);
    }

    
    inline void PushSequence(JT808Packet &packet, vector<unsigned char> &sequence, int len)
    {
        int seqlen = sequence.size();
        int offset = len - seqlen;
        if(offset < 0)
        {
            offset = 0;
        }
        
        for(int i = 0; i < len; i ++)
        {
            if(offset > i)
            {
                packet.push_back(0x00);
            }
            else
            {
                PushByte(packet, sequence[i - offset]);
            }
        }
    }

    inline void PushSequence(JT808Packet &packet, string &sequence, int len)
    {
        int seqlen = sequence.length();
        
        for(int i = 0; i < len; i ++)
        {
            if(seqlen >= i)
            {
                packet.push_back(sequence[i]);
            }
            else
            {
                PushByte(packet, 0x00);
            }
        }
    }
    
    void RefreshConfig();
    void GetPhoneNumber();
    
    void HandlePacket();
    void ProcessCommonReply(int start, int len);
    void ProcessRegisterReply(int start, int len);
    
    void PopQueue(int id, int seq);
    void PopQueue(int id);
    void ClearQueue();
    void SendQueue();
    int PushQueue(StruJT808Packet *packet);
    void ClearRxPacket()
    {
        m_packet_start = false;
        m_packet_escape = false;
        m_rxPacket.clear();
    }
    void HandleRetryTimer(void *data);
    void HandleRetryTimerLocation(void *data);

    void SendHello();
    void SendBody(StruJT808Packet *packet, unsigned short id, ENUM_JT808_CRYTO crypto);
    void PacketHead(StruJT808Packet *packet, unsigned short id, ENUM_JT808_CRYTO crypto);
    void PacketHead(StruJT808Packet *packet,JT808Packet *thepacket, unsigned short id, ENUM_JT808_CRYTO crypto,int index);
    void PacketCheckSumFlag(StruJT808Packet *packet);
    void PacketCheckSumFlag(JT808Packet *packet);
    void SendPacket(StruJT808Packet *packet);
    int SendContent(JT808Packet &content);
    void RSendMessage(unsigned short req, map<int, int> &idlist);
    
    GeneralTimerItem *m_helloTimer;
    GeneralTimerItem *m_queueTimer;
    GeneralTimerItem *m_retryTimer;
    GeneralTimerItem *m_retryRegister;
    GeneralTimerItem *m_retryTimer_location;
    GeneralTimerItem *m_Timer_Deadzone;
    unsigned short m_seq;
    pthread_mutex_t m_queueMutex;
    pthread_mutex_t m_packetMutex;
    
    JT808PacketFifo m_waitingQueue;
    JT808PacketFifo m_waitingQueue_reply;
    JT808PacketFifo m_waitingQueue_location;
    
    JT808Packet m_rxPacket;
    bool m_packet_start;
    bool m_packet_escape;
    
    int m_helloTimeout;
    int m_retryTimes;
    int m_retryTimeout;
    int m_DeadTimeout;
    
    int m_registered;
    int s_multipacket;
    unsigned char m_send_buffer[JT808_MAX_PACKET_SIZE];
    unsigned int m_send_len;

    int auth_failure;
    
    unsigned char m_phoneNumber[6];
};

#endif
