#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys_memory.h"
#include "TCC_VPU_CODEC.h" 


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <fcntl.h>         // O_RDWR
#include <sys/poll.h>

#define TCC_INTR_DEV_NAME		"/dev/tcc-intr-vc"
#define TCC_INTR_DEV_MAJOR		246
#define IOCTL_INTR_SET_SCORE	_IO(TCC_INTR_DEV_MAJOR, 1)
#define IOCTL_INTR_GET_SCORE	_IO(TCC_INTR_DEV_MAJOR, 2)


static int vpu_intr_fd = -1;
static struct pollfd tcc_event[1];

#define READ_FROM_PROC	1
#define RAM_256M_USED	1

#ifdef  READ_FROM_PROC
static unsigned int SYS_PHYSICAL_BASE = 0;
static unsigned int SYS_PHYSICAL_SIZE = 0;
#else
#ifdef RAM_256M_USED
#define SYS_PHYSICAL_BASE 0x45E00000
#else
#define SYS_PHYSICAL_BASE 0x46200000
#endif
#endif

static unsigned int gUsedMemSize = 0;


#ifdef READ_FROM_PROC
unsigned int HW_MEM_BASE = 0;
#endif

#ifdef READ_FROM_PROC
void Set_Scaler_PhyAddress(unsigned int address, unsigned int offset)
{
	HW_MEM_BASE = address + offset;
}
#endif

unsigned int get_scaler_physical_address(void)
{
#ifdef  READ_FROM_PROC
	HW_MEM_BASE = SYS_PHYSICAL_BASE + SYS_PHYSICAL_SIZE;
	return HW_MEM_BASE;
#else
	return HW_MEM_BASE;
#endif
}

void* sys_memcpy(void* pDst, const void* pSrc, unsigned int uiSize)
{
	return memcpy( pDst, pSrc, uiSize );
}

void* sys_memset(void* pPtr, int iVal, unsigned int uiOffset)
{
	return memset( pPtr, iVal, uiOffset );
}

void  sys_sleep( unsigned int dwMilliseconds )
{
	return;
}

unsigned int gRegBaseAddr;

static void write_reg(unsigned int addr, unsigned int val)
{
	*((volatile unsigned int *)(gRegBaseAddr + addr)) = (unsigned int)(val);
}

static unsigned int read_reg(unsigned int addr)
{
	return *(volatile unsigned int *)(gRegBaseAddr + addr);
}


void Uninit_Vpufd(void)
{
	close(vpu_intr_fd);
}

int
sys_interrupt(void)
{
#if 1
	while (1)
	{
		int ret;
		memset(tcc_event, 0, sizeof(tcc_event));
		tcc_event[0].fd = vpu_intr_fd;
		tcc_event[0].events = POLLIN;
		ret = poll((struct pollfd *)&tcc_event, 1, 1000); // 1 sec
		if (ret < 0) 
		{
			printf("vpu poll error\n");
			return RETCODE_CODEC_EXIT;
		}
		else if (ret == 0)
		{
			/* when vpu poll timeout occurred, we don't call vpu close function. so, close vpu interrupt here. */
			//tcc_vpu_freemem();
			printf("vpu poll timeout\n");		
			return RETCODE_CODEC_EXIT;
		}
		else if (ret > 0)
		{
			if (tcc_event[0].revents & POLLERR) 
			{
				printf("vpu poll POLLERR\n");
				return RETCODE_CODEC_EXIT;
			} 
			else if (tcc_event[0].revents & POLLIN) 
			{
				return RETCODE_SUCCESS;
			}
		}
	}
#endif	
	/* todo */
	//write_reg(0x174, 0);
	//write_reg(0x00C, 1);	
}


unsigned long sys_malloc_virtual_addr(int* pDev, unsigned long PAddr, unsigned long uiSize)
{
	void *mem_ptr = MAP_FAILED;

	*pDev = open("/dev/mem", O_RDWR | O_NDELAY);
	if (*pDev == -1)
	{
		printf("mem dirver open fail\n");
		return NULL;
	}

	return (unsigned long )mmap(0, uiSize, PROT_READ | PROT_WRITE, MAP_SHARED, *pDev, PAddr);
}
void sys_free_virtual_addr(int* pDev, unsigned long virtual_ptr, unsigned long mem_size)
{
	close(*pDev);
	munmap(virtual_ptr, mem_size);
}


