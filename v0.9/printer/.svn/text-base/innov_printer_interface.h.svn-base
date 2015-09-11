#ifndef _INNOV_PRINTER_INTERFACE_H_
#define _INNOV_PRINTER_INTERFACE_H_

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif 


#define CAR_NUMBER_MAX_LEN                32 //车牌号最长长度
#define DRIVING_LICENSE_NUMBER_MAX_LEN    32 //驾驶证号最长长度
#define CAR_VIN_NUM_MAX_LEN               17 //车辆识别码最长长度
#define STATUS_LEN                      10

typedef enum   // 只添加三种车型
{
    CAR_PLATE_TYPE_LARGE_BUS = 0,   //大型汽车 
    CAR_PLATE_TYPE_MINI_GOODS,      //小型货车  TODO： 航天科技 
    CAR_PLATE_TYPE_LARGE_GOODS,	    //大型货车  TODO： 航天科技    
} EnumCarPlateType;

typedef enum //汽车速度状态
{
    CAR_SPEED_STATUS_ABNORMAL = 0,  //异常
    CAR_SPEED_STATUS_NORMAL,        //正常
} EnumCarSpeedStatus;

typedef struct {
     unsigned char driving_license_number[DRIVING_LICENSE_NUMBER_MAX_LEN+1]; //驾驶证号，以\0结尾
     time_t start_time;
     time_t stop_time;
} StruTiredDrivingRecord;

typedef struct {
    unsigned char car_plate_number[CAR_NUMBER_MAX_LEN+1]; //车牌号，以\0结尾
    unsigned char car_plate_type[CAR_NUMBER_MAX_LEN+1]; //车牌类型
    unsigned char car_vin_num[CAR_VIN_NUM_MAX_LEN+1];
    EnumCarSpeedStatus car_speed_status; //速度状态
    unsigned char driving_license_number[DRIVING_LICENSE_NUMBER_MAX_LEN+1]; //驾驶证号，以\0结尾
    unsigned int tired_driving_records_number;   //疲劳驾驶记录个数
    int stop_time;
    unsigned long stopDateTime;
    int stop_speed[15];
    StruTiredDrivingRecord tired_driving_records[0]; //疲劳驾驶记录开始 
    char status[STATUS_LEN][STATUS_LEN];
} StruInnovPrinterContent;


//如下是平台提供给打印机模块的接口
int open_printer(void);
int close_printer(void);

int write_printer(unsigned char *buf, int len);

//如下是打印机模块提供给上层应用的接口定义
int StartPrint(StruInnovPrinterContent *content);

#ifdef __cplusplus
}
#endif 

#endif
