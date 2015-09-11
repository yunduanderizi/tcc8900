#include <stdio.h>
#include <time.h>
#include <string>
#include <boost/locale.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

#include "VDRRunParaTable.h"
#include "VDRTrackInfoTable.h"
#include "ElectronicFenceTable.h"
#include "DriversLoginTable.h"
#include "VDRDeployParaTable.h"
#include "VDRAccelerateTable.h"
#include "TableNameDef.h"

#include "GeneralTimer.h"
#include "LogUtility.h"
#include "Utility.h"

#include "updatedb.h"

//#define UPDATE_DB_TEST 1
#ifndef UPDATE_DB_TEST
#define NAND2_TABLES_DIR "/nand2/tables/"
#define NAND3_TABLES_DIR "/nand4/tables/"
#define TMP_NAND2_TABLES_DIR "/nand2/tmp/tmp_nand2_tables/"
#define TMP_NAND3_TABLES_DIR "/nand2/tmp/tmp_nand3_tables/"
#else
#define NAND2_TABLES_DIR "/nand2/tables/"
#define NAND3_TABLES_DIR "/nand3/tables/"
#define TMP_NAND2_TABLES_DIR "/ttt/nand2_tables/"
#define TMP_NAND3_TABLES_DIR "/ttt/nand3_tables/"
#endif

#define CUR_PATH_LENGTH 200
#define ENV_VARIABLE_NAME_LEN 200
#define ENV_VARIABLE_NUM 2
#define MULITI_TABLE_BUF_LEN 200
#define LARGE_TABLE_BUF_LEN 100
#define DEFAULT_LOC1_PROC_TAB_NUM 30
#define DEFAULT_LOC2_PROC_TAB_NUM 5

//#define DEBUG_LOG 1
//#define PRINT_NOW_TIME 1

using namespace std;

static struct deploy_st deploy;
static struct setting_st setting;
static struct acceleration_st acceleration;
static struct drivers_login_st login[MULITI_TABLE_BUF_LEN];
static struct loc_status_st locstatus[LARGE_TABLE_BUF_LEN];

static int old_hb_cur = 0;//旧数据库当前index
static int new_hb_cur = 0;//新数据库当前index
static int sum_record_count = -1;//当前数据库记录的总条数
static int large_record_count = 0;//loc1/loc2转换时，每次读取的数据条数
static int tmp_record_count = 0;
static int quiet = 0;
  
static char nand2_table_dir[CUR_PATH_LENGTH] = NAND2_TABLES_DIR;
static char nand3_table_dir[CUR_PATH_LENGTH] = NAND3_TABLES_DIR;
static char tmp_nand2_table_dir[CUR_PATH_LENGTH] = TMP_NAND2_TABLES_DIR;
static char tmp_nand3_table_dir[CUR_PATH_LENGTH] = TMP_NAND3_TABLES_DIR;

int db_is_new_hb(int *dep_exists)
{
    //check current database format, only use vdrdeploy.db
    if(*dep_exists)
    {
        int vdr_type, gnss_attr, commu_attr;
        int ret1, ret2, ret3;
        
        VDRDeployParaTable *table = new VDRDeployParaTable();
        ret1 = table->GetIntValue(VDR_DEPLOY_PARA_VDR_TYPE, vdr_type);
        ret2 = table->GetIntValue(VDR_DEPLOY_PARA_GNSS_ATTRIBUTE, gnss_attr);
        ret3 = table->GetIntValue(VDR_DEPLOY_PARA_VDR_TYPE, commu_attr);
        delete table;
        if(!quiet)
            printf("*** ret1 = %d, ret2 = %d, ret3 = %d ***\n",ret1, ret2, ret3);
        
        if(ret1 || ret2 || ret3)
            return 1;
        else
            fprintf(stderr, "Error: the database has already become new hb...\n");
    }

    return 0;
}

void check_single_db(int *dep_exists, int *run_exists, int *acc_exists, int *need_update)
{
    char tmp[CUR_PATH_LENGTH];

    //check whether db exists
    strcpy(tmp, nand2_table_dir);
    strcat(tmp, "vdrdeploy.db");
    if(access(tmp, F_OK) != 0)
    {
        *dep_exists = 0;
        fprintf(stderr, "Error: %s do not exist...\n", tmp);
    }

    strcpy(tmp, nand2_table_dir);
    strcat(tmp, "vdrsetting.db");
    if(access(tmp, F_OK) != 0)
    {
        *run_exists = 0;
        fprintf(stderr, "Error: %s do not exist...\n", tmp);
    }

    strcpy(tmp, nand2_table_dir);
    strcat(tmp, "acceleration.db");
    if(access(tmp, F_OK) != 0)
    {
        *acc_exists = 0;
        fprintf(stderr, "Error: %s do not exist...\n", tmp);
    }

    *need_update = db_is_new_hb(dep_exists);
    if(*need_update == 0)
        return;
    
    //init tmp nand2 tables dir
    if(access(tmp_nand2_table_dir, F_OK) == 0)
    {
        if(*dep_exists || *run_exists || *acc_exists)
        {
            strcpy(tmp, "rm -rf ");
            strcat(tmp, tmp_nand2_table_dir);
            strcat(tmp, "*");
            system(tmp);
            if(!quiet)
                printf("=== %s ===\n", tmp);
        }
    }
    else
    {
        strcpy(tmp, "mkdir -p ");
        strcat(tmp, tmp_nand2_table_dir);
        system(tmp);
        if(!quiet)
            printf("=== %s ===\n", tmp);
    }
}

