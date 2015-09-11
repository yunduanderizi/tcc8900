#ifndef _TOOL_H_
#define  _TOOL_H_

#include <sqlite3.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <termios.h>
#include <vector>
#include <string>

using namespace std;
#ifdef __GNUC__
#define likely(x) __buildin_expert ((x) , 1)
#define unlikely(x) __buildin_expert ((x) , 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#define __DEBUG__
#define INIT() 		(program_name = strrchr(argv[0],'/')) ? \
					program_name++ : \
					(program_name=argv[0])
#define EXIT(err)		exit(err)

#ifdef __DEBUG__
static void DEBUGMSGOUT(const char* pszFormat, ...)
{   
	char buf[256]={0};
	va_list arglist;
	va_start(arglist, pszFormat);
	vsprintf(&buf[strlen(buf)], pszFormat, arglist);
	va_end(arglist);
	//是否写入文件 有单独的类
	//OutputDebugStringA(buf);
	printf("%s", buf);
}
#define DEBUGMSG DEBUGMSGOUT("[%s %d %s] ", __FILE__, __LINE__, __FUNCTION__); DEBUGMSGOUT
#else
#define DEBUGMSG
#endif
#define alloc_assert(x) \
	do{\
		if(unlikely (!x))\
			sprintf(stderr, "FATAL ERROR: OUT OF MEMORY (%s:%d)\n", __FILE__, __LINE__);\
			abort();\ 
	}while(false)
void error(int status, int err, char *fmt, ...);


#define PI (4.0 * atan(1.0))

std::string format(const char * format, ...);
std::vector<std::string> split_string(std::string in, std::string split);
double get_ts();

#endif
