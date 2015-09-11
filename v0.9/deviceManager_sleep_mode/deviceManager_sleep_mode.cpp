#include <stdio.h>
#include <time.h>
#include <string>
#include <sys/ioctl.h>
#include <linux/rtc.h> 
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "VDRRunParaTable.h"
#include "VDRTrackInfoTable.h"
#include "ElectronicFenceTable.h"
#include "DriversLoginTable.h"
#include "VDRDeployParaTable.h"
#include "VDRAccelerateTable.h"

#include "GeneralTimer.h"
#include "Utility.h"

#include "dbusApplication.h"
#include "dbusClient.h"
#include "dbusServer.h"

#include "JT808DbusMessage.h"
#include "NaviDbusMessage.h"
#include "DeviceManagerDbusMessage.h"
#include "mcuprotocol.h"
#include "devicemanager.h"

DBusClientC *sp_ClientJT808;
DBusServerC	*dev_DbusServer;

GeneralTimerItem *mcutimer = NULL;             //read mcu time
static pthread_mutex_t gps_mutex;
static struct VDRSettingParam  vdr_param;
static struct VDRCurrentInfo vdr_current_info;
static struct AdditionalInformation add_info;
static struct SpeedMileageInfo speed_distance;
static struct NaviGPSInfo   gps_info;

VDRRunStatusTable runstable;
VDRLocStatus1Table loc1table;
VDRLocStatus2Table loc2table;

static unsigned int vdr_status_flag = 0;
static unsigned int vdr_alarm_flag = 0;
static unsigned int tmp_alarm_flag = 0;
static float tempmileage = 0;
static int cst_fd = -1;
static int sensor_fd = -1;
static int speed_fd = -1;

static int read_again = 0;
static unsigned long read_time = 0;
static unsigned long tmp_time = 0;

static int socket_dp;
struct sockaddr_in addr;
struct LocStatusData udp_loc;

int write_rtc(int time);
int  change_alarm_flag_and_upload(int flag,int set);
void check_speed_status_info(int vdr_speed, int gps_speed);
void read_mcu_data_handler_sleep_mode(string &name, void *data, void *data2);
void process_acc_off_info_sleep_mode();

void PreTired_Write();
void TiredDriving_Write();
void AllTiredDriving_Write();


int send_device_alarm_notify(int type, int flag)
{
    DeviceAlarmDbusNotifyC alarmnotify;
    alarmnotify.type = type;
    alarmnotify.flag = flag;
    dev_DbusServer->sendNotify(&alarmnotify);
    return 0;
}

int send_speed_udp_socket()
{
    int port =  8901;

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port);

    socket_dp = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_dp == -1)
    {
        perror("socket call failed");
        return -1;
    }

    return socket_dp;
}

int send_position_to_JT808_udp_sleep_mode()
{
    int ret = 0;

    ret = sendto(socket_dp, &udp_loc, sizeof(struct LocStatusData), 0, (struct sockaddr *)&addr, sizeof(addr));
    printf("dev_udp: time=%d,alarm=%d,staus=%d, fx=%d, fy=%d\n",udp_loc.itime,udp_loc.alarm_signal,udp_loc.status_falg,
        udp_loc.longitude,udp_loc.latitude);

    if(ret == -1)
    {
        perror("udp sendto jt808 call failed");
    } 
    return -1;
}

int send_position_to_JT808_sleep_mode(int itime)
{
    JT808LocReportDbusReqC    request;
    DBusReplyC  *reply=NULL;
    int ret = -1;
    int schema_colnum=0, schema_table=0, schema_index=0;
    
    loc1table.GetCurrentTableAndIndex(schema_colnum,schema_table,schema_index);
    request.m_tableIndex = schema_table*100+ schema_index-1;

    printf(" ---report index=%d---time=%d---------\n",(int)request.m_tableIndex,itime);
    ret = sp_ClientJT808->sendService(&request,reply,5*1000);
    if(ret < 0)
    {
        printf("Send Service Error. Error Code: %d\n",ret);
    }
    else
    {
        if(reply->getIdentifier()==DBUS_REP_ID_COMMON)
        {
            printf("Received Common Reply. result:%d\n",reply->result);
        }
        else if(reply->getIdentifier()==JT808_DBUS_REP_ID_LOC_REPORT)
        {
        	printf("Reply ok\n");
        }
        else
        {
            printf("Reply not recongnized.\n");
        }
        ret = 0;
        delete reply;
    }

    return ret;
}

