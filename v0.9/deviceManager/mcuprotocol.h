#ifndef _MCU_PROTOCOL_H_
#define _MCU_PROTOCOL_H_

#define MCUADDR 0x54
#define CAMERAADDR  0x45

#define READ_MCU_ACC_ADDR       0x1A
#define READ_MCU_SPEED_ADDR     0x1B
#define READ_MCU_ALARM_ADDR     0x1C
#define READ_MCU_ANALOGUE_QUANTITY_ADDR    0x20
#define READ_MCU_CAN_DATA_ADDR  0x21
#define READ_MCU_START_UP_MODE  0x06

#define WRITE_MCU_SHUTDOWN_ADDR     0x01
#define WRITE_MCU_SPEED_FACTOR_ADDR 0x18      //速度脉冲系数
#define WRITE_MCU_LAP_FACTOR_ADDR   0x19      // 单圈脉冲系数
#define WRITE_MCU_DB9_7_CHECk_ADDR  0x1D      // 检定DB9-7


#define SHUTDOWN_FLAG       0x01
#define REBOOT_FLAG         0x02
#define SOFT_SHUTDOWN_FLAG  0x04

#define READ_CAMERA_STATUS  0x01    //获取摄像头连接状态

typedef enum
{
    DB9_MILEAGE_CHECK = 0x1,             
    DB9_PULSE_FACTOR_CHECK = 0x02, 
    DB9_TIME_CHECK = 0x04,
    DB9_OUT_CHECK = 0x08          
}DB9_7_CHECK;

int I2c_read_speed_data(unsigned char *speed_buf, int speed_len);
int I2c_read_acc_data(unsigned char &value, int len);
int I2c_read_alarm_data(unsigned char &value, int len);
int I2c_read_analog_quantity_data(unsigned char *buf, int len);
int I2c_read_camera_status(unsigned char &value, int len);
int I2c_read_can_data(unsigned char *buf, int len);
int I2c_read_startup_data(unsigned char &value, int len);


int I2c_write_shutdown_data(unsigned char value);
int I2c_write_speed_pluse_factor_data(int value);
int I2c_write_lap_pluse_factor_data(unsigned char value);
int I2c_write_DB9_7_check_data(unsigned char value);

#endif
