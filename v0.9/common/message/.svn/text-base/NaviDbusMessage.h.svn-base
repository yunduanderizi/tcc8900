
#ifndef __NAVI_DBUS_MESSAGE_H__
#define __NAVI_DBUS_MESSAGE_H__

#include "dbusMessage.h"

#define NOTF_ID_NAVI_INFO				3800	// 实时导航信息通知
#define NOTF_ID_NAVI_ALERTINFO			3801	// 实时违章提示信息通知
#define NOTF_ID_NAVI_CARLOCATION		3802	// 车辆当前位置信息通知
#define REQ_ID_NAVI_CARLOCATION			3803	// 查询车辆当前位置信息（请求）
#define REP_ID_NAVI_CARLOCATION			3804	// 查询车辆当前位置信息（应答）
#define REQ_ID_NAVI_GPSINFO				3805	// 查询GPS信息（请求）
#define REP_ID_NAVI_GPSINFO				3806	// 查询GPS信息（应答）
#define REQ_ID_NAVI_WINDOWCONTROL		3807	// 窗口显示控制请求（请求）
#define REQ_ID_NAVI_GASSTATION			3808	// 查找加油站请求（请求）
#define REQ_ID_NAVI_PARK				3809	// 查找停车场请求（请求）
#define REQ_ID_NAVI_LOCATECAR			3810	// 定位车辆位置请求（请求）
#define REQ_ID_NAVI_LOCATEHOME			3811	// 定位我的家请求（请求）
#define REQ_ID_NAVI_DESTINATON			3812	// 目的地查询请求（请求）
#define REQ_ID_NAVI_DESTINATONRECORD	3813	// 查询目的地记录请求（请求）
#define REQ_ID_NAVI_NAVIGATION			3814	// 导航请求（请求）
#define REQ_ID_NAVI_GPSINFO_REPORT_CYCLE 3815	// 设置GPS信息上报周期（请求）
#define NOTF_ID_NAVI_GPSINFO_REPORT		3816	// GPS信息上报通知
#define REQ_ID_NAVI_ROADBOOK				3817	// 路书请求 （请求）
#define REQ_ID_NAVI_HOSPITAL 				3818 // 查找附近医院 请求  （请求）
#define REQ_ID_NAVI_REALTIMEROAD 				3819 // 实时路况 请求  （请求）
#define REQ_ID_NAVI_ROADREPORT 				3820 // 路况早报 请求  （请求）
#define REQ_ID_NAVI_COMMONREQ                   3821   //通用请求 (请求)
#define REP_ID_NAVI_COMMON				3999	// 导航软件向中控软件返回的通用应答消息

#define REQ_ID_NAVI_DAYNIGHTMODE        3822   // 变更白天黑夜模式(请求)

#define REQ_ID_NAVI_MULTIPOINTGPSINFO        3823 

// ==================== navi request gui 
#define REQ_ID_NAVI_RIGHT_BUTTON_ROTATE       3824   // 车机右旋钮旋转通知(请求)

#define REQ_ID_NAVI_ADDRESSBOOK		3825	//查找地址薄请求（请求）
#define REQ_ID_NAVI_SURROUNDINGSEARCH		3826  //查找周边搜索请求（请求）
#define REQ_ID_NAVI_ROADBOOK2		3827  //查找路书请求（请求）

#define REQ_ID_GUI_DAYNIGHTMODE        1014   // 查询白天黑夜模式(请求)

#define REQ_ID_NAVI_SHOWMENU                                  3828           //显示主菜单（请求）
#define NOTF_ID_NAVI_ELECTRONIC_FENCE                  3829           //电子围栏数据更新通知
#define NOTF_ID_NAVI_ELECTRONIC_FENCE_STATE         3830        //进入驶出电子围栏通知
#define NOTF_ID_NAVI_MAP_MENU                       3831        //导航显示模块切换通知
#define NOTF_ID_NAVI_GNSS_ATTR_CHANGE               3832        //GNSS模块属性变更通知
#define REQ_ID_NAVI_SATELLITE                       3833           //导航显示卫星定位页面

