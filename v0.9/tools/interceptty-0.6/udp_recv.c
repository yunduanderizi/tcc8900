/* File: server.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_SIZE 1024

int main(int argc, char **argv)
{
	int fd;
	int address_len;
	struct sockaddr_in address;
	int ret;
	char buf[65536];
	int n=0, recvd = 0;
	struct sockaddr_in client_address;
	int what;

	int port = atoi(argv[1]);
	printf("recv @%d\n",port);

	socklen_t len = sizeof(client_address);

	
	fd = socket(AF_INET, SOCK_DGRAM, 0);	//SOCK_DGRAM

	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	address_len = sizeof(address);
	bind(fd, (struct sockaddr *)&address, address_len);

    if(strcmp(argv[1], "1011") == 0 || strcmp(argv[1], "1010") == 0)
    {
        int what;
        do{		
            ret = recvfrom(fd, &what, 4, 0, (struct sockaddr *)&client_address, &len);      
            printf("===recv====%d====\n",what);   
        }while(ret>0);
    }
    else if(strcmp(argv[1], "1012") == 0)
    {
        char buffer[MAX_SIZE];
        ret = 0;
        do{
            memset(buffer, 0, MAX_SIZE);
        	ret = recvfrom(fd, buffer, MAX_SIZE - 1, 0, (struct sockaddr *)&client_address, &len);
            #if 1
            int i=0;
            printf("recv len=%d:",ret);
            for(i=0; i< ret; i++)
            {
                printf("%02x ", buffer[i]);
            }
            printf("\n");
            #endif
        	//printf("===recv===%s====\n", buffer);
            //usleep(2000);

        }while(ret>0);
    }
    else if(strcmp(argv[1], "1013") == 0)
    {
        int gps_buf[15];
        ret = 0;
        do{
            memset(gps_buf, 0, 15);
        	ret = recvfrom(fd, gps_buf, 15*4, 0, (struct sockaddr *)&client_address, &len);
            printf("=============position=%d, flon=%d, flat=%d, %d, %d, iOrientation=%d, vel=%d, s=%d, w=%d, alt=%d, mode=%d, gps=%d, bd=%d\n",
                gps_buf[0], gps_buf[1],gps_buf[2],gps_buf[3], gps_buf[4],gps_buf[5], gps_buf[6],gps_buf[7], gps_buf[8],gps_buf[9], gps_buf[10],
                gps_buf[11], gps_buf[12]);
            //usleep(2000);

        }while(ret>0);
    }    
	close(fd);

    return 0;
}


