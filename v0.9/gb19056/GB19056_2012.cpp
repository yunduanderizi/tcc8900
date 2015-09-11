#include "GB19056_2012.h"
#include "LogUtility.h"
#include "DriversLoginTable.h"
#include "VDRTrackInfoTable.h"
#include "VDRDeployParaTable.h"
#include "VDRRunParaTable.h"
#include "Utility.h"
#include <boost/locale.hpp>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/ioctl.h>

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


#define AT88SC_IOCTL_BASE      'C'
#define AT88SC_DATA            _IOW (AT88SC_IOCTL_BASE, 0, struct at88sc_ioctl_data)
#define AT88SC_PRODUCT_ID      _IOW (AT88SC_IOCTL_BASE, 1, struct at88sc_ioctl_data)

struct at88sc_ioctl_data {	
	unsigned char rw;
	unsigned char userzone;
	unsigned char password[13];
};

int read_tuid_from_at88sc(unsigned char *tuid)
{
    int fd;
    unsigned char passbuf[13]={'i','N','N','O','V',8,8,8,9,2,8,7,5};
    struct at88sc_ioctl_data ioctldata;
    
    fd = open("/dev/at88sc", O_RDONLY);
    if(fd < 0)
    {
        cerr << "Failed to open device." << endl;
        return -1;
    }
    
    ioctldata.rw = 1;
    ioctldata.userzone = 1;
    memcpy(ioctldata.password, passbuf, 13);
    
    if(ioctl(fd, AT88SC_DATA, &ioctldata) < 0)
    {
        cerr << "Failed to control device." << endl;
        close(fd);
        return -1;
    }
    
    if(read(fd, tuid, 32) != 32)
    {
        cerr << "Failed to read device." << endl;
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

int read_tuid_from_emmc(char * tuid)
{
	FILE* fp = NULL; 
    int ret;
    
    fp = fopen("/nand2/tuid.conf", "r");
    if(NULL == fp)
    {
        cerr << "Failed to open nor flash tuid.conf ." << endl;
        return -1;
    }
    printf("read tuid from nor flash ...\n");
    fgets(tuid, 33, fp);
    fclose(fp);
    return 0;
}

int read_tuid_get_ymd(unsigned char *buf)
{
    int ret;
    unsigned char tuid[33];
    
    ret = read_tuid_from_emmc((char *)tuid);
    if(ret < 0)
    {
        ret = read_tuid_from_at88sc(tuid);
        if(ret < 0)
        {
            cout <<"read tuid from emmc at88sc error.\n" <<endl;
            return -1;
        }
    }

    buf[0] = (tuid[8]-'0')*10 + (tuid[9]-'0');
    buf[1] = (tuid[10]-'0')*10 + (tuid[11]-'0');
    buf[2] = (tuid[12]-'0')*10 + (tuid[13]-'0');
        
    return 0;
}

static int DB9_check_flag = 0;

int GB19056_2012::ProcessCmd(GB19056Buffer &buffer)
{
    int ret = 0;
    int itime = (int)time(NULL);
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "Enter ProcessCmd.");
    
    switch(buffer.GetCmd())
    {
        case (unsigned char)ENUM_GB19056_CMD_QUERY_VER:
            ret = ProcessVersionQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_DRIVER:
            ret = ProcessDriverQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_TIME:
            ret = ProcessTimeQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_MILEAGE:
            ret = ProcessMileageQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_PULSE_FACTOR:
            ret = ProcessPulseFactorQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_CAR_INFO:
            ret = ProcessCarInfoQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_STATUS:
            ret = ProcessStatusQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_ID:
            ret = ProcessIdQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_RECORD:
            ret = ProcessFileDriveRecordQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_LOC_RECORD:
            ret = ProcessFileLocRecordQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_ACCIDENT_RECORD:
            ret = ProcessAccidentRecordQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_OVERTIME_RECORD:
            ret = ProcessTiredRecordQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_DRIVER_RECORD:
            ret = ProcessDriverInfoRecordQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_POWER_RECORD:
            ret = ProcessPowerRecordQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_PARA_MODIFY_RECORD:
            ret = ProcessParaSettingRecordQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_STATUS_RECORD:
            ret = ProcessSpeedInfoRecordQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_LAP_PULSE_FACTOR:
            ret = ProcessLapPulseFactorQuery(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_SET_CAR_INFO:
            //if(GetDriverAuthority() != 0)
                //return -1;
            ret = ProcessCarInfoSet(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_SET_DEPLOY_TIME:
            //if(GetDriverAuthority() != 0)
                //return -1;
            ret = ProcessDeployTimeSet(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_SET_STATUS_CONFIG:
            //if(GetDriverAuthority() != 0)
                //return -1;
            ret = ProcessStatusConfigSet(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_SET_TIME:
            //if(GetDriverAuthority() != 0)
                //return -1;
            ret = ProcessTimeSet(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_SET_PULSE_RATE:
            //if(GetDriverAuthority() != 0)
                //return -1;
            ret = ProcessPulseFactorSet(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_SET_DEPLOY_MILEAGE:
            //if(GetDriverAuthority() != 0)
                //return -1;
            ret = ProcessDeployMileageSet(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_SET_STATUS_CONFIG_2:
            //if(GetDriverAuthority() != 0)
                //return -1;
            ret = ProcessStatusConfig2Set(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_SET_LAP_PULSE_FACTOR:
            //if(GetDriverAuthority() != 0)
                //return -1;
            ret = ProcessLapPulseFactorSet(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_CHECK_IN:
            ret = ProcessInCheck(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_CHECK_MILEAGE:
            ret = ProcessMileageCheck(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_CHECK_PULSE_FACTOR:
            ret = ProcessPulseFactorCheck(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_CHECK_TIME:
            ret = ProcessTimeCheck(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_CHECK_OUT:
            ret = ProcessOutCheck(buffer);
            break;
            
        case (unsigned char)ENUM_PC_VDR_CMD_QUERY_MODE_ID:
            ret = ProcessQueryModeID(buffer);
            break;
        case (unsigned char)ENUM_PC_VDR_CMD_SET_SERVER_INFO:
            ret = ProcessSetServerInfo(buffer);
            break;
        case (unsigned char)ENUM_PC_VDR_CMD_SET_DEPLOY_PARA:
            ret = ProcessSetDeployPara(buffer);
            break;  
        case (unsigned char)ENUM_PC_VDR_CMD_SET_AUTHTOKEN:
            ret = ProcessSetAuthorken(buffer);
            break;
        case (unsigned char)ENUM_PC_VDR_CMD_SET_PULSE_FACTOR:
            ret = ProcessSetPulseFactor(buffer);
            break; 
        case (unsigned char)ENUM_PC_VDR_CMD_SET_REBOOT:
            ret = ProcessSetReboot(buffer);
            break;
            
        default:
            GenerateError(buffer);
            return -1;
    }
    
    if(IS_CMD_SET(buffer.GetCmd()))
    {
        WriteEventLog(buffer,itime);
    }
    
    if(ret < 0)
    {
        return -1;
    }
    
    return 0;
}

int GB19056_2012::ProcessCmd_JT808(GB19056Buffer &buffer)
{
    int ret = 0;
    int itime = (int)time(NULL);
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "Enter ProcessCmd.");
    
    switch(buffer.GetCmd())
    {
        case (unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_RECORD:
            ret = ProcessFileDriveRecordQuery_JT808(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_LOC_RECORD:
            ret = ProcessFileLocRecordQuery_JT808(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_ACCIDENT_RECORD:
            ret = ProcessAccidentRecordQuery_JT808(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_OVERTIME_RECORD:
            ret = ProcessTiredRecordQuery_JT808(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_DRIVER_RECORD:
            ret = ProcessDriverInfoRecordQuery_JT808(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_POWER_RECORD:
            ret = ProcessPowerRecordQuery_JT808(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_PARA_MODIFY_RECORD:
            ret = ProcessParaSettingRecordQuery_JT808(buffer);
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_STATUS_RECORD:
            ret = ProcessSpeedInfoRecordQuery_JT808(buffer);
            break;
        default:
            GenerateError(buffer);
            return -1;
    }
    
    if(IS_CMD_SET(buffer.GetCmd()))
    {
        WriteEventLog(buffer,itime);
    }
    
    if(ret < 0)
    {
        return -1;
    }
    
    return 0;
}

int GB19056_2012::ProcessVersionQuery(GB19056Buffer &buffer)
{
    buffer.PushByte(0x12);
    buffer.PushByte(0x00);
    return 0;
}

int GB19056_2012::GetDriverAuthority()
{
    DriversLoginTable *table = new DriversLoginTable();
    int login;
    string drivername;
    string authority="管理员";
    int ret = -1;
    
    if(table->RefreshBottom() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverQuery failed to refresh table.");
        delete table;
    }
    else
    {
        table->GetIntValue(VDR_DRV_LOGIN_LOGOUT, login);
        table->GetStringValue(VDR_DRV_DRIVER_NAME, drivername);
        delete table;
        if(login)
        {
            ret = drivername.compare(authority);
        }
    }
    printf("name=%s, ret=%d, login=%d, %s\n",drivername.c_str(), ret,login, authority.c_str());
    return ret;
}

int GB19056_2012::ProcessDriverQuery(GB19056Buffer &buffer)
{
    DriversLoginTable *table = new DriversLoginTable();
    int login;
    string license="000000000000000000";
    
    if(table->RefreshBottom() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverQuery failed to refresh table.");
        delete table;
    }
    else
    {
        table->GetIntValue(VDR_DRV_LOGIN_LOGOUT, login);
        table->GetStringValue(VDR_DRV_DRIVER_LICENSE_NUM, license);
        delete table;
        if(!login)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverQuery no driver login.");
            license="000000000000000000";
        }
    }
    
    buffer.PushString(license, 18);
    return 0;
}

int GB19056_2012::ProcessTimeQuery(GB19056Buffer &buffer)
{
    vector<unsigned char> bcd;

    GetTime((long long)time(NULL), bcd);
    
    buffer.PushVector(bcd, 6);
    return 0;
}

int GB19056_2012::ProcessMileageQuery(GB19056Buffer &buffer)
{
    vector<unsigned char> bcd;
    GetTime((long long)time(NULL), bcd);
    buffer.PushVector(bcd, 6);

    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessMileageQuery failed to refresh deploy_table.");
        delete deploy_table;
        return -1;
    }
    int deploy_time, deploy_mileage;
    deploy_table->GetIntValue(VDR_DEPLOY_PARA_DEPLOY_TIME, deploy_time);
    deploy_table->GetIntValue(VDR_DEPLOY_PARA_DEPLOY_MILEAGE, deploy_mileage);
    delete deploy_table;
    bcd.clear();
    GetTime((long long)deploy_time, bcd);
    buffer.PushVector(bcd, 6);
    
    bcd.clear();
    IntToBcd(deploy_mileage, bcd, 4);
    buffer.PushVector(bcd, 4);

#if 0
    VDRLocStatus1Table *loc_table = new VDRLocStatus1Table();
    if(loc_table->RefreshBottom() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessMileageQuery RefreshBottom loc_table failed.");
        delete loc_table;
        return -1;
    }
    
    int total_mileage = 0;
    loc_table->GetIntValue(VDR_LOCS_TOTAL_MILEAGE, total_mileage);
    delete loc_table;
    
    total_mileage += deploy_mileage;
#endif

    FILE *fp = NULL;
    int num = 5;
    struct LocStatusData locbuf[5];
    int total_mileage = 0;
    
    memset(locbuf,0,sizeof(struct LocStatusData)*5);

    fp = fopen("/nand2/loc0809.vdr", "r");
    
	if(fp == NULL) 
	{
		printf("Can't open loc0809.vdr %s\n", strerror(errno));
	}
    else
    {
        fread(locbuf,sizeof(struct LocStatusData),num,fp);
        total_mileage = locbuf[num-1].total_mileage;
        fclose(fp);
        printf("--num=%d, time=%d, mileage=%d---\n",num,locbuf[num-1].itime,total_mileage);
    }

    bcd.clear();
    IntToBcd(total_mileage, bcd, 4);
    buffer.PushVector(bcd, 4);

    return 0;
}

int GB19056_2012::ProcessPulseFactorQuery(GB19056Buffer &buffer)
{
    vector<unsigned char> bcd;
    GetTime((long long)time(NULL), bcd);
    buffer.PushVector(bcd, 6);

    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPulseFactorQuery failed to refresh deploy_table.");
        delete deploy_table;
        return -1;
    }
    int pulse_factor;
    deploy_table->GetIntValue(VDR_DEPLOY_PARA_SPEED_PULSE_FACTOR, pulse_factor);
    delete deploy_table;

    buffer.PushWord((unsigned short)pulse_factor);

    return 0;
}

int GB19056_2012::ProcessLapPulseFactorQuery(GB19056Buffer &buffer)
{
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPulseFactorQuery failed to refresh deploy_table.");
        delete deploy_table;
        return -1;
    }
    int pulse_factor;
    deploy_table->GetIntValue(VDR_DEPLOY_PARA_LAP_PULSE_FACTOR, pulse_factor);
    delete deploy_table;

    buffer.PushByte((unsigned char)pulse_factor);

    return 0;
}

int GB19056_2012::ProcessCarInfoQuery(GB19056Buffer &buffer)
{
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessCarInfoQuery failed to refresh deploy_table.");
        delete deploy_table;
        return -1;
    }
    
    string car_vin;
    deploy_table->GetStringValue(VDR_DEPLOY_PARA_CAR_VIN, car_vin);
    string plate_no;
    deploy_table->GetStringValue(VDR_DEPLOY_PARA_PLATE_ID, plate_no);
    string plate_gb_no = boost::locale::conv::from_utf<char>(plate_no, GB19056_CHAR_SET);
    string plate_type;
    deploy_table->GetStringValue(VDR_DEPLOY_PARA_PLATE_SORT, plate_type);
    string plate_gb_type = boost::locale::conv::from_utf<char>(plate_type, GB19056_CHAR_SET);
    delete deploy_table;

    buffer.PushString(car_vin, 17);
    buffer.PushString(plate_gb_no, 12);
    buffer.PushString(plate_gb_type, 12);

    return 0;
}

int GB19056_2012::ProcessStatusQuery(GB19056Buffer &buffer)
{
    vector<unsigned char> bcd;
    GetTime((long long)time(NULL), bcd);
    buffer.PushVector(bcd, 6);
    buffer.PushByte(0x01);
    VDRDeployStatusConfigTable *config_table = new VDRDeployStatusConfigTable();
    if(config_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessStatusQuery failed to refresh config_table.");
        delete config_table;
        return -1;
    }
    for(int i = 0; i < 8; i ++)
    {
        string status_string;
        config_table->GetStringValue(i, status_string);
        string status_gb_string = boost::locale::conv::from_utf<char>(status_string, GB19056_CHAR_SET);
        buffer.PushString(status_gb_string, 10);
    }
    delete config_table;
    return 0;
}

int GB19056_2012::ProcessIdQuery(GB19056Buffer &buffer)
{
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessIdQuery failed to refresh deploy_table.");
        delete deploy_table;
        return -1;
    }
    vector<unsigned char> factory_id, model, model_id;
    deploy_table->GetSequenceValue(VDR_DEPLOY_PARA_MANUFACTORY_ID, factory_id);
    deploy_table->GetSequenceValue(VDR_DEPLOY_PARA_MODEL_NO, model);
    deploy_table->GetSequenceValue(VDR_DEPLOY_PARA_ID, model_id);
    delete deploy_table;

    buffer.PushVector(factory_id, 7);
    buffer.PushVector(model, 16);
    
    if(model_id.size() < 7)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessIdQuery invalid id.");
        return -1;
    }
    unsigned char buf[3];
    read_tuid_get_ymd(buf);
    
    int seq = 0;
    for(int i = 3; i < (int)model_id.size(); i ++)
    {
        unsigned ch = model_id[i];
        seq = seq * 10 + ch;
    }
        
    buffer.PushByte(VALUE_TO_BCD(buf[0]));
    buffer.PushByte(VALUE_TO_BCD(buf[1]));
    buffer.PushByte(VALUE_TO_BCD(buf[2]));
    buffer.PushLong(seq);

    PacketReserve(buffer);
    PacketReserve(buffer);
    PacketReserve(buffer);
    PacketReserve(buffer);
    PacketReserve(buffer);
    
    return 0;
}

int GB19056_2012::ProcessCarInfoSet(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 41)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessCarInfoSet length invalid %d.", buffer.GetLength());
        return -1;
    }

    string car_vin, plate_no, plate_type;
    buffer.GetString(car_vin, 17);
    buffer.GetString(plate_no, 12);
    buffer.GetString(plate_type, 12);
    string car_vin_utf = boost::locale::conv::to_utf<char>(car_vin, GB19056_CHAR_SET);
    string plate_no_utf = boost::locale::conv::to_utf<char>(plate_no, GB19056_CHAR_SET);
    string plate_type_utf = boost::locale::conv::to_utf<char>(plate_type, GB19056_CHAR_SET);
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessCarInfoSet failed to refresh deploy_table.");
        delete deploy_table;
        return 0;
    }
    deploy_table->SetStringValue(VDR_DEPLOY_PARA_CAR_VIN, car_vin_utf);
    deploy_table->SetStringValue(VDR_DEPLOY_PARA_PLATE_ID, plate_no_utf);
    deploy_table->SetStringValue(VDR_DEPLOY_PARA_PLATE_SORT, plate_type_utf);
    if(deploy_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessCarInfoSet failed to commit deploy_table.");
        delete deploy_table;
        return 0;
    }
    
    delete deploy_table;
    
    return 0;
}

int GB19056_2012::ProcessDeployTimeSet(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 6)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDeployTimeSet length invalid %d.", buffer.GetLength());
        return -1;
    }

    vector<unsigned char> bcd;
    buffer.GetVector(bcd, 6);
    int deploy_time = GetTime(bcd);

    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDeployTimeSet failed to refresh deploy_table.");
        delete deploy_table;
        return 0;
    }
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_DEPLOY_TIME, deploy_time);
    if(deploy_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDeployTimeSet failed to commit deploy_table.");
        delete deploy_table;
        return 0;
    }
    
    delete deploy_table;
    
    return 0;
}

int GB19056_2012::ProcessStatusConfigSet(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 80)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessStatusConfigSet length invalid %d.", buffer.GetLength());
        return -1;
    }

    VDRDeployStatusConfigTable *config_table = new VDRDeployStatusConfigTable();
    if(config_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessStatusQuery failed to refresh config_table.");
        delete config_table;
        return 0;
    }

    for(int i = 0; i < 8; i ++)
    {
        string value;
        buffer.GetString(value, 10);
        string value_utf = boost::locale::conv::to_utf<char>(value, GB19056_CHAR_SET);
        config_table->SetStringValue(i, value_utf);
    }
    
    if(config_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessStatusQuery failed to commit config_table.");
        delete config_table;
        return 0;
    }
    delete config_table;
    
    return 0;
}

int GB19056_2012::ProcessStatusConfig2Set(GB19056Buffer &buffer)
{
    unsigned char enable=0xFF, level= 0xFF;
    
    if(buffer.GetLength() < 8)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessStatusConfigSet length invalid %d.", buffer.GetLength());
        return -1;
    }

    VDRDeployStatusConfigTable *config_table = new VDRDeployStatusConfigTable();
    if(config_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessStatusQuery failed to refresh config_table.");
        delete config_table;
        return 0;
    }

    for(int i = 0; i < 8; i ++)
    {
        switch(buffer.GetByte())
        {
            case 0x00:
                level &= ~(0x01<<i); 
                break;
            case 0x01:
                break;
            case 0x02:
                enable &= ~(0x01<<i);
                break;
        }
    }
    
    config_table->SetIntValue(VDR_DEPLOY_STATUS_ENABLE, enable);
    config_table->SetIntValue(VDR_DEPLOY_STATUS_BITSET, level);
    
    if(config_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessStatusQuery failed to commit config_table.");
        delete config_table;
        return 0;
    }
    delete config_table;
    
    return 0;
}

int GB19056_2012::ProcessTimeSet(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 6)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTimeSet length invalid %d.", buffer.GetLength());
        return -1;
    }

    vector<unsigned char> bcd;
    buffer.GetVector(bcd, 6);
    time_t current_time = (time_t)GetTime(bcd);
    
    if(stime(&current_time) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTimeSet failed to set time.");
        return 0;
    }
    
    int ret = system("hwclock -w");
    if(WIFEXITED(ret))
    {
        int exit_status = WEXITSTATUS(ret);
        if(exit_status != 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTimeSet failed to run hwclock(%d).", exit_status);
            return 0;
        }
    }
    else
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTimeSet hwclock not exit normally.");
        return 0;
    }
    
    return 0;
}

