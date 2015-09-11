#ifndef _JT808_MESSAGE_HELPER_H_
#define _JT808_MESSAGE_HELPER_H_

#include "JT808MessageHandler.h"
#include "VDRTrackInfoTable.h"
#include "VDRUpdateTable.h"
#include "GB19056Proxy.h"
#include "../deviceManager/devicemanager.h"
#include <string>
#include <vector>
#include <map>

#define JT808_CHAR_SET          "GBK"
#define JT808_UPDATE_PATH       "/nand3/update"
#define WY_MAGIC                0xcafe

using namespace std;
typedef vector<unsigned char>::size_type vsize;
typedef vector<unsigned char>::iterator viter;
typedef enum
{
    ENUM_REGISTER_SUCCESS = 0,
    ENUM_REGISTER_CAR_REGSITERED,
    ENUM_REGISTER_NO_CAR,
    ENUM_REGISTER_TERM_REGISTERED,
    ENUM_REGISTER_NO_TERM
}ENUM_JT808_REGISTER_RESULT;

struct tagPOI
{
	double	dlan;
	double	dlon;
	string	strName;
};
typedef enum
{
    ENUM_JT808_COMMON_OK = 0,
    ENUM_JT808_COMMON_FAIL = 1,
    ENUM_JT808_COMMON_ERROR = 2,
    ENUM_JT808_COMMON_NOT_SUPPORT = 3
}ENUM_JT808_COMMON_RESP_RESULT;

typedef enum
{
    ENUM_JT808_TERM_CONTROL_UPGRADE = 1,
    ENUM_JT808_TERM_CONTROL_SERVER,
    ENUM_JT808_TERM_CONTROL_SHUTDOWN,
    ENUM_JT808_TERM_CONTROL_RESET,
    ENUM_JT808_TERM_CONTROL_RESET_FACTORY,
    ENUM_JT808_TERM_CONTROL_CLOSE_DATA,
    ENUM_JT808_TERM_CONTROL_CLOSE_NETWORK
}ENUM_JT808_TERM_CONTROL_CMD;

typedef enum
{
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_URL = 0,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_APN,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_USERNAME,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_PASSWORD,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_ADDRESS,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_TCPPORT,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_UDPPORT,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_MFRS,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_HARDWARE,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_FIRM,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_TIME,
    ENUM_JT808_TERM_CONTROL_SERVER_UPDATE_BUTT
}ENUM_JT808_TERM_CONTROL_SERVER_UPDATE;

typedef enum
{
    ENUM_JT808_TERM_CONTROL_SERVER_ITEM_CONTROL  = 0,
    ENUM_JT808_TERM_CONTROL_SERVER_ITEM_AUTH,
    ENUM_JT808_TERM_CONTROL_SERVER_ITEM_APN,
    ENUM_JT808_TERM_CONTROL_SERVER_ITEM_USERNAME,
    ENUM_JT808_TERM_CONTROL_SERVER_ITEM_PASSWORD,
    ENUM_JT808_TERM_CONTROL_SERVER_ITEM_ADDRESS,
    ENUM_JT808_TERM_CONTROL_SERVER_ITEM_TCPPORT,
    ENUM_JT808_TERM_CONTROL_SERVER_ITEM_UDPPORT,
    ENUM_JT808_TERM_CONTROL_SERVER_ITEM_TIME,
    ENUM_JT808_TERM_CONTROL_SERVER_ITEM_BUTT
}ENUM_JT808_TERM_CONTROL_SERVER_ITEM;

typedef enum
{
    ENUM_JT808_CONTACTS_CLEAR = 0,
    ENUM_JT808_CONTACTS_UPDATE,
    ENUM_JT808_CONTACTS_APPEND,
    ENUM_JT808_CONTACTS_MODIFY
}ENUM_JT808_CONTACTS_OPER;

typedef enum
{
    ENUM_JT808_FENCE_UPDATE = 0,
    ENUM_JT808_FENCE_APPEND,
    ENUM_JT808_FENCE_MODIFY
}ENUM_JT808_FENCE_OPER;

typedef enum
{
    ENUM_JT808_MEDIA_PHOTO = 0,
    ENUM_JT808_MEDIA_AUDIO,
    ENUM_JT808_MEDIA_VIDEO
}ENUM_JT808_MEDIA_TYPE;

typedef enum
{
    ENUM_JT808_EVENT_PLATFORM = 0,
    ENUM_JT808_EVENT_TIMER,
    ENUM_JT808_EVENT_ALARM,
    ENUM_JT808_EVENT_CRASH
}ENUM_JT808_EVENT_TYPE;

typedef enum
{
    ENUM_JT808_EVENTS_DELETEALL= 0,
    ENUM_JT808_EVENTS_UPDATE,
    ENUM_JT808_EVENTS_ADD,
    ENUM_JT808_EVENTS_CHANGE,
    ENUM_JT808_EVENTS_DELETESOME,
}ENUM_JT808_EVENTS_TYPE;

typedef enum
{
    ENUM_JT808_FILE_FORMAT_JPG = 0,
    ENUM_JT808_FILE_FORMAT_TIF,
    ENUM_JT808_FILE_FORMAT_MP3,
    ENUM_JT808_FILE_FORMAT_WAV,
    ENUM_JT808_FILE_FORMAT_WMV,
    ENUM_JT808_FILE_FORMAT_OTHER
}ENUM_JT808_FILE_FORMAT_TYPE;

