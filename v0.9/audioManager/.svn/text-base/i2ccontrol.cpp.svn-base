/**********************************************************************
 Filename: i2ccontrol.cpp
 Description: Defines the IBI2CControl class.
 Author: Xing Qianqian
 Modify: Pang Bo
**********************************************************************/

#include "i2ccontrol.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include "debugMonitor.h"

#define IBI2C_RETRY_TIMES 1
#define IBI2C_SLEEP_INTERVAL 10000

IBI2CControl::IBI2CControl(int i2cbus)
//	:m_i2cbus(i2cbus)
//  ,m_fd(IBI2C_OPEN_ERR)
//  ,m_retryTimes(IBI2C_RETRY_TIMES)
//  ,m_sleep_interval(IBI2C_SLEEP_INTERVAL)
{
	m_i2cbus = i2cbus;
	m_fd = IBI2C_OPEN_ERR;
	m_retryTimes = IBI2C_RETRY_TIMES;
	m_sleep_interval = IBI2C_SLEEP_INTERVAL;

    if(i2copen() < 0)
    {
        IBD_PRINTF(DEBUG_EMERGENCY, "IBI2CControl: Fail to open i2c device(%d).\r\n", m_i2cbus);
    }
	if(getFuncs() < 0)
	{
        IBD_PRINTF(DEBUG_WARNING, "IBI2CControl: Fail to get funcs.\r\n");
	}
    pthread_mutexattr_t     mutexattr;

	pthread_mutexattr_init(&mutexattr);
	pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init (&m_lock,&mutexattr);
}

IBI2CControl::~IBI2CControl()
{
    i2cclose();
}

int IBI2CControl::i2copen()
{
    char filename1[IB_MAX_I2C_NODE_LENGTH];
    char filename2[IB_MAX_I2C_NODE_LENGTH];
    char filename3[IB_MAX_I2C_NODE_LENGTH];
    sprintf(filename1,"/dev/i2c-%d",m_i2cbus);
    sprintf(filename2,"/dev/i2c%d",m_i2cbus);
    sprintf(filename3,"/dev/i2c/%d",m_i2cbus);
    if ((m_fd = open(filename1,O_RDWR)) < 0)
    {
        if ((m_fd = open(filename2,O_RDWR)) < 0)
        {
            if ((m_fd = open(filename3,O_RDWR)) < 0)
            {
                return IBI2C_OPEN_ERR;
            }
        }
    }
    return 0;
}

void IBI2CControl::i2cclose()
{
    if(m_fd > 0)
    {
        if(close(m_fd) < 0)
        {
			IBD_PRINTF(DEBUG_WARNING, "IBI2CControl: Fail to close i2c device.\r\n");
        }
    }
}

int IBI2CControl::checkOK()
{
	if(m_fd < 0) 
	{
        IBD_PRINTF(DEBUG_WARNING, "IBI2CControl: I2C fd is not opened.\r\n");
		return 0;
	}
	return 1;
}

int IBI2CControl::getFuncs()
{
    /* check adapter functionality */
    if (ioctl(m_fd, I2C_FUNCS, &m_funcs) < 0)
    {
        IBD_PRINTF(DEBUG_WARNING, "IBI2CControl: Fail to get i2c funcs.\r\n");
		return IBI2C_IOCTL_FUNCS_ERR;
    }
	return 0;
}

void IBI2CControl::setOption(int retryTimes, int interval)
{
	m_retryTimes = retryTimes;
	m_sleep_interval = interval;
}

int IBI2CControl::readByte(int address, unsigned char &value)
{
	InternalLock    lock(&m_lock);
    int rTimes;
    int ret = -1;

    if(m_fd < 0)
    {
        return IBI2C_OPEN_ERR;
    }

    rTimes = m_retryTimes;
    while( (ret<0) && (rTimes--) )
    {
        if((ret = ioctl(m_fd,I2C_SLAVE_FORCE,address)) < 0)
        {
			ret = IBI2C_IOCTL_SLAVE_FORCE_ERR;
            usleep( m_sleep_interval );
        }
        else
        {
            if((ret = i2c_smbus_read_byte(value)) < 0)
            {
                usleep( m_sleep_interval );
            }
        }
    }
    return ret;
}