int GB19056_2012::ProcessPulseFactorSet(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 8)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPulseFactorSet length invalid %d.", buffer.GetLength());
        return -1;
    }
    buffer.GetByte();
    buffer.GetByte();
    buffer.GetByte();
    buffer.GetByte();
    buffer.GetByte();
    buffer.GetByte();
    
    int pulse_factor = buffer.GetWord();
    
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPulseFactorSet failed to refresh deploy_table.");
        delete deploy_table;
        return 0;
    }
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_SPEED_PULSE_FACTOR, pulse_factor);
    if(deploy_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPulseFactorSet failed to commit deploy_table.");
        delete deploy_table;
        return 0;
    }
    delete deploy_table;
    
    return 0;
}

int GB19056_2012::ProcessLapPulseFactorSet(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPulseFactorSet length invalid %d.", buffer.GetLength());
        return -1;
    }

    int pulse_factor = buffer.GetByte();
    
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPulseFactorSet failed to refresh deploy_table.");
        delete deploy_table;
        return 0;
    }
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_LAP_PULSE_FACTOR, pulse_factor);
    if(deploy_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPulseFactorSet failed to commit deploy_table.");
        delete deploy_table;
        return 0;
    }
    delete deploy_table;
    
    return 0;
}

int GB19056_2012::ProcessDeployMileageSet(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 4)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDeployMileageSet length invalid %d.", buffer.GetLength());
        return -1;
    }
    
    vector<unsigned char> bcd;
    
    buffer.GetVector(bcd, 4);
    
    int deploy_mileage = BcdToInt(bcd);
    
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDeployMileageSet failed to refresh deploy_table.");
        delete deploy_table;
        return 0;
    }
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_DEPLOY_MILEAGE, deploy_mileage);
    if(deploy_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDeployMileageSet failed to commit deploy_table.");
        delete deploy_table;
        return 0;
    }
    delete deploy_table;
    
    return 0;
}

int GB19056_2012::WriteEventLog(GB19056Buffer &buffer, int itime)
{
    int ret=0, index=0;
    
    VDRParamInfoTable *para_table = new VDRParamInfoTable();
    ret = para_table->RefreshBottom();
    if(ret == 0)
    {
        para_table->GetIntValue(VDR_PARAM_INFO_INDEX, index);
        index++;
    }
    
    para_table->SetIntValue(VDR_PARAM_INFO_INDEX, index);
    para_table->SetIntValue(VDR_PARAM_OCCUR_TIME, itime);
    para_table->SetIntValue(VDR_PARAM_EVENT_ID, (int)buffer.GetCmd());
    if(para_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::WriteEventLog failed to commit para_table.");
        delete para_table;
        return -1;
    }
    delete para_table;
    return 0;
}

int GB19056_2012::ProcessRecordQueryRequest(GB19056Buffer &buffer, int &start_time, int &stop_time, int &max_block_number)
{
    if(buffer.GetLength() < 14)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessRecordQueryRequest length invalid %d.", buffer.GetLength());
        return -1;
    }
    
    vector<unsigned char> bcd;
    buffer.GetVector(bcd, 6);
    start_time = GetTime(bcd);
    
    bcd.clear();
    buffer.GetVector(bcd, 6);
    stop_time = GetTime(bcd);
    
    max_block_number = buffer.GetWord();
    
    return 0;
}
int GB19056_2012::ProcessFileDriveRecordQuery(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;

    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriveRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriveRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }

    
    int stop, start;
    int stop_sec=0, start_sec=0;
    time_t ttime ;
    struct tm p;

    stop = MINUTE_ALINE(stop_time);
    start = MINUTE_ALINE(start_time);
       
    ttime = stop_time;
    localtime_r(&ttime, &p);
    stop_sec = p.tm_sec;

    ttime = start_time;
    localtime_r(&ttime, &p);
    start_sec = p.tm_sec;

    FILE *fp = NULL;
    int total_blocks = 0, current_blocks=0;
    unsigned char tmp_vector[126];
    int offset = 0, current_min=0, i=0, j=0; 
    struct tm lt;

    fp = fopen("/nand2/tmp08.vdr", "r");
    
	if(fp == NULL) 
	{
		printf("Can't open tmp08.vdr %s\n", strerror(errno));
		return -1;
	}

    fread(&total_blocks, 4,1, fp);
    fread(&current_blocks, 4,1, fp);

    if(max_block_number > 7)
        max_block_number = 7;
        
    for(i=0; i<total_blocks;  i++)
    {
        memset(tmp_vector,0xFF, 126);

        offset = 10 + (current_blocks-1)*126;
        fseek(fp, offset, SEEK_SET);
        fread(tmp_vector, 126, 1, fp);

        lt.tm_year = BCD_TO_VALUE(tmp_vector[0]) + 100;
        lt.tm_mon = BCD_TO_VALUE(tmp_vector[1]) - 1;
        lt.tm_mday = BCD_TO_VALUE(tmp_vector[2]);
        lt.tm_hour = BCD_TO_VALUE(tmp_vector[3]);
        lt.tm_min = BCD_TO_VALUE(tmp_vector[4]);
        lt.tm_sec = BCD_TO_VALUE(tmp_vector[5]);
        current_min = (unsigned int)mktime(&lt);
        //printf("current_min=%d,currentblocks=%d\n",current_min, current_blocks);
        if(current_min <= stop)
        {
            if(current_min < start)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery invaild data, start=%d, stop=%d,cur=%d\n.", 
                   start, stop, current_min);
                return 0;
            }
            
            if(current_min == stop)
            {
                if(start < stop)
                    start_sec = 0;
            }
            else
            {
                stop_sec = 59;
                if(current_min > start)
                    start_sec = 0;
                else
                    start_sec =  p.tm_sec;
            }
            //printf("start=%d,starts=%d,stop=%d,stops=%d,cur=%d..\n",start,start_sec,stop,stop_sec,current_min);
            for(j=0; j<start_sec; j++)
                memset(tmp_vector+6+j*2, 0xFF, 2);
            
            for(j=stop_sec+1; j<60; j++)
                memset(tmp_vector+6+j*2, 0xFF, 2);
            
            for(j = 0; j < 126; j ++)
            {
                buffer.PushByte(tmp_vector[j]);
            }  
            max_block_number--;
            if(max_block_number == 0)
                break;
        }

        current_blocks--;
        if(current_blocks == 0)
            current_blocks = total_blocks;
    }
    fclose(fp);

    if(current_min > stop)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery invaild data, start=%d, stop=%d,cur=%d\n.", 
           start, stop, current_min);
        return 0;
    }
    
    return 0;
}

