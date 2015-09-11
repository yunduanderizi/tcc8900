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

typedef void(*text)(int,int,int,int,int,string &);
typedef void(*question)(int,int,int,int,int,string &,map<int,string>&);
typedef void(*callback)(int,string&);
typedef void(*notify)(void);
typedef void(*regist)(int);
typedef void(*ConnectStatus)(int,int);

//text SendText;

//API
void JT808_Run(void);
void JT808RegisterServiceHandler(int flag, unsigned char phone[]);
void JT808AuthenticationServiceHandler(void);
void JT808AnswerServiceHandler(unsigned short seq, unsigned char answer);
void JT808ModifySettingServiceHandler(char *main_server, char *second_server, char *auth, int port);
void JT808MediaHandler(int type, int index, int flag);
void JT808EventServiceHandler(unsigned char up_event[], unsigned char num);

//callback
void SendJT808TextNotify(void(*func)(int emergency,int lcd,int tts,int bigscreen,int naviorcan,string &text));
void SendJT808QuestionNotify(void(*func)(int seq, int emergency,int lcd,int tts,int bigscreen,string &text, map<int,string>&answers));
void SendJT808CallBackNotify(void(*func)(int monitor, string &phone));
void SendJT808ContactsUpdateNotify(void(*func)(void));
void SendLogoffNotify(void(*func)(int result));
void SendRegisterNotify(void(*func)(int result));
void SendQuickRegisterNotify(void(*func)(int result));
void SendAuthenticationNotify(void(*func)(int result));
void SendJT808SettingUpdateNotify(void(*func)(void));
void SendJT808ConnectionStatusNotify(void(*func)(int main_status, int second_status));

#endif
