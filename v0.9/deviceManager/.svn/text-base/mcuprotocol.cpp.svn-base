#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "i2ccontrol.h"
#include "mcuprotocol.h"

int I2c_read_speed_data(unsigned char *buf, int len)
{
	int ret = -1;
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_SPEED_ADDR, buf, len*2);
	if(ret < 0)
	{
		printf("mcu I2c read byte err: %d\n", ret);
		return -1;
	}

	return ret;
}

int I2c_read_acc_data(unsigned char &value, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readByte(MCUADDR, READ_MCU_ACC_ADDR, value);
    
	return ret;
}

int I2c_read_alarm_data(unsigned char &value, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readByte(MCUADDR, READ_MCU_ALARM_ADDR, value);
    
	return ret;
}

int I2c_write_shutdown_data(unsigned char value)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeByte(MCUADDR, WRITE_MCU_SHUTDOWN_ADDR, value);
    
	return ret;
}

int I2c_write_speed_pluse_factor_data(int value)
{
    int ret = -1;	
    IBI2CControl i2cHander;
    unsigned char buf[2];

    if (i2cHander.checkOK() == 0)
    {
        printf("Check mcu i2cHander err!\n");
        return -1; 
    }
    buf[0] = (value >> 8) & 0xFF;
    buf[1] = value & 0xFF;
    ret = i2cHander.writeBlock(MCUADDR, WRITE_MCU_SPEED_FACTOR_ADDR, buf, 2);
    return ret;
}

int I2c_write_lap_pluse_factor_data(unsigned char value)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if (i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }

    ret = i2cHander.writeByte(MCUADDR, WRITE_MCU_LAP_FACTOR_ADDR, value);

    return ret;
}


int I2c_write_DB9_7_check_data(unsigned char value)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if (i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }

    ret = i2cHander.writeByte(MCUADDR, WRITE_MCU_DB9_7_CHECk_ADDR, value);

    return ret;
}

int I2c_read_analog_quantity_data(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_ANALOGUE_QUANTITY_ADDR, buf, len*2);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_read_camera_status(unsigned char &value, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readByte(CAMERAADDR, READ_CAMERA_STATUS, value);
    
	return ret;
}

int I2c_read_can_data(unsigned char *buf, int len)
{
    int ret = -1;
    IBI2CControl i2cHander;

    if (i2cHander.checkOK() == 0)
    {
        printf("Check mcu i2cHander err!\n");
        return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_CAN_DATA_ADDR, buf, len);
    if(ret < 0)
    {
        printf("mcu I2c read byte err: %d\n", ret);
        return -1;
    }

    return ret;
}

int I2c_read_startup_data(unsigned char &value, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readByte(MCUADDR, READ_MCU_START_UP_MODE, value);
    
	return ret;
}


#if 0
int main(int argc, char* argv[])
{
    int ret = -1;
	unsigned char speed_buf[128];
    unsigned char value;
	int speed_len = 20, distance=0;
    int i=0;

    
    if(!strcmp(argv[1], "1"))
    {
        ret = I2c_read_acc_data(value,1);
        printf("acc=0x%02x\n",value);
    }
    else
    {
        ret = I2c_read_speed_data(speed_buf,10);
        for(i= 0; i<ret; i++)
            printf(" 0x%02x ", speed_buf[i]);
    }
    #if 0
    distance = 0;
    for(i=0;i<speed_len/2; i++)
    {
        distance += speed_buf[i*2]/18;
    }
    #endif    
    
	
	return 0;

}
#endif