int GB19056_2012::ProcessFileLocRecordQuery(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    int stop , start;
    int start_min = 0, stop_min = 0; 
    time_t ttime ;
    struct tm p;

    ttime = start_time;
    localtime_r(&ttime, &p);
    start_min = p.tm_min;
    
    start = HOUR_ALINE(start_time);
    stop = HOUR_ALINE(stop_time);
    
    ttime = stop_time;
    localtime_r(&ttime, &p);
    stop_min = p.tm_min;

    printf("starth=%d,startm=%d,stoph=%d,stopm=%d...\n",start,start_min,stop,stop_min);
    
    FILE *fp = NULL;
    int total_blocks = 0, current_blocks=0;
    unsigned char tmp_vector[666];
    int offset = 0, current_hour=0, i=0; 
    struct tm lt;

    fp = fopen("/nand2/tmp09.vdr", "r");
    
	if(fp == NULL) 
	{
		printf("Can't open tmp09.vdr %s\n", strerror(errno));
		return -1;
	}

    fread(&total_blocks, 4,1, fp);
    fread(&current_blocks, 4,1, fp);
    
    for(i=0; i<total_blocks;  i++)
    {
        memset(tmp_vector,0xFF, 666);

        offset = 10 + (current_blocks-1)*666;
        fseek(fp, offset, SEEK_SET);
        fread(tmp_vector, 666, 1, fp);

        lt.tm_year = BCD_TO_VALUE(tmp_vector[0]) + 100;
        lt.tm_mon = BCD_TO_VALUE(tmp_vector[1]) - 1;
        lt.tm_mday = BCD_TO_VALUE(tmp_vector[2]);
        lt.tm_hour = BCD_TO_VALUE(tmp_vector[3]);
        lt.tm_min = BCD_TO_VALUE(tmp_vector[4]);
        lt.tm_sec = BCD_TO_VALUE(tmp_vector[5]);
        current_hour = (unsigned int)mktime(&lt);
        //printf("currenthoure=%d,currentblocks=%d\n",current_hour, current_blocks);
        if(current_hour <= stop)
            break;

        current_blocks--;
        if(current_blocks == 0)
            current_blocks = total_blocks;
    }
    fclose(fp);

    if((current_hour < start) || current_hour > stop)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery invaild data, start=%d, stop=%d,cur=%d\n.", 
           start, stop, current_hour);
        return 0;
    }
    
    if(current_hour == stop)
    {
        if(start < stop)
            start_min = 0;
    }
    else
    {
        stop_min = 59;
        if(current_hour > start)
            start_min = 0;        
    }
    printf("starth=%d,startm=%d,stoph=%d,stopm=%d,curhoure=%d..\n",start,start_min,stop,stop_min,current_hour);

    for(i=0; i<start_min; i++)
        memset(tmp_vector+6+i*11, 0xFF, 11);

    for(i=stop_min+1; i<60; i++)
        memset(tmp_vector+6+i*11, 0xFF, 11);

    for(int j = 0; j < 666; j ++)
    {
        buffer.PushByte(tmp_vector[j]);
    }    
    
    return 0;
}

int GB19056_2012::ProcessDriveRecordQuery(GB19056Buffer &buffer)
{
	static int start_time;
    static int stop_time;
    static int max_block_number;
	static int need_refresh = 1;
	static int top_index = -1;
	static int current_index = -1;
	static int stop = -1;

	static int occur_time = -1;

	int start_time_tmp;
	int stop_time_tmp;
	int max_block_number_tmp;
	if(ProcessRecordQueryRequest(buffer, start_time_tmp, stop_time_tmp, max_block_number_tmp) < 0)
	{
    	LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriveRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
    	return -1;
	}

	if(start_time_tmp != start_time)
		need_refresh = 1;
	
	int speed, status;

	if(need_refresh)
	{
		start_time = start_time_tmp;
		stop_time = stop_time_tmp;
		max_block_number = max_block_number_tmp;

		int block_number = stop_time - start_time;
		if(0 > block_number)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriveRecordQuery time invalid(%d, %d).", start_time, stop_time);
			return -1;
		}

		if(block_number > max_block_number)
			need_refresh = 0;
	
		loc_table = new VDRLocStatus1Table();

	    top_index = loc_table->RefreshNearest(VDR_LOCS_OCCUR_TIME, start_time, false);
	    if(top_index < 0)
	    {
	        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriveRecordQuery RefreshNearest start time failed(%d).", start_time);
	        delete loc_table;
	        return 0;
	    }

	    current_index = loc_table->RefreshNearest(VDR_LOCS_OCCUR_TIME, stop_time, true);
	    if(current_index < 0)
	    {
	        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriveRecordQuery RefreshNearest stop time failed(%d).", stop_time);
	        delete loc_table;
	        return 0;
	    }

		loc_table->GetIntValue(VDR_LOCS_VDR_SPEED, speed);
	    loc_table->GetIntValue(VDR_LOCS_VDR_STATUS, status);
	    loc_table->GetIntValue(VDR_LOCS_OCCUR_TIME, occur_time);
	    if(occur_time > stop_time)
	    {
	        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriveRecordQuery query invalid(%d, %d).", occur_time, stop_time);
	        delete loc_table;
	        return -1;
	    }

		stop = occur_time;
	}
	
    static int multi_record_index = -1;
    int total_blocks = 0;
    int speed_flag = 0;
    int index = 0;

	int time_index = -1;

	int start = (stop / 60 * 60);
	int startcnt = start - 1;

    for(; total_blocks < max_block_number;  stop = startcnt, startcnt -= 60, start = startcnt+1)
    {
        unsigned char tmp_vector[120];

        if(stop < start_time)
        {
			goto refresh;
		}
		
        if(occur_time < start)
		{
            continue;
        }
 
        memset(tmp_vector, 0xFF, 120);
		
        
        bool is_empty = true;
        speed_flag = 0;
        for(time_index = stop; time_index >= start; time_index --)
        {
            while(time_index < occur_time)
            {
                if((multi_record_index == -1) || (multi_record_index >= loc_table->GetResultsCount()))
                {
                    current_index = loc_table->RefreshPrevious(current_index, top_index);
					if(current_index < 0)
                    {
                        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriveRecordQuery RefreshPrevious reach end(%d, %d).", current_index, top_index);
                        goto refresh;
                    }
                    multi_record_index = 0;
                }
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_VDR_SPEED, speed);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_VDR_STATUS, status);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_OCCUR_TIME, occur_time);
                multi_record_index ++;
                index = time_index-occur_time;
            }
            if(speed_flag == 0 && speed != 0)
            {
                    speed_flag = 1;
            }
			
            int v_index = (time_index - start) * 2;
            if(((time_index == occur_time) ||((index > 0) && (index < 6))) && (time_index >= start_time))
            {
				tmp_vector[v_index] = (unsigned char)(speed/10);
                tmp_vector[v_index + 1] = (unsigned char)status;
                is_empty= false;
            }
        }

        if(!is_empty && speed_flag)
        {
            vector<unsigned char> bcd;
            GetTime(start, bcd);
            buffer.PushVector(bcd, 6);

            for(int j = 0; j < 120; j ++)
            {
                buffer.PushByte(tmp_vector[j]);
            }
            
            total_blocks ++;
        }

        //if((total_blocks + 1) * 126 > GB19056_2012_MAX_BLOCK_LEN)
        if(total_blocks * 126 > GB19056_2012_MAX_BLOCK_LEN)
        {
            break;
        }
    }

	return 0;

refresh:	
	multi_record_index = -1;
	need_refresh = 1;
	delete loc_table;

	return 0;
}

int GB19056_2012::ProcessLocRecordQuery(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDRLocStatus2Table *loc_table = new VDRLocStatus2Table();
    int top_index = loc_table->RefreshNearest(VDR_LOCS_OCCUR_TIME, start_time, false);
    if(top_index < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery RefreshNearest start time failed(%d).", start_time);
        delete loc_table;
        return 0;
    }
    int current_index = loc_table->RefreshNearest(VDR_LOCS_OCCUR_TIME, stop_time, true);
    if(current_index < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery RefreshNearest stop time failed(%d).", stop_time);
        delete loc_table;
        return 0;
    }
    int stop = stop_time, start;
    int longitude, latitude, altitude, status, speed;
    int tmplongitude, tmplatitude;
    int occur_time;
    loc_table->GetIntValue(VDR_LOCS_LONGITUDE, longitude);
    loc_table->GetIntValue(VDR_LOCS_LATITUDE, latitude);
    loc_table->GetIntValue(VDR_LOCS_ALTITUDE, altitude);
    loc_table->GetIntValue(VDR_LOCS_STATUS_FLAG, status);
    loc_table->GetIntValue(VDR_LOCS_OCCUR_TIME, occur_time);
    loc_table->GetIntValue(VDR_LOCS_VDR_SPEED, speed);
    occur_time = MINUTE_ALINE(occur_time);
    if(occur_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery query invalid(%d, %d).", occur_time, stop_time);
        delete loc_table;
        return -1;
    }
    
    int multi_record_index = -1;
    int total_blocks = 0;

    for(; total_blocks < max_block_number;  stop = start - 60)
    {
        unsigned char tmp_vector[660];

        stop = MINUTE_ALINE(stop);
        
        if(stop < start_time)
        {
            break;
        }
        start = HOUR_ALINE(stop);
        
        if(occur_time < start)
        {
            continue;
        }
        
        memset(tmp_vector, 0xFF, 660);
        
        bool is_empty = true;
        int time_index;
        for(time_index = stop; time_index >= start; time_index -= 60)
        {
            while(time_index < occur_time)
            {
                if((multi_record_index == -1) || (multi_record_index >= loc_table->GetResultsCount()))
                {
                    current_index = loc_table->RefreshPrevious(current_index, top_index);
                    if(current_index < 0)
                    {
                        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery RefreshPrevious reach end(%d, %d).", current_index, top_index);
                        break;
                    }
                    multi_record_index = 0;
                }
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_LONGITUDE, longitude);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_LATITUDE, latitude);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_ALTITUDE, altitude);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_STATUS_FLAG, status);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_OCCUR_TIME, occur_time);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_VDR_SPEED, speed);
                occur_time = MINUTE_ALINE(occur_time);
                multi_record_index ++;
            }
            
            if(time_index < occur_time)
            {
                break;
            }

            int v_index = (time_index - start)/60 * 11;
            if((time_index == occur_time) && (time_index >= start_time))
            {
                if(status & 0x02)
                {
                    tmplongitude = longitude*6/10;
                    if(status & 0x08)
                    {
                        tmplongitude = -tmplongitude;
                    }
                    tmplatitude = latitude*6/10;
                    if(status & 0x04)
                    {
                        tmplatitude = -tmplatitude;
                    }
                    
                    tmp_vector[v_index] = (tmplongitude >> 24) & 0xFF;
                    tmp_vector[v_index + 1] = (tmplongitude >> 16) & 0xFF;
                    tmp_vector[v_index + 2] = (tmplongitude >> 8) & 0xFF;
                    tmp_vector[v_index + 3] = tmplongitude & 0xFF;
                    tmp_vector[v_index + 4] = (tmplatitude >> 24) & 0xFF;
                    tmp_vector[v_index + 5] = (tmplatitude >> 16) & 0xFF;
                    tmp_vector[v_index + 6] = (tmplatitude >> 8) & 0xFF;
                    tmp_vector[v_index + 7] = tmplatitude & 0xFF;
                    tmp_vector[v_index + 8] = (altitude >> 8) & 0xFF;
                    tmp_vector[v_index + 9] = altitude & 0xFF;   
                    tmp_vector[v_index + 10] = (unsigned char)(speed/10);
                    is_empty= false;                    
                }
                else if(speed != 0)
                {
                    tmp_vector[v_index] = 0x7F;
                    tmp_vector[v_index + 4] = 0x7F;
                    tmp_vector[v_index + 8] = 0x7F;
                    tmp_vector[v_index + 10] = (unsigned char)(speed/10);
                    is_empty= false;                  
                }
                else
                {
                    tmp_vector[v_index] = 0x7F;
                    tmp_vector[v_index + 4] = 0x7F;
                    tmp_vector[v_index + 8] = 0x7F;
                    tmp_vector[v_index + 10] = (unsigned char)(speed/10);
                }
            }
        }

        if(!is_empty)
        {
            vector<unsigned char> bcd;
            GetTime(start, bcd);
            buffer.PushVector(bcd, 6);

            for(int j = 0; j < 660; j ++)
            {
                buffer.PushByte(tmp_vector[j]);
            }
            
            total_blocks ++;
        }
        
        if(time_index >= start)
        {
            break;
        }

        if((total_blocks + 1) * 666 > GB19056_2012_MAX_BLOCK_LEN)
        {
            break;
        }
    }
    
    delete loc_table;
    
    return 0;
}

int GB19056_2012::ProcessAccidentRecordQuery(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDRAccidentTable *acc_table = new VDRAccidentTable();
    int ret = acc_table->RefreshBetween(VDR_ACCIDENT_OCCUR_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete acc_table;
        return 0;
    }
    
    if(acc_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery RefreshBetween no results.");
        delete acc_table;
        return 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;

    for(int i = 0; i < max_block_number; i ++)
    {
        int occur_time;
        vector<unsigned char> speed;
        vector<unsigned char> status;
        string license;
        int longitude, latitude, altitude;
        if(multi_record_index >= acc_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery reach end.");
            break;
        }
        acc_table->GetSequenceValue(multi_record_index, VDR_ACCIDENT_SPEED, speed);
        acc_table->GetSequenceValue(multi_record_index, VDR_ACCIDENT_STATUS, status);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_OCCUR_TIME, occur_time);
        acc_table->GetStringValue(multi_record_index, VDR_ACCIDENT_DRIVER_LICENSE_NUM, license);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_LONGITUDE, longitude);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_LATITUDE, latitude);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_ALTITUDE, altitude);
        multi_record_index ++;
        
        if(speed.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery invalid speed.");
            continue;
        }

        if(status.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery invalid status.");
            continue;
        }

        vector<unsigned char> bcd;
        GetTime(occur_time, bcd);
        buffer.PushVector(bcd, 6);
        buffer.PushString(license, 18);

        for(int time_index = 0; time_index < 100; time_index ++)
        {
            if((int)speed.size() > time_index)
            {
               buffer.PushByte(speed[time_index]);
            }
            else
            {
               buffer.PushByte(0xFF);
            }
            if((int)status.size() > time_index)
            {
               buffer.PushByte(status[time_index]);
            }
            else
            {
               buffer.PushByte(0xFF);
            }
        }
        buffer.PushLong(longitude);
        buffer.PushLong(latitude);
        buffer.PushWord(altitude);
        total_blocks ++;
        
        if((total_blocks + 1) * 234 > GB19056_2012_MAX_BLOCK_LEN)
        {
            break;
        }
    }
    
    delete acc_table;
    
    return 0;
}