int IBI2CControl::readBlock(int address, unsigned char value[], int length)
{
	InternalLock    lock(&m_lock);
    int ret = -1;
    struct i2c_rdwr_ioctl_data queue;
    struct i2c_msg msg;
    int rTimes;

    if(m_fd < 0)
    {
        return IBI2C_OPEN_ERR;
    }
    
    msg.addr = address;
    msg.flags = 1;
    msg.buf = value;
    msg.len = length;
    queue.msgs = &msg;
    queue.nmsgs = 1;
    rTimes = m_retryTimes;
    while( (ret<0) && (rTimes--) )
    {
        if((ret = ioctl(m_fd,I2C_RDWR,&queue)) < 0)
        {
			ret = IBI2C_IOCTL_RDWR_ERR;
            usleep( m_sleep_interval );
        }
		else
		{
			ret = msg.len;
		}
    }
    return ret;
}

int IBI2CControl::writeBlock(int address, unsigned char value[], int length)
{
	InternalLock    lock(&m_lock);
    int ret = -1;
    struct i2c_rdwr_ioctl_data queue;
    struct i2c_msg msg;
    int rTimes;

    if(m_fd < 0)
    {
        return IBI2C_OPEN_ERR;
    }

    msg.addr = address;
    msg.flags = 0;
    msg.buf = value;
    msg.len = length;
    queue.msgs = &msg;
    queue.nmsgs = 1;
    rTimes = m_retryTimes;
    while( (ret<0) && (rTimes--) )
    {
        if((ret = ioctl(m_fd,I2C_RDWR,&queue)) < 0)
        {
			ret = IBI2C_IOCTL_RDWR_ERR;
            usleep( m_sleep_interval );
        }
		else
		{
			ret = msg.len;
		}
    }
    return ret;

}

int IBI2CControl::readByte(int address,int subAddress, unsigned char &value)
{
	InternalLock    lock(&m_lock);
    int res = -1;
    int rTimes;

    if(m_fd < 0)
    {
        return IBI2C_OPEN_ERR;
    }

    if( (address<0) || (address>0x7f) )
    {
        return IBI2C_ADDR_ERR;
    }

    if( (subAddress<0) || (subAddress>0xff) )
    {
        return IBI2C_ADDR_ERR;
    }

    if(!(m_funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA))
    {
          return IBI2C_FUNCS_ERR;
    }
    
    rTimes = m_retryTimes;
    while( (res<0) && (rTimes--) )
    {
        /* use FORCE so that we can write registers even when a driver is also running */
        if ((res = ioctl(m_fd,I2C_SLAVE_FORCE,address)) < 0)
        {
			res = IBI2C_IOCTL_SLAVE_FORCE_ERR;
            usleep( m_sleep_interval );
        }
        else
        {
            res = i2c_smbus_read_byte_data(subAddress, value);
            if(res<0)
            {
                usleep( m_sleep_interval );
            }
        }
    }
    return res;
}

int IBI2CControl::writeByte(int address, int subAddress, unsigned char value)
{
	InternalLock    lock(&m_lock);
    int res = -1;
    int rTimes;
    
    if(m_fd < 0)
    {
        return IBI2C_OPEN_ERR;
    }
    
    if((address<0) || (address>0xFF))
    {
        return IBI2C_ADDR_ERR;
    }

    if((subAddress<0) || (subAddress>0xFF))
    {
        return IBI2C_ADDR_ERR;
    }
    
    if(!(m_funcs & I2C_FUNC_SMBUS_WRITE_BYTE_DATA))
    {
        return IBI2C_FUNCS_ERR;
    }

    rTimes = m_retryTimes;
    while( (res<0) && (rTimes--) )
    {
        /* use FORCE so that we can write registers even when a driver is also running */
        if ( (res = ioctl(m_fd,I2C_SLAVE_FORCE,address)) < 0) 
        {
			res = IBI2C_IOCTL_SLAVE_FORCE_ERR;
            usleep( m_sleep_interval );
        }
        else
        {
            res = i2c_smbus_write_byte_data(subAddress, value);
            if(res<0)
            {
                usleep( m_sleep_interval );
            }
        }
    }
    return res;
}

