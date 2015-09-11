#include "serialbase.h"

static struct termios termios_old, termios_new;
static struct timeval tv_timeout;
static fd_set   fs_read, fs_write;

static int BAUDRATE (int baudrate);
static int _BAUDRATE (int baudrate);
static void SetDataBit (int databit);
static void SetStopBit (const char *stopbit);
static void SetParityCheck (char parity);
static int SetPortAttr (int fd,int baudrate,int databit, const char *stopbit, char parity);
static int GetBaudrate ();

static int BAUDRATE (int baudrate)
{
	switch (baudrate) {
	case 0:
	    return (B0);
	case 50:
	    return (B50);
	case 75:
	    return (B75);
	case 110:
	    return (B110);
	case 134:
	    return (B134);
	case 150:
	    return (B150);
	case 200:
	    return (B200);
	case 300:
	    return (B300);
	case 600:
	    return (B600);
	case 1200:
	    return (B1200);
	case 2400:
	    return (B2400);
	case 9600:
	    return (B9600);
	case 19200:
	    return (B19200);
	case 38400:
	    return (B38400);
	case 57600:
	    return (B57600);
	case 115200:
	    return (B115200);
	default:
	    return (B9600);
	}
}

static int _BAUDRATE (int baudrate)
{
/* reverse baudrate */
	switch (baudrate) {
	case B0:
	    return (0);
	case B50:
	    return (50);
	case B75:
	    return (75);
	case B110:
	    return (110);
	case B134:
	    return (134);
	case B150:
	    return (150);
	case B200:
	    return (200);
	case B300:
	    return (300);
	case B600:
	    return (600);
	case B1200:
	    return (1200);
	case B2400:
	    return (2400);
	case B9600:
	    return (9600);
	case B19200:
	    return (19200);
	case B38400:
	    return (38400);
	case B57600:
	    return (57600);
	case B115200:
	    return (115200);
	default:
	    return (9600);
	}
}

static int GetBaudrate()
{
    return (_BAUDRATE (cfgetospeed (&termios_new)));
}

static void SetDataBit (int databit)
{
	termios_new.c_cflag &= ~CSIZE;
	switch (databit) {
	case 8:
	    termios_new.c_cflag |= CS8;
	    break;
	case 7:
	    termios_new.c_cflag |= CS7;
	    break;
	case 6:
	    termios_new.c_cflag |= CS6;
	    break;
	case 5:
	    termios_new.c_cflag |= CS5;
	    break;
	default:
	    termios_new.c_cflag |= CS8;
	    break;
	}
}

static void SetStopBit (const char *stopbit)
{
	if (0 == strcmp (stopbit, "1")) {
		termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */
	}
	else if (0 == strcmp (stopbit, "1.5")) {
		termios_new.c_cflag &= ~CSTOPB; /* 1.5 stop bits */
	}
	else if (0 == strcmp (stopbit, "2")) {
		termios_new.c_cflag |= CSTOPB;  /* 2 stop bits */
	}
	else {
		termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */
	}
}

static void SetParityCheck (char parity)
{
	switch (parity)
	{
		case 'N':                  /* no parity check */
			termios_new.c_cflag &= ~PARENB;
			break;
		case 'E':                  /* even */
			termios_new.c_cflag |= PARENB;
			termios_new.c_cflag &= ~PARODD;
			termios_new.c_iflag |= INPCK;		/* Disnable parity checking */
			break;
		case 'O':                  /* odd */
			termios_new.c_cflag |= PARENB;
			termios_new.c_cflag |= PARODD;
			termios_new.c_iflag |= INPCK;		/* Disnable parity checking */
			break;
		default:                   /* no parity check */
			termios_new.c_cflag &= ~PARENB;
			break;
	}
}

