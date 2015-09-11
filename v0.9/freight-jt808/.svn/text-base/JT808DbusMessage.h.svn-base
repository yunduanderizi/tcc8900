#ifndef _JT808_DBUS_MESSAGE_H_
#define _JT808_DBUS_MESSAGE_H_

#include <stdio.h>
#include <dbusServer.h>
#include <dbusApplication.h>

#define JT808_DBUS_SERVER_ID                (char*)"com.innov.jt808"
#define JT808_DBUS_SERVER_OBJ               (char*)"/jt808"

#define JT808_DBUS_REQ_ID_REGISTER                  6000
#define JT808_DBUS_REP_ID_REGISTER                  6001
#define JT808_DBUS_NTF_ID_REGISTER                  6002
#define JT808_DBUS_NTF_ID_LOGOFF                    6006

#define JT808_DBUS_REQ_ID_AUTHENTICATION         6003
#define JT808_DBUS_REP_ID_AUTHENTICATION         6004
#define JT808_DBUS_NTF_ID_AUTHENTICATION          6005

#define JT808_DBUS_NTF_ID_SETTING_UPDATE            6010

#define JT808_DBUS_REQ_ID_LOC_REPORT                6020
#define JT808_DBUS_REP_ID_LOC_REPORT                6021
#define JT808_DBUS_NTF_ID_LOC_REPORT                6022
#define JT808_DBUS_NTF_ID_TEMP_LOC_REPORT           6023

#define JT808_DBUS_NTF_ID_TEXT                      6030

#define JT808_DBUS_NTF_ID_QUESTION                  6040

#define JT808_DBUS_REQ_ID_ANSWER                    6050
#define JT808_DBUS_REP_ID_ANSWER                    6051

#define JT808_DBUS_NTF_ID_CALL_BACK                 6060

#define JT808_DBUS_NTF_ID_CONTACTS_UPDATE           6070

#define JT808_DBUS_NTF_ID_CIRCLE_FENCE_UPDATE       6080
#define JT808_DBUS_NTF_ID_RECTANGLE_FENCE_UPDATE    6081
#define JT808_DBUS_NTF_ID_POLYGON_FENCE_UPDATE      6082
#define JT808_DBUS_NTF_ID_LINES_FENCE_UPDATE        6083


#define JT808_DBUS_REQ_ID_CAMERA                    6090
#define JT808_DBUS_REP_ID_CAMERA                    6091
#define JT808_DBUS_NTF_ID_CAMERA                    6092
#define JT808_DBUS_REQ_ID_RECORD                    6093
#define JT808_DBUS_REP_ID_RECORD                    6094
#define JT808_DBUS_NTF_ID_RECORD                    6095

#define JT808_DBUS_REQ_ID_SETTING_MODIFY            6100
#define JT808_DBUS_REP_ID_SETTING_MODIFY            6101

#define JT808_DBUS_NTF_ID_CONN_STATUS               6200

#define JT808_DBUS_REQ_ID_DRIVER_INFO               6300
#define JT808_DBUS_REP_ID_DRIVER_INFO               6301

#define JT808_DBUS_NTF_ID_PARKING_TEST              6400

#define JT808_DBUS_REQ_ID_DATA_TRANSMIT             6401
#define JT808_DBUS_NTF_ID_DATA_TRANSMIT             6402

#define JT808_DBUS_REQ_ID_VIDEO			    6500
#define JT808_DBUS_REQ_ID_EVENTR		    6600

#define JT808_DBUS_NTF_ID_MULTI_POINT               6096

#define PHONE_RESET                                     (4)
#define PHONE_RESTORE_DEFAULT                           (5)
#define PHONE_SHORTCUT_NUMBER                           (6)
#define PHONE_PARAS_SET                                 (7)
#define PHONE_ALARM                                     (8)

class JT808RegisterDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808RegisterDbusReqC)

