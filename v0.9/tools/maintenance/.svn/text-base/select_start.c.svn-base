#include <stdio.h>  
#include <linux/types.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/ioctl.h>  
#include <errno.h>  
#include <assert.h>  
#include <string.h>  
#include <linux/i2c.h>  
#include <linux/i2c-dev.h>  

#define DEV_NAME	"/dev/i2c-0"
#define MCU_ADDR 	0x54
#define SLAVE_ADDR	0x06

int i2c_read_reg(char *dev, unsigned char *buf, unsigned slave_address, unsigned reg_address, int len)  
{  
    struct i2c_rdwr_ioctl_data work_queue;  
    unsigned char w_val = reg_address;  
    int ret;  
  
    int fd = open(dev, O_RDWR);  
    if (!fd) {  
        printf("Error on opening the device file\n");  
        return 0;  
    }  
  
    work_queue.nmsgs = 2;  
    work_queue.msgs = (struct i2c_msg*)malloc(work_queue.nmsgs *sizeof(struct  
            i2c_msg));  
    if (!work_queue.msgs) {  
        printf("Memory alloc error\n");  
        close(fd);  
        return 0;  
    }  
  
    ioctl(fd, I2C_TIMEOUT, 2);  
    ioctl(fd, I2C_RETRIES, 1);  
  
    (work_queue.msgs[0]).len = 1;  
    (work_queue.msgs[0]).addr = slave_address;  
    (work_queue.msgs[0]).buf = &w_val;  
  
    (work_queue.msgs[1]).len = len;  
    (work_queue.msgs[1]).flags = I2C_M_RD;  
    (work_queue.msgs[1]).addr = slave_address;  
    (work_queue.msgs[1]).buf = buf;  
  
    ret = ioctl(fd, I2C_RDWR, (unsigned long) &work_queue);  
    if (ret < 0) {  
        //printf("Error during I2C_RDWR ioctl with error code: %d\n", ret);  
        close(fd);  
        free(work_queue.msgs);  
        return 0;  
    } else {  
        //printf("read salve:%02x reg:%02x\n", slave_address, reg_address);  
        close(fd);  
        free(work_queue.msgs);  
        return len;  
    }  
}  

int main(int argc, char **argv)
{
    unsigned int fd;  
    unsigned char rw_val;  

  
    fd = open(DEV_NAME, O_RDWR);  
    if (!fd) {  
        printf("Error on opening the device file %s\n", DEV_NAME);  
        return 0;  
    }  
  
  	i2c_read_reg(DEV_NAME, &rw_val, MCU_ADDR, SLAVE_ADDR, 1);  
    printf("%x\n", rw_val);
}
