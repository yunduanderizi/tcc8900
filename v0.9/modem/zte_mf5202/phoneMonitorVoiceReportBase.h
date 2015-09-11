#ifndef phoneMonitorVoiceReportBase_H_20101223
#define phoneMonitorVoiceReportBase_H_20101223

#include <QEvent>
#include <QObject>

//#include "bsCommand.h"
#include "debugMonitor.h"

#ifdef  SEND_MSG_TO_NAVI
#include "NaviDbusMessage.h"
#endif

//³£Á¿ºê¶¨Òå
#define PHONE_MONITOR_NUMBER_LEN		32

#define NOT_FREIGHTAGE_MANAGER_PHONE_NUMBER     (0)
#define IS_FREIGHTAGE_MANAGER_PHONE_NUMBER      (1)


//¶ÌÏûÏ¢½ÓÊÕÀ´µÄ¶ÌÐÅ·¢ËÍÕßµÄºÅÂëºÍÄÚÈÝ
typedef struct _ReportSMSData
{
	char 	cNumber[PHONE_MONITOR_NUMBER_LEN];		//ÐÂ¶ÌÐÅµÄºÅÂë
	char	cSMSTime[50];							//ÐÂ¶ÌÐÅµÄÈÕÆÚ£¬¸ñÊ½£º
													// 05-07-09 17:18:12 Äê-ÔÂ-ÈÕ Ê±:·Ö:Ãë
	char	*SMSContent;		//¶ÌÐÅÄÚÈÝ£¬ÏÞÖÆ³¤¶È140
}ReportSMSData;


//此头文件必须放在ReportSMSData 后,里边会用到此定义 [zhenyubin 2015/09/09]
#include "smscmdprotocol.h"

typedef enum _SignalStrength
{
	Strength_0 = 0,
	Strength_1 = 1,
	Strength_2 = 2,
	Strength_3 = 3,
	Strength_4 = 4,
	Strength_5 = 5,	
}SignalStrength;

typedef enum _NetworkType
{
	No_Network = 0,
	Limited_Network,
	Network_2G,
	Network_3G,
	Network_4G,
}NetworkType;

typedef enum _ServiceType
{
	Phone_Only = 0,
	Net_Only,
	Phone_And_Net,
	No_Service,
}ServiceType;

//»Øµ÷º¯ÊýÀà£¬ÐèÒª¾ßÌåµÄÓ¦ÓÃÀ´ÊµÏÖ
class PhoneMonitorVoiceReportBase: public QObject
{
	Q_OBJECT
public:
	/* API for Application */
	virtual void newCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);		//ÐÂÀ´µçÍ¨Öª»Øµ÷º¯Êý
	virtual void callConnect( void );										//µç»°½ÓÍ¨Í¨Öª»Øµ÷º¯Êý
	virtual void callHangUp( int reason );									//µç»°¹Ò¶ÏÍ¨Öª»Øµ÷º¯Êý
	virtual void missCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN] );	//Î´½ÓÀ´µçÍ¨Öª»Øµ÷º¯Êý
	virtual void newSMS( ReportSMSData  SMSData );							//ÐÂ¶ÌÐÅÍ¨Öª»Øµ÷º¯Êý
	virtual void smsSendResult( int result );								//¶ÌÐÅ·¢ËÍ½á¹ûÍ¨Öª»Øµ÷º¯Êý
	virtual void smsSendWithIdResult(unsigned int id, int result);
	virtual void signalAndNetworkType(int signal,int networkType);									//ÓïÒôµç»°ÐÅºÅÇ¿¶ÈÍ¨Öª»Øµ÷º¯Êý
	virtual void signalAndNetworkTypeAndServiceType(SignalStrength signal, NetworkType network, ServiceType service,int signal_strength);
	virtual void ring(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);			//ÕñÁåÍ¨Öª»Øµ÷º¯Êý
	virtual void newFaceTimeCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]); //¿ÉÊÓµç»°À´µç»Øµ÷º¯Êý
	virtual void endFaceTimeCall(int reason);
	virtual void connectFaceTimeCall( void );
	virtual void missFaceTimeCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);
	virtual void hangupBySelf(void);//for bug:1058,HANGUP-BUTTON may appear after hanguping by self
	//virtual void bsCommandProcessResult(Bs_Command_Type type, int result, void *pRetData);
        virtual void sendMSGToNavi(char *SMSContent);
        virtual void newSMSToReg(char *SMSContent);
        virtual bool newSMSToSetup(ReportSMSData SMSData);
        virtual int setPhoneMute(bool val, int channelID);
        virtual int freightageManager(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);
    virtual bool newCommandSMS(SMS_CMD_STRUCT recv); //add [zhenyubin 2015/09/09]
	
private:
	void toNewCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);	//´¦ÀíÐÂÀ´µçÍ¨Öª
	void toCallConnect( void );										//´¦Àíµç»°½ÓÍ¨Í¨Öª
	void toCallHangUp( int reason ); 							//´¦Àíµç»°¹Ò¶ÏÍ¨Öª
	void toCallHangUpBySelf(void);//for bug:1058,HANGUP-BUTTON may appear after hanguping by self
	void toMissCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN] ); //´¦ÀíÎ´½ÓÀ´µçÍ¨Öª
	void toNewSMS( ReportSMSData	SMSData );						//´¦ÀíÐÂ¶ÌÐÅÍ¨Öª
	void toSmsSendResult( int result ); 							//´¦Àí¶ÌÐÅ·¢ËÍ½á¹ûÍ¨Öª
	void toSmsSendWithIdResult(unsigned int id, int result);
	void toSignalAndNetworkType(int signal,int networkType); 							//´¦ÀíÓïÒôµç»°ÐÅºÅÇ¿¶ÈÍ¨Öª
	void toSignalAndNetworkTypeAndServiceType(SignalStrength signal, NetworkType network, ServiceType service,int signal_strength);
	void toRing(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);		//´¦ÀíÕñÁåÍ¨Öª
	void toNewFaceTimeCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);//´¦Àí¿ÉÊÓµç»°À´µç»Øµ÷º¯Êý
	void toEndFaceTimeCall(int reason);
	void toFaceTimeCallConnect( void );
	void toFaceTimeMissCall(char cPhoneNumber[PHONE_MONITOR_NUMBER_LEN]);

private:
	bool event(QEvent *e);

	friend class PhoneMonitorVoiceInterface;
	friend class VideoPhoneMonitorC;
	friend class VoicePhoneWorkThreadC;
};

#endif

