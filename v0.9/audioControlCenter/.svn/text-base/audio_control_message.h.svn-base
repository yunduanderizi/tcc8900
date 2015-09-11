/////////////////////////////////////////////////////////////////////////
// Header file for DBUS message for audio control
//
/////////////////////////////////////////////////////////////////////////

#ifndef AUDIO_CONTROL_MESSAGE_H
#define AUDIO_CONTROL_MESSAGE_H

#define TIMER_OUT_DBUS_AUDIOCTRL	(1000*5)

#define REQ_ID_AUDIO_CTRL 55
#define REP_ID_AUDIO_CTRL 55
#define NOTF_ID_AUDIO_CTRL 55

#define REQ_ID_AUDIO_SET 56
#define REP_ID_AUDIO_SET 56
#define NOTF_ID_AUDIO_SET 56

#define REQ_ID_AUDIO_GET 57
#define REP_ID_AUDIO_GET 57
#define NOTF_ID_AUDIO_GET 57

#define REQ_ID_AUDIO_SPEEDSETVOL 58
#define REP_ID_AUDIO_SPEEDSETVOL 58
#define NOTF_ID_AUDIO_SPEEDSETVOL 58

#include <dbusMessage.h>
#include "audio_control.h"

class AudioCtrlRequestC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(AudioCtrlRequestC)
public:
	AudioCtrlRequestC():DBusRequestC(REQ_ID_AUDIO_CTRL){}

	unsigned int devId;		// logic id of audio device
	int operId;	// operation: 1 - open; 2 - close; 3 - change volume
	int volume; // volume in percentage, 0 - min; 100 - max 100%
	int on_off; // switch of the audio device, 0 - off; 1 - on
	void print(FILE *fp);
};
class AudioCtrlReplyC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(AudioCtrlReplyC)

public:
	AudioCtrlReplyC():DBusReplyC(REP_ID_AUDIO_CTRL){}

	unsigned int devId;	// device id
	int volume;		// volume in percentage, 0 - min; 100 - max 100%
	int on_off;		// switch of the audio device, 0 - off; 1 - on
	unsigned char soundState;
	unsigned char otherState;
        void print(FILE *fp);
	
};
class AudioCtrlNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(AudioCtrlNotifyC)

public:
	AudioCtrlNotifyC():DBusNotifyC(NOTF_ID_AUDIO_CTRL){}
	
	unsigned int devId;		// logic id of audio device
	int volume; // volume in percentage, 0 - min; 100 - max 100%
	int on_off; //	 switch of the audio device, 0 - off; 1 - on	
	unsigned char soundStatus;
	unsigned char otherStatus;
        void print(FILE *fp);
}; 


class AudioSetRequestC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(AudioSetRequestC)
public:
	AudioSetRequestC():DBusRequestC(REQ_ID_AUDIO_SET){}

	unsigned int devId;		// logic id of audio device
	int operId;	// operation: 1 - open; 2 - close; 3 - change volume
	int volume; // volume in percentage, 0 - min; 100 - max 100%
	int on_off; // switch of the audio device, 0 - off; 1 - on
	unsigned char soundStatus;
	unsigned char otherStatus;	
	void print(FILE *fp);
};
class AudioSetReplyC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(AudioSetReplyC)

public:
	AudioSetReplyC():DBusReplyC(REP_ID_AUDIO_SET){}

	unsigned int devId;	// device id
	int volume;		// volume in percentage, 0 - min; 100 - max 100%
	int on_off;		// switch of the audio device, 0 - off; 1 - on
	unsigned char soundStatus;
	unsigned char otherStatus;
        void print(FILE *fp);
};
class AudioSetNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(AudioSetNotifyC)

public:
	AudioSetNotifyC():DBusNotifyC(NOTF_ID_AUDIO_SET){}
	
	unsigned int devId;		// logic id of audio device
	int volume; // volume in percentage, 0 - min; 100 - max 100%
	int on_off; //	 switch of the audio device, 0 - off; 1 - on	
	unsigned char soundStatus;
	unsigned char otherStatus;
        void print(FILE *fp);
}; 


class AudioGetRequestC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(AudioGetRequestC)
public:
	AudioGetRequestC():DBusRequestC(REQ_ID_AUDIO_GET){}

        void print(FILE *fp);

/*	unsigned int devId;		// logic id of audio device
	int operId;	// operation: 1 - open; 2 - close; 3 - change volume
	int volume; // volume in percentage, 0 - min; 100 - max 100%
	int on_off; // switch of the audio device, 0 - off; 1 - on
*/
};
class AudioGetReplyC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(AudioGetReplyC)

public:
	AudioGetReplyC():DBusReplyC(REP_ID_AUDIO_GET){}

/*	unsigned int devId;	// device id
	int volume;		// volume in percentage, 0 - min; 100 - max 100%
	int on_off;		// switch of the audio device, 0 - off; 1 - on
*/
	audioControlArray_t allInfo[IBAC_DEV_ID_MAX];

        void print(FILE *fp);
};
class AudioGetNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(AudioGetNotifyC)

public:
	AudioGetNotifyC():DBusNotifyC(NOTF_ID_AUDIO_GET){}
	
	unsigned int devId;		// logic id of audio device
	int volume; // volume in percentage, 0 - min; 100 - max 100%
	int on_off; //	 switch of the audio device, 0 - off; 1 - on	

        void print(FILE *fp);
}; 

class AudioSpeedSetVolRequestC:public DBusRequestC
{
	DBUS_REQUEST_OBJECT(AudioSpeedSetVolRequestC)
public:
	AudioSpeedSetVolRequestC():DBusRequestC(REQ_ID_AUDIO_SPEEDSETVOL){}
	
	int carSpeed;
	int carVol;
	void print(FILE *fp);
};
class AudioSpeedSetVolReplyC: public  DBusReplyC
{
	DBUS_REPLY_OBJECT(AudioSpeedSetVolReplyC)

public:
	AudioSpeedSetVolReplyC():DBusReplyC(REQ_ID_AUDIO_SPEEDSETVOL){}

	int carSpeed;
	int carVol;	
    void print(FILE *fp);
};
class AudioSpeedSetVolNotifyC: public  DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(AudioSpeedSetVolNotifyC)

public:
	AudioSpeedSetVolNotifyC():DBusNotifyC(REQ_ID_AUDIO_SPEEDSETVOL){}

	int carSpeed;
	int carVol;	
    void print(FILE *fp);
};

#endif
