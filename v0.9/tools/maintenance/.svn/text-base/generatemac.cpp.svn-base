#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <string>
#include <math.h>

#define AT88SC_IOCTL_BASE      'C'
#define AT88SC_DATA            _IOW (AT88SC_IOCTL_BASE, 0, struct at88sc_ioctl_data)
#define AT88SC_PRODUCT_ID      _IOW (AT88SC_IOCTL_BASE, 1, struct at88sc_ioctl_data)

struct at88sc_ioctl_data {	
	unsigned char rw;
	unsigned char userzone;
	unsigned char password[13];
};

#define MAC_SEED_LEN            8
#define TUID_LEN                32
#define MAC_PREFIX              "00:0E:10"
#define MAC_CHAR_NUM            6

using namespace std;

static unsigned char passbuf[13]={'i','N','N','O','V',8,8,8,9,2,8,7,5};

int generate_by_tuid(unsigned int &mac_seq)
{
    int fd;
    struct at88sc_ioctl_data ioctldata;
    unsigned char tuid[TUID_LEN];
    int ret;

    mac_seq = 0;
    
    fd = open("/dev/at88sc", O_RDONLY);
    if(fd < 0)
    {
        return -1;
    }
    
    ioctldata.rw = 1;
    ioctldata.userzone = 1;
    memcpy(ioctldata.password, passbuf, 13);
    
    if(ioctl(fd, AT88SC_DATA, &ioctldata) < 0)
    {
        close(fd);
        return -1;
    }
    
    if(read(fd, tuid, TUID_LEN) != TUID_LEN)
    {
        close(fd);
        return -1;
    }
    
    close(fd);
    
    for(int i = 0; i < MAC_SEED_LEN; i ++)
    {
        unsigned char c = tuid[TUID_LEN -  MAC_SEED_LEN + i];
        if(c < '0')
        {
            c = 0;
        }
        else if(c > '9')
        {
            c = 9;
        }
        else
        {
            c -= '0';
        }
        mac_seq = mac_seq * 10 + c;
    }
    
    return 0;
}

int generate_by_time(unsigned int &mac_seq)
{
    long long current_time = time(NULL);
    long long div = (long long)pow(10, MAC_SEED_LEN);

    mac_seq = (unsigned int)(current_time % div);
    
    return 0;
}

unsigned char NumToChar(unsigned char input)
{
    if(input > 0x0F)
    {
        input = 0x0F;
    }
    
    if((input >= 0) && (input <= 9))
    {
        return '0' + input;
    }
    else
    {
        return 'A' + input - 10;
    }
}

int main(int argc, char **argv)
{
    unsigned int mac_seq;
    string mac = MAC_PREFIX;
    long long div = 0x1000000;
    
    if(generate_by_tuid(mac_seq) < 0)
    {
        if(generate_by_time(mac_seq) < 0)
        {
            return -1;
        }
    }
    
    for(int i = 0; i < MAC_CHAR_NUM; i ++)
    {
        unsigned char ch = mac_seq & 0x0F;
        mac_seq = mac_seq >> 4;
        if((i % 2) == 0)
        {
            mac += ":";
        }
        mac += NumToChar(ch);
    }
    
    cout << mac << endl;

    return 0;
}
