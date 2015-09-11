/******************************************************************************
* include 
******************************************************************************/
#include <sys/mman.h>
#include <fcntl.h>         // O_RDWR
#include "Virtual.h"

#include	"TCCUtil.h"

int tcc_phyaddr;
int tcc_virladdr;
//int tcc_addr_offset;
int planeoffset1;
int planeoffset2;

/******************************************************************************
*	FUNCTIONS			: Init_Virtual
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int  Init_Virtual(Virtual_t * virtual, char * device_name)
{

	memset(virtual->device_name,0,TCC_VIRTUAL_DEVICE_NAME_SIZE);
	memcpy(virtual->device_name,device_name,sizeof(device_name));
	virtual->fd= open(device_name, O_RDWR);

	if (virtual->fd<=0)
	{
		memset(virtual->device_name,0,TCC_VIRTUAL_DEVICE_NAME_SIZE);
		printf("Could not open %s\n",device_name);
		return  FALSE;
	}

	return TRUE;
	
}

/******************************************************************************
*	FUNCTIONS			: Init_Virtual
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int DeInit_Virtual(Virtual_t * virtual)
{

	if (virtual->fd > 0)
	{
	    close(virtual->fd);	
            memset(virtual->device_name,0,TCC_VIRTUAL_DEVICE_NAME_SIZE);
	}

	return TRUE;
	
}



/******************************************************************************
*	FUNCTIONS			: SetVirtual
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
void *SetVirtual(Virtual_t * virtual,unsigned int physical_addr, unsigned int mem_size)
{
	if( virtual->fd )
	{
		int mem = (int)mmap(0,mem_size,PROT_READ|PROT_WRITE,MAP_SHARED, virtual->fd, physical_addr);   //lcd_base_addr
		if(mem<0)
		{
			printf("MmMapIoSpace GetLastError:\n");
			return NULL;
		}
		return (void *)mem;
	}
	else
	{
		printf("MmMapIoSpace GetLastError:\n");
		return NULL;
	}

}
/******************************************************************************
*	FUNCTIONS			: FreeVirtual()
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

int FreeVirtual(Virtual_t * virtual,void *virtual_ptr, unsigned int mem_size)
{
	if( virtual->fd )
	{
		if(virtual_ptr) 
		{
			munmap(virtual_ptr, mem_size);
			return (TRUE);
		}	
			
	}
	return (FALSE);
}

void Init_TccBuffer(int phyaddr,int virtaddr)
{
	tcc_phyaddr=phyaddr;
	tcc_virladdr=virtaddr;
//	tcc_addr_offset=0;

}

int tcc_malloc(int size)
{
//	int offset,addr;
//	offset=tcc_addr_offset;
//	tcc_addr_offset+=size;
//	addr=tcc_virladdr+offset;
	SetYuvMode(1);
	return tcc_virladdr;
}

void SetTccBufferOffset(int addr0,int addr1,int addr2)
{
	planeoffset1=addr1-addr0;
	planeoffset2=addr2-addr0;
	SetYuvMode(1);
	printf("addr0:0x%x,0x%x,0x%x,offset:0x%x,0x%x\n",addr0,addr1,addr2,planeoffset1,planeoffset2);

}

int ReturnTccPhyAddr(int index)
{
	if(index==0)
		return tcc_phyaddr;
	else if(index==1)
		return (tcc_phyaddr+planeoffset1);
	else	
		return (tcc_phyaddr+planeoffset2);

}

