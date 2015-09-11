#ifndef I2CTL_H
#define I2CTL_H
#include <linux/types.h>
int i2copen(int i2cbus);
int i2cset(int file,int address,__u8 *values,__u8 length);
int i2cset(int file,int address,int daddress,int value,char b_w);
int i2cget(int file,int address,__u8 *values,__u8 length);
int i2cget(int file,int address,int daddress,char b_w);
int i2cget(int file,int address);
void i2cclose(int file);
#endif

