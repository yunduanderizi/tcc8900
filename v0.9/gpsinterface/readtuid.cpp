#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "gpsinterface.h"

#define AT88SC_IOCTL_BASE      'C'
#define AT88SC_DATA            _IOW (AT88SC_IOCTL_BASE, 0, struct at88sc_ioctl_data)
#define AT88SC_PRODUCT_ID      _IOW (AT88SC_IOCTL_BASE, 1, struct at88sc_ioctl_data)

struct at88sc_ioctl_data {	
	unsigned char rw;
	unsigned char userzone;
	unsigned char password[13];
};

using namespace std;

#define MODEL_TYPE_LEN          20
#define MODEL_ID_LEN            7

int  read_tuid_from_at88sc(unsigned char *tuid)
{
    unsigned char passbuf[13]={'i','N','N','O','V',8,8,8,9,2,8,7,5};
    struct at88sc_ioctl_data ioctldata;
    int fd;
    
    printf("read tuid from at88sc ...\n");
    fd = open("/dev/at88sc", O_RDONLY);
    if(fd < 0)
    {
        cerr << "Failed to open tuid device." << endl;
        return -1;
    }
    
    ioctldata.rw = 1;
    ioctldata.userzone = 1;
    memcpy(ioctldata.password, passbuf, 13);
    
    if(ioctl(fd, AT88SC_DATA, &ioctldata) < 0)
    {
        cerr << "Failed to control tuid device." << endl;
        close(fd);
        return -1;
    }
    
    if(read(fd, tuid, 32) != 32)
    {
        cerr << "Failed to read device." << endl;
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

int read_tuid_from_emmc(char * tuid)
{
	FILE* fp = NULL; 
    int ret;
    
    fp = fopen("/nand2/tuid2.conf", "r");
    if(NULL == fp)
    {
        cerr << "Failed to open nor flash tuid2.conf ." << endl;
        
        fp = fopen("/nand2/tuid.conf", "r");
        if(NULL == fp)
        {
            cerr << "Failed to open nor flash tuid.conf ." << endl;
            return -1;
        }
    }

    printf("read tuid from nor flash ...\n");
    fgets(tuid, 33, fp);
    fclose(fp);
    return 0;
}

int InnovReadTuid(vector<unsigned char> &tuid)
{
    unsigned char id[33];
    int ret= 0;
    
    memset(id, 0, 33);
    ret = read_tuid_from_emmc((char *)id);
    if(ret < 0)
    {
        ret = read_tuid_from_at88sc(id);
        if(ret < 0)
        {
            cout <<"read tuid from emmc at88sc error.\n" <<endl;
            return -1;
        }
    }
    
    tuid.clear();
    
    tuid.insert(tuid.end(), (unsigned char *)id, id + 32);
    
    return 0;
}