void check_multi_and_large_db(int *drv_exists, int *loc1_exists, 
    int *loc2_exists, int *need_update, int mode)
{
    char tmp[CUR_PATH_LENGTH];
    
    //check whether db exists
    strcpy(tmp, nand3_table_dir);
    strcat(tmp, "drivers.db");
    if(access(tmp, F_OK) != 0)
    {
        *drv_exists = 0;
        fprintf(stderr, "Error: %s do not exist...\n", tmp);
    }

    strcpy(tmp, nand3_table_dir);
    strcat(tmp, "locstatus1.db");
    if(access(tmp, F_OK) != 0)
    {
        *loc1_exists = 0;
        fprintf(stderr, "Error: %s do not exist...\n", tmp);
    }

    strcpy(tmp, nand3_table_dir);
    strcat(tmp, "locstatus2.db");
    if(access(tmp, F_OK) != 0)
    {
        *loc2_exists = 0;
        fprintf(stderr, "Error: %s do not exist...\n", tmp);
    }

    //the db has not checked
    if(mode != 0)
    {
        char tmp[CUR_PATH_LENGTH];
        int dep_exists;
        strcpy(tmp, nand2_table_dir);
        strcat(tmp, "vdrdeploy.db");
        if(access(tmp, F_OK) != 0)
            dep_exists = 0;
        *need_update = db_is_new_hb(&dep_exists);
        if(*need_update == 0)
            return;
    }

    //init tmp nand3 tables dir
    if(access(tmp_nand3_table_dir, F_OK) == 0)
    {
        if(*drv_exists || *loc1_exists || *loc2_exists)
        {
            strcpy(tmp, "rm -rf ");
            strcat(tmp, tmp_nand3_table_dir);
            if(mode == 0)
            {
                strcat(tmp, "*");
                system(tmp);
            }
            else if(mode == 2)
            {
                strcat(tmp, "drivers.db");
                system(tmp);
            }
            else if(mode == 3)
            {
                strcat(tmp, "locstatus*");
                system(tmp);
            }
            
            if(!quiet)
                printf("=== %s ===\n", tmp);
        }
    }
    else
    {
        strcpy(tmp, "mkdir -p ");
        strcat(tmp, tmp_nand3_table_dir);
        system(tmp);
        if(!quiet)
            printf("=== %s ===\n", tmp);
    }
}

int read_deploy_para_table()
{
    VDRDeployParaTable *table = new VDRDeployParaTable();
    table->GetIntValue(VDR_DEPLOY_PARA_PROVINCE_ID, deploy.province_id);
    table->GetIntValue(VDR_DEPLOY_PARA_CITY_ID, deploy.city_id);
    table->GetSequenceValue(VDR_DEPLOY_PARA_MANUFACTORY_ID, deploy.manufactory_id);
    table->GetSequenceValue(VDR_DEPLOY_PARA_MODEL_NO, deploy.model_no);
    table->GetSequenceValue(VDR_DEPLOY_PARA_ID, deploy.model_id);
    table->GetIntValue(VDR_DEPLOY_PARA_PLATE_COLOR, deploy.plate_color);
    table->GetStringValue(VDR_DEPLOY_PARA_PLATE_ID, deploy.plate_id);
    table->GetStringValue(VDR_DEPLOY_PARA_PLATE_SORT, deploy.plate_sort);
    table->GetStringValue(VDR_DEPLOY_PARA_CAR_VIN, deploy.car_vin);
    table->GetStringValue(VDR_DEPLOY_PARA_VERSION, deploy.version);
    table->GetIntValue(VDR_DEPLOY_PARA_DEPLOY_TIME, deploy.deploy_time);
    table->GetIntValue(VDR_DEPLOY_PARA_DEPLOY_MILEAGE, deploy.deploy_mileage);
    table->GetIntValue(VDR_DEPLOY_PARA_SPEED_PULSE_FACTOR, deploy.speed_pulse_factor);
    table->GetIntValue(VDR_DEPLOY_PARA_LAP_PULSE_FACTOR, deploy.lap_pulse_factor);
    table->GetStringValue(VDR_DEPLOY_PARA_PHONE_NUMBER, deploy.phone_number);
    table->GetStringValue(VDR_DEPLOY_PARA_AUTH_TOKEN, deploy.auto_token);
    delete table;

    return 0;
}

void write_deploy_para_table()
{
    VDRDeployParaTable *table = new VDRDeployParaTable();
    table->SetIntValue(VDR_DEPLOY_PARA_PROVINCE_ID, deploy.province_id);
    table->SetIntValue(VDR_DEPLOY_PARA_CITY_ID, deploy.city_id);
    table->SetSequenceValue(VDR_DEPLOY_PARA_MANUFACTORY_ID, deploy.manufactory_id);
    table->SetSequenceValue(VDR_DEPLOY_PARA_MODEL_NO, deploy.model_no);
    table->SetSequenceValue(VDR_DEPLOY_PARA_ID, deploy.model_id);
    table->SetIntValue(VDR_DEPLOY_PARA_PLATE_COLOR, deploy.plate_color);
    table->SetStringValue(VDR_DEPLOY_PARA_PLATE_ID, deploy.plate_id);
    table->SetStringValue(VDR_DEPLOY_PARA_PLATE_SORT, deploy.plate_sort);
    table->SetStringValue(VDR_DEPLOY_PARA_CAR_VIN, deploy.car_vin);
    table->SetStringValue(VDR_DEPLOY_PARA_VERSION, deploy.version);
    table->SetIntValue(VDR_DEPLOY_PARA_DEPLOY_TIME, deploy.deploy_time);
    table->SetIntValue(VDR_DEPLOY_PARA_DEPLOY_MILEAGE, deploy.deploy_mileage);
    table->SetIntValue(VDR_DEPLOY_PARA_SPEED_PULSE_FACTOR, deploy.speed_pulse_factor);
    table->SetIntValue(VDR_DEPLOY_PARA_LAP_PULSE_FACTOR, deploy.lap_pulse_factor);
    table->SetStringValue(VDR_DEPLOY_PARA_PHONE_NUMBER, deploy.phone_number);
    table->SetStringValue(VDR_DEPLOY_PARA_AUTH_TOKEN, deploy.auto_token);
    table->Commit();
    delete table;
}