public:
    JT808RegisterDbusReqC():DBusRequestC(JT808_DBUS_REQ_ID_REGISTER){}
    void print(FILE *fp) {fprintf(fp,"\tresult=%d\r\n",result);}
    int result; //0:logoff 1:register 2:fast-register 3:gzip 4:phone:reset 5:phone:restore 6:phone:shortcut-number 7:phone PARAS-SET 8:alarm 11:off-net 12 encrypt_fast-register 13: on-net
    unsigned char buf[12];
};

class JT808RegisterDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT808RegisterDbusRepC)

public:
    JT808RegisterDbusRepC():DBusReplyC(JT808_DBUS_REP_ID_REGISTER){}
    
    void print(FILE *) {}
};

class JT808RegisterDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808RegisterDbusNotifyC)

public:
    JT808RegisterDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_REGISTER) {}
        
    void print(FILE *fp) {fprintf(fp,"\tresult=%d\r\n",result);}
    
    int result; // 0: Succeed; other: fail
};

class JT808LogoffDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808LogoffDbusNotifyC)

public:
    JT808LogoffDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_LOGOFF) {}
        
    void print(FILE *fp) {fprintf(fp,"\tresult=%d\r\n",result);}
    
    int result; // 0: Succeed; other: fail
};

class JT808AuthenticationDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808AuthenticationDbusReqC)

public:
    JT808AuthenticationDbusReqC():DBusRequestC(JT808_DBUS_REQ_ID_AUTHENTICATION){}
    void print(FILE *) {}
};

class JT808AuthenticationDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT808AuthenticationDbusRepC)

public:
    JT808AuthenticationDbusRepC():DBusReplyC(JT808_DBUS_REP_ID_AUTHENTICATION){}
    void print(FILE *) {}
};

class JT808AuthenticationDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808AuthenticationDbusNotifyC)

public:
    JT808AuthenticationDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_AUTHENTICATION) {}
        
    void print(FILE *fp) {fprintf(fp,"\tresult=%d\r\n",result);}
    
    int result; // 0: Succeed; other: fail
};

class JT808SettingUpdateDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808SettingUpdateDbusNotifyC)

public:
    JT808SettingUpdateDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_SETTING_UPDATE) {}
        
    void print(FILE *) {}
};

class JT808LocReportDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808LocReportDbusReqC)

public:
    JT808LocReportDbusReqC():DBusRequestC(JT808_DBUS_REQ_ID_LOC_REPORT){}
    void print(FILE *fp) {fprintf(fp,"\ttableIndex=[%lld]\r\n", m_tableIndex);}
    
    long long m_tableIndex;
};

class JT808LocReportDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT808LocReportDbusRepC)

public:
    JT808LocReportDbusRepC():DBusReplyC(JT808_DBUS_REP_ID_LOC_REPORT){}
    
    void print(FILE *) {}
};


class JT808LocReportDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808LocReportDbusNotifyC)

public:
    JT808LocReportDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_LOC_REPORT) {}
        
    void print(FILE *) {}
    int confirm_alarm;
};

class JT808TempLocReportDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808TempLocReportDbusNotifyC)

public:
    JT808TempLocReportDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_TEMP_LOC_REPORT) {}
        
    void print(FILE *fp)
    {
        fprintf(fp,"\tinterval=[%d]\r\n", interval);
        fprintf(fp,"\tvalidity=[%d]\r\n", validity);
    }
    int interval;
    int validity;
};

class JT808TextDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808TextDbusNotifyC)