int GB19056_2012::ProcessTiredRecordQuery(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTiredRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTiredRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDRTiredTable *tired_table = new VDRTiredTable();
    int ret = tired_table->RefreshBetween(VDR_TIRED_BEGIN_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTiredRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete tired_table;
        return 0;
    }
    
    if(tired_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTiredRecordQuery RefreshBetween no results.");
        delete tired_table;
        return 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;

    for(int i = 0; i < max_block_number; i ++)
    {
        int start_drive_time, stop_drive_time;
        int start_longitude, start_latitude, start_altitude;
        int stop_longitude, stop_latitude, stop_altitude;
        string license;

        if(multi_record_index >= tired_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTiredRecordQuery reach end.");
            break;
        }
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_TIME, start_drive_time);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_LONGITUDE, start_longitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_LATITUDE, start_latitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_ALTITUDE, start_altitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_TIME, stop_drive_time);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_LONGITUDE, stop_longitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_LATITUDE, stop_latitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_ALTITUDE, stop_altitude);
        tired_table->GetStringValue(multi_record_index, VDR_TIRED_DRIVER_LICENSE_NUM, license);
        multi_record_index ++;

        buffer.PushString(license, 18);
        vector<unsigned char> bcd;
        GetTime(start_drive_time, bcd);
        buffer.PushVector(bcd, 6);
        bcd.clear();
        GetTime(stop_drive_time, bcd);
        buffer.PushVector(bcd, 6);

        buffer.PushLong(start_longitude);
        buffer.PushLong(start_latitude);
        buffer.PushWord(start_altitude);
        buffer.PushLong(stop_longitude);
        buffer.PushLong(stop_latitude);
        buffer.PushWord(stop_altitude);
        total_blocks ++;
        
        if((total_blocks + 1) * 50 > GB19056_2012_MAX_BLOCK_LEN)
        {
            break;
        }
    }
    
    delete tired_table;
    
    return 0;
}

int GB19056_2012::ProcessDriverInfoRecordQuery(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverInfoRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverInfoRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    DriversLoginTable *login_table = new DriversLoginTable();
    int ret = login_table->RefreshBetween(VDR_DRV_OCCUR_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverInfoRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete login_table;
        return 0;
    }
    
    if(login_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverInfoRecordQuery RefreshBetween no results.");
        delete login_table;
        return 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;

    for(int i = 0; i < max_block_number; i ++)
    {
        int occur_time, event;
        string license;

        if(multi_record_index >= login_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverInfoRecordQuery reach end.");
            break;
        }
        login_table->GetIntValue(multi_record_index, VDR_DRV_OCCUR_TIME, occur_time);
        login_table->GetIntValue(multi_record_index, VDR_DRV_LOGIN_LOGOUT, event);
        login_table->GetStringValue(multi_record_index, VDR_DRV_DRIVER_LICENSE_NUM, license);
        multi_record_index ++;

        vector<unsigned char> bcd;
        GetTime(occur_time, bcd);
        buffer.PushVector(bcd, 6);
        buffer.PushString(license, 18);

        if(event)
        {
            buffer.PushByte(0x01);
        }
        else
        {
            buffer.PushByte(0x02);
        }

        total_blocks ++;
        
        if((total_blocks + 1) * 25 > GB19056_2012_MAX_BLOCK_LEN)
        {
            break;
        }
    }
    
    delete login_table;
    
    return 0;
}

int GB19056_2012::ProcessPowerRecordQuery(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    int max_flag = 0;
    int on_time, off_time;
    int tmp_off_time = 0;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPowerRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPowerRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDROFFStatusTable *off_table = new VDROFFStatusTable();
    int ret = off_table->RefreshBetween(VDR_OFFS_ACC_ON_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPowerRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete off_table;
        return 0;
    }
    
    if(off_table->GetResultsCount() <= 0)
    {
        off_time = 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;
    vector<unsigned char> bcd;

    for(int i = 0; i < max_block_number; i ++)
    {
        if(multi_record_index >= off_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPowerRecordQuery reach end.");
            break;
        }
        off_table->GetIntValue(multi_record_index, VDR_OFFS_ACC_ON_TIME, on_time);
        off_table->GetIntValue(multi_record_index, VDR_OFFS_ACC_OFF_TIME, off_time);
        multi_record_index ++;
        bcd.clear();
        
        if(off_time == 0)
        {
            GetTime(on_time, bcd);
            buffer.PushVector(bcd, 6);
            buffer.PushByte(0x01);
            total_blocks ++;
        }
        else
        {
            if(off_time < stop_time)
            {
                GetTime(off_time, bcd);
                buffer.PushVector(bcd, 6);
                buffer.PushByte(0x02); 
                total_blocks ++;
            }

            bcd.clear();
            GetTime(on_time, bcd);
            buffer.PushVector(bcd, 6);
            buffer.PushByte(0x01);
            total_blocks ++;
        }

        if((total_blocks + 1) * 7 > GB19056_2012_MAX_BLOCK_LEN)
        {
            max_flag = 1;
            break;
        }
    }

    if(!max_flag)
    {
        if(off_time == 0)
            tmp_off_time = stop_time;
        else
            tmp_off_time = off_time-1;

        int ret = off_table->RefreshBetween(VDR_OFFS_ACC_OFF_TIME, start_time, tmp_off_time, false);
        if(ret < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPowerRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
            delete off_table;
            return 0;
        }
        
        if(off_table->GetResultsCount() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPowerRecordQuery RefreshBetween no results.");
            delete off_table;
            return 0;
        }
        off_table->GetIntValue(0, VDR_OFFS_ACC_ON_TIME, on_time);
        off_table->GetIntValue(0, VDR_OFFS_ACC_OFF_TIME, off_time);
        bcd.clear();
        GetTime(off_time, bcd);
        buffer.PushVector(bcd, 6);
        buffer.PushByte(0x02); 
    }
    
    delete off_table;
    
    return 0;
}

int GB19056_2012::ProcessParaSettingRecordQuery(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessParaSettingRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessParaSettingRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDRParamInfoTable *para_table = new VDRParamInfoTable();
    int ret = para_table->RefreshBetween(VDR_PARAM_OCCUR_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessParaSettingRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete para_table;
        return 0;
    }
    
    if(para_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessParaSettingRecordQuery RefreshBetween no results.");
        delete para_table;
        return 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;

    for(int i = 0; i < max_block_number; i ++)
    {
        int occur_time;
        int event;

        if(multi_record_index >= para_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessParaSettingRecordQuery reach end.");
            break;
        }
        para_table->GetIntValue(multi_record_index, VDR_PARAM_OCCUR_TIME, occur_time);
        para_table->GetIntValue(multi_record_index, VDR_PARAM_EVENT_ID, event);
        multi_record_index ++;

        vector<unsigned char> bcd;
        GetTime(occur_time, bcd);
        buffer.PushVector(bcd, 6);
        buffer.PushByte((unsigned char)event);

        total_blocks ++;
        
        if((total_blocks + 1) * 7 > GB19056_2012_MAX_BLOCK_LEN)
        {
            break;
        }
    }
    
    delete para_table;
    
    return 0;
}

int GB19056_2012::ProcessSpeedInfoRecordQuery(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDRSpeedInfoTable *speed_table = new VDRSpeedInfoTable();
    int ret = speed_table->RefreshBetween(VDR_SPEED_BEGIN_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete speed_table;
        return 0;
    }
    
    if(speed_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery RefreshBetween no results.");
        delete speed_table;
        return 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;

    for(int i = 0; i < max_block_number; i ++)
    {
        int start_time, stop_time;
        vector<unsigned char> speed;
        vector<unsigned char> gps_speed;
        int status;
        
        if(multi_record_index >= speed_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery reach end.");
            break;
        }
        speed_table->GetSequenceValue(multi_record_index, VDR_SPEED_VDR_SPEED, speed);
        speed_table->GetSequenceValue(multi_record_index, VDR_SPEED_GPS_SPEED, gps_speed);
        speed_table->GetIntValue(multi_record_index, VDR_SPEED_BEGIN_TIME, start_time);
        speed_table->GetIntValue(multi_record_index, VDR_SPEED_END_TIME, stop_time);
        speed_table->GetIntValue(multi_record_index, VDR_SPEED_STATUS, status);
        multi_record_index ++;
        
        if(speed.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery invalid speed.");
            continue;
        }

        if(gps_speed.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery invalid gps_speed.");
            continue;
        }

        buffer.PushByte(status);
        vector<unsigned char> bcd;
        GetTime(start_time, bcd);
        buffer.PushVector(bcd, 6);
        bcd.clear();
        GetTime(stop_time, bcd);
        buffer.PushVector(bcd, 6);

        for(int time_index = 0; time_index < 60; time_index ++)
        {
            if((int)speed.size() > time_index)
            {
               buffer.PushByte(speed[time_index]);
            }
            else
            {
               buffer.PushByte(0xFF);
            }
            if((int)gps_speed.size() > time_index)
            {
               buffer.PushByte(gps_speed[time_index]);
            }
            else
            {
               buffer.PushByte(0xFF);
            }
        }
        total_blocks ++;
        
        if((total_blocks + 1) * 133 > GB19056_2012_MAX_BLOCK_LEN)
        {
            break;
        }
    }
    
    delete speed_table;
    
    return 0;
}

int GB19056_2012::ProcessInCheck(GB19056Buffer &)
{   
    DB9_check_flag = 1;
    return 0;
}

int GB19056_2012::ProcessMileageCheck(GB19056Buffer &buffer)
{
    if(DB9_check_flag == 0)
        return -1;
    
    int pulse_factor, deploy_mileage;
    int speed = 0, status = 0;
    int total_mileage = 0;   
    
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    deploy_table->GetIntValue(VDR_DEPLOY_PARA_SPEED_PULSE_FACTOR, pulse_factor);
    deploy_table->GetIntValue(VDR_DEPLOY_PARA_DEPLOY_MILEAGE, deploy_mileage);
    delete deploy_table;

    #if 0
    VDRLocStatus1Table *loc_table = new VDRLocStatus1Table();
    if(loc_table->RefreshBottom() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessMileageCheck RefreshBottom failed.");
        delete loc_table;
        return 0;
    }
    
    loc_table->GetIntValue(VDR_LOCS_VDR_SPEED, speed);
    loc_table->GetIntValue(VDR_LOCS_VDR_STATUS, status);
    loc_table->GetIntValue(VDR_LOCS_TOTAL_MILEAGE, total_mileage);
    delete loc_table;
    
    total_mileage += deploy_mileage;
    #endif
    
    FILE *fp = NULL;
    int num = 5;
    struct LocStatusData locbuf[5];
    
    memset(locbuf,0,sizeof(struct LocStatusData)*5);

    fp = fopen("/nand2/loc0809.vdr", "r");
    
	if(fp == NULL) 
	{
		printf("Can't open loc0809.vdr %s\n", strerror(errno));
	}
    else
    {
        fread(locbuf,sizeof(struct LocStatusData),num,fp);
        total_mileage = locbuf[num-1].total_mileage;
        speed = locbuf[num-1].vdr_speed;
        status = locbuf[num-1].vdr_status_old_hb;
        fclose(fp);
    }
    
    ProcessIdQuery(buffer);
    buffer.PushWord((unsigned short)pulse_factor);
    buffer.PushWord((unsigned short)speed);
    buffer.PushLong((unsigned int)total_mileage*100);
    buffer.PushByte((unsigned char)status);

    return 0;
}

int GB19056_2012::ProcessPulseFactorCheck(GB19056Buffer &)
{    
    if(DB9_check_flag == 0)
        return -1;

    return 0;
}

int GB19056_2012::ProcessTimeCheck(GB19056Buffer &)
{  
    if(DB9_check_flag == 0)
        return -1;

    return 0;
}

int GB19056_2012::ProcessOutCheck(GB19056Buffer &)
{
    if(DB9_check_flag == 0)
        return -1;
    
    DB9_check_flag = 0;
    return 0;
}

int GB19056_2012::DumpData(string path, GB19056CallBackInterface *notify)
{
    string filename = path;
    if((filename.length() <= 0) || (*(filename.end()) != '/' ))
    {
        filename.push_back('/');
    }

    vector<unsigned char> currenttime;
    GetTime((long long)time(NULL), currenttime);
    filename.push_back('D');
    filename += GetTime((long long)time(NULL), '_', false);
    filename.push_back('_');

    string plate_no;
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpData failed to refresh deploy_table.");
        plate_no = "未知车牌";
    }
    else
    {
        deploy_table->GetStringValue(VDR_DEPLOY_PARA_PLATE_ID, plate_no);
    }
    delete deploy_table;
    
    //string plate_gb_no = boost::locale::conv::from_utf<char>(plate_no, GB19056_CHAR_SET);
    filename += plate_no;
    filename += ".VDR";

    ifstream infile;
    infile.open(filename.c_str(), ios::in);
    if(infile)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpData file %s already exists.", filename.c_str());
        if(notify)
        {
            int ret = notify->FileExistNotify();
            if(ret)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpData call back return non-zero.");
                return -1;
            }
        }
    }
    infile.close();
    
    string temp_filename = filename + ".TEMP";
    
    ofstream outfile;
    outfile.open(temp_filename.c_str(), ios::out | ios::binary | ios::trunc);
    if(!outfile)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpData fail to open %s to write.", temp_filename.c_str());
        return -1;
    }
    
    vector<unsigned char> inbuffer, outbuffer;
    GB19056Buffer buffer(inbuffer, outbuffer);
    
    int total_cmd = 16;
    int cmd_count = 0;
    unsigned short actual_cmd_number = 0;
    unsigned char check_sum = 0;
    
    for(unsigned char i = (unsigned char)ENUM_GB19056_CMD_QUERY_VER; i <= (unsigned char)ENUM_GB19056_CMD_QUERY_ID; i ++)
    {
        buffer.Clear();
        buffer.SetCmd(i);
        
        if(ProcessCmd(buffer) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpData fail to execute cmd %d.", i);
            return -1;
        }
        
        DumpByte(outfile, i, check_sum);
        string cmd_name = GetDataName(i);
        DumpString(outfile, cmd_name, 18, check_sum);
        unsigned int length = outbuffer.size();
        DumpLong(outfile, length, check_sum);

        DumpVector(outfile, outbuffer, check_sum);

        cmd_count ++;
        actual_cmd_number ++;
        if(notify)
        {
            notify->ProgressNotify(cmd_count , total_cmd);
        }
    }
#if 0
    unsigned char cmd = (unsigned char)ENUM_GB19056_CMD_QUERY_LAP_PULSE_FACTOR;
    buffer.Clear();
    buffer.SetCmd(cmd);
    
    if(ProcessCmd(buffer) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpData fail to execute cmd %d.", (int)cmd);
        return -1;
    }
    
    DumpByte(outfile, cmd, check_sum);
    string cmd_name = GetDataName(cmd);
    DumpString(outfile, cmd_name, 18, check_sum);
    unsigned int length = outbuffer.size();
    DumpLong(outfile, length, check_sum);
    DumpVector(outfile, outbuffer, check_sum);

    cmd_count ++;
    actual_cmd_number ++;
    if(notify)
    {
        notify->ProgressNotify(cmd_count , total_cmd);
    }
#endif
    if(DumpFileSpeedRecord(filename, outfile, check_sum))
    {
        DumpZero(outfile,(unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_RECORD, check_sum);
    }
    actual_cmd_number ++;
    cmd_count ++;
    if(notify)
    {
        notify->ProgressNotify(cmd_count , total_cmd);
    }

    if(DumpFileLocRecord(filename, outfile, check_sum))
    {
       DumpZero(outfile,(unsigned char)ENUM_GB19056_CMD_QUERY_LOC_RECORD, check_sum); 
    }
    actual_cmd_number ++;
    cmd_count ++;
    if(notify)
    {
        notify->ProgressNotify(cmd_count , total_cmd);
    }

    if(DumpAccidentRecord(filename, outfile, check_sum))
    {
        DumpZero(outfile,(unsigned char)ENUM_GB19056_CMD_QUERY_ACCIDENT_RECORD, check_sum);
    }
    actual_cmd_number ++;
    cmd_count ++;
    if(notify)
    {
        notify->ProgressNotify(cmd_count , total_cmd);
    }

    if(DumpTiredRecord(filename, outfile, check_sum))
    {
        DumpZero(outfile,(unsigned char)ENUM_GB19056_CMD_QUERY_OVERTIME_RECORD, check_sum);
    }
    actual_cmd_number ++;
    cmd_count ++;
    if(notify)
    {
        notify->ProgressNotify(cmd_count , total_cmd);
    }
    
    if(DumpDriverRecord(filename, outfile, check_sum))
    {
        DumpZero(outfile,(unsigned char)ENUM_GB19056_CMD_QUERY_DRIVER_RECORD, check_sum);
    }
    actual_cmd_number ++;
    cmd_count ++;
    if(notify)
    {
        notify->ProgressNotify(cmd_count , total_cmd);
    }

    if(DumpPowerRecord(filename, outfile, check_sum))
    {
        DumpZero(outfile,(unsigned char)ENUM_GB19056_CMD_QUERY_POWER_RECORD, check_sum);
    }
    actual_cmd_number ++;
    cmd_count ++;
    if(notify)
    {
        notify->ProgressNotify(cmd_count , total_cmd);
    }

    if(DumpSettingRecord(filename, outfile, check_sum))
    {
        DumpZero(outfile,(unsigned char)ENUM_GB19056_CMD_QUERY_PARA_MODIFY_RECORD, check_sum);
    }
    actual_cmd_number ++;
    cmd_count ++;
    if(notify)
    {
        notify->ProgressNotify(cmd_count , total_cmd);
    }

    if(DumpSpeedStatusRecord(filename, outfile, check_sum))
    {
        DumpZero(outfile,(unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_STATUS_RECORD, check_sum);
    }
    actual_cmd_number ++;
    cmd_count ++;
    if(notify)
    {
        notify->ProgressNotify(cmd_count , total_cmd);
    }

    outfile.close();
    
    infile.clear();
    infile.open(temp_filename.c_str(), ios::in | ios::binary);
    if(!infile)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpData fail to open %s to read.", temp_filename.c_str());
        return -1;
    }
    
    outfile.clear();
    outfile.open(filename.c_str(), ios::out | ios::binary | ios::trunc);
    if(!outfile)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpData fail to open %s to write.", filename.c_str());
        return -1;
    }
    
    DumpShort(outfile, actual_cmd_number, check_sum);
    outfile << infile.rdbuf();
    outfile.write((const char *)&check_sum, 1);
    
    infile.close();
    outfile.close();
    unlink(temp_filename.c_str());
    system("sync");
    
    return 0;
}