static int SetPortAttr (int fd,int baudrate,int databit, const char *stopbit, char parity)
{
	bzero (&termios_new, sizeof (termios_new));
	cfmakeraw (&termios_new);
	termios_new.c_cflag = BAUDRATE (baudrate);  /* set baudrate */
	//termios_new.c_cflag |= CLOCAL | CREAD;      /* | CRTSCTS */
	//termios_new.c_cflag |= CLOCAL ;  //wll
	SetDataBit (databit);
	SetParityCheck (parity);
	SetStopBit (stopbit);
	termios_new.c_oflag = 0;
	termios_new.c_lflag |= 0;
	termios_new.c_oflag &= ~OPOST;
	termios_new.c_cc[VTIME] = 1;        /* unit: 1/10 second. */
	termios_new.c_cc[VMIN] = 1; /* minimal characters for reading */
	tcflush (fd, TCIFLUSH);
	return (tcsetattr (fd, TCSANOW, &termios_new));
}

/* Open serial port ComPort at baudrate baud rate. */
int OpenComPort (int ComPort, int baudrate, int databit,const char *stopbit, char parity)
{
	const char   *pComPort;
	int      retval;
	int        fd;

	switch (ComPort)
	{
	case 0:
   		pComPort = "/dev/tcc-uart0";
		break;
	case 1:
    	pComPort = "/dev/tcc-uart1";
		break;
	case 2:
		pComPort = "/dev/tcc-uart2";
		break;
	case 3:
		pComPort = "/dev/tcc-uart3";
		break;
    case 4:
        pComPort = "/dev/tcc-data";
        break;
	default:
    	pComPort = "/dev/tcc-uart0";
		break;
	}
	
	fd = open (pComPort, O_RDWR/*|O_NONBLOCK*/);
	if (fd == -1) 
	{
		fprintf (stderr, "cannot open port %s\n", pComPort);
		return (-1);
	}
	tcgetattr (fd, &termios_old);       /* save old termios value */
	retval = SetPortAttr (fd,baudrate,databit, stopbit, parity);
	if (retval == -1) 
	{
		fprintf (stderr, "\nport %s cannot set baudrate at %d\n", pComPort,
		baudrate);
		return -1;
	}
	return fd;
}

void CloseComPort (int fd)
{
	/* flush output data before close and restore old attribute */
	tcsetattr (fd, TCSADRAIN, &termios_old);
	close (fd);
}

int WriteComPort (int fd, unsigned char *data, int datalength)
{
	int   retval, len = 0, total_len = 0;
	int i;
	
	FD_ZERO (&fs_write);
	FD_SET (fd, &fs_write);
	tv_timeout.tv_sec = TIMEOUT_SEC (datalength, GetBaudrate ());
	tv_timeout.tv_usec = TIMEOUT_USEC;

	for (total_len = 0, len = 0; total_len < datalength;) 
	{
		retval = select (fd + 1, NULL, &fs_write, NULL, &tv_timeout);
		if (retval) 
		{
			len = write (fd, data+total_len, datalength - total_len);
			if (len > 0) {
				total_len += len;
			}
        }
		else if(retval == 0)
		{
			printf("WriteComPort  timeout \n");
			break ;
		}
		else {
			printf("select return error %d\n",retval);
			//tcflush (fd, TCOFLUSH);     /* flush all output data */
			break;
		}
	}
	tcdrain(fd);
	//usleep(500);
	
#if 0
	printf("write: ");
	for(i=0;i<total_len; i++)
		printf("0x%02x  ",data[i]);
	printf("(total_len = %d)\r\n",total_len);	
#endif
	
	if(total_len != datalength)
	{
		printf("WriteComPort error \n");
		return -1;
	}
	else
		return 0;
 }
int  ReadComPort (int fd, unsigned char  *data, int  datalength)
{
	int ret=1;
	int len = 0;

//	FD_ZERO (&fs_read);
//	FD_SET (fd, &fs_read);
//
//	ret = select (fd + 1, &fs_read, NULL, NULL, NULL); 

	if (ret) 
	{
		len = read (fd, data, datalength);
	#if 0
		printf("ReadComPort  len=%d \n",len);
		for(int t=0; t<len;t++)
		{
			printf("0x%02x ",data[t]);
		}
		printf("\n");
	#endif
		return len;
	}
	else
	{
		printf("selct() call failed : %s \n",strerror(errno));
		return -1;
	}

}


//end COM

