#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <stdio.h>
#include <sys/select.h>
#include <string.h>

#include "VDRDeployParaTable.h"

#define MAX_COMMAND_LEN  40
#define DEVICE_NAME "/dev/ttyUSB2"

static int fd;
static struct termios termios_old, termios_new;
static char command[MAX_COMMAND_LEN+2] = "AT+ZICCID\r\n";
static unsigned int command_len = 0;

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
	switch (parity) {
	case 'N':                  /* no parity check */
	    termios_new.c_cflag &= ~PARENB;
	    break;
	case 'E':                  /* even */
	    termios_new.c_cflag |= PARENB;
	    termios_new.c_cflag &= ~PARODD;
	    break;
	case 'O':                  /* odd */
	    termios_new.c_cflag |= PARENB;
	    termios_new.c_cflag |= ~PARODD;
	    break;
	default:                   /* no parity check */
	    termios_new.c_cflag &= ~PARENB;
	    break;
	}
}

static int SetPortAttr (int baudrate,int databit, const char *stopbit, char parity)
{
	bzero (&termios_new, sizeof (termios_new));
	cfmakeraw (&termios_new);
	termios_new.c_cflag = BAUDRATE (baudrate);  /* set baudrate */
	termios_new.c_cflag |= CLOCAL | CREAD;      /* | CRTSCTS */
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

static int OpenComPort (const char *dev, int baudrate, int databit,
                        const char *stopbit, char parity)
{
    int    retval;
    
    fd = open (dev, O_RDWR|O_NONBLOCK);
    if (fd == -1) 
    {
        fprintf (stderr, "Failed to open %s.\r\n", dev);
        return -1;
    }
    tcgetattr (fd, &termios_old);       /* save old termios value */
    retval = SetPortAttr (baudrate,databit, stopbit, parity);
    if (retval == -1) 
    {
        fprintf (stderr, "Failed to set port attr.\r\n");
        tcsetattr (fd, TCSANOW, &termios_old);
        close(fd);
        return -1;
    }

    return 0;
}

int main()
{
    int retval;
    static char sim_iccid[MAX_COMMAND_LEN];
    int i = 0;
    int flag = 1;
    fd_set rfds;

    if(OpenComPort(DEVICE_NAME, 115200, 8, "1", 'N') == -1)
        return 1; /* open device failed */

    while(1)
    {
        command_len = strlen(command);
        if(command_len > 0)
        {
            if(write(fd, command, command_len) < 0)
            {
                fprintf(stderr, "Failed to write to serial port.\r\n");
                fprintf(stderr, "Read SimICCID Failed!\n");
                tcsetattr (fd, TCSANOW, &termios_old);
                close(fd);
                return 2;
            }  
        }

        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 20000;
        
        retval = select(fd+1, &rfds, NULL, NULL, &tv);
        if(retval <= 0)
        {
            fprintf(stderr, "Select error...\r\n");
            fprintf(stderr, "Read SimICCID Failed!\n");
            tcsetattr (fd, TCSANOW, &termios_old);
            close(fd);
            return 2;
        }

        unsigned char ch;
        char *ptr;
        char match_str[20] = "+ZICCID: ";
        
        while((retval = read(fd, &ch, 1)) == 1)
        {
            fprintf(stderr, "%c", ch);
            sim_iccid[i++] = ch;
        }
        sim_iccid[i] = '\0';
        
        if((ptr = strstr(sim_iccid, match_str)) != NULL)
        {
            char tmp[MAX_COMMAND_LEN];
            int count = 0;
            int match_len = strlen(match_str);
            
            flag = 0;
            for(int j = match_len; ptr[j] != '\0'; j++)
            {
                if(ptr[j] >= '0' && ptr[j] <= '9')
                    continue;
                else
                {
                    count = j - match_len;
                    break;
                }
            }
            strncpy(tmp, ptr + match_len, count);
            strcpy(sim_iccid, tmp);
            sim_iccid[count] = '\0';
            fprintf(stdout, "Read SimICCID successful!\n");

            VDRDeployParaTable *table = new VDRDeployParaTable();
            table->SetStringValue(VDR_DEPLOY_PARA_VDR_SIM_ICCID, sim_iccid);
            table->Commit();
            delete table;
            break;
        } 
    }

    tcsetattr (fd, TCSANOW, &termios_old);
    close(fd);
    
    return 0;
}