int GB19056_2012::DumpFileSpeedRecord(string &filename, ofstream &outfile, unsigned char &check_sum)
{
    ofstream record_file;
    FILE *fp = NULL;
    int total_blocks = 0, current_blocks=0;
    unsigned char tmpbuf[126];
    int offset = 0; 

    
    if(PrepareTempRecFile(filename, record_file) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedRecord PrepareTempRecFile failed.");
        return -1;
    }

    fp = fopen("/nand2/tmp08.vdr", "r");
	if(fp == NULL) 
	{
		printf("Can't open tmp08.vdr %s\n", strerror(errno));
		return -1;
	}

    fread(&total_blocks, 4,1, fp);
    fread(&current_blocks, 4,1, fp);

    for(int i=0; i<total_blocks;  i++)
    {
        memset(tmpbuf,0, 126);

        offset = 10 + (current_blocks-1)*126;
        fseek(fp, offset, SEEK_SET);
        fread(tmpbuf, 126, 1, fp);
       
        for(int j = 0; j < 126; j ++)
        {
            DumpByte(record_file, tmpbuf[j], check_sum);
        }
        current_blocks--;
        if(current_blocks == 0)
            current_blocks = total_blocks;
    }
    fclose(fp);
    record_file.close();
    
    if(total_blocks <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedRecord no blocks.");
        DeleteTempRecFile(filename);
        return -1;
    } 
    
    ifstream infile;
    if(OpenTempRecFile(filename, infile) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedRecord OpenTempRecFile failed.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    DumpHeadAndRecord(outfile, infile, (unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_RECORD, check_sum);
    infile.close();
    DeleteTempRecFile(filename);
    return 0;
}

int GB19056_2012::DumpFileLocRecord(string &filename, ofstream &outfile, unsigned char &check_sum)
{
    ofstream record_file;
    FILE *fp = NULL;
    int total_blocks = 0, current_blocks=0;
    unsigned char tmpbuf[666];
    int offset = 0; 
    
    if(PrepareTempRecFile(filename, record_file) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpLocRecord PrepareTempRecFile failed.");
        return -1;
    }

    fp = fopen("/nand2/tmp09.vdr", "r");
    
	if(fp == NULL) 
	{
		printf("Can't open tmp09.vdr %s\n", strerror(errno));
		return -1;
	}

    fread(&total_blocks, 4,1, fp);
    fread(&current_blocks, 4,1, fp);

    for(int i=0; i<total_blocks;  i++)
    {
        memset(tmpbuf,0, 666);

        offset = 10 + (current_blocks-1)*666;
        fseek(fp, offset, SEEK_SET);
        fread(tmpbuf, 666, 1, fp);
        
        current_blocks--;
        if(current_blocks == 0)
            current_blocks = total_blocks;

        if(tmpbuf[0]==0x00 && tmpbuf[1]==0x00 && tmpbuf[2]==0x00)
            continue;
       
        for(int j = 0; j < 666; j ++)
        {
            DumpByte(record_file, tmpbuf[j], check_sum);
        }
    }
    fclose(fp);
    record_file.close();


    if(total_blocks <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpLocRecord no blocks.");
        DeleteTempRecFile(filename);
        return -1;
    }

    ifstream infile;
    if(OpenTempRecFile(filename, infile) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpLocRecord OpenTempRecFile failed.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    DumpHeadAndRecord(outfile, infile, (unsigned char)ENUM_GB19056_CMD_QUERY_LOC_RECORD, check_sum);
    infile.close();
    DeleteTempRecFile(filename);

    return 0;
}

int GB19056_2012::DumpSpeedRecord(string &filename, ofstream &outfile, unsigned char &check_sum)
{
    ofstream record_file;

    VDRLocStatus1Table *loc_table = new VDRLocStatus1Table();
    int top_index = loc_table->RefreshTop();
    if(top_index < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedRecord RefreshTop failed.");
        delete loc_table;
        return -1;
    }
    int current_index = loc_table->RefreshBottom();
    if(current_index < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedRecord RefreshBottom failed.");
        delete loc_table;
        return -1;
    }
    int speed, status;
    int occur_time;
    loc_table->GetIntValue(VDR_LOCS_VDR_SPEED, speed);
    loc_table->GetIntValue(VDR_LOCS_VDR_STATUS, status);
    loc_table->GetIntValue(VDR_LOCS_OCCUR_TIME, occur_time);

    if(PrepareTempRecFile(filename, record_file) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedRecord PrepareTempRecFile failed.");
        delete loc_table;
        return -1;
    }
    
    int stop = occur_time, start;

    int multi_record_index = -1;
    int total_blocks = 0;
    int speed_flag = 0;
    int index = 0;

    for(; GB19056_2012::get_udisk_stat();  stop = start - 1)
    {
        unsigned char tmp_vector[120];
        
        if(stop < 1325376000)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedRecord  stoptime < 2012.01.01 00:00:00.\n");
            break;
        }

        start = (stop / 60 * 60);
        
        if(occur_time < start)
        {
            continue;
        }
        
        memset(tmp_vector, 0xFF, 120);
        
        bool is_empty = true;
        int time_index;
        speed_flag = 0;
        for(time_index = stop; time_index >= start; time_index --)
        {
            while(time_index < occur_time)
            {
                if((multi_record_index == -1) || (multi_record_index >= loc_table->GetResultsCount()))
                {
                    current_index = loc_table->RefreshPrevious(current_index, top_index);
                    if(current_index < 0)
                    {
                        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedRecord RefreshPrevious reach end(%d, %d).", current_index, top_index);
                        break;
                    }
                    multi_record_index = 0;
                }
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_VDR_SPEED, speed);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_VDR_STATUS, status);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_OCCUR_TIME, occur_time);
                multi_record_index ++;
                index = time_index-occur_time;
            }

			
            if(time_index < occur_time)
            {
                break;
            }

            int v_index = (time_index - start) * 2;
            if((time_index == occur_time) ||((index > 0) && (index < 6)))
            {
                if(speed != 0)
                    speed_flag = 1;
                tmp_vector[v_index] = (unsigned char)(speed/10);
                tmp_vector[v_index + 1] = (unsigned char)status;
                is_empty= false;
            }
        }

        if(!is_empty && speed_flag)
        {
            vector<unsigned char> bcd;
            GetTime(start, bcd);
            DumpVector(record_file, bcd, check_sum);

            for(int j = 0; j < 120; j ++)
            {
                DumpByte(record_file, tmp_vector[j], check_sum);
            }
            
            total_blocks ++;
        }

		
        if(time_index >= start)
        {
            break;
        }
    }
    
    delete loc_table;
    
    record_file.close();
    
    if(total_blocks <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedRecord no blocks.");
        DeleteTempRecFile(filename);
        return -1;
    }

    ifstream infile;
    if(OpenTempRecFile(filename, infile) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedRecord OpenTempRecFile failed.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    DumpHeadAndRecord(outfile, infile, (unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_RECORD, check_sum);
    infile.close();
    DeleteTempRecFile(filename);

    return 0;
}

int GB19056_2012::DumpLocRecord(string &filename, ofstream &outfile, unsigned char &check_sum)
{
    ofstream record_file;

    VDRLocStatus2Table *loc_table = new VDRLocStatus2Table();
    int top_index = loc_table->RefreshTop();
    if(top_index < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpLocRecord RefreshTop failed.");
        delete loc_table;
        return -1;
    }
    int current_index = loc_table->RefreshBottom();
    if(current_index < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpLocRecord RefreshBottom failed.");
        delete loc_table;
        return -1;
    }

    int longitude, latitude, altitude, status, speed;
    int tmplongitude, tmplatitude;
    int occur_time;
    loc_table->GetIntValue(VDR_LOCS_LONGITUDE, longitude);
    loc_table->GetIntValue(VDR_LOCS_LATITUDE, latitude);
    loc_table->GetIntValue(VDR_LOCS_ALTITUDE, altitude);
    loc_table->GetIntValue(VDR_LOCS_STATUS_FLAG, status);
    loc_table->GetIntValue(VDR_LOCS_OCCUR_TIME, occur_time);
    loc_table->GetIntValue(VDR_LOCS_VDR_SPEED, speed);
    occur_time = MINUTE_ALINE(occur_time);

    if(PrepareTempRecFile(filename, record_file) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpLocRecord PrepareTempRecFile failed.");
        delete loc_table;
        return -1;
    }

    int stop = occur_time, start;
    
    int multi_record_index = -1;
    int total_blocks = 0;

    for(; GB19056_2012::get_udisk_stat();  stop = start - 60)
    {
        unsigned char tmp_vector[660];

        if(stop < 1325376000)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpLocRecord  stoptime < 2012.01.01 00:00:00.\n");
            break;
        }

        stop = MINUTE_ALINE(stop);
        
        start = HOUR_ALINE(stop);
        
        if(occur_time < start)
        {
            continue;
        }
        
        memset(tmp_vector, 0xFF, 660);
        
        bool is_empty = true;
        int time_index;
        for(time_index = stop; time_index >= start; time_index -= 60)
        {
            while(time_index < occur_time)
            {
                if((multi_record_index == -1) || (multi_record_index >= loc_table->GetResultsCount()))
                {
                    current_index = loc_table->RefreshPrevious(current_index, top_index);
                    if(current_index < 0)
                    {
                        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpLocRecord RefreshPrevious reach end(%d, %d).", current_index, top_index);
                        break;
                    }
                    multi_record_index = 0;
                }
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_LONGITUDE, longitude);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_LATITUDE, latitude);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_ALTITUDE, altitude);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_STATUS_FLAG, status);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_OCCUR_TIME, occur_time);
                loc_table->GetIntValue(multi_record_index, VDR_LOCS_VDR_SPEED, speed);
                occur_time = MINUTE_ALINE(occur_time);
                multi_record_index ++;
            }
            
            if(time_index < occur_time)
            {
                break;
            }

            int v_index = (time_index - start)/60 * 11;
            if(time_index == occur_time)
            {
                if(status & 0x02)
                {
                    tmplongitude = longitude*6/10;
                    if(status & 0x08)
                    {
                        tmplongitude = -tmplongitude;
                    }
                    tmplatitude = latitude*6/10;
                    if(status & 0x04)
                    {
                        tmplatitude = -tmplatitude;
                    }
                    
                    tmp_vector[v_index] = (tmplongitude >> 24) & 0xFF;
                    tmp_vector[v_index + 1] = (tmplongitude >> 16) & 0xFF;
                    tmp_vector[v_index + 2] = (tmplongitude >> 8) & 0xFF;
                    tmp_vector[v_index + 3] = tmplongitude & 0xFF;
                    tmp_vector[v_index + 4] = (tmplatitude >> 24) & 0xFF;
                    tmp_vector[v_index + 5] = (tmplatitude >> 16) & 0xFF;
                    tmp_vector[v_index + 6] = (tmplatitude >> 8) & 0xFF;
                    tmp_vector[v_index + 7] = tmplatitude & 0xFF;
                    tmp_vector[v_index + 8] = (altitude >> 8) & 0xFF;
                    tmp_vector[v_index + 9] = altitude & 0xFF;
                    tmp_vector[v_index + 10] = (unsigned char)(speed/10);
                    is_empty= false;
                }
                else if(speed != 0)
                {
                    tmp_vector[v_index] = 0x7F;
                    tmp_vector[v_index + 4] = 0x7F;
                    tmp_vector[v_index + 8] = 0x7F;
                    tmp_vector[v_index + 10] = (unsigned char)(speed/10);
                    is_empty= false;
                }
                else
                {
                    tmp_vector[v_index] = 0x7F;
                    tmp_vector[v_index + 4] = 0x7F;
                    tmp_vector[v_index + 8] = 0x7F;
                    tmp_vector[v_index + 10] = (unsigned char)(speed/10);
                }
            }
            else
            {
                tmp_vector[v_index] = 0xFF;
            }
        }

        if(!is_empty)
        {
            vector<unsigned char> bcd;
            GetTime(start, bcd);
            DumpVector(record_file, bcd, check_sum);

            for(int j = 0; j < 660; j ++)
            {
                DumpByte(record_file, tmp_vector[j], check_sum);
            }
            
            total_blocks ++;
        }
        
        if(time_index >= start)
        {
            break;
        }
    }
    
    delete loc_table;    
    
    record_file.close();

    if(total_blocks <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpLocRecord no blocks.");
        DeleteTempRecFile(filename);
        return -1;
    }

    ifstream infile;
    if(OpenTempRecFile(filename, infile) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpLocRecord OpenTempRecFile failed.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    DumpHeadAndRecord(outfile, infile, (unsigned char)ENUM_GB19056_CMD_QUERY_LOC_RECORD, check_sum);
    infile.close();
    DeleteTempRecFile(filename);

    return 0;
}

