#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#define DEV_MMCBLK0				"/dev/mmcblk0"
#define MMC_DEV_MAJOR			179
#define IOCTL_MMC_FW_WRITE		_IO(MMC_DEV_MAJOR, 1)
#define IOCTL_MMC_READ_STATUS		_IO(MMC_DEV_MAJOR, 2)
int main()
{
	int dev_fd, status, ret=0;

	dev_fd = open(DEV_MMCBLK0, O_RDWR|O_NDELAY);
	if (dev_fd == -1) {
		printf("[%s] cannot open %s.\n", DEV_MMCBLK0);
	}

	ret = ioctl(dev_fd, IOCTL_MMC_READ_STATUS, &status);
	if(ret)
		printf("IOCTL_MMC_READ_STATUS: fail\n");

	if(status & (1 << 19))
	{
		printf("########MMC Write protect status: %08x###########\n", status);
        ret = -1;
    }
    else
    {
        printf("------emmc status ok status=%08x--------\n",status);
        ret = 0;
    }
    
    close(dev_fd);
    return ret;

}