void write_track_locstatus_table(int type, int itime, struct NaviGPSInfo *locstatus)
{
    int i=0, speed=0;
    float distance=0;
    LargeTableBase * table;
    int tmp_newhb_status = 0;

    if(type == 1)
        table = &loc1table;
    else
        table = &loc2table;

    table->SetIntValue(VDR_LOCS_OCCUR_TIME, itime);
    vdr_alarm_flag = (~vdr_param.warning_mask) &vdr_alarm_flag;
    table->SetIntValue(VDR_LOCS_ALARM_SIGNAL, vdr_alarm_flag);
    table->SetIntValue(VDR_LOCS_STATUS_FLAG, vdr_status_flag);
    table->SetIntValue(VDR_LOCS_LATITUDE, locstatus->fY);
    table->SetIntValue(VDR_LOCS_LONGITUDE, locstatus->fX);
    table->SetIntValue(VDR_LOCS_ALTITUDE, locstatus->ialtitude);
    table->SetIntValue(VDR_LOCS_ORIENTATION, locstatus->iOrientation);
    table->SetIntValue(VDR_LOCS_GPS_SPEED, locstatus->iVelocity*10);    //0.1km/h
    table->SetIntValue(VDR_LOCS_TOTAL_MILEAGE, vdr_current_info.total_mileage/100);  //0.1km
    table->SetIntValue(VDR_LOCS_OIL_MASS, 0);
    table->SetIntValue(VDR_LOCS_VDR_SPEED, speed_distance.sec_speed[speed_distance.index]*10);  //0.1km/h
    table->SetIntValue(VDR_LOCS_VDR_STATUS, speed_distance.signal & 0xFF);
    
    if(type ==1)
    {
        int tmp_i_status = speed_distance.signal;
        tmp_newhb_status |= ((tmp_i_status & 0x01) << 5);// d0 -----> d5
        tmp_newhb_status |= (tmp_i_status & 0x06);       // d1 d2 not change
        tmp_newhb_status |= ((tmp_i_status & 0x08) >> 3);// d3 -----> d0
        tmp_newhb_status |= ((tmp_i_status & 0x10) >> 3);// d4 -----> d1
        tmp_newhb_status |= ((tmp_i_status & 0x20) >> 3);// d5 -----> d2
        tmp_newhb_status |= ((tmp_i_status & 0x40) >> 3);// d6 -----> d3
        tmp_newhb_status |= ((tmp_i_status & 0x80) >> 3);// d7 -----> d4
        tmp_newhb_status |= (tmp_i_status & 0x7F00);
        table->SetIntValue(VDR_LOCS_VDR_NEW_HB_STATUS, tmp_newhb_status);
        table->SetIntValue(VDR_LOCS_VDR_DISTANCE,speed_distance.sec_distance[speed_distance.index]); // m
        table->SetIntValue(VDR_LOCS_IO_STATUS, add_info.io_status);
        table->SetIntValue(VDR_LOCS_ANALOG_QUANTITY, add_info.analog_quantity);
        table->SetIntValue(VDR_LOCS_WIRELESS_INTENSITY, add_info.wireless_intensity);
        table->SetIntValue(VDR_LOCS_GNSS_SATELLITE_SUM, add_info.gnss_satellite_sum);
    }
    else
    {
        for(i=0;i<60;i++)
        {
            speed += speed_distance.sec_speed[i];
            distance += speed_distance.sec_distance[i];
        }
        table->SetIntValue(VDR_LOCS_VDR_DISTANCE, distance);
        table->SetIntValue(VDR_LOCS_VDR_SPEED, speed/6);      // speed*10/60
        if((speed != 0) && (vdr_current_info.running_flag == 1))
        {
            vdr_current_info.day_drive_dura++;
        }
    }

    table->SetIntValue(VDR_LOCS_OVERSPEED_ALARM, add_info.overspeed_alarm_flag);
    table->SetIntValue(VDR_LOCS_OVERSPEED_REGIONID, add_info.overspeed_region_id);
    table->SetIntValue(VDR_LOCS_INOUT_ALARM, add_info.inout_alarm_flag);
    table->SetIntValue(VDR_LOCS_INOUT_REGIONID, add_info.inout_alarm_region_id);
    table->SetIntValue(VDR_LOCS_INOUT_DIRECTION, add_info.inout_alarm_direction);
    table->SetIntValue(VDR_LOCS_SHORT_LONG_LINEID, add_info.short_long_line_id);
    table->SetIntValue(VDR_LOCS_SHORT_LONG_TIME, add_info.short_long_drive_time);
    table->SetIntValue(VDR_LOCS_SHORT_LONG_RESULT, add_info.short_long_result);
    table->SetIntValue(VDR_LOCS_EMERGENCY_ALARM_ID, vdr_current_info.emergency_alarm_num);

    udp_loc.type = type;
    udp_loc.itime = itime;
    udp_loc.alarm_signal = vdr_alarm_flag;
    udp_loc.status_falg = vdr_status_flag;
    udp_loc.latitude = locstatus->fY;
    udp_loc.longitude = locstatus->fX;
    udp_loc.altitude = locstatus->ialtitude;
    udp_loc.orientation = locstatus->iOrientation;
    udp_loc.gps_speed = locstatus->iVelocity*10;
    udp_loc.total_mileage = vdr_current_info.total_mileage/100;
    udp_loc.oil_mass = 0;
    udp_loc.vdr_status_old_hb = speed_distance.signal & 0xFF;
    udp_loc.vdr_status_new_hb = tmp_newhb_status;
    udp_loc.vdr_speed = speed_distance.sec_speed[speed_distance.index]*10;
    udp_loc.vdr_distance = speed_distance.sec_distance[speed_distance.index];
    udp_loc.io_status = add_info.io_status;
    udp_loc.analog_quantity = add_info.analog_quantity;
    udp_loc.wireless_intensity = add_info.wireless_intensity;
    udp_loc.gnss_satellite_sum = add_info.gnss_satellite_sum;
    udp_loc.overspeed_flag = add_info.overspeed_alarm_flag;
    udp_loc.overspeed_id = add_info.overspeed_region_id;
    udp_loc.fence_inout_alarm = add_info.inout_alarm_flag;
    udp_loc.fence_inout_id = add_info.inout_alarm_region_id;
    udp_loc.fence_inout_direction = add_info.inout_alarm_direction;
    udp_loc.fence_line_id= add_info.short_long_line_id;
    udp_loc.fence_line_time = add_info.short_long_drive_time;
    udp_loc.fence_line_result = add_info.short_long_result;
    udp_loc.emergency_num = vdr_current_info.emergency_alarm_num;
    send_position_to_JT808_udp_sleep_mode();
    table->Add();
    
}
int read_track_locstatus_table()
{
    int status = 0;
    int tmp = 0;
    int index = loc1table.RefreshBottom();
    if(index < 0)
    {
        printf("read locstatus bottom error...\n");
        return -1;
    }
    else
    {
        loc1table.GetIntValue(VDR_LOCS_STATUS_FLAG, status);
        loc1table.GetIntValue(VDR_LOCS_OCCUR_TIME, vdr_current_info.acc_off_time);
        loc1table.GetIntValue(VDR_LOCS_TOTAL_MILEAGE, tmp);
        vdr_current_info.total_mileage = tmp*100;
        
        if(status & Hw4)
            return 0;       //停运状态 
        else
            return 1;      //运行状态
        
    }
}