int GB19056_2012::DumpAccidentRecord(string &filename, ofstream &outfile, unsigned char &check_sum)
{
    ofstream record_file;

    VDRAccidentTable *acc_table = new VDRAccidentTable();
    int ret = acc_table->RefreshAll(VDR_ACCIDENT_OCCUR_TIME, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpAccidentRecord RefreshAll failed.");
        delete acc_table;
        return -1;
    }
    
    if(acc_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpAccidentRecord RefreshAll no results.");
        delete acc_table;
        return -1;
    }

    if(PrepareTempRecFile(filename, record_file) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpAccidentRecord PrepareTempRecFile failed.");
        delete acc_table;
        return -1;
    }

    int multi_record_index = 0;
    int total_blocks = 0;

    for(;GB19056_2012::get_udisk_stat();)
    {
        int occur_time;
        vector<unsigned char> speed;
        vector<unsigned char> status;
        string license;
        int longitude, latitude, altitude;
        if(multi_record_index >= acc_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpAccidentRecord reach end.");
            break;
        }
        acc_table->GetSequenceValue(multi_record_index, VDR_ACCIDENT_SPEED, speed);
        acc_table->GetSequenceValue(multi_record_index, VDR_ACCIDENT_STATUS, status);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_OCCUR_TIME, occur_time);
        acc_table->GetStringValue(multi_record_index, VDR_ACCIDENT_DRIVER_LICENSE_NUM, license);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_LONGITUDE, longitude);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_LATITUDE, latitude);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_ALTITUDE, altitude);
        multi_record_index ++;
        
        if(speed.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpAccidentRecord invalid speed.");
            continue;
        }

        if(status.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpAccidentRecord invalid status.");
            continue;
        }

        vector<unsigned char> bcd;
        GetTime(occur_time, bcd);
        DumpVector(record_file, bcd, check_sum);
        DumpString(record_file, license, 18, check_sum);

        for(int time_index = 0; time_index < 100; time_index ++)
        {
            if((int)speed.size() > time_index)
            {
                DumpByte(record_file, speed[time_index], check_sum);
            }
            else
            {
                DumpByte(record_file, 0xFF, check_sum);
            }
            if((int)status.size() > time_index)
            {
                DumpByte(record_file, status[time_index], check_sum);
            }
            else
            {
                DumpByte(record_file, 0xFF, check_sum);
            }
        }
        DumpLong(record_file, longitude, check_sum);
        DumpLong(record_file, latitude, check_sum);
        DumpShort(record_file, altitude, check_sum);
        total_blocks ++;
    }

    delete acc_table;
    
    record_file.close();

    if(total_blocks <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpAccidentRecord no blocks.");
        DeleteTempRecFile(filename);
        return -1;
    }

    ifstream infile;
    if(OpenTempRecFile(filename, infile) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpAccidentRecord OpenTempRecFile failed.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    DumpHeadAndRecord(outfile, infile, (unsigned char)ENUM_GB19056_CMD_QUERY_ACCIDENT_RECORD, check_sum);
    infile.close();
    DeleteTempRecFile(filename);

    return 0;
}

int GB19056_2012::DumpTiredRecord(string &filename, ofstream &outfile, unsigned char &check_sum)
{
    ofstream record_file;

    VDRTiredTable *tired_table = new VDRTiredTable();
    int ret = tired_table->RefreshAll(VDR_TIRED_BEGIN_TIME, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpTiredRecord RefreshAll failed.");
        delete tired_table;
        return -1;
    }
    
    if(tired_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpTiredRecord RefreshAll no results.");
        delete tired_table;
        return -1;
    }

    if(PrepareTempRecFile(filename, record_file) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpTiredRecord PrepareTempRecFile failed.");
        delete tired_table;
        return -1;
    }

    int multi_record_index = 0;
    int total_blocks = 0;

    for(;GB19056_2012::get_udisk_stat();)
    {
        int start_drive_time, stop_drive_time;
        int start_longitude, start_latitude, start_altitude;
        int stop_longitude, stop_latitude, stop_altitude;
        string license;

        if(multi_record_index >= tired_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpTiredRecord reach end.");
            break;
        }
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_TIME, start_drive_time);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_LONGITUDE, start_longitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_LATITUDE, start_latitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_ALTITUDE, start_altitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_TIME, stop_drive_time);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_LONGITUDE, stop_longitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_LATITUDE, stop_latitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_ALTITUDE, stop_altitude);
        tired_table->GetStringValue(multi_record_index, VDR_TIRED_DRIVER_LICENSE_NUM, license);
        multi_record_index ++;

        DumpString(record_file, license, 18, check_sum);
        vector<unsigned char> bcd;
        GetTime(start_drive_time, bcd);
        DumpVector(record_file, bcd, check_sum);
        bcd.clear();
        GetTime(stop_drive_time, bcd);
        DumpVector(record_file, bcd, check_sum);

        DumpLong(record_file, start_longitude, check_sum);
        DumpLong(record_file, start_latitude, check_sum);
        DumpShort(record_file, start_altitude, check_sum);
        DumpLong(record_file, stop_longitude, check_sum);
        DumpLong(record_file, stop_latitude, check_sum);
        DumpShort(record_file, stop_altitude, check_sum);
        total_blocks ++;
    }
    
    delete tired_table;
    
    record_file.close();

    if(total_blocks <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpTiredRecord no blocks.");
        DeleteTempRecFile(filename);
        return -1;
    }

    ifstream infile;
    if(OpenTempRecFile(filename, infile) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpTiredRecord OpenTempRecFile failed.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    DumpHeadAndRecord(outfile, infile, (unsigned char)ENUM_GB19056_CMD_QUERY_OVERTIME_RECORD, check_sum);
    infile.close();
    DeleteTempRecFile(filename);

    return 0;
}