int read_vdr_run_para_table()
{
    VDRRunParaTable* table = new VDRRunParaTable();
    table->GetIntValue(VDR_PARA_HEART_BEAT_INTERVAL, setting.heart_beat_interv);
    table->GetIntValue(VDR_PARA_TCP_REPLY_TIMEOUT, setting.tcp_reply_timeout);
    table->GetIntValue(VDR_PARA_TCP_RETRY_TIMES, setting.tcp_retry_times);
    table->GetIntValue(VDR_PARA_UDP_REPLY_TIMEOUT, setting.udp_reply_timesout);
    table->GetIntValue(VDR_PARA_UDP_RETRY_TIMES, setting.udp_retry_times);
    table->GetIntValue(VDR_PARA_SMS_REPLY_TIMEOUT, setting.sms_reply_timeout);
    table->GetIntValue(VDR_PARA_SMS_RETRY_TIMES, setting.sms_retry_times);
    table->GetStringValue(VDR_PARA_MAIN_SERVER_APN, setting.main_server_apn);
    table->GetStringValue(VDR_PARA_MAIN_SERVER_USERNAME, setting.main_server_username);
    table->GetStringValue(VDR_PARA_MAIN_SERVER_PASSWORD, setting.main_server_password);
    table->GetStringValue(VDR_PARA_MAIN_SERVER_ADDRESS, setting.main_server_address);
    table->GetStringValue(VDR_PARA_SECOND_SERVER_APN, setting.second_server_apn);
    table->GetStringValue(VDR_PARA_SECOND_SERVER_USERNAME, setting.second_server_username);
    table->GetStringValue(VDR_PARA_SECOND_SERVER_PASSWORD, setting.second_server_password);
    table->GetStringValue(VDR_PARA_SECOND_SERVER_ADDRESS, setting.second_server_address);
    table->GetIntValue(VDR_PARA_TCP_PORT, setting.tcp_port);
    table->GetIntValue(VDR_PARA_UDP_PORT, setting.udp_port);
    table->GetIntValue(VDR_PARA_LOC_REPORT_POLICY, setting.report_policy);
    table->GetIntValue(VDR_PARA_LOC_REPORT_SCENARIO, setting.report_scenario);
    table->GetIntValue(VDR_PARA_NOLOGIN_REPORT_INTERVAL, setting.nologin_report_interval);
    table->GetIntValue(VDR_PARA_SLEEP_REPORT_INTERVAL, setting.sleep_report_interval);
    table->GetIntValue(VDR_PARA_EMERGENCY_REPORT_INTERVAL, setting.emergency_report_interval);
    table->GetIntValue(VDR_PARA_DEFAULT_REPORT_INTERVAL, setting.default_report_interval);
    table->GetIntValue(VDR_PARA_DEFAULT_REPORT_DISTANCE, setting.default_report_distance);
    table->GetIntValue(VDR_PARA_NOLOGIN_REPORT_DISTANCE, setting.nologin_report_distance);
    table->GetIntValue(VDR_PARA_SLEEP_REPORT_DISTANCE, setting.sleep_report_distance);
    table->GetIntValue(VDR_PARA_EMERGENCY_REPORT_DISTANCE, setting.emergency_report_distance);
    table->GetIntValue(VDR_PARA_TURN_ANGLE, setting.turn_angle); 
    table->GetStringValue(VDR_PARA_PLATFORM_TEL, setting.platform_tel); 
    table->GetStringValue(VDR_PARA_RESET_TEL, setting.reset_tel); 
    table->GetStringValue(VDR_PARA_FACTORY_RESET_TEL, setting.factory_reset_tel); 
    table->GetStringValue(VDR_PARA_PLATFORM_SMS, setting.platform_sms); 
    table->GetStringValue(VDR_PARA_REPORT_SMS, setting.report_sms); 
    table->GetIntValue(VDR_PARA_CALL_ANSWER_POLICY, setting.call_answer_policy); 
    table->GetIntValue(VDR_PARA_CALL_DURATION_ONCE, setting.call_duration_once); 
    table->GetIntValue(VDR_PARA_CALL_DURATION_MONTH, setting.call_duration_month); 
    table->GetStringValue(VDR_PARA_MONITOR_TEL, setting.monitor_tel); 
    table->GetStringValue(VDR_PARA_MONITOR_SMS, setting.monitor_sms); 
    table->GetIntValue(VDR_PARA_WARNING_MASK, setting.warning_mask); 
    table->GetIntValue(VDR_PARA_WARNING_SMS_MASK, setting.warning_sms_mask); 
    table->GetIntValue(VDR_PARA_WARNING_PHOTO_MASK, setting.warning_photo_mask); 
    table->GetIntValue(VDR_PARA_WARNING_PHOTOSAVE_MASK, setting.warning_photosave_mask); 
    table->GetIntValue(VDR_PARA_WARNING_KEY_MASK, setting.warning_key_mask); 
    table->GetIntValue(VDR_PARA_MAX_SPEED, setting.max_speed);    
    table->GetIntValue(VDR_PARA_OVERSPEED_DURATION, setting.over_speed_duration);
    table->GetIntValue(VDR_PARA_CONT_DRIVE_THRESHOLD, setting.continue_drive_threshold);
    table->GetIntValue(VDR_PARA_DAY_DRIVE_THRESHOLD, setting.day_drive_time_threshold);
    table->GetIntValue(VDR_PARA_REST_MIN, setting.min_resting_time);
    table->GetIntValue(VDR_PARA_PARKING_MAX, setting.max_parking_time);
    table->GetIntValue(VDR_PARA_PHOTO_QUALITY, setting.photo_quality);
    table->GetIntValue(VDR_PARA_PHOTO_BRIGHTNESS, setting.photo_brightness);
    table->GetIntValue(VDR_PARA_PHOTO_CONTRAST, setting.photo_contrast);
    table->GetIntValue(VDR_PARA_PHOTO_SATUATION, setting.photo_satuation);
    table->GetIntValue(VDR_PARA_PHOTO_CHROMA, setting.photo_chroma);
    table->GetIntValue(VDR_PARA_FENCE_RADIUS, setting.fence_radius);    
    delete table;

    return 0;
}

