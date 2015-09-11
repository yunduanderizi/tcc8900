#ifndef _MOBILEINTERFACE_H_
#define _MOBILEINTERFACE_H_

#include <QObject>
#include <QMutex>
#include <QApplication>
#include <QThread>
#include <QStringList>

#include "phoneMonitorVoiceReportBase.h"

//#define PHONE_MODULE_ZTEMF5200_AUTH

/********************************************************************************/

//通用错误定义
#define	PHONE_MONITOR_SUCCESS				(0)			//调用成功
#define	PHONE_MONITOR_FAIL					(-2)			//调用失败(尚不能明确具体原因)
#define	PHONE_MONITOR_DEV_INVALID			(-5)		//移动设备终端不可用
#define	PHONE_MONITOR_SIM_INVALID			(-6)		//SIM卡不可用
#define	PHONE_MONITOR_NET_INVALID			(-7)		//未能注册到网络
#define	PHONE_MONITOR_CALL_BUSY				(-8)		//正在通话过程中
#define	PHONE_MONITOR_OPER_TIMEOUT			(-9)		//操作超时，未能返回执行结果
#define	PHONE_MONITOR_WRONG_STATUS			(-10)		//错误的执行状态
#define	PHONE_MONITOR_CALLBACK_NULL			(-11)		//尚未注册回调函数
#define	PHONE_MONITOR_CMD_BUSY				(-12)		//上一个调用尚未完成
#define	PHONE_MONITOR_CALLBACK_INVALID		(-13)		//回调对象不可用
#define	PHONE_MONITOR_PHONE_NUMBER_INVALID	(-14)		//电话号码有误

#define	PHONE_MONITOR_INTENT_HANG			(-15)		//自己挂断电话
#define	PHONE_MONITOR_OPPOSITE_HANG			(-16)		//对方挂断电话
#define	PHONE_MONITOR_TIMEOUT_HANG			(-17)		//超时无人接听

#define	PHONE_MONITOR_CANNOT_GET_MY_LOCATION			(-18)		

#define PHONE_STATUS_NOT_INIT	0
#define PHONE_STATUS_INITTING	1
#define PHONE_STATUS_INITED	2

#define	PHONE_STATUS_CALLING	(3)


#define        CALL_IDLE                   0           
#define        CALL_CALLING                1           
#define        CALL_TALKING                2          
#define        CALL_COMING_CALL            3         



class PhoneConnectionC;
class VoicePhoneWorkThreadC;
class VoicePhoneInitThreadC;
class PhoneMonitorVoiceReportBase;
class VideoPhoneMonitorC;

class PhoneMonitorVoiceInterface: public QThread
{
	Q_OBJECT

public:
	PhoneMonitorVoiceInterface();
 	~PhoneMonitorVoiceInterface();

public:
	//移动终端语音电话短信功能管理
	int start(PhoneMonitorVoiceReportBase *reportHandle);						//启动并初始化电话监控模块
	int stop();																//停止使用电话监控模块
	
	//语音通话管理
	int dial(const char* cpNumber);	//拨号。
	int pickUp();						//接听
	int hangUp();						//挂断
	int rtDial(char cbyte);			//二次拨号。
	
	//短信管理
	int sendSMS( const char* pcContent, const char* cpNumber );					//发送普通短信息
	int sendSMSWithId(const char* pcContent, const char* pcNumber, unsigned int* id);

	//音量管理
	int	getSKVol();						//获取听筒音量
	int	setSKVol(int val);				//设置听筒音量
	int	getMicSatus();					//获取Mic状态
	int	muteMic();						//静音Mic
	int 	unMuteMic();					//取消静音MIC
	
	
	//其他功能
	int	getRSSI();						//获取信号强度
	signed  char 	getNetworkType(); //error:<0; 2:2G  3:3G
	int getNetworkTypeAndServiceType(NetworkType &net, ServiceType &service);
	int getSignalStrength(SignalStrength &signal,int &signal_strength);
	int getNetworkTypeAndServiceType(NetworkType &net);
	int getNetworkTypeAndServiceType(ServiceType &service);

	int queryCallState(void);//-1:error; 0:success but not be calling; PHONE_STATUS_CALLING:success and be calling
	int getCallState(void){return m_callStatus;}
#ifdef PHONE_MODULE_ZTEMF5200_AUTH
private:
	int authentication();
	void getMd5Data(char *input, char *output);
#endif	
//为与原接口兼容，暂时保留的接口部分
public:
	bool init3gfun();
	bool close3gfun();
	int FaceTime_Dial(char* cpNumber);		//拨打可视电话。返回值: -2表示正在通话中，-1表示拨号失败，0表示拨号成功
	bool FaceTime_PickUp();				//接听可视电话
	bool FaceTime_HangUp();			//挂断可视电话
	bool setMicVol(int val);

private:
	void run();

protected:
	PhoneConnectionC		*m_pPhoneConnection;

private:
	PhoneMonitorVoiceReportBase	*m_pCallback;

	int				 m_callStatus;
	char				 m_phoneNumber[30];

	int				 m_phoneStatus;
	QMutex          		 m_mutex;

	VoicePhoneInitThreadC		 *m_pInitThread;
	VoicePhoneWorkThreadC		 *m_pWorkThread;

	VideoPhoneMonitorC		*m_videoPhone;

	
private:
	friend class VoicePhoneInitThreadC;
	friend class VoicePhoneWorkThreadC;
	friend class PhoneMonitorVoiceReportBase;

signals:
	void phoneInitedSignal(void);//for bug:calling,auto-system reboot suddently because of unpredictable factor,calling is going on
};

#endif