#define REQ_ID_NAVI_SHUTDOWN                        3840        //进入关机状态 （请求）

#define REQ_ID_NAVI_ENCRYPTINFO		3841	// 查询经纬度加密状态（请求）
#define REP_ID_NAVI_ENCRYPTINFO		3842	// 查询经纬度加密状态（应答）
// ======================================================================

// 实时违章提示信息通知
class CNaviAlertInfoNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(CNaviAlertInfoNotifyC)

public:
	CNaviAlertInfoNotifyC();
	~CNaviAlertInfoNotifyC();

	void print(FILE *fp);

public:
	int  iDistAlert;			// 到目的地距离
	char cTipType;				// 提示类型
	char cBeAlert;				// 是否违章
	char cAlertTip[50];			// 提示内容
};

// ======================================================================

// 实时导航信息通知
class CNaviNavigationInfoNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(CNaviNavigationInfoNotifyC)

public:
	CNaviNavigationInfoNotifyC();
	~CNaviNavigationInfoNotifyC();

	void print(FILE *fp);

public:
	char cStatus;				// 当前状态
	unsigned char cTipType;		// 提示类型
	char cRoadName[50];			// 下一转向路名
	int  iDistTurn;				// 转向距离
	int  iDistEnd;				// 到目的地距离
};

// ======================================================================

// 车辆当前位置信息通知
class CNaviCarLocationNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(CNaviCarLocationNotifyC)

public:
	CNaviCarLocationNotifyC();
	~CNaviCarLocationNotifyC();

	void print(FILE *fp);

public:
	char cCurType;                    // 当前状态
	char cReserve[3];                 // 备用
	char cRoadName[50];                   // 道路名称
	char cAreaName[50];                   // 区域名称
	char cityName[50];                    //城市名称
	bool cPositioning;                //是否已经定位成功 0 --未定位  1--成功定位
	int   cMode;                      //当前工作模式 0--gps模式  1--北斗模式  2--gps与北斗双模式
	int   cSatellitenum;              //gps卫星定位个数
	int   cBDSatellitenum;            //北斗卫星定位个数
};

// ======================================================================

// 查询车辆当前位置信息（请求）
class CNaviCarLocationReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviCarLocationReqC)

public:
	CNaviCarLocationReqC():DBusRequestC(REQ_ID_NAVI_CARLOCATION){}
	void print(FILE *fp);
};

// ======================================================================

// 查询车辆当前位置信息（应答）
class CNaviCarLocationRepC: public DBusReplyC
{
	DBUS_REPLY_OBJECT(CNaviCarLocationRepC)

public:
	CNaviCarLocationRepC():DBusReplyC(REP_ID_NAVI_CARLOCATION){}
	void print(FILE *fp);

public:
	char cCurType;				// 当前状态
	char cReserve[3];			// 备用
	char cRoadName[50];			// 道路名称
	char cAreaName[50];			// 区域名称
	char cityName[50];                       //城市名称
};

// ======================================================================

// 查询GPS信息（请求）
class CNaviGPSInfoReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviGPSInfoReqC)

public:
	CNaviGPSInfoReqC():DBusRequestC(REQ_ID_NAVI_GPSINFO){}
	void print(FILE *fp);
};

// ======================================================================

// 查询GPS信息（应答）
class CNaviGPSInfoRepC: public DBusReplyC
{
	DBUS_REPLY_OBJECT(CNaviGPSInfoRepC)

public:
	CNaviGPSInfoRepC():DBusReplyC(REP_ID_NAVI_GPSINFO){}
	void print(FILE *fp);

public:
	float	fX;				// 当前GPS坐标X
	float	fY;				// 当前GPS坐标Y
	int		iTime;			// 当前的GPS时间
	int		iSysTime;		// 当前的系统时间
	int		iOrientation;	// 汽车行驶方位角
	int		iVelocity;		// 汽车行驶速度
};

// ======================================================================

