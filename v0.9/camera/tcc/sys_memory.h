#ifndef _SYS_MEMORY_H_
#define _SYS_MEMORY_H_


/*
#if defined(__arm__) || defined(__arm) || defined(ARM) // Linux or ADS or WinCE
	#if ( defined(__GNUC__) && !defined(__ANDROID__) ) || defined(__LINUX__)	// ARM Linux
		#define SYS_LINUX
	#elif ( defined(__GNUC__) && !defined(__LINUX__) ) || defined(__ANDROID__)	// ARM EABI
		#define SYS_ANDROID
	#elif defined(_WIN32_WCE)					// ARM WinCE
		#define SYS_WINCE
		#define ARM_ARCH_V6
		#include <windows.h>
	#else
		#define SYS_ADS							// ARM ADS
	#endif
#endif
#define SYS_LINUX	1

#define RAM_256M_USED	1

#ifdef SYS_ANDROID
#	define SYS_PHYSICAL_BASE 0x4DC00000	//36MB
#else
#	ifdef RAM_OAX_USED	//OAX version
#		define SYS_PHYSICAL_BASE 0x0
#	else				//OXX version
#		ifdef RAM_256M_USED	
#			define SYS_PHYSICAL_BASE 0x44E00000
#		else
#			define SYS_PHYSICAL_BASE 0x46200000
#		endif
#	endif
#endif

#ifdef RAM_256M_USED	
#	define SYS_PHYSICAL_MEMSIZE (46*1024*1024)
#else
#	define SYS_PHYSICAL_MEMSIZE (32*1024*1024)
#endif
*/

#define VPU_CLK_CONTROL	1
#define SYS_LINUX	1


#define BE_16(x) ((((unsigned char*)(x))[0] << 8) | ((unsigned char*)(x))[1])
extern unsigned int gRegBaseAddr;

void* sys_memcpy(void* pDst, const void* pSrc, unsigned int uiSize);
void* sys_memset(void* pPtr, int iVal, unsigned int uiOffset);
void sys_sleep( unsigned int dwMilliseconds );
int sys_interrupt( void );


void* sys_malloc_physical_addr(unsigned long uiSize);

/*
#if defined(SYS_LINUX)
void* sys_malloc_virtual_addr(int* pDev, unsigned int PAddr, unsigned int uiSize);
void sys_free_virtual_addr(int* pDev, void* virtual_ptr, unsigned int mem_size);
#elif defined(SYS_WINCE)
unsigned long sys_malloc_virtual_addr(int* pDev, unsigned int PAddr, unsigned int uiSize);
void sys_free_virtual_addr(int* pDev, unsigned long BaseAddress, unsigned int mem_size);
#endif
*/
unsigned long sys_malloc_virtual_addr(int* pDev, unsigned long PAddr, unsigned long uiSize);
void sys_free_virtual_addr(int* pDev, unsigned long BaseAddress, unsigned long mem_size);

void sys_free_physical_addr(void* pPtr,unsigned long uiSize);


#endif//_SYS_MEMORY_H_