void read_vdr_run_param_table()
{
    VDRRunParaTable *vdrrunpara = new VDRRunParaTable();
    vdrrunpara->GetIntValue(VDR_PARA_LOC_REPORT_POLICY, vdr_param.report_policy);
    vdrrunpara->GetIntValue(VDR_PARA_LOC_REPORT_SCENARIO, vdr_param.report_scenario);
    vdrrunpara->GetIntValue(VDR_PARA_NOLOGIN_REPORT_INTERVAL, vdr_param.nologin_report_interval);
    vdrrunpara->GetIntValue(VDR_PARA_SLEEP_REPORT_INTERVAL, vdr_param.sleep_report_interval);
    vdrrunpara->GetIntValue(VDR_PARA_EMERGENCY_REPORT_INTERVAL, vdr_param.alarm_report_interval);
    vdrrunpara->GetIntValue(VDR_PARA_DEFAULT_REPORT_INTERVAL, vdr_param.default_report_interval);
    vdrrunpara->GetIntValue(VDR_PARA_DEFAULT_REPORT_DISTANCE, vdr_param.default_report_distance);
    vdrrunpara->GetIntValue(VDR_PARA_NOLOGIN_REPORT_DISTANCE, vdr_param.nologin_report_distance);
    vdrrunpara->GetIntValue(VDR_PARA_SLEEP_REPORT_DISTANCE, vdr_param.sleep_report_distance);
    vdrrunpara->GetIntValue(VDR_PARA_EMERGENCY_REPORT_DISTANCE, vdr_param.alarm_report_distance);

    vdrrunpara->GetIntValue(VDR_PARA_WARNING_MASK, vdr_param.warning_mask); 
    vdrrunpara->GetIntValue(VDR_PARA_WARNING_PHOTO_MASK, vdr_param.warning_photo_mask); 
    vdrrunpara->GetIntValue(VDR_PARA_WARNING_PHOTOSAVE_MASK, vdr_param.warning_photosave_mask); 
    
    vdrrunpara->GetIntValue(VDR_PARA_MAX_SPEED, vdr_param.max_speed);    
    vdrrunpara->GetIntValue(VDR_PARA_OVERSPEED_DURATION, vdr_param.over_speed_duration);
    vdrrunpara->GetIntValue(VDR_PARA_CONT_DRIVE_THRESHOLD, vdr_param.continue_drive_threshold);
    vdrrunpara->GetIntValue(VDR_PARA_DAY_DRIVE_THRESHOLD, vdr_param.day_drive_time_threshold);
    vdrrunpara->GetIntValue(VDR_PARA_REST_MIN, vdr_param.min_resting_time);
    vdrrunpara->GetIntValue(VDR_PARA_PARKING_MAX, vdr_param.max_parking_time);
    
    int collision_alarm_param;
    
    vdrrunpara->GetIntValue(VDR_PARA_COLLISION_ALARM, collision_alarm_param);
    vdr_param.collision_time = collision_alarm_param & 0xFF;//单位4ms
    vdr_param.collision_acceleration = (collision_alarm_param >> 8) & 0xFF;//单位0.1g
    vdrrunpara->GetIntValue(VDR_PARA_ROLLOVER_ALARM, vdr_param.rollover_angle);
    delete vdrrunpara;

    tempmileage = 0;

    vdr_current_info.report_time = 0;
    vdr_current_info.report_distance = 0;
    vdr_current_info.is_time_report = 0;
    vdr_current_info.is_distance_report = 0;
    vdr_current_info.temp_report_validity = 0;
    vdr_current_info.max_speed = vdr_param.max_speed;
    vdr_current_info.over_speed_dura = vdr_param.over_speed_duration;
    VDRBITSET(vdr_status_flag, LOCSTATUS_CAR_WORK_OFF);

    switch(vdr_param.report_policy)
    {
        case PRP_TIMEING:
            vdr_current_info.is_time_report = 1;
            break;
        case PRP_TIMEING_DISTANCE:
            vdr_current_info.is_time_report = 1;
            vdr_current_info.is_distance_report = 1;
            break;
        case PRP_DISTANCE:
            vdr_current_info.is_distance_report = 1;
            break;
    }
            
}