void sys_init_physical_size(void)
{

#ifdef  READ_FROM_PROC	
	FILE *fp;


	vpu_intr_fd = open(TCC_INTR_DEV_NAME, O_RDWR);
	if (vpu_intr_fd < 0)
	{
		printf("vpu fd open error\n");
		return -1;
	}
	
	fp = fopen("/proc/tcc/vcodec_addr","r");
	if(fp==NULL)
	{
		printf("can not open VIDEO ADDRESS\n");
		printf("can not open VIDEO ADDRESS\n");
		printf("can not open VIDEO ADDRESS\n");		
	}
	fscanf(fp, "0x%x", &SYS_PHYSICAL_BASE);
	printf("VPU ADDRESS : 0x%x\n", SYS_PHYSICAL_BASE);
	printf("VPU ADDRESS : 0x%x\n", SYS_PHYSICAL_BASE);
	printf("VPU ADDRESS : 0x%x\n", SYS_PHYSICAL_BASE);	
	fclose(fp);

	fp = fopen("/proc/tcc/vcodec_size","r");
	if(fp==NULL)
	{
		printf("can not open VIDEO SIZE\n");
		printf("can not open VIDEO SIZE\n");
		printf("can not open VIDEO SIZE\n");		
	}
	fscanf(fp, "%d", &SYS_PHYSICAL_SIZE);	
	SYS_PHYSICAL_SIZE -= 4;
	SYS_PHYSICAL_SIZE *= 1024*1024;
	printf("VPU SIZE : %d\n", SYS_PHYSICAL_SIZE);
	printf("VPU SIZE : %d\n", SYS_PHYSICAL_SIZE);
	printf("VPU SIZE : %d\n", SYS_PHYSICAL_SIZE);	
	fclose(fp);	

	Set_Scaler_PhyAddress(SYS_PHYSICAL_BASE, SYS_PHYSICAL_SIZE);
#endif
#ifdef TCC_CAMERA_INCLUDE
	gUsedMemSize = CMERA_BUFFER_OFFSET;
#else
	gUsedMemSize = 0;
#endif
}


void* 
sys_malloc_physical_addr(unsigned long uiSize)
{

	unsigned int phsical_addr = SYS_PHYSICAL_BASE + gUsedMemSize;
	//printf(">>>> Malloc:Prev Used Memory:%d ",gUsedMemSize);
	gUsedMemSize += uiSize;
	//printf(">>>> sys_malloc_physical_addr size %d bytes \n", gUsedMemSize);

	if( gUsedMemSize > SYS_PHYSICAL_SIZE )
	{
		//printf("sys_malloc_physical_addr error! %d bytes \n", gUsedMemSize);
		exit(-1);
	}
	else
	{
		//printf("ulSize:%d Current Used Memory:%d \n",uiSize,gUsedMemSize);
	}
	return (void*)( phsical_addr );

}

void
sys_free_physical_addr(void* pPtr,unsigned long uiSize)
{
	gUsedMemSize -= uiSize;
	return;
}

void TCC_VPU_HW_Reset(void)
{
	int membase;
	unsigned int mask_SWRESET, mask_SOFTRESET;

	typedef struct _tVPU_SWRESET{
	           unsigned int VPU_SWRESET[18];
	}tVPU_SWRESET;

	typedef struct _tVPU_SOFTRESET{
	           unsigned int VPU_SOFTRESET[2];
	}tVPU_SOFTRESET;         

	volatile tVPU_SWRESET* regAddr_SWRESET= (volatile tVPU_SWRESET*)sys_malloc_virtual_addr( &membase, 0xF0400000, sizeof(tVPU_SWRESET));  
	volatile tVPU_SOFTRESET* regAddr_SOFTRESET= (volatile tVPU_SOFTRESET*)sys_malloc_virtual_addr( &membase, 0xF0702000, sizeof(tVPU_SOFTRESET));            

//	vpu_clock_deinit();	
	mask_SWRESET      = (1<<6);//SWRESET_REG_VPU_MASK;
	mask_SOFTRESET    = (1<<2);//SOFTRESET_REG_VPU_MASK;

	(regAddr_SWRESET->VPU_SWRESET[17])|= mask_SWRESET;
	(regAddr_SOFTRESET->VPU_SOFTRESET[1]) |= mask_SOFTRESET;

	usleep(1000);

	(regAddr_SWRESET->VPU_SWRESET[17]) &= (~mask_SWRESET);
	(regAddr_SOFTRESET->VPU_SOFTRESET[1])  &= (~mask_SOFTRESET);

//	vpu_clock_init();
	usleep(10*1000);

	//restart VPU process..
	sys_free_virtual_addr( &membase, (void*)regAddr_SWRESET, sizeof(tVPU_SWRESET));
	sys_free_virtual_addr( &membase, (void*)regAddr_SOFTRESET, sizeof(tVPU_SOFTRESET));	
}