void write_vdr_run_para_table()
{
    VDRRunParaTable* table = new VDRRunParaTable();
    table->SetIntValue(VDR_PARA_HEART_BEAT_INTERVAL, setting.heart_beat_interv);
    table->SetIntValue(VDR_PARA_TCP_REPLY_TIMEOUT, setting.tcp_reply_timeout);
    table->SetIntValue(VDR_PARA_TCP_RETRY_TIMES, setting.tcp_retry_times);
    table->SetIntValue(VDR_PARA_UDP_REPLY_TIMEOUT, setting.udp_reply_timesout);
    table->SetIntValue(VDR_PARA_UDP_RETRY_TIMES, setting.udp_retry_times);
    table->SetIntValue(VDR_PARA_SMS_REPLY_TIMEOUT, setting.sms_reply_timeout);
    table->SetIntValue(VDR_PARA_SMS_RETRY_TIMES, setting.sms_retry_times);
    table->SetStringValue(VDR_PARA_MAIN_SERVER_APN, setting.main_server_apn);
    table->SetStringValue(VDR_PARA_MAIN_SERVER_USERNAME, setting.main_server_username);
    table->SetStringValue(VDR_PARA_MAIN_SERVER_PASSWORD, setting.main_server_password);
    table->SetStringValue(VDR_PARA_MAIN_SERVER_ADDRESS, setting.main_server_address);
    table->SetStringValue(VDR_PARA_SECOND_SERVER_APN, setting.second_server_apn);
    table->SetStringValue(VDR_PARA_SECOND_SERVER_USERNAME, setting.second_server_username);
    table->SetStringValue(VDR_PARA_SECOND_SERVER_PASSWORD, setting.second_server_password);
    table->SetStringValue(VDR_PARA_SECOND_SERVER_ADDRESS, setting.second_server_address);
    table->SetIntValue(VDR_PARA_TCP_PORT, setting.tcp_port);
    table->SetIntValue(VDR_PARA_UDP_PORT, setting.udp_port);
    table->SetIntValue(VDR_PARA_LOC_REPORT_POLICY, setting.report_policy);
    table->SetIntValue(VDR_PARA_LOC_REPORT_SCENARIO, setting.report_scenario);
    table->SetIntValue(VDR_PARA_NOLOGIN_REPORT_INTERVAL, setting.nologin_report_interval);
    table->SetIntValue(VDR_PARA_SLEEP_REPORT_INTERVAL, setting.sleep_report_interval);
    table->SetIntValue(VDR_PARA_EMERGENCY_REPORT_INTERVAL, setting.emergency_report_interval);
    table->SetIntValue(VDR_PARA_DEFAULT_REPORT_INTERVAL, setting.default_report_interval);
    table->SetIntValue(VDR_PARA_DEFAULT_REPORT_DISTANCE, setting.default_report_distance);
    table->SetIntValue(VDR_PARA_NOLOGIN_REPORT_DISTANCE, setting.nologin_report_distance);
    table->SetIntValue(VDR_PARA_SLEEP_REPORT_DISTANCE, setting.sleep_report_distance);
    table->SetIntValue(VDR_PARA_EMERGENCY_REPORT_DISTANCE, setting.emergency_report_distance);
    table->SetIntValue(VDR_PARA_TURN_ANGLE, setting.turn_angle); 
    table->SetStringValue(VDR_PARA_PLATFORM_TEL, setting.platform_tel); 
    table->SetStringValue(VDR_PARA_RESET_TEL, setting.reset_tel); 
    table->SetStringValue(VDR_PARA_FACTORY_RESET_TEL, setting.factory_reset_tel); 
    table->SetStringValue(VDR_PARA_PLATFORM_SMS, setting.platform_sms); 
    table->SetStringValue(VDR_PARA_REPORT_SMS, setting.report_sms); 
    table->SetIntValue(VDR_PARA_CALL_ANSWER_POLICY, setting.call_answer_policy); 
    table->SetIntValue(VDR_PARA_CALL_DURATION_ONCE, setting.call_duration_once); 
    table->SetIntValue(VDR_PARA_CALL_DURATION_MONTH, setting.call_duration_month); 
    table->SetStringValue(VDR_PARA_MONITOR_TEL, setting.monitor_tel); 
    table->SetStringValue(VDR_PARA_MONITOR_SMS, setting.monitor_sms); 
    table->SetIntValue(VDR_PARA_WARNING_MASK, setting.warning_mask); 
    table->SetIntValue(VDR_PARA_WARNING_SMS_MASK, setting.warning_sms_mask); 
    table->SetIntValue(VDR_PARA_WARNING_PHOTO_MASK, setting.warning_photo_mask); 
    table->SetIntValue(VDR_PARA_WARNING_PHOTOSAVE_MASK, setting.warning_photosave_mask); 
    table->SetIntValue(VDR_PARA_WARNING_KEY_MASK, setting.warning_key_mask); 
    table->SetIntValue(VDR_PARA_MAX_SPEED, setting.max_speed);    
    table->SetIntValue(VDR_PARA_OVERSPEED_DURATION, setting.over_speed_duration);
    table->SetIntValue(VDR_PARA_CONT_DRIVE_THRESHOLD, setting.continue_drive_threshold);
    table->SetIntValue(VDR_PARA_DAY_DRIVE_THRESHOLD, setting.day_drive_time_threshold);
    table->SetIntValue(VDR_PARA_REST_MIN, setting.min_resting_time);
    table->SetIntValue(VDR_PARA_PARKING_MAX, setting.max_parking_time);
    table->SetIntValue(VDR_PARA_PHOTO_QUALITY, setting.photo_quality);
    table->SetIntValue(VDR_PARA_PHOTO_BRIGHTNESS, setting.photo_brightness);
    table->SetIntValue(VDR_PARA_PHOTO_CONTRAST, setting.photo_contrast);
    table->SetIntValue(VDR_PARA_PHOTO_SATUATION, setting.photo_satuation);
    table->SetIntValue(VDR_PARA_PHOTO_CHROMA, setting.photo_chroma);
    table->SetIntValue(VDR_PARA_FENCE_RADIUS, setting.fence_radius);
    table->Commit();
    delete table;
}