void CNaviGPSInfoReportNotifyHandler_sleep_mode(DBusNotifyC *pNotify)
{        
    if(pNotify->getIdentifier()!=NOTF_ID_NAVI_GPSINFO_REPORT)
        return;
	
    CNaviGPSInfoReportNotifyC *locnotify = (CNaviGPSInfoReportNotifyC*)pNotify;
    
    pthread_mutex_lock(&gps_mutex);
    gps_info.position = locnotify->cPositioning;
    gps_info.fX = locnotify->fX*1000000;
    gps_info.fY = locnotify->fY*1000000;
    gps_info.iTime = locnotify->iTime;
    gps_info.iSysTime = locnotify->iSysTime;
    gps_info.iOrientation = locnotify->iOrientation;
    gps_info.iVelocity = locnotify->iVelocity;    
    gps_info.ilatitude = locnotify->ilatitude;
    gps_info.ilongitude = locnotify->ilongitude;
    gps_info.ialtitude = locnotify->ialtitude;

    add_info.io_status = 2;
    add_info.analog_quantity = 0;
    add_info.wireless_intensity = 0;
    if(locnotify->cMode == 0)
        add_info.gnss_satellite_sum = locnotify->cSatellitenum;
    else if(locnotify->cMode == 1)
        add_info.gnss_satellite_sum = locnotify->cBDSatellitenum;
    else
        add_info.gnss_satellite_sum = locnotify->cBDSatellitenum + locnotify->cSatellitenum;
    
    #if 0
	printf("fx=%d, fy=%d, itime=%d,isystime=%d,iOrientation=%d,iVelocity=%d,----ilatitude=%d,ilongitude=%d,ialtitude=%d \n",
		gps_info.fX,gps_info.fY,gps_info.iTime,gps_info.iSysTime,gps_info.iOrientation,gps_info.iVelocity,
		gps_info.ilatitude, gps_info.ilongitude,gps_info.ialtitude);
    #endif
    pthread_mutex_unlock(&gps_mutex);

}

