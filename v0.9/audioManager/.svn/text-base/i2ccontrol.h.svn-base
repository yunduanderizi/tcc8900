/**********************************************************************
 Filename: i2ccontrol.h
 Description: Declare the IBI2CControl class.
 Author: Xing Qianqian
 Modify: Pang Bo
**********************************************************************/

#ifndef _IB_I2CCONTROL_H_
#define _IB_I2CCONTROL_H_

#include <pthread.h>

#define IB_MAX_I2C_NODE_LENGTH   0x20

/*
 * return the error 
 */
#define IBI2C_SUCCESS 0
#define IBI2C_OPEN_ERR -1
#define IBI2C_ADDR_ERR -2
#define IBI2C_FUNCS_ERR -3
#define IBI2C_IOCTL_FUNCS_ERR -4
#define IBI2C_IOCTL_SMBUS_ERR -5
#define IBI2C_IOCTL_SLAVE_FORCE_ERR -6
#define IBI2C_IOCTL_RDWR_ERR -7


class IBI2CControl
{
public:
    IBI2CControl(int i2cbus = 0);
    virtual ~IBI2CControl();

	int checkOK();
	void setOption(int retryTimes,int interval);

    int readByte(int address,unsigned char &value);

    int readBlock(int address, unsigned char value[], int length);
    int writeBlock(int address, unsigned char value[], int length);


    int readByte(int address, int subAddress,unsigned char &value);
    int writeByte(int address, int subAddress, unsigned char value);

    int readWord(int address, int subAddress,unsigned short &value);
    int writeWord(int address, int subAddress, unsigned short value);

    int readBlock(int address, int subAddress, unsigned char value[], int length);
    int writeBlock(int address, int subAddress, unsigned char value[], int length);
    
private:
	int getFuncs();
    inline int i2c_smbus_read_byte(unsigned char &val);
    inline int i2c_smbus_write_byte(unsigned char value);
    
	inline int i2c_smbus_read_byte_data(unsigned char command, unsigned char &val);
    inline int i2c_smbus_write_byte_data(unsigned char command, unsigned char value);

    inline int i2c_smbus_read_word_data(unsigned char command, unsigned short &val);
    inline int i2c_smbus_write_word_data(unsigned char command, unsigned short value);
    
	inline int i2c_smbus_read_block_data(unsigned char command, unsigned char *values);
    inline int i2c_smbus_write_block_data(unsigned char command, unsigned char length, unsigned char *values);

    inline int i2c_smbus_read_i2c_block_data(unsigned char command, unsigned char length, unsigned char *values);
	inline int i2c_smbus_write_i2c_block_data(unsigned char command, unsigned char length, unsigned char *values);

    inline int i2c_smbus_access(char read_write, unsigned char command, int size, union i2c_smbus_data *data);
    inline int i2c_smbus_write_quick(unsigned char value);
    inline int i2c_smbus_process_call(unsigned char command, unsigned short value);
    inline int i2c_smbus_block_process_call(unsigned char command, unsigned char length, unsigned char *values);
    int i2copen();
    void i2cclose();
private:	
    int m_retryTimes;
    int m_sleep_interval;
	long m_funcs;
	int m_i2cbus;
    int m_fd;

    pthread_mutex_t m_lock;
    class   InternalLock
    {
    public:
           InternalLock(pthread_mutex_t* lock);
           ~InternalLock();
    private:
           pthread_mutex_t* m_lock;
    };
};

#endif

