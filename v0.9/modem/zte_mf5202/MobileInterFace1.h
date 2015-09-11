

#ifndef _MOBILEINTERFACE1_H_
#define _MOBILEINTERFACE1_H_


#include "MobileInterFace.h"

#ifndef WITH_NEW_AUDIO_CONTROL_SYSTEM
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
#include "audioControlLib.h"
#else
#include "audio_control_client.h"
#endif
#endif

//#include "sendDTMF.h"
#include "bsLocationUpload.h"
#include "bsGetTelephone.h"
//#include "common_config.h"

//#include "bsCommandProcessor.h"

#include "smscmdprotocol.h"

#define PHONE_MONITOR_IN_AUTOCALLBACK_MODE			(-100)		//current have been in auto callback mode

#define PHONE_MONITOR_INVALID_FILE                  (-18) //call ring file path is invalid
#define PHONE_MONITOR_SAVE_DATA_ERROR               (-19) //save music file path to database error
#define PHONE_MONITOR_GET_DATA_ERROR                (-20) //get music file path from database error

//#ifndef FILE_NAME
//#define FILE_NAME  INCAR_COMMON_MICRO_DATABASE_NAME_WITH_DIRECTORY	
//#endif

#define ENABLE_FLY_MODE

#ifdef ENABLE_FLY_MODE
typedef enum _FLY_MODE
{
        MIN_FUN = 0,
        FULL_FUN,
        DISABLE_RF_FUN,
        RESERVE_FUN,
}FLY_MODE;
#endif

class PhoneMonitorVoiceInterface1;
class Interface1PhoneReport: public PhoneMonitorVoiceReportBase
{
public:
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        Interface1PhoneReport(PhoneMonitorVoiceInterface1 *pFather, PhoneMonitorVoiceReportBase *reportBase);
#else
	Interface1PhoneReport(PhoneMonitorVoiceInterface1 *pFather);
#endif
	~Interface1PhoneReport();
	void newCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);
	void callConnect( void );
	void callHangUp( int reason );
	void missCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN] );
	void newSMS( ReportSMSData  SMSData );
	void smsSendResult( int result );
	void smsSendWithIdResult(unsigned int id, int result);
	void signalAndNetworkType( int signal,int networkType );
	void signalAndNetworkTypeAndServiceType(SignalStrength signal, NetworkType network, ServiceType service,int signal_strength);
	void ring(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);
	void newFaceTimeCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);
	void endFaceTimeCall(int reason);
	void connectFaceTimeCall(void);
	void missFaceTimeCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);
	void hangupBySelf(void);
	//void bsCommandProcessResult(Bs_Command_Type type, int result, void *pRetData);
        int closePhoneAudio(bool flag);

    //SMS短信回复 [zhenyubin 2015/09/09]
    void sendCommandSMSReply(SMS_CMD_STRUCT send);

	PhoneMonitorVoiceReportBase	*m_pCallback;
	QStringList					m_sCallbackNumber;
	bool							m_bIsAutoCallbackMode;


private:
	PhoneMonitorVoiceInterface1 *m_pFather;
        bool closeAudio;
#ifdef  SEND_MSG_TO_NAVI
        bool isSMSToNavi(char *SMSContent);
        int parseSMS(char *SMSContent);
#endif
        bool isSMSToReg(char *SMSContent);
        SmsCmdProtocol cmdProtocol; //SMS短信回复 [zhenyubin 2015/09/09]
        QString sendCmdPhone; //控制反馈查询设置短信发送后是否反馈给GUI消息 [zhenyubin 2015/09/09]
};


class PhoneAudioControlC;
class SmsAudioControlC;
class SendDTMFC;
class PhoneMonitorVoiceInterface1: public PhoneMonitorVoiceInterface
{
	Q_OBJECT
		
public:
	friend class Interface1PhoneReport;
	
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
        PhoneMonitorVoiceInterface1(PhoneMonitorVoiceReportBase *reportBase, BsLocationUploadC *pLocationUpload);
#else
#ifdef NEW_FUNCTION_AUDIO_CONTROL_1
	PhoneMonitorVoiceInterface1(BsLocationUploadC *pLocationUpload);
#else
	PhoneMonitorVoiceInterface1(AudioCtrlClientC *pAudioCtrlClient, BsLocationUploadC *pLocationUpload);
#endif
#endif
 	~PhoneMonitorVoiceInterface1();

public:
	int start(PhoneMonitorVoiceReportBase *reportHandle);
	int pickUp();
	int hangUp();
	int mutePhoneAudio();
	int unMutePhoneAudio();
	int dialCallback(const char* pNumber, const QStringList sCallbackNumber);
	int dial(const char* cpNumber);
	//int setNewCallRing(const char* cpNameWithPath);
	int queryModuleVersion(QString& moduleVerion);
	bool FaceTime_PickUp();
	bool FaceTime_HangUp();
	void reset3gModule();
	int searchLocationOfFriend(const char* pFriendNumber);
	int sendLocationOfMineToFriend(const char* pFriendNumber);
	int closePhoneAudio(bool flag){m_pInterface1PhoneReport->closePhoneAudio(flag);return 0;}
#ifdef ENABLE_FLY_MODE
        int setFlyMode(FLY_MODE mode);
#endif
	int set3GModuleAudioOutputVolume(int val);

    //SMS短信回复信号 [zhenyubin 2015/09/09]
    void sendCommandSMSReply(SMS_CMD_STRUCT send);

private:
	void timerEvent(QTimerEvent * event);
	int baseHangUp();
	void sendLocationMessage();

	void openPhoneAudio();
	void closePhoneAudio();
	
private slots:
	void slotFinishedSendDTMFMessage();
	//void slotBsCommandProcessResult(Bs_Command_Type type, int result, void* pData);
	void slotTelephoneChanged();
private:
	Interface1PhoneReport			*m_pInterface1PhoneReport;
        bool closeAudio;
#ifndef WITH_NEW_AUDIO_CONTROL_SYSTEM
#ifndef NEW_FUNCTION_AUDIO_CONTROL_1
	AudioCtrlClientC *m_pAudioCtrlClient;
#endif
#endif
	PhoneAudioControlC *m_pPhoneAudioControl;
	SmsAudioControlC *m_pSmsAudioControl;
	SendDTMFC *m_pSendDTMF;
	//BsLocationUploadC *m_pLocationUpload;
	//BsCommandProcessorC *m_pBsCommandProcessor;

	int m_iTimerForTimeout;
	QString	m_CallNumber;

	bool		m_bDTMFAlwaysSend;
	bool		m_bPhoneAudioOpen;
	QString	m_sServiceTelephone;
	QString	m_sHelpingTelephone;
	BsTelephoneList	m_BsTelephoneList;
};

#endif