void JT808LocReportDbusNotifyHandler(DBusNotifyC *notify)
{
    if(notify->getIdentifier()!=JT808_DBUS_NTF_ID_LOC_REPORT)
        return;
    
    process_acc_off_info_sleep_mode();
}

int  change_alarm_flag_and_upload(int flag,int set)
{
    if(vdr_param.warning_mask & flag)
        return -1;

    if(set == 1)
    {
        if((vdr_alarm_flag & flag) == 0)
        {
            tmp_alarm_flag = 1;
            VDRBITSET(vdr_alarm_flag, flag); 
        }
    }
    else
    {
        if((vdr_alarm_flag & flag)!=0)
        {
            VDRBITCLR(vdr_alarm_flag, flag);
            tmp_alarm_flag = 1;
        }
    }
    
    return 0;
}
    

int parse_speed_status_info(unsigned char *buf,int len, int itime)
{
    DeviceReverseDbusNotifyC notify;
    int speed = 0;
    float distance = 0;
    int i=0;
    int tmp_status = 0;
    
    for(i=0;i<len; i++)
    {
        speed += buf[i*2+1];
        buf[i*2] = STATUS_SWITCH(buf[i*2], vdr_current_info.status_level, vdr_current_info.status_enable);
        tmp_status |= buf[i*2];
        if(buf[i*2]&Hw0)
        {
            if(vdr_current_info.car_reverse_flag == 0)
            {
                vdr_current_info.car_reverse_count++;
                
                if(vdr_current_info.car_reverse_count > 2)
                {
                    vdr_current_info.car_reverse_flag = 1;
                    notify.flag = 1;
                    dev_DbusServer->sendNotify(&notify);
                }
            }
        }
        else 
        {
            if(vdr_current_info.car_reverse_flag == 1)
            {
                vdr_current_info.car_reverse_count--;

                if(vdr_current_info.car_reverse_count == 0)
                {
                    vdr_current_info.car_reverse_flag = 0;
                    notify.flag = 0;
                    dev_DbusServer->sendNotify(&notify);
                }
            }
            else
                vdr_current_info.car_reverse_count = 0;
             
        }
    }

    speed = speed/len;    
    distance = speed/3.6;

    speed_distance.index++;
    if(speed_distance.index == 60)
        speed_distance.index = 0;

    speed_distance.signal = tmp_status;
    speed_distance.sec_speed[speed_distance.index]=speed;        // km/h
    speed_distance.sec_distance[speed_distance.index]=distance;  //m

    vdr_current_info.day_drive_mileage += distance;
    vdr_current_info.total_mileage += distance;    
    tempmileage += distance; 
    
    return speed;
}    

