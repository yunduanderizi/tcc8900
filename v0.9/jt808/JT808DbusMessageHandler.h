#ifndef _JT808_DBUS_MESSAGE_HANDLER_H_
#define _JT808_DBUS_MESSAGE_HANDLER_H_

typedef enum
{
    CONN_STATUS_NTF_DISCONNECT = 0,
    CONN_STATUS_NTF_CONNECTING,
    CONN_STATUS_NTF_CONNECTED
} ENUM_CONN_STATUS_NTF;

typedef struct
{
    int chan;
    int command;
    int interval;
    int flag;
    int resolution;
    int quality;
    int brightness;
    int contrast;
    int saturation;
    int chroma;
}StruCameraCommand;

typedef struct
{
    int command;
    int duration;
    int flag;
    int sample;
}StruRecordCommand;

void JT808RegisterServiceHandler(DBusRequestC *request, DBusReplyC *&reply);
void JT808AuthenticationServiceHandler(DBusRequestC *request, DBusReplyC *&reply);
void JT808LocReportServiceHandler(DBusRequestC *, DBusReplyC *&);
void JT808AnswerServiceHandler(DBusRequestC *, DBusReplyC *&);
void JT808ModifySettingServiceHandler(DBusRequestC *, DBusReplyC *&);
void JT808DriverInfoServiceHandler(DBusRequestC *, DBusReplyC *&);
void JT808CameraServiceHandler(DBusRequestC *, DBusReplyC *&);
void JT808EventServiceHandler(DBusRequestC *, DBusReplyC *&);
void JT808VideoServiceHandler(DBusRequestC *, DBusReplyC *&);
void JT808RecordServiceHandler(DBusRequestC *, DBusReplyC *&);
void JT808TransmissionDataServiceHandler(DBusRequestC *, DBusReplyC *&);

void SendJT808RegisterNotify(int result);
void SendJT808AuthenticationNotify(int result);
void SendJT808SettingUpdateNotify(int result);
void SendJT808GnssUpdateNotify(int ,int ,int ,int );
void SendJT808LocReportNotify(int );
void SendJT808TempLocReportNotify(int interval,int validity);
void SendJT808TextNotify(int emergency, int lcd, int tts, int bigscreen, int naviorcan, string &text);
void SendJT808QuestionNotify(int seq, int emergency, int lcd, int tts, int bigscreen, string &text, map<int, string> &answers);
void SendJT808CallBackNotify(int monitor, string &phone);
void SendJT808ContactsUpdateNotify();
void SendJT808FenceUpdateNotify(int eflag,int etype,int eid,int estate);
void SendJT808CircleFenceUpdateNotify();
void SendJT808RectangleFenceUpdateNotify();
void SendJT808PolygonFenceUpdateNotify();
void SendJT808LinesFenceUpdateNotify();
void SendJT808ConncetionStatusNotify(int main_status, int second_status);
void SendJT808CameraStartNotify(StruCameraCommand &command);
void SendJT808RecordStartNotify(StruRecordCommand &command);

#endif
