#ifndef _DEVICE_MANAGER_H_
#define _DEVICE_MANAGER_H_
#include <pthread.h>

#include "VDRRunParaTable.h"
#include "VDRTrackInfoTable.h"
#include "ElectronicFenceTable.h"
#include "DriversLoginTable.h"
#include "VDRDeployParaTable.h"
#include "VDRAccelerateTable.h"

#define	Hw31								0x80000000
#define	Hw30								0x40000000
#define	Hw29								0x20000000
#define	Hw28								0x10000000
#define	Hw27								0x08000000
#define	Hw26								0x04000000
#define	Hw25								0x02000000
#define	Hw24								0x01000000
#define	Hw23								0x00800000
#define	Hw22								0x00400000
#define	Hw21								0x00200000
#define	Hw20								0x00100000
#define	Hw19								0x00080000
#define	Hw18								0x00040000
#define	Hw17								0x00020000
#define	Hw16								0x00010000
#define	Hw15								0x00008000
#define	Hw14								0x00004000
#define	Hw13								0x00002000
#define	Hw12								0x00001000
#define	Hw11								0x00000800
#define	Hw10								0x00000400
#define	Hw9									0x00000200
#define	Hw8									0x00000100
#define	Hw7									0x00000080
#define	Hw6									0x00000040
#define	Hw5									0x00000020
#define	Hw4									0x00000010
#define	Hw3									0x00000008
#define	Hw2									0x00000004
#define	Hw1									0x00000002
#define	Hw0									0x00000001


//location status
#define LOCSTATUS_GNSS_BD_MODE              Hw19
#define LOCSTATUS_GNSS_GPS_MODE             Hw18
#define LOCSTATUS_CARDOOR_LOCK              Hw12    //0: car door unlock; 1: lock
#define LOCSTATUS_CIRCUIT_OFF               Hw11    //0: circuit normal; 1: off
#define LOCSTATUS_OILWAY_OFF                Hw10    //0: oilway normal; 1: off
//Hw8:Hw9      00：空车 01：半载 10：保留 11：满载 
#define LOCSTATUS_CARRY_STATUS_HIGH         Hw9
#define LOCSTATUS_CARRY_STATUS_LOW          Hw8
#define LOCSTATUS_LONGLATIT_ENCRYPT         Hw5     //0: longitude latitude not encrypt;  1:encrypt
#define LOCSTATUS_CAR_WORK_OFF              Hw4     //0: work; 1: not work
#define LOCSTATUS_EAST_WEST                 Hw3     //0: east; 1: west
#define LOCSTATUS_NORTH_SOUTH               Hw2     //0: north; 1:south
#define LOCSTATUS_FIXED_POSITION            Hw1		
#define LOCSTATUS_ACC_ONO                   Hw0		

//alarm
#define ALARM_STATUS_EMERGENCY_ALARM        Hw0
#define ALARM_STATUS_OVERSPEED_ALARM        Hw1
#define ALARM_STATUS_FATIGUE_DRIVING        Hw2
#define ALARM_STATUS_EARLY_WARNING          Hw3
#define ALARM_STATUS_GNSS_FAILURE           Hw4
#define ALARM_STATUS_GNSS_UNCONNECT         Hw5
#define ALARM_STATUS_GNSS_SHORT_CIRCUIT     Hw6
#define ALARM_STATUS_POWER_UNDER_VOLTAGE    Hw7
#define ALARM_STATUS_POWER_DOWN_ELECTRICITY Hw8
#define ALARM_STATUS_LCD_FAILURE            Hw9
#define ALARM_STATUS_TTS_FAILURE            Hw10
#define ALARM_STATUS_CAMERA_FAILURE         Hw11
#define ALARM_STATUS_IC_CARD_FAILURE        Hw12
#define ALARM_STATUS_OVERSPEED_WARNING      Hw13
#define ALARM_STATUS_PREFATIGUE_DRIVING     Hw14