int write_rtc(int time)
{
	if(time < 0)
	{
		printf("time < 0\n");
		return -1;
	}

	int	fd=open("/dev/rtc",O_RDONLY);	
	if(fd<0)
	{
		printf("error open /dev/rtc\n");
		return -1;
	}

	printf("time is %d\n", time);
	if(ioctl(fd, RTC_WKALM_SET, &time) < 0)
	{
		printf("error RTC_WKALM_SET ioctl\n");
		return -1;
	}

	return 0;
}

void process_acc_off_info_sleep_mode()
{
    int tmp;
    
    DeviceAccOffDbusNotifyC notify;
    notify.flag = 0;
    dev_DbusServer->sendNotify(&notify);

    printf("shutdown ..\n");
    
    if(vdr_current_info.is_time_report)
    {
        tmp = time(NULL) - tmp_time;
        printf("read_time=%d, tmp_time=%d, tmp=%d, interval=%d\n",read_time,tmp_time,tmp,
            vdr_param.sleep_report_interval);
        if(tmp > vdr_param.sleep_report_interval)
            write_rtc(vdr_param.sleep_report_interval);
        else 
            write_rtc(vdr_param.sleep_report_interval - tmp - 60);
    }
    system("accoff.sh 1 &");
    sleep(2);
    system("i2cset b 0x54 0x01 0x01");

    exit(0);
}
void process_gps_position_info(int itime)
{
    struct NaviGPSInfo tmp_gps;
    
    pthread_mutex_lock(&gps_mutex);
    memcpy(&tmp_gps, &gps_info, sizeof(struct NaviGPSInfo));
    pthread_mutex_unlock(&gps_mutex);
    
    tmp_gps.iSysTime = itime;

    if(!tmp_gps.position)
    {
        VDRBITCLR(vdr_status_flag, LOCSTATUS_FIXED_POSITION);
    }
    else
    {
        VDRBITSET(vdr_status_flag, LOCSTATUS_FIXED_POSITION);
    }
    
    if(tmp_gps.ilatitude)
        VDRBITSET(vdr_status_flag, LOCSTATUS_EAST_WEST);
    else
        VDRBITCLR(vdr_status_flag, LOCSTATUS_EAST_WEST);
    if(tmp_gps.ilongitude)
        VDRBITSET(vdr_status_flag, LOCSTATUS_NORTH_SOUTH);
    else
        VDRBITCLR(vdr_status_flag, LOCSTATUS_NORTH_SOUTH);
    
    write_track_locstatus_table(1, itime, &tmp_gps);
    
}
void process_alarm_status_info(unsigned char alarmstatus)
{
    int alarmnum=0;
    int flag = 0;
    
    if(vdr_param.warning_mask&ALARM_STATUS_EMERGENCY_ALARM)
        return;

    flag = vdr_current_info.emergency_alarm_flag;
        
    for(int i=0; i<8; i++)
    {
        if((alarmstatus<<i) & Hw7)
        {
            if(flag == 0)
            {
                alarmnum ++;
                flag = 1;
            }
        }
        else
        {
            flag = 0;
        }

    }

    if(alarmnum > 0)
    {
        change_alarm_flag_and_upload(ALARM_STATUS_EMERGENCY_ALARM, 1);
        vdr_current_info.emergency_alarm_num += alarmnum;            
    }
    
    vdr_current_info.emergency_alarm_flag = flag;
        
}