public:
    JT808TextDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_TEXT)
    {
        m_text = NULL;
        DBUS_STRING_MEMBER(m_text);
    }

    ~JT808TextDbusNotifyC()
    {
        if(m_text)
        {
            free(m_text);
        }
    }
    
    void print(FILE *fp)
    {
        fprintf(fp,"\tm_emergency=[%d]\r\n", m_emergency);
        fprintf(fp,"\tm_lcd=[%d]\r\n", m_lcd);
        fprintf(fp,"\tm_tts=[%d]\r\n", m_tts);
        fprintf(fp,"\tm_bigscreen=[%d]\r\n", m_bigscreen);
        fprintf(fp,"\tm_naviorcan=[%d]\r\n", m_naviorcan);
        if(m_text)
        {
            fprintf(fp,"\tm_text=[%s]\r\n", m_text);
        }
        else
        {
            fprintf(fp,"\tm_text=[NULL]\r\n");
        }
    }
    
    int m_emergency; //1: Emergency
    int m_lcd;       //1: Display on LCD
    int m_tts;       //1: Read by TTS
    int m_bigscreen; //1: Display on BIG screen. ignore now.
    int m_naviorcan; //1: navigation 0:can
    char *m_text;
};

#define JT808_MAX_QUESTION_NUMBER               4

class JT808QuestionDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808QuestionDbusNotifyC)

public:
    JT808QuestionDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_QUESTION)
    {
        m_text = NULL;
        m_answer1 = NULL;
        m_answer2 = NULL;
        m_answer3 = NULL;
        m_answer4 = NULL;
        DBUS_STRING_MEMBER(m_text);
        DBUS_STRING_MEMBER(m_answer1);
        DBUS_STRING_MEMBER(m_answer2);
        DBUS_STRING_MEMBER(m_answer3);
        DBUS_STRING_MEMBER(m_answer4);
    }

    ~JT808QuestionDbusNotifyC()
    {
        if(m_text)
        {
            free(m_text);
        }
        
        if(m_answer1)
        {
            free(m_answer1);
        }

        if(m_answer2)
        {
            free(m_answer2);
        }

        if(m_answer3)
        {
            free(m_answer3);
        }
	
        if(m_answer4)
        {
            free(m_answer4);
        }
    }
    
    void print(FILE *fp)
    {
        fprintf(fp,"\tm_seq=[%d]\r\n", m_seq);
        fprintf(fp,"\tm_emergency=[%d]\r\n", m_emergency);
        fprintf(fp,"\tm_lcd=[%d]\r\n", m_lcd);
        fprintf(fp,"\tm_tts=[%d]\r\n", m_tts);
        fprintf(fp,"\tm_bigscreen=[%d]\r\n", m_bigscreen);
        if(m_text)
        {
            fprintf(fp,"\tm_text=[%s]\r\n", m_text);
        }
        else
        {
            fprintf(fp,"\tm_text=[NULL]\r\n");
        }
        fprintf(fp,"\tm_answerNumber=[%d]\r\n", m_answerNumber);
        fprintf(fp,"\tm_answer1_id=[%d]\r\n", m_answer1_id);
        if(m_answer1)
        {
            fprintf(fp,"\tm_answer1=[%s]\r\n", m_answer1);
        }
        else
        {
            fprintf(fp,"\tm_answer1=[NULL]\r\n");
        }
        fprintf(fp,"\tm_answer2_id=[%d]\r\n", m_answer2_id);
        if(m_answer2)
        {
            fprintf(fp,"\tm_answer2=[%s]\r\n", m_answer2);
        }
        else
        {
            fprintf(fp,"\tm_answer2=[NULL]\r\n");
        }
        fprintf(fp,"\tm_answer3_id=[%d]\r\n", m_answer3_id);
        if(m_answer3)
        {
            fprintf(fp,"\tm_answer3=[%s]\r\n", m_answer3);
        }
        else
        {
            fprintf(fp,"\tm_answer3=[NULL]\r\n");
        }
        fprintf(fp,"\tm_answer4_id=[%d]\r\n", m_answer4_id);
        if(m_answer4)
        {
            fprintf(fp,"\tm_answer4=[%s]\r\n", m_answer4);
        }
        else
        {
            fprintf(fp,"\tm_answer4=[NULL]\r\n");
        }
    }

    int m_seq;       // The sequence of the quesition. should contain it in the answer req message
    int m_emergency; //1: Emergency
    int m_lcd;       //1: Display on LCD
    int m_tts;       //1: Read by TTS
    int m_bigscreen; //1: Display on BIG screen. ignore now.
    char *m_text;
    int m_answerNumber;
    int m_answer1_id;
    char *m_answer1;
    int m_answer2_id;
    char *m_answer2;
    int m_answer3_id;
    char *m_answer3;
    int m_answer4_id;
    char *m_answer4;
};

