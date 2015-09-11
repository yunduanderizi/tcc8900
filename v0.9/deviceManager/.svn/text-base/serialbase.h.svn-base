#ifndef _SERIAL_H_
#define _SERIAL_H_
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>    
#include <unistd.h>
#include <string.h>

#define NO_ACK				0
#define ACK_1				1
#define ACK_3				2

#define TIMEOUT_SEC(buflen,baud) (buflen*20/baud+2)
#define TIMEOUT_USEC 0

int OpenComPort (int ComPort, int baudrate, int databit,const char *stopbit, char parity);
void CloseComPort (int fd);
int WriteComPort (int fd, unsigned char *data, int datalength);
int  ReadComPort (int fd, unsigned char  *data, int  datalength);


#endif