void process_collision_side_turn_alarm()
{
    int t_flag=-1, c_flag=-1;

    if(cst_fd < 0)
    {
        sensor_fd = open("/dev/sensor_timer", O_RDONLY);
        if(sensor_fd < 0)
        {
            printf("open sensor_timer device error!\n");
            return ;
        }
        cst_fd = open("/dev/lis3dh", O_RDONLY);
        if(cst_fd < 0)
        {
            printf("open acceleration device error!\n");
            return ;
        }
        
        int sideturn_angle[2], collision_min = 128;
        int collision_time;
        
        VDRAccelerationTable *table = new VDRAccelerationTable();
        table->GetIntValue(VDR_ACCELERATION_OFFSET_ANGLE, sideturn_angle[1]);
        table->GetIntValue(VDR_ACCELERATION_SIDE_TURN_ANGLE, sideturn_angle[0]);
        table->GetIntValue(VDR_ACCELERATION_COLLIDE_MIN, collision_min);
        table->GetIntValue(VDR_ACCELERATION_COLLIDE_TIME, collision_time);
        delete table;

        ioctl(cst_fd, 0x01, &collision_time);  //set 
        ioctl(cst_fd, 0x06, &collision_min);  //set 
        ioctl(cst_fd, 0x07, sideturn_angle);  //set 
        
    }

    ioctl(cst_fd, 0x00, &t_flag);   // turnover flag
    ioctl(cst_fd, 0x03, &c_flag);   // collision alarm

    if((t_flag == 0) && (vdr_current_info.car_turn_over_flag == 0))  
    {
        vdr_current_info.car_turn_over_flag = 1;
        change_alarm_flag_and_upload(ALARM_STATUS_CAR_TURNOVER_ALARM, 1);
        send_device_alarm_notify(DEV_ALARM_TURNOVER_ALARM, 1);
    }

    if((c_flag == 0) && (vdr_current_info.car_collision_flag== 0))  
    {
        vdr_current_info.car_collision_flag = 1;
        change_alarm_flag_and_upload(ALARM_STATUS_CAR_COLLISION_ALARM, 1);
        send_device_alarm_notify(DEV_ALARM_COLLISION_ALARM, 1);
    }

    if((t_flag != 0) && (vdr_current_info.car_turn_over_flag == 1))  
    {
        vdr_current_info.car_turn_over_flag = 0;
        change_alarm_flag_and_upload(ALARM_STATUS_CAR_TURNOVER_ALARM, 0);
        send_device_alarm_notify(DEV_ALARM_TURNOVER_ALARM, 0);
    }

    if((c_flag != 0) && (vdr_current_info.car_collision_flag== 1))  
    {
        vdr_current_info.car_collision_flag = 0;
        change_alarm_flag_and_upload(ALARM_STATUS_CAR_COLLISION_ALARM, 0);
        send_device_alarm_notify(DEV_ALARM_COLLISION_ALARM, 0);
    }

}

int read_speed_from_cpu(unsigned char *buf)
{
    int len= 0, ret= -1;
    
    while(1)
    {
        ret = read(speed_fd, buf+len, 10);
        if(ret == 0)
            break;
        len += ret;
    }
    #if 0
    printf("revlen=%d, ", len);
    for(int i=0; i<len; i++)
        printf("%d ", buf[i]);
    printf("\n");
    #endif
    return len;
}

void read_mcu_data_handler(unsigned char *mcubuf, int tmptime)
{
    unsigned char accstatus;
    unsigned char alarmstatus;
    int speednum = 5;
    int speed = 0;
    int itime = 0; 
    
    I2c_read_acc_data(accstatus, 1);
    I2c_read_alarm_data(alarmstatus, 1);

    speed = parse_speed_status_info(mcubuf,speednum, tmptime);
    process_alarm_status_info(alarmstatus);

    if(accstatus&Hw0)
        VDRBITSET(vdr_status_flag, LOCSTATUS_ACC_ONO); 
    else
        VDRBITCLR(vdr_status_flag, LOCSTATUS_ACC_ONO);

    if((accstatus&Hw4) && (vdr_current_info.car_smoke_flag == 0))
    {   
        vdr_current_info.car_smoke_flag = 1;
        change_alarm_flag_and_upload(ALARM_STATUS_SMOKE_ALARM, 1);
    }
    else if(!(accstatus&Hw4) && (vdr_current_info.car_smoke_flag == 1))
    {
        vdr_current_info.car_smoke_flag = 0;
        change_alarm_flag_and_upload(ALARM_STATUS_SMOKE_ALARM, 0);
    }

    if((accstatus&Hw5) && (vdr_current_info.undervoltage_flag == 0))
    {  
        vdr_current_info.undervoltage_flag = 1;
        change_alarm_flag_and_upload(ALARM_STATUS_POWER_UNDER_VOLTAGE, 1);
    }
    else if(!(accstatus&Hw5) && (vdr_current_info.undervoltage_flag == 1))
    {
        vdr_current_info.undervoltage_flag = 0;
        change_alarm_flag_and_upload(ALARM_STATUS_POWER_UNDER_VOLTAGE, 0);
    }
    
    itime = vdr_current_info.acc_off_time + vdr_param.sleep_report_interval;
    process_collision_side_turn_alarm(); 
    process_gps_position_info(itime);
    system("sync");
    //send_position_to_JT808_sleep_mode(itime);

}