class JT808AnswerDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808AnswerDbusReqC)

public:
    JT808AnswerDbusReqC():DBusRequestC(JT808_DBUS_REQ_ID_ANSWER){}
    void print(FILE *fp)
    {
        fprintf(fp,"\tm_seq=[%d]\r\n", m_seq);
        fprintf(fp,"\tm_answer_id=[%d]\r\n", m_answer_id);
    }
    
    int m_seq;
    int m_answer_id;
};

class JT808AnswerDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT808AnswerDbusRepC)

public:
    JT808AnswerDbusRepC():DBusReplyC(JT808_DBUS_REP_ID_ANSWER){}
    
    void print(FILE *) {}
};


class JT808CallBackDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808CallBackDbusNotifyC)

public:
    JT808CallBackDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_CALL_BACK)
    {
        m_phone = NULL;
        DBUS_STRING_MEMBER(m_phone);
    }

    ~JT808CallBackDbusNotifyC()
    {
        if(m_phone)
        {
            free(m_phone);
        }
    }
    
    void print(FILE *fp)
    {
        fprintf(fp,"\tm_monitor=[%d]\r\n", m_monitor);
        if(m_phone)
        {
            fprintf(fp,"\tm_phone=[%s]\r\n", m_phone);
        }
        else
        {
            fprintf(fp,"\tm_phone=[NULL]\r\n");
        }
    }
    
    int m_monitor; //0: Monitor, 1: tel num 2: phone in no net 3: phone out no net 4:restore factory 5: wireless updata
    char *m_phone;
};

class JT808ContactsUpdateDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808ContactsUpdateDbusNotifyC)

public:
    JT808ContactsUpdateDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_CONTACTS_UPDATE) {}
        
    void print(FILE *) {}
};

class JT808CircleFenceUpdateDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808CircleFenceUpdateDbusNotifyC)

public:
    JT808CircleFenceUpdateDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_CIRCLE_FENCE_UPDATE) {}
        
    void print(FILE *) {}
};

class JT808RectangleFenceUpdateDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808RectangleFenceUpdateDbusNotifyC)

public:
    JT808RectangleFenceUpdateDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_RECTANGLE_FENCE_UPDATE) {}
        
    void print(FILE *) {}
};

class JT808PolygonFenceUpdateDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808PolygonFenceUpdateDbusNotifyC)

public:
    JT808PolygonFenceUpdateDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_POLYGON_FENCE_UPDATE) {}
        
    void print(FILE *) {}
};

class JT808LinesFenceUpdateDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808LinesFenceUpdateDbusNotifyC)

public:
    JT808LinesFenceUpdateDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_LINES_FENCE_UPDATE) {}
        
    void print(FILE *) {}
};

class JT808CameraDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808CameraDbusReqC)

public:
    JT808CameraDbusReqC():DBusRequestC(JT808_DBUS_REQ_ID_CAMERA){}
    void print(FILE *fp) {fprintf(fp,"\ttableIndex=[%lld]\r\n", m_tableIndex);}
    
    long long m_tableIndex;
    int trans_flag;//0, no tran;1 trans.
    long long stop_time;
};

class JT808CameraDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT808CameraDbusRepC)

public:
    JT808CameraDbusRepC():DBusReplyC(JT808_DBUS_REP_ID_CAMERA){}
    
    void print(FILE *) {}
};

class JT808CameraDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808CameraDbusNotifyC)