// 查询GPS信息周期（请求）
class CNaviGPSInfoReportCycleReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviGPSInfoReportCycleReqC)

public:
	CNaviGPSInfoReportCycleReqC():DBusRequestC(REQ_ID_NAVI_GPSINFO_REPORT_CYCLE){}
	void print(FILE *fp);

public:
	int	iReportCycle;		// 上报周期，单位为秒
};

// ======================================================================

// 车辆当前位置信息通知
class CNaviGPSInfoReportNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(CNaviGPSInfoReportNotifyC)

public:
	CNaviGPSInfoReportNotifyC();
	~CNaviGPSInfoReportNotifyC();

	void print(FILE *fp);

public:
	float      fX;          // 当前GPS坐标X
	float      fY;          // 当前GPS坐标Y
	int        iTime;       // 当前的GPS时间
	int        iSysTime;    // 当前的系统时间
	int        iOrientation;// 汽车行驶方位角
	int        iVelocity;   // 汽车行驶速度
	int         ilatitude;    //当前gps坐标的纬度 北纬---0   南纬---1
	int         ilongitude;   //当前gps坐标的经度 东经---0   西经---1
	int         ialtitude;    //当前海拔信息
	bool        cPositioning;     //是否已经定位成功 0 --未定位  1--成功定位
	int         cMode;            //当前工作模式 0--gps模式  1--北斗模式  2--gps与北斗双模式
	int         cSatellitenum;    //gps卫星定位个数
	int         cBDSatellitenum;  //北斗卫星定位个数
};

// ======================================================================

// 窗口显示控制请求（请求）
class CNaviWindowDisplayControlReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviWindowDisplayControlReqC)

public:
	CNaviWindowDisplayControlReqC():DBusRequestC(REQ_ID_NAVI_WINDOWCONTROL){}
	void print(FILE *fp);

public:
	char	cShowMode;		// 窗口显示模式
	char	cHideMode;		// 隐藏属性
	char	cOther;			// 窗口附加属性
	char	ccReserve;		// 备用
	int		iX;				// 窗口位置X
	int		iY;				// 窗口位置Y
	int		iWidth;			// 窗口大小SX
	int		iHeight;		// 窗口大小SY

};

// ======================================================================

// 查找加油站请求（请求）
class CNaviGasStationReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviGasStationReqC)

public:
	CNaviGasStationReqC():DBusRequestC(REQ_ID_NAVI_GASSTATION){}
	void print(FILE *fp);
};

// ======================================================================

// 查找停车场请求（请求）
class CNaviParkReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviParkReqC)

public:
	CNaviParkReqC():DBusRequestC(REQ_ID_NAVI_PARK){}
	void print(FILE *fp);
};

// ======================================================================

// 定位车辆位置请求（请求）
class CNaviLocateCarReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviLocateCarReqC)

public:
	CNaviLocateCarReqC():DBusRequestC(REQ_ID_NAVI_LOCATECAR){}
	void print(FILE *fp);

public:
	int	iShowMode;		// 显示模式
};

// ======================================================================

// 定位我的家请求（请求）
class CNaviLocateHomeReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviLocateHomeReqC)

public:
	CNaviLocateHomeReqC():DBusRequestC(REQ_ID_NAVI_LOCATEHOME){}
	void print(FILE *fp);

public:
	int	iShowMode;		// 显示模式
};

// ======================================================================

// 目的地查询请求（请求）
class CNaviDestinatonReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviDestinatonReqC)

public:
	CNaviDestinatonReqC():DBusRequestC(REQ_ID_NAVI_DESTINATON){}
	void print(FILE *fp);
};
// ======================================================================

// 地址薄查询请求（请求）
class CNaviAddressBookReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviAddressBookReqC)

public:
	CNaviAddressBookReqC():DBusRequestC(REQ_ID_NAVI_ADDRESSBOOK){}
	void print(FILE *fp);
};

// ======================================================================

// 周边搜索查询请求（请求）
class CNaviSurroundingSearchReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviSurroundingSearchReqC)