int GB19056_2012::DumpDriverRecord(string &filename, ofstream &outfile, unsigned char &check_sum)
{
    ofstream record_file;

    DriversLoginTable *login_table = new DriversLoginTable();
    int ret = login_table->RefreshAll(VDR_DRV_OCCUR_TIME, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpDriverRecord RefreshAll failed.");
        delete login_table;
        return -1;
    }
    
    if(login_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpDriverRecord RefreshAll no results.");
        delete login_table;
        return -1;
    }

    if(PrepareTempRecFile(filename, record_file) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpDriverRecord PrepareTempRecFile failed.");
        delete login_table;
        return -1;
    }

    int multi_record_index = 0;
    int total_blocks = 0;

    for(;GB19056_2012::get_udisk_stat();)
    {
        int occur_time, event;
        string license;

        if(multi_record_index >= login_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpDriverRecord reach end.");
            break;
        }
        login_table->GetIntValue(multi_record_index, VDR_DRV_OCCUR_TIME, occur_time);
        login_table->GetIntValue(multi_record_index, VDR_DRV_LOGIN_LOGOUT, event);
        login_table->GetStringValue(multi_record_index, VDR_DRV_DRIVER_LICENSE_NUM, license);
        multi_record_index ++;

        vector<unsigned char> bcd;
        GetTime(occur_time, bcd);
        DumpVector(record_file, bcd, check_sum);
        DumpString(record_file, license, 18, check_sum);

        if(event)
        {
            DumpByte(record_file, 0x01, check_sum);
        }
        else
        {
            DumpByte(record_file, 0x02, check_sum);
        }

        total_blocks ++;
    }
    
    delete login_table;

    record_file.close();

    if(total_blocks <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpDriverRecord no blocks.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    ifstream infile;
    if(OpenTempRecFile(filename, infile) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpDriverRecord OpenTempRecFile failed.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    DumpHeadAndRecord(outfile, infile, (unsigned char)ENUM_GB19056_CMD_QUERY_DRIVER_RECORD, check_sum);
    infile.close();
    DeleteTempRecFile(filename);

    return 0;
}

int GB19056_2012::DumpPowerRecord(string &filename, ofstream &outfile, unsigned char &check_sum)
{
    ofstream record_file;

    VDROFFStatusTable *off_table = new VDROFFStatusTable();
    int ret = off_table->RefreshAll(VDR_OFFS_ACC_ON_TIME, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpPowerRecord RefreshAll failed.");
        delete off_table;
        return -1;
    }
    
    if(off_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpPowerRecord RefreshAll no results.");
        delete off_table;
        return -1;
    }

    if(PrepareTempRecFile(filename, record_file) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpPowerRecord PrepareTempRecFile failed.");
        delete off_table;
        return -1;
    }
        
    int multi_record_index = 0;
    int total_blocks = 0;

    for(;GB19056_2012::get_udisk_stat();)
    {
        int on_time, off_time;
        string license;
        vector<unsigned char> bcd;

        if(multi_record_index >= off_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpPowerRecord reach end.");
            break;
        }
        off_table->GetIntValue(multi_record_index, VDR_OFFS_ACC_ON_TIME, on_time);
        off_table->GetIntValue(multi_record_index, VDR_OFFS_ACC_OFF_TIME, off_time);
        multi_record_index ++;
        bcd.clear(); 

        if(off_time == 0)
        {
            GetTime(on_time, bcd);
            DumpVector(record_file, bcd, check_sum);
            DumpByte(record_file, 0x01, check_sum);
            total_blocks ++;
        }
        else
        {
            GetTime(off_time, bcd);
            DumpVector(record_file, bcd, check_sum);
            DumpByte(record_file, 0x02, check_sum);
            bcd.clear();        
            GetTime(on_time, bcd);
            DumpVector(record_file, bcd, check_sum);
            DumpByte(record_file, 0x01, check_sum);
            total_blocks += 2;
        }
    }
    
    delete off_table;

    record_file.close();

    if(total_blocks <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpPowerRecord PrepareTempRecFile no blocks.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    ifstream infile;
    if(OpenTempRecFile(filename, infile) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpPowerRecord OpenTempRecFile failed.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    DumpHeadAndRecord(outfile, infile, (unsigned char)ENUM_GB19056_CMD_QUERY_POWER_RECORD, check_sum);
    infile.close();
    DeleteTempRecFile(filename);

    return 0;
}

int GB19056_2012::DumpSettingRecord(string &filename, ofstream &outfile, unsigned char &check_sum)
{
    ofstream record_file;
    
    VDRParamInfoTable *para_table = new VDRParamInfoTable();
    int ret = para_table->RefreshAll(VDR_PARAM_OCCUR_TIME, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSettingRecord RefreshAll failed.");
        delete para_table;
        return -1;
    }
    
    if(para_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSettingRecord RefreshAll no results.");
        delete para_table;
        return -1;
    }

    if(PrepareTempRecFile(filename, record_file) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSettingRecord PrepareTempRecFile failed.");
        delete para_table;
        return -1;
    }

    int multi_record_index = 0;
    int total_blocks = 0;

    for(; GB19056_2012::get_udisk_stat();)
    {
        int occur_time;
        int event;

        if(multi_record_index >= para_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSettingRecord reach end.");
            break;
        }
        para_table->GetIntValue(multi_record_index, VDR_PARAM_OCCUR_TIME, occur_time);
        para_table->GetIntValue(multi_record_index, VDR_PARAM_EVENT_ID, event);
        multi_record_index ++;

        vector<unsigned char> bcd;
        GetTime(occur_time, bcd);
        DumpVector(record_file, bcd, check_sum);
        DumpByte(record_file, (unsigned char)event, check_sum);

        total_blocks ++;
    }
    
    delete para_table;

    record_file.close();

    if(total_blocks <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSettingRecord PrepareTempRecFile no blocks.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    ifstream infile;
    if(OpenTempRecFile(filename, infile) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSettingRecord OpenTempRecFile failed.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    DumpHeadAndRecord(outfile, infile, (unsigned char)ENUM_GB19056_CMD_QUERY_PARA_MODIFY_RECORD, check_sum);
    infile.close();
    DeleteTempRecFile(filename);

    return 0;
}

int GB19056_2012::DumpSpeedStatusRecord(string &filename, ofstream &outfile, unsigned char &check_sum)
{
    ofstream record_file;
    
    VDRSpeedInfoTable *speed_table = new VDRSpeedInfoTable();
    int ret = speed_table->RefreshAll(VDR_SPEED_BEGIN_TIME, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedStatusRecord RefreshAll failed.");
        delete speed_table;
        return -1;
    }
    
    if(speed_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedStatusRecord RefreshAll no results.");
        delete speed_table;
        return -1;
    }

    if(PrepareTempRecFile(filename, record_file) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedStatusRecord PrepareTempRecFile failed.");
        delete speed_table;
        return -1;
    }

    int multi_record_index = 0;
    int total_blocks = 0;

    for(; GB19056_2012::get_udisk_stat();)
    {
        int start_time, stop_time;
        vector<unsigned char> speed;
        vector<unsigned char> gps_speed;
        int status;
        
        if(multi_record_index >= speed_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedStatusRecord reach end.");
            break;
        }
        speed_table->GetSequenceValue(multi_record_index, VDR_SPEED_VDR_SPEED, speed);
        speed_table->GetSequenceValue(multi_record_index, VDR_SPEED_GPS_SPEED, gps_speed);
        speed_table->GetIntValue(multi_record_index, VDR_SPEED_BEGIN_TIME, start_time);
        speed_table->GetIntValue(multi_record_index, VDR_SPEED_END_TIME, stop_time);
        speed_table->GetIntValue(multi_record_index, VDR_SPEED_STATUS, status);
        multi_record_index ++;
        
        if(speed.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedStatusRecord invalid speed.");
            continue;
        }

        if(gps_speed.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedStatusRecord invalid gps_speed.");
            continue;
        }

        DumpByte(record_file, (unsigned char)status, check_sum);
        vector<unsigned char> bcd;
        GetTime(start_time, bcd);
        DumpVector(record_file, bcd, check_sum);
        bcd.clear();
        GetTime(stop_time, bcd);
        DumpVector(record_file, bcd, check_sum);

        for(int time_index = 0; time_index < 60; time_index ++)
        {
            if((int)speed.size() > time_index)
            {
                DumpByte(record_file, speed[time_index], check_sum);
            }
            else
            {
                DumpByte(record_file, 0xFF, check_sum);
            }
            if((int)gps_speed.size() > time_index)
            {
                DumpByte(record_file, gps_speed[time_index], check_sum);
            }
            else
            {
                DumpByte(record_file, 0xFF, check_sum);
            }
        }
        total_blocks ++;
    }
    
    delete speed_table;

    record_file.close();

    if(total_blocks <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedStatusRecord PrepareTempRecFile no blocks.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    ifstream infile;
    if(OpenTempRecFile(filename, infile) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::DumpSpeedStatusRecord OpenTempRecFile failed.");
        DeleteTempRecFile(filename);
        return -1;
    }
    
    DumpHeadAndRecord(outfile, infile, (unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_STATUS_RECORD, check_sum);
    infile.close();
    DeleteTempRecFile(filename);

    return 0;
}

string GB19056_2012::GetDataName(unsigned char cmd)
{
    string cmd_name;
    switch(cmd)
    {
        case (unsigned char)ENUM_GB19056_CMD_QUERY_VER:
            cmd_name = "执行标准版本年号";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_DRIVER:
            cmd_name = "当前驾驶人信息";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_TIME:
            cmd_name = "实时时间";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_MILEAGE:
            cmd_name = "累计行驶里程";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_PULSE_FACTOR:
            cmd_name = "脉冲系数";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_CAR_INFO:
            cmd_name = "车辆信息";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_STATUS:
            cmd_name = "状态信号配置信息";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_ID:
            cmd_name = "记录仪唯一性编号";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_RECORD:
            cmd_name = "行驶速度记录";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_LOC_RECORD:
            cmd_name = "位置信息记录";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_ACCIDENT_RECORD:
            cmd_name = "事故疑点记录";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_OVERTIME_RECORD:
            cmd_name = "超时驾驶记录";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_DRIVER_RECORD:
            cmd_name = "驾驶人身份记录";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_POWER_RECORD:
            cmd_name = "外部供电记录";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_PARA_MODIFY_RECORD:
            cmd_name = "参数修改记录";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_SPEED_STATUS_RECORD:
            cmd_name = "速度状态日志";
            break;
        case (unsigned char)ENUM_GB19056_CMD_QUERY_LAP_PULSE_FACTOR:
            cmd_name = "单圈脉冲系数";
            break;
        default:
            cmd_name = "未知命令";
            break;
    }
    
    return boost::locale::conv::from_utf<char>(cmd_name, GB19056_CHAR_SET);
}

int GB19056_2012::ProcessQueryModeID(GB19056Buffer &buffer)
{
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessQueryModeID failed to refresh deploy_table.");
        delete deploy_table;
        return -1;
    }
    vector<unsigned char> model_id;
    deploy_table->GetSequenceValue(VDR_DEPLOY_PARA_ID, model_id);
    delete deploy_table;

    buffer.PushVector(model_id, 7);
    
    return 0;
}
int GB19056_2012::ProcessSetServerInfo(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 32)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetServerInfo length invalid %d.", buffer.GetLength());
        return -1;
    }

    int tcp_port;
    string main_server, sec_server;

    tcp_port = buffer.GetWord();
    buffer.GetString(main_server, 15);
    buffer.GetString(sec_server, 15);
    
    VDRRunParaTable *runtable = new VDRRunParaTable();
    runtable->SetStringValue(VDR_PARA_MAIN_SERVER_ADDRESS, main_server);
    runtable->SetStringValue(VDR_PARA_SECOND_SERVER_ADDRESS, sec_server);
    runtable->SetIntValue(VDR_PARA_TCP_PORT, tcp_port);
    runtable->SetIntValue(VDR_PARA_UDP_PORT, tcp_port);
    
    if(runtable->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetServerInfo failed to commit deploy_table.");
        buffer.PushByte(0x02);
    }
    else
    {
        buffer.PushByte(0x01);
    }
    
    delete runtable;

    return 0;

}
int GB19056_2012::ProcessSetDeployPara(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 83)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetDeployPara length invalid %d.", buffer.GetLength());
        return -1;
    }
    int provinceid = 0, cityid = 0, platecolor = 0, deploytime = 0, mileage = 0;
    vector<unsigned char> manufactoryid, modelno, bcd;
    string plateid, platesort, carvin, phonenum, tmp; 

    buffer.GetString(tmp, 2);
    provinceid = stringToNum<int>(tmp);
    tmp.clear();
    buffer.GetString(tmp, 4);
    cityid = stringToNum<int>(tmp);
    buffer.GetVector(manufactoryid, 5);
    buffer.GetVector(modelno, 8);
    buffer.GetString(carvin, 17);
    platecolor = buffer.GetByte(); 
    buffer.GetString(plateid, 12);
    buffer.GetString(platesort, 12);
    buffer.GetVector(bcd, 6);
    deploytime = GetTime(bcd);
    bcd.clear();
    buffer.GetVector(bcd, 4);
    mileage = BcdToInt(bcd);
    buffer.GetString(phonenum, 12);
    
    string car_vin_utf = boost::locale::conv::to_utf<char>(carvin, GB19056_CHAR_SET);
    string plate_no_utf = boost::locale::conv::to_utf<char>(plateid, GB19056_CHAR_SET);
    string plate_type_utf = boost::locale::conv::to_utf<char>(platesort, GB19056_CHAR_SET);

    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetDeployPara failed to refresh deploy_table.");
        delete deploy_table;
        buffer.PushByte(0x02);
        return 0;
    }
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_PROVINCE_ID, provinceid);
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_CITY_ID, cityid);
    deploy_table->SetSequenceValue(VDR_DEPLOY_PARA_MANUFACTORY_ID, manufactoryid);
    deploy_table->SetSequenceValue(VDR_DEPLOY_PARA_MODEL_NO, modelno);
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_PLATE_COLOR, platecolor);
    deploy_table->SetStringValue(VDR_DEPLOY_PARA_PLATE_ID, plate_no_utf);
    deploy_table->SetStringValue(VDR_DEPLOY_PARA_PLATE_SORT, plate_type_utf);
    deploy_table->SetStringValue(VDR_DEPLOY_PARA_CAR_VIN, car_vin_utf);
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_DEPLOY_TIME, deploytime);
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_DEPLOY_MILEAGE, mileage);
    deploy_table->SetStringValue(VDR_DEPLOY_PARA_PHONE_NUMBER, phonenum);    
    
    if(deploy_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetDeployPara failed to commit deploy_table.");
        buffer.PushByte(0x02);
    }
    else
    {
        buffer.PushByte(0x01);
    }
    
    delete deploy_table;

    return 0;
}
int GB19056_2012::ProcessSetAuthorken(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 27)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetAuthorken length invalid %d.", buffer.GetLength());
        return -1;
    }

    string phoneNumber, phone, authtoken;

    buffer.GetString(phone, 12);
    buffer.GetString(authtoken, 15);
        
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetAuthorken failed to refresh deploy_table.");
        delete deploy_table;
        buffer.PushByte(0x02);
        return 0;
    }
    deploy_table->GetStringValue(VDR_DEPLOY_PARA_PHONE_NUMBER, phoneNumber);

    printf("GB19056_2012::ProcessSetAuthorken phoneold=%s, phonenew=%s\n",phoneNumber.c_str(), phone.c_str());
    if(phoneNumber != phone)
    {
       buffer.PushByte(0x02); 
       delete deploy_table;
       return 0;
    }

    deploy_table->SetStringValue(VDR_DEPLOY_PARA_AUTH_TOKEN, authtoken);

    if(deploy_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetAuthorken failed to commit deploy_table.");
        buffer.PushByte(0x02);
    }
    else
    {
        buffer.PushByte(0x01);
    }
    
    delete deploy_table;

    return 0;
}
int GB19056_2012::ProcessSetPulseFactor(GB19056Buffer &buffer)
{
    if(buffer.GetLength() < 3)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetPulseFactor length invalid %d.", buffer.GetLength());
        return -1;
    }

    int speed_pulse = buffer.GetWord();
    int lap_pulse = buffer.GetByte();
    
    VDRDeployParaTable *deploy_table = new VDRDeployParaTable();
    if(deploy_table->Refresh() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetPulseFactor failed to refresh deploy_table.");
        delete deploy_table;
        buffer.PushByte(0x02);
        return 0;
    }
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_SPEED_PULSE_FACTOR, speed_pulse);
    deploy_table->SetIntValue(VDR_DEPLOY_PARA_LAP_PULSE_FACTOR, lap_pulse);
    if(deploy_table->Commit() < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSetPulseFactor failed to commit deploy_table.");
        buffer.PushByte(0x02);
    }
    else
    {
        buffer.PushByte(0x01);
    }
    
    delete deploy_table;

    return 0;
}
int GB19056_2012::ProcessSetReboot(GB19056Buffer &)
{
    system("accoff.sh 2 &");

    return 0;
}