public:
    JT808CameraDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_CAMERA)
    {
    }

    void print(FILE *fp)
    {
        fprintf(fp,"\tm_channel=[%d]\r\n", m_channel);
        fprintf(fp,"\tm_command=[%d]\r\n", m_command);
        fprintf(fp,"\tm_interval=[%d]\r\n", m_interval);
        fprintf(fp,"\tm_flag=[%d]\r\n", m_flag);
        fprintf(fp,"\tm_resolution=[%d]\r\n", m_resolution);
        fprintf(fp,"\tm_quality=[%d]\r\n", m_quality);
        fprintf(fp,"\tm_brightness=[%d]\r\n", m_brightness);
        fprintf(fp,"\tm_saturation=[%d]\r\n", m_saturation);
        fprintf(fp,"\tm_chroma=[%d]\r\n", m_chroma);
    }
    
    int m_channel;
    int m_command; // 0: stop, 0xFFFF: Video, Other: Number of Pictures
    int m_interval;
    int m_flag;
    int m_resolution;
    int m_quality;
    int m_brightness;
    int m_saturation;
    int m_chroma;
};

class JT808VideoDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808VideoDbusReqC)

public:
    JT808VideoDbusReqC():DBusRequestC(JT808_DBUS_REQ_ID_VIDEO){}
    void print(FILE *fp) {fprintf(fp,"\ttableIndex=[%lld]\r\n", m_tableIndex);}
    
    long long m_tableIndex;
};

class JT808EventReportDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808EventReportDbusReqC)

public:
    JT808EventReportDbusReqC():DBusRequestC(JT808_DBUS_REQ_ID_EVENTR){}
    void print(FILE *fp) {fprintf(fp,"\tevent report\r\n");}
    
};

class JT808RecordDbusReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808RecordDbusReqC)

public:
    JT808RecordDbusReqC():DBusRequestC(JT808_DBUS_REQ_ID_RECORD){}
    void print(FILE *fp) {fprintf(fp,"\ttableIndex=[%lld]\r\n", m_tableIndex);}
    
    long long m_tableIndex;
};

class JT808RecordDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT808RecordDbusRepC)

public:
    JT808RecordDbusRepC():DBusReplyC(JT808_DBUS_REP_ID_RECORD){}
    
    void print(FILE *) {}
};

class JT808RecordDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808RecordDbusNotifyC)

public:
    JT808RecordDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_RECORD)
    {
    }

    void print(FILE *fp)
    {
        fprintf(fp,"\tm_command=[%d]\r\n", m_command);
        fprintf(fp,"\tm_time=[%d]\r\n", m_time);
        fprintf(fp,"\tm_flag=[%d]\r\n", m_flag);
        fprintf(fp,"\tm_sample=[%d]\r\n", m_sample);
    }
    
    int m_command; // 0: stop, 0xFFFF: Video, Other: Number of Pictures
    int m_time;
    int m_flag;
    int m_sample;
};

class JT808ModifySettingReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808ModifySettingReqC)

public:
    JT808ModifySettingReqC():DBusRequestC(JT808_DBUS_REQ_ID_SETTING_MODIFY)
    {
        m_main_server = NULL;
        m_second_server = NULL;
        m_auth = NULL;
        DBUS_STRING_MEMBER(m_main_server);
        DBUS_STRING_MEMBER(m_second_server);
        DBUS_STRING_MEMBER(m_auth);
    }
    ~JT808ModifySettingReqC()
    {
        if(m_main_server)
        {
            free(m_main_server);
        }
        if(m_second_server)
        {
            free(m_second_server);
        }
        if(m_auth)
        {
            free(m_auth);
        }
    }
    void print(FILE *fp)
    {
        if(m_main_server)
        {
            fprintf(fp,"\tm_main_server=[%s]\r\n", m_main_server);
        }
        else
        {
            fprintf(fp,"\tm_main_server=[NULL]\r\n");
        }
        if(m_second_server)
        {
            fprintf(fp,"\tm_second_server=[%s]\r\n", m_second_server);
        }
        else
        {
            fprintf(fp,"\tm_second_server=[NULL]\r\n");
        }
        fprintf(fp,"\tm_port=[%d]\r\n", m_port);
        if(m_auth)
        {
            fprintf(fp,"\tm_auth=[%s]\r\n", m_auth);
        }
        else
        {
            fprintf(fp,"\tm_auth=[NULL]\r\n");
        }
    }
    
    char *m_main_server;
    char *m_second_server;
    int m_port;
    int m_port2;
    char *m_auth;
};