int read_acceleration_table()
{
    VDRAccelerationTable *table = new VDRAccelerationTable();
    table->GetIntValue(VDR_ACCELERATION_OFFSET_ANGLE, acceleration.offset_angle);
    table->GetIntValue(VDR_ACCELERATION_SIDE_TURN_ANGLE, acceleration.side_turn_angle);
    table->GetIntValue(VDR_ACCELERATION_COLLIDE_MIN, acceleration.collide_min);
    delete table;

    return 0;
}

void write_acceleration_table()
{
    VDRAccelerationTable *table = new VDRAccelerationTable();
    table->SetIntValue(VDR_ACCELERATION_OFFSET_ANGLE, acceleration.offset_angle);
    table->SetIntValue(VDR_ACCELERATION_SIDE_TURN_ANGLE, acceleration.side_turn_angle);
    table->SetIntValue(VDR_ACCELERATION_COLLIDE_MIN, acceleration.collide_min);
    table->Commit();
    delete table;    
}

void read_drivers_login_table()
{   
    int count = 0;
    
    DriversLoginTable *table = new DriversLoginTable();
    sum_record_count = table->GetRowCount();
    table->RefreshNext(0);
    while(count < sum_record_count)
    {
        table->GetIntValue(VDR_DRV_OCCUR_TIME, login[count].occur_time);
#ifdef DEBUG_LOG
        printf("login[%d].occur_time = %d\n", count, login[count].occur_time);
#endif
        table->GetIntValue(VDR_DRV_LOGIN_LOGOUT, login[count].login_logout);
        table->GetStringValue(VDR_DRV_DRIVER_LICENSE_NUM, login[count].driver_license_num);
        table->GetSequenceValue(VDR_DRV_VAILD_DATA, login[count].vaild_date);
        table->GetStringValue(VDR_DRV_QUALIFICATION_NUM, login[count].qualification_num);
        table->GetStringValue(VDR_DRV_DRIVER_NAME, login[count].driver_name);
        table->GetStringValue(VDR_DRV_CERTIFYING_AUTHORITY, login[count].certifying_authority);
        table->GetIntValue(VDR_DRV_START_DRIVERING_TIME, login[count].start_drivering_time);
        table->GetIntValue(VDR_DRV_STOT_DRIVERING_TIME, login[count].stop_drivering_time);
        table->RefreshNext(); 
        count++;
        if(count == sum_record_count)
            break;
    }
    delete table;
}

void write_drivers_login_table()
{
    int count = 0;
    DriversLoginTable *table = new DriversLoginTable();
    while(count < sum_record_count)
    {
        table->SetIntValue(VDR_DRV_OCCUR_TIME, login[count].occur_time);
#ifdef DEBUG_LOG
        printf("login[%d].occur_time = %d\n", count, login[count].occur_time);
#endif
        table->SetIntValue(VDR_DRV_LOGIN_LOGOUT, login[count].login_logout);
        table->SetStringValue(VDR_DRV_DRIVER_LICENSE_NUM, login[count].driver_license_num);
        table->SetSequenceValue(VDR_DRV_VAILD_DATA, login[count].vaild_date);
        table->SetStringValue(VDR_DRV_QUALIFICATION_NUM, login[count].qualification_num);
        table->SetStringValue(VDR_DRV_DRIVER_NAME, login[count].driver_name);
        table->SetStringValue(VDR_DRV_CERTIFYING_AUTHORITY, login[count].certifying_authority);
        table->SetIntValue(VDR_DRV_START_DRIVERING_TIME, login[count].start_drivering_time);
        table->SetIntValue(VDR_DRV_STOT_DRIVERING_TIME, login[count].stop_drivering_time);
        /* VDR_DRV_IC_CARD_READ_RESULT has default value */
        table->Commit();
        table->RefreshNext();
        count++;
    }
    delete table;
}

int read_locstatus_table(int type)
{
    LargeTableBase *table;
    if(type == 1)
    {
        VDRLocStatus1Table *loc1_table = new VDRLocStatus1Table();
        table = loc1_table;
    }
    else if(type == 2)
    {
        VDRLocStatus2Table *loc2_table = new VDRLocStatus2Table();
        table = loc2_table;
    }
    else
        return -1;

    large_record_count = 0;
    if(table->RefreshNext(old_hb_cur) < 0)
    {
        delete table;
        fprintf(stderr, "Error: RefreshNext() failed...\n");
        return -1;
    }
    
    while(large_record_count < LARGE_TABLE_BUF_LEN)
    {
        table->GetIntValue(VDR_LOCS_OCCUR_TIME, locstatus[large_record_count].occur_time);
#ifdef DEBUG_LOG
        printf("locstatus%d[%d].occur_time = %d\n", type, large_record_count, locstatus[large_record_count].occur_time);
#endif
        table->GetIntValue(VDR_LOCS_ALARM_SIGNAL, locstatus[large_record_count].alarm_signal);
        table->GetIntValue(VDR_LOCS_STATUS_FLAG, locstatus[large_record_count].status_flag);
        table->GetIntValue(VDR_LOCS_LATITUDE, locstatus[large_record_count].latitude);
        table->GetIntValue(VDR_LOCS_LONGITUDE, locstatus[large_record_count].longitude);
        table->GetIntValue(VDR_LOCS_ALTITUDE, locstatus[large_record_count].altitude);
        table->GetIntValue(VDR_LOCS_ORIENTATION, locstatus[large_record_count].orientation);
        table->GetIntValue(VDR_LOCS_GPS_SPEED, locstatus[large_record_count].gps_speed);
        table->GetIntValue(VDR_LOCS_TOTAL_MILEAGE, locstatus[large_record_count].total_mileage);
        table->GetIntValue(VDR_LOCS_OIL_MASS, locstatus[large_record_count].oil_mass);
        table->GetIntValue(VDR_LOCS_VDR_SPEED, locstatus[large_record_count].vdr_speed);
        table->GetIntValue(VDR_LOCS_VDR_STATUS, locstatus[large_record_count].vdr_status);
        table->GetIntValue(VDR_LOCS_VDR_DISTANCE, locstatus[large_record_count].vdr_distance);
        table->GetIntValue(VDR_LOCS_OVERSPEED_ALARM, locstatus[large_record_count].overspeed_alarm);
        table->GetIntValue(VDR_LOCS_OVERSPEED_REGIONID, locstatus[large_record_count].overspeed_region_id);
        table->GetIntValue(VDR_LOCS_INOUT_ALARM, locstatus[large_record_count].inout_alarm);
        table->GetIntValue(VDR_LOCS_INOUT_REGIONID, locstatus[large_record_count].inout_region_id);
        table->GetIntValue(VDR_LOCS_INOUT_DIRECTION, locstatus[large_record_count].inout_direction);
        table->GetIntValue(VDR_LOCS_SHORT_LONG_LINEID, locstatus[large_record_count].short_long_line_id);
        table->GetIntValue(VDR_LOCS_SHORT_LONG_TIME, locstatus[large_record_count].short_long_time);
        table->GetIntValue(VDR_LOCS_SHORT_LONG_RESULT, locstatus[large_record_count].short_long_result);
        table->GetIntValue(VDR_LOCS_EMERGENCY_ALARM_ID, locstatus[large_record_count].emergency_alarm_id);
        table->RefreshNext(++old_hb_cur);
        if(type == 1)
        {
            if(old_hb_cur == VDR_TRACK_LOCATION_STATUS1_TABLE_MAX_ROWS)
            {
                old_hb_cur = 0;
                printf("current index -----> 0\n");
            }
        }
        if(type == 2)
        {
            if(old_hb_cur == VDR_TRACK_LOCATION_STATUS2_TABLE_MAX_ROWS)
            {
                old_hb_cur = 0;
                printf("current index -----> 0\n");
            }
        }
        large_record_count++;
        tmp_record_count--;
        if(tmp_record_count == 0)
        {
            delete table;
            return -1;
        }
    }
    delete table;

    return 0;
}