public:
	CNaviSurroundingSearchReqC():DBusRequestC(REQ_ID_NAVI_SURROUNDINGSEARCH){}
	void print(FILE *fp);
};

// ======================================================================

// 路书查询请求（请求）--调用GD路书列表界面
class CNaviRoadBook2ReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviRoadBook2ReqC)

public:
	CNaviRoadBook2ReqC():DBusRequestC(REQ_ID_NAVI_ROADBOOK2){}
	void print(FILE *fp);
};

// ======================================================================

// 查询目的地记录请求（请求）
class CNaviDestinatonRecordReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviDestinatonRecordReqC)

public:
	CNaviDestinatonRecordReqC():DBusRequestC(REQ_ID_NAVI_DESTINATONRECORD){}
	void print(FILE *fp);
};

// ======================================================================

// 导航请求（请求）
class CNaviNavigationReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviNavigationReqC)

public:
	CNaviNavigationReqC():DBusRequestC(REQ_ID_NAVI_NAVIGATION){}
	void print(FILE *fp);

public:

	char	cDistName[48];	// 目的地名称
	float	fX;				// 目的地GPS坐标X
	float	fY;				// 目的地GPS坐标Y
	int		iRuleSelect;	// 路由规划规则

};

// ======================================================================

struct RoadBookItems
{
    char cDistName[50];
    float	fX;				// 目的地GPS坐标X
    float	fY;				// 目的地GPS坐标Y
    struct RoadBookItems *next;
};   

// 路书请求（请求）
class CNaviRoadBookReqC:public DBusRequestC
{
public:
        int encode(void *&buffer,int  &len,DBusMessageFreeFunc &freeFunc);
        int decode(void *buffer,int len);
        static DBusRequestC* factory()
        {
                return new CNaviRoadBookReqC();
        }
        
public:
	CNaviRoadBookReqC();
    	~CNaviRoadBookReqC();
	void print(FILE *fp);
    
        void SetBookTitle(char *buffer);
        void SetDescription(char *buffer);
        void AddBookItem(char *name, float fx, float fy);    
        
        char * GetBookTitle();
        char * GetDescription();
        int GetItemNum();
        struct RoadBookItems  * GetBookItems();
        struct RoadBookItems * operator[ ](int index); 
        
private:
	char rBookTitle[128];				// 路书标题
	char rDescription[256];			        // 说明 
       int rItemNum;	
       struct RoadBookItems  *rBookItems;
   
};

// ======================================================================

// 查找附近医院 请求（请求）
class CNaviHospitalReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviHospitalReqC)

public:
	CNaviHospitalReqC():DBusRequestC(REQ_ID_NAVI_HOSPITAL){}
	void print(FILE *fp);
};

// ======================================================================

//实时路况 请求（请求）
class CNaviRealTimeRoadReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviRealTimeRoadReqC)

public:
	CNaviRealTimeRoadReqC():DBusRequestC(REQ_ID_NAVI_REALTIMEROAD){}
	void print(FILE *fp);
};

// ======================================================================

//路况早报请求（请求）
class CNaviRoadReportReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviRoadReportReqC)

public:
	CNaviRoadReportReqC():DBusRequestC(REQ_ID_NAVI_ROADREPORT){}
	void print(FILE *fp);

public:
	float	src_fX;				// 起始点GPS坐标X
	float	src_fY;				// 起始点GPS坐标Y
	float	dst_fX;				// 终止点GPS坐标X
	float	dst_fY;				// 终止点GPS坐标Y
       int    starttime;                    //开始时间 0~24
       int    endtime;                     //结束时间 0~24
       int    interval;                     //间隔时间  分钟
       int    flag;                          //是否自动更新
};

// ======================================================================

//通用 请求（请求）
class CNaviCommonReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviCommonReqC)

public:
	CNaviCommonReqC():DBusRequestC(REQ_ID_NAVI_COMMONREQ){}
	void print(FILE *fp);