int GB19056_2012::ProcessFileDriveRecordQuery_JT808(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;

    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriveRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriveRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }

    
    int stop, start;
    int stop_sec=0, start_sec=0;
    time_t ttime ;
    struct tm p;

    stop = MINUTE_ALINE(stop_time);
    start = MINUTE_ALINE(start_time);
       
    ttime = stop_time;
    localtime_r(&ttime, &p);
    stop_sec = p.tm_sec;

    ttime = start_time;
    localtime_r(&ttime, &p);
    start_sec = p.tm_sec;

    FILE *fp = NULL;
    int total_blocks = 0, current_blocks=0;
    unsigned char tmp_vector[126];
    int offset = 0, current_min=0, i=0, j=0; 
    struct tm lt;

    fp = fopen("/nand2/tmp08.vdr", "r");
    
	if(fp == NULL) 
	{
		printf("Can't open tmp08.vdr %s\n", strerror(errno));
		return -1;
	}

    fread(&total_blocks, 4,1, fp);
    fread(&current_blocks, 4,1, fp);
        
    for(i=0; i<total_blocks;  i++)
    {
        memset(tmp_vector,0xFF, 126);

        offset = 10 + (current_blocks-1)*126;
        fseek(fp, offset, SEEK_SET);
        fread(tmp_vector, 126, 1, fp);

        lt.tm_year = BCD_TO_VALUE(tmp_vector[0]) + 100;
        lt.tm_mon = BCD_TO_VALUE(tmp_vector[1]) - 1;
        lt.tm_mday = BCD_TO_VALUE(tmp_vector[2]);
        lt.tm_hour = BCD_TO_VALUE(tmp_vector[3]);
        lt.tm_min = BCD_TO_VALUE(tmp_vector[4]);
        lt.tm_sec = BCD_TO_VALUE(tmp_vector[5]);
        current_min = (unsigned int)mktime(&lt);
        //printf("current_min=%d,currentblocks=%d\n",current_min, current_blocks);
        if(current_min <= stop)
        {
            if(current_min < start)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery invaild data, start=%d, stop=%d,cur=%d\n.", 
                   start, stop, current_min);
                return 0;
            }
            
            if(current_min == stop)
            {
                if(start < stop)
                    start_sec = 0;
            }
            else
            {
                stop_sec = 59;
                if(current_min > start)
                    start_sec = 0;
                else
                    start_sec =  p.tm_sec;
            }
            //printf("start=%d,starts=%d,stop=%d,stops=%d,cur=%d..\n",start,start_sec,stop,stop_sec,current_min);
            for(j=0; j<start_sec; j++)
                memset(tmp_vector+6+j*2, 0xFF, 2);
            
            for(j=stop_sec+1; j<60; j++)
                memset(tmp_vector+6+j*2, 0xFF, 2);
            
            for(j = 0; j < 126; j ++)
            {
                buffer.PushByte(tmp_vector[j]);
            }  
            max_block_number--;
            if(max_block_number == 0)
                break;
        }

        current_blocks--;
        if(current_blocks == 0)
            current_blocks = total_blocks;
    }
    fclose(fp);

    if(current_min > stop)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery invaild data, start=%d, stop=%d,cur=%d\n.", 
           start, stop, current_min);
        return 0;
    }
    
    return 0;
}
int GB19056_2012::ProcessFileLocRecordQuery_JT808(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    int stop , start;
    int start_min = 0, stop_min = 0; 
    time_t ttime ;
    struct tm p;

    ttime = start_time;
    localtime_r(&ttime, &p);
    start_min = p.tm_min;
    
    start = HOUR_ALINE(start_time);
    stop = HOUR_ALINE(stop_time);
    
    ttime = stop_time;
    localtime_r(&ttime, &p);
    stop_min = p.tm_min;

    printf("starth=%d,startm=%d,stoph=%d,stopm=%d...\n",start,start_min,stop,stop_min);
    
    FILE *fp = NULL;
    int total_blocks = 0, current_blocks=0;
    unsigned char tmp_vector[666];
    int offset = 0, current_hour=0, i=0; 
    struct tm lt;

    fp = fopen("/nand2/tmp09.vdr", "r");
    
	if(fp == NULL) 
	{
		printf("Can't open tmp09.vdr %s\n", strerror(errno));
		return -1;
	}

    fread(&total_blocks, 4,1, fp);
    fread(&current_blocks, 4,1, fp);
    
    for(i=0; i<total_blocks;  i++)
    {
        memset(tmp_vector,0xFF, 666);

        offset = 10 + (current_blocks-1)*666;
        fseek(fp, offset, SEEK_SET);
        fread(tmp_vector, 666, 1, fp);

        lt.tm_year = BCD_TO_VALUE(tmp_vector[0]) + 100;
        lt.tm_mon = BCD_TO_VALUE(tmp_vector[1]) - 1;
        lt.tm_mday = BCD_TO_VALUE(tmp_vector[2]);
        lt.tm_hour = BCD_TO_VALUE(tmp_vector[3]);
        lt.tm_min = BCD_TO_VALUE(tmp_vector[4]);
        lt.tm_sec = BCD_TO_VALUE(tmp_vector[5]);
        current_hour = (unsigned int)mktime(&lt);
        //printf("currenthoure=%d,currentblocks=%d\n",current_hour, current_blocks);
        if(current_hour <= stop)
        {
            if((current_hour < start))
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessLocRecordQuery invaild data, start=%d, stop=%d,cur=%d\n.", 
                   start, stop, current_hour);
                return 0;
            }
            
            if(current_hour == stop)
            {
                if(start < stop)
                    start_min = 0;
            }
            else
            {
                stop_min = 59;
                if(current_hour > start)
                    start_min = 0;  
                else
                    start_min = p.tm_min; 
            }
            printf("starth=%d,startm=%d,stoph=%d,stopm=%d,curhoure=%d..\n",start,start_min,stop,stop_min,current_hour);
            for(i=0; i<start_min; i++)
                memset(tmp_vector+6+i*11, 0xFF, 11);

            for(i=stop_min+1; i<60; i++)
                memset(tmp_vector+6+i*11, 0xFF, 11);

            for(int j = 0; j < 666; j ++)
            {
                buffer.PushByte(tmp_vector[j]);
            }  
            max_block_number--;
            if(max_block_number == 0)
                break;
        }

        current_blocks--;
        if(current_blocks == 0)
            current_blocks = total_blocks;
    }
    fclose(fp);
    
    return 0;
}

int GB19056_2012::ProcessAccidentRecordQuery_JT808(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDRAccidentTable *acc_table = new VDRAccidentTable();
    int ret = acc_table->RefreshBetween(VDR_ACCIDENT_OCCUR_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete acc_table;
        return 0;
    }
    
    if(acc_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery RefreshBetween no results.");
        delete acc_table;
        return 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;

    for(int i = 0; i < max_block_number; i ++)
    {
        int occur_time;
        vector<unsigned char> speed;
        vector<unsigned char> status;
        string license;
        int longitude, latitude, altitude;
        if(multi_record_index >= acc_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery reach end.");
            break;
        }
        acc_table->GetSequenceValue(multi_record_index, VDR_ACCIDENT_SPEED, speed);
        acc_table->GetSequenceValue(multi_record_index, VDR_ACCIDENT_STATUS, status);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_OCCUR_TIME, occur_time);
        acc_table->GetStringValue(multi_record_index, VDR_ACCIDENT_DRIVER_LICENSE_NUM, license);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_LONGITUDE, longitude);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_LATITUDE, latitude);
        acc_table->GetIntValue(multi_record_index, VDR_ACCIDENT_ALTITUDE, altitude);
        multi_record_index ++;
        
        if(speed.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery invalid speed.");
            continue;
        }

        if(status.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessAccidentRecordQuery invalid status.");
            continue;
        }

        vector<unsigned char> bcd;
        GetTime(occur_time, bcd);
        buffer.PushVector(bcd, 6);
        buffer.PushString(license, 18);

        for(int time_index = 0; time_index < 100; time_index ++)
        {
            if((int)speed.size() > time_index)
            {
               buffer.PushByte(speed[time_index]);
            }
            else
            {
               buffer.PushByte(0xFF);
            }
            if((int)status.size() > time_index)
            {
               buffer.PushByte(status[time_index]);
            }
            else
            {
               buffer.PushByte(0xFF);
            }
        }
        buffer.PushLong(longitude);
        buffer.PushLong(latitude);
        buffer.PushWord(altitude);
        total_blocks ++;
    }
    
    delete acc_table;
    
    return 0;
}

int GB19056_2012::ProcessTiredRecordQuery_JT808(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTiredRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTiredRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDRTiredTable *tired_table = new VDRTiredTable();
    int ret = tired_table->RefreshBetween(VDR_TIRED_BEGIN_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTiredRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete tired_table;
        return 0;
    }
    
    if(tired_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTiredRecordQuery RefreshBetween no results.");
        delete tired_table;
        return 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;

    for(int i = 0; i < max_block_number; i ++)
    {
        int start_drive_time, stop_drive_time;
        int start_longitude, start_latitude, start_altitude;
        int stop_longitude, stop_latitude, stop_altitude;
        string license;

        if(multi_record_index >= tired_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessTiredRecordQuery reach end.");
            break;
        }
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_TIME, start_drive_time);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_LONGITUDE, start_longitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_LATITUDE, start_latitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_BEGIN_ALTITUDE, start_altitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_TIME, stop_drive_time);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_LONGITUDE, stop_longitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_LATITUDE, stop_latitude);
        tired_table->GetIntValue(multi_record_index, VDR_TIRED_END_ALTITUDE, stop_altitude);
        tired_table->GetStringValue(multi_record_index, VDR_TIRED_DRIVER_LICENSE_NUM, license);
        multi_record_index ++;

        buffer.PushString(license, 18);
        vector<unsigned char> bcd;
        GetTime(start_drive_time, bcd);
        buffer.PushVector(bcd, 6);
        bcd.clear();
        GetTime(stop_drive_time, bcd);
        buffer.PushVector(bcd, 6);

        buffer.PushLong(start_longitude);
        buffer.PushLong(start_latitude);
        buffer.PushWord(start_altitude);
        buffer.PushLong(stop_longitude);
        buffer.PushLong(stop_latitude);
        buffer.PushWord(stop_altitude);
        total_blocks ++;
        
    }
    
    delete tired_table;
    
    return 0;
}

int GB19056_2012::ProcessDriverInfoRecordQuery_JT808(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverInfoRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverInfoRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    DriversLoginTable *login_table = new DriversLoginTable();
    int ret = login_table->RefreshBetween(VDR_DRV_OCCUR_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverInfoRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete login_table;
        return 0;
    }
    
    if(login_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverInfoRecordQuery RefreshBetween no results.");
        delete login_table;
        return 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;

    for(int i = 0; i < max_block_number; i ++)
    {
        int occur_time, event;
        string license;

        if(multi_record_index >= login_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessDriverInfoRecordQuery reach end.");
            break;
        }
        login_table->GetIntValue(multi_record_index, VDR_DRV_OCCUR_TIME, occur_time);
        login_table->GetIntValue(multi_record_index, VDR_DRV_LOGIN_LOGOUT, event);
        login_table->GetStringValue(multi_record_index, VDR_DRV_DRIVER_LICENSE_NUM, license);
        multi_record_index ++;

        vector<unsigned char> bcd;
        GetTime(occur_time, bcd);
        buffer.PushVector(bcd, 6);
        buffer.PushString(license, 18);

        if(event)
        {
            buffer.PushByte(0x01);
        }
        else
        {
            buffer.PushByte(0x02);
        }

        total_blocks ++;
        
    }
    
    delete login_table;
    
    return 0;
}

int GB19056_2012::ProcessPowerRecordQuery_JT808(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    int on_time, off_time;
    int tmp_off_time = 0;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPowerRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPowerRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDROFFStatusTable *off_table = new VDROFFStatusTable();
    int ret = off_table->RefreshBetween(VDR_OFFS_ACC_ON_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPowerRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete off_table;
        return 0;
    }
    
    if(off_table->GetResultsCount() <= 0)
    {
        off_time = 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;
    vector<unsigned char> bcd;

    for(int i = 0; i < max_block_number; i ++)
    {
        if(multi_record_index >= off_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessPowerRecordQuery reach end.");
            break;
        }
        off_table->GetIntValue(multi_record_index, VDR_OFFS_ACC_ON_TIME, on_time);
        off_table->GetIntValue(multi_record_index, VDR_OFFS_ACC_OFF_TIME, off_time);
        multi_record_index ++;
        bcd.clear();
        
        if(off_time == 0)
        {
            GetTime(on_time, bcd);
            buffer.PushVector(bcd, 6);
            buffer.PushByte(0x01);
            total_blocks ++;
        }
        else
        {
            if(off_time < stop_time)
            {
                GetTime(off_time, bcd);
                buffer.PushVector(bcd, 6);
                buffer.PushByte(0x02); 
                total_blocks ++;
                i++;
            }

            bcd.clear();
            GetTime(on_time, bcd);
            buffer.PushVector(bcd, 6);
            buffer.PushByte(0x01);
            total_blocks ++;
        }
    }
    
    delete off_table;
    
    return 0;
}

int GB19056_2012::ProcessParaSettingRecordQuery_JT808(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessParaSettingRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessParaSettingRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDRParamInfoTable *para_table = new VDRParamInfoTable();
    int ret = para_table->RefreshBetween(VDR_PARAM_OCCUR_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessParaSettingRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete para_table;
        return 0;
    }
    
    if(para_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessParaSettingRecordQuery RefreshBetween no results.");
        delete para_table;
        return 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;

    for(int i = 0; i < max_block_number; i ++)
    {
        int occur_time;
        int event;

        if(multi_record_index >= para_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessParaSettingRecordQuery reach end.");
            break;
        }
        para_table->GetIntValue(multi_record_index, VDR_PARAM_OCCUR_TIME, occur_time);
        para_table->GetIntValue(multi_record_index, VDR_PARAM_EVENT_ID, event);
        multi_record_index ++;

        vector<unsigned char> bcd;
        GetTime(occur_time, bcd);
        buffer.PushVector(bcd, 6);
        buffer.PushByte((unsigned char)event);
    }
    
    delete para_table;
    
    return 0;
}

int GB19056_2012::ProcessSpeedInfoRecordQuery_JT808(GB19056Buffer &buffer)
{
    int start_time;
    int stop_time;
    int max_block_number;
    
    if(ProcessRecordQueryRequest(buffer, start_time, stop_time, max_block_number) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery ProcessRecordQueryRequest failed.", buffer.GetLength());
        return -1;
    }
    
    if(start_time > stop_time)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery time invalid(%d, %d).", start_time, stop_time);
        return -1;
    }
    
    VDRSpeedInfoTable *speed_table = new VDRSpeedInfoTable();
    int ret = speed_table->RefreshBetween(VDR_SPEED_BEGIN_TIME, start_time, stop_time, false);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery RefreshBetween failed(%d, %d).", start_time, stop_time);
        delete speed_table;
        return 0;
    }
    
    if(speed_table->GetResultsCount() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery RefreshBetween no results.");
        delete speed_table;
        return 0;
    }
    
    int multi_record_index = 0;
    int total_blocks = 0;

    for(int i = 0; i < max_block_number; i ++)
    {
        int start_time, stop_time;
        vector<unsigned char> speed;
        vector<unsigned char> gps_speed;
        int status;
        
        if(multi_record_index >= speed_table->GetResultsCount())
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery reach end.");
            break;
        }
        speed_table->GetSequenceValue(multi_record_index, VDR_SPEED_VDR_SPEED, speed);
        speed_table->GetSequenceValue(multi_record_index, VDR_SPEED_GPS_SPEED, gps_speed);
        speed_table->GetIntValue(multi_record_index, VDR_SPEED_BEGIN_TIME, start_time);
        speed_table->GetIntValue(multi_record_index, VDR_SPEED_END_TIME, stop_time);
        speed_table->GetIntValue(multi_record_index, VDR_SPEED_STATUS, status);
        multi_record_index ++;
        
        if(speed.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery invalid speed.");
            continue;
        }

        if(gps_speed.size() <= 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "GB19056_2012::ProcessSpeedInfoRecordQuery invalid gps_speed.");
            continue;
        }

        buffer.PushByte(status);
        vector<unsigned char> bcd;
        GetTime(start_time, bcd);
        buffer.PushVector(bcd, 6);
        bcd.clear();
        GetTime(stop_time, bcd);
        buffer.PushVector(bcd, 6);

        for(int time_index = 0; time_index < 60; time_index ++)
        {
            if((int)speed.size() > time_index)
            {
               buffer.PushByte(speed[time_index]);
            }
            else
            {
               buffer.PushByte(0xFF);
            }
            if((int)gps_speed.size() > time_index)
            {
               buffer.PushByte(gps_speed[time_index]);
            }
            else
            {
               buffer.PushByte(0xFF);
            }
        }
    }
    
    delete speed_table;
    
    return 0;
}