void write_locstatus_table(int type)
{
    int count = 0;
    
    LargeTableBase *table;
    if(type == 1)
    {
        VDRLocStatus1Table *loc1_table = new VDRLocStatus1Table();
        table = loc1_table;
    }
    else if(type == 2)
    {
        VDRLocStatus2Table *loc2_table = new VDRLocStatus2Table();
        table = loc2_table;
    }
    else
        return;
    
    table->RefreshBottom();
    while(count < large_record_count)
    {
        table->SetIntValue(VDR_LOCS_OCCUR_TIME, locstatus[count].occur_time);
#ifdef DEBUG_LOG
        printf("locstatus%d[%d].occur_time = %d\n", type, count, locstatus[count].occur_time);
#endif
        table->SetIntValue(VDR_LOCS_ALARM_SIGNAL, locstatus[count].alarm_signal);
        table->SetIntValue(VDR_LOCS_STATUS_FLAG, locstatus[count].status_flag);
        table->SetIntValue(VDR_LOCS_LATITUDE, locstatus[count].latitude);
        table->SetIntValue(VDR_LOCS_LONGITUDE, locstatus[count].longitude);
        table->SetIntValue(VDR_LOCS_ALTITUDE, locstatus[count].altitude);
        table->SetIntValue(VDR_LOCS_ORIENTATION, locstatus[count].orientation);
        table->SetIntValue(VDR_LOCS_GPS_SPEED, locstatus[count].gps_speed);
        table->SetIntValue(VDR_LOCS_TOTAL_MILEAGE, locstatus[count].total_mileage);
        table->SetIntValue(VDR_LOCS_OIL_MASS, locstatus[count].oil_mass);
        table->SetIntValue(VDR_LOCS_VDR_SPEED, locstatus[count].vdr_speed);
        table->SetIntValue(VDR_LOCS_VDR_STATUS, locstatus[count].vdr_status);
        table->SetIntValue(VDR_LOCS_VDR_DISTANCE, locstatus[count].vdr_distance);
        table->SetIntValue(VDR_LOCS_OVERSPEED_ALARM, locstatus[count].overspeed_alarm);
        table->SetIntValue(VDR_LOCS_OVERSPEED_REGIONID, locstatus[count].overspeed_region_id);
        table->SetIntValue(VDR_LOCS_INOUT_ALARM, locstatus[count].inout_alarm);
        table->SetIntValue(VDR_LOCS_INOUT_REGIONID, locstatus[count].inout_region_id);
        table->SetIntValue(VDR_LOCS_INOUT_DIRECTION, locstatus[count].inout_direction);
        table->SetIntValue(VDR_LOCS_SHORT_LONG_LINEID, locstatus[count].short_long_line_id);
        table->SetIntValue(VDR_LOCS_SHORT_LONG_TIME, locstatus[count].short_long_time);
        table->SetIntValue(VDR_LOCS_SHORT_LONG_RESULT, locstatus[count].short_long_result);
        table->SetIntValue(VDR_LOCS_EMERGENCY_ALARM_ID, locstatus[count].emergency_alarm_id);

        table->SetIntValue(VDR_LOCS_VDR_NEW_HB_STATUS, 0);//NOT NULL & no default
        table->SetIntValue(VDR_LOCS_IO_STATUS, 0);//NOT NULL & no default
        table->SetIntValue(VDR_LOCS_ANALOG_QUANTITY, 0);//NOT NULL & no default
        table->SetIntValue(VDR_LOCS_WIRELESS_INTENSITY, 0);//NOT NULL & no default
        table->SetIntValue(VDR_LOCS_GNSS_SATELLITE_SUM, 0);//NOT NULL & no default
        table->Add();
        table->RefreshNext(new_hb_cur++);
        count++;
    }
    delete table;
}

int process_single_tables(int dep_exists_flag, int run_exists_flag, int acc_exists_flag)
{
    setenv("INNOV_IMPORTANT_TABLE_DIR", nand2_table_dir, 1);
#ifdef PRINT_NOW_TIME
    printf("now time = %d\n", (int)time(NULL));
#endif
    if(!quiet)
        printf("--- read old hb single tables ---\n");

    if(dep_exists_flag)
        read_deploy_para_table();

    if(run_exists_flag)
        read_vdr_run_para_table();

    if(acc_exists_flag)
        read_acceleration_table();
    setenv("INNOV_IMPORTANT_TABLE_DIR", tmp_nand2_table_dir, 1);
#ifdef PRINT_NOW_TIME
    printf("now time = %d\n", (int)time(NULL));
#endif
    if(!quiet)
        printf("--- write new hb single tables ---\n");
    
    if(dep_exists_flag)
        write_deploy_para_table();
    
    if(run_exists_flag)
        write_vdr_run_para_table();

    if(acc_exists_flag)
        write_acceleration_table();
    
    setenv("INNOV_IMPORTANT_TABLE_DIR", nand2_table_dir, 1); 
    
    return 0;
}