#define ALARM_STATUS_SMOKE_ALARM            Hw17
#define ALARM_STATUS_DAY_DRIVING_OVERTIME   Hw18
#define ALARM_STATUS_PARKING_OVERTIIME      Hw19
#define ALARM_STATUS_IN_OUT_AREA            Hw20
#define ALARM_STATUS_IN_OUT_LINE            Hw21
#define ALARM_STATUS_ROAD_DRIVE_SHORT_LONG   Hw22
#define ALARM_STATUS_LINE_DEPARTURE_ALARM   Hw23
#define ALARM_STATUS_CAR_VSS_FAILURE        Hw24
#define ALARM_STATUS_CAR_OIL_FAILURE        Hw25
#define ALARM_STATUS_CAR_STOLEN_ALARM       Hw26
#define ALARM_STATUS_CAR_ILLEGAL_IGNITION   Hw27	//非法点火
#define ALARM_STATUS_CAR_ILLEGAL_OFFSET     Hw28
#define ALARM_STATUS_CAR_COLLISION_ALARM    Hw29
#define ALARM_STATUS_CAR_TURNOVER_ALARM    Hw30
#define ALARM_STATUS_CAR_ILLEGAL_DOOR_ALARM    Hw31		//非法开门报警

#define SPEED_STATUS_OK                     0x01
#define SPEED_STATUS_ERROR                  0x02

#define STATUS_SWITCH(X, LEVEL, ENABLE) ((~((X) ^ (LEVEL))) & (ENABLE) )
#define VDRBITSET(X, MASK) 			( (X) |= (unsigned int)(MASK) )
#define VDRBITCLR(X, MASK) 			( (X) &= ~((unsigned int)(MASK)) )

typedef enum
{
    MCU_TIMER_DATA = 1,             
    UPLOAD_TIMER_DATA, 
    TEMP_TIMER_DATA,
    LINE_TIMER_DATA,            
    SPEED_TIMER_DATA,           
    PARK_TIMER_DATA,            
    STOP_TIMER_DATA,            
    START_TIMER_DATA,           
    SLEEP_TIMER_DATA,
    STATUS_TIMER_DATA,
    PICTURE_TIMER_DATA,
    PICTURE_DISTANCE_DATA,
    NAVI_ENCRYPT_DATA,
    VSSALARM_TIMER_DATA,
    IGNITION_TIMER_DATA,
    ACCOFF_TIMER_DATA,
}DevTimerData;

typedef enum
{
    DEFAULT_MODE = 0,    
    NOLOGIN_MODE,        
    SLEEP_MODE,         
    ALARM_MODE,         
}DevReportMode;

typedef enum
{
	PRP_TIMEING = 0,
	PRP_DISTANCE,
	PRP_TIMEING_DISTANCE,	
}PositionReportPolicy;

typedef enum
{
	PRS_ACC_STATUS = 0,
	PRS_LOGIN_ACC_STATUS,
}PositionReportScenario;

struct PRETIRED_DRIVING{
	char VehicleDriverID[18]; 
	int TiredTime;	
	int StoppingTime;         
	int TiredLongitude;       
	int TiredLatitude;        
	int TiredAltitude;        
	int StopLongitude;        
	int StopLatitude;         
	int StopAltitude;         
	int RunningTime;
	int TotalTime;	
	int TripMeterCurDay;      
	int RunningTimeCurDay;    
	int Flag_TiredDrive;          
};

struct time_distance_capture
{
    int time_capture_switch;      //摄像通道定时拍照开关标志
    int time_capture_store;       //摄像道道定时拍照存储标志
    int time_interval;            //定时时间间隔

    int distance_capture_switch;  //摄像通道定距拍照开关标志
    int distance_capture_store;   //摄像通道定距拍照存储标志
    int distance_interval;        //定距距离间隔
};

struct VDRSettingParam 
{
    int report_policy;	    //位置汇报策略
    int	report_scenario;	//位置汇报方案
    
    int nologin_report_interval;       //驾驶员未登录汇报时间间隔
    int sleep_report_interval;         //休眠时汇报时间间隔
    int alarm_report_interval;         //紧急报警时汇报时间间隔
    int default_report_interval;       //缺省汇报时间间隔

    int default_report_distance;       //缺省距离汇报间隔
    int nologin_report_distance;       //驾驶员未登录汇报距离间隔
    int sleep_report_distance;         //休眠时汇报距离间隔
    int alarm_report_distance;         //紧急报警时汇报距离间隔

    int fence_radius;                  //电子围栏半径(非法位移阀值),单位米

    int warning_mask;
    int warning_SMS_mask;
    int warning_photo_mask;
    int warning_photosave_mask;
    int warning_key_mask;                     //关键标志