int IBI2CControl::readWord(int address,int subAddress,unsigned short &value)
{
	InternalLock    lock(&m_lock);
    int res = -1;
    int rTimes;

    if(m_fd < 0)
    {
        return IBI2C_OPEN_ERR;
    }
    
    if( (address<0) || (address>0x7f) )
    {
        return IBI2C_ADDR_ERR;
    }
    if( (subAddress<0) || (subAddress>0xff) )
    {
        return IBI2C_ADDR_ERR;
    }

        //case I2C_SMBUS_WORD_DATA:
    if(!(m_funcs & I2C_FUNC_SMBUS_READ_WORD_DATA))
    {
        return IBI2C_FUNCS_ERR;
    }
    
    rTimes = m_retryTimes;
    while( (res<0) && (rTimes--) )
    {
        /* use FORCE so that we can write registers even when a driver is also running */
        if ((res = ioctl(m_fd,I2C_SLAVE_FORCE,address)) < 0)
        {
			res = IBI2C_IOCTL_SLAVE_FORCE_ERR;
            usleep( m_sleep_interval );
        }
        else
        {
            res = i2c_smbus_read_word_data(subAddress, value);
            if(res<0)
            {
                usleep( m_sleep_interval );
            }
        }
    }
    return res;
}

int IBI2CControl::writeWord(int address, int subAddress, unsigned short value)
{
	InternalLock    lock(&m_lock);
    int res = -1;
    int rTimes;
    
    if(m_fd < 0)
    {
        return IBI2C_OPEN_ERR;
    }
    
    if((address<0) || (address>0xFF))
    {
        return IBI2C_ADDR_ERR;
    }

    if((subAddress<0) || (subAddress>0xFF))
    {
        return IBI2C_ADDR_ERR;
    }
    
    if(!(m_funcs & I2C_FUNC_SMBUS_WRITE_WORD_DATA))
    {
        return IBI2C_FUNCS_ERR;
    }
    
	rTimes = m_retryTimes;
    while( (res<0) && (rTimes--) )
    {
        /* use FORCE so that we can write registers even when a driver is also running */
        if ( (res = ioctl(m_fd,I2C_SLAVE_FORCE,address)) < 0) 
        {
			res = IBI2C_IOCTL_SLAVE_FORCE_ERR;
            usleep( m_sleep_interval );
        }
        else
        {
            res = i2c_smbus_write_word_data(subAddress, value);
            if(res<0)
            {
                usleep( m_sleep_interval );
            }
        }
    }
    return res;
}

int IBI2CControl::readBlock(int address, int subAddress, unsigned char value[], int length)
{
	InternalLock    lock(&m_lock);
    int res = -1;
    int rTimes;

    if(m_fd < 0)
    {
        return IBI2C_OPEN_ERR;
    }

    if( (address<0) || (address>0x7f) )
    {
        return IBI2C_ADDR_ERR;
    }

    if( (subAddress<0) || (subAddress>0xff) )
    {
        return IBI2C_ADDR_ERR;
    }

    if(!(m_funcs & I2C_FUNC_SMBUS_I2C_BLOCK))
    {
        return IBI2C_FUNCS_ERR;
    }
    
    rTimes = m_retryTimes;
    while( (res<0) && (rTimes--) )
    {
        /* use FORCE so that we can write registers even when a driver is also running */
        if ((res = ioctl(m_fd,I2C_SLAVE_FORCE,address)) < 0)
        {
			res = IBI2C_IOCTL_SLAVE_FORCE_ERR;
            usleep( m_sleep_interval );
        }
        else
        {
            res = i2c_smbus_read_i2c_block_data(subAddress, length, value);
            if(res<0)
            {
                usleep( m_sleep_interval );
            }
        }
    }
    return res;
}

int IBI2CControl::writeBlock(int address, int subAddress, unsigned char value[], int length)
{
	InternalLock    lock(&m_lock);
    int res = -1;
    int rTimes;
    
    if(m_fd < 0)
    {
        return IBI2C_OPEN_ERR;
    }
    
    if( (address<0) || (address>0x7f) )
    {
        return IBI2C_ADDR_ERR;
    }

    if( (subAddress<0) || (subAddress>0xff) )
    {
        return IBI2C_ADDR_ERR;
    }
    
    if(length > I2C_SMBUS_BLOCK_MAX) //pb ???
    {
        return -1;
    }
    
    if(!(m_funcs & I2C_FUNC_SMBUS_I2C_BLOCK))
    {
        return IBI2C_FUNCS_ERR;
    }

    rTimes = m_retryTimes;
    while( (res<0) && (rTimes--) )
    {
        /* use FORCE so that we can write registers even when a driver is also running */
        if ( (res = ioctl(m_fd,I2C_SLAVE_FORCE,address)) < 0) 
        {
			res = IBI2C_IOCTL_SLAVE_FORCE_ERR;
            usleep( m_sleep_interval );
        }
        else
        {
            res = i2c_smbus_write_i2c_block_data(subAddress, length, value);
            if(res<0)
            {
                usleep( m_sleep_interval );
            }
        }
    }
    return res;
}

