/* File: client.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int fd;

void sendudp_open()
{
	fd = socket(AF_INET, SOCK_DGRAM, 0);
}

void sendudp(int port, char *buf,int len)
{
	struct sockaddr_in address;

	memset(&address, 0,sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(port);

    #if 0
    int i=0;
    printf("sendudp port=%d:",port);
    for(i=0; i< len; i++)
    {
        printf("%02x ", buf[i]);
    }
    printf("\n");
    #endif
	sendto(fd, buf, len, 0, (struct sockaddr *)&address, sizeof(address));
}


void sendudp_close()
{
	close(fd);
}