    int max_speed;                     //最高速度
    int over_speed_duration;           //超速持续时间
    int continue_drive_threshold;      //连续驾驶时间门限
    int day_drive_time_threshold;      //当天累计驾驶时间门限
    int min_resting_time;              //最小休息时间
    int max_parking_time;              //最长停车时间
    
    string ic_main_serv_ip_addr;
    int ic_main_serv_tcp_port;
    int ic_main_serv_udp_port;
    string ic_second_serv_ip_addr;
    int overspeed_warning;         //超速报警预警差值
    int tired_drive_warning;       //疲劳驾驶预警差值

    int collision_time;            //碰撞时间
    int collision_acceleration;    //碰撞加速度
    int rollover_angle;            //侧翻报警参数: 侧翻角度
    struct time_distance_capture capture_control;

    int gnss_locate_mode;          //GNSS定位模式
    int gnss_baud_rate;            //GNSS波特率
    int gnss_data_out_rate;        //GNSS模块详细定位数据输出频率
    int gnss_data_collect_rate;    //GNSS模块详细定位数据采集频率
    int gnss_data_upload_mode;     //GNSS模块详细定位数据上传方式
    int gnss_data_upload_set;      //GNSS模块详细定位数据上传设置

    int can_1_collect_interv;      //CAN总线通道1采集时间间隔
    int can_1_upload_interv;       //CAN总线通道1上传时间间隔
    int can_2_collect_interv;      //CAN总线通道2采集时间间隔
    int can_2_upload_interv;       //CAN总线通道2上传时间间隔

    vector<unsigned char> can_id_collect;            //CAN总线ID单独采集设置
    vector<unsigned char> other_can_id_collect;      //用于其他CAN总线ID单独采集设置

};

struct VDRCurrentInfo
{
    int is_time_report;
    int is_distance_report;
    int report_mode;
    int report_time;
    int report_distance;
    int temp_report_validity;

    int status_enable;
    int status_level;
    
    int day_time;
    int day_drive_dura;
    int day_tmp_mileage;
    float day_drive_mileage;
    float illegal_displacement;
    double total_mileage;      // m
    int max_speed;
    int over_speed_dura;
    int day_info_flag;

    int speed_timer_flag;  
    int over_speed_warn_flag;//超速预警标志
    int park_timer_flag;
    int stop_timer_flag;
    int start_timer_flag;
    int sleep_timer_flag;
    int status_timer_flag;
    int vssalarm_timer_flag;
    int ignition_timer_flag;
    int accoff_timer_flag;
    int emergency_alarm_flag;
    int emergency_alarm_num;
    int car_reverse_flag;
    int car_reverse_count;
    int car_smoke_flag;
    int car_collision_flag;
    int car_turn_over_flag;
    int undervoltage_flag;
    int illegal_power_flag;
    int gnss_line_flag;
    int gnss_circuit_flag;
    int illegal_displacement_flag;

    int gps_mcu_speed_flag;
    int gps_speed_count;

    int vdr_sec_speed;
    int carry_status;
        
    int acc_on_time;
    int acc_off_time;
    int login_time;
//add 2 flag for tired arithmetic
    int stop_flag;
    int running_flag;
    int speed_flag;
};

struct LineInflexionInfo
{
    int id;
    int lineid;
    int lineattribute;
    int maxtime;
    int mintime;
    int maxspeed;
    int overspeeddura;
};

struct ElectronicFenceData
{
    int instatus;
        
    int nowpoint;
    int temptime;
    
    int etype;
    int regionid;
    int attribute;

    struct LineInflexionInfo inflexion_info[0];
};
    
struct SpeedMileageInfo
{
    int index;
    int signal;             // 1sec status signal
    int sec_speed[60];      // 1sec speed       km/h
    float sec_distance[60];   // 1sec distcance  m
};

struct AdditionalInformation
{
    int overspeed_alarm_flag;
    int overspeed_region_id;
    int inout_alarm_flag;
    int inout_alarm_region_id;
    int inout_alarm_direction;
    int short_long_line_id;
    int short_long_drive_time;
    int short_long_result;
    int io_status;
    int analog_quantity;
    int wireless_intensity;
    int gnss_satellite_sum;
};

