#ifndef _CHANGE_DATABASE_H_
#define _CHANGE_DATABASE_H_

#include "VDRRunParaTable.h"
#include "VDRTrackInfoTable.h"
#include "ElectronicFenceTable.h"
#include "DriversLoginTable.h"
#include "VDRDeployParaTable.h"
#include "VDRAccelerateTable.h"


struct deploy_st
{
    int province_id;
    int city_id;
    vector<unsigned char> manufactory_id;
    vector<unsigned char> model_no;
    vector<unsigned char> model_id;
    int plate_color;
    string plate_id;
    string plate_sort;
    string car_vin;
    string version;
    int deploy_time;
    int deploy_mileage;
    int speed_pulse_factor;
    int lap_pulse_factor;
    string phone_number;
    string auto_token;
};

struct setting_st 
{
    int heart_beat_interv;
    int tcp_reply_timeout;
    int tcp_retry_times;
    int udp_reply_timesout;
    int udp_retry_times;
    int sms_reply_timeout;
    int sms_retry_times;
    string main_server_apn;
    string main_server_username;
    string main_server_password;
    string main_server_address;
    string second_server_apn;
    string second_server_username;
    string second_server_password;
    string second_server_address;
    int tcp_port;
    int udp_port;
    int report_policy;
    int	report_scenario;
    int nologin_report_interval;
    int sleep_report_interval;
    int emergency_report_interval;
    int default_report_interval;
    int default_report_distance;
    int nologin_report_distance;
    int sleep_report_distance;
    int emergency_report_distance;
	int turn_angle;
	string platform_tel;
	string reset_tel;
	string factory_reset_tel;
	string platform_sms;
	string report_sms;
	int call_answer_policy;
	int call_duration_once;
	int call_duration_month;
	string monitor_tel;
	string monitor_sms;
    int warning_mask;
	int warning_sms_mask;
    int warning_photo_mask;
    int warning_photosave_mask;
	int warning_key_mask;
    int max_speed;
    int over_speed_duration;
    int continue_drive_threshold;
    int day_drive_time_threshold;
    int min_resting_time;
    int max_parking_time;
	int photo_quality;
	int photo_brightness;
	int photo_contrast;
	int photo_satuation;
	int photo_chroma;
	int fence_radius;
};

struct acceleration_st
{
    int offset_angle;
    int side_turn_angle;
    int collide_min;
};

struct drivers_login_st
{
    int occur_time;
    int login_logout;
    string driver_license_num;
    vector<unsigned char> vaild_date;
    string qualification_num;
    string driver_name;
    string certifying_authority;
    int start_drivering_time;
    int stop_drivering_time;
};

struct loc_status_st
{
    int occur_time;
    int alarm_signal;
    int status_flag;
    int latitude;
    int longitude;
    int altitude;
    int orientation;
    int gps_speed;
    int total_mileage;
    int oil_mass;
    int vdr_speed;
    int vdr_status;
    int vdr_distance;
    int overspeed_alarm;
    int overspeed_region_id;
    int inout_alarm;
    int inout_region_id;
    int inout_direction;
    int short_long_line_id;
    int short_long_time;
    int short_long_result;
    int emergency_alarm_id;
 };

#endif