void process_second_timer_handler(string & name, void * data, void * data2)
{
    unsigned char mcubuf[512];
    int len = 0, icount = 0;
    
    struct timeval tv;
    struct timezone tz;
    int tmptime = 0;
    
    gettimeofday (&tv , &tz);
    tmptime = (int)tv.tv_sec;
    memset(mcubuf, 0, sizeof(mcubuf));
    
    len = read_speed_from_cpu(mcubuf);    
    icount = len/10; 
    printf("readmcu: in sec=%d, usec=%d, icount=%d\n",(int)tv.tv_sec, (int)tv.tv_usec, icount);
    if(icount <= 0)
        return ; 
    
    struct NaviGPSInfo tmp_gps;
    
    pthread_mutex_lock(&gps_mutex);
    memcpy(&tmp_gps, &gps_info, sizeof(struct NaviGPSInfo));
    pthread_mutex_unlock(&gps_mutex);

	if(0 == read_again)
	{
		if(1 == tmp_gps.position || (tmptime - read_time) > 30)
		{
            if(tmptime > vdr_current_info.acc_off_time + vdr_param.sleep_report_interval - 1)
            {
			    read_mcu_data_handler(mcubuf+(icount-1)*10, tmptime);
			    tmp_time= tmptime;
			    read_again = 1;
            }
		}
	}
    else
    {
        if((tmptime - tmp_time) > 90)
        {
            process_acc_off_info_sleep_mode();
        }
        
    }    

}


static void SignalHandler(int sig)
{
    LogUtility::Log(LOG_LEVEL_INFO, "SignalHandler catch signal 0x%x.", sig);
}

int main(int argc, char **argv)
{
    DBusApplicationC app(argc, argv);
    
	DBusClientC *sp_ClientGps = NULL;

    signal(SIGUSR1,SignalHandler);
    signal(SIGPIPE,SignalHandler);

	sp_ClientGps = new DBusClientC((char*)"com.YunTu.Navi", (char*)"/Navi");
	sp_ClientGps->registerNotify(NOTF_ID_NAVI_GPSINFO_REPORT,CNaviGPSInfoReportNotifyHandler_sleep_mode,CNaviGPSInfoReportNotifyC::factory);
    if(sp_ClientGps ->connect() < 0)
    {
        printf("Cann't connect to gps DBUS server.\n");
        return -1;
    }
    
	read_time = time(NULL);

    dev_DbusServer= new DBusServerC(DEVICE_MANAGER_DBUS_SERVER_ID, DEVICE_MANAGER_DBUS_SERVER_OBJ);
    if(dev_DbusServer->connect() < 0)
    {
        printf("Cann't connect to device manager DBUS server.\n");
        return -1;
    }

    sp_ClientJT808 = new DBusClientC(JT808_DBUS_SERVER_ID, JT808_DBUS_SERVER_OBJ);
	sp_ClientJT808->registerReply(JT808_DBUS_REP_ID_LOC_REPORT,JT808LocReportDbusRepC::factory);
	sp_ClientJT808->registerNotify(JT808_DBUS_NTF_ID_LOC_REPORT,JT808LocReportDbusNotifyHandler,JT808LocReportDbusNotifyC::factory);
    
	if(sp_ClientJT808->connect()<0)
    {
        printf("Cann't connect to jt808 DBUS server.\n");
        return -1;
    }
    mcutimer = CreateTimer("readmcu", 1000, (void *)MCU_TIMER_DATA, process_second_timer_handler, true);

    read_vdr_run_param_table();
    read_track_locstatus_table();
    speed_fd = open("/dev/speed", O_RDONLY);
	if(!speed_fd)
		printf("open /dev/speed err\n");
    
    StartTimer(mcutimer);
    send_speed_udp_socket();
    
    return app.loop();
}