int process_multi_tables(int drv_exists_flag)
{
    //drivers.db
    if(!drv_exists_flag)
        return -1;
    
    setenv("INNOV_TABLE_DIR", nand3_table_dir, 1); 
#ifdef PRINT_NOW_TIME
    printf("now time = %d\n", (int)time(NULL));
#endif
    if(!quiet)
        printf("--- read old hb drivers login table ---\n");
    
    read_drivers_login_table();
    printf("sum_record_count = %d\n", sum_record_count);
    setenv("INNOV_TABLE_DIR", tmp_nand3_table_dir, 1); 
#ifdef PRINT_NOW_TIME
    printf("now time = %d\n", (int)time(NULL));
#endif
    if(!quiet)
        printf("--- write new hb drivers login table ---\n");
    
    write_drivers_login_table();
#ifdef PRINT_NOW_TIME
    printf("now time = %d\n", (int)time(NULL));
#endif

    setenv("INNOV_TABLE_DIR", nand3_table_dir, 1); 

    return 0;
}

int process_locstatus_tables(int type, int table_num, int loc_exists_flag)
{
    int ret;
    int bottom_index;
    int record_num;
    int loc_schema_colnum = 0;
    int loc_schema_table = 0;
    int loc_schema_index = 0;

    if(!loc_exists_flag)
        return -2;
    
    old_hb_cur = -1;
    new_hb_cur = -1;
    large_record_count = 0;
    setenv("INNOV_TABLE_DIR", nand3_table_dir, 1);
    
    if(type == 1)
    {
        if(!quiet)
            printf("######## begin to proc loc1 ########\n");
        
        record_num = table_num * 100;
        VDRLocStatus1Table *loc1_table = new VDRLocStatus1Table();
        loc1_table->GetCurrentTableAndIndex(loc_schema_colnum,loc_schema_table,loc_schema_index);
        if(!quiet)
            printf("=== loc%d colnum=%d,table=%d,index=%d ===\n",type,loc_schema_colnum,loc_schema_table,loc_schema_index);
        delete loc1_table;
    }
    else if(type == 2)
    {
        if(!quiet)
            printf("######## begin to proc loc2 ########\n");
        
        record_num = table_num * 100;
        VDRLocStatus2Table *loc2_table = new VDRLocStatus2Table();
        loc2_table->GetCurrentTableAndIndex(loc_schema_colnum,loc_schema_table,loc_schema_index);
        if(!quiet)
            printf("=== loc%d colnum=%d,table=%d,index=%d ===\n",type,loc_schema_colnum,loc_schema_table,loc_schema_index);
        delete loc2_table;
    }

    if(loc_schema_colnum == 0)
    {
       fprintf(stderr, "Error: locstatus%d is null...\n", type);
       return -1;
    }
    
    if(loc_schema_colnum < record_num)
    {
        record_num = loc_schema_colnum;
        fprintf(stderr, "Error: locstatus%d has not enough records, so change all records.\n", type);
    }
    
    tmp_record_count = record_num;
    bottom_index = loc_schema_table*100 + loc_schema_index + 1;//bottom_index != loc_schema_colnum
    if(record_num > bottom_index)
        old_hb_cur = VDR_TRACK_LOCATION_STATUS1_TABLE_MAX_ROWS - (record_num - bottom_index) - 1;
    else
        old_hb_cur = bottom_index - record_num;

    old_hb_cur -= 1;
    
    if(!quiet)
        printf("=== start index: old_hb_cur = %d ===\n",old_hb_cur);
    
    while(1)
    {
#ifdef PRINT_NOW_TIME
        printf("now time = %d\n", (int)time(NULL));
#endif
        if(!quiet)
            printf("--- read old hb loc table ---\n");
        
        ret = read_locstatus_table(type);
        if(!quiet)
            printf("loc_schema_colnum = %d, old_hb_cur = %d, new_hb_cur = %d\n", loc_schema_colnum, old_hb_cur, new_hb_cur);
        setenv("INNOV_TABLE_DIR", tmp_nand3_table_dir, 1);
#ifdef PRINT_NOW_TIME
        printf("now time = %d\n", (int)time(NULL));
#endif
        if(!quiet)
            printf("--- write new hb loc table ---\n");
        
        write_locstatus_table(type);
        setenv("INNOV_TABLE_DIR", nand3_table_dir, 1);
        if(ret < 0)
            break;
    }

#ifdef PRINT_NOW_TIME
    printf("now time = %d\n", (int)time(NULL));
#endif
    

    return 0;
}