typedef enum
{
    JT808_LOC_ADDITION_MILEAGE = 1,
    JT808_LOC_ADDITION_OILAGE,
    JT808_LOC_ADDITION_SPEED,
    JT808_LOC_ADDITION_EMERGENCY_ALRM,
    JT808_LOC_ADDITION_OVERAPEED = 0x11,
    JT808_LOC_ADDITION_FENCE,
    JT808_LOC_ADDITION_LINE_SHORT,
    JT808_LOC_ADDITION_STATE = 0x25,
    JT808_LOC_ADDITION_IO = 0x2A,
    JT808_LOC_ADDITION_AD,
    JT808_LOC_ADDITION_WIRELESS = 0x30,
    JT808_LOC_ADDITION_GNSS
}ENUM_JT808_LOC_ADDITION_ID;

typedef enum
{
    JT808_TT_GNSS = 0,
    JT808_TT_IC = 0x0B,
    JT808_TT_COM1= 0x41,
    JT808_TT_COM2= 0x42,
    JT808_TT_USERB= 0xF0,
    JT808_TT_USERE= 0xFF
}ENUM_JT808_TT_ID;

typedef struct
{
    int index;
    int type;
    int format;
    int event;
    int chan;
    int alarmflag;
    int statusflag;
    int latitude;
    int longitude;
    int altitude;
    int orient;
    int gpsspeed;
    vector<unsigned char> gpstime;
}StruMediaDataInfo;

class JT808MessageHelper
{
public:
    
    static void Init(JT808MessageHandler *handler);
    
    static void HandleRegisterResult(int result, string &auth);
    static void HandleCommonReply(int id, int result);
    static void HandlePacketResendRequest(JT808Packet &rec_packet, int body, int len, unsigned short seq);
    static void HandleSetParameters(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleRequryTermParameters(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleTermControl(JT808Packet &packet, int body, int len, unsigned short seq);
   // static void HandleTermUpdate(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleTermUpdate(JT808Packet &packet, int body,int len, unsigned short seq, unsigned short sum, unsigned short packetNum);
    static void HandleTempLocControl(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleAlarmConfirm(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleTextMessage(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleEvent(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleQuestion(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleCallBack(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandlePhoneBook(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleCarControl(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleSetCircle(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleDeleteCircle(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleSetRectangle(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleDeleteRectangle(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleSetPolygon(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleDeletePolygon(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleSetLine(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleDeleteLine(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleDataAcq(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleParaSet(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleCameraStart(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleRecordStart(JT808Packet &packet, int body, int len, unsigned short seq);
    static void HandleQueryMediaIndex(JT808Packet &rec_packet, int body, int len, unsigned short seq);
    static void HandleQueryMediaSingleIndex(JT808Packet &rec_packet, int body, int len, unsigned short seq);
    static void HandlePacketMediaDataRsp(JT808Packet &rec_packet, int body, int len, unsigned short seq);
    static void HandlePacketMediaUpload(JT808Packet &rec_packet, int body, int len, unsigned short seq);
    static void HandlePacketDownTransparentTransmission(JT808Packet &rec_packet, int body, int len, unsigned short seq);   

    static void SendXsjlErrorAck(unsigned short seq, unsigned short id, int result);
    static void SendAck(unsigned short seq, unsigned short id, int result);
    static int SendRegister(int ,unsigned char phone[]);
    static int SendLogout();
    static int SendAuth();
    static void push_deploy_para(JT808Packet & );
    static void SendParameters(unsigned short seq);
    static void SendParameters(unsigned short seq,map<int ,int>&idlist);
    static void SendAttribute(unsigned short seq);
    static void SendLocation(long long tableIndex);
    static void SendLocationbypass(struct LocStatusData &locData,int lock_door=WY_MAGIC);
    static void SendLocationMulti(struct LocStatusData *locData, int num, int lock_door=WY_MAGIC);
    static void SendLocationMultiO(void);
    static void SendLocationQueryResp(unsigned short seq);
    static void SendLocReport();
    static void SendAnswer(unsigned short seq, unsigned char answer);
    static void SendEvents(unsigned char * events, unsigned char num);
    static void SendElecBillInfo();
    static void SendDriverInfo(int timeIndex);
    static void SendCarControl(unsigned short seq,int flag);
    static void SendMedia(ENUM_JT808_MEDIA_TYPE type, int index, int flag, int stop_time);
    static void SendMedia8805(ENUM_JT808_MEDIA_TYPE type, int index, int flag, int id);
    static void SendTransmissionData(int type,int total, unsigned char * buf);
    static void SendCanData(int type,int total, unsigned char * buf);
    static void SendMediaFile(StruMediaDataInfo &info, string &file_name, map<int, int> &idlist);
    static void SendGzipPacket(unsigned char*);
    static void CreateMediaFilePacket(JT808Packet &packet, StruMediaDataInfo &info, int buflen,int flag_first);
    static void SetLocationPacket(JT808Packet &packet,struct LocStatusData &locData );
    static void SendUpdataResult(void);
    static void SendUpgradResult(void);
    static void Send070008(int , int);
    static void Send0700xx(int seq, int length, int ,vector<unsigned char> & );
    static void Send070009(int , int);
    static void Send070010(int , int);
    static void Send070011(int , int);
    static void Send070012(int , int);
    static void Send070013(int , int);
    static void Send070014(int , int);
    static void Send070015(int , int);
private:
    static void PackLocation(VDRLocStatus1Table *table, JT808Packet &packet,int lock_door=WY_MAGIC);
    static JT808MessageHandler *m_handler;
    static int seq_bak;
};
int GetData( const char* pData, int nLen, tagPOI* pPOI );
#endif
