#include "io.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define SI4740_I2C_ADDR  0x60

static int g_io_control_fd=-1;
int io2w_init(void)
{

    if(g_io_control_fd>=0)
        return -1;
    char* filename="/dev/i2c-0";

    g_io_control_fd=open(filename,O_RDWR);
    if(g_io_control_fd<0){
        printf("----open %s node failed!----\n",filename);
        g_io_control_fd=-1;
        return -1;
    }
    return 0;
}
int io2w_write(u8 number_bytes, u8 *data_out)
{
	int ret=-1;
	struct i2c_rdwr_ioctl_data queue;
	struct i2c_msg msg;
	int rTimes=2;

	if(g_io_control_fd<0)
		return -1;

	msg.addr=SI4740_I2C_ADDR;
	msg.flags=0;
	msg.buf=data_out;
	msg.len=number_bytes;
	
	queue.msgs=&msg;
	queue.nmsgs=1;

	
	while( (ret<0)&&(rTimes--) )
	{
		if( (ret=ioctl(g_io_control_fd,I2C_RDWR,&queue))<0 )
		{
			ret=-1;
			usleep(50);
		}else{
			ret=msg.len;
		}
	}

	return ret;
}
int io2w_read(u8  number_bytes, u8 *data_in)
{
	int ret= -1;
	struct i2c_rdwr_ioctl_data queue;
	struct i2c_msg msg;
	int rTimes=2;

	if(g_io_control_fd<0)
		return -1;

	msg.addr=SI4740_I2C_ADDR;
	msg.flags= 1;
	msg.buf=data_in;
	msg.len=number_bytes;
	
	queue.msgs=&msg;
	queue.nmsgs=1;

	
	while( (ret<0)&&(rTimes--) )
	{
		if( (ret=ioctl(g_io_control_fd,I2C_RDWR,&queue))<0 )
		{
			ret=-1;
			usleep(50);
		}else{
			ret=msg.len;
		}
	}

	return ret;

}