void print_usage()
{
    char name[CUR_PATH_LENGTH];
    strcpy(name, "./updatedb");
    fprintf(stderr, 
        "Usage: %s [-help][-h]\n"
        "%*s  [-quiet][-q][-mode 0|1|2|3]\n"
        "%*s  [-env1 environment_variable_1_path][env2 environment_variable_2_path]\n"
        "%*s  [-dir1 tmp_nand2_tables_dir] [-dir2 tmp_nand3_tables_dir]\n\n"
        "%*s  [-loc1 number] [-loc2 number]\n\n"
        "%*s  -h, -help: print usage\n"
        "%*s  -q, -quiet: don't print log\n"
        "%*s  -mode, -m: 0 ---> change all types tables\n"
        "%*s             1 ---> only change single  tables\n"
        "%*s             2 ---> only change multi tables\n"
        "%*s             3 ---> only change large tables\n"
        "%*s  -env1: \n"
        "%*s         environment_variable_1_path: the value of INNOV_IMPORTANT_TABLE_DIR\n"
        "%*s  -env2: \n"
        "%*s         environment_variable_2_path: the value of INNOV_TABLE_DIR\n"
        "%*s  -dir1: \n"
        "%*s         tmp_nand2_tables_dir: a path which temporary store the converted database of nand2_tables\n"
        "%*s  -dir2: \n"
        "%*s         tmp_nand3_tables_dir: a path which temporary store the converted database of nand3_tables\n"
        "%*s  -loc1: \n"
        "%*s         number: The number needed to convert the locstatus1.db table\n"
        "%*s  -loc2: \n"
        "%*s         number: The number needed to convert the locstatus2.db table\n"
        "Example: \n"
        "\t%s -mode 1 -env1 /nand2/tables/ -env2 /nand4/tables/ -dir1 /tmp1/ -dir2 /tmp2/ -loc1 50 -loc2 5\n\n",
name, 
(int)strlen(name),"", (int)strlen(name),"", (int)strlen(name),"", 
(int)strlen(name),"", (int)strlen(name),"", (int)strlen(name),"",
(int)strlen(name),"", (int)strlen(name),"", (int)strlen(name),"",
(int)strlen(name),"", (int)strlen(name),"", (int)strlen(name),"", 
(int)strlen(name),"", (int)strlen(name),"", (int)strlen(name),"", 
(int)strlen(name),"", (int)strlen(name),"", (int)strlen(name),"", 
(int)strlen(name),"", (int)strlen(name),"", (int)strlen(name),"", 
(int)strlen(name),"", 
name
);
}


extern char *optarg;
extern int optind, opterr, optopt;

int  main(int argc, char **argv)
{    
    int c;
    int mode = 0;//0:all 1:single 2:multi 3:large
    int dep_exists = 1;
    int run_exists = 1;
    int acc_exists = 1;
    int drv_exists = 1;
    int loc1_exists = 1;
    int loc2_exists = 1;
    int loc1_tables_num = DEFAULT_LOC1_PROC_TAB_NUM;
    int loc2_tables_num = DEFAULT_LOC2_PROC_TAB_NUM;
    int db_need_update = 1;

    if(argc < 2)
    {
        print_usage();
        return -1;
    }
    for(int i = 0; i < argc; i++)
        if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "-help"))
        {
            print_usage();
            return 1;
        }
            
    struct option long_options [] = 
    {
        {"help",no_argument,NULL,'h'},
        {"mode",required_argument,NULL,'m'},
        {"env1",required_argument,NULL,'e'},
        {"env2",required_argument,NULL,'f'},
        {"dir1",required_argument,NULL,'d'},
        {"dir2",required_argument,NULL,'g'},
        {"loc1",required_argument,NULL,'o'},
        {"loc2",required_argument,NULL,'p'},    
        {"quiet",no_argument,NULL,'q'},
        {NULL,0,NULL,0} 
    };
    int option_index = 0;

    /* Process options */
    while((c = getopt_long_only(argc, argv, "hm:e:f:d:g:q", long_options, &option_index)) != -1)
    {
        switch (c) 
        {
            case 'h':
                print_usage();
                return 1;
            case 'm':
                mode = atoi(optarg);
                break;
            case 'e':
                strcpy(nand2_table_dir, optarg);
                break;
            case 'f':
                strcpy(nand3_table_dir, optarg);
                break;
            case 'd':
                strcpy(tmp_nand2_table_dir, optarg);
                break;
            case 'g':
                strcpy(tmp_nand3_table_dir, optarg);
                break;
            case 'o':
                loc1_tables_num = atoi(optarg);
                break;
            case 'p':
                loc2_tables_num = atoi(optarg);
                break;
            case 'q':
                quiet = 1;
                break;
            case '?':
                fprintf(stderr, "Error:invalid argument %c\n", c);
                print_usage();
                return 1;
                break;
            default:
                break;
        }
    }
    
    if(nand2_table_dir[strlen(nand2_table_dir) - 1] != '/')
        strcat(nand2_table_dir, "/");
    if(nand3_table_dir[strlen(nand3_table_dir) - 1] != '/')
        strcat(nand3_table_dir, "/");
    if(tmp_nand2_table_dir[strlen(tmp_nand2_table_dir) - 1] != '/')
        strcat(tmp_nand2_table_dir, "/");
    if(tmp_nand3_table_dir[strlen(tmp_nand3_table_dir) - 1] != '/')
        strcat(tmp_nand3_table_dir, "/");
    
    if(!quiet)
        printf("mode:%d, nand2_path:%s, nand3_path:%s, tmpnand2:%s, tmpnand3:%s, loc1:%d, loc2:%d\n",
            mode, nand2_table_dir, nand3_table_dir, tmp_nand2_table_dir, tmp_nand3_table_dir, 
            loc1_tables_num, loc2_tables_num);

    setenv("INNOV_IMPORTANT_TABLE_DIR", nand2_table_dir, 1);
    //setenv("INNOV_TABLE_DIR", nand3_table_dir, 1);
    
    if(mode == 2)
        goto MULTI_TABLES;
    else if(mode == 3)
        goto LARGE_TABLES;

    check_single_db(&dep_exists, &run_exists, &acc_exists, &db_need_update);
    if(db_need_update)
        process_single_tables(dep_exists, run_exists, acc_exists);
    else
        return 2;//don't need update
    
    if(mode == 1)
        return 0;
    
MULTI_TABLES:
    check_multi_and_large_db(&drv_exists, &loc1_exists, &loc2_exists, &db_need_update, mode);
    if(db_need_update)
        process_multi_tables(drv_exists);
    else
        return 2;//don't need update
    
    if(mode == 2)
        return 0;
    
LARGE_TABLES:
    if(mode == 3)
        check_multi_and_large_db(&drv_exists, &loc1_exists, &loc2_exists, &db_need_update, mode);

    if(db_need_update)
    {
        process_locstatus_tables(1, loc1_tables_num, loc1_exists);
        process_locstatus_tables(2, loc2_tables_num, loc2_exists);
    }
    else
        return 2;//don't need update

    return 0;
}