public:
        int reqid;              // reqid=1 请求查看路况早报设置
};


// ======================================================================

// 导航软件向中控软件返回的通用应答消息
class CNaviCommonRepC: public DBusReplyC
{
	DBUS_REPLY_OBJECT(CNaviCommonRepC)

public:
	CNaviCommonRepC():DBusReplyC(REP_ID_NAVI_COMMON){}
	void print(FILE *fp);
};

// ======================================================================

//白天黑夜模式变更 请求（请求） gui to navi
class CNavDayNightModeReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNavDayNightModeReqC)

public:
	CNavDayNightModeReqC():DBusRequestC(REQ_ID_NAVI_DAYNIGHTMODE){}
	void print(FILE *fp);

public:
    int dayNightMode;              //
};

// ======================================================================

//白天黑夜模式查询 请求（请求）navi to gui
class CGuiDayNightModeReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CGuiDayNightModeReqC)

public:
	CGuiDayNightModeReqC():DBusRequestC(REQ_ID_GUI_DAYNIGHTMODE){}
	void print(FILE *fp);
};

struct MultipointItems
{
    char cDistName[50];
    float	fX;				// 目的地GPS坐标X
    float	fY;				// 目的地GPS坐标Y
    struct MultipointItems *next;
}; 

// ======================================================================

//车机右旋钮旋转通知 请求（请求） gui to navi
class CNavRightButtonRotateReqC: public DBusRequestC
{
        DBUS_REQUEST_OBJECT(CNavRightButtonRotateReqC)

public:
        CNavRightButtonRotateReqC():DBusRequestC(REQ_ID_NAVI_RIGHT_BUTTON_ROTATE){}
        void print(FILE *fp);

public:
    int direction;              //
    int steps;
};

// ======================================================================
//多点导航
class CNaviMultipointReqC:public DBusRequestC
{
public:
        int encode(void *&buffer,int  &len,DBusMessageFreeFunc &freeFunc);
        int decode(void *buffer,int len);
        static DBusRequestC* factory()
        {
                return new CNaviMultipointReqC();
        }
        
public:
	CNaviMultipointReqC();
    	~CNaviMultipointReqC();
	void print(FILE *fp);
    
        void SetTitle(char *buffer);
        void SetDescription(char *buffer);
        void AddPointItem(char *name, float fx, float fy);    
        
        char * GetTitle();
        char * GetDescription();
        int GetItemNum();
        struct MultipointItems  * GetPointItems();
        struct MultipointItems * operator[ ](int index); 
        
private:
	     char rTitle[128];				// 标题
	     char rDescription[256];			        // 说明 
       int rItemNum;	
       struct MultipointItems  *rPointItems;
   
};

//显示gps菜单or地图 请求（请求）gui to navi
class CNaviShowMenuC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviShowMenuC)

public:
	CNaviShowMenuC():DBusRequestC(REQ_ID_NAVI_SHOWMENU){}
	void print(FILE *fp);
public:
        int m_mode;// 0----map 1---menu
};

//地图、菜单模式（通知）navi->gui
class CNaviMapMenuNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(CNaviMapMenuNotifyC)

public:
	CNaviMapMenuNotifyC();
	~CNaviMapMenuNotifyC();
	void print(FILE *fp);
public:
	int m_mode;// 0----map 1---menu
};

//电子围栏更新通知gui to navi
class CNaviElectronicFenceNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(CNaviElectronicFenceNotifyC)

public:
	CNaviElectronicFenceNotifyC();
	~CNaviElectronicFenceNotifyC();
	void print(FILE *fp);
public:
	int eflag;        //(estate==0有效)操作类型 1--增加 0--修改  -1--删除
	int eid;           //(estate==0有效)电子围栏信息在数据库中的存储id
	int etype;       //电子围栏的类型  0--圆形  1--矩形  2--多边形  3--线路
	int estate;      //电子围栏操作总标示符 -1--update类型总操作,删除(参考etype)所有数据 1--delete类型总操作,更新(参考etype)所有数据  0--单围栏操作,具体参考eflag etype eid
};