struct NaviGPSInfo
{
    int     position;
    int     fX;				// 当前GPS坐标X
    int     fY;				// 当前GPS坐标Y
    int     iTime;			// 当前的GPS时间
    int     iSysTime;		// 当前的系统时间
    int     iOrientation;	// 汽车行驶方位角
    int     iVelocity;		// 汽车行驶速度              km/h
    int     ilatitude;  //当前gps坐标的纬度 北纬---0   南纬---1
    int     ilongitude; //当前gps坐标的经度 东经---0   西经---1
    int     ialtitude;  //当前海拔信息
    int     mode;       // 当前工作模式 0--gps模式  1--北斗模式  2--gps与北斗双模式
    int     gpsnum;    //gps卫星定位个数
    int     bdnum;     //北斗卫星定位个数
    int     antenna;   // 天线状态
};

struct SpeedStatusJudge
{
    int index;
    int beginsec;
    int result; 
    int day_speed_status;
    int vdr_speed[60];     // km/h
    int gps_speed[60];     // km/h
};

struct MinSpeedLocInfo
{
    int m_count;
    int m_speed;
    int m_status;
    int t_speed;
    int m_position;     //定位标志
    int m_time;
    int m_flag;
    struct NaviGPSInfo m_gps;
};

struct ICcardInfo
{
    int status;         // 1 login;  2 logout  0 default;
    char version[2];
    char manuid[2];
    int type;
    char drvname[20];
    char qualification[20];
    char authority[50];
    char validate[4];
};

struct LastSendLocData
{
    int latitude;
    int longitude;
    int altitude;
    int orientation;
    int gps_speed;
};

void read_deploy_para_table_set_pluse();
void read_deploy_status_config_table();
class WriteTableHandlerInterface
{
	public:
		virtual void WriteData(void *data)= 0;
};
class PreTiredWriteTable:public WriteTableHandlerInterface
{
	public:
		PreTiredWriteTable()
		{
			memset(&pretired_param,0,sizeof(PRETIRED_DRIVING));
		}	
		virtual void WriteData(void *data);
		struct PRETIRED_DRIVING pretired_param;
		static VDRPreTiredTable  *table; 
};
class TiredWriteTable:public WriteTableHandlerInterface
{
	public:
		TiredWriteTable()
		{
			memset(&pretired_param,0,sizeof(PRETIRED_DRIVING));
		}	
		virtual void WriteData(void *data);
		struct PRETIRED_DRIVING pretired_param;
		static VDRTiredTable *table; 
};

struct RunStatusData
{
    int itime;
    int accident_flag;
    vector<unsigned char> speed;
    vector<unsigned char> status;    
    struct NaviGPSInfo gpsinfo;
};

class RunStatusWriteTable:public WriteTableHandlerInterface
{
	public:
		virtual void WriteData(void *data);
		struct RunStatusData runstatus_data;
		
};

struct LocStatusData
{
    int type;
    int m_flag;     //full min flag 
    int up_alarm_flag;
    int upload_flag;
    int itime;
    int alarm_signal;
    int status_falg;
    int latitude;
    int longitude;
    int altitude;
    int orientation;
    int gps_speed;
    int total_mileage;
    int oil_mass;
    int vdr_status_old_hb;
    int vdr_status_new_hb;
    int vdr_speed;
    int vdr_distance;
    int overspeed_flag;
    int overspeed_id;
    int fence_inout_alarm;
    int fence_inout_id;
    int fence_inout_direction;
    int fence_line_id;
    int fence_line_time;
    int fence_line_result;
    int emergency_num;    
    int io_status;
    int analog_quantity;
    int wireless_intensity;
    int gnss_satellite_sum;
};

class LocStatusWriteTable:public WriteTableHandlerInterface
{
	public:
		virtual void WriteData(void *data);
		struct LocStatusData locstatus_data;
		
};

struct AccidentData
{
    int itime;
    vector<unsigned char> speed;
    vector<unsigned char> status; 
    char driverid[18];
    int longitude;
    int latitude;
    int ialtitude;
};

class AccidentWriteTable:public WriteTableHandlerInterface
{
	public:
        AccidentWriteTable()
        {
            memset(&accident_data, 0, sizeof(AccidentData));
        }	
		virtual void WriteData(void *data);
		struct AccidentData accident_data;
    static VDRAccidentTable *accident;
};

struct DayInfoData
{
    int itime;
    int drive_dura;
    int drive_mileage;
    int total_mileage;
};

class DayInfoWriteTable:public WriteTableHandlerInterface
{
	public:
		virtual void WriteData(void *data);
		struct DayInfoData dayinfo_data;
		static VDRDayInfoTable* table;
};

#endif