class JT808ModifySettingDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT808ModifySettingDbusRepC)

public:
    JT808ModifySettingDbusRepC():DBusReplyC(JT808_DBUS_REP_ID_SETTING_MODIFY){}
    
    void print(FILE *fp)
    {
        fprintf(fp,"\tm_result=[%d]\r\n", m_result);
    }
    int m_result;
};

class JT808ConnStatusDbusNotifyC: public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808ConnStatusDbusNotifyC)

public:
    JT808ConnStatusDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_CONN_STATUS)
    {
    }

    void print(FILE *fp)
    {
        fprintf(fp,"\tm_main_status=[%d]\r\n", m_main_status);
        fprintf(fp,"\tm_second_status=[%d]\r\n", m_second_status);
    }
    
    // 0: 断开 1：正在连接 2：已连接
    int m_main_status; //主服务器
    int m_second_status; //备服务器
};

class JT808DriverInfoReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808DriverInfoReqC)

public:
    JT808DriverInfoReqC():DBusRequestC(JT808_DBUS_REQ_ID_DRIVER_INFO)
    {
        ;
    }
    void print(FILE *fp)
    {
        fprintf(fp,"\tm_time_index=[%d]\r\n", m_time_index);
    }
    
    int m_time_index;
};

class JT808DriverInfoDbusRepC : public DBusReplyC
{
    DBUS_REPLY_OBJECT(JT808DriverInfoDbusRepC)

public:
    JT808DriverInfoDbusRepC():DBusReplyC(JT808_DBUS_REP_ID_DRIVER_INFO){}
    
    void print(FILE *)
    {
        ;
    }
};

class JT808ParkingDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808ParkingDbusNotifyC)

public:
    JT808ParkingDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_PARKING_TEST) {}
        
    void print(FILE *fp) {fprintf(fp,"\tflag=%d\r\n",flag);}
    
    int flag; // 1: 超时停车开始; 2:疲劳检测开始 3:can 4:解除超时停车 5：单次位置 6:更新总里程
};

class JT808DataTransmitReqC : public DBusRequestC
{
    DBUS_REQUEST_OBJECT(JT808DataTransmitReqC)

public:
    JT808DataTransmitReqC():DBusRequestC(JT808_DBUS_REQ_ID_DATA_TRANSMIT){}

    void print(FILE *fp)
    {
        fprintf(fp,"\ttype=[%d]\r\n", type);
    }
    
    int type;       //0x0B 道路运输IC卡信息 0xF0 elec-bill
    int total;
    unsigned char buf[512];
};

class JT808DataTransmitNotifyC : public DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(JT808DataTransmitNotifyC)

public:
    JT808DataTransmitNotifyC():DBusNotifyC(JT808_DBUS_NTF_ID_DATA_TRANSMIT){}
    
    void print(FILE *fp)
    {
        fprintf(fp,"\ttype=[%d]\r\n", type);
    }
    int type;
    int errnum;
    int total;
    unsigned char buf[512];
};

class JT808MultiPointDbusNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(JT808MultiPointDbusNotifyC)

	public:
		JT808MultiPointDbusNotifyC() : DBusNotifyC(JT808_DBUS_NTF_ID_MULTI_POINT)
	{   
	}   

		void print(FILE *)
		{   
			;   
		}   

		int m_result;
};
#endif