//进入驶出电子围栏通知navi to gui
class CNaviElectronicFenceStateNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(CNaviElectronicFenceStateNotifyC)

public:
	CNaviElectronicFenceStateNotifyC();
	~CNaviElectronicFenceStateNotifyC();
	void print(FILE *fp);
public:
	int iTime;       // 当前的GPS时间
	int etype;        //电子围栏的类型  0--圆形  1--矩形  2--多边形  3--线路
	int eid;          //电子围栏信息在数据库中的存储id
	int estate;       //电子围栏进入驶出状态  1--进入  0--驶出  2--keep
	int epoint;
	int edeviation;   //当围栏类型为线路时有效，在线路中间段  0--正常进入  1--偏离线路  2--重新返回线路
};


//进入关机状态请求
class CNavShutDownReqC: public DBusRequestC
{
        DBUS_REQUEST_OBJECT(CNavShutDownReqC)

public:
        CNavShutDownReqC():DBusRequestC(REQ_ID_NAVI_SHUTDOWN){}
        void print(FILE *fp){fprintf( fp, "\t CNavShutDownReqC \n\n" );}
};


//GNSS 模块属性变更通知gui to navi
class CGNSSAttrChangeNotifyC: public DBusNotifyC
{
	DBUS_NOTIFY_OBJECT(CGNSSAttrChangeNotifyC)

public:
	CGNSSAttrChangeNotifyC():DBusNotifyC(NOTF_ID_NAVI_GNSS_ATTR_CHANGE){}
	void print(FILE *fp){fprintf( fp, "\tCGNSSNotifyC %d, %d, %d, %d\n\n",mode, baudrate, outfrequency, samplerate);}
public:
	int mode;            //GNSS 定位模式,  bit0, 0:禁用GPS定位, 1:启用GPS定位;  bit1 , 0:禁用北斗定位,  1:启用北斗定位 ;  
	                    //                    bit2,0:禁用GLONASS定位, 1:启用GLONASS定位; bit3,0:禁用Galileo定位, 1:启用Galileo定位.
	int baudrate;       //GNSS波特率，0x00: 4800;   0x01: 9600;   0x02: 19200;   0x03:38400;  0x04: 57600;   0x05: 115200.
	int outfrequency;   //GNSS模块详细定位数据输出频率，0x00: 500ms;   0x01: 1000ms;   0x02: 2000ms;   0x03:3000ms;  0x04: 4000ms.
	int samplerate;    //GNSS模块详细定位数据采集频率，单位秒，默认为1.
};

//显示卫星定位页面请求（请求）gui->navi
class CNaviSatelliteUIC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviSatelliteUIC)
	
public:
	CNaviSatelliteUIC():DBusRequestC(REQ_ID_NAVI_SATELLITE){}
	void print(FILE *fp);
public:
  int m_mode;// 0----show 1---hide
};


// ======================================================================

//查询经纬度加密状态（请求）
class CNaviEncryptInfoReqC: public DBusRequestC
{
	DBUS_REQUEST_OBJECT(CNaviEncryptInfoReqC)

public:
	CNaviEncryptInfoReqC():DBusRequestC(REQ_ID_NAVI_ENCRYPTINFO){}
    void print(FILE *fp){fprintf( fp, "\t CNaviEncryptInfoReqC \n\n" );}
};

// 查询经纬度加密状态（应答）
class CNaviEncryptInfoRepC: public DBusReplyC
{
	DBUS_REPLY_OBJECT(CNaviEncryptInfoRepC)

public:
	CNaviEncryptInfoRepC():DBusReplyC(REP_ID_NAVI_ENCRYPTINFO){}
	void print(FILE *fp){fprintf( fp, "\t CNaviEncryptInfoRepC flag=%d \n\n",flag );}
    
	int	flag;		// 0: 未经保密插件加密；1:已经保密插件加密; 
};

#endif // __NAVI_DBUS_MESSAGE_H__