int IBI2CControl::i2c_smbus_read_byte(unsigned char &val) 
{
    union i2c_smbus_data data;
    if (i2c_smbus_access(I2C_SMBUS_READ,0,I2C_SMBUS_BYTE,&data))
	{
        return IBI2C_IOCTL_SMBUS_ERR;
	}
    else
	{
		val = 0x0FF & data.byte;
        return 0;
	}
}

int IBI2CControl::i2c_smbus_write_byte(unsigned char value)
{
    return i2c_smbus_access(I2C_SMBUS_WRITE,value,
                            I2C_SMBUS_BYTE,NULL);
}

/* Returns the number of read bytes */
int IBI2CControl::i2c_smbus_read_i2c_block_data(unsigned char command, unsigned char length, unsigned char *values)
{
    union i2c_smbus_data data;
    int i;
    data.block[0] = length;
    if (i2c_smbus_access(I2C_SMBUS_READ,command, I2C_SMBUS_I2C_BLOCK_DATA,&data))
	{
        return IBI2C_IOCTL_SMBUS_ERR;
	}
    else
	{
        for (i = 1; i <= data.block[0]; i++)
            values[i-1] = data.block[i];
        return data.block[0];
    }
}

int IBI2CControl::i2c_smbus_write_i2c_block_data(unsigned char command, unsigned char length, unsigned char *values)
{
    union i2c_smbus_data data;
    int i;
    if (length > I2C_SMBUS_BLOCK_MAX)
        length = I2C_SMBUS_BLOCK_MAX;
    for (i = 1; i <= length; i++)
        data.block[i] = values[i-1];
    data.block[0] = length;
	if (i2c_smbus_access(I2C_SMBUS_WRITE,command, I2C_SMBUS_I2C_BLOCK_DATA, &data))
	{
		return IBI2C_IOCTL_SMBUS_ERR;
	}
	else
	{
		return data.block[0];
	}
}

int IBI2CControl::i2c_smbus_access(char read_write, unsigned char command,
                                          int size, union i2c_smbus_data *data)
{
    struct i2c_smbus_ioctl_data args;

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;
    return ioctl(m_fd,I2C_SMBUS,&args);
}

int IBI2CControl::i2c_smbus_read_byte_data(unsigned char command, unsigned char &val)
{
    union i2c_smbus_data data;
    if (i2c_smbus_access(I2C_SMBUS_READ,command, I2C_SMBUS_BYTE_DATA,&data))
	{
        return IBI2C_IOCTL_SMBUS_ERR;
	}
    else
	{
		val = 0x0FF & data.byte;
        return 0;
	}
}

int IBI2CControl::i2c_smbus_write_byte_data(unsigned char command, unsigned char value)
{
    union i2c_smbus_data data;
    data.byte = value;
    if (i2c_smbus_access(I2C_SMBUS_WRITE,command, I2C_SMBUS_BYTE_DATA, &data))
	{
		return IBI2C_IOCTL_SMBUS_ERR;
	}
	else 
	{
		return 0;
	}
}

int IBI2CControl::i2c_smbus_read_word_data(unsigned char command, unsigned short &val)
{
    union i2c_smbus_data data;
    if (i2c_smbus_access(I2C_SMBUS_READ,command, I2C_SMBUS_WORD_DATA,&data))
	{
        return IBI2C_IOCTL_SMBUS_ERR;
	}
    else
	{
		val = 0x0FFFF & data.word;
        return 0;
	}
}

int IBI2CControl::i2c_smbus_write_word_data(unsigned char command, unsigned short value)
{
    union i2c_smbus_data data;
    data.word = value;
    if (i2c_smbus_access(I2C_SMBUS_WRITE,command, I2C_SMBUS_WORD_DATA, &data))
	{
		return IBI2C_IOCTL_SMBUS_ERR;
	}
	else
	{
		return 0; 
	}
}

IBI2CControl::InternalLock::InternalLock(pthread_mutex_t* lock)
{
	m_lock=lock;
	pthread_mutex_lock(m_lock);
}
IBI2CControl::InternalLock::~InternalLock()
{
	pthread_mutex_unlock(m_lock);
}

